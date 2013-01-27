//
//  URLRequest.mm
//  RestToolbox
//
//  Created by Matt Jarjoura on 1/24/13.
//  Copyright (c) 2013 Artcator Inc. All rights reserved.
//

#include "URLRequest.h"
#include <Availability.h>
#include <json/json.h>

double const kDefaultRequestTimeout = 45.0;

#pragma GCC visibility push(hidden)
typedef void (^__URLRequestCompletion)(NSHTTPURLResponse *response, NSData *data, NSError *error);

@interface __URLRequestOperation : NSOperation <NSURLConnectionDataDelegate, NSURLConnectionDelegate>
@property (nonatomic, copy) __URLRequestCompletion completion;

@end

@implementation __URLRequestOperation {
    @private
    NSURLConnection *_connection;
    NSMutableData *_data;
    
    NSURLRequest *_request;
    NSURLResponse *_response;
    BOOL _executing;
    BOOL _finished;
    
    
    NSPort *_port;
    NSRunLoop *_runloop;
#if __IPHONE_OS_VERSION_MIN_REQUIRED
    NSPort *_port;
    NSRunLoop *_runloop;
#endif
}

- (id)initWithURLRequest:(NSURLRequest *)urlRequest inQueue:(NSOperationQueue *)queue withCompletion:(__URLRequestCompletion)completion
{
    self = [super init];
    if (self == nil)
        return nil;
    
    self.completion = completion;
    _request = [urlRequest retain];
    _connection = [[NSURLConnection alloc] initWithRequest:_request delegate:self startImmediately:NO];
    
    [queue addOperation:self];
    
    return self;
}

- (void)dealloc
{
#if __IPHONE_OS_VERSION_MIN_REQUIRED
    [_port release];
    [_runloop release];
    _runloop = nil;
#endif
    
    [_port release];
    [_runloop release];
    _runloop = nil;
    
    [_data release];
    _data = nil;
    
    self.completion = nil;
    [_request release];
    [_connection release];
    [super dealloc];
}

- (void)start
{
    if (self.isCancelled)
        return;
    
    [self willChangeValueForKey:@"isExecuting"];
    _executing = YES;
    [self didChangeValueForKey:@"isExecuting"];
    
    _data = [[NSMutableData alloc] init];

    _port = [[NSPort alloc] init];
    _runloop = [[NSRunLoop currentRunLoop] retain];
    [_runloop addPort:_port forMode:NSRunLoopCommonModes];
    [_connection scheduleInRunLoop:_runloop forMode:NSRunLoopCommonModes];
    [_connection start];
    [_runloop run];
    
#if __IPHONE_OS_VERSION_MIN_REQUIRED
    // iOS 5 bug forces NSURLConnection to only work in a runloop, so schedule it in one
    if ([UICollectionView class] == nil)
    {
        _port = [[NSPort alloc] init];
        _runloop = [[NSRunLoop currentRunLoop] retain];
        [_runloop addPort:_port forMode:NSRunLoopCommonModes];
        [_connection scheduleInRunLoop:_runloop forMode:NSRunLoopCommonModes];
        [_connection start];
        [_runloop run];
    }
    else
    {
        [_connection start];
    }
#else
   // [_connection start];
#endif
}

- (BOOL)isConcurrent
{
    return YES;
}

- (void)setIsFinished:(BOOL)value
{
    if (_finished != value)
    {
#if __IPHONE_OS_VERSION_MIN_REQUIRED
        if (value == YES)
        {
            // iOS 5 bug forces NSURLConnection to only work in a runloop, so unschedule the port when finished
            if ([UICollectionView class] == nil)
                [_runloop removePort:_port forMode:NSRunLoopCommonModes];
        }
#endif
        [self willChangeValueForKey:@"isFinished"];
        _finished = value;
        [self didChangeValueForKey:@"isFinished"];
    }
}

- (BOOL)isFinished
{
    return _finished;
}

- (BOOL)isExecuting
{
    return _executing;
}

#pragma mark - NSURLConnectionDelegate

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
{
    self.isFinished = YES;
    [_connection cancel];
    
    if (self.completion)
        self.completion((NSHTTPURLResponse *)_response, nil, error);
}

- (BOOL)connectionShouldUseCredentialStorage:(NSURLConnection *)connection
{
    return NO;
}

#pragma mark - NSURLConnectionDataDelegate

- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response
{
    NSHTTPURLResponse *properResponse = (NSHTTPURLResponse *)response;
    _response = [response retain];
    
    if ([properResponse statusCode] < 200 || [properResponse statusCode] >= 300)
    {
        self.isFinished = YES;
        [_connection cancel];
        
        if (self.completion)
            self.completion(properResponse, nil, [NSError errorWithDomain:@"com.foodspotting" code:0 userInfo:nil]);
    }
}

- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
{
    [_data appendData:data];
}

- (void)connectionDidFinishLoading:(NSURLConnection *)connection
{
    if (self.completion)
    {
        self.completion((NSHTTPURLResponse *)_response, _data, nil);
    }
    self.isFinished = YES;
}

- (NSCachedURLResponse *)connection:(NSURLConnection *)connection willCacheResponse:(NSCachedURLResponse *)cachedResponse
{
    return nil;
}

@end
#pragma GCC visibility pop

#pragma mark -

using namespace RestToolbox::Models;

URLRequest::URLRequest(const BasicUri & uri, const std::string & method, const double timeout) : _uri(uri)
{
    _queue = [[NSOperationQueue alloc] init];
    
    CFURLRef requestUrl(uri);
    _platform_request = [[NSMutableURLRequest alloc] initWithURL:(NSURL *)requestUrl cachePolicy:NSURLRequestUseProtocolCachePolicy timeoutInterval:timeout];
    
    NSString *ns_method = [[NSString alloc] initWithUTF8String:method.c_str()];
    [_platform_request setHTTPMethod:ns_method];
    [ns_method release];
    
    [_platform_request setHTTPShouldUsePipelining:YES];
}

URLRequest::~URLRequest()
{
    [_queue release];
    [_platform_request release];
    [_operation release];
}

void URLRequest::Start()
{
    _operation = [[__URLRequestOperation alloc] initWithURLRequest:_platform_request inQueue:_queue withCompletion:^(NSHTTPURLResponse *response, NSData *data, NSError *error) {
        NSString *stringData = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
        Json::Reader reader(Json::Features::all());
        
        std::string document([stringData UTF8String]);
        Json::Value root;
        
        reader.parse(document, root);
        
        for (std::string member : root.Members)
        {
            
        }
        
        std::cout << "test" << std::endl;
        
        [stringData release];
    }];
}

void URLRequest::SetUseCookies(const bool value)
{
    [_platform_request setHTTPShouldHandleCookies:(value == true)];
}

bool URLRequest::UseCookies() const
{
    return (bool)[_platform_request HTTPShouldHandleCookies];
}


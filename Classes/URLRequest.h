//
//  URLRequest.h
//  RestToolbox
//
//  Created by Matt Jarjoura on 1/24/13.
//  Copyright (c) 2013 Artcator Inc. All rights reserved.
//

#pragma once

#include <iostream>
#include "BasicObject.h"
#include "BasicUri.h"

#if defined(APPLE)
#import <Foundation/Foundation.h>

@class __URLRequestOperation;
#endif

extern double const kDefaultRequestTimeout;

namespace RestToolbox
{
    namespace Models
    {
        class URLRequest final : public BasicObject
        {
        public:
            URLRequest(const BasicUri & uri, const std::string & method, const double timeout = kDefaultRequestTimeout);
            virtual ~URLRequest();
            
            void Start();
            
            void SetUseCookies(const bool value);
            bool UseCookies() const;
            
        private:
#if defined(APPLE)
            __URLRequestOperation *_operation;
            NSMutableURLRequest *_platform_request;
            NSOperationQueue *_queue;
#elif defined(WIN32)
            
#endif
            const BasicUri &_uri;
        };
    }
}

//
//  URLRequest.h
//  RestToolbox
//
//  Created by Matt Jarjoura on 1/24/13.
//  Copyright (c) 2013 Artcator Inc. All rights reserved.
//

#pragma once

#include <iostream>
#include <functional>
#include <vector>
#include "jsoncpp/json/json.h"
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
        //typedef decltype(f) function_t;

        typedef std::function<void(int status, const Json::Value & root)> URLRequestCompletion;
        
        class URLRequest final : public BasicObject
        {
        public:
            URLRequest(BasicUri const& uri, std::string const& method, const double timeout, URLRequestCompletion const& completion);
            //URLRequest(URLRequest && other);
            
            //URLRequest(const URLRequest & request) = delete;
            //URLRequest & operator=(const URLRequest & request) = delete;
            
            virtual ~URLRequest();
            
            void Start();
            
            void SetUseCookies(const bool value);
            bool UseCookies() const;
            
        private:
            void CallCompletion(int status, const Json::Value & root);
            
#if defined(APPLE)
            __URLRequestOperation *_operation;
            NSMutableURLRequest *_platform_request;
            NSOperationQueue *_queue;
#elif defined(WIN32)
            
#endif
            const double _timeout;
            const BasicUri _uri;
            URLRequestCompletion const& _completion;
        };
    }
}


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

#if defined(_WIN32)
  #include <wrl/implements.h>
  #include <ppl.h>
  #include <MsXml6.h>
  using namespace Microsoft::WRL;
  class __URLRequestOperation;
#elif defined(APPLE) && defined(__OBJC__)
  #import <Foundation/Foundation.h>
  @class __URLRequestOperation;
#elif defined(APPLE) && !defined(__OBJC__)
  typedef void __URLRequestOperation;
  typedef void NSMutableURLRequest;
  typedef void NSOperationQueue;
#endif

extern double const kDefaultRequestTimeout;

namespace RestToolbox
{
    namespace Models
    {
        typedef std::function<void(int status, const Json::Value & root)> URLRequestCompletion;
        
        template <typename _Functor>
        class URLRequestFunctor
        {
        public:
            URLRequestFunctor(_Functor const& functor) : _functor(std::cref(functor)) {}
            _Functor const& get() const { return _functor; }
        private:
            _Functor const& _functor;
        };
        
        class URLRequest final : public BasicObject
        {
        public:
            URLRequest(BasicUri const& uri, std::string const& method, const double timeout, URLRequestCompletion completion);
            URLRequest(const URLRequest & request);
            //URLRequest & operator=(const URLRequest & request) = delete;
            
            virtual ~URLRequest();
            
            void Start();
            void Cancel();
            
            void SetUseCookies(const bool value);
            bool UseCookies() const;
            
        private:
            void CallCompletion(int status, const Json::Value & root);
            
#if defined(APPLE)
            __URLRequestOperation *_operation;
            NSMutableURLRequest *_platform_request;
            NSOperationQueue *_queue;
#elif defined(_WIN32)
            friend class __URLRequestOperation;

            Concurrency::cancellation_token_source _cancelToken;
            ComPtr<IXMLHTTPRequest2> _operation;
#endif
            bool _canceled;
            const double _timeout;
            const BasicUri _uri;
            std::string _method;
            URLRequestCompletion _completion;
            //URLRequestFunctor<URLRequestCompletion> *_completion;
        };
    }
}


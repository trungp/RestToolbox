//
//  URIRequestQueue.h
//  RestToolbox
//
//  Created by Matt Jarjoura on 1/27/13.
//  Copyright (c) 2013 Artcator Inc. All rights reserved.
//

#pragma once

#include "BasicObject.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>
#include "URLRequest.h"

namespace RestToolbox
{
    namespace Models
    {
        class URIRequestQueue final : public BasicObject
        {
        public:
        	URIRequestQueue();
            virtual ~URIRequestQueue();

            void AddRequest(URLRequest const& request);

            void Start();
            void Stop();

        private:
            friend class URLRequest;

            std::deque<URLRequest> _queue;
            std::mutex _queueLock;
            std::condition_variable _condition;
            bool _canceled;

        };
    }
}



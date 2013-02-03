//
//  URIRequestQueue.cc
//  RestToolbox
//
//  Created by Matt Jarjoura on 1/27/13.
//  Copyright (c) 2013 Artcator Inc. All rights reserved.
//

#include "pch.h"
#include "URLRequestQueue.h"
#include <fstream>
#include <iostream>
#include <string>
#include <deque>
#include <vector>
#include <future>
#include <functional>
#include <thread>

using namespace RestToolbox::Models;

URIRequestQueue::URIRequestQueue()
{

}

URIRequestQueue::~URIRequestQueue()
{
    _condition.notify_all();
    _canceled = true;
}

void URIRequestQueue::AddRequest(URLRequest const& request)
{
    _queueLock.lock();
    _queue.push_back(request);
    _queueLock.unlock();
}

void URIRequestQueue::Start()
{

}

void URIRequestQueue::Stop()
{

}


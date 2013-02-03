//
//  TaskRunner.cp
//  RestToolbox
//
//  Created by Matt Jarjoura on 1/24/13.
//  Copyright (c) 2013 Artcator Inc. All rights reserved.
//

#include "pch.h"
#include <algorithm>
#include "TaskRunner.h"
#if defined(APPLE)
#include <dispatch/dispatch.h>
#include <Block.h>
#include <CoreFoundation/CFRunLoop.h>
#elif defined(WIN32)
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
#endif

using namespace RestToolbox::Models;

TaskRunner::TaskRunner()
{
    
}

TaskRunner::~TaskRunner()
{
    
}

void TaskRunner::RunDelayedTask(uint64_t delay, std::function<void ()> task)
{
#if defined(APPLE)
    dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, delay * NSEC_PER_SEC);
    
    //CFRunLoopRef currentLoop = CFRunLoopGetCurrent();
    __block dispatch_block_t taskBlock = Block_copy(^{ task(); });
    dispatch_after(popTime, dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        //CFRunLoopPerformBlock(currentLoop, kCFRunLoopDefaultMode, taskBlock);
        taskBlock();
        Block_release(taskBlock);
    });
#elif defined(WIN32)
    TimeSpan popTime;
    popTime.Duration = delay * 10000000;

    try
    {
        _delayedTask = ThreadPoolTimer::CreateTimer(ref new TimerElapsedHandler([task] (ThreadPoolTimer ^timer) {
            task();
        }), popTime);
        
    }
    catch (Platform::Exception ^ex)
    {
        
    }
#endif
}

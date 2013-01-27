//
//  TaskRunner.cp
//  RestToolbox
//
//  Created by Matt Jarjoura on 1/24/13.
//
//

#include "TaskRunner.h"
#if defined(APPLE)
#include <dispatch/dispatch.h>
#include <Block.h>
#include <CoreFoundation/CFRunLoop.h>
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
    
    CFRunLoopRef currentLoop = CFRunLoopGetCurrent();
    __block dispatch_block_t taskBlock = Block_copy(^{ task(); });
    dispatch_after(popTime, dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        //CFRunLoopPerformBlock(currentLoop, kCFRunLoopDefaultMode, taskBlock);
        taskBlock();
        Block_release(taskBlock);
    });
#endif
}

//
//  TaskRunner.h
//  RestToolbox
//
//  Created by Matt Jarjoura on 1/24/13.
//  Copyright (c) 2013 Artcator Inc. All rights reserved.
//

#pragma once

#include <iostream>
#include <functional>
#include "BasicObject.h"

namespace RestToolbox
{
    namespace Models
    {
        class TaskRunner final : public BasicObject
        {
        public:
            TaskRunner();
            virtual ~TaskRunner();
            
            void RunDelayedTask(uint64_t delay, std::function<void()> task);

        private:
#if defined(WIN32)
            Windows::System::Threading::ThreadPoolTimer ^_delayedTask;
#endif
        };
    }
}

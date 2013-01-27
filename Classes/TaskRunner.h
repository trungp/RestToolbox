//
//  TaskRunner.h
//  RestToolbox
//
//  Created by Matt Jarjoura on 1/24/13.
//
//

#pragma once

#include <iostream>
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
        };
    }
}

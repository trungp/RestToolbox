//
//  BasicApplication.h
//  RestToolbox
//
//  Created by Matt Jarjoura on 1/28/13.
//
//

#pragma once

#include <iostream>
#include "BasicObject.h"

#define RestApplication    RestToolbox::Models::BasicApplication::Instance()

namespace RestToolbox
{
    namespace Models
    {
        class BasicApplication : public BasicObject
        {
        public:
            static BasicApplication* Instance();
            
            BasicApplication();
            virtual ~BasicApplication();
            
            void Initialize();
        
        };
    }
}

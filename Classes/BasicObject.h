//
//  BasicObject.h
//  RestToolbox
//
//  Created by Matt Jarjoura on 1/24/13.
//  Copyright (c) 2013 Artcator Inc. All rights reserved.
//

#pragma once

#include <iostream>

#if defined(WIN32)
typedef unsigned __int64 uint64_t;
#endif

namespace RestToolbox
{
    namespace Models
    {
        class BasicObject
        {
        public:
            BasicObject();
            virtual ~BasicObject();
        };
    }
}

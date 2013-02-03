//
//  BasicObject.h
//  RestToolbox
//
//  Created by Matt Jarjoura on 1/24/13.
//  Copyright (c) 2013 Artcator Inc. All rights reserved.
//

#pragma once

#include <iostream>

#if defined(_WIN32)
typedef unsigned __int64 uint64_t;
#endif

namespace RestToolbox
{
#if defined(_WIN32)
    std::string utf8_encode(const std::wstring &wstr);
    std::wstring utf8_decode(const std::string &str);
#endif

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

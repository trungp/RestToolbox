//
//  BasicUri.h
//  RestToolbox
//
//  Created by Matt Jarjoura on 1/24/13.
//  Copyright (c) 2013 Artcator Inc. All rights reserved.
//

#pragma once

#include <iostream>
#include "BasicObject.h"

#if defined(APPLE)
#include "CFScopedPtr.h"
#endif

namespace RestToolbox
{
    namespace Models
    {
        class BasicUri : public BasicObject
        {
        public:
#if defined(APPLE)
            typedef CFURLRef system_base_type;
            typedef RestToolbox::CFScopedPtr<system_base_type> scoped_base_type;
#elif defined(_WIN32)
            typedef Windows::Foundation::Uri^ system_base_type;
            typedef system_base_type scoped_base_type;
#endif

            BasicUri(void);
            BasicUri(const std::string & uri);
            BasicUri(const BasicUri & other);
            
#if !defined(_WIN32)
            // Visual Studio 2012 doesn't support this C++11 feature
            BasicUri(BasicUri const && other) = delete;
            BasicUri& operator= (const BasicUri& other) = delete;
#endif

            virtual ~BasicUri();

            operator system_base_type() const;
            
        private:
            std::string _uri;
            scoped_base_type _systemUri;
        };
    }
}


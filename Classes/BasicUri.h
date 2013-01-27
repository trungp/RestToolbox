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
#include <CoreFoundation/CFURL.h>
#endif

namespace RestToolbox
{
    namespace Models
    {
        class BasicUri : public BasicObject
        {
        public:
            BasicUri(const std::string & uri);
            virtual ~BasicUri();
            
#if defined(APPLE)
            operator CFURLRef() const;
#endif
            
        private:
            const std::string &_uri;
#if defined(APPLE)
            CFURLRef _systemUri;
#endif
        };
    }
}


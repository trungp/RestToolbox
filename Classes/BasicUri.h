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
            BasicUri(void);
            BasicUri(const std::string & uri);
            BasicUri(const BasicUri & other);
            
            BasicUri(BasicUri const && other) = delete;
            BasicUri& operator= (const BasicUri& other) = delete;
            
            virtual ~BasicUri();
            
#if defined(APPLE)
            operator CFURLRef() const;
#elif defined(WIN32)
            operator Windows::Foundation::Uri^() const;
#endif
            
        private:
            std::string _uri;
#if defined(APPLE)
            RestToolbox::CFScopedPtr<CFURLRef> _systemUri;
#endif
        };
    }
}


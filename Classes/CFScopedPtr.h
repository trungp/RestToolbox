//
//  CFScopedPtr.h
//  RestToolbox
//
//  Created by Matt Jarjoura on 2/2/13.
//  Copyright (c) 2013 Artcator Inc. All rights reserved.
//

#pragma once

#include <CoreFoundation/CoreFoundation.h>

namespace RestToolbox {
    
    template<typename CFType>
    class CFScopedPtr {
    public:
        typedef CFType element_type;
        
        typedef enum {
            assumed = 0,
            retained = 1,
        } policy;
        
        explicit CFScopedPtr(CFType object = nullptr, policy ownership = assumed) : _object(object)
        {
            if (object && ownership == retained)
                CFRetain(_object);
        }
        
        CFScopedPtr(const CFScopedPtr<CFType>& other) : _object(other)
        {
            if (_object)
                CFRetain(_object);
        }
        
        CFScopedPtr& operator=(const CFScopedPtr<CFType>& other)
        {
            reset(other.get(), retained);
            return *this;
        }
        
        ~CFScopedPtr()
        {
            if (_object)
                CFRelease(_object);
        }
        
        bool operator==(CFType other) const {
            return _object == other;
        }
        
        bool operator!=(CFType other) const {
            return _object != other;
        }
        
        operator CFType() const
        {
            return _object;
        }
        
        CFType get() const
        {
            return _object;
        }
        
        void reset(CFType object, policy ownership = assumed)
        {
            if (object && ownership == retained)
                CFRetain(object);
            if (_object)
                CFRelease(_object);
            _object = object;
        }
        
        CFType release()
        {
            CFType temp = _object;
            _object = nullptr;
            return temp;
        }
        
    private:
        CFType _object;
    };
    
}


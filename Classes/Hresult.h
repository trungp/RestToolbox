//
//  Hresult.h
//  RestToolbox
//
//  Created by Matt Jarjoura on 1/27/13.
//  Copyright (c) 2013 Artcator Inc. All rights reserved.
//

#pragma once

#if defined(WIN32)
#include "BasicObject.h"

namespace RestToolbox
{
    namespace Models
    {
        class HResult final : public BasicObject
        {
        public:
            static HResult FromWin32(DWORD errorCode); 
            static HResult FromLastError(); 

        public:
            HResult();
            HResult(HRESULT hr);
            virtual ~HResult();

            HRESULT Get() const;
            operator HRESULT() const;

            bool Succeeded() const;

            std::wstring ToHexString() const; 
            std::wstring ToErrorMessage() const; 

        private:
            const HRESULT _hr;
        };

        inline bool operator==(const HResult & hr1, const HResult & hr2) 
        {
            return hr1.Get() == hr2.Get();
        }

        inline bool operator!=(const HResult & hr1, const HResult & hr2)
        {
            return hr1.Get() != hr2.Get();
        }
    }
}

#endif /* defined(WIN32) */

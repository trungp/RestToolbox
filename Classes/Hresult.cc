//
//  Hresult.cc
//  RestToolbox
//
//  Created by Matt Jarjoura on 1/27/13.
//  Copyright (c) 2013 Artcator Inc. All rights reserved.
//

#include "pch.h"
#include "Hresult.h"
#include <sstream>

using namespace RestToolbox::Models;

HResult HResult::FromWin32(DWORD errorCode)
{
    return HResult(HRESULT_FROM_WIN32(errorCode));
}

HResult HResult::FromLastError()
{
    DWORD lastError = ::GetLastError(); 
    return FromWin32(lastError); 
}

HResult::HResult() : _hr(E_FAIL)
{
}

HResult::HResult(HRESULT code) : _hr(code)
{
}

HResult::~HResult()
{
}

HRESULT HResult::Get() const
{
    return _hr;
}

HResult::operator HRESULT() const
{
    return _hr;
}

bool HResult::Succeeded() const
{
    return SUCCEEDED(_hr);
}

std::wstring HResult::ToHexString() const
{
    std::wostringstream os; 
    os << L"0x"; 
    os << std::hex << std::uppercase << _hr; 
    return os.str(); 
}

std::wstring HResult::ToErrorMessage() const
{
    std::wostringstream message; 
    
    wchar_t zTempWide[MAX_PATH+1] = {0};
    if (::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
        nullptr, 
        static_cast<DWORD>(_hr), 
        0, 
        zTempWide,
        MAX_PATH, 
        nullptr)) 
    {
        message << zTempWide;
    }
    else 
    { 
        message << L"FormatMessage() failed (hr=" << ToHexString() << L")."; 
    }

    return message.str(); 
}


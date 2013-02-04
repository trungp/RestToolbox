//
//  BasicUri.cp
//  RestToolbox
//
//  Created by Matt Jarjoura on 1/24/13.
//  Copyright (c) 2013 Artcator Inc. All rights reserved.
//

#include "pch.h"
#include "BasicUri.h"
#include "Logger.h"
#include <string>
#include <sstream>
#if defined(APPLE)
#include <CoreFoundation/CFString.h>
#include "CFScopedPtr.h"
#endif

using namespace RestToolbox::Models;

#if !defined(_WIN32)
#define __FUNCSIG__   __PRETTY_FUNCTION__
#endif

BasicUri::BasicUri(void) : _uri("")
{
    LogDebug("%s", __FUNCSIG__);
}

BasicUri::BasicUri(const std::string & uri) : _uri(uri)
{
    LogDebug("%s", __FUNCSIG__);

#if defined(APPLE)
    RestToolbox::CFScopedPtr<CFStringRef> urlString(CFStringCreateWithCString(kCFAllocatorDefault, _uri.c_str(), _uri.length()));
    
    if (urlString != nullptr)
    {
        _systemUri.reset(CFURLCreateWithString(kCFAllocatorDefault, urlString, nullptr));
    }
#elif defined(_WIN32)
    auto string = ref new Platform::String(utf8_decode(_uri).c_str());
    _systemUri = ref new Windows::Foundation::Uri(string);
#endif
}

BasicUri::BasicUri(const BasicUri & other) : _uri(other._uri)
{
    LogDebug("%s", __FUNCSIG__);
    _systemUri = other._systemUri;
}

BasicUri& BasicUri::operator= (const BasicUri& other)
{
    LogDebug("%s", __FUNCSIG__);
    
    _uri = other._uri;
    _systemUri = other._systemUri;
    
    return *this;
}

BasicUri::~BasicUri()
{
#if defined(_WIN32)
    delete _systemUri;
#endif
}

BasicUri::operator system_base_type() const
{
    return _systemUri;
}

std::wstring UriEncode(const std::wstring& text)
{
    const wchar_t* data = text.c_str();
    size_t length = wcslen(data);
    std::wstringstream stream;

    for (size_t i = 0; i < length; i++)
    {
        wchar_t current = data[i];
        switch (current)
        {
        case L'!':
            stream << L"%21";
            break;
        case L'*':
            stream << L"%2A";
            break;
        case L'\'':
            stream << L"%27";
            break;
        case L'(':
            stream << L"%28";
            break;
        case L')':
            stream << L"%29";
            break;
        case L';':
            stream << L"%3B";
            break;
        case L':':
            stream << L"%3A";
            break;
        case L'@':
            stream << L"%40";
            break;
        case L'&':
            stream << L"%26";
            break;
        case L'=':
            stream << L"%3D";
            break;
        case L'+':
            stream << L"%2B";
            break;
        case L'$':
            stream << L"%24";
            break;
        case L',':
            stream << L"%2C";
            break;
        case L'/':
            stream << L"%2F";
            break;
        case L'?':
            stream << L"%3F";
            break;
        case L'#':
            stream << L"%23";
            break;
        case L'[':
            stream << L"%5B";
            break;
        case L']':
            stream << L"%5D";
            break;
        case L' ':
            stream << L"%20";
            break;
        case L'\n':
            stream << L"%0D%0A";
            break;
        default:
            stream << current;
            break;
        }
    }

    return stream.str();
}


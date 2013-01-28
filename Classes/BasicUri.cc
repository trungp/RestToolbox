//
//  BasicUri.cp
//  RestToolbox
//
//  Created by Matt Jarjoura on 1/24/13.
//  Copyright (c) 2013 Artcator Inc. All rights reserved.
//

//#include "pch.h"
#include "BasicUri.h"
#include <string>
#include <sstream>
#if defined(APPLE)
#include <CoreFoundation/CFString.h>
#endif

using namespace RestToolbox::Models;

BasicUri::BasicUri(const std::string & uri) : _uri(uri)
{
#if defined(APPLE)
    CFStringRef urlString = CFStringCreateWithCString(kCFAllocatorDefault, uri.c_str(), uri.length());
    if (urlString != nullptr)
    {
        _systemUri = CFURLCreateWithString(kCFAllocatorDefault, urlString, nullptr);
        CFRelease(urlString);
    }
#endif
}

BasicUri::~BasicUri()
{
#if defined(APPLE)
    if (_systemUri != nullptr)
        CFRelease(_systemUri);
#endif
}

#if defined(APPLE)
BasicUri::operator CFURLRef() const
{
    return _systemUri;
}

#elif defined(WIN32)

BasicUri::operator Windows::Foundation::Uri^() const
{
    return nullptr;
}
#endif

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


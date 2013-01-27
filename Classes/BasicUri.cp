//
//  BasicUri.cp
//  RestToolbox
//
//  Created by Matt Jarjoura on 1/24/13.
//  Copyright (c) 2013 Artcator Inc. All rights reserved.
//

#include "BasicUri.h"
#include <CoreFoundation/CFString.h>

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

BasicUri::operator CFURLRef() const
{
    return _systemUri;
}

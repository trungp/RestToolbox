//
//  Logger.cc
//  RestToolbox
//
//  Created by Matt Jarjoura on 1/27/13.
//  Copyright (c) 2013 Artcator Inc. All rights reserved.
//

#include "pch.h"
#include "Logger.h"
#include <mutex>
#include <thread>

using namespace RestToolbox::Models;
using namespace RestToolbox::Debugging;

Logger * Logger::Instance()
{
    static Logger *sLogger = nullptr;
    static std::once_flag flag;
    std::call_once(flag, [] () {
        sLogger = new Logger();
    });
    return sLogger;
}

Logger::Logger()
{
}

Logger::~Logger()
{
}

void Logger::Log(const char *message, va_list argptr)
{
    char buffer[BUFSIZ];
#if defined(WIN32)
    vsnprintf_s(buffer, sizeof buffer, message, argptr);
#else
    vsnprintf(buffer, sizeof buffer, message, argptr);
#endif
    std::cerr << buffer << std::endl;
}

void Logger::Debug(const char *message, ...)
{
    va_list argptr;
    va_start(argptr, message);
    Log(message, argptr);
    va_end(argptr);
}

//void Logger::Debug(std::string &message, ...)
//{
//    va_list argptr;
//    va_start(argptr, message);
//
//    va_end(argptr);
//}

void Logger::Tester(const char *message, ...)
{
    va_list argptr;
    va_start(argptr, message);
    Log(message, argptr);
    va_end(argptr);
}

//void Logger::Tester(std::string &message, ...)
//{
//    va_list argptr;
//    va_start(argptr, message);
//
//    va_end(argptr);
//}

void Logger::Always(const char *message, ...)
{
    va_list argptr;
    va_start(argptr, message);
    Log(message, argptr);
    va_end(argptr);
}

//void Logger::Always(std::string &message, ...)
//{
//    va_list argptr;
//    va_start(argptr, message);
//
//    va_end(argptr);
//}
//

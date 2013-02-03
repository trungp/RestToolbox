//
//  Logger.h
//  RestToolbox
//
//  Created by Matt Jarjoura on 1/27/13.
//  Copyright (c) 2013 Artcator Inc. All rights reserved.
//

#pragma once

#include "BasicObject.h"

#define LogDebug(format, ...)  RestToolbox::Debugging::__LOG()->Debug(format, __VA_ARGS__)
#define LogTest(format, ...)   RestToolbox::Debugging::__LOG()->Tester(format, __VA_ARGS__)
#define LogAlways(format, ...) RestToolbox::Debugging::__LOG()->Always(format, __VA_ARGS__)

namespace RestToolbox
{
    namespace Debugging
    {
        class Logger final : public Models::BasicObject
        {
        public:
            static Logger * Instance();

        	Logger();
            virtual ~Logger();

            //void Debug(wchar_t *message, ...);
            //void Tester(wchar_t *message, ...);
            //void Always(wchar_t *message, ...);

            void Debug(const char *message, ...);
            void Tester(const char *message, ...);
            void Always(const char *message, ...);

        private:
            void Log(const char *message, va_list argptr);
        };

        inline Logger * __LOG() {
            return Logger::Instance();
        }
    }
}

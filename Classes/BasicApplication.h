//
//  BasicApplication.h
//  RestToolbox
//
//  Created by Matt Jarjoura on 1/28/13.
//
//

#pragma once

#include <iostream>
#include "BasicObject.h"
#include "easySQLite/SqlDatabase.h"

#define RestApplication    RestToolbox::Models::BasicApplication::Instance()

namespace RestToolbox
{
    namespace Models
    {
        class BasicApplication : public BasicObject
        {
        public:
            static BasicApplication* Instance();
            
            BasicApplication();
            virtual ~BasicApplication();
            
            virtual void Initialize();
            
            void SetPath(const std::string & value);
            const std::string & Path() const;
            
            bool DatabaseOpen() const;
            SQL::Database & Database();
            
        private:
            SQL::Database _db;
            std::string _path;
            
        };
    }
}

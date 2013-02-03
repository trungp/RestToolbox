//
//  BasicApplication.cc
//  RestToolbox
//
//  Created by Matt Jarjoura on 1/28/13.
//
//

#include "BasicApplication.h"
#include <mutex>
#include "SQLite3/sqlite3.h"

using namespace RestToolbox::Models;

BasicApplication* BasicApplication::Instance()
{
    static std::once_flag sOnceFlag;
    static BasicApplication *sBasicApplication = nullptr;
    std::call_once(sOnceFlag, [] () {
        sBasicApplication = new BasicApplication();
    });
    return sBasicApplication;
}

BasicApplication::BasicApplication()
{
    
}

BasicApplication::~BasicApplication()
{
    
}

void BasicApplication::SetPath(const std::string & value)
{
    _path = value;
}

const std::string & BasicApplication::Path() const
{
    return _path;
}

void BasicApplication::Initialize()
{
    _db.open(_path);
}


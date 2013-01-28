//
// Copyright (c) 2013 Artcator Inc.
// Copyright (C) 2010 Piotr Zagawa
//
// Released under BSD License
//

#include "SqlDatabase.h"
#include "SqlRecordset.h"
#include <time.h>

using namespace RestToolbox::SQL;

Database::Database(void)
{
	_db = nullptr;
	_result_open = SQLITE_ERROR;

	close();

#ifdef WIN32
	_tzset();
#else
    tzset();
#endif
}

Database::~Database(void)
{
	close();
}

sqlite3* Database::getHandle()
{
	return _db;
}

string Database::errMsg()
{
	return _err_msg;
}

void Database::close()
{
	if (_db)
	{
		sqlite3_close(_db);
		_db = NULL;
		_err_msg.clear();
		_result_open = SQLITE_ERROR;
	}
}

bool Database::isOpen()
{
	return (_result_open == SQLITE_OK);
}

bool Database::open(string filename)
{
	close();

	_result_open = sqlite3_open(filename.c_str(), &_db);

	if (isOpen())
	{
		return true;
	} else {
		_err_msg = sqlite3_errmsg(_db);
	}

	THROW_EXCEPTION("Database::open: " + errMsg())

	return false;
}

bool Database::transaction(std::function<void()> & commands)
{
    if (!transactionBegin())
        return false;
    
    try
    {
        commands();
    }
    catch (Exception ex)
    {
        transactionRollback();
        return false;
    }
    
    return transactionCommit();
}

bool Database::transactionBegin()
{
	RecordSet rs(_db);

	if (rs.query("BEGIN TRANSACTION"))
		return true;

	return false;
}

bool Database::transactionCommit()
{
	RecordSet rs(_db);

	if (rs.query("COMMIT TRANSACTION"))
		return true;

	return false;
}

bool Database::transactionRollback()
{
	RecordSet rs(_db);

	if (rs.query("ROLLBACK TRANSACTION"))
		return true;

	return false;
}

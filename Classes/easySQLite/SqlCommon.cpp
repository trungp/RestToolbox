//
// Copyright (c) 2013 Artcator Inc.
// Copyright (C) 2010 Piotr Zagawa
//
// Released under BSD License
//

#include "SqlCommon.h"
#include <sstream>
#if defined(APPLE)
#include <CommonCrypto/CommonCrypto.h>
#elif defined(_WIN32)
#include "SHA1.h"
#endif

using namespace RestToolbox::SQL;

time::time()
{
	if (::time(&_value) == -1)
		_value = -1;
}

time::time(const time& value)
{
	_value = value._value;
}

time::time(integer value)
{
	_value = value;
}

RestToolbox::SQL::time& RestToolbox::SQL::time::operator=(const time& value)
{
	if (this != &value)
	{
		this->_value = value._value;
	}
	return *this;
}

bool RestToolbox::SQL::time::operator==(const RestToolbox::SQL::time& value)
{
	if (this == &value)
		return true;

	if (value._value == value._value)
		return true;

	return false;
}

RestToolbox::SQL::time RestToolbox::SQL::time::now()
{
	RestToolbox::SQL::time t;
	return t;
}

double RestToolbox::SQL::time::diff(RestToolbox::SQL::time& value)
{
	return difftime(this->_value, value._value);
}

integer RestToolbox::SQL::time::asInteger()
{
	return _value;
}

time_t RestToolbox::SQL::time::get()
{
	return _value;
}

string RestToolbox::SQL::time::format(const char* format)
{
	string s;
	tm localtime;
	char buffer[128];
    
#ifdef WIN32
    if (localtime_s(&localtime, &_value) == 0)
		if (strftime(buffer, 128, format, &localtime) > 0)
			s = buffer;
#else
    localtime_r(&_value, &localtime);
    if (strftime(buffer, 128, format, &localtime) > 0)
        s = buffer;
#endif

	return s;
}

string RestToolbox::SQL::time::asString()
{
	return format("%d-%m-%Y %H:%M, %a");
}

string RestToolbox::SQL::time::asTimeString()
{
	return format("%H:%M");
}

string RestToolbox::SQL::time::asDateString()
{
	return format("%d-%m-%Y");
}

void RestToolbox::SQL::time::addValue(integer value)
{
	_value += value;
}

void RestToolbox::SQL::time::addMinutes(integer count)
{
	_value += (60 * count);
}

void RestToolbox::SQL::time::addHours(integer count)
{
	_value += (3600 * count);
}

void RestToolbox::SQL::time::addDays(integer count)
{
	_value += ((3600 * 24) * count);
}


string RestToolbox::SQL::intToStr(int value)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

string RestToolbox::SQL::intToStr(integer value)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

string RestToolbox::SQL::quoteStr(string value)
{
	string s;

	for (string::iterator it = value.begin(); it != value.end(); it++)
	{
		char c = *it;
		s += c;
		if (c == '\'')
			s += c;
	}

	return s;
}

//CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

string RestToolbox::SQL::binToHex(const char* buffer, int size)
{
	std::string s;

	char digit[4];

	unsigned char* p = (unsigned char*)buffer;

	for (int index = 0; index < size; index++)
	{
		sprintf(digit, "%02x", *p++);
		s += digit;
	}

	return s;
}

#pragma warning(default : 4996)

string RestToolbox::SQL::generateSHA(const std::string value)
{
#ifdef _WIN32
	CSHA1 sha;
    
	sha.Update((UINT_8*)value.c_str(), value.length());
    
	sha.Final();
    
	UINT_8 digest[20];
	if (sha.GetHash(digest))
	{
		const int size = sizeof(digest) / sizeof(UINT_8);
		return binToHex((char*)digest, size);
	}

	return "";
#else
    const char *str = value.c_str();
    unsigned char r[CC_SHA1_DIGEST_LENGTH];
    CC_SHA1(str, static_cast<CC_LONG>(value.length()), r);
    
    std::stringstream ss;
    ss << r;
    return ss.str();
#endif
}

string& RestToolbox::SQL::trimleft(string& s)
{
	string::iterator it;

	for( it = s.begin(); it != s.end(); it++ )
		if( !isspace( *it ) )
			break;

	s.erase( s.begin(), it );
	return s;
}

string& RestToolbox::SQL::trimright(string& s)
{
	string::difference_type dt;
	string::reverse_iterator it;

	for( it = s.rbegin(); it != s.rend(); it++ )
		if( !isspace( *it ) )
			break;

	dt = s.rend() - it;

	s.erase( s.begin() + dt, s.end() );
	return s;
}

string& RestToolbox::SQL::trim(string& s)
{
	RestToolbox::SQL::trimleft( s );
	RestToolbox::SQL::trimright( s );
	return s;
}

string RestToolbox::SQL::trim(const string& s)
{
	string t = s;
	return RestToolbox::SQL::trim(t);
}

//CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

void RestToolbox::SQL::listToVector(string s, std::vector<string>& vector, const char* sep)
{
	vector.clear();

	char* buffer = new char[s.length() + 1];

	memcpy(buffer, s.c_str(), s.length());
	buffer[s.length()] = 0;

	char* token = strtok(buffer, sep);

	while (token)
	{
		string sToken = token;
		vector.push_back(RestToolbox::SQL::trim(sToken));
		token = strtok(NULL, sep);
	}

	delete [] buffer;
}

#pragma warning(default : 4996)

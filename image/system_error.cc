// -*- mode: C++; tab-width: 4; c-basic-offset: 4 -*-

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(__CYGWIN__)
#define WINDOWS
#else
#define POSIX
#endif


#include <sstream>  // ostringstream

#ifdef WINDOWS
#include "windows.h"
#else
#include <errno.h>
#endif

#include "system_error.h"


namespace image {
	
	
	using namespace std;
	
	
	system_error::system_error(
		const char * function_name) throw () :
#ifdef WINDOWS
		error_code_(::GetLastError()),
#else
		error_code_(errno),
#endif
		function_name_(function_name)
	{}
	
	
	system_error::system_error(
		int error_code,
		const char * function_name) throw () :
		error_code_(error_code),
		function_name_(function_name)
	{}
	
	
	const char * system_error::what() const throw ()
	{
#ifdef WINDOWS
		return "system_error";
#else
		return ::strerror(error_code_);
#endif
	}
	
	
	string system_error::message() const
	{
		ostringstream oss;
		oss << "In function `" << function_name_ << "':" << std::endl
			<< "System error " << error_code_ << ": ";
		
#ifdef WINDOWS
		LPVOID buffer;
		::FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			0,
			error_code_,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			static_cast<LPSTR>(&buffer),
			0,
			0);
		oss << static_cast<LPCSTR>(buffer);
		::LocalFree(buffer);
#else
		oss << what();
#endif
		
		return oss.str();
	}
	
	
}

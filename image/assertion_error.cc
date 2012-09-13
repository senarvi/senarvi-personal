// -*- mode: C++; tab-width: 4; c-basic-offset: 4 -*-

#include <sstream>  // ostringstream

#define BOOST_ENABLE_ASSERT_HANDLER
#include <boost/assert.hpp>

#include "assertion_error.h"


namespace boost {
	
	
	using namespace image;
	
	
	void assertion_failed(
		const char * expression,
		const char * function_name,
		const char * file_name,
		long int line_number)
	{
		throw assertion_error(
			expression, function_name, file_name, line_number);
	}
	
	
}


namespace image {
	
	
	using namespace std;
	
	
	assertion_error::assertion_error(
		const char * expression,
		const char * function_name,
		const char * file_name,
		long int line_number) throw ()
		: logic_error("Assertion failed"),
		  expression_(expression),
		  function_name_(function_name),
		  file_name_(file_name),
		  line_number_(line_number)
	{}
	
	string assertion_error::message() const
	{
		ostringstream oss;
		oss <<
			file_name_ << ": In function `" << function_name_ << "':" << endl <<
			file_name_ << ":" << line_number_ << ": Assertion `" <<
			expression_ << "' failed.";
		return oss.str();
	}
	
	
}

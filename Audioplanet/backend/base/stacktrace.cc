//  backend/base/stacktrace.cc
//  Copyright (C) 1999-2002 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#include <string>

#include <execinfo.h>

#include "backend/base/stacktrace.h"
#include "backend/base/log.h"

extern "C" {
#include "demangle.h"
}


namespace ap
{

	StackTrace::StackTrace(int maxLength) :

		array_(new void * [maxLength])

	{
		length_ = backtrace(array_, maxLength);
	}


	StackTrace::~StackTrace()
	{
		delete array_;
	}


	void StackTrace::display(int firstSymbol) const
	{
		root.debug("STACK TRACE");
	
	
		char ** symbols = backtrace_symbols(array_, length_);
	
		for (int i = firstSymbol; i < length_; i++) {
			std::string s = symbols[i];

			int functionNamePosition = s.find_first_of('(') + 1;
			int functionNameLength = s.find_first_of('+') - functionNamePosition;
			std::string functionName = s.substr(functionNamePosition, functionNameLength);

			char * demangledName = cplus_demangle(functionName.c_str(), DMGL_PARAMS | DMGL_ANSI);
			if (demangledName != 0)
			{
				root.debug(std::string("> ") + demangledName);
				delete demangledName;
			}
			else
				root.debug(std::string("> ") + functionName);
		}
	}

}  // namespace ap

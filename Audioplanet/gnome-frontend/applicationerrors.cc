//  applicationerrors.cc
//  Copyright (C) 1999-2001 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#include <string>

#include "boost/lexical_cast.hpp"

#include "config.h"
#include "gui/control.h"
#include "gui/dialogs.h"
#include "system/debug.h"
#include "system/log.h"
#include "system/stacktrace.h"
#include "system/process.h"


void terminateHandler() throw ()
{
	try
	{
		root.emerg("Process terminated abnormally.");
		
		StackTrace stackTrace(32);
		stackTrace.display(8);
	}
	catch (...) {}
	
	Process::terminate(127);
}


void fatalError(const std::string& message) throw ()
{
	try
	{
		root.emerg(static_cast<std::string>("Fatal error: ") + message);
	}
	catch (...) {}
	
	std::terminate();
}


void unexpectedHandler() throw ()
{
	try
	{
		root.emerg("Internal error. Uncought exception in process " + boost::lexical_cast<std::string>(Process::id()) + ".");
	}
	catch (...) {}
	
	std::terminate();
}


void outOfMemoryHandler() throw ()
{
	try
	{
		root.emerg("Out of memory.");
	}
	catch (...) {}
	
	std::terminate();
}

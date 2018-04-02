//  backend/base/log.cc
//  Copyright (C) 1999-2002 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#include <unistd.h>  // getpid
#include <log4cpp/Category.hh>

#include "backend/base/log.h"
#include "backend/base/process.h"  // Process::executable


namespace ap
{

	std::string ReleaseLayout::format(const log4cpp::LoggingEvent & event)
	{
		std::stringstream messageStream;
		messageStream << process::executable << ": " << event.message << std::endl;
		return messageStream.str();
	}


	std::string DebugLayout::format(const log4cpp::LoggingEvent & event)
	{
		std::stringstream messageStream;
		messageStream << ::getpid() << ": " << event.message << std::endl;
		return messageStream.str();
	}


	log4cpp::Category & root = log4cpp::Category::getRoot();


	void log(const std::string& message)
	{
		root.debug(message);
	}


	void display(const std::string& message)
	{
		root.info(message);
	}

}

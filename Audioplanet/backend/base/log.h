//  backend/base/log.h
//  Copyright (C) 1999-2002 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef BACKEND_BASE_LOG_H
#define BACKEND_BASE_LOG_H


#include <string>

#include <log4cpp/Layout.hh>
#include <log4cpp/Category.hh>


namespace ap
{

	class ReleaseLayout : public log4cpp::Layout
	{
	public:
		std::string format(const log4cpp::LoggingEvent & event);
	};


	class DebugLayout : public log4cpp::Layout
	{
	public:
		std::string format(const log4cpp::LoggingEvent & event);
	};


	extern log4cpp::Category & root;


	void log(const std::string & message);
	void display(const std::string & message);

}  // namespace ap


#endif  // BACKEND_BASE_LOG_H

// Local Variables:
// mode:C++
// End:

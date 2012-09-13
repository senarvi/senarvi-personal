//  preferences.h
//  Copyright (C) 1999-2001 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef PREFERENCES_H
#define PREFERENCES_H

#ifdef __GNUG__
#pragma interface
#endif


#include <string>

#include "system/exception.h"


namespace Preferences {


	extern bool showRulers,
	            verbose;

	extern std::string systemDataDir,
	                   userDataDir,
	                   temporaryStorage,
	                   pluginSearchPath;


	extern void initialize() EXCEPTIONS;
	extern void readFromFile(const std::string & fileName) EXCEPTIONS;
	extern void readFromCommandLine(int argc, char * argv[]) EXCEPTIONS;
	extern void saveToFile(const std::string & fileName) EXCEPTIONS;


}  // namespace Preferences


#endif  // PREFERENCES_H

// Local Variables:
// mode:C++
// End:

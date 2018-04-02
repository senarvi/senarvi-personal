//  username.h
//  Copyright (C) 1999-2001 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef SYSTEM_USERNAME_H
#define SYSTEM_USERNAME_H


#include <string>

#include <unistd.h>
#include <pwd.h>


inline std::string userName()
{
	struct passwd * userInfo = getpwuid(getuid());
	return userInfo->pw_name;
}


#endif  // SYSTEM_USERNAME_H

// Local Variables:
// mode:C++
// End:

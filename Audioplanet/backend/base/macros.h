//  macros.h
//  Copyright (C) 1999-2001 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef SYSTEM_MACROS_H
#define SYSTEM_MACROS_H


#include <features.h>  // __GNUC_PREREQ

#include "boost/lexical_cast.hpp"


#define LOCATION (__FILE__ ":" + boost::lexical_cast<std::string>(__LINE__))


#if __GNUC_PREREQ(2, 6)
#define FUNCTION __PRETTY_FUNCTION__
#else
#define FUNCTION __func__
#endif


#endif  // SYSTEM_MACROS_H

// Local Variables:
// mode:C++
// End:

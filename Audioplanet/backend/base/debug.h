//  backend/base/debug.h
//  Copyright (C) 1999-2002 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef BACKEND_BASE_DEBUG_H
#define BACKEND_BASE_DEBUG_H

#ifdef _DEBUG


#include "backend/base/macros.h"
#include "backend/base/log.h"


#define trace() root.debug(FUNCTION)


#else  // _DEBUG


inline void trace() {}


#endif  // _DEBUG

#endif  // BACKEND_BASE_DEBUG_H

// Local Variables:
// mode:C++
// End:

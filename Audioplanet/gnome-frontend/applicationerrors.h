//  applicationerrors.h
//  Copyright (C) 1999-2001 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef APPLICATIONERRORS_H
#define APPLICATIONERRORS_H


#include <string>

#include "system/exception.h"


extern void terminateHandler() throw ();
extern void unexpectedHandler() throw ();
extern void fatalError(const std::string & message) throw ();


#endif  // APPLICATIONERRORS_H

// Local Variables:
// mode:C++
// End:

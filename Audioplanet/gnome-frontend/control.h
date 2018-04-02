//  control.h
//  Copyright (C) 1999-2000 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef GUI_CONTROL_H
#define GUI_CONTROL_H


#include "system/exception.h"


namespace GUI {


	extern void initialize(int argc, char* argv[]) EXCEPTIONS;
	extern void enterLoop() EXCEPTIONS;
	extern bool inLoop() EXCEPTIONS;
	
	
}  // namespace GUI


#endif  //  GUI_CONTROL_H

// Local Variables:
// mode:C++
// End:

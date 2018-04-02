//  selection.h
//  Copyright (C) 1999-2001 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef SELECTION_H
#define SELECTION_H

#ifdef __GNUG__
#pragma interface
#endif


class Selection {

public:

	typedef Uint32 Intensity;


	Intensity intensity(Size position) const EXCEPTIONS;


private:

	Intensity * data_;

};


#endif  // SELECTION_H

// Local Variables:
// mode:C++
// End:

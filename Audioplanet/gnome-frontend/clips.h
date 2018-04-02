//  clips.h
//  Copyright (C) 1999-2002 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef CLIPS_H
#define CLIPS_H


#include "system/exception.h"


class IClip;


namespace clips {


extern void initialize() EXCEPTIONS;

extern void add(IClip * clip) EXCEPTIONS;
extern void remove(IClip * clip) EXCEPTIONS;

extern void save(IClip * clip) EXCEPTIONS;
extern void saveAs(IClip * clip) EXCEPTIONS;

extern void open(const std::string & fileName) EXCEPTIONS;

// Prompt for saving changes and close a single clip.
// Return 0 if succesful, nonzero otherwise
extern int close(IClip * clip) EXCEPTIONS;

// Close all the clips.
// Return 0 if succesful, nonzero otherwise
extern int closeAll() EXCEPTIONS;


};  // namespace clips


#endif  // CLIPS_H

// Local Variables:
// mode:C++
// End:

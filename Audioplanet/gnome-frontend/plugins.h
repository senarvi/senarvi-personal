//  plugins.h
//  Copyright (C) 1999-2002 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef PLUGINS_H
#define PLUGINS_H


#include <string>


namespace plugins
{

    extern void initialize() EXCEPTIONS;

    extern void scanDirectory(const std::string & directoryName) EXCEPTIONS;

}


#endif  // PLUGIS_H

// Local Variables:
// mode:C++
// End:

//  io.h
//  Copyright (C) 1999-2001 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef WAV_IO_H
#define WAV_IO_H


#include <iostream>

#include "system/types.h"


inline Uint32 readMagic() EXCEPTIONS
{
	char buf[4];

	std::cin.read(buf, 4);
	if (std::cin.fail()) throw ParseError(LOCATION);
	return (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
}


inline Uint32 readInt32() EXCEPTIONS
{
	char buf[4];

	std::cin.read(buf, 4);
	if (std::cin.fail()) throw ParseError(LOCATION);
	return (buf[3] << 24) | (buf[2] << 16) | (buf[1] << 8) | buf[0];  // Little endian
}


inline Uint16 readInt16() EXCEPTIONS
{
	char buf[2];

	std::cin.read(buf, 2);
	if (std::cin.fail()) throw ParseError(LOCATION);
	return (buf[1] << 8) | buf[0];
}


inline void readRaw(char * buffer, std::streamsize length) EXCEPTIONS
{
	std::cin.read(buffer, length);
	if (std::cin.fail()) throw ParseError(LOCATION);
}


inline void skip(int numBytes) EXCEPTIONS
{
	std::cin.ignore(numBytes);
	if (std::cin.fail()) throw ParseError(LOCATION);
}


inline std::streamoff bytesAvailable() EXCEPTIONS
{
	std::streampos currentPosition = std::cin.tellg();
	std::cin.seekg(0, std::ios::end);
	std::streampos endPosition = std::cin.tellg();
	std::cin.seekg(currentPosition);
	
	if (std::cin.fail()) throw ParseError(LOCATION);
	return endPosition - currentPosition;
}


#endif  // WAV_IO_H

// Local Variables:
// mode:C++
// End:

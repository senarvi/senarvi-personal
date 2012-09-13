//  new.h
//  Copyright (C) 1999-2000 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef SYSTEM_NEW_H
#define SYSTEM_NEW_H


#include <cstdlib>

#include "system/debug.h"
#include "system/exception.h"


inline void* operator new (std::size_t size) throw (OutOfMemoryException) {

	void* ptr = std::malloc(size);
	if (ptr == NULL) throw OutOfMemoryException(LOCATION);
	return ptr;
}


inline void* operator new[] (std::size_t size) throw (OutOfMemoryException) {

	void* ptr = std::malloc(size);
	if (ptr == NULL) throw OutOfMemoryException(LOCATION);
	return ptr;
}


inline void* operator new(std::size_t size, void* place) throw (OutOfMemoryException) {

	void* ptr = std::realloc(place, size);
	if (ptr == NULL) throw OutOfMemoryException(LOCATION);
	return ptr;
}


inline void* operator new[](std::size_t size, void* place) throw (OutOfMemoryException) {

	void* ptr = std::realloc(place, size);
	if (ptr == NULL) throw OutOfMemoryException(LOCATION);
	return ptr;
}


#endif  // SYSTEM_NEW_H

// Local Variables:
// mode:C++
// End:

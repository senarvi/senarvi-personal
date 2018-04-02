//  types.h
//  Copyright (C) 1999-2001 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef SYSTEM_TYPES_H
#define SYSTEM_TYPES_H


#include <stdint.h>


//
// Primitive data types
//

typedef float Float32;
typedef double Float64;
typedef long double Float80;


//
// NumBits trait class
//

template <typename T>
struct NumBits {};


struct NumBits<int8_t> {
	static const int value = 8;
};


struct NumBits<int16_t> {
	static const int value = 16;
};


struct NumBits<int32_t> {
	static const int value = 32;
};


struct NumBits<int64_t> {
	static const int value = 64;
};


struct NumBits<uint8_t> {
	static const int value = 8;
};


struct NumBits<uint16_t> {
	static const int value = 16;
};


struct NumBits<uint32_t> {
	static const int value = 32;
};


struct NumBits<uint64_t> {
	static const int value = 64;
};


struct NumBits<Float32> {
	static const int value = 32;
};


struct NumBits<Float64> {
	static const int value = 64;
};


struct NumBits<Float80> {
	static const int value = 80;
};


#endif  // SYSTEM_TYPES_H

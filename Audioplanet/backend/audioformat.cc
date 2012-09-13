//  audioformat.cc
//  Copyright (C) 1999-2003 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#include "backend/audioformat.h"
#include "backend/base/debug.h"

namespace ap
{

	unsigned int AudioFormat::sampleSize() const
	{
		trace();
	
	
		switch (sampleFormat())
		{
		case AudioFormat::INT_16: return 2;
		case AudioFormat::INT_32:
		case AudioFormat::FLOAT_32: return 4;
		case AudioFormat::FLOAT_64: return 8;
		}
		assert(false);
	}

}  // namespace AP

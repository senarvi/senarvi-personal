//  esoundinputstream.cc
//  Copyright (C) 2001 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifdef __GNUG__
#pragma implementation
#endif


#include <unistd.h>

#include "esoundinputstream.h"


int EsoundInputStream::getSocket() EXCEPTIONS
{
	return ::esd_record_stream(formatIdentifier(),
	                           audioFormat().sampleRate(),
	                           hostName().c_str(),
	                           "Audioplanet");
}


void EsoundInputStream::read(void * buffer, std::size_t numSamples) const EXCEPTIONS
{
	::read(fd(), buffer, numChannels() * numSamples * audioFormat().sampleSize());
}

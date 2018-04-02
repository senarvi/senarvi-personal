//  esoundoutputstream.cc
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

#include "esoundoutputstream.h"


int EsoundOutputStream::getSocket() EXCEPTIONS
{
	return ::esd_play_stream(formatIdentifier(),
	                         audioFormat().sampleRate(),
	                         hostName().c_str(),
	                         "Audioplanet");
}


void EsoundOutputStream::write(const void * buffer, std::size_t numSamples) const EXCEPTIONS
{
	::write(fd(), buffer, numChannels() * numSamples * audioFormat().sampleSize());
}

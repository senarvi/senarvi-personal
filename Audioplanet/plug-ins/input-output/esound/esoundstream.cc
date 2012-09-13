//  esoundstream.cc
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
#include <esd.h>

#include "esoundstream.h"


void EsoundStream::setAudioFormat(const AP::AudioFormat & audioFormat) EXCEPTIONS
{
	if (audioFormat.sampleFormat() != AP::AudioFormat::INT_16) throw InvalidArgumentException(LOCATION);
	audioFormat_ = audioFormat;
}


void EsoundStream::setNumChannels(unsigned int numChannels) EXCEPTIONS
{
	if (numChannels == 0) throw InvalidArgumentException(LOCATION);
	if (numChannels > 2) throw InvalidArgumentException(LOCATION);


	numChannels_ = numChannels;
}


void EsoundStream::open() EXCEPTIONS
{
	formatIdentifier_ = ESD_BITS16 | ESD_STREAM;

	if (numChannels() == 1) formatIdentifier_ |= ESD_MONO;
	else formatIdentifier_ |= ESD_STEREO;

	if (direction() == input) formatIdentifier_ |= ESD_RECORD;
	else formatIdentifier_ |= ESD_PLAY;


	fd_ = getSocket();
}


void EsoundStream::close() EXCEPTIONS
{
	::close(fd_);
}

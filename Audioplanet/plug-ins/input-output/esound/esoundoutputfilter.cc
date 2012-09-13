//  esoundinputfilter.cc
//  Copyright (C) 1999-2001 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifdef __GNUG__
#pragma implementation
#endif


#include "esoundoutputfilter.h"
#include "libaudioplanet/packet.h"


AP::Filter * EsoundOutputFilter::create(const AP::AudioFormat & audioFormat) EXCEPTIONS
{
	trace();
	
	
	return new EsoundOutputFilter();
}


void EsoundOutputFilter::handleFormat(const AP::AudioFormat & audioFormat) EXCEPTIONS
{
}


void EsoundOutputFilter::handlePacket(const std::string & channelId, const AP::IPacket & packet) EXCEPTIONS
{
//	stream_.write(packet->voidData(), 2 * packet->length());
}


void EsoundOutputFilter::run() EXCEPTIONS
{
	while (true)
	{
		if (!readPacket()) break;
		testStop();
	}
}

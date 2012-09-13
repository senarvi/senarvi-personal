//  esoundinputfilter.h
//  Copyright (C) 1999-2001 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef ESOUNDINPUTFILTER_H
#define ESOUNDINPUTFILTER_H

#ifdef __GNUG__
#pragma interface
#endif


#include "esoundoutputstream.h"
#include "libaudioplanet/outputfilter.h"


class EsoundOutputFilter : public AP::OutputFilter
{
public:

	static AP::Filter * create(const AP::AudioFormat & audioFormat) EXCEPTIONS;
	
	void initialize() EXCEPTIONS;
	
	unsigned int numChannels() const EXCEPTIONS;
	
	// Implementation of the run method defined in Thread class
	void run() EXCEPTIONS;
	
	
private:
	
	void handleFormat(const AP::AudioFormat & audioFormat) EXCEPTIONS;
	void handlePacket(const std::string & channelId, const AP::IPacket & packet) EXCEPTIONS;
	
	
	EsoundOutputStream stream_;
	
};


//
// EsoundOutputFilter class Inline methods
//


inline unsigned int EsoundOutputFilter::numChannels() const EXCEPTIONS
{
	return 2;
}


#endif  // ESOUDNINPUTFILTER_H

// Local Variables:
// mode:C++
// End:


//  libaudioplanet/outputfilter.h
//  Copyright (C) 1999-2001 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef LIBAUDIOPLANET_OUTPUTFILTER_H
#define LIBAUDIOPLANET_OUTPUTFILTER_H

#ifdef __GNUG__
#pragma interface
#endif


#include "libaudioplanet/filter.h"
#include "libaudioplanet/consumer.h"


namespace AP {


class OutputFilter : public Filter, public Consumer
{
public:

	virtual ~OutputFilter() EXCEPTIONS {}
	
	// The default implementations allow the number of input pins to be set to
	// anything from 0 to numChannels. The number of output pins is always 0.
	virtual unsigned int minInputPins() const EXCEPTIONS;
	virtual unsigned int maxInputPins() const EXCEPTIONS;
	unsigned int minOutputPins() const EXCEPTIONS;
	unsigned int maxOutputPins() const EXCEPTIONS;
	
	
private:
	
	virtual unsigned int numChannels() const EXCEPTIONS = 0;
};


//
// Inline methods
//


inline unsigned int OutputFilter::minInputPins() const EXCEPTIONS
{
	return 0;
}


inline unsigned int OutputFilter::maxInputPins() const EXCEPTIONS
{
	return numChannels();
}


inline unsigned int OutputFilter::minOutputPins() const EXCEPTIONS
{
	return 0;
}


inline unsigned int OutputFilter::maxOutputPins() const EXCEPTIONS
{
	return 0;
}


}  // namespace AP


#endif  // LIBAUDIOPLANET_OUTPUTFILTER_H

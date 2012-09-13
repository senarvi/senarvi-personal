//  libaudioplanet/inputfilter.h
//  Copyright (C) 1999-2001 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef LIBAUDIOPLANET_INPUTFILTER_H
#define LIBAUDIOPLANET_INPUTFILTER_H

#ifdef __GNUG__
#pragma interface
#endif


#include "libaudioplanet/filter.h"
#include "libaudioplanet/producer.h"


namespace AP {


class InputFilter : public Filter, public Producer
{
public:
	
	virtual ~InputFilter() EXCEPTIONS {}
	
	// The default implementations allow the number of output pins to be set to
	// anything from 0 to numChannels. The number of input pins is always 0.
	unsigned int minInputPins() const EXCEPTIONS;
	unsigned int maxInputPins() const EXCEPTIONS;
	virtual unsigned int minOutputPins() const EXCEPTIONS;
	virtual unsigned int maxOutputPins() const EXCEPTIONS;
	
	
private:
	
	virtual unsigned int numChannels() const EXCEPTIONS = 0;
};


//
// Inline methods
//


inline unsigned int InputFilter::minInputPins() const EXCEPTIONS
{
	return 0;
}


inline unsigned int InputFilter::maxInputPins() const EXCEPTIONS
{
	return 0;
}


inline unsigned int InputFilter::minOutputPins() const EXCEPTIONS
{
	return 0;
}


inline unsigned int InputFilter::maxOutputPins() const EXCEPTIONS
{
	return numChannels();
}


}  // namespace AP


#endif  // LIBAUDIOPLANET_INPUTFILTER_H

// Local Variables:
// mode:C++
// End:

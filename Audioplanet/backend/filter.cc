//  filter.cc
//  Copyright (C) 1999-2003 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifdef __GNUG__
#pragma implementation
#endif


#include "backend/filter.h"
#include "backend/packet.h"
#include "backend/base/debug.h"
#include "backend/base/exceptions.h"


namespace ap {


    void Filter::setNumInputPins(unsigned int numInputPins)
    {
	if (numInputPins > maxInputPins())
	    throw InvalidArgumentException(THIS_LOCATION);
	if (numInputPins < minInputPins())
	    throw InvalidArgumentException(THIS_LOCATION);

	
	numInputPins_ = numInputPins;
    }


    void Filter::setNumOutputPins(unsigned int numOutputPins)
    {
	if (numOutputPins > maxOutputPins())
	    throw InvalidArgumentException(THIS_LOCATION);
	if (numOutputPins < minOutputPins())
	    throw InvalidArgumentException(THIS_LOCATION);
	
	
	numOutputPins_ = numOutputPins;
    }


    unsigned int Filter::numInputPins() const
    {
	return numInputPins_;
    }


    unsigned int Filter::numOutputPins() const
    {
	return numOutputPins_;
    }


}  // namespace AP

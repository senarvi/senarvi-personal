//  esoundinputstream.h
//  Copyright (C) 2001 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef ESOUNDINPUTSTREAM_H
#define ESOUNDINPUTSTREAM_H

#ifdef __GNUG__
#pragma interface
#endif


#include "esoundstream.h"


class EsoundInputStream : public EsoundStream {

public:

	virtual ~EsoundInputStream() throw () {}


	int getSocket() EXCEPTIONS;
	void read(void * buffer, std::size_t numSamples) const EXCEPTIONS;


private:

	Direction direction() const throw ();

};


//
// Inline methods
//


inline EsoundStream::Direction EsoundInputStream::direction() const throw ()
{
	return EsoundStream::input;
}


#endif  // ESOUNDINPUTSTREAM_H

// Local Variables:
// mode:C++
// End:


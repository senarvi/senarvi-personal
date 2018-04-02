//  esoundoutputstream.h
//  Copyright (C) 2001 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef ESOUNDOUTPUTSTREAM_H
#define ESOUNDOUTPUTSTREAM_H

//#ifdef __GNUG__
//#pragma interface
//#endif


#include "esoundstream.h"


class EsoundOutputStream : public EsoundStream {

public:

	virtual ~EsoundOutputStream() throw () {}


	int getSocket() EXCEPTIONS;
	void write(const void * buffer, std::size_t numSamples) const EXCEPTIONS;


private:

	Direction direction() const throw ();

};


//
// Inline methods
//


inline EsoundStream::Direction EsoundOutputStream::direction() const throw ()
{
	return EsoundStream::output;
}


#endif  // ESOUNDOUTPUTSTREAM_H

// Local Variables:
// mode:C++
// End:


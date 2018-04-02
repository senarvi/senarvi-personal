//  esoundstream.h
//  Copyright (C) 2001 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef ESOUNDSTREAM_H
#define ESOUNDSTREAM_H

//#ifdef __GNUG__
//#pragma interface
//#endif


#include <esd.h>

#include "libaudioplanet/audioformat.h"


class EsoundStream {

public:

	inline EsoundStream() throw ();
	virtual ~EsoundStream() throw () {}

	void setAudioFormat(const AP::AudioFormat & audioFormat) EXCEPTIONS;
	void setNumChannels(unsigned int numChannels) EXCEPTIONS;

	void open() EXCEPTIONS;   // Open a socket for playing or recording
	void close() EXCEPTIONS;  // Close the socket


protected:

	enum Direction {
		input,
		output
	};


	inline const AP::AudioFormat & audioFormat() const throw ();
	inline unsigned int numChannels() const throw ();
	inline ::esd_format_t formatIdentifier() const throw ();

	inline const std::string & hostName() const throw ();
	inline int fd() const throw ();


private:

	virtual Direction direction() const throw () = 0;
	virtual int getSocket() EXCEPTIONS = 0;


	AP::AudioFormat audioFormat_;
	unsigned int numChannels_;
	::esd_format_t formatIdentifier_;

	std::string hostName_;
	int fd_;

};


//
// Inline methods
//


inline EsoundStream::EsoundStream() throw () :

	hostName_("localhost") {}


inline const AP::AudioFormat & EsoundStream::audioFormat() const throw ()
{
	return audioFormat_;
}


inline unsigned int EsoundStream::numChannels() const throw ()
{
	return numChannels_;
}


inline ::esd_format_t EsoundStream::formatIdentifier() const throw ()
{
	return formatIdentifier_;
}


inline const std::string & EsoundStream::hostName() const throw ()
{
	return hostName_;
}


inline int EsoundStream::fd() const throw ()
{
	return fd_;
}


#endif  // ESOUNDSTREAM_H

// Local Variables:
// mode:C++
// End:

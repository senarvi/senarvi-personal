//  libaudioplanet/consumer.h
//  Copyright (C) 2001-2003 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef LIBAUDIOPLANET_CONSUMER_H
#define LIBAUDIOPLANET_CONSUMER_H

#ifdef __GNUG__
#pragma interface
#endif


#include <string>
#include <stdexcept>

#include "backend/messagetype.h"


namespace ap
{

    class AudioFormat;
    class PacketBase;


    class Consumer
    {
    public:
	class NoInputStreamException : public std::logic_error
	{
	public:
	    NoInputStreamException() :
		std::logic_error("setInputStream was not called")
	    {}
	};

	class MessageTypeException : public std::logic_error
	{
	public:
	    MessageTypeException(const MessageType & messageType) :
		std::logic_error("invalid message type"),
		messageType_(messageType)
	    {}
	private:
	    const MessageType & messageType_;
	};

	
	Consumer() throw ();
	virtual ~Consumer();
	
	void setInputStream(std::istream & inputStream) throw ();
	
    protected:
	bool readPacket();
	
    private:
	virtual void handlePacket(const std::string & channelId,
				  const PacketBase & packet) = 0;

	std::istream & inputStream();
	
	std::istream * inputStream_;
    };


//	extern void readFormat(const File & inputFile, AudioFormat & audioFormat);

}  // namespace ap


#endif  // LIBAUDIOPLANET_CONSUMER_H

// Local Variables:
// mode:C++
// End:

//  libaudioplanet/consumer.cc
//  Copyright (C) 2001-2003 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifdef __GNUG__
#pragma implementation
#endif


#include "boost/lexical_cast.hpp"

#include "backend/consumer.h"
#include "backend/packet.h"
#include "backend/base/file.h"


using namespace std;


namespace ap {


    Consumer::Consumer() throw () :
	inputStream_(0)
    {
    }


    Consumer::~Consumer()
    {
    }


    void Consumer::setInputStream(std::istream & inputStream) throw ()
    {
	inputStream_ = &inputStream;
    }


    istream & Consumer::inputStream()
    {
	if (inputStream_ == 0)
	    throw new NoInputStreamException();
	return *inputStream_;
    }


    bool Consumer::readPacket()
    {
	trace();
	
	
	MessageType messageType;
	unsigned int numChannels;
	std::string channelId;
	PacketBase * packet = 0;
	
	
	inputStream() >> messageType;
	switch (messageType)
	{
	case INCOMING_PACKETS:
	    inputStream() >> numChannels;
	    for (unsigned int channel = 0; channel < numChannels; channel++)
	    {
		inputStream() >> channelId;
		inputStream() >> packet;
		handlePacket(channelId, *packet);
		delete packet;
	    }
	    return true;
		
	case END_OF_STREAM:
	    return false;

	default:
	    throw MessageTypeException(messageType);
	}
    }


/*    void readFormat(const File & inputFile, AudioFormat & audioFormat)
    {
	trace();
	
	
	MessageType messageType;
	inputFile >> messageType;
	if (messageType == INCOMING_FORMAT)
	    inputFile >> audioFormat;
	else
	    throw MessageTypeException(LOCATION, messageType);
	    }*/


}  // namespace AP

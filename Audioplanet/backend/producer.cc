//  libaudioplanet/producer.cc
//  Copyright (C) 1999-2001 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifdef __GNUG__
#pragma implementation
#endif


#include "audioplanet/producer.h"
#include "audioplanet/messagetype.h"
#include "audioplanet/audioformat.h"
#include "audioplanet/packet.h"
#include "system/file.h"
#include "system/debug.h"


namespace AP {


Producer::Producer() throw () :

	outputFile_(0)

{}


Producer::~Producer() EXCEPTIONS
{
	if (outputFile_ != 0)
	{
		outputFile().close();
		delete outputFile_;
	}
}


// The descriptor will be closed when the Producer is deleted
void Producer::setOutputDescriptor(int descriptor) EXCEPTIONS
{
	if (outputFile_ != 0) throw AssertionException(LOCATION);
	outputFile_ = new File(descriptor);
}


File & Producer::outputFile() const EXCEPTIONS
{
	if (outputFile_ == 0) throw AssertionException(LOCATION);
	return *outputFile_;
}


void Producer::sendPackets(const Producer::Packets & packets) const EXCEPTIONS
{
	trace();
	
	
	MessageType messageType(INCOMING_PACKETS);
	outputFile() << messageType;
	unsigned int numChannels = packets.size();
	outputFile() << numChannels;
	
	Packets::const_iterator iter;
	for (iter = packets.begin(); iter != packets.end(); iter++)
	{
		const std::string & channelId = (*iter).first;
		IPacket * packetPtr = (*iter).second;
		outputFile() << channelId << *packetPtr;
	}
}


void Producer::sendEndOfStream() const EXCEPTIONS
{
	trace();
	
	
	MessageType messageType(END_OF_STREAM);
	outputFile() << messageType;
}


void sendFormat(const File & outputFile, const AP::AudioFormat & audioFormat) EXCEPTIONS
{
	trace();
	
	
	MessageType messageType(INCOMING_FORMAT);
	outputFile << messageType;
	outputFile << audioFormat;
}


}  // namespace AP

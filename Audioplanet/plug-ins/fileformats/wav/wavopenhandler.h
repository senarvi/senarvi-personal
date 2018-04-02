//  wavopenhandler.h
//  Copyright (C) 1999-2002 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef WAVOPENHANDLER_H
#define WAVOPENHANDLER_H


#include <cstdio>
#include <algorithm>
#include <limits>

#include <unistd.h>

#include "wavformat.h"
#include "audioplanet/inputfilter.h"
#include "audioplanet/conversion.h"
#include "audioplanet/packet.h"
#include "system/exception.h"
#include "system/debug.h"
#include "system/log.h"


template <typename T>
class WavOpenHandler : public AP::InputFilter
{
public:
	
	WavOpenHandler() EXCEPTIONS;
	
	
	static AP::Filter * create(AP::AudioFormat & audioFormat) EXCEPTIONS;
	static void getFormat(AP::AudioFormat & audioFormat) EXCEPTIONS;
	void initialize() EXCEPTIONS;
	
	inline unsigned int numChannels() const EXCEPTIONS;
	
	
private:
	
	void run() EXCEPTIONS;
	
	
	unsigned int numChannels_;
	unsigned int blockAlignment_;
	unsigned int inputSampleSize_;
	
	std::streampos dataStart_;
	std::streampos dataEnd_;
	
	bool initialized_;
};


template <class T>
inline WavOpenHandler<T>::WavOpenHandler() EXCEPTIONS :

	initialized_(false)
	
{}


template <class T>
inline void WavOpenHandler<T>::initialize() EXCEPTIONS
{
	trace();
	
	
	Uint32 magic = readMagic();
	if (magic != MAGIC_FMT) throw AssertionException(LOCATION);
	
	Uint32 length = readInt32();
	length = length + (length % 2);  // Even padding
	
	
	Header header;
	length = header.read(length);
	if (length > 0) skip(length);
	
	
	do
	{
		Uint32 magic = readMagic();
		
		length = readInt32();
		length = length + (length % 2);
		
		if (magic == MAGIC_DATA) break;
		
		skip(length);
	}
	while (true);
	
	
	numChannels_ = header.numChannels();
	blockAlignment_ = header.blockAlign();
	inputSampleSize_ = header.bitsPerSample();
	
	
	dataStart_ = std::cin.tellg();
	dataEnd_ = dataStart_ + static_cast<std::streamoff>(length);
	
	
	root.debug("Data start: " + boost::lexical_cast<std::string>(dataStart_));
	root.debug("Data end: " + boost::lexical_cast<std::string>(dataEnd_));
	root.debug("Block alignment: " + boost::lexical_cast<std::string>(blockAlignment_));
	
	
	initialized_ = true;
}


template <typename T>
inline void WavOpenHandler<T>::run() EXCEPTIONS
{
	trace();
	
	
	setOutputDescriptor(STDOUT_FILENO);
	
	while (true)
	{
		std::string channelId = "L";  // Currently ignores other channels
		
		
		Uint64 samplesLeft = (dataEnd_ - std::cin.tellg()) / blockAlignment_;
		if (samplesLeft <= 0)
		{
			// No more data left
			sendEndOfStream();
			return;
		}
		
		
		// Decide packet size, also check if there's enough data available in the stream
		
		Uint64 packetSize = std::min(static_cast<Uint64>(1024), samplesLeft);
		Uint64 inBufferSize = packetSize * blockAlignment_;
		
		std::streamoff dataAvailable = bytesAvailable();
		if (inBufferSize > static_cast<Uint64>(dataAvailable))
		{
			if (dataAvailable <= 0)
			{
				// No more data available in the input stream
				root.warn("WavOpenHandler: Unexpected end of file encountered");
				sendEndOfStream();
				return;
			}
			
			packetSize = dataAvailable / blockAlignment_;
			inBufferSize = packetSize * blockAlignment_;
		}
		
		
		AP::TdPacket<T> * packet = new AP::TdPacket<T>(44100, packetSize);
		T * outBuffer = packet->data();
		char inBuffer[inBufferSize];
		readRaw(inBuffer, inBufferSize);
		
		
		if (std::numeric_limits<T>::is_integer)
		{
/*			AP::Interpretation::LittleEndian::IntegerInterpreter<T> interpreter;
			if (inputSampleSize_ == 8)
				interpreter.initialize(inputSampleSize_, false);
			else
				interpreter.initialize(inputSampleSize_, true);
			
			
			for (Uint64 i = 0; i < packetSize; i++)
				outBuffer[i] = interpreter.read(inBuffer + i * blockAlignment_);*/
			for (Uint64 i = 0; i < packetSize; i++)
				outBuffer[i] = *reinterpret_cast<T *>(inBuffer + i * blockAlignment_);
		}
		else
		{
			for (Uint64 i = 0; i < packetSize; i++)
				outBuffer[i] = *reinterpret_cast<T *>(inBuffer + i * blockAlignment_);
		}
		
		
		Packets packets;
		packets.insert(Packets::value_type(channelId, packet));
		sendPackets(packets);
	}
}


template <class T>
inline unsigned int WavOpenHandler<T>::numChannels() const EXCEPTIONS
{
	if (!initialized_) throw AssertionException(LOCATION);
	
	
	return numChannels_;
}


#endif  // WAVOPENHANDLER_H

// Local Variables:
// mode:C++
// End:

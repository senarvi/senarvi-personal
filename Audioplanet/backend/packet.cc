//  backend/packet.cc
//  Copyright (C) 1999-2003 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#include "backend/packet.h"
//#include "backend/base/file.h"


namespace ap {


/*void PacketBase::setLength(Length lengthArg)
{
	length_ = lengthArg;
	reallocate();
}*/

/*
const File & operator>>(const File & file, AP::IPacket * packetPtr)
{
	AP::IPacket::Length length;
	file >> length;
	
	
	root.debug("Reading packet. Length: " + boost::lexical_cast<std::string>(length));
	
	
	AP::AudioFormat audioFormat;
	file >> audioFormat;
	if (audioFormat.domain() == AP::AudioFormat::TIME_DOMAIN)
	{
		switch (audioFormat.sampleFormat())
		{
		case AP::AudioFormat::INT_16:
			packetPtr = new AP::TdPacket<Int16>(audioFormat.sampleRate(), length);
			break;
		
		case AP::AudioFormat::INT_32:
			packetPtr = new AP::TdPacket<Int32>(audioFormat.sampleRate(), length);
			break;
		
		case AP::AudioFormat::FLOAT_32:
			packetPtr = new AP::TdPacket<Float32>(audioFormat.sampleRate(), length);
			break;
		
		case AP::AudioFormat::FLOAT_64:
			packetPtr = new AP::TdPacket<Float64>(audioFormat.sampleRate(), length);
			break;
			
		default:
			throw FlowOfControlException(LOCATION);
		}
		file.read(packetPtr->voidData(), length);
	}
	else
	{
		throw AssertionException(LOCATION);
	}
	return file;
}


const File & operator<<(const File & file, const AP::IPacket & packet)
{
	root.debug("Writing packet. Length: " + boost::lexical_cast<std::string>(packet.length()));
	
	
	file << packet.length();
	file << packet.audioFormat();
	file.write(packet.voidData(), packet.length());
	
	
	return file;
}
*/

}  // namespace ap

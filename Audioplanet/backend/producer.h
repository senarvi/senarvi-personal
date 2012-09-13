//  libaudioplanet/producer.h
//  Copyright (C) 1999-2001 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef LIBAUDIOPLANET_PRODUCER_H
#define LIBAUDIOPLANET_PRODUCER_H

#ifdef __GNUG__
#pragma interface
#endif


#include <string>
#include <map>


namespace ap {


class AudioFormat;
class PacketBase;


class Producer
{
public:
	
	Producer() throw ();
	virtual ~Producer();
	
	void setOutputDescriptor(int descriptor);
	
	
protected:
	
	typedef std::map<std::string, PacketBase *> Packets;
	
	
	void sendPackets(const Packets & packets) const;
	void sendEndOfStream() const;
	
	
private:
	
//	File & outputFile() const;
	
	
//	File * outputFile_;
};


//extern void sendFormat(const File & outputFile, const AP::AudioFormat & audioFormat);


}  // namespace Audioplanet


#endif  // LIBAUDIOPLANET_PRODUCER_H

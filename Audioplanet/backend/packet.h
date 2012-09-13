//  backend/packet.h
//  Copyright (C) 1999-2003 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef BACKEND_PACKET_H
#define BACKEND_PACKET_H


#include <istream>
#include <ostream>
#include <vector>

#include "backend/audioformat.h"
#include "backend/base/debug.h"


namespace ap
{

	class PacketBase
	{
	protected:
		PacketBase(AudioFormat & audioFormat);
	
	public:
		virtual ~PacketBase() {}
	
		AudioFormat & audioFormat() const { return audioFormat_; }
	
	private:
/*		friend const File & operator>>(const File & file, PacketBase * packetPtr);
		friend const File & operator<<(const File & file, const PacketBase & x);*/

		AudioFormat & audioFormat_;
	};


	template <typename T, typename Sequence = std::vector<std::vector<T> *> >
	class Packet : public PacketBase
	{
	public:
		typedef typename Sequence::value_type value_type;
		typedef typename Sequence::size_type size_type;
		typedef Sequence container_type;


		Packet(AudioFormat::SampleRate sampleRate);
	
		size_type numChannels() const { return c_.size(); }
		value_type data(size_type position) const;
	
	private:
		container_type c_;
	};


	inline PacketBase::PacketBase(AudioFormat & audioFormat) :
	
		audioFormat_(audioFormat)
	
	{}


	template <typename T, typename Sequence>
	inline Packet<T, Sequence>::Packet(AudioFormat::SampleRate sampleRate) :

		PacketBase(*new AudioFormat(SampleInfo<T>::sampleFormat, sampleRate, AudioFormat::TIME_DOMAIN))
	
	{}


	template <typename T, typename Sequence>
	inline typename Packet<T, Sequence>::value_type
	Packet<T, Sequence>::data(Packet<T, Sequence>::size_type position) const
	{
		return c_.at(position);
	}


/*	extern const File & operator>>(const File & file, PacketBase * packetPtr);
	extern const File & operator<<(const File & file, const PacketBase & packet);*/
	extern std::istream & operator>>(std::istream & source,
					 PacketBase * packetPtr);
	extern std::ostream & operator<<(std::ostream & destination,
					 const PacketBase & packet);

}  // namespace ap


#endif  // BACKEND_PACKET_H

// Local Variables:
// mode:C++
// End:

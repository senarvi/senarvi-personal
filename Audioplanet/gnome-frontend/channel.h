//  channel.h
//  Copyright (C) 1999-2002 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef CHANNEL_H
#define CHANNEL_H


#include <list>

#include "audiowindow.h"
#include "soundlevel.h"
#include "libaudioplanet/audioformat.h"
#include "libaudioplanet/packet.h"
#include "system/types.h"


class IChannel
{
public:
	
	struct EventListener
	{
		virtual ~EventListener() EXCEPTIONS {}
		virtual void atChange() EXCEPTIONS = 0;
	};
	
	
	IChannel() throw ();
	virtual ~IChannel() throw () {}
	
	
	Size length() const EXCEPTIONS;
	bool empty() const EXCEPTIONS;
	void addLength(Size quantity) EXCEPTIONS;
	void subtractLength(Size quantity) EXCEPTIONS;
	
	virtual AP::AudioFormat audioFormat() const EXCEPTIONS = 0;
	
	virtual void insert(const AP::IPacket & packet) EXCEPTIONS = 0;
	virtual void erase(Size eraseLength) EXCEPTIONS = 0;
	
	
private:
	
	Size length_;  // Number of samples
	
	
	virtual void reallocate() EXCEPTIONS = 0;
};


template <typename T>
class TdChannel : public IChannel, public GUI::TdClipWindow::Channel
{
public:

	TdChannel(EventListener & eventListener) EXCEPTIONS;
	virtual ~TdChannel() throw () {}
	
	
	AP::AudioFormat audioFormat() const EXCEPTIONS;
	
	void insert(const AP::IPacket & packet) EXCEPTIONS;
	void erase(Size eraseLength) EXCEPTIONS;
	
	Size cursor() EXCEPTIONS;
	
	T operator[](Size position) const EXCEPTIONS;
	
	
	
	SoundLevelRange<T> levelRange(
		Size sectionStart,
		Size sectionLength) const EXCEPTIONS;
	
	void getLevelRange(
		Size sectionStart,
		Size sectionLength,
		Float64 & minLevel,
		Float64 & maxLevel) const EXCEPTIONS;
	
	
private:
	
	void insert(const AP::IPacket & packet, Size insertPosition) EXCEPTIONS;
	void erase(Size erasePosition, Size eraseLength) EXCEPTIONS;
	
	void reallocate() EXCEPTIONS;
	
	
	EventListener & eventListener_;
	AP::AudioFormat audioFormat_;
	T * data_;
	Size cursor_;
};


//
// IChannel inline methods
//


inline IChannel::IChannel() throw () :

	length_(0)

{}


inline Size IChannel::length() const EXCEPTIONS
{
	return length_;
}


inline bool IChannel::empty() const EXCEPTIONS
{
	return length() == 0;
}


inline void IChannel::addLength(Size quantity) EXCEPTIONS
{
	length_ += quantity;
	reallocate();
}


inline void IChannel::subtractLength(Size quantity) EXCEPTIONS
{
	if (length() < quantity) throw AssertionException(LOCATION);
	
	
	length_ -= quantity;
	reallocate();
}


//
// TdChannel class
//


template <typename T>
inline TdChannel<T>::TdChannel(IChannel::EventListener & eventListener) EXCEPTIONS :

	eventListener_(eventListener),
	data_(NULL),
	cursor_(0)

{
	trace();
	
	
	audioFormat_.setDomain(AP::AudioFormat::TIME_DOMAIN);
	audioFormat_.setSampleFormat(AP::SampleInfo<T>::sampleFormat);
	reallocate();
}


template <typename T>
inline T TdChannel<T>::operator[](Size position) const EXCEPTIONS
{
#ifdef _DEBUG
	if (data_ == NULL) throw EmptyException(LOCATION);
#endif


	return data_[position];
}


template <typename T>
inline SoundLevelRange<T> TdChannel<T>::levelRange(Size sectionStart, Size sectionLength) const EXCEPTIONS
{
	SoundLevelRange<T> result;


	for (Size i = sectionStart; i < std::min(sectionStart + sectionLength, length()); i++)
		result.add(data_[i]);

	return result;
}


template <typename T>
void TdChannel<T>::reallocate() EXCEPTIONS
{
	if (length() == 0)
	{
		if (data_ != NULL) {
			delete data_;
			data_ = NULL;
		}
		return;
	}
	
	if (data_ == NULL) data_ = new T[length()];
	else data_ = static_cast<T *>(std::realloc(data_, length() * sizeof(T)));
	
	if (data_ == NULL) throw OutOfMemoryException(LOCATION);
}


template <typename T>
void TdChannel<T>::insert(const AP::IPacket & packet, Size insertPosition) EXCEPTIONS
{
	if (insertPosition > length()) throw AssertionException(LOCATION);

	try
	{
		const AP::TdPacket<T> & castedPacket = dynamic_cast<const AP::TdPacket<T> &>(packet);  // *** Will SIGSEGV ***
		const T * packetData = castedPacket.data();
		Size packetLength = static_cast<Size>(castedPacket.length());
		
		addLength(packetLength);  // addLength will ensure that we have enough memory allocated.

		Size moveLength = length() - insertPosition;  // Number of samples that has to be moved forward.
		std::memmove(
			data_ + insertPosition + packetLength,
			data_ + insertPosition,
			moveLength * sizeof(T)
		);
		
		std::memcpy(
			data_ + insertPosition,
			packetData,
			packetLength * sizeof(T)
		);
	}
	catch (const std::bad_cast & e)
	{
		root.debug("3");
		throw InvalidArgumentException(LOCATION, "The sample type of packet was incorrect");
	}
}


template <typename T>
void TdChannel<T>::erase(Size erasePosition, Size eraseLength) EXCEPTIONS
{
	trace();
	
	
	if (data_ == NULL) throw AssertionException(LOCATION);
	if (length() < erasePosition + eraseLength) throw InvalidArgumentException(LOCATION);
	
	
	Size moveLength = length() - (erasePosition + eraseLength);  // Number of samples to move backward
	
	std::memmove(
		data_ + erasePosition,
		data_ + erasePosition + eraseLength,
		moveLength * sizeof(T)
	);
	subtractLength(eraseLength);
}


template <typename T>
inline void TdChannel<T>::insert(const AP::IPacket & packet) EXCEPTIONS
{
	trace();
	
	
	insert(packet, cursor());
	cursor_ += packet.length();
	eventListener_.atChange();
}


template <typename T>
inline void TdChannel<T>::erase(Size eraseLength) EXCEPTIONS
{
	trace();
	
	
	if (data_ == NULL) throw AssertionException(LOCATION);
	if (length() < cursor_ + eraseLength) throw InvalidArgumentException(LOCATION);
	
	
	erase(cursor(), eraseLength);
	eventListener_.atChange();
}


template <typename T>
inline AP::AudioFormat TdChannel<T>::audioFormat() const EXCEPTIONS
{
	return audioFormat_;
}


template <typename T>
inline Size TdChannel<T>::cursor() EXCEPTIONS
{
	return cursor_;
}


template <typename T>
inline void TdChannel<T>::getLevelRange(
	Size sectionStart,
	Size sectionLength,
	Float64 & minLevel,
	Float64 & maxLevel) const EXCEPTIONS
	
{
	if (sectionLength < 1) throw AssertionException(LOCATION);
	
	if (sectionStart >= length()) {
		minLevel = 0;
		maxLevel = 0;
		return;
	}
	
	const SoundLevelRange<T> & result = levelRange(sectionStart, sectionLength);
	minLevel = result.min().relative();
	maxLevel = result.max().relative();
}


#endif  // CHANNEL_H

// Local Variables:
// mode:C++
// End:

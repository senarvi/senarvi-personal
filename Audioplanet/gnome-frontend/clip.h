//  clip.h
//  Copyright (C) 1999-2001 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef CLIP_H
#define CLIP_H


#include <string>
#include <list>
#include <map>

#include "audiowindow.h"
#include "channel.h"
#include "selection.h"
#include "clipbookkeeping.h"
#include "libaudioplanet/audioformat.h"
#include "libaudioplanet/producer.h"
#include "libaudioplanet/consumer.h"
#include "system/file.h"
#include "system/exception.h"
#include "system/stlextra.h"


class IClip : public GUI::IClipWindow::EventListener, public IChannel::EventListener
{
public:

	IClip(const std::string & name, const std::string & fileName) throw ();
	virtual ~IClip() EXCEPTIONS {}
	
	
	virtual AP::Producer & producer() EXCEPTIONS = 0;
	virtual AP::Consumer & consumer() EXCEPTIONS = 0;
	
	
	const std::string & name() const EXCEPTIONS;
	const std::string & fileName() const EXCEPTIONS;
	const std::string & title() const EXCEPTIONS;
	
	bool modified() const EXCEPTIONS;
	
	
private:

	std::string name_;
	std::string fileName_;
	
	bool modified_;
};


template <typename T>
class TdClip : public IClip
{
public:

	class Producer : public AP::Producer
	{
	public:
		
		Producer(TdClip & clip) EXCEPTIONS;
		virtual ~Producer() throw () {}
		
		void run() EXCEPTIONS;
		
		
	private:
		
		TdClip & clip_;
	};


	class Consumer : public AP::Consumer
	{
	public:

		Consumer(TdClip & clip) EXCEPTIONS;
		virtual ~Consumer() throw () {}
		
		
	private:
		
		void run() EXCEPTIONS;
		void handlePacket(const std::string & channelId, const AP::IPacket & packet) EXCEPTIONS;
		
		
		TdClip & clip_;
	};


	TdClip(const std::string & name, const std::string & fileName = "") EXCEPTIONS;
	virtual ~TdClip() EXCEPTIONS;


	void atDelete() EXCEPTIONS;
	void atChange() EXCEPTIONS;
	
	
	unsigned int numChannels() const EXCEPTIONS;
	TdChannel<T> & newChannel(const std::string & id) EXCEPTIONS;
	TdChannel<T> & channel(const std::string & id) EXCEPTIONS;
	TdChannel<T> & channel(unsigned int position) EXCEPTIONS;
	
	
	AP::Producer & producer() EXCEPTIONS;
	AP::Consumer & consumer() EXCEPTIONS;
	
	
private:

	typedef std::list<TdChannel<T> *> Channels;
	typedef std::map<std::string, TdChannel<T> *> ChannelMap;


	Channels channels_;
	ChannelMap channelMap_;
	Selection selection_;

	GUI::TdClipWindow window_;

	Producer producer_;
	Consumer consumer_;


	Size length() const EXCEPTIONS;
};


//
// IClip methods
//


inline IClip::IClip(const std::string & name, const std::string & fileName) throw () :

	name_(name),
	fileName_(fileName),
	modified_(false)

{}


inline const std::string & IClip::name() const EXCEPTIONS
{
	return name_;
}


inline const std::string & IClip::fileName() const EXCEPTIONS
{
	return fileName_;
}


inline const std::string & IClip::title() const EXCEPTIONS
{
	static const std::string defaultTitle("Audioplanet");
	
	if (name() != "") return name();
	if (fileName() != "") return fileName();
	return defaultTitle;
}


inline bool IClip::modified() const EXCEPTIONS
{
	return modified_;
}


//
// TdClip Constructors & destructors
//


template <typename T>
inline TdClip<T>::TdClip(const std::string & name, const std::string & fileName) EXCEPTIONS :
	
	IClip(name, fileName),
	window_(*this),
	producer_(*this),
	consumer_(*this)
	
{
	trace();
	
	
	ClipBookkeeping::add(this);
	window_.setTitle(title());
}


template <typename T>
inline TdClip<T>::~TdClip() EXCEPTIONS
{
	trace();
	
	
	window_.hide();
	deleteContents(channels_);
	ClipBookkeeping::remove(this);
}


template <typename T>
inline AP::Producer & TdClip<T>::producer() EXCEPTIONS
{
	return producer_;
}


template <typename T>
inline AP::Consumer & TdClip<T>::consumer() EXCEPTIONS
{
	return consumer_;
}


template <typename T>
inline TdClip<T>::Producer::Producer(TdClip<T> & clip) EXCEPTIONS :

	clip_(clip)
	
{}


template <typename T>
inline TdClip<T>::Consumer::Consumer(TdClip<T> & clip) EXCEPTIONS :

	clip_(clip)
	
{}


template <typename T>
inline Size TdClip<T>::length() const EXCEPTIONS
{
	typename Channels::const_iterator iter;
	Size result = 0;
	
	
	for (iter = channels_.begin(); iter != channels_.end(); iter++)
		result = std::max(result, (*iter)->length());
	
	return result;
}


//
// GUI::DataWindow::EventListener implementations
//


template <typename T>
inline void TdClip<T>::atDelete() EXCEPTIONS
{
	trace();
	ClipBookkeeping::close(this);
}


//
// Channel::EventListener implementations
//


template <typename T>
inline void TdClip<T>::atChange() EXCEPTIONS
{
	window_.configureAudioLength(length());
}


template <typename T>
inline unsigned int TdClip<T>::numChannels() const EXCEPTIONS
{
	trace();
	return channels_.size();
}


template <typename T>
inline TdChannel<T> & TdClip<T>::newChannel(const std::string & id) EXCEPTIONS
{
	trace();
	
	
	TdChannel<T> & createdChannel = *new TdChannel<T>(*this);
	window_.addChannel(createdChannel);
	channels_.push_back(&createdChannel);
	channelMap_.insert(ChannelMap::value_type(id, &createdChannel));
	
	return createdChannel;
}


template <typename T>
inline TdChannel<T> & TdClip<T>::channel(unsigned int position) EXCEPTIONS
{
	trace();
	
	
	if (channels_.empty()) throw InvalidArgumentException(LOCATION);
	
	typename Channels::iterator iter;
	unsigned int count = 0;
	
	for (iter = channels_.begin(); iter != channels_.end(); iter++) {
		if (count == position) return **iter;
		count++;
	}
	
	throw InvalidArgumentException(LOCATION);
}


template <typename T>
inline TdChannel<T> & TdClip<T>::channel(const std::string & id) EXCEPTIONS
{
	trace();
	
	
	typename ChannelMap::iterator iter = channelMap_.find(id);
	if (iter == channelMap_.end()) throw InvalidArgumentException(LOCATION);
	
	TdChannel<T> * ptr = (*iter).second;
	return *ptr;
}


template <typename T>
inline void TdClip<T>::Producer::run() EXCEPTIONS
{
	trace();
	
	
	// NOT IMPLEMENTED
	sendEndOfStream();
}


template <typename T>
inline void TdClip<T>::Consumer::run() EXCEPTIONS
{
	trace();
	
	
	while (true)
	{
		if (!readPacket()) break;
		testStop();
	}
}


template <typename T>
inline void TdClip<T>::Consumer::handlePacket(const std::string & channelId, const AP::IPacket & packet) EXCEPTIONS
{
	trace();
	
	
	try
	{
		TdChannel<T> & destinationChannel = clip_.channel(channelId);
		destinationChannel.insert(packet);
	}
	catch (InvalidArgumentException & e)
	{
		// There's no such channel ID yet
		clip_.newChannel(channelId);
		TdChannel<T> & destinationChannel = clip_.channel(channelId);
		destinationChannel.insert(packet);
	}
}


#endif  // CLIP_H

// Local Variables:
// mode:C++
// End:

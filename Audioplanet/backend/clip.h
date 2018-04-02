//  backend/clip.h
//  Copyright (C) 1999-2003 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef BACKEND_CLIP_H
#define BACKEND_CLIP_H


#include <string>
#include <list>
#include <set>
#include <cstring>

#include <audiofile.h>
#include <sigc++/signal.h>
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>

#include "backend/channel.h"
#include "backend/audioformat.h"
#include "backend/base/exceptions.h"
#include "backend/base/generalfunctors.h"
#include "backend/base/stringcopy.h"


namespace ap
{

    class UnknownFileTypeException : std::exception
    {
    public:
	const char * what() const throw ()
	{
	    return "The type of a file was not recognized.";
	}
    };


    class PacketTypeMismatchException : std::exception
    {
    public:
	const char * what() const throw ()
	{
	    return "Received a packet that was of incorrect type.";
	}
    };


    class ClipBase
    {
    public:

	// Sent after insertion and erasion.
	SigC::Signal0<void> onChange;


	ClipBase(const std::string & fileName);
	virtual ~ClipBase();
	

	static ClipBase & createFromFile(const std::string & fileName);
	void writeToFile();
		
	const std::string & fileName() const { return fileName_; }
	void setFileName(const std::string & x) { fileName_ = x; }
	bool modified() const { return modified_; }
	
	
    private:
	std::string fileName_;
	bool modified_;
	double sampleRate_;


	virtual void readFrames(AFfilehandle fileHandle) = 0;
    };


#define CLIP_ARGS T, ChannelSequence
#define CLIP_HEADER_ARGS typename T, typename ChannelSequence

    template <typename T, typename ChannelSequence = std::vector<Channel<T> *> >
    class Clip : public ClipBase
    {
    public:
	typedef typename ChannelSequence::value_type ch_pointer;
	BOOST_STATIC_ASSERT(boost::is_pointer<ch_pointer>::value);
	typedef typename boost::remove_pointer<ch_pointer>::type ch_value_type;
	typedef ChannelSequence ch_container_type;
	typedef typename ChannelSequence::size_type num_channels_type;
	typedef typename ch_value_type::size_type channel_length_type;


	Clip(const std::string & fileName = "");
	virtual ~Clip();


	void onChange();
	
	
	num_channels_type numChannels() const { return channels_.size(); }
	ch_pointer & createChannel();
	ch_pointer & channel(unsigned int position);

	
    private:
	void readFrames(AFfilehandle fileHandle);

	void consumerRoutine();
	void producerRoutine();
	void handlePacket(const PacketBase & packet);


	ch_container_type channels_;
	channel_length_type cursor_;


	channel_length_type length() const;

	void read();
    };


//
// Constructors & destructors
//


    inline ClipBase::ClipBase(const std::string & fileName) :

	fileName_(fileName)
    {
    }


    inline ClipBase::~ClipBase()
    {
    }


    template <CLIP_HEADER_ARGS>
    inline Clip<CLIP_ARGS>::Clip(const std::string & fileName) :
	
	ClipBase(fileName)
    {
    }


    template <CLIP_HEADER_ARGS>
    inline Clip<CLIP_ARGS>::~Clip()
    {
	std::for_each(channels_.begin(),
		      channels_.end(),
		      Delete<ch_pointer>());
	channels_.clear();
    }


    template <CLIP_HEADER_ARGS>
    inline typename Clip<CLIP_ARGS>::channel_length_type
    Clip<CLIP_ARGS>::length() const
    {
	MaxSize<ch_container_type> maxChannelSize(
	    std::for_each(
		channels_.begin(),
		channels_.end(),
		MaxSize<ch_container_type>()
		)
	    );
	
	return maxChannelSize.value();
    }


    template <CLIP_HEADER_ARGS>
    inline void Clip<CLIP_ARGS>::readFrames(AFfilehandle fileHandle)
    {
	// Frame size should be an integer!
	int frameSize = static_cast<int>(
	    afGetVirtualFrameSize(fileHandle, AF_DEFAULT_TRACK, 1));
	if (frameSize < 0)
	    // afGetVirtualFrameSize returns float -1 in case of an error.
	    throw FileIOException("afGetFrameSize failed", fileName());

	AFframecount totalFrameCount = afGetFrameCount(fileHandle, AF_DEFAULT_TRACK);
	if (totalFrameCount == -1)
	    throw FileIOException("afGetFrameCount failed", fileName());
		
	static const AFframecount chunkFrameCount = 1024;
	void * buffer = std::malloc(frameSize * chunkFrameCount);
	if (buffer == 0)
	    throw std::bad_alloc();

	// I think audiofile means "number of chars" with "number of bytes".
	char * bufferIterator = reinterpret_cast<char *>(buffer);
	while (totalFrameCount > 0)
	{
	    int numFramesRead =
		afReadFrames(fileHandle, AF_DEFAULT_TRACK, buffer, chunkFrameCount);
	    if (numFramesRead == -1)
		throw FileIOException("afReadFrames failed", fileName());

	    for (AFframecount frameIndex = 0;
		 frameIndex != numFramesRead;
		 ++frameIndex)
	    {
		T * frame = reinterpret_cast<T *>(bufferIterator);

		for (unsigned int channelIndex = 0;
		     channelIndex != numChannels();
		     ++channelIndex)
		{
		    channel(channelIndex)->push_back(frame[channelIndex]);
		}

		bufferIterator += frameSize;
	    }

	    totalFrameCount -= numFramesRead;
	}

	std::free(buffer);
    }


//
// Channel::EventListener implementations
//


    template <CLIP_HEADER_ARGS>
    inline typename Clip<CLIP_ARGS>::ch_pointer &
    Clip<CLIP_ARGS>::createChannel()
    {
	Channel<T> * result = new Channel<T>(*this);
	channels_.push_back(createdChannel);
	
	return *result;
    }


    template <CLIP_HEADER_ARGS>
    inline typename Clip<CLIP_ARGS>::ch_pointer &
    Clip<CLIP_ARGS>::channel(unsigned int position)
    {
	return channels_.at(position);
    }


    template <CLIP_HEADER_ARGS>
    inline void Clip<CLIP_ARGS>::producerRoutine()
    {
	// *** NOT IMPLEMENTED ***
	sendEndOfStream();
    }


    template <CLIP_HEADER_ARGS>
    inline void Clip<CLIP_ARGS>::consumerRoutine()
    {
	while (true)
	{
	    if (!readPacket()) break;
	    testStop();
	}
    }


    template <CLIP_HEADER_ARGS>
    inline void Clip<CLIP_ARGS>::handlePacket(const PacketBase & packet)
    {
	try
	{
	    const Packet<T> & p = dynamic_cast<const Packet<T> &>(packet);  // *** Will SIGSEGV ***
			
	    if (p.numChannels() != numChannels())
		throw PacketTypeMismatchException();

	    for (int i = 0; i < numChannels(); ++i)
	    {
		channel(i).insert(p.data(i), cursor());
	    }

	    onChange();
	}
	catch (const std::bad_cast & e)
	{
	    throw PacketTypeMismatchException();
	}
    }

}  // namespace ap


#endif  // BACKEND_CLIP_H

// Local Variables:
// mode:C++
// End:

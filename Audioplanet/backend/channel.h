//  backend/channel.h
//  Copyright (C) 1999-2003 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef BACKEND_CHANNEL_H
#define BACKEND_CHANNEL_H


#include <list>
#include <vector>

#include "backend/soundlevel.h"
#include "backend/audioformat.h"
#include "backend/packet.h"


namespace ap
{

#define CHANNEL_ARGS T, Sequence
#define CHANNEL_HEADER_ARGS typename T, typename Sequence
	
    template <typename T, typename Sequence = std::vector<T> >
    class Channel
    {
    public:
	typedef typename Sequence::value_type value_type;
	typedef typename Sequence::size_type size_type;
	typedef Sequence container_type;


	Channel();
	virtual ~Channel() {}
	
	
	const AudioFormat & audioFormat() { return audioFormat_; }

	size_type length() const { return c_.size(); }
	bool empty() const { return c_.empty(); }
	
	void insert(
	    const typename Packet<T>::value_type & insertion,
	    size_type position);
	void push_back(const T & x);
	void erase(size_type position, size_type length);
	
	const value_type & operator[](size_type position) const;
	
	
	SoundLevelRange<T> levelRange(
	    size_type position,
	    size_type length) const;
	
	void getLevelRange(
	    size_type position,
	    size_type length,
	    Float64 & minLevel,
	    Float64 & maxLevel) const;
	
	
    private:
	container_type c_;
	AudioFormat audioFormat_;
    };


    template <CHANNEL_HEADER_ARGS>
    inline Channel<CHANNEL_ARGS>::Channel()
    {
	audioFormat_.setDomain(AudioFormat::TIME_DOMAIN);
	audioFormat_.setSampleFormat(SampleInfo<T>::sampleFormat);
    }


    template <CHANNEL_HEADER_ARGS>
    inline const typename Channel<CHANNEL_ARGS>::value_type &
    Channel<CHANNEL_ARGS>::operator[](Channel<CHANNEL_ARGS>::size_type position) const
    {
	return c_.at(position);
    }


    template <CHANNEL_HEADER_ARGS>
    inline void Channel<CHANNEL_ARGS>::insert(
	const typename Packet<T>::value_type & insertion,
	Channel<CHANNEL_ARGS>::size_type position)
    {
	c_.reserve(insertion.length());
	std::copy(
	    insertion.begin(),
	    insertion.end(),
	    std::inserter(c_, position));
    }


    template <CHANNEL_HEADER_ARGS>
    inline void Channel<CHANNEL_ARGS>::push_back(const T & x)
    {
	c_.push_back(x);
    }


    template <CHANNEL_HEADER_ARGS>
    inline void Channel<CHANNEL_ARGS>::erase(
	Channel<CHANNEL_ARGS>::size_type position,
	Channel<CHANNEL_ARGS>::size_type length)
    {
	typename container_type::iterator first(c_.begin() + position);
	typename container_type::iterator last(first + length);
	typename container_type::iterator end(c_.end());
	std::copy(last, end, first);
	c_.erase(end - length, end);
    }


    template <CHANNEL_HEADER_ARGS>
    inline SoundLevelRange<T> Channel<CHANNEL_ARGS>::levelRange(
	Channel<CHANNEL_ARGS>::size_type position,
	Channel<CHANNEL_ARGS>::size_type length) const
    {
	SoundLevelRange<T> result;


	for (size_type i = position;
	     i < std::min(position + length, length());
	     ++i)
	    result.add(data_[i]);

	return result;
    }


    template <CHANNEL_HEADER_ARGS>
    inline void Channel<CHANNEL_ARGS>::getLevelRange(
	Channel<CHANNEL_ARGS>::size_type position,
	Channel<CHANNEL_ARGS>::size_type length,
	double & minLevel,
	double & maxLevel) const
    {
	const SoundLevelRange<T> result = levelRange(position, length);
	minLevel = result.min().relative();
	maxLevel = result.max().relative();
    }

}  // namespace AP


#endif  // BACKEND_CHANNEL_H

// Local Variables:
// mode:C++
// End:

//  soundlevel.h
//  Copyright (C) 1999-2001 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef SOUNDLEVEL_H
#define SOUNDLEVEL_H

#ifdef __GNUG__
#pragma interface
#endif


#include <algorithm>
#include <limits>
#include <cmath>


template <typename T>
class SoundLevel
{
public:

	SoundLevel(T sample) EXCEPTIONS;

	Float64 relative() const EXCEPTIONS;
	Float64 dB() const EXCEPTIONS;

	T sample() const EXCEPTIONS;
	void setSample(T sample) EXCEPTIONS;

private:

	T sample_;
};


template <typename T>
class SoundLevelRange
{
public:

	SoundLevelRange(T min = 0, T max = 0) EXCEPTIONS;

	const SoundLevel<T> & min() const EXCEPTIONS;
	const SoundLevel<T> & max() const EXCEPTIONS;

	void add(T sample) EXCEPTIONS;

private:

	SoundLevel<T> min_,
	              max_;
};


//
// SoundLevel methods
//


template <typename T>
inline SoundLevel<T>::SoundLevel(T sample) EXCEPTIONS :

	sample_(sample) {}

/*
template <class T>
inline int minimum() { return static_cast<int>(Limits<T>::min); }
template <class T>
inline int maximum() { return static_cast<int>(Limits<T>::max); }
*/

template <typename T>
inline Float64 SoundLevel<T>::relative() const EXCEPTIONS
{
	if (std::numeric_limits<T>::is_integer)
		return static_cast<Float64>(sample_) / std::numeric_limits<T>::max();
//		return static_cast<Float64>(sample_) / std::max(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
	else
		return static_cast<Float64>(sample_);
}


template <typename T>
inline Float64 SoundLevel<T>::dB() const EXCEPTIONS {

	return 10 * std::log(relative());
}


template <typename T>
T SoundLevel<T>::sample() const EXCEPTIONS {

	return sample_;
}


template <typename T>
void SoundLevel<T>::setSample(T sample) EXCEPTIONS {

	sample_ = sample;
}


//
// SoundLevelRange methods
//


template <typename T>
inline SoundLevelRange<T>::SoundLevelRange(T min, T max) EXCEPTIONS :

	min_(min),
	max_(max) {}


template <typename T>
inline const SoundLevel<T> & SoundLevelRange<T>::min() const EXCEPTIONS {

	return min_;
}


template <typename T>
inline const SoundLevel<T> & SoundLevelRange<T>::max() const EXCEPTIONS {

	return max_;
}


template <typename T>
inline void SoundLevelRange<T>::add(T sample) EXCEPTIONS {

	min_.setSample(std::min(min_.sample(), sample));
	max_.setSample(std::max(max_.sample(), sample));
}


#endif  // SOUNDLEVEL_H

// Local Variables:
// mode:C++
// End:

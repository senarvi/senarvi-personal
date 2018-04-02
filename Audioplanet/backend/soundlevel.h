//  backend/soundlevel.h
//  Copyright (C) 1999-2002 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef BACKEND_SOUNDLEVEL_H
#define BACKEND_SOUNDLEVEL_H


#include <algorithm>
#include <limits>
#include <cmath>


namespace ap
{

	template <typename T>
	class SoundLevel
	{
	public:
		SoundLevel(T sample);

		double relative() const;
		double dB() const;

		T sample() const { return sample_; }
		void setSample(T sample) { sample_ = sample; }

	private:
		T sample_;
	};


	template <typename T>
	class SoundLevelRange
	{
	public:
		SoundLevelRange(T min = 0, T max = 0);

		const SoundLevel<T> & min() const { return min_; }
		const SoundLevel<T> & max() const { return max_; }

		void add(T sample);

	private:
		SoundLevel<T> min_,
			max_;
	};


//
// SoundLevel methods
//


	template <typename T>
	inline SoundLevel<T>::SoundLevel(T sample) :

	sample_(sample) {}


	template <typename T>
	inline double SoundLevel<T>::relative() const
	{
		if (std::numeric_limits<T>::is_integer)
			return static_cast<double>(sample_) / std::numeric_limits<T>::max();
		else
			return static_cast<double>(sample_);
	}


	template <typename T>
	inline double SoundLevel<T>::dB() const
	{
		return 10 * std::log(relative());
	}


//
// SoundLevelRange methods
//


	template <typename T>
	inline SoundLevelRange<T>::SoundLevelRange(T min, T max) :

	min_(min),
	max_(max) {}


	template <typename T>
	inline void SoundLevelRange<T>::add(T sample) {

		min_.setSample(std::min(min_.sample(), sample));
		max_.setSample(std::max(max_.sample(), sample));
	}

}  // namespace ap


#endif  // BACKEND_SOUNDLEVEL_H

// Local Variables:
// mode:C++
// End:

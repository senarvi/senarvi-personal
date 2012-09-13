//  backend/filter.h
//  Copyright (C) 1999-2003 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef BACKEND_FILTER_H
#define BACKEND_FILTER_H


#include <string>
#include <map>
#include <pthread.h>

#include "backend/audioformat.h"
#include "backend/base/thread.h"


namespace ap
{

	class Filter : public virtual Thread
	{
	public:

		Filter();
		virtual ~Filter() {}


		virtual bool supports(AudioFormat::SampleFormat sampleFormat) const;


		virtual unsigned int minInputPins() const = 0;
		virtual unsigned int maxInputPins() const = 0;
		virtual unsigned int minOutputPins() const = 0;
		virtual unsigned int maxOutputPins() const = 0;

		void setNumInputPins(unsigned int numInputPins);
		void setNumOutputPins(unsigned int numOutputPins);


		// Initialize is the first method to be called
		virtual void initialize() {}

		// The methods below can
		// only be called after a call to complete.
		void complete();

		// Prepare is the last method to be called
		virtual void prepare() {}


	protected:

		unsigned int numInputPins() const;
		unsigned int numOutputPins() const;


	private:

		unsigned int numInputPins_;
		unsigned int numOutputPins_;

		bool completed_;


		void destroy();
	};


	inline Filter::Filter() :

		completed_(false)

	{}


	inline bool Filter::supports(AudioFormat::SampleFormat sampleFormat) const
	{
		return true;
	}

}  // namespace ap


#endif  // BACKEND_FILTER_H

// Local Variables:
// mode:C++
// End:

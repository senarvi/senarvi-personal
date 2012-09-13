//  backend/audioformat.h
//  Copyright (C) 1999-2002 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef BACKEND_AUDIOFORMAT_H
#define BACKEND_AUDIOFORMAT_H


#include <stdint.h>

#include <backend/base/types.h>


namespace ap
{

	class AudioFormat
	{
	public:

		typedef int_least32_t SampleRate;
	
		enum SampleFormat
		{
			INT_16   = 0x01,
			INT_32   = 0x02,
			FLOAT_32 = 0x04,
			FLOAT_64 = 0x08
		};
	
		enum Domain
		{
			TIME_DOMAIN,
			FREQUENCY_DOMAIN
		};

		AudioFormat() {}
		AudioFormat(SampleFormat sampleFormat, SampleRate sampleRate, Domain domain);

		void setSampleFormat(SampleFormat sampleFormat) { sampleFormat_ = sampleFormat; }
		void setSampleRate(SampleRate sampleRate) { sampleRate_ = sampleRate; }
		void setDomain(Domain domain) { domain_ = domain; }

		SampleFormat sampleFormat() const { return sampleFormat_; }
		unsigned int sampleSize() const;
		SampleRate sampleRate() const { return sampleRate_; }
		Domain domain() const { return domain_; }
	
	
	private:
	
		SampleFormat sampleFormat_;  // Sample size and format
		SampleRate sampleRate_;  // Sample rate in samples/second
		Domain domain_;  // Time/frequency domain
	};


	template <typename T>
	struct SampleInfo {};

	struct SampleInfo<int16_t>
	{
		static const AudioFormat::SampleFormat sampleFormat = AudioFormat::INT_16;
	};

	struct SampleInfo<int32_t>
	{
		static const AudioFormat::SampleFormat sampleFormat = AudioFormat::INT_32;
	};

	struct SampleInfo<Float32>
	{
		static const AudioFormat::SampleFormat sampleFormat = AudioFormat::FLOAT_32;
	};

	struct SampleInfo<Float64>
	{
		static const AudioFormat::SampleFormat sampleFormat = AudioFormat::FLOAT_64;
	};


//
// Inline methods
//


	inline AudioFormat::AudioFormat(SampleFormat sampleFormat, SampleRate sampleRate, Domain domain) :

		sampleFormat_(sampleFormat),
		sampleRate_(sampleRate),
		domain_(domain)

	{}


	inline bool operator==(const AudioFormat & x, const AudioFormat & y)
	{
		return ((x.sampleFormat() == y.sampleFormat()) &&
				(x.sampleRate() == y.sampleRate()) &&
				(x.domain() == y.domain()));
	}


	inline bool operator!=(const AudioFormat & x, const AudioFormat & y)
	{
		return !(x == y);
	}


// File operators

/*
  inline const File & operator>>(const File & file, AudioFormat & x) EXCEPTIONS
  {
  file.read(&x, sizeof(x));
  return file;
  }


  inline const File & operator<<(const File & file, const AudioFormat & x) EXCEPTIONS
  {
  file.write(&x, sizeof(x));
  return file;
  }
*/

}  // namespace ap


#endif  // BACKEND_AUDIOFORMAT_H

// Local Variables:
// mode:C++
// End:

//  wavformat.h
//  Copyright (C) 1999-2001 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef WAVFORMAT_H
#define WAVFORMAT_H


#include "io.h"


#define MAGIC_RIFF (('R' << 24) | ('I' << 16) | ('F' << 8) | 'F')
#define MAGIC_WAVE (('W' << 24) | ('A' << 16) | ('V' << 8) | 'E')
#define MAGIC_FMT  (('f' << 24) | ('m' << 16) | ('t' << 8) | ' ')
#define MAGIC_DATA (('d' << 24) | ('a' << 16) | ('t' << 8) | 'a')


#define WAVE_FORMAT_UNKNOWN		(0x0000)
#define WAVE_FORMAT_PCM			(0x0001)
#define WAVE_FORMAT_ADPCM		(0x0002)
#define WAVE_FORMAT_IEEE_FLOAT          (0x0003)
#define WAVE_FORMAT_IBM_CVSD		(0x0005)
#define WAVE_FORMAT_ALAW		(0x0006)
#define WAVE_FORMAT_MULAW		(0x0007)
#define WAVE_FORMAT_OKI_ADPCM		(0x0010)
#define WAVE_FORMAT_DVI_ADPCM		(0x0011)
#define WAVE_FORMAT_DIGISTD		(0x0015)
#define WAVE_FORMAT_DIGIFIX		(0x0016)
#define WAVE_FORMAT_YAMAHA_ADPCM        (0x0020)
#define WAVE_FORMAT_DSP_TRUESPEECH      (0x0022)
#define WAVE_FORMAT_GSM610		(0x0031)
#define IBM_FORMAT_MULAW		(0x0101)
#define IBM_FORMAT_ALAW			(0x0102)
#define IBM_FORMAT_ADPCM		(0x0103)
#define WAVE_FORMAT_CREATIVE_ADPCM      (0x0200)


class Header {

public:

    inline Uint32 read(int length) EXCEPTIONS;

    inline Uint32 format() const EXCEPTIONS;
    inline unsigned int numChannels() const EXCEPTIONS;
    inline Uint32 samplesPerSec() const EXCEPTIONS;
    inline unsigned int blockAlign() const EXCEPTIONS;
    inline unsigned int bitsPerSample() const EXCEPTIONS;
    inline Uint32 extraSize() const EXCEPTIONS;
    inline char * extraData() EXCEPTIONS;


private:

    Uint16 format_;
    Uint16 numChannels_;
    Uint32 samplesPerSec_;
    Uint32 avgBytesPerSec_;
    Uint16 blockAlign_;
    Uint16 bitsPerSample_;

    Uint16 extraSize_;
    char * extraData_;  // This won't be released

};


inline Uint32 Header::read(int length) EXCEPTIONS
{
	if (length < 14) throw ParseError(LOCATION);

	format_ = readInt16( );
	numChannels_ = readInt16( );
	samplesPerSec_ = readInt32( );
	avgBytesPerSec_ = readInt32( );
	blockAlign_ = readInt16( );

	length -= 14;

	if (length >= 2) {
		bitsPerSample_ = readInt16( );
		length -= 2;
	}
	else bitsPerSample_ = 8;

	return length;  // Bytes unread
}

inline Uint32 Header::format() const EXCEPTIONS { return static_cast<Uint32>(format_); }
inline unsigned int Header::numChannels() const EXCEPTIONS { return static_cast<Uint32>(numChannels_); }
inline Uint32 Header::samplesPerSec() const EXCEPTIONS { return samplesPerSec_; }
inline unsigned int Header::blockAlign() const EXCEPTIONS { return static_cast<Uint32>(blockAlign_); }
inline unsigned int Header::bitsPerSample() const EXCEPTIONS { return static_cast<Uint32>(bitsPerSample_); }
inline Uint32 Header::extraSize() const EXCEPTIONS { return static_cast<Uint32>(extraSize_); }
inline char * Header::extraData() EXCEPTIONS { return extraData_; }


#endif  // WAVFORMAT_H

// Local Variables:
// mode:C++
// End:

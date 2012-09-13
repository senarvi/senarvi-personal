//  wavplugin.cc
//  Copyright (C) 1999-2000 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifdef __GNUG__
#pragma implementation
#endif


#include "wavplugin.h"
#include "wavopenhandler.h"
#include "audioplanet/audioplanet.h"


AP::Filter * createWavOpenHandler(const AP::AudioFormat & audioFormat) EXCEPTIONS
{
	trace();
	
	
	if (audioFormat.domain() == AP::AudioFormat::FREQUENCY_DOMAIN)
		throw InvalidFormat(LOCATION);
	
	
	switch (audioFormat.sampleFormat())
	{
	case AP::AudioFormat::INT_16:
		return new WavOpenHandler<Int16>;
	case AP::AudioFormat::INT_32:
		return new WavOpenHandler<Int32>;
	case AP::AudioFormat::FLOAT_32:
		return new WavOpenHandler<Float32>;
	case AP::AudioFormat::FLOAT_64:
		return new WavOpenHandler<Float64>;
	default:
		throw FlowOfControlException(LOCATION);
	}
}


void getWavFormat(AP::AudioFormat & audioFormat) EXCEPTIONS
{
	trace();
	
	
	Uint32 length;
	
	
	if (readMagic() != MAGIC_RIFF) throw ParseError(LOCATION);
	skip(4);
	if (readMagic() != MAGIC_WAVE) throw ParseError(LOCATION);
	
	
	std::streampos headerG;
	do
	{
		headerG = std::cin.tellg();
		
		Uint32 magic = readMagic();
		if (magic == MAGIC_DATA) throw ParseError(LOCATION);
		
		length = readInt32();
		length = length + (length % 2);  // Even padding
		
		if (magic == MAGIC_FMT) break;
		
		skip(length);
	}
	while (true);
	
	
	Header header;
	header.read(length);
	
	
	Uint32 inputSampleSize = header.bitsPerSample();
	
	switch (header.format())
	{
	case WAVE_FORMAT_PCM:
		
		if (inputSampleSize <= 16)
			audioFormat.setSampleFormat(AP::AudioFormat::INT_16);
		else if (inputSampleSize <= 32)
			audioFormat.setSampleFormat(AP::AudioFormat::INT_32);
		else
			throw InvalidFormat(LOCATION);
		
		break;
		
	case WAVE_FORMAT_IEEE_FLOAT:
		
		switch (inputSampleSize) {
		case 32:
			audioFormat.setSampleFormat(AP::AudioFormat::FLOAT_32);
			break;
		case 64:
			audioFormat.setSampleFormat(AP::AudioFormat::FLOAT_64);
			break;
		default:
			throw InvalidFormat(LOCATION);
		}
		
		break;
		
	default:
		throw InvalidFormat(LOCATION);
	}
	
	audioFormat.setDomain(AP::AudioFormat::TIME_DOMAIN);
	audioFormat.setSampleRate(header.samplesPerSec());
	
	
	std::cin.seekg(headerG);  // Seek to the beginning of the header so that initialize can read it.
}


void WavPluginInfo::query() const EXCEPTIONS
{
	AP::FilterHandle openHandle = AP::installFilter(createWavOpenHandler);
	AP::registerOpenHandler("audio/x-wav", getWavFormat, openHandle);
}


WavPluginInfo AUDIOPLANET_PLUG_IN;

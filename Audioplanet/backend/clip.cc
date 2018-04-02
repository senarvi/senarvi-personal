//  packet/clip.cc
//  Copyright (C) 1999-2003 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#include <set>

#include <fcntl.h>  // O_RDONLY
#include <audiofile.h>

#include "boost/lexical_cast.hpp"

#include "backend/clip.h"
#include "backend/audioplanet.h"
#include "backend/filter.h"
#include "backend/base/process.h"
#include "backend/base/debug.h"
#include "backend/base/log.h"


namespace ap {


    ClipBase & ClipBase::createFromFile(const std::string & fileName)
    {
	ClipBase * result;


	// Open the file.
	//

	AFfilehandle fileHandle = afOpenFile(fileName.c_str(), "r", AF_NULL_FILESETUP);
	if (fileHandle == AF_NULL_FILEHANDLE)
	    throw FileIOException("afOpenFile failed", fileName.c_str());


	int numChannels;
	double sampleRate;


	try
	{
	    // Set virtual sample format.
	    //

	    int sampleFormat = -1;
	    int sampleWidth = -1;
	    afGetSampleFormat(fileHandle, AF_DEFAULT_TRACK, &sampleFormat, &sampleWidth);
	    if ((sampleFormat == -1) || (sampleWidth == -1))
		throw FileIOException("afGetSampleFormat failed", fileName.c_str());

	    if (sampleFormat == AF_SAMPFMT_UNSIGNED)
		sampleFormat = AF_SAMPFMT_TWOSCOMP;

	    if (sampleFormat == AF_SAMPFMT_TWOSCOMP)
	    {
		if (sampleWidth <= 16)
		    sampleWidth = 16;
		else
		    sampleWidth = 32;
	    }

	    if (afSetVirtualSampleFormat(fileHandle, AF_DEFAULT_TRACK,
					 sampleFormat, sampleWidth) == -1)
		throw FileIOException("afSetVirtualSampleFormat failed",
				      fileName.c_str());


	    // Get properties.
	    //

	    numChannels = afGetVirtualChannels(fileHandle, AF_DEFAULT_TRACK);
	    if (numChannels == -1)
		throw FileIOException("afGetVirtualChannels failed", fileName.c_str());

	    sampleRate = afGetRate(fileHandle, AF_DEFAULT_TRACK);
	    if (sampleRate < 0)
		// afGetVirtualRate returns double -1 in case of an error.
		throw FileIOException("afGetRate failed", fileName.c_str());


	    // Create the clip.
	    //

	    switch (sampleFormat)
	    {
	    case AF_SAMPFMT_TWOSCOMP:
		switch (sampleWidth)
		{
		case 16:
		    result = new Clip<int16_t>(fileName);
		    break;
		case 32:
		    result = new Clip<int32_t>(fileName);
		    break;
		default:
		    assert(false);
		}
		break;

	    case AF_SAMPFMT_FLOAT:
		result = new Clip<Float32>(fileName);
		break;
	    case AF_SAMPFMT_DOUBLE:
		result = new Clip<Float64>(fileName);
		break;

	    default:
		throw UnsupportedFormatException(fileName.c_str());
	    }

	}
	catch (const std::exception & e)
	{
	    afCloseFile(fileHandle);  // Possible failure in afCloseFile will be ignored.
	    throw;
	}


	result->sampleRate_ = sampleRate;

	// *** SHOULD CREATE THE CHANNELS ***
	

	try
	{
	    result->readFrames(fileHandle);
	}
	catch (const std::exception & e)
	{
	    delete result;
	    afCloseFile(fileHandle);  // Possible failure in afCloseFile will be ignored.
	    throw;
	}


	// Close the file.
	//

	if (afCloseFile(fileHandle) == -1)
	{
	    delete result;
	    throw FileIOException("afCloseFile failed", fileName.c_str());
	}

	return *result;
    }


    void ClipBase::writeToFile()
    {
    }
}

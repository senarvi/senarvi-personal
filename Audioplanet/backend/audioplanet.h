//  backend/audioplanet.h
//  Copyright (C) 1999-2003 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef BACKEND_AUDIOPLANET_H
#define BACKEND_AUDIOPLANET_H


#include <istream>
#include <stdint.h>
#include <exception>

//#include "backend/base/exception.h"


namespace ap
{

    class Filter;
    class AudioFormat;


    class OutOfFilterHandlesException : public std::exception
    {
    public:
	const char * what() const throw ()
	{
	    return "Too many filters created.";
	}
    };
	
	
    typedef int32_t FilterHandle;
    typedef Filter * (*CreateMethod)(const AudioFormat & audioFormat);
    typedef void (*GetFormatMethod)(AudioFormat & audioFormat);  // For input filters
	
	
    extern FilterHandle installFilter(CreateMethod createMethod);
    extern Filter * createFilterInstance(FilterHandle handle, const AudioFormat & audioFormat);
	
    extern void registerOpenHandler(const std::string & mimeType, GetFormatMethod getFormatMethod, FilterHandle handle);
    extern void registerSaveHandler(const std::string & mimeType, FilterHandle handle);
    extern void registerCompressor(const std::string & compressionType, FilterHandle handle);
    extern void registerDecompressor(const std::string & compressionType, FilterHandle handle);
	
    extern FilterHandle findOpenHandler(const std::string & mimeType);
    extern void getFormat(const std::string & mimeType, AudioFormat & audioFormat);
    extern FilterHandle findSaveHandler(const std::string & mimeType);
    extern FilterHandle findCompressor(const std::string & compressionType);
    extern FilterHandle findDecompressor(const std::string & compressionType);
	
    extern bool openHandlerRegistered(const std::string & mimeType);

}  // namespace ap


#endif  // BACKEND_AUDIOPLANET_H

// Local Variables:
// mode:C++
// End:

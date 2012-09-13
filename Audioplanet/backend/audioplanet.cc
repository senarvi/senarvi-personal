//  backend/audioplanet.cc
//  Copyright (C) 1999-2003 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#include <signal.h>
#include <unistd.h>
#include <string>
#include <map>
#include <list>

#include "backend/audioplanet.h"
#include "backend/base/debug.h"


namespace ap
{

    typedef std::map<std::string, FilterHandle> HandlerMap;
    typedef std::map<FilterHandle, CreateMethod> CreateMethodMap;
    typedef std::map<std::string, GetFormatMethod> GetFormatMethodMap;

    static CreateMethodMap createMethods;
    static HandlerMap openHandlers;
    static GetFormatMethodMap getFormatMethods;
    static HandlerMap saveHandlers;
    static HandlerMap decompressors;
    static HandlerMap compressors;
    static std::list<FilterHandle> effects;
    static std::list<FilterHandle> inputfilterCreators;
    static std::list<FilterHandle> outputfilterCreators;


    FilterHandle installFilter(CreateMethod createMethod)
    {
	trace();
	
	
	static FilterHandle handle = -1;
	handle += 1;
	if (handle == -1) throw OutOfFilterHandlesException();
	
	createMethods[handle] = createMethod;
	return handle;
    }


    Filter * createFilterInstance(FilterHandle handle, const AudioFormat & audioFormat)
    {
	trace();
	
	
	CreateMethodMap::iterator result = createMethods.find(handle);
	assert(result != createMethods.end());
	return createMethods[handle](audioFormat);
	// return (*result)(audioFormat);
    }


//
// Registrations
//

    void registerOpenHandler(const std::string & mimeType, GetFormatMethod getFormatMethod, FilterHandle handle)
    {
	trace();
	openHandlers[mimeType] = handle;
	getFormatMethods[mimeType] = getFormatMethod;
    }


    void registerSaveHandler(const std::string & mimeType, FilterHandle handle)
    {
	trace();
	saveHandlers[mimeType] = handle;
    }


    void registerCompressor(const std::string & compressionType, FilterHandle handle)
    {
	trace();
	compressors[compressionType] = handle;
    }


    void registerDecompressor(const std::string & compressionType, FilterHandle handle)
    {
	trace();
	decompressors[compressionType] = handle;
    }


//
// Retrievals
//

    static FilterHandle findHandler(HandlerMap & handlers, const std::string & type)
    {
	trace();
	
	
	HandlerMap::iterator result = handlers.find(type);
	assert(result != handlers.end());
	return handlers[type];
	// return *result;
    }


    FilterHandle findOpenHandler(const std::string & mimeType)
    {
	trace();
	return findHandler(openHandlers, mimeType);
    }


    void getFormat(const std::string & mimeType, AudioFormat & audioFormat)
    {
	trace();
	
	
	GetFormatMethodMap::iterator result = getFormatMethods.find(mimeType);
	assert(result != getFormatMethods.end());
	getFormatMethods[mimeType](audioFormat);
	// (*result)(audioFormat);
    }


    FilterHandle findSaveHandler(const std::string & mimeType)
    {
	trace();
	return findHandler(saveHandlers, mimeType);
    }


    FilterHandle findCompressor(const std::string & compressionType)
    {
	trace();
	return findHandler(compressors, compressionType);
    }


    FilterHandle findDecompressor(const std::string & compressionType)
    {
	trace();
	return findHandler(decompressors, compressionType);
    }


//
// Registration checks
//

    bool handlerRegistered(HandlerMap & handlers, const std::string & type)
    {
	trace();
	return handlers.find(type) != handlers.end();
    }


    bool openHandlerRegistered(const std::string & mimeType)
    {
	trace();
	return handlerRegistered(openHandlers, mimeType);
    }

}  // namespace ap

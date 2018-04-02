//  backend/base/exception.cc
//  Copyright (C) 1999-2003 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#include <cerrno>
#include <boost/lexical_cast.hpp>

#include "backend/base/exceptions.h"


namespace ap {


    SourceLocation::SourceLocation(const char * fileArg,
				   int lineArg,
				   const char * functionArg) :
	file(fileArg),
	line(lineArg),
	function(functionArg) {}


    LocationAwareException::LocationAwareException(SourceLocation * x)
    {
	if (x != 0)
	{
	    sourceFile_ = x->file;
	    sourceLine_ = x->line;
	    sourceFunction_ = x->function;
	    delete x;
	}
	else
	{
	    sourceFile_ = "";
	    sourceLine_ = -1;
	    sourceFunction_ = "";
	}
    }

    
    const char * LocationAwareException::sourceFile() const throw ()
    { return sourceFile_; }


    int LocationAwareException::sourceLine() const throw ()
    { return sourceLine_; }


    const char * LocationAwareException::sourceFunction() const throw ()
    { return sourceFunction_; }

    
    SystemException::SystemException(SourceLocation * throwLocation) :
	std::runtime_error(std::strerror(errno)),
	LocationAwareException(throwLocation) {}


    FileIOException::FileIOException (const std::string & what,
				      const std::string & fileName) :
	std::runtime_error(what),
	fileName_(fileName) {}


    const char * FileIOException::fileName() const throw ()
    { return fileName_.c_str(); }


    UnsupportedFormatException::UnsupportedFormatException(
	const std::string & fileName) :
	std::runtime_error("unsupported audio format"),
	fileName_(fileName) {}


    const char * UnsupportedFormatException::fileName() const throw ()
    { return fileName_.c_str(); }


    InvalidArgumentException::InvalidArgumentException(
	SourceLocation * throwLocation) :
	std::invalid_argument("invalid argument passed to a function"),
	LocationAwareException(throwLocation) {}


}  // namespace ap

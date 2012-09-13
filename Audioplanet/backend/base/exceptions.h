//  backend/base/exception.h
//  Copyright (C) 1999-2003 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef BACKEND_BASE_EXCEPTION_H
#define BACKEND_BASE_EXCEPTION_H


#include <string>
#include <stdexcept>


#define THIS_LOCATION (new ap::SourceLocation(__FILE__, __LINE__, __func__))


namespace ap {


    struct SourceLocation
    {
	explicit SourceLocation(const char * fileArg,
				int lineArg,
				const char * functionArg);

	const char * file;
	int line;
	const char * function;
    };


    class LocationAwareException
    {
    public:
	explicit LocationAwareException(SourceLocation * x);
	virtual ~LocationAwareException() throw () {}

	const char * sourceFile() const throw ();
	int sourceLine() const throw ();
	const char * sourceFunction() const throw ();

    private:
	const char * sourceFile_;
	int sourceLine_;
	const char * sourceFunction_;
    };


    class SystemException : public std::runtime_error,
			    public LocationAwareException
    {
    public:
	explicit SystemException(SourceLocation * throwLocation);
	virtual ~SystemException() throw () {}
    };


    // The file cannot be accessed, the file is corrupt or the library just
    // just doesn't understand the format of the file.
    class FileIOException : public std::runtime_error
    {
    public:
	explicit FileIOException(const std::string & what,
				 const std::string & fileName);
	virtual ~FileIOException() throw () {}

	const char * fileName() const throw ();

    private:
	std::string fileName_;
    };


    // The audio format is not currently supported.
    class UnsupportedFormatException : std::runtime_error
    {
    public:
	explicit UnsupportedFormatException(const std::string & fileName);
	virtual ~UnsupportedFormatException() throw () {}
	
	const char * fileName() const throw ();

    private:
	std::string fileName_;
    };


    // Invalid argument passed to a function.
    class InvalidArgumentException : public std::invalid_argument,
				     public LocationAwareException
    {
    public:
	explicit InvalidArgumentException(SourceLocation * throwLocation);
	virtual ~InvalidArgumentException() throw () {}
    };


}  // namespace ap


#endif  // BACKEND_BASE_EXCEPTION_H

// Local Variables:
// mode:C++
// End:

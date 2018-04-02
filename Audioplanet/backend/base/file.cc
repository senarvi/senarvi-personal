//  backend/base/file.cc
//  Copyright (C) 2001-2002 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#include <unistd.h>
#include <fcntl.h>
#include <boost/assert.hpp>

#include "backend/base/file.h"
#include "backend/base/exceptions.h"
#include "backend/base/debug.h"


namespace ap
{

	void File::close()
	{
		switch (::close(descriptor_))
		{
		case 0:
			descriptor_ = -1;
			return;
		
		case -1:
			throw SystemException(THIS_LOCATION);
		
		default:
			BOOST_ASSERT(false);
		}
	}


	void File::open(const std::string & fileName, int flags)
	{
		trace();
	
	
		descriptor_ = ::open(fileName.c_str(), flags);
		if (descriptor_ == -1) throw SystemException(THIS_LOCATION);
	}


	ssize_t File::read(void * buffer, size_t count) const
	{
		trace();
	
	
		ssize_t result = ::read(descriptor_, buffer, count);
		if (result == -1) throw SystemException(THIS_LOCATION);
		return result;
	}


	ssize_t File::write(const void * buffer, size_t count) const
	{
		trace();
	
	
		ssize_t result = ::write(descriptor_, buffer, count);
		if (result == -1) throw SystemException(THIS_LOCATION);
		return result;
	}


	void File::attachToStdin() const
	{
		if (::dup2(descriptor_, STDIN_FILENO) == -1)
			throw SystemException(THIS_LOCATION);
	}


	void File::attachToStdout() const
	{
		if (::dup2(descriptor_, STDOUT_FILENO) == -1)
			throw SystemException(THIS_LOCATION);
	}

}

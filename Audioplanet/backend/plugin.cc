//  backend/plugin.cc
//  Copyright (C) 1999-2002 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#include <dlfcn.h>

#include "libaudioplanet/plugin.h"


namespace ap
{

	void Plugin::open(const std::string & fileName)
	{
		trace();
	
	
#ifdef RTLD_NOW
		handle_ = dlopen(fileName.c_str(), RTLD_NOW);
#else
		handle_ = dlopen(fileName.c_str(), 0);
#endif
		if (handle_ == NULL) throw FileOpenException(LOCATION, fileName, dlerror());
	
	
		info_ = static_cast<Info*>(dlsym(handle_, "AUDIOPLANET_PLUG_IN"));
	
		if (info_ == NULL) {
			dlclose(handle_);
			throw PluginException(LOCATION, fileName);
		}

		info_->query();
	}


	void Plugin::close()
	{
		trace();
	
	
		// Clean up all the procedures installed by this plug-in
		dlclose(handle_);
	}

}  // namespace ap

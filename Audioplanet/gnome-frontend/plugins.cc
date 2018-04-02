//  plugins.cc
//  Copyright (C) 1999-2002 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#include <cstdlib>  // atexit
#include <string>
#include <set>
#include <algorithm>  // for_each

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include "config.h"
#include "main.h"
#include "applicationerrors.h"
#include "preferences.h"

#include "system/debug.h"
#include "system/log.h"
#include "libaudioplanet/audioplanet.h"
#include "libaudioplanet/plugin.h"


namespace plugins
{

	static std::set<AP::Plugin *> pluginList;


	static void addPlugin(const std::string & fileName) EXCEPTIONS;
	static void removePlugin(AP::Plugin * plugin) EXCEPTIONS;


	static void cleanup() throw()
	{
		try
		{
			trace();
		
		
			for_each(pluginList.begin(), pluginList.end(), removePlugin);
			pluginList.clear();
		}
		catch (const Exception & e)
		{
			try { display(e.what()); }
			catch (...) {}
		}
	}


	void initialize() EXCEPTIONS
	{
		trace();
		if (std::atexit(cleanup) != 0) throw AtexitException(LOCATION);
	}


	void scanDirectory(const std::string & directoryName) EXCEPTIONS
	{
		trace();


		DIR * directoryStream = opendir(directoryName.c_str());
		if (directoryStream == NULL) throw SystemException(LOCATION);

		struct dirent * directoryEntry = readdir(directoryStream);
		while (directoryEntry != NULL)
		{
			std::string fileName = static_cast<std::string>(directoryEntry->d_name);
			std::string filePath = directoryName + "/" + fileName;

			struct stat fileStatus;
			if (stat(filePath.c_str(), &fileStatus) == 0)
			{
				if (S_ISREG(fileStatus.st_mode))  // Regular file?
				{
					int lastDot = fileName.find_last_of('.');
					if (lastDot != -1)  // Has an extension?
					{
						std::string fileExtension = fileName.substr(lastDot + 1);
						if (fileExtension == "so")
						{
							addPlugin(filePath);			
						}
					}
				}
			}
			directoryEntry = readdir(directoryStream);
		}
	}


	static void addPlugin(const std::string& fileName) EXCEPTIONS
	{
		try
		{
			AP::Plugin * plugin = new AP::Plugin;
			plugin->open(fileName);
			pluginList.insert(plugin);
		
			root.debug("Added plugin `" + plugin->name() + "'");
		}
		catch (const FileOpenException & e)
		{
			root.warn(e.description());
		}
	}


	static void removePlugin(AP::Plugin * plugin) EXCEPTIONS
	{
		root.debug("Closing plugin `" + plugin->name() + "'");
	
		plugin->close();
	}

}

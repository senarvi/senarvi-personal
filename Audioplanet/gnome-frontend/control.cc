//  control.cc
//  Copyright (C) 1999-2001 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.


#include <gtk--/main.h>
#include <cstdlib>  // atexit

#include "system/debug.h"
#include "system/log.h"
#include "system/exception.h"


namespace GUI {


	static Gtk::Main* kit = 0;
	static bool in_loop;


	void cleanup() EXCEPTIONS
	{
		try
		{
			trace();
			delete kit;
		}
		catch (const Exception& e)
		{
			log(e.what());
		}
	}
	

	void initialize(int argc, char* argv[]) EXCEPTIONS
	{
		trace();


		if (kit != NULL) return;
		kit = new Gtk::Main(argc, argv);

		if (std::atexit(cleanup) != 0) {
			delete kit;
			throw AtexitException(LOCATION);
		}
	}


	void enterLoop() EXCEPTIONS
	{
		trace();


		if (kit == NULL) throw;
		in_loop = true;
		kit->run();
	}
	
	
	bool inLoop() EXCEPTIONS
	{
		trace();
		return in_loop;
	}


}  // namespace GUI

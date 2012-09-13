//  main.cc
//  Copyright (C) 1999-2001 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#include <signal.h>
#include <log4cpp/OstreamAppender.hh>

#include "main.h"
#include "config.h"  // RETSIGTYPE
#include "applicationerrors.h"

#include "mainwindow.h"
#include "gui/control.h"

#include "clipbookkeeping.h"
#include "pluginbookkeeping.h"
#include "threadbookkeeping.h"
#include "preferences.h"

#include "system/debug.h"
#include "system/log.h"
#include "system/process.h"

#include "boost/lexical_cast.hpp"


// Signal handler for fatal signals
static RETSIGTYPE fatalSignalHandler(int signalNumber) throw ()
{
	static bool firstTime = true;
	
	if (!firstTime) raise(SIGKILL);
	firstTime = false;
	
	switch (signalNumber)
	{
	case SIGHUP:
		fatalError("Hangup signal received");
		break;
	case SIGINT:
		fatalError("Interrupt signal received");
		break;
	case SIGQUIT:
		fatalError("Quit signal received");
		break;
	case SIGABRT:
		fatalError("Abort signal received");
		break;
	case SIGBUS:
		fatalError("Bus error signal received");
		break;
	case SIGFPE:
		fatalError("Floating-point exception signal received");
		break;
	case SIGSEGV:
		fatalError("Segmentation violation signal received");
		break;
	case SIGPIPE:
		fatalError("Broken pipe signal received");
		break;
	case SIGTERM:
		fatalError("Termination signal received");
	}
}


// Catch falling children
static void childExitHandler(int signalNumber)
{
	try
	{
		trace();
		
		
		::pid_t pid;
		
		// Free any system resources used by zombie processes
		while (true)
		{
			pid = Process::detach();
			
			if (pid == 0) return;
			
			log("PROCESS FINISHED\n");
		}
	}
	catch (const Exception & e) {
		raiseException(e);
	}
}


// Program entry
int main(int argc, char * argv[])
{
	try
	{
		// Set up logging
		//
		log4cpp::Appender * appender = new log4cpp::OstreamAppender("default", &std::cerr);
		log4cpp::Layout * layout;
#ifdef _DEBUG
		layout = new DebugLayout();
		root.setPriority(log4cpp::Priority::DEBUG);
#else
		layout = new ReleaseLayout();
		root.setPriority(log4cpp::Priority::INFO);
#endif
		appender->setLayout(layout);
		root.removeAllAppenders();
		root.addAppender(appender);
		

		trace();
		
		
		Process::executable = argv[0];
		
		
		// Set up signal handlers
		//
		if ((signal(SIGHUP, fatalSignalHandler) == SIG_ERR) ||
		    (signal(SIGINT, fatalSignalHandler) == SIG_ERR) ||
		    (signal(SIGQUIT, fatalSignalHandler) == SIG_ERR) ||
		    (signal(SIGABRT, fatalSignalHandler) == SIG_ERR) ||
		    (signal(SIGBUS, fatalSignalHandler) == SIG_ERR) ||
		    (signal(SIGFPE, fatalSignalHandler) == SIG_ERR) ||
		    (signal(SIGSEGV, fatalSignalHandler) == SIG_ERR) ||
		    (signal(SIGPIPE, fatalSignalHandler) == SIG_ERR) ||
		    (signal(SIGTERM, fatalSignalHandler) == SIG_ERR) ||
		    (signal(SIGCHLD, childExitHandler) == SIG_ERR))
			fatalError("Failed to initialize signal handler");
		
		std::set_terminate(terminateHandler);
		std::set_unexpected(unexpectedHandler);
		
		
		// Initialize
		//
		Preferences::initialize();
		Preferences::readFromFile(Preferences::systemDataDir + "/preferences");
		Preferences::readFromCommandLine(argc, argv);
		
		PluginBookkeeping::initialize();
		PluginBookkeeping::scanDirectory("/home/seppo/src/audioplanet/current/plug-ins/fileformats/wav/.libs");
		PluginBookkeeping::scanDirectory("/home/seppo/src/audioplanet/current/plug-ins/input-output/esound/.libs");
		
		ClipBookkeeping::initialize();
		
		
		// Display main window & enter GUI loop
		//
		AudioplanetUI ui = new AudioplanetUI(argc, argv, "/home/seppo/src/audioplanet/current/gnome-frontend/gtkrc");
		TextReceiver receiver("audioplanet");
		ui.run(receiver);
		
		return 0;
	}
	catch (const Exception & e)
	{
		raiseException(e);
	}
}

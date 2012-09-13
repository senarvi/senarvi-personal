//  threads.cc
//  Copyright (C) 1999-2002 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifdef __GNUG__
#pragma implementation
#endif


#include <cstdlib>
#include <fstream>

#include <signal.h>

#include "threads.h"
#include "config.h"
#include "applicationerrors.h"
#include "system/stlextra.h"
#include "system/log.h"


namespace threads
{

	//
	// ThreadEntity class
	//


	ThreadEntity::ThreadEntity() EXCEPTIONS
	{
		trace();
		threads_.clear();
	}


	ThreadEntity::~ThreadEntity() EXCEPTIONS
	{
		trace();


		// Make sure all the threads have finished
		Threads::iterator threadIterator;
		for (threadIterator = threads_.begin(); threadIterator != threads_.end(); threadIterator++)
			(*threadIterator)->kill();
	}


	void ThreadEntity::add(Thread & thread) EXCEPTIONS
	{
		trace();


		mutex_.lock();
		threads_.insert(&thread);
		mutex_.unlock();
	}


	void ThreadEntity::remove(Thread & thread) EXCEPTIONS
	{
		trace();


		mutex_.lock();
		threads_.erase(&thread);
		mutex_.unlock();
	}


	void ThreadEntity::run() EXCEPTIONS
	{
#ifdef _DEBUG
		log("Starting thread entity");
#endif


		Threads::iterator iter;
		// Start all the threads
		for (iter = threads_.begin(); iter != threads_.end(); iter++)
			(*iter)->start();
		// Join the threads, one by one
		for (iter = threads_.begin(); iter != threads_.end(); iter++)
			(*iter)->join();


#ifdef _DEBUG
		log("Thread entity finished");
#endif
	}


	void ThreadEntity::cleanup() EXCEPTIONS
	{
		trace();
		
		
		Threads::iterator iter;
		for (iter = threads_.begin(); iter != threads_.end(); iter++)
			(*iter)->stop();
	}


	//
	// DiscreteProcess::ProgressWindow class
	//


	void DiscreteProcess::ProgressWindow::atCancel() EXCEPTIONS
	{
		trace();


		threadEntity_->stop();
		delete threadEntity_;
	}


	//
	// DiscreteProcess class
	//


	DiscreteProcess::DiscreteProcess(const std::string & windowTitle) EXCEPTIONS :
	
		progressWindow_(windowTitle, this)
		
	{
		progressWindow_.show();
	}


	DiscreteProcess::~DiscreteProcess() EXCEPTIONS
	{
		progressWindow_.hide();
	}

}

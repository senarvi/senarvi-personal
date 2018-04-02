//  backend/base/thread.cc
//  Copyright (C) 1999-2003 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#include <map>
#include <cerrno>

#include <signal.h>
#include <boost/lexical_cast.hpp>

#include "backend/base/thread.h"
#include "backend/base/log.h"
#include "backend/base/debug.h"


using namespace std;


namespace ap
{

//
// Mutex class
//


    Mutex::Mutex()
    {
	pthread_mutex_init(&posixMutex_, NULL);
    }
	
	
    Mutex::~Mutex()
    {
	unlock();
	if (pthread_mutex_destroy(&posixMutex_) != 0) throw SystemException(THIS_LOCATION);
    }
	

//
// Condition class
//
	

    Condition::Condition()
    {
	pthread_cond_init(&posixCondition_, NULL);
    }
	
	
    Condition::~Condition()
    {
	if (pthread_cond_destroy(&posixCondition_) != 0)
	    throw SystemException(THIS_LOCATION);
    }
	

//
// Thread class
//

    Thread::Thread(bool createJoinable, bool selfDestruct) :

	alreadyStopped_(true),
	doneCleanup_(false),
	selfDestruct_(selfDestruct)
	
    {
	int detachState;
	
	
	if (pthread_attr_init(&posixAttributes_) != 0)
	    throw SystemException(THIS_LOCATION);
	
	if (createJoinable) detachState = PTHREAD_CREATE_JOINABLE;
	else detachState = PTHREAD_CREATE_DETACHED;	
	
	if (pthread_attr_setdetachstate(&posixAttributes_, detachState) != 0)
	    throw SystemException(THIS_LOCATION);
    }


    void * threadCaller(void * object)
    {
	static_cast<Thread *>(object)->entry();
	return NULL;
    }


    void Thread::entry() throw ()
    {
#ifdef DEBUG
	log("NEW THREAD");
#endif
		
		
	run();
	exit();
		
	if (selfDestruct_) delete this;
    }


// Exit() is only called from entry() and from stop(). Since stop()
// waits until the thread terminates before calling exit(), we cannot
// get called simultaneously from these two functions (no need for
// any mutexes)
    void Thread::exit()
    {
	if (doneCleanup_) return;
	doneCleanup_ = true;
	cleanup();


#ifdef DEBUG
	log("THREAD FINISHED\n");
#endif
    }


    void Thread::start()
    {
	trace();
	
	
	alreadyStopped_ = false;  // For stop()
	
	if (pthread_create(&posixThread_, &posixAttributes_, threadCaller, this) != 0)
	    throw SystemException(THIS_LOCATION);
    }


// Send a cancellation request to the thread and wait until it terminates
// (if it has not already finished). After that, detach the thread and call
// exit (if it has not already been detached)
    void Thread::stop()
    {
#ifdef DEBUG
	log("Stopping thread " + boost::lexical_cast<std::string>(id()));
#endif


	// We have to ensure we won't get called from two threads simultaneously
	stopMutex_.lock();

	if (!alreadyStopped_)
	{
	    int error = pthread_cancel(posixThread_);
	    if (error != ESRCH)
	    {
		// The thread was running

		if (error != 0)
		    // The function failed even though the thread was running
		    throw SystemException(THIS_LOCATION);

		try {
		    join();
		    // Fails if the thread has been detached, in case
		    // we have no way to call exit after the thread
		    // finishes

		    exit();
		}
		catch (exception & e) {}
	    }
	    else
	    {
		// The thread was not running but may still be joinable (if it
		// has finished by itself). In this case exit has already been
		// called

		try {
		    detach();
		}
		catch (exception & e) {}
	    }
		
	    alreadyStopped_ = true;
	}
	
	if (selfDestruct_ && (this != NULL)) delete this;

	stopMutex_.unlock();
    }


// Wait until the thread terminates and call exit
    void Thread::join() const
    {
	root.debug("Waiting for thread to finish: " + boost::lexical_cast<std::string>(id()));
	
	
	if (pthread_join(posixThread_, NULL) != 0) throw SystemException(THIS_LOCATION);
	
	
	root.debug("Synchronized to thread " + boost::lexical_cast<std::string>(id()));
    }


// Kill and detach the thread
    void Thread::kill() const
    {
	root.debug("Killing thread " + boost::lexical_cast<std::string>(id()));
	
	
	int error = pthread_kill(posixThread_, SIGKILL);
	if (error != ESRCH)
	{
	    // The thread was running

	    if (error != 0)
		// The function failed even though the thread was running
		throw SystemException(THIS_LOCATION);

	    detach();
	}
	else
	{
	    // The thread was not running but may still be joinable

	    try {
		detach();
	    }
	    catch (std::exception & e) {}
	}
    }


    void Thread::detach() const
    {
	trace();
	if (pthread_detach(posixThread_) != 0) throw SystemException(THIS_LOCATION);
    }

}  // namespace ap

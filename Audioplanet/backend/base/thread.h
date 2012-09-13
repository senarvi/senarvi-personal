//  backend/base/thread.h
//  Copyright (C) 1999-2003 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef BACKEND_BASE_THREAD_H
#define BACKEND_BASE_THREAD_H


#include <pthread.h>

#include <backend/base/exceptions.h>


namespace ap
{

    class Mutex
    {
	friend class Condition;
	
	
    public:
	
	Mutex();
	~Mutex();
	
	inline void lock();
	inline void unlock();
	
	
    private:
	
	pthread_mutex_t posixMutex_;
    };
	
	
    class Condition
    {
    public:
	
	Condition();
	~Condition();
	
	inline void signal();
	inline void broadcast();

	inline void wait(Mutex & mutex);
	
	
    private:

	pthread_cond_t posixCondition_;
    };


    class Thread
    {
	friend void * threadCaller(void * object);
	
	
    public:
	
	typedef pthread_t Id;
	
	
	Thread(bool createJoinable = true, bool selfDestruct = false);
	virtual ~Thread() {}
	
	void start();
	void stop();
	void join() const;
	void kill() const;
	void detach() const;
	
	Id id() const;     // Identifier of the thread this object is running
	static Id self();  // Identifier of the calling thread
	
	
    protected:
	
	// Check if stop has been called
	void testStop() const;
	
	
    private:
	
	void entry() throw ();
	void exit();
	
	virtual void run() = 0;
	virtual void cleanup() {}
	
	
	pthread_t posixThread_;
	pthread_attr_t posixAttributes_;
	
	Mutex stopMutex_;
	bool alreadyStopped_;
	bool doneCleanup_;
	bool selfDestruct_;
    };


//
// Mutex class inline methods
//


    inline void Mutex::lock()
    {
	if (pthread_mutex_lock(&posixMutex_) != 0)
	    throw SystemException(THIS_LOCATION);
    }


    inline void Mutex::unlock()
    {
	if (pthread_mutex_unlock(&posixMutex_) != 0)
	    throw SystemException(THIS_LOCATION);
    }


//
// Condition class inline methods
//


    inline void Condition::signal()
    {
	pthread_cond_signal(&posixCondition_);
    }


    inline void Condition::broadcast()
    {
	pthread_cond_broadcast(&posixCondition_);
    }


    inline void Condition::wait(Mutex & mutex)
    {
	pthread_cond_wait(&posixCondition_, &(mutex.posixMutex_));
    }


//
// Thread class inline methods
//


    inline Thread::Id Thread::id() const
    {
	return posixThread_;
    }


    inline Thread::Id Thread::self()
    {
	return pthread_self();
    }


    inline void Thread::testStop() const
    {
	pthread_testcancel();
    }

}  // namespace ap


#endif  // BACKEND_BASE_THREAD_H

// Local Variables:
// mode:C++
// End:

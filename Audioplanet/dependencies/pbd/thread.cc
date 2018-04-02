/*
    Copyright (C) 1998-99 Paul Barton-Davis 

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    $Id: thread.cc,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/
#include <sys/time.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

#include <iostream>

#include <pbd/rtthread.h>
#include <pbd/error.h>
#include <pbd/thread.h>

bool QMThread::all_threads_must_die = false;

QMThread::QMThread (const char *name,
		    void *(start)(void *), void *arg, 
		    bool realtime, int rt_priority)

{
	int err = 0;

	pthread_mutex_init (&status_lock, 0);

	lock ();

	_name = name;
	_must_exit = false;
	_running = false;
	_thread_active = false;
	_thread_waiting = false;
	_have_thread = false;
	work_cnt = 0;

	pthread_cond_init (&wake_up, 0);
	pthread_cond_init (&asleep, 0);
	pthread_cond_init (&running, 0);
	pthread_cond_init (&exited, 0);

	if (realtime) {
		err = pthread_create_realtime (&_thread, start, 
					       arg, rt_priority); 
	} 
	
	if (!realtime || err) {
		err = pthread_create (&_thread, 0, start, arg);
	}
	
	if (err) {
		error << "cannot create thread \"" 
		      << name
		      << "\" ("
		      << strerror (err) 
		      << ')' 
		      << endmsg;

	} else {
		_have_thread = true;
	}

	unlock ();
}	

QMThread::~QMThread ()

{
	/* used to call stop then wait. this is cleaner */

	pthread_cancel (_thread);
}

void
QMThread::exit (void *status)

{
	if (!myself()) {
		fatal << "inter-thread kill-off" << endmsg;
	}
	pthread_exit (status);
}

int
QMThread::run ()

{
        /* on return, the caller knows that the thread is actually
	   running.
	*/

        lock ();

	if (!_have_thread) {
		unlock ();
		return -1;
	}

	_must_exit = false;
	_running = true;

	if ((!_thread_active || _thread_waiting) && !myself()) {
	    pthread_cond_signal (&wake_up);
	    pthread_cond_wait (&running, &status_lock);
	    if (!_have_thread) {
		    return -1;
	    }
	}

	unlock ();
	return 0;
}

void
QMThread::poke ()

{
        /* this differs from run() in that on return, the caller
	   does not know that the thread is actually running.
	*/

        lock ();
	_must_exit = false;
	_running = true;

	if (!myself ()) {
		pthread_cond_signal (&wake_up);
	}

	unlock ();
}

void
QMThread::stop ()

{
        if (myself ()) {
		fatal << "QMThread: a thread cannot stop itself by calling stop()" 
		      << endmsg;
	}

        lock ();

	if (!_have_thread) {
		warning << "QMThread: no thread to stop at this point"
			<< endmsg;
		unlock ();
		return;
	}

	_must_exit = true;
	_running = false;

	if (_thread_active) {
		pthread_cond_signal (&wake_up);
		pthread_cond_wait (&exited, &status_lock);
	}

	unlock ();
}

void
QMThread::pause ()

{
        lock();
	_running = false;

	if (!myself ()) {

		if (!_have_thread) {
			warning << "QMThread: no thread to pause at this point" 
				<< endmsg;
			unlock ();
			return;
		}
		
		pthread_cond_signal (&wake_up);
		pthread_cond_wait (&asleep, &status_lock);
		/* thread is now asleep */
	} else {
		/* we'll be asleep soon enough */
	}

	unlock ();
}

void *
QMThread::wait ()

{
	void *status;

	if (myself ()) {
		fatal << "QMThread: a thread cannot wait() for itself"
		      << endmsg;

	}

	if (!_have_thread) {
		warning << "QMThread: no thread to wait for at this point"
			<< endmsg;
	}

	pthread_join (_thread, &status);

	pthread_mutex_destroy (&status_lock);
	pthread_cond_destroy (&wake_up);
	pthread_cond_destroy (&asleep);
	pthread_cond_destroy (&running);
	pthread_cond_destroy (&exited);

	return status;
}

void
QMThread::signal_catcher (int sig)

{
	cerr << "**** Signal " << sig 
	     << " received by thread "
	     << pthread_self()
	     << endl;

	::_exit (1);
}

void
QMThread::setup_signals ()

{
}

void *
QMThread::main ()

{
	void *work_status = 0;
	
	pthread_setcanceltype (PTHREAD_CANCEL_ASYNCHRONOUS, 0);
	
	lock();
	_thread_active = true;
	pthread_cond_signal (&running);
	unlock();

	while (!_must_exit && !all_threads_must_die) {
		while (_running && !all_threads_must_die) {
			work_cnt++;
			if ((work_status = do_work ())) {
				_must_exit = true;
				break;
			}
		}

		if (!_must_exit) {
			lock ();
			_thread_waiting = true;
			pthread_cond_signal (&asleep);
			pthread_cond_wait (&wake_up, &status_lock);
			pthread_cond_signal (&running);
			_thread_waiting = false;
			unlock ();
		}
	}
	
	lock();
	_thread_active = false;
	pthread_cond_signal (&exited);
	unlock();

	return work_status;
}
 


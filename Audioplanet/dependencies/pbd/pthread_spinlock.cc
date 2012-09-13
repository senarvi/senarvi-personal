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

    $Id: pthread_spinlock.cc,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#include <errno.h>
#include <sys/time.h>

#include <pbd/error.h>
#include <pbd/pthread_spinlock.h>

static unsigned int spinlimit = 0;
static unsigned int context_switch_usecs;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t recv_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t send_lock = PTHREAD_MUTEX_INITIALIZER;
static struct timeval cswitch_start;
static struct timeval cswitch_end;
static unsigned int switch_time = 0;

#define LOOPCOUNT 10000

static void *
measure_context_switch (void *arg)

{
	int id = *((int *) arg);
	unsigned int loops;
	bool first_time = true;

	loops = LOOPCOUNT;

	if (id == 0) {
		pthread_mutex_lock (&recv_lock);
		while (--loops) {
			if (first_time) {
				/* tell the main thread that we are ready */
				pthread_cond_signal (&cond);
				first_time = false;
			}
			pthread_cond_wait (&cond, &recv_lock);
			gettimeofday (&cswitch_end, 0);
			
			switch_time += 
				((cswitch_end.tv_sec * 1000000) +
				 cswitch_end.tv_usec) -
				((cswitch_start.tv_sec * 1000000) +
				 cswitch_start.tv_usec);

			
			pthread_mutex_lock (&send_lock);
			pthread_cond_signal (&cond);
			pthread_mutex_unlock (&send_lock);
		}
		pthread_mutex_unlock (&recv_lock);
			
	} else {
		pthread_mutex_lock (&send_lock);

		while (--loops) {
			pthread_mutex_lock (&recv_lock);
			gettimeofday (&cswitch_start, 0);
			pthread_cond_signal (&cond);
			pthread_mutex_unlock (&recv_lock);
			pthread_cond_wait (&cond, &send_lock);
		}
		pthread_mutex_unlock (&send_lock);
		
		context_switch_usecs = switch_time / LOOPCOUNT;

	}

	pthread_exit (0);
	/*NOTREACHED*/
	return 0;
}

unsigned int
pthread_calibrate_spinlimit ()

{
	pthread_t thread1;
	pthread_t thread2;
	struct timeval now, then;
	int id0, id1;
	unsigned int tries;
	unsigned int then_usecs;

	/* RETURNS: number of calls to pthread_try_spinlock() that 
	   can be executed during an average pthread context switch,
	   where that context switch is caused by a condition variable
	   being signalled while a lock is held.
	*/

	/* 1) create two threads, and two condition variables.  then
	      context switch back and forth between them many times
	      and get an average for the context switch time (which
	      includes the time spent sending the signal, which is
	      completely relevant for our purpose). The first thread
	      will store its average context switch time in a global
	      variable.  
	*/

	/* make both threads wait till we're ready to roll */

	pthread_mutex_lock (&recv_lock);

	id0 = 0;
	pthread_create (&thread1, 0, measure_context_switch, &id0);
	pthread_cond_wait (&cond, &recv_lock);
	pthread_mutex_unlock (&recv_lock);

	/* we now know that the receiver thread is waiting for
	   the first cond_signal from the sender thread.
	*/

	id1 = 1;
	pthread_create (&thread2, 0, measure_context_switch, &id1);

	pthread_join (thread1, 0);
	pthread_join (thread2, 0);

	info << "PThreads: average context switch time: " 
	     << context_switch_usecs 
	     << "usecs" 
	     << endmsg;

	/* 2) count how many attempts to take a lock can be executed
	      in the time it takes for an averag single context switch.
	*/

	pthread_mutex_lock (&recv_lock);
	
	gettimeofday (&then, 0);
	then_usecs = (then.tv_sec * 1000000) + then.tv_usec;
	tries = 0;
	while (1) {
		tries++;
		pthread_mutex_trylock (&recv_lock);
		gettimeofday (&now, 0);
		if ((((now.tv_sec * 1000000) + now.tv_usec) -
		     then_usecs ) > context_switch_usecs) {
			break;
		}
	}

	info << "PThreads: "
	     << tries 
	     << " lock attempts per context switch" 
	     << endmsg;

	return tries;
}

void
pthread_set_spinlimit (unsigned int spins)

{
	spinlimit = spins;
}

int
pthread_mutex_spinlock (pthread_mutex_t *mp)

{
	int err = EBUSY;
	unsigned int i;

	i = spinlimit;

	while (i > 0 && ((err = pthread_mutex_trylock (mp)) == EBUSY))
		i--;
	
	if (err == EBUSY) {
		err = pthread_mutex_lock (mp);
	}
	
	return err;
}


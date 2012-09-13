/*
    Copyright (C) 1998 Paul Barton-Davis
    
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

    $Id: rt.cc,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#include <sched.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>

#include <pbd/error.h>
#include <pbd/rt.h>

int 
become_real_time_thread (int rt_priority, bool fifo)

{
    int retval = 1;

    /* Attempts to make the calling thread to use POSIX Real Time scheduling,
       with the specified priority and scheduling class.
       
       Additionally attempts to wire down all current and future memory
       so that no page faulting occurs for the thread's address space.

       NOTE: removes all root priviledges after being called.
    */

    seteuid (0);

    if (geteuid () == 0) {

	struct sched_param sp;

	memset (&sp, 0, sizeof sp);
	sp.sched_priority = rt_priority;

	if (sched_setscheduler (0, fifo ? SCHED_FIFO : SCHED_RR, &sp) != 0) {
	    error << "Error changing to RT class:" 
		  << strerror (errno) 
		  << endmsg;

	    retval = -1;
	    goto cleanup;
	}
	
	if (mlockall (MCL_CURRENT | MCL_FUTURE) != 0) {
	    error << "Error locking pages: " 
		  << strerror (errno) 
		  << endmsg;

	    retval = -1;
	    goto cleanup;
	}

	retval = 0;

      cleanup:
	seteuid (getuid());
    }

    return retval;
}

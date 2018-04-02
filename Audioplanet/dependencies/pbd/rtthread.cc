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

    $Id: rtthread.cc,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <sched.h>
#include <string.h>

#include <pbd/rtthread.h>
#include <pbd/error.h>

int
pthread_create_realtime (pthread_t *new_thread,
			 void *(*start)(void *), void *arg,
			 int priority)

{	
	pthread_attr_t *rt_attributes;
	struct sched_param *rt_param;
	int retval;

	rt_attributes = (pthread_attr_t *) malloc (sizeof (pthread_attr_t));
	rt_param = (struct sched_param *) malloc (sizeof (struct sched_param));

	pthread_attr_init (rt_attributes);

	if (seteuid (0)) {
		retval = errno;
		error << "Cannot obtain root UID for RT scheduling operations"
		      << endmsg;
		return retval;

	} else {

	    if (pthread_attr_setschedpolicy (rt_attributes, SCHED_FIFO)) {
		error << "Cannot set FIFO scheduling attributes for RT thread" << endmsg;

	    }
	    
	    if (pthread_attr_setscope (rt_attributes, PTHREAD_SCOPE_SYSTEM)) {
		error << "Cannot set scheduling scope for RT thread" << endmsg;

	    }
	    
	    rt_param->sched_priority = priority;
	    if (pthread_attr_setschedparam (rt_attributes, rt_param)) {
		error << "Cannot set scheduling priority for RT thread" << endmsg;

	    }
	}

	retval = pthread_create (new_thread, rt_attributes, start, arg);
	seteuid (getuid());

	return retval;
}

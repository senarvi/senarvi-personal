/*
    Copyright (C) 1999 Paul Barton-Davis 

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

    $Id: linux_platform.cc,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

#include <pbd/error.h>
#include <pbd/pthread_spinlock.h>
#include <pbd/linux_platform.h>

static int
count_processors ()

{
	char buf[16];
	FILE *kludge_pipe;
	int nread;

	/* XXX If you know a better way to do this, please substitute it! */

	kludge_pipe = popen ("grep -c '^processor' /proc/cpuinfo", "r");
	if (kludge_pipe == 0) {
		error << "Linux: cannot determine processor count; assume 1"
		      << endmsg;
		return 1;
	}

	nread = fread (buf, sizeof (char), sizeof (buf - 1), kludge_pipe);
	pclose (kludge_pipe);
	buf[nread] = '\0';
	return atoi (buf);
}

static int
smp_kernel ()

{
	char buf[16];
	FILE *kludge_pipe;
	int nread;

	/* XXX If you know a better way to do this, please substitute it! */

	kludge_pipe = popen ("uname -v | grep -c SMP", "r");
	if (kludge_pipe == 0) {
		error << "Linux: cannot determine processor count; assume 1"
		      << endmsg;
		return 1;
	}

	nread = fread (buf, sizeof (char), sizeof (buf - 1), kludge_pipe);
	pclose (kludge_pipe);
	buf[nread] = '\0';
	
	return atoi (buf);
}

LinuxPlatform::LinuxPlatform ()
	: Platform () 
{
}

int
LinuxPlatform::pre_config ()

{
	return 0;
}

int
LinuxPlatform::post_config ()

{
	int nprocs;
	int smp;

	/* figure out if we are running on SMP hardware and have
	   an SMP-aware kernel.
	*/

	nprocs = count_processors ();
	smp = smp_kernel ();

	if (nprocs > 1) {
		if (smp) {
#ifndef CONFIG_WITHOUT_SPINLOCKS
			int spinlimit;

			spinlimit = pthread_calibrate_spinlimit ();
			pthread_set_spinlimit (spinlimit);
#endif
		} else {
			warning << "Linux: you appear to have "
				<< nprocs
				<< " processors, but don't appear to be "
				   "running an SMP kernel"
				<< endmsg;
		}
	}

	if (nprocs > 1) { 
		info << "Linux: running on " << nprocs << " processors";
		if (smp) {
			info << " with SMP kernel";
		} 
	} else {
		info << "Linux: running on 1 processor. Consider a dual CPU system.";
	}

	info << endmsg;

	return 0;
}

int
LinuxPlatform::pre_ui ()

{
	return 0;
}

int
LinuxPlatform::post_ui ()

{
	return 0;
}

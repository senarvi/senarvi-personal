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

    $Id: precision_timer.cc,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#include <cmath>
#include <cstdio>
#include <cerrno>
#include <string>

#include <sys/time.h>
#include <pbd/precision_timer.h>
#include <pbd/error.h>

precision_time_t PrecisionTimer::cycles_per_usec = 0;

int 
PrecisionTimer::get_mhz (void)
{
	FILE *f;

	if ((f = fopen("/proc/cpuinfo", "r")) == 0) {
		fatal << "can't open /proc/cpuinfo ("
		      << strerror (errno)
		      << ')'
		      << endmsg;
		/*NOTREACHED*/
		return 0;
	}

	while (1) {
		int mhz;
		int ret;
		char buf[1000];
		
		if (fgets (buf, sizeof(buf), f) == NULL) {
			fatal << "cannot locate cpu MHz in /proc/cpuinfo"
			      << endmsg;
			/*NOTREACHED*/
			return 0;
		}
		
#ifdef __powerpc__
		ret = sscanf(buf, "clock\t: %dMHz", &mhz);
#else
		ret = sscanf(buf, "cpu MHz         : %d", &mhz);
#endif /* __powerpc__ */

		if (ret == 1) {
			fclose(f);
			return mhz;
		}
	}
}

PrecisionTimer::PrecisionTimer ()
{
	if (cycles_per_usec == 0) {
		cycles_per_usec = get_mhz();
	}
}






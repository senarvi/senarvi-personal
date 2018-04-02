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

    $Id: solaris_platform.cc,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#include <signal.h>
#include <string.h>
#include <errno.h>

// getuid
#include <sys/types.h>
#include <unistd.h>

#include <pbd/solaris_platform.h>
#include <pbd/error.h>

static uid_t normal_uid;

int
SolarisPlatform::pre_config ()

{
	return 0;
}

int
SolarisPlatform::post_config ()

{
	normal_uid = getuid();

	if (need_root) {
		
		if (setuid ((uid_t) 0)) {
			warning << "Solaris: cannot seteuid to super-user ("
				<< strerror (errno)
				<< ')'
				<< endmsg;
		}
	}
	return 0;
}

int
SolarisPlatform::pre_ui ()

{
	/* IMPORTANT: all setuid or other priviledges *MUST* be
	   revoked before starting the UI. Not doing this creates an
	   enormous security hole because some toolkits (e.g. GTK+)
	   essentially allows execution of arbitrary code during its
	   initialization process.  
        */

	seteuid (normal_uid);
	return 0;
}

int
SolarisPlatform::post_ui ()

{
	return 0;
}


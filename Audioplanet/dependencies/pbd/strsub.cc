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

    $Id: strsub.cc,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#include <unistd.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <pbd/strsub.h>

/* return a copy of str, with all occurences of "target"
   of "replacement"
 */

char *
strsub (char *str, const char *target, const char *replacement)

{
	char *p, *cp;
	char *copy;
	int replstart[100];
	int replcount = 0;
	int sizediff;
	int nextrepl;
	int targetlen;
	int replen;
	int slen;

	if (str == 0) {
		return 0;
	}

	p = str;

	memset (replstart,0, 100);
	targetlen = strlen (target);
	replen = strlen (replacement);
	sizediff = targetlen - replen;

	/* First pass: figure out positions where replacement text should
	   be repled, and count the number of repls we're going to do.
	*/

	while (*p) {
		if (*p == *target && memcmp (p, target, targetlen) == 0) {
			replstart[replcount] = p - str - (replcount * sizediff);
			replcount++;
			p += targetlen;
		} else {
			p++;
		}
	}

	slen = (int) (p - str);

	if (replcount == 0) {
		return (strdup (str));
	}
    
	/* rewind, allocate enough space for the copy
	 */
    
	if (sizediff < 0) {
		copy = (char *)malloc (slen + (replcount * -sizediff) + 1);
	} else {
		copy = (char *)malloc (slen + 1);
	}

	copy[0] = '\0';
	cp = copy;
	p = str;
	nextrepl = 0;

	while (*p) {
		if (nextrepl < replcount && 
		    (cp - copy) == replstart[nextrepl]) {

			const char *rp;

			/* insert replacement text here */
	    
			for (rp = replacement; *rp; rp++) {
				*cp++ = *rp;
			}

			/* bump nextrepl index */

			nextrepl++;

			/* move src pointer forward past target */

			p += targetlen;
		} else {
			/* copy original here */

			*cp++ = *p++;
		}
	}

	*cp = '\0';
	return (copy);
}    

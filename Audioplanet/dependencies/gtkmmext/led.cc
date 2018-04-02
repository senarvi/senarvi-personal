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

    $Id: led.cc,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#include <gtkmmext/led.h>
#include <gtkmmext/pix.h>

using namespace std;
using namespace Gtkmmext;

Pix *LED::pix;

LED::LED () : ShapedWindow (pix)
{
	set_events (GDK_ENTER_NOTIFY_MASK|GDK_LEAVE_NOTIFY_MASK|
		    GDK_BUTTON_PRESS_MASK|GDK_BUTTON_RELEASE_MASK);
	blink_base = 0;
	blink_on = true;
	timeout_tag = -1;
}

void
LED::blink ()

{
	if (blink_base > 0) {
		timeout_tag = gtk_timeout_add (500, static_blink, this);
	}
}

void
LED::no_blink ()

{
	if (timeout_tag >= 0) {
		gtk_timeout_remove (timeout_tag);
		timeout_tag = -1;
	}
}

gint
LED::static_blink (gpointer arg)

{
	((LED *) arg)->do_blink ();
	return TRUE;
}

void
LED::do_blink ()

{
	if (blink_on) {
		set_current_pix (0); /* black */
	} else {
		set_current_pix (blink_base);
	}
	blink_on = !blink_on;
}

void
LED::red ()

{
	blink_base = 2;
	no_blink ();
	set_current_pix (2);
}

void
LED::green ()

{
	blink_base = 1;
	no_blink ();
	set_current_pix (1);
}

void
LED::black ()

{
	/* not usable as a blink base */
	no_blink ();
	set_current_pix (0);
}

void
LED::yellow ()

{
	blink_base = 3;
	no_blink ();
	set_current_pix (3);
}

int
LED::get_pix (const string &path)

{
	if ((pix = ::get_pix (path, "[a-zA-Z0-9][a-zA-Z0-9]*-led.xpm$")) == 0) {
		return -1;
	}

	if (pix->n_pixmaps () < 3) {
		return -2;
	}

	pix->ref ();

	return 0;
}
	

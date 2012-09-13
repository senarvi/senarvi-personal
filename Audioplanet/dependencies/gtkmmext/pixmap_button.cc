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

    $Id: pixmap_button.cc,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#include <gtkmmext/pixmap_button.h>
#include <gtkmmext/pix.h>

using namespace Gtkmmext;

PixmapButton::PixmapButton (Pix *pixset, 
			    const char *widget_name,
			    bool pl_sensitive,
			    int bmask) :
    ShapedWindow (pixset)
{
	guint event_mask;

	if (widget_name) {
		set_name (widget_name);
	}

	mode = ClickOnPress;
	prelight_sensitive = pl_sensitive;
	pre_enter_state = get_state();

	event_mask = GDK_BUTTON_PRESS_MASK|GDK_BUTTON_RELEASE_MASK|
		GDK_ENTER_NOTIFY_MASK|GDK_LEAVE_NOTIFY_MASK;

	set_events (event_mask);
	button_mask = bmask;
	active = false;
}

gint
PixmapButton::button_press_event_impl (GdkEventButton *ev)

{
	if ((button_mask & (1<<(ev->button-1))) == 0) {
		return FALSE;
	}

	if (mode == ClickOnPress) {
		set_active (!active);

	        if (ev->type == GDK_2BUTTON_PRESS) {
			double_clicked();
		} else {
			clicked();
		}

	}
	pressed ();
	return TRUE;
}

gint
PixmapButton::button_release_event_impl (GdkEventButton *ev)

{
	if ((button_mask & (1<<(ev->button-1))) == 0) {
		return FALSE;
	}

	if (mode == ClickOnRelease) {
		set_active (!active);
		clicked();
	}

	released ();
	return TRUE;
}

gint
PixmapButton::enter_notify_event_impl (GdkEventCrossing *ev)

{
	if (!prelight_sensitive) {
		return FALSE;
	}

	pre_enter_state = get_state ();
	set_state (GTK_STATE_PRELIGHT);
	return FALSE;
}

gint
PixmapButton::leave_notify_event_impl (GdkEventCrossing *ev)

{
	if (!prelight_sensitive) {
		return FALSE;
	}

        if (get_state() == GTK_STATE_PRELIGHT) {
		set_state (pre_enter_state);
	}
	return FALSE;
}




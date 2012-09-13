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

    $Id: click_box.cc,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#include <stdio.h> /* for sprintf, sigh ... */
#include <gtkmmext/utils.h>
#include <gtkmmext/click_box.h>

using namespace std;
using namespace Gtkmmext;
using namespace SigC;

ClickBox::ClickBox (Gtk::Adjustment *adjp, const string &name)
	: AutoSpin (*adjp)
{
	print_func = default_printer;
	print_arg = 0;

	set_name (name);
	add_events (GDK_BUTTON_RELEASE_MASK|
		    GDK_BUTTON_PRESS_MASK|
		    GDK_ENTER_NOTIFY_MASK|
		    GDK_LEAVE_NOTIFY_MASK);
	set_label ();

	get_adjustment().value_changed.connect (slot(*this,&ClickBox::set_label));
	button_press_event.connect (slot(*(static_cast<AutoSpin*>(this)),&AutoSpin::button_press));
	button_release_event.connect (slot(*(static_cast<AutoSpin*>(this)),&AutoSpin::stop_spinning));

	button_press_event.connect_after (slot (do_not_propagate));
	button_release_event.connect_after (slot (do_not_propagate));
}

ClickBox::~ClickBox ()

{
}

void
ClickBox::default_printer (char buf[32], Gtk::Adjustment &adj, 
			       void *ignored)
{
	sprintf (buf, "%.2f", adj.get_value());
}

void
ClickBox::set_label ()
{
	queue_draw ();
}

gint
ClickBox::expose_event_impl (GdkEventExpose *ev)
{
	/* Why do we do things like this rather than use a Gtk::Label?
	   Because whenever Gtk::Label::set_label() is called, it
	   triggers a recomputation of its own size, along with that
	   of its container and on up the tree. That's intended
	   to be unnecessary here.
	*/

	Gtk::DrawingArea::expose_event_impl (ev);

	if (print_func) {

		char buf[32];
		gint swidth;
		gint sheight;
		gint lbearing;
		gint rbearing;
		gint ascent;
		gint descent;
		gint xoffset, yoffset;

		Gtk::Style *style = get_style();

		print_func (buf, get_adjustment(), print_arg);

		get_window().draw_rectangle (style->get_bg_gc(get_state()),
					     TRUE, 0, 0, -1, -1);

		gdk_string_extents (style->get_font(), 
				    buf,
				    &lbearing,
				    &rbearing,
				    &swidth,
				    &ascent,
				    &descent);
		
		if (swidth > width()) {
			xoffset = 2;
		} else {
			xoffset = (width() - swidth) / 2;
		}

		sheight = ascent+descent;

		if (sheight > height()) {
			yoffset = height() - 2;
		} else {
			yoffset = height() - ((height() - (sheight))/2) - descent;
		}

		get_window().draw_string(style->get_font(), 
					 style->get_fg_gc(get_state()), xoffset, yoffset, buf);
	}

	return TRUE;
}

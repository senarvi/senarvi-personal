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

    $Id: popup.cc,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#include <gtkmmext/popup.h>
#include <gtkmmext/utils.h>

using namespace std;
using namespace Gtkmmext;

PopUp::PopUp (GtkWindowPosition pos, unsigned int showfor_msecs, bool doh)
	  
{
	window = 0;
	window_position = pos;
	delete_on_hide = doh;
	popdown_time = showfor_msecs;
}

gint
PopUp::remove_prompt_timeout (void *arg)

{
	PopUp *pup = (PopUp *) arg;

	pup->remove ();
	return FALSE;
}

void
PopUp::remove ()

{
	if (delete_on_hide) {
		manage (this);
		destroy ();
	} else {
		window->hide ();
	}
}

void
PopUp::touch ()
	
{
	if (window == 0) {
		window = new Gtk::Window (GTK_WINDOW_POPUP);

		if (popdown_time == 0) {
			gtk_widget_set_events 
				(GTK_WIDGET(window->gtkobj()),
				 GDK_BUTTON_RELEASE_MASK);
			window->button_release_event.connect(SigC::slot(this,&PopUp::button_click));
		} 
		
		frame = new Gtk::Frame;
		label = new Gtk::Label;
		window->add (*frame);
		frame->add (*label);
		frame->show ();
		label->show ();
		window->set_position (window_position);
	}

	if (window->is_visible ()) {
		remove ();
	} else {
		set_usize_to_display_given_text
			(*label, my_text.c_str(), 25, 10);
		
		label->set_text (my_text);
		window->show ();

		if (popdown_time != 0) {
			timeout = gtk_timeout_add (popdown_time, 
						   remove_prompt_timeout, 
						   this);
		}


	}
}

PopUp::~PopUp ()

{
	if (window) {
		delete window;
		delete label;
		delete frame;
	}
}

gint
PopUp::button_click (GdkEventButton *ev)

{
	remove ();
	return TRUE;
}

void
PopUp::set_text (const string &txt)

{
	my_text = txt;
}





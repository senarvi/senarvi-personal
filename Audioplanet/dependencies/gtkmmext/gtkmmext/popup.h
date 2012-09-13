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

    $Id: popup.h,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#ifndef __qui_popup_h__
#define __qui_popup_h__

#include <string>
#include <gtk--.h>

#include <pbd/touchable.h>

namespace Gtkmmext {

class PopUp : public Gtk::Misc, public Touchable

{
	Gtk::Window *window;
	Gtk::Frame *frame;
	Gtk::Label *label;
	std::string my_text;
	gint timeout;
	GtkWindowPosition window_position;
	static gint remove_prompt_timeout (void *);
	bool delete_on_hide;
	unsigned int popdown_time;

  public:
	PopUp (GtkWindowPosition pos, unsigned int show_for_msecs = 0,
	       bool delete_on_hide = false);
	virtual ~PopUp ();
	void touch ();
	void remove ();
	void set_text (const std::string &);
	gint button_click (GdkEventButton *);
};

} /* namespace */

#endif  // __qui_popup_h__

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

    $Id: click_box.h,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#ifndef __gtkmmext_click_box_h__
#define __gtkmmext_click_box_h__

#include <string>
#include <gtk--.h>

#include <gtkmmext/auto_spin.h>

namespace Gtkmmext {

class ClickBox : public Gtk::DrawingArea, public AutoSpin
{
  public:
	ClickBox (Gtk::Adjustment *adj, const std::string &name);
	~ClickBox ();

	void set_print_func(void (*pf)(char buf[32], Gtk::Adjustment &, void *),
			    void *arg) {
		print_func = pf;
		print_arg = arg;
		set_label ();
	}

  protected:
	gint expose_event_impl (GdkEventExpose*);

  private:
	void (*print_func) (char buf[32], Gtk::Adjustment &, void *);
	void *print_arg;

	gint button_release (GdkEventButton *);
	void set_label ();
	static void default_printer (char buf[32], Gtk::Adjustment &, void *);
};

} /* namespace */

#endif  /* __gtkmmext_click_box_h__ */



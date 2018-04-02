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

    $Id: shaped_window.h,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#ifndef __gtkmm_shapedwindow_h__
#define __gtkmm_shapedwindow_h__

namespace Gtkmmext {

class Pix;

class ShapedWindow : public Gtk::DrawingArea

{
  private: 
        Gtk::Widget *shapeable_parent;
	Pix *pix;
	const GtkStyle *mystyle;
	size_t current_pix;
	void draw_pixmap ();
	bool exposed;

 protected:
	virtual void realize_impl ();
	virtual gint expose_event_impl (GdkEventExpose *);
	virtual void state_changed_impl (GtkStateType);

  public:
       ShapedWindow (Pix *pix, Gtk::Widget *shapeable_parent = 0);
       virtual ~ShapedWindow();

       void set_current_pix (size_t pixnum);
};

} /* namespace Gtkmmext */

#endif // __gtkmm_shapedwindow_h__

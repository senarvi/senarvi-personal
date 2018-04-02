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

    $Id: textviewer.h,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#ifndef __pbd_gtkmm_textviewer_h__
#define __pbd_gtkmm_textviewer_h__

#include <string>

#include <gtk--.h>
#include <pbd/transmitter.h>

using std::string;

namespace Gtkmmext {

class TextViewer : public Gtk::Window, public Transmitter

{
	Gtk::Text etext;
	Gtk::VBox vbox1;
	Gtk::VBox vbox2;
	Gtk::ScrolledWindow scrollwin;
	Gtk::Button dismiss;
	bool _editable;

	void toggle_edit ();
	void toggle_word_wrap ();

//	friend ostream& ::endmsg (ostream& ostr);

 public:
	TextViewer (size_t width, size_t height);
	Gtk::Text &text() { return etext; }
	void insert_file (const string &);
	void scroll_to_bottom ();
	
	/* this is supposed to be private, but egcs 2.91.66
	   won't allow me to put "friend ... endmsg" into
	   the global namespace, so for now, its public.
	*/

	void deliver ();
};

} /* namespace */

#endif  // __pbd_gtkmm_textviewer_h__

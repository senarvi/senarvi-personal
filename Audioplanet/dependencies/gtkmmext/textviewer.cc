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

    $Id: textviewer.cc,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#include <string>
#include <fstream>

#include <gdk/gdk.h>
#include <gtkmmext/textviewer.h>

using namespace std;
using namespace Gtkmmext;

TextViewer::TextViewer (size_t xsize, size_t ysize) :
	Gtk::Window (GTK_WINDOW_TOPLEVEL),
	Transmitter (Transmitter::Info), /* channel arg is irrelevant */
	dismiss ("Close")
{
	set_usize (xsize, ysize);

	set_title ("Text Viewer");
	set_name ("TextViewer");
	set_policy (TRUE, TRUE, FALSE);
	set_border_width (0);

	vbox1.set_homogeneous (false);
	vbox1.set_spacing (0);
	add (vbox1);
	vbox1.show ();

	vbox2.set_homogeneous (false);
	vbox2.set_spacing (10);
	vbox2.set_border_width (10);

	vbox1.pack_start (vbox2, true, true, 10);
	vbox2.show ();

	vbox2.pack_start (scrollwin, TRUE, TRUE, 0);
	scrollwin.set_policy (GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
	scrollwin.show ();

	etext.set_editable (false);
	etext.set_word_wrap (true);
	scrollwin.add (etext);
	etext.show ();

	vbox1.pack_start (dismiss, false, false, 0);
	dismiss.show ();

	dismiss.released.connect(hide.slot());
}

void
TextViewer::insert_file (const string &path)

{
	char buf[1024];
	ifstream f (path.c_str());
	Gtk::Text_Helpers::Context context;

	if (!f) {
		return;
	}

	etext.freeze ();

	while (f) {
		f.read (buf, sizeof (buf));

		if (f.gcount()) {
			buf[f.gcount()] = '\0';
			string foo (buf);
			etext.insert (context, foo);
		}
	}

	etext.thaw ();
}

void
TextViewer::scroll_to_bottom ()

{
        GtkAdjustment *adj;

 	adj = (GtkAdjustment *) scrollwin.get_vadjustment()->gtkobj();
 	gtk_adjustment_set_value (adj, MAX(0,(adj->upper - adj->page_size)));
}
	
void
TextViewer::deliver ()

{
	char buf[1024];
	Gtk::Text_Helpers::Context context;

	while (!eof()) {
		read (buf, sizeof (buf));
		buf[gcount()] = '\0';
		string foo (buf);
		etext.insert (context, foo);
	}
	scroll_to_bottom ();
	clear ();
}

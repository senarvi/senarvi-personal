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

    $Id: prompter.cc,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#include <string>

#include <gtkmmext/prompter.h>

using namespace std;
using namespace Gtkmmext;

Prompter::Prompter (bool modal)
	: Gtk::Window (GTK_WINDOW_DIALOG),
	  ok ("OK"),
	  cancel ("Cancel")
{
	set_position (GTK_WIN_POS_MOUSE);
	set_name ("Prompter");
	set_modal (modal);

	add (packer);

	entryLabel.set_line_wrap (true);
	entryLabel.set_name ("PrompterLabel");

	entryBox.set_homogeneous (false);
	entryBox.set_spacing (25);
	entryBox.set_border_width (10);
	entryBox.pack_start (entryLabel);
	entryBox.pack_start (entry, false, false);

	buttonBox.set_homogeneous (true);
	buttonBox.set_border_width (10);
	buttonBox.pack_start (ok, false, true);
	buttonBox.pack_start (cancel, false, true);
	
	packer.pack_start (entryBox);
	packer.pack_start (buttonBox);

	entry.activate.connect(SigC::slot(this,&Prompter::activated));
	ok.clicked.connect(SigC::slot(this,&Prompter::activated));
	cancel.clicked.connect(SigC::slot(this,&Prompter::cancel_click));
	delete_event.connect (slot (this, &Prompter::deleted));
}	

void
Prompter::change_labels (string okstr, string cancelstr)
{
	dynamic_cast<Gtk::Label*>(ok.get_child())->set_text (okstr);
	dynamic_cast<Gtk::Label*>(cancel.get_child())->set_text (cancelstr);
}

void
Prompter::realize_impl ()
{
	Gtk::Window::realize_impl ();
	Gdk_Window win = get_window ();
	win.set_decorations (GdkWMDecoration (GDK_DECOR_BORDER|GDK_DECOR_RESIZEH|GDK_DECOR_MENU));
}

void
Prompter::map__impl ()
{
	entry.grab_focus();
	Gtk::Window::map__impl ();
}
	
void
Prompter::activated ()

{
	status = entered;
	hide_all ();
	done ();
}

void
Prompter::cancel_click ()

{
	entry.set_text ("");
	status = cancelled;
	hide_all ();
	done ();
}

gint
Prompter::deleted (GdkEventAny *ev)
{
	cancel_click ();
	return FALSE;
}

void
Prompter::get_result (string &str)

{
	str = entry.get_text ();
}

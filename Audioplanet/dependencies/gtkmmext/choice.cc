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

    $Id: choice.cc,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#include <gtkmmext/choice.h>

using namespace std;
using namespace Gtkmmext;

Choice::Choice (const string &prompt,
		unsigned int nch,
		const string *choices,
		bool delete_at_done) :
	Gtk::Window (GTK_WINDOW_DIALOG),
        prompt_label (prompt),
	delete_when_done (delete_at_done)

{
	int i;
	
	nchoices = nch;

	set_title ("I've just got to ask ...");
	set_position (GTK_WIN_POS_MOUSE);
	add (packer);
	packer.pack_start (prompt_label);
	packer.pack_start (button_packer);
	prompt_label.set_name ("ChoicePrompt");
	buttons = new Gtk::Button * [nchoices+1];
	
	for (i = 0; i < nchoices; i++) {
		buttons[i] = new Gtk::Button (choices[i]);
		buttons[i]->set_name ("ChoiceButton");
		button_packer.pack_start (*buttons[i]);
		buttons[i]->clicked.connect
			(SigC::bind(SigC::slot(this,&Choice::_choice_made),i));
	}

	buttons[i] = new Gtk::Button ("Cancel");
	buttons[i]->set_name ("ChoiceButton");
	packer.pack_start (*buttons[i]);
	buttons[i]->clicked.connect
		(SigC::bind(SigC::slot(this,&Choice::_choice_made),i));

	delete_event.connect(SigC::slot(this,&Choice::closed));

	packer.show_all ();
}

Choice::~Choice ()

{
	int i;

	for (i = 0; i <= nchoices; i++) {
		delete buttons[i];
	}

	delete buttons;
}

void
Choice::_choice_made (int nbutton)

{
	if (delete_when_done) {
		manage (this);
		destroy ();
	} else {
		hide_all ();
	}

	choice_made (nbutton == nchoices ? -1 : nbutton);
}

gint
Choice::closed (GdkEventAny *ev)

{
	choice_made (nchoices);
	return TRUE;
}



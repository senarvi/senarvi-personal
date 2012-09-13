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

    $Id: popup_selector.cc,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#include <gtkmmext/popup_selector.h>

using namespace Gtkmmext;

PopupSelector::PopupSelector (const char *title,
			      SelectorRefillFunction func,
			      void *arg,
			      bool delete_at_cancel,
			      size_t ncols) :
	Gtk::Window (GTK_WINDOW_DIALOG),
        cancel ("Close"),
	accept ("OK"),
        rescan ("Rescan"),
	selector (func, arg, ncols, true, true),
	delete_on_cancel (delete_at_cancel)

{
	set_title (title);
	set_position (GTK_WIN_POS_MOUSE);
	set_policy (true, true, true);

	add (vpacker);
	vpacker.pack_start (selector, true, true);
	vpacker.pack_start (hpacker, false, false);
	vpacker.pack_start (cancel, false, false);

	hpacker.pack_start (accept, true, true);
	hpacker.pack_start (rescan, true, true);

	selector.selection_made.connect
		(SigC::slot(this,&PopupSelector::selector_made_selection));
	cancel.clicked.connect(SigC::slot(this,&PopupSelector::cancelled));
	accept.clicked.connect(SigC::slot(&selector,&Selector::accept));
	rescan.clicked.connect(SigC::slot(&selector,&Selector::rescan));

	vpacker.show_all ();
}

gint
PopupSelector::delete_event_impl (GdkEventAny *ev)

{
	cancelled ();
	return TRUE;
}

gint
PopupSelector::map_event_impl (GdkEventAny *ev)

{
	selector.rescan ();
	return Gtk::Window::map_event_impl (ev);
}

void
PopupSelector::cancelled ()

{
	SelectionResult result;
	selection_made (this, &result);

	if (delete_on_cancel) {
		manage (this);
		destroy ();
	} else {
		hide ();
	}
}

void
PopupSelector::selector_made_selection (Selector *sel, 
					SelectionResult *res)
	
{
	selection_made (this, res);
}

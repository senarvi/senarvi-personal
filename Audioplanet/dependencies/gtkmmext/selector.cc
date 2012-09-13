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

    $Id: selector.cc,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#include <algorithm>
#include <functional>
#include <vector>
#include <string>

#include <gdk/gdk.h>
#include <gtkmmext/selector.h>
#include <gtkmmext/utils.h>
#include <pbd/pathscanner.h>
#include <pbd/error.h>

using namespace std;
using namespace Gtkmmext;

Selector::Selector (void (*func)(Gtk::CList &, void *), void *arg,
		    size_t ncols, bool hscroll, bool vscroll,
		    const gchar **titles) 
	: list (ncols, titles)

{
	scroll.add (list);

	scroll.set_policy (
		hscroll ? 
		GTK_POLICY_AUTOMATIC : GTK_POLICY_NEVER,
		vscroll ?
		GTK_POLICY_AUTOMATIC : GTK_POLICY_NEVER);

	pack_start (scroll, true, true);

	list.select_row.connect(SigC::slot(this,&Selector::row_clicked));
	show.connect(SigC::slot(this,&Selector::rescan));
	update_contents.connect(SigC::slot(this,&Selector::rescan));

	list.set_selection_mode (GTK_SELECTION_SINGLE);
	list.column_titles_passive ();
	list.show ();

	refiller = func;
	refill_arg = arg;
	selected_row = -1;
	selected_column = -1;
}

Selector::~Selector ()

{
	/* ensure that any row data set with set_row_data_full() is deleted */
	hide_all ();
	list.clear ();
}

void
Selector::map__impl ()

{
	Gtk::VBox::map__impl ();

	selected_row = -1;
	selected_column = -1;
	refill();
}

void
Selector::reset (void (*func)(Gtk::CList &, void *), void *arg)

{
	refiller = func;
	refill_arg = arg;
	selected_row = -1;
	selected_column = -1;

	refill();
}

void
Selector::refill ()

{
	if (refiller) {
		list.freeze ();
		list.clear ();
		refiller (list, refill_arg);
		list.thaw ();
	}
}

void
Selector::row_clicked (gint row, gint col, GdkEvent *ev)

{
	if (ev && (ev->type == GDK_BUTTON_RELEASE || ev->type == GDK_2BUTTON_PRESS)) {
		GdkEventButton* press = (GdkEventButton*) ev;

		if (press->state & GDK_CONTROL_MASK) {
			chosen_row = row;
			chosen_column = col;
			gtk_idle_add (Selector::_control_clicked, this);
		} else if (press->state & GDK_SHIFT_MASK) {
			chosen_row = row;
			chosen_column = col;
			gtk_idle_add (Selector::_shift_clicked, this);
		} else if (press->type == GDK_2BUTTON_PRESS) {
			selected_row = row;
			selected_column = col;
			gtk_idle_add (Selector::_accept, this);
		} else {
			chosen_row = row;
			chosen_column = col;
			gtk_idle_add (Selector::_chosen, this);
		}
	}
}

gint
Selector::_accept (gpointer arg)

{
	((Selector *) arg)->accept ();
	return FALSE;
}

gint
Selector::_chosen (gpointer arg)

{
	((Selector *) arg)->chosen ();
	return FALSE;
}

gint
Selector::_shift_clicked (gpointer arg)
{
	((Selector *) arg)->shift_clicked ();
	return FALSE;
}

gint
Selector::_control_clicked (gpointer arg)
{
	((Selector *) arg)->control_clicked ();
	return FALSE;
}

void
Selector::accept ()

{
	Gtk::nstring buf;
	SelectionResult result;


	if (selected_row >= 0) {
		
		/* XXX if Gtk::nstring wasn't used here, we could
		   avoid the copy ....
		*/

	  again:
		if (list.get_text (selected_row, selected_column,&buf)) {
			result.text = new string (buf);
			result.data = list.get_row_data(selected_row);
			result.row = selected_row;
			result.column = selected_column;
			selection_made (this, &result);
		}

		selected_row = -1;
		selected_column = -1;
		
	} else if (chosen_row >= 0) {

		selected_row = chosen_row;
		selected_column = chosen_column;
		chosen_row = -1;
		chosen_column = -1;
		goto again;

	} else {
		cancel ();
	}
}

void
Selector::chosen ()

{
	Gtk::nstring buf;
	SelectionResult result;

	if (chosen_row >= 0) {
		
		/* XXX if Gtk::nstring wasn't used here, we could
		   avoid the copy ....
		*/

		if (list.get_text (chosen_row, chosen_column,&buf)) {
			result.text = new string (buf);
			result.data = list.get_row_data(chosen_row);
			result.row = chosen_row;
			result.column = chosen_column;
			choice_made (this, &result);
		}

	}
}

void
Selector::shift_clicked ()

{
	Gtk::nstring buf;
	SelectionResult result;

	if (chosen_row >= 0) {
		
		/* XXX if Gtk::nstring wasn't used here, we could
		   avoid the copy ....
		*/

		if (list.get_text (chosen_row, chosen_column,&buf)) {
			result.text = new string (buf);
			result.data = list.get_row_data(chosen_row);
			result.row = chosen_row;
			result.column = chosen_column;
			shift_made (this, &result);
		}

	}
}

void
Selector::control_clicked ()

{
	Gtk::nstring buf;
	SelectionResult result;

	if (chosen_row >= 0) {
		
		/* XXX if Gtk::nstring wasn't used here, we could
		   avoid the copy ....
		*/

		if (list.get_text (chosen_row, chosen_column,&buf)) {
			result.text = new string (buf);
			result.data = list.get_row_data(chosen_row);
			result.row = chosen_row;
			result.column = chosen_column;
			control_made (this, &result);
		}

	}
}

void
Selector::cancel ()

{
	SelectionResult foo;
	foo.text = new string ("");
	foo.data = NULL;

	selection_made (this, &foo);
}

void
Selector::rescan ()

{
	selected_row = -1;
	selected_column = -1;
	refill ();
	show_all ();
}

struct string_cmp {
    bool operator()(const string* a, const string* b) {
	    return *a < *b;
    }
};

void
Gtkmmext::dirpath_lister (Gtk::CList &list, const char *dirpath,
			  bool (*filter)(const string &, void *), 
			  void *arg, 
			  bool with_suffix)

{
	vector<string *> *matched_files;
	vector<string *>::iterator i;
	PathScanner scanner;
	int row;
	string_cmp cmp;

	matched_files = scanner (dirpath, filter, arg, false, true);

	sort (matched_files->begin(), matched_files->end(), cmp);

	if (matched_files == 0) {
		return;
	}

	if (matched_files->size() == 0) {
		delete matched_files;
		error << "No files found along "
		      << dirpath
		      << endmsg;
		return;
	}

	for (row = 0, i = matched_files->begin(); 
	     i != matched_files->end(); 
	     row++, i++) {

		const gchar *rowdata[1];
		string &fullpath = *(*i);
		int start, end;

		start = fullpath.find_last_of ('/') + 1;
		
		if (with_suffix) {
			end = fullpath.length();
		} else {
			if ((end = fullpath.find_last_of ('.')) < 0) {
				end = fullpath.length();
			}
		}
		
		string foo = fullpath.substr (start, (end-start));
		rowdata[0] = foo.c_str();
		list.insert_row (row, rowdata);
		list.set_row_data_full (row, *i, deferred_delete<string>);
	}
}

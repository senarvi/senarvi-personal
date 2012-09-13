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

    $Id: selector.h,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#ifndef __gtkselector_h__
#define __gtkselector_h__

#include <string>

#include <gtk--.h>

namespace Gtkmmext {

typedef void (SelectorRefillFunction)(Gtk::CList&, void *);

struct SelectionResult {
	int row;
	int column;
    const std::string *text;
	void *data;

	SelectionResult () {
		row = -1;
		column = -1;
		text = 0;
		data = 0;
	}
	
	~SelectionResult() { 
		if (text) delete text;
	}
};

class Selector : public Gtk::VBox
{
public:
	Selector (SelectorRefillFunction, void *arg, 
		      size_t columns = 1,
		      bool hscroll = false, 
		      bool vscroll = true,
		      const gchar **titles = 0);
	virtual ~Selector ();
	Gtk::CList &clist () { return list; }
	void reset (void (*refiller)(Gtk::CList &, void *), void *arg);
	void set_size (unsigned int w, unsigned int h) {
		scroll.set_usize (w, h);
		list.set_column_width (1, w);
	}

	/* selection is activated via a double click, choice via
	   a single click.
	*/

	SigC::Signal2<void,Selector *, SelectionResult *> selection_made;
	SigC::Signal2<void,Selector *, SelectionResult *> choice_made;
	SigC::Signal2<void,Selector *, SelectionResult *> shift_made;
	SigC::Signal2<void,Selector *, SelectionResult *> control_made;

	SigC::Signal0<void> update_contents;

	void accept();
	void cancel();
	void rescan();

  protected:
	virtual void map__impl ();

  private:
	Gtk::ScrolledWindow scroll;
	Gtk::CList list;
	void (*refiller)(Gtk::CList &, void *);
	void *refill_arg;
	gint selected_row;
	gint selected_column;
	gint chosen_row;
	gint chosen_column;

	void row_clicked (gint row, gint col, GdkEvent *ev);

	void refill ();
	void chosen ();
	void shift_clicked ();
	void control_clicked ();

	static gint _accept (gpointer);
	static gint _chosen (gpointer);
	static gint _shift_clicked (gpointer);
	static gint _control_clicked (gpointer);

};

void  dirpath_lister (Gtk::CList &list, const char *dirpath,
		      bool (*filter)(const std::string &, void *), void *arg, 
		      bool with_suffix);

} /* namespace */

#endif // __gtkselector_h__



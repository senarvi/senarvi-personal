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

    $Id: shaped_window.cc,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#include <gtk--.h>
#include <gtkmmext/pix.h>
#include <gtkmmext/shaped_window.h>

using namespace Gtkmmext;

ShapedWindow::ShapedWindow (Pix *pixset, Gtk::Widget *w)

{
	pix = pixset;
	current_pix = 0;
	exposed = false;
	shapeable_parent = w;
}

ShapedWindow::~ShapedWindow ()

{
	finish_pix (pix);
}

void
ShapedWindow::set_current_pix (size_t n)

{
	n = MIN(n,pix->max_pixmap());

	if (n != current_pix) {
		current_pix = n;
		draw_pixmap ();
	}
}

void
ShapedWindow::realize_impl ()

{
	gint width, height;
	Gtk::Widget *parent;

	Gtk::DrawingArea::realize_impl ();

	mystyle = get_style()->gtkobj();

	if (!shapeable_parent) {
		parent = this;
	} else {
		parent = shapeable_parent;
	}
	
	pix->generate (get_window(),
		       (GdkColor *) &mystyle->bg[GTK_STATE_NORMAL]);
	
	gdk_window_get_size (pix->pixmap (0), &width, &height);
	set_usize (width, height);

	gtk_widget_shape_combine_mask (GTK_WIDGET(parent->gtkobj()),
				       pix->shape_mask (0),
				       0, 0);

	exposed = true;
	draw_pixmap ();
}

gint
ShapedWindow::expose_event_impl (GdkEventExpose *ev)

{
	if (ev->count == 0) {
		draw_pixmap ();
	}
	return TRUE;
}

void
ShapedWindow::state_changed_impl (GtkStateType somestate)

{
	current_pix = (size_t) get_state() % pix->n_pixmaps();
	draw_pixmap();
}

void
ShapedWindow::draw_pixmap ()

{
	if (exposed) {

		if (!pix->homogenous()) {
			Gtk::Widget *parent;
			
			if (!shapeable_parent) {
				parent = this;
			} else {
				parent = shapeable_parent;
			}

			gtk_widget_shape_combine_mask 
				(GTK_WIDGET(parent->gtkobj()),
				 pix->shape_mask (current_pix),
				 0, 0);
		}

		gdk_draw_pixmap (get_window(),
				 mystyle->bg_gc[get_state()],
				 pix->pixmap (current_pix),
				 0, 0, 0, 0,
				 -1, -1);
	}
}

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

    $Id: gtkutils.c,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#include <gtk/gtk.h>
#include <gtkmmext/gtkutils.h>

void
gtk_widget_set_usize_to_display_given_text (GtkWidget *w,
					    const gchar *text,
					    gint hpadding,
					    gint vpadding)

{
	guint width;
	guint lbearing;
	guint rbearing;
	guint ascent;
	guint descent;
	
	gdk_string_extents (w->style->font,
			    text,
			    &lbearing,
			    &rbearing,
			    &width,
			    &ascent,
			    &descent);

	if (vpadding >= 0) {
		vpadding += ascent+descent;
	} 

	if (hpadding >= 0) {
		hpadding += width;
	}

	gtk_widget_set_usize (w, hpadding, vpadding);
}

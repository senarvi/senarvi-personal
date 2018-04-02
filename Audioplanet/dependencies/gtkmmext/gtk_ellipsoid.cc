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

    $Id: gtk_ellipsoid.cc,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#include <gtkmmext/gtk_ellipsoid.h>

using namespace Gtkmmext;

void
Gtkmmext::Ellipsoid::transparent_draw (Gtk::Transparency &t, GdkGC *gc)

{
	if (!ready()) {
		return;
	}

	compute ();

	gtk_transparency_draw_arc (t.gtkobj(), gc,
				   FALSE,
				   arc[0].rect_x,
				   arc[0].rect_y,
				   arc[0].rect_w,
				   arc[0].rect_h,
				   arc[0].start_angle,
				   arc[0].counter_clockwise ?
				   arc[0].arc_angle : -arc[0].arc_angle);
	
	gtk_transparency_draw_arc (t.gtkobj(), gc,
				   FALSE,
				   arc[1].rect_x,
				   arc[1].rect_y,
				   arc[1].rect_w,
				   arc[1].rect_h,
				   arc[1].start_angle,
				   arc[1].counter_clockwise ?
				   arc[1].arc_angle : -arc[1].arc_angle);
}
	
void
Gtkmmext::Ellipsoid::draw (Gdk_Window win, GdkGC *gc)

{
	if (!ready()) {
		return;
	}

	compute ();

	gdk_draw_arc (win, gc,
		      FALSE,
		      arc[0].rect_x,
		      arc[0].rect_y,
		      arc[0].rect_w,
		      arc[0].rect_h,
		      arc[0].start_angle,
		      arc[0].counter_clockwise ?
		      arc[0].arc_angle : -arc[0].arc_angle);
	
	gdk_draw_arc (win, gc,
		      FALSE,
		      arc[1].rect_x,
		      arc[1].rect_y,
		      arc[1].rect_w,
		      arc[1].rect_h,
		      arc[1].start_angle,
		      arc[1].counter_clockwise ?
		      arc[1].arc_angle : -arc[1].arc_angle);
}


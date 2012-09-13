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

    $Id: gtk_ellipsoid.h,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#ifndef __qm_gtk_ellipsoid_h__
#define __qm_gtk_ellipsoid_h__

#include <pbd/ellipsoid.h>
#include <gtkmmext/transparency.h>

namespace Gtkmmext {

class Ellipsoid : public ::Ellipsoid 

{
 public:
	void draw (Gdk_Window win, GdkGC *gc);
	void transparent_draw (Gtk::Transparency &t, GdkGC *gc);
};

} /* namespace */

#endif  // __qm_gtk_ellipsoid_h__

/*
    Copyright (C) 2000 Paul Barton-Davis 

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

    $Id: led.h,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#ifndef __gtkmmext_led_h__
#define __gtkmmext_led_h__

#include <string>

#include <gtk--.h>
#include <gtkmmext/shaped_window.h>

namespace Gtkmmext {

class LED : public ShapedWindow

{
  public:
	LED ();

	void black ();
	void red ();
	void green ();
	void yellow ();

	/* ... more to come ? */

	void blink();
	void no_blink ();

	static int get_pix (const std::string &);

  private:
	static Pix *pix;
	int blink_base;
	gint timeout_tag;
	bool blink_on;

	void do_blink ();
	static gint static_blink (gpointer);
};

} /* namespace */

#endif  // __gtkmmext_led_h__

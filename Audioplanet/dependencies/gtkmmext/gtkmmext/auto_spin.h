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

    $Id: auto_spin.h,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#ifndef __gtkmmext_auto_spin_h__
#define __gtkmmext_auto_spin_h__

#include <gtk--.h>

namespace Gtkmmext {

class AutoSpin : public SigC::Object

{
  public:
	AutoSpin (Gtk::Adjustment &adj, gfloat cr = 0);

	Gtk::Adjustment &get_adjustment() { return adjustment; }

	void use_left_as_decrement (bool yn) { left_is_decrement = yn; }
	void set_wrap (bool yn) { wrap = yn; }
	void set_climb_rate (gfloat cr) { climb_rate = cr; }
	void set_bounds (gfloat initial, gfloat low, gfloat high,
			 bool with_reset = true);

	gint button_press (GdkEventButton *);
	gint stop_spinning (GdkEventButton *ignored_but_here_for_clicked);
	void start_spinning (bool decrementing, bool use_page);

  private:
	Gtk::Adjustment &adjustment;
	gfloat climb_rate;
	gfloat timer_increment;
	gfloat initial;
	unsigned int timer_calls;
	bool have_timer;
	bool need_timer;
	bool wrap;
	gint timeout_tag;
	bool left_is_decrement;

	static const unsigned int initial_timer_interval;
	static const unsigned int timer_interval;
	static const unsigned int climb_timer_calls;

	void stop_timer ();
	static gint _timer (void *arg);
	gint timer ();
	bool adjust_value (gfloat increment);
	void set_value (gfloat value);
};

} /* namespace */
	
#endif /* __gtkmmext_auto_spin_h__ */

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

    $Id: motionfeedback.h,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/
#ifndef __qm_motion_feedback_h__
#define __qm_motion_feedback_h__

#include <gtkmmext/pix.h>
#include <gtkmmext/shaped_window.h>

namespace Gtkmmext {

class MotionFeedback : public Gtk::VBox

{
 public:
	MotionFeedback (Pix *pixset, const char *widget_name = NULL,
			Gtk::Adjustment *adj = NULL, 
			bool with_numeric_display = true);
	virtual ~MotionFeedback ();

	void set_adjustment (Gtk::Adjustment *adj);
	Gtk::Adjustment *get_adjustment () { return adjustment; }

	Gtk::Widget &eventwin () { return pixwin; }

	gfloat lower () { return _lower; }
	gfloat upper () { return _upper; }
	gfloat range () { return _range; }

 protected:
	gfloat _range;
	gfloat _lower;
	gfloat _upper;

	gint pixwin_button_press_event (GdkEventButton *);
	gint pixwin_button_release_event (GdkEventButton *);
	gint pixwin_motion_notify_event (GdkEventMotion *);
	gint pixwin_key_press_event (GdkEventKey *);
	gint pixwin_enter_notify_event (GdkEventCrossing *);
	gint pixwin_leave_notify_event (GdkEventCrossing *);
	gint pixwin_focus_in_event (GdkEventFocus*);
	gint pixwin_focus_out_event (GdkEventFocus *);

  private:
	ShapedWindow pixwin;
	Gtk::SpinButton   *value;
	Gtk::Adjustment   *adjustment;
	
	gfloat step_inc;
	gfloat page_inc;
	size_t max_pixmap;
	gfloat current_step;
	size_t current_pix_index;
	bool   grab_is_fine;
	gdouble grabbed_y;
	gdouble grabbed_x;

	void adjustment_changed ();
	int value_to_pix_index (gfloat);

	bool i_own_my_adjustment;
};

} /* namespace */
	
#endif // __qm_motion_feedback_h__

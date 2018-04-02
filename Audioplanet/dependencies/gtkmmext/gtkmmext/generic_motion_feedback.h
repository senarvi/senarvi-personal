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

    $Id: generic_motion_feedback.h,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/
#ifndef __qm_generic_motion_feedback_h__
#define __qm_generic_motion_feedback_h__

#include <gtkmmext/pix.h>
#include <gtkmmext/shaped_window.h>

class GenericMotionFeedback : public Gtk::VBox

{
 public:
	GenericMotionFeedback (Gtk::Widget &, 
			       const char *widget_name = NULL,
			       Gtk::Adjustment *adj = NULL,
			       gfloat y_scale_factor = 10);
	virtual ~GenericMotionFeedback ();

	void set_adjustment (Gtk::Adjustment *adj);
	Gtk::Adjustment *get_adjustment () const { return adjustment; }

	Gtk::Widget &eventwin () const { return widget; }

	gfloat lower () const { return _lower; }
	gfloat upper () const { return _upper; }
	gfloat range () const { return _range; }

	gfloat yscale ()const { return _yscale; }
	void set_yscale (gfloat f) { _yscale = f; }

 protected:
	gfloat _yscale;
	gfloat _range;
	gfloat _lower;
	gfloat _upper;

	gint widget_button_press_event (GdkEventButton *);
	gint widget_button_release_event (GdkEventButton *);
	gint widget_motion_notify_event (GdkEventMotion *);
	gint widget_key_press_event (GdkEventKey *);
	gint widget_enter_notify_event (GdkEventCrossing *);
	gint widget_leave_notify_event (GdkEventCrossing *);
	gint widget_focus_in_event (GdkEventFocus*);
	gint widget_focus_out_event (GdkEventFocus *);

  private:
	Gtk::Widget       &widget;
	Gtk::Adjustment   *adjustment;
	
	gfloat step_inc;
	gfloat page_inc;
	gfloat current_step;
	bool   grab_is_fine;
	gdouble grabbed_y;
	gdouble grabbed_x;

	bool i_own_my_adjustment;
};
	
#endif // __qm_generic_motion_feedback_h__

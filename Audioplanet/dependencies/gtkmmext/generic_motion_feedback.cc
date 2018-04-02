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

    $Id: generic_motion_feedback.cc,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#include <math.h>
#include <unistd.h>

#include <gdk/gdkkeysyms.h>
#include <gtk--.h>
#include <gtkmmext/pix.h>
#include <gtkmmext/shaped_window.h>
#include <gtkmmext/generic_motion_feedback.h>


GenericMotionFeedback::GenericMotionFeedback (Gtk::Widget &widg,
					      const char *widget_name, 
					      Gtk::Adjustment *adj,
					      gfloat yscale_factor) :
	_yscale (yscale_factor),
	widget (widg)

{
	set_homogeneous (false);

	if (adj == NULL) {
	    i_own_my_adjustment = true;
	    set_adjustment (new Gtk::Adjustment (0, 0, 10000, 1, 10, 0));
	} else {
	    i_own_my_adjustment = false;
	    set_adjustment (adj);
	}

	set_spacing (3);
	pack_start (widget, false, true);

	current_step = adjustment->gtkobj()->value;

	widget.add_events (GDK_BUTTON_PRESS_MASK|
			   GDK_BUTTON_RELEASE_MASK|
			   GDK_POINTER_MOTION_MASK|
			   GDK_ENTER_NOTIFY_MASK|
			   GDK_LEAVE_NOTIFY_MASK|
			   GDK_KEY_PRESS_MASK|
			   GDK_KEY_RELEASE_MASK);

	widget.set_flags (GTK_CAN_FOCUS);

	/* Proxy all important events on the pixwin to ourselves */

	widget.button_press_event.connect(SigC::slot(this,&GenericMotionFeedback::widget_button_press_event));
	widget.button_release_event.connect(SigC::slot(this,&GenericMotionFeedback::widget_button_release_event));
	widget.motion_notify_event.connect(SigC::slot(this,&GenericMotionFeedback::widget_motion_notify_event));
	widget.enter_notify_event.connect(SigC::slot(this,&GenericMotionFeedback::widget_enter_notify_event));
	widget.leave_notify_event.connect(SigC::slot(this,&GenericMotionFeedback::widget_leave_notify_event));
	widget.key_press_event.connect(SigC::slot(this,&GenericMotionFeedback::widget_key_press_event));
}

GenericMotionFeedback::~GenericMotionFeedback()

{
	if (i_own_my_adjustment) {
		delete adjustment;
	}
}

void
GenericMotionFeedback::set_adjustment (Gtk::Adjustment *adj)

{
	adjustment = adj;

	_lower = adj->gtkobj()->lower;
	_upper = adj->gtkobj()->upper;
	_range = _upper - _lower;
	step_inc = adj->gtkobj()->step_increment;
	page_inc = adj->gtkobj()->page_increment;
}

gint 
GenericMotionFeedback::widget_button_press_event (GdkEventButton *ev) 
{ 
	switch (ev->button) {
	case 2:
		return FALSE;  /* XXX why ? */

	case 1:
		grab_is_fine = false;
		break;
	case 3:
		grab_is_fine = true;
		break;
	}

	gtk_grab_add(GTK_WIDGET(widget.gtkobj()));
	grabbed_y = ev->y_root;
	grabbed_x = ev->x_root;

	/* XXX should we return TRUE ? */

	return FALSE;
}

gint 
GenericMotionFeedback::widget_button_release_event (GdkEventButton *ev) 
{ 
	switch (ev->button) {
	case 1:
		if (widget.has_grab()) {
			if (!grab_is_fine) {
				gtk_grab_remove
					(GTK_WIDGET(widget.gtkobj()));
			}
		}
		break;
		
	case 3:
		if (widget.has_grab()) {
			if (grab_is_fine) {
				gtk_grab_remove
					(GTK_WIDGET(widget.gtkobj()));
			}
		}
		break;
	}

	return Gtk::VBox::button_release_event_impl(ev); 
}

gint 
GenericMotionFeedback::widget_motion_notify_event (GdkEventMotion *ev) 
{ 
	gfloat multiplier;
	gfloat x_delta;
	gfloat y_delta;

	if(!widget.has_grab()) return Gtk::VBox::motion_notify_event_impl(ev); 

	multiplier = (ev->state & GDK_SHIFT_MASK ? 100 : 1) *
	    (ev->state & GDK_CONTROL_MASK ? 10 : 1) * 
	    (ev->state & GDK_MOD1_MASK ? 2 : 1);

	y_delta = grabbed_y - ev->y_root;
	grabbed_y = ev->y_root;

	x_delta = ev->x_root - grabbed_x;

	if (y_delta == 0) return TRUE;

	y_delta *= 1 + (x_delta/100);
	y_delta *= multiplier;
	y_delta /= _yscale;

	adjustment->set_value (adjustment->get_value() + 
			       ((grab_is_fine ? step_inc : page_inc) * y_delta));

	return TRUE;
}

gint 
GenericMotionFeedback::widget_enter_notify_event (GdkEventCrossing *ev) 

{
	widget.grab_focus();
	return FALSE;
}

gint 
GenericMotionFeedback::widget_leave_notify_event (GdkEventCrossing *ev) 

{
	widget.unset_flags (GTK_HAS_FOCUS);
	return FALSE;
}

gint 
GenericMotionFeedback::widget_key_press_event (GdkEventKey *ev) 

{
        gint retval = FALSE;
	gfloat curval;
	gfloat multiplier;

	multiplier = (ev->state & GDK_SHIFT_MASK ? 100 : 1) *
	    (ev->state & GDK_CONTROL_MASK ? 10 : 1) * 
	    (ev->state & GDK_MOD1_MASK ? 2 : 1);

	switch (ev->keyval) {
	case GDK_Page_Up:
	        retval = TRUE;
		curval = adjustment->get_value();
		adjustment->set_value (curval + (multiplier * page_inc));
		break;

	case GDK_Page_Down:
	        retval = TRUE;
		curval = adjustment->get_value();
		adjustment->set_value (curval - (multiplier * page_inc));
		break;

	case GDK_Up:
	        retval = TRUE;
		curval = adjustment->get_value();
		adjustment->set_value (curval + (multiplier * step_inc));
		break;

	case GDK_Down:
	        retval = TRUE;
		curval = adjustment->get_value();
		adjustment->set_value (curval - (multiplier * step_inc));
		break;

	case GDK_Home:
	        retval = TRUE;
		adjustment->set_value (_lower);
		break;

	case GDK_End:
	        retval = TRUE;
		adjustment->set_value (_upper);
		break;
	}
	
	if (retval) {
		gtk_signal_emit_stop_by_name (GTK_OBJECT(widget.gtkobj()),
					      "key_press_event");
	}

	return retval;
}


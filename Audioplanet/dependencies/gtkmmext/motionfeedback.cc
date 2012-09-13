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

    $Id: motionfeedback.cc,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#include <math.h>
#include <unistd.h>
#include <stdio.h> /* for sprintf, sigh ... */

#include <gdk/gdkkeysyms.h>
#include <gtk--.h>
#include <gtkmmext/pix.h>
#include <gtkmmext/shaped_window.h>
#include <gtkmmext/motionfeedback.h>

using namespace Gtkmmext;

MotionFeedback::MotionFeedback (Pix *pixset, 
				const char *widget_name, 
				Gtk::Adjustment *adj,
				bool with_numeric_display) :
        pixwin (pixset)

{
	char value_name[1024];

	set_homogeneous (false);

	max_pixmap = pixset->max_pixmap ();

	value = NULL;

	if (adj == NULL) {
	    i_own_my_adjustment = true;
	    set_adjustment (new Gtk::Adjustment (0, 0, 10000, 1, 10, 0));
	} else {
	    i_own_my_adjustment = false;
	    set_adjustment (adj);
	}

	set_spacing (3);
	pack_start (pixwin, false, true);

	current_step = adjustment->gtkobj()->value;
	current_pix_index = value_to_pix_index (current_step);

	pixwin.set_current_pix (current_pix_index);
	pixwin.show ();

	if (with_numeric_display) {

		value = new Gtk::SpinButton (*adjustment);

		if (step_inc < 1) {
			value->set_digits (abs ((int) ceil (log10 (step_inc))));
		}
		
		pack_start (*value, false, false);

		if (widget_name) {
			snprintf (value_name, sizeof(value_name), "%sValue", widget_name);
			value->set_name (value_name);
		}

		value->show ();
	}

	adjustment->value_changed.connect(SigC::slot(this,&MotionFeedback::adjustment_changed));

	pixwin.set_events (GDK_BUTTON_PRESS_MASK|
			   GDK_BUTTON_RELEASE_MASK|
			   GDK_POINTER_MOTION_MASK|
			   GDK_ENTER_NOTIFY_MASK|
			   GDK_LEAVE_NOTIFY_MASK|
			   GDK_KEY_PRESS_MASK|
			   GDK_KEY_RELEASE_MASK);

	pixwin.set_flags (GTK_CAN_FOCUS);

	/* Proxy all important events on the pixwin to ourselves */

	pixwin.button_press_event.connect(SigC::slot(this,&MotionFeedback::pixwin_button_press_event));
	pixwin.button_release_event.connect(SigC::slot(this,&MotionFeedback::pixwin_button_release_event));
	pixwin.motion_notify_event.connect(SigC::slot(this,&MotionFeedback::pixwin_motion_notify_event));
	pixwin.enter_notify_event.connect(SigC::slot(this,&MotionFeedback::pixwin_enter_notify_event));
	pixwin.leave_notify_event.connect(SigC::slot(this,&MotionFeedback::pixwin_leave_notify_event));
	pixwin.key_press_event.connect(SigC::slot(this,&MotionFeedback::pixwin_key_press_event));
}

MotionFeedback::~MotionFeedback()

{
	if (i_own_my_adjustment) {
		delete adjustment;
	}
	delete value;
}

void
MotionFeedback::set_adjustment (Gtk::Adjustment *adj)

{
	adjustment = adj;

	if (value) {
		value->set_adjustment (*adj);
	}

	_lower = adj->gtkobj()->lower;
	_upper = adj->gtkobj()->upper;
	_range = _upper - _lower;
	step_inc = adj->gtkobj()->step_increment;
	page_inc = adj->gtkobj()->page_increment;
}

gint 
MotionFeedback::pixwin_button_press_event (GdkEventButton *ev) 
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

	gtk_grab_add(GTK_WIDGET(pixwin.gtkobj()));
	grabbed_y = ev->y_root;
	grabbed_x = ev->x_root;

	/* XXX should we return TRUE ? */

	return FALSE;
}

gint 
MotionFeedback::pixwin_button_release_event (GdkEventButton *ev) 
{ 
	switch (ev->button) {
	case 1:
		if (pixwin.has_grab()) {
			if (!grab_is_fine) {
				gtk_grab_remove
					(GTK_WIDGET(pixwin.gtkobj()));
			}
		}
		break;
		
	case 3:
		if (pixwin.has_grab()) {
			if (grab_is_fine) {
				gtk_grab_remove
					(GTK_WIDGET(pixwin.gtkobj()));
			}
		}
		break;
	}

	return Gtk::VBox::button_release_event_impl(ev); 
}

gint 
MotionFeedback::pixwin_motion_notify_event (GdkEventMotion *ev) 
{ 
	gfloat multiplier;
	gfloat x_delta;
	gfloat y_delta;

	if(!pixwin.has_grab()) return Gtk::VBox::motion_notify_event_impl(ev); 

	multiplier = (ev->state & GDK_SHIFT_MASK ? 100 : 1) *
	    (ev->state & GDK_CONTROL_MASK ? 10 : 1) * 
	    (ev->state & GDK_MOD1_MASK ? 2 : 1);

	y_delta = grabbed_y - ev->y_root;
	grabbed_y = ev->y_root;

	x_delta = ev->x_root - grabbed_x;

	if (y_delta == 0) return TRUE;

	y_delta *= 1 + (x_delta/100);
	y_delta *= multiplier;
	y_delta /= 10;

	adjustment->set_value (adjustment->get_value() + 
			       ((grab_is_fine ? step_inc : page_inc) * y_delta));

	return TRUE;
}

gint 
MotionFeedback::pixwin_enter_notify_event (GdkEventCrossing *ev) 

{
	pixwin.grab_focus();
	return FALSE;
}

gint 
MotionFeedback::pixwin_leave_notify_event (GdkEventCrossing *ev) 

{
	pixwin.unset_flags (GTK_HAS_FOCUS);
	return FALSE;
}

gint 
MotionFeedback::pixwin_key_press_event (GdkEventKey *ev) 

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
		gtk_signal_emit_stop_by_name (GTK_OBJECT(pixwin.gtkobj()),
					      "key_press_event");
	}

	return retval;
}

void
MotionFeedback::adjustment_changed ()

{
	pixwin.set_current_pix (value_to_pix_index (adjustment->get_value()));
}

int
MotionFeedback::value_to_pix_index (gfloat value)

{
	current_step = fabs (_lower + value);
	return (int) floor ((current_step/_range) * max_pixmap);
}


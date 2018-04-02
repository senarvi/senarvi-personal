/*
    Copyright (C) 1998-99 Paul Davis
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

    $Id: slider_controller.cc,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#include <string>
#include <climits>

#include <midi++/controllable.h>

#include <gtkmmext/gtk_ui.h>
#include <gtkmmext/gtkpixscrollbar.h>
#include <gtkmmext/slider_controller.h>
#include <gtkmmext/pix.h>

using namespace Gtkmmext;

SliderController::SliderController (Pix *pixset,
				    Gtk::Adjustment *adj,
				    MIDI::Controllable *mc,
				    bool with_numeric)

	: spin (*adj, 0, 2),
	  prompter (GTK_WIN_POS_MOUSE, 30000, false),
	  midi_control (mc)

{			  
	pixset->generate (0, 0);
	pixscrollbar = gtk_pix_scrollbar_new (pixset->pixmap (0),
					      pixset->shape_mask (0),
					      pixset->pixmap (1),
					      pixset->shape_mask (1),
					      GTK_ADJUSTMENT(adj->gtkobj()),
					      true);

	evbox = Gtk::wrap (&(GTK_PIX_SCROLLBAR(pixscrollbar)->event_box));
	evbox->button_press_event.connect (slot (this, &SliderController::button_press));
	spin.set_name ("SliderControllerValue");
	spin.set_usize (70,-1); // should be based on font size somehow
	spin.set_numeric (true);
	spin.set_snap_to_ticks (false);
	
	
	if (mc) {
		mc->learning_started.connect (slot (*this, &SliderController::midicontrol_prompt));
		mc->learning_stopped.connect (slot (*this, &SliderController::midicontrol_unprompt));
	}
}

void
SliderController::set_value (float v)
{
	gtk_adjustment_set_value (GTK_PIX_SCROLLBAR(pixscrollbar)->adjustment, v);
}

gint
SliderController::button_press (GdkEventButton *ev)
{
        /* XXX make this configurable */

	if ((ev->state & GDK_CONTROL_MASK) && ev->button == 2) { 
		if (midi_control) {
			midi_control->learn_about_external_control ();
		}
		return TRUE;
	}

	return FALSE;
}

void
SliderController::midicontrol_set_tip ()

{
	if (midi_control) {
		// Gtkmmext::UI::instance()->set_tip (evbox, midi_control->control_description());
	}
}

void
SliderController::midicontrol_prompt ()

{
	string prompt = "operate MIDI controller now";
	prompter.set_text (prompt);
	Gtkmmext::UI::instance()->touch_display (&prompter);
}

void
SliderController::midicontrol_unprompt ()

{
	Gtkmmext::UI::instance()->touch_display (&prompter);
}

VSliderController::VSliderController (Pix *pixset,
				      Gtk::Adjustment *adj,
				      MIDI::Controllable *mcontrol,
				      bool with_numeric)

	: SliderController (pixset, adj, mcontrol, with_numeric)
{
	set_spacing (5);
	gtk_box_pack_start (GTK_BOX(gtkobj()), pixscrollbar, FALSE, TRUE, 0);

	if (with_numeric) {
		spin_frame.add (spin);
		spin_frame.set_shadow_type (GTK_SHADOW_IN);
		spin_frame.set_name ("BaseFrame");
		spin_hbox.pack_start (spin_frame, false, true);
		pack_start (spin_hbox, false, false);
	}
}

HSliderController::HSliderController (Pix *pixset,
				      Gtk::Adjustment *adj,
				      MIDI::Controllable *mcontrol,
				      bool with_numeric)
	
	: SliderController (pixset, adj, mcontrol, with_numeric)
{
	set_spacing (5);
	gtk_box_pack_start (GTK_BOX(gtkobj()), pixscrollbar, FALSE, TRUE, 0);
	if (with_numeric) {
		spin_frame.add (spin);
		//spin_frame.set_shadow_type (GTK_SHADOW_IN);
		spin_frame.set_name ("BaseFrame");
		spin_hbox.pack_start (spin_frame, false, true);
		pack_start (spin_hbox, false, false);
	}
}

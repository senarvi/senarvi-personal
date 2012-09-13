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

    $Id: slider_controller.h,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#ifndef __gtkmmext_motion_controller_h__
#define __gtkmmext_motion_controller_h__

#include <gtk--.h>
#include <gtkmmext/popup.h>

namespace Gtkmmext {
	class Pix;
}

namespace MIDI {
	class Controllable;
}

namespace Gtkmmext {

class SliderController : public SigC::Object

{
  public:
	SliderController (Gtkmmext::Pix* pixset,
			  Gtk::Adjustment* adj,
			  MIDI::Controllable*,
			  bool with_numeric = true);

        virtual ~SliderController () {}

	void midicontrol_set_tip ();
	
	void set_value (float);
	void set_sensitive (bool yn) {
		gtk_widget_set_sensitive (GTK_WIDGET(pixscrollbar), yn);
		spin.set_sensitive (yn);
	}

	Gtk::SpinButton & get_spin_button () { return spin; }
	
  protected:
	GtkWidget*          pixscrollbar;
	Gtk::SpinButton     spin;
	Gtk::Frame          spin_frame;
	Gtk::HBox           spin_hbox;
	Gtk::EventBox*      evbox;
	Gtkmmext::PopUp     prompter;
	MIDI::Controllable* midi_control;

	gint button_press (GdkEventButton *);
	void midicontrol_prompt ();
	void midicontrol_unprompt ();
	void update_midi_control ();
};

class VSliderController : public Gtk::HBox, public SliderController
{
  public:
	VSliderController (Gtkmmext::Pix *pixset,
			   Gtk::Adjustment *adj,
			   MIDI::Controllable *,
			   bool with_numeric = true);
};

class HSliderController : public Gtk::HBox, public SliderController
{
  public:
	HSliderController (Gtkmmext::Pix *pixset,
			   Gtk::Adjustment *adj,
			   MIDI::Controllable *,
			   bool with_numeric = true);
};


}; /* namespace */

#endif // __gtkmmext_motion_controller_h__		

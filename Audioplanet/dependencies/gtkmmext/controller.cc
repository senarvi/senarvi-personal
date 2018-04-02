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

    $Id: controller.cc,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#include <string.h>
#include <limits.h>

#include <midi++/channel.h>
#include <gtkmmext/gtk_ui.h>
#include <gtkmmext/controller.h>

using namespace Gtkmmext;

Controller::Controller (Gtk::Adjustment *adj, MIDI::Port *p, MIDI::channel_t chn)
	: MIDI::Controllable (p, chn),
	  adjustment (adj),
	  prompter (GTK_WIN_POS_MOUSE, 30000, false)
{
	new_value_pending = false;

	/* hear about MIDI control learning */

	learning_started.connect 
		(SigC::slot (this, &Controller::midicontrol_prompt));
	learning_stopped.connect 
		(SigC::slot (this, &Controller::midicontrol_unprompt));
}

void
Controller::midicontrol_prompt ()

{
	string prompt = "operate MIDI controller now";

	prompter.set_text (prompt);
	Gtkmmext::UI::instance()->touch_display (&prompter);
}

void
Controller::midicontrol_unprompt ()

{
	Gtkmmext::UI::instance()->touch_display (&prompter);
}

int
Controller::update_controller_value (void *arg)

{
	Controller *c = (Controller *) arg;

	c->adjustment->set_value (c->new_value);
	c->new_value_pending = false;

	return FALSE;
}

void
Controller::set_value (float v)

{
	/* This is called from a MIDI callback. It could happen
	   a thousand times a second, or more. Therefore, instead
	   of going straight to the X server, which may not work for
	   thread-related reasons, simply request an update whenever
	   the GTK main loop is idle.
	*/
	
	new_value = v;

	if (!new_value_pending) {
		new_value_pending = true;
		Gtkmmext::UI::instance()->idle_add (update_controller_value, this);
	}
}

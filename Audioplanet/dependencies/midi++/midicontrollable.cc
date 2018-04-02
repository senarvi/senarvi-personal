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

    $Id: midicontrollable.cc,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#include <stdio.h> /* for sprintf, sigh */
#include <pbd/error.h>
#include <midi++/port.h>
#include <midi++/channel.h>
#include <midi++/controllable.h>

using namespace SigC;
using namespace MIDI;

Controllable::Controllable (Port *p, channel_t c, bool is_bistate)
{
	midi_msg_id = -1;
	midi_rebind (p, c);
	bistate = is_bistate;
	connections = 0;
}

Controllable::~Controllable ()

{
	drop_external_control ();
}

void
Controllable::midi_rebind (Port *p, channel_t c)

{
	drop_external_control ();
	port = p;
	channel = c;
}

void
Controllable::learn_about_external_control ()

{
	drop_external_control ();

	if (port) {
		midi_learn_connection = 
			port->input()->any.connect 
			(slot (*this, &Controllable::midi_receiver));
		learning_started ();

	} else {
		info << "No MIDI port specified - external control disabled"
		     << endmsg;
	}
}

void
Controllable::stop_learning ()

{
	midi_learn_connection.disconnect ();
}

void
Controllable::drop_external_control ()

{
	if (connections > 0) {
		midi_sense_connection[0].disconnect ();
	} 
	if (connections > 1) {
		midi_sense_connection[1].disconnect ();
	}
	connections = 0;
	midi_learn_connection.disconnect ();
	midi_msg_id = -1;
}

void 
Controllable::midi_sense_note_on (Parser &p, EventTwoBytes *tb) 
{
	midi_sense_note (p, tb, true);
}

void 
Controllable::midi_sense_note_off (Parser &p, EventTwoBytes *tb) 
{
	midi_sense_note (p, tb, false);
}

void
Controllable::midi_sense_note (Parser &p, EventTwoBytes *msg, bool is_on)

{
	if (!bistate) {
		set_value (lower() + (range() * (msg->note_number/127.0)));
	} else {

		/* Note: parser handles the use of zero velocity to
		   mean note off. if we get called with is_on=true, then we
		   got a *real* note on.  
		*/

		if (msg->note_number == midi_msg_id) {
			set_value (is_on ? 1 : 0);
		}
	}
}

void
Controllable::midi_sense_controller (Parser &, EventTwoBytes *msg)

{
	if (midi_msg_id == msg->controller_number) {
		if (!bistate) {
			set_value (lower() + (range() * (msg->value/127.0)));
		} else {
			if (msg->value > 64.0) {
				set_value (1);
			} else {
				set_value (0);
			}
		}
	}
}

void
Controllable::midi_sense_program_change (Parser &p, byte msg)

{
	/* XXX program change messages make no sense for bistates */

	if (!bistate) {
		set_value (lower() + (range() * (msg/127.0)));
	} 
}

void
Controllable::midi_sense_pitchbend (Parser &p, pitchbend_t pb)

{
	/* pitchbend messages make no sense for bistates */

	/* XXX gack - get rid of assumption about typeof pitchbend_t */

	set_value (lower() + (range() * (pb/(float) SHRT_MAX)));
}			

void
Controllable::midi_receiver (Parser &p, byte *msg, size_t len)

{
	/* we only respond to channel messages */

	if (msg[0] & 0xF0 < 0x80 ||
	    msg[0] & 0xF0 > 0xE0) {
		return;
	}

	/* if its not our channel, forget it */

	if (msg[0] & 0xF != channel) {
		return;
	}

	drop_external_control ();

	/* if the our port doesn't do input anymore, forget it ... */

	if (!port->input()) {
		return;
	}

	switch (msg[0] & 0xF0) {
	case MIDI::off:
		midi_sense_connection[0] = p.channel_note_off[channel].connect
			(slot (*this, &Controllable::midi_sense_note_off));

		/* if this is a bistate, connect to noteOn as well,
		   and we'll toggle back and forth between the two.
		*/

		if (bistate) {
			midi_msg_id = msg[1];
			midi_sense_connection[1] = 
				p.channel_note_on[channel].connect
				(slot (*this, 
				       &Controllable::midi_sense_note_on));
			connections = 2;
		} else {
			connections = 1;
		}
		printf ("learnt control: noteoff\n");
		sprintf (control_tip, "MIDI control: NoteOff");
		break;

	case MIDI::on:
		midi_sense_connection[0] = p.channel_note_on[channel].connect
			(slot (*this, &Controllable::midi_sense_note_on));
		if (bistate) {
			midi_msg_id = msg[1];
			midi_sense_connection[1] = 
				p.channel_note_off[channel].connect
				(slot (*this, 
				       &Controllable::midi_sense_note_off));
			connections = 2;
		} else {
			connections = 1;
		}
		printf ("learnt control: noteon\n");
		sprintf (control_tip, "MIDI control: NoteOn");
		break;

	case MIDI::controller:
		midi_msg_id = msg[1];
		midi_sense_connection[0] = 
			p.channel_controller[channel].connect
			(slot (*this, 
			       &Controllable::midi_sense_controller));
		connections = 1;
		printf ("learnt control: CC %d\n", midi_msg_id);
		sprintf (control_tip, "MIDI control: Controller %d",
			 midi_msg_id);
		break;

	case MIDI::program:
		if (!bistate) {
			midi_sense_connection[0] = 
				p.channel_program_change[channel].connect
				(slot (*this, 
				       &Controllable::midi_sense_program_change));
			connections = 1;
			sprintf (control_tip, "MIDI control: ProgramChange");
		}
		break;

	case MIDI::pitchbend:
		if (!bistate) {
			midi_sense_connection[0] = 
				p.channel_pitchbend[channel].connect
				(slot (*this, 
				       &Controllable::midi_sense_pitchbend));
			connections = 1;
			sprintf (control_tip, "MIDI control: Pitchbend");
		}
		break;
	}

	learning_stopped ();

	if (connections) {
		midi_set_tip (control_tip);
	}
}




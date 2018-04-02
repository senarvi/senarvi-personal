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

    $Id: controllable.h,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#ifndef __qm_midicontrollable_h__
#define __qm_midicontrollable_h__

#include <sigc++/signal_system.h>
#include <midi++/types.h>

namespace MIDI {

class Channel;
class Port;
class Parser;

class Controllable : public SigC::Object
{
  public:
	Controllable (Port *, channel_t channel, bool bistate = false);
	virtual ~Controllable ();

	void midi_rebind (Port *, channel_t channel);
	void learn_about_external_control ();
	void stop_learning ();
	void drop_external_control ();

	void midi_sense_note_on (Parser &p, EventTwoBytes *tb);
	void midi_sense_note_off (Parser &p, EventTwoBytes *tb);
	void midi_sense_controller (Parser &, EventTwoBytes *);
	void midi_sense_program_change (Parser &, byte);
	void midi_sense_pitchbend (Parser &, pitchbend_t);

	virtual void set_value (float) = 0;
	virtual float lower () { return 0; }
	virtual float upper () { return 0; }
	virtual float range () { return 0; }
	virtual void midi_set_tip (const char *tip) { }

	SigC::Signal0<void> learning_started;
	SigC::Signal0<void> learning_stopped;

  protected:
	char control_tip[64];

  private:
	bool bistate;
	int midi_msg_id;            /* controller ID or note number */
	SigC::Connection midi_sense_connection[2];
	SigC::Connection midi_learn_connection;
	size_t connections;
	channel_t channel;
	Port *port;

	void midi_receiver (Parser &p, byte *, size_t);
	void midi_sense_note (Parser &, EventTwoBytes *, bool is_on);
};

}; /* namespace MIDI */

#endif // __qm_midicontrollable_h__


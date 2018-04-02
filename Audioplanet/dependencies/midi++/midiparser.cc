/*
    Copyright (C) 1998 Paul Barton-Davis

    This file was inspired by the MIDI parser for KeyKit by 
    Tim Thompson. 
    
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

    $Id: midiparser.cc,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>

#include <midi++/types.h>
#include <midi++/parser.h>
#include <midi++/port.h>
#include <midi++/mmc.h>
#include <pbd/transmitter.h>

using namespace SigC;
using namespace MIDI;

const char *
Parser::midi_event_type_name (eventType t)

{
	switch (t) {
	case none:
		return "no midi messages";

	case raw:
		return "raw midi data";

	case MIDI::any:
		return "any midi message";
	  
	case off:
		return "note off";
	  
	case on:
		return "note on";
	  
	case polypress:
		return "aftertouch";
	  
	case MIDI::controller:
		return "controller";
	  
	case program:
		return "program change";
	  
	case chanpress:
		return "channel pressure";
	  
	case MIDI::pitchbend:
		return "pitch bend";
	  
	case MIDI::sysex:
		return "system exclusive";
	  
	case MIDI::song:
		return "song position";
	  
	case MIDI::tune:
		return "tune";
	  
	case MIDI::eox:
		return "end of sysex";
	  
	case MIDI::timing:
		return "timing";
	  
	case MIDI::start:
		return "start";
	  
	case MIDI::stop:
		return "continue";
	  
	case MIDI::contineu:
		return "stop";
	  
	case active:
		return "active sense";
	  
	default:
		return "unknow MIDI event type";
	}
};

Parser::Parser (Port &p) 
	: _port (p)

{
	trace_stream = 0;
	trace_prefix = "";
	memset (message_counter, 0, sizeof (message_counter[0]) * 256);
	msgindex = 0;
	msgtype = none;
	msglen = 256;
	msgbuf = (unsigned char *) malloc (msglen);
	msgbuf[msgindex++] = 0x90;
	_mmc_forward = false;
	_mtc_forward = false;
	_mtc_running = MTC_Stopped;
	_mtc_locked = false;
	expected_mtc_quarter_frame_code = (1|(1<<7));
	memset (_mtc_time, 0, sizeof (_mtc_time));
	memset (_qtr_mtc_time, 0, sizeof (_mtc_time));

	/* this hack deals with the possibility of our first MIDI
	   bytes being running status messages.
	*/

	channel_msg (0x90);
	state = NEEDSTATUS;
}

Parser::~Parser ()

{
	delete msgbuf;
}

void
Parser::trace_event (Parser &p, byte *msg, unsigned int len)

{
	eventType type;
	ostream *o;

	if ((o = trace_stream) == NULL) { /* can be asynchronously removed */
		return;
	}
	
	type = (eventType) (msg[0]&0xF0);

	switch (type) {
	case off:
		*o << trace_prefix
		   << "Channel "
		   << (msg[0]&0xF)+1
		   << " NoteOff NoteNum "
		   << (int) msg[1]
		   << " Vel "
		   << (int) msg[2]
		   << endmsg;
		break;
		
	case on:
		*o << trace_prefix
		   << "Channel "
		   << (msg[0]&0xF)+1
		   << " NoteOn NoteNum "
		   << (int) msg[1]
		   << " Vel "
		   << (int) msg[2]
		   << endmsg;
		break;
	    
	case polypress:
		*o << trace_prefix
		   << "Channel "
		   << (msg[0]&0xF)+1
		   << " PolyPressure"
		   << (int) msg[1]
		   << endmsg;
		break;
	    
	case MIDI::controller:
		*o << trace_prefix
		   << "Channel "
		   << (msg[0]&0xF)+1
		   << " Controller "
		   << (int) msg[1]
		   << " Value "
		   << (int) msg[2]
		   << endmsg;
		break;
		
	case program:
		*o << trace_prefix 
		   << "Channel "
		   << (msg[0]&0xF)+1
		   <<  " Program Change ProgNum "
		   << (int) msg[1]
		   << endmsg;
		break;
		
	case chanpress:
		*o << trace_prefix 
		   << "Channel "
		   << (msg[0]&0xF)+1
		   << " Channel Pressure "
		   << (int) msg[1]
		   << endmsg;
		break;
	    
	case MIDI::pitchbend:
		*o << trace_prefix
		   << "Channel "
		   << (msg[0]&0xF)+1
		   << " Pitch Bend "
		   << ((msg[1]<<7)|msg[2])
		   << endmsg;
		break;
	    
	case MIDI::sysex:
		*o << trace_prefix
		   << "System Exclusive ("
		   << len
		   << " bytes)"
		   << endmsg;
		break;
	    
	case MIDI::song:
		*o << trace_prefix << "Song" << endmsg;
		break;
	    
	case MIDI::tune:
		*o << trace_prefix << "Tune" << endmsg;
		break;
	    
	case MIDI::eox:
		*o << trace_prefix << "End-of-System Exclusive" << endmsg;
		break;
	    
	case MIDI::timing:
		*o << trace_prefix << "Timing" << endmsg;
		break;
	    
	case MIDI::start:
		*o << trace_prefix << "Start" << endmsg;
		break;
	    
	case MIDI::stop:
		*o << trace_prefix << "Stop" << endmsg;
		break;
	    
	case MIDI::contineu:
		*o << trace_prefix << "Continue" << endmsg;
		break;
	    
	case active:
		*o << trace_prefix << "Active Sense" << endmsg;
		break;
	    
	default:
		*o << trace_prefix << "Unrecognized MIDI message" << endmsg;
		break;
	}
}

void
Parser::trace (bool onoff, ostream *o, const string &prefix)

{
	if (onoff) {
		trace_stream = o;
		trace_prefix = prefix;
		trace_connection = any.connect 
			(slot (*this, &Parser::trace_event));
	} else {
		trace_connection.disconnect ();
		trace_prefix = "";
		trace_stream = 0;
	}
}

void
Parser::scanner (unsigned char inbyte)
{
	bool statusbit;

	/* Check active sensing early, so it doesn't interrupt sysex. 
	   
	   NOTE: active sense messages are not considered to fit under
	   "any" for the purposes of callbacks. If a caller wants
	   active sense messages handled, which is unlikely, then
	   they can just ask for it specifically. They are so unlike
	   every other MIDI message in terms of semantics that its
	   counter-productive to treat them similarly.
	*/
	
	if (inbyte == 0xfe) {
	        message_counter[inbyte]++;
		active_sense (*this);
		return;
	}
	
	/* If necessary, allocate larger message buffer. */
	
	if (msgindex >= msglen) {
		msglen *= 2;
		msgbuf = (unsigned char *) realloc (msgbuf, msglen);
	}
	
	/*
	  Real time messages can occur ANYPLACE,
	  but do not interrupt running status. 
	*/

	bool rtmsg = false;
	
	switch (inbyte) {
	case 0xf8:
		rtmsg = true;
		break;
	case 0xfa:
		rtmsg = true;
		break;
	case 0xfb:
		rtmsg = true;
		break;
	case 0xfc:
		rtmsg = true;
		break;
	case 0xfd:
		rtmsg = true;
		break;
	case 0xfe:
		rtmsg = true;
		break;
	case 0xff:
		rtmsg = true;
		break;
	}

	if (rtmsg) {
		if (edit (&inbyte, 1) >= 0) {
			realtime_msg (inbyte);
		}
		
		return;
	} 

	statusbit = (inbyte & 0x80);

	/*
	 * Variable length messages (ie. the 'system exclusive')
	 * can be terminated by the next status byte, not necessarily
	 * an EOX.  Actually, since EOX is a status byte, this
	 * code ALWAYS handles the end of a VARIABLELENGTH message.
	 */
	
	if (state == VARIABLELENGTH && statusbit)  {
		/* The message has ended, so process it */

		if (msgindex > 0 && edit (msgbuf, msgindex) >= 0) {
			if (!possible_mmc (msgbuf, msgindex + 1) || _mmc_forward) {
				if (!possible_mtc (msgbuf, msgindex + 1) || _mtc_forward) {
					sysex (*this, msgbuf, msgindex);
				}
			}
			any (*this, msgbuf, msgindex);
		}
		
		state = NEEDSTATUS;
	}
	
	/*
	 * Status bytes always start a new message.
	 */
	
	if (statusbit) {
		msgindex = 0;
		msgbuf[msgindex++] = inbyte;
		if ((inbyte & 0xf0) == 0xf0) {
			system_msg (inbyte);
			runnable = false;
		} else {
			channel_msg (inbyte);
		}
		
		return;
	}
	
	/*
	 * We've got a Data byte.
	 */
	
	msgbuf[msgindex++] = inbyte;
	
	switch (state) {
	case NEEDSTATUS:
		/*
		 * We shouldn't get here, since in NEEDSTATUS mode
		 * we're expecting a new status byte, NOT any
		 * data bytes. On the other hand, some equipment
		 * with leaky modwheels and the like might be
		 * sending data bytes as part of running controller
		 * messages, so just handle it silently.
		 */
		break;
		
	case NEEDTWOBYTES:
		/* wait for the second byte */
		if (msgindex < 3)
			return;
		/*FALLTHRU*/
		
	case NEEDONEBYTE:
		/* We've completed a 1 or 2 byte message. */
		
		if (edit (msgbuf, msgindex) == 0) {
			
			/* message not cancelled by an editor */
			
		        message_counter[msgbuf[0] & 0xF0]++;
			signal (msgbuf, msgindex);
		}
		
		if (runnable) {
			/* In Runnable mode, we reset the message */
			/* index, but keep the callbacks_pending and state the */
			/* same.  This provides the "running status */
			/* byte" feature. */
			msgindex = 1;
		} else {
			/* If not Runnable, reset to NEEDSTATUS mode */
			state = NEEDSTATUS;
		}
		break;
		
	case VARIABLELENGTH:
		/* nothing to do */
		break;
	}
	return;
}

/*
 * realtime_msg(inbyte)
 *
 * Call the real-time function for the specified byte, immediately.
 * These can occur anywhere, so they don't change the state.
 */

void
Parser::realtime_msg(unsigned char inbyte)

{
	message_counter[inbyte]++;

	switch (inbyte) {
	case 0xf8:
		timing (*this);
		break;
	case 0xfa:
		start (*this);
		break;
	case 0xfb:
		contineu (*this);
		break;
	case 0xfc:
		stop (*this);
		break;
	case 0xfe:
		/* !!! active sense message in realtime_msg: should not reach here
		 */  
		break;
	case 0xff:
		reset (*this);
		break;
	}

	any (*this, &inbyte, 1);
}

/*
 * channel_msg(inbyte)
 *
 * Interpret a Channel (voice or mode) Message status byte.
 */

void
Parser::channel_msg(unsigned char inbyte)
{
	runnable = true;		/* Channel messages can use running status */
    
	/* The high 4 bits, which determine the type of channel message. */
    
	switch (inbyte&0xF0) {
	case 0x80:
		msgtype = off;
		state = NEEDTWOBYTES;
		break;
	case 0x90:
		msgtype = on;
		state = NEEDTWOBYTES;
		break;
	case 0xa0:
		msgtype = polypress;
		state = NEEDTWOBYTES;
		break;
	case 0xb0:
		msgtype = MIDI::controller;
		state = NEEDTWOBYTES;
		break;
	case 0xc0:
		msgtype = program;
		state = NEEDONEBYTE;
		break;
	case 0xd0:
		msgtype = chanpress;
		state = NEEDONEBYTE;
		break;
	case 0xe0:
		msgtype = MIDI::pitchbend;
		state = NEEDTWOBYTES;
		break;
	}
}

/*
 * system_msg(inbyte)
 *
 * Initialize (and possibly emit) the signals for the
 * specified byte.  Set the state that the state-machine
 * should go into.  If the signal is not emitted
 * immediately, it will be when the state machine gets to
 * the end of the MIDI message.
 */

void
Parser::system_msg (unsigned char inbyte)

{
	message_counter[inbyte]++;
    
	switch (inbyte) {
	case 0xf0:
		msgtype = MIDI::sysex;
		state = VARIABLELENGTH;
		break;
	case 0xf1:
		msgtype = MIDI::mtc_quarter;
		state = NEEDONEBYTE;
		break;
	case 0xf2:
		msgtype = MIDI::position;
		state = NEEDTWOBYTES;
		break;
	case 0xf3:
		msgtype = MIDI::song;
		state = NEEDONEBYTE;
		break;
	case 0xf6:
		tune (*this);
		state = NEEDSTATUS;
		break;
	case 0xf7:
		eox (*this);
		state = NEEDSTATUS;
		break;
	}

	any (*this, &inbyte, 1);

}

void 
Parser::signal (byte *msg, size_t len)
{
	
	channel_t chan = msg[0]&0xF;

	switch (msgtype) {
	case none:
		break;
		
	case off:
		channel_active_preparse[chan] (*this);
		note_off (*this, (EventTwoBytes *) &msg[1]);
		channel_note_off[chan] 
			(*this, (EventTwoBytes *) &msg[1]);
		channel_active_postparse[chan] (*this);
		break;
		
	case on:
		channel_active_preparse[chan] (*this);

		/* Hack to deal with MIDI sources that use velocity=0
		   instead of noteOff.
		*/

		if (msg[2] == 0) {
			note_off (*this, (EventTwoBytes *) &msg[1]);
			channel_note_off[chan] 
				(*this, (EventTwoBytes *) &msg[1]);
		} else {
			note_on (*this, (EventTwoBytes *) &msg[1]);
			channel_note_on[chan] 
				(*this, (EventTwoBytes *) &msg[1]);
		}

		channel_active_postparse[chan] (*this);
		break;
		
	case MIDI::controller:
		channel_active_preparse[chan] (*this);
		controller (*this, (EventTwoBytes *) &msg[1]);
		channel_controller[chan] 
			(*this, (EventTwoBytes *) &msg[1]);
		channel_active_postparse[chan] (*this);
		break;
		
	case program:
		channel_active_preparse[chan] (*this);
		program_change (*this, msg[1]);
		channel_program_change[chan] (*this, msg[1]);
		channel_active_postparse[chan] (*this);
		break;
		
	case chanpress:
		channel_active_preparse[chan] (*this);
		pressure (*this, msg[1]);
		channel_pressure[chan] (*this, msg[1]);
		channel_active_postparse[chan] (*this);
		break;
		
	case polypress:
		channel_active_preparse[chan] (*this);
		poly_pressure (*this, (EventTwoBytes *) &msg[1]);
		channel_poly_pressure[chan] 
			(*this, (EventTwoBytes *) &msg[1]);
		channel_active_postparse[chan] (*this);
		break;
		
	case MIDI::pitchbend:
		channel_active_preparse[chan] (*this);
		pitchbend (*this, (msg[1]<<7)|msg[2]);
		channel_pitchbend[chan] (*this, (msg[1]<<7)|msg[2]);
		channel_active_postparse[chan] (*this);
		break;
		
	case MIDI::sysex:
		sysex (*this, msg, len);
		break;

	case MIDI::mtc_quarter:
		process_mtc_quarter_frame (msg);
		mtc_quarter_frame (*this, *msg);
		break;
		
	case MIDI::position:
		position (*this, msg, len);
		break;
		
	case MIDI::song:
		song (*this, msg, len);
		break;
	
	case MIDI::tune:
		tune (*this);
	
	default:
		/* XXX some kind of warning ? */
		break;
	}
	
	any (*this, msg, len);
}

bool
Parser::possible_mmc (byte *msg, size_t msglen)
{
	if (!MachineControl::is_mmc (msg, msglen)) {
		return false;
	}

	/* hand over the just the interior MMC part of
	   the sysex msg without the leading 0xF0
	*/

	mmc (*this, &msg[1], msglen - 1);
	return true;
}

bool
Parser::possible_mtc (byte *sysex_buf, size_t msglen)
{
	if (msglen != 10 || sysex_buf[0] != 0xf0 || sysex_buf[1] != 0x7f || sysex_buf[3] != 0x01 || sysex_buf[4] != 0x01) {
		return false;
	}
		
        /* full MTC */
	
	_mtc_time[0] = sysex_buf[8]; // frames
	_mtc_time[1] = sysex_buf[7]; // minutes
	_mtc_time[2] = sysex_buf[6]; // seconds
	_mtc_time[3] = (sysex_buf[5] & 0x1f); // hours
	
	_mtc_fps = MTC_FPS ((sysex_buf[5] & 0x60) >> 5); // fps
	
	/* wait for first quarter frame, which could indicate forwards
	   or backwards ...
	*/

	expected_mtc_quarter_frame_code = 1 | (1<<7);
	_mtc_running = MTC_Stopped;
	_mtc_locked = false;
	memset (_qtr_mtc_time, 0, sizeof (_qtr_mtc_time));
	
	/* emit signals */

	mtc (*this, &sysex_buf[1], msglen - 1);
	mtc_time (_mtc_time, true);
	mtc_status (MTC_Stopped);

	return true;
}

void
Parser::process_mtc_quarter_frame (byte *msg)
{
	int which_quarter_frame = (msg[1] & 0xf0) >> 4;
	int next_quarter_frame;

	/* Is it an expected frame? If not, stop and wait for the first
	   message of a series. Remember, the first can be frame 7 or frame 0,
	   depending on the direction of the MTC generator ...
	*/

	if (((1 << which_quarter_frame) & expected_mtc_quarter_frame_code) == 0) {
		/* go back to waiting for the first frame */

		if (_mtc_running != MTC_Stopped) {
			_mtc_running = MTC_Stopped;
			_mtc_locked = false;
			expected_mtc_quarter_frame_code = (1 | (1<<7));
			memset (_qtr_mtc_time, 0, sizeof (_qtr_mtc_time));
			mtc_status (MTC_Stopped);
		}
		return;
	}

	switch (which_quarter_frame) {
	case 0: // frames LS nibble
		_qtr_mtc_time[0] |= msg[1] & 0xf;
		break;
	case 1:  // frames MS nibble
		_qtr_mtc_time[0] |= (msg[1] & 0xf)<<4;
		break;
	case 2: // seconds LS nibble
		_qtr_mtc_time[1] |= msg[1] & 0xf;
		break;
	case 3: // seconds MS nibble
		_qtr_mtc_time[1] |= (msg[1] & 0xf)<<4;
		break;
	case 4: // minutes LS nibble
		_qtr_mtc_time[2] |= msg[1] & 0xf;
		break;
	case 5: // minutes MS nibble
		_qtr_mtc_time[2] |= (msg[1] & 0xf)<<4;
		break;
	case 6: // hours LS nibble
		_qtr_mtc_time[3] |= msg[1] & 0xf;
		break;
	case 7: 

		/* last quarter frame msg has the MS bit of
                   the hour in bit 0, and the SMPTE FPS type
                   in bits 5 and 6 
		*/

		_qtr_mtc_time[3] |= ((msg[1] & 0x1) << 4);
		_mtc_fps = MTC_FPS ((msg[1] & 0x6) >> 1);
		break;

	default:
		/*NOTREACHED*/
		break;

	} 
	
	mtc (*this, &msg[1], msglen - 1);

	if (_mtc_running == MTC_Stopped) {

		switch (which_quarter_frame) {
		case 0:
			_mtc_running = MTC_Forward;
			break;
		case 7:
			_mtc_running = MTC_Backward;
			break;

		default:
			break;
		}
	}

	switch (_mtc_running) {
	case MTC_Forward:
		if (which_quarter_frame == 7) {

			/* we've reached the final of 8 quarter frame messages.
			   store the time, reset the pending time holder,
			   and signal anyone who wants to know the time.
			*/

			memcpy (_mtc_time, _qtr_mtc_time, sizeof (_mtc_time));
			memset (_qtr_mtc_time, 0, sizeof (_qtr_mtc_time));
			if (!_mtc_locked) {
				_mtc_locked = true;
				mtc_status (MTC_Forward);
			}
			mtc_time (_mtc_time, false);
			next_quarter_frame = 0;

		} else {
			next_quarter_frame = which_quarter_frame + 1;
		}
		expected_mtc_quarter_frame_code = (1<<next_quarter_frame);
		break;
		
	case MTC_Backward:
		if (which_quarter_frame == 0) {
			
			/* we've reached the final of 8 quarter frame messages.
			   store the time, reset the pending time holder,
			   and signal anyone who wants to know the time.
			*/

			memcpy (_mtc_time, _qtr_mtc_time, sizeof (_mtc_time));
			memset (_qtr_mtc_time, 0, sizeof (_qtr_mtc_time));
			if (!_mtc_locked) {
				_mtc_locked = true;
				mtc_status (MTC_Backward);
			}
			mtc_time (_mtc_time, false);
			next_quarter_frame = 7;

		} else {
			next_quarter_frame = which_quarter_frame - 1;
		}
		expected_mtc_quarter_frame_code = (1<<next_quarter_frame);
		break;

	default:
		break;
	}

}





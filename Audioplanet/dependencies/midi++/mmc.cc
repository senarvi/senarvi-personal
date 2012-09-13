/*
    Copyright (C) 2000 Paul Barton-Davis 

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

    $Id: mmc.cc,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#include <pbd/error.h>
#include <midi++/mmc.h>
#include <midi++/port.h>
#include <midi++/parser.h>

using namespace MIDI;

MachineControl::MachineControl (Port &p, float version,
				CommandSignature &csig,
				ResponseSignature &rsig)

	: _port (p)
{
	Parser *parser;

	_device_id = 1;
	
	if ((parser = _port.input()) != 0) {
		parser->mmc.connect 
			(slot (*this, &MachineControl::process_mmc_message));
	} else {
		warning << "MMC connected to a non-input port: useless!"
			<< endmsg;
	}
}

void
MachineControl::set_device_id (byte id)

{
	_device_id = id & 0x7f;
}

bool
MachineControl::is_mmc (byte *sysex_buf, size_t len)

{
	if (len < 4 || len > 48) {
		return false;
	}

	if (sysex_buf[1] != 0x7f) {
		return false;
	}

	if (sysex_buf[3] != 0x6 && /* MMC Command */
	    sysex_buf[3] != 0x7) { /* MMC Response */
		return false;
	}
	
	return true;
}

void
MachineControl::process_mmc_message (Parser &p, byte *msg, size_t len)

{
	size_t skiplen;
	byte *msgbuf;

	/* Reject if its not for us. 0x7f is the "all-call" device ID */

	/* msg[0] = 0x7f (MMC sysex ID(
	   msg[1] = device ID
	   msg[2] = 0x6 (MMC command) or 0x7 (MMC response)
	   msg[3] = MMC command code
	*/
	
	if (msg[1] != 0x7f && msg[1] != _device_id) {
		return;
	}

	skiplen = 1;
	len -= 3;
	msgbuf = &msg[3];

	do {
		switch (*msgbuf) {
		case cmdStop:
			Stop (*this);
			break;

		case cmdPlay:
			Play (*this);
			break;

		case cmdDeferredPlay:
			DeferredPlay (*this);
			break;

		case cmdFastForward:
			FastForward (*this);
			break;

		case cmdRewind:
			Rewind (*this);
			break;

		case cmdRecordStrobe:
			RecordStrobe (*this);
			break;

		case cmdRecordExit:
			RecordExit (*this);
			break;

		case cmdRecordPause:
			RecordPause (*this);
			break;

		case cmdPause:
			Pause (*this);
			break;

		case cmdEject:
			Eject (*this);
			break;

		case cmdChase:
			Chase (*this);
			break;

		case cmdCommandErrorReset:
			CommandErrorReset (*this);
			break;

		case cmdMmcReset:
			MmcReset (*this);
			break;

		case cmdMaskedWrite:
			skiplen = do_masked_write (msgbuf, len);
			break;

		case cmdLocate:
			skiplen = do_locate (msgbuf, len);
			break;

		case cmdShuttle:
			skiplen = do_shuttle (msgbuf, len);
			break;

		case cmdWrite:
		case cmdRead:
		case cmdUpdate:
		case cmdVariablePlay:
		case cmdSearch:
		case cmdStep:
		case cmdAssignSystemMaster:
		case cmdGeneratorCommand:
		case cmdMtcCommand:
		case cmdMove:
		case cmdAdd:
		case cmdSubtract:
		case cmdDropFrameAdjust:
		case cmdProcedure:
		case cmdEvent:
		case cmdGroup:
		case cmdCommandSegment:
		case cmdDeferredVariablePlay:
		case cmdRecordStrobeVariable:
		case cmdWait:
		case cmdResume:
			warning << "MIDI::MachineControl: unimplemented MMC command "
				<< hex << msg[1] << dec
				<< endmsg;
			break;

		default:
			warning << "MIDI::MachineControl: unknown MMC command "
				<< hex << msg[1] << dec
				<< endmsg;
		}

		if (len <= skiplen) {
			break;
		}

		msgbuf += skiplen;
		len -= skiplen;

	} while (len);
}		

int
MachineControl::do_masked_write (byte *msg, size_t len)

{
	/* return the number of bytes "consumed" */

	int retval = msg[1] + 2; /* bytes following + 2 */
	
	switch (msg[2]) {
	case 0x4f:  /* Track Record Ready Status */
		write_track_record_ready (&msg[3], len - 3);
		break;

	default:
		warning << "MIDI::MachineControl: masked write to "
			<< hex << (int) msg[2] << dec
			<< " not implemented"
			<< endmsg;
	}

	return retval;
}

void
MachineControl::write_track_record_ready (byte *msg, size_t len)

{
	size_t n;
	size_t base_track;

	/* Bits 0-4 of the first byte are for special tracks:

	   bit 0: video
	   bit 1: reserved
	   bit 2: time code
	   bit 3: aux track a
	   bit 4: aux track b

	*/

	/* XXX check needed to make sure we don't go outside the
	   support number of tracks.
	*/

	base_track = (msg[0] * 7) - 5;

	for (n = 0; n < 7; n++) {
		if (msg[1] & (1<<n)) {

			/* Only touch tracks that have the "mask"
			   bit set.
			*/

			if (msg[2] & (1<<n)) {
				trackRecordStatus[base_track+n] = true;
				TrackRecordStatusChange (*this, base_track+n,
							 true);
			} else {
				trackRecordStatus[base_track+n] = false;
				TrackRecordStatusChange (*this, base_track+n,
							 false);
			}
		} 

	}
}

int
MachineControl::do_locate (byte *msg, size_t msglen)

{
	if (msg[2] == 0) {
		warning << "MIDI::MMC: locate [I/F] command not supported"
			<< endmsg;
		return 4;
	}

	/* regular "target" locate command */

	Locate (*this, &msg[3]);
	return 8;
}

int
MachineControl::do_shuttle (byte *msg, size_t msglen)

{
	size_t forward;
	byte sh = msg[2];
	byte sm = msg[3];
	byte sl = msg[4];
	size_t left_shift;
	size_t integral;
	size_t fractional;
	float shuttle_speed;

	if (sh & (1<<6)) {
		forward = false;
	} else {
		forward = true;
	}
	
	left_shift = (sh & 0x38);

	integral = ((sh & 0x7) << left_shift) | (sm >> (7 - left_shift));
	fractional = ((sm << left_shift) << 7) | sl;

	shuttle_speed = integral + 
		((float)fractional / (1 << (14 - left_shift)));

	Shuttle (*this, shuttle_speed, forward);

	return 5;
}


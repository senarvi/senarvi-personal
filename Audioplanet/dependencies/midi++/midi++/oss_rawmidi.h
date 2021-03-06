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

    $Id: oss_rawmidi.h,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#ifndef __oss_rawmidi_h__
#define __oss_rawmidi_h__

#include <vector>
#include <string>

#include <fcntl.h>
#include <unistd.h>

#include <midi++/port.h>
#include <midi++/fd_midiport.h>
#include <midi++/port_request.h>

namespace MIDI {

class OSS_RawMidiPort : public FD_MidiPort

{
  public:
	OSS_RawMidiPort (PortRequest &req) 
		: FD_MidiPort (req, "/dev", "midi") {}
	virtual ~OSS_RawMidiPort () {}
};

}; /* namespace MIDI */
#endif // __oss_rawmidi_h__


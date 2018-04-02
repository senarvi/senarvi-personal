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

    $Id: irix_rawmidi.h,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#ifndef __irix_rawmidi__
#define __irix_rawmidi__

#include <vector>
#include <string>

#include <fcntl.h>
#include <unistd.h>

#include <midi++/port.h>
#include <midi++/port_request.h>
#include <dmedia/midi.h>

namespace MIDI {

class IRIX_RawMidiPort : public Port

{
  public:
	IRIX_RawMidiPort (PortRequest &);
	virtual ~IRIX_RawMidiPort ();

	virtual int write (byte *msg, size_t msglen);
	virtual int read (byte *buf, size_t max);	
	virtual int three_byte_msg (byte a, byte b, byte c);

	virtual int selectable() const { return _fd; }
	static vector<string *> *list_devices ();

	static char* irix_midi_register;

  private:
	int _fd;
	MDport _port;
	MDevent* _events;
	int _events_size;

};

}; /* namespace MIDI */

#endif // __irix_rawmidi__


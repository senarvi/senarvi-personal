/*
    Copyright (C) 1998-99 Stephane Conversy

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

    $Id: irix_rawmidi.cc,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>

#include <dmedia/midi.h>

#include <midi++/types.h>
#include <midi++/irix_rawmidi.h>

#include <pbd/error.h>

using namespace MIDI;

char* IRIX_RawMidiPort::irix_midi_register;

IRIX_RawMidiPort::IRIX_RawMidiPort (PortRequest &req)
: Port (req), 
	  _events_size(20)
{
	mdInit();

	req.status = PortRequest::Unknown;

	if((req.mode & O_ACCMODE)==O_RDONLY) {
		_port = mdOpenInPort(irix_midi_register);
	}
	else if((req.mode & O_ACCMODE)==O_WRONLY) {
		_port = mdOpenOutPort(irix_midi_register);
	}
	else {
		return;
	}
	
	if(!_port) {
		return;
	}
	
	_fd = mdGetFd(_port);
	_type = MIDI::Port::IRIX_RawMidi;
	_events = new MDevent[_events_size];
	req.status = PortRequest::OK;
	_ok = true;
}

IRIX_RawMidiPort::~IRIX_RawMidiPort ()
{
	mdClosePort(_port);
	delete [] _events;
}

static int
is_midi_device_name (const struct dirent *dir)
{
	return strncmp (dir->d_name, "midi", 4) == 0;
}

vector<string *> *
IRIX_RawMidiPort::list_devices ()

{
	vector<string *> *devs;
	struct dirent **mididevs;
	int i, n;
	
	if ((n = scandir ("/dev", &mididevs, 
			  (int (*)(dirent_t *))is_midi_device_name, 
			  alphasort)) < 0) {
		return NULL;
	}
	
	if (n == 0) {
		return NULL;
	}
	
	devs = new vector<string *>;
	
	for (i = 0; i < n; i++) {
		string *fullpath;

		fullpath = new string ("/dev/");
		*fullpath += mididevs[i]->d_name;
		devs->push_back (fullpath);

		free (mididevs[i]);
	}

	return devs;
}

int
IRIX_RawMidiPort::write (MIDI::byte *msg, size_t msglen)
{
#if 0
	int nwritten=msglen;
		
	if ((_mode & O_ACCMODE) == O_RDONLY) {
		return -EACCES;
	}
		
	if ((nwritten = ::write (_fd, msg, msglen)) > 0) {
		bytes_written += nwritten;
	}
		
	return nwritten;
#else
	if ((_mode & O_ACCMODE) == O_RDONLY) {
		return -EACCES;
	}


	int nbevents = msglen/3 + 1;
	if(nbevents > _events_size) {
		_events_size = nbevents;
		delete [] _events;
		_events = new MDevent[_events_size];
	}

	int nwritten = 0;
	for(int i=0; i<nbevents; ++i) {
		_events[i].msg[0] = msg[nwritten++];
		if(_events[i].msg[0]==0xF7) {
			error << "******" << __FILE__ << ":" << __LINE__ << ": IRIX_RawMidiPort::write sysex msg not implemented" << endmsg;
		}
		_events[i].msg[1] = msg[nwritten++];
		_events[i].msg[2] = msg[nwritten++];
	}

	mdSend(_port, _events, nbevents);
	bytes_written += nwritten;

	return nwritten;
#endif
}

int
IRIX_RawMidiPort::read (MIDI::byte *buf, size_t max)
{
#if 0
	int nread=max;
		
	if ((_mode & O_ACCMODE) == O_WRONLY) {
		return -EACCES;
	}
		
	if ((nread = ::read (_fd, buf, max)) > 0) {
		bytes_read += nread;

		input_parser.raw_preparse (*this, buf, nread);
		for (int i = 0; i < nread; i++) {
			input_parser.scanner (buf[i]);
		}	
		input_parser.raw_postparse (*this, buf, nread);
	}
	
	return nread;
#else
//	cerr << "******" << __FILE__ << ":" << __LINE__ << ": IRIX_RawMidiPort::read not implemented" << endl;
	
	if ((_mode & O_ACCMODE) == O_WRONLY) {
		return -EACCES;
	}

	int nbevents = max/3;
	if(nbevents > _events_size) {
		_events_size = nbevents;
		delete [] _events;
		_events = new MDevent[_events_size];
	}

	fd_set read_set;
	FD_ZERO(&read_set);
	FD_SET(_fd, &read_set);
	timeval t;
	t.tv_sec = t.tv_usec = 0;

	select(FD_SETSIZE, &read_set, 0, 0, &t);

	if(FD_ISSET(_fd, &read_set)) {
		int nread=0;
		int nbrecv = mdReceive(_port, _events, _events_size);
		
		for(int l=0; l<nbrecv;++l) {
			buf[nread++] = _events[l].msg[0];
			if(_events[l].msg[0] == 0xF7) {
				error << "******" << __FILE__ << ":" << __LINE__ << ": IRIX_RawMidiPort::read sysex msg not implemented" << endmsg;
				mdFree(_events[l].sysexmsg);
			}
			buf[nread++] = _events[l].msg[1];
			buf[nread++] = _events[l].msg[2];
		}

		bytes_read += nread;

		input_parser->raw_preparse (*input_parser, buf, nread);
		for (int i = 0; i < nread; i++) {
			input_parser->scanner (buf[i]);
		}	
		input_parser->raw_postparse (*input_parser, buf, nread);
		return nread;
	}

	return 0;
#endif
}


int
IRIX_RawMidiPort::three_byte_msg (MIDI::byte a, MIDI::byte b, MIDI::byte c)
{
	if((_mode & O_ACCMODE) == O_WRONLY) {
		return -EACCES;
	}
	
	MDevent e;
	e.msg[0]=a;
	e.msg[1]=b;
	e.msg[2]=c;
	e.sysexmsg=0;
	mdSend(_port, &e, 1);

	return 0;
}




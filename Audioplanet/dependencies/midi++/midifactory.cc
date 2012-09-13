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

    $Id: midifactory.cc,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <midi++/types.h>
#include <midi++/factory.h>
#include <midi++/nullmidi.h>
#include <midi++/fifomidi.h>

#ifdef OSS_RAWMIDI_SUPPORT
#include <midi++/oss_rawmidi.h>
#endif // OSS_RAWMIDI_SUPPORT

#ifdef IRIX_RAWMIDI_SUPPORT
#include <midi++/irix_rawmidi.h>
#endif // SGI_RAWMIDI_SUPPORT

#ifdef ALSA_RAWMIDI_SUPPORT
#include <midi++/alsa_rawmidi.h>
#endif // ALSA_RAWMIDI_SUPPORT

#ifdef ALSA_SEQUENCER_SUPPORT
#include <midi++/alsa_sequencer.h>
#endif // ALSA_SEQUENCER_SUPPORT

#ifdef SOLARIS_RAWMIDI_SUPPORT
#include <midi++/solaris_rawmidi.h>
#endif // SOLARIS_RAWMIDI_SUPPORT

using namespace MIDI;

Port *
PortFactory::create_port (PortRequest &req)

{
	Port *port;

	switch (req.type) {
#ifdef OSS_RAWMIDI_SUPPORT
	case Port::OSS_RawMidi:
		port = new OSS_RawMidiPort (req);
		break;
#endif

#ifdef IRIX_RAWMIDI_SUPPORT
	case Port::IRIX_RawMidi:
		port = new IRIX_RawMidiPort (req);
		break;
#endif

#ifdef ALSA_RAWMIDI_SUPPORT
	case Port::ALSA_RawMidi:
		port = new ALSA_RawMidiPort (req);
		break;
#endif

#ifdef ALSA_SEQUENCER_SUPPORT
	case Port::ALSA_Sequencer:
		port = new ALSA_SequencerMidiPort (req);
		break;
#endif

#ifdef SOLARIS_RAWMIDI_SUPPORT
	case Port::Solaris_RawMidi:
		port = new Solaris_RawMidiPort (req);
		break;
#endif
		
	case Port::Null:
		port = new Null_MidiPort (req);
		break;

	case Port::FIFO:
		port = new FIFO_MidiPort (req);
		break;

	default:
		req.status = PortRequest::TypeUnsupported;
		return 0;
	}

	req.status = PortRequest::OK;

	return port;
}

void
PortFactory::add_port_request (vector<PortRequest *> &reqs, 
			           const string &str)
	
{
	PortRequest *req;

	req = new PortRequest;
	req->devname = strdup (str.c_str());
	req->tagname = strdup (str.c_str());

	req->mode = O_RDWR;
	req->type = Port::ALSA_RawMidi;

	reqs.push_back (req);
}

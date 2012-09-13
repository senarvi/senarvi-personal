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

    $Id: port_request.cc,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#include <fcntl.h>
#include <string.h>
#include <midi++/port.h>
#include <midi++/port_request.h>

using namespace MIDI;

PortRequest::PortRequest (const string &xdev, 
			  const string &xtag, 
			  const string &xmode,
			  const string &xtype)

{
	status = OK;

	devname = strdup (xdev.c_str());
	tagname = strdup (xtag.c_str());

	if (xmode == "output" ||
	    xmode == "out" || 
	    xmode == "OUTPUT" ||
	    xmode == "OUT") {
		mode = O_WRONLY;

	} else if (xmode == "input" ||
		   xmode == "in" ||
		   xmode == "INPUT" ||
		   xmode == "IN") {
		mode = O_RDONLY;

	} else if (xmode == "duplex" ||
		   xmode == "DUPLEX" ||
		   xmode == "inout" || 
		   xmode == "INOUT") {
		mode = O_RDWR;
	} else {
		status = Unknown;
	}

	if (xtype == "OSS/RAW" ||
	    xtype == "oss/raw") {
		type = Port::OSS_RawMidi;
	} else if (xtype == "OSS/Sequencer" ||
		   xtype == "oss/sequencer") {
		type = Port::OSS_Sequencer;
	} else if (xtype == "ALSA/RAW" ||
		   xtype == "alsa/raw") {
		type = Port::ALSA_RawMidi;
	} else if (xtype == "ALSA/SEQUENCER" ||
		   xtype == "ALSA/sequencer") {
		type = Port::ALSA_Sequencer;
	} else if (xtype == "IRIX/RAW" ||
		   xtype == "irix/raw") {
		type = Port::IRIX_RawMidi;
	} else if (xtype == "SOLARIS/RAW" ||
		   xtype == "solaris/raw") {
		type = Port::Solaris_RawMidi;
	} else if (xtype == "NULL" ||
		   xtype == "null") {
		type = Port::Null;
	} else if (xtype == "FIFO" ||
		   xtype == "fifo") {
		type = Port::FIFO;
	} else {
		status = Unknown;
	}
}

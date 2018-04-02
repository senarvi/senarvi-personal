//  wavplugin.h
//  Copyright (C) 1999-2001 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef WAVPLUGIN_H
#define WAVPLUGIN_H

#ifdef __GNUG__
#pragma interface
#endif


#include "audioplanet/plugin.h"


class WavPluginInfo : AP::Plugin::Info {

public:

	WavPluginInfo() EXCEPTIONS :
		AP::Plugin::Info("Windows WAV",
		                 "Reads and writes files of Microsoft Wave (.wav) audio file format",
		                 "Seppo Enarvi",
		                 "Seppo Enarvi",
		                 "2000--2001") {}

	void query() const EXCEPTIONS;

};


#endif  // WAVPLUGIN_H

// Local Variables:
// mode:C++
// End:

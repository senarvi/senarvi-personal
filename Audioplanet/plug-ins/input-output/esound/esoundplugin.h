//  esoundplugin.h
//  Copyright (C) 1999-2001 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef ESOUNDPLUGIN_H
#define ESOUNDPLUGIN_H

#ifdef __GNUG__
#pragma interface
#endif


#include "libaudioplanet/plugin.h"


class EsoundPluginInfo : AP::Plugin::Info {

public:

	EsoundPluginInfo() EXCEPTIONS :
		AP::Plugin::Info("EsounD",
		                 "Enlightened Sound Daemon input/output",
		                 "Seppo Enarvi",
		                 "Seppo Enarvi",
		                 "2001") {}

	void query() const EXCEPTIONS;

};


#endif  // ESOUNDPLUGIN_H

// Local Variables:
// mode:C++
// End:

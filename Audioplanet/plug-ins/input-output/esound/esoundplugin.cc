//  esoundplugin.cc
//  Copyright (C) 1999-2001 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifdef __GNUG__
#pragma implementation
#endif


#include "esoundplugin.h"
#include "esoundoutputfilter.h"
#include "libaudioplanet/audioplanet.h"


void EsoundPluginInfo::query() const EXCEPTIONS {

	AP::FilterHandle inputHandle = AP::installFilter(EsoundOutputFilter::create);
}


EsoundPluginInfo AUDIOPLANET_PLUG_IN;

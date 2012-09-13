//  menufile.cc
//  Copyright (C) 1999-2001 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifdef __GNUG__
#pragma implementation
#endif


#include "clipbookkeeping.h"
#include "clip.h"
#include "gui/dialogs.h"
#include "system/log.h"
#include "system/debug.h"


namespace Menu {
    

namespace File {


void newAudioClip()
{
	trace();
	
	
	// Should ask for sample type
	new TdClip<Int16>("Audio Clip");
}


void open()
{
	try
	{
		trace();
		
		
		std::string fileName = GUI::openDialog();
		if (fileName == "") return;
		
		ClipBookkeeping::open(fileName);
	}
	catch (const Exception & e)
	{
		root.error(e.what());
	}
}


void save()
{
	trace();
}


void saveAs()
{
	trace();
}


void quit()
{
	trace();
	
	
	if (ClipBookkeeping::closeAll() == 0)
	{
		std::exit(0);
	}
}


}  // namespace File


}  // namespace Menu

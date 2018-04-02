//  progresswindow.cc
//  Copyright (C) 1999-2001 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifdef __GNUG__
#pragma implementation
#endif


#include "progresswindow.h"
#include "system/debug.h"


namespace GUI {


using SigC::slot;


ProgressWindow::ProgressWindow(const std::string& title) EXCEPTIONS :

	_cancelButton("Cancel")
	
{
	trace();
	
	
	set_title(title);
	
	_progressBar.set_show_text(false);
	_progressBar.set_percentage(0);
	_box.pack_start(_progressBar, true, false, 10);
	_box.pack_start(_separator, false, true, 0);
	_box.pack_start(_cancelButton, false, false, 10);
	add(_box);
	
	_cancelButton.clicked.connect(slot(this, &ProgressWindow::cancelCallback));
	
	show_all();
}


// The default atCancel procedure. Should be overriden
void ProgressWindow::atCancel() EXCEPTIONS
{
	trace();
	
	
	hide();
	delete this;
}


void ProgressWindow::cancelCallback() throw ()
{
	try
	{
		trace();
		atCancel();
	}
	catch (Exception& e)
	{
		raiseException(e);
	}
}


ProgressWindow::~ProgressWindow() EXCEPTIONS
{
	trace();
	_progressBar.hide();
}


void ProgressWindow::setProgress(Float32 progress) EXCEPTIONS
{
	trace();
	_progressBar.set_percentage(progress);
}


}  // namespace GUI

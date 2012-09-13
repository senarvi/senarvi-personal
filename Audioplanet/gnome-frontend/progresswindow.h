//  progresswindow.h
//  Copyright (C) 1999-2000 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef GUI_GNOME_PROGRESSWINDOW_H
#define GUI_GNOME_PROGRESSWINDOW_H

#ifdef __GNUG__
#pragma interface
#endif


#include <string>

#include <gtk--/window.h>
#include <gtk--/box.h>
#include <gtk--/progressbar.h>
#include <gtk--/separator.h>
#include <gtk--/button.h>

#include "gui/iprogresswindow.h"


namespace GUI {


	class ProgressWindow : public Gtk::Window, public IProgressWindow {
	
		Gtk::VBox _box;
		Gtk::ProgressBar _progressBar;
		Gtk::HSeparator _separator;
		Gtk::Button _cancelButton;

		virtual void atCancel() EXCEPTIONS;
		void cancelCallback() throw ();

	public:
	
		ProgressWindow(const std::string& title) EXCEPTIONS;
		~ProgressWindow() EXCEPTIONS;
	
		void setProgress(Float32 progress) EXCEPTIONS;
	
	};
	
	
}  // namespace GUI


#endif // GUI_GNOME_PROGRESSWINDOW_H

// Local Variables:
// mode:C++
// End:


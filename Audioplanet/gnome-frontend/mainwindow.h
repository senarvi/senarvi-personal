//  mainwindow.h
//  Copyright (C) 1999-2001 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef GUI_GNOME_MAINWINDOW_H
#define GUI_GNOME_MAINWINDOW_H

#ifdef __GNUG__
#pragma interface
#endif



#include <gtk--/window.h>
#include <gtk--/box.h>
#include <gtk--/statusbar.h>

#include "gui/imainwindow.h"


namespace GUI {


	class MainWindow : public Gtk::Window
	{

	public:

		MainWindow() EXCEPTIONS;
		MainWindow(const std::string & title = "Audioplanet") EXCEPTIONS;
		~MainWindow() throw () {}

		void showEditMenus() EXCEPTIONS;
		
	private:
	
		Gtk::VBox vbox_;
		Gtk::HBox hbox_;
		Gtk::Statusbar statusbar_;

		gint delete_event_impl(GdkEventAny *) throw ();

	};


}  // namespace GUI


#endif  // GUI_GNOME_MAINWINDOW_H

// Local Variables:
// mode:C++
// End:

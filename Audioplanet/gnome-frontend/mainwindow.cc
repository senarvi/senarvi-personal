//  mainwindow.cc
//  Copyright (C) 1999-2001 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifdef __GNUG__
#pragma implementation
#endif


#include <gtk--/statusbar.h>
#include <gtk--/box.h>
#include <gtk--/menu.h>
#include <gtk--/menubar.h>
#include <gtk--/eventbox.h>
#include <X11/Xlib.h>

#include "config.h"
#include "system/debug.h"
#include "system/types.h"
#include "mainwindow.h"
#include "app/menufile.h"


namespace GUI {


	using SigC::slot;
	using SigC::bind;


	static int screenWidth() EXCEPTIONS
	{
		trace();


		Display * display = XOpenDisplay(NULL);
		if (display == NULL) throw GUIException(LOCATION, "Cannot open display");
		int width = WidthOfScreen(DefaultScreenOfDisplay(display));
		XCloseDisplay(display);
		return width;
	}


	void MainWindow::showEditMenus() EXCEPTIONS
	{
		trace();


		Gtk::MenuBar * menu = manage(new Gtk::MenuBar());

		{
			using namespace Gtk::Menu_Helpers;

			// Create the file menu
			Gtk::Menu * menu_file = manage(new Gtk::Menu());
			MenuList  & list_file = menu_file->items();
     
			list_file.push_back(MenuElem("_New Audio Clip", "<control>n", slot(Menu::File::newAudioClip)));
			list_file.push_back(MenuElem("_Open", "<control>o", slot(Menu::File::open)));

			list_file.push_back(MenuElem("Save", "<control>s", slot(Menu::File::save)));
			list_file.push_back(MenuElem("Save As", slot(Menu::File::saveAs)));

			list_file.push_back(SeparatorElem());

			list_file.push_back(MenuElem("_Quit", "<control>q", slot(Menu::File::quit)));


			// Create the menu bar
			//   Gtk+ does not have O(1) tail lookups so you should build menus
			//   backwards whenever you plan to make lots of access to back().
			MenuList & list = menu->items();
			list.push_front(MenuElem("_File", "<control>f", *menu_file));
		}

		vbox_.pack_start(*menu, false, false);

		show_all();
	}

  
	MainWindow::MainWindow(const std::string & title) EXCEPTIONS :

		vbox_(false, 1)
	
	{
		trace();


		int width = screenWidth();
		if (width < 0) set_usize(800, 50);
		else set_usize(width, 50);
		set_uposition(0, 0);

		set_title(title);

		showEditMenus();

		vbox_.pack_end(statusbar_, true);  // Do not expand the status bar to fill in all the extra space
		add(vbox_);
		show_all();
	}


	// Called when program is closed by window manager
	gint MainWindow::delete_event_impl(GdkEventAny *) throw ()
	{
		try
		{
			trace();
		
		
			exit(0);
			return 0;
		}
		catch (const Exception & e)
		{
			raiseException(e);
			return 0;
		}
	}


}  // namespace GUI

//  opendialog.cc
//  Copyright (C) 1999-2001 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifdef __GNUG__
#pragma implementation
#endif


#include <gtk--/main.h>
#include <gtk--/fileselection.h>
#include <string>

#include "config.h"
#include "system/debug.h"
#include "dialogs.h"


namespace GUI {


using SigC::slot;


class OpenDialog : public Gtk::FileSelection
{
public:
	
	OpenDialog() EXCEPTIONS;
	const std::string& result() const EXCEPTIONS { return _result; }
	
	
private:
	
	std::string _result;
	
	void okCallback() throw ();
	void cancelCallback() throw ();
	gint delete_event_impl(GdkEventAny*) throw ();
};


	OpenDialog::OpenDialog() EXCEPTIONS : Gtk::FileSelection("Open") {

		trace();


		get_ok_button()->clicked.connect(slot(this, &OpenDialog::okCallback));
		get_cancel_button()->clicked.connect(slot(this, &OpenDialog::cancelCallback));		
	}


	void OpenDialog::okCallback() throw () {

		try {

			trace();


			_result = get_filename();
			Gtk::Main::quit();

		} catch (const Exception& e) {
			raiseException(e);
		}
	}


	void OpenDialog::cancelCallback() throw () {

		try {

			trace();


			_result = "";
			Gtk::Main::quit();

		} catch (const Exception& e) {
			raiseException(e);
		}
	}


	gint OpenDialog::delete_event_impl(GdkEventAny*) throw () {

		try {

			trace();


			_result = "";
			Gtk::Main::quit();
			return 1;

		} catch (const Exception& e) {
			raiseException(e);
			return 0;
		}
	}


	std::string openDialog() EXCEPTIONS {

		trace();


		OpenDialog dialog;
		dialog.show();
		Gtk::Main::grab_add(dialog);
		Gtk::Main::run();
		dialog.hide();
		Gtk::Main::grab_remove(dialog);
		return dialog.result();
	}


}  // namespace GUI

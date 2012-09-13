//  messagedialog.cc
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
#include <gtk--/dialog.h>
#include <gtk--/button.h>
#include <gtk--/label.h>
#include <gtk--/main.h>
#include <string>
#include <algorithm>

#include "system/debug.h"
#include "config.h"
#include "dialogs.h"
#include "control.h"


namespace GUI {


using SigC::slot;
using SigC::bind;


class MessageDialog : public Gtk::Dialog
{
	Gtk::Label* _messageLabel;
	int _buttonSet,
	    _numButtons,
	    _result;
	HelpContext _helpContext;
	
	
	void addButton(MessageDialogButton type, const std::string& text, int result);
	
	void resultCallback(int result) throw ();
	void helpCallback() throw ();
	
public:
	
	MessageDialog(const std::string& message, int buttonSet, HelpContext helpContext);
	int result() const { return _result; }
};


void MessageDialog::addButton(MessageDialogButton type, const std::string& text, int result)
{
	if ((_buttonSet & type) != 0)
	{
		Gtk::Button* button = manage(new Gtk::Button(text.c_str()));
		get_action_area()->pack_start(*button);
		button->clicked.connect(bind<int>(slot(this, &MessageDialog::resultCallback), result));
		_numButtons++;
	}
}


MessageDialog::MessageDialog(const std::string& message, int buttonSet, HelpContext helpContext) :

	_buttonSet(buttonSet),
	_numButtons(0),
	_helpContext(helpContext)

{
	trace();
	
	
	set_title("Audioplanet");
	set_position(GTK_WIN_POS_MOUSE);
	get_action_area()->set_border_width(2);
	
	_messageLabel = manage(new Gtk::Label(message.c_str()));
	_messageLabel->set_padding(10, 1);
	_messageLabel->set_line_wrap(true);
	get_vbox()->pack_start(*_messageLabel);
	
	addButton(mbYes, "Yes", mrYes);
	addButton(mbNo, "No", mrNo);
	addButton(mbYesToAll, "Yes to All", mrYes | mrAll);
	addButton(mbNoToAll, "No to All", mrNo | mrAll);
	addButton(mbAbort, "Abort", mrAbort);
	addButton(mbRetry, "Retry", mrRetry);
	addButton(mbIgnore, "Ignore", mrIgnore);
	addButton(mbOk, "Ok", mrOk);
	addButton(mbCancel, "Cancel", mrCancel);
	
	if ((buttonSet & mbHelp) != 0)
	{
		Gtk::Button* helpButton = manage(new Gtk::Button("Help"));
		get_action_area()->pack_start(*helpButton);
		helpButton->clicked.connect(slot(this, &MessageDialog::helpCallback));
		_numButtons++;
	}
	
	show_all();
}


void MessageDialog::resultCallback(int result) throw ()
{
	try
	{
		trace();
		
		
		_result = result;
		Gtk::Main::quit();
	}
	catch (Exception & e)
	{
		raiseException(e);
	}
}


void MessageDialog::helpCallback() throw ()
{
	try
	{
		trace();
		
		
		// Should display help about _helpContext now
	}
	catch (Exception & e)
	{
		raiseException(e);
	}
}


int messageDialog(const std::string& message,

	MessageDialogType type,
	int buttonSet,
	HelpContext helpContext) EXCEPTIONS
	
{
	trace();
	
	
	MessageDialog dialog(message, buttonSet, helpContext);
	
	// Prevent events to anything else than dialog. This
	// way the user cannot start further event loops (otherwise
	// we couldn't know where Gtk::Main::quit() returns)
	Gtk::Main::grab_add(dialog);
	
	// Start a recursive event loop to display the dialog. Loop ends and
	// program continues execution here when we call Gtk::Main::quit()
	// in the callback function
	Gtk::Main::run();
	
	dialog.hide();
	Gtk::Main::grab_remove(dialog);
	return dialog.result();
}


}  // namespace GUI

//  dialogs.h
//  Copyright (C) 1999-2000 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef GUI_DIALOGS_H
#define GUI_DIALOGS_H


#include <string>

#include "app/help.h"
#include "system/exception.h"


namespace GUI {


    enum MessageDialogType {
		mtWarning,
		mtError,
		mtInformation,
		mtConfirmation,
		mtCustom
    };

    enum MessageDialogButton {
		mbYes      = 0x0001,
		mbNo       = 0x0002,
		mbOk       = 0x0004,
		mbCancel   = 0x0008,
		mbAbort    = 0x0010,
		mbRetry    = 0x0020,
		mbIgnore   = 0x0040,
		mbAll      = 0x0080,
		mbNoToAll  = 0x0100,
		mbYesToAll = 0x0200,
		mbHelp     = 0x0400
    };

    enum MessageDialogResult {
		mrYes    = 0x0001,
		mrNo     = 0x0002,
		mrOk     = 0x0004,
		mrCancel = 0x0008,
		mrAbort  = 0x0010,
		mrRetry  = 0x0020,
		mrIgnore = 0x0040,
		mrAll    = 0x0080
    };

    extern int messageDialog(const std::string& message,
	                         MessageDialogType type,
	                         int buttonSet,
	                         HelpContext helpContext) EXCEPTIONS;
    extern std::string openDialog() EXCEPTIONS;


}  // namespace GUI


#endif  // GUI_DIALOGS_H

// Local Variables:
// mode:C++
// End:

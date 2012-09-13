//  textcolor.h
//  Copyright (C) 1999-2000 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef SYSTEM_TEXTCOLOR_H
#define SYSTEM_TEXTCOLOR_H


#include <string>


enum TextAttribute {
    taNone = 0x00,
    taBold = 0x01,
    taUnderscore = 0x02,
    taBlink = 0x04,
    taReverse = 0x08,
    taConcealed = 0x10
};

enum TextColor {
    tcDefault,
    tcBlack,
    tcRed,
    tcGreen,
    tcYellow,
    tcBlue,
    tcMagenta,
    tcCyan,
    tcWhite
};


std::string textColor(int attributes, TextColor foregroundColor, TextColor backgroundColor);
inline std::string defaultTextColor() { return "\033[m"; }


#endif // SYSTEM_TEXTCOLOR_H

// Local Variables:
// mode:C++
// End:

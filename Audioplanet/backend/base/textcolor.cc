//  textcolor.cc
//  Copyright (C) 1999-2000 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#include "textcolor.h"


std::string textColor(int attributes, TextColor foregroundColor, TextColor backgroundColor) {

	std::string result;

	if ((attributes & taBold) != 0)       result += ";01";
	if ((attributes & taUnderscore) != 0) result += ";04";
	if ((attributes & taBlink) != 0)      result += ";05";
	if ((attributes & taReverse) != 0)    result += ";07";
	if ((attributes & taConcealed) != 0)  result += ";08";

	switch (foregroundColor) {
	case tcBlack:
		result += ";30";
		break;
	case tcRed:
		result += ";31";
		break;
	case tcGreen:
		result += ";32";
		break;
	case tcYellow:
		result += ";33";
		break;
	case tcBlue:
		result += ";34";
		break;
	case tcMagenta:
		result += ";35";
		break;
	case tcCyan:
		result += ";36";
		break;
	case tcWhite:
		result += ";37";
	case tcDefault:
		break;
	}

	switch (backgroundColor) {
	case tcBlack:
		result += ";40";
		break;
	case tcRed:
		result += ";41";
		break;
	case tcGreen:
		result += ";42";
		break;
	case tcYellow:
		result += ";43";
		break;
	case tcBlue:
		result += ";44";
		break;
	case tcMagenta:
		result += ";45";
		break;
	case tcCyan:
		result += ";46";
		break;
	case tcWhite:
		result += ";47";
	case tcDefault:
		break;
	}

    if (result.length() > 0) result.erase(0, 1);

    return "\033[" + result + "m";
}

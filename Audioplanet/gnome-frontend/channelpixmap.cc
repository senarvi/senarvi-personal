//  channelpixmap.cc
//  Copyright (C) 1999-2002 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifdef __GNUG__
#pragma implementation
#endif


#include <gtk--/style.h>

#include "channelpixmap.h"
#include "system/debug.h"


namespace GUI {


////////////////////////////////////////////////////////////////////////////////
// ChannelPixmap class
//


	void ChannelPixmap::create(gint width, gint height) EXCEPTIONS
	{
		width_ = width;
		height_ = height;

		/*if (Gdk_Pixmap)*/ Gdk_Pixmap::release();
		Gdk_Pixmap::create(drawingArea_.get_window(), width_, height_);
	}


	void ChannelPixmap::clear() EXCEPTIONS
	{
		Gdk_GC gc = drawingArea_.get_style()->get_white_gc();
		draw_rectangle(gc, true, 0, 0, width_, height_);
	}


////////////////////////////////////////////////////////////////////////////////
// TdChannelPixmap class
//


	void TdChannelPixmap::shade(unsigned int x, Float64 lower, Float64 upper) EXCEPTIONS
	{
		Gdk_GC gc = drawingArea_.get_style()->get_black_gc();
		draw_line(gc, x, zeroLevel() - static_cast<gint>(lower * maxLevel()),
		              x, zeroLevel() - static_cast<gint>(upper * maxLevel()));
	}


}  // namespace GUI

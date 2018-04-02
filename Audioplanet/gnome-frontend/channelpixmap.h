//  channelpixmap.h
//  Copyright (C) 1999-2002 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef GUI_GNOME_CHANNELPIXMAP_H
#define GUI_GNOME_CHANNELPIXMAP_H

#ifdef __GNUG__
#pragma interface
#endif


#include <gdk--/pixmap.h>
#include <gtk--/drawingarea.h>

#include <system/exception.h>
#include <system/types.h>


namespace GUI {


	class ChannelPixmap : public Gdk_Pixmap
	{
	public:

		ChannelPixmap(Gtk::DrawingArea & drawingArea) EXCEPTIONS;

		void create(gint width, gint height) EXCEPTIONS;
		void clear() EXCEPTIONS;

	protected:

		Gtk::DrawingArea & drawingArea_;

		gint width_,
		     height_;
	};


	class TdChannelPixmap : public ChannelPixmap
	{
	public:

		TdChannelPixmap(Gtk::DrawingArea & drawingArea) EXCEPTIONS;
		void shade(unsigned int x, Float64 lower, Float64 upper) EXCEPTIONS;

	private:

		inline unsigned int zeroLevel() const EXCEPTIONS;
		inline unsigned int maxLevel() const EXCEPTIONS;
	};


//
// Inline methods
//


	inline ChannelPixmap::ChannelPixmap(Gtk::DrawingArea & drawingArea) EXCEPTIONS :

		drawingArea_(drawingArea) {}


	inline TdChannelPixmap::TdChannelPixmap(Gtk::DrawingArea & drawingArea) EXCEPTIONS :

		ChannelPixmap(drawingArea) {}


	inline unsigned int TdChannelPixmap::zeroLevel() const EXCEPTIONS
	{
		return height_ / 2;  // Rounded downwards
	}


	inline unsigned int TdChannelPixmap::maxLevel() const EXCEPTIONS
	{
		return height_ / 2;  // Rounded downwards
	}


}  // namespace GUI


#endif  // GUI_GNOME_CHANNELPIXMAP_H

// Local Variables:
// mode:C++
// End:

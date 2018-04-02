//  audiowindow.h
//  Copyright (C) 1999-2002 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef GUI_GNOME_AUDIOWINDOW_H
#define GUI_GNOME_AUDIOWINDOW_H

#ifdef __GNUG__
#pragma interface
#endif



#include <string>
#include <list>

#include <gtk--/window.h>
#include <gtk--/table.h>
#include <gtk--/adjustment.h>
#include <gtk--/scrollbar.h>
#include <gtk--/ruler.h>
#include <gtk--/scrolledwindow.h>
#include <gtk--/box.h>
#include <gtk--/drawingarea.h>

#include "gui/iaudiowindow.h"
#include "channelpixmap.h"


namespace GUI {


class IChannelCanvas : public Gtk::DrawingArea
{
public:
	
	virtual void renderPixmap() EXCEPTIONS = 0;
	virtual void paint() EXCEPTIONS = 0;
//	virtual bool empty() const EXCEPTIONS = 0;
};


class TdClipWindow;


class TdChannelCanvas : public IChannelCanvas
{
public:
	
	TdChannelCanvas(TdClipWindow & clipWindow, const ITdClipWindow::Channel & channel) EXCEPTIONS;
	
	void renderPixmap() EXCEPTIONS;
	
	void paintArea(gint x, gint y, gint width, gint height) EXCEPTIONS;
	void paint() EXCEPTIONS;
	
//	bool empty() const EXCEPTIONS;
	
	gint configure_event_impl(GdkEventConfigure * event) throw ();
	gint expose_event_impl(GdkEventExpose * event) throw ();
	virtual void realize_impl() throw ();
	
	
private:
	
	TdClipWindow & clipWindow_;
	const ITdClipWindow::Channel & channel_;
	
	TdChannelPixmap pixmap_;
	Gdk_Window gdkWindow_;
	bool validWindow_;
};


class ClipWindow : public Gtk::Window, public virtual IClipWindow
{
public:

	ClipWindow(EventListener & eventListener) throw ();
	virtual ~ClipWindow() EXCEPTIONS {}
	
	
	void setTitle(const std::string & title) EXCEPTIONS;
	
	
private:
	
	gint delete_event_impl(GdkEventAny * event) throw ();
	EventListener & eventListener_;
};


class TdClipWindow : public ClipWindow, public ITdClipWindow
{
public:
	
	TdClipWindow(EventListener & eventListener) EXCEPTIONS;
	virtual ~TdClipWindow() EXCEPTIONS {}
	
	
	void addChannel(Channel & channel) EXCEPTIONS;
	
	void configureCanvasWidth(Size canvasWidth) EXCEPTIONS;
	void configureAudioLength(Size audioLength) EXCEPTIONS;
	
	Size sectionStart() const EXCEPTIONS;
	Size sectionWidth() const EXCEPTIONS;
	
	Size pixelsToLength(Size pixels) const EXCEPTIONS;		
	Size positionToUnit(Size position) const EXCEPTIONS;
	
	
private:
	
	friend class TdChannelCanvas;  // Should get rid of this
	
	
	typedef std::list<TdChannelCanvas *> CanvasList;
	
	
	void updateRulerAdjustment() EXCEPTIONS;
	void updateScrollAdjustment() EXCEPTIONS;
	void displayScrollAdjustment() const EXCEPTIONS;
	
	
	void scrollCallback() throw ();
	
	
	Gtk::Table table_;
	Gtk::HRuler topRuler_, bottomRuler_;
	Gtk::Adjustment scrollAdjustment_;
	Gtk::HScrollbar scrollbar_;
	Gtk::ScrolledWindow scrolledWindow_;
	Gtk::VBox vBox_;
	
	Size canvasWidth_;
	Size audioLength_;
	Size horizontalRange_;  // Number of samples shown on the screen
	
	CanvasList canvasList_;
};


//
// Inline methods
//


inline ClipWindow::ClipWindow(EventListener & eventListener) throw () :

	eventListener_(eventListener)
	
{}


inline void ClipWindow::setTitle(const std::string & title) EXCEPTIONS
{
	set_title(title);
}


}  // namespace GUI


#endif  // GUI_GNOME_AUDIOWINDOW_H

// Local Variables:
// mode:C++
// End:

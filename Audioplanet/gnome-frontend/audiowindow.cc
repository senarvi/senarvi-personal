//  audiowindow.cc
//  Copyright (C) 1999-2002 Seppo Enarvi (senarvi@iki.fi)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifdef __GNUG__
#pragma implementation
#endif


#include <algorithm>

#include <gtk--/style.h>
#include <gtk--/adjustment.h>

#include "boost/lexical_cast.hpp"
#include "system/debug.h"
#include "system/thread.h"
#include "system/log.h"
#include "config.h"
#include "audiowindow.h"


using SigC::slot;


namespace GUI {


////////////////////////////////////////////////////////////////////////////////
// TdChannelCanvas class
//


TdChannelCanvas::TdChannelCanvas(TdClipWindow & clipWindow, const ITdClipWindow::Channel & channel) EXCEPTIONS :

	clipWindow_(clipWindow),
	channel_(channel),
	pixmap_(*this)

{
	trace();
	
	
	set_events(GDK_EXPOSURE_MASK |
		GDK_LEAVE_NOTIFY_MASK |
		GDK_BUTTON_PRESS_MASK |
		GDK_POINTER_MOTION_MASK |
		GDK_POINTER_MOTION_HINT_MASK);
}


// Draw a new backing pixmap
void TdChannelCanvas::renderPixmap() EXCEPTIONS
{
	trace();


	Float64 minLevel,
		maxLevel;


	pixmap_.create(width(), height());
	pixmap_.clear();


	if (clipWindow_.sectionWidth() > static_cast<Size>(width()))
	{
		// More than one samples per pixel

		Size position = clipWindow_.sectionStart();

		for (int x = 0; x < width(); x++)
		{
			Size nextPosition = clipWindow_.sectionStart()
				+ clipWindow_.pixelsToLength(x + 1);
			// If we merely calculated multiplies of pixelsPerSample, we might get rounding errors

			channel_.getLevelRange(position, nextPosition - position, minLevel, maxLevel);
			pixmap_.shade(x, minLevel, maxLevel);

			position = nextPosition;
		}
	}
	else
	{
		// One or more pizels per sample

		for (int x = 0; x < width(); x++)
		{
			Size position = clipWindow_.sectionStart()
				+ clipWindow_.pixelsToLength(x + 1);

			channel_.getLevelRange(position, 1, minLevel, maxLevel);
			pixmap_.shade(x, minLevel, maxLevel);
		}
	}
}


// Paint an area to the screen from the backing pixmap
void TdChannelCanvas::paintArea(gint x, gint y, gint width, gint height) EXCEPTIONS
{
	root.debug(
		"Painting area: ("
		+ boost::lexical_cast<std::string>(x) + ", "
		+ boost::lexical_cast<std::string>(y) + ") + "
		+ boost::lexical_cast<std::string>(width) + "x"
		+ boost::lexical_cast<std::string>(height));


//	if (!validWindow_) throw AssertionException(LOCATION);
	
	
	Gdk_GC gc = get_style()->get_fg_gc(get_state());
	get_window().draw_pixmap(gc, pixmap_, x, y, x, y, width, height);
}


// Paint the backing pixmap to the screen
void TdChannelCanvas::paint() EXCEPTIONS
{
	paintArea(0, 0, width(), height());
}


/*
bool TdChannelCanvas::empty() const EXCEPTIONS
{
	return channel_.empty();
}
*/


//
// Callbacks
//


gint TdChannelCanvas::configure_event_impl(GdkEventConfigure * event) throw ()
{
	try
	{
		trace();


		clipWindow_.configureCanvasWidth(event->width);

		renderPixmap();
		paint();

		return 0;
	}
	catch (const Exception & e)
	{
		raiseException(e);
		return 1;
	}
}


// Redraw the exposed area from the backing pixmap
gint TdChannelCanvas::expose_event_impl(GdkEventExpose * event) throw () {

	try {

		paintArea(event->area.x, event->area.y, event->area.width, event->area.height);
		return 1;

	} catch (const Exception & e) {
		raiseException(e);
		return 1;
	}
}


void TdChannelCanvas::realize_impl() throw ()
{
	try {

		trace();


		Gtk::DrawingArea::realize_impl();


//		gdkWindow_ = get_window();
//		validWindow_ = true;
		renderPixmap();

	} catch (const Exception & e) {
		raiseException(e);
	}
}


////////////////////////////////////////////////////////////////////////////////
// TdClipWindow class
//


TdClipWindow::TdClipWindow(TdClipWindow::EventListener & eventListener) EXCEPTIONS :

	ClipWindow(eventListener),
	
	table_(4, 2, false),
	scrollAdjustment_(
		0,   // value
		0,   // lower
		0,   // upper
		1,   // step increment
		1,   // page increment
		1),  // page size
	scrollbar_(scrollAdjustment_),
	
	audioLength_(0),
	horizontalRange_(0)
	
{
	trace();
	
	
	set_usize(500, 500);
	set_policy(true, true, false);
	
	
	add(table_);
	
	table_.attach(topRuler_, 0, 1, 0, 1, GTK_EXPAND | GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL);
	table_.attach(bottomRuler_, 0, 1, 2, 3, GTK_EXPAND | GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL);
	table_.attach(scrollbar_, 0, 1, 3, 4, GTK_EXPAND | GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL);
	table_.attach(scrolledWindow_, 0, 2, 1, 2, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL);
	
	topRuler_.set_metric(GTK_PIXELS);
	bottomRuler_.set_metric(GTK_PIXELS);
	updateRulerAdjustment();
	scrolledWindow_.motion_notify_event.connect(topRuler_.motion_notify_event.slot());
	scrolledWindow_.motion_notify_event.connect(bottomRuler_.motion_notify_event.slot());
	
	scrollAdjustment_.value_changed.connect(slot(this, &TdClipWindow::scrollCallback));
	
	scrolledWindow_.set_policy(GTK_POLICY_AUTOMATIC, GTK_POLICY_NEVER);
	scrolledWindow_.add_with_viewport(vBox_);
	
	
	show_all();
}


void TdClipWindow::addChannel(ITdClipWindow::Channel & channel) EXCEPTIONS
{
	trace();
	
	
	TdChannelCanvas * canvas = manage(new TdChannelCanvas(*this, channel));
	vBox_.pack_end(*canvas);
	canvas->show();
	
	canvasList_.push_back(canvas);
}


//
// Anyone can use these methods to determine the data range currently display on the canvas
//


Size TdClipWindow::sectionStart() const EXCEPTIONS
{
	return static_cast<Size>(scrollbar_.get_adjustment()->get_value());
}


Size TdClipWindow::sectionWidth() const EXCEPTIONS
{
	return horizontalRange_;
}


	//
	// Conversion methods
	//


	Size TdClipWindow::pixelsToLength(Size pixels) const EXCEPTIONS {

		if (canvasWidth_ == 0) return 0;

		return pixels * sectionWidth() / canvasWidth_;
		// We hope pixels * sectionWidth() won't get too large
	}


	Size TdClipWindow::positionToUnit(Size position) const EXCEPTIONS {

		return position;
	}


	//
	// Private methods
	//


	void TdClipWindow::updateRulerAdjustment() EXCEPTIONS {

		gfloat lower = 0,//positionToUnit(sectionStart()),
		       upper = 1000;//positionToUnit(sectionStart() + sectionWidth());


		topRuler_.set_range(lower, upper, 1, 1);
		bottomRuler_.set_range(lower, upper, 1, 1);


#ifdef DEBUG
		log("Ruler range: " + boost::lexical_cast<std::string>(lower) + " - " + boost::lexical_cast<std::string>(upper));
#endif
	}


	void TdClipWindow::updateScrollAdjustment() EXCEPTIONS {

		// We allways have to update also the page size when we want to update the upper limit,
		// because the page size may be larger than the upper limit

		scrollAdjustment_.set_upper(static_cast<gfloat>(audioLength_));
		scrollAdjustment_.set_page_size(static_cast<gfloat>(sectionWidth()));

#ifdef DEBUG
		displayScrollAdjustment();
#endif
	}


	void TdClipWindow::displayScrollAdjustment() const EXCEPTIONS
	{
/*		gfloat pageSize = scrollAdjustment_.get_page_size();
		gfloat upperLimit = scrollAdjustment_.get_upper();

		log.engage();
		log << "Visible range: " << sectionStart()
			<< " - " << sectionStart() + pageSize
			<< " (" << pageSize << " / " << upperLimit << ")";
		log.release();
*/	}


	//
	// Methods other classes use to keep us up to date
	//


	void TdClipWindow::configureCanvasWidth(Size canvasWidth) EXCEPTIONS {

		canvasWidth_ = canvasWidth;
	}


	void TdClipWindow::configureAudioLength(Size audioLength) EXCEPTIONS {

		// If horizontal range was the audio length, we'll keep it that way
		if (horizontalRange_ == audioLength_) horizontalRange_ = audioLength_ = audioLength;
		else audioLength_ = audioLength;
		updateScrollAdjustment();

#ifdef DEBUG
		displayScrollAdjustment();
#endif
	}


//
// Gtk callbacks
//


// Called when window manager closes the window
gint ClipWindow::delete_event_impl(GdkEventAny * event) throw ()
{
	try
	{
		trace();
		
		
		eventListener_.atDelete();
		return 1;  // Don't delete the window automatically
	}
	catch (Exception & e)
	{
		raiseException(e);
		return 1;
	}
}


void TdClipWindow::scrollCallback() throw ()
{
	try
	{
#ifdef DEBUG
		displayScrollAdjustment();
#endif
		
		CanvasList::iterator iter;
		for (iter = canvasList_.begin(); iter != canvasList_.end(); iter++)
		{
			TdChannelCanvas * canvas = *iter;
			
			// Scroll the canvas (currently just draw the whole canvas again)
			canvas->renderPixmap();
			canvas->paint();
		}
		
		updateRulerAdjustment();
	}
	catch (Exception & e)
	{
		raiseException(e);
	}
}


}  // namespace GUI

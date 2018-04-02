#include <algorithm>
#include <functional>

#include <gdk--/colormap.h>
#include <gtk--/style.h>
#include <boost/bind.hpp>

#include "channeldrawingarea.h"


namespace frontend
{

	ChannelDrawingArea::ChannelDrawingArea() :

		gcTable_(numColors_)

	{
		using SigC::slot;


		if (!colorTableAllocated_)
			allocateColors();

		set_events(
			GDK_EXPOSURE_MASK |
			GDK_LEAVE_NOTIFY_MASK |
			GDK_POINTER_MOTION_MASK |
			GDK_POINTER_MOTION_HINT_MASK |
			GDK_BUTTON_PRESS_MASK);

		configure_event.connect(slot(this, &ChannelDrawingArea::onConfigure));
		expose_event.connect(slot(this, &ChannelDrawingArea::onExpose));
	}
	

	void ChannelDrawingArea::allocateColors()
	{
		// Attention: GCC 2.95 fails if we don't import boost namespace.
		using namespace boost;


		colorTable_[COLOR_BACKGROUND].set_rgb(65000, 64000, 63000);
		colorTable_[COLOR_BACKGROUND_SELECTED].set_rgb(30000, 30000, 30000);
		colorTable_[COLOR_WAVE].set_rgb(65535, 0, 0);
		colorTable_[COLOR_WAVE_SELECTED].set_rgb(0, 65535, 65535);
		colorTable_[COLOR_CURSOR].set_rgb(0, 0, 0);
		colorTable_[COLOR_SCALE].set_rgb(0, 0, 65535);

		Gdk_Colormap colormap(Gdk_Colormap::get_system());
		std::for_each(
			colorTable_.begin(),
			colorTable_.end(),
			bind(&Gdk_Colormap::alloc, ref(colormap), _1));
	}


	// Create a backing pixmap and draw the image of the channel. Assumes
	// that
	//  - the pixmap is released or hasn't been created at all and
	//  - the drawing area has been realized and window_ set accordingly.
	//
	void ChannelDrawingArea::createPixmap()
	{
		pixmapWidth_ = width();
		pixmapHeight_ = height();

		// Division of positive integers always truncates, so the height
		// is at least 2 * amplitude_ pixels.
		amplitude_ = pixmapHeight_ / 2;

		pixmap_.create(window_, pixmapWidth_, pixmapHeight_);
		clear();
		shade(10, -0.5, 0.8);
	}


	void ChannelDrawingArea::clear()
	{
		pixmap_.draw_rectangle(
			gcTable_[COLOR_BACKGROUND], true,
			0, 0,
			pixmapWidth_, pixmapHeight_);
	}


	void ChannelDrawingArea::shade(gint x, double lower, double upper)
	{
		if (lower < -1) lower = -1;
		else if (lower > 1) lower = 1;
		if (upper < -1) upper = -1;
		else if (upper > 1) upper = 1;

		pixmap_.draw_line(
			gcTable_[COLOR_WAVE],
			x, amplitude_ - static_cast<gint>(lower * amplitude_),
			x, amplitude_ - static_cast<gint>(upper * amplitude_));
	}


	gint ChannelDrawingArea::onConfigure(GdkEventConfigure * event)
	{
		std::cout << this << " onConfigure" << std::endl;

		// This cannot be done in a 'realize' event handler, because
		// the default implementation of realize_impl emits the
		// 'configure' signal before returning.
		window_ = get_window();


		if (!pixmap_)
		{
			// This is the first configure event.

			for (unsigned int i = 0; i < gcTable_.size(); i++)
			{
				gcTable_[i].create(window_);
				gcTable_[i].set_foreground(colorTable_[i]);
			}

			createPixmap();
		}

		else
			if ((width() != pixmapWidth_) ||
				(height() != pixmapHeight_))
			{
				pixmap_.release();
				createPixmap();
			}

		return 1;
	}


	gint ChannelDrawingArea::onExpose(GdkEventExpose * event)
	{
		std::cout << "onExpose" << std::endl;

		// From GDK Reference Manual:
		// "Most fields in gc are not used for this operation, but notably
		// the clip mask or clip region will be honored."
		//
		// I don't think the current states affects drawing of the pixmap
		// in any way, but this is how it's done in the example programs.

		Gdk_GC gc = get_style()->get_fg_gc(get_state());

		window_.draw_pixmap(
			gc,
			pixmap_,
			event->area.x,
			event->area.y,
			event->area.x,
			event->area.y,
			event->area.width,
			event->area.height);

		return 0;
	}


	std::vector<Gdk_Color> ChannelDrawingArea::colorTable_(ChannelDrawingArea::numColors_);
	bool ChannelDrawingArea::colorTableAllocated_(false);

}

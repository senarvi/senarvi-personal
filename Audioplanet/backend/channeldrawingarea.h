#ifndef CHANNELDRAWINGAREA_H
#define CHANNELDRAWINGAREA_H

#include <vector>

#include <gtk--/drawingarea.h>
#include <boost/utility.hpp>


class Gdk_Color;
class Gdk_Window;
class Gdk_Pixmap;


namespace frontend
{
	
	class ChannelPixmap;


	class ChannelDrawingArea :
		public Gtk::DrawingArea,
		private boost::noncopyable
	{
	public:
		ChannelDrawingArea();


	private:
		enum ColorIndex
		{
			COLOR_BACKGROUND = 0,
			COLOR_BACKGROUND_SELECTED = 1,
			COLOR_WAVE = 2,
			COLOR_WAVE_SELECTED = 3,
			COLOR_CURSOR = 4,
			COLOR_SCALE = 5
		};
		static const unsigned int numColors_ = 6;
		

		void allocateColors();
		void createPixmap();
		void clear();
		void shade(gint x, double lower, double upper);

		// Signal handlers
		gint onConfigure(GdkEventConfigure * event);
		gint onExpose(GdkEventExpose * event);

		// Private data
		static std::vector<Gdk_Color> colorTable_;
		std::vector<Gdk_GC> gcTable_;
		static bool colorTableAllocated_;
		Gdk_Window window_;
		Gdk_Pixmap pixmap_;
		gint16 pixmapWidth_, pixmapHeight_;
		gint16 amplitude_;
	};

}


#endif  // CHANNELDRAWINGAREA_H

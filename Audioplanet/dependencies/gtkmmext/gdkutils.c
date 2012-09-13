/*
    Copyright (C) 1998-99 Paul Barton-Davis
 
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    $Id: gdkutils.c,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <gtkmmext/gdkutils.h>

void
gdk_gc_change_values (GdkGC             *gc,
		      GdkGCValues	*values,
		      gint	         values_mask)
{
  GdkGCPrivate *private = (GdkGCPrivate*) gc;
  XGCValues xvalues;
  unsigned long xvalues_mask;

  g_return_if_fail (values != NULL);

  xvalues.function = GXcopy;
  xvalues.fill_style = FillSolid;
  xvalues.arc_mode = ArcPieSlice;
  xvalues.subwindow_mode = ClipByChildren;
  xvalues.graphics_exposures = True;
  xvalues_mask = GCFunction | GCFillStyle | GCArcMode | GCSubwindowMode | GCGraphicsExposures;

  if (values_mask & GDK_GC_FOREGROUND)
    {
      xvalues.foreground = values->foreground.pixel;
      xvalues_mask |= GCForeground;
    }
  if (values_mask & GDK_GC_BACKGROUND)
    {
      xvalues.background = values->background.pixel;
      xvalues_mask |= GCBackground;
    }
  if ((values_mask & GDK_GC_FONT) && (values->font->type == GDK_FONT_FONT))
    {
      xvalues.font = ((XFontStruct *) ((GdkFontPrivate*) values->font)->xfont)->fid;
      xvalues_mask |= GCFont;
    }
  if (values_mask & GDK_GC_FUNCTION)
    {
      switch (values->function)
	{
	case GDK_COPY:
	  xvalues.function = GXcopy;
	  break;
	case GDK_INVERT:
	  xvalues.function = GXinvert;
	  break;
	case GDK_XOR:
	  xvalues.function = GXxor;
	  break;
	case GDK_CLEAR:
	  xvalues.function = GXclear;
	  break;
	case GDK_AND:
	  xvalues.function = GXand;
	  break;
	case GDK_AND_REVERSE:
	  xvalues.function = GXandReverse;
	  break;
	case GDK_AND_INVERT:
	  xvalues.function = GXandInverted;
	  break;
	case GDK_NOOP:
	  xvalues.function = GXnoop;
	  break;
	case GDK_OR:
	  xvalues.function = GXor;
	  break;
	case GDK_EQUIV:
	  xvalues.function = GXequiv;
	  break;
	case GDK_OR_REVERSE:
	  xvalues.function = GXorReverse;
	  break;
	case GDK_COPY_INVERT:
	  xvalues.function = GXcopyInverted;
	  break;
	case GDK_OR_INVERT:
	  xvalues.function = GXorInverted;
	  break;
	case GDK_NAND:
	  xvalues.function = GXnand;
	  break;
	case GDK_SET:
	  xvalues.function = GXset;
	  break;
	}
      xvalues_mask |= GCFunction;
    }
  if (values_mask & GDK_GC_FILL)
    {
      switch (values->fill)
	{
	case GDK_SOLID:
	  xvalues.fill_style = FillSolid;
	  break;
	case GDK_TILED:
	  xvalues.fill_style = FillTiled;
	  break;
	case GDK_STIPPLED:
	  xvalues.fill_style = FillStippled;
	  break;
	case GDK_OPAQUE_STIPPLED:
	  xvalues.fill_style = FillOpaqueStippled;
	  break;
	}
      xvalues_mask |= GCFillStyle;
    }
  if (values_mask & GDK_GC_TILE)
    {
      xvalues.tile = ((GdkPixmapPrivate*) values->tile)->xwindow;
      xvalues_mask |= GCTile;
    }
  if (values_mask & GDK_GC_STIPPLE)
    {
      xvalues.stipple = ((GdkPixmapPrivate*) values->stipple)->xwindow;
      xvalues_mask |= GCStipple;
    }
  if (values_mask & GDK_GC_CLIP_MASK)
    {
      xvalues.clip_mask = ((GdkPixmapPrivate*) values->clip_mask)->xwindow;
      xvalues_mask |= GCClipMask;
    }
  if (values_mask & GDK_GC_SUBWINDOW)
    {
      xvalues.subwindow_mode = values->subwindow_mode;
      xvalues_mask |= GCSubwindowMode;
    }
  if (values_mask & GDK_GC_TS_X_ORIGIN)
    {
      xvalues.ts_x_origin = values->ts_x_origin;
      xvalues_mask |= GCTileStipXOrigin;
    }
  if (values_mask & GDK_GC_TS_Y_ORIGIN)
    {
      xvalues.ts_y_origin = values->ts_y_origin;
      xvalues_mask |= GCTileStipYOrigin;
    }
  if (values_mask & GDK_GC_CLIP_X_ORIGIN)
    {
      xvalues.clip_x_origin = values->clip_x_origin;
      xvalues_mask |= GCClipXOrigin;
    }
  if (values_mask & GDK_GC_CLIP_Y_ORIGIN)
    {
      xvalues.clip_y_origin = values->clip_y_origin;
      xvalues_mask |= GCClipYOrigin;
    }
 
  if (values_mask & GDK_GC_EXPOSURES)
    xvalues.graphics_exposures = values->graphics_exposures;
  else
    xvalues.graphics_exposures = False;
  xvalues_mask |= GCGraphicsExposures;

  if (values_mask & GDK_GC_LINE_WIDTH)
    {
      xvalues.line_width = values->line_width;
      xvalues_mask |= GCLineWidth;
    }
  if (values_mask & GDK_GC_LINE_STYLE)
    {
      switch (values->line_style)
	{
	case GDK_LINE_SOLID:
	  xvalues.line_style = LineSolid;
	  break;
	case GDK_LINE_ON_OFF_DASH:
	  xvalues.line_style = LineOnOffDash;
	  break;
	case GDK_LINE_DOUBLE_DASH:
	  xvalues.line_style = LineDoubleDash;
	  break;
	}
      xvalues_mask |= GCLineStyle;
    }
  if (values_mask & GDK_GC_CAP_STYLE)
    {
      switch (values->cap_style)
	{
	case GDK_CAP_NOT_LAST:
	  xvalues.cap_style = CapNotLast;
	  break;
	case GDK_CAP_BUTT:
	  xvalues.cap_style = CapButt;
	  break;
	case GDK_CAP_ROUND:
	  xvalues.cap_style = CapRound;
	  break;
	case GDK_CAP_PROJECTING:
	  xvalues.cap_style = CapProjecting;
	  break;
	}
      xvalues_mask |= GCCapStyle;
    }
  if (values_mask & GDK_GC_JOIN_STYLE)
    {
      switch (values->join_style)
	{
	case GDK_JOIN_MITER:
	  xvalues.join_style = JoinMiter;
	  break;
	case GDK_JOIN_ROUND:
	  xvalues.join_style = JoinRound;
	  break;
	case GDK_JOIN_BEVEL:
	  xvalues.join_style = JoinBevel;
	  break;
	}
      xvalues_mask |= GCJoinStyle;
    }

  XChangeGC (private->xdisplay, private->xgc, xvalues_mask, &xvalues);
}


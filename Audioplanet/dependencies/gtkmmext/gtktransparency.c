/* This file is intended to be part of GTK - The GIMP Toolkit
 * Copyright (C) 1999 Paul Barton-Davis
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <gtkmmext/gtktransparency.h>

static void gtk_transparency_class_init    (GtkTransparencyClass *klass);
static void gtk_transparency_init          (GtkTransparency      *tarea);
static void gtk_transparency_realize       (GtkWidget           *widget);
static void gtk_transparency_size_allocate (GtkWidget           *widget,
					    GtkAllocation       *allocation);
static void gtk_transparency_send_configure (GtkTransparency     *tarea);


guint
gtk_transparency_get_type (void)
{
  static guint drawing_area_type = 0;

  if (!drawing_area_type)
    {
      static const GtkTypeInfo drawing_area_info =
      {
	"GtkTransparency",
	sizeof (GtkTransparency),
	sizeof (GtkTransparencyClass),
	(GtkClassInitFunc) gtk_transparency_class_init,
	(GtkObjectInitFunc) gtk_transparency_init,
	/* reserved_1 */ NULL,
        /* reserved_2 */ NULL,
        (GtkClassInitFunc) NULL,
      };

      drawing_area_type = gtk_type_unique (gtk_widget_get_type (), &drawing_area_info);
    }

  return drawing_area_type;
}

static void
gtk_transparency_class_init (GtkTransparencyClass *class)
{
  GtkWidgetClass *widget_class;

  widget_class = (GtkWidgetClass*) class;

  widget_class->realize = gtk_transparency_realize;
  widget_class->size_allocate = gtk_transparency_size_allocate;
}

static void
gtk_transparency_init (GtkTransparency *tarea)
{
  tarea->draw_data = NULL;
}

GtkWidget*
gtk_transparency_new (void)
{
  return GTK_WIDGET (gtk_type_new (gtk_transparency_get_type ()));
}

void
gtk_transparency_size (GtkTransparency *tarea,
		       gint            width,
		       gint            height)
{
  g_return_if_fail (tarea != NULL);
  g_return_if_fail (GTK_IS_TRANSPARENCY (tarea));

  GTK_WIDGET (tarea)->requisition.width = width;
  GTK_WIDGET (tarea)->requisition.height = height;
}

static void
gtk_transparency_realize (GtkWidget *w)
{
  GtkTransparency *tarea;
  GdkWindowAttr attributes;
  gchar *clear_data;
  GdkColor zero;
  GdkColor one;

  gint attributes_mask;

  g_return_if_fail (w != NULL);
  g_return_if_fail (GTK_IS_TRANSPARENCY (w));

  tarea = GTK_TRANSPARENCY (w);
  GTK_WIDGET_SET_FLAGS (w, GTK_REALIZED);

  attributes.window_type = GDK_WINDOW_CHILD;
  attributes.x = w->allocation.x;
  attributes.y = w->allocation.y;
  attributes.width = w->allocation.width;
  attributes.height = w->allocation.height;
  attributes.wclass = GDK_INPUT_OUTPUT;
  attributes.visual = gtk_widget_get_visual (w);
  attributes.colormap = gtk_widget_get_colormap (w);
  attributes.event_mask = gtk_widget_get_events (w) | GDK_EXPOSURE_MASK;

  attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL | GDK_WA_COLORMAP;

  w->window = gdk_window_new (gtk_widget_get_parent_window (w),
				   &attributes, attributes_mask);

  tarea->buffer = gdk_pixmap_new (w->window,
				  w->allocation.width,
				  w->allocation.height, -1);
  tarea->mask  = gdk_pixmap_new  (w->window,
				  w->allocation.width,
				  w->allocation.height, 1);

  /* create a GC for bitmap (depth=1) operations */

  gdk_color_black (gdk_colormap_get_system (), &zero);

  if (zero.pixel != 0) {
      gdk_color_white (gdk_colormap_get_system(), &zero);
      gdk_color_black (gdk_colormap_get_system(), &one);
  } else {
      gdk_color_white (gdk_colormap_get_system(), &one);
  }

  
  tarea->bitmap_gc = gdk_gc_new (tarea->mask);
  gdk_gc_set_foreground (tarea->bitmap_gc, &one);
  gdk_gc_set_background (tarea->bitmap_gc, &zero);

  /* Create a clear bitmap */

  clear_data = (gchar *) g_malloc (w->allocation.width * w->allocation.height);
  memset (clear_data, 0, w->allocation.width * w->allocation.height);
  tarea->clearmask = gdk_pixmap_create_from_data (w->window,
						  clear_data,
						  w->allocation.width,
						  w->allocation.height,
						  1, 
						  &one,
						  &zero);

  /* use it to clear the mask */

  /* XXX use filled rectangle instead */

  gdk_window_copy_area (tarea->mask,
			tarea->bitmap_gc,
			0, 0,
			tarea->clearmask,
			0, 0,
			w->allocation.width,
			w->allocation.height);

  gdk_window_shape_combine_mask (w->window, tarea->mask, 0, 0);
  g_free (clear_data);

  gdk_window_set_user_data (w->window, tarea);

  w->style = gtk_style_attach (w->style, w->window);

  gtk_transparency_send_configure (GTK_TRANSPARENCY (w));
}

static void
gtk_transparency_size_allocate (GtkWidget     *widget,
					    GtkAllocation *allocation)
{
  g_return_if_fail (widget != NULL);
  g_return_if_fail (GTK_IS_TRANSPARENCY (widget));
  g_return_if_fail (allocation != NULL);

  widget->allocation = *allocation;

  if (GTK_WIDGET_REALIZED (widget))
    {
      gdk_window_move_resize (widget->window,
			      allocation->x, allocation->y,
			      allocation->width, allocation->height);

      gtk_transparency_send_configure (GTK_TRANSPARENCY (widget));
    }
}

static void
gtk_transparency_send_configure (GtkTransparency *tarea)
{
  GtkWidget *widget;
  GdkEventConfigure event;

  widget = GTK_WIDGET (tarea);

  event.type = GDK_CONFIGURE;
  event.window = widget->window;
  event.x = widget->allocation.x;
  event.y = widget->allocation.y;
  event.width = widget->allocation.width;
  event.height = widget->allocation.height;
  
  gtk_widget_event (widget, (GdkEvent*) &event);
}

GdkWindow *
gtk_transparency_buffer (GtkTransparency *tarea)

{
    return tarea->buffer;
}

void
gtk_transparency_project (GtkTransparency *tarea)

{
    GtkWidget *w = GTK_WIDGET(tarea);

    /* Reshape the window */

    gdk_window_shape_combine_mask (w->window, tarea->mask, 0, 0);

    /* flush the current pixmap into the window */

    gdk_draw_pixmap (w->window, w->style->fg_gc[GTK_STATE_NORMAL],
		     tarea->buffer, 0, 0, 0, 0, 
		     w->allocation.width, w->allocation.height);
}

void
gtk_transparency_wipe (GtkTransparency *tarea)

{
    GtkWidget *w = GTK_WIDGET(tarea);

    gdk_window_copy_area (tarea->mask,
			  tarea->bitmap_gc,
			  0, 0,
			  tarea->clearmask,
			  0, 0,
			  w->allocation.width,
			  w->allocation.height);
}

/* Drawing Functions */

#define CHECK_GC(t,gc) \
    if (gc == NULL) { \
       gc = GTK_WIDGET(t)->style->fg_gc[GTK_WIDGET_STATE(GTK_WIDGET(t))];\
    }

void 
gtk_transparency_draw_rectangle	 (GtkTransparency  *tarea,
				  GdkGC	       *gc,
				  gint		filled,
				  gint		x,
				  gint		y,
				  gint		width,
				  gint		height)

{
    CHECK_GC(tarea,gc);
    gdk_draw_rectangle (tarea->mask, tarea->bitmap_gc, 
			filled, x, y, width, height);
    gdk_draw_rectangle (tarea->buffer, gc, filled, x, y, width, height);
}


void gtk_transparency_draw_point	    (GtkTransparency  *tarea,
				     GdkGC	       *gc,
				     gint		x,
				     gint		y)

{
        CHECK_GC(tarea,gc);
	gdk_draw_point (tarea->mask, tarea->bitmap_gc, x, y);
	gdk_draw_point (tarea->buffer, gc, x, y);
}

void gtk_transparency_draw_line	    (GtkTransparency  *tarea,
				     GdkGC	       *gc,
				     gint		x1,
				     gint		y1,
				     gint		x2,
				     gint		y2)

{
        CHECK_GC(tarea,gc);
	gdk_draw_line (tarea->mask, tarea->bitmap_gc, x1, y1, x2, y2);
	gdk_draw_line (tarea->buffer, gc, x1, y1, x2, y2);
}

void gtk_transparency_draw_arc	    (GtkTransparency  *tarea,
				     GdkGC	       *gc,
				     gint		filled,
				     gint		x,
				     gint		y,
				     gint		width,
				     gint		height,
				     gint		angle1,
				     gint		angle2)

{
        CHECK_GC(tarea,gc);
	gdk_draw_arc (tarea->mask, tarea->bitmap_gc, filled, x, y,
		      width, height, angle1, angle2);
	gdk_draw_arc (tarea->buffer, gc, filled, x, y,
		      width, height, angle1, angle2);
}

void gtk_transparency_draw_polygon   (GtkTransparency  *tarea,
				     GdkGC	       *gc,
				     gint		filled,
				     GdkPoint     *points,
				     gint		npoints)
{
        CHECK_GC(tarea,gc);
	gdk_draw_polygon (tarea->mask, tarea->bitmap_gc, filled,
			  points, npoints);
	gdk_draw_polygon (tarea->buffer, gc, filled,
			  points, npoints);
}

void gtk_transparency_draw_string    (GtkTransparency  *tarea,
				     GdkFont      *font,
				     GdkGC	       *gc,
				     gint		x,
				     gint		y,
				     const gchar  *string)

{
        CHECK_GC(tarea,gc);
	gdk_draw_string (tarea->mask, font, tarea->bitmap_gc, x, y, string);
	gdk_draw_string (tarea->buffer, font, gc, x, y, string);
}

void gtk_transparency_draw_text	    (GtkTransparency  *tarea,
				     GdkFont      *font,
				     GdkGC	       *gc,
				     gint		x,
				     gint		y,
				     const gchar  *text,
				     gint		text_length)
{
        CHECK_GC(tarea,gc);
	gdk_draw_text (tarea->mask, font, tarea->bitmap_gc, x, y,
			 text, text_length);
	gdk_draw_text (tarea->buffer, font, gc, x, y, 
			 text, text_length);
}	

void gtk_transparency_draw_text_wc   (GtkTransparency	 *tarea,
				     GdkFont	 *font,
				     GdkGC		 *gc,
				     gint		  x,
				     gint		  y,
				     const GdkWChar *text,
				     gint		  text_length)
{
        CHECK_GC(tarea,gc);
	gdk_draw_text_wc (tarea->mask, font, tarea->bitmap_gc, x, y,
			  text, text_length);
	gdk_draw_text_wc (tarea->buffer, font, gc, x, y, 
			  text, text_length);
}

void gtk_transparency_draw_pixmap    (GtkTransparency  *tarea,
				     GdkGC	       *gc,
				     GdkDrawable       *src,
				     gint		xsrc,
				     gint		ysrc,
				     gint		xdest,
				     gint		ydest,
				     gint		width,
				     gint		height)
{
        CHECK_GC(tarea,gc);
	gdk_draw_pixmap (tarea->mask, tarea->bitmap_gc,
			 src, xsrc, ysrc, xdest, ydest,
			 width, height);
	gdk_draw_pixmap (tarea->buffer, gc,
			 src, xsrc, ysrc, xdest, ydest,
			 width, height);
}

void gtk_transparency_draw_image	    (GtkTransparency  *tarea,
				     GdkGC	       *gc,
				     GdkImage     *image,
				     gint		xsrc,
				     gint		ysrc,
				     gint		xdest,
				     gint		ydest,
				     gint		width,
				     gint		height)
{
        CHECK_GC(tarea,gc);
	gdk_draw_image (tarea->mask, tarea->bitmap_gc,
			 image, xsrc, ysrc, xdest, ydest,
			 width, height);
	gdk_draw_image (tarea->buffer, gc,
			 image, xsrc, ysrc, xdest, ydest,
			 width, height);
}

void gtk_transparency_draw_points    (GtkTransparency  *tarea,
				     GdkGC	       *gc,
				     GdkPoint     *points,
				     gint		npoints)
{
        CHECK_GC(tarea,gc);
	gdk_draw_points (tarea->mask, tarea->bitmap_gc, points, npoints);
	gdk_draw_points (tarea->buffer, gc, points, npoints);
}

void gtk_transparency_draw_segments  (GtkTransparency  *tarea,
				     GdkGC	       *gc,
				     GdkSegment   *segs,
				     gint		nsegs)
{
        CHECK_GC(tarea,gc);
	gdk_draw_segments (tarea->mask, tarea->bitmap_gc, segs, nsegs);
	gdk_draw_segments (tarea->buffer, gc, segs, nsegs);
}

void gtk_transparency_draw_lines     (GtkTransparency  *tarea,
				     GdkGC        *gc,
				     GdkPoint     *points,
				     gint          npoints)
{
        CHECK_GC(tarea,gc);
	gdk_draw_lines (tarea->mask, tarea->bitmap_gc, points, npoints);
	gdk_draw_lines (tarea->buffer, gc, points, npoints);
}	

void       
gtk_transparency_set_mask_gc (GtkTransparency *tarea,
			      GdkGCValues     *values,
			      gint             values_mask)

{
	gdk_gc_change_values (tarea->bitmap_gc,
			      values, 
			      values_mask);
}

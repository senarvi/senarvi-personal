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

#ifndef __GTK_TRANSPARENCY_H__
#define __GTK_TRANSPARENCY_H__

#include <gdk/gdk.h>
#include <gtk/gtkwidget.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define GTK_TRANSPARENCY(obj)          GTK_CHECK_CAST (obj, gtk_transparency_get_type (), GtkTransparency)
#define GTK_TRANSPARENCY_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, gtk_transparency_get_type (), GtkTransparencyClass)
#define GTK_IS_TRANSPARENCY(obj)       GTK_CHECK_TYPE (obj, gtk_transparency_get_type ())


typedef struct _GtkTransparency       GtkTransparency;
typedef struct _GtkTransparencyClass  GtkTransparencyClass;

struct _GtkTransparency
{
  GtkWidget widget;
  GdkPixmap *buffer;
  GdkBitmap *mask;
  GdkBitmap *clearmask;
  GdkGC *bitmap_gc;	 
  gpointer draw_data;
};

struct _GtkTransparencyClass
{
	GtkWidgetClass parent_class;
};

guint      gtk_transparency_get_type   (void);
GtkWidget* gtk_transparency_new        (void);
void       gtk_transparency_size       (GtkTransparency      *darea,
					gint                 width,
					gint                 height);
GdkWindow* gtk_transparency_buffer     (GtkTransparency*);
void       gtk_transparency_project    (GtkTransparency*);
void       gtk_transparency_wipe       (GtkTransparency*);
void       gtk_transparency_set_mask_gc (GtkTransparency*,
					 GdkGCValues *values,
					 gint         values_mask);

/* Drawing
 */

void gtk_transparency_draw_point    (GtkTransparency  *drawable,
				     GdkGC	       *gc,
				     gint		x,
				     gint		y);
void gtk_transparency_draw_line	    (GtkTransparency  *drawable,
				     GdkGC	       *gc,
				     gint		x1,
				     gint		y1,
				     gint		x2,
				     gint		y2);
void gtk_transparency_draw_rectangle (GtkTransparency  *drawable,
				     GdkGC	       *gc,
				     gint		filled,
				     gint		x,
				     gint		y,
				     gint		width,
				     gint		height);
void gtk_transparency_draw_arc	    (GtkTransparency  *drawable,
				     GdkGC	       *gc,
				     gint		filled,
				     gint		x,
				     gint		y,
				     gint		width,
				     gint		height,
				     gint		angle1,
				     gint		angle2);
void gtk_transparency_draw_polygon   (GtkTransparency  *drawable,
				     GdkGC	       *gc,
				     gint		filled,
				     GdkPoint     *points,
				     gint		npoints);
void gtk_transparency_draw_string    (GtkTransparency  *drawable,
				     GdkFont      *font,
				     GdkGC	       *gc,
				     gint		x,
				     gint		y,
				     const gchar  *string);
void gtk_transparency_draw_text	    (GtkTransparency  *drawable,
				     GdkFont      *font,
				     GdkGC	       *gc,
				     gint		x,
				     gint		y,
				     const gchar  *text,
				     gint		text_length);
void gtk_transparency_draw_text_wc   (GtkTransparency	 *drawable,
				     GdkFont	 *font,
				     GdkGC		 *gc,
				     gint		  x,
				     gint		  y,
				     const GdkWChar *text,
				     gint		  text_length);
void gtk_transparency_draw_pixmap    (GtkTransparency  *drawable,
				     GdkGC	       *gc,
				     GdkDrawable       *src,
				     gint		xsrc,
				     gint		ysrc,
				     gint		xdest,
				     gint		ydest,
				     gint		width,
				     gint		height);
void gtk_transparency_draw_image	    (GtkTransparency  *drawable,
				     GdkGC	       *gc,
				     GdkImage     *image,
				     gint		xsrc,
				     gint		ysrc,
				     gint		xdest,
				     gint		ydest,
				     gint		width,
				     gint		height);
void gtk_transparency_draw_points    (GtkTransparency  *drawable,
				     GdkGC	       *gc,
				     GdkPoint     *points,
				     gint		npoints);
void gtk_transparency_draw_segments  (GtkTransparency  *drawable,
				     GdkGC	       *gc,
				     GdkSegment   *segs,
				     gint		nsegs);
void gtk_transparency_draw_lines     (GtkTransparency  *drawable,
				     GdkGC        *gc,
				     GdkPoint     *points,
				     gint          npoints);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __GTK_TRANSPARENCY_H__ */

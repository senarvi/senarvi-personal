/* This file is intended to be part of GTK - The GIMP Toolkit
 * Copyright (C) 1999 Eric S. Tiedemann (est@hyperreal.org)
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

#ifndef __GTK_RGB_H__
#define __GTK_RGB_H__

#include <gdk/gdk.h>
#include <gdk/gdkrgb.h>
#include <gtk/gtk.h>
#include <gtk/gtkwidget.h>

#ifdef __cplusplus
extern "C" {
#endif

extern guint gtk_rgb_get_type();

#define GTK_TYPE_RGB		(gtk_rgb_get_type ())
#define GTK_RGB(obj)		(GTK_CHECK_CAST ((obj), GTK_TYPE_RGB, GtkRgb))
#define GTK_RGB_CLASS(klass)	(GTK_CHECK_CLASS_CAST ((klass), GTK_TYPE_RGB, GtkRgbClass))
#define GTK_IS_RGB(obj)		(GTK_CHECK_TYPE ((obj), GTK_TYPE_RGB))
#define GTK_IS_RGB_CLASS(klass)	(GTK_CHECK_CLASS_TYPE ((klass), GTK_TYPE_RGB))


typedef struct _GtkRgb	   GtkRgb;
typedef struct _GtkRgbClass  GtkRgbClass;

struct _GtkRgb
{
  GtkWidget widget;
  guchar *img;
  guint h, w;
  gboolean can_resizep;
};

struct _GtkRgbClass
{
  GtkWidgetClass parent_class;
};

GtkWidget* gtk_rgb_new(unsigned height, unsigned width, gboolean can_resizep);

#ifdef __cplusplus
}
#endif
/* __GTK_RGB_H__ */
#endif

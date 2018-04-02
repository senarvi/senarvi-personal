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

#ifndef __GTK_LEVELHOLD_H__
#define __GTK_LEVELHOLD_H__

#include <gdk/gdk.h>
#include <gdk/gdkrgb.h>
#include <gtk/gtk.h>
#include <gtk/gtkwidget.h>

#include "gtkrgb.h"

#ifdef __cplusplus
extern "C" {
#endif

extern guint gtk_level_hold_get_type();

#define GTK_TYPE_LEVEL_HOLD		(gtk_level_hold_get_type ())
#define GTK_LEVEL_HOLD(obj)		(GTK_CHECK_CAST ((obj), GTK_TYPE_LEVEL_HOLD, GtkLevelHold))
#define GTK_LEVEL_HOLD_CLASS(klass)	(GTK_CHECK_CLASS_CAST ((klass), GTK_TYPE_LEVEL_HOLD, GtkLevelHoldClass))
#define GTK_IS_LEVEL_HOLD(obj)		(GTK_CHECK_TYPE ((obj), GTK_TYPE_LEVEL_HOLD))
#define GTK_IS_LEVEL_HOLD_CLASS(klass)	(GTK_CHECK_CLASS_TYPE ((klass), GTK_TYPE_LEVEL_HOLD))


typedef struct _GtkLevelHold	   GtkLevelHold;
typedef struct _GtkLevelHoldClass  GtkLevelHoldClass;

struct _GtkLevelHold
{
  GtkRgb rgb;
  int *data;
  int options;
  unsigned steps;
  unsigned hold_pixels;
  float hold_factor;
};

enum gtk_level_hold_option
{
  GTK_LEVEL_HOLD_HORIZONTAL = 1,
  GTK_LEVEL_HOLD_FADE = 2,
  GTK_LEVEL_HOLD_FIXED = 4,
};

struct _GtkLevelHoldClass
{
  GtkRgbClass parent_class;
};

GtkWidget* gtk_level_hold_new(unsigned height, unsigned width, 
                              int options, unsigned steps, 
                              unsigned hold_pixels, float hold_factor);
void     gtk_level_hold_set(GtkLevelHold *, float);
void     gtk_level_hold_clear(GtkLevelHold *);

#ifdef __cplusplus
}
#endif
/* __GTK_LEVELHOLD_H__ */
#endif

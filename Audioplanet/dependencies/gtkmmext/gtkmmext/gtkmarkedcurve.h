/* GTK - The GIMP Toolkit
 * Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
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

/*
 * Modified by the GTK+ Team and others 1997-1999.  See the AUTHORS
 * file for a list of people on the GTK+ Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GTK+ at ftp://ftp.gtk.org/pub/gtk/. 
 */

#ifndef __GTK_MARKED_CURVE_H__
#define __GTK_MARKED_CURVE_H__


#include <gdk/gdk.h>
#include <gtk/gtkdrawingarea.h>
#include <gtk/gtkcurve.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#define GTK_TYPE_MARKED_CURVE                  (gtk_marked_curve_get_type ())
#define GTK_MARKED_CURVE(obj)                  (GTK_CHECK_CAST ((obj), GTK_TYPE_MARKED_CURVE, GtkMarkedCurve))
#define GTK_MARKED_CURVE_CLASS(klass)          (GTK_CHECK_CLASS_CAST ((klass), GTK_TYPE_MARKED_CURVE, GtkMarkedCurveClass))
#define GTK_IS_MARKED_CURVE(obj)               (GTK_CHECK_TYPE ((obj), GTK_TYPE_MARKED_CURVE))
#define GTK_IS_MARKED_CURVE_CLASS(klass)       (GTK_CHECK_CLASS_TYPE ((klass), GTK_TYPE_MARKED_CURVE))


typedef struct _GtkMarkedCurve	GtkMarkedCurve;
typedef struct _GtkMarkedCurveClass	GtkMarkedCurveClass;
typedef struct _GtkMarkedCurveMarkPoint GtkMarkedCurveMarkPoint;

struct _GtkMarkedCurveMarkPoint {
    gfloat x;
    gfloat y;
};

struct _GtkMarkedCurve
{
    GtkDrawingArea graph;
    
    gint cursor_type;
    gfloat min_x;
    gfloat max_x;
    gfloat min_y;
    gfloat max_y;
    GdkPixmap *pixmap;
    GtkCurveType curve_type;
    gint height;                  /* (cached) graph height in pixels */
    gint grab_point;              /* point currently grabbed */
    gint last;
    
    /* (cached) curve points: */
    gint num_points;
    GdkPoint *point;
    
    /* control points: */
    gint num_ctlpoints;           /* number of control points */
    gfloat (*ctlpoint)[2];        /* array of control points */
    
    /* mark points */
    guint  num_markpoints;
    guint  markpoint_size;
    GtkMarkedCurveMarkPoint *markpoint;
    gint   grab_mark;
};

struct _GtkMarkedCurveClass
{
  GtkDrawingAreaClass parent_class;

  void (* curve_type_changed) (GtkMarkedCurve *curve);
  void (* curve_mark)  (GtkMarkedCurve *curve, guint n, GtkMarkedCurveMarkPoint *);
  void (* curve_cursor_motion) (GtkMarkedCurve *curve, gfloat x, gfloat y);
};


GtkType		gtk_marked_curve_get_type	(void);
GtkWidget*	gtk_marked_curve_new		(void);
void		gtk_marked_curve_reset		(GtkMarkedCurve *curve);
void		gtk_marked_curve_set_gamma	(GtkMarkedCurve *curve, gfloat gamma);
void		gtk_marked_curve_set_range	(GtkMarkedCurve *curve,
					 gfloat min_x, gfloat max_x,
					 gfloat min_y, gfloat max_y);
void		gtk_marked_curve_get_vector	(GtkMarkedCurve *curve,
					 int veclen, gfloat vector[]);
void		gtk_marked_curve_set_vector	(GtkMarkedCurve *curve,
					 int veclen, gfloat vector[]);
void		gtk_marked_curve_set_curve_type (GtkMarkedCurve *curve, GtkCurveType type);
gint            gtk_marked_curve_add_markpoint (GtkMarkedCurve *curve, gfloat x, gfloat y);
void            gtk_marked_curve_remove_markpoint (GtkMarkedCurve *curve, gint);
void            gtk_marked_curve_clear_markpoints (GtkMarkedCurve *curve);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __GTK_MARKED_CURVE_H__ */

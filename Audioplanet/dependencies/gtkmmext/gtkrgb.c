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

#include <stdio.h>
#include <math.h>
#include <assert.h>

#include <gtkmmext/gtkrgb.h>

static GtkWidgetClass *parent_class;

static void
gtk_rgb_draw(GtkWidget *w, GdkRectangle *a)
{
  GtkRgb *rgb = GTK_RGB(w);

  g_return_if_fail(w != NULL);
  g_return_if_fail(w->window != NULL);
  g_return_if_fail(GTK_IS_RGB(w));

  if (rgb->img == NULL) {
	  return;
  }
  
  gdk_draw_rgb_image(w->window, w->style->fg_gc[GTK_STATE_NORMAL],
		     0, 0,
		     rgb->w, rgb->h,
		     GDK_RGB_DITHER_NORMAL,
		     (guchar *) rgb->img,
		     3 * rgb->w);

  /* gdk_flush(); */
}

static void
gtk_rgb_map(GtkWidget *w)
{
  int i, j;
  GdkVisual *visual;
  GtkRgb *rgb = GTK_RGB(w);
  GdkColormap *c;
  GtkStyle *style;
  GdkColor *color;
  GdkColor bg_color;

  /* image is already inited to black */
  /* draw it */
  gtk_rgb_draw(w, 0);

  if (parent_class->map)
    parent_class->map(w);
}

static void
gtk_rgb_send_configure (GtkRgb *rgb)
{
  GtkWidget *widget;
  GdkEventConfigure event;

  widget = GTK_WIDGET (rgb);

  event.type = GDK_CONFIGURE;
  event.window = widget->window;
  event.send_event = TRUE;
  event.x = widget->allocation.x;
  event.y = widget->allocation.y;
  event.width = widget->allocation.width;
  event.height = widget->allocation.height;
  
  gtk_widget_event (widget, (GdkEvent*) &event);
}

static void
gtk_rgb_realize (GtkWidget *widget)
{
  GtkRgb *rgb;
  GdkWindowAttr attributes;
  gint attributes_mask;

  g_return_if_fail (widget != NULL);
  g_return_if_fail (GTK_IS_RGB (widget));

  rgb = GTK_RGB (widget);
  GTK_WIDGET_SET_FLAGS (widget, GTK_REALIZED);

  attributes.window_type = GDK_WINDOW_CHILD;
  attributes.x = widget->allocation.x;
  attributes.y = widget->allocation.y;
  attributes.width = widget->allocation.width;
  attributes.height = widget->allocation.height;
  attributes.wclass = GDK_INPUT_OUTPUT;
  attributes.visual = gdk_rgb_get_visual();
  attributes.colormap = gdk_rgb_get_cmap();
  attributes.event_mask = gtk_widget_get_events (widget) | GDK_EXPOSURE_MASK;

  attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL | GDK_WA_COLORMAP;

  widget->window = gdk_window_new(gtk_widget_get_parent_window (widget), 
				  &attributes, attributes_mask);
  gdk_window_set_user_data (widget->window, rgb);

  widget->style = gtk_style_attach (widget->style, widget->window);
  gtk_style_set_background (widget->style, widget->window, GTK_STATE_NORMAL);

  gtk_rgb_send_configure (GTK_RGB (widget));
}

static void
gtk_rgb_size_allocate (GtkWidget     *widget,
				GtkAllocation *allocation)
{
  GtkRgb *rgb;

  g_return_if_fail (widget != NULL);
  g_return_if_fail (GTK_IS_RGB (widget));
  g_return_if_fail (allocation != NULL);

  rgb = GTK_RGB(widget);

  widget->allocation = *allocation;

  if (!rgb->can_resizep)
    {
      g_free(rgb->img);
      rgb->img = g_new0(guchar, rgb->h * rgb->w * 3);
    }
  else
    {
      rgb->h = allocation->height;
      rgb->w = allocation->width;

      g_free(rgb->img);
      rgb->img = g_new0(guchar, rgb->h * rgb->w * 3);
    }

  if (GTK_WIDGET_REALIZED (widget))
    {
      gdk_window_move_resize (widget->window,
			      allocation->x, allocation->y,
			      allocation->width, allocation->height);

      gtk_rgb_send_configure (GTK_RGB (widget));
    }
}

static gint
gtk_rgb_expose(GtkWidget *w, GdkEventExpose *e)
{
  gtk_rgb_draw(w, 0);
  return FALSE;
}

static void
gtk_rgb_destroy(GtkObject *object)
{
  GtkRgb *rgb;

  g_return_if_fail(object != NULL);
  g_return_if_fail(GTK_IS_RGB(object));

  rgb = GTK_RGB(object);

  g_free(rgb->img);

  rgb->img = 0;
  rgb->h = 0;
  rgb->w = 0;

  /*fprintf(stderr, "destroying rgb\n");*/
  if (GTK_OBJECT_CLASS(parent_class)->destroy)
    GTK_OBJECT_CLASS(parent_class)->destroy(object);
}

static void
gtk_rgb_class_init(GtkRgbClass *class)
{
  GtkObjectClass *object_class;
  GtkWidgetClass *widget_class;

  /* we'll need this stuff */
  gdk_rgb_init();

  parent_class = gtk_type_class(gtk_widget_get_type());

  object_class = (GtkObjectClass*) class;
  widget_class = (GtkWidgetClass*) class;

  object_class->destroy = gtk_rgb_destroy;
  widget_class->realize = gtk_rgb_realize;
  widget_class->size_allocate = gtk_rgb_size_allocate;
  widget_class->expose_event = gtk_rgb_expose;
  widget_class->map = gtk_rgb_map;
  widget_class->draw = gtk_rgb_draw;
}

static void
gtk_rgb_init(GtkRgb *rgb)
{
}

guint
gtk_rgb_get_type()
{
  static guint mytype = 0;

  if (!mytype)
    {
      GtkTypeInfo rgb_info =
      {
	"GtkRgb",
	sizeof(GtkRgb),
	sizeof(GtkRgbClass),
	(GtkClassInitFunc) gtk_rgb_class_init,
	(GtkObjectInitFunc) gtk_rgb_init,
	(GtkArgSetFunc) NULL,
	(GtkArgGetFunc) NULL
      };

      mytype = gtk_type_unique (gtk_widget_get_type(), &rgb_info);
    }

  return mytype;
}

GtkWidget*
gtk_rgb_new(unsigned h, unsigned w, gboolean can_resizep)
{
  GtkRgb *rgb = gtk_type_new (gtk_rgb_get_type ());

  rgb->h = h;
  rgb->w = w;
  rgb->img = NULL;
  rgb->can_resizep = can_resizep;

  /* request a pleasant default size */
  GTK_WIDGET(rgb)->requisition.width = rgb->w;
  GTK_WIDGET(rgb)->requisition.height = rgb->h;

  return GTK_WIDGET (rgb);
}

/*#define TEST*/
#ifdef TEST

gint
delete_event(GtkWidget *w, GdkEvent *e, gpointer o)
{
  gtk_main_quit();
}

gint
bar(gpointer o)
{
  GtkRgb *rgb = GTK_RGB(o);
  static int ghee = 255;
  rgb->img[0] = ghee;
  ghee = ((ghee == 255) ? 0 : 255);
  gtk_widget_draw(GTK_WIDGET(rgb), 0);
  return TRUE;
}

int
main(int argc, char **argv)
{
  GtkWidget *w, *rgb;

  gtk_init(&argc, &argv);

  w = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  
  gtk_window_set_policy(GTK_WINDOW(w), TRUE, TRUE, TRUE);

  gtk_signal_connect(GTK_OBJECT(w), "delete_event", 
		     GTK_SIGNAL_FUNC(delete_event), (gpointer) 0);

  rgb = gtk_rgb_new(200, 10, TRUE);

  gtk_container_add(GTK_CONTAINER(w), GTK_WIDGET(rgb));

  gtk_widget_show_all(w);

  gtk_timeout_add(1000, bar, rgb);

  gtk_main();

  return 0;
}
#endif

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

#include <glib.h>		/* for inline */

#include <gtkmmext/gtklevelhold.h>

static GtkRgbClass *parent_class;

static void
gtk_level_hold_size_allocate (GtkWidget     *widget,
				GtkAllocation *allocation)
{
  GtkLevelHold *lh;
  GtkRgb *rgb;

  g_return_if_fail (widget != NULL);
  g_return_if_fail (GTK_IS_LEVEL_HOLD (widget));
  g_return_if_fail (allocation != NULL);

  /* chain up first to update rgb->{h,w} */
  if (GTK_WIDGET_CLASS(parent_class)->size_allocate)
    GTK_WIDGET_CLASS(parent_class)->size_allocate(widget, allocation);

  lh = GTK_LEVEL_HOLD(widget);

  rgb = GTK_RGB(widget);

  g_free(lh->data);
  if (lh->options & GTK_LEVEL_HOLD_HORIZONTAL)
    lh->data = g_new0(int, rgb->w);
  else
    lh->data = g_new0(int, rgb->h);
}

static void
gtk_level_hold_destroy(GtkObject *object)
{
  GtkLevelHold *lh;

  g_return_if_fail(object != NULL);
  g_return_if_fail(GTK_IS_LEVEL_HOLD(object));

  lh = GTK_LEVEL_HOLD(object);

  g_free(lh->data);
  lh->data = NULL;

  /*fprintf(stderr, "destroying levelhold\n");*/
  if (GTK_OBJECT_CLASS(parent_class)->destroy)
    GTK_OBJECT_CLASS(parent_class)->destroy(object);
}

static void
gtk_level_hold_class_init(GtkLevelHoldClass *class)
{
  GtkObjectClass *object_class;
  GtkWidgetClass *widget_class;

  /* we'll need this stuff */
  gdk_rgb_init();

  parent_class = gtk_type_class(gtk_rgb_get_type());

  object_class = (GtkObjectClass*) class;
  widget_class = (GtkWidgetClass*) class;

  object_class->destroy = gtk_level_hold_destroy;
  widget_class->size_allocate = gtk_level_hold_size_allocate;
}

static void
gtk_level_hold_init(GtkLevelHold *lh)
{
}

guint
gtk_level_hold_get_type()
{
  static guint mytype = 0;

  if (!mytype)
    {
      GtkTypeInfo lh_info =
      {
	"GtkLevelHold",
	sizeof(GtkLevelHold),
	sizeof(GtkLevelHoldClass),
	(GtkClassInitFunc) gtk_level_hold_class_init,
	(GtkObjectInitFunc) gtk_level_hold_init,
	(GtkArgSetFunc) NULL,
	(GtkArgGetFunc) NULL
      };

      mytype = gtk_type_unique (gtk_rgb_get_type(), &lh_info);
    }

  return mytype;
}

GtkWidget*
gtk_level_hold_new(unsigned h, unsigned w, int options, unsigned steps, 
		   unsigned hold_pixels, float hold_factor)
{
  GtkLevelHold *lh = gtk_type_new (gtk_level_hold_get_type ());
  GtkRgb *rgb = GTK_RGB(lh);

  rgb->h = h;
  rgb->w = w;
  rgb->can_resizep = !(options & GTK_LEVEL_HOLD_FIXED);

  /* request a pleasant default size */
  GTK_WIDGET(lh)->requisition.width = rgb->w;
  GTK_WIDGET(lh)->requisition.height = rgb->h;

  lh->hold_factor = MAX(hold_factor, 1);
  lh->hold_pixels = MIN(hold_pixels, h);
  lh->options = options;
  lh->steps = steps;
  lh->data = NULL;
  
  return GTK_WIDGET (lh);
}

static inline void
inner_vloop(guchar *img, unsigned h, unsigned w, int i, 
	    guint r, guint g, guint b)
{
  guchar *p = img + ((h - 1) - i) * 3 * w;
  guchar *sent = p + 3 * w;
      
  while(p < sent)
    {
      *p++ = r;
      *p++ = g;
      *p++ = b;
    }
}

/* somewhat bummed */
static inline void
inner_vloop1(guchar *img, unsigned h, unsigned w, int i, 
	     guint r, guint g, guint b)
{
  guchar *p = img + ((h - 1) - i) * 3 * w;
  guchar *sent = p + 3 * w;
  union {guchar b[4]; guint32 i;}
  rgbr, gbrg, brgb;
  guint32 *p32 = (guint32 *) p;
  guint32 *sent32 = (guint32 *) sent;

  rgbr.b[0] = rgbr.b[3] = gbrg.b[2] = brgb.b[1] = r;
  rgbr.b[1] = gbrg.b[0] = gbrg.b[3] = brgb.b[2] = g;
  rgbr.b[2] = brgb.b[0] = gbrg.b[1] = brgb.b[3] = b;

  while(p32 <= sent32 - 3)
    {
      *p32++ = rgbr.i;
      *p32++ = gbrg.i;
      *p32++ = brgb.i;
    }
  
  p = (guchar *) p32;
  
  while (p < sent)
    {
      *p++ = r;
      *p++ = g;
      *p++ = b;
    }
}

static inline void
munge_data(int *data, unsigned n, unsigned steps, float hold_factor, 
	   unsigned hold_pixels, int lev)
{
  int i;
  /* flag to indicate if there's a non-expiring hold-bar with top
     greater than lev */
  int currentp = 0;

  for (i = n - 1; i >= 0; i--)
    {
      int di = data[i];
      
      if (i <= lev)
	{
	  if (!currentp && hold_pixels && i > lev - hold_pixels)
	    {
	      /* set to hold-bar max */
	      data[i] = rint(steps * hold_factor);
	    }
	  else if (currentp && di > steps)
	    /* decay hold-bar */
	    data[i]--;
	  else
	    /* set to normal max */
	    data[i] = steps - 1;
	}
      /* i > lev */
      else if (di == steps)
	/* ok..we're killing a hold bar */
	data[i] = 0;
      else if (di > steps)
	{
	  /* decay a hold bar */
	  data[i]--;
	  /* ..and note its presence */
	  currentp = 1;
	}
      else if (di > 0)
	/* normal fade */
	data[i]--;
    }
  
#if 0
  /* heavy debug! */
  for (i = 0; i < n; i++)
    fprintf(stderr, "%2.2d ", data[i]);
  fprintf(stderr, "\n");
#endif

}

/* R, G and B are zapped with the rgb values for point I (in [0, N)) on a
   green->red spectrum scaled to brightness SCALEN/SCALED */
/* LH is provided as an extra arg in case we decide to cache some of
   this info there */
static inline void
calc_hue(GtkLevelHold *lh, unsigned n, unsigned i, 
	 int scalen, int scaled,
	 int *r, int *g, int *b)
{
  /* calculate the basic hue */
  *r = ((i < (n/2)) ? ((i/(((float) (n/2)))) * 255) : (255));
  *g = ((i < (n/2)) 
	? (255) 
	: (((n - i)/((float) (n/2))) * 255));
  *b = 0;

  /* now scale it */
  *r = (*r * scalen) / scaled;
  *g = (*g * scalen) / scaled;
  *b = (*b * scalen) / scaled;
}

static inline void
inner_hloop(guchar *img, unsigned h, unsigned w, int i,
	    guint r, guint g, guint b)
{
  guchar *p = img + 3 * i;
  int j;

  for(j = 0; j < h; j++, p += 3 * w)
    {
      p[0] = r;
      p[1] = g;
      p[2] = b;
    }
}

void
gtk_level_hold_paint(GtkLevelHold *lh, unsigned n, float levf,
		     void (*inner_loop)(guchar *, unsigned, unsigned, int, 
					guint, guint, guint))
{
  int i;
  guchar *p, *sent;
  int lev;
  GtkRgb *rgb = GTK_RGB(lh);

  if (lh->data == NULL)
	  return;

  /* calc the max activated level in our internal units */
  lev = (int) floor(((n - 1) * CLAMP(levf, 0.0, 1.0)));

  munge_data(lh->data, n, lh->steps, 
	     lh->hold_factor, lh->hold_pixels, lev);

  for (i = 0; i < n; i++)
    {
      int r, g, b;
      /* clamp to normalize pseudo-values used in hold bars */
      int di = CLAMP(lh->data[i], 0, lh->steps - 1);
      int scalen = lh->steps - 1;

      /* adjust scalen (scale numerator..lh->steps is the denominator)
         to reflect data level */
      if (lh->options & GTK_LEVEL_HOLD_FADE)
	scalen = di;
      else
	/* anything less than max goes black if we're not fading */
	{
	  if (di != lh->steps - 1)
	    scalen = 0;
	}

      calc_hue(lh, n, i, scalen, lh->steps, &r, &g, &b);

      inner_loop(rgb->img, rgb->h, rgb->w, i, r, g, b);
    }
}

void
gtk_level_hold_erase(GtkLevelHold *lh, unsigned n, 
		     void (*inner_loop)(guchar *, unsigned, unsigned, int, 
					guint, guint, guint))
{
  int i;
  GtkRgb *rgb = GTK_RGB(lh);

  for (i = 0; i < n; i++)
    {
      inner_loop(rgb->img, rgb->h, rgb->w, i, 0, 0, 0);
    }
}

static void
gtk_level_hold_vertical_paint(GtkLevelHold *lh, float levf)
{
  gtk_level_hold_paint(lh, GTK_RGB(lh)->h, levf, inner_vloop);
}

static void
gtk_level_hold_horizontal_paint(GtkLevelHold *lh, float levf)
{
  gtk_level_hold_paint(lh, GTK_RGB(lh)->w, levf, inner_hloop);
}

void
gtk_level_hold_set(GtkLevelHold *lh, gfloat levf)
{
  if (GTK_WIDGET_DRAWABLE(GTK_WIDGET(lh)))
    {
      if (lh->options & GTK_LEVEL_HOLD_HORIZONTAL)
	gtk_level_hold_horizontal_paint(lh, levf);
      else
	gtk_level_hold_vertical_paint(lh, levf);

      gtk_widget_draw(GTK_WIDGET(lh), 0);
    }
}

void
gtk_level_hold_clear(GtkLevelHold *lh)
{
  if (lh->data == NULL)
	  return;

  if (GTK_WIDGET_DRAWABLE(GTK_WIDGET(lh)))
    {
        if (lh->options & GTK_LEVEL_HOLD_HORIZONTAL) 
	  {
	     memset (lh->data, 0, sizeof (int) * GTK_RGB(lh)->w);
	     gtk_level_hold_erase (lh, GTK_RGB(lh)->w, inner_hloop);
	  }
      else
          {
 	     memset (lh->data, 0, sizeof (int) * GTK_RGB(lh)->h);
	     gtk_level_hold_erase (lh, GTK_RGB(lh)->h, inner_vloop);
	  }
      gtk_widget_draw(GTK_WIDGET(lh), 0);
    }
}

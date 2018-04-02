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

    $Id: ellipsoid.cc,v 1.1 2007/07/04 07:26:39 seppo Exp $
*/

#include <cmath>
#include <cstdlib>

#include <pbd/ellipsoid.h>

const unsigned int Ellipsoid::narcs = 2;

void
Ellipsoid::set_start (int x, int y)

{
	start_x = x;
	start_y = y;
}

void
Ellipsoid::set_end (int x, int y)

{
	end_x = x;
	end_y = y;
}

void
Ellipsoid::compute ()

{
	int dx;
	int dy;
	int bc_y;
	int bc_x;
	int x1, x2, y1, y2;
	Arc *left;
	Arc *right;
	Arc *loose;
	Arc *tight;
	float droop_factor;

	x1 = start_x;
	x2 = end_x;
	y1 = start_y;
	y2 = end_y;

	dx = abs (x1 - x2);
	dy = abs (y1 - y2);

	left = &arc[0];
	right = &arc[1];

	if (y2 > y1) {
		if (x2 < x1) {
			 tight = left;
			 loose = right;
		} else {
			 tight = right;
			 loose = left;
		}
	} else {
		if (x2 < x1) {
			 tight = right;
			 loose = left;
		} else {
			 tight = left;
			 loose = right;
		}
	}

	/* For now, all widths and heights are actually 1/2 of
	   their true value. This saves on mults and divs, and
	   makes things a bit easier to understand (if you think of
	   the dimensions as those of a quadrant)
	*/

	/* Note that tight->rect_h really isn't connected with 
	   dy; instead, its an added vertical distance to ensure
	   we get cable droop.

	   20 and 8 are arbitrary constants. the first one
	   dictates the minimum cable drop from its origin in 
	   the tight_arc; the second increases the drop by
	   an amount that is proportional to the total distance
	   between the start and end point. 
	*/

	tight->rect_h = 20 + (int) floor (dx / 8.00);

	/* the width of the tight arc is arbitrarily chosen to be
	   between 1/3 and 1/2 of the total x-axis distance between
	   the two points.  We move to 1/2 as the y-axis distance
	   goes to zero. The actual range where dy affects things
	   is 0-100 (units of pixels).
	*/

	
	if (dy < 100) {
		 droop_factor = 0.5 - (0.1777 * dy/100);
	} else {
		droop_factor = 0.333;
	}

	tight->rect_w = (int) floor (dx * droop_factor);

	/* the loose arc gets values derived from the distances
	   between the points, modulated by whatever the tight
	   arc is doing. Note that the height is the sum of
	   the actual vertical distance plus the extra we created
	   for "cable droop".
	*/

	loose->rect_h  = dy + tight->rect_h;
	loose->rect_w = dx - tight->rect_w;

	/* (bc_x, bc_y) is the coordinate pair for the bottom center
	   vertex of the bottom halves of the rectangles . This is a
	   drawing of just 2 quadrants, in which the left side is
	   assumed to hold the loose arc, and right side, the tight
	   one.

	     +----------------+	    
	     |	       	      |         
	     |	   LLL 	      |	   LLL - left, lower left quadrant
	     |		      |    RLR - right, lower right quadrant
	     | 	    	      |	    
	     |	              +-------+
	     |	              |       |
	     |	              |  RLR  |
	     |	              |       |
	     |	              |       |
             +----------------X-------+
	     
           We're interested in computing the point X, since its
	   the common intersection of the two arcs. From this, we
	   then figure the upper left corners of the arcs' enclosing
	   rectangles.  

	   What we're fundamentally trying to do is to ensure that
	   the origin of the curve remains fixed in place as we move
	   the other end around.
	*/

	if (x2 < x1) {

		/* origin is the end of the right hand arc; quadrant
		   vertex is left (-) and down (+)
		 */

		bc_x = x1 - right->rect_w;
		bc_y = y1 + right->rect_h;

	} else {

		/* origin is the start of the left hand arc; quadrant
		   vertex is right (+) and down (+)
		 */

		bc_x = x1 + left->rect_w;
		bc_y = y1 + left->rect_h;

	}
	
	/* Now pick upper-left hand corners so that the center of
	   the bottom edge of each elli pse is in the same place.
	   Double up the width and height since we don't need the
	   quadrant dimensions anymore.
	*/

	right->rect_x = bc_x - right->rect_w;
	right->rect_w *= 2;
	right->rect_h *= 2;
	right->rect_y = bc_y - right->rect_h;
	right->counter_clockwise = true;

	left->rect_x = bc_x - left->rect_w;
	left->rect_w *= 2;
	left->rect_h *= 2;
	left->rect_y = bc_y - left->rect_h;
	left->counter_clockwise = false;
}

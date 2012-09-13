/***********************************************************************
 *  J a v a G P S - GPS access library and Java API                    *
 *  Copyright (C) 2001 Ulrich Walther                                  *
 *                                                                     *
 *  This program is free software; you can redistribute it and/or      *
 *  modify it under the terms of the GNU General Public License as     *
 *  published by the Free Software Foundation; either version 2 of     *
 *  the License, or (at your option) any later version.                *
 *                                                                     *
 *  This program is distributed in the hope that it will be useful,    *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of     *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU   *
 *  General Public License for more details.                           *
 *                                                                     *
 *  You should have received a copy of the GNU General Public          *
 *  License along with this program; if not, write to the Free         *
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,     *
 *  MA 02111-1307 USA                                                  *
 ***********************************************************************/

package com.marjaniemi.nmeac;


/**
 *  Takes a NMEA-0183 message to tokenize.
 *
 *@author    walther
 */
class NMEATokenizer
{
    private String msg_;


    NMEATokenizer(String msg)
    {
	msg_ = msg;
    }


    boolean hasMoreTokens()
    {
	return getPos() >= 0;
    }


    // return next token (filters delimiter)
    String nextToken()
    {
	String t = msg_.substring(0, getPos());
	msg_ = msg_.substring(getPos() + 1);
	return t;
    }


    // return position of next delimiter
    // delimeters are , and *
    // -1 means no more delimiters available
    private int getPos()
    {
	int p1 = msg_.indexOf(",");
	int p2 = msg_.indexOf("*");

	if (p1 == -1 && p2 == -1)
	{
	    return -1;
	}

	if (p1 == -1)
	{
	    p1 = msg_.length() - 1;
	}
	if (p2 == -1)
	{
	    p2 = msg_.length() - 1;
	}

	return Math.min(p1, p2);
    }
}

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

import java.lang.reflect.*;
import java.util.logging.Logger;


/**
 *  NMEA0183 message parser. Uses reflection to be able to easily specify new
 *  NMEA messages (see below: SENTENCE_DEFINITIONS).
 */
public class NMEAParser
{
    private static Logger logger_ = Logger.getLogger("com.marjaniemi.nmeac");


    // Here are the definitions of GPS sentences we can handle
    // Note: the argument names correspond to members of class GPSInfo and
    //       are automatically converted to the type of the members.

    /**
     *  Definition of understood NMEA0183 sentences. The values given here as
     *  symbolic names are taken as field names of the info object given to the
     *  parse method; values from the sentence are automatically converted to the
     *  field's type in the info object. E.g. the definition of<br>
     *  <code>$GPTST,lat,long</code><br>
     *  specifies a new message that has the header <code>$GPTST</code>, and two
     *  parameters lat and long. Those two values are automatically parsed and
     *  stored inside the GPSInfo class, in the fields with names lat and long. The
     *  type of the value is automatically converted to fit the type of the fields.
     */
    public final static String[] SENTENCE_DEFINITIONS = new String[]{
	"$GPGGA,utc,latitude,northHemi,longitude,eastHemi,quality," +
	"numberOfSatellites,horDilution,height,,geoidalHeight,,diffCorrection,diffStationId,",
	"$GPGSA,mode,fixtype,s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,s12," +
	"posDilution,horDilution,verDilution,",
	"$GPGSV,gsvnum,gsvcur,satInView",
	"$GPRMC,utc,status,latitude,northHemi,longitude,eastHemi," +
	"speedOverGroundKnots,courseOverGround,utcdate,magnVariation,magnVarDirection,",
	"$GPVTG,courseOverGround,,magnCourse,,speedOverGroundKnots,,speedOverGround,",
	"$LCGLL,latitude,northHemi,longitude,eastHemi,utc,",
	"$LCVTG,courseOverGround,,magnCourse,,speedOverGroundKnots,,speedOverGround,",
	"$PGRME,HPE,,VPE,,EPE,",
	"$PGRMF,GPSWeek,GPSSeconds,utcdate,utc,GPSLeapSecondCount," +
	"latitude,northHemi,longitude,eastHemi,mode,fixType,speedOverGround,courseOverGround,",
	"$PGRMT,GPSModel,romChecksum,recvFailure,storedDataLost,timeLost," +
	"oscillatorDrift,dataCollection,boardTemperature,boardConfig",
	"$PGRMV,eastVelocity,northVelocity,upVelocity,",
    };


    /**
     * Parse given NMEA0183 message (usually from GPS device) and
     * convert the parsed values to fit into the GPSInfo structure.
     *
     * @param  sentence  NMEA sentence to parse
     * @param  info      Object to fill in with values from the sentence
     * @return           <code>true</code> if successful, <code>false</code> otherwise
     */
    public static boolean parse(String sentence, NMEAInfo info)
    {
	try
	{
	    String type = sentence.substring(0, 7);
	    for (int i = 0; i < SENTENCE_DEFINITIONS.length; i++)
	    {
		if (SENTENCE_DEFINITIONS[i].startsWith(type))
		{
		    parse(sentence.substring(7), SENTENCE_DEFINITIONS[i].substring(7), info);
		    return true;
		}
	    }

	    //	    logger_.warning("NMEA sentence " + type + " is not supported.");
	}
	catch (Exception e)
	{
	    logger_.warning("Cannot parse NMEA sentence: " + sentence);
	}

	return false;
    }


    private static void parse(String sentence, String syntax, NMEAInfo info)
    {
	NMEATokenizer t = new NMEATokenizer( sentence );
	NMEATokenizer t2 = new NMEATokenizer( syntax );
	String s;
	String s2;
	Field f;
	Field fage;
	Class c = info.getClass();

	while ( t.hasMoreTokens() && t2.hasMoreTokens() )
	{
	    s = t.nextToken();
	    s2 = t2.nextToken();
	    if ( s.length() == 0 || s2.length() == 0 )
	    {
		continue;
	    }

	    logger_.finest(s2 + " = " + s);

	    try
	    {
		f = c.getField( s2 );
	    }
	    catch ( NoSuchFieldException e )
	    {
		//		logger_.warning("NoSuchFieldException while parsing a sentence: " + e);
		continue;
	    }

	    boolean gotValue = true;

	    try
	    {
		String type = f.getType().getName();

		if (type.equals("char"))
		{
		    f.setChar(info, new String(s).charAt(0));
		}
		else if (type.equals("int"))
		{
		    f.setInt(info, new Integer(s).intValue());
		}
		else if (type.equals("double"))
		{
		    f.setDouble(info, new Double(s).doubleValue());
		}
		else if (type.equals("java.lang.String"))
		{
		    f.set(info, s);
		}
		else
		{
		    logger_.warning("Argument " + s2 + " cannot be converted to " + type);
		    gotValue = false;
		}

	    }
	    catch (Exception e)
	    {
		logger_.warning("Exception while parsing a sentence: " + e);
		continue;
	    }
	}
    }


    /**
     * Perform NMEA checksum calculation.
     */
    static String checkSum(String msg)
    {
	int chk = 0;
	for ( int i = 1; i < msg.length(); i++ )
	{
	    chk ^= msg.charAt( i );
	}

	String chk_s = Integer.toHexString( chk ).toUpperCase();

	// checksum must be 2 characters!
	while ( chk_s.length() < 2 )
	{
	    chk_s = "0" + chk_s;
	}

	return chk_s;
    }


    /**
     * Calculate checksum of NMEA message and compare.
     */
    static boolean check( String msg )
    {
	int msglen = msg.length();

	if ( msglen > 4 )
	{
	    if ( msg.charAt( msglen - 3 ) == '*' )
	    {
		// perform NMEA checksum calculation

		String chk_s = checkSum( msg.substring( 0, msglen - 3 ) );

		// compare checksum to encoded checksum in msg
		return ( msg.substring( msglen - 2, msglen ).equals( chk_s ) );
	    }
	    else
	    {
		// message doesn't have a checksum: accept it
		return true;
	    }
	}

	// don't accept messages without checksum
	return false;
    }


    static String addCheckSum(String msg)
    {
	return msg + "," + checkSum(msg + ",") + "*" + ((char)13) + ((char)10);
    }
}

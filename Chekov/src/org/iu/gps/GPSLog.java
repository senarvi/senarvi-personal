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

package org.iu.gps;

import java.io.*;
import java.util.*;

/**
 *  This class represents a device-independent GPS log. Further, it is capable
 *  of converting raw GPS log files from GPS devices into our internal
 *  representation on the fly.
 *
 *@author    walther
 */
public class GPSLog implements Serializable {
	/**
	 *  Specifies the start time of the log.
	 */
	protected Date start;

	/**
	 *  Specifies the duration of the log in seconds.
	 */
	protected int duration;

	/**
	 *  Specifies the GPSInfo elements of the log, one for each second of logged
	 *  data.
	 */
	protected Vector gpslog;


	/**
	 *  Creates an empty GPSLog.
	 */
	public GPSLog()
	{
		duration = 0;
		gpslog = new Vector();
	}


	/**
	 *  Creates an empty GPSLog with given time/date.
	 *
	 *@param  _start  Creation date of the log.
	 */
	public GPSLog( Date _start )
	{
		this();
		start = _start;
	}


	/**
	 *  Loads GPSLog from a raw GPS driver log file.
	 *
	 *@param  is            Input stream.
	 *@return               Returns a new GPSLog object representing the given
	 *      stream's log data.
	 *@throws  IOException  In case of I/O Error.
	 */
	public static GPSLog loadRaw( InputStream is ) throws IOException
	{
		BufferedReader in = new BufferedReader( new InputStreamReader(
				is ) );

		//System.out.println("Converting GPS RAW logfile...");

		GPSInfo gpsInfo = new GPSInfo();
		NMEA.infiniteAge( gpsInfo );

		long t0;

		long t1;

		long t2;
		String msg = "";
		boolean replayLog = false;
		long replayBase = -1;
		// seconds counter for GPSLog creation
		int t = 0;

		// (try to) read the first line
		try
		{
			msg = filter( in.readLine() );
		}
		catch ( Exception e )
		{
			// cannot read, return null log
			return null;
		}

		//System.out.println( msg );

		// check that we are reading previously logged messages
		if ( msg.charAt( 0 ) != '$' && msg.indexOf( ":" ) > 0
				 && Character.isDigit( msg.charAt( 0 ) ) )
		{
			replayBase = Long.parseLong( msg.substring( 0, msg.indexOf( ':' ) ) );
		}
		else
		// check for raw log without timestamp
				if ( msg.charAt( 0 ) == '$' )
		{
			replayBase = 0;

			while ( ( msg = filter( in.readLine() ) ) != null )
			{
				// parse NMEA message until we find the first UTC time
				if ( msg != null && NMEA.check( msg ) )
				{
					//System.out.println("DEBUG "+msg);
					// parse known messages
					synchronized ( gpsInfo )
					{
						if ( NMEA.parse( msg, gpsInfo ) )
						{
							;
						}
					}
				}

				if ( gpsInfo.utc != null )
				{
					replayBase = UTCtoMS( gpsInfo.utc );
					break;
				}
			}

			// could not find time base in NMEA messages
			if ( replayBase == 0 )
			{
				return null;
			}
		}
		else
		{
			return null;
		}

		t0 = t1 = replayBase;
		//System.currentTimeMillis();

		// create a new GPS log with start time/date
		GPSLog gl = new GPSLog( new Date( t0 ) );

		// read over the whole log file
		while ( ( msg = filter( in.readLine() ) ) != null )
		{
			try
			{
				// read the time when the msg was logged, and
				// skip time in msg

				if ( msg.indexOf( ':' ) > 0 )
				{
					// parse line with timestamp
					long twait = Long.parseLong( msg.substring( 0, msg.indexOf( ':' ) ) ) - replayBase;
					msg = msg.substring( msg.indexOf( ':' ) + 1 );
					//System.out.println("Replay "+msg);
					long ct = System.currentTimeMillis();
					t2 = twait + replayBase;
					// wait to replay correct timely behaviour
					if ( ct - t0 < twait )
					{
						//System.out.println(" sleeping "+ (twait - (ct-t0)) );
						//Thread.currentThread().sleep( twait - (ct-t0) );
					}
				}
				else
				{
					// parse line without timestamp
					// these are raw logs from the GPS
					t2 = UTCtoMS( gpsInfo.utc );
//				System.err.println(t2);
				}

				if ( msg != null && NMEA.check( msg ) )
				{
					//System.out.println("DEBUG "+msg);
					// parse known messages
					synchronized ( gpsInfo )
					{
						if ( NMEA.parse( msg, gpsInfo ) )
						{
							;
						}
					}
				}

				// check if GPSInfo has to be aged...
				while ( t2 - t1 >= 1000 )
				{
					GPSInfo gc = ( GPSInfo ) gpsInfo.clone();
					GPSDriver.normGPSInfo( gc );
					gl.setInfoAt( t, gc );
					t++;
					//System.out.println("t="+t1+" x="+gc.X+" y="+gc.Y);

					NMEA.increaseAge( gpsInfo );
					t1 += 1000;

					//System.out.println("\n\n*** GPSInfo ***");
					//NMEA.printAll( gpsInfo );
				}
			}
			catch ( Exception e )
			{
				e.printStackTrace();
			}
		}
		return gl;
	}


	/**
	 *  Loads a GPSLog from a given stream, uses device-independent file format.
	 *  Falls back and tries to load raw GPS log if the given input stream is not
	 *  in the device-independent format.
	 *
	 *@param  _is  Input stream.
	 *@return      Returns new GPSLog object representing the log from the given
	 *      stream.
	 */
	public static GPSLog load( InputStream _is )
	{
		BufferedInputStream is = new BufferedInputStream( _is );
		is.mark( 0 );
		ObjectInputStream ois = null;
		try
		{
			ois = new ObjectInputStream( is );
			GPSLog gl = ( GPSLog ) ois.readObject();
			if ( gl != null )
			{
				return gl;
			}
		}
		catch ( Exception e )
		{
			//e.printStackTrace();
		}

		try
		{
			if ( ois != null )
			{
				ois.close();
			}
			// reset input stream
			is.reset();

			//System.out.println("Fallback to raw log...");

			// try to load stream as a raw log
			return loadRaw( is );
		}
		catch ( Exception e2 )
		{
			//e2.printStackTrace();
		}
		return null;
	}


	/**
	 *  Test method.
	 *  Converts old log file into new log file format.
	 *
	 *@param  arg
	 *@throws  Exception
	 */
	public static void main( String[] arg ) throws Exception
	{

		if ( arg.length != 2 )
		{
			System.err.println( "Syntax: oldlog_name converted_log_name" );
			System.exit( -1 );
		}
		GPSLog gl = GPSLog.loadRaw( new FileInputStream( arg[0] ) );
		gl.save( new java.util.zip.GZIPOutputStream( new FileOutputStream( arg[1] ) ) );
	}


	private static long UTCtoMS( String utc )
	{
		int h = Integer.parseInt( utc.substring( 0, 2 ) );
		int m = Integer.parseInt( utc.substring( 2, 4 ) );
		int s = Integer.parseInt( utc.substring( 4, 6 ) );

		return 1000l * ( h * 3600 + m * 60 + s );
	}


	private static String filter( String s )
	{
		if ( s == null )
		{
			return s;
		}

		int idx = s.indexOf( ':' );
		if ( idx >= 0 )
		{
			return s.substring( idx + 1 );
		}
		else
		{
			return s;
		}
	}


	/**
	 *  Sets GPSInfo in the log for a specific time.
	 *
	 *@param  t   time in seconds.
	 *@param  gi  GPSInfo to set.
	 */
	public void setInfoAt( int t, GPSInfo gi )
	{
		gpslog.add( t, gi );
		if ( t > duration )
		{
			duration = t;
		}
	}


	/**
	 *  Returns the duration of the log in seconds.
	 *
	 *@return    Duration in seconds.
	 */
	public int getDuration()
	{
		return duration;
	}


	/**
	 *  Gets the GPSInfo from the log at a certain point of time, where t>=0 and t
	 *  smaller than duration.
	 *
	 *@param  t  time in seconds (>=0 and smaller duration)
	 *@return    GPSInfo at specified time, or null if not available.
	 */
	public GPSInfo getInfoAt( int t )
	{
		return ( GPSInfo ) gpslog.elementAt( t );
	}


	/**
	 *  Saves the log in the given output stream.
	 *
	 *@param  os            Output stream to use for saving.
	 *@throws  IOException  In case of I/O error.
	 */
	public void save( OutputStream os ) throws IOException
	{
		ObjectOutputStream oos = new ObjectOutputStream( os );
		oos.writeObject( this );
		oos.close();
		os.flush();
	}
}

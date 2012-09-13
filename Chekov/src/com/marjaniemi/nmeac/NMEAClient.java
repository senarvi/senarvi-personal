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

import java.io.*;
import java.net.*;
import java.util.*;
import java.util.logging.*;


/**
 * This class realizes access to GPS devices over a network socket or any
 * other stream NMEA0183 message format.
 */
public class NMEAClient
{
    private static Logger logger_ = Logger.getLogger("com.marjaniemi.nmeac");


    InputStream inputStream_ = null;
    OutputStream outputStream_ = null;
    OutputStream logStream_ = null;

    Thread gpsThread_ = null;

    NMEAInfo nmeaInfo_ = new NMEAInfo();

    boolean inputLoggingEnabled_ = false;
    boolean simulate_;

    // GPS listeners
    Vector listeners_ = new Vector();


    /**
     * @param hostname Hostname of the NMEA server.
     * @param port     Port number of the NMEA server.
     */
    public NMEAClient(String hostname, int port) throws ConnectException, IOException
    {
	logger_.config("NMEA server: " + hostname + ":" + port);

	Socket socket = new Socket(hostname, port);
	inputStream_ = socket.getInputStream();

	logStream_ = new FileOutputStream("nmeainput.log");

	simulate_ = false;
	inputLoggingEnabled_ = true;

	
/*
	// configure GPS
	try
	{
	    // GARMIN GPS25: enable all output messages
	    //outputStream.write( new String("$PGRMO,,3"+((char)13)+((char)10)).getBytes() );
	    outputStream.write( NMEA.addCheckSum( "$PGRMO,,3" ).getBytes() );

	    // trigger GPS to send current configuration
	    //outputStream.write( new String("$PGRMCE,"+((char)13)+((char)10)).getBytes() );
	    outputStream.write( NMEA.addCheckSum( "$PGRMCE," ).getBytes() );
	    // default configuration string is:
	    // $PGRMC,A,218.8,100,6378137.000,298.257223563,0.0,0.0,0.0,A,3,1,1,4,30

	    // GARMIN GPS25: set to German earth datum (parameter 3=27)
	    outputStream.write( new String( "$PGRMC,A,,27,,,,,,A,3,1,1,4,30" + ( ( char ) 13 ) + ( ( char ) 10 ) ).getBytes() );
	    //outputStream.write( NMEA.addCheckSum("$PGRMC,A,,100,,,,,,A,3,1,1,4,30").getBytes() );
	}
	catch ( IOException e )
	{
	    logger_.warning("Cannot configure GPS.");
	}
*/

	startThread();
    }


    /**
     * Open an NMEA input stream to replay logged GPS information.
     *
     * @param  logStream   Input stream with logged GPS information.
     * @throws  Exception  If the stream is in error.
     */
    public NMEAClient(InputStream logStream) throws Exception
    {
	inputStream_ = logStream;
	simulate_ = true;
	inputLoggingEnabled_ = false;
	startThread();
    }


    /**
     *  Normalize GPS Information: convert from degrees/minutes to degrees only
     *  representation, convert from the GPS ellipsoid, and fill in the
     *  GaussKrueger coordinates
     *
     *@param  gi  NMEAInfo to normalize.
     */
    public static NMEAInfo normalizedNMEAInfo(NMEAInfo i)
    {
	NMEAInfo result = (NMEAInfo)i.clone();
	try
	{
	    result.latitude = normLatLong(i.latitude);
	    result.longitude = normLatLong(i.longitude);
	}
	catch (Exception e)
	{
	}
	return result;
    }


    /**
     * convert from deg/min to degrees
     *
     *@param  c  Parameter
     *@return    Returned Value
     */
    public static double normLatLong( double c )
    {
	double deg = Math.floor( c * 1e-2 );
	double min = c - deg * 1e2;
	logger_.finest("deg=" + deg + " min=" + min + " c="+c);
	return deg + min / 60.0;
    }


    // Expects 6-letter UTC and returns # of seconds
    static long utcToSeconds( String utc )
    {
	return Integer.parseInt( utc.substring( 0, 2 ) ) * 3600 +
	    Integer.parseInt( utc.substring( 2, 4 ) ) * 60 +
	    Integer.parseInt( utc.substring( 4, 6 ) );
    }


    /**
     *  Close the GPS device.
     */
    public void close()
    {
	boolean interrupted = false;

	try
	{
	    stopThread();
	}
	catch (InterruptedException ie)
	{
	    logger_.warning("A thread was interrupted while it waited for the NMEA client thread to die.");
	}


	if (inputStream_ != null)
	{
	    try { inputStream_.close(); }
	    catch (IOException e)
	    {
		logger_.warning("I/O exception while trying to close input stream:\n"
				+ e.getMessage());
	    }
	    inputStream_ = null;
	}

	if (outputStream_ != null)
	{
	    try { outputStream_.close(); }
	    catch (IOException e)
	    {
		logger_.warning("I/O exception while trying to close output stream:\n"
				+ e.getMessage());
	    }
	    outputStream_ = null;
	}
    }


    /**
     * Read characters from the <code>BufferedReader</code> until the input line is
     * terminated or no more characters are available. A line is considered to be
     * terminated by any one of a line feed ('\n'), a carriage return ('\r'), or a
     * carriage return followed immediately by a linefeed.
     *
     * @param      result A <code>StringBuffer</code> where the text will be appended.
     *
     * @return     <code>true</code>, if the line was finished, <code>false</code>,
     *             if not enough characters were available.
     * 
     * @see        java.io.LineNumberReader#readLine()
     *
     * @exception  IOException  If an I/O error occurs
     */
    boolean readAndAppend(StringBuffer result, BufferedReader in) throws IOException
    {
	boolean emptyLine = true;
	char[] cb = new char[1];

	while (in.ready())
	{
	    int readResult = in.read(cb, 0, 1);
	    if (readResult != 1)
	    {
		throw new IOException("Failed to read from the input stream.");
	    }

	    if ((cb[0] == '\n') || (cb[0] == '\r'))
	    {
		// End of line.
		if (emptyLine) return false;
		return true;
	    }
	    result.append(cb);
	    emptyLine = false;
	}
	return false;
    }


    /**
     * Read a line from the input stream. If the thread is interrupted
     * during the read operation, throw InterruptedException.
     */
    private String readLine(BufferedReader in)
	throws InterruptedException, IOException
    {
	StringBuffer sb = new StringBuffer();
	while (readAndAppend(sb, in) == false)
	{
	    if (Thread.interrupted())
	    {
		throw new InterruptedException();
	    }
            Thread.sleep(50);
	}
	return sb.toString();
    }


    /**
     *  Main thread.
     */
    public void listen()
    {
	try
	{
	    if (inputStream_ == null)
	    {
		logger_.severe("Input stream is null in NMEA listening thread.");
		return;
	    }
	    
	    BufferedReader in = new BufferedReader( new InputStreamReader(
							inputStream_ ) );


	    String msg = "";

	    // The first message may be missing characters from the beginning.
	    try
	    {
		msg = readLine(in);
	    }
	    catch (InterruptedException ie) { return; }
	    catch (IOException ioe)
	    {
		logger_.severe("I/O exception while reading NMEA stream.");
		return;
	    }

	    while (!Thread.interrupted())
	    {
		try
		{
		    msg = readLine(in);
		}
		catch (InterruptedException ie) { return; }
		catch (IOException ioe)
		{
		    logger_.severe("I/O exception while reading NMEA stream.");
		    return;
		}


		///////////////////*******====??????????????????????????????????
		if (msg == null)
		{
		    try
		    {
			Thread.sleep(50);
		    }
		    catch (InterruptedException ie) { return; }
		}


		if (inputLoggingEnabled_)
		{
		    try
		    {
			logStream_.write(new String("" + System.currentTimeMillis()
						    + ":").getBytes());
			logStream_.write((msg + "\n").getBytes());
		    }
		    catch (Exception e)
		    {
			logger_.warning("Exception while trying to log input.");
			e.printStackTrace();
			logger_.warning("Input logging will be disabled.");
			inputLoggingEnabled_ = false;
		    }
		}


		if (!NMEAParser.check(msg))
		{
		    logger_.warning("Message failed cheksum check.");
		}
		else
		{
		    if (msg != null)
		    {
			// parse known messages
			synchronized (nmeaInfo_)
			{
			    NMEAParser.parse(msg, nmeaInfo_);
			    signalListeners(normalizedNMEAInfo(nmeaInfo_));
			}
		    }
		}
	    }

	    logger_.info("NMEA listener thread died normally.");
	}
	catch (Exception e)
	{
	    logger_.severe("Exception in NMEA listener thread.");
	    e.printStackTrace();
	}
    }


    private void startThread()
    {
	gpsThread_ = new Thread(new Runnable() {
	    public void run() { listen(); }
	});
	gpsThread_.start();
    }


    private void stopThread() throws InterruptedException
    {
	if (gpsThread_ != null)
	{
	    gpsThread_.interrupt();
	    gpsThread_.join(1000);
	    if (gpsThread_.isAlive())
	    {
		logger_.warning("NMEA client thread refused to die.");
	    }
	}
    }


    /**
     * Register a new NMEAInfoListener unless the listener has already
     * been registered.
     *
     * @param listener The <code>NMEAInfoListener</code> to add.
     */
    public void addListener(NMEAInfoListener listener)
    {
	synchronized (listeners_)
	{
	    if (!listeners_.contains(listener))
	    {
		listeners_.add(listener);
	    }
	}
    }


    /**
     * Remove an NMEAInfoListener.
     *
     * @param listener The <code>NMEAInfoListener</code> to remove.
     */
    public void removeListener(NMEAInfoListener listener)
    {
	synchronized (listeners_)
	{
	    listeners_.remove(listener);
	}
    }


    /**
     * Send updated NMEAInfo to all registered listeners.
     */
    void signalListeners(NMEAInfo info)
    {
	synchronized (listeners_)
	{
	    for (int i = 0; i < listeners_.size(); ++i)
	    {
		NMEAInfoListener l = (NMEAInfoListener)listeners_.get(i);
		l.nmeaInfoEvent(info);
	    }
	}
    }
}

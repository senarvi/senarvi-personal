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
import javax.comm.*;
import java.util.logging.*;


// Read NMEA-0183 messages, perform checksum calculation + parse GPGGA (GPS position) msg.
// 10.11.1999 UW [Uli Walther]: created
// 16.11.1999 UW: cleanup, added NMEATokenizer and NMEA classes
// 05.2000 & 06.2000: parse all GPS messages + proprietary GARMIN messages
// 07.2000: add age of values in GPS info

/**
 *  This class realizes access to GPS devices over a serial line (NMEA0183
 *  message format). GPSDriver has the following features: - auto-detection of
 *  the GPS device's serial line - logging of GPS information into a file -
 *  playback of previously logged GPS information (application transparent;
 *  application thinks data comes from GPS)
 *
 *@author    walther
 */
public class GPSDriver implements Runnable, Serializable
{
    private static Logger logger_ = Logger.getLogger("org.iu.gps");


    final static int[] baudRates = {2400, 4800, 9600, 19200};
    String port;
    int baudRate;

    String buffer;

    InputStream inputStream = null;
    OutputStream outputStream = null;
    OutputStream logStream = null;
    OutputStream logRawStream = null;
    SerialPort serialPort = null;
    Thread gpsThread = null;
    Thread replayThread = null;
    Thread logThread = null;

    boolean logInput;
    boolean logRawInput;
    boolean replayMode;
    boolean gpsMode;

    GPSLog gpsLog;
    GPSLog gpsReplayLog;
    GPSInfo gpsInfo;
    GPSInfo tmpInfo;
//    boolean gpsInfoUpdated;
    int gpsParsedMessages;
    boolean simulate;

    // GPS listeners
    Vector listeners = new Vector();


    public static class AlreadyOpenException extends Exception
    {
	AlreadyOpenException()
	{
	    super("already open");
	}
    }


    /**
     *  Creates a new GPSDriver instance, open and listening on the given serial
     *  port with the specified baudRate.
     *
     *@param  port           Symbolic name of serial port to listen on (/dev/ttySX
     *      on UN*X, or COMx: on Win32)
     *@param  baudRate       Baudrate to use in communication with the GPS device
     *@exception  Exception  Exception
     *@throws  Exception     Throws various exceptions if comm. cannot be
     *      established, or if the port does not exist.
     */
    public GPSDriver( String port, int baudRate )
	throws AlreadyOpenException, NoSuchPortException, PortInUseException, IOException
    {
	logger_.config("Using port " + port + " to receive GPS messages.");

	setupGPS(port, baudRate);
    }


    /**
     *  Creates a close GPSDriver instance.
     */
    public GPSDriver() {}


    /**
     *  Returns the list of all known serial ports.
     *
     *@return    List of ports.
     */
    public static String[] getPortList()
    {
	Vector v = new Vector();
	Enumeration portList = CommPortIdentifier.getPortIdentifiers();

	while ( portList.hasMoreElements() )
	{
	    CommPortIdentifier portId = ( CommPortIdentifier ) portList.nextElement();
	    //logger_.config(portId.getName());
	    if ( portId.getPortType() == CommPortIdentifier.PORT_SERIAL )
	    {
		v.add( portId.getName() );
	    }
	}

	return ( String[] ) v.toArray( new String[v.size()] );
    }


    /**
     *  Returns the list of all known baud rates.
     *
     *@return    List of Baud rates.
     */
    public static int[] getBaudrateList()
    {
	return baudRates;
    }


    /**
     *  The main program for the GPSDriver class
     *
     *@param  arg            The command line arguments
     *@exception  Exception  Exception
     */
    public static void main( String[] arg ) throws Exception
    {
        logger_.finer("The following serial ports are available:");
        String[] ports = getPortList();
        for (int i=0; i<ports.length; i++)
	    logger_.finer(ports[i]);

        logger_.finer("The following communication speeds are available:");
        int[] bauds = getBaudrateList();
        for (int i=0; i<bauds.length; i++)
	    logger_.finer(bauds[i] + " bps");

	logger_.info("Starting autodetection of GPS device.");

	new GPSDriver().open( GPSDriver.detect() );
    }


    /**
     *  Tries to find the serial line + baudRate to which a GPS is connected to.
     *
     *@return    GPS configuration, or null if no GPS was found.
     */
    public static Configuration detect()
    {
	String[] ports = getPortList();

	GPSDriver gps = new GPSDriver();

	// loop through all possible ports & baudrates
	for ( int i = 0; i < ports.length; i++ )
	{
	    for ( int k = 0; k < baudRates.length; k++ )
	    {
		try
		{
		    gps.setupGPS( ports[i], baudRates[k] );
		}
		catch ( Exception e )
		{
		    continue;
		}

		long t1 = System.currentTimeMillis();

		while ( System.currentTimeMillis() - t1 < 5000 )
		{
		    if ( gps.isAlive() )
		    {
			// found GPS
			Configuration cfg = new Configuration();
			cfg.port = ports[i];
			cfg.baudRate = baudRates[k];

			gps.close();
			return cfg;
		    }

		    try
		    {
			Thread.currentThread().sleep( 500 );
		    }
		    catch (Exception e)
		    {
		    }
		}

		gps.close();
	    }
	}

	// unfortunately we couldn't detect any GPS...
	return null;
    }


    /**
     *  Normalize GPS Information: convert from degrees/minutes to degrees only
     *  representation, convert from the GPS ellipsoid, and fill in the
     *  GaussKrueger coordinates
     *
     *@param  gi  GPSInfo to normalize.
     */
    public static void normGPSInfo( GPSInfo gi )
    {
	try
	{
	    // convert from deg/min to degrees
	    gi.latitude = normLatLong( gi.latitude );
	    gi.longitude = normLatLong( gi.longitude );

	    // convert from ellipsoid GPS uses
//			XY c = new COORD(100).translate(false,gi.latitude,gi.longitude,28);

//			XY c = COORD.translate( 28, 100, gi.latitude, gi.longitude );
//			gi.latitude = c.x;
//			gi.longitude = c.y;

	    // convert coordinates to German Gauss-Krueger system
//	    XY gk = COORD.convertToGK( gi.latitude, gi.longitude );
//	    gi.X = gk.x;
//	    gi.Y = gk.y;
//	    gi.XAge = gi.latitudeAge;
//	    gi.YAge = gi.longitudeAge;
	}
	catch ( Exception e )
	{
	}
    }


    /**
     *  Method
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
     *  Simulate a GPS device that returns the given GPSInfo. Note: Disables
     *  hardware and replay modes.
     *
     *@param  gi  GPSInfo to use.
     */

    public void setSimulationInfo( GPSInfo gi )
    {
	disableHardware();
	disableReplay();

	gpsInfo = gi;
	processListeners();
    }


    /**
     *  Sets the current GPSInfo to the given position. The rest of the GPSInfo
     *  fields are set to default values. Note: This is a convenience method for
     *  setSimulationInfo(...)
     *
     *@param  lat  Latitude to use for simulation.
     *@param  lon  Longitude to use for simulation.
     */
    public void setSimulationPosition( double lat, double lon )
    {
	GPSInfo gi = new GPSInfo();
    }


    /**
     *  Setup the GPS hardware.
     *
     *@param  port        Serial port.
     *@param  baudRate    Baud rate of GPS device.
     */
    public void setupGPS( String port, int baudRate )
	throws AlreadyOpenException, NoSuchPortException, PortInUseException, IOException
    {
	if (isOpen()) throw new AlreadyOpenException();//close();


	//            serialPort = (SerialPort) portId.open("GPSDaemon", 2000);
	serialPort = ( SerialPort ) CommPortIdentifier.getPortIdentifier( port ).open( "GPSDriver", 60 );

	try
	{
	    inputStream = serialPort.getInputStream();
	    //			inputStream = new FileInputStream( "_log.txt" );
	    outputStream = serialPort.getOutputStream();
	}
	catch ( IOException e )
	{
	    close();
	    throw e;
	}



	/*
	  try
	  {
	  serialPort.addEventListener(this);
	  }
	  catch (TooManyListenersException e)
	  {
	  logger_.severe("Unable to listen on serial line.");
	  //System.exit(-1);
	  }
	  serialPort.notifyOnDataAvailable(true);
	*/
	try
	{
	    serialPort.setSerialPortParams( baudRate,
					    SerialPort.DATABITS_8,
					    SerialPort.STOPBITS_1,
					    SerialPort.PARITY_NONE );
	}
	catch ( UnsupportedCommOperationException e )
	{
	    logger_.warning("Cannot program serial line parameters.");
	}

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

//	gpsInfoUpdated = false;
	gpsParsedMessages = 0;

	buffer = new String();
	this.port = port;
	this.baudRate = baudRate;

	enableHardware();

	//gpsThread = new Thread(this,"gpsThread");
	//gpsThread.start();
    }


    /**
     *  Return the current GPS configuration.
     *
     *@return    Configuration
     */
    public Configuration getConfiguration()
    {
	Configuration cfg = new Configuration();
	cfg.port = port;
	cfg.baudRate = baudRate;
	return cfg;
    }


    /**
     *  Checks if the GPS is alive according to the following definition: There
     *  have been more than 5 valid NMEA messages on the current serial line. This
     *  method can be used to search for the serial port.
     *
     *@return    True if GPS is alive, false otherwise.
     */
    public boolean isAlive()
    {
	if ( gpsMode )
	{
	    return gpsParsedMessages > 5;
	}
	else if ( replayMode )
	{
	    return true;
	}
	else
	{
	    return false;
	}
    }


    /**
     *  Obtain the current GPS information.
     *
     *@return    GPS information.
     */
    public GPSInfo getGPSInfo()
    {
	return gpsInfo;
	/*
	  try
	  {
	  GPSInfo gi = (GPSInfo)gpsInfo.clone();
	  normGPSInfo( gi );
	  return gi;
	  }
	  catch (Exception e)
	  {
	  return null;
	  }
	*/
    }


    /**
     *  Open the GPSDevice using the given configuration.
     *
     *@param  cfg         Configuration.
     *@throws  Exception  If comm. to GPS cannot be established.
     */
    public void open( Configuration cfg ) throws Exception
    {
	simulate = false;

	setupGPS( cfg.port, cfg.baudRate );
    }


    /**
     *  Open GPSDevice using an input stream to replay logged GPS information.
     *
     *@param  logStream   Input stream with logged GPS information.
     *@throws  Exception  If the stream is in error.
     */
    public void open( InputStream logStream ) throws Exception
    {
	simulate = true;

	inputStream = logStream;

//	gpsInfoUpdated = false;
	gpsParsedMessages = 0;

	gpsThread = new Thread( this, "gpsthread" );
	gpsThread.start();
    }


    /**
     *  Enables replay from a previously logged stream. Automatically detects raw
     *  and our internal log formats.
     *
     *@param  logStream   Stream with logged information.
     *@throws  Exception  In case the stream cannnot be opened or the format cannot
     *      be detected, an IllegalArgumentException is thrown.
     */
    public void enableReplay( InputStream logStream ) throws Exception
    {
	disableHardware();
	gpsReplayLog = GPSLog.load( logStream );
	if ( gpsReplayLog == null )
	{
	    throw new IllegalArgumentException( "logStream cannot be opened." );
	}

	if ( replayMode )
	{
	    disableReplay();
	}

	replayMode = true;
	replayThread = new Thread( this, "replaythread" );
	replayThread.start();
    }


    /**
     *  Disables replay mode.
     */
    public void disableReplay()
    {
	if ( replayThread != null )
	{
	    replayMode = false;
	    while ( replayThread != null )
	    {
		delay();
	    }
	}
    }


    /**
     *  Enables GPS hardware reporting.
     */
    public void enableHardware()
    {
	if ( gpsMode == false )
	{
	    gpsMode = true;

	    gpsThread = new Thread( this, "gpsthread" );
	    gpsThread.start();

	    logger_.info("GPS driver enabled.");
	}
    }


    /**
     *  Disables GPS hardware reporting.
     */
    public void disableHardware()
    {
	if ( gpsMode )
	{
	    gpsMode = false;
	    while ( gpsThread != null )
	    {
		delay();
	    }

	    logger_.info("GPS driver disabled.");
	}
    }


    /**
     *  Start logging GPS information into the given output stream (usually a
     *  FileOutputStream will be used).
     *
     *@param  s  Output stream to use for logging.
     */
    public void enableRawLogging( OutputStream s )
    {
	disableRawLogging();

	logRawStream = s;

	logRawInput = true;
    }


    /**
     *  Stops logging process. Does nothing if logging was not enabled.
     */
    public void disableRawLogging()
    {
	if ( logRawStream != null )
	{
	    try
	    {
		logRawStream.close();
	    }
	    catch ( IOException ioe )
	    {
	    }
	}

	logRawInput = false;
	logRawStream = null;
    }


    /**
     *  Enables logging current GPS data into a stream.
     *
     *@param  s  Stream to use for logging.
     */
    public void enableLogging( OutputStream s )
    {
	gpsLog = new GPSLog( new Date() );

	disableLogging();

	logStream = s;

	logInput = true;

	logThread = new Thread( this, "logthread" );
	logThread.start();
    }


    /**
     *  Disable logging.
     */
    public void disableLogging()
    {
	if ( logInput == false )
	{
	    return;
	}

	logInput = false;
	while ( logThread != null )
	{
	    delay();
	}

	try
	{
	    gpsLog.save( logStream );
	    logStream.flush();
	}
	catch ( IOException ioe )
	{
	}

	logInput = false;
	logStream = null;
    }


    /**
     *  Close the GPS device.
     */
    public void close()
    {
/*		if ( gpsThread != null )
		{
		gpsThread.stop();  // DEPRECATED
		gpsThread = null;
		}*/
	disableHardware();

	if ( inputStream != null )
	{
	    try { inputStream.close(); }
	    catch (IOException e)
	    {
		logger_.severe("I/O exception: " + e.getMessage());
	    }
	    inputStream = null;
	}

	if ( outputStream != null )
	{
	    try { outputStream.close(); }
	    catch (IOException e)
	    {
		logger_.severe("I/O exception: " + e.getMessage());
	    }
	    outputStream = null;
	}

	if ( serialPort != null )
	{
	    serialPort.close();
	    serialPort = null;
	}
    }


    public boolean isOpen()
    {
	if (gpsThread != null) return true;
	if (inputStream != null) return true;
	if (outputStream != null) return true;
	if (serialPort != null) return true;
	return false;
    }


    /**
     *  Method
     */
    public void replayThread()
    {
	int t = 0;
	long t0 = System.currentTimeMillis();

	try
	{

	    while ( t < gpsReplayLog.getDuration() && replayMode == true )
	    {
                info("replay at t="+t+" / "+gpsReplayLog.getDuration());

                gpsInfo = gpsReplayLog.getInfoAt( t );
		t++;
		processListeners();

		long t1 = System.currentTimeMillis();

		try
		{
		    Thread.currentThread().sleep( t0 + 1000 * t - t1 );
		}
		catch ( Exception ie )
		{
		}
	    }

	}
	catch (Exception e)
	{
	    logger_.log(Level.SEVERE, "Exception while replaying", e);
	}

	replayThread = null;
    }


    /**
     *  Method
     */
    public void logThread()
    {
	long t0 = System.currentTimeMillis();
	int t = 0;

	while ( logInput == true )
	{
	    GPSInfo gc = null;
	    try
	    {
		gc = ( GPSInfo ) gpsInfo.clone();
	    }
	    catch ( CloneNotSupportedException cnse )
	    {
	    }

	    //normGPSInfo( gc );
	    gpsLog.setInfoAt( t, gc );
	    t++;

	    long t1 = System.currentTimeMillis();
	    try
	    {
		long sl = 1000 * t + t0 - t1;
		info( "logging at t="+gpsLog.getDuration() );
		if ( sl > 0 )
		{
		    Thread.currentThread().sleep( sl );
		}
	    }
	    catch ( InterruptedException ie )
	    {
	    }
	}

	logThread = null;
    }


    /**
     * Read a line of text.  A line is considered to be terminated by any one
     * of a line feed ('\n'), a carriage return ('\r'), or a carriage return
     * followed immediately by a linefeed.
     *
     * @param      result A <code>StringBuffer</code> where the text will be appended.
     *
     * @return     <code>true</code>, if the line was finished, <code>false</code>,
     *             if not enough characters was available.
     * 
     * @see        java.io.LineNumberReader#readLine()
     *
     * @exception  IOException  If an I/O error occurs
     */
    boolean readLine(StringBuffer result, BufferedReader in) throws IOException
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
     *  Method
     */
    public void gpsThread()
    {
	try
	{
	    if ( inputStream == null )
	    {
		gpsThread = null;
		return;
	    }

	    BufferedReader in = new BufferedReader( new InputStreamReader(
							inputStream ) );

	    tmpInfo = new GPSInfo();
	    gpsInfo = new GPSInfo();
	    NMEA.infiniteAge( tmpInfo );

	    long t0;

	    long t1;

	    long t2;
	    String msg = "";
	    boolean replayLog = false;
	    long replayBase = -1;

	    try
	    {
		StringBuffer sb = new StringBuffer();
		while (readLine(sb, in) == false)
		{
		    if (!gpsMode)
		    {
			gpsThread = null;
			return;
		    }
		}
		msg = sb.toString();
	    }
	    catch ( Exception e )
	    {
	    }
	    logger_.finest(msg);

	    // check if we are reading previously logged messages
	    if ( simulate == true && msg.charAt( 0 ) != '$' && msg.indexOf( ":" ) > 0
		 && Character.isDigit( msg.charAt( 0 ) ) )
	    {
		replayLog = true;
		replayBase = Long.parseLong( msg.substring( 0, msg.indexOf( ':' ) ) );
	    }

	    t0 = t1 = System.currentTimeMillis();

	    /*
	     * Bug corrected on 05 June 2002. Thanks to F.Duerr!
	     * Used reference to unused variable readMode
	     */
	    while ( gpsMode == true )
	    {
		StringBuffer sb = new StringBuffer();
		while (readLine(sb, in) == false)
		{
		    if (!gpsMode)
		    {
			gpsThread = null;
			return;
		    }
		}
		msg = sb.toString();
//				msg = in.readLine();  // WOULD BLOCK!

		info( "msg from GPS device: "+msg );

		if ( msg == null )
		{
		    delay();
		}

		// check if we are reading previously logged messages
		if ( msg != null && replayLog )
		{
		    // read the time when the msg was logged, and
		    // skip time in msg
		    long twait = Long.parseLong( msg.substring( 0, msg.indexOf( ':' ) ) ) - replayBase;
		    msg = msg.substring( msg.indexOf( ':' ) + 1 );
		    logger_.finest("Replay " + msg);
		    long ct = System.currentTimeMillis();
		    // wait to replay correct timely behaviour
		    if ( ct - t0 < twait )
		    {
			logger_.finest("Sleeping " + (twait - (ct-t0)));
			Thread.currentThread().sleep( twait - ( ct - t0 ) );
			//Thread.currentThread().sleep( 10 );
		    }
		}

		/*				if (simulate && msg!=null)
						{
						if (replayBase<0)
						{
						if (gpsInfo.utc!=null)
						{
						replayBase = utcToSeconds( gpsInfo.utc );
						}
						}
						else
						{
						if (gpsInfo.utc!=null)
						{
						long ti = utcToSeconds( gpsInfo.utc );
						Thread.currentThread().sleep( 1000*(ti-replayBase) );
						replayBase = ti;
						}
						}
						}
		*/
		if ( logInput )
		{
		    try
		    {
			logStream.write( new String( "" + System.currentTimeMillis() + ":" ).getBytes() );
			logStream.write( ( msg + "\n" ).getBytes() );
		    }
		    catch ( Exception e )
		    {
			e.printStackTrace();
		    }
		}

		if ( msg != null && NMEA.check( msg ) )
		{
		    logger_.finest(msg);
		    // parse known messages
		    synchronized ( gpsInfo )
		    {
			if ( NMEA.parse( msg, tmpInfo ) )
			{
			    gpsParsedMessages++;
			}

//			gpsInfoUpdated = true;
			gpsInfo = ( GPSInfo ) tmpInfo.clone();
			normGPSInfo( gpsInfo );
			processListeners();
		    }
		}

		t2 = System.currentTimeMillis();
		// check if GPSInfo has to be aged...
		while ( t2 - t1 > 1000 )
		{
		    NMEA.increaseAge( tmpInfo );
		    t1 += 1000;

		    logger_.finest("*** GPSInfo ***");
		    NMEA.printAll(gpsInfo);
		}
	    }
	}
	catch (Exception e)
	{
	    gpsMode = false;

	    logger_.severe("Exception in GPS thread.");
	    e.printStackTrace();
	}

	gpsThread = null;
    }


    /**
     *  Main thread.
     */
    public void run()
    {
	String name = Thread.currentThread().getName();
	if ( name.equals( "logthread" ) )
	{
	    logThread();
	}
	else if ( name.equals( "gpsthread" ) )
	{
	    gpsThread();
	}
	else if ( name.equals( "replaythread" ) )
	{
	    replayThread();
	}
    }


    /**
     *  Add new GPS listener.
     *
     *@param  gl  GPSListener to add.
     */
    public void addGPSListener( GPSListener gl )
    {
	synchronized ( listeners )
	{
	    // insert listeners once only!
	    if ( !listeners.contains( gl ) )
	    {
		listeners.add( gl );
	    }
	}
    }


    /**
     *  Remove GPS listener.
     *
     *@param  gl  GPSListener to remove.
     */
    public void removeGPSListener( GPSListener gl )
    {
	synchronized ( listeners )
	{
	    listeners.remove( gl );
	}
    }


    /**
     *  Send updated GPSInfo to all registered listeners.
     */
    void processListeners()
    {
	for ( int i = 0; i < listeners.size(); i++ )
	{
	    GPSListener vl = ( GPSListener ) listeners.get( i );
	    vl.gpsEvent( getGPSInfo() );
	}
    }

    /**
     * Delay for a couple of milliseconds.
     */
    protected void delay()
    {
        try
        {
            Thread.sleep( 50 );
        }
        catch (InterruptedException ignored)
        {}
    }

    /**
     * Log info message
     */
    protected void info(String msg)
    {
	logger_.info(msg);
    }


    /**
     *  Description of the Class
     *
     *@author    walther
     */
    public static class Configuration
    {
	public String port;
	public int baudRate;

	private static final String F_PORT = "port";
	private static final String F_BAUDRATE = "baud rate";


	public Configuration()
	{
	    port = "/dev/ttyS0";
	    baudRate = 4800;
	}

	public Configuration(InputStream is) throws IOException
	{
	    read(is);
	}

	public Configuration(String fileName) throws IOException, FileNotFoundException
	{
	    read(fileName);
	}

	public void write(OutputStream os) throws IOException
	{
	    Properties p = new Properties();

	    p.put(F_PORT, port);
	    p.put(F_BAUDRATE, Integer.toString(baudRate));

	    p.store(os, "GPSDriver.Configuration");
	}

	public void write(String fileName) throws IOException
	{
	    write(new FileOutputStream(fileName));
	}

	public void read(InputStream is) throws IOException
	{
	    Properties p = new Properties();
	    p.load(is);
	    String s;

	    s = (String)p.get(F_PORT);
	    port = s;

	    s = (String)p.get(F_BAUDRATE);
	    baudRate = Integer.parseInt(s, 10);
	}

	public void read(String fileName) throws IOException, FileNotFoundException
	{
	    read(new FileInputStream(fileName));
	}
    }
}

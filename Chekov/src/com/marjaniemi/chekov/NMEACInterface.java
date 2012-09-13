/*
 * Copyright (c) 2002 Seppo Enarvi. All Rights Reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * -Redistributions of source code must retain the above copyright
 *  notice, this list of conditions and the following disclaimer.
 * 
 * -Redistribution in binary form must reproduct the above copyright
 *  notice, this list of conditions and the following disclaimer in
 *  the documentation and/or other materials provided with the distribution.
 */


package com.marjaniemi.chekov;

import java.net.ConnectException;
import java.net.UnknownHostException;
import com.marjaniemi.nmeac.*;
import java.util.logging.Logger;


/**
 * GPS position reader.
 */
public class NMEACInterface implements NMEAInfoListener
{
    /**
     * The <code>NMEAClient</code> or <code>null</code> if the client hasn't
     * been set up yet.
     */
    private NMEAClient nmeac_ = null;

    private static Logger logger_ = Logger.getLogger("com.marjaniemi.chekov");

    /**
     * The current position in a geographic coordinate system
     * (latitude/longitude) as read from the GPS receiver. This
     * field should only be accessed with the synchronized
     * <code>gpsEvent</code> and <code>getGeographicPosition</code>
     * methods.
     */
//    private Point2D.Double position_ = new Point2D.Double(25.12144, 60.20284);

    private NMEAInfo nmeaInfo_ = new NMEAInfo();
    private boolean nmeaInfoChanged_ = false;


    /**
     * Indicates that the position has changed since the last
     * call to getGeographicPosition.
     */
//    private boolean positionChanged_ = true;


    /**
     * An event that is called everytime the GPS driver has new
     * GPS data available. Stores the current geographic position.
     *
     * @param gi Current GPSInfo.
     */
    public synchronized void nmeaInfoEvent(NMEAInfo nmeaInfo)
    {
	// The order longitude-latitude / latitude-longitude actually
	// depends on the coordinate system.
/*	Point2D.Double newPosition = new Point2D.Double(gi.longitude, gi.latitude);

	if ((newPosition.getX() != 0) &&
	    (newPosition.getY() != 0) &&
	    ((newPosition.getX() != position_.getX()) ||
	     (newPosition.getY() != position_.getY())))
	{
	    position_ = newPosition;
	    positionChanged_ = true;
	    notifyAll();
	    }*/

	nmeaInfo_ = (NMEAInfo)nmeaInfo.clone();
	nmeaInfoChanged_ = true;
	notifyAll();
    }


    /**
     * Wait until the position has changed and return the new
     * position.
     *
     * @exception InterruptedException Another thread has
     *            interrupted this thread with the interrupt
     *            method.
     */
/*    public synchronized CoordinatePoint getGeographicPosition()
	throws InterruptedException
    {
	if (!positionChanged_)
	{
	    wait();
	}
	positionChanged_ = false;
	return new CoordinatePoint(position_);
    }*/
    public synchronized NMEAInfo getNMEAInfo()
	throws InterruptedException
    {
	if (!nmeaInfoChanged_)
	{
	    wait();
	}
	nmeaInfoChanged_ = false;
	return (NMEAInfo)nmeaInfo_.clone();
    }


    /**
     * Start the NMEA position reader with given setup. If the reader has
     * already been running, close the previous reader before starting the
     * new one.
     *
     * @return <code>true</code> if successful, <code>false</code> otherwise.
     */
    public boolean setup(String hostname, int port)
    {
	close();

	try
	{
	    nmeac_ = new NMEAClient(hostname, port);
	    nmeac_.addListener(this);
	}
	catch (ConnectException ce)
	{
	    logger_.info("Unable to connect to nmead:\n" + ce.getMessage());
	    return false;
	}
	catch (UnknownHostException uhe)
	{
	    logger_.info("Unable to connect to nmead.");
	    logger_.info("Unknown host: " + hostname + ":" + port);
	    return false;
	}
	catch (Exception e)
	{
	    logger_.severe("Unexpected exception while setting up NMEA " +
			   "client:\n" + e.getMessage());
	    e.printStackTrace();
	    return false;
	}

	return true;
    }


    /**
     * Close the NMEA position reader, if it's running.
     */
    public void close()
    {
	if (nmeac_ != null)
	    {
		nmeac_.close();
		nmeac_ = null;
	    }
    }
}

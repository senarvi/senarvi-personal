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
 *  GPSInfo encapsulates all information as retrieved by a GPS device.
 */
public class NMEAInfo implements Cloneable, java.io.Serializable
{
    // time
    public String utc;

    // position
    public double latitude;
    public char northHemi;
    public double longitude;
    public char eastHemi;

    // height
    public double height;
    public double geoidalHeight;

    // course
    public double courseOverGround;
    public double speedOverGround;

    // quality
    public int quality;
    public int numberOfSatellites;
    public double horDilution;
    public double verDilution;
    public double HPE;
    public double VPE;
    public double EPE;

    // DGPS info
    public int diffCorrection;
    public int diffStationID;

    // GPS device info
    public String GPSModel;
    public String romChecksum;
    public double boardTemperature;


    public Object clone()
    {
	Object result;
	try
	    {
		result = super.clone();
	    }
	catch (CloneNotSupportedException e)
	    {
		// Should never happen.
		throw new InternalError("Unexpected exception: " + e.getMessage());
	    }
	return result;
    }


    public NMEAInfo()
    {
	latitude = 0;
	longitude = 0;
    }


    /*
    public NMEAInfo(NMEAInfo x)
    {
	utc = x.utc;
	latitude = x.latitude;
	northHemi = x.northHemi;
	longitude = x.longitude;
	eastHemi = x.eastHemi;

	height = x.height;
	geoidalHeight = x.geoidalHeight;

	courseOverGround = x.courseOverGround;
	speedOverGround = x.speedOverGround;
	quality = x.quality;
	numberOfSatellites = x.numberOfSatellites;
	horDilution = x.horDilution;
	verDilution = x.verDilution;
	HPE = x.HPE;
	VPE = x.VPE;
	EPE = x.EPE;
	diffCorrection = x.diffCorrection;
	diffStationID = x.diffStationID;
	GPSModel = x.GPSModel;
	romChecksum = x.romChecksum;
	boardTemperature = x.boardTemperature;
	}*/
}

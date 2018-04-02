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

/**
 *  GPSInfo encapsulates all information as retrieved by a GPS device.
 *
 *@author    walther
 */
public class GPSInfo implements Cloneable, java.io.Serializable {

    // time
    public String utc;

    // position
    public double latitude;
    public int latitudeAge;
    public char northHemi;
    public int northHemiAge;
    public double longitude;
    public int longitudeAge;
    public char eastHemi;
    public int eastHemiAge;

    // Gauss-Krueger coordinates of lat/long:
//	public double X;
//	public int XAge;
//	public double Y;
//	public int YAge;

    // height
    public double height;
    public int heightAge;
    public double geoidalHeight;
    public int geoidalHeightAge;

    // course
    public double courseOverGround;
    public int courseOverGroundAge;
    public double speedOverGround;
    public int speedOverGroundAge;

    // quality
    public int quality;
    public int qualityAge;
    public int numberOfSatellites;
    public double horDilution;
    public int horDilutionAge;
    public double verDilution;
    public int verDilutionAge;
    public double HPE;
    public int HPEAge;
    public double VPE;
    public int VPEAge;
    public double EPE;
    public int EPEAge;

    // DGPS info
    public int diffCorrection;
    public int diffCorrectionAge;
    public int diffStationID;

    // GPS device info
    public String GPSModel;
    public int GPSModelAge;
    public String romChecksum;
    public double boardTemperature;


    /**
     *  Method
     *
     *@return                                 Returned Value
     *@exception  CloneNotSupportedException  Exception
     */
    public Object clone() throws CloneNotSupportedException
    {
	return super.clone();
    }

    public GPSInfo()
    {
	latitude = 0;
	longitude = 0;
    }

    public GPSInfo(GPSInfo x)
    {
	utc = x.utc;
	latitude = x.latitude;
	latitudeAge = x.latitudeAge;
	northHemi = x.northHemi;
	northHemiAge = x.northHemiAge;
	longitude = x.longitude;
	longitudeAge = x.longitudeAge;
	eastHemi = x.eastHemi;
	eastHemiAge = x.eastHemiAge;

	height = x.height;
	heightAge = x.heightAge;
	geoidalHeight = x.geoidalHeight;
	geoidalHeightAge = x.geoidalHeightAge;

	courseOverGround = x.courseOverGround;
	courseOverGroundAge = x.courseOverGroundAge;
	speedOverGround = x.speedOverGround;
	speedOverGroundAge = x.speedOverGroundAge;
	quality = x.quality;
	qualityAge = x.qualityAge;
	numberOfSatellites = x.numberOfSatellites;
	horDilution = x.horDilution;
	horDilutionAge = x.horDilutionAge;
	verDilution = x.verDilution;
	verDilutionAge = x.verDilutionAge;
	HPE = x.HPE;
	HPEAge = x.HPEAge;
	VPE = x.VPE;
	VPEAge = x.VPEAge;
	EPE = x.EPE;
	EPEAge = x.EPEAge;
	diffCorrection = x.diffCorrection;
	diffCorrectionAge = x.diffCorrectionAge;
	diffStationID = x.diffStationID;
	GPSModel = x.GPSModel;
	GPSModelAge = x.GPSModelAge;
	romChecksum = x.romChecksum;
	boardTemperature = x.boardTemperature;
    }
}

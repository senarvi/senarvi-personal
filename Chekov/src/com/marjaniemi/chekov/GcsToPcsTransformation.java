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

import org.geotools.cs.*;
import org.geotools.ct.*;
import org.geotools.pt.CoordinatePoint;
import org.opengis.referencing.FactoryException;
import org.opengis.referencing.operation.TransformException;

public class GcsToPcsTransformation {
	private CoordinateSystem sourceCS_ = GeographicCoordinateSystem.WGS84;

	private MathTransform mathTransform_ = null;

	public GcsToPcsTransformation(CoordinateSystem sourceCS, String targetEPSG)
			throws FactoryException, TransformException {
		assert targetEPSG == "2393";

		// We should retrieve the WKT according to the given targetEPSG, but the
		// EPSG database is available only in Microsoft Access format, so currently
		// I'll have to use the hardcoded WKT string below.

		CoordinateSystemFactory csFactory = CoordinateSystemFactory
				.getDefault();
		CoordinateTransformationFactory ctFactory = CoordinateTransformationFactory
				.getDefault();

		String wkt = "PROJCS[\"KKJ / Finland Uniform Coordinate System\", GEOGCS[\"KKJ\","
				+ "DATUM[\"Kartasto Koordinaati Jarjestelma 1966\","
				+ "SPHEROID[\"International 1924\",6378388.0,297.0, AUTHORITY[\"EPSG\",\"7022\"]],"
				+ "TOWGS84[-90.7, -106.1, -119.2, 4.09, 0.218, -1.05, 1.37], AUTHORITY[\"EPSG\",\"6123\"]],"
				+ "PRIMEM[\"Greenwich\", 0.0, AUTHORITY[\"EPSG\",\"8901\"]],"
				+ "UNIT[\"degr� d'angle\",0.017453292519943295], AXIS[\"Geodetic latitude\",NORTH],"
				+ "AXIS[\"Geodetic longitude\",EAST], AUTHORITY[\"EPSG\",\"4123\"]],"
				+ "PROJECTION[\"Transverse_Mercator\"],"
				+ "PARAMETER[\"semi_major\", 6378388.0],"
				+ "PARAMETER[\"semi_minor\", 6356911.9461279465],"
				+ "PARAMETER[\"central_meridian\", 27.0],"
				+ "PARAMETER[\"latitude_of_origin\", 0.0],"
				+ "PARAMETER[\"scale_factor\", 1.0],"
				+ "PARAMETER[\"false_easting\", 3500000.0],"
				+ "PARAMETER[\"false_northing\", 0.0],"
				+ "UNIT[\"m�tre\",1.0], AXIS[\"Northing\",NORTH], AXIS[\"Easting\",EAST],"
				+ "AUTHORITY[\"EPSG\",\"2393\"]]";

		CoordinateSystem targetCS = csFactory.createFromWKT(wkt);

		CoordinateTransformation ct = ctFactory.createFromCoordinateSystems(
				sourceCS, targetCS);

		mathTransform_ = ct.getMathTransform();
	}

	/**
	 * Transform coordinates from a geographic coordinate system (given to the
	 * constructor) to a projected coordinate system (as specified by the EPSG
	 * code given to the constructor.
	 *
	 * @param sourceCP The <code>CoordinatePoint</code> to transform.
	 *
	 * @return The transformed coordinate as a <code>PlanarCoordinate</code>.
	 */
	public PlanarCoordinate transform(CoordinatePoint cp)
			throws TransformException {
		CoordinatePoint result = new CoordinatePoint(2);
		mathTransform_.transform(cp, result);
		return new PlanarCoordinate(result);
	}
}

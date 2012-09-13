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

import java.util.logging.Logger;
import java.awt.geom.Point2D;
import javax.media.jai.PlanarImage;
import javax.swing.JOptionPane;
import org.geotiff.image.jai.GeoTIFFDirectory;
import org.geotools.cs.GeographicCoordinateSystem;
import org.geotools.pt.CoordinatePoint;
import org.opengis.referencing.operation.TransformException;
import com.marjaniemi.nmeac.NMEAInfo;

public class PositionThread implements Runnable {
	private static final Logger logger_ = Logger
			.getLogger("com.marjaniemi.chekov");

	private GeographicCoordinateSystem sourceCS_ = GeographicCoordinateSystem.WGS84;

	private GcsToPcsTransformation gcsToPcs_ = null;

	private PcsToImageTransformation pcsToImage_ = null;

	private Point2D.Double position_ = new Point2D.Double(0, 0);

	private Map map_ = null;

	private Thread thread_ = null;

	private PlanarCoordinate gcsToPcs(CoordinatePoint gcsPoint)
			throws TransformException {
		assert gcsToPcs_ != null;

		logger_.finer("GCS: X=" + gcsPoint.getOrdinate(0) + " Y="
				+ gcsPoint.getOrdinate(1));
		PlanarCoordinate pcsPoint = gcsToPcs_.transform(gcsPoint);
		logger_.finer("PCS: X=" + pcsPoint.getX() + " Y=" + pcsPoint.getY());
		return pcsPoint;
	}

	private PlanarCoordinate pcsToImage(PlanarCoordinate pcsPoint) {
		assert pcsToImage_ != null;

		logger_.finer("PCS: X=" + pcsPoint.getX() + " Y=" + pcsPoint.getY());
		PlanarCoordinate imagePoint = pcsToImage_.transform(pcsPoint);
		logger_.finer("Image: X=" + imagePoint.getX() + " Y="
				+ imagePoint.getY());
		return imagePoint;
	}

	public void run() {
		// We have to know the GCS in advance to find the map that covers
		// the current position.
		try {
			gcsToPcs_ = new GcsToPcsTransformation(sourceCS_, "2393");
		} catch (Exception e) {
			logger_.severe("Unable to create transformation to the PCS used "
					+ "in the map.");
			logger_.severe("Error that occured: " + e.getMessage());
			logger_.severe("Position thread is stopped.");
			return;
		}

		while (!Thread.interrupted()) {
			NMEAInfo nmeai = null;

			try {
				// getNMEAInfo() may block.
				nmeai = Chekov.nmeacInterface.getNMEAInfo();
			} catch (InterruptedException ie) {
				return;
			}

			// ***** SAMPLE POSITION *****
			//	    nmeai = new NMEAInfo();
			// nmeai.longitude = 25 + 07.0/60 + 20.9/3600;
			// nmeai.latitude  = 60 + 12.0/60 + 11.5/3600;

			try {
				Point2D.Double newPosition = new Point2D.Double(
						nmeai.longitude, nmeai.latitude);
				if (
				// The NMEAInfo structure has longitude=latitude=0 until
				// the NMEAClient first receives some coordinates.
				(newPosition.getX() != 0)
						&& (newPosition.getY() != 0)
						&&

						((newPosition.getX() != position_.getX()) || (newPosition
								.getY() != position_.getY()))) {
					position_ = newPosition;
					CoordinatePoint gcsPosition = new CoordinatePoint(position_);

					PlanarCoordinate pcsPosition = gcsToPcs(gcsPosition);
					changeMap(pcsPosition);
					if (map_ != null) {
						PlanarCoordinate imagePosition = pcsToImage(pcsPosition);
						Chekov.mainFrame.displayPosition(imagePosition);
					}
				}

				Chekov.mainFrame.displayGeographicPosition(nmeai.latitude,
						nmeai.longitude);
				Chekov.mainFrame.displayTime(nmeai.utc);
				Chekov.mainFrame.displayHeight(nmeai.height);
				Chekov.mainFrame.displayCourse(nmeai.courseOverGround);
				Chekov.mainFrame.displaySpeed(nmeai.speedOverGround);
				Chekov.mainFrame.displayNumSatellites(nmeai.numberOfSatellites);
			} catch (TransformException te) {
				logger_.severe("Unable to perform coordinate transformation.");
				logger_.severe("Error that occured: " + te.getMessage());
			}
		}
	}

	/**
	 * Find the map covering the given position from the database and open the
	 * map. The new <code>Map</code> object will be stored to map_, a new
	 * <code>PcsToImageTransformation</code> will be store to pcsToImage_ and
	 * information about this map will be displayed in the GUI. If appropriate
	 * map won't be found, map_ and pcsToImage_ will be left intact.
	 *
	 * @return <code>true</code>, if succesful, <code>false</code> if
	 *         appropriate map wasn't found.
	 */
	private boolean changeMap(PlanarCoordinate position) {
		if ((map_ != null) && map_.contains(position))
			return true;

		logger_.finer("Changing map.");

		Map newMap;
		GeoTIFFDirectory directory;
		PlanarImage image;

		boolean succeeded;
		do {

			newMap = Chekov.mapDB.getMapCovering(position);

			if (newMap == null) {
				logger_.warning("None of the maps covers current position.");
				return false;
			}

			boolean readSameFileAgain;
			do {

				directory = newMap.getDirectory();
				image = newMap.getImage();
				succeeded = (directory != null) && (image != null);

				if (succeeded)

					readSameFileAgain = false;

				else {

					logger_.warning("An error was encountered while trying "
							+ "to open a map file.");
					logger_.warning("The error was: " + newMap.getLastError());

					Object[] options = { "Retry",
							"Don't try to use this map anymore" };
					int feedback = JOptionPane.showOptionDialog(
							Chekov.mainFrame,
							"An error was encountered while trying to open a "
									+ "map file:\n" + newMap.getLastError()
									+ "\n" + "What should I do?", "Chekov",
							JOptionPane.YES_NO_OPTION,
							JOptionPane.ERROR_MESSAGE, null, options,
							options[1]);
					readSameFileAgain = feedback == JOptionPane.YES_OPTION;

					if (readSameFileAgain) {
						logger_.warning("Retrying to open the map file.");
					} else {
						logger_.warning("I will not try to reopen the same "
								+ "file.");
					}
				}

			} while (readSameFileAgain);

		} while (!succeeded);

		if (map_ != null)
			map_.invalidate();
		map_ = newMap;

		pcsToImage_ = new PcsToImageTransformation(directory);

		Chekov.mainFrame.displayMap(image, map_.getName(), "2393");

		return true;
	}

	public void start() {
		thread_ = new Thread(this);
		thread_.start();
	}

	public void stop() throws InterruptedException {
		if (thread_ != null) {
			thread_.interrupt();
			thread_.join(1000);
			if (thread_.isAlive()) {
				logger_.warning("Position thread refused to die.");
			}
		}
	}
}

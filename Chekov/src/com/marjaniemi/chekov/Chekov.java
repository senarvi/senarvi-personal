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

import java.io.IOException;
import java.util.logging.*;

public class Chekov {
	public static NMEACInterface nmeacInterface = new NMEACInterface();

	public static MapDB mapDB = new MapDB();

	public static MainFrame mainFrame = new MainFrame();

	public static PositionThread positionThread = new PositionThread();

	private static final Logger chekovLogger_ = Logger
			.getLogger("com.marjaniemi.chekov");

	/**
	 * Set logging levels and logging handlers.
	 */
	private static void setupLogging() {
		Logger nmeacLogger = Logger.getLogger("com.marjaniemi.nmeac");

		chekovLogger_.setLevel(Level.ALL);
		nmeacLogger.setLevel(Level.ALL);

		try {
			FileHandler chekovFh = new FileHandler("chekov.log");
			chekovFh.setLevel(Level.ALL);
			chekovFh.setFormatter(new SimpleFormatter());
			chekovLogger_.addHandler(chekovFh);
		} catch (Exception e) {
			chekovLogger_.warning("Unable to set file logging: " + e);
		}

		try {
			FileHandler nmeacFh = new FileHandler("nmeac.log");
			nmeacFh.setLevel(Level.ALL);
			nmeacFh.setFormatter(new SimpleFormatter());
			nmeacLogger.addHandler(nmeacFh);
		} catch (Exception e) {
			chekovLogger_.warning("Unable to set file logging: " + e);
		}
	}

	public static void main(String args[]) {
		setupLogging();

		try {
			mapDB.parseDefinitions("maplist.txt");
		} catch (IOException e) {
			chekovLogger_.severe("I/O Exception while trying to parse map "
					+ "definitions.");
			chekovLogger_.severe(e.getMessage());
		}
		chekovLogger_.info("" + mapDB.numMaps() + " map definitions read.");

		positionThread.start();
		// The main thread will terminate as soon as the position thread is
		// started.
	}

	public static void terminate() {
		nmeacInterface.close();

		try {
			positionThread.stop();
		} catch (InterruptedException ie) {
			chekovLogger_
					.warning("A thread was interrupted while it was waiting for "
							+ "the position thread to die.");
		}

		chekovLogger_.fine("Program terminated normally.");
		System.exit(0);
	}
}

package com.marjaniemi.chekov;

import java.util.Collection;
import java.util.Vector;
import java.util.Iterator;
import java.util.StringTokenizer;
import java.util.logging.*;
import java.io.*;


public class MapDB
{
    private static final Logger logger_ =
	Logger.getLogger("com.marjaniemi.chekov");
    private Collection maps_ = new Vector();


    public void add(double x1, double y1, double x2, double y2,
		    String fileName)
    {
	maps_.add(new Map(x1, y1, x2, y2, fileName));
    }


    private static class ParseException
	extends java.lang.Exception
    {
	public ParseException(String message)
	{
	    super(message);
	}
    }


    public void parseDefinitions(String mapListFileName) throws IOException
    {
	int numErrorsLogged = 0;

	BufferedReader reader =
	    new BufferedReader(new FileReader(mapListFileName));


	while (true) {

	    String line = reader.readLine();
	    if (line == null)
		break;  // No more lines - break from the while loop.

	    try {

		StringTokenizer st = new StringTokenizer(line);
		if (!st.hasMoreTokens())
		    // Empty line - continue from the beginning of the while
		    // loop.
		    continue;
		
		double x1 = Double.parseDouble(st.nextToken());
		
		if (!st.hasMoreTokens())
		    throw new ParseException("Only one coordinate is " +
					     "given.");
		
		double y1 = Double.parseDouble(st.nextToken());
		
		if (!st.hasMoreTokens())
		    throw new ParseException("Only two coordinates " +
					     "are given.");
		
		double x2 = Double.parseDouble(st.nextToken());
		
		if (!st.hasMoreTokens())
		    throw new ParseException("Only three coordinates " +
					     "are given.");
		
		double y2 = Double.parseDouble(st.nextToken());
		
		if (!st.hasMoreTokens())
		    throw new ParseException("File name is not given.");
		
		String fileName = st.nextToken();
		
		if (st.hasMoreTokens())
		    throw new ParseException("Too many tokens.");
		
		add(x1, y1, x2, y2, fileName);
	    }
	    catch (ParseException pe) {
		
		if (numErrorsLogged++ < 10) {
		    
		    logger_.warning("A map definition line is in " +
				    "incorrect format.");
		    logger_.warning(pe.getMessage());
		    logger_.warning("The Incorrect line: " + line);
		}
	    }
	    catch (NumberFormatException nfe) {
		
		if (numErrorsLogged++ < 10) {
		    
		    logger_.warning("Unable to parse a coordinate in a " +
				    "map definition line.");
		    logger_.warning(nfe.getMessage());
		    logger_.warning("The Incorrect line: " + line);
		}
	    }
	}


	if (reader != null) {

	    try {
		reader.close();
	    }
	    catch (IOException e) {
		logger_.warning("I/O exception while trying to close the " +
				"map definition file.");
		logger_.warning(e.getMessage());
	    }
	}
    }


    /**
     * Return a map from the database that covers the given position.
     * The position is in PCS, but it would be better if the position
     * were in WGS-84, so that we could find a map before we know the
     * PCS.
     *
     * @return The <code>Map</code> or <code>null</code> if no map in
     *         the database covers the given position.
     */
    public Map getMapCovering(PlanarCoordinate position)
    {
	Iterator i = maps_.iterator();

	while (i.hasNext())
	{
	    Map map = (Map)i.next();
	    if (map.contains(position) && map.isGood())
		return map;
	}
	return null;
    }


    /**
     * Return the number of maps in the database.
     */
    public int numMaps()
    {
	return maps_.size();
    }
}

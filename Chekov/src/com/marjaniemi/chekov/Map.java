/*
 * Portions copyright (c) 2002 Sun Microsystems, Inc. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 * 
 * -Redistributions of source code must retain the above copyright notice, this 
 * list of conditions and the following disclaimer.
 *
 * -Redistribution in binary form must reproduct the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of Sun Microsystems, Inc. or the names of contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * This software is provided "AS IS," without a warranty of any kind. ALL
 * EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES, INCLUDING ANY
 * IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NON-INFRINGEMENT, ARE HEREBY EXCLUDED. SUN AND ITS LICENSORS SHALL NOT BE
 * LIABLE FOR ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING
 * OR DISTRIBUTING THE SOFTWARE OR ITS DERIVATIVES. IN NO EVENT WILL SUN OR ITS
 * LICENSORS BE LIABLE FOR ANY LOST REVENUE, PROFIT OR DATA, OR FOR DIRECT,
 * INDIRECT, SPECIAL, CONSEQUENTIAL, INCIDENTAL OR PUNITIVE DAMAGES, HOWEVER
 * CAUSED AND REGARDLESS OF THE THEORY OF LIABILITY, ARISING OUT OF THE USE OF
 * OR INABILITY TO USE SOFTWARE, EVEN IF SUN HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES.
 *
 * You acknowledge that Software is not designed,licensed or intended for use in
 * the design, construction, operation or maintenance of any nuclear facility.
 */


package com.marjaniemi.chekov;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.awt.image.IndexColorModel;
import java.awt.image.RenderedImage;
import javax.media.jai.JAI;
import javax.media.jai.LookupTableJAI;
import javax.media.jai.PlanarImage;
import com.sun.media.jai.codec.*;
import org.geotiff.image.jai.*;
import java.util.logging.Logger;


public class Map
{
    private static Logger logger_ = Logger.getLogger("com.marjaniemi.chekov");

    private double x1_;  // The smaller X value.
    private double y1_;  // The smaller Y value.
    private double x2_;  // The larger X value.
    private double y2_;  // The larger Y value.

    private String fileName_;

    private PlanarImage image_ = null;
    private GeoTIFFDirectory directory_ = null;

    /* Is the map file good or has there been errors opening it? */
    private boolean isGood_ = true;

    private static String lastError_ = "Succeeded";


    public Map(double x1, double y1, double x2, double y2,
	       final String fileName)
    {
	if (x1 <= x2) {

	    x1_ = x1;
	    x2_ = x2;
	}
	else {
		
	    x1_ = x2;
	    x2_ = x1;
	}

	if (y1 <= y2) {

	    y1_ = y1;
	    y2_ = y2;
	}
	else {

	    y1_ = y2;
	    y2_ = y1;
	}

	fileName_ = "maps/" + fileName;
    }


    /**
     *  Try to open the GeoTIFF file of this map. If this fails, set lastError_
     * to explain why, set isGood_ to <code>false</code> and return
     * <code>false</code>. Otherwise set isGood_ to <code>true</code> and
     * return <code>true</code>.
     */
    private boolean read()
    {
	// In case of an exception in the stream constructor, the stream object
	// will be null.
	SeekableStream stream = null;

	try {

	    stream = new FileSeekableStream(fileName_);

	    // Use the JAI API unless JAI_IMAGE_READER_USE_CODECS is set.
	    if (System.getProperty("JAI_IMAGE_READER_USE_CODECS") == null)
	    {
		logger_.finer("Using the JAI API");
		image_ = JAI.create("fileload", fileName_);
	    }
	    else
	    {
		logger_.finer("Using the ImageCodec API.");
		String[] names = ImageCodec.getDecoderNames(stream);
		ImageDecoder dec =
		    ImageCodec.createImageDecoder(names[0], stream, null);
		RenderedImage ri = dec.decodeAsRenderedImage();
		image_ = PlanarImage.wrapRenderedImage(ri);
	    }

	    // If the image is colormapped, convert it to 3-band RGB.
	    if (image_.getColorModel() instanceof IndexColorModel)
	    {
		// Retrieve the IndexColorModel
		IndexColorModel icm = (IndexColorModel)image_.getColorModel();

		// Cache the number of elements in each band of the colormap.
		int mapSize = icm.getMapSize();

		// Allocate an array for the lookup table data.
		byte[][] lutData = new byte[3][mapSize];

		// Load the lookup table data from the IndexColorModel.
		icm.getReds(lutData[0]);
		icm.getGreens(lutData[1]);
		icm.getBlues(lutData[2]);

		// Create the lookup table object.
		LookupTableJAI lut = new LookupTableJAI(lutData);

		// Replace the original image with the 3-band RGB image.
		image_ = JAI.create("lookup", image_, lut);
	    }

	    directory_ = new GeoTIFFDirectory(stream, 0);

	    stream.close();
	    isGood_ = true;
	    return true;
	}
	catch (FileNotFoundException fnfe)
	{
	    lastError_ = "Map file was not found: " + fnfe.getMessage();
	    logger_.warning(lastError_);
	}
	catch (IOException ioe)
	{
	    lastError_ = "I/O error while trying to read map file: " +
		ioe.getMessage();
	    logger_.severe(lastError_);
	}

	if (stream != null)
	    try { stream.close(); }
	    catch (IOException ioe) {}

	isGood_ = false;
	invalidate();
	return false;
    }


    /**
     * Invalidate the data read with <code>read</code> method. This forces the
     * new <code>getImage</code> or <code>getDirectory</code> call to do a
     * <code>read</code>. This will hopefully also make the grabage collector
     * free the memory occupied by the image data and the GeoTIFF directory.
     */
    public void invalidate()
    {
	image_ = null;
	directory_ = null;
    }


    public String getName()
    {
	return fileName_;
    }


    /**
     * Get the raster image of the map. If the image file has not yet been
     * opened, open it. If an error is encountered while opening the image
     * file, isGood_ is set to <code>false</code> and <code>false</code> is
     * returned, but the state of isGood_ won't be checked before opening the
     * file.
     */
    public PlanarImage getImage()
    {
	boolean succeeded = true;
	if (image_ == null)
	    succeeded = read();
	if (!succeeded) return null;
	return image_;
    }


    /**
     * Get the GeoTIFF metadata directory. If the image file has not yet been
     * opened, open it. If an error is encountered while opening the image
     * file, isGood_ is set to <code>false</code> and <code>false</code> is
     * returned, but the state of isGood_ won't be checked before opening the
     * file.
     */
    public GeoTIFFDirectory getDirectory()
    {
	boolean succeeded = true;
	if (directory_ == null)
	    succeeded = read();
	if (!succeeded) return null;
	return directory_;
    }


    public boolean contains(PlanarCoordinate position)
    {
	double x = position.getX();
	double y = position.getY();

	return
	    (x >= x1_) &&
	    (x <= x2_) &&
	    (y >= y1_) &&
	    (y <= y2_);
    }


    public static String getLastError()
    {
	return lastError_;
    }


    public boolean isGood()
    {
	return isGood_;
    }
}

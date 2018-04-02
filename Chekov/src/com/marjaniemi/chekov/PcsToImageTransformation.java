package com.marjaniemi.chekov;

import org.geotiff.image.jai.GeoTIFFDirectory;


/**
 * A class that transforms coordinates from a projected coordinate system to an
 * image space. The parameters to the transformation are given as GeoTIFF tags.
 */
public class PcsToImageTransformation
{
    /**
     * Construct a transformation object with transformation parameters read
     * from a GeoTIFF directory.
     *
     * @param directory A <code>GeoTIFFDirectory</code> created from a GeoTIFF
     *                  file stream. This contains the transformation
     *                  attributes.
     */
    PcsToImageTransformation(GeoTIFFDirectory directory)
    {
        directory_ = directory;
    }

    /**
     * Transform horizontal projection coordinates to pixel/line coordinates.
     *
     * At this time this function does not support PCS to image translations for
     * tiepoints-only based definitions, only matrix and pixelscale/tiepoints
     * formulations are supposed.
     *
     * @param sourceCP The <code>PlanarCoordinate</code> to transform.
     *
     * @return <code>PlanarCoordinate</code> with the pixel offsets to the image,
     * or <code>null</code> if the transformation fails.  It may fail if the
     * GeoTIFF directory doesn't have proper transformation information, or it
     * is in a form unsupported by this function.
     */
    public PlanarCoordinate transform(PlanarCoordinate pc)
    {
	// The orientation actually depends on the coordinate system.
	double x = pc.getY();
	double y = pc.getX();

        double[] tiepoints = directory_.getTiepoints();
        int      tiepoint_count;

        double[] pixel_scale = directory_.getPixelScale();
        int      count;

        double[] transform = directory_.getTransformationMatrix();
        int      transform_count;


        // Fetch tiepoints and pixel scale.

        if (tiepoints == null)
            tiepoint_count = 0;
        else
            tiepoint_count = tiepoints.length;

        if (pixel_scale == null)
            count = 0;
        else
            count = pixel_scale.length;

        if (transform == null)
            transform_count = 0;
        else
            transform_count = transform.length;


        // If the pixelscale count is zero, but we have tiepoints use
        // the tiepoint based approach (not yet implemented).
        
        if (tiepoint_count > 6 && count == 0)
        {
        }


        // Handle matrix - convert to "geotransform" format, invert and apply.
        
        else if (transform_count == 16)
        {
            double[] gtMatrix = new double[6];
            gtMatrix[0] = transform[0];
            gtMatrix[1] = transform[1];
            gtMatrix[2] = transform[3];
            gtMatrix[3] = transform[4];
            gtMatrix[4] = transform[5];
            gtMatrix[5] = transform[7];
            double[] invMatrix = gtMatrixInverse(gtMatrix);
            
            if (invMatrix != null)
            {
                double resultX = x * invMatrix[0] + y * invMatrix[1] + invMatrix[2];
                double resultY = x * invMatrix[3] + y * invMatrix[4] + invMatrix[5];
                return new PlanarCoordinate(resultX, resultY);
            }
        }


        // For now we require one tie point, and a valid pixel scale.
        
        else if (count >= 3 && tiepoint_count >= 6)
        {
            double resultX = (x - tiepoints[3]) / pixel_scale[0] + tiepoints[0];
            double resultY = (y - tiepoints[4]) / (-1 * pixel_scale[1]) + tiepoints[1];
            return new PlanarCoordinate(resultX, resultY);
        }


        return null;
    }
    
    
    /**
     * Invert a 6 term geotransform style matrix.
     *
     * @param in The matrix to be inverted. This is an array of doubles
     *           containing the first and the second line of the matrix. The
     *           third line is assumed to be [0 0 1]. That is, the input matrix
     *           is:
     *
     *   [element_0 element_1 element_2]
     *   [element_3 element_4 element_5]
     *   [    0         0         1    ]
     *
     * @return <code>null</code> if the matrix is singular, otherwise the
     *         inverted matrix.
     */
    private static double[] gtMatrixInverse(double[] in)
    {
        double determinate = in[0] * in[4] - in[1] * in[3];

        // The input matrix is singular and canno tbe inverted.
        if (java.lang.Math.abs(determinate) < 0.000000000000001)
            return null;

        double invDeterminate = 1.0 / determinate;

        // Compute adjoint, and devide by determinate.

        double[] outMatrix = new double[6];

        outMatrix[0] =  in[4] * invDeterminate;
        outMatrix[3] = -in[3] * invDeterminate;

        outMatrix[1] = -in[1] * invDeterminate;
        outMatrix[4] =  in[0] * invDeterminate;

        outMatrix[2] = ( in[1] * in[5] - in[2] * in[4]) * invDeterminate;
        outMatrix[5] = (-in[0] * in[5] + in[2] * in[3]) * invDeterminate;

        return outMatrix;
    }


    // A TIFF tag directory containing the GeoTIFF tags.
    private GeoTIFFDirectory directory_;
}

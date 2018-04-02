package com.marjaniemi.chekov;

import java.lang.Math;
import org.geotools.pt.CoordinatePoint;


public class PlanarCoordinate
{
    private double x_, y_;


    public PlanarCoordinate(double x, double y)
    {
	x_ = x;
	y_ = y;
    }


    public PlanarCoordinate(CoordinatePoint cp)
    {
	assert cp.getDimension() == 2;

	x_ = cp.getOrdinate(0);
	y_ = cp.getOrdinate(1);
    }


    // Get ordinate.
    public double getX() { return x_; }
    public double getY() { return y_; }


    public void add(PlanarCoordinate p)
    {
	x_ += p.x_;
	y_ += p.y_;
    }

    
    public void sub(PlanarCoordinate p)
    {
	x_ -= p.x_;
	y_ -= p.y_;
    }

    
    public void mul(double d)
    {
	x_ *= d;
	y_ *= d;
    }

    
    public void div(double d)
    {
	x_ /= d;
	y_ /= d;
    }

    
    public static double distance(PlanarCoordinate p1, PlanarCoordinate p2)
    {
	double dx = Math.abs(p1.x_ - p2.x_);
	double dy = Math.abs(p1.y_ - p2.y_);
	return Math.sqrt(dx * dx + dy * dy);
    }
}

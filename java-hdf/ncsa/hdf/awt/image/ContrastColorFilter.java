/****************************************************************************
 * NCSA HDF                                                                 *
 * National Comptational Science Alliance                                   *
 * University of Illinois at Urbana-Champaign                               *
 * 605 E. Springfield, Champaign IL 61820                                   *
 *                                                                          *
 * For conditions of distribution and use, see the accompanying             *
 * hdf/COPYING file.                                                        *
 *                                                                          *
 ****************************************************************************/

package ncsa.hdf.awt.image;


import   java.awt.*;
import java.awt.image.*;

public class ContrastColorFilter extends RGBImageFilter {

	double factor=1.0d;

	boolean redMask   = false;
	boolean greenMask = false;
	boolean blueMask  = false;

    public ContrastColorFilter() {

	canFilterIndexColorModel = true;	
    }

    public ContrastColorFilter(double f) {

	this();
	this.factor = f;	
    }

    public void setFactor(double f) {
	
	this.factor = f;	
    }

    public double getFactor() {
	
	return this.factor;
    }

   public int range(int a, int b, int c) {
	
	// set a to b & c
	if (a<b) 
	   a = b;

	if (a>c)
	   a = c;
	  
	return a;    
    }

 

    // get a separate RGB value(r,g,b) , then multiply the factor for the new RGB
    public int modify( int val, int shifts) {

	double d;
 	int retVal;
	int mask = 0x000000ff << shifts;
	d = (double)((val & mask) >> shifts);  // separate RGB value
	d *= factor;

	retVal = (int)d;
	retVal = range(retVal,0,255)<< shifts;

        return retVal;
   }

  public int filterRGB( int x, int y, int rgb) {
                
	int alpha = rgb & 0xff000000;   // (rgb >> 24) & 0xff;
    
	int red   = (rgb ) & 0x00ff0000;
        int green = (rgb ) & 0x0000ff00;
        int blue  = (rgb ) & 0x000000ff;
 
	if (!redMask) 
           red   = modify( rgb,16 );

	if (!greenMask) 
	   green = modify( rgb, 8 );

	if (!blueMask) 
	   blue  = modify( rgb, 0 );

 	return (alpha | red | green | blue);
  }

  public  void setFilterMask(boolean redMask, boolean greenMask, 
			     boolean blueMask) {

	this.redMask   = redMask;
	this.greenMask = greenMask;
	this.blueMask  = blueMask;
  }
}

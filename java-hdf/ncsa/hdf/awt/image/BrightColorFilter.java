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

package  ncsa.hdf.awt.image;

import java.awt.image.*;

/**
 * An image filter to highlight an image by brightening or darkening
 * the pixels in the images.
 *
 */
public class BrightColorFilter extends RGBImageFilter {
    boolean 	brighter;
    int 	percent;

    boolean redMask   = false;
    boolean greenMask = false;
    boolean blueMask  = false;

    public BrightColorFilter(boolean b, int p) {
	brighter = b;
	percent  = p;
	canFilterIndexColorModel = true;
    }

     public int range(int a, int b, int c) {
	
	// set a to b & c
	if (a<b) 
	   a = b;

	if (a>c)
	   a = c;
	  
	return a;    
    }

 

 

 

 public void imageComplete(int status) {

	super.imageComplete(status);
 }

  public  void setFilterMask(boolean redMask, boolean greenMask, 
			     boolean blueMask) {

	this.redMask   = redMask;
	this.greenMask = greenMask;
	this.blueMask  = blueMask;
  }

    public int filterRGB(int x, int y, int rgb) {

	int r = (rgb >> 16) & 0xff;
	int g = (rgb >> 8) & 0xff;
	int b = (rgb >> 0) & 0xff;
	if (brighter) {
	    if (!redMask)
	       r = (255 - ((255 - r) * (100 - percent) / 100));
	    if (!greenMask)
	       g = (255 - ((255 - g) * (100 - percent) / 100));
	    if (!blueMask)
	       b = (255 - ((255 - b) * (100 - percent) / 100));
	} else {
 	    if (!redMask)
	       	r = (r * (100 - percent) / 100);
 	    if (!greenMask)
	    	g = (g * (100 - percent) / 100);
	    if (!blueMask)
	    	b = (b * (100 - percent) / 100);
	}
	
	// valid
	r = range(r, 0 , 255);
	g = range(g, 0 , 255);
	b = range(b, 0 , 255);

	return (rgb & 0xff000000) | (r << 16) | (g << 8) | (b << 0);
    }
}

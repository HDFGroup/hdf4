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

import java.util.Hashtable;
import java.awt.*;
import java.awt.image.*;

public class FlipFilter extends ImageFilter {
	
	// flip direction 
	int	direction;

	// default color model
	private static ColorModel defaultRGB = ColorModel.getRGBdefault();

	// pixel value
	private int raster[] = null;

	// width & height
	private int imageWidth, imageHeight;

	// flip direction
	public static final int HORIZONTAL = 0;
	public static final int VERTICAL   = 1;

    public FlipFilter(int direction) {

	this.direction = range(direction,HORIZONTAL, VERTICAL);
	
    }

    public int range(int a, int b, int c) {
	
	// set a to b & c
	if (a<b) 
	   a = b;

	if (a>c)
	   a = c;
	  
	return a;    
    }

 

    public float range(float a, float b, float  c) {
	
	// set a to b & c
	if (a<b) 
	   a = b;

	if (a>c)
	   a = c;
	  
	return a;    
    }

 

 public void setDimensions(int width, int height) {
	
	this.imageWidth = width;
	this.imageHeight= height;

	// specify the raster
	if (raster == null)
	   raster = new int[imageWidth*imageHeight];

	consumer.setDimensions(width, height);

    }

 

 public void setProperties(Hashtable props) {
	props = (Hashtable) props.clone();
	Object o = props.get("filters");
	if (o == null) {
	    props.put("filters", toString());
	} else if (o instanceof String) {
	    props.put("filters", ((String) o)+toString());
	}
	consumer.setProperties(props);
    }

 

 public void setHints(int hintflags) {
	consumer.setHints(TOPDOWNLEFTRIGHT
			  | COMPLETESCANLINES
			  | SINGLEPASS
			  | (hintflags & SINGLEFRAME));
    }

  public void setColorModel_old(ColorModel model) {

	consumer.setColorModel(defaultRGB);
	// consumer.setColorModel(model);
    }

 public void setPixels(int x, int y, int w, int h, ColorModel model,
			  byte pixels[], int off, int scansize) {

	int srcoff = off;
	int dstoff = y * imageWidth + x;
	for (int yc = 0; yc < h; yc++) {
	    for (int xc = 0; xc < w; xc++) {
		raster[dstoff++] = model.getRGB(pixels[srcoff++] & 0xff);
	    }

	    srcoff += (scansize - w);
	    dstoff += (imageWidth - w);
	}

	

    }

 public void setPixels(int x, int y, int w, int h, ColorModel model,
                          int pixels[], int off, int scansize) {

        int srcoff = off;
        int dstoff = y * imageWidth + x;

        for (int yc = 0; yc < h; yc++) {
            for (int xc = 0; xc < w; xc++) {
                    raster[dstoff++] = model.getRGB(pixels[srcoff++]);
            }
            srcoff += (scansize - w);
            dstoff += (imageWidth - w);
        }

    }

 public void imageComplete(int status) {

	if (status == IMAGEERROR || status == IMAGEABORTED) {
	    consumer.imageComplete(status);
	    return;
	}

	int pixels[] = new int[imageWidth];

	for (int y = 0; y < imageHeight; y++) {
	
	    if (direction == VERTICAL ) { // vertical way ...
	    	// make a copy
		int pos = (imageHeight-1-y)*imageWidth;
		for (int kk=0; kk<imageWidth; kk++)
		    pixels[kk] = raster[pos+kk];
	     }
	     else {
		// make a copy
		int pos = y*imageWidth;
		for (int kk=0; kk<imageWidth; kk++)
		    pixels[kk] = raster[pos+kk];

		for (int  kk=0; kk<imageWidth/2; kk++) {

			int tmp     = pixels[kk];
			pixels[kk]  = pixels[imageWidth-kk-1];
			pixels[imageWidth-kk-1] = tmp;
		}
	    }

	    // consumer it ....
	    consumer.setPixels(0, y, imageWidth, 1, defaultRGB, pixels, 0, imageWidth);
	}

	// complete ?
	consumer.imageComplete(status);

    }
}

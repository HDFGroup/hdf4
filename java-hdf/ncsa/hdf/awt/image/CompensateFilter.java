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

import java.awt.Image;
import java.awt.image.*;
import java.util.Hashtable;

// Filter which generate another image based on the background 
public class CompensateFilter extends ImageFilter {

    private static ColorModel defaultRGB = ColorModel.getRGBdefault();

    // image operation
    public final static int ADD      = 1;
    public final static int SUBTRACT = 2;

    int		operation;

    private int bgRaster[];
    private int raster[];

    // width & height
    private int imageWidth, imageHeight;

    // default constructor
    public CompensateFilter(int[] raster, int op) {
	this.bgRaster  = raster;	
	this.operation = op;
    }

  public int range(int a, int b, int c) {
	
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

    public void setColorModel(ColorModel model) {
	consumer.setColorModel(defaultRGB);
    }

    public void setHints(int hintflags) {
	consumer.setHints(TOPDOWNLEFTRIGHT
			  | COMPLETESCANLINES
			  | SINGLEPASS
			  | (hintflags & SINGLEFRAME));
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

	    int dstoff = 0;

	    for (int x=0; x < imageWidth; x++) {

		int rgb;
	
		// compute weighted average for pixel value 
		if (operation == ADD)  // pixel = (p1 + p2)
		   rgb = (bgRaster[y*imageWidth+x] + raster[y*imageWidth+x]);
		else
		   rgb = (bgRaster[y*imageWidth+x] - raster[y*imageWidth+x]);

        	int red   = (rgb >> 16) & 0xff;
        	int green = (rgb >>  8) & 0xff;
        	int blue  = (rgb      ) & 0xff;
 
		red   = range(red,    0,255);
		green = range(green,  0,255);
		blue  = range(blue,   0,255);
	
		rgb = ((255 << 24) | (red << 16) | (green<< 8) | (blue));
		
		pixels[dstoff++] =  rgb ; 

	    } //  for (int x=0; x < imageWidth; x++) {		
	    consumer.setPixels(0, y, imageWidth, 1, defaultRGB, pixels, 0, imageWidth);
	}  // for (int y = 0; y < imageHeight; y++) {
	
	consumer.imageComplete(status);
    }
}

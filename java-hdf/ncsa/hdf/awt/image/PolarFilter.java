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

/** This class implement the another kind type of image filter - that will
 *  display your data in polar coordinates (r, theta).
 *  The algorithm was excepted  from NCSA's ximage.
 */

public class PolarFilter extends ImageFilter {
	
	// default color model
	private static ColorModel defaultRGB = ColorModel.getRGBdefault();

	// pixel value
	private int raster[] = null;

	// width & height
	private int imageWidth, imageHeight;

	// curent image width & height
	private int  width, height;

    public PolarFilter() {

	
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

	consumer.setDimensions(height, height);

	// set current image width & height
	this.width = this.height = height;

    }

 

 public int getImageWidth() {

	return width;
 } 

 public int getImageHeight() {

	return height;
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

  public void setColorModel(ColorModel model) {
	consumer.setColorModel(defaultRGB);
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
                    raster[dstoff++] = model.getRGB(pixels[srcoff++] );
            }
            srcoff += (scansize - w);
            dstoff += (imageWidth - w);
        }

    }

 public void imageComplete_1(int status) {

	// Cartesian -> Polar ...
	if (status == IMAGEERROR || status == IMAGEABORTED) {
	    consumer.imageComplete(status);
	    return;
	}

	int pixels[] = new int[imageHeight];

	long	rscope = (long)imageHeight*imageHeight;

	// scan line ...
	int line = 0;

	for (int y = 0; y < 2*imageHeight; y += 2 ) {
	   
	    long  ys = (y-imageHeight) * (y-imageHeight)  ;

	    for (int x=0; x<2*imageHeight; x += 2) {
		// int  dstoff = x + 2*imageHeight*y;
		// trun to 90c.
		//int dstoff = imageHeight*x/2 + imageHeight - 1 - y/2;
		int dstoff = x/2;
		
		long rad = (long)(x-imageHeight)*(x-imageHeight) + ys;
		if (rad <= rscope) { 
		
		    double radius = Math.sqrt((double)rad);
		    double angle ;

		    if (radius<1.0d) angle = 0.0d; 
		    else 
			angle = Math.acos((double)(x-imageHeight)/radius);
			
		    if (y>imageHeight) angle = 2.0d*Math.PI - angle;
		    
		    double dx = (angle*imageWidth)/(2.0d*Math.PI);
		    int    dy = (int)radius;
			
		    int  pos = (int)((double)dx+(double)imageWidth*dy);
		    pos = range(pos,0,imageHeight*imageWidth-1);

		    pixels[dstoff] = raster[pos];
		}
		else pixels[dstoff] = 0;
	    } // for (int x=0; x<2*imageWidth; x++) {
		    
	    // consumer it ....
	    consumer.setPixels(0, line++, imageHeight, 1, defaultRGB, pixels, 0, imageHeight);
	}  // for (int y = 0; y < imageHeight; y++) {

	// consumer.setPixels(0, 0, imageHeight, imageHeight, defaultRGB, pixels, 0, imageHeight);

	// complete ?
	consumer.imageComplete(status);

    }

 public void imageComplete(int status) {

	// Cartesian -> Polar ...
	if (status == IMAGEERROR || status == IMAGEABORTED) {
	    consumer.imageComplete(status);
	    return;
	}

	int pixels[] = new int[imageHeight*imageHeight];

	long	rscope = (long)imageHeight*imageHeight;

	// scan line ...
	// int line = 0;

	for (int y = 0; y < 2*imageHeight; y += 2 ) {
	   
	    long  ys = (y-imageHeight) * (y-imageHeight)  ;

	    for (int x=0; x<2*imageHeight; x += 2) {
		// int  dstoff = x + 2*imageHeight*y;
		// trun to 90c. [y,x] ==>> [h-1-x,y];
		// int dstoff = imageHeight -1 - y/2 + imageHeight * (imageHeight -1 - x/2);
		int dstoff = (imageHeight -1 - x/2)*imageHeight +  y/2;

		long rad = (long)(x-imageHeight)*(x-imageHeight) + ys;
		if (rad <= rscope) { 
		
		    double radius = Math.sqrt((double)rad);
		    double angle ;

		    if (radius<1.0d) angle = 0.0d; 
		    else 
			angle = Math.acos((double)(x-imageHeight)/radius);
			
		    if (y>imageHeight) angle = 2.0d*Math.PI - angle;
		    
		    double dx = (angle*imageWidth)/(2.0d*Math.PI);
		    int    dy = (int)radius;
			
		    int  pos = (int)((double)dx+(double)imageWidth*dy);
		    pos = range(pos,0,imageHeight*imageWidth-1);

		    pixels[dstoff] = raster[pos];
		}
		else pixels[dstoff] = 0;
	    } // for (int x=0; x<2*imageWidth; x++) {
		    
	    // consumer it ....
	    // consumer.setPixels(0, line++, imageHeight, 1, defaultRGB, pixels, 0, imageHeight);
	}  // for (int y = 0; y < imageHeight; y++) {

	// consumer.setPixels(0, 0, imageHeight, imageHeight, defaultRGB, pixels, 0, imageHeight);

	int line[] = new int[imageHeight];
	for (int y = 0; y < imageHeight; y++) {
		    
	    for (int x=0; x < imageHeight; x++) 
		line[x] = pixels[y*imageHeight+x];	

	    // consumer it ....
	    consumer.setPixels(0, y, imageHeight, 1, defaultRGB, line, 0, imageHeight);
	}  
	// complete ?
	consumer.imageComplete(status);

    }
}

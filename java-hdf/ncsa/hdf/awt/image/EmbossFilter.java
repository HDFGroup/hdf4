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
import java.awt.image.*;

/** This class implement the image filter that can emboss image */
public class EmbossFilter extends ImageFilter {

	// default color model
	private  ColorModel defaultRGB = ColorModel.getRGBdefault();
	
	// pixel value
	private int raster[];

	// width & height
	private int imageWidth, imageHeight;

    public EmbossFilter() {
	
	
    }

  public int range(int a, int b, int c) {
	
	// set a to b & c
	if (a<b) 
	   a = b;

	if (a>c)
	   a = c;
	  
	return a;    
    }

  public int monoColorValue(int rgb) {

	int retVal;

	// get separate value
	int alpha = (rgb >> 24) & 0xff ;
        int red   = (rgb >> 16) & 0xff ;
        int green = (rgb >>  8) & 0xff ;
        int blue  = (rgb      ) & 0xff ;  
	
	retVal = (red * 11 + green * 16 + blue * 5 ) >> 5;
	retVal = range(retVal,0,255);	

	retVal = ((alpha << 24) | (retVal << 16) | (retVal << 8) | (retVal));
	
	return retVal;
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

   public void setColorModel(ColorModel model) {

	consumer.setColorModel(defaultRGB);

    }

   public void setPixels(int x, int y, int w, int h, ColorModel model,
			  byte pixels[], int off, int scansize) {

	int dstoff = y*imageWidth+x;
	int srcoff = off;

	for (int yc = 0; yc < h; yc++) {
	    for (int xc = 0; xc < w; xc++) {

		int rgb = model.getRGB(pixels[srcoff++]&0xff);
		// int rgb = pixels[srcoff++]&0xff ;

   		// save ...
		raster[dstoff++] = rgb;
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
	
		int rgb = model.getRGB(pixels[srcoff++]);
		// int rgb = pixels[srcoff++];

   		// save ...
		raster[dstoff++] = rgb;
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

	// first line
	System.arraycopy(pixels,0,raster,0,imageWidth);
	// draw it ....
	consumer.setPixels(0, 0, imageWidth, 1, defaultRGB, pixels, 0, imageWidth);

	for (int y = 1; y < imageHeight-1; y++) {
	
	    // make a copy
	    // System.arraycopy(pixels,0,raster,y*imageWidth,imageWidth);
	    int pos = y*imageWidth;
	    pixels[0]            = monoColorValue(raster[pos]);
	    pixels[imageWidth-1] = monoColorValue(raster[pos + imageWidth -1 ]);

	    int dstoff = 1;	
	    int ind = y*imageWidth + 1;

	    for (int x=1; x < imageWidth-1; x++, ind++) {
			
		int rsum = 0;
		int gsum = 0;
		int bsum = 0;
		// mask code 
		/*        -2   -1   0
			  -1    0   1
     		 	   0    1   2     */
		// compute weighted average for pixel value 
		// top left
		int rgb = raster[ind-imageWidth-1]; 
      		// int alpha = (rgb >> 24) & 0xff;
        	int red   = (rgb >> 16) & 0xff;
        	int green = (rgb >>  8) & 0xff;
        	int blue  = (rgb      ) & 0xff;
 
		rsum -= red*2;
		gsum -= green*2;
		bsum -= blue*2;

		// top mid
		rgb = raster[ind-imageWidth];
      		// alpha = (rgb >> 24) & 0xff;
        	red   = (rgb >> 16) & 0xff;
        	green = (rgb >>  8) & 0xff;
        	blue  = (rgb      ) & 0xff;

		rsum -= red;
		gsum -= green;
		bsum -= blue;

		// mid left
		rgb = raster[ind-1];
        	red   = (rgb >> 16) & 0xff;
        	green = (rgb >>  8) & 0xff;
        	blue  = (rgb      ) & 0xff;

		rsum -= red;
		gsum -= green;
		bsum -= blue;

		// mid right
		rgb = raster[ind+1];
        	red   = (rgb >> 16) & 0xff;
        	green = (rgb >>  8) & 0xff;
        	blue  = (rgb      ) & 0xff;

		rsum += red;
		gsum += green;
		bsum += blue;

		// bottom mid
		rgb = raster[ind+imageWidth];
        	red   = (rgb >> 16) & 0xff;
        	green = (rgb >>  8) & 0xff;
        	blue  = (rgb      ) & 0xff;

		rsum += red;
		gsum += green;
		bsum += blue;

		// bottom right
		rgb = raster[ind+imageWidth+1];
        	red   = (rgb >> 16) & 0xff;
        	green = (rgb >>  8) & 0xff;
        	blue  = (rgb      ) & 0xff;

		rsum += red  *2 ;
		gsum += green*2;
		bsum += blue *2 ;

		rsum  /= 8;
		gsum  /= 8;
		bsum  /= 8;

		/****************************
		rgb = raster[ind];
		alpha = (rgb >> 24) & 0xff;
		red   = (rgb >> 16) & 0xff;
        	green = (rgb >>  8) & 0xff;
        	blue  = (rgb      ) & 0xff;
		*****************************/

		// add gray128
		rsum = range((rsum+128),  0,255);
		gsum = range((gsum+128),  0,255);
		bsum = range((bsum+128),  0,255);
	
		rgb = ((255 << 24) | (rsum << 16) | (gsum<< 8) | (bsum));

		pixels[dstoff++] =  rgb ; // monoColorValue(rgb);

	    } 
	    consumer.setPixels(0, y, imageWidth, 1, defaultRGB, pixels, 0, imageWidth);
	} 

	// last line
	System.arraycopy(pixels,0,raster,imageWidth*(imageHeight-1),imageWidth);

	// draw it ....
	consumer.setPixels(0, imageHeight-1, imageWidth, 1, defaultRGB, pixels, 0, imageWidth);

	consumer.imageComplete(status);
    }
}

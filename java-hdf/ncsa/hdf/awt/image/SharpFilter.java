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

public class SharpFilter extends ImageFilter {

	int 	sharpFactor = 3;

	// default color model
	private static ColorModel defaultRGB = ColorModel.getRGBdefault();

	// pixel value
	private int raster[];

	// width & height
	private int imageWidth, imageHeight;

    public SharpFilter(int n) {

	this.sharpFactor = n;

	// odd ?
	if (((n/2) * 2) == n)  // even
	   this.sharpFactor = n + 1;
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

 public void imageComplete(int status) {

	if (status == IMAGEERROR || status == IMAGEABORTED) {
	    consumer.imageComplete(status);
	    return;
	}

	int pixels[] = new int[imageWidth];

	double factor = sharpFactor / 100.0;
	double ifactor = 1.0 - factor;
	
	if (imageWidth * imageHeight < 10) return; // too small image

	float[] line0  = new float[imageWidth];
	float[] linep1 = new float[imageWidth];
	float[] linem1 = new float[imageWidth];
	
	float[] hsbvalue = new float[3];

	// compute line 0
	int ind = 0;
	for (int x=0; x<imageWidth; x++) {
		
		int rgb = raster[ind++];

		Color.RGBtoHSB( (rgb >> 16) & 0xff, // red
				(rgb >> 8 ) & 0xff, // green
				(rgb      ) & 0xff, // blue
				hsbvalue);

		line0[x] =  hsbvalue[2]; // save bright value
	}

	// compute line 1
	ind = imageWidth;
	for (int x=0; x<imageWidth; x++) {
		
		int rgb = raster[ind++];
		Color.RGBtoHSB( (rgb >> 16) & 0xff, // red
				(rgb >> 8 ) & 0xff, // green
				(rgb      ) & 0xff, // blue
				hsbvalue);

		linep1[x] =  hsbvalue[2]; // save bright value
	}

	// first line
	System.arraycopy(pixels,0,raster,0,imageWidth);

	// draw it ....
	consumer.setPixels(0, 0, imageWidth, 1, defaultRGB, pixels, 0, imageWidth);

	float[] tmp = new float[imageWidth];
	for (int y = 1; y < imageHeight-1; y++) {
	
	    // make a copy
	    // System.arraycopy(pixels,0,raster,y*imageWidth,imageWidth);
	    int pos = y*imageWidth;
	    pixels[0]            = raster[pos];
	    pixels[imageWidth-1] = raster[pos + imageWidth -1 ];

	    int dstoff = 1;
		
	    //tmp    = linem1;
	    System.arraycopy(linem1,0,tmp,0,imageWidth);

	    // linem1 = line0;
	    System.arraycopy(line0,0,linem1,0,imageWidth);

	    // line0  = linep1;
	    System.arraycopy(linep1,0,line0,0,imageWidth);

	    //linep1 = tmp;
	    System.arraycopy(tmp,0,linep1,0,imageWidth);

	    // compute line (next line)
	    ind = (y+1)*imageWidth;

	    for (int x=0; x<imageWidth; x++) {
		
		int rgb = raster[ind++];

		Color.RGBtoHSB( (rgb >> 16) & 0xff, // red
				(rgb >> 8 ) & 0xff, // green
				(rgb      ) & 0xff, // blue
				hsbvalue);

		linep1[x] =  hsbvalue[2]; // save bright value

	    } //  for (int x=0; x<imageWidth; x++) {

	    ind = y*imageWidth + 1;
	    for (int x=1; x < imageWidth-1; x++) {
		
		float  sum = 0.0f;
		int    i   =   x;

		sum = linem1[i-1] + linem1[i] + linem1[i+1] +  // current line
		      line0[i-1]  + line0[i]  + line0[i+1]  +  // last first line
		      linep1[i-1] + linep1[i] + linep1[i+1];  // last second line

		int rgb = raster[ind++];
		Color.RGBtoHSB( (rgb >> 16) & 0xff, // red
				(rgb >> 8 ) & 0xff, // green
				(rgb      ) & 0xff, // blue
				hsbvalue);

		float bval = (float)((hsbvalue[2] - (factor * sum) / 9 ) / ifactor);

		bval = range(bval,0.0f,1.0f);
		
		rgb = Color.HSBtoRGB(hsbvalue[0], hsbvalue[1], bval);

		pixels[dstoff++] = rgb;

	    } 
	    consumer.setPixels(0, y, imageWidth, 1, defaultRGB, pixels, 0, imageWidth);
	}  

	// last line
	System.arraycopy(pixels,0,raster,imageWidth*(imageHeight-1),imageWidth);

	// draw it ....
	consumer.setPixels(0, imageHeight-1, imageWidth, 1, defaultRGB, pixels, 0, imageWidth);

	// complete ?
	consumer.imageComplete(status);

    }
}

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

/** This class implement the image filter that can detect the  image edge */
public class EdgeFilter extends ImageFilter {

	// default color model
	private  ColorModel defaultRGB = ColorModel.getRGBdefault();
	
	// pixel value
	private int raster[];

	// width & height
	private int imageWidth, imageHeight;

    public EdgeFilter() {
	
	
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
	    pixels[0]            = (raster[pos]);
	    pixels[imageWidth-1] = (raster[pos + imageWidth -1 ]);

	    int dstoff = 1;	
	    int ind = y*imageWidth + 1;

	    for (int x=1; x < imageWidth-1; x++, ind++) {
			
		int rsum = 0;
		int gsum = 0;
		int bsum = 0;
	
		int a = 0;
		int b = 0;
		int c = 0;
		int d = 0;
		
		int br =  raster[ind+imageWidth+1];  // bottom right
		int bl =  raster[ind+imageWidth-1];  // bottom left
		int bm =  raster[ind+imageWidth];    // bottom mid

		int tr =  raster[ind-imageWidth+1];  // top right
		int tl =  raster[ind-imageWidth-1];  // top left
		int tm =  raster[ind-imageWidth];    // top mid

		int mr =  raster[ind +1];  // mid right
		int ml =  raster[ind -1];  // mid left
		int mm =  raster[ind   ];    // mid mid

		// RED plane
		// bottom right - top left
		a = ((br  >>   16) & 0xff) - ((tl  >>   16) & 0xff);

		// mid right - mid left
		b = ((mr  >>   16) & 0xff) - ((ml  >>   16) & 0xff);

		// top right - bottom left
		c = ((tr  >>   16) & 0xff) - ((bl  >>   16) & 0xff);

		// top mid - bottom  mid
		d = ((tm  >>   16) & 0xff) - ((bm  >>   16) & 0xff);

		rsum = a + b + c; // horizontal gradient
		rsum = Math.abs(rsum);

		a = a -c -d;
		a = Math.abs(a);

		rsum = Math.max(a,rsum)/3;

		// Green Plane .....
		// bottom right - top left
		a = ((br  >>   8) & 0xff) - ((tl  >>   8) & 0xff);

		// mid right - mid left
		b = ((mr  >>   8) & 0xff) - ((ml  >>   8) & 0xff);

		// top right - bottom left
		c = ((tr  >>   8) & 0xff) - ((bl  >>   8) & 0xff);

		// top mid - bottom  mid
		d = ((tm  >>   8) & 0xff) - ((bm  >>   8) & 0xff);

		gsum = a + b + c; // horizontal gradient
		gsum = Math.abs(gsum);

		a = a -c -d;      // vertical gradient
		a = Math.abs(a);

		gsum = Math.max(a,gsum)/3;

		// Blue Plane ...
		// bottom right - top left
		a = ((br) & 0xff) - ((tl) & 0xff);

		// mid right - mid left
		b = ((mr) & 0xff) - ((ml) & 0xff);

		// top right - bottom left
		c = ((tr) & 0xff) - ((bl) & 0xff);

		// top mid - bottom  mid
		d = ((tm) & 0xff) - ((bm) & 0xff);

		bsum = a + b + c; // horizontal gradient
		bsum = Math.abs(bsum);

		a = a -c -d;      // vertical gradient
		a = Math.abs(a);

		bsum = Math.max(a,bsum)/3;
		
		int rgb = ((255 << 24) | (rsum << 16) | (gsum << 8) | (bsum));

		pixels[dstoff++] =  rgb ; //monoColorValue(rgb);

	    } 
	    consumer.setPixels(0, y, imageWidth, 1, defaultRGB, pixels, 0, imageWidth);
	}  

	// last line
	System.arraycopy(pixels,0,raster,imageWidth*(imageHeight-1),imageWidth);

	// draw it ....
 	consumer.setPixels(0, imageHeight-1, imageWidth, 1, defaultRGB, pixels, 0, imageWidth);

	consumer.imageComplete(status);

    }

   public void imageComplete_normalize(int status) {

	if (status == IMAGEERROR || status == IMAGEABORTED) {
	    consumer.imageComplete(status);
	    return;
	}

	int[] retVal = new int[imageWidth*imageHeight];
	int index = 0;

	int pixels[] = new int[imageWidth];

	// first line
	System.arraycopy(pixels,0,raster,0,imageWidth);
	// draw it ....
//	consumer.setPixels(0, 0, imageWidth, 1, defaultRGB, pixels, 0, imageWidth);

for (int kk =0; kk<imageWidth; kk++)
	retVal[index++] = monoColorValue(pixels[kk]);

	for (int y = 1; y < imageHeight-1; y++) {
	
	    // make a copy
	    // System.arraycopy(pixels,0,raster,y*imageWidth,imageWidth);
	    int pos = y*imageWidth;
	    pixels[0]            = (raster[pos]);
	    pixels[imageWidth-1] = (raster[pos + imageWidth -1 ]);


	    int dstoff = 1;	
	    int ind = y*imageWidth + 1;

	    for (int x=1; x < imageWidth-1; x++, ind++) {
			
		int rsum = 0;
		int gsum = 0;
		int bsum = 0;
	
		int a = 0;
		int b = 0;
		int c = 0;
		int d = 0;
		
		int br =  raster[ind+imageWidth+1];  // bottom right
		int bl =  raster[ind+imageWidth-1];  // bottom left
		int bm =  raster[ind+imageWidth];    // bottom mid

		int tr =  raster[ind-imageWidth+1];  // top right
		int tl =  raster[ind-imageWidth-1];  // top left
		int tm =  raster[ind-imageWidth];    // top mid

		int mr =  raster[ind +1];  // mid right
		int ml =  raster[ind -1];  // mid left
		int mm =  raster[ind   ];    // mid mid

		// RED plane
		// bottom right - top left
		a = ((br  >>   16) & 0xff) - ((tl  >>   16) & 0xff);

		// mid right - mid left
		b = ((mr  >>   16) & 0xff) - ((ml  >>   16) & 0xff);

		// top right - bottom left
		c = ((tr  >>   16) & 0xff) - ((bl  >>   16) & 0xff);

		// top mid - bottom  mid
		d = ((tm  >>   16) & 0xff) - ((bm  >>   16) & 0xff);

		rsum = a + b + c; // horizontal gradient
		rsum = Math.abs(rsum);

		a = a -c -d;
		a = Math.abs(a);

		rsum = Math.max(a,rsum)/3;

		// Green Plane .....
		// bottom right - top left
		a = ((br  >>   8) & 0xff) - ((tl  >>   8) & 0xff);

		// mid right - mid left
		b = ((mr  >>   8) & 0xff) - ((ml  >>   8) & 0xff);

		// top right - bottom left
		c = ((tr  >>   8) & 0xff) - ((bl  >>   8) & 0xff);

		// top mid - bottom  mid
		d = ((tm  >>   8) & 0xff) - ((bm  >>   8) & 0xff);

		gsum = a + b + c; // horizontal gradient
		gsum = Math.abs(gsum);

		a = a -c -d;      // vertical gradient
		a = Math.abs(a);

		gsum = Math.max(a,gsum)/3;

		// Blue Plane ...
		// bottom right - top left
		a = ((br) & 0xff) - ((tl) & 0xff);

		// mid right - mid left
		b = ((mr) & 0xff) - ((ml) & 0xff);

		// top right - bottom left
		c = ((tr) & 0xff) - ((bl) & 0xff);

		// top mid - bottom  mid
		d = ((tm) & 0xff) - ((bm) & 0xff);

		bsum = a + b + c; // horizontal gradient
		bsum = Math.abs(bsum);

		a = a -c -d;      // vertical gradient
		a = Math.abs(a);

		bsum = Math.max(a,bsum)/3;
		
		int rgb = ((255 << 24) | (rsum << 16) | (gsum << 8) | (bsum));

		pixels[dstoff++] =  rgb ; //monoColorValue(rgb);

	    } 

for (int kk =0; kk<imageWidth; kk++)
	retVal[index++] = monoColorValue(pixels[kk]);
	
//	    consumer.setPixels(0, y, imageWidth, 1, defaultRGB, pixels, 0, imageWidth);
	}  

	// last line
	System.arraycopy(pixels,0,raster,imageWidth*(imageHeight-1),imageWidth);
for (int kk =0; kk<imageWidth; kk++)
	retVal[index++] = monoColorValue(pixels[kk]);

	// draw it ....
//	consumer.setPixels(0, 0, imageWidth, 1, defaultRGB, pixels, 0, imageWidth);

// find max value
int maxVal = retVal[0];
for (int kk=1; kk<imageWidth*imageHeight; kk++)
	maxVal = Math.max(maxVal, retVal[kk]);

if (maxVal > 0) {

for (int kk=1; kk<imageWidth*imageHeight; kk++) {
	int rgb = retVal[kk];
	int alpha = (rgb >> 24) & 0xff;
        int red   = ((rgb >> 16) & 0xff) * 255 / maxVal;
        int green = ((rgb >>  8) & 0xff) * 255 / maxVal;
        int blue  = ((rgb      ) & 0xff) * 255 / maxVal;

	red = range(red,0,255);
	green = range(green,0,255);
	blue = range(blue,0,255);

	rgb = (alpha << 24) | (red << 16) | (green << 8) | (blue);
 	retVal[kk] = rgb;
 }
}
consumer.setPixels(0, 0, imageWidth, imageHeight, defaultRGB, retVal, 0, imageWidth);


	consumer.imageComplete(status);
    }
}

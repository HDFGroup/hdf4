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

/** This class implement the image filter that can make image smooth(blur?) */
public class BlurFilter extends ImageFilter {

	int 	blurFactor = 3;

	// default color model
	private static ColorModel defaultRGB = ColorModel.getRGBdefault();

	// pixel value
	private int raster[];

	// width & height
	private int imageWidth, imageHeight;

    public BlurFilter() {

	
    }

    public BlurFilter(int n) {

	this.blurFactor = n;

	// odd ?
	if (((n/2) * 2) == n)  // even
	   this.blurFactor = n + 1;
	
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

	//  how many pixel value will   be selected
	int n = blurFactor / 2;

	// each line
	for (int y = 0; y < imageHeight; y++) {
	
	    int dstoff = 0;
	    // each pixel in specified line
	    for (int x=0; x < imageWidth; x++) {
		
		int rsum = 0;
		int gsum = 0;
		int bsum = 0;
		int count= 0;

		// construct pixel value, look like, n=3
	        /*    x  x  x  x  x
		      x  -------  x
		      x  |  v  |  x 
		      x  -------  x
		      x  x  x  x  x
		*****/

		for (int y1=y-n; y1<= y+n; y1++) {

		    if ((y1>=0)&&(y1<imageHeight) ) {
			
			int ind = y1*imageWidth + (x-n);
		
			for ( int x1 = x -n; x1 <= x +n; x1++) {

			    if ((x1>=0) && (x1<imageWidth)) {

				int rgb = raster[ind++];
				count++;
				
				rsum += (rgb >> 16) & 0xff;
				gsum += (rgb >> 8 ) & 0xff;
				bsum += (rgb      ) & 0xff;
			     }
			     else ind++;
			} // for ( int x1 = x -n; x1 <= x +n; x1++) {
		    } // if ((y1>=0)&&(y1<imageHeight) && (x>=n)) {
		} // for (int y1=y-n; y1<= y+n; y1++) {

		rsum /= count;
		gsum /= count;
		bsum /= count;

		rsum =  range(rsum,0,255);
		gsum =  range(gsum,0,255);
		bsum =  range(bsum,0,255);

		pixels[dstoff++] = (255 << 24) | (rsum << 16) | (gsum << 8) | (bsum);
	    } //  for (int x=0; x < imageWidth; x++) {		
	    consumer.setPixels(0, y, imageWidth, 1, defaultRGB, pixels, 0, imageWidth);
	}  // for (int y = 0; y < imageHeight; y++) {

	consumer.imageComplete(status);

    }
}

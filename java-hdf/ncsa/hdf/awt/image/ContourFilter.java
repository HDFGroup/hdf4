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

public class ContourFilter extends ImageFilter {
	
	// contour level
	int	level;

	// the table of the contour levels 
	int	levels[];

	// colors for drawable contour line
	int[]	levelColors = new int[9];

	// default RGB
	private final int maxValue = 255;
	private final int minValue = 000;
	
	// default color model
	private static ColorModel defaultRGB = ColorModel.getRGBdefault();

	// pixel value
	private int raster[] = null;

	// width & height
	private int imageWidth, imageHeight;

    public ContourFilter(int level) {

	// set level;
	this.level = range(level, 1, 9);

	levelColors[0] =    Color.white.getRGB();  // white
	levelColors[1] =    Color.red.getRGB();    // red
	levelColors[2] =    Color.yellow.getRGB(); // yellow
	levelColors[3] =    Color.blue.getRGB();   // blue
	levelColors[4] =    Color.orange.getRGB(); // orange
	levelColors[5] =    Color.green.getRGB();  // green	
	levelColors[6] =    Color.cyan.getRGB();   // cyan
	levelColors[7] =    Color.pink.getRGB();   // pink
	levelColors[8] =    Color.gray.getRGB();   // gray	

	int dx    = 0;
	int count = 0;

	dx  = maxValue - minValue;
	dx /= level;

	//  compute level table
	// for (int x = minValue; x<maxValue; x+=dx, count++);

	// specift the size of levels[]
	levels = new int[level];
	count = 0;

	for (int x = minValue+dx; x<maxValue; x+=dx, count++) 
	    levels[count] = x;
	
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

	// retVal = ((alpha << 24) | (retVal << 16) | (retVal << 8) | (retVal));
	
	return retVal;
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
		raster[dstoff++] = monoColorValue(model.getRGB(pixels[srcoff++] & 0xff));

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
                    raster[dstoff++] = monoColorValue(model.getRGB(pixels[srcoff++] & 0xff));
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

	int pixels[] = new int[imageWidth*imageHeight];

	// for one level
	for (int z=0; z<levels.length; z++) {

		int currentLevel = levels[z];
		int color        = levelColors[z];

		setContourLine(raster, pixels, currentLevel, color, imageWidth, imageHeight);
	}

	int line[] = new int[imageWidth];
	for (int y = 0; y < imageHeight; y++) {
		    
	    for (int x=0; x < imageWidth; x++) 
		line[x] = pixels[y*imageWidth+x];	

	    // consumer it ....
	    consumer.setPixels(0, y, imageWidth, 1, defaultRGB, line, 0, imageWidth);
	}  // for (int y = 0; y < imageHeight; y++) {
	
	// consumer it ....
	// consumer.setPixels(0, 0, imageWidth, imageHeight, defaultRGB, pixels, 0, imageWidth);

	// complete ?
	consumer.imageComplete(status);

    }

    /** draw a contour line based on the current parameter---level, color */
    private void setContourLine(int[] raster, int[] pixels, 
				int level, int color, int w, int h) {

	int p = 0;   // entrance point
	int q = p + (w*h-1);   // bottom right point
	int u = 0 + (w-1);     // top right point

	// first round
	while(true) {
		
		while ( p < u ) {

		   int rgb = raster[p];
		   if (rgb < level) {
			
			while ((raster[p] < level)&&(p < u)) p++;
		   	if (raster[p] >= level) 
			   pixels[p] = color;
		   }
		   else if (rgb == level) {
			
			while ((raster[p] == level)&&(p < u)) p++;
		   	if ((raster[p] < level)  || (raster[p] > level))
			   pixels[p] = color;
			
		   } else { 

			while ((raster[p] > level)&&(p < u)) p++;
		   	if ((raster[p] <= level))
			   pixels[p] = color;
		   }
		}

		if (u == q) break;
		else {
			u += w;
			p++;
		}
	}

	/******************************
	// second round
	p = 0;
	q = p + (w*h-1);
	u = q - (w-1);

	while(u<=q) {

		while ( p < u ) {

		   int rgb = raster[p];
		   if (rgb < level) {
			
			while ((raster[p] < level)&&(p < u)) p++;
		   	if (raster[p] >= level) 
			   pixels[p] = color;
		   }
		   else if (rgb == level) {
			
			while ((raster[p] == level)&&(p < u)) p++;
		   	if ((raster[p] < level)  || (raster[p] > level))
			   pixels[p] = color;
			
		   } else { 

			while ((raster[p] > level)&&(p < u)) p++;
		   	if ((raster[p] <= level))
			   pixels[p] = color;
		   }
		}

		if (u == q) break;
		else {
			u ++;
			p  = (w-1) - (q-u);
		}

	}  // while(u<=q) {
	***********************************/
	
    }
}

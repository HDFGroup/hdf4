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

import java.awt.Color;
import java.awt.image.*;

// This class can adjust the seprate thr RGB value
public class RGBColorFilter extends RGBImageFilter {

	int red = 0;
	int green=0;
	int blue =0;

    public RGBColorFilter() {
	canFilterIndexColorModel = true;
    }

    public RGBColorFilter(int r, int g, int b) {

	this();
	this.red   =r;
	this.green =g;
	this.blue  =b;
    }

   /**
     * Filter an individual pixel in the image by the color mask
     */
    public int filterRGB(int x, int y, int rgb) {

        int a  = (rgb >> 24) & 0xff;
        int r  = (rgb >> 16) & 0xff;
        int g  = (rgb >>  8) & 0xff;
        int b  = (rgb      ) & 0xff;

        r   += red;
        g   += green;
        b   += blue;

        return (a << 24) | (r << 16) | (g << 8) | (b);
    }
}

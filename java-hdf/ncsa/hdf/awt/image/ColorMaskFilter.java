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

public class ColorMaskFilter extends RGBImageFilter {
	Color	color;

  public ColorMaskFilter(Color mask) {

	color = mask;
	canFilterIndexColorModel = true;
    }

  /**
     * Filter an individual pixel in the image by the color mask
     */
    public int filterRGB(int x, int y, int rgb) {

        int alpha = (rgb >> 24) & 0xff;
        int red   = (rgb >> 16) & 0xff;
        int green = (rgb >>  8) & 0xff;
        int blue  = (rgb      ) & 0xff;

        red   *= color.getRed()/255;
        green *= color.getGreen()/255;
        blue  *= color.getBlue()/255;

        return (alpha << 24) | (red << 16) | (green << 8) | (blue);
    }
}

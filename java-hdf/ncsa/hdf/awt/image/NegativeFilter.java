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
import java.awt.image.*;

public class NegativeFilter extends RGBImageFilter {

    public NegativeFilter() {
	canFilterIndexColorModel = true;
    }

    /**
     * Filter an individual pixel in the image 
     */
    public int filterRGB(int x, int y, int rgb) {
        int alpha = (rgb >> 24) & 0xff;
        int red   = (rgb >> 16) & 0xff;
        int green = (rgb >>  8) & 0xff;
        int blue  = (rgb      ) & 0xff;
        red   = 255 - red;
        green = 255 - green;
        blue  = 255 - blue;
        return (alpha << 24) | (red << 16) | (green << 8) | (blue);
    }

 public void imageComplete(int status) {

	super.imageComplete(status);
 }
}

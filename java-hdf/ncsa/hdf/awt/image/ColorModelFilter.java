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

import java.awt.*;
import java.awt.image.*;

// image filter for new RGB setting
public class ColorModelFilter extends RGBImageFilter {

	ColorModel	colorModel;

    // default constructor
    public ColorModelFilter() {
	
	canFilterIndexColorModel = true;
    }

    // constructor by new color model
    public ColorModelFilter(ColorModel cm) {
	this();
	colorModel = cm;
    }

    public int filterRGB(int x,int y,int pixel) {

	// rgb value
	return(255<<24 |
	      (pixel&0xff0000) | (pixel&0xff00) | (pixel&0xff));
	      
    }

    // set new color model
    public void setColorModel(ColorModel model) {

	if (canFilterIndexColorModel && (model instanceof IndexColorModel)) {

		substituteColorModel(model, (IndexColorModel)colorModel);
		consumer.setColorModel((IndexColorModel)colorModel);
	}
	else {
		//  direct color model
		consumer.setColorModel(ColorModel.getRGBdefault());
	}
	
    }
}

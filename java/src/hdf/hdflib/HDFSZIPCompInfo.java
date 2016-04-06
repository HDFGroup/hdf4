/****************************************************************************
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF Java Products. The full HDF Java copyright       *
 * notice, including terms governing use, modification, and redistribution,  *
 * is contained in the file, COPYING.  COPYING can be found at the root of   *
 * the source code distribution tree. You can also access it online  at      *
 * http://www.hdfgroup.org/products/licenses.html.  If you do not have       *
 * access to the file, you may request a copy from help@hdfgroup.org.        *
 ****************************************************************************/

package hdf.hdflib;

/**
 * <p>
 *  This class is a container for the parameters to the HDF
 *  SZIP compression algorithm.
 * <p>
 * In this case, the only parameter is the ``level'' of deflation.
 */


public class HDFSZIPCompInfo extends HDFNewCompInfo {

    public int bits_per_pixel;
    public int options_mask;
    public int pixels;
    public int pixels_per_block;
    public int pixels_per_scanline;

    public HDFSZIPCompInfo() {
        ctype = HDFConstants.COMP_CODE_SZIP;
    }

    public HDFSZIPCompInfo(
    int bits_per_pixel_in,
    int options_mask_in,
    int pixels_in,
    int pixels_per_block_in,
    int pixels_per_scanline_in) {
        ctype = HDFConstants.COMP_CODE_SZIP;
        bits_per_pixel =     bits_per_pixel_in;
        options_mask =     options_mask_in;
        pixels =     pixels_in;
        pixels_per_block =     pixels_per_block_in;
        pixels_per_scanline =     pixels_per_scanline_in;
        
    }
}



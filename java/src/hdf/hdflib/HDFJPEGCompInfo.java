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
 *  JPEG compression class.
 * <p>
 * In this case, the parameters are the quality and baseline.
 */


public class HDFJPEGCompInfo extends HDFOldCompInfo {

    /* Struct to contain information about how to compress */
     /* or decompress a JPEG encoded 24-bit image */

     public int    quality;    /* Quality factor for JPEG compression, should be from */
     /* 0 (terrible) to 100 (very good) */

     public int    force_baseline;     /* If force_baseline is set to TRUE then */
     /* quantization tables are limited to */
     /* 0..255 for JPEG baseline compability */
     /* This is only an issue for quality */
     /* settings below 24 */

    public HDFJPEGCompInfo() {
    	ctype = HDFConstants.COMP_JPEG;
    }

    public HDFJPEGCompInfo(int qual, int fb) {
    	ctype = HDFConstants.COMP_JPEG;
        quality = qual;
        force_baseline = fb;
    }

}

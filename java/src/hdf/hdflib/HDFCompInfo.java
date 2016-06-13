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
 *  comp_info union.
 * <p>
 * The parameters for different compression schemes are
 *  represented as sub-classes of this class
 */


public class HDFCompInfo {
    public int ctype;
    public HDFCompInfo() {
        ctype = HDFConstants.COMP_CODE_NONE;
    } ;
}



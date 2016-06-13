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
 *  This class is a generic container for the parameters to the HDF
 *  compressed classes, with the ``ole'' encoding.
 * <p>
 *  Compression parameters are expressed as instances of sub-classes
 *  of this type.
 */


public class HDFOldCompInfo extends HDFCompInfo {
    public int ctype; /* from COMP_NONE defines */
    public HDFOldCompInfo() {
        ctype = HDFConstants.COMP_NONE;
    } ;
}



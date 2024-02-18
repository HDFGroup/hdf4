/****************************************************************************
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF Java Products. The full HDF Java copyright       *
 * notice, including terms governing use, modification, and redistribution,  *
 * is contained in the file, LICENSE.  LICENSE can be found at the root of   *
 * the source code distribution tree. You can also access it online  at      *
 * http://www.hdfgroup.org/products/licenses.html.  If you do not have       *
 * access to the file, you may request a copy from help@hdfgroup.org.        *
 ****************************************************************************/

package hdf.hdflib;

/**
 * <p>
 * This class is a container for the parameters to the HDF DEFLATION compression algorithm.
 * <p>
 * In this case, the only parameter is the ``level'' of deflation.
 */

public class HDFDeflateCompInfo extends HDFNewCompInfo {
    /** */
    public int level;

    /** */
    public HDFDeflateCompInfo() { ctype = HDFConstants.COMP_CODE_DEFLATE; }

    /** */
    public HDFDeflateCompInfo(int l)
    {
        ctype = HDFConstants.COMP_CODE_DEFLATE;
        level = l;
    }
}

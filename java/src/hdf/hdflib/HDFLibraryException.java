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
 *  <p>
 *  The class HDFException returns errors raised by the HDF
 *  library.
 *  <p>
 *  In principle, this includes any and all errors possible
 *  from the HDF library.  However, most error conditions
 *  are not yet detected in this version of the Java
 *  interface.  This will be added in future releases.
 *
 *  The only HDF library error currently raised are errors
 *  in Hopen, such as ``file not found''.
 */


public class HDFLibraryException extends HDFException 
{

    int HDFerror;
    String msg;

    public HDFLibraryException() {
        HDFerror = 0;
        msg = null;
    }

    public HDFLibraryException(String s) {
        msg = "HDFLibraryException: "+s;
    }

    public HDFLibraryException(int err) {
        HDFerror = err;
    }

    @Override
	public String getMessage() {
        if (msg != null) {
            return msg;
        }

        String s;
        try {
            s = HDFLibrary.HEstring(HDFerror);
        } catch (HDFException e) {
            s = new String("HDF error number: "+HDFerror+", HEstring failed");
        }
        msg = "HDFLibraryException: "+s;
        return msg;
    }
}

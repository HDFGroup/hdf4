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
 *  The class HDFException returns errors from the HDF
 *  library.
 *  <p>
 *  Two sub-classes of HDFException are defined:
 *  <ol>
 *  <li>
 *   HDFLibraryException -- errors raised the HDF library code
 *  <li>
 *   HDFJavaException -- errors raised the HDF Java wrapper code
 *  </ol>
 *  <p>
 *  These exceptions will be sub-classed to represent specific
 *  error conditions, as needed.
 *  <p>
 *  The only specific exception currently defined is
 *  HDFNotImplementedException, indicating a function that is part
 *  of the HDF API, but which cannot or will not be implemented
 *  for Java.
 */
public class HDFException extends Exception
{
    static public final String OutOfMemoryMessage="ERROR: HDF Library: Out of memory";
    static public final String HDFExceptionMessage="ERROR: HDF Library Error";
    static public final String HDFMessage="ERROR: Unknown HDF Error";

    protected int HDFerror;
    protected String msg;

    public HDFException() {
        super();
        HDFerror = 0;
        msg = null;
    }

    public HDFException(String s) {
        super();
        msg = s;
    }

    public HDFException(int err) {
        super();
        HDFerror = err;
        msg = null;
    }

    @Override
    public String getMessage() {
        return msg;
    }
}

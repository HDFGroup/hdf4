/****************************************************************************
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF Java Products. The full HDF Java copyright       *
 * notice, including terms governing use, modification, and redistribution,  *
 * is contained in the file, COPYING.  COPYING can be found at the root of   *
 * the source code distribution tree. You can also access it online  at      *
 * https://www.hdfgroup.org/licenses.  If you do not have access to the      *
 * file, you may request a copy from help@hdfgroup.org.                      *
 ****************************************************************************/

package hdf.hdflib;

/**
 * <p>
 * HDFNotImplementedException indicates a function that is part of the HDF API, but which cannot or
 * will not be implemented for Java.
 * <p>
 * For instance, C routines which take Unix FILE objects as parameters are not appropriate for the
 * Java interface and will not be implemented. These routines will raise an
 * HDFNotImplementedException.
 */

public class HDFNotImplementedException extends HDFJavaException {

    /** */
    public HDFNotImplementedException() { super(); }

    /**
     * Constructs an <code>HDFException</code> with the specified unimplemented
     * message.
     *
     * @param s
     *            the additional message detail.
     */
    public HDFNotImplementedException(String s)
    {
        super("HDFJavaException: HDF function not implemented (yet): " + s);
    }
}

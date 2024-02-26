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
 * The class HDFJavaException returns errors from the Java wrapper of theHDF library.
 * <p>
 * These errors include Java configuration errors, security violations, and resource exhaustion.
 */
public class HDFJavaException extends HDFException {
    /**
     * Constructs an <code>HDFJavaException</code> with no specified detail
     * message.
     */
    public HDFJavaException() { super(); }

    /**
     * Constructs an <code>HDFJavaException</code> with the specified detail
     * message.
     *
     * @param s
     *            the detail message.
     */
    public HDFJavaException(String s) { super(s); }
}

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

    public HDFLibraryException() {
        super();
    }

    public HDFLibraryException(String s) {
        super("HDFLibraryException: "+s);
    }

    public HDFLibraryException(int err) {
        super(err);
    }

    @Override
    public String getMessage() {
        if (msg != null) {
            return msg;
        }

        String s;
        try {
            s = HDFLibrary.HEstring(HDFerror);
        }
        catch (HDFException e) {
            s = new String("HDF error number: "+HDFerror+", HEstring failed");
        }
        msg = "HDFLibraryException: "+s;
        return msg;
    }

    /**
     * Prints this <code>HDFLibraryException</code>, the HDF Library error
     * stack, and and the Java stack trace to the standard error stream.
     */
    @Override
    public void printStackTrace() {
        System.err.println(this);
        printStackTrace0(null); // the HDF Library error stack
        super.printStackTrace(); // the Java stack trace
    }

    /**
     * Prints this <code>HDFLibraryException</code> the HDF Library error
     * stack, and and the Java stack trace to the specified print stream.
     *
     * @param f
     *            the file print stream.
     */
    public void printStackTrace(java.io.File f) {
        if ((f == null) || !f.exists() || f.isDirectory() || !f.canWrite()) {
            printStackTrace();
        }
        else {
            try {
                java.io.FileOutputStream o = new java.io.FileOutputStream(f);
                java.io.PrintWriter p = new java.io.PrintWriter(o);
                p.println(this);
                p.close();
            }
            catch (Exception ex) {
                System.err.println(this);
            }
            ;
            // the HDF Library error stack
            printStackTrace0(f.getPath());
            super.printStackTrace(); // the Java stack trace
        }
    }

    /*
     * This private method calls the HDF library to extract the error codes
     * and error stack.
     */
    private native void printStackTrace0(String s);
}

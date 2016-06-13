/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF Products. The full HDF copyright                 *
 * notice, including terms governing use, modification, and redistribution,  *
 * is contained in the file, COPYING.  COPYING can be found at the root of   *
 * the source code distribution tree. You can also access it online  at      *
 * http://www.hdfgroup.org/products/licenses.html.  If you do not have       *
 * access to the file, you may request a copy from help@hdfgroup.org.        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

package test;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import java.io.File;

import hdf.hdflib.HDFLibrary;
import hdf.hdflib.HDFException;
import hdf.hdflib.HDFConstants;

import org.junit.After;
import org.junit.Before;
import org.junit.Ignore;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.TestName;

/**
 *
 */
public class TestH4DFPparams {
    @Rule public TestName testname = new TestName();

    @Before
    public void showTestName() {
        System.out.print(testname.getMethodName());
    }

    @After
    public void nextTestName() {
        System.out.println();
    }

    @Test(expected = NullPointerException.class)
    public void testDFPaddpalNull() throws Throwable {
        byte[] palette = {0};
        HDFLibrary.DFPaddpal(null, palette);
    }

    @Test(expected = NullPointerException.class)
    public void testDFPaddpalNullPalette() throws Throwable {
        String str = "";
        HDFLibrary.DFPaddpal(str, null);
    }

    @Test(expected = NullPointerException.class)
    public void testDFPgetpalNull() throws Throwable {
        byte[] palette = {0};
        HDFLibrary.DFPgetpal(null, palette);
    }

    @Test(expected = NullPointerException.class)
    public void testDFPgetpalNullPalette() throws Throwable {
        String str = "";
        HDFLibrary.DFPgetpal(str, null);
    }

    @Test(expected = NullPointerException.class)
    public void testDFPnpalsNull() throws Throwable {
        HDFLibrary.DFPnpals(null);
    }

    @Test(expected = NullPointerException.class)
    public void testDFPputpalNull() throws Throwable {
        byte[] palette = {0};
        String arg = "a";
        HDFLibrary.DFPputpal(null, palette, 0, arg);
    }

    @Test(expected = NullPointerException.class)
    public void testDFPputpalArgNull() throws Throwable {
        byte[] palette = {0};
        String arg = "a";
        HDFLibrary.DFPputpal(arg, palette, 0, null);
    }

    @Test(expected = NullPointerException.class)
    public void testDFPputpalNullPalette() throws Throwable {
        String arg = "a";
        String mode = "a";
        HDFLibrary.DFPputpal(arg, null, 0, mode);
    }

    @Test(expected = NullPointerException.class)
    public void testDFPreadrefNull() throws Throwable {
        short ref = 0;
        HDFLibrary.DFPreadref(null, ref);
    }

    @Test(expected = NullPointerException.class)
    public void testDFPwriterefNull() throws Throwable {
        short ref = 0;
        HDFLibrary.DFPwriteref(null, ref);
    }
}

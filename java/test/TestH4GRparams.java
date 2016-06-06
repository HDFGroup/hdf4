/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
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
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

package test;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import java.io.File;

import hdf.hdflib.HDFLibrary;
import hdf.hdflib.HDFException;
import hdf.hdflib.HDFChunkInfo;
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
public class TestH4GRparams {
    @Rule public TestName testname = new TestName();

    @Before
    public void showTestName() {
        System.out.print(testname.getMethodName());
    }

    @After
    public void nextTestName() {
        System.out.println();
    }

    @Test(expected = HDFException.class)
    public void testGRstartIllegalId() throws Throwable {
        HDFLibrary.GRstart(-1);
    }

    @Test(expected = HDFException.class)
    public void testGRendIllegalId() throws Throwable {
        HDFLibrary.GRend(-1);
    }

    @Test(expected = HDFException.class)
    public void testGRfileinfoIllegalId() throws Throwable {
        int[] args = {0, 0};
        HDFLibrary.GRfileinfo(-1, args);
    }

    @Test(expected = NullPointerException.class)
    public void testGRfileinfoNull() throws Throwable {
        HDFLibrary.GRfileinfo(0, null);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testGRfileinfoIArgument() throws Throwable {
        int[] args = {0};
        HDFLibrary.GRfileinfo(0, args);
    }

    @Test(expected = HDFException.class)
    public void testGRgetchunkinfoIllegalId() throws Throwable {
        int[] args = {0};
        HDFChunkInfo chunk_def = new HDFChunkInfo();
        HDFLibrary.GRgetchunkinfo(-1, chunk_def, args);
    }

    @Test(expected = NullPointerException.class)
    public void testGRgetchunkinfoNull() throws Throwable {
        HDFChunkInfo chunk_def = new HDFChunkInfo();
        HDFLibrary.GRgetchunkinfo(0, chunk_def, null);
    }
}

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
import hdf.hdflib.HDFChunkInfo;
import hdf.hdflib.HDFConstants;

import org.junit.After;
import org.junit.Before;
import org.junit.Ignore;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.TestName;

public class TestH4DFparams {
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
    public void testDF24getdimsNullFilename() throws Throwable {
        HDFLibrary.DF24getdims(null, new int[] { 0, 0, 0 });
    }

    @Test(expected = NullPointerException.class)
    public void testDF24getdimsNullArguments() throws Throwable {
        String str = "";
        HDFLibrary.DF24getdims(str, null);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testDF24getdimsIllegalArgument() throws Throwable {
        String str = "";
        HDFLibrary.DF24getdims(str, new int[] { 0, 0 });
    }

    @Test(expected = NullPointerException.class)
    public void testDF24getimageNullFilename() throws Throwable {
        HDFLibrary.DF24getimage(null, new byte[] { (byte) 0x0 }, 0, 0);
    }

    @Test(expected = NullPointerException.class)
    public void testDF24getimageNullImageData() throws Throwable {
        String str = "";
        HDFLibrary.DF24getimage(str, null, 0, 0);
    }

    @Test(expected = NullPointerException.class)
    public void testDF24readrefNullFilename() throws Throwable {
        HDFLibrary.DF24readref(null, 0);
    }

    @Test(expected = HDFException.class)
    public void testDF24readrefIllegalRef() throws Throwable {
        String str = "";
        HDFLibrary.DF24readref(str, -1);
    }

    @Test(expected = NullPointerException.class)
    public void testDF24nimagesNullFilename() throws Throwable {
        HDFLibrary.DF24nimages(null);
    }

    @Test(expected = NullPointerException.class)
    public void testDF24addimageNullFilename() throws Throwable {
        HDFLibrary.DF24addimage(null, new byte[] { (byte) 0x0 }, 0, 0);
    }

    @Test(expected = NullPointerException.class)
    public void testDF24addimageNullImage() throws Throwable {
        String str = "";
        HDFLibrary.DF24addimage(str, null, 0, 0);
    }

    @Test(expected = NullPointerException.class)
    public void testDF24putimageNullFilename() throws Throwable {
        byte[] img = {0};
        HDFLibrary.DF24putimage(null, img, 0, 0);
    }

    @Test(expected = NullPointerException.class)
    public void testDF24putimageNullImage() throws Throwable {
        String str = "";
        byte[] img = null;
        HDFLibrary.DF24putimage(str, img, 0, 0);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testDF24putimageIllegalArgument() throws Throwable {
        String str = "";
        HDFLibrary.DF24putimage(str, new byte[] { }, 0, 0);
    }

    @Test(expected = NullPointerException.class)
    public void testDF24setcompressNullCompInfo() throws Throwable {
        HDFLibrary.DF24setcompress(0, null);
    }

    @Test(expected = HDFException.class)
    public void testDF24setilIllegalIl() throws Throwable {
        HDFLibrary.DF24setil(-1);
    }
}

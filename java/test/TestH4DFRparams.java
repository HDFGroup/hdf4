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


public class TestH4DFRparams {
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
    public void testDFR8getdimsNullFilename() throws Throwable {
        HDFLibrary.DFR8getdims(null, new int[] {0, 0}, new boolean[] { true });
    }

    @Test(expected = NullPointerException.class)
    public void testDFR8getdimsNullDimensions() throws Throwable {
        String str = "";
        HDFLibrary.DFR8getdims(str, null, new boolean[] { true });
    }

    @Test(expected = NullPointerException.class)
    public void testDFR8getdimsNullPalette() throws Throwable {
        String str = "";
        HDFLibrary.DFR8getdims(str, new int[] {0, 0}, null);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testDFR8getdimsIllegalArgument() throws Throwable {
        String str = "";
        HDFLibrary.DFR8getdims(str, new int[] {0}, new boolean[] { true });
    }

    @Test(expected = NullPointerException.class)
    public void testDFR8getimageNullFilename() throws Throwable {
        HDFLibrary.DFR8getimage(null, new byte[] { (byte) 0x0 }, 0, 0, new byte[] { (byte) 0x0 });
    }

    @Test(expected = NullPointerException.class)
    public void testDFR8getimageNullImageData() throws Throwable {
        String str = "";
        HDFLibrary.DFR8getimage(str, null, 0, 0, new byte[] { (byte) 0x0 });
    }

    @Test(expected = NullPointerException.class)
    public void testDFR8readrefNullFilename() throws Throwable {
        HDFLibrary.DFR8readref(null, 0);
    }

    @Test(expected = HDFException.class)
    public void testDFR8readrefIllegalRef() throws Throwable {
        String str = "";
        HDFLibrary.DFR8readref(str, -1);
    }

    @Test(expected = NullPointerException.class)
    public void testDFR8nimagesNullFilename() throws Throwable {
        HDFLibrary.DFR8nimages(null);
    }

    @Test(expected = NullPointerException.class)
    public void testDFR8addimageNullFilename() throws Throwable {
        HDFLibrary.DFR8addimage(null, new byte[] { (byte) 0x0 }, 0, 0, (short) 0);
    }

    @Test(expected = NullPointerException.class)
    public void testDFR8addimageNullImage() throws Throwable {
        String str = "";
        HDFLibrary.DFR8addimage(str, null, 0, 0, (short) 0);
    }

    @Test(expected = NullPointerException.class)
    public void testDFR8putimageNullFilename() throws Throwable {
        HDFLibrary.DFR8putimage(null, new byte[] { (byte) 0x0 }, 0, 0, (short) 0);
    }

    @Test(expected = NullPointerException.class)
    public void testDFR8putimageNullImage() throws Throwable {
        String str = "";
        HDFLibrary.DFR8putimage(str, null, 0, 0, (short) 0);
    }

    @Test(expected = NullPointerException.class)
    public void testDFR8setcompressNullCompInfo() throws Throwable {
        HDFLibrary.DFR8setcompress(0, null);
    }

    @Test(expected = NullPointerException.class)
    public void testDFR8getpalrefNullPalRef() throws Throwable {
        HDFLibrary.DFR8getpalref(null);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testDFR8getpalrefIllegalArgument() throws Throwable {
        HDFLibrary.DFR8getpalref(new short[] { });
    }

    @Test(expected = NullPointerException.class)
    public void testDFR8writerefNullFilename() throws Throwable {
        HDFLibrary.DFR8writeref(null, (short) 0);
    }
}

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
import hdf.hdflib.HDFConstants;

import org.junit.After;
import org.junit.Before;
import org.junit.Ignore;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.TestName;

/**
 * @author xcao
 *
 */
public class TestH4 {
    @Rule public TestName testname = new TestName();
    private static final String H4_FILE = "test.hdf";
    long Hfid = -1;

    private final void _deleteFile(String filename) {
        File file = new File(filename);

        if (file.exists()) {
            try {file.delete();} catch (SecurityException e) {}
        }
    }

    @Before
    public void showTestName() {
        System.out.print(testname.getMethodName());
    }

    @After
    public void nextTestName() {
        if (Hfid > 0)
            try {HDFLibrary.Hclose(Hfid);} catch (Exception ex) {}

        _deleteFile(H4_FILE);
        System.out.println();
    }

    /**
     * Test method for {@link hdf.hdflib.HDFLibrary#J2C(int)}.
     */
    @Test
    public void testJ2C() {
        int DFACC_RDONLY = 0x0001;
        int DFACC_RDWR = 0x0003;
        int DFACC_CREATE = 0x004;
        int AN_DATA_LABEL = 0x0000;
        int AN_DATA_DESC = 0x0001;
        int AN_FILE_LABEL = 0x0002;
        int AN_FILE_DESC = 0x0003;
        int DFREF_NONE = 0x0000;

        int definedValues[] = { DFACC_RDONLY, DFACC_RDWR, DFACC_CREATE, AN_DATA_LABEL,
                AN_DATA_DESC, AN_FILE_LABEL, AN_FILE_DESC, DFREF_NONE };

        int j2cValues[] = { HDFConstants.DFACC_RDONLY,
                HDFConstants.DFACC_RDWR, HDFConstants.DFACC_CREATE, HDFConstants.AN_DATA_LABEL,
                HDFConstants.AN_DATA_DESC, HDFConstants.AN_FILE_LABEL,
                HDFConstants.AN_FILE_DESC, HDFConstants.DFREF_NONE };

        for (int i = 0; i < definedValues.length; i++) {
            assertEquals(definedValues[i], j2cValues[i]);
        }

        assertFalse(DFACC_RDONLY == HDFConstants.DFACC_RDWR);
        assertFalse(AN_DATA_LABEL == HDFConstants.AN_FILE_DESC);
    }

    /**
     * Test method for {@link hdf.hdflib.HDFLibrary#Hgetlibversion(int[], Strin[])}.
     */
    @Test
    public void testHgetlibversion() {
        int LIB_VERSION[] = { 4, 2, 11 };
        int libversion[] = { 0, 0, 0 };
        String version[] = { "" };

        try {
            HDFLibrary.Hgetlibversion(libversion, version);
        }
        catch (Throwable err) {
            fail("HDFLibrary.Hgetlibversion: " + err);
        }

        for (int i = 0; i < 2; i++)
            assertEquals(LIB_VERSION[i], libversion[i]);

        for (int i = 0; i < 2; i++)
            assertFalse(libversion[i] == 0);
    }

    /**
     * Test method for {@link hdf.hdflib.HDFLibrary#Hopen()}.
     */
    @Test
    public void testCreateCloseOpen() {
        try {
            Hfid = HDFLibrary.Hopen(H4_FILE, HDFConstants.DFACC_CREATE);
        }
        catch (Throwable err) {
            fail("HDFLibrary.Hopen create failed: " + err);
        }
        assertTrue(Hfid > 0);
        try {
            HDFLibrary.Hclose(Hfid);
        }
        catch (Throwable err) {
            fail("HDFLibrary.Hclose close failed: " + err);
        }
        try {
            Hfid = HDFLibrary.Hopen(H4_FILE, HDFConstants.DFACC_RDWR);
        }
        catch (Throwable err) {
            fail("HDFLibrary.Hopen open failed: " + err);
        }
        assertTrue(Hfid > 0);
    }

    /**
     * Test method for {@link hdf.hdflib.HDFLibrary#Hishdf()}.
     */
    @Test
    public void testHishdf() {
        boolean fileIsHDF4 = false;

        try {
            Hfid = HDFLibrary.Hopen(H4_FILE, HDFConstants.DFACC_CREATE);
        }
        catch (Throwable err) {
            fail("HDFLibrary.Hopen create failed: " + err);
        }
        assertTrue(Hfid > 0);
        try {
            HDFLibrary.Hclose(Hfid);
        }
        catch (Throwable err) {
            fail("HDFLibrary.Hclose close failed: " + err);
        }
        try {
            fileIsHDF4 = HDFLibrary.Hishdf(H4_FILE);
        }
        catch (Throwable err) {
            fail("HDFLibrary.Hishdf failed: " + err);
        }
        assertTrue(fileIsHDF4);
    }

    /**
     * Test method for {@link hdf.hdflib.HDFLibrary#HDgetNTdesc()}.
     */
    @Test
    public void testHDgetNTdesc() {
        String teststr = null;

        try {
            teststr = HDFLibrary.HDgetNTdesc(HDFConstants.DFNT_NATIVE);
        }
        catch (Throwable err) {
            fail("HDFLibrary.HDgetNTdesc DFNT_NATIVE failed: " + err);
        }
        assertEquals("native format number-type not set", teststr);
        try {
            teststr = HDFLibrary.HDgetNTdesc(HDFConstants.DFNT_CUSTOM);
        }
        catch (Throwable err) {
            fail("HDFLibrary.HDgetNTdesc DFNT_CUSTOM failed: " + err);
        }
        assertEquals("custom format number-type not set", teststr);
        try {
            teststr = HDFLibrary.HDgetNTdesc(HDFConstants.DFNT_FLOAT32);
        }
        catch (Throwable err) {
            fail("HDFLibrary.HDgetNTdesc DFNT_FLOAT32 failed: " + err);
        }
        assertEquals("32-bit floating point", teststr);
        try {
            teststr = HDFLibrary.HDgetNTdesc(HDFConstants.DFNT_INT8);
        }
        catch (Throwable err) {
            fail("HDFLibrary.HDgetNTdesc DFNT_INT8 failed: " + err);
        }
        assertEquals("8-bit signed integer", teststr);
        try {
            teststr = HDFLibrary.HDgetNTdesc(HDFConstants.DFNT_CHAR8);
        }
        catch (Throwable err) {
            fail("HDFLibrary.HDgetNTdesc DFNT_CHAR8 failed: " + err);
        }
        assertEquals("8-bit signed char", teststr);
    }

    /**
     * Test method for {@link hdf.hdflib.HDFLibrary#Hnumber()}.
     */
    @Test
    public void testHnumber() {
        int numberobjs = -1;

        try {
            Hfid = HDFLibrary.Hopen(H4_FILE, HDFConstants.DFACC_CREATE);
        }
        catch (Throwable err) {
            fail("HDFLibrary.Hopen create failed: " + err);
        }
        assertTrue(Hfid > 0);
        try {
            numberobjs = HDFLibrary.Hnumber(Hfid);
        }
        catch (Throwable err) {
            fail("HDFLibrary.Hnumber failed: " + err);
        }
        assertTrue(numberobjs > 0);
    }

    /**
     * Test method for {@link hdf.hdflib.HDFLibrary#DFKNTsize()}.
     */
    @Test
    public void testDFKNTsize() {
        int testsize = -1;

        try {
            testsize = HDFLibrary.DFKNTsize(HDFConstants.DFNT_FLOAT64);
        }
        catch (Throwable err) {
            fail("HDFLibrary.DFKNTsize DFNT_FLOAT64 failed: " + err);
        }
        assertEquals(8, testsize);
        try {
            testsize = HDFLibrary.DFKNTsize(HDFConstants.DFNT_INT16);
        }
        catch (Throwable err) {
            fail("HDFLibrary.DFKNTsize DFNT_INT16 failed: " + err);
        }
        assertEquals(2, testsize);
        try {
            testsize = HDFLibrary.DFKNTsize(HDFConstants.DFNT_CHAR);
        }
        catch (Throwable err) {
            fail("HDFLibrary.DFKNTsize DFNT_CHAR failed: " + err);
        }
        assertEquals(1, testsize);
    }
}

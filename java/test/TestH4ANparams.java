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
public class TestH4ANparams {
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
    public void testANstartIllegalId() throws Throwable {
        HDFLibrary.ANstart(-1);
    }

    @Test(expected = HDFException.class)
    public void testANendIllegalId() throws Throwable {
        HDFLibrary.ANend(-1);
    }

    @Test
    public void testANendaccessIllegalId() throws Throwable {
        //function does nothing
        assertTrue(HDFLibrary.ANendaccess(-1));
    }

    @Test(expected = HDFException.class)
    public void testANfileinfoIllegalId() throws Throwable {
        int[] ids = {0, 0, 0, 0};
        HDFLibrary.ANfileinfo(-1, ids);
    }

    @Test(expected = NullPointerException.class)
    public void testANfileinfoNull() throws Throwable {
        HDFLibrary.ANfileinfo(0, null);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testANfileinfoArgument() throws Throwable {
        int[] ids = {0, 0};
        HDFLibrary.ANfileinfo(0, ids);
    }

    @Test(expected = HDFException.class)
    public void testANselectIllegalId() throws Throwable {
        HDFLibrary.ANselect(-1, 0, 0);
    }

    @Test(expected = HDFException.class)
    public void testANnumannIllegalId() throws Throwable {
        short tag = 0;
        short ref = 0;
        HDFLibrary.ANnumann(-1, 0, tag, ref);
    }

    @Test
    public void testANatype2tagIllegalId() throws Throwable {
        assertEquals(HDFLibrary.ANatype2tag(-1), HDFConstants.DFTAG_NULL);
    }

    @Test(expected = HDFException.class)
    public void testANtag2atypeIllegalId() throws Throwable {
        short anttype = -1;
        HDFLibrary.ANtag2atype(anttype);
    }

    @Test(expected = HDFException.class)
    public void testANannlistIllegalId() throws Throwable {
        short tag = 0;
        short ref = 0;
        int[] ids = {0, 0, 0, 0};
        HDFLibrary.ANannlist(-1, 0, tag, ref, ids);
    }

    @Test(expected = NullPointerException.class)
    public void testANannlistNull() throws Throwable {
        short tag = 0;
        short ref = 0;

        HDFLibrary.ANannlist(0, 0, tag, ref, null);
    }

    @Test(expected = HDFException.class)
    public void testANannlenIllegalId() throws Throwable {
        HDFLibrary.ANannlen(-1);
    }

    @Test(expected = HDFException.class)
    public void testANreadannIllegalId() throws Throwable {
        String[] str = {""};
        HDFLibrary.ANreadann(-1, str, 0);
    }

    @Test(expected = HDFException.class)
    public void testANcreateIllegalId() throws Throwable {
        short tag = 0;
        short ref = 0;
        HDFLibrary.ANcreate(-1, tag, ref, 0);
    }

    @Test(expected = HDFException.class)
    public void testANcreatefIllegalId() throws Throwable {
        HDFLibrary.ANcreatef(-1, 0);
    }

    @Test(expected = HDFException.class)
    public void testANget_tagrefIllegalId() throws Throwable {
        short[] ref = {0, 0};
        HDFLibrary.ANget_tagref(-1, 0, 0, ref);
    }

    @Test(expected = NullPointerException.class)
    public void testANget_tagrefNull() throws Throwable {
        HDFLibrary.ANget_tagref(0, 0, 0, null);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testANget_tagrefArgument() throws Throwable {
        short[] ref = {0};
        HDFLibrary.ANget_tagref(0, 0, 0, ref);
    }

    @Test(expected = HDFException.class)
    public void testANid2tagrefIllegalId() throws Throwable {
        short[] tag = {0, 0};
        HDFLibrary.ANid2tagref(-1, tag);
    }

    @Test(expected = NullPointerException.class)
    public void testANid2tagrefNull() throws Throwable {
        HDFLibrary.ANid2tagref(0, null);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testANid2tagrefArgument() throws Throwable {
        short[] tag = {0};
        HDFLibrary.ANid2tagref(0, tag);
    }

    @Test(expected = HDFException.class)
    public void testANtagref2idIllegalId() throws Throwable {
        short tag = 0;
        short ref = 0;
        HDFLibrary.ANtagref2id(-1, tag, ref);
    }

    @Test(expected = HDFException.class)
    public void testANwriteannIllegalId() throws Throwable {
        String str = "";
        HDFLibrary.ANwriteann(-1, str, 0);
    }

    @Test(expected = NullPointerException.class)
    public void testANwriteannNull() throws Throwable {
        HDFLibrary.ANwriteann(0, null, 0);
    }
}

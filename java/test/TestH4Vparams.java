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

public class TestH4Vparams {
    @Rule public TestName testname = new TestName();

    @Before
    public void showTestName() {
        System.out.print(testname.getMethodName());
    }

    @After
    public void nextTestName() {
        System.out.println();
    }

//    @Test(expected = HDFException.class)
//    public void testVQueryrefIllegalVKey() throws Throwable {
//        HDFLibrary.VQueryref(-1);
//    }

//    @Test(expected = HDFException.class)
//    public void testVQuerytagIllegalVKey() throws Throwable {
//        HDFLibrary.VQuerytag(-1);
//    }

//    @Test(expected = HDFException.class)
//    public void testVstartIllegalId() throws Throwable {
//        HDFLibrary.Vstart(-1);
//    }

    @Test(expected = NullPointerException.class)
    public void testVattachNullAccess() throws Throwable {
        HDFLibrary.Vattach(0, 0, null);
    }

    @Test(expected = HDFException.class)
    public void testVattachIllegalId() throws Throwable {
        String str = "";
        HDFLibrary.Vattach(-1, 0, str);
    }

//    @Test(expected = HDFException.class)
//    public void testVdetachIllegalId() throws Throwable {
//        HDFLibrary.Vdetach(-1);
//    }

//    @Test(expected = HDFException.class)
//    public void testVendIllegalId() throws Throwable {
//        HDFLibrary.Vend(-1);
//    }

//    @Test(expected = HDFException.class)
//    public void testVgetidIllegalId() throws Throwable {
//        HDFLibrary.Vgetid(-1, 0);
//    }

    @Test(expected = NullPointerException.class)
    public void testVgetclassNullClassName() throws Throwable {
        HDFLibrary.Vgetclass(0, null);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testVgetclassIllegalArgument() throws Throwable {
        HDFLibrary.Vgetclass(0, new String[0]);
    }

    @Test(expected = HDFException.class)
    public void testVgetclassIllegalId() throws Throwable {
        HDFLibrary.Vgetclass(-1, new String[1]);
    }

    @Test(expected = NullPointerException.class)
    public void testVgetnameNullName() throws Throwable {
        HDFLibrary.Vgetname(0, null);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testVgetnameIllegalArgument() throws Throwable {
        HDFLibrary.Vgetname(0, new String[0]);
    }

    @Test(expected = HDFException.class)
    public void testVgetnameIllegalId() throws Throwable {
        HDFLibrary.Vgetname(-1, new String[1]);
    }

    @Test(expected = HDFException.class)
    public void testVisvgIllegalId() throws Throwable {
        HDFLibrary.Visvg(-1, 0);
    }

    @Test(expected = HDFException.class)
    public void testVisvsIllegalId() throws Throwable {
        HDFLibrary.Visvs(-1, 0);
    }

    @Test(expected = NullPointerException.class)
    public void testVgettagrefsNullTags() throws Throwable {
        int arraySize = 10;
        HDFLibrary.Vgettagrefs(0, null, new int[arraySize], arraySize);
    }

    @Test(expected = NullPointerException.class)
    public void testVgettagrefsNullRefs() throws Throwable {
        int arraySize = 10;
        HDFLibrary.Vgettagrefs(0, new int[arraySize], null, arraySize);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testVgettagrefsIllegalTagsArgument() throws Throwable {
        int arraySize = 10;
        HDFLibrary.Vgettagrefs(0, new int[1], new int[arraySize], arraySize);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testVgettagrefsIllegalRefsArgument() throws Throwable {
        int arraySize = 10;
        HDFLibrary.Vgettagrefs(0, new int[arraySize], new int[1], arraySize);
    }

    @Test(expected = HDFException.class)
    public void testVgettagrefsIllegalId() throws Throwable {
        int arraySize = 10;
        HDFLibrary.Vgettagrefs(-1, new int[arraySize], new int[arraySize], arraySize);
    }

    @Test(expected = NullPointerException.class)
    public void testVgettagrefNullTagRef() throws Throwable {
        HDFLibrary.Vgettagref(0, 0, null);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testVgettagrefIllegalTagRefArgument() throws Throwable {
        HDFLibrary.Vgettagref(0, 0, new int[1]);
    }

    @Test(expected = HDFException.class)
    public void testVgettagrefIllegalId() throws Throwable {
        HDFLibrary.Vgettagref(-1, 0, new int[2]);
    }

//    @Test(expected = HDFException.class)
//    public void testVntagrefsIllegalId() throws Throwable {
//        HDFLibrary.Vntagrefs(-1);
//    }

//    @Test(expected = HDFException.class)
//    public void testVinqtagrefIllegalId() throws Throwable {
//        HDFLibrary.Vinqtagref(-1, 0, 0);
//    }

    @Test(expected = NullPointerException.class)
    public void testVloneNullRefArray() throws Throwable {
        HDFLibrary.Vlone(0, null, 0);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testVloneIllegalSizeRefArray() throws Throwable {
        int arraySize = 10;
        HDFLibrary.Vlone(0, new int[arraySize - 1], arraySize);
    }

//    @Test(expected = HDFException.class)
//    public void testVloneIllegalId() throws Throwable {
//        HDFLibrary.Vlone(-1, new int[10], 10);
//    }

//    @Test(expected = HDFException.class)
//    public void testVaddtagrefIllegalId() throws Throwable {
//        HDFLibrary.Vaddtagref(-1, 0, 0);
//    }

//    @Test(expected = HDFException.class)
//    public void testVdeletetagrefIllegalId() throws Throwable {
//        HDFLibrary.Vdeletetagref(-1, 0, 0);
//    }

    @Test(expected = NullPointerException.class)
    public void testVfindNullVGroupName() throws Throwable {
        HDFLibrary.Vfind(0, null);
    }

//    @Test(expected = HDFException.class)
//    public void testVfindIllegalId() throws Throwable {
//        String str = "";
//        HDFLibrary.Vfind(-1, str);
//    }

    @Test(expected = NullPointerException.class)
    public void testVfindclassNullClassName() throws Throwable {
        HDFLibrary.Vfindclass(0, null);
    }

//    @Test(expected = HDFException.class)
//    public void testVfindclassIllegalId() throws Throwable {
//        String str = "";
//        HDFLibrary.Vfindclass(-1, str);
//    }

    @Test(expected = NullPointerException.class)
    public void testVflocateNullClassName() throws Throwable {
        HDFLibrary.Vflocate(0, null);
    }

    @Test(expected = HDFException.class)
    public void testVflocateIllegalKey() throws Throwable {
        String str = "";
        HDFLibrary.Vflocate(-1, str);
    }

//    @Test(expected = HDFException.class)
//    public void testVgetnextIllegalKey() throws Throwable {
//        HDFLibrary.Vgetnext(-1, 0);
//    }

    @Test(expected = NullPointerException.class)
    public void testVinquireNullNEntries() throws Throwable {
        HDFLibrary.Vinquire(0, null, new String[1]);
    }

    @Test(expected = NullPointerException.class)
    public void testVinquireNullVGroupName() throws Throwable {
        HDFLibrary.Vinquire(0, new int[1], null);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testVinquireIllegalNEntriesArgument() throws Throwable {
        HDFLibrary.Vinquire(0, new int[0], new String[1]);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testVinquireIllegalVGroupNameArgument() throws Throwable {
        HDFLibrary.Vinquire(0, new int[1], new String[0]);
    }

    @Test(expected = HDFException.class)
    public void testVinquireIllegalId() throws Throwable {
        HDFLibrary.Vinquire(-1, new int[1], new String[1]);
    }

//    @Test(expected = HDFException.class)
//    public void testVinsertIllegalVGroupId() throws Throwable {
//        HDFLibrary.Vinsert(-1, 0);
//    }

//    @Test(expected = HDFException.class)
//    public void testVinsertIllegalVId() throws Throwable {
//        HDFLibrary.Vinsert(0, -1);
//    }

//    @Test(expected = HDFException.class)
//    public void testVnrefsIllegalKey() throws Throwable {
//        HDFLibrary.Vnrefs(-1, 0);
//    }

    @Test(expected = NullPointerException.class)
    public void testVsetclassNullClassName() throws Throwable {
        HDFLibrary.Vsetclass(0, null);
    }

    @Test(expected = HDFException.class)
    public void testVsetclassIllegalId() throws Throwable {
        String str = "";
        HDFLibrary.Vsetclass(-1, str);
    }

    @Test(expected = NullPointerException.class)
    public void testVsetnameNullName() throws Throwable {
        HDFLibrary.Vsetname(0, null);
    }

    @Test(expected = HDFException.class)
    public void testVsetnameIllegalId() throws Throwable {
        String str = "";
        HDFLibrary.Vsetname(-1, str);
    }

    @Test(expected = NullPointerException.class)
    public void testVattrinfoNullName() throws Throwable {
        HDFLibrary.Vattrinfo(0, 0, null, new int[5]);
    }

    @Test(expected = NullPointerException.class)
    public void testVattrinfoNullArgv() throws Throwable {
        HDFLibrary.Vattrinfo(0, 0, new String[1], null);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testVattrinfoIllegalNameArgument() throws Throwable {
        HDFLibrary.Vattrinfo(0, 0, new String[0], new int[5]);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testVattrinfoIllegalArgvArgument() throws Throwable {
        HDFLibrary.Vattrinfo(0, 0, new String[1], new int[1]);
    }

    @Test(expected = HDFException.class)
    public void testVattrinfoIllegalId() throws Throwable {
        HDFLibrary.Vattrinfo(-1, 0, new String[1], new int[5]);
    }

    @Test(expected = NullPointerException.class)
    public void testVfindattrNullName() throws Throwable {
        HDFLibrary.Vfindattr(0, null);
    }

    @Test(expected = HDFException.class)
    public void testVfindattrIllegalId() throws Throwable {
        String str = "";
        HDFLibrary.Vfindattr(-1, str);
    }

    @Test(expected = NullPointerException.class)
    public void testVgetattrNullData() throws Throwable {
        byte[] byteArray = null;
        HDFLibrary.Vgetattr(0, 0, byteArray);
    }

    @Test(expected = HDFException.class)
    public void testVgetattrIllegalId() throws Throwable {
        byte[] byteArray = new byte[10];
        HDFLibrary.Vgetattr(-1, 0, byteArray);
    }

//    @Test(expected = HDFException.class)
//    public void testVgetversionIllegalId() throws Throwable {
//        HDFLibrary.Vgetversion(-1);
//    }

//    @Test(expected = HDFException.class)
//    public void testVnattrsIllegalId() throws Throwable {
//        HDFLibrary.Vnattrs(-1);
//    }

    @Test(expected = NullPointerException.class)
    public void testVsetattrNullName() throws Throwable {
        String values = "";
        HDFLibrary.Vsetattr(0, null, 0, 0, values);
    }

    @Test(expected = NullPointerException.class)
    public void testVsetattrNullName2() throws Throwable {
        HDFLibrary.Vsetattr(0, null, 0, 0, new byte[10]);
    }

    @Test(expected = NullPointerException.class)
    public void testVsetattrNullValuesString() throws Throwable {
        String str = "";
        String values = null;
        HDFLibrary.Vsetattr(0, str, 0, 0, values);
    }

    @Test(expected = NullPointerException.class)
    public void testVsetattrNullDataByteArray() throws Throwable {
        String str = "";
        byte[] data = null;
        HDFLibrary.Vsetattr(0, str, 0, 0, data);
    }

    @Test(expected = HDFException.class)
    public void testVsetattrIllegalId() throws Throwable {
        String str = "";
        String values = "";
        HDFLibrary.Vsetattr(-1, str, 0, 0, values);
    }

    @Test(expected = HDFException.class)
    public void testVsetattrIllegalId2() throws Throwable {
       String str = "";
       HDFLibrary.Vsetattr(-1, str, 0, 0, new byte[10]);
    }
}

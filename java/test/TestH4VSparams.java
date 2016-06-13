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

public class TestH4VSparams {
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
    public void testVSQuerycountNullNRecords() throws Throwable {
        HDFLibrary.VSQuerycount(0, null);
    }

    @Test(expected = HDFException.class)
    public void testVSQuerycountIllegalId() throws Throwable {
        HDFLibrary.VSQuerycount(-1, new int[] { 0 });
    }

    @Test(expected = IllegalArgumentException.class)
    public void testVSQuerycountIllegalArgument() throws Throwable {
        HDFLibrary.VSQuerycount(0, new int[] { });
    }

    @Test(expected = NullPointerException.class)
    public void testVSQueryfieldsNullFields() throws Throwable {
        HDFLibrary.VSQueryfields(0, null);
    }

    @Test(expected = HDFException.class)
    public void testVSQueryfieldsIllegalId() throws Throwable {
        HDFLibrary.VSQueryfields(-1, new String[1]);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testVSQueryfieldsIllegalArgument() throws Throwable {
        HDFLibrary.VSQueryfields(0, new String[0]);
    }

    @Test(expected = NullPointerException.class)
    public void testVSQueryinterlaceNullInterlace() throws Throwable {
        HDFLibrary.VSQueryinterlace(0, null);
    }

    @Test(expected = HDFException.class)
    public void testVSQueryinterlaceIllegalId() throws Throwable {
        HDFLibrary.VSQueryinterlace(-1, new int[1]);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testVSQueryinterlaceIllegalArgument() throws Throwable {
        HDFLibrary.VSQueryinterlace(0, new int[0]);
    }

    @Test(expected = NullPointerException.class)
    public void testVSQuerynameNullVDataName() throws Throwable {
        HDFLibrary.VSQueryname(0, null);
    }

    @Test(expected = HDFException.class)
    public void testVSQuerynameIllegalId() throws Throwable {
        HDFLibrary.VSQueryname(-1, new String[1]);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testVSQuerynameIllegalArgument() throws Throwable {
        HDFLibrary.VSQueryname(0, new String[0]);
    }

//    @Test(expected = HDFException.class)
//    public void testVSQueryrefIllegalId() throws Throwable {
//        HDFLibrary.VSQueryref(-1);
//    }

//    @Test(expected = HDFException.class)
//    public void testVSQuerytagIllegalId() throws Throwable {
//        HDFLibrary.VSQuerytag(-1);
//    }

    @Test(expected = NullPointerException.class)
    public void testVSQueryvsizeNullVDataSize() throws Throwable {
        HDFLibrary.VSQueryvsize(0, null);
    }

    @Test(expected = HDFException.class)
    public void testVSQueryvsizeIllegalId() throws Throwable {
        HDFLibrary.VSQueryvsize(-1, new int[1]);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testVSQueryvsizeIllegalArgument() throws Throwable {
        HDFLibrary.VSQueryvsize(0, new int[0]);
    }

    @Test(expected = NullPointerException.class)
    public void testVSattachNullAccess() throws Throwable {
        HDFLibrary.VSattach(0, 0, null);
    }

    @Test(expected = HDFException.class)
    public void testVSattachIllegalId() throws Throwable {
        HDFLibrary.VSattach(-1, 0, "w");
    }

    @Test(expected = HDFException.class)
    public void testVSattachIllegalReg() throws Throwable {
        HDFLibrary.VSattach(0, -1, "w");
    }

//    @Test(expected = HDFException.class)
//    public void testVSdetachIllegalId() throws Throwable {
//        HDFLibrary.VSdetach(-1);
//    }

//    @Test(expected = HDFException.class)
//    public void testVSgetidIllegalId() throws Throwable {
//        HDFLibrary.VSgetid(-1, 0);
//    }

//    @Test(expected = HDFException.class)
//    public void testVSgetidIllegalRef() throws Throwable {
//        HDFLibrary.VSgetid(0, -1);
//    }

    @Test(expected = NullPointerException.class)
    public void testVSgetclassNullClassName() throws Throwable {
        HDFLibrary.VSgetclass(0, null);
    }

    @Test(expected = HDFException.class)
    public void testVSgetclassIllegalId() throws Throwable {
        HDFLibrary.VSgetclass(-1, new String[1]);
    }

    @Test(expected = NullPointerException.class)
    public void testVSgetnameNullName() throws Throwable {
        HDFLibrary.VSgetname(0, null);
    }

    @Test(expected = HDFException.class)
    public void testVSgetnameIllegalId() throws Throwable {
        HDFLibrary.VSgetname(-1, new String[1]);
    }

//    @Test(expected = HDFException.class)
//    public void testVSeltsIllegalId() throws Throwable {
//        HDFLibrary.VSelts(-1);
//    }

    @Test(expected = NullPointerException.class)
    public void testVSfdefineNullFieldName() throws Throwable {
        HDFLibrary.VSfdefine(0, null, 0, 0);
    }

    @Test(expected = HDFException.class)
    public void testVSfdefineIllegalId() throws Throwable {
        HDFLibrary.VSfdefine(-1, "", 0, 0);
    }

    @Test(expected = NullPointerException.class)
    public void testVSfexistNullFields() throws Throwable {
        HDFLibrary.VSfexist(0, null);
    }

    @Test(expected = HDFException.class)
    public void testVSfexistIllegalId() throws Throwable {
        HDFLibrary.VSfexist(-1, "");
    }

    @Test(expected = NullPointerException.class)
    public void testVSfindNullVDataName() throws Throwable {
        HDFLibrary.VSfind(0, null);
    }

//    @Test(expected = HDFException.class)
//    public void testVSfindIllegalId() throws Throwable {
//        HDFLibrary.VSfind(-1, "");
//    }

    @Test(expected = HDFException.class)
    public void testVSsetblocksizeIllegalId() throws Throwable {
        HDFLibrary.VSsetblocksize(-1, 0);
    }

    @Test(expected = HDFException.class)
    public void testVSsetnumblocksIllegalId() throws Throwable {
        HDFLibrary.VSsetnumblocks(-1, 0);
    }

    @Test(expected = NullPointerException.class)
    public void testVSgetfieldsNullFieldName() throws Throwable {
        HDFLibrary.VSgetfields(0, null);
    }

    @Test(expected = HDFException.class)
    public void testVSgetfieldsIllegalId() throws Throwable {
        HDFLibrary.VSgetfields(-1, new String[1]);
    }

//    @Test(expected = HDFException.class)
//    public void testVSgetinterlaceIllegalId() throws Throwable {
//        HDFLibrary.VSgetinterlace(-1);
//    }

    @Test(expected = NullPointerException.class)
    public void testVSinquireNullIArgs() throws Throwable {
        HDFLibrary.VSinquire(0, null, new String[2]);
    }

    @Test(expected = NullPointerException.class)
    public void testVSinquireNullSArgs() throws Throwable {
        HDFLibrary.VSinquire(0, new int[3], null);
    }

    @Test(expected = HDFException.class)
    public void testVSinquireIllegalId() throws Throwable {
        HDFLibrary.VSinquire(-1, new int[3], new String[2]);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testVSinquireIllegalIArgs() throws Throwable {
        HDFLibrary.VSinquire(0, new int[1], new String[2]);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testVSinquireIllegalSArgs() throws Throwable {
        HDFLibrary.VSinquire(0, new int[3], new String[1]);
    }

    @Test(expected = NullPointerException.class)
    public void testVSloneNullRefArray() throws Throwable {
        HDFLibrary.VSlone(0, null, 0);
    }

//    @Test(expected = HDFException.class)
//    public void testVSloneIllegalId() throws Throwable {
//        HDFLibrary.VSlone(-1, new int[10], 0);
//    }

    @Test(expected = NullPointerException.class)
    public void testVSreadNullDataBuffer() throws Throwable {
        HDFLibrary.VSread(0, null, 0, 0);
    }

//    @Test(expected = HDFException.class)
//    public void testVSreadIllegalId() throws Throwable {
//        HDFLibrary.VSread(-1, new byte[] { }, 0, 0);
//    }

//    @Test(expected = HDFException.class)
//    public void testVSseekIllegalId() throws Throwable {
//        HDFLibrary.VSseek(-1, 0);
//    }

    @Test(expected = NullPointerException.class)
    public void testVSsetfieldsNullFields() throws Throwable {
        HDFLibrary.VSsetfields(0, null);
    }

    @Test(expected = HDFException.class)
    public void testVSsetfieldsIllegalId() throws Throwable {
        HDFLibrary.VSsetfields(-1, "");
    }

    @Test(expected = HDFException.class)
    public void testVSsetinterlaceIllegalId() throws Throwable {
        HDFLibrary.VSsetinterlace(-1, 0);
    }

    @Test(expected = NullPointerException.class)
    public void testVSsizeofNullFields() throws Throwable {
        HDFLibrary.VSsizeof(0, null);
    }

    @Test(expected = HDFException.class)
    public void testVSsizeofIllegalId() throws Throwable {
        HDFLibrary.VSsizeof(-1, "");
    }

    @Test(expected = HDFException.class)
    public void testVSappendableIllegalVKey() throws Throwable {
        HDFLibrary.VSappendable(-1, 0);
    }

    @Test(expected = NullPointerException.class)
    public void testVSfindclassNullClass() throws Throwable {
        HDFLibrary.VSfindclass(0, null);
    }

//    @Test(expected = HDFException.class)
//    public void testVSfindclassIllegalId() throws Throwable {
//        HDFLibrary.VSfindclass(-1, "");
//    }

//    @Test(expected = HDFException.class)
//    public void testVSgetversionIllegalVKey() throws Throwable {
//        HDFLibrary.VSgetversion(-1);
//    }

    @Test(expected = NullPointerException.class)
    public void testVSsetclassNullClass() throws Throwable {
        HDFLibrary.VSsetclass(0, null);
    }

    @Test(expected = HDFException.class)
    public void testVSsetclassIllegalId() throws Throwable {
        HDFLibrary.VSsetclass(-1, "");
    }

    @Test(expected = NullPointerException.class)
    public void testVSsetexternalfileNullFilename() throws Throwable {
        HDFLibrary.VSsetexternalfile(0, null, 0);
    }

    @Test(expected = HDFException.class)
    public void testVSsetexternalfileIllegalVKey() throws Throwable {
        HDFLibrary.VSsetexternalfile(-1, "", 0);
    }

    @Test(expected = NullPointerException.class)
    public void testVSsetnameNullVDataName() throws Throwable {
        HDFLibrary.VSsetname(0, null);
    }

    @Test(expected = HDFException.class)
    public void testVSsetnameIllegalId() throws Throwable {
        HDFLibrary.VSsetname(-1, "");
    }

    @Test(expected = NullPointerException.class)
    public void testVSwriteNullDataBuffer() throws Throwable {
        HDFLibrary.VSwrite(0, null, 0, 0);
    }

    @Test(expected = HDFException.class)
    public void testVSwriteIllegalId() throws Throwable {
        HDFLibrary.VSwrite(-1, new byte[] { }, 0, 0);
    }

    @Test(expected = NullPointerException.class)
    public void testVSattrinfoNullName() throws Throwable {
        HDFLibrary.VSattrinfo(0, 0, 0, null, new int[3]);
    }

    @Test(expected = NullPointerException.class)
    public void testVSattrinfoNullArgs() throws Throwable {
        HDFLibrary.VSattrinfo(0, 0, 0, new String[1], null);
    }

    @Test(expected = HDFException.class)
    public void testVSattrinfoIllegalId() throws Throwable {
        HDFLibrary.VSattrinfo(-1, 0, 0, new String[1], new int[3]);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testVSattrinfoIllegalNameArgument() throws Throwable {
        HDFLibrary.VSattrinfo(0, 0, 0, new String[0], new int[3]);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testVSattrinfoIllegalAttributeArgs() throws Throwable {
        HDFLibrary.VSattrinfo(0, 0, 0, new String[1], new int[1]);
    }

    @Test(expected = NullPointerException.class)
    public void testVSfindexNullName() throws Throwable {
        HDFLibrary.VSfindex(0, null, new int[1]);
    }

    @Test(expected = NullPointerException.class)
    public void testVSfindexNullIndexArray() throws Throwable {
        HDFLibrary.VSfindex(0, "", null);
    }

    @Test(expected = HDFException.class)
    public void testVSfindexIllegalId() throws Throwable {
        HDFLibrary.VSfindex(-1, "", new int[1]);
    }

    @Test(expected = NullPointerException.class)
    public void testVSfindattrNullName() throws Throwable {
        HDFLibrary.VSfindattr(0, 0, null);
    }

    @Test(expected = HDFException.class)
    public void testVSfindattrIllegalId() throws Throwable {
        HDFLibrary.VSfindattr(-1, 0, "");
    }

//    @Test(expected = HDFException.class)
//    public void testVSfnattrsIllegalId() throws Throwable {
//        HDFLibrary.VSfnattrs(-1, 0);
//    }

    @Test(expected = NullPointerException.class)
    public void testVSgetattrNullData() throws Throwable {
        HDFLibrary.VSgetattr(0, 0, 0, null);
    }

    @Test(expected = HDFException.class)
    public void testVSgetattrIllegalId() throws Throwable {
        HDFLibrary.VSgetattr(-1, 0, 0, new byte[] { });
    }

//    @Test(expected = HDFException.class)
//    public void testVSisattrIllegalId() throws Throwable {
//        HDFLibrary.VSisattr(-1);
//    }

//    @Test(expected = HDFException.class)
//    public void testVSnattrsIllegalId() throws Throwable {
//        HDFLibrary.VSnattrs(-1);
//    }

    @Test(expected = NullPointerException.class)
    public void testVSsetattrNullAttributeName() throws Throwable {
        HDFLibrary.VSsetattr(0, 0, null, 0, 0, new byte[] { });
    }

    @Test(expected = NullPointerException.class)
    public void testVSsetattrNullAttributeName2() throws Throwable {
        HDFLibrary.VSsetattr(0, 0, null, 0, 0, "");
    }

//    @Test(expected = NullPointerException.class)
//    public void testVSsetattrNullValues() throws Throwable {
//        HDFLibrary.VSsetattr(0, 0, "", 0, 0, null);
//    }

    @Test(expected = HDFException.class)
    public void testVSsetattrIllegalId() throws Throwable {
        HDFLibrary.VSsetattr(-1, 0, "", 0, 0, new byte[] { });
    }

    @Test(expected = HDFException.class)
    public void testVSsetattrIllegalId2() throws Throwable {
        HDFLibrary.VSsetattr(-1, 0, "", 0, 0, "");
    }
}

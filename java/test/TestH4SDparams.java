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
import hdf.hdflib.HDFCompInfo;
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
public class TestH4SDparams {
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
    public void testSDstartNull() throws Throwable {
        HDFLibrary.SDstart(null, 0);
    }

    @Test(expected = HDFException.class)
    public void testSDendIllegalId() throws Throwable {
        HDFLibrary.SDend(-1);
    }

    @Test(expected = HDFException.class)
    public void testSDfileinfoIllegalId() throws Throwable {
        int[] args = {0, 0};
        HDFLibrary.SDfileinfo(-1, args);
    }

    @Test(expected = NullPointerException.class)
    public void testSDfileinfoNull() throws Throwable {
        HDFLibrary.SDfileinfo(0, null);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testSDfileinfoArgument() throws Throwable {
        int[] args = {0};
        HDFLibrary.SDfileinfo(0, args);
    }

    @Test(expected = HDFException.class)
    public void testSDselectIllegalId() throws Throwable {
        HDFLibrary.SDselect(-1, 0);
    }

    @Test(expected = HDFException.class)
    public void testSDnametoindexIllegalId() throws Throwable {
        HDFLibrary.SDnametoindex(-1, "");
    }

    @Test(expected = NullPointerException.class)
    public void testSDnametoindexNull() throws Throwable {
        HDFLibrary.SDnametoindex(0, null);
    }

    @Test(expected = HDFException.class)
    public void testSDgetinfoIllegalId() throws Throwable {
        String[] name = {""};
        int[] args = {0, 0, 0};
        int[] dim_sizes = {0, 0};
        HDFLibrary.SDgetinfo(-1, name, dim_sizes, args);
    }

    @Test(expected = NullPointerException.class)
    public void testSDgetinfoNullName() throws Throwable {
        int[] args = {0, 0, 0};
        int[] dim_sizes = {0, 0};
        HDFLibrary.SDgetinfo(0, null, dim_sizes, args);
    }

    @Test(expected = NullPointerException.class)
    public void testSDgetinfoNullArgs() throws Throwable {
        String[] name = {""};
        int[] dim_sizes = {0, 0};
        HDFLibrary.SDgetinfo(0, name, dim_sizes, null);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testSDgetinfoArgumentArgs() throws Throwable {
        String[] name = {""};
        int[] args = {0};
        int[] dim_sizes = {0, 0};
        HDFLibrary.SDgetinfo(0, name, dim_sizes, args);
    }

    @Test(expected = NullPointerException.class)
    public void testSDgetinfoNullDims() throws Throwable {
        String[] name = {""};
        int[] args = {0, 0, 0};
        HDFLibrary.SDgetinfo(0, name, null, args);
    }

    @Test(expected = HDFException.class)
    public void testSDcheckemptyIllegalId() throws Throwable {
        int[] emptySDS = {0};
        HDFLibrary.SDcheckempty(-1, emptySDS);
    }

    @Test(expected = NullPointerException.class)
    public void testSDcheckemptyNull() throws Throwable {
        HDFLibrary.SDcheckempty(0, null);
    }

    @Test(expected = HDFException.class)
    public void testSDreaddataIllegalId() throws Throwable {
        int[] start = {0, 0};
        int[] stride = {0, 0};
        int[] count = {0, 0};
        byte[] data = {0};
        HDFLibrary.SDreaddata(-1, start, stride, count, data);
    }

    @Test(expected = NullPointerException.class)
    public void testSDreaddataNullData() throws Throwable {
        int[] start = {0, 0};
        int[] stride = {0, 0};
        int[] count = {0, 0};
        HDFLibrary.SDreaddata(0, start, stride, count, null);
    }

    @Test(expected = NullPointerException.class)
    public void testSDreaddataNullStart() throws Throwable {
        int[] stride = {0, 0};
        int[] count = {0, 0};
        byte[] data = {0};
        HDFLibrary.SDreaddata(0, null, stride, count, data);
    }

    @Test(expected = NullPointerException.class)
    public void testSDreaddataNullCount() throws Throwable {
        int[] start = {0, 0};
        int[] stride = {0, 0};
        byte[] data = {0};
        HDFLibrary.SDreaddata(0, start, stride, null, data);
    }

    @Test(expected = HDFException.class)
    public void testSDreaddata_shortIllegalId() throws Throwable {
        int[] start = {0, 0};
        int[] stride = {0, 0};
        int[] count = {0, 0};
        short[] data = {0};
        HDFLibrary.SDreaddata_short(-1, start, stride, count, data);
    }

    @Test(expected = HDFException.class)
    public void testSDreaddata_intIllegalId() throws Throwable {
        int[] start = {0, 0};
        int[] stride = {0, 0};
        int[] count = {0, 0};
        int[] data = {0};
        HDFLibrary.SDreaddata_int(-1, start, stride, count, data);
    }

    @Test(expected = HDFException.class)
    public void testSDreaddata_longIllegalId() throws Throwable {
        int[] start = {0, 0};
        int[] stride = {0, 0};
        int[] count = {0, 0};
        long[] data = {0};
        HDFLibrary.SDreaddata_long(-1, start, stride, count, data);
    }

    @Test(expected = HDFException.class)
    public void testSDreaddata_floatIllegalId() throws Throwable {
        int[] start = {0, 0};
        int[] stride = {0, 0};
        int[] count = {0, 0};
        float[] data = {0};
        HDFLibrary.SDreaddata_float(-1, start, stride, count, data);
    }

    @Test(expected = HDFException.class)
    public void testSDreaddata_doubleIllegalId() throws Throwable {
        int[] start = {0, 0};
        int[] stride = {0, 0};
        int[] count = {0, 0};
        double[] data = {0};
        HDFLibrary.SDreaddata_double(-1, start, stride, count, data);
    }

    @Test(expected = HDFException.class)
    public void testSDendaccessIllegalId() throws Throwable {
        HDFLibrary.SDendaccess(-1);
    }

    @Test(expected = HDFException.class)
    public void testSDgetdimidIllegalId() throws Throwable {
        HDFLibrary.SDgetdimid(-1, 0);
    }

    @Test(expected = HDFException.class)
    public void testSDdiminfoIllegalId() throws Throwable {
        String[] name = {""};
        int[] argv = {0, 0, 0};
        HDFLibrary.SDdiminfo(-1, name, argv);
    }

    @Test(expected = NullPointerException.class)
    public void testSDdiminfoNullName() throws Throwable {
        int[] argv = {0, 0, 0};
        HDFLibrary.SDdiminfo(0, null, argv);
    }

    @Test(expected = NullPointerException.class)
    public void testSDdiminfoNullArgs() throws Throwable {
        String[] name = {""};
        HDFLibrary.SDdiminfo(0, name, null);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testSDdiminfoArgumentArgs() throws Throwable {
        String[] name = {""};
        int[] argv = {0};
        HDFLibrary.SDdiminfo(0, name, argv);
    }

    @Test(expected = HDFException.class)
    public void testSDidtorefIllegalId() throws Throwable {
        HDFLibrary.SDidtoref(-1);
    }

    @Test(expected = HDFException.class)
    public void testSDreftoindexIllegalId() throws Throwable {
        short ref = 0;
        HDFLibrary.SDreftoindex(-1, ref);
    }

    @Test(expected = HDFException.class)
    public void testSDattrinfoIllegalId() throws Throwable {
        String[] name = {""};
        int index = 0;
        int[] argv = {0, 0};
        HDFLibrary.SDattrinfo(-1, index, name, argv);
    }

    @Test(expected = NullPointerException.class)
    public void testSDattrinfoNullName() throws Throwable {
        int index = 0;
        int[] argv = {0, 0};
        HDFLibrary.SDattrinfo(0, index, null, argv);
    }

    @Test(expected = NullPointerException.class)
    public void testSDattrinfoNullArgs() throws Throwable {
        String[] name = {""};
        int index = 0;
        HDFLibrary.SDattrinfo(0, index, name, null);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testSDattrinfoArgumentArgs() throws Throwable {
        String[] name = {""};
        int index = 0;
        int[] argv = {0};
        HDFLibrary.SDattrinfo(0, index, name, argv);
    }

    @Test(expected = HDFException.class)
    public void testSDreadattrIllegalId() throws Throwable {
        int index = 0;
        byte[] data = {0};
        HDFLibrary.SDreadattr(-1, index, data);
    }

    @Test(expected = NullPointerException.class)
    public void testSDreadattrNull() throws Throwable {
        int index = 0;
        HDFLibrary.SDreadattr(0, index, null);
    }

    @Test(expected = HDFException.class)
    public void testSDfindattrIllegalId() throws Throwable {
        String str = "";
        HDFLibrary.SDfindattr(-1, str);
    }

    @Test(expected = NullPointerException.class)
    public void testSDfindattrNull() throws Throwable {
        HDFLibrary.SDfindattr(0, null);
    }

    @Test(expected = HDFException.class)
    public void testSDiscoordvarIllegalId() throws Throwable {
        HDFLibrary.SDiscoordvar(-1);
    }

    @Test(expected = HDFException.class)
    public void testSDgetcalIllegalId() throws Throwable {
        double[] args = {0, 0, 0, 0};
        int[] NT = {0};
        HDFLibrary.SDgetcal(-1, args, NT);
    }

    @Test(expected = NullPointerException.class)
    public void testSDgetcalNull() throws Throwable {
        double[] args = {0, 0, 0, 0};
        HDFLibrary.SDgetcal(0, args, null);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testSSDgetcalArgument() throws Throwable {
        double[] args = {0};
        int[] NT = {0};
        HDFLibrary.SDgetcal(0, args, NT);
    }

    @Test(expected = NullPointerException.class)
    public void testSDgetcalNullArgument() throws Throwable {
        int[] NT = {0};
        HDFLibrary.SDgetcal(0, null, NT);
    }

    @Test(expected = HDFException.class)
    public void testSDgetdatastrsIllegalId() throws Throwable {
        String[] str = {"", "", "", ""};
        int len = 0;
        HDFLibrary.SDgetdatastrs(-1, str, len);
    }

    @Test(expected = NullPointerException.class)
    public void testSDgetdatastrsNull() throws Throwable {
        int len = 0;
        HDFLibrary.SDgetdatastrs(0, null, len);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testSDgetdatastrsArgument() throws Throwable {
        String[] str = {""};
        int len = 0;
        HDFLibrary.SDgetdatastrs(-1, str, len);
    }

    @Test(expected = HDFException.class)
    public void testSDgetdimstrsIllegalId() throws Throwable {
        String[] str = {"", "", "", ""};
        int len = 0;
        HDFLibrary.SDgetdimstrs(-1, str, len);
    }

    @Test(expected = NullPointerException.class)
    public void testSDgetdimstrsNull() throws Throwable {
        int len = 0;
        HDFLibrary.SDgetdimstrs(0, null, len);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testSDgetdimstrsArgument() throws Throwable {
        String[] str = {""};
        int len = 0;
        HDFLibrary.SDgetdimstrs(-1, str, len);
    }

    @Test(expected = HDFException.class)
    public void testSDgetdimscaleIllegalId() throws Throwable {
        byte[] data = {0};
        HDFLibrary.SDgetdimscale(-1, data);
    }

    @Test(expected = NullPointerException.class)
    public void testSDgetdimscaleNull() throws Throwable {
        HDFLibrary.SDgetdimscale(0, null);
    }

    @Test(expected = HDFException.class)
    public void testSDgetfillvalueIllegalId() throws Throwable {
        byte[] data = {0};
        HDFLibrary.SDgetfillvalue(-1, data);
    }

    @Test(expected = NullPointerException.class)
    public void testSDgetfillvalueNull() throws Throwable {
        byte[] data = null;
        HDFLibrary.SDgetfillvalue(0, data);
    }

    @Test(expected = HDFException.class)
    public void testSDgetrangeIllegalId() throws Throwable {
        byte[] min = {0};
        byte[] max = {0};
        HDFLibrary.SDgetrange(-1, max, min);
    }

    @Test(expected = NullPointerException.class)
    public void testSDgetrangeNullMin() throws Throwable {
        byte[] min = null;
        byte[] max = {0};
        HDFLibrary.SDgetrange(0, max, min);
    }

    @Test(expected = NullPointerException.class)
    public void testSDgetrangeNullMax() throws Throwable {
        byte[] min = {0};
        byte[] max = null;
        HDFLibrary.SDgetrange(0, max, min);
    }

    @Test(expected = HDFException.class)
    public void testSDcreateIllegalId() throws Throwable {
        String name = "";
        long num_type = 0;
        int rank = 0;
        int[] dim_sizes = {0};
        HDFLibrary.SDcreate(-1, name, num_type, rank, dim_sizes);
    }

    @Test(expected = NullPointerException.class)
    public void testSDcreateNullName() throws Throwable {
        long num_type = 0;
        int rank = 0;
        int[] dim_sizes = {0, 0};
        HDFLibrary.SDcreate(0, null, num_type, rank, dim_sizes);
    }

    @Test(expected = NullPointerException.class)
    public void testSDcreateNullDims() throws Throwable {
        String name = "";
        long num_type = 0;
        int rank = 0;
        HDFLibrary.SDcreate(0, name, num_type, rank, null);
    }

    // this API call returns false for not record and for failures
    // maybe failures should return a negative?
    @Ignore//(expected = HDFException.class)
    public void testSDisrecordIllegalId() throws Throwable {
        HDFLibrary.SDisrecord(-1);
    }

    @Test(expected = HDFException.class)
    public void testSDsetattrIllegalId() throws Throwable {
        String name = "";
        long num_type = 0;
        int count = 0;
        byte[] data = {0};
        HDFLibrary.SDsetattr(-1, name, num_type, count, data);
    }

    @Test(expected = NullPointerException.class)
    public void testSDsetattrNullName() throws Throwable {
        long num_type = 0;
        int count = 0;
        byte[] data = {0};
        HDFLibrary.SDsetattr(0, null, num_type, count, data);
    }

    @Test(expected = NullPointerException.class)
    public void testSDsetattrNullVals() throws Throwable {
        String name = "";
        long num_type = 0;
        int count = 0;
        byte[] data = null;
        HDFLibrary.SDsetattr(0, name, num_type, count, data);
    }

    @Test(expected = HDFException.class)
    public void testSDsetcalIllegalId() throws Throwable {
        double val = 0;
        int num_type = 0;
        HDFLibrary.SDsetcal(-1, val, val, val, val, num_type);
    }

    @Test(expected = HDFException.class)
    public void testSDsetdatastrsIllegalId() throws Throwable {
        String[] str = {"", "", "", ""};
        HDFLibrary.SDsetdatastrs(-1, str[0], str[1], str[2], str[3]);
    }

    @Test(expected = HDFException.class)
    public void testSDsetdimnameIllegalId() throws Throwable {
        String str = "";
        HDFLibrary.SDsetdimname(-1, str);
    }

    @Test(expected = NullPointerException.class)
    public void testSDsetdimnameNull() throws Throwable {
        String str = null;
        HDFLibrary.SDsetdimname(-1, str);
    }

    @Test(expected = HDFException.class)
    public void testSDsetdimscaleIllegalId() throws Throwable {
        int num_type = 0;
        int count = 0;
        byte[] data = {0};
        HDFLibrary.SDsetdimscale(-1, count, num_type, data);
    }

    @Test(expected = NullPointerException.class)
    public void testSDsetdimscaleNull() throws Throwable {
        int num_type = 0;
        int count = 0;
        byte[] data = null;
        HDFLibrary.SDsetdimscale(0, count, num_type, data);
    }

    @Test(expected = HDFException.class)
    public void testSDsetdimstrsIllegalId() throws Throwable {
        String[] str = {"", "", ""};
        HDFLibrary.SDsetdimstrs(-1, str[0], str[1], str[2]);
    }

    @Test(expected = HDFException.class)
    public void testSDsetexternalfileIllegalId() throws Throwable {
        String str = "";
        int offset = 0;
        HDFLibrary.SDsetexternalfile(-1, str, offset);
    }

    @Test(expected = NullPointerException.class)
    public void testSDsetexternalfileNull() throws Throwable {
        String str = null;
        int offset = 0;
        HDFLibrary.SDsetexternalfile(-1, str, offset);
    }

    @Test(expected = HDFException.class)
    public void testSDsetfillvalueIllegalId() throws Throwable {
        byte[] data = {0};
        HDFLibrary.SDsetfillvalue(-1, data);
    }

    @Test(expected = NullPointerException.class)
    public void testSDsetfillvalueNull() throws Throwable {
        byte[] data = null;
        HDFLibrary.SDsetfillvalue(0, data);
    }

    @Test(expected = HDFException.class)
    public void testSDsetrangeIllegalId() throws Throwable {
        byte[] min = {0};
        byte[] max = {0};
        HDFLibrary.SDsetrange(-1, max, min);
    }

    @Test(expected = NullPointerException.class)
    public void testSDsetrangeNullMin() throws Throwable {
        byte[] min = null;
        byte[] max = {0};
        HDFLibrary.SDsetrange(0, max, min);
    }

    @Test(expected = NullPointerException.class)
    public void testSDsetrangeNullMax() throws Throwable {
        byte[] min = {0};
        byte[] max = null;
        HDFLibrary.SDsetrange(0, max, min);
    }

    @Test(expected = HDFException.class)
    public void testSDwritedataIllegalId() throws Throwable {
        int[] start = {0, 0};
        int[] stride = {0, 0};
        int[] count = {0, 0};
        byte[] data = {0};
        HDFLibrary.SDwritedata(-1, start, stride, count, data);
    }

    @Test(expected = NullPointerException.class)
    public void testSDwritedataNullData() throws Throwable {
        int[] start = {0, 0};
        int[] stride = {0, 0};
        int[] count = {0, 0};
        HDFLibrary.SDwritedata(0, start, stride, count, null);
    }

    @Test(expected = NullPointerException.class)
    public void testSDwritedataNullStart() throws Throwable {
        int[] stride = {0, 0};
        int[] count = {0, 0};
        byte[] data = {0};
        HDFLibrary.SDwritedata(0, null, stride, count, data);
    }

    @Test(expected = NullPointerException.class)
    public void testSDwritedataNullCount() throws Throwable {
        int[] start = {0, 0};
        int[] stride = {0, 0};
        byte[] data = {0};
        HDFLibrary.SDwritedata(0, start, stride, null, data);
    }

    @Test(expected = HDFException.class)
    public void testSDsetnbitdatasetIllegalId() throws Throwable {
        int start_bit = 0;
        int bit_len = 0;
        int sign_ext = 0;
        int fill_one = 0;
        HDFLibrary.SDsetnbitdataset(-1, start_bit, bit_len, sign_ext, fill_one);
    }

    @Test(expected = HDFException.class)
    public void testSDsetcompressIllegalId() throws Throwable {
        int type = 0;
        HDFCompInfo comp_info = new HDFCompInfo();
        HDFLibrary.SDsetcompress(-1, type, comp_info);
    }

    @Test(expected = NullPointerException.class)
    public void testSDsetcompressNull() throws Throwable {
        int type = 0;
        HDFCompInfo comp_info = null;
        HDFLibrary.SDsetcompress(0, type, null);
    }

    @Test(expected = HDFException.class)
    public void testSDgetcompinfoIllegalId() throws Throwable {
        HDFCompInfo comp_info = new HDFCompInfo();
        HDFLibrary.SDgetcompinfo(-1, comp_info);
    }

    @Test(expected = NullPointerException.class)
    public void testSDgetcompinfoNull() throws Throwable {
        HDFCompInfo comp_info = null;
        HDFLibrary.SDgetcompinfo(0, null);
    }

    @Test(expected = HDFException.class)
    public void testSDsetaccesstypeIllegalId() throws Throwable {
        int index = 0;
        HDFLibrary.SDsetaccesstype(-1, index);
    }

    @Test(expected = HDFException.class)
    public void testSDsetblocksizeIllegalId() throws Throwable {
        int block_size = 0;
        HDFLibrary.SDsetblocksize(-1, block_size);
    }

    @Test(expected = HDFException.class)
    public void testSDsetfillmodeIllegalId() throws Throwable {
        boolean fill_enable = false;
        HDFLibrary.SDsetfillmode(-1, fill_enable);
    }

    @Test(expected = HDFException.class)
    public void testSDsetdimval_compIllegalId() throws Throwable {
        int comp_mode = 0;
        HDFLibrary.SDsetdimval_comp(-1, comp_mode);
    }

    @Test(expected = HDFException.class)
    public void testSDisdimval_bwcompIllegalId() throws Throwable {
        HDFLibrary.SDisdimval_bwcomp(-1);
    }

    @Test(expected = HDFException.class)
    public void testSDsetchunkIllegalId() throws Throwable {
        int flags = 0;
        HDFChunkInfo chunk_def = new HDFChunkInfo();
        HDFLibrary.SDsetchunk(-1, chunk_def, flags);
    }

    @Test(expected = NullPointerException.class)
    public void testSDsetchunkNull() throws Throwable {
        int flags = 0;
        HDFChunkInfo chunk_def = null;
        HDFLibrary.SDsetchunk(0, null, flags);
    }

    //Library routine check of ID will not fail - it will just crash
    @Ignore//(expected = HDFException.class)
    public void testSDreadchunkIllegalId() throws Throwable {
        int[] args = {0, 0};
        byte[] data = {0};
        HDFLibrary.SDreadchunk(-1, args, data);
    }

    @Test(expected = NullPointerException.class)
    public void testSDreadchunkNull() throws Throwable {
        int[] args = {0, 0};
        byte[] data = null;
        HDFLibrary.SDreadchunk(0, args, data);
    }

    @Test(expected = NullPointerException.class)
    public void testSDreadchunkNullArgument() throws Throwable {
        byte[] data = {0};
        HDFLibrary.SDreadchunk(0, null, data);
    }

    @Test(expected = HDFException.class)
    public void testSDsetchunkcacheIllegalId() throws Throwable {
        int maxcache = 0;
        int flags = 0;
        HDFLibrary.SDsetchunkcache(-1, maxcache, flags);
    }

    //Library routine check of ID will not fail - it will just crash
    @Ignore//(expected = HDFException.class)
    public void testSDwritechunkIllegalId() throws Throwable {
        int[] args = {0, 0};
        byte[] data = {0};
        HDFLibrary.SDwritechunk(-1, args, data);
    }

    @Test(expected = NullPointerException.class)
    public void testSDwritechunkNull() throws Throwable {
        int[] args = {0, 0};
        byte[] data = null;
        HDFLibrary.SDwritechunk(0, args, data);
    }

    @Test(expected = NullPointerException.class)
    public void testSDwritechunkNullArgument() throws Throwable {
        byte[] data = {0};
        HDFLibrary.SDwritechunk(0, null, data);
    }
}

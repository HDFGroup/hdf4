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
    public void testGRfileinfoArgument() throws Throwable {
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

    @Test(expected = HDFException.class)
    public void testGRselectIllegalId() throws Throwable {
        HDFLibrary.GRselect(-1, 0);
    }

    @Test(expected = HDFException.class)
    public void testGRnametoindexIllegalId() throws Throwable {
        HDFLibrary.GRnametoindex(-1, "");
    }

    @Test(expected = NullPointerException.class)
    public void testGRnametoindexNull() throws Throwable {
        HDFLibrary.GRnametoindex(0, null);
    }

    @Test(expected = HDFException.class)
    public void testGRgetiminfoIllegalId() throws Throwable {
        String[] gr_name = {""};
        int[] args = {0, 0, 0 ,0};
        int[] dim_sizes = {0, 0};
        HDFLibrary.GRgetiminfo(-1, gr_name, args, dim_sizes);
    }

    @Test(expected = NullPointerException.class)
    public void testGRgetiminfoNullName() throws Throwable {
        int[] args = {0, 0, 0 ,0};
        int[] dim_sizes = {0, 0};
        HDFLibrary.GRgetiminfo(0, null, args, dim_sizes);
    }

    @Test(expected = NullPointerException.class)
    public void testGRgetiminfoNullArgs() throws Throwable {
        String[] gr_name = {""};
        int[] dim_sizes = {0, 0};
        HDFLibrary.GRgetiminfo(0, gr_name, null, dim_sizes);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testGRgetiminfoArgumentArgs() throws Throwable {
        String[] gr_name = {""};
        int[] args = {0};
        int[] dim_sizes = {0, 0};
        HDFLibrary.GRgetiminfo(0, gr_name, args, dim_sizes);
    }

    @Test(expected = NullPointerException.class)
    public void testGRgetiminfoNullDims() throws Throwable {
        String[] gr_name = {""};
        int[] args = {0, 0, 0 ,0};
        HDFLibrary.GRgetiminfo(0, gr_name, args, null);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testGRgetiminfoArgumentDims() throws Throwable {
        String[] gr_name = {""};
        int[] args = {0, 0, 0 ,0};
        int[] dim_sizes = {0};
        HDFLibrary.GRgetiminfo(0, gr_name, args, dim_sizes);
    }

    @Test(expected = HDFException.class)
    public void testGRreadimageIllegalId() throws Throwable {
        int[] start = {0, 0};
        int[] stride = {0, 0};
        int[] count = {0, 0};
        byte[] data = {0};
        HDFLibrary.GRreadimage(-1, start, stride, count, data);
    }

    @Test(expected = NullPointerException.class)
    public void testGRreadimageNullData() throws Throwable {
        int[] start = {0, 0};
        int[] stride = {0, 0};
        int[] count = {0, 0};
        HDFLibrary.GRreadimage(0, start, stride, count, null);
    }

    @Test(expected = NullPointerException.class)
    public void testGRreadimageNullStart() throws Throwable {
        int[] stride = {0, 0};
        int[] count = {0, 0};
        byte[] data = {0};
        HDFLibrary.GRreadimage(0, null, stride, count, data);
    }

    @Test(expected = NullPointerException.class)
    public void testGRreadimageNullCount() throws Throwable {
        int[] start = {0, 0};
        int[] stride = {0, 0};
        byte[] data = {0};
        HDFLibrary.GRreadimage(0, start, stride, null, data);
    }

    @Test(expected = HDFException.class)
    public void testGRendaccessIllegalId() throws Throwable {
        HDFLibrary.GRendaccess(-1);
    }

    @Test(expected = HDFException.class)
    public void testGRidtorefIllegalId() throws Throwable {
        HDFLibrary.GRidtoref(-1);
    }

    @Test(expected = HDFException.class)
    public void testGRreftoindexIllegalId() throws Throwable {
        short ref = 0;
        HDFLibrary.GRreftoindex(-1, ref);
    }

    @Test(expected = HDFException.class)
    public void testGRreqlutilIllegalId() throws Throwable {
        int interlace = 0;
        HDFLibrary.GRreqlutil(-1, interlace);
    }

    @Test(expected = HDFException.class)
    public void testGRreqimageilIllegalId() throws Throwable {
        int interlace = 0;
        HDFLibrary.GRreqimageil(-1, interlace);
    }

    @Test(expected = HDFException.class)
    public void testGRgetlutidIllegalId() throws Throwable {
        int index = 0;
        HDFLibrary.GRgetlutid(-1, index);
    }

    @Test(expected = HDFException.class)
    public void testGRgetnlutsIllegalId() throws Throwable {
        HDFLibrary.GRgetnluts(-1);
    }

    @Test(expected = HDFException.class)
    public void testGRgetlutinfoIllegalId() throws Throwable {
        int[] args = {0, 0, 0, 0};
        HDFLibrary.GRgetlutinfo(-1, args);
    }

    @Test(expected = NullPointerException.class)
    public void testGRgetlutinfoNull() throws Throwable {
        HDFLibrary.GRgetlutinfo(0, null);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testGRgetlutinfoArgument() throws Throwable {
        int[] args = {0};
        HDFLibrary.GRgetlutinfo(0, args);
    }

    @Test(expected = HDFException.class)
    public void testGRreadlutIllegalId() throws Throwable {
        byte[] data = {0};
        HDFLibrary.GRreadlut(-1, data);
    }

    @Test(expected = NullPointerException.class)
    public void testGRreadlutNull() throws Throwable {
        HDFLibrary.GRreadlut(0, null);
    }

    @Test(expected = HDFException.class)
    public void testGRattrinfoIllegalId() throws Throwable {
        String[] name = {""};
        int index = 0;
        int[] argv = {0, 0};
        HDFLibrary.GRattrinfo(-1, index, name, argv);
    }

    @Test(expected = NullPointerException.class)
    public void testGRattrinfoNullName() throws Throwable {
        int index = 0;
        int[] argv = {0, 0};
        HDFLibrary.GRattrinfo(0, index, null, argv);
    }

    @Test(expected = NullPointerException.class)
    public void testGRattrinfoNullArgs() throws Throwable {
        String[] name = {""};
        int index = 0;
        HDFLibrary.GRattrinfo(0, index, name, null);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testGRattrinfoArgumentArgs() throws Throwable {
        String[] name = {""};
        int index = 0;
        int[] argv = {0};
        HDFLibrary.GRattrinfo(0, index, name, argv);
    }

    @Test(expected = HDFException.class)
    public void testGRgetattrIllegalId() throws Throwable {
        int index = 0;
        byte[] data = {0};
        HDFLibrary.GRgetattr(-1, index, data);
    }

    @Test(expected = NullPointerException.class)
    public void testGRgetattrNull() throws Throwable {
        int index = 0;
        HDFLibrary.GRgetattr(0, index, null);
    }

    @Test(expected = HDFException.class)
    public void testGRfindattrIllegalId() throws Throwable {
        String str = "";
        HDFLibrary.GRfindattr(-1, str);
    }

    @Test(expected = NullPointerException.class)
    public void testGRfindattrNull() throws Throwable {
        HDFLibrary.GRfindattr(0, null);
    }

    @Test(expected = HDFException.class)
    public void testGRcreateIllegalId() throws Throwable {
        String name = "";
        int ncomp = 0;
        long data_type = 0;
        int interlace_mode = 0;
        int[] dim_sizes = {0, 0};
        HDFLibrary.GRcreate(-1, name, ncomp, data_type, interlace_mode, dim_sizes);
    }

    @Test(expected = NullPointerException.class)
    public void testGRcreateNullName() throws Throwable {
        int ncomp = 0;
        long data_type = 0;
        int interlace_mode = 0;
        int[] dim_sizes = {0, 0};
        HDFLibrary.GRcreate(0, null, ncomp, data_type, interlace_mode, dim_sizes);
    }

    @Test(expected = NullPointerException.class)
    public void testGRcreateNullDims() throws Throwable {
        String name = "";
        int ncomp = 0;
        long data_type = 0;
        int interlace_mode = 0;
        HDFLibrary.GRcreate(0, name, ncomp, data_type, interlace_mode, null);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testGRcreateArgumentDims() throws Throwable {
        String name = "";
        int ncomp = 0;
        long data_type = 0;
        int interlace_mode = 0;
        int[] dim_sizes = {0};
        HDFLibrary.GRcreate(0, name, ncomp, data_type, interlace_mode, dim_sizes);
    }

    @Test(expected = HDFException.class)
    public void testGRluttorefIllegalId() throws Throwable {
        HDFLibrary.GRluttoref(-1);
    }

    @Test(expected = HDFException.class)
    public void testGRsetattrStrIllegalId() throws Throwable {
        String name = "";
        long data_type = 0;
        int count = 0;
        String val = "";
        HDFLibrary.GRsetattr(-1, name, data_type, count, val);
    }

    @Test(expected = NullPointerException.class)
    public void testGRsetattrStrNullName() throws Throwable {
        long data_type = 0;
        int count = 0;
        String val = "";
        HDFLibrary.GRsetattr(0, null, data_type, count, val);
    }

    @Test(expected = NullPointerException.class)
    public void testGRsetattrStrNullVals() throws Throwable {
        String name = "";
        long data_type = 0;
        int count = 0;
        String val = null;
        HDFLibrary.GRsetattr(0, name, data_type, count, val);
    }

    @Test(expected = HDFException.class)
    public void testGRsetattrIllegalId() throws Throwable {
        String name = "";
        long data_type = 0;
        int count = 0;
        byte[] data = {0};
        HDFLibrary.GRsetattr(-1, name, data_type, count, data);
    }

    @Test(expected = NullPointerException.class)
    public void testGRsetattrNullName() throws Throwable {
        long data_type = 0;
        int count = 0;
        byte[] data = {0};
        HDFLibrary.GRsetattr(0, null, data_type, count, data);
    }

    @Test(expected = NullPointerException.class)
    public void testGRsetattrNullVals() throws Throwable {
        String name = "";
        long data_type = 0;
        int count = 0;
        byte[] data = null;
        HDFLibrary.GRsetattr(0, name, data_type, count, data);
    }

    @Test(expected = HDFException.class)
    public void testGRsetchunkIllegalId() throws Throwable {
        int flags = 0;
        HDFChunkInfo chunk_def = new HDFChunkInfo();
        HDFLibrary.GRsetchunk(-1, chunk_def, flags);
    }

    @Test(expected = NullPointerException.class)
    public void testGRsetchunkNull() throws Throwable {
        int flags = 0;
        HDFChunkInfo chunk_def = null;
        HDFLibrary.GRsetchunk(0, null, flags);
    }

    @Test(expected = HDFException.class)
    public void testGRsetchunkcacheIllegalId() throws Throwable {
        int maxcache = 0;
        int flags = 0;
        HDFLibrary.GRsetchunkcache(-1, maxcache, flags);
    }

    @Test(expected = HDFException.class)
    public void testGRsetcompressIllegalId() throws Throwable {
        int comp_type = 0;
        HDFCompInfo comp_info = new HDFCompInfo();
        HDFLibrary.GRsetcompress(-1, comp_type, comp_info);
    }

    @Test(expected = NullPointerException.class)
    public void testGRsetcompressNull() throws Throwable {
        int comp_type = 0;
        HDFLibrary.GRsetcompress(0, comp_type, null);
    }

    @Test(expected = HDFException.class)
    public void testGRgetcompressIllegalId() throws Throwable {
        HDFCompInfo comp_info = new HDFCompInfo();
        HDFLibrary.GRgetcompress(-1, comp_info);
    }

    @Test(expected = NullPointerException.class)
    public void testGRgetcompressNull() throws Throwable {
        HDFLibrary.GRgetcompress(0, null);
    }

    @Test(expected = HDFException.class)
    public void testGRsetexternalfileIllegalId() throws Throwable {
        String str = "";
        int offset = 0;
        HDFLibrary.GRsetexternalfile(-1, str, offset);
    }

    @Test(expected = NullPointerException.class)
    public void testGRsetexternalfileNull() throws Throwable {
        int offset = 0;
        HDFLibrary.GRsetexternalfile(0, null, offset);
    }

    @Test(expected = HDFException.class)
    public void testGRwriteimageIllegalId() throws Throwable {
        int[] start = {0, 0};
        int[] stride = {0, 0};
        int[] count = {0, 0};
        byte[] data = {0};
        HDFLibrary.GRwriteimage(-1, start, stride, count, data);
    }

    @Test(expected = NullPointerException.class)
    public void testGRwriteimageNullData() throws Throwable {
        int[] start = {0, 0};
        int[] stride = {0, 0};
        int[] count = {0, 0};
        HDFLibrary.GRwriteimage(0, start, stride, count, null);
    }

    @Test(expected = NullPointerException.class)
    public void testGRwriteimageNullStart() throws Throwable {
        int[] stride = {0, 0};
        int[] count = {0, 0};
        byte[] data = {0};
        HDFLibrary.GRwriteimage(0, null, stride, count, data);
    }

    @Test(expected = NullPointerException.class)
    public void testGRwriteimageNullCount() throws Throwable {
        int[] start = {0, 0};
        int[] stride = {0, 0};
        byte[] data = {0};
        HDFLibrary.GRwriteimage(0, start, stride, null, data);
    }

    @Test(expected = HDFException.class)
    public void testGRwritelutIllegalId() throws Throwable {
        int ncomp = 0;
        int data_type = 0;
        int interlace_mode = 0;
        int num_entries = 0;
        byte[] data = {0};
        HDFLibrary.GRwritelut(-1, ncomp, data_type, interlace_mode, num_entries, data);
    }

    @Test(expected = NullPointerException.class)
    public void testGRwritelutNull() throws Throwable {
        int ncomp = 0;
        int data_type = 0;
        int interlace_mode = 0;
        int num_entries = 0;
        HDFLibrary.GRwritelut(0, ncomp, data_type, interlace_mode, num_entries, null);
    }

    @Test(expected = HDFException.class)
    public void testGRreadchunkIllegalId() throws Throwable {
        int[] args = {0, 0};
        byte[] data = {0};
        HDFLibrary.GRreadchunk(-1, args, data);
    }

    @Test(expected = NullPointerException.class)
    public void testGRreadchunkNull() throws Throwable {
        int[] args = {0, 0};
        HDFLibrary.GRreadchunk(0, args, null);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testGRreadchunkArgument() throws Throwable {
        int[] args = {0};
        byte[] data = {0};
        HDFLibrary.GRreadchunk(0, args, data);
    }

    @Test(expected = NullPointerException.class)
    public void testGRreadchunkNullArgument() throws Throwable {
        byte[] data = {0};
        HDFLibrary.GRreadchunk(0, null, data);
    }
}

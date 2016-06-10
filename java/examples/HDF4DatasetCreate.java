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

package examples;

import hdf.hdflib.HDFConstants;
import hdf.hdflib.HDFLibrary;

/**
 * <p>
 * Title: HDF Native Package (Java) Example
 * </p>
 * <p>
 * Description: this example shows how to create HDF4 datasets using the
 * "HDF Native Package (Java)". The example creates the group structure and
 * datasets:
 *
 * <pre>
 *     "/" (root)
 *         integer arrays
 *             2D 32-bit integer 20x10
 *             3D unsigned 8-bit integer 20x10x5
 *         float arrays
 *             2D 64-bit double 20x10
 *             3D 32-bit float  20x10x5
 * </pre>
 *
 * </p>
 */
public class HDF4DatasetCreate {
    private static String fname  = "HDF4DatasetCreate.hdf";
    private static int[] dims2D = { 20, 10 };
    private static int[] dims3D = { 20, 10, 5 };

    public static void main(String args[]) throws Exception {
        long file_id = -1;
        long vgroup_id1 = -1;
        long vgroup_id2 = -1;
        long sd_id = -1;
        long sds_id = -1;

        // Create a new file using default properties.
        try {
            file_id = HDFLibrary.Hopen(fname, HDFConstants.DFACC_CREATE);
            // Initialize the V interface.
            if (file_id >= 0)
                HDFLibrary.Vstart(file_id);
        }
        catch (Exception e) {
            e.printStackTrace();
            System.err.println("Failed to create file:" + fname);
            return;
        }

        try {
            // Create the vgroup.  Note that the vgroup reference number is set
            // to -1 for creating and the access mode is "w" for writing.
            if (file_id >= 0) {
                vgroup_id1 = HDFLibrary.Vattach (file_id, -1, "w");
                if (vgroup_id1 >= 0) {
                    HDFLibrary.Vsetname(vgroup_id1, "integer arrays");
                }
                vgroup_id2 = HDFLibrary.Vattach (file_id, -1, "w");
                if (vgroup_id2 >= 0) {
                    HDFLibrary.Vsetname(vgroup_id2, "float arrays");
                }
            }

        }
        catch (Exception e) {
            e.printStackTrace();
        }

        try {
            // Create the file and initialize the SD interface.
            sd_id = HDFLibrary.SDstart (fname, HDFConstants.DFACC_CREATE);

            // create 2D 32-bit (4 bytes) integer dataset of 20 by 10
            if (sd_id >= 0)
                sds_id = HDFLibrary.SDcreate (sd_id, "2D 32-bit integer 20x10", (long)HDFConstants.DFNT_INT32, 2, dims2D);
        }
        catch (Exception e) {
            e.printStackTrace();
        }

        // Terminate access to the data set.
        try {
            if (sds_id > 0)
                HDFLibrary.SDendaccess(sds_id);
        }
        catch (Exception e) {
            e.printStackTrace();
        }

        try {
            // create 3D 8-bit (1 byte) unsigned integer dataset of 20 by 10 by 5
            if (sd_id >= 0)
                sds_id = HDFLibrary.SDcreate (sd_id, "3D 8-bit unsigned integer 20x10x5", (long)HDFConstants.DFNT_INT8, 3, dims3D);
        }
        catch (Exception e) {
            e.printStackTrace();
        }

        // Terminate access to the data set.
        try {
            if (sds_id > 0)
                HDFLibrary.SDendaccess(sds_id);
        }
        catch (Exception e) {
            e.printStackTrace();
        }

        try {
            // create 2D 64-bit (8 bytes) double dataset of 20 by 10
            if (sd_id >= 0)
                sds_id = HDFLibrary.SDcreate (sd_id, "2D 64-bit double 20x10", HDFConstants.DFNT_FLOAT64, 2, dims2D);
        }
        catch (Exception e) {
            e.printStackTrace();
        }

        // Terminate access to the data set.
        try {
            if (sds_id > 0)
                HDFLibrary.SDendaccess(sds_id);
        }
        catch (Exception e) {
            e.printStackTrace();
        }

        try {
            // create 3D 32-bit (4 bytes) float dataset of 20 by 10 by 5
            if (sd_id >= 0)
                sds_id = HDFLibrary.SDcreate (sd_id, "3D 32-bit float  20x10x5", HDFConstants.DFNT_FLOAT32, 3, dims3D);
        }
        catch (Exception e) {
            e.printStackTrace();
        }

        // Terminate access to the data set.
        try {
            if (sds_id > 0)
                HDFLibrary.SDendaccess(sds_id);
        }
        catch (Exception e) {
            e.printStackTrace();
        }

        // Terminate access to the SD interface and close the file.
        try {
            if (sd_id > 0)
                HDFLibrary.SDend(sd_id);
        }
        catch (Exception e) {
            e.printStackTrace();
        }

        // Close the groups.
        try {
            if (vgroup_id2 >= 0)
                HDFLibrary.Vdetach(vgroup_id2);
        }
        catch (Exception e) {
            e.printStackTrace();
        }
        try {
            if (vgroup_id1 >= 0)
                HDFLibrary.Vdetach(vgroup_id1);
        }
        catch (Exception e) {
            e.printStackTrace();
        }

        // Close the file.
        try {
            if (file_id >= 0) {
                HDFLibrary.Vend (file_id);
                HDFLibrary.Hclose(file_id);
            }
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }
}

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
 * Reference the C Example, VG_add_sds_to_vgroup.
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
        int sds_ref = -1;

        // Create a new file using default properties.
        try {
            file_id = HDFLibrary.Hopen(fname, HDFConstants.DFACC_CREATE);
            // Initialize the V interface.
            HDFLibrary.Vstart(file_id);
        }
        catch (Exception e) {
            e.printStackTrace();
            System.err.println("Failed to create file:" + fname);
            return;
        }

        // Create two vgroups and set their name and class.
        try {
            // Create the vgroup.  Note that the vgroup reference number is set
            // to -1 for creating and the access mode is "w" for writing.
            vgroup_id1 = HDFLibrary.Vattach (file_id, -1, "w");
            if (vgroup_id1 >= 0) {
                HDFLibrary.Vsetname(vgroup_id1, "integer arrays");
                HDFLibrary.Vsetclass (vgroup_id1, "Common Vgroups");
            }
            vgroup_id2 = HDFLibrary.Vattach (file_id, -1, "w");
            if (vgroup_id2 >= 0) {
                HDFLibrary.Vsetname(vgroup_id2, "float arrays");
                HDFLibrary.Vsetclass (vgroup_id2, "Common Vgroups");
            }
        }
        catch (Exception e) {
            e.printStackTrace();
        }

        // Initialize the SD interface.
        try {
            sd_id = HDFLibrary.SDstart (fname, HDFConstants.DFACC_WRITE);
        }
        catch (Exception e) {
            e.printStackTrace();
        }

        if (sd_id > 0) {
            // create the SDS, 2D 32-bit (4 bytes) integer dataset of 20 by 10
            try {
                sds_id = HDFLibrary.SDcreate (sd_id, "2D 32-bit integer 20x10", (long)HDFConstants.DFNT_INT32, 2, dims2D);
                if (sds_id >= 0) {
                    // Obtain the reference number of the SDS using its identifier.
                    sds_ref = HDFLibrary.SDidtoref(sds_id);
                    System.out.println("sds_ref:" + sds_ref);
                    // Add the SDS to the vgroup.  Note: the tag DFTAG_NDG is used
                    // when adding an SDS.  Refer to Appendix A for the entire list of tags.
                    HDFLibrary.Vaddtagref(vgroup_id1, HDFConstants.DFTAG_NDG, sds_ref);
                    // Terminate access to the data set.
                    HDFLibrary.SDendaccess(sds_id);
                }
            }
            catch (Exception e) {
                e.printStackTrace();
            }

            try {
                // create 3D 8-bit (1 byte) unsigned integer dataset of 20 by 10 by 5
                sds_id = HDFLibrary.SDcreate (sd_id, "3D 8-bit unsigned integer 20x10x5", (long)HDFConstants.DFNT_INT8, 3, dims3D);
                if (sds_id >= 0) {
                    // Obtain the reference number of the SDS using its identifier.
                    sds_ref = HDFLibrary.SDidtoref(sds_id);
                    System.out.println("sds_ref:" + sds_ref);
                    // Add the SDS to the vgroup.  Note: the tag DFTAG_NDG is used
                    // when adding an SDS.  Refer to Appendix A for the entire list of tags.
                    HDFLibrary.Vaddtagref(vgroup_id1, HDFConstants.DFTAG_NDG, sds_ref);
                    // Terminate access to the data set.
                    HDFLibrary.SDendaccess(sds_id);
                }
            }
            catch (Exception e) {
                e.printStackTrace();
            }

            try {
                // create 2D 64-bit (8 bytes) double dataset of 20 by 10
                sds_id = HDFLibrary.SDcreate (sd_id, "2D 64-bit double 20x10", HDFConstants.DFNT_FLOAT64, 2, dims2D);
                if (sds_id >= 0) {
                    // Obtain the reference number of the SDS using its identifier.
                    sds_ref = HDFLibrary.SDidtoref(sds_id);
                    System.out.println("sds_ref:" + sds_ref);
                    // Add the SDS to the vgroup.  Note: the tag DFTAG_NDG is used
                    // when adding an SDS.  Refer to Appendix A for the entire list of tags.
                    HDFLibrary.Vaddtagref(vgroup_id2, HDFConstants.DFTAG_NDG, sds_ref);
                    // Terminate access to the data set.
                    HDFLibrary.SDendaccess(sds_id);
                }
            }
            catch (Exception e) {
                e.printStackTrace();
            }

            try {
                // create 3D 32-bit (4 bytes) float dataset of 20 by 10 by 5
                sds_id = HDFLibrary.SDcreate (sd_id, "3D 32-bit float  20x10x5", HDFConstants.DFNT_FLOAT32, 3, dims3D);
                if (sds_id >= 0) {
                    // Obtain the reference number of the SDS using its identifier.
                    sds_ref = HDFLibrary.SDidtoref(sds_id);
                    System.out.println("sds_ref:" + sds_ref);
                    // Add the SDS to the vgroup.  Note: the tag DFTAG_NDG is used
                    // when adding an SDS.  Refer to Appendix A for the entire list of tags.
                    HDFLibrary.Vaddtagref(vgroup_id2, HDFConstants.DFTAG_NDG, sds_ref);
                    // Terminate access to the data set.
                    HDFLibrary.SDendaccess(sds_id);
                }
            }
            catch (Exception e) {
                e.printStackTrace();
            }

            // Terminate access to the SD interface and close the file.
            try {
                HDFLibrary.SDend(sd_id);
            }
            catch (Exception e) {
                e.printStackTrace();
            }
        }
        else {
            System.err.println("Could not initialize SDS interface");
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

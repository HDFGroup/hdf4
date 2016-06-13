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
 * Description: this example shows how to create HDF4 groups using the
 * "HDF Native Package (Java)". The example creates the group structure:
 *
 * <pre>
 *     "/" (root)
 *         g1
 *             g11
 *             g12
 *         g2
 *             g21
 *             g22
 * </pre>
 *
 * </p>
 */
public class HDF4GroupCreate {
    private static String fname = "HDF4GroupCreate.hdf";

    public static void main(String args[]) throws Exception {
        long file_id = -1;
        long subvgroup_id = -1;
        long vgroup_id1 = -1;
        long vgroup_id2 = -1;

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
                vgroup_id1 = HDFLibrary.VSattach (file_id, -1, "w");
                if (vgroup_id1 >= 0) {
                    HDFLibrary.VSsetname(vgroup_id1, "g1");
                    HDFLibrary.VSsetclass (vgroup_id1, "Empty Vdatas");
                }
                vgroup_id2 = HDFLibrary.VSattach (file_id, -1, "w");
                if (vgroup_id2 >= 0) {
                    HDFLibrary.VSsetname(vgroup_id2, "g2");
                    HDFLibrary.VSsetclass (vgroup_id2, "Empty Vdatas");
                }
            }

        }
        catch (Exception e) {
            e.printStackTrace();
        }

        // Close the groups.
        try {
            if (vgroup_id2 >= 0)
                HDFLibrary.VSdetach(vgroup_id2);
        }
        catch (Exception e) {
            e.printStackTrace();
        }
        try {
            if (vgroup_id1 >= 0)
                HDFLibrary.VSdetach(vgroup_id1);
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

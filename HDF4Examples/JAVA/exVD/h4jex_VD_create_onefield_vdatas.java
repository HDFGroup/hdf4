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

import hdf.hdflib.HDFConstants;
import hdf.hdflib.HDFLibrary;

public class h4jex_VD_create_onefield_vdatas {
    private static String FILE_NAME   = "h4jex_VD_create_onefield_vdatas.hdf";
    private static String CLASS1_NAME = "5x1 Array";
    private static String CLASS2_NAME = "6x4 Array";
    private static String VDATA1_NAME = "First Vdata";
    private static String VDATA2_NAME = "Second Vdata";
    private static String FIELD1_NAME = "Single-component Field";
    private static String FIELD2_NAME = "Multi-component Field";
    private static String VDATA_NAME  = "Vdata 1";
    private static String VDATA_CLASS = "Empty Vdatas";
    private static int N_RECORDS_1    = 5; // number of records the first vdata contains
    private static int N_RECORDS_2    = 6; // number of records the second vdata contains
    private static int ORDER_2        = 4; // order of the field in the second vdata
                                           // Note that the order of the field in the first vdata is 1

    public static void main(String args[]) throws Exception
    {
        //************************* Variable declaration **************************

        long file_id, vdata1_ref, vdata2_ref;

        // Define an array to buffer the data of the first vdata.
        byte[] vdata1_buf = {'V', 'D', 'A', 'T', 'A'};

        // Define an array to buffer the data of the second vdata.
        int[] vdata2_buf = {1, 2, 3, 4, 2, 4, 6, 8, 3, 6, 9, 12, 4, 8, 12, 16, 5, 10, 15, 20, 6, 12, 18, 24};

        //********************** End of variable declaration **********************

        // create file
        VD_create_vdatas();

        // Open the HDF file for writing.
        try {
            file_id = HDFLibrary.Hopen(FILE_NAME, HDFConstants.DFACC_WRITE);
        }
        catch (Exception e) {
            e.printStackTrace();
            System.err.println("Unable to open file " + FILE_NAME + "for writing.");
            return;
        }

        try {
            // Initialize the VS interface.
            HDFLibrary.Vstart(file_id);
            try {
                // Create the first vdata and populate it with data from the vdata1_buf
                // array.
                vdata1_ref = HDFLibrary.VHstoredata(file_id, FIELD1_NAME, vdata1_buf, N_RECORDS_1,
                                                    HDFConstants.DFNT_CHAR8, VDATA1_NAME, CLASS1_NAME);

                // Create the second vdata and populate it with data from the vdata2_buf array.
                vdata2_ref =
                    HDFLibrary.VHstoredatam(file_id, FIELD2_NAME, vdata2_buf, N_RECORDS_2,
                                            HDFConstants.DFNT_INT32, VDATA2_NAME, CLASS2_NAME, ORDER_2);
            }
            catch (Exception e) {
                e.printStackTrace();
                System.err.println("Cannot populate data");
            }
        }
        catch (Exception e) {
            e.printStackTrace();
            System.err.println("Cannot start V interface handling on the file " + FILE_NAME);
        }

        // Terminate access to the VS interface and close the HDF file.
        try {
            if (file_id >= 0) {
                HDFLibrary.Vend(file_id);
                HDFLibrary.Hclose(file_id);
            }
        }
        catch (Exception e) {
            e.printStackTrace();
        }
        System.out.println("VD_create_onefield_vdatas done");
    }

    public static void VD_create_vdatas() throws Exception
    {
        long file_id      = -1;
        long subvgroup_id = -1;
        long vgroup_id1   = -1;

        // Create a new file using default properties.
        try {
            file_id = HDFLibrary.Hopen(FILE_NAME, HDFConstants.DFACC_CREATE);
            // Initialize the V interface.
            if (file_id >= 0)
                HDFLibrary.Vstart(file_id);
        }
        catch (Exception e) {
            e.printStackTrace();
            System.err.println("Failed to create file:" + FILE_NAME);
            return;
        }

        try {
            // Create the vgroup.  Note that the vgroup reference number is set
            // to -1 for creating and the access mode is "w" for writing.
            if (file_id >= 0) {
                vgroup_id1 = HDFLibrary.VSattach(file_id, -1, "w");
                if (vgroup_id1 >= 0) {
                    HDFLibrary.VSsetname(vgroup_id1, VDATA_NAME);
                    HDFLibrary.VSsetclass(vgroup_id1, VDATA_CLASS);
                }
            }
        }
        catch (Exception e) {
            e.printStackTrace();
        }

        // Close the group.
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
                HDFLibrary.Vend(file_id);
                HDFLibrary.Hclose(file_id);
            }
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }
}

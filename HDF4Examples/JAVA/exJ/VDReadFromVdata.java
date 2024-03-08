/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF Products. The full HDF copyright                 *
 * notice, including terms governing use, modification, and redistribution,  *
 * is contained in the file, COPYING.  COPYING can be found at the root of   *
 * the source code distribution tree. You can also access it online  at      *
 * https://www.hdfgroup.org/licenses.  If you do not have access to the      *
 * file, you may request a copy from help@hdfgroup.org.                      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

import hdf.hdflib.HDFConstants;
import hdf.hdflib.HDFLibrary;

/**
 * <p>
 * Title: HDF Native Package (Java) Example
 * </p>
 * <p>
 * Description: this example shows how to read from a HDF4 vdata file using the
 * "HDF Native Package (Java)".
 * </p>
 */
public class VDReadFromVdata {
    private static String fname          = "General_Vdatas.hdf";
    private static String CLASS_NAME     = "Particle Data";
    private static String VDATA_NAME     = "Solid Particle";
    private static String FIELD1_NAME    = "Position";             /* contains x, y, z values */
    private static String FIELD2_NAME    = "Mass";                 /* contains weight values */
    private static String FIELD3_NAME    = "Temperature";          /* contains min and max values */
    private static String FIELDNAME_LIST = "Position,Temperature"; /* only two fields are read */
    private static int N_RECORDS         = 5;                      /* number of records the vdata contains */
    private static int ORDER_1           = 3;                      /* order of first field */
    private static int ORDER_2           = 2;                      /* order of second field */
    private static int N_VALS_PER_REC    = 5; /* number of values per record (ORDER_1 + ORDER_2) */
    private static int RECORD_INDEX      = 3; /* position where reading starts - 4th record */

    public static void main(String args[]) throws Exception
    {
        long file_id       = -1;
        int vdata_ref      = -1;
        long vdata_id      = -1;
        int num_of_records = -1;
        float data_buf[][] = new float[N_RECORDS][N_VALS_PER_REC]; /* buffer for vdata values */

        // Create a new file using default properties.
        try {
            file_id = HDFLibrary.Hopen(fname, HDFConstants.DFACC_READ);
            // Initialize the V interface.
            if (file_id >= 0)
                HDFLibrary.Vstart(file_id);
        }
        catch (Exception e) {
            e.printStackTrace();
            System.err.println("Failed to open file:" + fname);
            return;
        }

        try {
            /*
             * Get the reference number of the vdata, whose name is specified in
             * VDATA_NAME, using VSfind, which will be discussed in Section 4.7.3.
             */
            if (file_id >= 0) {
                vdata_ref = HDFLibrary.VSfind(file_id, VDATA_NAME);
                if (vdata_ref > 0) {
                    vdata_id = HDFLibrary.VSattach(file_id, vdata_ref, "r");
                }
            }
        }
        catch (Exception e) {
            e.printStackTrace();
        }

        try {
            /*
             * Specify the fields that will be read.
             */
            HDFLibrary.VSsetfields(vdata_id, FIELDNAME_LIST);
        }
        catch (Exception e) {
            e.printStackTrace();
        }

        try {
            /*
             * Place the current point to the position specified in RECORD_INDEX.
             */
            HDFLibrary.VSseek(vdata_id, RECORD_INDEX);
        }
        catch (Exception e) {
            e.printStackTrace();
        }

        try {
            /*
             * Read the next N_RECORDS records from the vdata and store the data
             * in the buffer databuf with fully interlaced mode.
             */
            num_of_records = HDFLibrary.VSread(vdata_id, data_buf, N_RECORDS, HDFConstants.FULL_INTERLACE);
        }
        catch (Exception e) {
            e.printStackTrace();
        }

        /*
         * Display the read data as many records as the number of records
         * returned by VSread.
         */
        System.out.println("       Particle Position        Temperature Range");
        for (int rec_num = 0; rec_num < num_of_records; rec_num++) {
            System.out.println("   " + data_buf[rec_num][0] + ", " + data_buf[rec_num][1] + ", " +
                               data_buf[rec_num][2] + ", " + data_buf[rec_num][3] + ", " +
                               data_buf[rec_num][4]);
        }

        // Close the groups.
        try {
            if (vdata_id >= 0)
                HDFLibrary.VSdetach(vdata_id);
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

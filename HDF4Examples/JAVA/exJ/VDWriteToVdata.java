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

/**
 * <p>
 * Title: HDF Native Package (Java) Example
 * </p>
 * <p>
 * Description: this example shows how to create a HDF4 vdata file using the
 * "HDF Native Package (Java)".
 * </p>
 */
public class VDWriteToVdata {
    private static String fname          = "General_Vdatas.hdf";
    private static String CLASS_NAME     = "Particle Data";
    private static String VDATA_NAME     = "Solid Particle";
    private static String FIELD1_NAME    = "Position";                  /* contains x, y, z values */
    private static String FIELD2_NAME    = "Mass";                      /* contains weight values */
    private static String FIELD3_NAME    = "Temperature";               /* contains min and max values */
    private static String FIELDNAME_LIST = "Position,Mass,Temperature"; /* No spaces b/w names */
    private static int N_RECORDS         = 10; /* number of records the vdata contains */
    private static int ORDER_1           = 3;  /* order of first field */
    private static int ORDER_2           = 1;  /* order of second field */
    private static int ORDER_3           = 2;  /* order of third field */
    private static int N_VALS_PER_REC    = 6;  /* number of values per record (ORDER_1 + ORDER_2 + ORDER_3) */

    public static void main(String args[]) throws Exception
    {
        long file_id       = -1;
        long vdata_id      = -1;
        float data_buf[][] = new float[N_RECORDS][N_VALS_PER_REC]; /* buffer for vdata values */

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
        System.out.println("File created:" + fname);

        try {
            // Create the vgroup.  Note that the vgroup reference number is set
            // to -1 for creating and the access mode is "w" for writing.
            if (file_id >= 0) {
                vdata_id = HDFLibrary.VSattach(file_id, -1, "w");
                if (vdata_id >= 0) {
                    HDFLibrary.VSsetname(vdata_id, VDATA_NAME);
                    HDFLibrary.VSsetclass(vdata_id, CLASS_NAME);
                }
            }
        }
        catch (Exception e) {
            e.printStackTrace();
        }

        try {
            /*
             * Introduce each field's name, data type, and order.  This is the first
             * part in defining a field.
             */
            HDFLibrary.VSfdefine(vdata_id, FIELD1_NAME, HDFConstants.DFNT_FLOAT32, ORDER_1);
            HDFLibrary.VSfdefine(vdata_id, FIELD2_NAME, HDFConstants.DFNT_FLOAT32, ORDER_2);
            HDFLibrary.VSfdefine(vdata_id, FIELD3_NAME, HDFConstants.DFNT_FLOAT32, ORDER_3);
        }
        catch (Exception e) {
            e.printStackTrace();
        }

        try {
            /*
             * Finalize the definition of the fields.
             */
            HDFLibrary.VSsetfields(vdata_id, FIELDNAME_LIST);
        }
        catch (Exception e) {
            e.printStackTrace();
        }

        /*
         * Buffer the data by the record for fully interlaced mode.  Note that the
         * first three elements contain the three values of the first field, the
         * fourth element contains the value of the second field, and the last two
         * elements contain the two values of the third field.
         */
        for (int rec_num = 0; rec_num < N_RECORDS; rec_num++) {
            data_buf[rec_num][0] = (float)(1.0 * rec_num);
            data_buf[rec_num][1] = (float)(2.0 * rec_num);
            data_buf[rec_num][2] = (float)(3.0 * rec_num);
            data_buf[rec_num][3] = (float)(0.1 + rec_num);
            data_buf[rec_num][4] = (float)0.0;
            data_buf[rec_num][5] = (float)65.0;
        }

        try {
            /*
             * Write the data from data_buf to the vdata with full interlacing mode.
             */
            HDFLibrary.VSwrite(vdata_id, data_buf, N_RECORDS, HDFConstants.FULL_INTERLACE);
        }
        catch (Exception e) {
            e.printStackTrace();
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

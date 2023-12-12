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

public class h4jex_SD_unlimited_sds {

    private static String FILE_NAME = "h4jex_SD_unlimited_sds.hdf";
    private static String SDS_NAME  = "AppendableData";
    private static int X_LENGTH     = 10;
    private static int Y_LENGTH     = 10;
    private static int RANK         = 2;

    public static void main(String args[]) throws Exception
    {
        //************************* Variable declaration **************************

        long sd_id;
        long sds_id;
        int sds_index;
        int[] dim_sizes   = new int[2];
        int[][] data      = new int[Y_LENGTH][X_LENGTH];
        int[] append_data = new int[X_LENGTH];
        int[] start       = new int[2];
        int[] edges       = new int[2];

        //********************** End of variable declaration **********************

        // Data initialization.
        for (int j = 0; j < Y_LENGTH; j++) {
            for (int i = 0; i < X_LENGTH; i++)
                data[j][i] = (i + 1) + (j + 1);
        }

        // Create the file and initialize the SD interface.
        try {
            sd_id = HDFLibrary.SDstart(FILE_NAME, HDFConstants.DFACC_CREATE);
        }
        catch (Exception e) {
            e.printStackTrace();
            System.err.println("Unable to open file " + FILE_NAME + "for writing.");
            return;
        }

        // Define dimensions of the array. Make the first dimension
        // appendable by defining its length to be unlimited.
        dim_sizes[0] = HDFConstants.SD_UNLIMITED;
        dim_sizes[1] = X_LENGTH;

        try {
            // Create the array data set.
            sds_id = HDFLibrary.SDcreate(sd_id, SDS_NAME, HDFConstants.DFNT_INT32, RANK, dim_sizes);

            // Define the location and the size of the data to be written
            // to the data set.
            start[0] = start[1] = 0;
            edges[0]            = Y_LENGTH;
            edges[1]            = X_LENGTH;

            try {
                // Write the data.
                HDFLibrary.SDwritedata(sds_id, start, null, edges, data);
                try {
                    // Terminate access to the array data set, terminate access
                    // to the SD interface, and close the file.
                    HDFLibrary.SDendaccess(sds_id);
                    HDFLibrary.SDend(sd_id);

                    // Store the array values to be appended to the data set.
                    for (int i = 0; i < X_LENGTH; i++)
                        append_data[i] = 1000 + i;

                    try {
                        // Reopen the file and initialize the SD interface.
                        sd_id = HDFLibrary.SDstart(FILE_NAME, HDFConstants.DFACC_WRITE);
                    }
                    catch (Exception e) {
                        e.printStackTrace();
                        System.err.println("Unable to reopen file " + FILE_NAME + "for writing.");
                        return;
                    }

                    // Select the first data set.
                    sds_index = 0;
                    try {
                        sds_id = HDFLibrary.SDselect(sd_id, sds_index);

                        // Check if selected SDS is unlimited. If it is not, then terminate access
                        // to the SD interface and close the file.
                        if (HDFLibrary.SDisrecord(sds_id)) {
                            // Define the location of the append to start at the first column
                            // of the 11th row of the data set and to stop at the end of the
                            // eleventh row.
                            start[0] = Y_LENGTH;
                            start[1] = 0;
                            edges[0] = 1;
                            edges[1] = X_LENGTH;
                            try {
                                // Append data to the data set.
                                HDFLibrary.SDwritedata(sds_id, start, null, edges, append_data);
                            }
                            catch (Exception e) {
                                e.printStackTrace();
                                System.err.println("Failed to append data.");
                            }
                        }
                    }
                    catch (Exception e) {
                        e.printStackTrace();
                        System.err.println("Could not select the first data set");
                    }
                }
                catch (Exception e) {
                    e.printStackTrace();
                    System.err.println("Failed to terminate access.");
                }
            }
            catch (Exception e) {
                e.printStackTrace();
                System.err.println("Failed to write data.");
            }
            // Terminate access to the data set.
            HDFLibrary.SDendaccess(sds_id);
        }
        catch (Exception e) {
            e.printStackTrace();
            System.err.println("Cannot create the array data set " + SDS_NAME);
        }
        // Terminate access to the SD interface and close the HDF file.
        HDFLibrary.SDend(sd_id);

        System.out.println("SD_unlimited_sds done");
    }
}

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

public class h4jex_GR_create_and_write_image {

    private static String FILE_NAME  = "h4ex_GR_create_and_write_image.hdf";
    private static String IMAGE_NAME = "Image Array 1";
    private static int X_LENGTH      = 10; // number of columns in the image
    private static int Y_LENGTH      = 5;  // number of rows in the image
    private static int N_COMPS       = 2;  // number of components in the image

    public static void main(String args[]) throws Exception
    {
        //************************* Variable declaration **************************

        long file_id;                 // HDF file identifier
        long gr_id;                   // GR interface identifier
        long ri_id;                   // raster image identifier
        int[] start     = new int[2]; // start position to write for each dimension
        int[] edges     = new int[2]; // number of elements to be written along each dimension
        int[] dim_sizes = new int[2]; // dimension sizes of the image array
        int interlace_mode;           // interlace mode of the image
        int data_type;                // data type of the image data
        short[][][] image_buf = new short[Y_LENGTH][X_LENGTH][N_COMPS];

        //********************** End of variable declaration **********************

        // Create and open the file.
        try {
            file_id = HDFLibrary.Hopen(FILE_NAME, HDFConstants.DFACC_CREATE);
        }
        catch (Exception e) {
            e.printStackTrace();
            System.err.println("Unable to open file " + FILE_NAME + "for writing.");
            return;
        }

        // Initialize the GR interface.
        try {
            gr_id = HDFLibrary.GRstart(file_id);

            // Set the data type, interlace mode, and dimensions of the image.
            data_type      = HDFConstants.DFNT_INT16;
            interlace_mode = HDFConstants.MFGR_INTERLACE_PIXEL;
            dim_sizes[0]   = X_LENGTH;
            dim_sizes[1]   = Y_LENGTH;
            try {
                // Create the raster image array.
                ri_id = HDFLibrary.GRcreate(gr_id, IMAGE_NAME, N_COMPS, data_type, interlace_mode, dim_sizes);

                // Fill the image data buffer with values.
                for (int i = 0; i < Y_LENGTH; i++) {
                    for (int j = 0; j < X_LENGTH; j++) {
                        image_buf[i][j][0] = (short)((i + j) + 1); // first component
                        image_buf[i][j][1] = (short)((i + j) + 1); // second component
                    }
                }

                // Define the size of the data to be written, i.e., start from the origin
                // and go as long as the length of each dimension.
                start[0] = start[1] = 0;
                edges[0]            = X_LENGTH;
                edges[1]            = Y_LENGTH;

                // Write the data in the buffer into the image array.
                try {
                    HDFLibrary.GRwriteimage(ri_id, start, null, edges, image_buf);
                }
                catch (Exception e) {
                    e.printStackTrace();
                    System.err.println("Unable to create the raster image array");
                }
                // Terminate access to the raster image
                HDFLibrary.GRendaccess(ri_id);
            }
            catch (Exception e) {
                e.printStackTrace();
                System.err.println("Unable to create the raster image array");
            }
            // Terminate access to the GR interface
            HDFLibrary.GRend(gr_id);
        }
        catch (Exception e) {
            e.printStackTrace();
            System.err.println("Cannot start GR interface handling on the file " + FILE_NAME);
        }
        HDFLibrary.Hclose(file_id);
        System.out.println("GR_create_and_write_image done");
    }
}

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

public class h4jex_AN_create_annotation {

    private static String FILE_NAME      = "h4jex_AN_create_annotation.hdf";
    private static String VG_NAME        = "AN Vgroup";
    private static String FILE_LABEL_TXT = "General HDF objects";
    private static String FILE_DESC_TXT  = "This is an HDF file that contains general HDF objects";
    private static String DATA_LABEL_TXT = "Common AN Vgroup";
    private static String DATA_DESC_TXT  = "This is a vgroup that is used to test data annotations";

    public static void main(String args[]) throws Exception
    {
        //************************* Variable declaration **************************

        long file_id;       // HDF file identifier
        long an_id;         // AN interface identifier
        long file_label_id; // file label identifier
        long file_desc_id;  // file description identifier
        long data_label_id; // data label identifier
        long data_desc_id;  // data description identifier
        long vgroup_id;
        int vgroup_tag;
        int vgroup_ref;

        //********************** End of variable declaration **********************

        // Create the HDF file.
        try {
            file_id = HDFLibrary.Hopen(FILE_NAME, HDFConstants.DFACC_CREATE);
        }
        catch (Exception e) {
            e.printStackTrace();
            System.err.println("Unable to open file " + FILE_NAME + "for writing.");
            return;
        }
        // Initialize the AN interface.
        try {
            an_id = HDFLibrary.ANstart(file_id);
            try {
                // Create the file label.
                file_label_id = HDFLibrary.ANcreatef(an_id, HDFConstants.AN_FILE_LABEL);
                try {
                    // Write the annotations to the file label.
                    HDFLibrary.ANwriteann(file_label_id, FILE_LABEL_TXT, FILE_LABEL_TXT.length());
                }
                catch (Exception e) {
                    e.printStackTrace();
                    System.err.println("Failed to write label annotation.");
                }
                try {
                    // Create file description.
                    file_desc_id = HDFLibrary.ANcreatef(an_id, HDFConstants.AN_FILE_DESC);
                    try {
                        // Write the annotation to the file description.
                        HDFLibrary.ANwriteann(file_desc_id, FILE_DESC_TXT, FILE_DESC_TXT.length());
                    }
                    catch (Exception e) {
                        e.printStackTrace();
                        System.err.println("Failed to write description annotation.");
                    }
                    try {
                        // Create a vgroup in the V interface.  Note that the vgroup's ref number
                        // is set to -1 for creating and the access mode is "w" for writing.
                        HDFLibrary.Vstart(file_id);
                        try {
                            vgroup_id = HDFLibrary.Vattach(file_id, -1, "w");
                            try {
                                HDFLibrary.Vsetname(vgroup_id, VG_NAME);
                            }
                            catch (Exception e) {
                                e.printStackTrace();
                                System.err.println("Could not name group.");
                            }
                            try {
                                // Obtain the tag and ref number of the vgroup for subsequent
                                // references.
                                vgroup_tag = HDFLibrary.VQuerytag(vgroup_id);
                                vgroup_ref = HDFLibrary.VQueryref(vgroup_id);

                                // Create the data label for the vgroup identified by its tag
                                // and ref number.
                                data_label_id = HDFLibrary.ANcreate(
                                    an_id, (short)vgroup_tag, (short)vgroup_ref, HDFConstants.AN_DATA_LABEL);
                                try {
                                    // Write the annotation text to the data label.

                                    HDFLibrary.ANwriteann(data_label_id, DATA_LABEL_TXT,
                                                          DATA_LABEL_TXT.length());
                                }
                                catch (Exception e) {
                                    e.printStackTrace();
                                    System.err.println("Failed to write data label annotation.");
                                }
                                try {
                                    // Create the data description for the vgroup identified by its tag
                                    // and ref number.
                                    data_desc_id =
                                        HDFLibrary.ANcreate(an_id, (short)vgroup_tag, (short)vgroup_ref,
                                                            HDFConstants.AN_DATA_DESC);
                                    try {
                                        // Write the annotation text to the data description.
                                        HDFLibrary.ANwriteann(data_desc_id, DATA_DESC_TXT,
                                                              DATA_DESC_TXT.length());
                                    }
                                    catch (Exception e) {
                                        e.printStackTrace();
                                        System.err.println("Failed to write data description annotation.");
                                    }
                                    HDFLibrary.ANendaccess(data_desc_id);
                                }
                                catch (Exception e) {
                                    e.printStackTrace();
                                    System.err.println("annot create a new data description annotation.");
                                }
                                HDFLibrary.ANendaccess(data_label_id);
                            }
                            catch (Exception e) {
                                e.printStackTrace();
                                System.err.println("Cannot create a new data label annotation.");
                            }
                            // Terminate access to the vgroup and to the V interface.
                            HDFLibrary.Vdetach(vgroup_id);
                        }
                        catch (Exception e) {
                            e.printStackTrace();
                            System.err.println("Could not attach to VGroup interface");
                        }
                        HDFLibrary.Vend(file_id);
                    }
                    catch (Exception e) {
                        e.printStackTrace();
                        System.err.println("Could not start VGroup interface");
                    }
                    HDFLibrary.ANendaccess(file_desc_id);
                }
                catch (Exception e) {
                    e.printStackTrace();
                    System.err.println("Cannot create a new file description annotation.");
                }
                HDFLibrary.ANendaccess(file_label_id);
            }
            catch (Exception e) {
                e.printStackTrace();
                System.err.println("Cannot create a new file label annotation.");
            }
            // Terminate access to the AN interface and close the HDF file.
            HDFLibrary.ANend(an_id);
        }
        catch (Exception e) {
            e.printStackTrace();
            System.err.println("Cannot start annotation handling on the file " + FILE_NAME);
        }
        HDFLibrary.Hclose(file_id);
        System.out.println("AN_create_annotation done");
    }
}

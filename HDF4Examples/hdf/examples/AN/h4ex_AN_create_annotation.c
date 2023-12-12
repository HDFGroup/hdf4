#include "hdf.h"

#define  FILE_NAME      "h4ex_AN_create_annotation.hdf"
#define  VG_NAME        "AN Vgroup"
#define  FILE_LABEL_TXT "General HDF objects"
#define  FILE_DESC_TXT  "This is an HDF file that contains general HDF objects"
#define  DATA_LABEL_TXT "Common AN Vgroup"
#define  DATA_DESC_TXT  "This is a vgroup that is used to test data annotations"

int main() {
    /************************* Variable declaration **************************/

    intn status_n; /* returned status for functions returning an intn  */
    int32 status_32; /* returned status for functions returning an int32 */
    int32 file_id; /* HDF file identifier */
    int32 an_id; /* AN interface identifier */
    int32 file_label_id; /* file label identifier */
    int32 file_desc_id; /* file description identifier */
    int32 data_label_id; /* data label identifier */
    int32 data_desc_id; /* data description identifier */
    int32 vgroup_id;
    uint16 vgroup_tag;
    uint16 vgroup_ref;
    int ret_val = FAIL;

    /********************** End of variable declaration **********************/

    /*
    * Create the HDF file.
    */
    file_id = Hopen(FILE_NAME, DFACC_CREATE, 0);
    if (file_id == FAIL)
        printf("\nUnable to open file %s for writing.\n", FILE_NAME);
    else {
        /*
        * Initialize the AN interface.
        */
        an_id = ANstart(file_id);
        if (an_id == FAIL)
            printf("\nCannot start annotation handling on the file %s.\n", FILE_NAME);
        else {
            /*
            * Create the file label.
            */
            file_label_id = ANcreatef(an_id, AN_FILE_LABEL);
            if (file_label_id == FAIL)
                printf("\nCannot create a new file label annotation.\n");
            else {
                /*
                * Write the annotations to the file label.
                */
                status_32 = ANwriteann(file_label_id, FILE_LABEL_TXT, (int32)strlen(FILE_LABEL_TXT));
                if (status_32 == FAIL)
                    printf("\nFailed to write label annotation.\n");

                /*
                * Create file description.
                */
                file_desc_id = ANcreatef(an_id, AN_FILE_DESC);
                if (file_desc_id == FAIL)
                    printf("\nCannot create a new file description annotation.\n");
                else {
                    /*
                    * Write the annotation to the file description.
                    */
                    status_32 = ANwriteann(file_desc_id, FILE_DESC_TXT, (int32)strlen(FILE_DESC_TXT));
                    if (status_32 == FAIL)
                        printf("\nFailed to write description annotation.\n");

                    /*
                    * Create a vgroup in the V interface.  Note that the vgroup's ref number
                    * is set to -1 for creating and the access mode is "w" for writing.
                    */
                    status_n = Vstart(file_id);
                    if (status_n == FAIL)
                        printf("\nCould not start VGroup interface\n");
                    else {
                        vgroup_id = Vattach(file_id, -1, "w");
                        if (vgroup_id == FAIL)
                            printf("\nCould not attach to VGroup interface\n");
                        else {
                            status_32 = Vsetname(vgroup_id, VG_NAME);
                            if (status_32 == FAIL)
                                printf("\nCould not name group.\n");

                            /*
                            * Obtain the tag and ref number of the vgroup for subsequent
                            * references.
                            */
                            vgroup_tag = (uint16) VQuerytag(vgroup_id);
                            vgroup_ref = (uint16) VQueryref(vgroup_id);

                            /*
                            * Create the data label for the vgroup identified by its tag
                            * and ref number.
                            */
                            data_label_id = ANcreate(an_id, vgroup_tag, vgroup_ref, AN_DATA_LABEL);
                            if (data_label_id == FAIL)
                                printf("\nCannot create a new data label annotation.\n");
                            else {
                                /*
                                * Write the annotation text to the data label.
                                */
                                status_32 = ANwriteann(data_label_id, DATA_LABEL_TXT, (int32)strlen(DATA_LABEL_TXT));
                                if (status_32 == FAIL)
                                    printf("\nFailed to write data label annotation.\n");

                                /*
                                * Create the data description for the vgroup identified by its tag
                                * and ref number.
                                */
                                data_desc_id = ANcreate(an_id, vgroup_tag, vgroup_ref, AN_DATA_DESC);
                                if (data_desc_id == FAIL)
                                    printf("\nCannot create a new data description annotation.\n");
                                else {
                                    /*
                                    * Write the annotation text to the data description.
                                    */
                                    status_32 = ANwriteann(data_desc_id, DATA_DESC_TXT, strlen(DATA_DESC_TXT));
                                    if (status_32 == FAIL) {
                                        printf("\nFailed to write data description annotation.\n");
                                    }
                                    else {
                                        ret_val = SUCCEED;
                                    }
                                    status_n = ANendaccess(data_desc_id);
                                }
                                status_n = ANendaccess(data_label_id);
                            }
                            /*
                            * Terminate access to the vgroup and to the V interface.
                            */
                            status_32 = Vdetach(vgroup_id);
                        }
                        status_n = Vend(file_id);
                    }
                    status_n = ANendaccess(file_desc_id);
                }
                status_n = ANendaccess(file_label_id);
            }
            /*
            * Terminate access to the AN interface and close the HDF file.
            */
            status_32 = ANend(an_id);
        }
        status_n = Hclose(file_id);
    }
    if (ret_val == SUCCEED)
        printf("\nSuccessful!\n");
    return ret_val;
}

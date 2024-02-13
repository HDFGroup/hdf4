#include <stdio.h>
#include <string.h>

#include "hdf.h"

#define FILE_NAME      "h4ex_AN_create_annotation.hdf"
#define VG_NAME        "AN Vgroup"
#define FILE_LABEL_TXT "General HDF objects"
#define FILE_DESC_TXT  "This is an HDF file that contains general HDF objects"
#define DATA_LABEL_TXT "Common AN Vgroup"
#define DATA_DESC_TXT  "This is a vgroup that is used to test data annotations"

int
main()
{
    /************************* Variable declaration **************************/

    int32 file_id,     /* HDF file identifier */
        an_id,         /* AN interface identifier */
        file_label_id, /* file label identifier */
        file_desc_id,  /* file description identifier */
        data_label_id, /* data label identifier */
        data_desc_id,  /* data description identifier */
        vgroup_id;
    uint16 vgroup_tag, vgroup_ref;

    /********************** End of variable declaration **********************/

    /*
     * Create the HDF file.
     */
    if ((file_id = Hopen(FILE_NAME, DFACC_CREATE, 0)) == FAIL)
        printf("*** ERROR from Hopen\n");

    /*
     * Initialize the AN interface.
     */
    if ((an_id = ANstart(file_id)) == FAIL)
        printf("*** ERROR from ANstart\n");

    /*
     * Create the file label.
     */
    if ((file_label_id = ANcreatef(an_id, AN_FILE_LABEL)) == FAIL)
        printf("*** ERROR from ANcreatef\n");

    /*
     * Write the annotations to the file label.
     */
    if (ANwriteann(file_label_id, FILE_LABEL_TXT, strlen(FILE_LABEL_TXT)) == FAIL)
        printf("*** ERROR from ANwriteann\n");

    /*
     * Create file description.
     */
    if ((file_desc_id = ANcreatef(an_id, AN_FILE_DESC)) == FAIL)
        printf("*** ERROR from ANcreatef\n");

    /*
     * Write the annotation to the file description.
     */
    if (ANwriteann(file_desc_id, FILE_DESC_TXT, strlen(FILE_DESC_TXT)) == FAIL)
        printf("*** ERROR from ANwriteann\n");

    /*
     * Create a vgroup in the V interface.  Note that the vgroup's ref number
     * is set to -1 for creating and the access mode is "w" for writing.
     */
    if (Vstart(file_id) == FAIL)
        printf("*** ERROR from Vstart\n");
    if ((vgroup_id = Vattach(file_id, -1, "w")) == FAIL)
        printf("*** ERROR from Vattach\n");
    if (Vsetname(vgroup_id, VG_NAME) == FAIL)
        printf("*** ERROR from Vsetname\n");

    /*
     * Obtain the tag and ref number of the vgroup for subsequent
     * references.
     */
    vgroup_tag = (uint16)VQuerytag(vgroup_id);
    vgroup_ref = (uint16)VQueryref(vgroup_id);

    /*
     * Create the data label for the vgroup identified by its tag
     * and ref number.
     */
    if ((data_label_id = ANcreate(an_id, vgroup_tag, vgroup_ref, AN_DATA_LABEL)) == FAIL)
        printf("*** ERROR from ANcreate\n");

    /*
     * Write the annotation text to the data label.
     */
    if (ANwriteann(data_label_id, DATA_LABEL_TXT, strlen(DATA_LABEL_TXT)) == FAIL)
        printf("*** ERROR from ANwriteann\n");

    /*
     * Create the data description for the vgroup identified by its tag
     * and ref number.
     */
    if ((data_desc_id = ANcreate(an_id, vgroup_tag, vgroup_ref, AN_DATA_DESC)) == FAIL)
        printf("*** ERROR from ANcreate\n");

    /*
     * Write the annotation text to the data description.
     */
    if (ANwriteann(data_desc_id, DATA_DESC_TXT, strlen(DATA_DESC_TXT)) == FAIL)
        printf("*** ERROR from ANwriteann\n");

    /*
     * Terminate access to the vgroup and to the V interface.
     */
    if (Vdetach(vgroup_id) == FAIL)
        printf("*** ERROR from Vdetach\n");
    if (Vend(file_id) == FAIL)
        printf("*** ERROR from Vend\n");

    /*
     * Terminate access to each annotation explicitly.
     */
    if (ANendaccess(file_label_id) == FAIL)
        printf("*** ERROR from ANendaccess(file_label_id)\n");
    if (ANendaccess(file_desc_id) == FAIL)
        printf("*** ERROR from ANendaccess(file_desc_id)\n");
    if (ANendaccess(data_label_id) == FAIL)
        printf("*** ERROR from ANendaccess(data_label_id)\n");
    if (ANendaccess(data_desc_id) == FAIL)
        printf("*** ERROR from ANendaccess(data_desc_id)\n");

    /*
     * Terminate access to the AN interface and close the HDF file.
     */
    if (ANend(an_id) == FAIL)
        printf("*** ERROR from ANend\n");
    if (Hclose(file_id) == FAIL)
        printf("*** ERROR from Hclose\n");
    return 0;
}

#include <stdio.h>

#include "hdf.h"   /* Note: in this example, hdf.h can be omitted...*/
#include "mfhdf.h" /* ...since mfhdf.h already includes hdf.h */

#define FILE_NAME "h4ex_VG_add_sds_to_vgroup.hdf"
#define SDS_NAME  "Test SD"
#define VG_NAME   "SD Vgroup"
#define VG_CLASS  "Common Vgroups"

int
main()
{
    /************************* Variable declaration **************************/

    int32 sd_id,      /* SD interface identifier */
        sds_id,       /* data set identifier */
        sds_ref,      /* reference number of the data set */
        dim_sizes[1], /* dimension of the data set - only one */
        rank = 1,     /* rank of the data set array */
        vgroup_id,    /* vgroup identifier */
        file_id;      /* HDF file identifier, same for V interface */

    /********************** End of variable declaration **********************/

    /*
     * Create the HDF file.
     */
    if ((file_id = Hopen(FILE_NAME, DFACC_CREATE, 0)) == FAIL)
        printf("*** ERROR from Hopen\n");

    /*
     * Initialize the V interface.
     */
    if (Vstart(file_id) == FAIL)
        printf("*** ERROR from Vstart\n");

    /*
     * Initialize the SD interface.
     */
    sd_id = SDstart(FILE_NAME, DFACC_WRITE);

    /*
     * Set the size of the SDS's dimension.
     */
    dim_sizes[0] = 10;

    /*
     * Create the SDS.
     */
    sds_id = SDcreate(sd_id, SDS_NAME, DFNT_INT32, rank, dim_sizes);

    /*
     * Create a vgroup and set its name and class.
     */
    vgroup_id = Vattach(file_id, -1, "w");
    if (Vsetname(vgroup_id, VG_NAME) == FAIL)
        printf("*** ERROR from Vsetname\n");
    if (Vsetclass(vgroup_id, VG_CLASS) == FAIL)
        printf("*** ERROR from Vsetclass\n");

    /*
     * Obtain the reference number of the SDS using its identifier.
     */
    sds_ref = SDidtoref(sds_id);

    /*
     * Add the SDS to the vgroup.  Note: the tag DFTAG_NDG is used
     * when adding an SDS.  Refer to Appendix A for the entire list of tags.
     */
    if (Vaddtagref(vgroup_id, DFTAG_NDG, sds_ref) == FAIL)
        printf("*** ERROR from Vaddtagref\n");

    /*
     * Terminate access to the SDS and to the SD interface.
     */
    if (SDendaccess(sds_id) == FAIL)
        printf("*** ERROR from SDendaccess\n");
    if (SDend(sd_id) == FAIL)
        printf("*** ERROR from SDend\n");

    /*
     * Terminate access to the vgroup and to the V interface, and
     * close the HDF file.
     */
    if (Vdetach(vgroup_id) == FAIL)
        printf("*** ERROR from Vdetach\n");
    if (Vend(file_id) == FAIL)
        printf("*** ERROR from Vend\n");
    if (Hclose(file_id) == FAIL)
        printf("*** ERROR from Hclose\n");

    return 0;
}

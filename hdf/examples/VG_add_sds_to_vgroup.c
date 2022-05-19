#include   "hdf.h"      /* Note: in this example, hdf.h can be omitted...*/
#include   "mfhdf.h"    /* ...since mfhdf.h already includes hdf.h */

#define  FILE_NAME    "General_Vgroups.hdf"
#define  SDS_NAME     "Test SD"
#define  VG_NAME      "SD Vgroup"
#define  VG_CLASS     "Common Vgroups"

int main()
{
   /************************* Variable declaration **************************/

   int32  sd_id,        /* SD interface identifier */
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
   file_id = Hopen (FILE_NAME, DFACC_CREATE, 0);

   /*
   * Initialize the V interface.
   */
   Vstart (file_id);

   /*
   * Initialize the SD interface.
   */
   sd_id = SDstart (FILE_NAME, DFACC_WRITE);

   /*
   * Set the size of the SDS's dimension.
   */
   dim_sizes[0] = 10;

   /*
   * Create the SDS.
   */
   sds_id = SDcreate (sd_id, SDS_NAME, DFNT_INT32, rank, dim_sizes);

   /*
   * Create a vgroup and set its name and class.
   */
   vgroup_id = Vattach (file_id, -1, "w");
   Vsetname (vgroup_id, VG_NAME);
   Vsetclass (vgroup_id, VG_CLASS);

   /*
   * Obtain the reference number of the SDS using its identifier.
   */
   sds_ref = SDidtoref (sds_id);

   /*
   * Add the SDS to the vgroup.  Note: the tag DFTAG_NDG is used
   * when adding an SDS.  Refer to Appendix A for the entire list of tags.
   */
   Vaddtagref (vgroup_id, DFTAG_NDG, sds_ref);

   /*
   * Terminate access to the SDS and to the SD interface.
   */
   SDendaccess (sds_id);
   SDend (sd_id);

   /*
   * Terminate access to the vgroup and to the V interface, and
   * close the HDF file.
   */
   Vdetach (vgroup_id);
   Vend (file_id);
   Hclose (file_id);
   return 0;
}

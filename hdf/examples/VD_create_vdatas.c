#include "hdf.h"

#define  FILE1_NAME     "General_Vdatas.hdf"
#define  FILE2_NAME     "Two_Vdatas.hdf"
#define  VDATA_NAME     "Vdata 1"
#define  VDATA_CLASS    "Empty Vdatas"

int main( )
{
   /************************* Variable declaration **************************/

   int32 file1_id, file2_id,
         vdata_id, vdata1_id, vdata2_id,
         vdata_ref = -1;     /* ref number of a vdata, set to -1 to create  */

   /********************** End of variable declaration **********************/

   /*
   * Create the first HDF file.
   */
   file1_id = Hopen (FILE1_NAME, DFACC_CREATE, 0);

   /*
   * Initialize the VS interface associated with the first HDF file.
   */
   Vstart (file1_id);

   /*
   * Create a vdata in the first HDF file.
   */
   vdata_id = VSattach (file1_id, vdata_ref, "w");

   /*
   * Assign a name to the vdata.
   */
   VSsetname (vdata_id, VDATA_NAME);

   /*
   * Other operations on the vdata identified by vdata_id can be carried
   * out starting from this point.
   */

   /*
   * Create the second HDF file.
   */
   file2_id = Hopen (FILE2_NAME, DFACC_CREATE, 0);

   /*
   * Initialize the VS interface associated with the second HDF file.
   */
   Vstart (file2_id);

   /*
   * Create the first vdata in the second HDF file.
   */
   vdata1_id = VSattach (file2_id, vdata_ref, "w");

   /*
   * Create the second vdata in the second HDF file.
   */
   vdata2_id = VSattach (file2_id, vdata_ref, "w");

   /*
   * Assign a class name to these vdatas.
   */
   VSsetclass (vdata1_id, VDATA_CLASS);
   VSsetclass (vdata2_id, VDATA_CLASS);

   /*
   * Other operations on the vdatas identified by vdata1_id and vdata2_id
   * can be carried out starting from this point.
   */

   /*
   * Terminate access to the first vdata in the second HDF file.
   */
   VSdetach (vdata1_id);

   /*
   * Terminate access to the second vdata in the second HDF file.
   */
   VSdetach (vdata2_id);

   /*
   * From this point on, any operations on the vdatas identified by vdata1_id
   and vdata2_id are invalid but not on the vdata identified by vdata_id.
   */

   /*
   * Terminate access to the VS interface associated with the second HDF file.
   */
   Vend (file2_id);

   /*
   * Close the second HDF file.
   */
   Hclose (file2_id);

   /*
   * Terminate access to the vdata in the first HDF file.
   */
   VSdetach (vdata_id);

   /*
   * Terminate access to the VS interface associated with the first HDF file.
   */
   Vend (file1_id);

   /*
   * Close the first HDF file.
   */
   Hclose (file1_id);
   return 0;
}

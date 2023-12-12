#include "mfhdf.h"

#define FILE_NAME      "SDS.hdf"
#define FILE_ATTR_NAME "File_contents"
#define SDS_ATTR_NAME  "Valid_range"
#define DIM_ATTR_NAME  "Dim_metric"

int main()
{
   /************************* Variable declaration **************************/

   int32   sd_id, sds_id, dim_id;
   intn    status;
   int32   attr_index, data_type, n_values; 
   char    attr_name[H4_MAX_NC_NAME];
   int8    *file_data;
   int8    *dim_data;
   float32 *sds_data;
   int     i;
   
   /********************* End of variable declaration ***********************/

   /*
   * Open the file and initialize SD interface.
   */
   sd_id = SDstart (FILE_NAME, DFACC_READ);

   /*
   * Find the file attribute defined by FILE_ATTR_NAME.
   */
   attr_index = SDfindattr (sd_id, FILE_ATTR_NAME);

   /*
   * Get information about the file attribute. Note that the first
   * parameter is an SD interface identifier.
   */
   status = SDattrinfo (sd_id, attr_index, attr_name, &data_type, &n_values);

   /*
   * Allocate a buffer to hold the attribute data.
   */
   file_data = (int8 *)malloc (n_values * sizeof (data_type));

   /*
   * Read the file attribute data.
   */
   status = SDreadattr (sd_id, attr_index, file_data);

   /*
   * Print out file attribute value. 
   */
   printf ("File attribute value is : %s\n", file_data);

   /*
   * Select the first data set.
   */
   sds_id = SDselect (sd_id, 0);

   /*
   * Find the data set attribute defined by SDS_ATTR_NAME. Note that the
   * first parameter is a data set identifier.
   */
   attr_index = SDfindattr (sds_id, SDS_ATTR_NAME);

   /*
   * Get information about the data set attribute.
   */
   status = SDattrinfo (sds_id, attr_index, attr_name, &data_type, &n_values);

   /*
   * Allocate a buffer to hold the data set attribute data.
   */
   sds_data = (float32 *)malloc (n_values * sizeof (data_type));

   /*
   * Read the SDS attribute data.
   */
   status = SDreadattr (sds_id, attr_index, sds_data);

   /*
   * Print out SDS attribute data type and values. 
   */
   if (data_type == DFNT_FLOAT32) 
                 printf ("SDS attribute data type is : float32\n");
   printf ("SDS attribute values are :  ");
   for (i=0; i<n_values; i++) printf (" %f", sds_data[i]);
   printf ("\n"); 

   /*    
   * Get the identifier for the second dimension of the SDS.
   */
   dim_id = SDgetdimid (sds_id, 1);

   /*
   * Find dimension attribute defined by DIM_ATTR_NAME.
   */
   attr_index = SDfindattr (dim_id, DIM_ATTR_NAME);

   /*
   * Get information about the dimension attribute.
   */
   status = SDattrinfo (dim_id, attr_index, attr_name, &data_type, &n_values);

   /*
   * Allocate a buffer to hold the dimension attribute data.
   */
   dim_data = (int8 *)malloc (n_values * sizeof (data_type));

   /*
   * Read the dimension attribute data.
   */
   status = SDreadattr (dim_id, attr_index, dim_data);

   /*
   * Print out dimension attribute value. 
   */
   printf ("Dimensional attribute values is : %s\n", dim_data);

   /*
   * Terminate access to the data set and to the SD interface and 
   * close the file.
   */
   status = SDendaccess (sds_id);
   status = SDend (sd_id);

   /*
   * Free all buffers.
   */
   free (dim_data);
   free (sds_data);
   free (file_data);

   /*   Output of this program is :
   *
   *    File attribute value is : Storm_track_data
   *    SDS attribute data type is : float32
   *    SDS attribute values are :   2.000000 10.000000
   *    Dimensional attribute values is : Seconds
   */

   return 0;
}

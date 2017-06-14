/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF/releases/.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Creating vdatas with names and classes that are longer than 64 characters
   to test bug HDFFR-1267. -BMR */

#include "hdf.h"

#define	FILENAME	"vslongname.hdf" 
#define	VDATA1_NAME	"Vdata 1 9112345678921234567893123456789412345678951234567896123456789 72"
#define	VDATA2_NAME	"Vdata 2 9112345678921234567893123456789412345678951234567896123456789 72"
#define	VDATA_CLASS	"Long Name Vdatas78921234567893123456789412345678951234567896123456789 72"


int main( )
{
   /************************* Variable declaration **************************/

   intn  status_n;	/* returned status for functions returning an intn  */
   int32 status_32;	/* returned status for functions returning an int32 */
   int32 file_id, file2_id, vdata_id, vdata1_id, vdata2_id;
   int32 vdata_ref = -1;     /* ref number of a vdata, set to -1 to create  */
   char vsclass[100], fields[10];

   /********************** End of variable declaration **********************/

   /* 
   * Create the first HDF file. 
   */
   file_id = Hopen (FILENAME, DFACC_CREATE, 0);

   /* 
   * Initialize the VS interface associated with the first HDF file. 
   */
   status_n = Vstart (file_id);

   /* 
   * Create a vdata in the first HDF file. 
   */
   vdata1_id = VSattach (file_id, -1, "w");
   vdata2_id = VSattach (file_id, -1, "w");

   /*
   * Assign name and class to the vdata.
   */
   status_32 = VSsetname (vdata1_id, VDATA1_NAME);
   status_32 = VSsetname (vdata2_id, VDATA2_NAME);

   status_32 = VSsetclass (vdata1_id, VDATA_CLASS);
   status_32 = VSsetclass (vdata2_id, VDATA_CLASS);

   /*
   * Add fields to the vdata.
   */
   status_n = VSfdefine (vdata1_id, "field 1", DFNT_FLOAT32, 1);
   status_n = VSfdefine (vdata2_id, "field 1", DFNT_FLOAT32, 1);

   status_n = VSsetfields (vdata1_id, "field 1");
   status_n = VSsetfields (vdata2_id, "field 1");

   /* 
   * Terminate access to the vdata in the first HDF file. 
   */
   status_32 = VSdetach (vdata1_id);
   status_32 = VSdetach (vdata2_id);

   /* 
   * Terminate access to the VS interface associated with the first HDF file. 
   */
   status_n = Vend (file_id);

   /* 
   * Close the first HDF file. 
   */
   status_n = Hclose (file_id);
   return 0;
}

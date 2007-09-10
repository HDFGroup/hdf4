/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the files COPYING and Copyright.html.  COPYING can be found at the root   *
 * of the source code distribution tree; Copyright.html can be found at      *
 * http://hdfgroup.org/products/hdf4/doc/Copyright.html.  If you do not have *
 * access to either file, you may request a copy from help@hdfgroup.org.     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include "hdf.h"
#include "mfhdf.h"
#include "hrepack_pal.h"



/*-------------------------------------------------------------------------
 * Function: copy_pal
 *
 * Purpose: list/copy lone palettes
 *
 *-------------------------------------------------------------------------
 */

int copy_pal(char* infname,char* outfname,int32 infile_id,int32 outfile_id,
              table_t *table,options_t *options)
{
 uint8  palette_data[256*3];
 intn   nPals, j;
 uint16 ref;
 
 if ( options->trip==0 ) 
 {
  return SUCCEED;
 }

 DFPrestart();
 
 if((nPals = DFPnpals (infname))==FAIL ) {
  printf( "Failed to get palettes in <%s>\n", infname);
  return FAIL;
 }
 
 for ( j = 0; j < nPals; j++) 
 {
  if (DFPgetpal(infname, (VOIDP)palette_data)==FAIL ) {
   printf( "Failed to read palette <%d> in <%s>\n", j, infname);
   continue;
  }
  
  ref=DFPlastref();
  
  /* check if already inserted in image */
  if ( table_search(table,DFTAG_IP8,ref)>=0 ){
   continue;
  }
  
  if (DFPaddpal(outfname,palette_data)==FAIL){
   printf( "Failed to write palette in <%s>\n", outfname);
   return FAIL;
  }
  
 }
 
 return SUCCEED;
}



/****************************************************************************
 * NCSA HDF                                                                 *
 * Software Development Group                                               *
 * National Center for Supercomputing Applications                          *
 * University of Illinois at Urbana-Champaign                               *
 * 605 E. Springfield, Champaign IL 61820                                   *
 *                                                                          *
 * For conditions of distribution and use, see the accompanying             *
 * hdf/COPYING file.                                                        *
 *                                                                          *
 ****************************************************************************/



#include <assert.h>
#include "hdf.h"
#include "mfhdf.h"

#include "hrepack_vg.h"
#include "hrepack_utils.h"
#include "hrepack_an.h"


/*-------------------------------------------------------------------------
 * Function: copy_vgroup_attrs
 *
 * Purpose: copy VG attributes
 *
 * Return: FAIL, SUCCEED
 *
 *-------------------------------------------------------------------------
 */

int copy_vgroup_attrs(int32 vg_in, int32 vg_out, char *path,options_t *options) 
{
 int    n_attrs;
 int32  data_type, size,  n_values;
 char   attr_name[H4_MAX_NC_NAME];
 int    i;
 char   *buf=NULL;

 if ( options->trip==0 ) 
 {
  return 1;
 }

 /* Get the number of attributes attached to this vgroup.  */
 if((n_attrs = Vnattrs (vg_in))==FAIL) {
  printf( "Failed to get attributes for <%s>\n", path);
  return-1;
 }
 
 for (i = 0; i < n_attrs; i++) 
 {
  if((Vattrinfo (vg_in, i, attr_name, &data_type, &n_values, &size))==FAIL) {
   printf( "Failed to get attribute %d of <%s>\n", i, path);
   continue;
  }
  if ((buf = (char *)malloc( (size_t) (size * n_values) ))==NULL ) {
   printf( "Failed to get memory for attribute %d of <%s>\n", i, path);
   continue;
  }
  if((Vgetattr (vg_in, i, buf))==FAIL){
   printf( "Failed to get attribute %d of <%s>\n", i, path);
   if (buf) free(buf);
   continue;
  }
  if((Vsetattr(vg_out, attr_name, data_type, n_values, buf))==FAIL){
   printf( "Failed to set attribute %d of <%s>\n", i, path);
  }
  if (buf) free(buf);
 }
 return 1;
}



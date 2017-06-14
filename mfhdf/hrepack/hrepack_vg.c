/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF/releases/.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


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
    if((n_attrs = Vnattrs2 (vg_in))==FAIL) {
        printf( "Failed to get attributes for <%s>\n", path);
        return-1;
    }
    
    for (i = 0; i < n_attrs; i++) 
    {
        if((Vattrinfo2(vg_in, i, attr_name, &data_type, &n_values, &size, NULL, NULL))==FAIL) {
            printf( "Failed to get attribute %d of <%s>\n", i, path);
            continue;
        }
        if ((buf = (char *)HDmalloc( (size_t) (size * n_values) ))==NULL ) {
            printf( "Failed to get memory for attribute %d of <%s>\n", i, path);
            continue;
        }
        if((Vgetattr2 (vg_in, i, buf))==FAIL){
            printf( "Failed to get attribute %d of <%s>\n", i, path);
            if (buf) HDfree(buf);
            continue;
        }
        if((Vsetattr(vg_out, attr_name, data_type, n_values, buf))==FAIL){
            printf( "Failed to set attribute %d of <%s>\n", i, path);
        }
        if (buf) HDfree(buf);
    }
    return 1;
}



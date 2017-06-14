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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "hdf.h"
#include "mfhdf.h"
#include "vg.h"
#include "hdiff.h"


/*-------------------------------------------------------------------------
 * Function: gattr_diff
 *
 * Purpose: compare global SDS attributes
 *
 *-------------------------------------------------------------------------
 */

uint32 gattr_diff(int32 sdid1, 
                  int32 sdid2,
                  diff_opt_t *opt)
{
 int32   nvars1, nvars2;      /* number of variables */
 int32   ngatts1, ngatts2;    /* number of global attributes */
 struct  ncatt att1, att2;    /* attribute */
 int     ia, ib;              /* attribute number */
 int     iret2;
 uint32  nfound=0;

 SDfileinfo(sdid1, &nvars1, &ngatts1);
 
 /* get global attributes */
 
 for (ia = 0; ia < ngatts1; ia++) 
 {
  SDattrinfo(sdid1, ia, att1.name, &att1.type, &att1.len);
  ib = SDfindattr(sdid2, att1.name);
  if (ib == -1)     /* attribute doesn't exist in file2 */
  {
   printf("\n---------------------------\n");
   printf("< %s\n", att1.name);
   printf("> '%s' does not exist in file2\n", att1.name);
   nfound ++;
   continue;
  }
  iret2 = SDattrinfo(sdid2, ib, att2.name, &att2.type, &att2.len);
  att1.val = (void *) malloc((unsigned) (att1.len*DFKNTsize(att1.type | DFNT_NATIVE)));
  if (!att1.val) 
  {
   fprintf(stderr,"Out of memory!\n");
   goto out;
  }
  att2.val = (void *) malloc((unsigned) (att2.len*DFKNTsize(att2.type | DFNT_NATIVE)));
  if (!att2.val) 
  {
   fprintf(stderr,"Out of memory!\n");
   goto out;
  }
  SDreadattr(sdid1, ia, att1.val);
  iret2 = SDreadattr(sdid2, ib, att2.val);
  
  iret2 = 0;
  if (att1.type != att2.type || att1.len != att2.len) 
   iret2 = 1;
  if (iret2 == 0)       /* compare the data */
   iret2 = memcmp((void *) att1.val, att2.val, 
   att1.len*DFKNTsize(att1.type | DFNT_NATIVE));
  
  if (iret2 != 0)
  {
   printf("\n---------------------------\n");
   printf("Attr Name: %s\n", att1.name);
   printf("< ");
   pr_att_vals((nc_type)att1.type, att1.len, att1.val);
   printf("\n> ");
   pr_att_vals((nc_type)att2.type, att2.len, att2.val);
   printf("\n");
   
   nfound ++;
  }
  
  free ((char *) att1.val);
  free ((char *) att2.val);
 }
 
 /* check any global attributes in file2 but not in file1 */
 
 iret2 =  SDfileinfo(sdid2, &nvars2, &ngatts2);
 
 for (ib = 0; ib < ngatts2; ib++) 
 {
  iret2 = SDattrinfo(sdid2, ib, att2.name, &att2.type, &att2.len);
  ia = SDfindattr(sdid1, att2.name);
  if (ia == -1)     /* attribute doesn't exist in file2 */
  {
   printf("\n---------------------------\n");
   printf("< '%s' does not exist in file1\n", att2.name);
   printf("> %s\n", att2.name);
   
   nfound ++;
  }
 }
 
 return nfound;


out:

 opt->err_stat = 1;
 return 0;
}


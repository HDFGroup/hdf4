#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "hdf.h"
#include "mfhdf.h"
#include "vg.h"

#include "hdiff.h"
#include "dumplib.h"

int
gattr_diff(int32 sdid1, int32 sdid2, struct fspec specp)
{
 int32   nvars1, nvars2;      /* number of variables */
 int32   ngatts1, ngatts2;    /* number of global attributes */
 struct  ncatt att1, att2;    /* attribute */
 int     ia, ib;              /* attribute number */
 int     iret1, iret2, ret=0;

 iret1 =  SDfileinfo(sdid1, &nvars1, &ngatts1);
 
 /* get global attributes */
 
 for (ia = 0; ia < ngatts1; ia++) 
 {
  iret1 = SDattrinfo(sdid1, ia, att1.name, &att1.type, &att1.len);
  ib = SDfindattr(sdid2, att1.name);
  if (ib == -1)     /* attribute doesn't exist in file2 */
  {
   printf("\n---------------------------\n");
   printf("< %s\n", att1.name);
   printf("> '%s' does not exist in file2\n", att1.name);
   ret=1;
   continue;
  }
  iret2 = SDattrinfo(sdid2, ib, att2.name, &att2.type, &att2.len);
  att1.val = (void *) malloc((unsigned) (att1.len*DFKNTsize(att1.type)));
  if (!att1.val) 
  {
   error("Out of memory!");
   SDend(sdid1);
   SDend(sdid2);
   exit(0);
  }
  att2.val = (void *) malloc((unsigned) (att2.len*DFKNTsize(att2.type)));
  if (!att2.val) 
  {
   error("Out of memory!");
   SDend(sdid1);
   SDend(sdid2);
   exit(0);
  }
  iret1 = SDreadattr(sdid1, ia, att1.val);
  iret2 = SDreadattr(sdid2, ib, att2.val);
  
  iret2 = 0;
  if (att1.type != att2.type || att1.len != att2.len) 
   iret2 = 1;
  if (iret2 == 0)       /* compare the data */
   iret2 = memcmp((void *) att1.val, att2.val, 
   att1.len*DFKNTsize(att1.type));
  
  if (iret2 != 0)
  {
   printf("\n---------------------------\n");
   printf("Attr Name: %s\n", att1.name);
   printf("< ");
   pr_att_vals(att1.type, att1.len, att1.val);
   printf("\n> ");
   pr_att_vals(att2.type, att2.len, att2.val);
   printf("\n");
   ret=1;
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
   ret=1;
  }
 }
 
 return ret;
}

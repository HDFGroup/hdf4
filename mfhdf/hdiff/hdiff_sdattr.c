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
sdattr_diff(int32 sdid1, int32 sdid2, struct fspec specp)
{
 int32 nvars1, nvars2;           /* number of variables */
 int32 ngatts1, ngatts2;         /* number of global attributes */
 int varid1, varid2;             /* variable id */
 struct ncvar var1, var2;        /* variable */
 struct ncatt att1, att2;        /* attribute */
 int id;                         /* dimension number per variable */
 int ia, ib;                     /* attribute number */
 int iv1, iv2;                   /* variable number */
 int iret1, iret2, iflag, istop, ret=0;

 iret1 =  SDfileinfo(sdid1, &nvars1, &ngatts1);
 iret2 =  SDfileinfo(sdid2, &nvars2, &ngatts2);
 
 /* check any sd in file2 but not in file1 */
 
 for (iv2 = 0; iv2 < nvars2; iv2++) 
 {
  varid2 = SDselect(sdid2, iv2);
  iret2 = SDgetinfo(varid2, var2.name, &var2.ndims, var2.dims, &var2.type,
   &var2.natts);
  
  iv1 = SDnametoindex(sdid1, var2.name);
  if (iv1 == -1)     /* sd doesn't exist in file2 */
  {
   SDendaccess(varid2);
   printf("\n---------------------------\n");
   printf("< '%s' does not exist in file1\n", var2.name);
   printf("> %s\n", var2.name);
   ret=1;
   continue;
  }
 }
 
 /* complete comparison of file1 to file2 */
 
 for (iv1 = 0; iv1 < nvars1; iv1++) 
 {
  varid1 = SDselect(sdid1, iv1);
  iret1 = SDgetinfo(varid1, var1.name, &var1.ndims, var1.dims, &var1.type,
   &var1.natts);
  
  iv2 = SDnametoindex(sdid2, var1.name);
  if (iv2 == -1)     /* sd doesn't exist in file2 */
  {
   SDendaccess(varid1);
   printf("\n---------------------------\n");
   printf("< %s\n", var1.name);
   printf("> '%s' does not exist in file2\n", var1.name);
   ret=1;
   continue;
  }
  
  varid2 = SDselect(sdid2, iv2);
  iret2 = SDgetinfo(varid2, var2.name, &var2.ndims, var2.dims, &var2.type,
   &var2.natts);
  
  iflag = 0;
  if (var1.ndims != var2.ndims || var1.type != var2.type || 
   var1.natts != var2.natts)
   iflag = 1;
  
  if (iflag == 0)
  {
   for (id = 0; id < var1.ndims; id++)
    if (var1.dims[id] != var2.dims[id])
     iflag = 1;
  }
  
  if (iflag == 0)    /* check attributes of the sd */
  {
   istop = 0;
   for (ia = 0; ia < var1.natts; ia++) 
   {
    iret1 = SDattrinfo(varid1, ia, att1.name, &att1.type, &att1.len);
    ib = SDfindattr(varid2, att1.name);
    if (ib == -1)
    {
     istop = 1;
     iflag = 1;
     continue;
    }
    iret2 = SDattrinfo(varid2, ib, att2.name, &att2.type, &att2.len);
    if (att1.type != att2.type || att1.len != att2.len)
    {
     istop = 1;
     iflag = 1;
     ret=1;
     continue;
    }
    
    att1.val = (void *) malloc((unsigned)att1.len*DFKNTsize(att1.type));
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
    iret1 = SDreadattr(varid1, ia, att1.val);
    iret2 = SDreadattr(varid2, ib, att2.val);
    
    iret1 = memcmp(att1.val, att2.val, att1.len*DFKNTsize(att1.type));
    
    if (iret1 != 0)
     iflag = 1;
    
    free ((char *) att1.val);
    free ((char *) att2.val);
   }
  }
  
  if (iflag == 0)    /* check if any attributes in var2 but not in var1 */
  {
   istop = 0;
   for (ib = 0; ib < var2.natts; ib++) 
   {
    iret2 = SDattrinfo(varid2, ib, att2.name, &att2.type, &att2.len);
    ia = SDfindattr(varid1, att2.name);
    if (ia == -1)
    {
     istop = 1;
     iflag = 1;
     continue;
    }
   }
  }
  
  if (iflag == 1)    /* print out the difference */
  {
   ret=1;
   printf("\n---------------------------\n");
   printf("SD Name: %s\n", var1.name);
   printf("<<<<\n");
   
   if (var1.ndims > 0)
    Printf ("Dimension: (");
   
   for (id = 0; id < var1.ndims; id++) 
    Printf ("%d%s", var1.dims[id], id < var1.ndims-1 ? ", " : ")");
   
   Printf (" ;\n");
   /* get variable attributes */
   for (ia = 0; ia < var1.natts; ia++) 
   {
    iret1 = SDattrinfo(varid1, ia, att1.name, &att1.type, &att1.len);
    
    Printf ("\t%s:%s = ", var1.name, att1.name);
    
    att1.val = (void *) malloc((unsigned)att1.len*DFKNTsize(att1.type));
    if (!att1.val) 
    {
     error("Out of memory!");
     SDend(sdid1);
     SDend(sdid2);
     exit(0);
    }
    iret1 = SDreadattr(varid1, ia, att1.val);
    pr_att_vals(att1.type, att1.len, att1.val);
    Printf (" ;\n");
    free ((char *) att1.val);
   }
   printf(">>>>\n");
   
   if (var2.ndims > 0)
    Printf ("Dimension: (");
   
   for (id = 0; id < var2.ndims; id++) 
    Printf ("%d%s", var2.dims[id], id < var2.ndims-1 ? ", " : ")");
   
   Printf (" ;\n");
   /* get variable attributes */
   for (ia = 0; ia < var2.natts; ia++) 
   {
    iret1 = SDattrinfo(varid2, ia, att2.name, &att2.type, &att2.len);
    
    Printf ("\t%s:%s = ", var2.name, att2.name);
    
    att2.val = (void *) malloc((unsigned)att2.len*DFKNTsize(att2.type));
    if (!att2.val) 
    {
     error("Out of memory!");
     SDend(sdid1);
     SDend(sdid2);
     exit(0);
    }
    iret2 = SDreadattr(varid2, ia, att2.val);
    pr_att_vals(att2.type, att2.len, att2.val);
    Printf (" ;\n");
    free ((char *) att2.val);
   }
  }
  SDendaccess(varid1);
  SDendaccess(varid2);
  }

 return ret;
}



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <float.h>


#include "hdf.h"
#include "mfhdf.h"
#include "vg.h"

#include "hdiff.h"
#include "dumplib.h"


int grdata_diff(int32 sdid1, int32 sdid2, struct fspec specp)
{
 int32    nvars1;              /* number of variables */
 int32    ngatts1;             /* number of global attributes */
 int      varid1, varid2;      /* variable id */
 int      iv1, iv2;            /* variable number */
 vnode*   vlist = newvlist();  /* list for vars specified with -v option */
 int      k, iret1, iret2;
 int      tot_cnt;
 int      tot_cnt1, tot_cnt2;
 int32    start[MAX_NC_DIMS];
 void     *buf1, *buf2;
 int      max_err_cnt, ret=0;
 typedef struct{
  char    name[MAX_NC_NAME];  /* name */
  int32   dims[2],            /* dimensions of an image */
          ncomps,             /* number of components an image contains */
          interlace,          /* interlace mode of an image */ 
          type,               /* number type of an image */
          natts;              /* number of attributes belong to an image */
 }grvar;
 grvar    var1, var2;     
 
  
/*
 * If any vars were specified with -v option, get list of associated
 * variable ids
 */
 for (k=0; k < specp.nlvars; k++) 
 {
  varid1 = GRnametoindex(sdid1, specp.lvars[k]);
  varadd(vlist, varid1);
 }

 iret1 =  GRfileinfo(sdid1, &nvars1, &ngatts1);
 
 for (k = 0; k < nvars1; k++)
 {
  varid1 = GRselect(sdid1, k);
  
  /* if var list specified, test for membership */
  if (specp.nlvars > 0 && ! varmember(vlist, k))
   continue;

  iret1 = GRgetiminfo(varid1, var1.name, &var1.ncomps, &var1.type, &var1.interlace, 
                      var1.dims, &var1.natts);
  
  iv2 = GRnametoindex(sdid2, var1.name);
  
  if (iv2 == -1)     /* sd doesn't exist in file2 */
  {
   GRendaccess(varid1);
   ret=1;
   continue;
  }
  
  varid2 = GRselect(sdid2, iv2);
  iret2  = GRgetiminfo(varid2, var2.name, &var2.ncomps, &var2.type, &var2.interlace,
                       var2.dims, &var2.natts);
  
  if (var1.type != var2.type || var1.ncomps!=var2.ncomps) 
  {
   GRendaccess(varid1);
   ret=1;
   continue;
  }
  
  if (specp.verbose) {
  printf("GR Name: %s .... Comparing\n", var1.name);
  }
  
  tot_cnt1 = 1;
  for (iv1 = 0; iv1 < 2; iv1++) 
  {
   start[iv1] = 0;
   tot_cnt1 *= var1.dims[iv1];
  }
  
  tot_cnt2 = 1;
  for (iv1 = 0; iv1 < 2; iv1++) 
  {
   start[iv1] = 0;
   tot_cnt2 *= var2.dims[iv1];
  }
  
  /* for images multiply with n_comps */
  buf1 = (void *) malloc((unsigned) (tot_cnt1*var1.ncomps*DFKNTsize(var1.type)));
  buf2 = (void *) malloc((unsigned) (tot_cnt2*var2.ncomps*DFKNTsize(var2.type)));
  
  iret1 = GRreadimage(varid1, start, (int32 *)NULL, var1.dims, buf1);
  iret2 = GRreadimage(varid2, start, (int32 *)NULL, var2.dims, buf2);
  
  /*  in case there is a difference in the elements, compare up to the
  limit of the less one */
  
  if (tot_cnt1 > tot_cnt2)
   tot_cnt = tot_cnt2;
  else
   tot_cnt = tot_cnt1; 
  
  /* If max_err_cnt is set (i.e. not its default -1), use it otherwise set it
  to tot_err_cnt so it doesn't trip  */
  max_err_cnt = (specp.max_err_cnt >= 0) ? specp.max_err_cnt : tot_cnt;
  ret=array_diff(buf1, buf2, tot_cnt, var1.type, specp.err_limit, 
   max_err_cnt, specp.statistics, NULL, NULL);
  free((char *) buf1);
  free((char *) buf2);
  
  GRendaccess(varid1);
  GRendaccess(varid2);
 }
 return ret;
}


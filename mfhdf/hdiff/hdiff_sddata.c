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




#define MYMAX(A,B) (((A) > (B)) ? (A) : (B))
#define MYMIN(A,B) (((A) < (B)) ? (A) : (B))
#define PRINT_FSTATS(T) {\
 printf("Type: %s  Npts: %d  Ndiff: %d (%lf%%)\n", \
 T, tot_cnt, n_diff, 100.*(double)n_diff/(double)tot_cnt); \
 printf("Avg Diff: %.3le  Max Diff: %.3le\n",  \
 d_avg_diff/tot_cnt, d_max_diff); \
 printf("Range File1: %lf/%lf  File2: %lf/%lf\n", \
d_min_val1, d_max_val1, d_min_val2, d_max_val2); }
#define PRINT_ISTATS(T) {\
 printf("Type: %s  Npts: %d  Ndiff: %d (%lf%%)\n", \
 T, tot_cnt,n_diff, 100.*(double)n_diff/(double)tot_cnt); \
 printf("Avg Diff: %le   Max. Diff: %ld\n",  \
 (d_avg_diff / tot_cnt), i4_max_diff); \
 printf("Range File1: %ld/%ld  File2: %ld/%ld\n", \
i4_min_val1, i4_max_val1, i4_min_val2, i4_max_val2); }


int sddata_diff(int32 sdid1, int32 sdid2, struct fspec specp)
{
 int32    nvars1;              /* number of variables */
 int32    ngatts1;             /* number of global attributes */
 int      varid1, varid2;      /* variable id */
 struct   ncvar var1, var2;    /* variable */
 int      iv1, iv2;            /* variable number */
 vnode*   vlist = newvlist();  /* list for vars specified with -v option */
 int      k, iret1, iret2;
 int      tot_cnt;
 int      tot_cnt1, tot_cnt2;
 int32    start[MAX_NC_DIMS];
 void     *buf1, *buf2;
 void     *fill1, *fill2;
 int      max_err_cnt, ret=0;
  
/*
 * If any vars were specified with -v option, get list of associated
 * variable ids
 */
 for (k=0; k < specp.nlvars; k++) 
 {
  varid1 = SDnametoindex(sdid1, specp.lvars[k]);
  varadd(vlist, varid1);
 }

 iret1 =  SDfileinfo(sdid1, &nvars1, &ngatts1);
 
 for (k = 0; k < nvars1; k++)
 {
  varid1 = SDselect(sdid1, k);
  
  /* if var list specified, test for membership */
  if (specp.nlvars > 0 && ! varmember(vlist, k))
   continue;
  
  iret1 = SDgetinfo(varid1, var1.name, &var1.ndims, var1.dims, &var1.type,
   &var1.natts);
  
  iv2 = SDnametoindex(sdid2, var1.name);
  
  if (iv2 == -1)     /* sd doesn't exist in file2 */
  {
   SDendaccess(varid1);
   ret=1;
   continue;
  }
  
  varid2 = SDselect(sdid2, iv2);
  iret2 = SDgetinfo(varid2, var2.name, &var2.ndims, var2.dims, &var2.type,
   &var2.natts);
  
  if (var1.ndims != var2.ndims || var1.type != var2.type) 
  {
   SDendaccess(varid1);
   ret=1;
   continue;
  }
  
  if (specp.verbose) {
  printf("SD Name: %s .... Comparing\n", var1.name);
  }
  
  tot_cnt1 = 1;
  for (iv1 = 0; iv1 < var1.ndims; iv1++) 
  {
   start[iv1] = 0;
   tot_cnt1 *= var1.dims[iv1];
  }
  
  tot_cnt2 = 1;
  for (iv1 = 0; iv1 < var2.ndims; iv1++) 
  {
   start[iv1] = 0;
   tot_cnt2 *= var2.dims[iv1];
  }
  
  buf1 = (void *) malloc((unsigned) (tot_cnt1*DFKNTsize(var1.type)));
  buf2 = (void *) malloc((unsigned) (tot_cnt2*DFKNTsize(var2.type)));
  
  iret1 = SDreaddata(varid1, start, (int32 *)NULL, var1.dims, buf1);
  iret2 = SDreaddata(varid2, start, (int32 *)NULL, var2.dims, buf2);
  
  fill1 = (void *)malloc((unsigned) (sizeof(double)));
  fill2 = (void *)malloc((unsigned) (sizeof(double)));
  
  iret1 = SDgetfillvalue(varid1, fill1);
  if (iret1 == FAIL) {
   free(fill1);
   fill1 = NULL;
  }
  iret2 = SDgetfillvalue(varid2, fill2);
  if (iret2 == FAIL) {
   free(fill2);
   fill2 = NULL;
  }
  
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
   max_err_cnt, specp.statistics, fill1, fill2);
  free((char *) buf1);
  free((char *) buf2);
  
  SDendaccess(varid1);
  SDendaccess(varid2);
 }
 return ret;
}


int array_diff(void *buf1, 
               void *buf2, 
               int32 tot_cnt, 
               int32 type, 
               float err_limit, 
               int32 max_err_cnt, 
               int32 statistics,
               void *fill1, 
               void *fill2)


{
 int32   i;
 int8    *i1ptr1, *i1ptr2;
 int16   *i2ptr1, *i2ptr2;
 int32   *i4ptr1, *i4ptr2;
 float32 *fptr1, *fptr2;
 float64 *dptr1, *dptr2;
 double  d_diff, d_avg_diff = 0., d_max_diff = 0.;
 double  d_max_val1, d_min_val1, d_max_val2, d_min_val2;
 double  d_val1, d_val2;
 double  d_sumx = 0., d_sumy = 0., d_sumx2 = 0., d_sumy2 = 0., d_sumxy=0.;
 double  slope, intercept, correlation;
 float   f_diff;
 long    i4_diff, i4_max_diff = 0;
 long    i4_max_val1, i4_min_val1, i4_max_val2, i4_min_val2;
 short   i2_diff;
 char    c_diff;
 int     n_diff = 0;
 int     is_fill1, is_fill2;
 int     n_stats = 0;
 char    *debug;
 FILE    *fp;
 debug = getenv("DEBUG");
 if (debug) {
  fp = fopen("hdiff.debug", "w");
 }
 
 switch(type) {
 case DFNT_INT8:
  i4_max_val1 = SCHAR_MIN;
  i4_min_val1 = SCHAR_MAX;
  i4_max_val2 = SCHAR_MIN;
  i4_min_val2 = SCHAR_MAX;
  break;
 case DFNT_UINT8:
 case DFNT_UCHAR8:
  i4_max_val1 = 0;
  i4_min_val1 = UCHAR_MAX;
  i4_max_val2 = 0;
  i4_min_val2 = UCHAR_MAX;
  break;
 case DFNT_INT16:
  i4_max_val1 = SHRT_MIN;
  i4_min_val1 = SHRT_MAX;
  i4_max_val2 = SHRT_MIN;
  i4_min_val2 = SHRT_MAX;
  break;
 case DFNT_UINT16:
  i4_max_val1 = 0;
  i4_min_val1 = USHRT_MAX;
  i4_max_val2 = 0;
  i4_min_val2 = USHRT_MAX;
  break;
 case DFNT_INT32:
  i4_max_val1 = LONG_MIN;
  i4_min_val1 = LONG_MAX;
  i4_max_val2 = LONG_MIN;
  i4_min_val2 = LONG_MAX;
  break;
 case DFNT_UINT32:
  i4_max_val1 = 0;
  i4_min_val1 = ULONG_MAX;
  i4_max_val2 = 0;
  i4_min_val2 = ULONG_MAX;
  break;
 case DFNT_FLOAT:
  d_max_val1 = -FLT_MAX;
  d_min_val1 = FLT_MAX;
  d_max_val2 = -FLT_MAX;
  d_min_val2 = FLT_MAX;
  break;
 case DFNT_DOUBLE:
  d_max_val1 = -DBL_MAX;
  d_min_val1 = DBL_MAX;
  d_max_val2 = -DBL_MAX;
  d_min_val2 = DBL_MAX;
  break;
 default:
  error(" bad type - %d\n", type);
 }
 switch(type)
 {
 case DFNT_INT8:
 case DFNT_UINT8:
 case DFNT_UCHAR8:
  i1ptr1 = (char *) buf1;
  i1ptr2 = (char *) buf2;
  for (i=0; i<tot_cnt; i++)
  {
   c_diff = abs(*i1ptr1 - *i1ptr2);
   is_fill1 = fill1 && (*i1ptr1 == *((char *)fill1));
   is_fill2 = fill2 && (*i1ptr2 == *((char *)fill2));
   if (!is_fill1 && !is_fill2) {
    d_avg_diff += (double)c_diff;
    i4_max_diff = MYMAX(i4_max_diff, c_diff);
    d_val2 = (double)(*i1ptr2);
    d_val1 = (double)(*i1ptr1);
    d_sumx += d_val1;
    d_sumy += d_val2;
    d_sumx2 += d_val1 * d_val1;
    d_sumy2 += d_val2 * d_val2;
    d_sumxy += d_val1 * d_val2;
    n_stats++;
   }
   if (!is_fill1) {
    i4_max_val1 = MYMAX(i4_max_val1, (long)(*i1ptr1));
    i4_min_val1 = MYMIN(i4_min_val1, (long)(*i1ptr1));
   }
   if (!is_fill2) {
    i4_max_val2 = MYMAX(i4_max_val2, (long)(*i1ptr2));
    i4_min_val2 = MYMIN(i4_min_val2, (long)(*i1ptr2));
   }
   if (c_diff > (int) err_limit)
   {
    n_diff++;
    if (n_diff <= max_err_cnt) {
     printf("Index: %d,   File1: %d,   File2: %d\n",
      i, *i1ptr1, *i1ptr2);
    }
   }                                               
   i1ptr1++;  i1ptr2++;
  }
  if (statistics) {
   PRINT_ISTATS("Byte");
  }
  
  break;
  
 case DFNT_INT16:
 case DFNT_UINT16:
  i2ptr1 = (short *) buf1;
  i2ptr2 = (short *) buf2;
  for (i=0; i<tot_cnt; i++)
  {
   i2_diff = abs(*i2ptr1 - *i2ptr2);
   is_fill1 = fill1 && (*i2ptr1 == *((short *)fill1));
   is_fill2 = fill2 && (*i2ptr2 == *((short *)fill2));
   if (debug) {
    fprintf(fp, "%d %d %d %d\n", is_fill1, is_fill2, (int)(*i2ptr1), (int)(*i2ptr2));
   }
   if (!is_fill1 && !is_fill2) {
    d_val1 = (double)(*i2ptr1);
    d_val2 = (double)(*i2ptr2);
    d_sumx += d_val1;
    d_sumy += d_val2;
    d_sumx2 += d_val1 * d_val1;
    d_sumy2 += d_val2 * d_val2;
    d_sumxy += d_val1 * d_val2;
    d_avg_diff += (double)i2_diff;
    i4_max_diff = MYMAX(i4_max_diff, i2_diff);
    n_stats++;
   }
   if (!is_fill1) {
    i4_max_val1 = MYMAX(i4_max_val1, (long)(*i2ptr1));
    i4_min_val1 = MYMIN(i4_min_val1, (long)(*i2ptr1));
   }
   if (!is_fill2) {
    i4_max_val2 = MYMAX(i4_max_val2, (long)(*i2ptr2));
    i4_min_val2 = MYMIN(i4_min_val2, (long)(*i2ptr2));
   }
   if (i2_diff > (int) err_limit)
   {
    n_diff++;
    if (n_diff <= max_err_cnt) {
     printf("Index: %d,   File1: %d,   File2: %d\n",
      i, *i2ptr1, *i2ptr2);
    }
   }                                               
   i2ptr1++;  i2ptr2++;
  }
  if (statistics) {
   PRINT_ISTATS("Integer2");
  }
  break;
  
 case DFNT_INT32:
 case DFNT_UINT32:
  i4ptr1 = (int32 *) buf1;
  i4ptr2 = (int32 *) buf2;
  for (i=0; i<tot_cnt; i++)
  {
   i4_diff = labs(*i4ptr1 - *i4ptr2);
   is_fill1 = fill1 && (*i4ptr1 == *((int32 *)fill1));
   is_fill2 = fill2 && (*i4ptr2 == *((int32 *)fill2));
   if (!is_fill1 && !is_fill2) {
    d_avg_diff += (double)i4_diff;
    d_val1 = (double)(*i4ptr1);
    d_val2 = (double)(*i4ptr2);
    d_sumx += d_val1;
    d_sumy += d_val2;
    d_sumx2 += d_val1 * d_val1;
    d_sumy2 += d_val2 * d_val2;
    d_sumxy += d_val1 * d_val2;
    i4_max_diff = (long)MYMAX(i4_max_diff, (double)(i4_diff));
    n_stats++;
   }
   if (! is_fill1) {
    i4_max_val1 = MYMAX(i4_max_val1, (int32)(*i4ptr1));
    i4_min_val1 = MYMIN(i4_min_val1, (int32)(*i4ptr1));
   }
   if (! is_fill2) {
    i4_max_val2 = MYMAX(i4_max_val2, (int32)(*i4ptr2));
    i4_min_val2 = MYMIN(i4_min_val2, (int32)(*i4ptr2));
   }
   if (i4_diff > (int32) err_limit)
   {
    n_diff++;
    if (n_diff <= max_err_cnt) {
     printf("Index: %d,   File1: %ld,   File2: %ld\n",
      i, *i4ptr1, *i4ptr2);
    }
   }                                               
   i4ptr1++;  i4ptr2++;
  }
  if (statistics) {
   PRINT_ISTATS("Integer4");
  }
  
  break;
  
 case DFNT_FLOAT:
  fptr1 = (float *) buf1;
  fptr2 = (float *) buf2;
  for (i=0; i<tot_cnt; i++)
  {
   f_diff = (float)fabs(*fptr1 - *fptr2);
   is_fill1 = fill1 && (*fptr1 == *((float *)fill1));
   is_fill2 = fill2 && (*fptr2 == *((float *)fill2));
   if (debug) {
    fprintf(fp, "%d %d %f %f\n", is_fill1, is_fill2, *fptr1, *fptr2);
   }
   if (!is_fill1 && !is_fill2) {
    d_avg_diff += (double)f_diff;
    d_val1 = (double)(*fptr1);
    d_val2 = (double)(*fptr2);
    d_sumx += d_val1;
    d_sumy += d_val2;
    d_sumx2 += d_val1 * d_val1;
    d_sumy2 += d_val2 * d_val2;
    d_sumxy += d_val1 * d_val2;
    d_max_diff = MYMAX(d_max_diff, (double)(f_diff));
    n_stats++;
   }
   if (!is_fill1) {
    d_max_val1 = MYMAX(d_max_val1, (double)(*fptr1));
    d_min_val1 = MYMIN(d_min_val1, (double)(*fptr1));
   }
   if (!is_fill2) {
    d_max_val2 = MYMAX(d_max_val2, (double)(*fptr2));
    d_min_val2 = MYMIN(d_min_val2, (double)(*fptr2));
   }
   if (f_diff > err_limit)
   {
    n_diff++;
    if (n_diff <= max_err_cnt) {
     printf("Index: %d,   File1: %#.8g,   File2: %#.8g\n",
      i, *fptr1, *fptr2);
    }
   }                                               
   fptr1++;  fptr2++;
  }
  if (statistics) {
   PRINT_FSTATS("Float");
  }
  break;
  
 case DFNT_DOUBLE:
  dptr1 = (double *) buf1;
  dptr2 = (double *) buf2;
  for (i=0; i<tot_cnt; i++)
  {
   d_diff = fabs(*dptr1 - *dptr2);
   is_fill1 = fill1 && (*dptr1 == *((double *)fill1));
   is_fill2 = fill2 && (*dptr2 == *((double *)fill2));
   if (!is_fill1 && !is_fill2) {
    d_avg_diff += d_diff;
    d_val1 = (double)(*dptr1);
    d_val2 = (double)(*dptr2);
    d_sumx += d_val1;
    d_sumy += d_val2;
    d_sumx2 += d_val1 * d_val1;
    d_sumy2 += d_val2 * d_val2;
    d_sumxy += d_val1 * d_val2;
    d_max_diff = MYMAX(d_max_diff, (d_diff));
    n_stats++;
   }
   if (! is_fill1) {
    d_max_val1 = MYMAX(d_max_val1, (*dptr1));
    d_min_val1 = MYMIN(d_min_val1, (*dptr1));
   }
   if (! is_fill2) {
    d_max_val2 = MYMAX(d_max_val2, (*dptr2));
    d_min_val2 = MYMIN(d_min_val2, (*dptr2));
   }
   if (d_diff > (double) err_limit)
   {
    n_diff++;
    if (n_diff <= max_err_cnt) {
     printf("Index: %d,   File1: %#.16g,   File2: %#.16g\n",
      i, *dptr1, *dptr2);
    }
   }
   dptr1++;  dptr2++;
  }
  if (statistics) {
   PRINT_FSTATS("Double");
  }
  break;
  
 default:
  error(" bad type - %d\n", type);
  }
  if (statistics) {
   double sqrt_arg;
   if ((double)n_stats * d_sumx2 - d_sumx * d_sumx != 0.0) {
    slope = ((double)n_stats * d_sumxy - d_sumx * d_sumy) / 
     ((double)n_stats * d_sumx2 - d_sumx * d_sumx);
    intercept = (d_sumy - slope * d_sumx) / (double)n_stats;
    sqrt_arg = ((double)n_stats*d_sumx2 - d_sumx*d_sumx) /
     ((double)n_stats * d_sumy2 - d_sumy * d_sumy);
    correlation = slope * sqrt(sqrt_arg);
    printf("Regression  N: %d  Slope: %le  Intercept: %le  R: %le\n",
     n_stats, slope, intercept, correlation);
   }
   else {
    printf("Regression  Slope: NaN  Intercept: NaN  R: NaN\n");
   }
  }
  if (debug) {
   fclose(fp);
  }
  return (n_diff>0 ? 1 : 0 );
}

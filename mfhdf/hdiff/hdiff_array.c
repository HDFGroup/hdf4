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
#include <math.h>
#include <limits.h>
#include <float.h>
#include <assert.h>
#include "hdf.h"
#include "mfhdf.h"
#include "hdiff.h"
#include "vgint.h"

#ifndef ABS
#   define ABS(a)		(((a)>=0) ? (a) : -(a))
#endif


#define MYMAX(A,B) (((A) > (B)) ? (A) : (B))
#define MYMIN(A,B) (((A) < (B)) ? (A) : (B))
#define PRINT_FSTATS(T) {\
 printf("Type: %s  Npts: %lu  Ndiff: %lu (%f%%)\n", \
 T, tot_cnt, n_diff, 100.*(float64)n_diff/(float64)tot_cnt); \
 printf("Avg Diff: %.3e  Max Diff: %.3e\n",  \
 d_avg_diff/n_stats, d_max_diff); \
 printf("Range File1: %f/%f  File2: %f/%f\n", \
d_min_val1, d_max_val1, d_min_val2, d_max_val2); }
#define PRINT_ISTATS(T) {\
 printf("Type: %s  Npts: %lu  Ndiff: %lu (%f%%)\n", \
 T, tot_cnt,n_diff, 100.*(float64)n_diff/(float64)tot_cnt); \
 printf("Avg Diff: %e   Max. Diff: %ld\n",  \
 (d_avg_diff / n_stats), i4_max_diff); \
 printf("Range File1: %ld/%ld  File2: %ld/%ld\n", \
i4_min_val1, i4_max_val1, i4_min_val2, i4_max_val2); }


/*-------------------------------------------------------------------------
 * printf formatting
 *-------------------------------------------------------------------------
 */
#define SPACES     "          "
#define FFORMAT    "%-15f %-15f %-15f\n"
#define FFORMATP   "%-15f %-15f %.0f%%\n"
#define I8FORMAT   "%-15d %-15d %-15d\n"
#define I8FORMATP  "%-15d %-15d %.0f%%\n"
#define I16FORMAT  "%-15d %-15d %-15d\n"
#define I16FORMATP "%-15d %-15d %.0f%%\n"
#define IFORMAT    "%-15ld %-15ld %-15ld\n"
#define IFORMATP   "%-15ld %-15ld %.0f%%\n"
#define CFORMAT    "%-16c %-17c\n"
#define SFORMAT    "%-16s %-17s\n"
#define UIFORMAT   "%-15lu %-15lu %-15lu\n"
#define LIFORMAT   "%-15ld %-15ld %-15ld\n"
#define ULIFORMAT  "%-15lu %-15lu %-15lu\n"


#define I16FORMATP_NOTCOMP "%-15d %-15d not comparable\n"
#define I8FORMATP_NOTCOMP  "%-15d %-15d not comparable\n"
#define IFORMATP_NOTCOMP   "%-15ld %-15ld not comparable\n"
#define FFORMATP_NOTCOMP   "%-15f %-15f not comparable\n"


/*-------------------------------------------------------------------------
 * relative error
 *-------------------------------------------------------------------------
 */

#define PER(A,B) { per=-1;                           \
    not_comparable=0;                                \
    both_zero=0;                                     \
    if (A==0 && B==0)                                \
    both_zero=1;                                     \
    if (A!=0)                                        \
    per = (double)ABS( ( double)(B-A) / (double)A ); \
    else                                             \
    not_comparable=1;                                \
}

/*-------------------------------------------------------------------------
 * local prototypes
 *-------------------------------------------------------------------------
 */
static void print_pos( int        *ph, 
                       uint32     curr_pos, 
                       int32      *acc, 
                       int32      *pos, 
                       int        rank, 
                       const char *obj1, 
                       const char *obj2 );



/*-------------------------------------------------------------------------
 * Function: array_diff
 *
 * Purpose: compare the 2 buffers BUF1 and BUF2
 *
 *-------------------------------------------------------------------------
 */

uint32 array_diff(void *buf1, 
                  void *buf2, 
                  uint32 tot_cnt, 
                  const char *name1,
                  const char *name2,
                  int rank,
                  int32 *dims,
                  int32 type, 
                  float32 err_limit, 
                  float32 err_rel,
                  uint32 max_err_cnt, 
                  int32 statistics,
                  void *fill1, 
                  void *fill2)


{
 uint32   i;
 int8    *i1ptr1, *i1ptr2;
 int16   *i2ptr1, *i2ptr2;
 int32   *i4ptr1, *i4ptr2;
 float32 *fptr1, *fptr2;
 float64 *dptr1, *dptr2;
 float64 d_diff, d_avg_diff = 0., d_max_diff = 0.;
 float64 d_max_val1=0, d_min_val1=0, d_max_val2=0, d_min_val2=0;
 float64 d_val1, d_val2;
 float64 d_sumx = 0., d_sumy = 0., d_sumx2 = 0., d_sumy2 = 0., d_sumxy=0.;
 float64 slope, intercept, correlation;
 float32 f_diff;
 int32   i4_diff, i4_max_diff = 0;
 int32   i4_max_val1=0, i4_min_val1=0, i4_max_val2=0, i4_min_val2=0;
 int16   i2_diff;
 int8    c_diff;
 int     is_fill1, is_fill2;
 int     n_stats = 0;
 char    *debug;
 FILE    *fp=NULL;
 int32   acc[H4_MAX_VAR_DIMS];   /* accumulator position */
 int32   pos[H4_MAX_VAR_DIMS];   /* matrix position */
 int     ph=1;                /* print header  */
 int     j;
 double  per;
 int     both_zero;
 int     not_comparable;
 uint32  n_diff = 0;


 acc[rank-1]=1;
 for(j=(rank-2); j>=0; j--)
 {
  acc[j]=acc[j+1]*(int)dims[j+1];
 }
 for ( j = 0; j < rank; j++)
  pos[j]=0;


 debug = getenv("DEBUG");
 if (debug) {
  fp = fopen("hdiff.debug", "w");
 }
 
 switch(type) {
 case DFNT_INT8:
 case DFNT_CHAR8:
  i4_max_val1 = SCHAR_MIN;
  i4_min_val1 = SCHAR_MAX;
  i4_max_val2 = SCHAR_MIN;
  i4_min_val2 = SCHAR_MAX;
  break;
 case DFNT_UINT8:
 case DFNT_UCHAR8:
  i4_max_val1 = -UCHAR_MAX -1;
  i4_min_val1 = UCHAR_MAX;
  i4_max_val2 = -UCHAR_MAX -1;
  i4_min_val2 = UCHAR_MAX;
  break;
 case DFNT_INT16:
  i4_max_val1 = SHRT_MIN;
  i4_min_val1 = SHRT_MAX;
  i4_max_val2 = SHRT_MIN;
  i4_min_val2 = SHRT_MAX;
  break;
 case DFNT_UINT16:
  i4_max_val1 = -USHRT_MAX -1;
  i4_min_val1 = USHRT_MAX;
  i4_max_val2 = -USHRT_MAX -1;
  i4_min_val2 = USHRT_MAX;
  break;
 case DFNT_INT32:
  i4_max_val1 = INT_MIN;
  i4_min_val1 = INT_MAX;
  i4_max_val2 = INT_MIN;
  i4_min_val2 = INT_MAX;
  break;
 case DFNT_UINT32:
  i4_max_val1 = INT_MIN;
  i4_min_val1 = INT_MAX;
  i4_max_val2 = INT_MIN;
  i4_min_val2 = INT_MAX;
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
  printf(" bad type - %ld\n", type);
 }
 switch(type)
 {

/*-------------------------------------------------------------------------
 * DFNT_INT8, DFNT_UINT8, DFNT_UCHAR8, DFNT_CHAR8
 *-------------------------------------------------------------------------
 */
 case DFNT_INT8:
 case DFNT_UINT8:
 case DFNT_UCHAR8:
 case DFNT_CHAR8:
  i1ptr1 = (int8 *) buf1;
  i1ptr2 = (int8 *) buf2;
  for (i=0; i<tot_cnt; i++)
  {
   c_diff = (int8)abs(*i1ptr1 - *i1ptr2);
   is_fill1 = fill1 && (*i1ptr1 == *((int8 *)fill1));
   is_fill2 = fill2 && (*i1ptr2 == *((int8 *)fill2));
   if (!is_fill1 && !is_fill2) {
    d_avg_diff += (float64)c_diff;
    i4_max_diff = MYMAX(i4_max_diff, c_diff);
    d_val2 = (float64)(*i1ptr2);
    d_val1 = (float64)(*i1ptr1);
    d_sumx += d_val1;
    d_sumy += d_val2;
    d_sumx2 += d_val1 * d_val1;
    d_sumy2 += d_val2 * d_val2;
    d_sumxy += d_val1 * d_val2;
    n_stats++;
   }
   if (!is_fill1) {
    i4_max_val1 = MYMAX(i4_max_val1, (int32)(*i1ptr1));
    i4_min_val1 = MYMIN(i4_min_val1, (int32)(*i1ptr1));
   }
   if (!is_fill2) {
    i4_max_val2 = MYMAX(i4_max_val2, (int32)(*i1ptr2));
    i4_min_val2 = MYMIN(i4_min_val2, (int32)(*i1ptr2));
   }


/*-------------------------------------------------------------------------
 * relative
 *-------------------------------------------------------------------------
 */

   if (err_rel)
   {
       
       PER(*i1ptr1,*i1ptr2);
       
       if (not_comparable && !both_zero) /* not comparable */
       {
           print_pos(&ph,i,acc,pos,rank,name1,name2);
           printf(SPACES);
           printf(I8FORMATP_NOTCOMP,*i1ptr1,*i1ptr2);
           n_diff++;
       }
       
       else
           
           if (per > err_rel)
           {
               n_diff++;
               if (n_diff <= max_err_cnt) 
               {
                   print_pos(&ph,i,acc,pos,rank,name1,name2);
                   printf(SPACES);
                   printf(I8FORMATP,*i1ptr1,*i1ptr2,per*100);
               }
           }  
           
   }

   else if (c_diff > (int32) err_limit)
   {
    n_diff++;
    if (n_diff <= max_err_cnt) {
     print_pos(&ph,i,acc,pos,rank,name1,name2);
     printf(SPACES);
     printf(I8FORMAT,*i1ptr1,*i1ptr2,abs(*i1ptr1-*i1ptr2));
    }
   }                                               
   i1ptr1++;  i1ptr2++;
  }
  if (statistics) {
   PRINT_ISTATS("Byte");
  }
  
  break;

/*-------------------------------------------------------------------------
 * DFNT_INT16, DFNT_UINT16
 *-------------------------------------------------------------------------
 */
  
 case DFNT_INT16:
 case DFNT_UINT16:
  i2ptr1 = (int16 *) buf1;
  i2ptr2 = (int16 *) buf2;
  for (i=0; i<tot_cnt; i++)
  {
   i2_diff = (int16)abs(*i2ptr1 - *i2ptr2);
   is_fill1 = fill1 && (*i2ptr1 == *((int16 *)fill1));
   is_fill2 = fill2 && (*i2ptr2 == *((int16 *)fill2));
   if (debug) {
    fprintf(fp, "%d %d %ld %ld\n", is_fill1, is_fill2, (int32)(*i2ptr1), (int32)(*i2ptr2));
   }
   if (!is_fill1 && !is_fill2) {
    d_val1 = (float64)(*i2ptr1);
    d_val2 = (float64)(*i2ptr2);
    d_sumx += d_val1;
    d_sumy += d_val2;
    d_sumx2 += d_val1 * d_val1;
    d_sumy2 += d_val2 * d_val2;
    d_sumxy += d_val1 * d_val2;
    d_avg_diff += (float64)i2_diff;
    i4_max_diff = MYMAX(i4_max_diff, i2_diff);
    n_stats++;
   }
   if (!is_fill1) {
    i4_max_val1 = MYMAX(i4_max_val1, (int32)(*i2ptr1));
    i4_min_val1 = MYMIN(i4_min_val1, (int32)(*i2ptr1));
   }
   if (!is_fill2) {
    i4_max_val2 = MYMAX(i4_max_val2, (int32)(*i2ptr2));
    i4_min_val2 = MYMIN(i4_min_val2, (int32)(*i2ptr2));
   }


/*-------------------------------------------------------------------------
 * relative
 *-------------------------------------------------------------------------
 */

   if (err_rel)
   {
       
       PER(*i2ptr1,*i2ptr2);
       
       if (not_comparable && !both_zero) /* not comparable */
       {
           print_pos(&ph,i,acc,pos,rank,name1,name2);
           printf(SPACES);
           printf(I16FORMATP_NOTCOMP,*i2ptr1,*i2ptr2);
           n_diff++;
       }
       
       else
           
           if (per > err_rel)
           {
               n_diff++;
               if (n_diff <= max_err_cnt) 
               {
                   print_pos(&ph,i,acc,pos,rank,name1,name2);
                   printf(SPACES);
                   printf(I16FORMATP,*i2ptr1,*i2ptr2,per*100);
               }
           }  
           
   }


   else if (i2_diff > (int) err_limit)
   {
    n_diff++;
    if (n_diff <= max_err_cnt) {
     print_pos(&ph,i,acc,pos,rank,name1,name2);
     printf(SPACES);
     printf(I16FORMAT,*i2ptr1,*i2ptr2,abs(*i2ptr1-*i2ptr2));
    }
   }                                               
   i2ptr1++;  i2ptr2++;
  }
  if (statistics) {
   PRINT_ISTATS("Integer2");
  }
  break;

/*-------------------------------------------------------------------------
 * DFNT_INT32, DFNT_UINT32
 *-------------------------------------------------------------------------
 */
  
  
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
    d_avg_diff += (float64)i4_diff;
    d_val1 = (float64)(*i4ptr1);
    d_val2 = (float64)(*i4ptr2);
    d_sumx += d_val1;
    d_sumy += d_val2;
    d_sumx2 += d_val1 * d_val1;
    d_sumy2 += d_val2 * d_val2;
    d_sumxy += d_val1 * d_val2;
    i4_max_diff = (int32)MYMAX(i4_max_diff, (float64)(i4_diff));
    n_stats++;
   }
   if (! is_fill1) {
    i4_max_val1 = MYMAX(i4_max_val1,*i4ptr1);
    i4_min_val1 = MYMIN(i4_min_val1,*i4ptr1);
   }
    if (! is_fill2) {
    i4_max_val2 = MYMAX(i4_max_val2,*i4ptr2);
    i4_min_val2 = MYMIN(i4_min_val2,*i4ptr2);
   }


/*-------------------------------------------------------------------------
 * relative
 *-------------------------------------------------------------------------
 */

    if (err_rel)
    {
        
        PER(*i4ptr1,*i4ptr2);
        
        if (not_comparable && !both_zero) /* not comparable */
        {
            print_pos(&ph,i,acc,pos,rank,name1,name2);
            printf(SPACES);
            printf(IFORMATP_NOTCOMP,*i4ptr1,*i4ptr2);
            n_diff++;
        }
        
        else
            
            if (per > err_rel)
            {
                n_diff++;
                if (n_diff <= max_err_cnt) 
                {
                    print_pos(&ph,i,acc,pos,rank,name1,name2);
                    printf(SPACES);
                    printf(IFORMATP,*i4ptr1,*i4ptr2,per*100);
                }
            }  
            
    }

   else if (i4_diff > (int32) err_limit)
   {
    n_diff++;
    if (n_diff <= max_err_cnt) {
     print_pos(&ph,i,acc,pos,rank,name1,name2);
     printf(SPACES);
     printf(IFORMAT,*i4ptr1,*i4ptr2,i4_diff);
    }
   }                                               
   i4ptr1++;  i4ptr2++;
  }
  if (statistics) {
   PRINT_ISTATS("Integer4");
  }
  
  break;

/*-------------------------------------------------------------------------
 * DFNT_FLOAT
 *-------------------------------------------------------------------------
 */
  
  
 case DFNT_FLOAT:
  fptr1 = (float32 *) buf1;
  fptr2 = (float32 *) buf2;
  for (i=0; i<tot_cnt; i++)
  {
   f_diff = (float32)fabs(*fptr1 - *fptr2);
   is_fill1 = fill1 && (*fptr1 == *((float32 *)fill1));
   is_fill2 = fill2 && (*fptr2 == *((float32 *)fill2));
   if (debug) {
    fprintf(fp, "%d %d %f %f\n", is_fill1, is_fill2, *fptr1, *fptr2);
   }
   if (!is_fill1 && !is_fill2) {
    d_avg_diff += (float64)f_diff;
    d_val1 = (float64)(*fptr1);
    d_val2 = (float64)(*fptr2);
    d_sumx += d_val1;
    d_sumy += d_val2;
    d_sumx2 += d_val1 * d_val1;
    d_sumy2 += d_val2 * d_val2;
    d_sumxy += d_val1 * d_val2;
    d_max_diff = MYMAX(d_max_diff, (float64)(f_diff));
    n_stats++;
   }
   if (!is_fill1) {
    d_max_val1 = MYMAX(d_max_val1, (float64)(*fptr1));
    d_min_val1 = MYMIN(d_min_val1, (float64)(*fptr1));
   }
   if (!is_fill2) {
    d_max_val2 = MYMAX(d_max_val2, (float64)(*fptr2));
    d_min_val2 = MYMIN(d_min_val2, (float64)(*fptr2));
   }


/*-------------------------------------------------------------------------
 * relative
 *-------------------------------------------------------------------------
 */

   if (err_rel)
   {
       
       PER(*fptr1,*fptr2);
       
       if (not_comparable && !both_zero) /* not comparable */
       {
           print_pos(&ph,i,acc,pos,rank,name1,name2);
           printf(SPACES);
           printf(FFORMATP_NOTCOMP,*fptr1,*fptr2);
           n_diff++;
       }
       
       else
           
           if (per > err_rel)
           {
               n_diff++;
               if (n_diff <= max_err_cnt) 
               {
                   print_pos(&ph,i,acc,pos,rank,name1,name2);
                   printf(SPACES);
                   printf(FFORMATP,*fptr1,*fptr2,per*100);
               }
           }  
           
   }

   else if (f_diff > err_limit)
   {
    n_diff++;
    if (n_diff <= max_err_cnt) {
     print_pos(&ph,i,acc,pos,rank,name1,name2);
     printf(SPACES);
     printf(FFORMAT,*fptr1,*fptr2,fabs(*fptr1-*fptr2));
    }
   }                                               
   fptr1++;  fptr2++;
  }
  if (statistics) {
   PRINT_FSTATS("Float");
  }
  break;


/*-------------------------------------------------------------------------
 * DFNT_DOUBLE
 *-------------------------------------------------------------------------
 */
  
 case DFNT_DOUBLE:
  dptr1 = (float64 *) buf1;
  dptr2 = (float64 *) buf2;
  for (i=0; i<tot_cnt; i++)
  {
   d_diff = fabs(*dptr1 - *dptr2);
   is_fill1 = fill1 && (*dptr1 == *((float64 *)fill1));
   is_fill2 = fill2 && (*dptr2 == *((float64 *)fill2));
   if (!is_fill1 && !is_fill2) {
    d_avg_diff += d_diff;
    d_val1 = (float64)(*dptr1);
    d_val2 = (float64)(*dptr2);
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


/*-------------------------------------------------------------------------
 * relative
 *-------------------------------------------------------------------------
 */

   if (err_rel)
   {
       
       PER(*dptr1,*dptr2);
       
       if (not_comparable && !both_zero) /* not comparable */
       {
           print_pos(&ph,i,acc,pos,rank,name1,name2);
           printf(SPACES);
           printf(FFORMATP_NOTCOMP,*dptr1,*dptr2);
           n_diff++;
       }
       
       else
           
           if (per > err_rel)
           {
               n_diff++;
               if (n_diff <= max_err_cnt) 
               {
                   print_pos(&ph,i,acc,pos,rank,name1,name2);
                   printf(SPACES);
                   printf(FFORMATP,*dptr1,*dptr2,per*100);
               }
           }  
           
   }


   else if (d_diff > (float64) err_limit)
   {
    n_diff++;
    if (n_diff <= max_err_cnt) {
     print_pos(&ph,i,acc,pos,rank,name1,name2);
     printf(SPACES);
     printf(FFORMAT,*dptr1,*dptr2,fabs(*dptr1-*dptr2));
    }
   }
   dptr1++;  dptr2++;
  }
  if (statistics) {
   PRINT_FSTATS("Double");
  }
  break;
  
 default:
  printf(" bad type - %ld\n", type);
  }
  if (statistics) {
   float64 sqrt_arg;
   if ((float64)n_stats * d_sumx2 - d_sumx * d_sumx != 0.0) {
    slope = ((float64)n_stats * d_sumxy - d_sumx * d_sumy) / 
     ((float64)n_stats * d_sumx2 - d_sumx * d_sumx);
    intercept = (d_sumy - slope * d_sumx) / (float64)n_stats;
    sqrt_arg = ((float64)n_stats*d_sumx2 - d_sumx*d_sumx) /
     ((float64)n_stats * d_sumy2 - d_sumy * d_sumy);
    correlation = slope * sqrt(sqrt_arg);
    printf("Regression  N: %d  Slope: %e  Intercept: %e  R: %e\n",
     n_stats, slope, intercept, correlation);
   }
   else {
    printf("Regression  Slope: NaN  Intercept: NaN  R: NaN\n");
   }
  }
  if (debug) {
   fclose(fp);
  }
 
  return n_diff;
}


/*-------------------------------------------------------------------------
 * Function: print_pos
 *
 * Purpose: convert an array index position to matrix notation
 *
 * Return: pos matrix array
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: May 9, 2003
 *
 *-------------------------------------------------------------------------
 */
static void print_pos( int        *ph, 
                       uint32     curr_pos, 
                       int32      *acc, 
                       int32      *pos, 
                       int        rank, 
                       const char *obj1, 
                       const char *obj2 )
{
 int i;

 /* print header */
 if ( *ph==1 )
 {
  *ph=0;
  printf("%-15s %-15s %-15s %-20s\n", 
   "position", 
   (obj1!=NULL) ? obj1 : " ", 
   (obj2!=NULL) ? obj2 : " ",
   "difference");
  printf("------------------------------------------------------------\n");
 }

 for ( i = 0; i < rank; i++)
 {
  pos[i] = curr_pos/acc[i];
  curr_pos -= acc[i]*pos[i];
 }
 assert( curr_pos == 0 );

 printf("[ " );  
 for ( i = 0; i < rank; i++)
 {
  fprintf(stdout,"%ld ", pos[i]  );
 }
 printf("]" );
}


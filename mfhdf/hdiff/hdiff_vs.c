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

#include "hdiff.h"
#include "hdiff_list.h"
#include "hdiff_mattbl.h"

#include "hdf.h"
#include "mfhdf.h"
#include "hdiff.h"
#include "vgint.h"

static void fmt_print(uint8 *x, int32 type);
static uint32 vdata_cmp(int32  vs1, int32  vs2, char   *gname, char   *cname, diff_opt_t * opt);


/*-------------------------------------------------------------------------
 * Function: diff_vs
 *
 * Purpose: diff for VS 
 *
 * Return: Number of differences found
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: August 26, 2003
 *
 *-------------------------------------------------------------------------
 */

uint32 diff_vs( int32 file1_id,
                int32 file2_id,
                int32 ref1,              
                int32 ref2,
                diff_opt_t * opt)          
{
 int32 vdata1_id,             /* vdata identifier */
       n_records1,            /* number of records */
       vdata1_size, 
       interlace1_mode,
       vdata2_id=-1,          /* vdata identifier */
       n_records2,            /* number of records */
       vdata2_size, 
       interlace2_mode;
 char  vdata1_name [VSNAMELENMAX];
 char  vdata1_class[VSNAMELENMAX];
 char  fieldname1_list[VSFIELDMAX*FIELDNAMELENMAX];
 char  vdata2_name [VSNAMELENMAX];
 char  vdata2_class[VSNAMELENMAX];
 char  fieldname2_list[VSFIELDMAX*FIELDNAMELENMAX];
 uint32 nfound=0;


 
/*-------------------------------------------------------------------------
 * object 1
 *-------------------------------------------------------------------------
 */

 if (Vstart (file1_id)==FAIL) {
  printf("Error: Could not start VS interface in VS ref %ld\n", ref1);
  goto out;
 }

 if ((vdata1_id  = VSattach (file1_id, ref1, "r")) == FAIL ){
  printf( "Failed to attach VS ref %ld\n", ref1);
  goto out;
 }
 if (VSgetname  (vdata1_id, vdata1_name) == FAIL ){
  printf( "Failed to name for VS ref %ld\n", ref1);
  goto out;
 }
 if (VSgetclass (vdata1_id, vdata1_class) == FAIL ){
  printf( "Failed to name for VS ref %ld\n", ref1);
  goto out;
 }
 
 if (VSinquire(vdata1_id, &n_records1, &interlace1_mode, fieldname1_list, 
  &vdata1_size, vdata1_name) == FAIL) {
  printf( "Failed to get info for VS ref %ld\n", ref1);
  goto out;
 }
 
 if (VFnfields(vdata1_id)== FAIL ){
  printf( "Failed getting fields forVS ref %ld\n", ref1);
  goto out;
 }
 

/*-------------------------------------------------------------------------
 * object 2
 *-------------------------------------------------------------------------
 */

 if (Vstart (file2_id)==FAIL) {
  printf("Error: Could not start VS interface in VS ref %ld\n", ref1);
  goto out;
 }

 if ((vdata2_id  = VSattach (file2_id, ref2, "r")) == FAIL ){
  printf( "Failed to attach VS ref %ld\n", ref2);
  goto out;
 }
 if (VSgetname  (vdata2_id, vdata2_name) == FAIL ){
  printf( "Failed to name for VS ref %ld\n", ref2);
  goto out;
 }
 if (VSgetclass (vdata2_id, vdata2_class) == FAIL ){
  printf( "Failed to name for VS ref %ld\n", ref2);
  goto out;
 }
 
 if (VSinquire(vdata2_id, &n_records2, &interlace2_mode, fieldname2_list, 
  &vdata2_size, vdata2_name) == FAIL) {
  printf( "Failed to get info for VS ref %ld\n", ref2);
  goto out;
 }
 
 if (VFnfields(vdata2_id)== FAIL ){
  printf( "Failed getting fields forVS ref %ld\n", ref2);
  goto out;
 }

/*-------------------------------------------------------------------------
 * check for input VSs
 *-------------------------------------------------------------------------
 */
 
 if (opt->nuvars > 0)   /* if specified vdata is selected */
 {
  int imatch = 0, j;
  for (j = 0; j < opt->nuvars; j++)
  {
   if (strcmp(vdata1_name, opt->uvars[j]) == 0)
   {
    imatch = 1;
    break;
   }
  }
  if (imatch == 0)
  {
   goto do_nothing;
  }
 }   


/*-------------------------------------------------------------------------
 * Comparing
 *-------------------------------------------------------------------------
 */

 if (opt->verbose)
  printf("Comparing <%s>\n",vdata1_name);  

 nfound=vdata_cmp(vdata1_id,vdata2_id,vdata1_name,vdata1_class,opt);

do_nothing:

 /* terminate access to the VSs */
 if (VSdetach (vdata1_id)==FAIL) {
     printf( "Failed to dettach VS ref %ld\n", ref1);
     goto out;
 }
 if (vdata2_id!=-1)
 {
     if (VSdetach (vdata2_id)==FAIL) {
         printf( "Failed to dettach VS ref %ld\n", ref2);
         goto out;
     }
 }

 
 return nfound;

out:

 opt->err_stat = 1;
 return 0;
}



/*-------------------------------------------------------------------------
 * Function: vdata_cmp
 *
 * Purpose: compare vdata
 *
 *-------------------------------------------------------------------------
 */

static uint32 vdata_cmp(int32  vs1, 
                        int32  vs2, 
                        char   *gname, 
                        char   *cname, 
                        diff_opt_t * opt)
{
 int32   i, j, k, iflag;
 uint32  err_cnt;
 int32   nv1, interlace1, vsize1;
 int32   vsotag1;
 char    fields1[VSFIELDMAX*FIELDNAMELENMAX];
 char    vsclass1[VSNAMELENMAX], vsname1[VSNAMELENMAX];
 int32   nv2, interlace2, vsize2;
 int32   vsotag2;
 char    fields2[VSFIELDMAX*FIELDNAMELENMAX];
 char    vsclass2[VSNAMELENMAX], vsname2[VSNAMELENMAX];
 uint8   *buf1, *buf2, *b1, *b2;
 int32   off1[60], off2[60];
 DYN_VWRITELIST *w1, *w2;
 uint32  nfound=0;
 uint32  max_err_cnt = opt->max_err_cnt; 
 
 VSinquire(vs1, &nv1, &interlace1, fields1, &vsize1, vsname1);
 VSinquire(vs2, &nv2, &interlace2, fields2, &vsize2, vsname2);
 
 vsotag1 = VSQuerytag(vs1);
 VSgetclass(vs1,vsclass1);
 
 vsotag2 = VSQuerytag(vs2);
 VSgetclass(vs2,vsclass2);
 
 if (vsotag1 != vsotag2 || nv1 != nv2 || interlace1 != interlace2 ||
  strcmp(fields1, fields2) != 0 || strcmp(vsclass1, vsclass2) != 0 ||
  (strcmp(vsclass1, "Attr0.0") != 0 && vsize1 != vsize2))
 {
  printf("\n---------------------------\n");
  printf("Vdata Name: %s <%s/%s> (Different attributes)\n",
   vsname1, gname, cname);
  printf("> <%ld> nrec=%ld interlace=%ld fld=[%s] vsize=%ld class={%s})\n",
   vsotag1, nv1, interlace1, fields1, vsize1, vsclass1);
  printf("< <%ld> nrec=%ld interlace=%ld fld=[%s] vsize=%ld class={%s})\n",
   vsotag2, nv2, interlace2, fields2, vsize2, vsclass2);
  return 0;
 }
 
 
 /* compare the data */
 
    buf1 = (uint8 *)HDmalloc((unsigned) (nv1 * vsize1));
    buf2 = (uint8 *)HDmalloc((unsigned) (nv2 * vsize2));
 if (!buf1 || !buf2) 
 {
  printf("Out of memory!");
  opt->err_stat = 1;
  return 0;
 }
 
 VSsetfields(vs1, fields1);
 VSread(vs1, buf1, nv1, interlace1);
 w1 = (DYN_VWRITELIST*) vswritelist(vs1);
 
 VSsetfields(vs2, fields2);
 VSread(vs2, buf2, nv2, interlace2);
 w2 = (DYN_VWRITELIST*) vswritelist(vs2);
 
 b1 = buf1;
 b2 = buf2;
 
 for (j=0; j < w1->n; j++)
  off1[j] = DFKNTsize(w1->type[j] | DFNT_NATIVE);
 
 for (j=0; j < w2->n; j++)
  off2[j] = DFKNTsize(w2->type[j] | DFNT_NATIVE);
 
 iflag = 0;
 
 err_cnt = 0;
 
 if (vsize1 == vsize2)
 {
  for (i=0; i<nv1; i++)
  {
            if (HDmemcmp(b1, b2, (size_t)vsize1) == 0)
   {
    b1 += vsize1;   
    b2 += vsize2;
    continue;
   }
   if (iflag == 0)
   {
    iflag = 1;         /* there is a difference */
    printf("\n---------------------------\n");
    printf("Vdata Name: %s (Data record comparison)\n", 
     vsname1);
    nfound ++;
   }
   
   printf("> %ld: ", i);
   for (j=0; j<w1->n; j++)
   {
    for (k=0; k<w1->order[j]; k++)
    {
     fmt_print(b1, w1->type[j]);
     b1 += off1[j];
     if (w1->type[j] != DFNT_CHAR)
      putchar(' ');
    }
   }        
   putchar('\n');
   printf("< %ld: ", i);
   for (j=0; j<w2->n; j++)
   {
    for (k=0; k<w2->order[j]; k++)
    {
     fmt_print(b2, w2->type[j]);
     b2 += off2[j];
     if (w2->type[j] != DFNT_CHAR)
      putchar(' ');
    }
   }        
   putchar('\n');
   
   if (max_err_cnt > 0)
   {
    err_cnt++;
    if (err_cnt >= max_err_cnt)
     break;
   }
  }
 }
 else
 {
  printf("****....\n");
  for (i=0; i<nv1; i++)
  {
   if (iflag == 0)
   {
    iflag = 1;         /* there is a difference */
    printf("\n---------------------------\n");
    printf("Vdata Name: %s (Data record comparison)\n", 
     vsname1);
    nfound ++;
   }
   printf("> %ld: ", i);
   for (j=0; j<w1->n; j++)
   {
    for (k=0; k<w1->order[j]; k++)
    {
     fmt_print(b1, w1->type[j]);
     b1 += off1[j];
     if (w1->type[j] != DFNT_CHAR)
      putchar(' ');
    }
   }  
   putchar('\n');
   printf("< %ld: ", i);
   for (j=0; j<w2->n; j++)
   {
    for (k=0; k<w2->order[j]; k++)
    {
     fmt_print(b2, w2->type[j]);
     b1 += off2[j];
     if (w2->type[j] != DFNT_CHAR)
      putchar(' ');
    }
   }  
   putchar('\n');
   
   if (max_err_cnt > 0)
   {
    err_cnt++;
    if (err_cnt >= max_err_cnt)
     break;
   }
  }
  
 }
 
    if (buf1) HDfree((char *) buf1);
    if (buf2) HDfree((char *) buf2);

 return nfound;
}


/*-------------------------------------------------------------------------
 * Function: fmt_print
 *
 * Purpose: print HDF types
 *
 *-------------------------------------------------------------------------
 */

void
fmt_print(uint8 *x, int32 type)
{
 int16    s = 0;
 int32    l = 0;
 float32  f = 0;
 float64  d = 0;
 
 switch(type) 
 {
 case DFNT_CHAR:
  putchar(*x);
  break;
  
 case DFNT_UINT8:
 case DFNT_INT8:
  printf("%02x ", *x);
  break;
  
 case DFNT_UINT16:
 case DFNT_INT16:
  HDmemcpy(&s, x, sizeof(int16));
  printf("%d", s);
  break;
  
 case DFNT_UINT32:
  HDmemcpy(&l, x, sizeof(int32));
  printf("%lu", l);
  break;

 case DFNT_INT32:
  HDmemcpy(&l, x, sizeof(int32));
  printf("%ld", l);
  break;
  
 case DFNT_FLOAT32:
  HDmemcpy(&f, x, sizeof(float32));
  printf("%f", f);
  break;
  
 case DFNT_FLOAT64:
  HDmemcpy(&d, x, sizeof(float64));
  printf("%f", d);
  break;
  
 default: 
  fprintf(stderr,"sorry, type [%ld] not supported\n", type); 
  break;
  
 }
}

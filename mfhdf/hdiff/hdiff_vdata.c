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
#include "vgint.h"


int 
vdata_diff(int32 fid1, int32 fid2, struct fspec specp, 
           int32 cmp_flag,  /* to control any data comparison */
           int32 f_flag)    /* to control the print out message */
{
 int32 vsid1, vsid2;
 int32 vgid1, vgid2;
 int32 vstag1, vstag2;
 int32 n1, n2;
 int32 vs1, vs2, vg1, vg2;
 int32 *lonevs1=NULL, *lonevs2=NULL;   /* refs of all lone vdatas */
 int32 nlone1, nlone2;                 /* total no. of lone vdatas */
 int32 i, j, imatch, gfind, sfind;
 int   ret=0;
 
 char  vgname1[65], vgclass1[65], vsname1[65];
 char  vgname2[65], vgclass2[65], vsname2[65];
 
 Vstart(fid1);
 Vstart(fid2);

 vgid1 = -1;
 
 /* compare vdata inside a vgroup */
 
 while ( (vgid1 = Vgetid(fid1, vgid1)) != -1)
 {
  vg1 = Vattach(fid1, vgid1, "r");
  if (vg1 == FAIL)
  {
   printf("Cannot open vg id=%d in file1\n", vgid1);
  }
  Vinquire(vg1, &n1, vgname1);
  Vgetclass(vg1, vgclass1);
  if (strcmp(vgclass1, "CDF0.0") == 0)    /* skip the special group */
  {
   Vdetach(vg1);
   continue;
  }
  
  /* skip special classes */
  if (!is_readable(vgclass1))
  {
   Vdetach(vg1);
   continue;
  }
  
  
  /* look into file2 if there is a match with the same group 
  name and class name as in file1 */
  vgid2 = -1;
  gfind = 0;
  while ( (vgid2 = Vgetid(fid2, vgid2)) != -1)
  {
   vg2 = Vattach(fid2, vgid2, "r");
   if (vg2 == FAIL)
    printf("cannot open vg id=%d in file2\n", vgid2);
   
   Vinquire(vg2, &n2, vgname2);
   Vgetclass(vg2, vgclass2);
 
   if (strcmp(vgname1, vgname2) == 0 && strcmp(vgclass1, vgclass2) == 0)
   {
    gfind = 1;
    break;
   }
  }
  
  for (i = 0; i < Vntagrefs(vg1); i++)
  {
   Vgettagref(vg1, i, &vstag1, &vsid1);
   
   if (vstag1 == DFTAG_VH)     /* only interested in Vdata */
   {
    vs1 = VSattach(fid1, vsid1, "r");
    VSgetname(vs1, vsname1);
    
    if (specp.nuvars > 0)   /* if specified vdata is selected */
    {
     imatch = 0;
     for (j = 0; j < specp.nuvars; j++)
     {
      if (strcmp(vsname1, specp.uvars[j]) == 0)
      {
       imatch = 1;
       break;
      }
     }
     
     if (imatch == 0)
     {
      VSdetach(vs1);
      continue;
     }
     
    }   /* end if (specp.nuvars > 0) */
    
    VSdetach(vs1);
    
    if (gfind == 1)
    {
    /* look into file2 if there is a match with the same 
     vdata name as in file1 */
     sfind = 0;
     for (j = 0; j < Vntagrefs(vg2); j++)
     {
      Vgettagref(vg2, j, &vstag2, &vsid2);
      if (vstag2 == DFTAG_VH)
      {  
       vs2 = VSattach(fid2, vsid2, "r");
       VSgetname(vs2, vsname2);
       if (strcmp(vsname1, vsname2) == 0)
       {
        sfind = 1;
        break;
       }
       VSdetach(vs2);
       
      }  /* end if */
     }  /* end for */
     if (sfind == 1)
     {
      if (cmp_flag == 1)   /* compare the data only requested */
      {
       vs1 = VSattach(fid1, vsid1, "r");
       if (specp.verbose)
       printf("VS Name: %s/%s .... Comparing\n", vgname1, vsname1);
       ret=vdata_cmp(vs1, vs2, vgname1, vgclass1, specp.max_err_cnt);
       VSdetach(vs1);
      }
      VSdetach(vs2);
     }
     else  
     {
      if (f_flag == 1)
      {
       printf("\n---------------------------\n");
       printf("< Vdata '%s' in <%s/%s> does not exist in file1\n", 
        vsname1, vgname1, vgclass1);
       printf("> Vdata '%s' in <%s/%s>\n", vsname1, vgname1, vgclass1);
      }
      else
      {
       printf("\n---------------------------\n");
       printf("> Vdata '%s' in <%s/%s>\n", vsname1, vgname1, vgclass1);
       printf("< Vdata '%s' in <%s/%s> does not exist in file2\n", 
        vsname1, vgname1, vgclass1);
      }
     } 
    }  /* end if gfind == 1 */
    else   /* group doesn't exist in file2 */
    {
     if (f_flag == 1)
     {
      printf("\n---------------------------\n");
      printf("< Vdata '%s' in <%s/%s> does not exist in file1\n", 
       vsname1, vgname1, vgclass1);
      printf("> Vdata '%s' in <%s/%s>\n", vsname1, vgname1, vgclass1);
      ret=1;
     }
     else
     {
      printf("\n---------------------------\n");
      printf("> Vdata '%s' in <%s/%s>\n", vsname1, vgname1, vgclass1);
      printf("< Vdata '%s' in <%s/%s> does not exist in file2\n", 
       vsname1, vgname1, vgclass1);
      ret=1;
     }
    }
    
   }  /* end if vstag1 == DFTAG_VH */
   
  }  /* end of i loop */
  
  Vdetach(vg1);
  Vdetach(vg2);
  }  /* end of while loop */
  
  /*  check vdata that doesn't belong to any group  */
  
  nlone1 = VSlone(fid1, NULL, 0);
  nlone2 = VSlone(fid2, NULL, 0);
  /* only compare if there is something to compare  */
  if (nlone1 > 0 && nlone2 > 0 )
  {
   if (NULL == (lonevs1 = (int32 *) HDgetspace(sizeof(int) * nlone1)))
   {
    error("Out of memory!");
    exit(0);
   }
   if (nlone2 > 0)
   {
    if (NULL == (lonevs2 = (int32 *) HDgetspace(sizeof(int) * nlone2)))
    {
     error("Out of memory!");
     exit(0);
    }
   }
   
   /* get them */
   VSlone(fid1, lonevs1, nlone1);
   VSlone(fid2, lonevs2, nlone2);
   for (i=0; i<nlone1; i++)
   {
    vsid1 = lonevs1[i];
    vs1 = VSattach(fid1, vsid1, "r");
    VSgetname(vs1, vsname1);
    
    VSgetclass(vs1, vgclass1);
    /* skip special classes */
    if (!is_readable(vgclass1))
    {
     VSdetach(vs1);
     continue;
    }
  
    if (specp.nuvars > 0)   /* if specified vadata is selected */
    {
     imatch = 0;
     for (j = 0; j < specp.nuvars; j++)
     {
      if (strcmp(vsname1, specp.uvars[j]) == 0)
      {
       imatch = 1;
       break;
      }
     }
     if (imatch == 0)
     {
      VSdetach(vs1);
      continue; 
     }
    }   /* end if (specp.nuvars > 0) */
    
    sfind = 0;
    for (j=0; j<nlone2; j++)
    {
     vsid2 = lonevs2[j];
     vs2 = VSattach(fid2, vsid2, "r");
     VSgetname(vs2, vsname2);
     if (strcmp(vsname1, vsname2) == 0)
     {
      sfind = 1;
      break;
     }
    }  /* end of j loop */
    
    if (sfind == 1)
    {
     if (cmp_flag == 1)   /* compare the data only requested */
     {
      if (specp.verbose)
      printf("VS Name: %s .... Comparing\n", vsname1);
      ret=vdata_cmp(vs1, vs2, "No_Group","No_Class", specp.max_err_cnt);
     }
    }
    else  
    {
     if (f_flag == 1)
     {
      printf("\n---------------------------\n");
      printf("< Vdata '%s' does not exist in file1\n", vsname1);
      printf("> Vdata '%s'\n", vsname1);
      ret=1;
     }
     else
     {
      printf("\n---------------------------\n");
      printf("> Vdata '%s'\n", vsname1);
      printf("< Vdata '%s' does not exist in file2\n", vsname1);
      ret=1;
     }
    } 
    VSdetach(vs2);
    VSdetach(vs1);
   }   /* end of i loop */
   
   if (lonevs1)
    HDfreespace((VOIDP) lonevs1);
   if (lonevs2)
    HDfreespace((VOIDP) lonevs2);
   
  }   /* end if (nlone1 > 0) */
  
  Vend(fid1);
  Vend(fid2);
  return ret;
}


int
vdata_cmp(int32  vs1, int32  vs2, 
          char   *gname, 
          char   *cname, 
          int32  max_err_cnt)
{
 int32   i, j, k, iflag, err_cnt;
 int32   nv1, interlace1, vsize1;
 int32   vsotag1, vsoref1;
 char    fields1[150], vsclass1[50], vsname1[50];
 int32   nv2, interlace2, vsize2;
 int32   vsotag2, vsoref2;
 char    fields2[150], vsclass2[50], vsname2[50];
 uint8   *buf1, *buf2, *b1, *b2;
 int32   off1[60], off2[60];
 int     ret=0;
 DYN_VWRITELIST *w1, *w2;
 
 VSinquire(vs1, &nv1, &interlace1, fields1, &vsize1, vsname1);
 VSinquire(vs2, &nv2, &interlace2, fields2, &vsize2, vsname2);
 
 vsotag1 = VSQuerytag(vs1);
 vsoref1 = VSQueryref(vs1);
 VSgetclass(vs1,vsclass1);
 
 vsotag2 = VSQuerytag(vs2);
 vsoref2 = VSQueryref(vs2);
 VSgetclass(vs2,vsclass2);
 
 if (vsotag1 != vsotag2 || nv1 != nv2 || interlace1 != interlace2 ||
  strcmp(fields1, fields2) != 0 || strcmp(vsclass1, vsclass2) != 0 ||
  (strcmp(vsclass1, "Attr0.0") != 0 && vsize1 != vsize2))
 {
  printf("\n---------------------------\n");
  printf("Vdata Name: %s <%s/%s> (Different attributes)\n",
   vsname1, gname, cname);
  printf("> <%d> nrec=%d interlace=%d fld=[%s] vsize=%d class={%s})\n",
   vsotag1, nv1, interlace1, fields1, vsize1, vsclass1);
  printf("< <%d> nrec=%d interlace=%d fld=[%s] vsize=%d class={%s})\n",
   vsotag2, nv2, interlace2, fields2, vsize2, vsclass2);
  return 1;
 }
 
 
 /* compare the data */
 
 buf1 = (uint8 *) malloc((unsigned) (nv1 * vsize1));
 buf2 = (uint8 *) malloc((unsigned) (nv2 * vsize2));
 if (!buf1 || !buf2) 
 {
  error("Out of memory!");
  exit(0);
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
   if (memcmp(b1, b2, vsize1) == 0)
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
    ret=1;
   }
   
   printf("> %d: ", i);
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
   printf("< %d: ", i);
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
    ret=1;
   }
   printf("> %d: ", i);
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
   printf("< %d: ", i);
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
 
 if (buf1)free((char *) buf1);
 if (buf2)free((char *) buf2);

 return ret;
}


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
  printf("%f", f);
  break;
  
 default: 
  fprintf(stderr,"sorry, type [%d] not supported\n", type); 
  break;
  
 }
}

/*-------------------------------------------------------------------------
 * Function: is_readable
 *
 * Purpose: check for reserved Vgroup/Vdata class/names
 *
 * Return: 1 if reserved, 0 if not
 *
 *-------------------------------------------------------------------------
 */

int is_readable( char *vgroup_class ) 
{
 int ret=1;
 
 /* ignore reserved HDF groups/vdatas */
 if(vgroup_class != NULL) {
  if( (strcmp(vgroup_class,_HDF_ATTRIBUTE)==0) ||
   (strcmp(vgroup_class,_HDF_VARIABLE) ==0) || 
   (strcmp(vgroup_class,_HDF_DIMENSION)==0) ||
   (strcmp(vgroup_class,_HDF_UDIMENSION)==0) ||
   (strcmp(vgroup_class,DIM_VALS)==0) ||
   (strcmp(vgroup_class,DIM_VALS01)==0) ||
   (strcmp(vgroup_class,_HDF_CDF)==0) ||
   (strcmp(vgroup_class,GR_NAME)==0) ||
   (strcmp(vgroup_class,RI_NAME)==0) || 
   (strcmp(vgroup_class,RIGATTRNAME)==0) ||
   (strcmp(vgroup_class,RIGATTRCLASS)==0) ){
   ret=0;
  }

  /* class and name(partial) for chunk table i.e. Vdata */
  if( (strncmp(vgroup_class,"_HDF_CHK_TBL_",13)==0)){
   ret=0;
  }

 }
 
 return ret;
 
}


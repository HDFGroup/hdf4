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

#ifdef RCSID
static char RcsId[] = "@(#)Revision";
#endif

/* $Id$ */


#include <stdio.h>
#include "mfhdf.h"
#include "hdp.h"
#ifndef MIPSEL
#include <math.h>
#endif /* MIPSEL */


static intn dsd(dump_info_t *dumpsds_opts, intn curr_arg, intn argc, char *argv[]);

void sort(int32 chosen[MAXCHOICES])
{
   int32 temp, i, j;

   for (i=0; chosen[i]!=-1; i++)
      for (j=i; chosen[j]!=-1; j++) 
         if (chosen[i]>chosen[j]) {
            temp = chosen[i];
            chosen[i] = chosen[j];
            chosen[j] = temp;
         }
}

void dumpsds_usage(intn argc,char *argv[])
{
	printf("Usage:\n");
	printf("%s dumpsds [-a|-i <index>|-n <name>|-r <ref>] [-dhv] [-o <filename> [-bx]] <filelist>\n",argv[0]);
	printf("\t-a\tDump all SDSs in the file (default)\n");
	printf("\t-i <index>\tDump the <index>th SDS in the file \n");
	printf("\t-n <name>\tDump the SDS with name <name>\n");
	printf("\t-r <ref>\tDump the SDS with reference number <ref>\n");
	printf("\t-d\tDump data only, no tag/ref, formatted to input to hp2hdf\n");
	printf("\t-h\tDump header only, no annotation for elements nor data\n");
	printf("\t-v\tDump everything including all annotations (default)\n");
	printf("\t-o <filename>\tOutput to file <filename>\n");
	printf("\t-b\tBinary format of output\n");
	printf("\t-x\tAscii text format of output (default)\n");
}	/* end list_usage() */

void init_dumpsds_opts(dump_info_t *dumpsds_opts)
{
	dumpsds_opts->filter=DALL;		/* default dump all SDSs */
	dumpsds_opts->filter_num=FALSE;       	/* not by reference nor by index */
	dumpsds_opts->contents=DVERBOSE;	/* default dump all information */
	dumpsds_opts->dump_to_file=FALSE;	/* don't dump to output file*/
	dumpsds_opts->file_type=DASCII;		/* default output is ASCII file */
	HDstrcpy(dumpsds_opts->file_name, "\0");
}	/* end init_list_opts() */

intn parse_dumpsds_opts(dump_info_t *dumpsds_opts,intn *curr_arg,intn argc,char *argv[])
{
     int32 i, lastItem, numItems;
     char *tempPtr, *ptr, *string;

    if (*curr_arg >= argc) return(FAIL); 

	while ((*curr_arg < argc) && 
              (argv[*curr_arg][0]=='-')) { 
	    switch(argv[*curr_arg][1]) {
		case 'a':	/* dump all, default */
                    dumpsds_opts->filter = DALL;
                    (*curr_arg)++;
	 	    break;

		case 'i':	/* dump by index */
		    dumpsds_opts->filter=DINDEX;	
            (*curr_arg)++;

		    ptr = argv[*curr_arg];
            numItems=0;
            while((tempPtr=HDstrchr(ptr,','))!=NULL)
              {
                  numItems++;
                  ptr++;
              } /* end while */

		    dumpsds_opts->filter_num = (intn*)HDmalloc(sizeof(intn)*numItems);
		    if (dumpsds_opts->filter_num==NULL) {
		       printf("Not enough memory!\n");
		       exit(-1);
            }
		    ptr = argv[*curr_arg];
            i=0;
            while((tempPtr=HDstrchr(ptr,','))!=NULL)
              {
               *tempPtr = '\0';
		       dumpsds_opts->filter_num[i] = atoi(ptr);
		       ptr = tempPtr + 1;
               i++;
		    }
		    dumpsds_opts->filter_num[i] = -1;
		    (*curr_arg)++;
		    break;

            case 'r':       /* dump by reference */
                dumpsds_opts->filter=DREFNUM;
                (*curr_arg)++; 

                ptr = argv[*curr_arg];
                numItems=0;
                while((tempPtr=HDstrchr(ptr,','))!=NULL)
                  {
                      numItems++;
                      ptr++;
                  } /* end while */

                dumpsds_opts->filter_num = (intn*)HDmalloc(sizeof(intn)*numItems);
                if (dumpsds_opts->filter_num==NULL) {
                   printf("Not enough memory!\n");
                   exit(-1);
                        }

                ptr = argv[*curr_arg];
                i=0;
                while((tempPtr=HDstrchr(ptr,','))!=NULL)
                  {
                   *tempPtr = '\0';
                   dumpsds_opts->filter_num[i] = atoi(ptr);
                   ptr = tempPtr + 1;
                   i++;
                }
                dumpsds_opts->filter_num[i] = -1;
                (*curr_arg)++;
                break;

		case 'n':	/* dump by names */
		    dumpsds_opts->filter=DNAME;	/* dump by name */
		    (*curr_arg)++;
		    lastItem = 0;
		    ptr = argv[*curr_arg];
		    for (i=0; !lastItem; i++) {
		       tempPtr = HDstrchr(ptr, ',');
		       if (tempPtr==NULL)
			  lastItem = 1;
		       else
			  *tempPtr = '\0';
		       string = (char*)HDmalloc(sizeof(char)*MAXNAMELEN);
		       if (string==NULL) {
		          printf("Not enough memory!\n");
		          exit(-1);
                       }
		       HDstrcpy(string, ptr);
		       dumpsds_opts->filter_str[i] = string;
		       ptr = tempPtr + 1;
 		    }
		    dumpsds_opts->filter_str[i] = NULL;
		    (*curr_arg)++;
		    break;

		case 'd':	/* dump data only */
                    dumpsds_opts->contents = DDATA;
                    (*curr_arg)++;
		    break;

		case 'h':	/* no annotations nor data */
                    dumpsds_opts->contents = DHEADER;
                    (*curr_arg)++;
		    break;

		case 'v':	/* dump all info */
                    dumpsds_opts->contents = DVERBOSE;
                    (*curr_arg)++;
                    break;

		case 'o':	/* specify output file */
                    dumpsds_opts->dump_to_file = TRUE; /* get filename */
                    HDstrcpy(dumpsds_opts->file_name, argv[++(*curr_arg)]);
                    if (++(*curr_arg)<argc)  { /* binary or ascii */
                        if (argv[*curr_arg][0] == '-') 
                            dumpsds_opts->file_type = (argv[*curr_arg][1] == 'b')?
                                     DBINARY : DASCII;
                        else (*curr_arg)--; 
                    }
                    (*curr_arg)++;
	            break;

		default:	/* invalid dumpsds option */
		    printf("Warning: Invalid dumpsds option %s\n", 
                             argv[*curr_arg]);
                    return(FAIL);
	  }	/* end switch */
      }	/* end while */
      return(SUCCEED);
}	/* end parse_dumpsds_opts */

void do_dumpsds(intn curr_arg,intn argc,char *argv[],dump_opt_t *glob_opts)
{
    dump_info_t dumpsds_opts;	/* dumpsds options */
    intn ret;

    if(glob_opts->help==TRUE) {
	dumpsds_usage(argc,argv);
	return;
    }	/* end if */
    init_dumpsds_opts(&dumpsds_opts);
    if(parse_dumpsds_opts(&dumpsds_opts,&curr_arg,argc,argv)==FAIL) {
	dumpsds_usage(argc,argv);
	return;
    }	/* end if */
    switch (dumpsds_opts.filter)  {
        case DINDEX : 
            ret = dsd(&dumpsds_opts, curr_arg, argc, argv);
            if (ret == FAIL) {
                printf("Failure in dump %dth SDS \n", dumpsds_opts.filter_num);
                exit(1);
            }
            break;
        case DREFNUM :
            ret = dsd(&dumpsds_opts, curr_arg, argc, argv);
            if (ret == FAIL) {
                printf("Failure in dump ref=%d SDS \n", dumpsds_opts.filter_num);
                exit(1);
            }
            break;
        case DNAME :
            ret = dsd(&dumpsds_opts, curr_arg, argc, argv);
            if (ret == FAIL) {
                printf("Failure in dump name=%s SDS \n", dumpsds_opts.filter_str);
                exit(1);
            }
            break;
        default:
            ret = dsd(&dumpsds_opts, curr_arg, argc, argv);
            if (ret == FAIL) {
                printf("Failure in dump all SDSs \n");
                exit(1);
            }
            break;
        }  /* switch of filter  */
    return; 
}	/* end do_dumpsds() */


/* --------------------------------------------------------- 
* printing functions copied from vshow.c and used by sdsdumpfull(). 
*/
static int32 cn = 0;


int32 fmtbyte(unsigned char *x, FILE *ofp)
{
    cn += fprintf(ofp,"%02x ",*x);
    return(1);
}
int32 fmtint8(VOIDP x, FILE *ofp)
{
    cn += fprintf(ofp, "%d", *((signed char *)x));
    return(1);
}

int32 fmtuint8(VOIDP x, FILE *ofp)
{
    cn += fprintf(ofp, "%u", *((unsigned char *)x));
    return(1);
}

int32 fmtint16(VOIDP x, FILE *ofp)
{
  short s = 0;
  HDmemcpy(&s, x, sizeof(int16));
  cn += fprintf(ofp, "%d", s);
  return(1);
}

int32 fmtuint16(VOIDP x, FILE *ofp)
{
  unsigned short s = 0;
  HDmemcpy(&s, x, sizeof(int16));
  cn += fprintf(ofp, "%u", s);
  return(1);
}


int32 fmtchar(VOIDP x, FILE *ofp)
{
    if (isprint(*(unsigned char *)x))   {
        putc(*((char *)x), ofp);
        cn++;
    }
    else   {
        putc('\\', ofp);
        cn++;
        cn += fprintf(ofp, "%03o", *((uchar8 *)x));
    }

    return(1);
}

int32 fmtuchar8(VOIDP x, FILE *ofp)
{
    cn++;
    putc('\\', ofp);
    cn++;
    fprintf(ofp, "%o", *((uchar8 *)x));
/*
    putc(*((uchar8 *)x), ofp);
*/
    return(1);
}

int32 fmtint(VOIDP x, FILE *ofp)
{
  int i = 0;
  HDmemcpy(&i, x, sizeof(int32));
  cn += fprintf(ofp, "%d", i);
  return(1);
}

int32 fmtfloat32(VOIDP x, FILE *ofp)
{
  float32 f = 0, epsi = 1.0e-20;
  HDmemcpy(&f, x, sizeof(float32));
  if (fabs(f - FILL_FLOAT) <= epsi)
      cn += fprintf(ofp, "FloatInf ");
  else cn += fprintf(ofp, "%f", f);
  return(1);
}

int32 fmtint32(VOIDP x, FILE *ofp)
{
  long l = 0;
  HDmemcpy(&l, x, sizeof(int32));
  cn += fprintf(ofp, "%ld", l);
  return(1);
}

int32 fmtuint32(VOIDP x, FILE *ofp)
{
  uint32 l = 0;
  HDmemcpy(&l, x, sizeof(int32));
  cn += fprintf(ofp, "%lu", l);
  return(1);
}

int32 fmtshort(VOIDP x, FILE *ofp)
{
  short s = 0;
  HDmemcpy(&s, x, sizeof(int16));
  cn += fprintf(ofp, "%d", s);
  return(1);
}

int32 fmtfloat64(char *x, FILE *ofp)
{
  double d = 0, epsi = 1.0e-20;

  HDmemcpy(&d, x, sizeof(float64));
  if (fabs(d - FILL_DOUBLE) <= epsi)
      cn += fprintf(ofp, "DoubleInf ");
  else cn += fprintf(ofp, "%f", d);
  return(1);
}

#define BUFFER 1000000

int32 dumpfull(int32 nt,int32 cnt,VOIDP databuf,intn indent,FILE *ofp)
{
    intn i; 
    VOIDP b; 
    int32 (*fmtfunct)()=NULL;
    int32 off;
    
    switch (nt)  {
       case DFNT_CHAR:
            fmtfunct = fmtchar;
            break;
       case DFNT_UCHAR:
            fmtfunct = fmtuchar8;
            break;

       case DFNT_UINT8:
            fmtfunct = fmtuint8;
       case DFNT_INT8:
            fmtfunct = fmtint8;
            break;
       case DFNT_UINT16:
            fmtfunct = fmtuint16;
       case DFNT_INT16:
            fmtfunct = fmtint16;
            break;
       case DFNT_UINT32:
            fmtfunct = fmtuint32;
       case DFNT_INT32:
            fmtfunct = fmtint32;
            break;
       case DFNT_FLOAT32:
            fmtfunct = fmtfloat32;
            break;
       case DFNT_FLOAT64:
            fmtfunct = fmtfloat64;
            break;
       default:
            fprintf(ofp, "HDP does not support type [%d] \n", (int)nt);
            break;
    }
    b = databuf;
    off = DFKNTsize(nt | DFNT_NATIVE);
    cn = indent;
    for (i=0; i<cnt; i++)   {
        fmtfunct(b, ofp);
        b = (char *)b+ off;
        if (nt != DFNT_CHAR)   {
            putc(' ', ofp);
            cn++;
        }
        if (cn > 65)   {
            putc('\n', ofp);
            for (cn=0; cn<indent; cn++) putc(' ', ofp);
        }
    }
    return(0);
}	    


int32 sdsdumpfull(int32 sds_id, int32 rank, int32 dimsizes[], int32 nt, 
                  intn indent, FILE *fp)
{   
    /* "rank" is the number of dimensions and 
       "dimsizes[i]" is size of dimension "i".*/

    int32 j,i, ret;
    VOIDP buf; 
    int32 eltsz, read_nelts;
    int32 done;    /* number of rows we have done */
    int32 *left, *start, *edge;

    if (indent>65)  { /* This block is probably not necessary. */
         printf("Bad indentation %d\n", indent);
         exit(1);
    } 
   
    /* Compute the number of the bytes for each value. */
    eltsz = DFKNTsize(nt | DFNT_NATIVE); 

    read_nelts = dimsizes[rank-1];
    buf=(VOIDP)HDmalloc(read_nelts*eltsz);
    if (buf==NULL) {
       printf("Not enough memory!\n");
       exit(-1);
    }
    left = (int32 *)HDmalloc(rank * sizeof(int32));
    if (left==NULL) {
       printf("Not enough memory!\n");
       exit(-1);
    }
    start = (int32 *)HDmalloc(rank * sizeof(int32));
    if (start==NULL) {
       printf("Not enough memory!\n");
       exit(-1);
    }
    edge = (int32 *)HDmalloc(rank*sizeof(int32));
    if (edge==NULL) {
       printf("Not enough memory!\n");
       exit(-1);
    }
    for (i=0; i<rank; i++)   {
        start[i] = 0; /* Starting location to read the data. */
        left[i] = dimsizes[i]; 
        edge[i] = 1; /* Number of values to read in each dimension. */
    }
    edge[rank-1] = dimsizes[rank-1];
 
    if (rank==1) {   /* If there is only one dimension, then dump the data
			and the job is done. */
        ret = SDreaddata(sds_id, start, NULL, edge, buf); 
        ret = dumpfull(nt, read_nelts, buf, indent, fp);
        fprintf(fp,"\n"); 
        cn=0;
    }
    else if (rank>1) {
       done = 0; 
       while (!done)  {  /* In each iteration, a row in dumped and 
			    "left[]" is modified accordingly. */
          ret = SDreaddata(sds_id, start, NULL, edge, buf); 
          ret = dumpfull(nt, read_nelts, buf, indent, fp);
          fprintf(fp,"\n");
          for (cn=0; cn<indent; cn++) 
	     fprintf(fp, " ");

	  /* Modify the values for "start[]" and "left[]" that are to be used
	     for dumping the next row. */
          /* The following index variable "j" starts from "rank-2" because:
	     (1) the range is from 0 to rank-1;
	     (2) each element in dimension rank-1 is just an element in a row
		 which is read in each time, and so we don't have to compute
		 the "start" of it. */
          for (j=rank-2; j>=0; j--) { /* Examine each dimension. */
             if (--left[j] > 0) { /* Proceed in the same dimension; as
				     long as there are elements in this
				     dimension, this loop breaks here
				     After the last element in the current
				     dimension has been subtracted, we
				     substract one fro the next lower
				     dimension and reset "left[j]" to be
				     the size of dimension j. */
                  start[j]++;
                  break;
             }
             else { /* Nothing left in the current dimension.
		       So, subtract one from the (j-1)th dimension and
		       reset the value of "left[j]". */
                left[j] = dimsizes[j];
                start[j] = 0;
                if (j==0) 
		   done=1;
                if (j==rank-2)  {
                   fprintf(fp, "\n");
                   for (cn=0; cn<indent; cn++) putc(' ', fp);
                }
             }
          }    /* for j */
       }  /* while   */
    } /* else */
    HDfree((VOIDP)start);
    HDfree((VOIDP)left);
    HDfree((VOIDP)buf);
    HDfree((VOIDP)edge);
    fprintf(fp, "\n");
    return(0);
} /* sdsdumpfull */
/* ------------------------------------- */


static intn dsd(dump_info_t *dumpsds_opts, intn curr_arg, intn argc, char *argv[])  
{
    intn  i, ret, isdimvar;
    int32 sdf_id, sds_id, sd_chosen[MAXCHOICES];
    int32 rank, nt, nattr, ndsets, nglb_attr;
    int32 j, k, attr_nt, attr_count, attr_buf_size, attr_index;
    char file_name[MAXFNLEN], name[MAXNAMELEN]; 
    char attr_name[MAXNAMELEN], dim_nm[MAXNAMELEN];
    int32 dimsizes[MAXRANK], dim_id, dimNT[MAXRANK], dimnattr[MAXRANK];
    FILE  *fp;
    int32 /* ref ,*/ index;
    VOIDP attr_buf;
    char *nt_desc, *attr_nt_desc;
    int index_error, x, a, b, dumpall=0;
    int32 sd_ref;
    
    while (curr_arg < argc)   { /* Examine each file. */
        HDstrcpy(file_name, argv[curr_arg]);
        curr_arg++;
        sdf_id = SDstart(file_name, DFACC_RDONLY);
        if (sdf_id == FAIL) {
           printf("Failure in open %s\n", file_name);
           exit(1);
        }
        for (i=0; i<MAXCHOICES; i++)
	   sd_chosen[i] = -1;
        k = 0; 
	switch (dumpsds_opts->filter) {  /* Determine the SDs having been
					    selected. */
             case DINDEX:
		for (i=0; dumpsds_opts->filter_num[i]!= -1; i++) {
		   sd_chosen[i] = dumpsds_opts->filter_num[i];
		   sd_chosen[i]--;
		}
                break;
             case DREFNUM:
		for (i=0; dumpsds_opts->filter_num[i]!= -1; i++) {
                   index = SDreftoindex(sdf_id, dumpsds_opts->filter_num[i]);
		   if (index==-1) {
		      printf("SD with reference number %d: not found\n",
			     dumpsds_opts->filter_num[i]);
		      index_error = 1;
		   }
		   else {
		      sd_chosen[k] = index;
		      k++;
                   }
		}
                break;
             case DNAME:
		for (i=0; dumpsds_opts->filter_str[i]!=NULL; i++) {
                   index = SDnametoindex(sdf_id, dumpsds_opts->filter_str[i]);
                   if (index==-1) {
		      printf("SD with nmae %s: not found\n", 
			     dumpsds_opts->filter_str[i]);
		      index_error = 1;
		   }
		   else {
		      sd_chosen[k] = index;
		      k++;
                   }
		}
                break;
	     case DCLASS:
		  printf("Currently, no class defined on an SD.\n");
		  exit(1);
             case DALL:
                  break;
             default:
                  printf("Unknown filter option\n");
                  exit(1);
        }
        if ((index_error) && (sd_chosen[0]==-1))
	   continue;
                    /* get output file name  */
        if (dumpsds_opts->dump_to_file) 
	   fp = fopen(dumpsds_opts->file_name, "w");
        else
	   fp = stdout;
        if (dumpsds_opts->contents!=DDATA)
           fprintf(fp, "File name: %s \n", file_name);
        ret=SDfileinfo( sdf_id, &ndsets, &nglb_attr);
        if (ret == FAIL) {
            printf("Failure in SDfileinfo %s\n", file_name);
            exit(1);
        }
	x = 0;
	if (sd_chosen[x]==-1)
	   dumpall = 1;
	else 
	   sort(sd_chosen);
	for (i=0; i<ndsets; i++)   {  /* Examine each SD. */
             int data_only, match=0;

	     if ((!dumpall) && (i!=sd_chosen[x])) 
		continue;
	     x++;
             /* Reset variables. */
             for (k=0; k<MAXRANK; k++) {
                  dimsizes[k]=0;
                  dimNT[k]=0;
                  dimnattr[k]=0;
             }

             sds_id = SDselect(sdf_id, i);
             if (sds_id == FAIL) {
                 printf("Failure in selecting %s\n", file_name);
                 exit(1);
             }
	     if ((sd_ref=SDidtoref(sds_id))==FAIL) {
		 printf("Failure in determining reference no.\n");
		 exit(1);
	     }
             isdimvar=(SDiscoordvar(sds_id))? 1 : 0;
             ret = SDgetinfo(sds_id, name, &rank, dimsizes, &nt, &nattr);
             nt_desc = HDgetNTdesc(nt);
             if (ret == FAIL) {
                 printf("Failure in SDfileinfo %s\n", file_name);
                 exit(1);
             }

             switch (dumpsds_opts->contents)   {
                 case DVERBOSE:
/*                     ref = SDidtoref(sds_id);
                     if (ref == FAIL) {
                          printf("Failure in SDidtoref %s\n", file_name);
                          exit(1);
                     }
*/                     
                 case DHEADER:
                     if (isdimvar) {
                         fprintf(fp,"\nDimension Variable Name = %s\n\t ",
				 name); 
			 fprintf(fp,"Index = %d\n\t Type= %s\n", i, nt_desc);
                     } 
		     else {
                         fprintf(fp,"\nVariable Name = %s\n\t Index = ", name);
			 fprintf(fp,"%d\n\t Type= %s\n", i, nt_desc);
                     }
		     HDfree(nt_desc);
		     fprintf(fp,"\t Ref. = %d\n", (int)sd_ref);
                     fprintf(fp,"\t Rank = %d\n\t Number of attributes = %d\n",
                                   (int)rank, (int)nattr);
                     for (j=0; j<rank; j++)   {
			 int32 temp;
                         dim_id = SDgetdimid(sds_id, j);
                         ret = SDdiminfo(dim_id, dim_nm, &temp,
                                 &(dimNT[j]), &(dimnattr[j]));
                         attr_nt_desc = HDgetNTdesc(dimNT[j]);
                         fprintf(fp, "\t Dim%d: Name=%s\n", (int)j, dim_nm);
                         if (temp == 0)  {
			  fprintf(fp, "\t\t Size = UNLIMITED ");
			  fprintf(fp, "(currently %d)\n", (int)dimsizes[j]);
			 }
			 else
			    fprintf(fp, "\t\t Size = %d\n",(int)dimsizes[j]);
			 fprintf(fp, "\t\t Type = %s\n",attr_nt_desc); 
			 HDfree(attr_nt_desc);
                         fprintf(fp, "\t\t Number of attributes = %d\n", 
				 (int)dimnattr[j]);
                     }
		     /* Print annotations */
                     for (j=0; j<nattr; j++)   {
                         ret=SDattrinfo(sds_id, j,  attr_name, &attr_nt, 
					&attr_count);
                         attr_nt_desc = HDgetNTdesc(attr_nt);
                         if (ret == FAIL) {
                              printf("Failure in SDattrinfo %s\n", file_name);
                              exit(1);
                         }
                         attr_index=SDfindattr(sds_id, attr_name);
                         if (ret == FAIL) {
                             printf("Failure in SDfindattr %s\n", file_name);
                             exit(1);
                         }
                         attr_buf_size = DFKNTsize(attr_nt)*attr_count;
                         attr_buf =(VOIDP) HDmalloc(attr_buf_size);
                         if (attr_buf==NULL) {
                            printf("Not enough memory!\n");
                            exit(-1);
                         }
                         ret=SDreadattr(sds_id, attr_index, attr_buf);
                         if (ret == FAIL) {
                             printf("Failure in SDfindattr %s\n", file_name);
                             exit(1);
                         }
                         fprintf(fp, "\t Attr%d: Name = %s\n", (int)attr_index,
				 attr_name);
                         fprintf(fp, "\t\t Type = %s \n\t\t Count= %d\n", 
                                        attr_nt_desc, (int)attr_count);
                         HDfree(attr_nt_desc);
                         fprintf(fp, "\t\t Value = ");
                         ret = dumpfull(attr_nt, attr_count, attr_buf, 20, fp);
                         cn=0;
                         fprintf(fp, "\n");
                         HDfree((VOIDP)attr_buf);
                     }
                      
                     if (dumpsds_opts->contents == DHEADER) break;
                     
                  case DDATA:
                       if (dumpsds_opts->contents == DDATA)   {
                           /* print rank, dimsizes, nt, maxmin, cal info */
                       }
                       if (dumpsds_opts->contents!=DDATA)
                          fprintf(fp, "\t Data : \n");
                       if (rank >0 && dimsizes[0] != 0)    {
                           for (cn=0; cn<16; cn++)   
			      fprintf(fp, " ");
                           ret = sdsdumpfull(sds_id,rank,dimsizes,nt,16,fp);
                       }
		     
                       break;
              }  /* switch  */                     
              SDendaccess(sds_id);
          }  /* for ndsets  */
          SDend(sdf_id);
      }      /* while argc  */
      return(0);
}     /* dsd */


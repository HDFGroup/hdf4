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
static char RcsId[] = "@(#)$Revision$";
#endif

/* $Id$ */


#include <stdio.h>
#include "mfhdf.h"
#include "hdp.h"
#include <math.h>
#include "dfgr.h"
#include "dfrig.h"

#define IMAGE 1


extern void sort(int32 chosen[MAXCHOICES]);

static intn drig(dump_info_t *dumprig_opts, intn curr_arg, intn argc, 
		 char *argv[], int model);

void dumprig_usage(intn argc,char *argv[])
{
	printf("Usage:\n");
	printf("%s dumprig [-a|-i <index>|-n <name>|-r <ref>] [-dhv] [-o <filename> [-bx]] <filelist>\n",argv[0]);
	printf("\t-a\tDump all RIGs in the file (default)\n");
	printf("\t-i <index>\tDump the <index>th RIG in the file \n");
	printf("\t-n <name>\tDump the RIG with name <name>\n");
	printf("\t-r <ref>\tDump the RIG with reference number <ref>\n");
	printf("\t-d\tDump data only, no tag/ref, formatted to input to hp2hdf\n");
	printf("\t-h\tDump header only, no annotation for elements nor data\n");
	printf("\t-v\tDump everything including all annotations (default)\n");
	printf("\t-o <filename>\tOutput to file <filename>\n");
	printf("\t-b\tBinary format of output\n");
	printf("\t-x\tAscii text format of output (default)\n");
}	/* end list_usage() */

void init_dumprig_opts(dump_info_t *dumprig_opts)
{
	dumprig_opts->filter=DALL;		/* default dump all RIGs */
	dumprig_opts->filter_num=FALSE;       	/* not by reference nor by index */
	dumprig_opts->contents=DVERBOSE;	/* default dump all information */
	dumprig_opts->dump_to_file=FALSE;	/* don't dump to output file*/
	dumprig_opts->file_type=DASCII;		/* default output is ASCII file */
	HDstrcpy(dumprig_opts->file_name, "\0");
}	/* end init_list_opts() */

intn parse_dumprig_opts(dump_info_t *dumprig_opts, intn *curr_arg,
			intn argc, char *argv[], int *model)
{
   int32 i, k, lastItem, number=1;
   char *tempPtr, *ptr, *string;

        if (*curr_arg >= argc) 
	   return(FAIL); 
	while ((*curr_arg < argc) && (argv[*curr_arg][0]=='-')) {
	    switch(argv[*curr_arg][1]) {
		case 'm': /* dump the rigs with model specified as 8-bit
			     or 24-bit */
		    (*curr_arg)++;
		    *model = atoi(argv[*curr_arg]);
		    if (((*model)!=8) && ((*model)!=24)) {
		       printf("%s-bit raster model: not available.\n", 
			      argv[*curr_arg]);
		       exit(1);
		    }
		    (*curr_arg)++;
		    break;

		case 'a':	/* dump all, default */
                    dumprig_opts->filter = DALL;
                    (*curr_arg)++;
	 	    break;

		case 'i':	/* dump by index */
		    dumprig_opts->filter=DINDEX;	
                    (*curr_arg)++;
		    dumprig_opts->filter_num = (int*)HDmalloc(sizeof(int*)*1000);
		    string = (char*)HDmalloc(sizeof(char)*MAXNAMELEN);
		    ptr = argv[*curr_arg];
		    lastItem = 0;
		    for (i=0; !lastItem; i++) {
		       tempPtr = strchr(ptr, ',');
		       if (tempPtr==NULL)
			  lastItem = 1;
		       else
			  *tempPtr = '\0';
			  strcpy(string, ptr);
			  dumprig_opts->filter_num[i] = atoi(string);
			  ptr = tempPtr + 1;
		    }
		    dumprig_opts->filter_num[i] = -1;
                    (*curr_arg)++;
		    break;

                case 'r':       /* dump by reference */
                    dumprig_opts->filter=DREFNUM;
		    (*curr_arg)++;
                    dumprig_opts->filter_num = (int*)HDmalloc(sizeof(int*)*1000);
		    string = (char*)HDmalloc(sizeof(char)*MAXNAMELEN);
		    ptr = argv[*curr_arg]; 
		    lastItem = 0;
		    for (i=0; !lastItem; i++) {
		       tempPtr = strchr(ptr, ',');
		       if (tempPtr==NULL)
			  lastItem = 1;
		       else
			  *tempPtr = '\0';
		       strcpy(string,ptr);
		       dumprig_opts->filter_num[i] = atoi(string);
		       ptr = tempPtr + 1;
		    }
		    dumprig_opts->filter_num[i] = -1;
                    (*curr_arg)++;
                    break;

		case 'd':	/* dump data only */
                    dumprig_opts->contents = DDATA;
                    (*curr_arg)++;
		    break;

		case 'h':	/* no annotations nor data */
                    dumprig_opts->contents = DHEADER;
                    (*curr_arg)++;
		    break;

		case 'v':	/* dump all info */
                    dumprig_opts->contents = DVERBOSE;
                    (*curr_arg)++;
                    break;

		case 'o':	/* specify output file */
                    dumprig_opts->dump_to_file = TRUE; /* get filename */
                    HDstrcpy(dumprig_opts->file_name, argv[++(*curr_arg)]);
                    if (++(*curr_arg)<argc)  { /* binary or ascii */
                        if (argv[*curr_arg][0] == '-') 
                            dumprig_opts->file_type = (argv[*curr_arg][1]=='b')?
                                     DBINARY : DASCII;
                        else (*curr_arg)--; 
                    }
                    (*curr_arg)++;
	            break;

		default:	/* invalid dumprig option */
		    printf("Warning: Invalid dumprig option %s\n", 
                             argv[*curr_arg]);
                    return(FAIL);
	  }	/* end switch */
      }	/* end while */
      return(SUCCEED);
}	/* end parse_dumprig_opts */

void do_dumprig(intn curr_arg,intn argc,char *argv[],dump_opt_t *glob_opts)
{
    dump_info_t dumprig_opts;	/* dumprig options */
    intn ret;
    int model=0;

    if(glob_opts->help==TRUE) {
	dumprig_usage(argc,argv);
	return;
    }	/* end if */
    init_dumprig_opts(&dumprig_opts);
    if(parse_dumprig_opts(&dumprig_opts,&curr_arg,argc,argv,&model)==FAIL) {
	dumprig_usage(argc,argv);
	return;
    }	/* end if */
    switch (dumprig_opts.filter)  {
        case DINDEX : 
            ret = drig(&dumprig_opts, curr_arg, argc, argv, model);
            if (ret == FAIL) {
                printf("Failure in dump %ith RIG \n", dumprig_opts.filter_num);
                exit(1);
            }
            break;
        case DREFNUM :
            ret = drig(&dumprig_opts, curr_arg, argc, argv, model);
            if (ret == FAIL) {
                printf("Failure in dump ref=%i RIG \n", 
		       dumprig_opts.filter_num);
                exit(1);
            }
            break;
        default:
            ret = drig(&dumprig_opts, curr_arg, argc, argv, model);
            if (ret == FAIL) {
                printf("Failure in dump all RIGs \n");
                exit(1);
            }
            break;
        }  /* switch of filter  */
    return; 
}	/* end do_dumprig() */


static int32 cn = 0;

#define BUFFER 1000000

extern int32 dumpfull(int32 nt,int32 cnt,VOIDP databuf,intn indent,FILE *ofp);

static intn drig(dump_info_t *dumprig_opts, intn curr_arg, intn argc, 
		 char *argv[], int model)  
{
    int32 rig_id, rig_chosen[MAXCHOICES];
    int32 width, height, nt, ndsets, temp, index;
    int32 i, j, k, x, y, ret;
    char file_name[MAXFNLEN], name[MAXNAMELEN]; 
    FILE  *fp;
    VOIDP image;
    int index_error=0, dumpall=0, ncomps, il, rig;
    
    while (curr_arg < argc)   { /* Examine all files. */
        HDstrcpy(file_name, argv[curr_arg]);
        curr_arg++;
	for (i=0; i<MAXCHOICES; i++)
	   rig_chosen[i] = -1;

	/* Determine which RIGs are to be displayed. */
        switch (dumprig_opts->filter) {  
             case DINDEX:
		  for (i=0; dumprig_opts->filter_num[i]!=-1; i++) {
		     rig_chosen[i] = dumprig_opts->filter_num[i];
		     rig_chosen[i]--;
		  }
                  break;
             case DREFNUM:
                  break;
             case DALL:
                  break;
             default:
                  printf("Unknown filter option\n");
                  exit(1);
        }
        /* Get the name of the output file. */
        if (dumprig_opts->dump_to_file) 
	   fp = fopen(dumprig_opts->file_name, "w");
        else 
	   fp = stdout;
        if (dumprig_opts->contents!=DDATA)
           fprintf(fp, "File name: %s \n\n", file_name);

	/* Determine the number of images in a file. */
	if (model==8) {
	   if ((ndsets=DFR8nimages(file_name))==-1)
	      ndsets = 0;
        }
	else if (model==24) {
	   if ((ndsets=DF24nimages(file_name))==-1)
	      ndsets = 0;
        }
	else {
	   if ((temp = DFR8nimages(file_name))==-1)
	      temp = 0;
           if ((ndsets = DF24nimages(file_name))==-1)
              ndsets = 0;
           ndsets += temp;
        }

	x = 0; /* Used as the index of the array of "rig_chosen[x]". */

	if (rig_chosen[x]==-1) /* If all RIGs will be dumped, set the flat. */
	   dumpall = 1;
        else /* Otherwise, sort the indices of the chosen RIGs in increasing
		order. */
	   sort(rig_chosen); 
        ret = 0;

        for (i=0; i<ndsets; i++)   {  /* Examine all RIGs. */
	  int indent=5, compressed, has_pal;
          int32 m, n, ret;
          int32 eltsz, read_nelts;
	  uint16 rig_ref, compr_type;

             rig = DFGRgetimdims(file_name, &width, &height, &ncomps,
				 &il);
	     /* Determine the size of each element; "ncomps" is 1 for an
		8-bit image or 3 for a 24-bit image. */ 
	     eltsz = DFKNTsize(DFNT_INT8 | DFNT_NATIVE)*ncomps;
             read_nelts = width*height; /* Number of elements to be read in. */
             image=(VOIDP)HDmalloc(read_nelts*eltsz);  /* Allocate memory. */
	     if ((ret = DFGRIgetimlut((const char*)file_name, image, width, 
				       height, IMAGE, 0, &compressed, 
				       &compr_type, &has_pal))==-1) {
       	        printf("Read error!\n");
                exit(1);
             }

	     rig_ref = DFGRIlastref(); /* Determine the reference of the image
					  just read. */
	     
	     /* If the user has specificed the reference option, then 
		something has to be done. 
		Note: the reason why the following part was not done  
		inside the above "switch" statement is that the reference 
		number of a raster image cannot be appropriately retrieved
		before actually reading in a raster image. */
	     if (dumprig_opts->filter==DREFNUM) {
	        int ref_found=0, m;
		/* Determine if the image just read has the reference 
		   specified by the user. */
		for (m=0; dumprig_opts->filter_num[m]!=-1; m++)
		   if (dumprig_opts->filter_num[m]==rig_ref)
		      ref_found = 1;
		if (!ref_found) { /* If no match, then the current image is
				     not what the user wants and so skip it. */
		   HDfree((VOIDP)image);
		   continue;
		}
	     } 

	     /* If not all images are to be dumped out and the current image
		is not what the user wants or if the user has specified a 
		model and the model of the current image is not that one, then
		skip the current image. */
	     if (((!dumpall) && (i!=rig_chosen[x])) || 
		 (((ncomps*8)!=model) && (model!=0))) {
		HDfree((VOIDP)image);
		continue;
	     }
	     x++; 

	     /* Determine what to be dumped out. */
             switch (dumprig_opts->contents)   {
                 case DVERBOSE:
                 case DHEADER:
                     fprintf(fp,"Data model: %d-bit raster image ", ncomps*8);
		     if (has_pal)
			fprintf(fp,"with palette.\n");
		     else
			fprintf(fp,"without palette.\n");
		     fprintf(fp,"\twidth=%d;  height=%d\n", width, height);
		     fprintf(fp,"\treference=%d\n", rig_ref);
		     if (compressed) {
			fprintf(fp,"\t*data is compressed and the ");
			fprintf(fp,"compression scheme is ");
			switch (compr_type) {
			   case DFTAG_RLE: 
				fprintf(fp,"RLE compression.\n");
				break;
			   case DFTAG_IMCOMP: 
				fprintf(fp,"IMCOMP conmpression.\n");
				break;
			   case DFTAG_JPEG: 
				fprintf(fp,"JPEG conmpression ");
				fprintf(fp,"(24-bit data).\n");
				break;
			   case DFTAG_GREYJPEG: 
				fprintf(fp,"JPEG conmpression ");
				fprintf(fp,"(8-bit data).\n");
				break;
			} /* switch */
			fprintf(fp,"\n");
		     } /* if (compressed) */
		     else
			fprintf(fp,"\t*data is not compressed.\n");
                     if (dumprig_opts->contents==DHEADER) 
			break;
                  case DDATA:
                       if (dumprig_opts->contents!=DDATA)
                          fprintf(fp, "\tData : \n");
                       for (cn=0; cn<indent; cn++)   
			  fprintf(fp, " ");
		       ret = dumpfull(DFNT_INT8, read_nelts, image, indent,fp);
		       HDfree((VOIDP)image);
		       fprintf(fp, "\n");
		       fprintf(fp, "\n");
                       break;
              }  /* switch  */                     
          }  /* for ndsets  */
      }      /* while argc  */
      return(0);
}     /* drig */


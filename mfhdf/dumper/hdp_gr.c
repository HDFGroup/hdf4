/**********************************************************************
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
#include "mfgr.h"
#include "hdp.h"
#ifndef MIPSEL
#include <math.h>
#endif /* MIPSEL */

static intn dgr(dump_info_t * dumpgr_opts, intn curr_arg, intn argc, char *argv[], int model);

void 
dumpgr_usage(intn argc, char *argv[])
{
    printf("Usage:\n");
    printf("%s dumpgr [-a|-i <index>|-n <name>|-r <ref>] [-dhv] [-o <filename> [-bx]] <filelist>\n", argv[0]);
    printf("\t-a\tDump all GRs in the file (default)\n");
    printf("\t-i <index>\tDump the <index>th GR in the file \n");
    printf("\t-n <name>\tDump the GR with name <name>\n");
    printf("\t-r <ref>\tDump the GR with reference number <ref>\n");
    printf("\t-d\tDump data only, no tag/ref, formatted to input to hp2hdf\n");
    printf("\t-h\tDump header only, no annotation for elements nor data\n");
    printf("\t-v\tDump everything including all annotations (default)\n");
    printf("\t-o <filename>\tOutput to file <filename>\n");
    printf("\t-b\tBinary format of output\n");
    printf("\t-x\tAscii text format of output (default)\n");
}	/* end list_usage() */

void 
init_dumpgr_opts(dump_info_t * dumpgr_opts)
{
    dumpgr_opts->filter = DALL;	/* default dump all GRs */
    dumpgr_opts->filter_num = NULL;	/* not by reference nor by index */
    dumpgr_opts->filter_str = NULL;	/* no strings */
    dumpgr_opts->num_chosen = (-1);	/* default dump all items */
    dumpgr_opts->contents = DVERBOSE;	/* default dump all information */
    dumpgr_opts->dump_to_file = FALSE;		/* don't dump to output file */
    dumpgr_opts->file_type = DASCII;	/* default output is ASCII file */
    HDstrcpy(dumpgr_opts->file_name, "\0");
}	/* end init_list_opts() */

intn 
parse_dumpgr_opts(dump_info_t * dumpgr_opts, intn *curr_arg, intn argc, char *argv[], int *model)
{

    int32       i, numItems;
    char       *tempPtr, *ptr;

    if (*curr_arg >= argc)
        return (FAIL);

    while ((*curr_arg < argc) && (argv[*curr_arg][0] == '-'))
      {
          switch (argv[*curr_arg][1])
            {
            case 'a':	/* dump all, default */
                dumpgr_opts->filter = DALL;
                (*curr_arg)++;
                break;
            
            case 'm':   /* dump the rigs with model specified  */
                (*curr_arg)++;
                *model = atoi(argv[*curr_arg]);
                break;
                

            case 'i':	/* dump by index */
            case 'r':	/* dump by reference */
                if ((argv[*curr_arg][1]) == 'i')
                    dumpgr_opts->filter = DINDEX;
                else
                    dumpgr_opts->filter = DREFNUM;
                (*curr_arg)++;

                ptr = argv[*curr_arg];
                numItems = 0;
                while ((tempPtr = HDstrchr(ptr, ',')) != NULL)
                  {
                      numItems++;
                      ptr=tempPtr+1;
                  }		/* end while */
                if (*ptr != '\0')	/* count the last item */
                    numItems++;

                dumpgr_opts->filter_num = (intn *) HDmalloc(sizeof(intn) * numItems);
                if (dumpgr_opts->filter_num == NULL)
                  {
                      printf("Not enough memory!\n");
                      exit(-1);
                  }
                ptr = argv[*curr_arg];
                i = 0;
                while ((tempPtr = HDstrchr(ptr, ',')) != NULL)
                  {
                      *tempPtr = '\0';
                      dumpgr_opts->filter_num[i] = atoi(ptr);
                      ptr = tempPtr + 1;
                      i++;
                  }
                dumpgr_opts->filter_num[i] = atoi(ptr);	/* get the last item */
                dumpgr_opts->num_chosen = numItems;	/* save the number of items */

                (*curr_arg)++;
                break;

            case 'n':	/* dump by names */
                dumpgr_opts->filter = DNAME;	/* dump by name */
                (*curr_arg)++;

                ptr = argv[*curr_arg];
                numItems = 0;
                while ((tempPtr = HDstrchr(ptr, ',')) != NULL)
                  {
                      numItems++;
                      ptr=tempPtr+1;
                  }		/* end while */
                if (*ptr != '\0')	/* count the last item */
                    numItems++;

                    /* allocate space for the array */
                if ((dumpgr_opts->filter_str = (char **) HDmalloc(sizeof(char *) * numItems)) == NULL)
                  {
                      printf("Not enough memory!\n");
                      exit(-1);
                  }

                ptr = argv[*curr_arg];
                i = 0;
                while ((tempPtr = HDstrchr(ptr, ',')) != NULL)
                  {
                      *tempPtr = '\0';
                      if ((dumpgr_opts->filter_str[i] = (char *) HDmalloc(sizeof(char) * (HDstrlen(ptr) + 1))) == NULL)
                        {
                            printf("Not enough memory!\n");
                            exit(-1);
                        }
                      HDstrcpy(dumpgr_opts->filter_str[i], ptr);
                      ptr = tempPtr + 1;
                      i++;
                  }		/* end while */

                    /* Get the last string */
                if ((dumpgr_opts->filter_str[i] = (char *) HDmalloc(sizeof(char) * (HDstrlen(ptr) + 1))) == NULL)
                  {
                      printf("Not enough memory!\n");
                      exit(-1);
                  }
                HDstrcpy(dumpgr_opts->filter_str[i], ptr);
                dumpgr_opts->num_chosen = numItems;	/* save the number of items */

                (*curr_arg)++;
                break;

            case 'd':	/* dump data only */
                dumpgr_opts->contents = DDATA;
                (*curr_arg)++;
                break;

            case 'h':	/* no annotations nor data */
                dumpgr_opts->contents = DHEADER;
                (*curr_arg)++;
                break;

            case 'v':	/* dump all info */
                dumpgr_opts->contents = DVERBOSE;
                (*curr_arg)++;
                break;

            case 'o':	/* specify output file */
                dumpgr_opts->dump_to_file = TRUE;	/* get filename */
                HDstrcpy(dumpgr_opts->file_name, argv[++(*curr_arg)]);
                if (++(*curr_arg) < argc)
                  {		/* binary or ascii */
                      if (argv[*curr_arg][0] == '-')
                          dumpgr_opts->file_type = (argv[*curr_arg][1] == 'b') ? DBINARY : DASCII;
                      else
                          (*curr_arg)--;
                  }
                (*curr_arg)++;
                break;

            default:	/* invalid dumpgr option */
                printf("Warning: Invalid dumpgr option %s\n", argv[*curr_arg]);
                return (FAIL);
            }	/* end switch */
      }		/* end while */
    return (SUCCEED);
}	/* end parse_dumpgr_opts */

void 
do_dumpgr(intn curr_arg, intn argc, char *argv[], dump_opt_t * glob_opts)
{
    dump_info_t dumpgr_opts;	/* dumpgr options */
    int model = 0;

    if (glob_opts->help == TRUE)
      {
          dumpgr_usage(argc, argv);
          return;
      }		/* end if */

    init_dumpgr_opts(&dumpgr_opts);
    if (parse_dumpgr_opts(&dumpgr_opts, &curr_arg, argc, argv, &model) == FAIL)
      {
          dumpgr_usage(argc, argv);
          return;
      }		/* end if */

    if (dgr(&dumpgr_opts, curr_arg, argc, argv, model) == FAIL)
      {
          printf("Failure in dumping GR data\n");
          exit(1);
      }

	if ( dumpgr_opts.filter_num != NULL )
           HDfree( dumpgr_opts.filter_num);

        if( dumpgr_opts.filter_str != NULL)
          HDfree(dumpgr_opts.filter_str);

}	/* end do_dumpgr() */

int32 
grdumpfull(dump_info_t * dumpgr_opts, int32 ri_id, file_type_t ft, int32 ncomp,                int32 dimsizes[], int32 nt , intn indent, FILE * fp)
{
	/* "ncomp" is the number of components in each element of the data 
            set 
	   "dimsizes[i]" is size of dimension "i". */
    int32       ret;
    VOIDP       buf;
    int32       numtype, eltsz, read_nelts;
    int32       *start, *edge, *stride;

    if (indent > 65)
      {		/* This block is probably not necessary. */
          printf("Bad indentation %d\n", indent);
          exit(1);
      }

	/* Compute the number of the bytes for each value. */
    numtype = nt & DFNT_MASK;
    eltsz = DFKNTsize(numtype | DFNT_NATIVE)*ncomp;
    
    read_nelts = dimsizes[0]*dimsizes[1];
    buf = (VOIDP) HDmalloc(read_nelts * eltsz);
    if (buf == NULL)
      {
          printf("Not enough memory!\n");
          exit(-1);
      }
    start = (int32 *) HDmalloc(ncomp * sizeof(int32));
    if (start == NULL)
      {
          printf("Not enough memory!\n");
          exit(-1);
      }
   edge = (int32 *) HDmalloc(ncomp * sizeof(int32));
    if (edge == NULL)
      {
          printf("Not enough memory!\n");
          exit(-1);
      }

    stride = (int32 *) HDmalloc(ncomp * sizeof(int32));
    if (stride == NULL)
      {
          printf("Not enough memory!\n");
          exit(-1);
      }

    start[0]=start[1]=0;
    edge[0]=dimsizes[0];
    edge[1]=dimsizes[1];
    stride[0]=1;
    stride[1]=1;
 
    ret = GRreadimage(ri_id, start, stride, edge, buf);
    /*  GRendaccess(ri_id);   */

/*---------------------------------------------------------*/


    ret = dumpfull(nt, ft, read_nelts*ncomp, buf, indent, fp);

    HDfree((VOIDP) edge);
    HDfree((VOIDP) start);
    HDfree((VOIDP) stride);
    HDfree((VOIDP) buf);
    return (0);
}	/* grdumpfull */

static intn 
dgr(dump_info_t * dumpgr_opts, intn curr_arg, intn argc, char *argv[], 
    int model)   
{
    intn        i, ret;
    int32       grf_id, gr_id, ri_id, *gr_chosen=NULL;
    int32       num_gr_chosen;
    int32       ncomp, nt, nattr, ndsets, il, nglb_attr;
    int32       j, k, attr_nt, attr_count, attr_buf_size, attr_index;
    char        file_name[MAXFNLEN], name[MAXNAMELEN];
    char        attr_name[MAXNAMELEN];
    int32       dimsizes[MAXRANK], dimNT[MAXRANK], dimnattr[MAXRANK];
    FILE       *fp=NULL;
    int32 /* ref , */ index;
    VOIDP       attr_buf;
    char       *nt_desc, *attr_nt_desc;
    int         index_error=0, x, dumpall = 0, ref_found=0;
    int32       ri_ref;
    file_type_t    ft;

    while (curr_arg < argc)
      {		/* Examine each file. */
          HDstrcpy(file_name, argv[curr_arg]);
          curr_arg++;
          grf_id = Hopen(file_name, DFACC_RDONLY,0);
          if (grf_id == FAIL)
            {
                printf("Failure in open %s\n", file_name);
                exit(1);
            }
          gr_id = GRstart(grf_id);
          num_gr_chosen = dumpgr_opts->num_chosen;
          if (num_gr_chosen > 0)
            {
                if ((gr_chosen = (int32 *) HDmalloc(sizeof(int32) * num_gr_chosen)) == NULL)
                  {
                      printf("Memory allocation error\n");
                      exit(1);
                  }		/* end if */

                k = (-1);
                HDmemfill(gr_chosen, &k, sizeof(int32), num_gr_chosen);
            }	/* end if */

          k = 0;
          switch (dumpgr_opts->filter)
            {	/* Determine the GRs having been selected. */
            case DINDEX:
                    /* Note: Don't replace this with HDmemcpy unless you change the sizes of the objects correctly -QAK */
                for (i = 0; i < dumpgr_opts->num_chosen; i++)
                  {
                      gr_chosen[i] = dumpgr_opts->filter_num[i];
                      k++;
                  }
                break;

            case DREFNUM:
              /*  for (i = 0; i < dumpgr_opts->num_chosen; i++)
                  {
                      index = GRreftoindex(gr_id, dumpgr_opts->filter_num[i]);
                 
                      if (index == -1)
                        {
                            printf("GR with reference number %d: not found\n", dumpgr_opts->filter_num[i]);
                            index_error = 1;     
                        }
                      else
                        {
                            gr_chosen[k] = index;
                            k++;
                        }
                  }             */
                break;

            case DNAME:
                for (i = 0; i < dumpgr_opts->num_chosen; i++)
                  {
                      index = GRnametoindex(gr_id, dumpgr_opts->filter_str[i]);
                      if (index == -1)
                        {
                            printf("GR with name %s: not found\n", dumpgr_opts->filter_str[i]);
                            index_error = 1;
                        }
                      else
                        {
                            gr_chosen[k] = index;
                            k++;
                        }
                  }
                break;

            case DCLASS:
                printf("Currently, no class defined on an GR.\n");
                exit(1);

            case DALL:
                k= -1;
                break;

            default:
                printf("Unknown filter option\n");
                exit(1);
            }

          if (index_error && k==0)
            {
                if(gr_chosen!=NULL)
                  {
                      HDfree(gr_chosen);
                      gr_chosen=NULL;
                  } /* end if */
                GRend(gr_id);
                continue;
            } /* end if */
          num_gr_chosen=k;  /* update actual count of items chosen */

          ft=dumpgr_opts->file_type;
         
          switch(ft)
          {
           case DASCII:       /* ASCII file */

              /* get output file name  */
          if (dumpgr_opts->dump_to_file)
              fp = fopen(dumpgr_opts->file_name, "w");
          else
              fp = stdout;

          if (dumpgr_opts->contents != DDATA)
              fprintf(fp, "File name: %s \n", file_name);
          ret = GRfileinfo(gr_id, &ndsets, &nglb_attr);
          if (ret == FAIL)
            {
                printf("Failure in GRfileinfo %s\n", file_name);
                exit(1);
            }

          x = 0;
          if (num_gr_chosen == (-1))
              dumpall = 1;
          else
              sort(gr_chosen, num_gr_chosen);

          for (i = 0; i < ndsets && (dumpall!=0 || x<dumpgr_opts->num_chosen); i++)
            {	/* Examine each GR. */
                if ((!dumpall) && (i != gr_chosen[x])&&(dumpgr_opts->filter!= DREFNUM))
                    continue;
                if(dumpgr_opts->filter!= DREFNUM) x++;

                    /* Reset variables. */
                HDmemset(dimsizes, 0, sizeof(int32) * MAXRANK);
                HDmemset(dimNT, 0, sizeof(int32) * MAXRANK);
                HDmemset(dimnattr, 0, sizeof(int32) * MAXRANK);

                ri_id = GRselect(gr_id, i);
                if (ri_id == FAIL)
                  {
                      printf("Failure in selecting %s\n", file_name);
                      exit(1);
                  }
                if ((ri_ref = GRidtoref(ri_id)) == FAIL)
                  {
                      printf("Failure in determining reference no.\n");
                      exit(1);
                  }

    if (dumpgr_opts->filter == DREFNUM)
                  {
                      int  m;
                      ref_found = 0;

                      for (m = 0; m < dumpgr_opts->num_chosen; m++)
                          if (dumpgr_opts->filter_num[m] == ri_ref)
                             {
                              ref_found = 1;
                              break;
                             };
                      if (!ref_found)
                        continue;  
                  }

                ret = GRgetiminfo(ri_id, name, &ncomp, &nt, &il, dimsizes, &nattr);

                switch (dumpgr_opts->contents)
                  {
                  case DVERBOSE:
/*                     ref = GRidtoref(ri_id);
                       if (ref == FAIL) {
                       printf("Failure in GRidtoref %s\n", file_name);
                       exit(1);
                       }
                       */
                  case DHEADER:
                      nt_desc = HDgetNTdesc(nt);
                      if (ret == FAIL)
                        {
                            printf("Failure in GRgetiminfo %s\n", file_name);
                            exit(1);
                        }
                            fprintf(fp, "\n\t Image  Name = %s\n\t Index = ", name);
                            fprintf(fp, "%d\n\t Type= %s\n", i, nt_desc);
                      HDfree(nt_desc);
                      fprintf(fp, "\t width=%d; height=%d\n", (int) dimsizes[0], (int) dimsizes[1]);
                      fprintf(fp, "\t Ref. = %d\n", (int) ri_ref);
                      fprintf(fp, "\t ncomps = %d\n\t Number of attributes = %d\n\t Interlace= %d\n", (int) ncomp, (int) nattr,(int) il);

                          /* Print annotations */
                      for (j = 0; j < nattr; j++)
                        {
                            ret = GRattrinfo(ri_id, j, attr_name, &attr_nt, &attr_count);
                            attr_nt_desc = HDgetNTdesc(attr_nt);
                            if (ret == FAIL)
                              {
                                  printf("Failure in GRattrinfo %s\n", file_name);
                                  exit(1);
                              }
                            attr_index = GRfindattr(ri_id, attr_name);
                            if (ret == FAIL)
                              {
                                  printf("Failure in GRfindattr %s\n", file_name);
                                  exit(1);
                              }
                            attr_buf_size = DFKNTsize(attr_nt) * attr_count;
                            attr_buf = (VOIDP) HDmalloc(attr_buf_size);
                            if (attr_buf == NULL)
                              {
                                  printf("Not enough memory!\n");
                                  exit(-1);
                              }
                            ret = GRgetattr(ri_id, attr_index, attr_buf);
                            if (ret == FAIL)
                              {
                                  printf("Failure in GRfindattr %s\n", file_name);
                                  exit(1);
                              }
                            fprintf(fp, "\t Attr%d: Name = %s\n", (int) attr_index, attr_name);
                            fprintf(fp, "\t\t Type = %s \n\t\t Count= %d\n", attr_nt_desc, (int) attr_count);
                            HDfree(attr_nt_desc);
                            fprintf(fp, "\t\t Value = ");
                            ret = dumpfull(attr_nt, ft, attr_count, attr_buf, 20, fp);
                            HDfree((VOIDP) attr_buf);
                        }

                      if (dumpgr_opts->contents == DHEADER)
                         { GRendaccess(ri_id);
                          break;
                         }

                  case DDATA:
                      if (dumpgr_opts->contents == DDATA)
                        {
                                /* print ncomp, dimsizes, nt, maxmin, cal info */
                        }
                      if (dumpgr_opts->contents != DDATA)
                          fprintf(fp, "\t Data : \n");
                      if (ncomp > 0 && dimsizes[0] != 0)
                        {
                            intn        count;

                            for (count = 0; count < 16; count++)
                                fprintf(fp, " ");
                            ret = grdumpfull(dumpgr_opts, ri_id, ft, ncomp, 
                                             dimsizes, nt, 16, fp);
                        }

                      break;
                  }		/* switch  */
                GRendaccess(ri_id);    
            }	/* for ndsets  */
          break;

       case DBINARY:       /* binary file */
         /* get output file name  */
          if (dumpgr_opts->dump_to_file)
              fp = fopen(dumpgr_opts->file_name, "w");
          else
              fp = stdout;

       /*   if (dumpgr_opts->contents != DDATA)
              fprintf(fp, "File name: %s \n", file_name);     */

          ret = GRfileinfo(gr_id, &ndsets, &nglb_attr);
          if (ret == FAIL)
            {
                printf("Failure in GRfileinfo %s\n", file_name);
                exit(1);
            }

          x = 0;
          if (num_gr_chosen == (-1))
              dumpall = 1;
          else
              sort(gr_chosen, num_gr_chosen);

          for(i = 0; i < ndsets && (dumpall!=0 || x<dumpgr_opts->num_chosen); i++)
            {   /* Examine each GR. */
                if ((!dumpall) && (i != gr_chosen[x])&&(dumpgr_opts->filter!=DREFNUM))
                    continue;
                if(dumpgr_opts->filter!=DREFNUM) x++;

                    /* Reset variables. */
                HDmemset(dimsizes, 0, sizeof(int32) * MAXRANK);
                HDmemset(dimNT, 0, sizeof(int32) * MAXRANK);
                HDmemset(dimnattr, 0, sizeof(int32) * MAXRANK);

                ri_id = GRselect(gr_id, i);
                if (ri_id == FAIL)
                  {
                      printf("Failure in selecting %s\n", file_name);
                      exit(1);
                  }
                if ((ri_ref = GRidtoref(ri_id)) == FAIL)
                 {
                      printf("Failure in determining reference no.\n");
                      exit(1);
                  }
              if (dumpgr_opts->filter == DREFNUM)
                  {
                      int  m;
                      ref_found = 0;

                      for (m = 0; m < dumpgr_opts->num_chosen; m++)
                          if (dumpgr_opts->filter_num[m] == ri_ref)
                             {
                              ref_found = 1;
                              break;
                             };
                      if (!ref_found)
                        continue;
                  }  
               ret = GRgetiminfo(ri_id, name, &ncomp, &nt, &il, dimsizes, &nattr);

          /*   output data to binary file   */

            if (ncomp > 0 && dimsizes[0] != 0)
                ret = grdumpfull(dumpgr_opts, ri_id, ft, ncomp, dimsizes, nt, 16, fp);
               GRendaccess(ri_id);
             }        /* for ndsets */
            break;
        } /* switch for output file */
                       

          GRend(gr_id);
          Hclose(grf_id);
          if (gr_chosen!=NULL)
            {
                HDfree(gr_chosen);
                gr_chosen=NULL;
            } /* end if */
          if (dumpgr_opts->dump_to_file)
              fclose(fp);
      }		/* while argc  */
    return (0);
}	/* dgr */

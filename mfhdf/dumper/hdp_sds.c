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
#include "hdp.h"
#ifndef MIPSEL
#include <math.h>
#endif /* MIPSEL */

static intn dsd(dump_info_t * dumpsds_opts, intn curr_arg, intn argc, char *argv[]);

void 
dumpsds_usage(intn argc, char *argv[])
{
    printf("Usage:\n");
    printf("%s dumpsds [-a|-i <index>|-n <name>|-r <ref>] [-dhv] [-o <filename> [-bx]] <filelist>\n", argv[0]);
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

void 
init_dumpsds_opts(dump_info_t * dumpsds_opts)
{
    dumpsds_opts->filter = DALL;	/* default dump all SDSs */
    dumpsds_opts->filter_num = NULL;	/* not by reference nor by index */
    dumpsds_opts->filter_str = NULL;	/* no strings */
    dumpsds_opts->num_chosen = (-1);	/* default dump all items */
    dumpsds_opts->contents = DVERBOSE;	/* default dump all information */
    dumpsds_opts->dump_to_file = FALSE;		/* don't dump to output file */
    dumpsds_opts->file_type = DASCII;	/* default output is ASCII file */
    HDstrcpy(dumpsds_opts->file_name, "\0");
}	/* end init_list_opts() */

intn 
parse_dumpsds_opts(dump_info_t * dumpsds_opts, intn *curr_arg, intn argc, char *argv[])
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
                dumpsds_opts->filter = DALL;
                (*curr_arg)++;
                break;

            case 'i':	/* dump by index */
            case 'r':	/* dump by reference */
                if ((argv[*curr_arg][1]) == 'i')
                    dumpsds_opts->filter = DINDEX;
                else
                    dumpsds_opts->filter = DREFNUM;
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

                dumpsds_opts->filter_num = (intn *) HDmalloc(sizeof(intn) * numItems);
                if (dumpsds_opts->filter_num == NULL)
                  {
                      printf("Not enough memory!\n");
                      exit(-1);
                  }
                ptr = argv[*curr_arg];
                i = 0;
                while ((tempPtr = HDstrchr(ptr, ',')) != NULL)
                  {
                      *tempPtr = '\0';
                      dumpsds_opts->filter_num[i] = atoi(ptr);
                      ptr = tempPtr + 1;
                      i++;
                  }
                dumpsds_opts->filter_num[i] = atoi(ptr);	/* get the last item */
                dumpsds_opts->num_chosen = numItems;	/* save the number of items */

                (*curr_arg)++;
                break;

            case 'n':	/* dump by names */
                dumpsds_opts->filter = DNAME;	/* dump by name */
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
                if ((dumpsds_opts->filter_str = (char **) HDmalloc(sizeof(char *) * numItems)) == NULL)
                  {
                      printf("Not enough memory!\n");
                      exit(-1);
                  }

                ptr = argv[*curr_arg];
                i = 0;
                while ((tempPtr = HDstrchr(ptr, ',')) != NULL)
                  {
                      *tempPtr = '\0';
                      if ((dumpsds_opts->filter_str[i] = (char *) HDmalloc(sizeof(char) * (HDstrlen(ptr) + 1))) == NULL)
                        {
                            printf("Not enough memory!\n");
                            exit(-1);
                        }
                      HDstrcpy(dumpsds_opts->filter_str[i], ptr);
                      ptr = tempPtr + 1;
                      i++;
                  }		/* end while */

                    /* Get the last string */
                if ((dumpsds_opts->filter_str[i] = (char *) HDmalloc(sizeof(char) * (HDstrlen(ptr) + 1))) == NULL)
                  {
                      printf("Not enough memory!\n");
                      exit(-1);
                  }
                HDstrcpy(dumpsds_opts->filter_str[i], ptr);
                dumpsds_opts->num_chosen = numItems;	/* save the number of items */

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
                dumpsds_opts->dump_to_file = TRUE;	/* get filename */
                HDstrcpy(dumpsds_opts->file_name, argv[++(*curr_arg)]);
                if (++(*curr_arg) < argc)
                  {		/* binary or ascii */
                      if (argv[*curr_arg][0] == '-')
                          dumpsds_opts->file_type = (argv[*curr_arg][1] == 'b') ? DBINARY : DASCII;
                      else
                          (*curr_arg)--;
                  }
                (*curr_arg)++;
                break;

            default:	/* invalid dumpsds option */
                printf("Warning: Invalid dumpsds option %s\n", argv[*curr_arg]);
                return (FAIL);
            }	/* end switch */
      }		/* end while */
    return (SUCCEED);
}	/* end parse_dumpsds_opts */

void 
do_dumpsds(intn curr_arg, intn argc, char *argv[], dump_opt_t * glob_opts)
{
    int32 i;
    char tempPtr;
    dump_info_t dumpsds_opts;	/* dumpsds options */

    if (glob_opts->help == TRUE)
      {
          dumpsds_usage(argc, argv);
          return;
      }		/* end if */

    init_dumpsds_opts(&dumpsds_opts);
    if (parse_dumpsds_opts(&dumpsds_opts, &curr_arg, argc, argv) == FAIL)
      {
          dumpsds_usage(argc, argv);
          return;
      }		/* end if */

    if (dsd(&dumpsds_opts, curr_arg, argc, argv) == FAIL)
      {
          printf("Failure in dumping SDS data\n");
          exit(1);
      }

	if ( dumpsds_opts.filter_num != NULL )
           HDfree( dumpsds_opts.filter_num);

        if( dumpsds_opts.filter_str != NULL)
          HDfree(dumpsds_opts.filter_str);

}	/* end do_dumpsds() */

int32 
sdsdumpfull(int32 sds_id, int32 rank, int32 dimsizes[], int32 nt,
            intn indent, FILE * fp)
{
	/* "rank" is the number of dimensions and 
	   "dimsizes[i]" is size of dimension "i". */
    int32       j, i, ret;
    VOIDP       buf;
    int32       numtype, eltsz, read_nelts;
    int32       done;			/* number of rows we have done */
    int32      *left, *start, *edge;
    intn        count;

    if (indent > 65)
      {		/* This block is probably not necessary. */
          printf("Bad indentation %d\n", indent);
          exit(1);
      }

	/* Compute the number of the bytes for each value. */
    numtype = nt & DFNT_MASK;
    eltsz = DFKNTsize(numtype | DFNT_NATIVE);

    read_nelts = dimsizes[rank - 1];
    buf = (VOIDP) HDmalloc(read_nelts * eltsz);
    if (buf == NULL)
      {
          printf("Not enough memory!\n");
          exit(-1);
      }
    left = (int32 *) HDmalloc(rank * sizeof(int32));
    if (left == NULL)
      {
          printf("Not enough memory!\n");
          exit(-1);
      }
    start = (int32 *) HDmalloc(rank * sizeof(int32));
    if (start == NULL)
      {
          printf("Not enough memory!\n");
          exit(-1);
      }
    edge = (int32 *) HDmalloc(rank * sizeof(int32));
    if (edge == NULL)
      {
          printf("Not enough memory!\n");
          exit(-1);
      }
    for (i = 0; i < rank; i++)
      {
          start[i] = 0;		/* Starting location to read the data. */
          left[i] = dimsizes[i];
          edge[i] = 1;	/* Number of values to read in each dimension. */
      }
    edge[rank - 1] = dimsizes[rank - 1];

    if (rank == 1)
      {		/* If there is only one dimension, then dump the data
                   and the job is done. */
          ret = SDreaddata(sds_id, start, NULL, edge, buf);
          ret = dumpfull(numtype, read_nelts, buf, indent, fp);
      }
    else if (rank > 1)
      {
          done = 0;
          while (!done)
            {	/* In each iteration, a row in dumped and "left[]" is modified accordingly. */
                ret = SDreaddata(sds_id, start, NULL, edge, buf);
                ret = dumpfull(numtype, read_nelts, buf, indent, fp);
                for (count = 0; count < indent; count++)
                    putc(' ', fp);

                    /* Modify the values for "start[]" and "left[]" that are to be used
                       for dumping the next row. */

                    /* The following index variable "j" starts from "rank-2" because:
                       (1) the range is from 0 to rank-1
                       (2) each element in dimension rank-1 is just an element in a row
                       which is read in each time, and so we don't have to compute
                       the "start" of it. */

                for (j = rank - 2; j >= 0; j--)
                  {		/* Examine each dimension. */
                      if (--left[j] > 0)
                        {	/* Proceed in the same dimension; as
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
                      else
                        {	/* Nothing left in the current dimension.
                                   So, subtract one from the (j-1)th dimension and
                                   reset the value of "left[j]". */

                            left[j] = dimsizes[j];
                            start[j] = 0;
                            if (j == 0)
                                done = 1;
                            if (j == rank - 2)
                              {
                                  fprintf(fp, "\n");
                                  for (count = 0; count < indent; count++)
                                      putc(' ', fp);
                              }
                        }
                  }		/* for j */
            }	/* while   */
      }		/* else */
    HDfree((VOIDP) edge);
    HDfree((VOIDP) start);
    HDfree((VOIDP) left);
    HDfree((VOIDP) buf);
    fprintf(fp, "\n");
    return (0);
}	/* sdsdumpfull */

static intn 
dsd(dump_info_t * dumpsds_opts, intn curr_arg, intn argc, char *argv[])
{
    intn        i, ret, isdimvar;
    int32       sdf_id, sds_id, *sd_chosen=NULL;
    int32       num_sd_chosen;
    int32       rank, nt, nattr, ndsets, nglb_attr;
    int32       j, k, attr_nt, attr_count, attr_buf_size, attr_index;
    char        file_name[MAXFNLEN], name[MAXNAMELEN];
    char        attr_name[MAXNAMELEN], dim_nm[MAXNAMELEN];
    int32       dimsizes[MAXRANK], dim_id, dimNT[MAXRANK], dimnattr[MAXRANK];
    FILE       *fp;
    int32 /* ref , */ index;
    VOIDP       attr_buf;
    char       *nt_desc, *attr_nt_desc;
    int         index_error=0, x, dumpall = 0;
    int32       sd_ref;

    while (curr_arg < argc)
      {		/* Examine each file. */
          HDstrcpy(file_name, argv[curr_arg]);
          curr_arg++;
          sdf_id = SDstart(file_name, DFACC_RDONLY);
          if (sdf_id == FAIL)
            {
                printf("Failure in open %s\n", file_name);
                exit(1);
            }

          num_sd_chosen = dumpsds_opts->num_chosen;
          if (num_sd_chosen > 0)
            {
                if ((sd_chosen = (int32 *) HDmalloc(sizeof(int32) * num_sd_chosen)) == NULL)
                  {
                      printf("Memory allocation error\n");
                      exit(1);
                  }		/* end if */

                k = (-1);
                HDmemfill(sd_chosen, &k, sizeof(int32), num_sd_chosen);
            }	/* end if */

          k = 0;
          switch (dumpsds_opts->filter)
            {	/* Determine the SDs having been selected. */
            case DINDEX:
                    /* Note: Don't replace this with HDmemcpy unless you change the sizes of the objects correctly -QAK */
                for (i = 0; i < dumpsds_opts->num_chosen; i++)
                  {
                      sd_chosen[i] = dumpsds_opts->filter_num[i];
                      k++;
                  }
                break;

            case DREFNUM:
                for (i = 0; i < dumpsds_opts->num_chosen; i++)
                  {
                      index = SDreftoindex(sdf_id, dumpsds_opts->filter_num[i]);
                      if (index == -1)
                        {
                            printf("SD with reference number %d: not found\n", dumpsds_opts->filter_num[i]);
                            index_error = 1;
                        }
                      else
                        {
                            sd_chosen[k] = index;
                            k++;
                        }
                  }
                break;

            case DNAME:
                for (i = 0; i < dumpsds_opts->num_chosen; i++)
                  {
                      index = SDnametoindex(sdf_id, dumpsds_opts->filter_str[i]);
                      if (index == -1)
                        {
                            printf("SD with name %s: not found\n", dumpsds_opts->filter_str[i]);
                            index_error = 1;
                        }
                      else
                        {
                            sd_chosen[k] = index;
                            k++;
                        }
                  }
                break;

            case DCLASS:
                printf("Currently, no class defined on an SD.\n");
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
                if(sd_chosen!=NULL)
                  {
                      HDfree(sd_chosen);
                      sd_chosen=NULL;
                  } /* end if */
                SDend(sdf_id);
                continue;
            } /* end if */
          num_sd_chosen=k;  /* update actual count of items chosen */

              /* get output file name  */
          if (dumpsds_opts->dump_to_file)
              fp = fopen(dumpsds_opts->file_name, "w");
          else
              fp = stdout;

          if (dumpsds_opts->contents != DDATA)
              fprintf(fp, "File name: %s \n", file_name);
          ret = SDfileinfo(sdf_id, &ndsets, &nglb_attr);
          if (ret == FAIL)
            {
                printf("Failure in SDfileinfo %s\n", file_name);
                exit(1);
            }

          x = 0;
          if (num_sd_chosen == (-1))
              dumpall = 1;
          else
              sort(sd_chosen, num_sd_chosen);

          for (i = 0; i < ndsets && (dumpall!=0 || x<dumpsds_opts->num_chosen); i++)
            {	/* Examine each SD. */
                if ((!dumpall) && (i != sd_chosen[x]))
                    continue;
                x++;

                    /* Reset variables. */
                HDmemset(dimsizes, 0, sizeof(int32) * MAXRANK);
                HDmemset(dimNT, 0, sizeof(int32) * MAXRANK);
                HDmemset(dimnattr, 0, sizeof(int32) * MAXRANK);

                sds_id = SDselect(sdf_id, i);
                if (sds_id == FAIL)
                  {
                      printf("Failure in selecting %s\n", file_name);
                      exit(1);
                  }
                if ((sd_ref = SDidtoref(sds_id)) == FAIL)
                  {
                      printf("Failure in determining reference no.\n");
                      exit(1);
                  }
                isdimvar = (SDiscoordvar(sds_id)) ? 1 : 0;
                ret = SDgetinfo(sds_id, name, &rank, dimsizes, &nt, &nattr);

                switch (dumpsds_opts->contents)
                  {
                  case DVERBOSE:
/*                     ref = SDidtoref(sds_id);
                       if (ref == FAIL) {
                       printf("Failure in SDidtoref %s\n", file_name);
                       exit(1);
                       }
                       */
                  case DHEADER:
                      nt_desc = HDgetNTdesc(nt);
                      if (ret == FAIL)
                        {
                            printf("Failure in SDgetinfo %s\n", file_name);
                            exit(1);
                        }
                      if (isdimvar)
                        {
                            fprintf(fp, "\nDimension Variable Name = %s\n\t ", name);
                            fprintf(fp, "Index = %d\n\t Type= %s\n", i, nt_desc);
                        }
                      else
                        {
                            fprintf(fp, "\nVariable Name = %s\n\t Index = ", name);
                            fprintf(fp, "%d\n\t Type= %s\n", i, nt_desc);
                        }
                      HDfree(nt_desc);
                      fprintf(fp, "\t Ref. = %d\n", (int) sd_ref);
                      fprintf(fp, "\t Rank = %d\n\t Number of attributes = %d\n", (int) rank, (int) nattr);
                      for (j = 0; j < rank; j++)
                        {
                            int32       temp;

                            dim_id = SDgetdimid(sds_id, j);
                            ret = SDdiminfo(dim_id, dim_nm, &temp, &(dimNT[j]), &(dimnattr[j]));
                            attr_nt_desc = HDgetNTdesc(dimNT[j]);
                            fprintf(fp, "\t Dim%d: Name=%s\n", (int) j, dim_nm);
                            if (temp == 0)
                              {
                                  fprintf(fp, "\t\t Size = UNLIMITED ");
                                  fprintf(fp, "(currently %d)\n", (int) dimsizes[j]);
                              }
                            else
                                fprintf(fp, "\t\t Size = %d\n", (int) dimsizes[j]);
                            fprintf(fp, "\t\t Type = %s\n", attr_nt_desc);
                            HDfree(attr_nt_desc);
                            fprintf(fp, "\t\t Number of attributes = %d\n", (int) dimnattr[j]);
                        }

                          /* Print annotations */
                      for (j = 0; j < nattr; j++)
                        {
                            ret = SDattrinfo(sds_id, j, attr_name, &attr_nt, &attr_count);
                            attr_nt_desc = HDgetNTdesc(attr_nt);
                            if (ret == FAIL)
                              {
                                  printf("Failure in SDattrinfo %s\n", file_name);
                                  exit(1);
                              }
                            attr_index = SDfindattr(sds_id, attr_name);
                            if (ret == FAIL)
                              {
                                  printf("Failure in SDfindattr %s\n", file_name);
                                  exit(1);
                              }
                            attr_buf_size = DFKNTsize(attr_nt) * attr_count;
                            attr_buf = (VOIDP) HDmalloc(attr_buf_size);
                            if (attr_buf == NULL)
                              {
                                  printf("Not enough memory!\n");
                                  exit(-1);
                              }
                            ret = SDreadattr(sds_id, attr_index, attr_buf);
                            if (ret == FAIL)
                              {
                                  printf("Failure in SDfindattr %s\n", file_name);
                                  exit(1);
                              }
                            fprintf(fp, "\t Attr%d: Name = %s\n", (int) attr_index, attr_name);
                            fprintf(fp, "\t\t Type = %s \n\t\t Count= %d\n", attr_nt_desc, (int) attr_count);
                            HDfree(attr_nt_desc);
                            fprintf(fp, "\t\t Value = ");
                            ret = dumpfull(attr_nt, attr_count, attr_buf, 20, fp);
                            HDfree((VOIDP) attr_buf);
                        }

                      if (dumpsds_opts->contents == DHEADER)
                          break;

                  case DDATA:
                      if (dumpsds_opts->contents == DDATA)
                        {
                                /* print rank, dimsizes, nt, maxmin, cal info */
                        }
                      if (dumpsds_opts->contents != DDATA)
                          fprintf(fp, "\t Data : \n");
                      if (rank > 0 && dimsizes[0] != 0)
                        {
                            intn        count;

                            for (count = 0; count < 16; count++)
                                fprintf(fp, " ");
                            ret = sdsdumpfull(sds_id, rank, dimsizes, nt, 16, fp);
                        }

                      break;
                  }		/* switch  */
                SDendaccess(sds_id);
            }	/* for ndsets  */
          SDend(sdf_id);
          if (sd_chosen!=NULL)
            {
                HDfree(sd_chosen);
                sd_chosen=NULL;
            } /* end if */
          if (dumpsds_opts->dump_to_file)
              fclose(fp);
      }		/* while argc  */
    return (0);
}	/* dsd */

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

static void 
dumpsds_usage(intn argc, 
              char *argv[])
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

static void 
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

static intn 
parse_dumpsds_opts(dump_info_t *dumpsds_opts, 
                   intn *curr_arg, 
                   intn argc, char *argv[])
{

    int32 i;
    int32 numItems;
    char  *tempPtr = NULL;
    char  *ptr = NULL;

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
                      fprintf(stderr,"Not enough memory!\n");
                      return FAIL;
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
                      fprintf(stderr,"Not enough memory!\n");
                      return FAIL;
                  }

                ptr = argv[*curr_arg];
                i = 0;
                while ((tempPtr = HDstrchr(ptr, ',')) != NULL)
                  {
                      *tempPtr = '\0';
                      if ((dumpsds_opts->filter_str[i] = (char *) HDmalloc(sizeof(char) * (HDstrlen(ptr) + 1))) == NULL)
                        {
                            fprintf(stderr,"Not enough memory!\n");
                            return FAIL;
                        }
                      HDstrcpy(dumpsds_opts->filter_str[i], ptr);
                      ptr = tempPtr + 1;
                      i++;
                  }		/* end while */

                    /* Get the last string */
                if ((dumpsds_opts->filter_str[i] = (char *) HDmalloc(sizeof(char) * (HDstrlen(ptr) + 1))) == NULL)
                  {
                      fprintf(stderr,"Not enough memory!\n");
                      return FAIL;
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

static int32 
sdsdumpfull(const char *fname,
            int32       sds_id, 
            file_type_t ft, 
            int32       rank, 
            int32       dimsizes[], 
            int32       nt, 
            intn        indent, 
            FILE       *fp)
{
	/* "rank" is the number of dimensions and 
	   "dimsizes[i]" is size of dimension "i". */
    int32    j, i;
    VOIDP    buf = NULL;
    int32    numtype;
    int32    eltsz;
    int32    read_nelts;
    int32    done;			/* number of rows we have done */
    int32   *left = NULL;
    int32   *start = NULL;
    int32   *edge = NULL;
    intn     count;
    int32    ret_value = SUCCEED;

    if (indent > 65)
      { /* This block is probably not necessary. */
          printf("Bad indentation %d\n", indent);
          ret_value = FAIL;
          goto done;
      }

	/* Compute the number of the bytes for each value. */
    numtype = nt & DFNT_MASK;
    eltsz = DFKNTsize(numtype | DFNT_NATIVE);

    read_nelts = dimsizes[rank - 1];
    buf = (VOIDP) HDmalloc(read_nelts * eltsz);
    if (buf == NULL)
      {
          fprintf(stderr,"Not enough memory!\n");
          ret_value = FAIL;
          goto done;
      }

    left = (int32 *) HDmalloc(rank * sizeof(int32));
    if (left == NULL)
      {
          fprintf(stderr,"Not enough memory!\n");
          ret_value = FAIL;
          goto done;
      }

    start = (int32 *) HDmalloc(rank * sizeof(int32));
    if (start == NULL)
      {
          fprintf(stderr,"Not enough memory!\n");
          ret_value = FAIL;
          goto done;
      }

    edge = (int32 *) HDmalloc(rank * sizeof(int32));
    if (edge == NULL)
      {
          fprintf(stderr,"Not enough memory!\n");
          ret_value = FAIL;
          goto done;
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
          if (FAIL == SDreaddata(sds_id, start, NULL, edge, buf))
            {
                fprintf(stderr,"SDreaddata failed for sds_id(%d) in file %s\n",
                        (int)sds_id, fname);
                ret_value = FAIL;
                goto done;
            }

          if(FAIL == dumpfull(numtype, ft, read_nelts, buf, indent, fp))
            {
                fprintf(stderr,"dumpfull failed for sds_id(%d) in file %s\n",
                        (int)sds_id, fname);
                ret_value = FAIL;
                goto done;
            }

      }
    else if (rank > 1)
      {
          done = 0;
          while (!done)
            {	/* In each iteration, a row in dumped and "left[]" is modified accordingly. */
                if (FAIL == SDreaddata(sds_id, start, NULL, edge, buf))
                  {
                      fprintf(stderr,"SDreaddata failed for sds_id(%d) in file %s\n",
                              (int)sds_id, fname);
                      ret_value = FAIL;
                      goto done;
                  }

                if (FAIL == dumpfull(numtype, ft, read_nelts, buf, indent, fp))
                  {
                      fprintf(stderr,"dumpfull failed for sds_id(%d) in file %s\n",
                              (int)sds_id, fname);
                      ret_value = FAIL;
                      goto done;
                  }

                if(ft == DASCII)
                  {
                      for (count = 0; count < indent; count++)
                          putc(' ', fp);
                  }
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
                            if(ft==DASCII)
                              {
                                  if (j == rank - 2)
                                    {
                                        fprintf(fp, "\n");
                                        for (count = 0; count < indent; count++)
                                            putc(' ', fp);
                                    }
                              }
                        }
                  }		/* for j */
            }	/* while   */
      }		/* else */

    if (ft == DASCII)
        fprintf(fp, "\n");

done:
    if (ret_value == FAIL)
      { /* Failure cleanup */
      }
    /* Normal cleanup */
    if (edge != NULL)
        HDfree((VOIDP) edge);
    if (start != NULL)
        HDfree((VOIDP) start);
    if (left != NULL)
        HDfree((VOIDP) left);
    if (buf != NULL)
        HDfree((VOIDP) buf);    

    return ret_value;
}	/* sdsdumpfull */

static intn 
dsd(dump_info_t *dumpsds_opts, 
    intn         curr_arg, 
    intn         argc, 
    char        *argv[])
{
    intn        i;
    intn        ret;
    intn        isdimvar;
    int32       sdf_id = FAIL;
    int32       sds_id = FAIL;
    int32      *sd_chosen = NULL;
    int32       num_sd_chosen;
    int32       rank;
    int32       nt;
    int32       nattr;
    int32       ndsets;
    int32       nglb_attr;
    intn        j, k;
    int32       attr_nt;
    int32       attr_count;
    int32       attr_buf_size;
    int32       attr_index;
    char        file_name[MAXFNLEN];
    char        name[MAXNAMELEN];
    char        attr_name[MAXNAMELEN];
    char        dim_nm[MAXNAMELEN];
    int32       dimsizes[MAXRANK];
    int32       dim_id = FAIL;
    int32       dimNT[MAXRANK];
    int32       dimnattr[MAXRANK];
    FILE       *fp = NULL;
    int32       index;
    VOIDP       attr_buf = NULL;
    char       *nt_desc = NULL;
    char       *attr_nt_desc = NULL;
    int         index_error = 0;
    int         x;
    int         dumpall = 0;
    int32       sd_ref;
    file_type_t ft;
    intn        ret_value = SUCCEED;

    while (curr_arg < argc)
      {		/* Examine each file. */
          HDstrcpy(file_name, argv[curr_arg]);
          curr_arg++;

          sdf_id = SDstart(file_name, DFACC_RDONLY);
          if (sdf_id == FAIL)
            {
                fprintf(stderr,"Failure in open %s\n", file_name);
                ret_value = FAIL;
                goto done;
            }

          num_sd_chosen = dumpsds_opts->num_chosen;
          if (num_sd_chosen > 0)
            {
                if ((sd_chosen = (int32 *) HDmalloc(sizeof(int32) * num_sd_chosen)) == NULL)
                  {
                      fprintf(stderr,"Memory allocation error\n");
                      ret_value = FAIL;
                      goto done;
                  }		/* end if */

                k = (-1);
                HDmemfill(sd_chosen, &k, sizeof(int32), num_sd_chosen);
            }	/* end if */

          k = 0;
          switch (dumpsds_opts->filter)
            {	/* Determine the SDs having been selected. */
            case DINDEX:
                /* Note: Don't replace this with HDmemcpy 
                   unless you change the sizes of the 
                   objects correctly -QAK */
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
                      if (index == FAIL)
                        {
                            printf("SD with reference number %d: not found\n", dumpsds_opts->filter_num[i]);
                            index_error = 1; /* error */
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
                      if (index == FAIL)
                        {
                            printf("SD with name %s: not found\n", dumpsds_opts->filter_str[i]);
                            index_error = 1; /* error */
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
                ret_value = FAIL;
                goto done;

            case DALL:
                k= -1;
                break;

            default:
                printf("Unknown filter option\n");
                ret_value = FAIL;
                goto done;
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

          ft = dumpsds_opts->file_type;

          /* ASCII or binary dump? */
          switch(ft)
            {
            case DASCII:       /* ASCII file */

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
                      fprintf(stderr,"Failure in SDfileinfo for file %s\n", file_name);
                      ret_value = FAIL;
                      goto done;
                  }

                x = 0;
                if (num_sd_chosen == (-1))
                    dumpall = 1;
                else
                    sort(sd_chosen, num_sd_chosen);

                for (i = 0; 
                     i < ndsets 
                         && (dumpall!=0 || x<dumpsds_opts->num_chosen); 
                     i++)
                  {	/* Examine each SD. */

                      if ((!dumpall) && (i != sd_chosen[x]))
                          continue; /* skip */
                      x++;

                      /* Reset variables. */
                      HDmemset(dimsizes, 0, sizeof(int32) * MAXRANK);
                      HDmemset(dimNT, 0, sizeof(int32) * MAXRANK);
                      HDmemset(dimnattr, 0, sizeof(int32) * MAXRANK);

                      sds_id = SDselect(sdf_id, i);
                      if (sds_id == FAIL)
                        {
                            fprintf(stderr,"Failure in selecting %d'th SDS in file %s\n",
                                    i, file_name);
                            ret_value = FAIL;
                            goto done;
                        }

                      if ((sd_ref = SDidtoref(sds_id)) == FAIL)
                        {
                            fprintf(stderr,"Failure in determining reference for sds_id(%d) in file %s.\n",
                                    (int)sds_id, file_name);
                            ret_value = FAIL;
                            goto done;
                        }

                      isdimvar = (SDiscoordvar(sds_id)) ? 1 : 0;

                      if (FAIL == SDgetinfo(sds_id, name, &rank, dimsizes, &nt, &nattr))
                        {
                            fprintf(stderr,"SDgetinfo failed for sds_id(%d) in file %s\n", 
                                    (int)sds_id, file_name);
                            ret_value = FAIL;
                            goto done;
                        }

                      /* dump contents based on options */
                      switch (dumpsds_opts->contents)
                        {
                        case DVERBOSE:
                        case DHEADER:
                            nt_desc = HDgetNTdesc(nt);
                            if (nt_desc == NULL)
                              {
                                  fprintf(stderr,"Failure in HDgetNTdesc for sds_id(%d) in file %s\n", 
                                          (int)sds_id,file_name);
                                  ret_value = FAIL;
                                  goto done;
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
                            nt_desc = NULL;

                            fprintf(fp, "\t Ref. = %d\n", (int) sd_ref);
                            fprintf(fp, "\t Rank = %d\n\t Number of attributes = %d\n", (int) rank, (int) nattr);

                            /* for each dimension */
                            for (j = 0; j < rank; j++)
                              {
                                  int32       temp;

                                  if (FAIL == (dim_id = SDgetdimid(sds_id, j)))
                                    {
                                        fprintf(stderr,"SDgetdimid failed for sds_id(%d) in file %s\n", 
                                                (int)sds_id,file_name);
                                        ret_value = FAIL;
                                        goto done;
                                    }

                                  if (FAIL == SDdiminfo(dim_id, dim_nm, &temp, &(dimNT[j]), &(dimnattr[j])))
                                    {
                                        fprintf(stderr,"SDdiminfo failed for dim_id(%d) of sds_id(%d) in file %s\n", 
                                                (int)dim_id,(int)sds_id,file_name);
                                        ret_value = FAIL;
                                        goto done;
                                    }

                                  attr_nt_desc = HDgetNTdesc(dimNT[j]);
                                  if (attr_nt_desc == NULL)
                                    {
                                        fprintf(stderr,"HDgetNTdesc failed for dim_id(%d) of sds_id(%d) in file %s\n", 
                                                (int)dim_id,(int)sds_id,file_name);
                                        ret_value = FAIL;
                                        goto done;
                                    }

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
                                  attr_nt_desc = NULL; /* reset */

                                  fprintf(fp, "\t\t Number of attributes = %d\n", (int) dimnattr[j]);
                              } /* end for dimension */

                            /* Print annotations */
                            for (j = 0; j < nattr; j++)
                              {
                                  ret = SDattrinfo(sds_id, j, attr_name, &attr_nt, &attr_count);
                                  if (ret == FAIL)
                                    {
                                        fprintf(stderr,"SDattrinfo failed for %d'th attribute of sds_id(%d) in file %s\n", 
                                                j,(int)sds_id, file_name);
                                        ret_value = FAIL;
                                        goto done;
                                    }

                                  attr_nt_desc = HDgetNTdesc(attr_nt);
                                  if (attr_nt_desc == NULL)
                                    {
                                        fprintf(stderr,"HDgetNTdesc failed for %d'th attribute of sds_id(%d) in file %s\n", 
                                                j,(int)sds_id, file_name);
                                        ret_value = FAIL;
                                        goto done;
                                    }

                                  attr_index = SDfindattr(sds_id, attr_name);
                                  if (attr_index == FAIL)
                                    {
                                        fprintf(stderr,"SDfindattr failed for %d'th attribute of sds_id(%d) in file %s\n", 
                                                j,(int)sds_id, file_name);
                                        ret_value = FAIL;
                                        goto done;
                                    }

                                  attr_buf_size = DFKNTsize(attr_nt) * attr_count;

                                  attr_buf = (VOIDP) HDmalloc(attr_buf_size);
                                  if (attr_buf == NULL)
                                    {
                                        fprintf(stderr,"Not enough memory!\n");
                                        ret_value = FAIL;
                                        goto done;
                                    }

                                  ret = SDreadattr(sds_id, attr_index, attr_buf);
                                  if (ret == FAIL)
                                    {
                                        fprintf(stderr,"SDreadattr failed for %d'th attribute ofr sds_id(%d) in file %s\n", 
                                                j,(int)sds_id, file_name);
                                        ret_value = FAIL;
                                        goto done;
                                    }

                                  fprintf(fp, "\t Attr%d: Name = %s\n", (int) attr_index, attr_name);
                                  fprintf(fp, "\t\t Type = %s \n\t\t Count= %d\n", attr_nt_desc, (int) attr_count);

                                  HDfree(attr_nt_desc);
                                  attr_nt_desc = NULL; /* reset */

                                  fprintf(fp, "\t\t Value = ");

                                  if (FAIL == dumpfull(attr_nt, ft, attr_count, attr_buf, 20, fp))
                                    {
                                        fprintf(stderr,"dumpfull failed for %d'th attribute of sds_id(%d) in file %s\n", 
                                                j, (int)sds_id, file_name);
                                        ret_value = FAIL;
                                        goto done;
                                    }

                                  HDfree((VOIDP) attr_buf);
                                  attr_buf = NULL; /* reset */
                              } /* for each attribute */

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
                                  if (FAIL == sdsdumpfull(file_name, sds_id, ft, rank, dimsizes, nt, 16, fp))
                                    {
                                        fprintf(stderr,"sdsdumpfull failed for sds_id(%d) in file %s\n", 
                                                (int)sds_id, file_name);
                                        ret_value = FAIL;
                                        goto done;
                                    }
                              }

                            break;
                        default:
                            printf("dumping SDS, unknown option \n");
                            ret_value = FAIL;
                            goto done;
                        }		/* switch  */

                      if (FAIL == SDendaccess(sds_id))
                        {
                            fprintf(stderr,"SDendaccess failed for sds_id(%d) in file %s\n", 
                                    (int)sds_id, file_name);
                            ret_value = FAIL;
                            goto done;
                        }

                      sds_id = FAIL;/* reset */

                  }	/* for ndsets  */
                break;

            case DBINARY:       /* binary file */
                /* get output file name  */
                if (dumpsds_opts->dump_to_file)
                    fp = fopen(dumpsds_opts->file_name, "w");
                else
                    fp = stdout;

                ret = SDfileinfo(sdf_id, &ndsets, &nglb_attr);
                if (ret == FAIL)
                  {
                      fprintf(stderr,"SDfileinfo failed for file %s\n", file_name);
                      ret_value = FAIL;
                      goto done;
                  }

                x = 0;
                if (num_sd_chosen == (-1))
                    dumpall = 1;
                else
                    sort(sd_chosen, num_sd_chosen);

                for (i = 0; 
                     i < ndsets 
                         && (dumpall!=0 || x<dumpsds_opts->num_chosen);
                     i++)
                  {   /* Examine each SD. */
                      if ((!dumpall) && (i != sd_chosen[x]))
                          continue; /* skip */

                      x++;

                      /* Reset variables. */
                      HDmemset(dimsizes, 0, sizeof(int32) * MAXRANK);
                      HDmemset(dimNT, 0, sizeof(int32) * MAXRANK);
                      HDmemset(dimnattr, 0, sizeof(int32) * MAXRANK);

                      sds_id = SDselect(sdf_id, i);
                      if (sds_id == FAIL)
                        {
                            fprintf(stderr,"Failure in selecting %d'th SDS in file %s\n",
                                    i, file_name);
                            ret_value = FAIL;
                            goto done;
                        }

                      if ((sd_ref = SDidtoref(sds_id)) == FAIL)
                        {
                            fprintf(stderr, "Failure in determining reference no.of sds_id(%d) in file %s\n", 
                                    (int)sds_id, file_name);
                            ret_value = FAIL;
                            goto done;
                        }

                      isdimvar = (SDiscoordvar(sds_id)) ? 1 : 0;

                      if (FAIL == SDgetinfo(sds_id, name, &rank, dimsizes, &nt, &nattr))
                        {
                            fprintf(stderr, "SDgetinfo failed for sds_id(%d) in file %s\n", 
                                    (int)sds_id, file_name);
                            ret_value = FAIL;
                            goto done;
                        }
                         
                      /*   output data to binary file   */
                      if (rank > 0 && dimsizes[0] != 0)
                        {
                            if (FAIL == sdsdumpfull(file_name,sds_id, ft, rank, dimsizes, nt, 16, fp))
                              {
                                  fprintf(stderr, "sdsdumpfull failed for sds_id(%d) in file %s\n", 
                                          (int)sds_id, file_name);
                                  ret_value = FAIL;
                                  goto done;
                              }
                        }

                      if (FAIL == SDendaccess(sds_id))
                        {
                            fprintf(stderr, "SDendaccess failed for sds_id(%d) in file %s\n", 
                                    (int)sds_id, file_name);
                            ret_value = FAIL;
                            goto done;
                        }

                      sds_id = FAIL; /* reset */
                  }  /* for all ndsets in file */
                break;

            default:
                printf("unknown ouput file option \n");
                ret_value = FAIL;
                goto done;
            } /* switch for output file */

          if (dumpsds_opts->dump_to_file)
              fclose(fp);                       

          if (FAIL == SDend(sdf_id))
            {
                fprintf(stderr, "SDend failed for file %s\n", 
                        file_name);
                ret_value = FAIL;
                goto done;
            }

          sdf_id = FAIL; /* reset */

          if (sd_chosen != NULL)
            {
                HDfree(sd_chosen);
                sd_chosen = NULL;
            } /* end if */

      }		/* while processing files  */

done:
    if (ret_value == FAIL)
      { /* Failure cleanup */
          if (sds_id != FAIL)
              SDendaccess(sds_id);

          if (sdf_id != FAIL)
              SDend(sdf_id);

          if (attr_nt_desc != NULL)
              HDfree(attr_nt_desc);

          if (attr_buf != NULL)
              HDfree(attr_buf);

          if (sd_chosen != NULL)
            {
                HDfree(sd_chosen);
                sd_chosen=NULL;
            } /* end if */

      }
    /* Normal cleanup */

    return ret_value;
}	/* dsd */

/* Exported */
intn
do_dumpsds(intn        curr_arg, 
           intn        argc, 
           char       *argv[], 
           dump_opt_t *glob_opts)
{
    dump_info_t dumpsds_opts;	/* dumpsds options */
    intn        ret_value = SUCCEED;

    if (glob_opts->help == TRUE)
      {
          dumpsds_usage(argc, argv);
          goto done;
      }		/* end if */

    init_dumpsds_opts(&dumpsds_opts);

    if (parse_dumpsds_opts(&dumpsds_opts, &curr_arg, argc, argv) == FAIL)
      {
          printf("Failure in parsing options to dump SDS data \n");
          dumpsds_usage(argc, argv);
          ret_value = FAIL;
          goto done;
      }		/* end if */

    if (dsd(&dumpsds_opts, curr_arg, argc, argv) == FAIL)
      {
          fprintf(stderr,"Failure in dumping SDS data\n");
          ret_value = FAIL;
          goto done;
      }

  done:
    if (ret_value == FAIL)
      { /* Failure cleanup */
      }
    /* Normal cleanup */
	if (dumpsds_opts.filter_num != NULL )
        HDfree( dumpsds_opts.filter_num);

    if(dumpsds_opts.filter_str != NULL)
        HDfree(dumpsds_opts.filter_str);

    return ret_value;
}	/* end do_dumpsds() */

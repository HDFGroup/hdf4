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

#include "hdp.h"
#ifndef MIPSEL
#include <math.h>
#endif /* MIPSEL */

static intn dvd(dump_info_t * dumpvd_opts, intn curr_arg, intn argc,
				char *argv[], char *flds_chosen[MAXCHOICES],
				int dumpallfields);

int32       dumpvd(int32 vd_id, file_type_t ft, int data_only, FILE * fp, char sep[2],
				   int32 flds_indices[MAXCHOICES], int dumpallfields);

int32       VSref_index(int32 file_id, int32 vd_ref);

int32       VSstr_index(int32 file_id, char *filter_str,
						int name, int32 *find_ref, int32 *index);

void 
dumpvd_usage(intn argc, char *argv[])
{
	printf("Usage:\n");
	printf("%s dumpvd [-a|-i <index>|-n <name>|-r <ref>] [-dhv] [-o <filename> [-bx]] <filelist>\n", argv[0]);
	printf("\t-a\tDump all VDs in the file (default)\n");
	printf("\t-i <index>\tDump the <index>th SDS in the file \n");
	printf("\t-n <name>\tDump the VDS with name <name>\n");
	printf("\t-r <ref>\tDump the VDS with reference number <ref>\n");
	printf("\t-d\tDump data only, no tag/ref, formatted to input to hp2hdf\n");
	printf("\t-h\tDump header only, no annotation for elements nor data\n");
	printf("\t-v\tDump everything including all annotations (default)\n");
	printf("\t-o <filename>\tOutput to file <filename>\n");
	printf("\t-b\tBinary format of output\n");
	printf("\t-x\tAscii text format of output (default)\n");
}	/* end list_usage() */

void 
init_dumpvd_opts(dump_info_t * dumpvd_opts)
{
	dumpvd_opts->filter = DALL;		   /* default dump all Vdatas */
	dumpvd_opts->filter_num=NULL;      /* not by reference nor by index */
	dumpvd_opts->filter_str = NULL;	   /* no strings */
	dumpvd_opts->num_chosen = (-1);	   /* default dump all items */
	dumpvd_opts->contents = DVERBOSE;  /* default dump all information */
	dumpvd_opts->dump_to_file = FALSE; /* don't dump to output file */
	dumpvd_opts->file_type = DASCII;   /* default output is ASCII file */
	HDstrcpy(dumpvd_opts->file_name, "\0");
}	/* end init_list_opts() */

intn 
parse_dumpvd_opts(dump_info_t * dumpvd_opts, intn *curr_arg, intn argc,
                  char *argv[], char *flds_chosen[MAXCHOICES],
                  int *dumpallfields)
{
    int32       i, lastItem, numItems;
    char       *tempPtr, *ptr;

    if (*curr_arg >= argc)
        return (FAIL);
    while ((*curr_arg < argc) && (argv[*curr_arg][0] == '-'))
      {
          switch (argv[*curr_arg][1])
            {
            case 'a':	/* dump all, default */
                dumpvd_opts->filter = DALL;
                dumpvd_opts->num_chosen = -1;
                (*curr_arg)++;
                break;

            case 'i':	/* dump by index */
            case 'r':	/* dump by reference */
                if ((argv[*curr_arg][1]) == 'i')
                    dumpvd_opts->filter = DINDEX;
                else
                    dumpvd_opts->filter = DREFNUM;
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

                dumpvd_opts->filter_num = (int *) HDmalloc(sizeof(intn) * numItems);
                if (dumpvd_opts->filter_num == NULL)
                  {
                      printf("Not enough memory!\n");
                      exit(-1);
                  }
                ptr = argv[*curr_arg];
                i = 0;
                while ((tempPtr = HDstrchr(ptr, ',')) != NULL)
                  {
                      *tempPtr = '\0';
                      dumpvd_opts->filter_num[i] = atoi(ptr);
                      ptr = tempPtr + 1;
                      i++;
                  }
                dumpvd_opts->filter_num[i] = atoi(ptr);	/* get the last item */
                dumpvd_opts->num_chosen = numItems;	/* save the number of items */

                (*curr_arg)++;
                break;

            case 'n':	/* dump by names */
            case 'c':	/* dump by class */
                if ((argv[*curr_arg][1]) == 'n')
                    dumpvd_opts->filter = DNAME;
                else
                    dumpvd_opts->filter = DCLASS;
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
                if ((dumpvd_opts->filter_str = (char **) HDmalloc(sizeof(char *) * (numItems+1))) == NULL)
                  {
                      printf("Not enough memory!\n");
                      exit(-1);
                  }
                dumpvd_opts->filter_str[numItems] = NULL;

                ptr = argv[*curr_arg];
                i = 0;
                while ((tempPtr = HDstrchr(ptr, ',')) != NULL)
                  {
                      *tempPtr = '\0';
                      if ((dumpvd_opts->filter_str[i] = (char *) HDmalloc(sizeof(char) * (HDstrlen(ptr) + 1))) == NULL)
                        {
                            printf("Not enough memory!\n");
                            exit(-1);
                        }
                      HDstrcpy(dumpvd_opts->filter_str[i], ptr);
                      ptr = tempPtr + 1;
                      i++;
                  }		/* end while */

                    /* Get the last string */
                if ((dumpvd_opts->filter_str[i] = (char *) HDmalloc(sizeof(char) * (HDstrlen(ptr) + 1))) == NULL)
                  {
                      printf("Not enough memory!\n");
                      exit(-1);
                  }
                HDstrcpy(dumpvd_opts->filter_str[i], ptr);
                dumpvd_opts->num_chosen = numItems;	/* save the number of items */

                (*curr_arg)++;
                break;

            case 'f':	/* dump a subset of the fields */
                if (!HDstrcmp(argv[1], "dumpvd"))
                  {
                      if (dumpvd_opts->filter == DALL)
                          dumpvd_opts->filter = DFIELDS;
                      *dumpallfields = 0;
                      (*curr_arg)++;

                      lastItem = 0;
                      ptr = argv[*curr_arg];
                      for (i = 0; !lastItem; i++)
                        {
                            tempPtr = HDstrchr(ptr, ',');
                            if (tempPtr == NULL)
                                lastItem = 1;
                            else
                                *tempPtr = '\0';
                            flds_chosen[i] = (char *) HDmalloc(sizeof(char) * (HDstrlen(ptr)+1));
                            if (flds_chosen[i] == NULL)
                              {
                                  printf("Not enough memory!\n");
                                  exit(-1);
                              }
                            HDstrcpy(flds_chosen[i], ptr);
                            ptr = tempPtr + 1;
                        }
                      flds_chosen[i] = NULL;
                      (*curr_arg)++;
                  }
                else
                  {
                      printf("Warning: Invalid %s option %s\n", argv[1], argv[*curr_arg]);
                      return (FAIL);
                  }
                break;

            case 'd':	/* dump data only */
                dumpvd_opts->contents = DDATA;
                (*curr_arg)++;
                break;

            case 'h':	/* no annotations nor data */
                dumpvd_opts->contents = DHEADER;
                (*curr_arg)++;
                break;

            case 'v':	/* dump all info */
                dumpvd_opts->contents = DVERBOSE;
                (*curr_arg)++;
                break;

            case 'o':	/* specify output file */
                dumpvd_opts->dump_to_file = TRUE;	/* get filename */
                HDstrcpy(dumpvd_opts->file_name, argv[++(*curr_arg)]);
                if (++(*curr_arg) < argc)
                  {		/* binary or ascii */
                      if (argv[*curr_arg][0] == '-')
                          dumpvd_opts->file_type = (argv[*curr_arg][1] == 'b') ?  DBINARY : DASCII;
                      else
                          (*curr_arg)--;
                  }
                (*curr_arg)++;
                break;

            default:	/* invalid dumpvd option */
                printf("Warning: Invalid dumpvd option %s\n", argv[*curr_arg]);
                return (FAIL);
            }	/* end switch */
      }		/* end while */
    return (SUCCEED);
}	/* end parse_dumpvd_opts */

void 
do_dumpvd(intn curr_arg, intn argc, char *argv[], dump_opt_t * glob_opts)
{
    dump_info_t dumpvd_opts;	/* dumpvd options */
    char       *flds_chosen[MAXCHOICES];
    int         dumpallfields;

    flds_chosen[0] = NULL;
    dumpallfields = 1;
    if (glob_opts->help == TRUE)
      {
          dumpvd_usage(argc, argv);
          return;
      }		/* end if */

    init_dumpvd_opts(&dumpvd_opts);
    if (parse_dumpvd_opts(&dumpvd_opts, &curr_arg, argc, argv, flds_chosen, &dumpallfields) == FAIL)
      {
          dumpvd_usage(argc, argv);
          return;
      }		/* end if */

    if (dvd(&dumpvd_opts, curr_arg, argc, argv, flds_chosen, dumpallfields) == FAIL)
      {
          printf("Failure in dumping Vdata data\n");
          exit(1);
      } 

	if(dumpvd_opts.filter_num != NULL)
	  HDfree(dumpvd_opts.filter_num);

}	/* end do_dumpvd() */

void 
choose_vd(dump_info_t * dumpvd_opts, int32 **vd_chosen,
          int32 *num_vd_chosen, int32 file_id, int *index_error)
{
    int32       i;
    int32 k = 0;
    int32 index, find_ref, number;

    switch (dumpvd_opts->filter)
      {		/* Determine which VDs have been chosen. */
      case DINDEX:
          for (i = 0; i<dumpvd_opts->num_chosen; i++)
            {
                (*vd_chosen)[i] = dumpvd_opts->filter_num[i];
                k++;
            }
          break;
          
      case DREFNUM:
          for (i = 0; i<dumpvd_opts->num_chosen; i++)
            {
                index = VSref_index(file_id, dumpvd_opts->filter_num[i]);
                if (index == -1)
                  {
                      printf("Vdata with reference number %d: not found\n", dumpvd_opts->filter_num[i]);
                      *index_error = 1;
                  }
                else
                  {
                      (*vd_chosen)[k] = index;
                      k++;
                  }
            }	/* for */
          break;

      case DNAME:
          for (i = 0; i<dumpvd_opts->num_chosen; i++)
            {
                find_ref = -1;
                number = 0;
                index = VSstr_index(file_id, dumpvd_opts->filter_str[i], 1, &find_ref, &number);
                if (index == -1)
                  {
                      printf("Vdata with name %s: not found\n", dumpvd_opts->filter_str[i]);
                      *index_error = 1;
                  }
                else
                  {
                      (*vd_chosen)[k] = index;
                      k++;
                  }
            }	/* for */
          break;

      case DCLASS:
          for (i = 0; dumpvd_opts->filter_str[i] != NULL; i++)
            {
                int32       found = 0;

                find_ref = -1;
                number = 0;
                while ((index = VSstr_index(file_id, dumpvd_opts->filter_str[i], 0,
                                            &find_ref, &number)) != -1)
                  {
                      if (k < (int32)(*num_vd_chosen))
                          (*vd_chosen)[k] = index;
                      else
                        {
                            if ((*vd_chosen = (int32 *) HDrealloc(*vd_chosen,sizeof(int32) * (*num_vd_chosen+1))) == NULL)
                              {
                                  printf("Memory re-allocation error\n");
                                  exit(1);
                              }		/* end if */
                            (*vd_chosen)[k] = index;
                            (*num_vd_chosen)++;
                        }
                      k++;
                      found = 1;
                  }
                if (!found)
                  {
                      printf("Vdata with class %s: not found\n", dumpvd_opts->filter_str[i]);
                      *index_error = 1;
                  }
            }	/* for */
          break;

      case DFIELDS:	/* Don't have to worry about which chosen fields yet. */
          break;

      case DALL:
          k= -1;
          break;

      default:
          printf("Unknown filter option\n");
          exit(1);
      }		/* switch */
    *num_vd_chosen=k;
}	/* choose_vd */

static intn 
dvd(dump_info_t * dumpvd_opts, intn curr_arg,
    intn argc, char *argv[], char *flds_chosen[MAXCHOICES], int dumpallfields)
{
    int32       file_id, vd_id, *vd_chosen=NULL, flds_indices[MAXCHOICES];
    int32       num_vd_chosen;
    int32       i, j, k, m, x;
    int32       nvf, interlace, vsize, len, lastItem;
    int32       vdata_ref = -1, vdata_tag, status;
    char       *label_str;
    char        vdclass[VSNAMELENMAX], vdname[VSNAMELENMAX];
    char        fields[FIELDNAMELENMAX], tempflds[FIELDNAMELENMAX];
    char       *tempPtr, *ptr, string[MAXNAMELEN];
    char        file_name[MAXFNLEN];
    FILE       *fp=NULL;
    intn         index_error = 0, dumpall = 0, ret;
    file_type_t  ft;

    while (curr_arg < argc)
      {		/* Loop until all specified files have been 
                   processed */
          HDstrcpy(file_name, argv[curr_arg]);
          curr_arg++;
          file_id = Hopen(file_name, DFACC_READ, 0);
          if (file_id == FAIL)
            {
                printf("Failure in open %s\n", file_name);
                exit(1);
            }

          num_vd_chosen = dumpvd_opts->num_chosen;
          if (num_vd_chosen > 0)
            {
                if ((vd_chosen = (int32 *) HDmalloc(sizeof(int32) * num_vd_chosen)) == NULL)
                  {
                      printf("Memory allocation error\n");
                      exit(1);
                  }		/* end if */

                k = (-1);
                HDmemfill(vd_chosen, &k, sizeof(int32), num_vd_chosen);
            }	/* end if */

          Vstart(file_id);

              /* Find out which VDs have been chosen. */
          choose_vd(dumpvd_opts, &vd_chosen, &num_vd_chosen, file_id, &index_error);

              /* In case of index error, skip the current file. */
          if (index_error && num_vd_chosen==0)
            {
                if(vd_chosen!=NULL)
                  {
                      HDfree(vd_chosen);
                      vd_chosen=NULL;
                  } /* end if */
                Vend(file_id);
                Hclose(file_id);
                continue;
            } /* end if */

          ft=dumpvd_opts->file_type;
          switch(ft)
           {
           case DASCII:  /*    ASCII file   */

              /* Get output file name.  */
          if (dumpvd_opts->dump_to_file)
              fp = fopen(dumpvd_opts->file_name, "w");
          else
              fp = stdout;

          if (dumpvd_opts->contents != DDATA)
              fprintf(fp, "File name: %s \n\n", file_name);
          x = 0;	/* "x" is used to index the array of "vd_chosen". */

              /* Determine if all VDs are to be dumped out. */
          if (num_vd_chosen<=0)	/* If so, set the corresponding flag. */
              dumpall = 1;
          else
                  /* Otherwise, sort the indices of the chosen VDs in increasing 
                     order so that they will be dumped out in such order. */
              sort(vd_chosen,num_vd_chosen);

              /* Examine each VD. */
          for (i = 0; (vdata_ref = VSgetid(file_id, vdata_ref)) != -1 && (dumpall!=0 || x<dumpvd_opts->num_chosen); i++)
            {
                int         data_only, flds_match = 0;
                char        sep[2];

                    /* Only dump the info of the chosen VDs or all of the VDs if none
                       has been selected. */
                if ((!dumpall) && (i != vd_chosen[x]))
                    continue;

                x++;	/* One vdata has been located; so go to the next one in 
                           the array. */
                vd_id = VSattach(file_id, vdata_ref, "r");
                if (vd_id == FAIL)
                  {
                      printf("Cannot open vd id=%d\n", (int) vdata_ref);
                      continue;
                  }

                VSinquire(vd_id, &nvf, &interlace, fields, &vsize, vdname);
                vdata_tag = VSQuerytag(vd_id);
                VSgetclass(vd_id, vdclass);

                    /* If one or more fields were specified by the user, then find out
                       what they were, determine their corresponding indices in 
                       "fields", and store these indices in the array "flds_indices" so
                       that they can be used to determine whether a field should be
                       dumped later on. */
                if (flds_chosen[0] != NULL)
                  {
                      HDstrcpy(tempflds, fields);
                      ptr = tempflds;

                      j=(-1);
                      HDmemfill(flds_indices, &j, sizeof(int32), MAXCHOICES);

                      m = 0;
                      lastItem = 0;
                          /* Extract each field name from the list of fields of the 
                             current record. */
                      for (j = 0; !lastItem; j++)
                        {
                            tempPtr = HDstrchr(ptr, ',');
                            if (tempPtr == NULL)
                                lastItem = 1;
                            else
                                *tempPtr = '\0';
                            HDstrcpy(string, ptr);
                            ptr = tempPtr + 1;
                                /* Compare the extracted field name with each of the names
                                   of the fields having been chosen. */
                            for (k = 0; flds_chosen[k] != NULL; k++)
                              {
                                  if (!HDstrcmp(flds_chosen[k], string))
                                    {
                                        flds_indices[m] = j;
                                        m++;
                                        flds_match = 1;
                                    }
                              }		/* for (k...) */
                        }	/* for (j...) */
                  }		/* if  */

                    /* If no fields were chosen, all fields are to be dumped out, and
                       so all fileds match. */
                if (flds_chosen[0] == NULL)
                    flds_match = 1;
                if (flds_match)
                  {
                      int32       z, lastItem, count = 0;
                      switch (dumpvd_opts->contents)
                        {
                        case DVERBOSE:		/* dump all information */
                            fprintf(fp, "Vdata: %d\n", (int) i);
                            fprintf(fp, "   tag = %d; ", (int) vdata_tag);
                            fprintf(fp, "reference = %d;\n", (int) vdata_ref);
                            fprintf(fp, "   number of records = %d; ", (int) nvf);
                            fprintf(fp, "interlace = %d;\n", (int) interlace);
                            fprintf(fp, "   fields = [");

                                /* The list of field names can be very long and would 
                                   look very messy when being displayed if it were to
                                   be dumped out at once. The following block of 
                                   operations is to display a list in a nice way even
                                   if the list is long. */
                            lastItem = 0;
                            HDstrcpy(tempflds, fields);
                            ptr = tempflds;
                            for (z = 0; !lastItem; z++)
                              {
                                  tempPtr = HDstrchr(ptr, ',');
                                  if (tempPtr == NULL)
                                      lastItem = 1;
                                  else
                                      *tempPtr = '\0';
                                  HDstrcpy(string, ptr);
                                  count += HDstrlen(string);
                                  if (count > 50)
                                    {
                                        fprintf(fp, "\n               ");
                                        count = 0;
                                    }
                                  fprintf(fp, "%s", string);
                                  if (!lastItem)
                                      fprintf(fp, ", ");
                                  ptr = tempPtr + 1;
                              }
                            fprintf(fp, "];\n");

                            fprintf(fp, "   record size (in bytes) = %d;\n", (int)vsize);
                            fprintf(fp, "   name = %s; class = %s;\n", vdname, vdclass);
                            ret = dumpattr(vd_id, (int32)_HDF_VDATA, 1, ft, fp);     
                                /* read in all of the annotations */
                            len = DFANgetdesclen(file_name, vdata_tag, vdata_ref);
                            if (len != FAIL)
                              {
                                  label_str = (char *) HDmalloc((uint32) len + 1);
                                  if (label_str == NULL)
                                    {
                                        printf("Not enough memory!\n");
                                        exit(-1);
                                    }
                                  status = DFANgetdesc(file_name, vdata_tag, vdata_ref, label_str, len + 1);
                                  label_str[len] = '\0';
                                  if (status == FAIL)
                                      printf("\t  Unable to read description.\n");
                                  else
                                      printf("\t  Description: %s\n", label_str);
                                  HDfree(label_str);
                              }		/* if (len != FAIL) */

                        case DHEADER:	/* header only, no annotations nor data */
                            if (dumpvd_opts->contents == DHEADER)
                                break;

                        case DDATA:	/* data only */
                            if (dumpvd_opts->contents == DDATA)
                              {
                                  data_only = 1;
                                  HDstrcpy(sep, "");
                              }
                            else
                              {
                                  data_only = 0;
                                  HDstrcpy(sep, ";");
                              }
                                /* Only the chosen or all fields will be dumped out. */
                            dumpvd(vd_id, ft, data_only, fp, sep, flds_indices, dumpallfields);
                        }	/* switch */
                  }
                VSdetach(vd_id);
            }	/* for */
            break;
            case DBINARY:   /*  binary file, not fully tested yet  */
 
             /* Get output file name.  */
          if (dumpvd_opts->dump_to_file)
              fp = fopen(dumpvd_opts->file_name, "w");
          else
              fp = stdout;

          
          x = 0;	/* "x" is used to index the array of "vd_chosen". */

              /* Determine if all VDs are to be dumped out. */
          if (num_vd_chosen<=0)	/* If so, set the corresponding flag. */
              dumpall = 1;
          else
                  /* Otherwise, sort the indices of the chosen VDs in increasing 
                     order so that they will be dumped out in such order. */
              sort(vd_chosen,num_vd_chosen);

              /* Examine each VD. */
          for (i = 0; (vdata_ref = VSgetid(file_id, vdata_ref)) != -1 && (dumpall!=0 || x<dumpvd_opts->num_chosen); i++)
            {
                int         data_only, flds_match = 0;
                char        sep[2];

                    /* Only dump the info of the chosen VDs or all of the VDs if none
                       has been selected. */
                if ((!dumpall) && (i != vd_chosen[x]))
                    continue;

                x++;	/* One vdata has been located; so go to the next one in 
                           the array. */
                vd_id = VSattach(file_id, vdata_ref, "r");
                if (vd_id == FAIL)
                  {
                      printf("Cannot open vd id=%d\n", (int) vdata_ref);
                      continue;
                  }

                VSinquire(vd_id, &nvf, &interlace, fields, &vsize, vdname);
                vdata_tag = VSQuerytag(vd_id);
                VSgetclass(vd_id, vdclass);

                    /* If one or more fields were specified by the user, then find out
                       what they were, determine their corresponding indices in 
                       "fields", and store these indices in the array "flds_indices" so
                       that they can be used to determine whether a field should be
                       dumped later on. */
                if (flds_chosen[0] != NULL)
                  {
                      HDstrcpy(tempflds, fields);
                      ptr = tempflds;

                      j=(-1);
                      HDmemfill(flds_indices, &j, sizeof(int32), MAXCHOICES);

                      m = 0;
                      lastItem = 0;
                          /* Extract each field name from the list of fields of the 
                             current record. */
                      for (j = 0; !lastItem; j++)
                        {
                            tempPtr = HDstrchr(ptr, ',');
                            if (tempPtr == NULL)
                                lastItem = 1;
                            else
                                *tempPtr = '\0';
                            HDstrcpy(string, ptr);
                            ptr = tempPtr + 1;
                                /* Compare the extracted field name with each of the names
                                   of the fields having been chosen. */
                            for (k = 0; flds_chosen[k] != NULL; k++)
                              {
                                  if (!HDstrcmp(flds_chosen[k], string))
                                    {
                                        flds_indices[m] = j;
                                        m++;
                                        flds_match = 1;
                                    }
                              }		/* for (k...) */
                        }	/* for (j...) */
                  }		/* if  */

                    /* If no fields were chosen, all fields are to be dumped out, and
                       so all fileds match. */
                if (flds_chosen[0] == NULL)
                    flds_match = 1;
                if (flds_match)
                  {
                      if (dumpvd_opts->contents == DDATA)
                              {
                                  data_only = 1;
                                  HDstrcpy(sep, "");
                              }
                            else
                              {
                                  data_only = 0;
                                  HDstrcpy(sep, ";");
                              }
                                /* Only the chosen or all fields will be dumped out. */
                            dumpvd(vd_id, ft, data_only, fp, sep, flds_indices, dumpallfields);
                  }
                VSdetach(vd_id);
            }	/* for */
            break;
      }    /* switch for output file   */

          Vend(file_id);
          if(vd_chosen!=NULL)
            {
                HDfree(vd_chosen);
                vd_chosen=NULL;
            } /* end if */
          if (dumpvd_opts->dump_to_file)
              fclose(fp);
          Hclose(file_id);
      }		/* while (curr_arg < argc)  */

    return (0);
}	/* dvd */

int32 
VSref_index(int32 file_id, int32 vd_ref)
{
    int32       find_ref = -1;
    int         index = 0;

    while ((find_ref = VSgetid(file_id, find_ref)) != -1)
      {
          if (find_ref == vd_ref)
              return(index);
          index++;
      }
    return (-1);
}	/* VSref_index */

int32 
VSstr_index(int32 file_id, char *filter_str,
            int name, int32 *find_ref, int32 *index)
{
    int32       vdata_id;
    char        vdata_name[MAXNAMELEN];

    while ((*find_ref = VSgetid(file_id, *find_ref)) != -1)
      {
          vdata_id = VSattach(file_id, *find_ref, "r");
          if (name)
              VSgetname(vdata_id, vdata_name);
          else
              VSgetclass(vdata_id, vdata_name);
          VSdetach(vdata_id);
          if (HDstrcmp(vdata_name, filter_str) == 0)
              return (*index);
          (*index)++;
      }
    return (-1);
}

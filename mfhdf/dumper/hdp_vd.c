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

static void 
dumpvd_usage(intn argc, 
             char *argv[])
{
	printf("Usage:\n");
	printf("%s dumpvd [-a|-i <index>|-n <name>|-r <ref>] [-dhv] [-o <filename> [-bx]] <filelist>\n", argv[0]);
	printf("\t-a\tDump all VDs in the file (default)\n");
	printf("\t-i <index>\tDump the <index>th SDS in the file \n");
	printf("\t-n <name>\tDump the VDS with name <name>\n");
	printf("\t-r <ref>\tDump the VDS with reference number <ref>\n");
	printf("\t-c <class>\tDump the VDS with class <class>\n");
	printf("\t-d\tDump data only, no tag/ref, formatted to input to hp2hdf\n");
	printf("\t-f <f1, f2,..> \tDump based on fields in vdata header\n");
	printf("\t-h\tDump header only, no annotation for elements nor data\n");
	printf("\t-v\tDump everything including all annotations (default)\n");
	printf("\t-o <filename>\tOutput to file <filename>\n");
	printf("\t-b\tBinary format of output\n");
	printf("\t-x\tAscii text format of output (default)\n");
}	/* end list_usage() */

static void 
init_dumpvd_opts(dump_info_t *dumpvd_opts)
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

/* exported */
intn 
parse_dumpvd_opts(dump_info_t *dumpvd_opts, 
                  intn *curr_arg, 
                  intn  argc,
                  char *argv[], 
                  char *flds_chosen[MAXCHOICES],
                  int  *dumpallfields)
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
                      fprintf(stderr,"Not enough memory!\n");
                      return FAIL;
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
                      fprintf(stderr,"Not enough memory!\n");
                      return FAIL;
                  }
                dumpvd_opts->filter_str[numItems] = NULL;

                ptr = argv[*curr_arg];
                i = 0;
                while ((tempPtr = HDstrchr(ptr, ',')) != NULL)
                  {
                      *tempPtr = '\0';
                      if ((dumpvd_opts->filter_str[i] = (char *) HDmalloc(sizeof(char) * (HDstrlen(ptr) + 1))) == NULL)
                        {
                            fprintf(stderr,"Not enough memory!\n");
                            return FAIL;
                        }
                      HDstrcpy(dumpvd_opts->filter_str[i], ptr);
                      ptr = tempPtr + 1;
                      i++;
                  }		/* end while */

                    /* Get the last string */
                if ((dumpvd_opts->filter_str[i] = (char *) HDmalloc(sizeof(char) * (HDstrlen(ptr) + 1))) == NULL)
                  {
                      fprintf(stderr,"Not enough memory!\n");
                      return FAIL;
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
                                  fprintf(stderr,"Not enough memory!\n");
                                  return FAIL;
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


static int32 
VSref_index(int32 file_id, 
            int32 vd_ref)
{
    int32  find_ref = -1;
    int    index    = 0; /* index is zero based? */
    int32  ret_value = FAIL;   

    while ((find_ref = VSgetid(file_id, find_ref)) != FAIL)
      {
          if (find_ref == vd_ref)
            {
              ret_value = index;
              goto done; /* found, done */
            }
          index++;
      }

done:
    if (ret_value == FAIL)
      { /* Failure cleanup */
      }
    /* Normal cleanup */
    
    return ret_value;
}	/* VSref_index */

static int32 
VSstr_index(int32 file_id, 
            char *filter_str,
            int   name, 
            int32 *find_ref, 
            int32 *index)
{
    int32  vdata_id = FAIL;
    char   vdata_name[MAXNAMELEN];
    int32  ret_value = FAIL;

    while ((*find_ref = VSgetid(file_id, *find_ref)) != FAIL)
      {
          vdata_id = VSattach(file_id, *find_ref, "r");
          if (FAIL == vdata_id)
              goto done; /* FAIL */

          if (name)
            {
              if (FAIL == VSgetname(vdata_id, vdata_name))
                  goto done; /* FAIL */
            }
          else
            {
              if (FAIL == VSgetclass(vdata_id, vdata_name))
                  goto done; /* FAIL */
            }

          if (FAIL == VSdetach(vdata_id))
              goto done; /* FAIL */

          if (HDstrcmp(vdata_name, filter_str) == 0)
            {
              ret_value = (*index);
              goto done; /* succeeded */
            }

          (*index)++;
      } /* end while getting vdatas */

done:
    if (ret_value == FAIL)
      { /* Failure cleanup */
      }
    /* Normal cleanup */
    
    return ret_value;
} /* VSstr_index() */


static intn
choose_vd(dump_info_t * dumpvd_opts, 
          int32 **vd_chosen,
          int32 *num_vd_chosen, 
          int32 file_id, 
          int *index_error)
{
    int32  i;
    int32  k = 0;
    int32  index;
    int32  find_ref;
    int32  number;
    intn   ret_value = SUCCEED;

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
                if (index == FAIL)
                  {
                      printf("Vdata with reference number %d: not found\n", dumpvd_opts->filter_num[i]);
                      *index_error = 1; /* index error */
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
                if (index == FAIL)
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
                                            &find_ref, &number)) != FAIL)
                  {
                      if (k < (int32)(*num_vd_chosen))
                          (*vd_chosen)[k] = index;
                      else
                        {
                            if ((*vd_chosen = (int32 *) HDrealloc(*vd_chosen,sizeof(int32) * (*num_vd_chosen+1))) == NULL)
                              {
                                  fprintf(stderr,"Memory re-allocation error\n");
                                  ret_value = FAIL;
                                  goto done;
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
                      *index_error = 1; /* index error */
                  }
            }	/* for */
          break;

      case DFIELDS:	/* Don't have to worry about which chosen fields yet. */
          break;

      case DALL:
          k = -1;
          break;

      default:
          printf("Unknown filter option\n");
          ret_value = FAIL;
          goto done;
      }		/* switch */

    *num_vd_chosen = k;

done:
    if (ret_value == FAIL)
      { /* Failure cleanup */
      }
    /* Normal cleanup */

    return ret_value;
}	/* choose_vd */

static intn
dumpvd_ascii(dump_info_t * dumpvd_opts, 
             int32 file_id,
             char  *file_name,
             int32 num_vd_chosen, 
             char *flds_chosen[MAXCHOICES],
             int32 *vd_chosen,
             int dumpallfields)
{
    int32       flds_indices[MAXCHOICES];
    int32       i, j, k, m, x;
    int32       nvf;
    int32       interlace;
    int32       vsize;
    int32       lastItem;
    int32       vdata_ref = -1;
    int32       vdata_tag;
    char        vdclass[VSNAMELENMAX];
    char        vdname[VSNAMELENMAX];
    char        fields[VSFIELDMAX*FIELDNAMELENMAX]; 
    char        tempflds[VSFIELDMAX*FIELDNAMELENMAX];
    char       *tempPtr = NULL;
    char       *ptr = NULL;
    char        string[MAXNAMELEN];
    FILE       *fp = NULL;
    intn        dumpall = 0;
    file_type_t ft = DASCII;
    int32       vd_id = FAIL;
    int32       an_handle   = FAIL;
    intn        ret_value = SUCCEED;

    /* set output file */
    if (dumpvd_opts->dump_to_file)
        fp = fopen(dumpvd_opts->file_name, "w");
    else
        fp = stdout;

    if (dumpvd_opts->contents != DDATA)
        fprintf(fp, "File name: %s \n\n", file_name);

    x = 0;	/* "x" is used to index the array of "vd_chosen". */

    /* Determine if all VDs are to be dumped out. */
    if (num_vd_chosen <= 0)	/* If so, set the corresponding flag. */
        dumpall = 1;
    else
      {
          /* Otherwise, sort the indices of the chosen VDs in increasing 
             order so that they will be dumped out in such order. */
          sort(vd_chosen,num_vd_chosen);
      }

    /* Examine each VD. */
    for (i = 0; (vdata_ref = VSgetid(file_id, vdata_ref)) != -1 && (dumpall!=0 || x<dumpvd_opts->num_chosen); i++)
      {
          int  data_only;
          int  flds_match = 0;
          char sep[2];

          /* Only dump the info of the chosen VDs or all of the VDs if none
             has been selected. */
          if ((!dumpall) && (i != vd_chosen[x]))
              continue; /* skip */

          x++;	/* One vdata has been located; so go to the next one in 
                   the array. */
          vd_id = VSattach(file_id, vdata_ref, "r");
          if (vd_id == FAIL)
            {
                fprintf(stderr,"VSattach failed for vdata_ref(%d) in file %s\n", 
                        (int) vdata_ref, file_name);
                ret_value = FAIL;
                goto done;
            }

          if (FAIL == VSinquire(vd_id, &nvf, &interlace, fields, &vsize, vdname))
            {
                fprintf(stderr,"VSinqure failed on vdid(%d) in file %s\n", 
                        (int) vdata_ref, file_name);
                ret_value = FAIL;
                goto done;
            }

 	  vsize = VShdfsize(vd_id, fields);
 	  if (vsize == FAIL)
            {
              	fprintf(stderr,"VShdfsize failed on vdid(%d) in file %s\n", 
 			(int) vdata_ref, file_name);
                ret_value = FAIL;
                goto done;
            }

          if (FAIL == (vdata_tag = VSQuerytag(vd_id)))
            {
                fprintf(stderr,"VSQuerytag failed on vd_id(%d) in file %s\n", 
                        (int) vdata_ref, file_name);
                ret_value = FAIL;
                goto done;
            }

          if (FAIL == VSgetclass(vd_id, vdclass))
            {
                fprintf(stderr,"VSgetclass failed on vd_id(%d) in file %s\n", 
                        (int) vdata_ref, file_name);
                ret_value = FAIL;
                goto done;
            }


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
                int32 z;
                int32 lastItem;
                int32 count = 0;

                switch (dumpvd_opts->contents)
                  {
                  case DVERBOSE: /* dump all information */
                  case DHEADER:	 /* header only, no attributes, annotations 
                                    or data */
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

                      /* check if only printing header */
                      if (dumpvd_opts->contents != DHEADER)
                        {
                            /* dump attributes */
                            if (FAIL == dumpattr(vd_id, _HDF_VDATA, 1, ft, fp))
                              {
                                  fprintf(stderr,"Failed to print vdata attributes for vd_id(%d) in file %s\n",
                                          (int) vd_id, file_name);
                                  ret_value = FAIL;
                                  goto done;
                              }

                            /* dump annotations */
                            /* start Annotation inteface */
                            if ((an_handle = ANstart(file_id)) == FAIL)
                              {
                                  fprintf(stderr,"ANstart failed on file_id(%d) for file %s\n", 
                                          (int)file_id, file_name);
                                  ret_value = FAIL;
                                  goto done;
                              }
                                  
                            /* print labels of vdata if any */
                            if (FAIL == print_data_labels(file_name,an_handle,vdata_tag, vdata_ref))
                              {
                                  fprintf(stderr,"Failed to print data labels for vdata_ref(%d) in file %s\n", 
                                          (int) vdata_ref, file_name);
                                  ret_value = FAIL;
                                  goto done;
                              }

                            /* print descriptions of vdata if any */
                            if (FAIL == print_data_descs(file_name,an_handle,vdata_tag, vdata_ref))
                              {
                                  printf("Failed to print data descriptions for vdata_ref(%d) in file %s\n", 
                                         (int) vdata_ref, file_name);
                                  ret_value = FAIL;
                                  goto done;
                              }

                            /* close annotation interface */
                            if (FAIL == ANend(an_handle))
                              {
                                  fprintf(stderr,"ANend failed for an_handle(%d) for file %s\n",
                                          (int)an_handle, file_name);
                                  ret_value = FAIL;
                                  goto done;
                              }

                            an_handle = FAIL; /* reset */

                            /* BMR - 6/30/98 to fix bug #236
				if no fields are defined or no data is
                                written, break out and don't fall through */
                            if ( fields[0] == '\0' || nvf == 0 )
                            {
                                fprintf( stderr, "<No data written>\n\n");
                                break;
                            }
                        }
                      else /* only header, no attributes, annotations or data */
                        {
                            break; /* break out and don't fall through */
                        }

                      /* note we fall through */
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
                      if (FAIL == dumpvd(vd_id, ft, data_only, fp, sep, flds_indices, dumpallfields))
                        {
                            fprintf(stderr,"Failed to dump vdata data for vd_id(%d) in file %s\n", 
                                    (int) vdata_ref, file_name);
                            ret_value = FAIL;
                            goto done;
                        }
                      break;
                  default:
                      printf("dumping vdata in file %s, unknown option \n",file_name);
                      ret_value = FAIL;
                      goto done;
                  }	/* switch */
            }

          if (FAIL == VSdetach(vd_id))
            {
                fprintf(stderr,"VSdetach failed on vd_id(%d) in file %s\n", 
                        (int) vd_id, file_name);
                ret_value = FAIL;
                goto done;
            }

          vd_id = FAIL; /* reset */

      }	/* for each vdata */

done:
    if (ret_value == FAIL)
      { /* Failure cleanup */
          if (an_handle != FAIL)
              ANend(an_handle);

          if (vd_id != FAIL)
              VSdetach(vd_id);
      }
    /* Normal cleanup */

    return ret_value;
} /* dumpvd_ascii() */


static intn
dumpvd_binary(dump_info_t * dumpvd_opts, 
              int32 file_id,
              char  *file_name,
              int32 num_vd_chosen, 
              char *flds_chosen[MAXCHOICES],
              int32 *vd_chosen,
              int dumpallfields)
{
    int32       flds_indices[MAXCHOICES];
    int32       i, j, k, m, x;
    int32       nvf;
    int32       interlace;
    int32       vsize;
    int32       lastItem;
    int32       vdata_ref = -1;
    int32       vdata_tag;
    char        vdclass[VSNAMELENMAX];
    char        vdname[VSNAMELENMAX];
    char        fields[VSFIELDMAX*FIELDNAMELENMAX]; 
    char        tempflds[VSFIELDMAX*FIELDNAMELENMAX];
    char       *tempPtr = NULL;
    char       *ptr = NULL;
    char        string[MAXNAMELEN];
    FILE       *fp = NULL;
    intn        dumpall = 0;
    file_type_t ft = DBINARY;
    int32       vd_id = FAIL;
    intn        ret_value = SUCCEED;


    /* Get output file name.  */
    if (dumpvd_opts->dump_to_file)
        fp = fopen(dumpvd_opts->file_name, "w");
    else
        fp = stdout;

    x = 0;	/* "x" is used to index the array of "vd_chosen". */

    /* Determine if all VDs are to be dumped out. */
    if (num_vd_chosen <= 0)	/* If so, set the corresponding flag. */
        dumpall = 1;
    else
      {
        /* Otherwise, sort the indices of the chosen VDs in increasing 
                       order so that they will be dumped out in such order. */
        sort(vd_chosen,num_vd_chosen);
      }

    /* Examine each VD. */
    for (i = 0; 
         (vdata_ref = VSgetid(file_id, vdata_ref)) != -1 
             && (dumpall != 0 || x < dumpvd_opts->num_chosen); 
         i++)
      {
          int         data_only, flds_match = 0;
          char        sep[2];

          /* Only dump the info of the chosen VDs or all of the VDs if none
             has been selected. */
          if ((!dumpall) && (i != vd_chosen[x]))
              continue; /* skip */

          x++;	/* One vdata has been located; so go to the next one in 
                   the array. */

          vd_id = VSattach(file_id, vdata_ref, "r");
          if (vd_id == FAIL)
            {
                fprintf(stderr,"VSattach failed for vdata_ref(%d) in file %s\n", 
                        (int) vdata_ref, file_name);
                ret_value = FAIL;
                goto done;
            }

          if (FAIL == VSinquire(vd_id, &nvf, &interlace, fields, &vsize, vdname))
            {
                fprintf(stderr,"VSinqure failed on vd_id(%d) in file %s\n", 
                        (int) vdata_ref,file_name);
                ret_value = FAIL;
                goto done;
            }

          if (FAIL == (vdata_tag = VSQuerytag(vd_id)))
            {
                fprintf(stderr,"VSQuerytag failed on vd_id(%d) in file %s\n", 
                        (int) vdata_ref, file_name);
                ret_value = FAIL;
                goto done;
            }

          if (FAIL == VSgetclass(vd_id, vdclass))
            {
                fprintf(stderr,"VSgetclass failed on vd_id(%d) in file %s\n", 
                        (int) vdata_ref, file_name);
                ret_value = FAIL;
                goto done;
            }


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
                if (FAIL == dumpvd(vd_id, ft, data_only, fp, sep, flds_indices, dumpallfields))
                  {
                      fprintf(stderr,"Failed to dump vdata data for vdid(%d) in file %s\n", 
                              (int) vd_id, file_name);
                      ret_value = FAIL;
                      goto done;
                  }
            }
                      
          if (FAIL == VSdetach(vd_id))
            {
                fprintf(stderr,"VSdetach failed on vd_id(%d) in file %s\n", 
                        (int) vd_id, file_name);
                ret_value = FAIL;
                goto done;
            }

          vd_id = FAIL; /* reset */

      }	/* for each vdata */

done:
    if (ret_value == FAIL)
      { /* Failure cleanup */
          if (vd_id != FAIL)
              VSdetach(vd_id);
      }
    /* Normal cleanup */

    return ret_value;
} /* dumpvd_binary */


static intn 
dvd(dump_info_t * dumpvd_opts, 
    intn curr_arg,
    intn argc, 
    char *argv[], 
    char *flds_chosen[MAXCHOICES], 
    int dumpallfields)
{
    int32       k;
    int32       file_id = FAIL;
    char        file_name[MAXFNLEN];
    int32      *vd_chosen = NULL;
    FILE       *fp = NULL;
    int32       num_vd_chosen;
    intn        index_error = 0;
    file_type_t ft;
    intn        ret_value = SUCCEED;

    while (curr_arg < argc)
      {	/* Loop until all specified files have been 
           processed */

          /* get file name */
          HDstrcpy(file_name, argv[curr_arg]);
          curr_arg++;

          /* try opening file */
          file_id = Hopen(file_name, DFACC_READ, 0);
          if (file_id == FAIL)
            {
                fprintf(stderr,"Failure in Hopen on file %s\n", file_name);
                ret_value = FAIL;
                goto done;
            }

          num_vd_chosen = dumpvd_opts->num_chosen;
          if (num_vd_chosen > 0)
            {
                if ((vd_chosen = (int32 *) HDmalloc(sizeof(int32) * num_vd_chosen)) == NULL)
                  {
                      fprintf(stderr,"Memory allocation error\n");
                      ret_value = FAIL;
                      goto done;
                  }		/* end if */

                k = (-1);
                HDmemfill(vd_chosen, &k, sizeof(int32), num_vd_chosen);
            }	/* end if */

          if (FAIL == Vstart(file_id))
            {
                fprintf(stderr,"Vstart failed for file %s\n", file_name);
                ret_value = FAIL;
                goto done;
            }

          /* Find out which VDs have been chosen. */
          if (FAIL == choose_vd(dumpvd_opts, &vd_chosen, &num_vd_chosen, file_id, &index_error))
            {
                fprintf(stderr,"failed to choose vdata in file %s\n", file_name);
                ret_value = FAIL;
                goto done;
            }

          /* In case of index error, skip the current file. 
             Isn't this bad anyway? */
          if (index_error && num_vd_chosen==0)
            {
                if(vd_chosen != NULL)
                  {
                      HDfree(vd_chosen);
                      vd_chosen = NULL;
                  } /* end if */

                Vend(file_id);
                Hclose(file_id);
                continue;
            } /* end if */

          /* output file? */
          ft = dumpvd_opts->file_type;

          switch(ft)
            {
            case DASCII:  /*    ASCII file   */

                if (FAIL == dumpvd_ascii(dumpvd_opts, file_id, file_name,
                                         num_vd_chosen, flds_chosen, 
                                         vd_chosen, dumpallfields))
                  {
                     ret_value = FAIL;
                     goto done;
                  }
                break;
            case DBINARY:   /*  binary file, not fully tested yet  */

                if (FAIL == dumpvd_binary(dumpvd_opts, file_id, file_name,
                                         num_vd_chosen, flds_chosen, 
                                         vd_chosen, dumpallfields))
                  {
                     ret_value = FAIL;
                     goto done;
                  }

                break;
            default:
                printf("dumping vdata, unknown ouput file option \n");
                ret_value = FAIL;
                goto done;
            }    /* switch for output file   */

          if(vd_chosen != NULL)
            {
                HDfree(vd_chosen);
                vd_chosen = NULL;
            } 

          if (dumpvd_opts->dump_to_file)
              fclose(fp);

          if (FAIL == Vend(file_id))
            {
              fprintf(stderr,"Vend failed on file %s\n", file_name);
              ret_value = FAIL;
              goto done;
            }

          if (FAIL == Hclose(file_id))
            {
              fprintf(stderr,"Hclose failed on file %s\n", file_name);
              ret_value = FAIL;
              goto done;
            }

          file_id = FAIL; /* reset */

      }	/* while processing files  */

done:
    if (ret_value == FAIL)
      { /* Failure cleanup */
          if (file_id != FAIL)
            {
                Vend(file_id);
                Hclose(file_id);
            }

          if(vd_chosen != NULL)
            {
                HDfree(vd_chosen);
                vd_chosen = NULL;
            } 
      }
    /* Normal cleanup */
    
    return ret_value;
}	/* dvd */

/* exported */
intn
do_dumpvd(intn curr_arg, 
          intn argc, 
          char *argv[], 
          dump_opt_t *glob_opts)
{
    dump_info_t dumpvd_opts;	/* dumpvd options */
    char       *flds_chosen[MAXCHOICES];
    int         dumpallfields;
    intn        ret_value = SUCCEED;

    flds_chosen[0] = NULL;
    dumpallfields = 1;

    if (glob_opts->help == TRUE)
      {
          dumpvd_usage(argc, argv);
          goto done;
      }		

    init_dumpvd_opts(&dumpvd_opts);

    if (parse_dumpvd_opts(&dumpvd_opts, &curr_arg, argc, argv, flds_chosen, &dumpallfields) == FAIL)
      {
          printf("Failure in parsing options to dump Vdata data \n");
          dumpvd_usage(argc, argv);
          ret_value = FAIL;
          goto done;
      }		

    if (dvd(&dumpvd_opts, curr_arg, argc, argv, flds_chosen, dumpallfields) == FAIL)
      {
          fprintf(stderr,"Failure in dumping Vdata data \n");
          ret_value = FAIL;
          goto done;
      } 

  done:
    if (ret_value == FAIL)
      { /* Failure cleanup */
      }
    /* Normal cleanup */
	if(dumpvd_opts.filter_num != NULL)
	  HDfree(dumpvd_opts.filter_num);

    return ret_value;
}	/* end do_dumpvd() */


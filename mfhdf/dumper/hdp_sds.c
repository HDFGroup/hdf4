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

void 
dumpsds_usage(intn argc, 
              char *argv[])
{
    printf("Usage:\n");
    printf("%s dumpsds [-a|-i <indices>|-r <refs>|-n <names>] [-dhv] [-o <filename>] [-bx] <filelist>\n", argv[0]);
    printf("\t-a\tDump all SDSs in the file (default)\n");
    printf("\t-i <indices>\tDump the SDSs at positions listed in <indices>\n");
    printf("\t-r <refs>\tDump the SDSs with reference number listed in <refs>\n");
    printf("\t-n <names>\tDump the SDSs with name listed in <names>\n");
    printf("\t-d\tDump data only, no tag/ref, formatted to input to hp2hdf\n");
    printf("\t-h\tDump header only, no annotation for elements nor data\n");
    printf("\t-v\tDump everything including all annotations (default)\n");
    printf("\t-o <filename>\tOutput to file <filename>\n");
    printf("\t-b\tBinary format of output\n");
    printf("\t-x\tAscii text format of output (default)\n");
    printf("\t<filelist>\tList of hdf file names, separated by spaces\n");
}	/* end list_usage() */

intn 
parse_dumpsds_opts(dump_info_t *dumpsds_opts, 
                   intn *curr_arg, 
                   intn argc,
                   char *argv[])
{
   /* traverse the command and process each option */
#if defined(WIN386) || defined(DOS386)
   while ((*curr_arg < argc) && ((argv[*curr_arg][0] == '-') ||
                                 (argv[*curr_arg][0] == '/')))
#else
   while ((*curr_arg < argc) && (argv[*curr_arg][0] == '-'))
#endif /* for the use of / as option on PC */
   {
      switch (argv[*curr_arg][1])
      {
         case 'a':	/* dump all, default */
            dumpsds_opts->filter = DALL;

            /* indicate that no specific SDS requested, will dump all */
            dumpsds_opts->num_chosen = NO_SPECIFIC;
            (*curr_arg)++;
            break;

         case 'i':	/* dump by index */
            dumpsds_opts->filter |= DINDEX;  /* set bit DINDEX */
            (*curr_arg)++;

            /* parse and store the given indices in structure by_index */
            parse_number_opts( argv, curr_arg, &dumpsds_opts->by_index);
            (*curr_arg)++;
            break;

         case 'r':	/* dump by reference */
            dumpsds_opts->filter |= DREFNUM;  /* set bit DREFNUM */
            (*curr_arg)++;

            /* parse and store the given ref numbers in structure by_ref */
            parse_number_opts( argv, curr_arg, &dumpsds_opts->by_ref);
           (*curr_arg)++;
            break;

         case 'n':	/* dump by names */
            dumpsds_opts->filter |= DNAME;   /* set bit DNAME */
            (*curr_arg)++;

            /* parse and store the given names in structure by_name */
            parse_string_opts( argv, curr_arg, &dumpsds_opts->by_name);
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

         case 'o':   /* specify output file */
                dumpsds_opts->dump_to_file = TRUE;

                /* Get file name */
                HDstrcpy(dumpsds_opts->file_name, argv[++(*curr_arg)]);

                (*curr_arg)++;
                break;

         case 'b':   /* dump data in binary */
                dumpsds_opts->file_type = DBINARY;
                (*curr_arg)++;
                break;

         case 'x':   /* dump data in ascii, also default */
                dumpsds_opts->file_type = DASCII;
                (*curr_arg)++;
                break;

         default:    /* invalid dumpsds option */
                printf("Invalid dumpsds option %s\n", argv[*curr_arg]);
                return (FAIL);
            }	/* end switch */
      }		/* end while */

   /* add the number of datasets requested by index, by ref#, and by name
      to have a total number of requested datasets */
   dumpsds_opts->num_chosen = dumpsds_opts->by_index.num_items +
                             dumpsds_opts->by_ref.num_items +
                             dumpsds_opts->by_name.num_items;

    return (SUCCEED);
}	/* end parse_dumpsds_opts */

int32 
sdsdumpfull( int32        sds_id, 
             dump_info_t *dumpsds_opts,
             int32        rank, 
             int32        dimsizes[], 
             int32        nt, 
             FILE        *fp)
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
    intn     count, indent = 16;  /* better than in parameter list */
    intn     emptySDS = TRUE;
    file_type_t ft;
    char     curr_file_name[MAXFNLEN];
    int32    ret_value = SUCCEED;

   /* temp. names for file type and file name for ease of use */
   ft = dumpsds_opts->file_type;
   HDstrcpy( curr_file_name, dumpsds_opts->ifile_name );

   /* Compute the number of the bytes for each value. */
   numtype = nt & DFNT_MASK;
   eltsz = DFKNTsize(numtype | DFNT_NATIVE);

   read_nelts = dimsizes[rank - 1];

   /* make sure we are not allocating 0 elements */
   validate_pos( read_nelts, "read_nelts", "sdsdumpfull" );
   validate_pos( eltsz, "eltsz", "sdsdumpfull" );
   validate_pos( rank, "rank", "sdsdumpfull" );

   buf = (VOIDP) HDmalloc(read_nelts * eltsz);
   if (buf == NULL)
   {
      fprintf(stderr,"Failure in sdsdumpfull: Not enough memory!\n");
      exit(1);
   }

   left = (int32 *) HDmalloc(rank * sizeof(int32));
   if (left == NULL)
   {
      fprintf(stderr,"Failure in sdsdumpfull: Not enough memory!\n");
      exit(1);
   }

    start = (int32 *) HDmalloc(rank * sizeof(int32));
    if (start == NULL)
   {
      fprintf(stderr,"Failure in sdsdumpfull: Not enough memory!\n");
      exit(1);
   }

   edge = (int32 *) HDmalloc(rank * sizeof(int32));
    if (edge == NULL)
   {
      fprintf(stderr,"Failure in sdsdumpfull: Not enough memory!\n");
      exit(1);
   }

/* BMR - how come GR has stride ? */
    for (i = 0; i < rank; i++)
   {
          start[i] = 0;		/* Starting location to read the data. */
          left[i] = dimsizes[i];
          edge[i] = 1;	/* Number of values to read in each dimension. */
   }
   edge[rank - 1] = dimsizes[rank - 1];

    /* check if the SDS has data before proceeding */
   if( SDcheckempty( sds_id, &emptySDS ) == FAIL )
   {
      fprintf( stderr, "SDcheckempty failed for sds_id(%d) in file %s\n",
			(int) sds_id, curr_file_name );
      ret_value = FAIL;  /* caller will go to next sds */
      goto done;
   }
    if( emptySDS )
   {
      if( ft == DASCII )
         fprintf( fp, "No data written.\n" );
      ret_value = SUCCEED;  /* because the dump for this SDS is */
      goto done;	/* successful although it's empty -> next SDS */
   }
   if (rank == 1)
   { /* If there is only one dimension, then dump the data
               and the job is done. */
      if (FAIL == SDreaddata(sds_id, start, NULL, edge, buf))
      {
         fprintf(stderr,"SDreaddata failed for sds_id(%d) in file %s\n",
                        (int)sds_id, curr_file_name );
         ret_value = FAIL;
         goto done;
      }

      if(FAIL == dumpfull(numtype, ft, read_nelts, buf, indent, fp))
      {
         fprintf(stderr,"dumpfull failed for sds_id(%d) in file %s\n",
                        (int)sds_id, curr_file_name );
         ret_value = FAIL;
         goto done;
      }
   }
   else if (rank > 1)
   {
      done = 0;

      /* In each iteration, a row in dumped and "left[]" is modified accordingly(?) */
      while (!done)
      {
         if (FAIL == SDreaddata(sds_id, start, NULL, edge, buf))
         {
            fprintf(stderr,"SDreaddata failed for sds_id(%d) in file %s\n",
                              (int)sds_id, curr_file_name );
            ret_value = FAIL;
            goto done;
         }

         if (FAIL == dumpfull(numtype, ft, read_nelts, buf, indent, fp))
         {
            fprintf(stderr,"dumpfull failed for sds_id(%d) in file %s\n",
                              (int)sds_id, curr_file_name );
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
	    {  /* Proceed in the same dimension; as long as there are
	       elements in this dimension, this loop breaks here After the
	       last element in the current dimension has been subtracted,
	       we substract one fro the next lower dimension and reset
	       "left[j]" to be the size of dimension j. */
                 start[j]++;
                 break;
            }
            else
            {  /* Nothing left in the current dimension.  So, subtract one
               from the (j-1)th dimension and reset the value of "left[j]". */

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

/* if there are any specific datasets requested, alloc_index_list 
   allocates space for the list of indices of these requested items */ 
void
alloc_index_list(
	int32 **index_list,
	int32 num_chosen )
{
   int32 i = -1;	/* used to pass into HDmemfill as dummmy? */ 

   *index_list = (int32 *) HDmalloc(sizeof(int32) * num_chosen); 
   if( *index_list == NULL )
   {
      fprintf(stderr,"Failure to allocate space in alloc_index_list\n");
      exit(1);
   }   /* end if */

   i = (-1);
   HDmemfill(*index_list, &i, sizeof(int32), num_chosen);
}  /* end of alloc_index_list */

/* compose the list of indices of the requested SDSs although some
   SDSs are requested by ref# or name.
   The routine returns:
        - the number of SDSs to be processed, or
        - NO_SPECIFIC if all SDSs are to be processed, or
        - 0 if none.
   If any failure occurs, the parameter index_error will be set to TRUE */
intn
get_SDSindex_list( 
        int32 sd_id, 
        dump_info_t *dumpsds_opts,
        int32 **sds_chosen,/* array of indices of SDSs to be processed */
        intn *index_error )
{
   intn     i;
   int32    index,          /* index of an SDS */
            sds_count = 0,  /* number of SDSs to be processed */
            num_sds_chosen = dumpsds_opts->num_chosen;
   filter_t filter = dumpsds_opts->filter; /* temporary name */
   intn     ret_value = 0;  /* assume that no SDS will be processed */

   /* if no specific datasets are requested, return the SDS count as 
      NO_SPECIFIC (-1) to indicate that all datasets are to be dumped */
   if( filter == DALL )
   {
      ret_value = NO_SPECIFIC;
      goto done;
   }

   /* if specific datasets were requested, allocate space for the array
      of indices */
   if (num_sds_chosen > 0)
      alloc_index_list( sds_chosen, num_sds_chosen );

   /* else, no chosen SDSs but filter is not DALL, it shouldn't be this
      combination, return SDS count as NO_SPECIFIC to dump all */
   else
   {
      ret_value = NO_SPECIFIC;
      goto done;
   }

   /* if there are some SDSs requested by index, store the indices in
      the array sds_chosen */
   if( filter & DINDEX )
         /* Note: Don't replace this with HDmemcpy unless you change 
                  the sizes of the objects correctly -QAK */
      for (i = 0; i < dumpsds_opts->by_index.num_items; i++)
      {
         (*sds_chosen)[sds_count] = dumpsds_opts->by_index.num_list[i];
         sds_count++;
      }

   /* if there are some SDSs requested by ref#, convert the ref#s to indices
      and store the indices in the array provided by the caller, sds_chosen */
   if( filter & DREFNUM )
      for (i = 0; i < dumpsds_opts->by_ref.num_items; i++)
      {
         index = SDreftoindex(sd_id, dumpsds_opts->by_ref.num_list[i]);
         if (index == FAIL)
         {
         /*fprintf( stderr, "SDS with reference number %d: not found\n",*/
            printf( "SDS with reference number %d: not found\n",
                               dumpsds_opts->by_ref.num_list[i]);
            *index_error = TRUE; /* error */
         }
         else
         {
            (*sds_chosen)[sds_count] = index;
            sds_count++;
         }
      }

   /* if there are some SDSs requested by name, convert the names to indices 
      and store the indices in the array provided by the caller, sds_chosen */
   if( filter & DNAME )
      for (i = 0; i < dumpsds_opts->by_name.num_items; i++)
      {
         index = SDnametoindex(sd_id, dumpsds_opts->by_name.str_list[i]);
         if (index == FAIL)
         {
         /*I want to use fprintf but it causes make test fail dumpsds-4.out*/
         /* fprintf( stderr, "SDS with name %s: not found\n", */
            printf( "SDS with name %s: not found\n",
                              dumpsds_opts->by_name.str_list[i]);
            *index_error = TRUE; /* error */
         }
         else
         {
            (*sds_chosen)[sds_count] = index;
            sds_count++;
         }
      }

   ret_value = sds_count;
done:
    if (ret_value == FAIL)
      { /* Failure cleanup */
      }
    /* Normal cleanup */
    return ret_value;
} /* end of get_SDSindex_list */

/* displays all SD file attributes */
intn
print_SDattrs( int32 sd_id,
               FILE *fp,
               int32 n_file_attrs,
               const char *curr_file_name )
{
   int32 attr_index,
         attr_count,
         attr_nt,
         attr_buf_size;
   char  attr_name[MAXNAMELEN],
        *attr_nt_desc = NULL;
   VOIDP attr_buf=NULL;
   intn  printed = FALSE; /* whether file attr title has been printed */
   intn  status,          /* status from a called routine */
         ret_value = SUCCEED;

   /* for each file attribute, print its info and values */
   for (attr_index = 0; attr_index < n_file_attrs; attr_index++)
   {
      /* get the current attr's name, number type, and number of values */
      status = SDattrinfo(sd_id, attr_index, attr_name, &attr_nt, &attr_count);
      if( status == FAIL )
      {
         fprintf(stderr, "SDattrinfo failed for %d'th attribute in file %s\n", attr_index, curr_file_name );
         ret_value = FAIL; /* so caller can be traced in debugging */
         continue; /* to the next attribute */
      }
      /* to be sure that attr_buf is free before reuse since sometimes we
         have to break the current loop and continue to the next item */
      resetBuff( &attr_buf );

      /* calculate the buffer size of the attribute using the number of
         values in the attribute and its value size */
      attr_buf_size = DFKNTsize(attr_nt) * attr_count;

      /* make sure we are not allocating 0 elements */
      validate_pos( attr_buf_size, "attr_buf_size", "print_SDattrs" );

      /* allocate space for the attribute's values */
      attr_buf = (VOIDP) HDmalloc(attr_buf_size);
      if (attr_buf == NULL)
      {
         fprintf(stderr,"Failure to allocate space in print_SDattrs\n");
         exit(1);
      }

      /* read the values of the attribute into the buffer attr_buf */
      status = SDreadattr(sd_id, attr_index, attr_buf);
      if( status == FAIL )
      {
         fprintf(stderr,"SDreadattr failed for %d'th attribute in file %s\n", attr_index, curr_file_name );
         ret_value = FAIL; /* so caller can be traced in debugging */
         continue; /* to the next attribute */
      }

      /* get number type description of the attribute */
      attr_nt_desc = HDgetNTdesc(attr_nt);
      if (attr_nt_desc == NULL)
      {
         fprintf(stderr,"HDgetNTdesc failed for %d'th attribute in file %s\n", attr_index, curr_file_name );
         ret_value = FAIL; /* so caller can be traced in debugging */
         continue; /* to the next attribute */
      }

      /* print a title line for file attributes if it's not printed
         yet and set flag so it won't be printed again */
      if( !printed )
      {
         fprintf( fp, "\nFile attributes:\n" );
         printed = TRUE;
      }

      /* display the attribute's information then free buffer */
      fprintf(fp,"\t Attr%i: Name = %s\n", (int) attr_index, attr_name);
      fprintf(fp,"\t\tType = %s \n\t\tCount= %i\n", attr_nt_desc, (int) attr_count);
      resetBuff(( VOIDP *) &attr_nt_desc );

      /* display the attribute's values */
      /* Note that filetype is DASCII since binary format does not contain
         these information - it's data only */
      fprintf(fp,"\t\tValue = ");
      status = dumpfull(attr_nt, DASCII, attr_count, attr_buf, 20, fp);
      if( status == FAIL )
      {
         fprintf(stderr,"dumpfull failed for %d'th attribute in file %s\n", attr_index, curr_file_name );
         ret_value = FAIL; /* so caller can be traced in debugging */
         continue; /* to the next attribute */
      }
      resetBuff( &attr_buf );  /* free buffer and reset it to NULL */
   }/* for each file attribute */

  done:
    if (ret_value == FAIL)
      { /* Failure cleanup */
         resetBuff((VOIDP *) &attr_nt_desc );
         resetBuff( &attr_buf );
      }
    /* Normal cleanup */

    return ret_value;
}   /* end of print_SDattrs */

intn
print_SDSattrs( int32 sds_id,
                int32 sds_index,
                int32 nattrs,
                FILE *fp,
                char *curr_file_name )
{
   int32 attr_index,
         attr_count,
         attr_nt,
         attr_buf_size;
   char  attr_name[MAXNAMELEN],
        *attr_nt_desc = NULL;
   VOIDP attr_buf=NULL;
   intn  status,   /* status returned from a called routine */
         ret_value = SUCCEED; /* returned value of print_SDSattrs */

   /* for each attribute, display its info and data */
   for (attr_index = 0; attr_index < nattrs; attr_index++)
   {
      /* get the current attr's name, number type, and number of values */
      status = SDattrinfo(sds_id, attr_index, attr_name, &attr_nt, &attr_count);
      if (status == FAIL)
      {
         fprintf(stderr,"SDattrinfo failed for %d'th attribute of sds_id(%d) in file %s\n", attr_index,(int)sds_id, curr_file_name);
         ret_value = FAIL;  /* so caller can be traced in debugging */
         continue; /* to the next attribute */
      }

      /* to be sure that attr_buf is free before reuse since sometimes we
         have to break the current loop and continue to the next item */
      resetBuff( &attr_buf );

      /* calculate the buffer size of the attribute using the number of
         values in the attribute and its value size */
      attr_buf_size = DFKNTsize(attr_nt|DFNT_NATIVE) * attr_count;

      /* make sure we are not allocating 0 elements */
      validate_pos( attr_buf_size, "attr_buf_size", "print_SDSattrs" );

      /* allocate space for attribute's values */
      attr_buf = (VOIDP) HDmalloc(attr_buf_size);
      if (attr_buf == NULL)
      {
         fprintf(stderr,"Not enough memory!\n");
         exit(1);
      }

      /* read the values of the attribute into buffer attr_buf */
      status = SDreadattr(sds_id, attr_index, attr_buf);
      if (status == FAIL)
      {
         fprintf(stderr,"SDreadattr failed for %d'th attribute for sds_id(%d) in file %s\n", attr_index,(int)sds_id, curr_file_name);
         ret_value = FAIL;  /* so caller can be traced in debugging */
         continue; /* to the next attribute */
      }

      /* get number type description of the attribute */
      attr_nt_desc = HDgetNTdesc(attr_nt);
      if (attr_nt_desc == NULL)
      {
         fprintf(stderr,"HDgetNTdesc failed for %d'th attribute of sds_id(%d) in file %s\n", attr_index,(int)sds_id, curr_file_name);
         ret_value = FAIL;  /* so caller can be traced in debugging */
         continue; /* to the next attribute */
      }

      /* display the attribute's information then free the buffer */
      fprintf(fp, "\t Attr%d: Name = %s\n", (int) attr_index, attr_name);
      fprintf(fp, "\t\t Type = %s \n\t\t Count= %d\n", attr_nt_desc, (int) attr_count);
      resetBuff((VOIDP *) &attr_nt_desc );

      /* display the attribute's values then free buffer */
      /* Note that filetype is DASCII since binary format does not contain
         these information - it's data only */
      fprintf(fp, "\t\t Value = ");

      if (FAIL == dumpfull(attr_nt, DASCII, attr_count, attr_buf, 20, fp))
      {
         fprintf(stderr,"dumpfull failed for %d'th attribute of sds_id(%d) in file %s\n", attr_index, (int)sds_id, curr_file_name);
         ret_value = FAIL;  /* so caller can be traced in debugging */
         continue; /* to the next attribute */
      }
      resetBuff( &attr_buf );  /* free buffer and reset it to NULL */
   } /* for each attribute */
done:
    if (ret_value == FAIL)
      { /* Failure cleanup */
          resetBuff((VOIDP *) &attr_nt_desc );
          resetBuff( &attr_buf );
      }
    /* Normal cleanup */

    return ret_value;
} /* end of print_SDSattrs */

void
resetSDS(
	int32 *sds_id,
	int32  sds_index,
	char  *curr_file_name )
{
   if( *sds_id != FAIL )
   {
      if( FAIL == SDendaccess( *sds_id ))
         fprintf(stderr,"SDendaccess failed for %d'th SDS in file %s\n",
                    sds_index, curr_file_name );
      *sds_id = FAIL;
   }
}  /* end of resetSDS */

intn
printSD_ASCII( 
	int32 sd_id,
	dump_info_t *dumpsds_opts,
	int32 ndsets,         /* # of datasets in the file */
	int32 *sds_chosen,    /* list of indices of SDSs */
	int32 num_sds_chosen, /* # of items in sds_chosen */
	FILE *fp )
{
   int32 sds_id = FAIL, /* SDS id, always reset to FAIL when not used */
         sds_ref,       /* ref# of an SDS */
         sds_count,     /* count of SDSs being printed */
         sds_index,     /* index of SDSs in the file */
         dim_id = FAIL, /* id of an SDS dimension */
         rank,          /* number of dimensions of an SDS */
         nt,            /* number type of an SDS */
         nattrs,        /* # of attributes assigned to an SDS */
         dimsizes[MAXRANK],     /* SDS dimensions */
         dimNT[MAXRANK],        /* number type of dimension */
         dimnattr[MAXRANK];     /* # of attributes of a dim */
   char  dim_nm[MAXNAMELEN],    /* dimension name */
         name[MAXNAMELEN],      /* SDS name */
         attr_name[MAXNAMELEN], /* attribute name */
         *nt_desc = NULL,       /* SDS's or dim's num type description */
         *attr_nt_desc = NULL,  /* attr's nt description */
         curr_file_name[MAXFNLEN]; /* curr hdf file name */
   intn  isdimvar,      /* TRUE if curr SDS is used for a dim */
         j,
         dumpall = FALSE,    /* TRUE if all SDSs are to be dumped */
         status,             /* status returned from a routine */
         ret_value = SUCCEED;/* returned value of printSD_ASCII */

   /* temp. name for curr input file name for ease of use */
   HDstrcpy( curr_file_name, dumpsds_opts->ifile_name );

   /* when there are no SDS specified, dumper dumps all datasets */
   if (num_sds_chosen == (NO_SPECIFIC))  /* NO_SPECIFIC = -1 */
      dumpall = TRUE;
   else		/* otherwise, sort index list */
      sort(sds_chosen, num_sds_chosen);

   /* for each valid index, if the user request to dump all SDSs or if
      there are more requested SDSs to be processed, process the curr SDS */
   sds_count = 0;   /* no SDS has been processed yet */
   for (sds_index = 0; sds_index < ndsets  /* validate index */
        && (dumpall                /* either all datasets are dumped or */
        || sds_count < num_sds_chosen); /* or more requested datasets */
        sds_index++)
   {
      /* if the user neither requests dump all nor the current dataset */
      if ((!dumpall) && (sds_index != sds_chosen[sds_count]))
         continue; /* skip */

      sds_count++;   /* count the # of datasets being processed */

      /* Reset variables. */
      HDmemset(dimsizes, 0, sizeof(int32) * MAXRANK);
      HDmemset(dimNT, 0, sizeof(int32) * MAXRANK);
      HDmemset(dimnattr, 0, sizeof(int32) * MAXRANK);

      /* get access to the current dataset */
      sds_id = SDselect(sd_id, sds_index);
      if (sds_id == FAIL)
      {
         fprintf(stderr,"SDselect failed for %d'th SDS in file %s\n",
                                    sds_index, curr_file_name);
         ret_value = FAIL;   /* so caller can be traced in debugging */
         continue; /* to the next dataset */
      }

      /* get dataset's information */
      status = SDgetinfo(sds_id, name, &rank, dimsizes, &nt, &nattrs);
      if (FAIL == status )
      {
         fprintf(stderr,"SDgetinfo failed for %d'th SDS in file %s\n", 
                                    sds_index, curr_file_name);
         resetSDS( &sds_id, sds_index, curr_file_name );
         ret_value = FAIL;   /* so caller can be traced in debugging */
         continue; /* to next dimension */
      }

      /* BMR: it seems like this whole block of code is to get number
         type of dim0, all the other info will be discarded */
      isdimvar = (SDiscoordvar(sds_id)) ? TRUE : FALSE;
      if (isdimvar) 
      { /* use dim0 nt instead of dimvar nt, because when no dim values 
           dimvar nt was set to float32 by default */
         int32 size, num_attrs;

         /* get dimension id for accessing */
         dim_id = SDgetdimid(sds_id, 0);
         if( dim_id == FAIL )
         {
            fprintf(stderr,"SDgetdimid failed for %d'th SDS in file %s\n",
                                    (int)sds_index, curr_file_name);
            resetSDS( &sds_id, sds_index, curr_file_name );
            ret_value = FAIL;   /* so caller can be traced in debugging */
            continue; /* to next dataset */
         }

         /* get information of current dimension */
         status = SDdiminfo(dim_id, NULL, &size, &nt, &num_attrs);
         if( status == FAIL )
         {
            fprintf(stderr,"SDdiminfo failed for %d'th SDS in file %s\n",
                                    (int)sds_index, curr_file_name);
            resetSDS( &sds_id, sds_index, curr_file_name );
            ret_value = FAIL;   /* so caller can be traced in debugging */
            continue; /* to next dataset */
         }
      }                                                       
      /* print the current SDS's as specified by user's options */
      switch (dumpsds_opts->contents)
      {
         case DVERBOSE:
         case DHEADER:
            nt_desc = HDgetNTdesc(nt);
            if (nt_desc == NULL)
            {
               fprintf(stderr,"HDgetNTdesc failed for %d'th SDS in file %s\n", 
                               sds_index, curr_file_name);
               ret_value = FAIL;   /* so caller can be traced in debugging */
               /* did this one fail on allocation and need exit(1)? */
               break;
            }

            /* Note: a variable can be used to hold an SDS or a dimension. */
            /* display dimension info if the variable is used to contain a
               dimension */
            if (isdimvar)
            {
               fprintf(fp, "\nDimension Variable Name = %s\n\t ", name);
               fprintf(fp, "Index = %d\n\t Scale Type= %s\n", sds_index, nt_desc);
            }
            /* display the SDS info, otherwise */
            else
            {
               fprintf(fp, "\nVariable Name = %s\n\t Index = ", name);
               fprintf(fp, "%d\n\t Type= %s\n", sds_index, nt_desc);
            }

            resetBuff(( VOIDP *) &nt_desc );  /* done with nt_desc */

            /* get SDS's ref# from its id */
            if ((sds_ref = SDidtoref(sds_id)) == FAIL)
            {
               fprintf(stderr,"Failure in determining reference for %d'th SDS in file %s.\n", sds_index, curr_file_name);
               ret_value = FAIL;   /* so caller can be traced in debugging */
               break;  /* out of switch */
            }

            fprintf(fp, "\t Ref. = %d\n", (int) sds_ref);
            fprintf(fp, "\t Rank = %d\n\t Number of attributes = %d\n",
                                        (int) rank, (int) nattrs);

            /* print each dimension of the current SDS */
            for (j = 0; j < rank; j++)
            {
               int32 size;    /* size of the current dimension */

               /* get current dimension id for access */
               if (FAIL == (dim_id = SDgetdimid(sds_id, j)))
               {
                  fprintf(stderr,"SDgetdimid failed for %d'th SDS in file %s\n",
                                  sds_index, curr_file_name);
                  ret_value = FAIL;   /* so caller can be traced in debugging */
                  break;  /* out of switch */
               }

               /* get information of current dimension */
               status = SDdiminfo(dim_id,dim_nm,&size,&(dimNT[j]),&(dimnattr[j])
);
               if (FAIL == status )
               {
                  fprintf(stderr, "SDdiminfo failed for %d'th dimension of %d'th SDS in file %s\n", j, sds_index, curr_file_name);
                  ret_value = FAIL;   /* so caller can be traced in debugging */
                  break;  /* out of switch */
               }

               fprintf(fp, "\t Dim%d: Name=%s\n", (int) j, dim_nm);
               if (size == 0)
               {
                  fprintf(fp, "\t\t Size = UNLIMITED ");
                  fprintf(fp, "(currently %d)\n", (int) dimsizes[j]);
               }
               else
                  fprintf(fp, "\t\t Size = %d\n", (int) dimsizes[j]);

               /* don't print type and # of attrs for dim var */
               if (isdimvar == 0)
               {  
                  attr_nt_desc = HDgetNTdesc(dimNT[j]);
                  if (attr_nt_desc == NULL)
                  {
                     fprintf(stderr,"HDgetNTdesc failed for %d'th dimension of %d'th SDS in file %s\n", j, sds_index, curr_file_name);
                     ret_value = FAIL;
                     break;
                  }
                  fprintf(fp, "\t\t Scale Type = %s\n", attr_nt_desc);
                  fprintf(fp, "\t\t Number of attributes = %d\n", (int) dimnattr[j]);
                  resetBuff(( VOIDP *) &attr_nt_desc);
               }
            } /* end each for dimension */

            /* print dataset's attributes */
            status = print_SDSattrs(sds_id, sds_index,nattrs, fp, curr_file_name);
            if( status == FAIL )
            {
               fprintf( stderr, "Failure in print_SDSattrs\n" );
               ret_value = FAIL;
               break; /* so SDendaccess can be called, then go to next SDS */
            }

            /* header only, don't go into case DDATA */
            if (dumpsds_opts->contents == DHEADER)
               break;

            /* case DDATA doesn't need this */
            fprintf(fp, "\t Data : \n");

         case DDATA:
            if (rank > 0 && dimsizes[0] != 0)
            {
               if (!isdimvar || nt != 0)
               { /* no dump if dimvar w/o scale values */

                  intn        count;

                  fprintf(fp, "                ");/* spaces to line up output */

                  status = sdsdumpfull( sds_id, dumpsds_opts, rank, dimsizes, nt, fp);
                  if( FAIL == status )
                  {
                     fprintf(stderr,"sdsdumpfull failed for %d'th SDS in file %s\n",
                                 sds_index, curr_file_name);
                     ret_value = FAIL;
                     break;
                  }
               }
            }
            break;
         default:
            printf("Output format must be either -d, -h, or -v only.\n");
      }	 /* switch  */

      resetSDS( &sds_id, sds_index, curr_file_name ); /* end access to current SDS */

   }	/* for each SDS in the file */

   return( ret_value );
} /* end of printSD_ASCII() */

intn
printSD_BINARY( int32 sd_id,
             FILE *fp,
             dump_info_t *dumpsds_opts,
             int32 *sds_chosen,
             int32 num_sds_chosen,
             int32 ndsets )
{
   int32 sds_id,
         sds_index,
         sds_count,
         dimsizes[MAXRANK],
         dim_id = FAIL,
         dimNT[MAXRANK],
         dimnattr[MAXRANK],
         rank,
         nt,
         nattrs;
   char  dim_nm[MAXNAMELEN],
         name[MAXNAMELEN],
         attr_name[MAXNAMELEN],
         curr_file_name[MAXFNLEN];
   intn  dumpall = FALSE,
         status,
         ret_value = SUCCEED;
   
   /* temp. names for file type and curr input file name for ease of use */
   HDstrcpy( curr_file_name, dumpsds_opts->ifile_name );

   /* when there are no datasets specified, dumper dumps all datasets */
   if (num_sds_chosen == NO_SPECIFIC )
      dumpall = TRUE;
   else
      sort(sds_chosen, num_sds_chosen);

   /* for each valid index, if the user request to dump all datasets or
      if there are more requested datasets to be processed, process the
      indexed dataset */
   sds_count = 0;   /* no requested dataset has been processed yet */
   for (sds_index = 0; sds_index < ndsets  /* validate index */
        && (dumpall                /* either all datasets are dumped or */
        || sds_count < num_sds_chosen); /* more requested datasets */
        sds_index++)
   {
      /* if the user neither requests dump all nor the current dataset */
      if ((!dumpall) && (sds_index != sds_chosen[sds_count]))
         continue; /* skip */

      sds_count++;   /* count the # of datasets have been processed */

      /* Reset variables. */
      HDmemset(dimsizes, 0, sizeof(int32) * MAXRANK);
      HDmemset(dimNT, 0, sizeof(int32) * MAXRANK);
      HDmemset(dimnattr, 0, sizeof(int32) * MAXRANK);

      sds_id = SDselect(sd_id, sds_index);
      if (sds_id == FAIL)
      {
         fprintf(stderr,"SDselect failed for %d'th SDS in file %s\n",
                                    sds_index, curr_file_name);
         ret_value = FAIL;   /* so caller can be traced in debugging */
         continue; /* to the next dataset */
      }

      if (FAIL == SDgetinfo(sds_id, name, &rank, dimsizes, &nt, &nattrs))
      {
         fprintf(stderr,"SDgetinfo failed for sds_id(%d) in file %s\n", 
                                    (int)sds_id, curr_file_name);
         resetSDS( &sds_id, sds_index, curr_file_name );
         ret_value = FAIL;   /* so caller can be traced in debugging */
         continue; /* to next dataset */
      }

      /* output data to binary file   */
      if (rank > 0 && dimsizes[0] != 0)
      {
         status = sdsdumpfull(sds_id, dumpsds_opts, rank, dimsizes, nt, fp);
         if( FAIL == status )
         {
            fprintf(stderr,"sdsdumpfull failed for %d'th SDS in file %s\n",                                 sds_index, curr_file_name);
            ret_value = FAIL;
            /* will end access then go to the next dataset naturally */
         }
      }
      resetSDS( &sds_id, sds_index, curr_file_name );
   }  /* for each dataset in file */
done:
    if (ret_value == FAIL)
      { /* Failure cleanup */
      }
    /* Normal cleanup */

    return ret_value;
} /* end of printSD_BINARY */

intn 
dsd(dump_info_t *dumpsds_opts, 
    intn         curr_arg, 
    intn         argc, 
    char        *argv[])
{
   int32  sd_id = FAIL,
         *sds_chosen = NULL,
          num_sds_chosen,
          ndsets,
          n_file_attrs;
   char   file_name[MAXFNLEN],
          name[MAXNAMELEN],
          dim_nm[MAXNAMELEN];
   FILE  *fp = NULL;
   intn   index_error = 0,
          i, j,
          status,
          ret_value = SUCCEED;

   /* check for missing input file name */
   if( curr_arg >= argc )
   {
      fprintf( stderr, "Missing input file name.  Please try again.\n");
      return( FAIL ); /* nothing to be cleaned up at this point */
   }

   /* going through each input file, open the file, try to compose the list
      of indices of the SDSs in the file that are requested, then read and
      display information and data of each SDS in the specified manner */
   while (curr_arg < argc)
   {
      HDstrcpy(file_name, argv[curr_arg]);
      HDstrcpy( dumpsds_opts->ifile_name, file_name ); /* record file name */
      curr_arg++;

      sd_id = SDstart(file_name, DFACC_RDONLY);
      if (sd_id == FAIL)
      {
         fprintf(stderr,"Failure in opening file %s\n", file_name);
         ret_value = FAIL;
         continue; /* to the next file */
      }

      /* compose the list of indices of SDSs to be processed in the current 
         file: sds_chosen is the list and return the number of items in it */
      num_sds_chosen = get_SDSindex_list( sd_id, dumpsds_opts, &sds_chosen, &index_error );

      /* if there are errors with the given indices, ref#s, or names of the
         requested datasets, and so the input yields no valid datasets,
         then close the interface and the input file, and move on to the
         next file */
      if (index_error && num_sds_chosen == 0)
      {
         if(sds_chosen!=NULL)
         {
            HDfree(sds_chosen);
            sds_chosen=NULL;
         } /* end if */
         if( FAIL == SDend(sd_id))
            fprintf( stderr, "SDend failed for the current file\n" );
         continue; /* to the next file */
      } /* end if */

      /* obtain number of datasets in the file and number of file attributes,
         ndsets will be used to process the datasets, n_file_attrs will be
         used to print file attributes */
      status = SDfileinfo(sd_id, &ndsets, &n_file_attrs);
      if (status == FAIL)
      {
         fprintf(stderr,"SDfileinfo failed for file %s\n", file_name);
         ret_value = FAIL;
         goto done;
      }

      fp = stdout;                /* assume no output file given */

      /* ASCII or binary dump? */
      switch( dumpsds_opts->file_type )
      {
         case DASCII:       /* ASCII file */
            /* open output file for ASCII or direct to standard output */
            if (dumpsds_opts->dump_to_file)
               fp = fopen(dumpsds_opts->file_name, "w");

            /* display the name and global attributes of the current file if
               data only option is not selected */
            if (dumpsds_opts->contents != DDATA)
            {
               fprintf(fp, "File name: %s \n", file_name);
               /* print SD file attributes */
               status = print_SDattrs( sd_id, fp, n_file_attrs, file_name);
               if( status == FAIL )
               {
                  fprintf(stderr,"print_SDattrs failed for file %s\n", file_name);
                  ret_value = FAIL;
                  continue; /* to the next file */

               }
            }
            status = printSD_ASCII( sd_id, dumpsds_opts, ndsets, sds_chosen, num_sds_chosen, fp );
            if( status == FAIL )
               fprintf( stderr, "Failure in printSD_ASCII: Unable to dump data for file %s\n", file_name );

            /* close output file only if option -o is given */
            if (dumpsds_opts->dump_to_file)
               fclose(fp);                       
            break;

         case DBINARY:       /* binary file */
             /* get output file name  */
             if (dumpsds_opts->dump_to_file)
                 fp = fopen(dumpsds_opts->file_name, "wb");
            status = printSD_BINARY( sd_id, fp, dumpsds_opts, sds_chosen, num_sds_chosen, ndsets );
            if( status == FAIL )
               fprintf( stderr, "Failure in printSD_BINARY: Unable to dump data for file %s\n", file_name );

            /* close output file only if option -o is given */
            if (dumpsds_opts->dump_to_file)
               fclose(fp);                       
            break;

         default:
            printf("Output file type must be either ascii or binary only\n");
      } /* switch for output file */

      if (sds_chosen != NULL)
      {
         HDfree(sds_chosen);
         sds_chosen = NULL;
      } /* end if */

      if (FAIL == SDend(sd_id))
      {
         fprintf(stderr, "SDend failed for file %s\n", file_name);
         continue; /* to the next file */
      }
      sd_id = FAIL; /* reset */
   }  /* while processing files  */

done:
    if (ret_value == FAIL)
      { /* Failure cleanup */
          if (sd_id != FAIL)
              SDend(sd_id);

          if (sds_chosen != NULL)
            {
                HDfree(sds_chosen);
                sds_chosen=NULL;
            } /* end if */
      }
    /* Normal cleanup */

    return ret_value;
}	/* dsd */

/* Exported */
intn
do_dumpsds(intn  curr_arg, 
           intn  argc, 
           char *argv[], 
           intn  help )
{
    dump_info_t dumpsds_opts;	/* dumpsds options */
    intn status, ret_value = SUCCEED;

   /* initialize the structure that holds user's options and inputs */
   init_dump_opts(&dumpsds_opts);

   /* command line: hdp help */
   if (help == TRUE)
   {
      dumpsds_usage(argc, argv);
      goto done;
   }  /* end if */

   /* incomplete command */
   if( curr_arg >= argc )
   {
      dumpsds_usage(argc, argv);
      ret_value = FAIL; /* so caller can be traced in debugging */
      goto done;
   }            /* end if */

   /* parse the user's command and store the inputs in dumpsds_opts */
   status = parse_dumpsds_opts( &dumpsds_opts, &curr_arg, argc, argv );
   if( status == FAIL )
   {
      dumpsds_usage(argc, argv);
      ret_value = FAIL; /* so caller can be traced in debugging */
      goto done; /* skip dsd */
   }

   /* display data and information as specified in dumpsds_opts */
   status = dsd( &dumpsds_opts, curr_arg, argc, argv );
   if( status == FAIL )
   {
      fprintf( stderr, "Failure in dsd.\n" );
      ret_value = FAIL; /* so caller can be traced in debugging */
      goto done; /* skip dsd */
   }

  done:
    if (ret_value == FAIL)
      { /* Failure cleanup */
      }
    /* Normal cleanup */

   /* free the lists for given indices, ref#s, and names if
      they had been allocated */
   free_num_list(dumpsds_opts.by_index.num_list );
   free_num_list(dumpsds_opts.by_ref.num_list );
   free_str_list(dumpsds_opts.by_name.str_list, dumpsds_opts.by_name.num_items);

   return ret_value;
}	/* end do_dumpsds() */


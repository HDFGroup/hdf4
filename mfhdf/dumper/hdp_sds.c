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

/* $Id$ */

#include <stdio.h>
#include "mfhdf.h"
#include "hdp.h"
#ifndef MIPSEL
#include <math.h>
#endif /* MIPSEL */

#ifdef H4_HAVE_LIBSZ
#include "szlib.h"
intn have_szip = 1;
#else
intn have_szip = 0;
#endif
#include "cszip.h"
#include "local_nc.h"	/* to use some definitions */

void dumpsds_usage(intn argc, 
              char *argv[])
{
    printf("Usage:\n");
    printf("%s dumpsds [-k][-a|-i <indices>|-r <refs>|-n <names>] [-cdhvs] [-o <filename>] [-bx] <filelist>\n", argv[0]);
    printf("\t-k\tDump SDSs in the given order, must be specified before -i/-r/-n\n");
    printf("\t-a\tDump all SDSs in the file (default)\n");
    printf("\t-i <indices>\tDump the SDSs at positions listed in <indices>\n");
    printf("\t-r <refs>\tDump the SDSs with reference number listed in <refs>\n");
    printf("\t-n <names>\tDump the SDSs with name listed in <names>\n");
    printf("\t-d\tDump data only, no tag/ref, formatted to input to hp2hdf\n");
    printf("\t-h\tDump header only, no annotation for elements nor data\n");
    printf("\t-v\tDump everything including all annotations (default)\n");
    printf("\t-c\tPrint space characters as they are, not \\digit\n");
    printf("\t-g\tDo not print data of file (global) attributes\n");
    printf("\t-l\tDo not print data of local attributes\n");
    printf("\t-s\tDo not add carriage return to a long line - dump it as a stream\n");
    printf("\t-o <filename>\tOutput to file <filename>\n");
    printf("\t-b\tBinary format of output\n");
    printf("\t-x\tAscii text format of output (default)\n");
    printf("\t<filelist>\tList of hdf file names, separated by spaces\n");
}	/* end list_usage() */

intn parse_dumpsds_opts(dump_info_t *dumpsds_opts, 
                   intn *curr_arg, 
                   intn argc,
                   char *argv[])
{
   intn ret_value = SUCCEED;

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
	 case 'k':	/* dump SDSs in the order they were given */
/* need to document that this option must be provided before any -i, -r, -n */
	    dumpsds_opts->keep_order = TRUE;
            (*curr_arg)++;
	    break;

         case 'a':	/* dump all, default */
            dumpsds_opts->filter = DALL;

            /* indicate that no specific SDS requested, will dump all */
            dumpsds_opts->num_chosen = NO_SPECIFIC;
            (*curr_arg)++;
            break;

         case 'i':	/* dump by index */
            (*curr_arg)++; /* go to the parameters for this flag */

            /* Parse and store the given indices in structure all_types */
	    parse_value_opts(argv, curr_arg, &dumpsds_opts, IS_INDEX);
            (*curr_arg)++;
            break;

         case 'r':	/* dump by reference */
            (*curr_arg)++; /* go to the parameters for this flag */

            /* Parse and store the given ref numbers in structure all_types */
	    parse_value_opts(argv, curr_arg, &dumpsds_opts, IS_REFNUM);
            (*curr_arg)++;
            break;

         case 'n':	/* dump by names */
            (*curr_arg)++; /* go to the parameters for this flag */

            /* Parse and store the given names in structure all_types */
	    parse_value_opts(argv, curr_arg, &dumpsds_opts, IS_NAME);
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

         case 's':	/* do not add carriage returns to output data lines */
                dumpsds_opts->as_stream = TRUE;
                (*curr_arg)++;
                break;

         case 'c':	/* print space characters as they are, not \digit */
                dumpsds_opts->clean_output = TRUE;
                (*curr_arg)++;
                break;

         case 'g':	/* suppress file (global) attr data, print its header */
                dumpsds_opts->no_gattr_data = TRUE;
                (*curr_arg)++;
                break;

         case 'l':	/* suppress local attr data, only print its header */
                dumpsds_opts->no_lattr_data = TRUE;
                (*curr_arg)++;
                break;

         case 'o':   /* specify output file */
                dumpsds_opts->dump_to_file = TRUE;

                /* Get file name */
                HDstrcpy(dumpsds_opts->file_name, argv[++(*curr_arg)]);

                (*curr_arg)++;
                break;

         case 'b':   /* dump data in binary */
                dumpsds_opts->file_format = DBINARY;
                (*curr_arg)++;
                break;

         case 'x':   /* dump data in ascii, also default */
                dumpsds_opts->file_format = DASCII;
                (*curr_arg)++;
                break;

         default:    /* invalid dumpsds option */
                printf("HDP ERROR>>> Invalid dumpsds option %s\n", argv[*curr_arg]);
                HGOTO_DONE(FAIL);
            }	/* end switch */
      }		/* end while */

done:
   if (ret_value == FAIL)
   { /* Failure cleanup */
      /* free the list if it had been allocated */
      free_obj_chosen_t_list(&dumpsds_opts->all_types, dumpsds_opts->num_chosen);
   }
   /* Normal cleanup */
   return (ret_value);
}	/* end parse_dumpsds_opts */

/* sdsdumpfull prints a single SDS */
int32 sdsdumpfull( int32        sds_id, 
             dump_info_t *dumpsds_opts,
             int32        rank, 
             int32        dimsizes[], 
             int32        nt, 
             FILE        *fp)
{
	/* "rank" is the number of dimensions and 
	   "dimsizes[i]" is size of dimension "i". */
   int32    j, i;
   VOIDP    buf = NULL;		/* holds one row of data */
   int32    numtype;
   int32    eltsz;
   int32    read_nelts;		/* number of elements in one row */
   int32    done;			/* number of rows we have done */
   int32   *left = NULL;
   int32   *start = NULL;
   int32   *edge = NULL;
   intn     emptySDS = TRUE;
   file_format_t ff;
   intn     status = FAIL;
   int32    status32 = FAIL;
   int32    ret_value = SUCCEED;

   /* temp. names for ease of use */
   ff = dumpsds_opts->file_format;

   /* Compute the number of the bytes for each value. */
   numtype = nt & DFNT_MASK;
   eltsz = DFKNTsize(numtype | DFNT_NATIVE);

   read_nelts = dimsizes[rank - 1];

   /* make sure we are not allocating 0 elements */
   CHECK_POS( read_nelts, "read_nelts", "sdsdumpfull" );
   CHECK_POS( eltsz, "eltsz", "sdsdumpfull" );
   CHECK_POS( rank, "rank", "sdsdumpfull" );

   buf = (VOIDP) HDmalloc(read_nelts * eltsz);
   CHECK_ALLOC( buf, "buf", "sdsdumpfull" );

   left = (int32 *) HDmalloc(rank * sizeof(int32));
   CHECK_ALLOC( left, "left", "sdsdumpfull" );

   start = (int32 *) HDmalloc(rank * sizeof(int32));
   CHECK_ALLOC( start, "start", "sdsdumpfull" );

   edge = (int32 *) HDmalloc(rank * sizeof(int32));
   CHECK_ALLOC( edge, "edge", "sdsdumpfull" );

/* BMR - how come this doesn't have stride as for GR? */
    for (i = 0; i < rank; i++)
   {
          start[i] = 0;		/* Starting location to read the data. */
          left[i] = dimsizes[i];
          edge[i] = 1;	/* Number of values to read in each dimension. */
   }

   /* so that the last edge has many elements as the last dimension??? */
   edge[rank - 1] = dimsizes[rank - 1];

   /* check if the SDS has data before proceeding if the file is HDF file */
   /* see bug HDFFR- regarding non-HDF files */
   if (dumpsds_opts->file_type == HDF_FILE)
   {
      status32 = SDcheckempty( sds_id, &emptySDS );
      if( status32 == FAIL )
         ERROR_GOTO_2( "in %s: SDcheckempty failed for sds_id(%d)",
			"sdsdumpfull", (int) sds_id );
      if( emptySDS )
      {
         if( ff == DASCII ) /* what about binary??? - BMR */
            fprintf( fp, "                No data written.\n" );
         HGOTO_DONE( SUCCEED );  /* because the dump for this SDS is */
      			/* successful although it's empty -> next SDS */
      }
   } /* file is HDF */

   /* Should handle the case of rank==0 here. -BMR */

   if (rank == 1)
   { /* If there is only one dimension, then dump the data
               and the job is done. */
      if (FAIL == SDreaddata(sds_id, start, NULL, edge, buf))
      {
	 /* If the data set has external element, get the external file
	    name to provide information */
	 intn extfile_namelen = SDgetexternalfile(sds_id, 0, NULL, NULL);
	 if (extfile_namelen > 0)
	 {
	    char *extfile_name = NULL;
	    extfile_name = (char *)HDmalloc(sizeof(char *)*(extfile_namelen+1));
	    CHECK_ALLOC(extfile_name, "extfile_name", "sdsdumpfull" );

	    /* Get the external file information, we don't need offset here */
	    extfile_namelen = SDgetexternalfile(sds_id, extfile_namelen, extfile_name, NULL);
            ERROR_GOTO_3( "in %s: SDreaddata failed for sds_id(%d) with external file %s.  Please verify the file exists in the same directory.",
			"sdsdumpfull", (int)sds_id, extfile_name);
	 }
	 else
            ERROR_GOTO_2( "in %s: SDreaddata failed for sds_id(%d)",
			"sdsdumpfull", (int)sds_id );
      }

      /* if printing data only, print with no indentation */
      if( dumpsds_opts->contents == DDATA )
         status = dumpfull(numtype, dumpsds_opts, read_nelts, buf, fp,
				0, 0 );
      else
         status = dumpfull(numtype, dumpsds_opts, read_nelts, buf, fp,
				DATA_INDENT, DATA_CONT_INDENT );
      if( FAIL == status )
         ERROR_GOTO_2( "in %s: dumpfull failed for sds_id(%d)",
                        "sdsdumpfull", (int)sds_id );
   }
   else if (rank > 1)
   {
      done = 0;

      /* In each iteration, a row in dumped and "left[]" is modified 
	accordingly(?) */
      while (!done)
      {
         if (FAIL == SDreaddata(sds_id, start, NULL, edge, buf))
         {
	    /* If the data set has external element, get the external file
	       name to provide information */
	    intn extfile_namelen = SDgetexternalfile(sds_id, 0, NULL, NULL);
	    if (extfile_namelen > 0)
	    {
	       char *extfile_name = NULL;
	       extfile_name = (char *)HDmalloc(sizeof(char *)*(extfile_namelen+1));
	       CHECK_ALLOC(extfile_name, "extfile_name", "sdsdumpfull" );

	       /* Get the external file information, we don't need offset here */
	       extfile_namelen = SDgetexternalfile(sds_id, extfile_namelen, extfile_name, NULL);
               ERROR_GOTO_3( "in %s: SDreaddata failed for sds_id(%d) with external file %s.  Please verify the file exists in the same directory.",
			"sdsdumpfull", (int)sds_id, extfile_name);
	    }
	    else
               ERROR_GOTO_2( "in %s: SDreaddata failed for sds_id(%d)",
                        "sdsdumpfull", (int)sds_id );
        }

         /* if printing data only, print with no indentation */
         if( dumpsds_opts->contents == DDATA )
            status = dumpfull(numtype, dumpsds_opts, read_nelts, buf, fp,
				0, 0 );
         else
            status = dumpfull(numtype, dumpsds_opts, read_nelts, buf, fp,
				DATA_INDENT, DATA_CONT_INDENT );

         if( FAIL == status )
            ERROR_GOTO_2( "in %s: dumpfull failed for sds_id(%d)",
                        "sdsdumpfull", (int)sds_id );

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
	       elements in this dimension, this loop breaks here after the
	       last element in the current dimension has been subtracted,
	       we substract one for the next lower dimension and reset
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

   	       /* someone added an extra line b/w two dims of data for nice format;
		  this causes 1 extra line at the end of the output but I still
		  don't understand the logic here so I left it alone; just 
		  removed the spaces attempting to line up the data. BMR 7/13/00 */
               /*if( ff==DASCII && !dumpsds_opts->as_stream )*/
               if( ff==DASCII )
                  if (j == rank - 2)
                     fprintf(fp, "\n");
            }
         }  /* for j */
      }	 /* while   */
   }  /* else */

   /* add an extra line between two datasets for pretty format 
      this also causes 1 extra line at the end of the output! */
    /*if (ff == DASCII && !dumpsds_opts->as_stream )*/
    if (ff == DASCII )
        fprintf(fp, "\n");

done:
    if (ret_value == FAIL)
      { /* Failure cleanup */
      }
    /* Normal cleanup */
      SAFE_FREE(edge)
      SAFE_FREE(start)
      SAFE_FREE(left)
      SAFE_FREE(buf)

    return ret_value;
}	/* sdsdumpfull */

/* compose the list of indices of the requested SDSs although some
   SDSs are requested by ref# or name.
   The routine returns:
        - the number of SDSs to be processed, or
        - NO_SPECIFIC if all SDSs are to be processed, or
        - 0 if none.
   If any failure occurs, the parameter index_error will be set to TRUE
*/
intn get_SDSindex_list( 
        int32 sd_id, 
        dump_info_t *dumpsds_opts,
        int32 **sds_chosen,/* array of indices of SDSs to be processed */
        intn *index_error )
{
    intn     ii;
    int32    sds_index,          /* index of an SDS */
             sds_count = 0,  /* number of SDSs to be processed */
             num_sds_chosen = dumpsds_opts->num_chosen;
    intn     ret_value = 0;  /* assume that no SDS will be processed */

    /* if no specific datasets are requested, return the SDS count as 
       NO_SPECIFIC (-1) to indicate that all datasets are to be dumped */
    if (dumpsds_opts->num_chosen == NO_SPECIFIC)
	HGOTO_DONE( NO_SPECIFIC );

    /* if specific datasets were requested, allocate space for the array
       of indices */
    if (num_sds_chosen > 0)
	alloc_index_list( sds_chosen, num_sds_chosen );

    /* else, no chosen SDSs but filter is not DALL, it shouldn't be this
       combination, return SDS count as NO_SPECIFIC to dump all */
    else
	HGOTO_DONE( NO_SPECIFIC );

    for (ii = 0; ii < num_sds_chosen; ii++)
    {
int jj;
	/* if the current chosen SDS was requested by its index, store the
	index in the array sds_chosen */
	switch (dumpsds_opts->all_types[ii].type_of_info)
	{
	  case IS_INDEX:
	    (*sds_chosen)[ii] = dumpsds_opts->all_types[ii].index;
	    sds_count++;
	  break;

	  /* if the current chosen SDS was requested by its ref#, convert the
	     ref# to index and store the index in the array sds_chosen */
	  case IS_REFNUM:
	    sds_index = SDreftoindex(sd_id, dumpsds_opts->all_types[ii].refnum);
	    if (sds_index == FAIL)
	    {
	        printf( "SDS with reference number %d: not found\n",
                               (int)dumpsds_opts->all_types[ii].refnum);
	        *index_error = TRUE; /* error */
	    }
	    else
	    {
	        (*sds_chosen)[sds_count] = sds_index;
	        sds_count++;
	    }
	    break;

	  /* if the current chosen SDS was requested by its name, convert the
	     name to index and store the index in the array sds_chosen */
	  case IS_NAME:
	    sds_index = SDnametoindex(sd_id, dumpsds_opts->all_types[ii].name);
/* NOTE: should handle the case of more than one var of the same name too */
	    if (sds_index == FAIL)
	    {
                printf( "SDS with name '%s': not found\n",
                              dumpsds_opts->all_types[ii].name);
                *index_error = TRUE; /* error */
	    }
	    else
	    {
                (*sds_chosen)[sds_count] = sds_index;
                sds_count++;
	    }
	    break;

	  default:
	    fprintf(stderr, "in get_SDSindex_list: Info should only be an index, ref number, or name of an SDS\n");
	    *index_error = TRUE; /* error */
	} /* end switch */
    } /* end for loop */

    ret_value = sds_count;
done:
    if (ret_value == FAIL)
      { /* Failure cleanup */
      }
    /* Normal cleanup */
    return ret_value;
} /* end of get_SDSindex_list */

/* Displays all SD file attributes */
intn print_SDattrs( int32 sd_id,
               FILE *fp,
               int32 n_file_attrs,
	       dump_info_t* dumpsds_opts )
{
   int32 attr_index,
         attr_count,
         attr_nt,
         attr_buf_size;
   char  attr_name[MAXNAMELEN],
        *attr_nt_desc = NULL;
   VOIDP attr_buf=NULL;
   intn  printed = FALSE; /* whether file attr title has been printed */
   intn  status = FAIL,          /* status from a called routine */
         ret_value = SUCCEED;

   /* for each file attribute, print its info and values */
   for (attr_index = 0; attr_index < n_file_attrs; attr_index++)
   {
      /* get the current attr's name, number type, and number of values */
      status = SDattrinfo(sd_id, attr_index, attr_name, &attr_nt, &attr_count);
      if( status == FAIL )
         ERROR_CONT_2( "in %s: SDattrinfo failed for %d'th attribute", 
			"print_SDattrs", (int)attr_index );
     
      /* get number type description of the attribute */
      attr_nt_desc = HDgetNTdesc(attr_nt);
      if (attr_nt_desc == NULL)
         ERROR_CONT_2( "in %s: HDgetNTdesc failed for %d'th attribute", 
			"print_SDattrs", (int)attr_index );

      /* print a title line for file attributes if it's not printed
         yet and set flag so it won't be printed again */
      if( !printed )
      {
         fprintf( fp, "\nFile attributes:\n" );
         printed = TRUE;
      }

      /* display the attribute's information */
      fprintf(fp,"\t Attr%i: Name = %s\n", (int) attr_index, attr_name);
      fprintf(fp,"\t\t Type = %s \n\t\t Count= %i\n", attr_nt_desc, (int) attr_count);
      resetBuff((VOIDP) &attr_nt_desc);

      /* display the attribute's values unless user chose to suppress them or
	there are no values stored */
      if (dumpsds_opts->no_gattr_data == FALSE && attr_count != 0)
      {
         /* to be sure that attr_buf is free before reuse since sometimes we
            have to break the current loop and continue to the next item */
         resetBuff( &attr_buf );

	 /* calculate the buffer size of the attribute using the number of
	    values in the attribute and its value size */
	 attr_buf_size = DFKNTsize(attr_nt) * attr_count;

	 /* make sure we are not allocating 0 elements */
	 CHECK_POS( attr_buf_size, "attr_buf_size", "print_SDattrs" );

	 /* allocate space for the attribute's values */
	 attr_buf = (VOIDP) HDmalloc(attr_buf_size);

	 /* if allocation fails, handle the failure */
	 CHECK_ALLOC( attr_buf, "attr_buf", "print_SDattrs" );

	 /* read the values of the attribute into the buffer attr_buf */
	 status = SDreadattr(sd_id, attr_index, attr_buf);
	 if( status == FAIL )
	    ERROR_CONT_2( "in %s: SDreadattr failed for %d'th attribute", 
			"print_SDattrs", (int)attr_index );

	 fprintf( fp,"\t\t Value = ");

	 /* if the user wishes to have clean output, i.e. option -c is 
	    selected - Note that this option is only applicable to DFNT_CHAR 
	    type, the option will be ignored for other types */
	 if( dumpsds_opts->clean_output && attr_nt == DFNT_CHAR )
	 {
	    status = dumpclean(attr_nt, dumpsds_opts, attr_count, attr_buf, fp);
	    if( status == FAIL )
		ERROR_CONT_2( "in %s: dumpclean failed for %d'th attribute", 
			"print_SDattrs", (int)attr_index );
	 }
	 else  /* show tab, lf, null char... in octal as \011, \012, \000... */
	 {
	    status = dumpfull(attr_nt, dumpsds_opts, attr_count, attr_buf, fp,
				ATTR_INDENT, ATTR_CONT_INDENT );
	    if( status == FAIL )
		ERROR_CONT_2( "in %s: dumpfull failed for %d'th attribute", 
			"print_SDattrs", (int)attr_index );
	 }
      }  /* end of if no file attributes */
   }  /* for each file attribute */

   return( ret_value );
}   /* end of print_SDattrs */

intn print_SDSattrs( int32 sds_id,
                int32 nattrs,
                FILE *fp,
	        dump_info_t *dumpsds_opts)
{
   int32 attr_index,
         attr_count,
         attr_nt,
         attr_buf_size;
   char  attr_name[MAXNAMELEN],
        *attr_nt_desc = NULL;
   VOIDP attr_buf=NULL;
   intn  status = FAIL,   /* status returned from a called routine */
         ret_value = SUCCEED; /* returned value of print_SDSattrs */

   /* for each attribute, display its info and data */
   for (attr_index = 0; attr_index < nattrs; attr_index++)
   {
      /* get the current attr's name, number type, and number of values */
      status = SDattrinfo(sds_id, attr_index, attr_name, &attr_nt, &attr_count);
      if (status == FAIL)
         ERROR_CONT_2( "in %s: SDattrinfo failed for %d'th attribute", 
			"print_SDSattrs", (int)attr_index );

      /* calculate the buffer size of the attribute using the number of
         values in the attribute and its value size */
      attr_buf_size = DFKNTsize(attr_nt|DFNT_NATIVE) * attr_count;

      /* make sure we are not allocating 0 elements */
      CHECK_POS( attr_buf_size, "attr_buf_size", "print_SDSattrs" );

      /* get number type description of the attribute */
      attr_nt_desc = HDgetNTdesc(attr_nt);
      if (attr_nt_desc == NULL)
         ERROR_CONT_2( "in %s: HDgetNTdesc failed for %d'th attribute", 
			"print_SDSattrs", (int)attr_index );

      /* display the attribute's information */
      fprintf(fp, "\t Attr%d: Name = %s\n", (int) attr_index, attr_name);
      fprintf(fp, "\t\t Type = %s \n\t\t Count= %d\n", 
			attr_nt_desc, (int) attr_count);

      /* free buffer and reset it to NULL */
      resetBuff((VOIDP) &attr_nt_desc);

      /* display the attribute's values unless user chose to suppress them 
	 or there are no values stored */
      if (dumpsds_opts->no_lattr_data == FALSE && attr_count != 0)
      {
         /* to be sure that attr_buf is free before reuse since sometimes we
            have to break the current loop and continue to the next item */
         resetBuff( &attr_buf );

        /* allocate space for attribute's values */
        attr_buf = (VOIDP) HDmalloc(attr_buf_size);
        CHECK_ALLOC( attr_buf, "attr_buf", "print_SDSattrs" );

        /* read the values of the attribute into buffer attr_buf */
        status = SDreadattr(sds_id, attr_index, attr_buf);
        if (status == FAIL)
        {
            HDfree(attr_buf);
            ERROR_CONT_2( "in %s: SDreadattr failed for %d'th attribute", 
			              "print_SDSattrs", (int)attr_index );
        }

        fprintf(fp, "\t\t Value = ");

        /* if the user wishes to have clean output, i.e. option -c is 
        selected - Note that this option is only applicable to DFNT_CHAR 
        type, the option will be ignored for other types */
        if( dumpsds_opts->clean_output && attr_nt == DFNT_CHAR )
        {
            status = dumpclean(attr_nt, dumpsds_opts, attr_count, attr_buf, fp);
            if( status == FAIL )
            {
                HDfree(attr_buf);
                ERROR_CONT_2( "in %s: dumpclean failed for %d'th attribute", 
			                  "print_SDSattrs", (int)attr_index );
            }
        }
        else  /* show tab, lf, null char... in octal as \011, \012, \000... */
        {
            status = dumpfull(attr_nt, dumpsds_opts, attr_count, attr_buf, fp,
				ATTR_INDENT, ATTR_CONT_INDENT );
            if( status == FAIL )
            {
                HDfree(attr_buf);
                ERROR_CONT_2( "in %s: dumpfull failed for %d'th attribute", 
			                  "print_SDSattrs", (int)attr_index );
            }
        }
      }  /* end of if no local attributes */
      SAFE_FREE(attr_buf);
   } /* for each attribute */

   return( ret_value );
} /* end of print_SDSattrs */

void resetSDS(
	int32 *sds_id,
	int32  sds_index,
	char  *curr_file_name )
{
   if( *sds_id != FAIL )
   {
      if( FAIL == SDendaccess( *sds_id ))
         fprintf(stderr,"SDendaccess failed for %d'th SDS in file %s\n",
                    (int)sds_index, curr_file_name );
      *sds_id = FAIL;
   }
}  /* end of resetSDS */

/*
 * Forms a string containing all the szip encoding schemes and other options
 * that are set in the parameter options_mask.  BMR - bugzilla 1202 - Jul, 2008
 */
intn option_mask_string(int32 options_mask, char* opt_mask_strg)
{
    intn ret_value = SUCCEED;
    char numval[10];

    strcpy(opt_mask_strg, "");	/* init string to empty string */

    /* just in case options_mask is invalid */
    if (options_mask == 0)
	ret_value = FAIL;

    /* mask options_mask with each szip encoding schemes and options to 
       form the associate option mask string */
    if ((options_mask & H4_SZ_ALLOW_K13_OPTION_MASK) != 0)
	strcpy(opt_mask_strg, "H4_SZ_ALLOW_K13_OPTION_MASK");
    if ((options_mask & H4_SZ_CHIP_OPTION_MASK) != 0)
    {
	if (strlen(opt_mask_strg) > 0)
	{
	    strcat(opt_mask_strg, "|");
	    strcat(opt_mask_strg, "H4_SZ_CHIP_OPTION_MASK");
	}
	else
	    strcpy(opt_mask_strg, "H4_SZ_CHIP_OPTION_MASK");
    }
    if ((options_mask & H4_SZ_EC_OPTION_MASK) != 0)
    {
	if (strlen(opt_mask_strg) > 0)
	{
	    strcat(opt_mask_strg, "|");
	    strcat(opt_mask_strg, "H4_SZ_EC_OPTION_MASK");
	}
	else
	    strcpy(opt_mask_strg, "H4_SZ_EC_OPTION_MASK");
    }
    if ((options_mask & H4_SZ_LSB_OPTION_MASK) != 0)
    {
	if (strlen(opt_mask_strg) > 0)
	{
	    strcat(opt_mask_strg, "|");
	    strcat(opt_mask_strg, "H4_SZ_LSB_OPTION_MASK");
	}
	else
	    strcpy(opt_mask_strg, "H4_SZ_LSB_OPTION_MASK");
    }
    if ((options_mask & H4_SZ_MSB_OPTION_MASK) != 0)
    {
	if (strlen(opt_mask_strg) > 0)
	{
	    strcat(opt_mask_strg, "|");
	    strcat(opt_mask_strg, "H4_SZ_MSB_OPTION_MASK");
	}
	else
	    strcpy(opt_mask_strg, "H4_SZ_MSB_OPTION_MASK");
    }
    if ((options_mask & H4_SZ_NN_OPTION_MASK) != 0)
    {
	if (strlen(opt_mask_strg) > 0)
	{
	    strcat(opt_mask_strg, "|");
	    strcat(opt_mask_strg, "H4_SZ_NN_OPTION_MASK");
	}
	else
	    strcpy(opt_mask_strg, "H4_SZ_NN_OPTION_MASK");
    }
    if ((options_mask & H4_SZ_RAW_OPTION_MASK) != 0)
    {
	if (strlen(opt_mask_strg) > 0)
	{
	    strcat(opt_mask_strg, "|");
	    strcat(opt_mask_strg, "H4_SZ_RAW_OPTION_MASK");
	}
	else
	    strcpy(opt_mask_strg, "H4_SZ_RAW_OPTION_MASK");
    }

    /* also print the numerical value of the options mask */
    sprintf(numval, " (%d)", options_mask);
    strcat (opt_mask_strg, numval);

    return(ret_value);
}   /* option_mask_string */
 /* #endif
 */ 

/*
 * Prints compression method and compression information of a data set.
 * BMR - bugzilla 1202 - Jul, 2008
 */
intn print_comp_info(
	FILE *fp,
	int32 sds_id,
	comp_coder_t *comp_type)
{
   comp_info c_info;                /* Compression structure */
   int32 comp_size=0, orig_size=0;  /* compressed and original sizes */
   intn status = FAIL;	    /* returned status from a called function */

   /* get compression info */
   HDmemset(&c_info, 0, sizeof(c_info));
   status = SDgetcompinfo(sds_id, comp_type, &c_info);

   /* if getting comp info succeeds, proceed to print out appropriate 
      compression information */
   if (status != FAIL)
   {
      /* print compression method or "NONE" */
      fprintf(fp, "\t Compression method = %s\n", comp_method_txt(*comp_type));

      switch (*comp_type)
      {
        case COMP_CODE_SKPHUFF:
	    fprintf(fp, "\t\t Skipping unit size = %d\n", c_info.skphuff.skp_size);
	    break;
	case COMP_CODE_DEFLATE:
	    fprintf(fp, "\t\t Deflate level = %d\n", c_info.deflate.level);
	    break;
	case COMP_CODE_SZIP:
	{
	    char mask_strg[160]; /* 160 is to cover all options and number val*/
	    if (option_mask_string(c_info.szip.options_mask, mask_strg) != FAIL)
		fprintf(fp, "\t\t Option mask = %s\n", mask_strg);
	    else
		fprintf(fp, "\t\t Option mask might be invalid = %d\n", (int)c_info.szip.options_mask);
	    fprintf(fp, "\t\t Pixels per block = %d\n", (int)c_info.szip.pixels_per_block);
	    fprintf(fp, "\t\t Pixels per scanline = %d\n", (int)c_info.szip.pixels_per_scanline);
	    fprintf(fp, "\t\t Bits per pixel = %d\n", (int)c_info.szip.bits_per_pixel);
	    fprintf(fp, "\t\t Pixels = %d\n", (int)c_info.szip.pixels);
	    break;
	}
        default:
	    /* nothing */
	    break;
      } /* switch */
   }
   else
      fprintf(fp, "\t Compression method = <Unable to get compression method>\n");

   /* print compression ratio */
   if (*comp_type != COMP_CODE_NONE)
   {
   status = SDgetdatasize(sds_id, &comp_size, &orig_size);
   if (status != FAIL)
   {
      /* calculate and print compression ratio */
      if (comp_size > 0 && orig_size > 0)
      {
         double orig=orig_size, comp=comp_size, ratio = 0.00;
	 ratio = orig/comp;
	 fprintf(fp, "\t Compression ratio (original:compressed) = %.2f:1\n", ratio);
      }
      else if (comp_size == 0 && orig_size == 0)
      {
	 fprintf(fp, "\t Compression ratio = <No data written yet!\n");
      }
      else  /* any size < 0 means something wrong */
	 fprintf(fp, "\t Compression ratio = <Unable to get data sizes\n");
   }
   else
      fprintf(fp, "\t Compression ratio = <Unable to determine compression ratio>\n");
   }
   return(status);
} /* print_comp_info */


/* printSDS_ASCII prints all of the requested SDSs in the file */
intn printSDS_ASCII( 
	int32 sd_id,
	dump_info_t *dumpsds_opts,
	int32 sds_index,    /* index of the SDS */
	FILE *fp )
{
   int32 sds_id = FAIL, /* SDS id, always reset to FAIL when not used */
         sds_ref,       /* ref# of an SDS */
         dim_id = FAIL, /* id of an SDS dimension */
         rank,          /* number of dimensions of an SDS */
         nt,            /* number type of an SDS */
         nattrs,        /* # of attributes assigned to an SDS */
         dimsizes[MAXRANK],     /* SDS dimensions */
         dimNT[MAXRANK],        /* number type of dimension */
         dimnattr[MAXRANK];     /* # of attributes of a dim */
   char  dim_nm[MAXNAMELEN],    /* dimension name */
         *sdsname = NULL,	/* SDS name */
         *nt_desc = NULL,       /* SDS's or dim's num type description */
         *attr_nt_desc = NULL,  /* attr's nt description */
         curr_file_name[MAXFNLEN]; /* curr hdf file name */
   uint16 name_len=0;
   comp_coder_t comp_type = COMP_CODE_NONE;
   intn  isdimvar,      /* TRUE if curr SDS is used for a dim */
         j,
         status = FAIL,		/* status returned from a routine */
         ret_value = SUCCEED;	/* returned value of printSDS_ASCII */

      /* Reset variables. */
      HDmemset(dimsizes, 0, sizeof(int32) * MAXRANK);
      HDmemset(dimNT, 0, sizeof(int32) * MAXRANK);
      HDmemset(dimnattr, 0, sizeof(int32) * MAXRANK);

      /* get access to the current dataset */
      sds_id = SDselect(sd_id, sds_index);
      if (sds_id == FAIL)
         ERROR_GOTO_3( "in %s: %s failed for %d'th SDS",
                      "printSDS_ASCII", "SDselect", (int)sds_index );

      status = SDgetnamelen(sds_id, &name_len);
      if( FAIL == status )
      {
         ERROR_GOTO_3( "in %s: %s failed for %d'th SDS",
			"printSDS_ASCII", "SDgetnamelen", (int)sds_index );
      }

      /* allocate space for sds name */
      sdsname = (char *) HDmalloc(name_len+1);
      CHECK_ALLOC( sdsname, "sdsname", "printSDS_ASCII" );

      /* get dataset's information */
      status = SDgetinfo(sds_id, sdsname, &rank, dimsizes, &nt, &nattrs);
      if( status == FAIL )
      {
         ERROR_GOTO_3( "in %s: %s failed for %d'th SDS", 
                       "printSDS_ASCII", "SDgetinfo", (int)sds_index );
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
            ERROR_GOTO_3( "in %s: %s failed for %d'th SDS", 
                       "printSDS_ASCII", "SDgetdimid", (int)sds_index );
         }

         /* get information of current dimension */
         if( SDdiminfo(dim_id, NULL, &size, &nt, &num_attrs) == FAIL )
         {
            ERROR_GOTO_3( "in %s: %s failed for %d'th SDS", 
                       "printSDS_ASCII", "SDdiminfo", (int)sds_index );
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
               ERROR_BREAK_3( "in %s: %s failed for %d'th SDS", 
			"printSDS_ASCII", "HDgetNTdesc", (int)sds_index, FAIL );
               /* did this one fail on allocation and need exit(1)? */
            }

            /* Note: a variable can be used to hold an SDS or a dimension. */
            /* display dimension info if the variable is used to contain a
               dimension */
            if (isdimvar)
            {
               fprintf(fp, "\nDimension Variable Name = %s\n\t ", sdsname);
               fprintf(fp, "Index = %d\n\t Scale Type= %s\n", 
						(int)sds_index, nt_desc);
            }
            /* display the SDS info, otherwise */
            else
            {
               fprintf(fp, "\nVariable Name = %s\n\t Index = ", sdsname);
               fprintf(fp, "%d\n\t Type= %s\n", (int)sds_index, nt_desc);
            }

            resetBuff((VOIDP) &nt_desc );  /* done with nt_desc */

	    /* If the current file is not a netCDF, print the SDS' ref#
	       and compression information */
	    if(dumpsds_opts->file_type == HDF_FILE)
	    {
		/* get SDS's ref# from its id */
	  	sds_ref = SDidtoref(sds_id);
		if (sds_ref == FAIL)
		    ERROR_BREAK_3( "in %s: %s failed for %d'th SDS", 
			"printSDS_ASCII", "SDidtoref", (int)sds_index, FAIL );
		fprintf(fp, "\t Ref. = %d\n", (int) sds_ref);

                /* print compression method and info or "NONE" */
		status = print_comp_info(fp, sds_id, &comp_type);
		if (status == FAIL)
		{
                    ERROR_GOTO_3( "in %s: %s failed for %d'th SDS",
                       "printSDS_ASCII", "print_comp_info", (int)sds_index );
                }
            }
            fprintf(fp, "\t Rank = %d\n\t Number of attributes = %d\n",
                                        (int) rank, (int) nattrs);

            /* print each dimension of the current SDS */
            for (j = 0; j < rank; j++)
            {
               int32 size;    /* size of the current dimension */

               /* get current dimension id for access */
               if (FAIL == (dim_id = SDgetdimid(sds_id, j)))
                  ERROR_BREAK_3( "in %s: %s failed for %d'th SDS", 
			"printSDS_ASCII", "SDgetdimid", (int)sds_index, FAIL );

               /* get information of current dimension */
               ret_value = SDdiminfo(dim_id,dim_nm,&size,&(dimNT[j]),&(dimnattr[j]));
               if (FAIL == ret_value )
                  ERROR_BREAK_4( "in %s: %s failed for %d'th dimension of %d'th SDS", 
			"printSDS_ASCII", "SDdiminfo", j, (int)sds_index, FAIL );

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
                     ERROR_BREAK_4( "in %s: %s failed for %d'th dimension of %d'th SDS", 
		     "printSDS_ASCII", "HDgetNTdesc", j, (int)sds_index, FAIL);

                  fprintf(fp, "\t\t Scale Type = %s\n", attr_nt_desc);
                  fprintf(fp, "\t\t Number of attributes = %d\n", (int) dimnattr[j]);
                  resetBuff((VOIDP) &attr_nt_desc);
               }
            } /* end each for dimension */

            /* print dataset's attributes */
            status = print_SDSattrs(sds_id, nattrs, fp, dumpsds_opts);
            if( status == FAIL )
               ERROR_BREAK_3( "in %s: %s failed for %d'th SDS",
		"printSDS_ASCII", "print_SDSattrs", (int)sds_index, FAIL );

            /* header only, don't go into case DDATA */
            if (dumpsds_opts->contents == DHEADER)
               break;

            /* case DDATA doesn't need this */
            fprintf(fp, "\t Data : \n");

         case DDATA:
	    if (comp_type == COMP_CODE_SZIP && have_szip == 0)
	    {
		fprintf(fp, "\t\t <SZIP library is not available>\n");
		fprintf(fp, "\t\t <Unable to read SZIP compressed data>\n");
	    }
	    else {
            if (rank > 0 && dimsizes[0] != 0)
            {
               if (!isdimvar || nt != 0)
               { /* no dump if dimvar w/o scale values */
                  status = sdsdumpfull( sds_id, dumpsds_opts, rank, dimsizes, nt, fp);
                  if( FAIL == status )
                     ERROR_BREAK_3( "in %s: %s failed for %d'th SDS", 
			"printSDS_ASCII", "sdsdumpfull", (int)sds_index, FAIL );
               }
            } }
            break;
         default:
            printf("Output format must be either -d, -h, or -v only.\n");
      }	 /* switch  */

      resetSDS( &sds_id, sds_index, curr_file_name ); /* end access to current SDS */
      SAFE_FREE(sdsname);

done:
    if (ret_value == FAIL)
      { /* Failure cleanup */
        if (sds_id != FAIL)
            SDendaccess(sds_id);
	SAFE_FREE(sdsname);
      }
    /* Normal cleanup */
    return ret_value;
} /* end of printSDS_ASCII() */

intn printSDS_BINARY(
	int32 sd_id,
	dump_info_t *dumpsds_opts,
	int32 sds_index,    /* index of the SDS */
	FILE *fp )
{
   int32 sds_id=FAIL,
         dimsizes[MAXRANK],
         rank,
         nt,
         nattrs;
   char  curr_file_name[MAXFNLEN];
   comp_coder_t comp_type=COMP_CODE_NONE;
   intn  status=FAIL,
         ret_value=SUCCEED;
   
    /* temp. names for file type and curr input file name for ease of use */
    HDstrcpy( curr_file_name, dumpsds_opts->ifile_name );

    /* Reset variable */
    HDmemset(dimsizes, 0, sizeof(int32) * MAXRANK);

    sds_id = SDselect(sd_id, sds_index);
    if (sds_id == FAIL)
         ERROR_GOTO_3( "in %s: %s failed for %d'th SDS",
			"printSDS_BINARY", "SDselect", (int)sds_index );

    status = SDgetinfo(sds_id, NULL, &rank, dimsizes, &nt, &nattrs);
    if( FAIL == status )
    {
	resetSDS( &sds_id, sds_index, curr_file_name );
	ERROR_GOTO_3( "in %s: %s failed for %d'th SDS",
			"printSDS_BINARY", "SDgetinfo", (int)sds_index );
    }

    /* get compression method and if szipped compressed data is being read,
       make sure that szip library is available before reading */
    status = SDgetcomptype(sds_id, &comp_type);
    if (comp_type == COMP_CODE_SZIP && have_szip == 0)
    {
	fprintf(fp, "\t\t <SZIP library is not available>\n");
	fprintf(fp, "\t\t <Unable to read SZIP compressed data>\n");
    }
    else { /* can output data to binary file   */
	if (rank > 0 && dimsizes[0] != 0)
	{
	    status = sdsdumpfull(sds_id, dumpsds_opts, rank, dimsizes, nt, fp);
            if( FAIL == status )
               ERROR_GOTO_3( "in %s: %s failed for %d'th SDS", 
			"printSDS_BINARY", "sdsdumpfull", (int)sds_index );
	}
    } /* can output data */
    resetSDS( &sds_id, sds_index, curr_file_name );

done:
    if (ret_value == FAIL)
      { /* Failure cleanup */
          if (sds_id != FAIL)
              SDendaccess(sds_id);
      }
    /* Normal cleanup */

    return ret_value;
} /* end of printSDS_BINARY */

intn dsd(dump_info_t *dumpsds_opts, 
    intn         curr_arg, 
    intn         argc, 
    char        *argv[])
{
   int32  sd_id=FAIL,
         *sds_chosen=NULL,
          num_sds_chosen,
          ndsets,
          n_file_attrs;
   char   file_name[MAXFNLEN];
   FILE  *fp=NULL;
   file_format_t ff = dumpsds_opts->file_format;
   intn   index_error=0,
	  ii,
          status=FAIL,
          ret_value=SUCCEED;

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
      HDstrcpy(file_name, argv[curr_arg]); /* get current file name */
      HDstrcpy( dumpsds_opts->ifile_name, file_name ); /* record file name */
      curr_arg++; /* move argument pointer forward */

      /* HDF4 doesn't process netCDF 64-bit files */
      if (HDisnetcdf64(file_name))
      {
         printf("Invalid input file: hdp cannot read a netCDF 64-bit file, %s\n", file_name );
         continue; /* to the next file */
      }

      if (HDisnetcdf(file_name)) /* record if file is netCDF */
         dumpsds_opts->file_type = netCDF_FILE;
      else if (Hishdf(file_name)) /* record if file is HDF */
         dumpsds_opts->file_type = HDF_FILE;
      else
      {
         /* if there are no more files to be processed, print informative
            message, then returns with FAIL */
	 if( curr_arg == argc )
	    {ERROR_GOTO_1( "in dsd: %s is not an HDF or netCDF file", file_name);}
         else /* print message, then continue processing the next file */
            {ERROR_CONT_1( "in dsd: %s is not an HDF or netCDF file", file_name);}
      }
	 
      /* open current hdf file with error check, if fail, go to next file */
      sd_id = SDstart(file_name, DFACC_RDONLY);
      if (sd_id == FAIL)
      {
	 /* if there are no more files to be processed, print error 
	    message, then returns with FAIL */
	 if( curr_arg == argc )
            {ERROR_GOTO_1( "in dsd: Failure in opening file %s", file_name);}
	 else /* print message, then continue processing the next file */
            ERROR_CONT_1( "in dsd: Failure in opening file %s", file_name );
      }

      /* compose the list of indices of SDSs to be processed in the current 
         file: sds_chosen is the list and return value is the number of items
	 in the list */
      num_sds_chosen = get_SDSindex_list( sd_id, dumpsds_opts, &sds_chosen, &index_error );

      /* if there are errors with the given indices, ref#s, or names of the
         requested datasets, and so the input yields no valid datasets,
         then close the interface and the input file, and move on to the
         next file */
      if (index_error && num_sds_chosen == 0)
      {
         SAFE_FREE(sds_chosen)
         if( FAIL == SDend(sd_id))
            fprintf( stderr, "in dsd: SDend failed in closing file %s\n",
			file_name );
         continue; /* to the next file */
      } /* end if */

      /* obtain number of datasets in the file and number of file
	attributes, ndsets will be used to process the datasets,
	n_file_attrs will be used to print file attributes */
      status = SDfileinfo(sd_id, &ndsets, &n_file_attrs);
      if (status == FAIL)
	  ERROR_GOTO_2( "in dsd: %s failed for file %s", "SDfileinfo", file_name);

      fp = stdout;                /* assume no output file given */

      /* ASCII or binary dump? */
      switch( ff )
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
               status = print_SDattrs( sd_id, fp, n_file_attrs, dumpsds_opts );
               if( status == FAIL )
                  ERROR_CONT_2( "in dsd: %s failed for file %s", file_name, "print_SDattrs" );
            }

	    /* If the user requests to dump the SDSs in the order they were
	       given, -k given */
	    if (dumpsds_opts->keep_order)
	    {
		for (ii = 0; ii < num_sds_chosen; ii++)
		{
		    /* Print the current SDS in ASCII format */
		    status = printSDS_ASCII(sd_id, dumpsds_opts, sds_chosen[ii], fp);
		    if( status == FAIL )
			fprintf( stderr, "HDP ERROR>>> in dsd: %s failed for file %s\n", 
				"printSDS_ASCII", file_name );
		}
	    } /* if: -k given */

	    else /* no -k */
	    {
		int32 sds_count, sds_index;
		intn dumpall = FALSE;

		/* if only specific datasets were requested, sort the list
		   to dump them in index order */
		if (num_sds_chosen == NO_SPECIFIC )
		    dumpall = TRUE;
		else
		    sort(sds_chosen, num_sds_chosen);

		sds_count = 0;   /* no SDS has been processed yet */
		/* For each index, if the user requests to dump all SDSs or if
		   there are more requested SDSs, process the curr SDS */
		for (sds_index = 0; sds_index < ndsets  /* validate index */
		     && (dumpall      /* either all datasets are dumped or */
		     || sds_count < num_sds_chosen);/* or more requested SDSs */
		     sds_index++)
		{
		/* If the user neither requests dump all nor the current SDS */
		if ((!dumpall) && (sds_index != sds_chosen[sds_count]))
		    continue; /* skip */

		/* Count the # of datasets being processed */
		sds_count++;

		/* Print the current SDS in ASCII format */
		status = printSDS_ASCII(sd_id, dumpsds_opts, sds_index, fp);
		if( status == FAIL )
		    fprintf(stderr,
				"HDP ERROR>>> in dsd: %s failed for file %s\n", 
				"printSDS_ASCII", file_name );
		}
	    } /* else: no -k */

            /* Close output file only if option -o is given */
            if (dumpsds_opts->dump_to_file)
               fclose(fp);                       
            break;

         case DBINARY:       /* binary file */
             /* Get output file name  */
             if (dumpsds_opts->dump_to_file)
                 fp = fopen(dumpsds_opts->file_name, "wb");

	    /* If the user requests to dump the SDSs in the order they were
	       given, -k given */
	    if (dumpsds_opts->keep_order)
	    {
		for (ii = 0; ii < num_sds_chosen; ii++)
		{
		    /* Print the current SDS in BINARY format */
		    status = printSDS_BINARY(sd_id, dumpsds_opts, sds_chosen[ii], fp);
		    if( status == FAIL )
			fprintf( stderr, "HDP ERROR>>> in dsd: %s failed for file %s\n", 
				"printSDS_BINARY", file_name );
		}
	    } /* if: -k given */

	    else /* no -k */
	    {
		int32 sds_count, sds_index;
		intn dumpall = FALSE;

		/* if only specific datasets were requested, sort the list
		   to dump them in index order */
		if (num_sds_chosen == NO_SPECIFIC )
		    dumpall = TRUE;
		else
		    sort(sds_chosen, num_sds_chosen);

		sds_count = 0;   /* no SDS has been processed yet */
		/* For each index, if the user requests to dump all SDSs or if
		   there are more requested SDSs, process the curr SDS */
		for (sds_index = 0; sds_index < ndsets  /* validate index */
		     && (dumpall      /* either all datasets are dumped or */
		     || sds_count < num_sds_chosen);/* or more requested SDSs */
		     sds_index++)
		{
		/* If the user neither requests dump all nor the current SDS */
		if ((!dumpall) && (sds_index != sds_chosen[sds_count]))
		    continue; /* skip */

		/* Count the # of datasets being processed */
		sds_count++;

		/* Print the current SDS in BINARY format */
		status = printSDS_BINARY(sd_id, dumpsds_opts, sds_index, fp);
		if( status == FAIL )
		    fprintf(stderr,
				"HDP ERROR>>> in dsd: %s failed for file %s\n", 
				"printSDS_BINARY", file_name );
		}
	    } /* else: no -k */

            /* Close output file only if option -o is given */
            if (dumpsds_opts->dump_to_file)
               fclose(fp);                       
            break;

         default:
            printf("Output file type must be either ascii or binary only\n");
      } /* switch for output file */

      SAFE_FREE(sds_chosen)

      if (FAIL == SDend(sd_id))
         ERROR_CONT_1( "in dsd: SDend failed for file %s", file_name );
      sd_id = FAIL; /* reset */

   }  /* while processing files  */

done:
    if (ret_value == FAIL)
      { /* Failure cleanup */
          if (sd_id != FAIL)
              SDend(sd_id);

          if (fp != NULL && fp != stdout)
              fclose(fp);                       

	  SAFE_FREE(sds_chosen);
      }
    /* Normal cleanup */
    return ret_value;
}	/* dsd */

/* Exported */
intn do_dumpsds(intn  curr_arg, 
           intn  argc, 
           char *argv[], 
           intn  help )
{
    dump_info_t dumpsds_opts;	/* dumpsds options */
    intn status=FAIL, ret_value=SUCCEED;

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
      ERROR_GOTO_0( "in do_dumpsds: command is incomplete");
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
      ERROR_GOTO_0( "in do_dumpsds: dsd failed" );

  done:
    if (ret_value == FAIL)
      { /* Failure cleanup */
      }
    /* Normal cleanup */

    /* free the list of structs containg info of selected SDSs */
    free_obj_chosen_t_list(&dumpsds_opts.all_types, dumpsds_opts.num_chosen);

   return ret_value;
}	/* end do_dumpsds() */


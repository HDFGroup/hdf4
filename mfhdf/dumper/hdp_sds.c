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
    printf("\tfilelist\tNames of hdf files, separated by a space\n");
}	/* end list_usage() */

intn 
parse_dumpsds_opts(dump_info_t *dumpsds_opts, 
                   intn *curr_arg, 
                   intn argc,
                   char *argv[])
{
   /* traverse the command and process each option */
   while ((*curr_arg < argc) && (argv[*curr_arg][0] == '-'))
   {
      switch (argv[*curr_arg][1])
      {
         case 'a':	/* dump all, default */
            dumpsds_opts->filter = DALL;
            dumpsds_opts->num_chosen = -1; /* indicate all are dumped */
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
                dumpsds_usage( argc, argv );
                return (FAIL);
            }	/* end switch */
      }		/* end while */

   /* add the number of images requested by index, by ref#, and by name
      to have a total number of requested images */
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
      if( ft == DASCII )
         fprintf( fp, "Unable to obtain data.\n" );
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

/* get_SDSindex_list returns the number of items in sds_chosen, i.e.,
SDSs to be processed.  If there are any failures occur in this routine,
index_error will be incremented to indicate that */
intn
get_SDSindex_list( 
        int32 sdf_id, 
        dump_info_t *dumpsds_opts,
        int32 *sds_chosen,/* array of indices of SDSs to be processed */
        intn *index_error )
{
   int32 index;
   intn i, sds_count;  /* number of SDSs to be processed */
   filter_t filter = dumpsds_opts->filter; /* temporary name */

   sds_count = 0;   /* no image requested yet */

   /* if no specific images are requested, set image count to -1 to
      indicate that all images are to be dumped */
   if( filter == DALL )
   {
      sds_count= -1;
      return( sds_count );
   }

   /* if there are some SDSs requested by index, store the indices in
      the array provided by the caller, sds_chosen */
   if( filter & DINDEX )
         /* Note: Don't replace this with HDmemcpy unless you change 
                  the sizes of the objects correctly -QAK */
      for (i = 0; i < dumpsds_opts->by_index.num_items; i++)
      {
         sds_chosen[i] = dumpsds_opts->by_index.num_list[i];
         (sds_count)++;
      }

   /* if there are some SDSs requested by ref#, convert the ref#s to indices
      and store the indices in the array provided by the caller, sds_chosen */
   if( filter & DREFNUM )
      for (i = 0; i < dumpsds_opts->by_ref.num_items; i++)
      {
         index = SDreftoindex(sdf_id, dumpsds_opts->by_ref.num_list[i]);
         if (index == FAIL)
         {
            printf("SDS with reference number %d: not found\n", dumpsds_opts->by_ref.num_list[i]);
            *index_error = 1; /* error */
         }
         else
         {
            sds_chosen[sds_count] = index;
            (sds_count)++;
         }
      }

   /* if there are some SDSs requested by name, convert the names to indices 
      and store the indices in the array provided by the caller, sds_chosen */
   if( filter & DNAME )
      for (i = 0; i < dumpsds_opts->by_name.num_items; i++)
      {
         index = SDnametoindex(sdf_id, dumpsds_opts->by_name.str_list[i]);
         if (index == FAIL)
         {
            printf("SD with name %s: not found\n", dumpsds_opts->by_name.str_list[i]);
            *index_error = 1; /* error */
         }
         else
         {
            sds_chosen[sds_count] = index;
            (sds_count)++;
         }
      }

   if( filter & DCLASS )
      printf("Currently, no class defined on an SD.\n");

   return( sds_count );

} /* end of get_SDSindex_list */

/* displays all SD file attributes */
intn
print_SDattrs( int32 sdf_id,
               FILE *fp,
               int32 n_file_attrs)
{
   int32 attr_index,
         attr_count,
         attr_nt,
         attr_buf_size;
   char  attr_name[MAXNAMELEN],
        *attr_nt_desc = NULL;
   VOIDP attr_buf=NULL;
   intn  printed = FALSE; /* indicate whether the file attr tittle line 
                             is printed */
   intn  i, ret, ret_value = SUCCEED;

   for (i = 0; i < n_file_attrs; i++)
   {
      if (SDattrinfo(sdf_id, i, attr_name, &attr_nt, &attr_count) != FAIL )
      {
         attr_nt_desc = HDgetNTdesc(attr_nt);
         if (attr_nt_desc == NULL)
         {
            fprintf(stderr,"Failure in HDgetNTdesc for %d'th attribute\n",i );
            continue; /* to the next attribute */
         }
         attr_index = SDfindattr(sdf_id, attr_name);
         if (attr_index == FAIL)
         {
            fprintf(stderr,"Failure in SDfindattr for %d'th attribute\n", i );
            continue; /* to the next attribute */
         }
         attr_buf_size = DFKNTsize(attr_nt) * attr_count;
         if ((attr_buf = (VOIDP) HDmalloc(attr_buf_size)) == NULL)
         {
            fprintf(stderr,"Failure to allocate space \n");
            exit(1); 
         }
         if (FAIL == SDreadattr(sdf_id, attr_index, attr_buf))
         {
            fprintf(stderr,"Failure in SDreadattr for %d'th attribute\n", i );
            continue; /* to the next attribute */
         }
         if( !printed )
         {
            fprintf( fp, "\nFile attributes:\n" );
            printed = TRUE;
         }
         fprintf(fp,"\t Attr%i: Name = %s\n", (int) attr_index, attr_name);
         fprintf(fp,"\t\tType = %s \n\t\tCount= %i\n", attr_nt_desc, (int) attr_count);
         fprintf(fp,"\t\tValue = ");

         /* Note that filetype is DASCII since binary does not have
            these information - it's data only */
         if (FAIL == dumpfull(attr_nt, DASCII, attr_count, attr_buf, 20, fp))
         {
            fprintf(stderr,"Failure from dumpfull for %d'th attribute\n", i );
            continue; /* to the next attribute */
         }
         printf("\n");

         /* clean up */
         HDfree(attr_nt_desc);
         attr_nt_desc = NULL;
         HDfree((VOIDP) attr_buf);
         attr_buf = NULL;
      }  /* end if SDattrinfo */

      else /* SDattrinfo failed */
      {
         fprintf(stderr, "Failure in SDattrinfo for %d'th attribute\n", i );
         /* go to the next attribute */
      }

   }/* for each file attribute */

  done:
    if (ret_value == FAIL)
      { /* Failure cleanup */
        /*  if (desc != NULL)
              HDfree(desc);
         */ if (attr_nt_desc != NULL)
              HDfree(attr_nt_desc);
          if (attr_buf != NULL)
              HDfree((VOIDP) attr_buf);
      }
    /* Normal cleanup */

    return ret_value;
}   /* end of print_SDattrs */

intn
print_SDSattrs( int32 sds_id,
                int32 sds_index,
                int32 nattrs,
                FILE *fp,
                dump_info_t *dumpsds_opts )
{
   int32 attr_index,
         attr_count,
         attr_nt,
         attr_buf_size;
   char  attr_name[MAXNAMELEN],
         curr_file_name[MAXFNLEN],
        *attr_nt_desc = NULL;
   VOIDP attr_buf=NULL;
   intn  ret, ret_value = SUCCEED;

   /* temporary name for current input file */
   HDstrcpy( curr_file_name, dumpsds_opts->ifile_name );

   for (attr_index = 0; attr_index < nattrs; attr_index++)
   {
      ret = SDattrinfo(sds_id, attr_index, attr_name, &attr_nt, &attr_count);
      if (ret == FAIL)
      {
         fprintf(stderr,"SDattrinfo failed for %d'th attribute of sds_id(%d) in file %s\n", attr_index,(int)sds_id, curr_file_name);
         continue; /* to the next attribute */
      }

      attr_nt_desc = HDgetNTdesc(attr_nt);
      if (attr_nt_desc == NULL)
      {
         fprintf(stderr,"HDgetNTdesc failed for %d'th attribute of sds_id(%d) in file %s\n", attr_index,(int)sds_id, curr_file_name);
         continue; /* to the next attribute */
      }

      attr_buf_size = DFKNTsize(attr_nt|DFNT_NATIVE) * attr_count;

      attr_buf = (VOIDP) HDmalloc(attr_buf_size);
      if (attr_buf == NULL)
      {
         fprintf(stderr,"Not enough memory!\n");
         exit(1);
      }     

      /* read the values of the attribute into the buffer */
      ret = SDreadattr(sds_id, attr_index, attr_buf);
      if (ret == FAIL)
      {
         fprintf(stderr,"SDreadattr failed for %d'th attribute ofr sds_id(%d) in file %s\n", attr_index,(int)sds_id, curr_file_name);
         continue; /* to the next attribute */
      }

      /* display the attribute's information then free the buffer */
      fprintf(fp, "\t Attr%d: Name = %s\n", (int) attr_index, attr_name);
      fprintf(fp, "\t\t Type = %s \n\t\t Count= %d\n", attr_nt_desc, (int) attr_count);

      HDfree(attr_nt_desc);
      attr_nt_desc = NULL; /* reset */

      fprintf(fp, "\t\t Value = ");

      if (FAIL == dumpfull(attr_nt, dumpsds_opts->file_type, attr_count, attr_buf, 20, fp))
      {
         fprintf(stderr,"dumpfull failed for %d'th attribute of sds_id(%d) in file %s\n", attr_index, (int)sds_id, curr_file_name);
         continue; /* to the next attribute */
      }
      HDfree((VOIDP) attr_buf);
      attr_buf = NULL; /* reset */
   } /* for each attribute */
done:
    if (ret_value == FAIL)
      { /* Failure cleanup */
          if (attr_nt_desc != NULL)
              HDfree(attr_nt_desc);
          if (attr_buf != NULL)
              HDfree(attr_buf);
      }
    /* Normal cleanup */

    return ret_value;
} /* end of print_SDSattrs */

static intn
printSD_ASCII( int32 sdf_id,
            FILE *fp,
            dump_info_t *dumpsds_opts,
            int32 *sds_chosen,
            int32 num_sds_chosen,
            int32 ndsets )
{
   int32 sds_id,
         sds_ref,
         sds_count,
         sds_index,
         dim_id = FAIL;
   intn  ret_value = SUCCEED;
   intn  dumpall = FALSE;
   char  dim_nm[MAXNAMELEN];
    char        name[MAXNAMELEN];
    char        attr_name[MAXNAMELEN];
    int32       dimsizes[MAXRANK];
    int32       dimNT[MAXRANK];
    int32       dimnattr[MAXRANK];
    char       *nt_desc = NULL;
    char       *attr_nt_desc = NULL;
    int32       rank;
    int32       nt;
    int32       nattrs;
    int32       n_file_attrs;
    intn        isdimvar;
    intn        j, k;
    char        curr_file_name[MAXFNLEN];
   int ret;  /* returned status of called routines */

   /* temp. names for file type and curr input file name for ease of use */
   HDstrcpy( curr_file_name, dumpsds_opts->ifile_name );

   /* when there are no datasets specified, dumper dumps all datasets */
   if (num_sds_chosen == (-1))
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

      sds_count++;   /* count the # of images have been processed */

      /* Reset variables. */
      HDmemset(dimsizes, 0, sizeof(int32) * MAXRANK);
      HDmemset(dimNT, 0, sizeof(int32) * MAXRANK);
      HDmemset(dimnattr, 0, sizeof(int32) * MAXRANK);

      sds_id = SDselect(sdf_id, sds_index);
      if (sds_id == FAIL)
      {
         fprintf(stderr,"Failure in selecting %d'th SDS in file %s\n",
                                    sds_index, curr_file_name);
         continue; /* to the next dataset */
      }

      if (FAIL == SDgetinfo(sds_id, name, &rank, dimsizes, &nt, &nattrs))
      {
         fprintf(stderr,"SDgetinfo failed for sds_id(%d) in file %s\n", 
                                    (int)sds_id, curr_file_name);
         continue; /* to next dimension */
      }

      isdimvar = (SDiscoordvar(sds_id)) ? 1 : 0;
      if (isdimvar) 
      { /* use dim0 nt instead of dimvar nt, because when no dim values 
           dimvar nt was set to float32 by default */
         int32 temp, temp2;
         dim_id = SDgetdimid(sds_id, 0);
         if( dim_id == FAIL )
         {
            fprintf(stderr,"SDdiminfo failed for sds_id(%d) in file %s\n",
                                    (int)sds_id, curr_file_name);
            continue; /* to next dimension */
         }

         ret = SDdiminfo(dim_id, NULL, &temp, &nt, &temp2);
         if( ret == FAIL )
         {
            fprintf(stderr,"SDdiminfo failed for sds_id(%d) in file %s\n",
                                    (int)sds_id, curr_file_name);
            continue; /* to next dataset */
         }
      }                                                       
      /* dump contents based on options */
      switch (dumpsds_opts->contents)
      {
         case DVERBOSE:
         case DHEADER:
            nt_desc = HDgetNTdesc(nt);
            if (nt_desc == NULL)
            {
               fprintf(stderr,"Failure in HDgetNTdesc for sds_id(%d) in file %s\n", (int)sds_id,curr_file_name);
               /* is this one allocation and needs exit(1)? */
               exit(1);
            }

            if (isdimvar)
            {
               fprintf(fp, "\nDimension Variable Name = %s\n\t ", name);
               fprintf(fp, "Index = %d\n\t Scale Type= %s\n", sds_index, nt_desc);
            }
            else
            {
               fprintf(fp, "\nVariable Name = %s\n\t Index = ", name);
               fprintf(fp, "%d\n\t Type= %s\n", sds_index, nt_desc);
            }

            HDfree(nt_desc);
            nt_desc = NULL;

            if ((sds_ref = SDidtoref(sds_id)) == FAIL)
            {
               fprintf(stderr,"Failure in determining reference for sds_id(%d) in file %s.\n", (int)sds_id, curr_file_name);
               continue; /* to the next dataset */
            }

            fprintf(fp, "\t Ref. = %d\n", (int) sds_ref);
            fprintf(fp, "\t Rank = %d\n\t Number of attributes = %d\n", (int) rank, (int) nattrs);

            /* for each dimension */
            for (j = 0; j < rank; j++)
            {
               int32       temp;

               if (FAIL == (dim_id = SDgetdimid(sds_id, j)))
               {
                  fprintf(stderr,"SDgetdimid failed for sds_id(%d) in file %s\n", (int)sds_id, curr_file_name);
                  continue; /* to the next dimension */
               }

               if (FAIL == SDdiminfo(dim_id, dim_nm, &temp, &(dimNT[j]), &(dimnattr[j])))
               {
                  fprintf(stderr,"SDdiminfo failed for dim_id(%d) of sds_id(%d) in file %s\n", (int)dim_id,(int)sds_id,curr_file_name);
                  continue; /* to the next dimension */
               }

               fprintf(fp, "\t Dim%d: Name=%s\n", (int) j, dim_nm);
               if (temp == 0)
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
                     fprintf(stderr,"HDgetNTdesc failed for dim_id(%d) of sds_id(%d) in file %s\n", (int)dim_id,(int)sds_id,curr_file_name);
                     continue; /* to the next dimension */
                  }
                  fprintf(fp, "\t\t Scale Type = %s\n", attr_nt_desc);

                  HDfree(attr_nt_desc);
                  attr_nt_desc = NULL; /* reset */

                  fprintf(fp, "\t\t Number of attributes = %d\n", (int) dimnattr[j]);
               }
            } /* end each for dimension */

            /* print dataset's attributes */
            ret = print_SDSattrs( sds_id, sds_index, nattrs, fp, dumpsds_opts );
            if( ret == FAIL )
               break; /* so SDendaccess can be called, then go to next SDS */

            if (dumpsds_opts->contents == DHEADER)
               break;
            fprintf(fp, "\t Data : \n");

         case DDATA:
            if (dumpsds_opts->contents == DDATA)
            {
               /* print rank, dimsizes, nt, maxmin, cal info */
            }
            if (rank > 0 && dimsizes[0] != 0)
            {
               if (!isdimvar || nt != 0)
               { /* no dump if dimvar w/o scale values */

                  intn        count;

                  fprintf(fp, "                ");/* spaces to line up output */

                  if( FAIL == sdsdumpfull( sds_id, dumpsds_opts, rank, dimsizes, nt, fp))
                  {
                    /*fprintf(stderr,"sdsdumpfull failed for sds_id(%d) in file %s\n", 
                                 (int)sds_id, curr_file_name);
*/
                     fprintf( fp, "Unable to dump data\n");
                  }
               }
            }
            break;
         default:
            printf("Output format must be either -d, -h, or -v only.\n");
      }	 /* switch  */

      if (FAIL == SDendaccess(sds_id))
         fprintf(stderr,"SDendaccess failed for sds_id(%d) in file %s\n", 
                                    (int)sds_id, curr_file_name);
      sds_id = FAIL;/* reset */

   }	/* for ndsets  */

   return( ret_value );
} /* end of printSD_ASCII() */

intn
printSD_BINARY( int32 sdf_id,
             FILE *fp,
             dump_info_t *dumpsds_opts,
             int32 *sds_chosen,
             int32 num_sds_chosen,
             int32 ndsets )
{
   int32 sds_id,
         sds_index,
         sds_count;
   intn  dumpall = FALSE;
   char  dim_nm[MAXNAMELEN];
    char        name[MAXNAMELEN];
    char        attr_name[MAXNAMELEN];
    int32       dimsizes[MAXRANK];
    int32       dim_id = FAIL;
    int32       dimNT[MAXRANK];
    int32       dimnattr[MAXRANK];
    int32       rank;
    int32       nt;
    int32       nattrs;
    char        curr_file_name[MAXFNLEN];
   intn ret_value = SUCCEED;
   
   /* temp. names for file type and curr input file name for ease of use */
   HDstrcpy( curr_file_name, dumpsds_opts->ifile_name );

   /* when there are no datasets specified, dumper dumps all datasets */
   if (num_sds_chosen == (-1))
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

      sds_count++;   /* count the # of images have been processed */

      /* Reset variables. */
      HDmemset(dimsizes, 0, sizeof(int32) * MAXRANK);
      HDmemset(dimNT, 0, sizeof(int32) * MAXRANK);
      HDmemset(dimnattr, 0, sizeof(int32) * MAXRANK);

      sds_id = SDselect(sdf_id, sds_index);
      if (sds_id == FAIL)
      {
         fprintf(stderr,"Failure in selecting %d'th SDS in file %s\n",
                                    sds_index, curr_file_name);
         continue; /* to the next dataset */
      }

      if (FAIL == SDgetinfo(sds_id, name, &rank, dimsizes, &nt, &nattrs))
      {
         fprintf(stderr,"SDgetinfo failed for sds_id(%d) in file %s\n", 
                                    (int)sds_id, curr_file_name);
         continue; /* to next dataset */
      }

/*this isdimvar may not be necessary, confirm then remove it */
/*      isdimvar = (SDiscoordvar(sds_id)) ? 1 : 0; */

      /* output data to binary file   */
      if (rank > 0 && dimsizes[0] != 0)
      {
         if (FAIL == sdsdumpfull(sds_id, dumpsds_opts, rank, dimsizes, nt, fp))
         {
            fprintf(stderr, "sdsdumpfull failed for sds_id(%d) in file %s\n", 
                                          (int)sds_id, curr_file_name);
            continue; /* to next SDS*/
         }
      }

      if (FAIL == SDendaccess(sds_id))
      {
         fprintf(stderr, "SDendaccess failed for sds_id(%d) in file %s\n", 
                                    (int)sds_id, curr_file_name);
         continue; /* to next SDS*/
      }
      sds_id = FAIL; /* reset */
   }  /* for all ndsets in file */
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
   intn   i, ret, j;
   int32  sdf_id = FAIL,
          sds_id = FAIL,
            sds_count,
          *sds_chosen = NULL,
          num_sds_chosen,
          rank,
          nt,
          nattr,
          ndsets,
          n_file_attrs,
          attr_nt,
          attr_count,
          attr_buf_size,
          attr_index;
   char   file_name[MAXFNLEN],
          name[MAXNAMELEN],
          dim_nm[MAXNAMELEN];
    int32       dimsizes[MAXRANK];
    int32       dim_id = FAIL;
    int32       dimNT[MAXRANK];
    int32       dimnattr[MAXRANK];
    FILE       *fp = NULL;
    int32       index;
    VOIDP       attr_buf = NULL;
    char       *nt_desc = NULL;
    int         index_error = 0;
    intn        ret_value = SUCCEED;

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

      sdf_id = SDstart(file_name, DFACC_RDONLY);
      if (sdf_id == FAIL)
      {
         fprintf(stderr,"Failure in open %s\n", file_name);
         continue; /* to the next file */
      }

      /* obtain the list of indices of requested datasets and the number 
         of items in that list, which is the actual number of datasets
         are to be processed for the current hdf file */
      num_sds_chosen = dumpsds_opts->num_chosen;
      if (num_sds_chosen > 0)
      {
         if ((sds_chosen = (int32 *) HDmalloc(sizeof(int32) * num_sds_chosen)) == NULL)
         {
            fprintf(stderr,"Failure in dsd: Not enough memory!\n");
            exit(1);
         }   /* end if */

         sds_count = (-1);
         HDmemfill(sds_chosen, &sds_count, sizeof(int32), num_sds_chosen);
      }	/* end if */

      /* compose the list of indices of SDSs to be processed in the current 
         file: sds_chosen is the list and return the number of items in it */
      sds_count = get_SDSindex_list( sdf_id, dumpsds_opts, sds_chosen, &index_error );

/* if there are errors with the given indices, ref#s, or names of the
requested datasets, and so the input yields no valid datasets, then close 
the interface and the input file, and move on to the next file */
      if (index_error && sds_count == 0)
      {
         if(sds_chosen!=NULL)
         {
            HDfree(sds_chosen);
            sds_chosen=NULL;
         } /* end if */
         if( FAIL == SDend(sdf_id))
            fprintf( stderr, "SDend failed for the current file\n" );
         continue; /* to the next file */
      } /* end if */

      /* obtain number of datasets in the file and number of file attributes,
         ndsets will be used to process the images, n_file_attrs will be
         used to print file attributes */
      ret = SDfileinfo(sdf_id, &ndsets, &n_file_attrs);
      if (ret == FAIL)
      {
         fprintf(stderr,"Failure in SDfileinfo for file %s\n", file_name);
         ret_value = FAIL;
         goto done;
      }

      fp = stdout;                /* assume no output file given */
      num_sds_chosen= sds_count;  /* update actual count of items chosen */

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
               ret = print_SDattrs( sdf_id, fp, n_file_attrs);
               if( ret == FAIL )
               {
                  fprintf(stderr,"Failure in print_SDattrs for file %s\n", file_name);
                  continue; /* to the next file */

               }
            }
            ret = printSD_ASCII( sdf_id, fp, dumpsds_opts, sds_chosen, num_sds_chosen, ndsets);
            if( ret == FAIL )
               fprintf( stderr, "Failure in printSD_ASCII: Unable to dump data for file %s\n", file_name );

            /* close output file only if option -o is given */
            if (dumpsds_opts->dump_to_file)
               fclose(fp);                       
            break;

         case DBINARY:       /* binary file */
             /* get output file name  */
             if (dumpsds_opts->dump_to_file)
                 fp = fopen(dumpsds_opts->file_name, "wb");
            ret = printSD_BINARY( sdf_id, fp, dumpsds_opts, sds_chosen, num_sds_chosen, ndsets );
            if( ret == FAIL )
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

      if (FAIL == SDend(sdf_id))
      {
         fprintf(stderr, "SDend failed for file %s\n", file_name);
         continue; /* to the next file */
      }
      sdf_id = FAIL; /* reset */
   }  /* while processing files  */

done:
    if (ret_value == FAIL)
      { /* Failure cleanup */
          if (sds_id != FAIL)
              SDendaccess(sds_id);

          if (sdf_id != FAIL)
              SDend(sdf_id);

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

   /* incomplete command */
   if (help == TRUE)
   {
      dumpsds_usage(argc, argv);
      goto done;
   }  /* end if */

   /* incomplete command */
   if( curr_arg >= argc )
   {
      dumpsds_usage(argc, argv);
      goto done;
   }            /* end if */

   /* parse the user's command and store the inputs in dumpvg_opts */
   status = parse_dumpsds_opts( &dumpsds_opts, &curr_arg, argc, argv );
   if( status == FAIL )
      goto done; /* skip dsd */

   /* display data and information as specified in dumpvg_opts */
   status = dsd( &dumpsds_opts, curr_arg, argc, argv );

  done:
    if (ret_value == FAIL)
      { /* Failure cleanup */
      }
    /* Normal cleanup */

   /* free the lists for given indices, ref#s, names, and classes if 
      they had been allocated */
   free_num_list(dumpsds_opts.by_index.num_list );
   free_num_list(dumpsds_opts.by_ref.num_list );
   free_str_list(dumpsds_opts.by_name.str_list, dumpsds_opts.by_name.num_items);

   return ret_value;
}	/* end do_dumpsds() */


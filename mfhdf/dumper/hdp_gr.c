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

#define  N_ENTRIES      256     /* number of elements of each color */

void
dumpgr_usage(intn  argc,
             char *argv[])
{
    printf("Usage:\n");
    printf("%s dumpgr [-a|-i <index>|-n <name>|-r <ref>] [-dhv] [-o <filename> [ -bx]] <filelist>\n", argv[0]);
    printf("\t-a\tDump all RIs in the file (default)\n");
    printf("\t-i <index>\tDump the <index>th RI in the file \n");
    printf("\t-n <name>\tDump the RI with name <name>\n");
    printf("\t-r <ref>\tDump the RI with reference number <ref>\n");
    printf("\t-d\tDump data only, no tag/ref, formatted to input to hp2hdf\n");
    printf("\t-h\tDump header only, no annotation for elements nor data\n");
    printf("\t-v\tDump everything including all annotations (default)\n");
    printf("\t-o <filename>\tOutput to file <filename>\n");
    printf("\t-b\tBinary format of output\n");
    printf("\t-x\tAscii text format of output (default)\n");
    printf("\tfilelist\tNames of hdf files, separated by a space\n");
}       /* end list_usage() */

void
parse_number_opts( char *argv[],
                   int *curr_arg, 
                   number_filter_t *filter)
{
   int32 numItems = 0, i;
   char *tempPtr = NULL;
   char *ptr = NULL;

   /* put a temp ptr at the beginning of the given list of numbers, 
      separated by commas, for example, 1,2,3 */
   ptr = argv[*curr_arg];

   /* then traverse the list and count the number of items in it */
   while ((tempPtr = HDstrchr(ptr, ',')) != NULL)
   {
      numItems++;       /* count number of items in the list */
      ptr = tempPtr + 1;/* forward pointer to next item, after a comma */
   }  /* end while */
   if (*ptr != '\0')	/* count the last item */
      numItems++;

   /* allocate space to hold all the items in the list */
   filter->num_list = (int32 *) HDmalloc(sizeof(intn) * numItems);
   if (filter->num_list == NULL)
   {
      printf("Failure in parse_number_opts: Not enough memory!\n");
      exit(1);
   }

   /* go back to the beginning of the list and read in the numbers */
   ptr = argv[*curr_arg];
   i = 0;  /* index of the list */
   while ( i < numItems )
   {
      tempPtr = HDstrchr(ptr, ',');
      if( tempPtr != NULL )
         *tempPtr = '\0';  /* end the string of digits */
      filter->num_list[i] = atoi(ptr);  /* convert string to digits */
      ptr = tempPtr + 1;
      i++;
   }
   filter->num_items = numItems;   /* save the number of items */
}  /* parse_number_opts */

void
parse_string_opts( char *argv[],
                   int *curr_arg, 
                   char_filter_t *filter)
{
   int32 numItems = 0, i;
   char *tempPtr = NULL;
   char *ptr = NULL;

   /* put a temp pointer at the beginning of the list of strings,
      separated by commas */
   ptr = argv[*curr_arg];

   /* check if it's the end of the command */
   if( ptr == NULL )
   {
      printf("Missing values for option\n");
      exit(1);
   }

   /* then traverse the list and count the number of strings in it */
   while ((tempPtr = HDstrchr(ptr, ',')) != NULL)
   {
      numItems++;
      ptr=tempPtr+1;
   }  /* end while */
   if (*ptr != '\0')	/* count the last item */
      numItems++;

   /* allocate space to hold pointers that will point to the given strings */
   if ((filter->str_list = (char **) HDmalloc(sizeof(char *) * numItems)) == NULL)
   {
      printf("Failure in parse_string_opts: Not enough memory!\n");
      exit(1);
   }

   /* go back to the beginning of the list and read in the given strings */
   ptr = argv[*curr_arg];
   i = 0;  /* init the index of the list */
   while ( i < numItems )
   {
      tempPtr = HDstrchr(ptr, ','); /* find the end of a string */
      if( tempPtr != NULL )
         *tempPtr = '\0';  /* end the string with a NULL char */

      /* allocate space for each string */
      if ((filter->str_list[i] = (char *) HDmalloc(sizeof(char) * (HDstrlen(ptr) + 1))) == NULL)
      {
         printf("Failure in parse_string_opts: Not enough memory!\n");
         exit(1);
      }
      HDstrcpy(filter->str_list[i], ptr);  /* get the current string */
      ptr = tempPtr + 1;  /* move pointer to next item or end of list */
      i++;
   }  /* end while */

   filter->num_items = numItems;	/* save the number of items */

} /* parse_string_opts */

intn 
parse_dumpgr_opts(dump_info_t *dumpgr_opts, 
                  intn        *curr_arg, 
                  intn         argc, 
                  char        *argv[] )
{

   /* traverse the command and process each option */
   while ((*curr_arg < argc) && (argv[*curr_arg][0] == '-'))
   {
      switch (argv[*curr_arg][1])
      {
         case 'a':	/* dump all, default */
             dumpgr_opts->filter = DALL;
             dumpgr_opts->num_chosen = -1; /* indicates all items dumped*/
             (*curr_arg)++;
             break;
            
         case 'i':	/* dump by index */
             dumpgr_opts->filter |= DINDEX;  /* set bit DINDEX */
             (*curr_arg)++;

             /* parse and store the given indices in structure by_index */
             parse_number_opts( argv, curr_arg, &dumpgr_opts->by_index);
             (*curr_arg)++;
             break;

         case 'r':	/* dump by reference */
             dumpgr_opts->filter |= DREFNUM; /* set bit DREFNUM */
             (*curr_arg)++;

             /* parse and store the given ref numbers in structure by_ref */
             parse_number_opts( argv, curr_arg, &dumpgr_opts->by_ref);
             (*curr_arg)++;
             break;

         case 'n':	/* dump by names */
             dumpgr_opts->filter |= DNAME;   /* set bit DNAME */
             (*curr_arg)++;

             /* parse and store the given names in structure by_name */
             parse_string_opts( argv, curr_arg, &dumpgr_opts->by_name);
             (*curr_arg)++;
             break;

         case 'c':	/* dump by class */
             printf("Currently, no class defined on an RI.\n");
             return( FAIL );
             
         case 'd':	/* dump data only */
             dumpgr_opts->contents = DDATA;
             (*curr_arg)++;
             break;

         case 'h':	/* no annotations nor data */
             dumpgr_opts->contents = DHEADER;
                (*curr_arg)++;
             break;

         case 'v':	/* dump all info, default */
             dumpgr_opts->contents = DVERBOSE;
             (*curr_arg)++;
             break;

         case 'o':   /* specify output file */
             dumpgr_opts->dump_to_file = TRUE;

             /* Get file name */
             HDstrcpy(dumpgr_opts->file_name, argv[++(*curr_arg)]);

             (*curr_arg)++;
             break;

         case 'b':   /* dump data in binary */
             dumpgr_opts->file_type = DBINARY;
             (*curr_arg)++;
             break;

         case 'x':   /* dump data in ascii, also default */
             dumpgr_opts->file_type = DASCII;
             (*curr_arg)++;
             break;

         default:	/* invalid dumpgr option */
             printf("Invalid dumpgr option %s\n", argv[*curr_arg]);
             dumpgr_usage(argc, argv);
             return (FAIL);
         }   /* end switch */
      }	 /* end while */
   
   /* add the number of images requested by index, by ref#, and by name
      to have a total number of requested images */
   dumpgr_opts->num_chosen = dumpgr_opts->by_index.num_items +
                             dumpgr_opts->by_ref.num_items +
                             dumpgr_opts->by_name.num_items;
   return (SUCCEED);
}	/* end parse_dumpgr_opts */

intn 
grdumpfull(int32        ri_id, 
           dump_info_t  *dumpgr_opts,
           int32        ncomp,	/* "ncomp" is the number of components 
                                   in each element of the data set */
           int32        dimsizes[], /*  size of dimension "i". */
           int32        nt, 
           FILE        *fp)
{
   VOIDP  buf = NULL;
   int32  numtype;
   int32  eltsz;
   int32  read_nelts;
   int32 *start = NULL;
   int32 *edge = NULL;
   int32 *stride = NULL;
   intn   ret,
          ret_value = SUCCEED;

   /* Compute the number of the bytes for each value. */
   numtype = nt & DFNT_MASK;
   eltsz = DFKNTsize(numtype | DFNT_NATIVE)*ncomp;
    
   read_nelts = dimsizes[0]*dimsizes[1];
   buf = (VOIDP) HDmalloc(read_nelts * eltsz);
   if (buf == NULL)
   {
      fprintf(stderr,"Failure in grdumpfull: Not enough memory!\n");
      exit(1);
   }
   start = (int32 *) HDmalloc(ncomp * sizeof(int32));
   if (start == NULL)
   {
      fprintf(stderr,"Failure in grdumpfull: Not enough memory!\n");
      exit(1);
   }
   edge = (int32 *) HDmalloc(ncomp * sizeof(int32));
   if (edge == NULL)
   {
      fprintf(stderr,"Failure in grdumpfull: Not enough memory!\n");
      exit(1);
   }

   stride = (int32 *) HDmalloc(ncomp * sizeof(int32));
   if (stride == NULL)
   {
      fprintf(stderr,"Failure in grdumpfull: Not enough memory!\n");
      exit(1);
   }

   start[0]=start[1]=0;
   edge[0]=dimsizes[0];
   edge[1]=dimsizes[1];
   stride[0]=1;
   stride[1]=1;
 
   if (FAIL == GRreadimage(ri_id, start, stride, edge, buf))
   {
      fprintf(stderr,"GRreadimage failed for ri_id(%d) in file %s\n",
                  (int)ri_id, dumpgr_opts->ifile_name );
      ret_value = FAIL;
      goto done;
   }

   ret = dumpfull(nt, dumpgr_opts->file_type, read_nelts*ncomp, buf, 16, fp);
   if( ret == FAIL )
   {
      fprintf(stderr,"dumpfull failed for ri_id(%d) in file %s\n",
                  (int)ri_id, dumpgr_opts->ifile_name );
      ret_value = FAIL;
      goto done;
   }

done:
    if (ret_value == FAIL)
      { /* Failure cleanup */
      }
    /* Normal cleanup */
    if (edge != NULL)
        HDfree((VOIDP) edge);
    if (start != NULL)
        HDfree((VOIDP) start);
    if (stride != NULL)
        HDfree((VOIDP) stride);
    if (buf != NULL)
        HDfree((VOIDP) buf);

    return ret_value;
}	/* grdumpfull */

/* compose the list of indices of the requested images although some
   images are requested by ref# or name.
   The routine returns the number of images to be processed, or -1 if
   all images are to be processed, or 0 if none.  If there are any
errors, the parameter index_error will return the number of errors
occured */
intn
get_RIindex_list( int32 gr_id,
                  dump_info_t *dumpgr_opts, 
                  int32 *gr_chosen,/* array of indices of RIs to be processed */
                  intn *index_error )
{
   intn i, ri_count;  /* number of RIs to be processed */
   int32 index;
   filter_t filter = dumpgr_opts->filter; /* temporary name */
   intn  ret_value = SUCCEED;

   ri_count = 0;   /* no image requested yet */

   /* if no specific images are requested, set image count to -1 to 
      indicate that all images are to be dumped */
   if( filter == DALL )
   {
      ri_count= -1;
      return( ri_count );
   } 

   /* if there are some images requested by index, store the indices
      in the array provided by the caller */
   if( filter & DINDEX )
      for (i = 0; i < dumpgr_opts->by_index.num_items; i++)
      {
      /* Note: Don't replace this with HDmemcpy unless you change the sizes of the objects correctly -QAK */
         gr_chosen[i] = dumpgr_opts->by_index.num_list[i];
         ri_count++;
      }

   /* if there are some images requested by ref#, convert the ref#
      to indices and store them in the array provided by the caller */
   if( filter & DREFNUM ) 
      for (i = 0; i < dumpgr_opts->by_ref.num_items; i++)
      {
         index = GRreftoindex(gr_id, dumpgr_opts->by_ref.num_list[i]);
                
         if (index == -1)
         {
            printf("Image with reference number %d: not found\n", dumpgr_opts->by_ref.num_list[i]);
            *index_error = 1;     
         }
         else
         {
            gr_chosen[ri_count] = index;
            ri_count++;
         }
      }             

   /* if there are some images requested by name, convert the names to
      indices and store them in the array provided by the caller */
   if( filter & DNAME )
      for (i = 0; i < dumpgr_opts->by_name.num_items; i++)
      {
         index = GRnametoindex(gr_id, dumpgr_opts->by_name.str_list[i]);
         if (index == -1)
         {
            printf("Image with name %s: not found\n", dumpgr_opts->by_name.str_list[i]);
            *index_error = 1;
         }
         else
         {
            gr_chosen[ri_count] = index;
            ri_count++;
         }
      }

   /* if there are some images requested by class, convert the names to
      indices and store them in the array provided by the caller */
   if( filter & DCLASS )  /* redundant?  may be removed */
   {
      printf("Currently, no class defined on an RI.\n");
   }

done:
    if (ret_value == FAIL)
      { /* Failure cleanup */
      }
    /* Normal cleanup */

    return ri_count;
} /* get_RIindex_list */

/* prints all GR file attributes in the current file */
intn
print_GRattrs( int32 gr_id,
               int32 n_file_attrs,
               FILE *fp,
               dump_info_t *dumpgr_opts )
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
   intn  printed = FALSE;  /* whether file attr title is printed */

   /* temporary name for current input file */
   HDstrcpy( curr_file_name, dumpgr_opts->ifile_name );

   /* for each file attribute, print its info and data */
   for (attr_index = 0; attr_index < n_file_attrs; attr_index++)
   {
      /* get the current attr's name, number type, and number of values
*/
      if (FAIL == GRattrinfo(gr_id, attr_index, attr_name, &attr_nt,
&attr_count
))
      {
         fprintf(stderr,"GRattrinfo failed for %d'th attribute of file %s\n", attr_index, curr_file_name);
         continue; /* to the next attribute */
      }

      attr_nt_desc = HDgetNTdesc(attr_nt);
      if (NULL == attr_nt_desc)
      {
         fprintf(stderr,"HDgetNTdesc failed for %d'th attribute of file %s\n", attr_index, curr_file_name);
         continue; /* to the next attribute */
      }

      /* calculate the buffer size of the attribute using the number of
         values in the attribute and its value size */
      attr_buf_size = DFKNTsize(attr_nt) * attr_count;
      attr_buf = (VOIDP) HDmalloc(attr_buf_size);
      if (attr_buf == NULL)
      {
         fprintf(stderr,"Failure in print_GRattrs: Not enough memory!\n");
         exit(1);
      }

      /* read the values of the attribute into the buffer */
      ret = GRgetattr( gr_id, attr_index, attr_buf );
      if (ret == FAIL)
      {
         fprintf(stderr,"GRgetattr failed for %d'th attribute in file %s\n", attr_index, curr_file_name);
         continue; /* to the next attribute */
      }

      /* just print a title line for file attributes if it's not printed
         yet and set flag so it won't print that again */
      if( !printed )
      {
         fprintf(fp, "\n    File attributes:\n");
         printed = TRUE;
      }
      /* display the attribute's information then free buffer */
      fprintf(fp, "\t Attr%d: Name = %s\n", (int) attr_index, attr_name);
      fprintf(fp, "\t\t Type = %s \n\t\t Count= %d\n", attr_nt_desc, (int) attr_count);
      HDfree(attr_nt_desc);
      attr_nt_desc = NULL; /* reset */

      /* display the attribute's values then free buffer */
      fprintf(fp, "\t\t Value = ");
      if (FAIL == dumpfull(attr_nt, dumpgr_opts->file_type, attr_count, attr_buf, 20, fp))
      {
         fprintf(stderr,"dumpfull failed for %d'th attribute in filei%s\n", attr_index, curr_file_name);
         continue; /* to the next attribute */
      }
      HDfree((VOIDP) attr_buf);
      attr_buf = NULL; /* reset */
   } /* for all attributes of GR */

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
} /* end of print_GRattrs */

intn
print_RIattrs( int32 ri_id,
               intn ri_index,
               int32 nattrs,
               FILE *fp,
               dump_info_t *dumpgr_opts )
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
   HDstrcpy( curr_file_name, dumpgr_opts->ifile_name );

   /* for each attribute, display its info and data */
   for (attr_index = 0; attr_index < nattrs; attr_index++)
   {
      /* get the current attr's name, number type, and number of values */
      if (FAIL == GRattrinfo(ri_id, attr_index, attr_name, &attr_nt, &attr_count))
      {
         fprintf(stderr,"GRattrinfo failed for %d'th attribute of %d'th RI in file %s\n", attr_index, ri_index, curr_file_name);
         continue; /* to the next attribute */
      }

      attr_nt_desc = HDgetNTdesc(attr_nt);
      if (NULL == attr_nt_desc)
      {
         fprintf(stderr,"HDgetNTdesc failed for %d'th attribute of %d'th RI in file %s\n", attr_index, ri_index, curr_file_name);
         continue; /* to the next attribute */
      }

      /* calculate the buffer size of the attribute using the number of 
         values in the attribute and its value size */
      attr_buf_size = DFKNTsize(attr_nt) * attr_count;
      attr_buf = (VOIDP) HDmalloc(attr_buf_size);
      if (attr_buf == NULL)
      {
         fprintf(stderr,"Failure in print_RIattrs: Not enough memory!\n");
         exit(1);
      }

      /* read the values of the attribute into the buffer */
      ret = GRgetattr( ri_id, attr_index, attr_buf );
      if (ret == FAIL)
      {
         fprintf(stderr,"GRgetattr failed for %d'th attribute of %d'th RI in file %s\n", attr_index, ri_index, curr_file_name);
         continue; /* to the next attribute */
      }

      /* display the attribute's information then free buffer */
      fprintf(fp, "\t Attr%d: Name = %s\n", (int) attr_index, attr_name);
      fprintf(fp, "\t\t Type = %s \n\t\t Count= %d\n", attr_nt_desc, (int) attr_count);
      HDfree(attr_nt_desc);
      attr_nt_desc = NULL; /* reset */

      /* display the attribute's values then free buffer */
      fprintf(fp, "\t\t Value = ");
      if (FAIL == dumpfull(attr_nt, dumpgr_opts->file_type, attr_count, attr_buf, 20, fp))
      {
         fprintf(stderr,"dumpfull failed for %d'th attribute of %d'th RI in file %s\n", attr_index, ri_index, curr_file_name);
         continue; /* to the next attribute */
      }
      HDfree((VOIDP) attr_buf);
      attr_buf = NULL; /* reset */
   } /* for all attributes of an RI */

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
} /* end of print_RIattrs */

/* Displays the palette data.  Recall that HDF supports only 256 colors.
   Each color is defined by its 3 components. Therefore,
   verifying the value of n_entries and n_comps is not necessary and
   the buffer to hold the palette data can be static.  However,
   if more values or colors are added to the model, these parameters
   must be checked to allocate sufficient space when reading a palette */
intn
print_Palette( int32 ri_id, 
               int32 pal_index,
               FILE *fp )
{
   int32 pal_id = FAIL,
         data_type,
         n_comps,
         n_entries,
         interlace_mode;
   uint8 palette_data[N_ENTRIES][3];  /* static because of fixed size */
   intn  i, j;
   intn  status = SUCCEED, ret_value = SUCCEED;

   /* Get the identifier of the palette attached to the image. */
   pal_id = GRgetlutid (ri_id, pal_index);
   if( pal_id == FAIL )
   {
      fprintf( stderr, "GRgetlutid failed for palette #%d\n", pal_index);
      ret_value = FAIL;
      goto done;
   }

   /* Obtain and display information about the palette. */
   status = GRgetlutinfo (pal_id, &n_comps, &data_type, &interlace_mode,
                         &n_entries);
   if( status == FAIL )
   {
      fprintf( stderr, "GRgetlutinfo failed for palette #%d\n", pal_index);
      ret_value = FAIL;
      goto done;
   }
   fprintf (fp, "\t Palette: %d components; %d entries\n", n_comps, n_entries);

   /* Read the palette data. */
   status = GRreadlut (pal_id, (VOIDP)palette_data);
   if( status == FAIL )
   {
      fprintf( stderr, "GRreadlut failed for palette #%d\n", pal_index);
      ret_value = FAIL;
      goto done;
   }

   /* Display the palette data. */
   printf ("\t Palette Data: \n");
   for (i=0; i< n_entries; i++)
   {
      printf ("\t\t");
      for (j = 0; j < n_comps; j++)
         printf ("%i ", palette_data[i][j]);
      printf ("\n");
   }
   printf ("\n"); 

done:
   if (ret_value == FAIL)
   { /* Failure cleanup */
   }
   /* Normal cleanup */
   return ret_value;
}  /* end of print_Palette */

intn
printGR_ASCII( 
int32 gr_id,
dump_info_t *dumpgr_opts,
int32 num_ri_chosen,
int32 ndsets,
int32 *gr_chosen,
FILE *fp )
{
   intn  dumpall = FALSE;    /* TRUE when no specific images requested */
   int32 ri_ref,             /* ref# of an image */
         ri_index,           /* index of images in the file */
         ri_count = 0,       /* count of images being printed */
         nt,                 /* number type of an image */
         il,                 /* interlace mode of an image */
         ncomp,              /* number of components in an image */
         dimsizes[MAXRANK],  /* dimension sizes of an image */
         dimNT[MAXRANK],     /* number type of a dimension ??? */
         dimnattr[MAXRANK],  /* ??? */
         ri_id = FAIL;
   char  name[MAXNAMELEN],   /* name of an image */
         curr_file_name[MAXFNLEN], /* current input (hdf) file's name */
        *nt_desc = NULL;     /* ??? */
   int32 nattrs,             /* number of attributes of an image */
         nglb_attrs;         /* number of global (file) attributes */
   file_type_t ft;           /* temporary type for output file type */
   intn  ret = SUCCEED,      /* status returned from a routine */
         ret_value = SUCCEED;/* return value of printGR_ASCII */

   /* temp. name for curr input file name for ease of use */
   HDstrcpy( curr_file_name, dumpgr_opts->ifile_name );

   /* when there are no images specified, dumper dumps all images */
   if (num_ri_chosen == (-1))
      dumpall = TRUE;
   else
      sort(gr_chosen, num_ri_chosen);

   /* for each valid index, if the user requests to dump all images or 
      if there are more requested images to be processed, process the 
      indexed image */
   ri_count = 0;  /* no requested images has been processed yet */
   for (ri_index = 0; ri_index < ndsets /* validate index */
        && (dumpall                     /* either all images are dumped */
        || ri_count < num_ri_chosen);   /* or more requested images */
        ri_index++)
   {
      /* if the user neither requests dump all nor the current image */
      if ((!dumpall) && (ri_index != gr_chosen[ri_count]))
         continue; /* skip */

      ri_count++;  /* count the # of images have been processed */

      /* Reset variables. */
      HDmemset(dimsizes, 0, sizeof(int32) * MAXRANK);
      HDmemset(dimNT, 0, sizeof(int32) * MAXRANK);
      HDmemset(dimnattr, 0, sizeof(int32) * MAXRANK);

      ri_id = GRselect(gr_id, ri_index);
      if (ri_id == FAIL)
      {
         fprintf(stderr,"Failure in selecting %d'th RI in file %s\n", 
                                    ri_index, curr_file_name );
         continue; /* to the next image */
      }

      if (FAIL == GRgetiminfo(ri_id, name, &ncomp, &nt, &il, dimsizes, &nattrs))
      {
         fprintf(stderr,"GRgetiminfo failed for %d'th RI in file %s\n",
                                   ri_index, curr_file_name );
         if (FAIL == GRendaccess(ri_id))    
            fprintf(stderr,"GRendaccess failed for %d'th RI in file %s\n",
                                    ri_index, curr_file_name );
         continue; /* to the next image */
      }

      switch (dumpgr_opts->contents)
      {
         case DVERBOSE:
         case DHEADER:
            nt_desc = HDgetNTdesc(nt);
            if (NULL == nt_desc)
            {
               fprintf(stderr,"HDgetNTdesc failed for %d'th RI in file %s\n",
                                          ri_index, curr_file_name );
               if (FAIL == GRendaccess(ri_id))    
                  fprintf(stderr,"GRendaccess failed for %d'th RI in file %s\n",
                                    ri_index, curr_file_name );
               continue; /* to the next image */
            }

            fprintf(fp, "\n\t Image  Name = %s\n\t Index = ", name);
            fprintf(fp, "%d\n\t Type= %s\n", ri_index, nt_desc);

            HDfree(nt_desc);
            nt_desc = NULL; /* reset */

            if ((ri_ref = GRidtoref(ri_id)) == FAIL)
            {
               fprintf(stderr,"Failure in determining reference no. for %d'th RI in file %s\n", ri_index, curr_file_name );
               if (FAIL == GRendaccess(ri_id))    
                  fprintf(stderr,"GRendaccess failed for %d'th RI in file %s\n",
                                    ri_index, curr_file_name );
               continue; /* to the next image */
            }

            fprintf(fp, "\t width=%d; height=%d\n", (int) dimsizes[0], (int) dimsizes[1]);
            fprintf(fp, "\t Ref. = %d\n", (int) ri_ref);
            fprintf(fp, "\t ncomps = %d\n\t Number of attributes = %d\n\t Interlace= %d\n", (int) ncomp, (int) nattrs,(int) il);

            /* Print image attributes */
            ret = print_RIattrs( ri_id, ri_index, nattrs, fp, dumpgr_opts );
            if( ret == FAIL )
               break; /* so GRendaccess can be called, then go to next image */

            /* print image palette. Note: currently only 1 pal assigned
               to an image, so its index is always 0 */
            /* ret = print_Palette( ri_id, 0, fp );
            if( ret == FAIL )
               break;*/ /* so GRendaccess can be called, then go to next image */

            if (dumpgr_opts->contents == DHEADER)
               break; /* break out for header only */

         case DDATA:
            if (dumpgr_opts->contents != DDATA)
               fprintf(fp, "\t Data : \n");
            if (ncomp > 0 && dimsizes[0] != 0)
            {
               fprintf(fp, "                ");/* spaces to line up output */

               /* print the current image's data */
               ret = grdumpfull( ri_id, dumpgr_opts, ncomp, dimsizes, nt, fp);
               if ( ret == FAIL )
                 {/* it'll break and end access to current image anyway */}
            }
            break; /* data section */
         default:
            printf("Unknown output type option \n" );
      } /* switch  on contents */

      if (FAIL == GRendaccess(ri_id))    
      {
         fprintf(stderr,"GRendaccess failed for %d'th RI in file %s\n",
                                    ri_index, curr_file_name );
         continue; /* to the next image */
      }
   }	/* for ndsets  */

done:
    if (ret_value == FAIL)
      { /* Failure cleanup */
          if (nt_desc != NULL)
              HDfree(nt_desc);
      }
    /* Normal cleanup */

    return ret_value;
} /* end of printGR_ASCII */

intn
printGR_BINARY(
int32 gr_id,
dump_info_t *dumpgr_opts,
int32 num_ri_chosen,
int32 ndsets,
int32 *gr_chosen,
FILE *fp )
{
   intn  ri_index,
         ri_count = 0,
         dumpall = FALSE;
   int32 ri_id = FAIL,
         nattrs,
         ri_ref,
         nt,
         il,
         ncomp,
         dimsizes[MAXRANK],
         dimNT[MAXRANK],
         dimnattr[MAXRANK];
   char  name[MAXNAMELEN],
         curr_file_name[MAXFNLEN]; /* temp. name for current hdf file name */
   intn ret, ret_value = SUCCEED;

   /* temp. name for curr input file name for ease of use */
   HDstrcpy( curr_file_name, dumpgr_opts->ifile_name );

   if (num_ri_chosen == (-1))
      dumpall = TRUE;
   else
      sort(gr_chosen, num_ri_chosen);

   /* for each valid index, display the image's data
      if the user request to dump all images or if there are more
      requested images to be processed */
   ri_count = 0;  /* no requested image has been processed yet */
   for (ri_index = 0; ri_index < ndsets /* validate index */
        && ( dumpall                /* either all images are dumped */
          || ri_count < num_ri_chosen); /* or more requested images */
                     ri_index++)
   {
      /* if the user neither requests dump all nor the current images */
      if ((!dumpall) && (ri_index != gr_chosen[ri_count]))
         continue; /* skip */

      ri_count++;

      /* Reset variables. */
      HDmemset(dimsizes, 0, sizeof(int32) * MAXRANK);
      HDmemset(dimNT, 0, sizeof(int32) * MAXRANK);
      HDmemset(dimnattr, 0, sizeof(int32) * MAXRANK);

      ri_id = GRselect(gr_id, ri_index);
      if (ri_id == FAIL)
      {
         fprintf(stderr,"Failure in selecting %d'th RI in file %s\n", 
                                    ri_index, curr_file_name);
         continue; /* to the next image */
      }
/* BMR: removed the part about reference number processing here, used
all indices instead */
      if (FAIL == GRgetiminfo(ri_id, name, &ncomp, &nt, &il, dimsizes, &nattrs))
      {
         fprintf(stderr,"GRgetiminfo failed for %d'th RI in file %s\n",
                                   ri_index, curr_file_name);
         continue; /* to the next image */
      }

      /*   output data to binary file   */
      if (ncomp > 0 && dimsizes[0] != 0)
      {
         /* print the current image's data */
         ret = grdumpfull(ri_id, dumpgr_opts, ncomp, dimsizes, nt, fp);
         if ( ret == FAIL )
           {/* go to endaccess this image then process next image */}
      }

      if (FAIL == GRendaccess(ri_id))
      {
         fprintf(stderr,"GRendaccess failed for %d'th RI in file %s\n",
                                    ri_index, curr_file_name);
         continue; /* to the next image */
      }
   }  /* for ndsets */

done:
    if (ret_value == FAIL)
      { /* Failure cleanup */
      }
    /* Normal cleanup */

    return ret_value;
} /* end of printGR_BINARY */

/* 
shutdown combines the process of GRend, Hclose, and resetting the
   file id and the interface id after validating the ids first.
   When either GRend or Hclose fails, shutdown returns
   FAIL which will be used by the calling routine to goto done with its
   ret_value=FAIL and without a message since the message is printed
   here.  In addition, shutdown deallocates some allocated space - BMR */
int32
shutdown(
    int32 grf_id,
    int32 gr_id )
{
   if( gr_id != FAIL )
   {
      if (FAIL == GRend(gr_id))
      {
         fprintf(stderr,"GRend failed for the current file\n" );
         return( FAIL );
      }
      gr_id = FAIL; /* reset */
   }

   if( grf_id != FAIL )
   {
      if (FAIL == Hclose(grf_id))
      {
         fprintf(stderr,"Hclose failed for the current file\n" );
         return( FAIL );
      }
      grf_id = FAIL; /* reset */
   }
} /* end of shutdown */

intn 
dgr(dump_info_t *dumpgr_opts, 
    intn         curr_arg, 
    intn         argc, 
    char        *argv[] )
{
    intn        i, ret;
    int32       grf_id = FAIL;
    int32       gr_id = FAIL;
    int32       ri_id = FAIL;
    int32       *gr_chosen = NULL;
    int32       num_ri_chosen;
    int32       ncomp;
    int32       nt;
    int32       nglb_attrs;
    int32       ndsets;
    int32       il;
    intn        j, ri_count;
    char        file_name[MAXFNLEN];
    char        name[MAXNAMELEN];
    int32       dimsizes[MAXRANK];
    int32       dimNT[MAXRANK];
    int32       dimnattr[MAXRANK];
    int32       index;
    char       *nt_desc = NULL;
    intn        index_error=0;
    int         ref_found = 0;
    int32       ri_ref = 0;
    FILE  *fp;
    intn        ret_value = SUCCEED;

   /* check for missing input file name */
   if( curr_arg >= argc )
   {
      fprintf( stderr, "Missing input file name.  Please try again.\n" );
      return( FAIL ); /* nothing to be cleaned up at this point */
   }

   /* going through each input file, open the file, try to compose the list
      of indices of the images in the file that are requested, then read and
      display information and data of each image in the specified manner */
   while (curr_arg < argc)
   {
      HDstrcpy(file_name, argv[curr_arg]);   /* get file name */
      HDstrcpy( dumpgr_opts->ifile_name, file_name ); /* record file name */
      curr_arg++;   /* forward the pointer to the current argument */

      grf_id = Hopen(file_name, DFACC_RDONLY,0);
      if (grf_id == FAIL)
        {
            fprintf(stderr,"Unable to open file %s\n", file_name);
            continue; /* to the next file */
        }

      /*printf( "Processing file: %s...\n", file_name ); */
      gr_id = GRstart(grf_id);
      if (FAIL == gr_id)
        {
            fprintf(stderr,"GRstart failed for file %s\n", file_name);
            /* only need to Hclose the file before going to the next */
            shutdown( grf_id, FAIL ); 
            continue; /* to the next file */
        }

      /* BMR: obtain the list of indices of requested dataset and the 
         number of items in that list, which is the actual number of images 
         are to be processed */
      num_ri_chosen = dumpgr_opts->num_chosen;
      if (num_ri_chosen > 0)
      {
         if ((gr_chosen = (int32 *) HDmalloc(sizeof(int32) * num_ri_chosen)) == NULL)
         {
            fprintf(stderr,"Failure in dgr: Not enough memory!\n");
            exit(1);
         }  /* end if */

         ri_count = (-1);
         HDmemfill(gr_chosen, &ri_count, sizeof(int32), num_ri_chosen);
      }  /* end if */

      /* BMR: compose the list of indices of RIs to be processed in the current
         file: gr_chosen is the list and return the number of items in it */
      ri_count = get_RIindex_list( gr_id, dumpgr_opts, gr_chosen, &index_error);

/* BMR: if there are errors with the given indices, ref#s, or names of
the requested images, and yields no valid images, then close the
interface and the input file, and move on to the next file */ 
      if (index_error && ri_count==0)
      {
         if(gr_chosen!=NULL)
         {
            HDfree(gr_chosen);
            gr_chosen=NULL;
         } /* end if */

         /* shutting down the current interface and file */
         shutdown( grf_id, gr_id );
         continue;
      } /* end if */

      /* obtain number of datasets in the file and number of file attributes,
         ndsets will be used to process the images, nglb_attrs will be 
         used to print file attributes */
      ret = GRfileinfo(gr_id, &ndsets, &nglb_attrs);
      if (ret == FAIL)
      {
         fprintf(stderr,"Failure in GRfileinfo for file %s\n", file_name);
         ret_value = FAIL;
         goto done; /* back to dgr which will go to next file */
      }

      fp = stdout;  /* assume that output option is not given */
      num_ri_chosen = ri_count;  /* update actual count of items chosen */

      switch(dumpgr_opts->file_type)
      {
         case DASCII:       /* ASCII file */
            /* open output file for ASCII or direct to standard output */
            if (dumpgr_opts->dump_to_file)
               fp = fopen( dumpgr_opts->file_name, "w");

            /* display the name and global attributes of the current file if 
               data only option is not selected */
            if (dumpgr_opts->contents != DDATA)
            {
               fprintf(fp, "File name: %s \n", file_name );
         
               /* print GR file attributes */
               ret = print_GRattrs( gr_id, nglb_attrs, fp, dumpgr_opts );
               if( ret == FAIL )
               {
                  fprintf(stderr,"Failure in print_GRattrs for file %s\n", file_name);
                  continue;  /* to next file */
               }
            }
            /* print RIs'data and information as requested */
            ret = printGR_ASCII( gr_id, dumpgr_opts, num_ri_chosen, ndsets, 
                              gr_chosen, fp );
            if( ret == FAIL )
               fprintf( stderr, "Failure in printGR_ASCII: Unable to dump data for file %s\n", file_name );

            /* close output file only if option -o is given */
            if (dumpgr_opts->dump_to_file)
               fclose(fp);

            break; /* ASCII */

         case DBINARY:       /* binary file */
            /* open output file for ASCII or print to screen */
            if (dumpgr_opts->dump_to_file)
               fp = fopen(dumpgr_opts->file_name, "wb");

            ret = printGR_BINARY( gr_id, dumpgr_opts, num_ri_chosen, ndsets, 
                               gr_chosen, fp );
            if( ret == FAIL )
               fprintf( stderr, "Failure in printGR_BINARY: Unable to dump data for file %s\n", file_name );

            /* close output file only if option -o is given */
            if (dumpgr_opts->dump_to_file)
               fclose(fp);

            break; /* BINARY */
         default:
            printf("Output file type must be either ascii or binary only\n" );
         } /* switch for output file */
                       
      if(gr_chosen!=NULL)  /* free the array to contain the indices */
      {
         HDfree(gr_chosen);
         gr_chosen=NULL;
      } /* end if */

      /* shutting down the current interface and file */
      shutdown( grf_id, gr_id ); 
   }  /* while more files to process */

done:
    if (ret_value == FAIL)
      { /* Failure cleanup - kind of redundant but sometime necessary */
          if (gr_id != FAIL)
              GRend(gr_id);
          if (grf_id != FAIL)
              Hclose(grf_id);
          if (gr_chosen!=NULL)
              HDfree(gr_chosen);
      }
    /* Normal cleanup */

    return ret_value;
}	/* dgr */

intn 
do_dumpgr(intn        curr_arg, 
          intn        argc, 
          char       *argv[], 
          intn        help)
{
   dump_info_t dumpgr_opts;	/* dumpgr options */
   intn status, ret_value = SUCCEED;
   intn j;  /* index of str_list; to deallocate each string */

   /* initialize the structure that holds user's options and inputs */
   init_dump_opts(&dumpgr_opts);

   if( help == TRUE )
   {
      dumpgr_usage(argc, argv);
      goto done;
   }		/* end if */

   /* incomplete command */
   if( curr_arg >= argc )
   {
      dumpgr_usage(argc, argv);
      goto done;
   }		/* end if */

   /* parse the user's command and store the inputs in dumpgr_opts */
   status = parse_dumpgr_opts( &dumpgr_opts, &curr_arg, argc, argv );
   if( status == FAIL)
      goto done;  /* skip dgr */

   /* display data and information as specified in dumpgr_opts */
   status = dgr( &dumpgr_opts, curr_arg, argc, argv );

   ret_value = status; /* return status to caller */
done:
   if (ret_value == FAIL)
   { /* Failure cleanup */
   }
    /* Normal cleanup */
   free_num_list(dumpgr_opts.by_index.num_list );
   free_num_list(dumpgr_opts.by_ref.num_list );
   free_str_list(dumpgr_opts.by_name.str_list, dumpgr_opts.by_name.num_items);

   return ret_value;
}  /* end do_dumpgr() */

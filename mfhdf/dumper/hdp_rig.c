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
#ifndef MIPSEL
#include <math.h>
#endif /* MIPSEL */

#define IMAGE 1

static intn drig(dump_info_t * dumprig_opts, intn curr_arg, intn argc,
                 char *argv[], int model);

void 
dumprig_usage(intn argc, char *argv[])
{
    printf("Usage:\n");
#ifdef LATER
    printf("%s dumprig [-a|-i <index>|-n <name>|-r <ref>] [-dhv] [-o <filename> [-bx]] <filelist>\n", argv[0]);
#endif
    printf("%s dumprig [-a|-i <index>|-m <n>|-r <ref>] [-dhv] [-o <filename> [-bx]] <filelist>\n", argv[0]);
    printf("\t-a\tDump all RIGs in the file (default)\n");
    printf("\t-i <index>\tDump the <index>th RIG in the file \n");
#ifdef LATER
    printf("\t-n <name>\tDump the RIG with name <name>\n");
#endif
    printf("\t-m <n>\tDump the 8- or 24-bit RIGs only, <n> may be 8 or 24 \n");
    printf("\t-r <ref>\tDump the RIG with reference number <ref>\n");
    printf("\t-d\tDump data only, no tag/ref, formatted to input to hp2hdf\n");
    printf("\t-h\tDump header only, no annotation for elements nor data\n");
    printf("\t-v\tDump everything including all annotations (default)\n");
    printf("\t-o <filename>\tOutput to file <filename>\n");
    printf("\t-b\tBinary format of output\n");
    printf("\t-x\tAscii text format of output (default)\n");
}	/* end list_usage() */

void 
init_dumprig_opts(dump_info_t * dumprig_opts)
{
    dumprig_opts->filter = DALL;	/* default dump all RIGs */
    dumprig_opts->filter_num = NULL;	/* not by reference nor by index */
    dumprig_opts->filter_str = NULL;	/* no strings */
    dumprig_opts->num_chosen = (-1);	/* default dump all items */
    dumprig_opts->contents = DVERBOSE;	/* default dump all information */
    dumprig_opts->dump_to_file = FALSE;		/* don't dump to output file */
    dumprig_opts->file_type = DASCII;	/* default output is ASCII file */
    HDstrcpy(dumprig_opts->file_name, "\0");
}	/* end init_list_opts() */

intn 
parse_dumprig_opts(dump_info_t * dumprig_opts, intn *curr_arg,
                   intn argc, char *argv[], int *model)
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
                dumprig_opts->filter = DALL;
                (*curr_arg)++;
                break;

            case 'm':	/* dump the rigs with model specified as 8-bit or 24-bit */
                (*curr_arg)++;
                *model = atoi(argv[*curr_arg]);
                if (((*model) != 8) && ((*model) != 24))
                  {
                      printf("%s-bit raster model: not available.\n", argv[*curr_arg]);
                      exit(1);
                  }
                (*curr_arg)++;
                break;

            case 'i':	/* dump by index */
            case 'r':	/* dump by reference */
                if ((argv[*curr_arg][1]) == 'i')
                    dumprig_opts->filter = DINDEX;
                else
                    dumprig_opts->filter = DREFNUM;
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

                dumprig_opts->filter_num = (intn *) HDmalloc(sizeof(intn) * numItems);
                if (dumprig_opts->filter_num == NULL)
                  {
                      printf("Not enough memory!\n");
                      exit(-1);
                  }

                ptr = argv[*curr_arg];
                i = 0;
                while ((tempPtr = HDstrchr(ptr, ',')) != NULL)
                  {
                      *tempPtr = '\0';
                      dumprig_opts->filter_num[i] = atoi(ptr);
                      ptr = tempPtr + 1;
                      i++;
                  }
                dumprig_opts->filter_num[i] = atoi(ptr);	/* get the last item */
                dumprig_opts->num_chosen = numItems;	/* save the number of items */

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
                dumprig_opts->dump_to_file = TRUE;	/* get filename */
                HDstrcpy(dumprig_opts->file_name, argv[++(*curr_arg)]);
                if (++(*curr_arg) < argc)	/* binary or ascii */
                  {
                      if (argv[*curr_arg][0] == '-')
                          dumprig_opts->file_type = (argv[*curr_arg][1] == 'b') ? DBINARY : DASCII;
                      else
                          (*curr_arg)--;
                  }
                (*curr_arg)++;
                break;

            default:	/* invalid dumprig option */
                printf("Warning: Invalid dumprig option %s\n", argv[*curr_arg]);
                return (FAIL);
            }	/* end switch */
      }		/* end while */
    return (SUCCEED);
}	/* end parse_dumprig_opts */

void 
do_dumprig(intn curr_arg, intn argc, char *argv[], dump_opt_t * glob_opts)
{
    dump_info_t dumprig_opts;	/* dumprig options */
    int         model = 0;

    if (glob_opts->help == TRUE)
      {
          dumprig_usage(argc, argv);
          return;
      }		/* end if */

    init_dumprig_opts(&dumprig_opts);
    if (parse_dumprig_opts(&dumprig_opts, &curr_arg, argc, argv, &model) == FAIL)
      {
          dumprig_usage(argc, argv);
          return;
      }		/* end if */

    if (drig(&dumprig_opts, curr_arg, argc, argv, model) == FAIL)
      {
          printf("Failure in dumping RIG data\n");
          exit(1);
      }
    if(dumprig_opts.filter_num != NULL)
      HDfree(dumprig_opts.filter_num);
}	/* end do_dumprig() */

static intn 
drig(dump_info_t * dumprig_opts, intn curr_arg, intn argc,
     char *argv[], int model)
{
    int32      *rig_chosen=NULL;
    int32       num_rig_chosen;
    int32       width, height, ndsets, temp;
    int32       i, k, x;
    char        file_name[MAXFNLEN];
    FILE       *fp;
    VOIDP       image;
    int         dumpall = 0, ncomps, il, rig;
    intn        count;

    while (curr_arg < argc)
      {		/* Examine all files. */
          HDstrcpy(file_name, argv[curr_arg]);
          curr_arg++;

          num_rig_chosen = dumprig_opts->num_chosen;
          if (num_rig_chosen > 0)
            {
                if ((rig_chosen = (int32 *) HDmalloc(sizeof(int32) * num_rig_chosen)) == NULL)
                  {
                      printf("Memory allocation error\n");
                      exit(1);
                  }		/* end if */

                k = (-1);
                HDmemfill(rig_chosen, &k, sizeof(int32), num_rig_chosen);
            }	/* end if */

              /* Determine which RIGs are to be displayed. */
          if (dumprig_opts->filter == DINDEX)
            {
                for (i = 0; i < dumprig_opts->num_chosen; i++)
                    rig_chosen[i] = dumprig_opts->filter_num[i];
                sort(rig_chosen, num_rig_chosen);  /* DREFNUM doesn't need this */ 
            }	/* end if */

              /* Get the name of the output file. */
          if (dumprig_opts->dump_to_file)
              fp = fopen(dumprig_opts->file_name, "w");
          else
              fp = stdout;
          if (dumprig_opts->contents != DDATA)
              fprintf(fp, "File name: %s \n\n", file_name);

              /* Determine the number of images in a file. */
          if (model == 8)
            {
                if ((ndsets = DFR8nimages(file_name)) == -1)
                    ndsets = 0;
            }
          else if (model == 24)
            {
                if ((ndsets = DF24nimages(file_name)) == -1)
                    ndsets = 0;
            }
          else
            {
                if ((temp = DFR8nimages(file_name)) == -1)
                    temp = 0;
                if ((ndsets = DF24nimages(file_name)) == -1)
                    ndsets = 0;
                ndsets += temp;
            }

          if (num_rig_chosen == -1)		/* If all RIGs will be dumped, set the flat. */
              dumpall = 1;

          x = 0;	/* Used as the index of the array of "rig_chosen[x]". */
          printf("ndsets=%d, dumpall=%d, num_chosen=%d\n",(int)ndsets,(int)dumpall,(int)dumprig_opts->num_chosen);

          for(i=0; i<dumprig_opts->num_chosen; i++)
             { if((rig_chosen[i] >= ndsets)||(rig_chosen[i]<0))
                printf("\nThe index %d is out of range\n",(int)rig_chosen[i]);
             }

          for (i = 0; i < ndsets && (dumpall!=0 || x<dumprig_opts->num_chosen); i++)
            {	/* Examine all RIGs. */
                int         indent = 5, compressed, has_pal;
                int32       ret;
                int32       eltsz, read_nelts;
                uint16      rig_ref, compr_type;

                rig = DFGRgetimdims(file_name, &width, &height, &ncomps, &il);

                    /* Determine the size of each element; "ncomps" is 1 for an
                       8-bit image or 3 for a 24-bit image. */
                eltsz = DFKNTsize(DFNT_INT8 | DFNT_NATIVE) * ncomps;
                read_nelts = width * height;	/* Number of elements to be read in. */
                if ((image = (VOIDP) HDmalloc(read_nelts * eltsz)) == NULL)
                  {
                      printf("Not enough memory!\n");
                      exit(-1);
                  }
                if ((ret = DFGRIgetimlut((const char *) file_name, image, width,
                                         height, IMAGE, 0, &compressed, &compr_type, &has_pal)) == -1)
                  {
                      printf("Read error!\n");
                      exit(1);
                  }

                rig_ref = DFGRIlastref();	/* Determine the reference of the image just read. */

                    /* If the user has specificed the reference option, then 
                       something has to be done. 
                       Note: the reason why the following part was not done  
                       inside the above "switch" statement is that the reference 
                       number of a raster image cannot be appropriately retrieved
                       before actually reading in a raster image. */
                if (dumprig_opts->filter == DREFNUM)
                  {
                      int         ref_found = 0, m;

                          /* Determine if the image just read has the reference specified by the user. */
                      for (m = 0; m < dumprig_opts->num_chosen; m++)
                          if (dumprig_opts->filter_num[m] == rig_ref)
                              ref_found = 1;
                      if (!ref_found)
                        {	/* If no match, then the current image is
                                   not what the user wants and so skip it. */
                            HDfree((VOIDP) image);
                            continue;
                        }
                  }

                    /* If not all images are to be dumped out and the current image
                       is not what the user wants or if the user has specified a 
                       model and the model of the current image is not that one, then
                       skip the current image. */
                if (((dumprig_opts->filter == DINDEX) && (i != rig_chosen[x])) || 
                    (((ncomps * 8) != model) && (model != 0)))
                  {
                      HDfree((VOIDP) image);
                      continue;
                  }

                    /* Determine what to be dumped out. */
                switch (dumprig_opts->contents)
                  {
                  case DVERBOSE:
                  case DHEADER:
                      fprintf(fp, "Data model: %d-bit raster image ", ncomps * 8);
                      if (has_pal)
                          fprintf(fp, "with palette.\n");
                      else
                          fprintf(fp, "without palette.\n");
                      fprintf(fp, "\twidth=%d;  height=%d\n", (int) width, (int) height);
                      fprintf(fp, "\treference=%d\n", (int) rig_ref);
                      if (compressed)
                        {
                            fprintf(fp, "\t*data is compressed and the compression scheme is ");
                            switch (compr_type)
                              {
                              case DFTAG_RLE:
                                  fprintf(fp, "RLE compression.\n");
                                  break;
                              case DFTAG_IMCOMP:
                                  fprintf(fp, "IMCOMP conmpression.\n");
                                  break;
                              case DFTAG_JPEG:
                                  fprintf(fp, "JPEG conmpression (24-bit data).\n");
                                  break;
                              case DFTAG_GREYJPEG:
                                  fprintf(fp, "JPEG conmpression (8-bit data).\n");
                                  break;
                              }		/* switch */
                            fprintf(fp, "\n");
                        }	/* if (compressed) */
                      else
                          fprintf(fp, "\t*data is not compressed.\n");
                      if (dumprig_opts->contents == DHEADER)
                          break;
                  case DDATA:
                      if (dumprig_opts->contents != DDATA)
                          fprintf(fp, "\tData : \n");
                      for (count = 0; count < indent; count++)
                          putc(' ', fp);
                      dumpfull(DFNT_INT8, read_nelts, image, indent, fp);
                      HDfree((VOIDP) image);
                      break;
                  }		/* switch  */

          if(dumpall!=1 && i == rig_chosen[x])
            x++;

            }	/* for ndsets  */
          if (rig_chosen!=NULL)
            {
                HDfree(rig_chosen);
                rig_chosen=NULL;
            } /* end if */
          if (dumprig_opts->dump_to_file)
              fclose(fp);
      }		/* while argc  */
    return (0);
}	/* drig */

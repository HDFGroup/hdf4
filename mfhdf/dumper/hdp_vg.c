/***************************************************************************e
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
#include "vg.h"

#define NUM_VGS 20;

struct node
{
    int32       index, displayed;
    char        name[MAXNAMELEN];
    char      **children, **type;
};

void        display(struct node *ptr, int32 level, struct node **list,
                    int32 num_nodes, int32 firstchild);

static intn dvg(dump_info_t * dumpvg_opts, intn curr_arg, intn argc,
                char *argv[]);

int32       Vref_index(int32 file_id, int32 vg_ref);

void        vgdumpfull(int32 vg_id, int32 file_id, file_type_t ft, 
                 FILE * fp, struct node *aNode, int32 skip);

int32       Vstr_index(int32 file_id, char filter_str[MAXNAMELEN], int name,
                       int32 *find_ref, int32 *index);

void 
dumpvg_usage(intn argc, char *argv[])
{
    printf("Usage:\n");
    printf("%s dumpvg [-a|-i <index>|-n <name>|-r <ref>] [-dhv] [-o <filename> [-bx]] <filelist>\n", argv[0]);
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
init_dumpvg_opts(dump_info_t * dumpvg_opts)
{
    dumpvg_opts->filter = DALL;		/* default dump all VGs */
    dumpvg_opts->filter_num = FALSE;	/* not by reference nor by index */
    dumpvg_opts->filter_str = NULL;		/* no strings */
    dumpvg_opts->num_chosen = (-1);		/* default dump all items */
    dumpvg_opts->contents = DVERBOSE;	/* default dump all information */
    dumpvg_opts->dump_to_file = FALSE;	/* don't dump to output file */
    dumpvg_opts->file_type = DASCII;	/* default output is ASCII file */
    HDstrcpy(dumpvg_opts->file_name, "\0");
}	/* end init_list_opts() */

void 
do_dumpvg(intn curr_arg, intn argc, char *argv[], dump_opt_t * glob_opts)
{
    dump_info_t dumpvg_opts;	/* dumpvg options */
    char       *dummy1[MAXNAMELEN];
    int         dummy2;

    if (glob_opts->help == TRUE)
      {
          dumpvg_usage(argc, argv);
          return;
      }		/* end if */

    init_dumpvg_opts(&dumpvg_opts);
    if (parse_dumpvd_opts(&dumpvg_opts, &curr_arg, argc, argv, dummy1, &dummy2) == FAIL)
      {
          dumpvg_usage(argc, argv);
          return;
      }		/* end if */

    if (dvg(&dumpvg_opts, curr_arg, argc, argv) == FAIL)
      {
          printf("Failure in dumping Vgroup data\n");
          exit(1);
      }
}	/* end do_dumpvg() */

static intn 
dvg(dump_info_t * dumpvg_opts, intn curr_arg, intn argc, char *argv[])
{
    int32       file_id, vg_id, *vg_chosen=NULL;
    int32       num_vg_chosen;
    int32       i, k, n, x, y, j;
    int32       len, nvg, index, status;
    int32       vg_ref = -1, vg_tag;
    int32       level, num_nodes = 0, max_vgs, num_vgs;
    int         index_error = 0, dumpall = 0;
    char       *label_str;
    char        file_name[MAXFNLEN];
    char        vgclass[VGNAMELENMAX], vgname[VGNAMELENMAX];
    FILE       *fp;
    struct node **list, *ptr;

    while (curr_arg < argc)
      {		/* Examine each file. */
          HDstrcpy(file_name, argv[curr_arg]);
          curr_arg++;
          file_id = Hopen(file_name, DFACC_READ, 0);
          if (file_id == FAIL)
            {
                printf("Failure in open %s\n", file_name);
                exit(1);
            }
          Vstart(file_id);

          num_vg_chosen = dumpvg_opts->num_chosen;
          if (num_vg_chosen > 0)
            {
                if ((vg_chosen = (int32 *) HDmalloc(sizeof(int32) * num_vg_chosen)) == NULL)
                  {
                      printf("Memory allocation error\n");
                      exit(1);
                  }		/* end if */

                k = (-1);
                HDmemfill(vg_chosen, &k, sizeof(int32), num_vg_chosen);
            }	/* end if */

          k = 0;
          switch (dumpvg_opts->filter)
            {	/* Determine the vgroups to be dumped out. */
                int32       find_ref, number;

            case DINDEX:
                for (i = 0; i<dumpvg_opts->num_chosen; i++)
                  {
                      vg_chosen[i] = dumpvg_opts->filter_num[i];
                      k++;
                  }
                break;

            case DREFNUM:
                for (i = 0; i<dumpvg_opts->num_chosen; i++)
                  {
                      index = Vref_index(file_id, dumpvg_opts->filter_num[i]);
                      if (index == -1)
                        {
                            printf("Vgroup with reference number %d: not found\n", dumpvg_opts->filter_num[i]);
                            index_error = 1;
                        }
                      else
                        {
                            vg_chosen[k] = index;
                            k++;
                        }
                  }
                break;

            case DNAME:
                for (i = 0; i<dumpvg_opts->num_chosen; i++)
                  {
                      find_ref = (-1);
                      number = 0;
                      index = Vstr_index(file_id, dumpvg_opts->filter_str[i], 1, &find_ref, &number);
                      if (index == -1)
                        {
                            printf("Vgroup with name %s: not found\n", dumpvg_opts->filter_str[i]);
                            index_error = 1;
                        }
                      else
                        {
                            vg_chosen[k] = index;
                            k++;
                        }
                  }
                break;

            case DCLASS:
                for (i = 0; i<dumpvg_opts->num_chosen; i++)
                  {
                      int32       found = 0;

                      number = 0;
                      find_ref = (-1);
                      while ((index = Vstr_index(file_id, dumpvg_opts->filter_str[i], 0, &find_ref, &number)) != -1)
                        {
                            if (k < num_vg_chosen)
                                vg_chosen[k] = index;
                            else
                              {
                                  if ((vg_chosen = (int32 *) HDrealloc(vg_chosen,sizeof(int32) * (num_vg_chosen+1))) == NULL)
                                    {
                                        printf("Memory re-allocation error\n");
                                        exit(1);
                                    }		/* end if */
                                  vg_chosen[k] = index;
                                  num_vg_chosen++;
                              }
                            found = 1;
                            k++;
                        }
                      if (!found)
                        {
                            printf("Vgroup with class %s: not found\n", dumpvg_opts->filter_str[i]);
                            index_error = 1;
                        }
                  }
                break;

            case DALL:
                k= -1;
                break;

            default:
                printf("Unknown filter option\n");
                exit(1);
            }	/* switch */

          if (index_error && k==0)
            {
                if(vg_chosen!=NULL)
                  {
                      HDfree(vg_chosen);
                      vg_chosen=NULL;
                  } /* end if */
                file_id = Hclose(file_id);
                Vend(file_id);
                continue;
            } /* end if */
          num_vg_chosen=k;

              /* Get the name of the output file. */
          if (dumpvg_opts->dump_to_file)
              fp = fopen(dumpvg_opts->file_name, "w");
          else
              fp = stdout;

          if (dumpvg_opts->contents != DDATA)
              fprintf(fp, "File name: %s \n", file_name);

          nvg = 0;
          x = 0;
          if (num_vg_chosen == -1)
              dumpall = 1;
          else
              sort(vg_chosen, num_vg_chosen);

          max_vgs = NUM_VGS;
          if (( list = (struct node **) HDmalloc(sizeof(struct node *) * max_vgs)) == NULL)
            {
                printf("Not enough memory!\n");
                exit(-1);
            }
          for (j=0; j < max_vgs; j++)
              list[j] = NULL;

          num_vgs = 0;
          for (i = 0; (vg_ref = Vgetid(file_id, vg_ref)) != -1 && (dumpall!=0 || x<dumpvg_opts->num_chosen); i++)
            {
                int32       skip = FALSE;
                content_t   save = dumpvg_opts->contents;

                if ((!dumpall) && (i != vg_chosen[x]))
                  {
                      skip = TRUE;
                      dumpvg_opts->contents = DDATA;
                  }
                if (( vg_id = Vattach(file_id, vg_ref, "r")) == FAIL)
                  {
                      printf("Cannot open vg id=%d\n", (int) vg_ref);
                      continue;
                  }
                if (!skip)
                    x++;

                Vinquire(vg_id, &n, vgname);
                vg_tag = VQuerytag(vg_id);
                Vgetclass(vg_id, vgclass);

                num_vgs++;
                if (num_vgs > max_vgs)
                  {
                      max_vgs += NUM_VGS;
                      if ((list = HDrealloc(list, (uint32) sizeof(struct node) * max_vgs)) == NULL)
                        {
                            printf("Not enough memory!\n");
                            exit(-1);
                        }
                  }

                if (( list[i] = (struct node *) HDmalloc(sizeof(struct node))) == NULL)
                  {
                      printf("Not enough memory!\n");
                      exit(-1);
                  }
                num_nodes++;

                switch (dumpvg_opts->contents)
                  {
                  case DVERBOSE:	/* dump all information */
                      fprintf(fp, "\n");
                      fprintf(fp, "\nVgroup:%d\n", (int) i);
                      fprintf(fp, "     tag = %d; reference = %d;\n", (int) vg_tag, (int) vg_ref);
                      fprintf(fp, "     name = %s; class = %s;\n", vgname, vgclass);
                      fprintf(fp, "     number of entries = %d;\n", (int) n);
                      dumpattr(vg_id, 0, 0, dumpvg_opts->file_type, fp);  
                          /* Read in all of the annotations. */
                            {
                                /* Re-vamped label annotation handling to use new ANxxx interface 
                                 *  -georgev 6/11/97 */
                                int32  file_handle = FAIL;
                                int32  an_handle   = FAIL;
                                int32  *dlabels    = NULL;
                                char   *ann_label  = NULL;
                                int32  ann_len;
                                intn   num_dlabels;
                                uint16 atag;
                                uint16 aref;

                                /* start Annotation inteface */
                                if ((an_handle = ANstart(file_id)) == FAIL)
                                  {
                                      /* continue */
                                      goto done_alabel;
                                  }

                                /* Get number of data labels with this tag/ref */
                                num_dlabels = ANnumann(an_handle, AN_DATA_LABEL, vg_tag, vg_ref);
#ifdef AN_DEBUG
                                fprintf(stderr,"VG has %d labels \n", num_dlabels);
#endif

                                if (num_dlabels != 0)
                                  {
                                      /* allocate space for list of label annotation id's with this tag/ref */
                                      if ((dlabels = (int32 *)HDmalloc(num_dlabels * sizeof(int32))) == NULL)
                                        {
#ifdef AN_DEBUG
                                            fprintf(stderr,"failed to allocate space for %d labels \n", num_dlabels);
#endif
                                            goto done_alabel;
                                        }

                                      /* get list of label annotations id's with this tag/ref */
                                      if (ANannlist(an_handle, AN_DATA_LABEL, vg_tag, vg_ref, dlabels) != num_dlabels)
                                        {
#ifdef AN_DEBUG
                                            fprintf(stderr,"failed to get %d label list \n", num_dlabels);
#endif
                                            goto done_alabel;
                                        }

                                      /* loop through label list */
                                      for (i = 0; i < num_dlabels; i++)
                                        {
                                            if ((ann_len = ANannlen(dlabels[i])) == FAIL)
                                              {
#ifdef AN_DEBUG
                                                  fprintf(stderr,"failed to get %d label  length \n", i);
#endif
                                                  continue;
                                              }
        
                                            /* allocate space for label */
                                            if (ann_label == NULL)
                                              {
                                                  if ((ann_label = (char *)HDmalloc((ann_len+1)*sizeof(char))) == NULL)
                                                    {
#ifdef AN_DEBUG
                                                        fprintf(stderr,"failed to allocate space for label %d \n", i);
#endif
                                                        continue;
                                                    }
                                                  HDmemset(ann_label,'\0', ann_len+1);
                                              }
      
                                            /* read label */
                                            if (ANreadann(dlabels[i], ann_label, ann_len+1) == FAIL)
                                              {
#ifdef AN_DEBUG
                                                  fprintf(stderr,"failed to read %d label \n", i);
#endif
                                                  continue;
                                              }

                                            printf("\t  label: %s\n", ann_label);

                                            /* end access */
                                            ANendaccess(dlabels[i]);

                                            /* free buffer */
                                            if(ann_label != NULL)
                                              {
                                                  HDfree(ann_label);
                                                  ann_label = NULL;
                                              }
                                        }

                                  } /* end if labels */

                              done_alabel:
                                if(dlabels != NULL)
                                    HDfree(dlabels);

                                if(an_handle != FAIL)
                                    ANend(an_handle);

                            }

                            {
                                /* Re-vamped desc annotation handling to use new ANxxx interface 
                                 *  -georgev 6/11/97 */
                                int32  file_handle = FAIL;
                                int32  an_handle   = FAIL;
                                int32  *ddescs    = NULL;
                                char   *ann_desc  = NULL;
                                int32  ann_len;
                                intn   num_ddescs;
                                uint16 atag;
                                uint16 aref;
                                char   hremark[30] = "";

                                /* start Annotation inteface */
                                if ((an_handle = ANstart(file_id)) == FAIL)
                                  {
                                      goto done_adesc;
                                  }
                                /* Get number of data descs with this tag/ref */
                                num_ddescs = ANnumann(an_handle, AN_DATA_DESC, vg_tag, vg_ref);
#ifdef AN_DEBUG
                                fprintf(stderr,"VG has %d descs \n", num_ddescs);
#endif
                                if (num_ddescs != 0)
                                  {
                                      /* allocate space for list of desc annotation id's with this tag/ref */
                                      if ((ddescs = (int32 *)HDmalloc(num_ddescs * sizeof(int32))) == NULL)
                                        {
#ifdef AN_DEBUG
                                            fprintf(stderr,"failed to allocate space for %d descs \n", num_ddescs);
#endif
                                            goto done_adesc;
                                        }

                                      /* get list of desc annotations id's with this tag/ref */
                                      if (ANannlist(an_handle, AN_DATA_DESC, vg_tag, vg_ref, ddescs) != num_ddescs)
                                        {
#ifdef AN_DEBUG
                                            fprintf(stderr,"failed to get %d descs list \n", num_ddescs);
#endif
                                            goto done_adesc;
                                        }

                                      /* loop through desc list */
                                      for (i = 0; i < num_ddescs; i++)
                                        {
                                            if ((ann_len = ANannlen(ddescs[i])) == FAIL)
                                              {
#ifdef AN_DEBUG
                                                  fprintf(stderr,"failed to get %d desc  length \n", i);
#endif
                                                  continue;
                                              }
        
                                            /* allocate space for desc */
                                            if (ann_desc == NULL)
                                              {
                                                  if ((ann_desc = (char *)HDmalloc((ann_len+1)*sizeof(char))) == NULL)
                                                    {
#ifdef AN_DEBUG
                                                        fprintf(stderr,"failed to allocate space for desc %d \n", i);
#endif
                                                        continue;
                                                    }
                                                  HDmemset(ann_desc,'\0', ann_len+1);
                                              }
      
                                            /* read desc */
                                            if (ANreadann(ddescs[i], ann_desc, ann_len+1) == FAIL)
                                              {
#ifdef AN_DEBUG
                                                  fprintf(stderr,"failed to read %d desc \n", i);
#endif
                                                  continue;
                                              }

                                            printf("\t  Description: %s\n", ann_desc);

                                            /* end access */
                                            ANendaccess(ddescs[i]);

                                            /* free buffer */
                                            if(ann_desc != NULL)
                                              {
                                                  HDfree(ann_desc);
                                                  ann_desc = NULL;
                                              }
                                        }

                                  } /* end if descs */

                              done_adesc:
                                if(ddescs != NULL)
                                    HDfree(ddescs);

                                if(an_handle != FAIL)
                                    ANend(an_handle);

                            }


                  case DHEADER:	/* header only, no annotations nor data */
                      if (dumpvg_opts->contents == DHEADER)
                          break;

                  case DDATA:	/* data only */
                      dumpvg_opts->contents = save;
                      if ((dumpvg_opts->contents != DDATA) && (!skip))
                          fprintf(fp, "Entries:-\n");
                      vgdumpfull(vg_id, file_id, dumpvg_opts->file_type,
                                  fp, list[i], skip);
                      if (dumpvg_opts->contents == DDATA)
                          fprintf(fp, "\n");
                  }		/* switch */
                Vdetach(vg_id);
                list[i]->index = i;
                HDstrcpy(list[i]->name, vgname);
                list[i]->displayed = FALSE;
            }	/* for */
          Vend(file_id);
          if (dumpvg_opts->contents != DDATA)
            {
                printf("\n\nGraphical representation of the file:-\n");
                printf("(vg#: vgroup;   vd: vdata)\n\n");
                for (y = 0; y < num_nodes; y++)
                  {
                      int32       firstchild = FALSE;

                      level = -1;
                      ptr = list[y];
                      printf("   ");
                      display(ptr, level, list, num_nodes, firstchild);
                      printf("\n");
                  }		/* for */
            }
          if (list != NULL)
            {
                for (i=0; i < max_vgs; i++)
                  {
                      if (list[i] != NULL)
                          HDfree(list[i]);
                  }
                HDfree(list);
            }
          if (vg_chosen!=NULL)
            {
                HDfree(vg_chosen);
                vg_chosen=NULL;
            } /* end if */
          Vend(file_id);
          Hclose(file_id);
          if (dumpvg_opts->dump_to_file)
              fclose(fp);
      } /* while (curr_arg < argc)  */
    return (0);
}	/* dvg */

void 
vgdumpfull(int32 vg_id, int32 file_id, file_type_t ft, FILE * fp,  
            struct node *aNode, int32 skip)
{
    int32       vgt, vgotag, vgoref;
    int32       t, vsid, ne, tag;
    int         found = 0, num_entries;
    int32       vs, nv, vsotag, vsoref, interlace, vsize;
    char        fields[VSFIELDMAX*FIELDNAMELENMAX], vsname[MAXNAMELEN], vsclass[VSNAMELENMAX];
    char        vgname[VGNAMELENMAX], vgclass[VGNAMELENMAX];
    char       *name;
    int32       z, lastItem, count = 0;
    char       *tempPtr, *ptr, string[MAXNAMELEN], tempflds[VSFIELDMAX*FIELDNAMELENMAX];

    num_entries = Vntagrefs(vg_id);
    aNode->children = (char **) HDmalloc(sizeof(char *) * (num_entries+1));
    if (aNode->children == NULL)
      {
          printf("Not enough memory!\n");
          exit(-1);
      }
    aNode->type = (char **) HDmalloc(sizeof(char *) * (num_entries+1));
    if (aNode->type == NULL)
      {
          printf("Not enough memory!\n");
          exit(-1);
      }
    for (t = 0; t < num_entries; t++)
      {
          Vgettagref(vg_id, t, &tag, &vsid);
          found = 1;
          if (tag == DFTAG_VG)
            {
                vgt = Vattach(file_id, vsid, "r");
                if (vgt == FAIL)
                  {
                      printf("Cannot open vg id=%d\n", (int) vsid);
                      continue;
                  }
                Vinquire(vgt, &ne, vgname);
                if (HDstrlen(vgname) == 0)
                    HDstrcat(vgname, "NoName");
                vgotag = VQuerytag(vgt);
                vgoref = VQueryref(vgt);
                Vgetclass(vgt, vgclass);
                if (!skip)
                  {
                      fprintf(fp, "     #%d (Vgroup)\n\ttag = %d;",
                              (int) t, (int) vgotag);
                      fprintf(fp, "reference = %d;\n\tnumber of entries = %d;\n",
                              (int) vgoref, (int) ne);
                      fprintf(fp, "\tname = %s; class = %s\n", vgname, vgclass);
                      dumpattr(vgt, 0, 0, ft, fp);
                  }
                Vdetach(vgt);
                aNode->children[t] = (char *) HDmalloc(sizeof(char) * (HDstrlen(vgname)+1));
                if (aNode->children[t] == NULL)
                  {
                      printf("Not enough memory!\n");
                      exit(-1);
                  }
                HDstrcpy(aNode->children[t], vgname);
                aNode->type[t] = (char *) HDmalloc(sizeof(char) * 3);
                if (aNode->type[t] == NULL)
                  {
                      printf("Not enough memory!\n");
                      exit(-1);
                  }
                HDstrcpy(aNode->type[t], "vg");
            }	/* if */
          else if (tag == VSDESCTAG)
            {
                vs = VSattach(file_id, vsid, "r");
                if (vs == FAIL)
                  {
                      printf("Cannot open vs id=%d\n", (int) vsid);
                      continue;
                  }
                VSinquire(vs, &nv, &interlace, fields, &vsize, vsname);
                vsotag = VSQuerytag(vs);
                vsoref = VSQueryref(vs);
                if (HDstrlen(vsname) == 0)
                    HDstrcat(vsname, "NoName");
                VSgetclass(vs, vsclass);

                if (!skip)
                  {
                      fprintf(fp, "     #%d (Vdata)\n", (int) t);
                      fprintf(fp, "\ttag = %d; ", (int) vsotag);
                      fprintf(fp, "reference = %d; \n", (int) vsoref);
                      fprintf(fp, "\tnumber of records = %d; ", (int) nv);
                      fprintf(fp, "interlace = %d;\n", (int) interlace);
                      fprintf(fp, "\tfields = [");

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
                                  fprintf(fp, "\n\t          ");
                                  count = 0;
                              }
                            fprintf(fp, "%s", string);
                            if (!lastItem)
                                fprintf(fp, ", ");
                            ptr = tempPtr + 1;
                        }
                      fprintf(fp, "];\n");
                      fprintf(fp, "\trecord size (in bytes) = %d;\n", (int)vsize);
                      fprintf(fp, "\tname = %s; class = %s;\n", vsname, vsclass);
                  }
                fprintf(fp, "\ttotal number of attributes = %d.\n", VSnattrs(vs));
                VSdetach(vs);
                aNode->children[t] = (char *) HDmalloc(sizeof(char) * (HDstrlen(vsname)+1));
                if (aNode->children[t] == NULL)
                  {
                      printf("Not enough memory!\n");
                      exit(-1);
                  }
                HDstrcpy(aNode->children[t], vsname);

                aNode->type[t] = (char *) HDmalloc(sizeof(char) * 3);
                if (aNode->type[t] == NULL)
                  {
                      printf("Not enough memory!\n");
                      exit(-1);
                  }
                HDstrcpy(aNode->type[t], "vd");
            }
          else
            {
                char *cptr = NULL;
                int slen;
                int found=1;

                cptr = HDgettagsname((uint16) tag);
                if (!cptr)
                  {
                      slen = HDstrlen("Unknown Tag")+1;
                      cptr = "Unknown Tag";
                      found = 0;
                  }
                else
                    slen = HDstrlen(cptr)+1;

                if ((name = (char *)HDmalloc(sizeof(char) * (HDstrlen(cptr)+1))) == NULL)
                  {
                      printf("Not enough memory!\n");
                      exit(-1);
                  }

                HDstrcpy(name,cptr);
                if (found)
                    HDfree(cptr);
                if (!skip)
                  {
                      fprintf(fp, "     #%d (%s)\n", (int) t, name);
                      fprintf(fp, "\ttag = %d; reference = %d;\n", (int) tag, (int) vsid);
                  }
                aNode->children[t] = (char *) HDmalloc(sizeof(char) * 4);
                if (aNode->children[t] == NULL)
                  {
                      printf("Not enough memory!\n");
                      exit(-1);
                  }
                HDstrcpy(aNode->children[t], "***");

                if (!strcmp(name, "Unknown Tag"))
                  {
                      aNode->type[t] = (char *) HDmalloc(sizeof(char) * 15);
                      if (aNode->type[t] == NULL)
                        {
                            printf("Not enough memory!\n");
                            exit(-1);
                        }
                      HDstrcpy(aNode->type[t], "Unknown Object");
                  }
                else
                    aNode->type[t] = name;

            }
      }		/* for */
    aNode->children[num_entries] = NULL;
    if (!found)
        printf("     None.\n");
}	/* vgdumpfull */

void 
display(struct node *ptr, int32 level, struct node **list, int32 num_nodes, int32 firstchild)
{
    char       *name;
    int         i, k, x, z, num = 1, newline = TRUE;

    level++;
    if (!firstchild)
        for (k = 0; k < level; k++)
            for (z = 0; z < num; z++)
                printf("\t");
    else
        for (z = 0; z < num; z++)
            printf("\t");
    if (level > 0)
        printf("-- ");
    printf("vg%d ", (int)ptr->index);
    if (ptr->children[0] == NULL)
        printf("\n");
    if (!ptr->displayed)
      {
          for (i = 0; ptr->children[i] != NULL; i++)
            {
                if (i == 0)
                    firstchild = TRUE;
                else
                    firstchild = FALSE;
                name = ptr->children[i];
                if ((HDstrcmp(ptr->type[i], "vd")) &&
                    (HDstrcmp(ptr->children[i], "***")))
                  {
                      x = 0;
                      while (HDstrcmp(name, list[x]->name))
                          x++;
                      display(list[x], level, list, num_nodes, firstchild);
                  }		/* if */
                else
                  {
                      if (i > 0)
                        {
                            for (k = 0; k < level + 1; k++)
                                for (z = 0; z < num; z++)
                                    printf("\t");
                        }
                      if (firstchild)
                          for (z = 0; z < num; z++)
                              printf("\t");
                      printf("-- ");
                          /* printf("%s%d ", ptr->type[i], i); */
                      printf("%s  \n", ptr->type[i]);
                      newline = FALSE;
                  }
            }	/* for (i...) */
              /* ptr->displayed = TRUE; */
      }		/* if (!ptr->displayed) */
    else
        printf("\n");
}	/* display */

int32 
Vref_index(int32 file_id, int32 vg_ref)
{
    int32       find_ref = -1;
    int         index = 0;

    while ((find_ref = Vgetid(file_id, find_ref)) != -1)
      {
          if (find_ref == vg_ref)
              return(index);
          index++;
      }
    return (-1);
}	/* Vref_index */

int32 
Vstr_index(int32 file_id, char filter_str[MAXNAMELEN],
           int name, int32 *find_ref, int32 *index)
{
    int32       vg_id;
    char        vg_name[MAXNAMELEN];

    while ((*find_ref = Vgetid(file_id, *find_ref)) != -1)
      {
          vg_id = Vattach(file_id, *find_ref, "r");
          if (name)
              Vgetname(vg_id, vg_name);
          else
              Vgetclass(vg_id, vg_name);
          Vdetach(vg_id);
          if (HDstrcmp(vg_name, filter_str) == 0)
              return(*index);
          (*index)++;
      }
    return (-1);
}

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
    int32  index;
    int32  displayed;
    char   name[MAXNAMELEN];
    char **children;
    char **type;
};


static void 
dumpvg_usage(intn argc, 
             char *argv[])
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

static void 
init_dumpvg_opts(dump_info_t *dumpvg_opts)
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

static int32 
Vref_index(int32 file_id, 
           int32 vg_ref)
{
    int32  find_ref = -1;
    int    index = 0;
    int32  ret_value = FAIL;   

    while ((find_ref = Vgetid(file_id, find_ref)) != FAIL)
      {
          if (find_ref == vg_ref)
            {
              ret_value = index;
              goto done; /* found , done */
            }
          index++;
      }

done:
    if (ret_value == FAIL)
      { /* Failure cleanup */
      }
    /* Normal cleanup */
    
    return ret_value;
}	/* Vref_index */

static int32 
Vstr_index(int32 file_id, 
           char filter_str[MAXNAMELEN],
           int name, 
           int32 *find_ref, 
           int32 *index)
{
    int32  vg_id = FAIL;
    char   vg_name[MAXNAMELEN];
    int32  ret_value = FAIL;

    while ((*find_ref = Vgetid(file_id, *find_ref)) != FAIL)
      {
          vg_id = Vattach(file_id, *find_ref, "r");
          if (FAIL == vg_id)
              goto done; /* FAIL */

          if (name)
            {
              if (FAIL == Vgetname(vg_id, vg_name))
                  goto done; /* FAIL */
            }
          else
            {
              if (FAIL == Vgetclass(vg_id, vg_name))
                  goto done; /* FAIL */
            }

          if (FAIL == Vdetach(vg_id))
              goto done; /* FAIL */

          if (HDstrcmp(vg_name, filter_str) == 0)
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
} /* Vstr_index() */


static void 
display(struct node  *ptr, 
        int32         level, 
        struct node **list, 
        int32         num_nodes, 
        int32         firstchild)
{
    char *name = NULL;
    int   i, k, x, z;
    int   num = 1;
    int   newline = TRUE;

    level++;
    if (!firstchild)
      {
        for (k = 0; k < level; k++)
          {
            for (z = 0; z < num; z++)
                printf("\t");
          }
      }
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

                if ((HDstrcmp(ptr->type[i], "vd")) 
                    && (HDstrcmp(ptr->children[i], "***")))
                  {
                      x = 0;
                      while (HDstrcmp(name, list[x]->name))
                        {
                          x++;
                        }

                      display(list[x], level, list, num_nodes, firstchild);
                  }		
                else
                  {
                      if (i > 0)
                        {
                            for (k = 0; k < level + 1; k++)
                              {
                                for (z = 0; z < num; z++)
                                    printf("\t");
                              }
                        }

                      if (firstchild)
                        {
                          for (z = 0; z < num; z++)
                              printf("\t");
                        }

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

static intn
vgdumpfull(int32        vg_id, 
           char         *file_name,
           int32        file_id, 
           file_type_t  ft, 
           FILE        *fp,  
           struct node *aNode, 
           int32        skip)
{
    int32  vgt;
    int32  vgotag;
    int32  vgoref;
    int32  t;
    int32  vsid = FAIL;
    int32  ne;
    int32  tag;
    int    found = 0;
    int    num_entries;
    int32  vs;
    int32  nv;
    int32  vsotag;
    int32  vsoref;
    int32  interlace;
    int32  vsize;
    char   fields[VSFIELDMAX*FIELDNAMELENMAX];
    char   vsname[MAXNAMELEN];
    char   vsclass[VSNAMELENMAX];
    char   vgname[VGNAMELENMAX];
    char   vgclass[VGNAMELENMAX];
    char  *name = NULL;
    int32  z,i;
    int32  lastItem;
    int32  count = 0;
    char  *tempPtr;
    char  *ptr;
    char   string[MAXNAMELEN];
    char   tempflds[VSFIELDMAX*FIELDNAMELENMAX];
    int32  an_handle = FAIL;
    intn   ret_value = SUCCEED;

    num_entries = Vntagrefs(vg_id);
    if (FAIL == num_entries)
      {
          fprintf(stderr,"Vntagrefs failed for vg_id(%d) in file %s\n", 
                  (int) vg_id, file_name);
          ret_value = FAIL;
          goto done;
      } 

    aNode->children = (char **) HDmalloc(sizeof(char *) * (num_entries+1));
    if (aNode->children == NULL)
      {
          fprintf(stderr,"Not enough memory!\n");
          ret_value = FAIL;
          goto done;
      }

    for (i = 0; i < num_entries; i++)
        aNode->children[i] = NULL;

    aNode->type = (char **) HDmalloc(sizeof(char *) * (num_entries+1));
    if (aNode->type == NULL)
      {
          fprintf(stderr,"Not enough memory!\n");
          ret_value = FAIL;
          goto done;
      }

    for (i = 0; i < num_entries; i++)
        aNode->type[i] = NULL;

    for (t = 0; t < num_entries; t++)
      {
          if (FAIL == Vgettagref(vg_id, t, &tag, &vsid))
            {
                fprintf(stderr,"Vgettagref failed for vg_id(%d) in file %s\n", 
                        (int) vg_id, file_name);
                ret_value = FAIL;
                goto done;
            }

          found = 1;
          if (tag == DFTAG_VG)
            { /* vgroup */
                vgt = Vattach(file_id, vsid, "r");
                if (vgt == FAIL)
                  {
                      fprintf(stderr,"Vattach failed for vsid(%d) for file %s\n", 
                              (int) vsid, file_name);
                      ret_value = FAIL;
                      goto done;
                  }

                if (FAIL == Vinquire(vgt, &ne, vgname))
                  {
                      fprintf(stderr,"Vinquire failed for vg_id(%d) for file %s\n", 
                              (int) vgt, file_name);
                      ret_value = FAIL;
                      goto done;
                  }

                if (HDstrlen(vgname) == 0)
                    HDstrcat(vgname, "NoName");

                if (FAIL == (vgotag = VQuerytag(vgt)))
                  {
                      fprintf(stderr,"VQuerytag failed for vg_id(%d) for file %s\n",
                              (int) vgt, file_name);
                      ret_value = FAIL;
                      goto done;
                  }

                if (FAIL == (vgoref = VQueryref(vgt)))
                  {
                      fprintf(stderr,"VQueryref failed for vg_id(%d) for file %s\n",
                              (int) vgt, file_name);
                      ret_value = FAIL;
                      goto done;
                  }


                if (FAIL == Vgetclass(vgt, vgclass))
                  {
                      fprintf(stderr,"Vgetclass failed for vg_id(%d) for file %s\n",
                              (int) vgt, file_name);
                      ret_value = FAIL;
                      goto done;
                  }


                if (!skip)
                  {
                      fprintf(fp, "     #%d (Vgroup)\n\ttag = %d;",
                              (int) t, (int) vgotag);
                      fprintf(fp, "reference = %d;\n\tnumber of entries = %d;\n",
                              (int) vgoref, (int) ne);
                      fprintf(fp, "\tname = %s; class = %s\n", vgname, vgclass);

                      /* dump attributes for vgroup */
                      if (FAIL == dumpattr(vgt, 0, 0, ft, fp))
                        {
                            fprintf(stderr,"Failed to dump attributes for vg_id(%d) in file %s\n", 
                                    (int) vgt, file_name);
                            ret_value = FAIL;
                            goto done;
                        }

                      /* dump all of the annotations for this vgroup. */
                      /* start Annotation inteface */
                      if ((an_handle = ANstart(file_id)) == FAIL)
                        {
                            fprintf(stderr,"ANstart failed on file_id(%d) for file %s\n", 
                                    (int)file_id, file_name);
                            ret_value = FAIL;
                            goto done;
                        }
                                  
                      /* print labels of vgroup if any */
                      if (FAIL == print_data_labels(file_name, an_handle,vgotag, vgoref))
                        {
                            fprintf(stderr,"Failed to print data labels for vg_ref(%d) in file %s\n", 
                                    (int) vgoref, file_name);
                            ret_value = FAIL;
                            goto done;
                        }

                      /* print descriptions of vgroup if any */
                      if (FAIL == print_data_descs(file_name, an_handle,vgotag, vgoref))
                        {
                            printf("Failed to print data descriptions for vg-ref(%d) in file %s\n", 
                                   (int) vgoref, file_name);
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
                  } /* if !skip */

                if (FAIL == Vdetach(vgt))
                  {
                      printf("Vdetach failed for vg_id(%d) for file %s\n", 
                             (int) vgt, file_name);
                      ret_value = FAIL;
                      goto done;
                  }

                aNode->children[t] = (char *) HDmalloc(sizeof(char) * (HDstrlen(vgname)+1));
                if (aNode->children[t] == NULL)
                  {
                      fprintf(stderr,"Not enough memory!\n");
                      ret_value = FAIL;
                      goto done;
                  }
                HDstrcpy(aNode->children[t], vgname);

                aNode->type[t] = (char *) HDmalloc(sizeof(char) * 3);
                if (aNode->type[t] == NULL)
                  {
                      fprintf(stderr,"Not enough memory!\n");
                      ret_value = FAIL;
                      goto done;
                  }
                HDstrcpy(aNode->type[t], "vg");

            }	/* if */
          else if (tag == VSDESCTAG)
            { /* vdata */
                vs = VSattach(file_id, vsid, "r");
                if (vs == FAIL)
                  {
                      fprintf(stderr,"VSattach failed on vs_ref(%d) for file %s\n", 
                              (int) vsid, file_name);
                      ret_value = FAIL;
                      goto done;
                  }

                if (FAIL == VSinquire(vs, &nv, &interlace, fields, &vsize, vsname))
                  {
                      fprintf(stderr,"VSinquire failed on vsid(%d) for file %s\n", 
                              (int) vs, file_name);
                      ret_value = FAIL;
                      goto done;
                  }

                if (FAIL == (vsotag = VSQuerytag(vs)))
                  {
                      fprintf(stderr,"VSQuerytag failed on vsid(%d) for file %s\n", 
                              (int) vs, file_name);
                      ret_value = FAIL;
                      goto done;
                  }

                if (FAIL == (vsoref = VSQueryref(vs)))
                  {
                      fprintf(stderr,"VSQueryref failed on vsid(%d) for file %s\n",
                              (int) vs, file_name);
                      ret_value = FAIL;
                      goto done;
                  }

                if (HDstrlen(vsname) == 0)
                    HDstrcat(vsname, "NoName");

                if (FAIL == VSgetclass(vs, vsclass))
                  {
                      fprintf(stderr,"VSgetclass failed on vsid=%d\n for file %s", 
                              (int) vs, file_name);
                      ret_value = FAIL;
                      goto done;
                  }

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

                /* hmm.....just print the number of attributes but not dump them */
                fprintf(fp, "\ttotal number of attributes = %d.\n", VSnattrs(vs));

                if (FAIL == VSdetach(vs))
                  {
                      fprintf(stderr, "VSdetach failed on vsid(%d) for file %s\n", 
                              (int) vsid, file_name);
                      ret_value = FAIL;
                      goto done;
                  }

                aNode->children[t] = (char *) HDmalloc(sizeof(char) * (HDstrlen(vsname)+1));
                if (aNode->children[t] == NULL)
                  {
                      fprintf(stderr,"Not enough memory!\n");
                      ret_value = FAIL;
                      goto done;
                  }
                HDstrcpy(aNode->children[t], vsname);

                aNode->type[t] = (char *) HDmalloc(sizeof(char) * 3);
                if (aNode->type[t] == NULL)
                  {
                      fprintf(stderr,"Not enough memory!\n");
                      ret_value = FAIL;
                      goto done;
                  }
                HDstrcpy(aNode->type[t], "vd");
            }
          else /* else something else */
            {
                name = HDgettagsname((uint16) tag);
                if (!name)
                    name = HDstrdup("Unknown Tag");

                if (!skip)
                  {
                      fprintf(fp, "     #%d (%s)\n", (int) t, name);
                      fprintf(fp, "\ttag = %d; reference = %d;\n", (int) tag, (int) vsid);
                  }

                aNode->children[t] = (char *) HDmalloc(sizeof(char) * 4);
                if (aNode->children[t] == NULL)
                  {
                      fprintf(stderr,"Not enough memory!\n");
                      ret_value = FAIL;
                      goto done;
                  }
                HDstrcpy(aNode->children[t], "***");

                if (!strcmp(name, "Unknown Tag"))
                  {
                      aNode->type[t] = (char *) HDmalloc(sizeof(char) * 15);
                      if (aNode->type[t] == NULL)
                        {
                            fprintf(stderr,"Not enough memory!\n");
                            ret_value = FAIL;
                            goto done;
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

done:
    if (ret_value == FAIL)
      { /* Failure cleanup */
          if (aNode->children != NULL)
            {
                for (i = 0; i < num_entries; i++)
                  {
                      if (aNode->children[i] != NULL)
                          HDfree(aNode->children[i]);
                  }
            }
          if (aNode->type != NULL)
            {
                for (i = 0; i < num_entries; i++)
                  {
                      if (aNode->type[i] != NULL)
                          HDfree(aNode->type[i]);
                  }
            }
          if (an_handle != FAIL)
              ANend(an_handle);
      }
    /* Normal cleanup */
    
    return ret_value;
}	/* vgdumpfull */

static intn 
dvg(dump_info_t *dumpvg_opts, 
    intn         curr_arg, 
    intn         argc, 
    char        *argv[])
{
    int32       file_id = FAIL;
    int32       vg_id = FAIL;
    int32      *vg_chosen = NULL;
    int32       num_vg_chosen;
    int32       i, k, n, x, y, j;
    int32       nvg;
    int32       index;
    int32       vg_ref = -1;
    int32       vg_tag;
    int32       level;
    int32       num_nodes = 0;
    int32       max_vgs=0;
    int32       num_vgs;
    int         index_error = 0;
    int         dumpall = 0;
    char        file_name[MAXFNLEN];
    char        vgclass[VGNAMELENMAX];
    char        vgname[VGNAMELENMAX];
    FILE       *fp = NULL;
    struct node **list = NULL;
    struct node *ptr = NULL;
    int32       an_handle = FAIL;
    intn        ret_value = SUCCEED;

    while (curr_arg < argc)
      {	/* Examine each file. */
          HDstrcpy(file_name, argv[curr_arg]);
          curr_arg++;

          file_id = Hopen(file_name, DFACC_READ, 0);
          if (file_id == FAIL)
            {
                fprintf(stderr, "Failure in open %s\n", file_name);
                ret_value = FAIL;
                goto done;
            }

          if (FAIL == Vstart(file_id))
            {
                fprintf(stderr,"Vstart failed for file %s\n", file_name);
                ret_value = FAIL;
                goto done;
            }

          num_vg_chosen = dumpvg_opts->num_chosen;
          if (num_vg_chosen > 0)
            {
                if ((vg_chosen = (int32 *) HDmalloc(sizeof(int32) * num_vg_chosen)) == NULL)
                  {
                      fprintf(stderr,"Memory allocation error\n");
                      ret_value = FAIL;
                      goto done;
                  }	

                k = (-1);
                HDmemfill(vg_chosen, &k, sizeof(int32), num_vg_chosen);
            }	/* end if */

          k = 0;
          switch (dumpvg_opts->filter)
            {	/* Determine the vgroups to be dumped out. */
                int32 find_ref;
                int32 number;

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
                            index_error = 1; /* error */
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
                            index_error = 1; /* error */
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
                                        fprintf(stderr,"Memory re-allocation error\n");
                                        ret_value = FAIL;
                                        goto done;
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
                            index_error = 1; /* error */
                        }
                  }
                break;

            case DALL:
                k= -1;
                break;

            default:
                printf("Unknown filter option\n");
                ret_value = FAIL;
                goto done;
                break;
            }	/* switch */

          if (index_error && k==0)
            {
                if(vg_chosen!=NULL)
                  {
                      HDfree(vg_chosen);
                      vg_chosen=NULL;
                  } /* end if */

                Vend(file_id);
                Hclose(file_id);
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
                fprintf(stderr,"Not enough memory!\n");
                ret_value = FAIL;
                goto done;
            }

          for (j = 0; j < max_vgs; j++)
              list[j] = NULL;

          /* for all vgroups? */
          num_vgs = 0;
          for (i = 0; 
               (vg_ref = Vgetid(file_id, vg_ref)) != -1 
                   && (dumpall!=0 || x<dumpvg_opts->num_chosen); 
               i++)
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
                      fprintf(stderr,"Vattach open vg id=%d\n", (int) vg_ref);
                      ret_value = FAIL;
                      goto done;
                  }

                if (!skip)
                    x++;

                if (FAIL == Vinquire(vg_id, &n, vgname))
                  {
                      fprintf(stderr,"Vinquire failed on vg_id(%d) in file %s\n", 
                              (int) vg_id, file_name);
                      ret_value = FAIL;
                      goto done;
                  }

                if (FAIL == (vg_tag = VQuerytag(vg_id)))
                  {
                      fprintf(stderr,"VQuerytag failed on vg_id(%d) in file %s\n", 
                              (int) vg_id, file_name);
                      ret_value = FAIL;
                      goto done;
                  }

                if (FAIL == Vgetclass(vg_id, vgclass))
                  {
                      fprintf(stderr,"Vgetclass failed on vg_id(%d) in file %s\n", 
                              (int) vg_id, file_name);
                      ret_value = FAIL;
                      goto done;
                  }

                num_vgs++;
                if (num_vgs > max_vgs)
                  {
                      max_vgs += NUM_VGS;
                      if ((list = HDrealloc(list, (uint32) sizeof(struct node) * max_vgs)) == NULL)
                        {
                            fprintf(stderr,"Not enough memory!\n");
                            ret_value = FAIL;
                            goto done;
                        }
                  }

                if (( list[i] = (struct node *) HDmalloc(sizeof(struct node))) == NULL)
                  {
                      fprintf(stderr,"Not enough memory!\n");
                      ret_value = FAIL;
                      goto done;
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

                      /* dump attributes of vgroup */
                      if (FAIL == dumpattr(vg_id, 0, 0, dumpvg_opts->file_type, fp))
                        {
                            fprintf(stderr,"Failed to dump attributes for vg_id(%d) in file %s\n", 
                                    (int) vg_id, file_name);
                            ret_value = FAIL;
                            goto done;
                        }
  
                      /* Read in all of the annotations. */
                      /* Re-vamped annotation handling to use new ANxxx interface 
                       *  -georgev 6/11/97 */
                      /* start Annotation inteface */
                      if ((an_handle = ANstart(file_id)) == FAIL)
                        {
                            fprintf(stderr,"ANstart failed on file_id(%d) for file %s\n", 
                                    (int)file_id, file_name);
                            ret_value = FAIL;
                            goto done;
                        }
                                  
                      /* print labesl of vdata if any */
                      if (FAIL == print_data_labels(file_name, an_handle,vg_tag, vg_ref))
                        {
                            fprintf(stderr,"Failed to print data labels for vg_ref(%d) in file %s\n", 
                                    (int) vg_ref, file_name);
                            ret_value = FAIL;
                            goto done;
                        }

                      /* print descriptions of vdata if any */
                      if (FAIL == print_data_descs(file_name, an_handle,vg_tag, vg_ref))
                        {
                            fprintf(stderr,"Failed to print data descriptions for vg_ref(%d) in file %s\n", 
                                    (int) vg_ref,file_name);
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

                      /* fall through to next case */

                  case DHEADER:	/* header only, no annotations nor data */
                      if (dumpvg_opts->contents == DHEADER)
                          break;

                  case DDATA:	/* data only */
                      dumpvg_opts->contents = save;
                      if ((dumpvg_opts->contents != DDATA) && (!skip))
                          fprintf(fp, "Entries:-\n");

                      if (FAIL == vgdumpfull(vg_id, file_name, file_id, dumpvg_opts->file_type,
                                  fp, list[i], skip))
                        {
                            fprintf(stderr,"Failed to dump vgroup data for vg_id(%d) in file %s\n", 
                                    (int) vg_id, file_name);
                            ret_value = FAIL;
                            goto done;
                        }
  
                      if (dumpvg_opts->contents == DDATA)
                          fprintf(fp, "\n");
                      break;
                  default:
                      printf("dumping vgroup, unknown option \n");
                      ret_value = FAIL;
                      goto done;
                      break;
                  }		/* switch */

                if (FAIL == Vdetach(vg_id))
                  {
                      fprintf(stderr,"Vdetach failed for vg_id(%d) in file %s\n", 
                              (int) vg_id, file_name);
                      ret_value = FAIL;
                      goto done;
                  }
  
                list[i]->index = i;
                HDstrcpy(list[i]->name, vgname);
                list[i]->displayed = FALSE;
            }	/* for all vgroups */

  
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
                      /* print tree */
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

          if (dumpvg_opts->dump_to_file)
              fclose(fp);

          if (FAIL == Vend(file_id))
            {
                fprintf(stderr,"Vend failed for file_id(%d) or file %s\n", 
                        (int) file_id, file_name);
                ret_value = FAIL;
                goto done;
            }

          if (FAIL == Hclose(file_id))
            {
                fprintf(stderr,"Hclose failed for file_id(%d) for file %s\n", 
                        (int) file_id, file_name);
                ret_value = FAIL;
                goto done;
            }

          file_id = FAIL; /* reset */

      } /* while (curr_arg < argc)  */

done:
    if (ret_value == FAIL)
      { /* Failure cleanup */
          if (an_handle != FAIL)
              ANend(an_handle);

          if (vg_id != FAIL)
              Vdetach(vg_id);
          
          if (file_id != FAIL)
            {
                Vend(file_id);
                Hclose(file_id);
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
              HDfree(vg_chosen);
      }
    /* Normal cleanup */

    return ret_value;
}	/* dvg */


/* exported */
intn
do_dumpvg(intn        curr_arg, 
          intn        argc, 
          char       *argv[], 
          dump_opt_t *glob_opts)
{
    dump_info_t dumpvg_opts;	/* dumpvg options */
    char       *dummy1[MAXNAMELEN];
    int         dummy2;
    intn        ret_value = SUCCEED;

    if (glob_opts->help == TRUE)
      {
          dumpvg_usage(argc, argv);
          goto done;
      }		/* end if */

    init_dumpvg_opts(&dumpvg_opts);

    if (parse_dumpvd_opts(&dumpvg_opts, &curr_arg, argc, argv, dummy1, &dummy2) == FAIL)
      {
          printf("Failure in parsing options to dump Vgroup data \n");
          dumpvg_usage(argc, argv);
          ret_value = FAIL;
          goto done;
      }		/* end if */

    if (dvg(&dumpvg_opts, curr_arg, argc, argv) == FAIL)
      {
          fprintf(stderr,"Failure in dumping Vgroup data\n");
          ret_value = FAIL;
          goto done;
      }

  done:
    if (ret_value == FAIL)
      { /* Failure cleanup */
      }
    /* Normal cleanup */

    return ret_value;
}	/* end do_dumpvg() */

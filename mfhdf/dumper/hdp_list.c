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

#include "mfhdf.h"
#include "hdp.h"
#include <ctype.h>

#define NUM_FIELD_WIDTH 5
#define TAGNAME_FIELD_WIDTH 20
#define TAG_FIELD_WIDTH 8
#define REF_FIELD_WIDTH 8
#define INDEX_FIELD_WIDTH 12
#define OFFSET_FIELD_WIDTH 12
#define LENGTH_FIELD_WIDTH 12
#define LABEL_FIELD_WIDTH 15
#define DESC_FIELD_WIDTH 15

static void 
list_usage(intn argc, char *argv[])
{
    printf("Usage:\n");
    printf("%s list [-acensldg] [-o<f|g|t|n>] [-t tag] [files...]\n", argv[0]);
    printf("\t-a\tPrint annotations of items (sets long output)\n");
    printf("\t-c\tPrint classes of items (sets long output)\n");
    printf("\t-n\tPrint names or labels of items (sets long output)\n");
    printf("\t-e\tPrint special element information of items (sets long output)\n");
    printf("\t-s\tShort output (default)\n");
    printf("\t-l\tLong output\n");
    printf("\t-d\tDebugging output\n");
    printf("\t-g\tPrint groups only\n");
    printf("\t-t <number>\tPrint items of with a given tag number\n");
    printf("\t-t <name>\tPrint items of with a given tag name\n");
    printf("\t-of\tPrint items in the order found in the file\n");
    printf("\t-og\tPrint items in group order\n");
    printf("\t-ot\tPrint items in tag order (default)\n");
    printf("\t-on\tPrint items in name or label order\n");
}	/* end list_usage() */

static void 
init_list_opts(list_info_t * list_opts)
{
    list_opts->order = OTAG;	/* default ordering is by tag */
    list_opts->verbosity = VSHORT;	/* default verbosity is a short list */
    list_opts->limit = LNONE;	/* default is all the tag/refs */
    list_opts->class = FALSE;	/* don't dump class information */
    list_opts->name = FALSE;	/* don't dump name information */
    list_opts->desc = FALSE;	/* don't dump annotation information */
    list_opts->spec = FALSE;	/* don't dump special element information */
    list_opts->group = FALSE;	/* don't dump group information */
    list_opts->limit_tag = 0;	/* initialize... */
}	/* end init_list_opts() */

static intn 
parse_list_opts(list_info_t * list_opts, intn curr_arg, intn argc, char *argv[])
{
    intn        ret = 0;

    for (; curr_arg < argc; curr_arg++)
      {
          if (argv[curr_arg][0] == '-' || argv[curr_arg][0] == '/')
            {
                ret++;
                switch (argv[curr_arg][1])
                  {
                  case 'a':	/* print annotations */
                      list_opts->desc = TRUE;	/* dump description information */
                      if (list_opts->verbosity == VSHORT)
                          list_opts->verbosity = VLONG;		/* verbosity is a long list */
                      break;

                  case 'c':	/* print classes */
                      list_opts->class = TRUE;	/* dump class information */
                      if (list_opts->verbosity == VSHORT)
                          list_opts->verbosity = VLONG;		/* verbosity is a long list */
                      break;

                  case 'n':	/* print names */
                      list_opts->name = TRUE;	/* dump name/label information */
                      if (list_opts->verbosity == VSHORT)
                          list_opts->verbosity = VLONG;		/* verbosity is a long list */
                      break;

                  case 'e':	/* print special element info */
                      list_opts->spec = TRUE;	/* dump special element information */
                      if (list_opts->verbosity == VSHORT)
                          list_opts->verbosity = VLONG;		/* verbosity is a long list */
                      break;

                  case 's':	/* short output */
                      list_opts->verbosity = VSHORT;	/* verbosity is short */
                      break;

                  case 'l':	/* long output */
                      list_opts->verbosity = VLONG;		/* verbosity is long */
                      break;

                  case 'd':	/* debugging output */
                      list_opts->verbosity = VDEBUG;	/* verbosity is debug */
                      break;

                  case 'g':	/* print only groups */
#ifdef LATER
                      list_opts->limit = LGROUP;	/* limit to group output */
#endif
                      list_opts->group = TRUE;	/* dump group info */
                      if (list_opts->verbosity == VSHORT)
                          list_opts->verbosity = VLONG;		/* verbosity is long */
                      break;

                  case 't':	/* print only items of one tag */
                      curr_arg++;
                      ret++;
                      if (isdigit(argv[curr_arg][0]))
                        {
                            list_opts->limit = LTAGNUM;		/* limit to tag name output */
                            list_opts->limit_tag = atoi(argv[curr_arg]);
                            list_opts->limit_name = tagnum_to_name(list_opts->limit_tag);
                        }	/* end if */
                      else
                        {	/* must be a tag name */
                            list_opts->limit = LTAGNAME;	/* limit to tag name output */
                            list_opts->limit_name = HDstrdup(argv[curr_arg]);
                            list_opts->limit_tag = tagname_to_num(list_opts->limit_name);
                            if (list_opts->limit_tag == DFTAG_NULL)
                              {
                                  printf("ERROR: invalid tag name: %s\n", list_opts->limit_name);
                                  return (FAIL);
                              }		/* end if */
                        }	/* end else */
                      break;

                  case 'o':	/* order the items in some way */
                      switch (argv[curr_arg][2])
                        {
                        case 'g':
                            list_opts->order = OGROUP;	/* ordering is by group */
                            break;

                        case 't':
                            list_opts->order = OTAG;	/* ordering is by tag */
                            break;

                        case 'f':
                            list_opts->order = OFILE;	/* ordering is by file */
                            break;

                        case 'n':
                            list_opts->order = ONAME;	/* ordering is by name */
                            break;

                        default:
                            printf("ERROR: Invalid list ordering!\n");
                            return (FAIL);
                        }	/* end switch */
                      break;

                  default:		/* invalid list option */
                      printf("ERROR: Invalid list option!\n");
                      return (FAIL);
                  }		/* end switch */
            }	/* end if */
      }		/* end for */
    return (ret);
}	/* end parse_list_opts */

static void 
print_list_header(list_info_t * list_opts)
{
    switch (list_opts->verbosity)
      {
      case VSHORT:		/* short output */
              /* no header */
          break;

      case VLONG:	/* long output */
          printf("%*s%*s%*s%*s%*s\n",
                 NUM_FIELD_WIDTH, "no",
                 TAGNAME_FIELD_WIDTH, "tagname",
                 TAG_FIELD_WIDTH, "tag", REF_FIELD_WIDTH, "ref",
                 INDEX_FIELD_WIDTH, "  index_by_tag");
          break;

      case VDEBUG:		/* debugging output */
          printf("%*s%*s%*s%*s%*s%*s%*s\n",
                 NUM_FIELD_WIDTH, "no",
                 TAGNAME_FIELD_WIDTH, "tagname",
                 TAG_FIELD_WIDTH, "tag", REF_FIELD_WIDTH, "ref",
                 INDEX_FIELD_WIDTH, "  index_by_tag",
                 OFFSET_FIELD_WIDTH, "offset",
                 LENGTH_FIELD_WIDTH, "length");
          break;
      }		/* end switch() */
}	/* end print_list_header() */

static void 
print_file_label(int32 an_id)
{
    
    int32       len;
    char       *label = NULL;

    int32 ann_id, i;
    int32 n_file_label;
    int32 n_file_desc;
    int32 n_data_label;
    int32 n_data_desc;
    int32 retn;
    
    retn = ANfileinfo(an_id, &n_file_label, &n_file_desc, &n_data_label, &n_data_desc);
    for(i=0; i< n_file_label; i++) {  
      ann_id = ANselect(an_id, i, AN_FILE_LABEL);
      len = ANannlen(ann_id);

        /* allocate room for a label */
      if ((label = (char *) HDmalloc(len + 1)) == NULL)
        {
          printf("Failure to allocate space \n");
          return;
         }
      if(ANreadann(ann_id, label, len+1)!= FAIL)
        printf("File Label #%ld: %s\n", (long)i, label);
      retn = ANendaccess(ann_id);
      HDfree(label);
    }; 

}	/* end print_file_label() */

static void
print_data_label(int32 an_id)
{

    int32       len;
    char       *label = NULL;

    int32 ann_id, i;
    int32 n_file_label;
    int32 n_file_desc;
    int32 n_data_label;
    int32 n_data_desc;
    int32 retn;

    retn = ANfileinfo(an_id, &n_file_label, &n_file_desc, &n_data_label, 
                      &n_data_desc);
    for(i=0; i< n_data_label; i++) {
      ann_id = ANselect(an_id, i, AN_DATA_LABEL);
      len = ANannlen(ann_id);

        /* allocate room for a label */
      if ((label = (char *) HDmalloc(len + 1)) == NULL)
        {
          printf("Failure to allocate space \n");
          return;
         }
      if(ANreadann(ann_id, label, len+1)!= FAIL)
       printf("Data ID Label #%ld: %s\n", (long)i, label);
      retn = ANendaccess(ann_id);
      HDfree(label);
    };

}       /* end print_data_label() */

static void
print_data_desc(const char *fname, int32 an_id)
{
    int32       len;
    char       *desc = NULL;

    int32 ann_id, i;
    int32 n_file_label;
    int32 n_file_desc;
    int32 n_data_label;
    int32 n_data_desc;
    int32 retn;

    retn = ANfileinfo(an_id, &n_file_label, &n_file_desc,
           &n_data_label, &n_data_desc);
    for(i=0; i< n_data_desc; i++) {
      ann_id = ANselect(an_id, i, AN_DATA_DESC);
      len = ANannlen(ann_id);

        /* allocate room for a label */
      if ((desc = (char *) HDmalloc(len + 1)) == NULL)
        {
          printf("Failure to allocate space \n");
          return;
         }
      if(ANreadann(ann_id, desc, len+1)!= FAIL)
        printf("Data ID Annotation #%ld: %s\n", (long)i, desc);

      retn = ANendaccess(ann_id);
    }
}

static void 
print_file_desc(const char *fname, int32 an_id)
{
    int32       sd_fid;
    int32       len; 
    char       *desc = NULL;
    file_type_t ft;

    int32 ann_id, i;
    int32 n_file_label;
    int32 n_file_desc;
    int32 n_data_label;
    int32 n_data_desc;
    int32 retn;

    retn = ANfileinfo(an_id, &n_file_label, &n_file_desc, 
           &n_data_label, &n_data_desc);
    for(i=0; i< n_file_desc; i++) {
      ann_id = ANselect(an_id, i, AN_FILE_DESC);
      len = ANannlen(ann_id);

        /* allocate room for a label */
      if ((desc = (char *) HDmalloc(len + 1)) == NULL)
        {
          printf("Failure to allocate space \n");
          return;
         }
      if(ANreadann(ann_id, desc, len+1)!= FAIL)
        printf("File description #%ld: %s\n", (long)i, desc);

      retn = ANendaccess(ann_id);
    } 
       
      if ((sd_fid = SDstart(fname, DFACC_READ)) != FAIL)
      {         /* SD global attributes */
          int32       ndsets, nattrs;
          intn        i;
          char        name[MAXNAMELEN];
          int32       attr_nt;
          int32       attr_count;
          int32       attr_index;
          char       *attr_nt_desc;
          int32       ret = 0;
          int32       attr_buf_size;
          VOIDP       attr_buf;

          if (SDfileinfo(sd_fid, &ndsets, &nattrs) != FAIL)
            {
                for (i = 0; i < nattrs; i++)
                  {
                      if (SDattrinfo(sd_fid, i, name, &attr_nt, &attr_count) !=
FAIL)
                        {
                            attr_nt_desc = HDgetNTdesc(attr_nt);
                            if (ret == FAIL)
                              {
                                   printf("Failure in SDattrinfo %s\n", fname);
                                  exit(1);
                              }
                            attr_index = SDfindattr(sd_fid, name);
                            if (ret == FAIL)
                              {
                                  printf("Failure in SDfindattr %s\n", fname);
                                  exit(1);
                              }
                            attr_buf_size = DFKNTsize(attr_nt) * attr_count;
                            if ((attr_buf = (VOIDP) HDmalloc(attr_buf_size)) ==
NULL)
                              {
                                  printf("Failure to allocate space \n");
                                  exit(1);
                              }
                            ret = SDreadattr(sd_fid, attr_index, attr_buf);
                            if (ret == FAIL)
                              {
                                  printf("Failure in SDfindattr %s\n", fname);
                                  exit(1);
                               printf("\t Attr%i: Name = %s\n", (int) attr_index, name);

                            printf("\t\t Type = %s \n\t\t Count= %i\n",
                                   attr_nt_desc, (int) attr_count);
                            printf("\t\t Value = ");
                            ret = dumpfull(attr_nt, ft, attr_count, attr_buf, 20, stdout);
                            printf("\n");
                                /* clean up */
                            HDfree(attr_nt_desc);
                            HDfree((VOIDP) attr_buf);

                        }       /* end if */
                  }             /* end for */
            }   /* end if */
      }         /* end if */
        /* clean up */
      HDfree(desc);
    };
}	/* end print_file_desc() */

static void
print_list_obj(list_info_t * l_opts, objinfo_t * o_info, intn o_num, int32 an_id)
{
    char       *s;
    char       *buf;
    int32 ann_num, ann_length, retn, ann_id,i;
    int32      *ann_list;

    switch (l_opts->verbosity)
      {
      case VSHORT:		/* short output */
              /* handled elsewhere */
          break;

      case VLONG:	/* long output */
          printf("%*d%*s%*d%*d%*ld\n",
                 NUM_FIELD_WIDTH, o_num,
                 TAGNAME_FIELD_WIDTH,
                 ((s = HDgettagsname(o_info->tag)) == NULL ? HDstrdup("Unknown") : s),
                 TAG_FIELD_WIDTH, o_info->tag, REF_FIELD_WIDTH, o_info->ref,
                 INDEX_FIELD_WIDTH, (long) o_info->index);
          HDfree(s);	/* free tagname string */
          break;

      case VDEBUG:		/* debugging output */
          printf("%*d%*s%*d%*d%*ld%*ld%*ld\n",
                 NUM_FIELD_WIDTH, o_num,
                 TAGNAME_FIELD_WIDTH,
                 ((s = HDgettagsname(o_info->tag)) == NULL ? HDstrdup("Unknown") : s),
                 TAG_FIELD_WIDTH, o_info->tag, REF_FIELD_WIDTH, o_info->ref,
                 INDEX_FIELD_WIDTH, (long) o_info->index,
                 OFFSET_FIELD_WIDTH, (long) o_info->offset,
                 LENGTH_FIELD_WIDTH, (long) o_info->length);
          HDfree(s);	/* free tagname string */
          break;
      }		/* end switch */
    if (l_opts->name == TRUE && o_info->has_label){
        ann_num = ANnumann(an_id, AN_DATA_LABEL,o_info->tag, o_info->ref);
        ann_list = HDmalloc(ann_num*sizeof(int32));
        retn = ANannlist(an_id,AN_DATA_LABEL,o_info->tag, o_info->ref, ann_list);

        for(i=0; i<ann_num; i++){
        ann_id = *ann_list+i;
        ann_length =  ANannlen(ann_id);
        buf= HDmalloc((ann_length+1) * sizeof(char));
        ANreadann(ann_id, buf, ann_length+1);
        printf("%*s%s\n", LABEL_FIELD_WIDTH, "Name/Label=", buf);
        HDfree(buf);
        }
    }
      /*  printf("%*s%s\n", LABEL_FIELD_WIDTH, "Name/Label=", o_info->lab_info);*/
    if (l_opts->class == TRUE)
      {
      }		/* end if */
    if (l_opts->desc == TRUE && o_info->has_desc){
        ann_num = ANnumann(an_id, AN_DATA_DESC, o_info->tag, o_info->ref);
        ann_list = HDmalloc(ann_num*sizeof(int32));
       
        retn = ANannlist(an_id,AN_DATA_DESC, o_info->tag, o_info->ref, ann_list)
;

        for(i=0; i<ann_num; i++){
        ann_id = *ann_list+i;
        ann_length =  ANannlen(ann_id);
        buf= HDmalloc((ann_length+1) * sizeof(char));
        ANreadann(ann_id, buf, ann_length+1);
        printf("%*s%s\n", LABEL_FIELD_WIDTH, "Description=", buf);
        HDfree(buf);
        }
    }

       /* printf("%*s%s\n", DESC_FIELD_WIDTH, "Description=", o_info->desc_info); */
    if (l_opts->spec == TRUE && o_info->is_special)
      {
          switch (o_info->spec_info->key)
            {
            case SPECIAL_LINKED:
                printf("\tLinked Block: first %ld standard %ld per unit %ld\n",
                       (long) o_info->spec_info->first_len,
                       (long) o_info->spec_info->block_len,
                       (long) o_info->spec_info->nblocks);
                break;

            case SPECIAL_EXT:
                printf("\tExternal File: path %s  offset %ld\n",
                       o_info->spec_info->path, (long) o_info->spec_info->offset);
                break;

            case SPECIAL_COMP:
                printf("\tCompressed Element: compression type: %s  modeling type %s\n",
                       (o_info->spec_info->comp_type == COMP_CODE_NONE ? "None" :
                        (o_info->spec_info->comp_type == COMP_CODE_RLE ? "Run-Length" :
                         (o_info->spec_info->comp_type == COMP_CODE_NBIT ? "N-Bit" : "Unknown"))),
                       (o_info->spec_info->model_type == COMP_MODEL_STDIO ? "Standard" : "Unknown"));
                break;

            default:
                printf("\tDo not understand special element type %d\n",
                       o_info->spec_info->key);
                break;
            }	/* end switch */
      }		/* end if */
    if (l_opts->group == TRUE && o_info->is_group)
      {
          DFdi       *g_obj;
          int32       num;

          if ((num = get_group_max(o_info->group_info)) != FAIL)
            {
                printf("\tContents: (%ld objects)\n", (long) num);
                g_obj = get_next_group(o_info->group_info, 0);
                while (g_obj != NULL)
                  {
                      printf("\t\t%-30s: (tag=%6d) ref=%d\n",
                             ((s = HDgettagsname(g_obj->tag)) == NULL ? HDstrdup("Unknown") : s),
                             g_obj->tag, g_obj->ref);
                      HDfree(s);	/* free tagname string */
                      g_obj = get_next_group(o_info->group_info, 1);
                  }		/* end while */
            }	/* end if */
      }		/* end if */
}	/* end print_obj_list() */


/* print the library version of the file */
void printfilever(int32 file_id)
{
    uint32 major, minor, release;
    char string[LIBVSTR_LEN+1];

    if (Hgetfileversion(file_id, &major, &minor, &release, string) == SUCCEED)
      {
        string[LIBVSTR_LEN] = '\0';		/* make it a null terminated string */
        printf("\nFile library version: ");
        printf("Major= %u, Minor=%u, Release=%u\nString=%s\n",
	    (unsigned)major, (unsigned)minor, (unsigned)release, string);
      }
    else
        printf("(Does not have libraray version information)\n");
}

void 
do_list(intn curr_arg, intn argc, char *argv[], dump_opt_t * glob_opts)
{
    list_info_t list_opts;		/* list options */
    filelist_t *f_list;			/* list of files to dump */
    objlist_t  *o_list;			/* list of DD objects in a file */
    objinfo_t  *o_info;			/* pointer to a DD object */
    char       *f_name;			/* current file name to list */
    int32       fid;			/* HDF file ID */
    intn        obj_num;		/* number of the object we are displaying */
    intn        status;			/* status from various function calls */
    char       *s;				/* temporary character pointer */

    int32  an_id;

    if (glob_opts->help == TRUE || curr_arg >= argc)
      {
          list_usage(argc, argv);
          return;
      }		/* end if */
    init_list_opts(&list_opts);
    if ((status = parse_list_opts(&list_opts, curr_arg, argc, argv)) == FAIL)
      {
          list_usage(argc, argv);
          return;
      }		/* end if */
    curr_arg += status;
    if (curr_arg >= argc || (f_list = make_file_list(curr_arg, argc, argv)) == NULL)
      {
          printf("ERROR: No files to dump!\n");
          list_usage(argc, argv);
          return;
      }		/* end if */
    f_name = get_next_file(f_list, 0);
    while (f_name != NULL)
      {
          vinit_done = FALSE;	/* reset global Vset variable */
          obj_num = 0;	/* number of the object we are displaying */
          if ((fid = Hopen(f_name, DFACC_READ, -1)) != FAIL)
            {
                an_id = ANstart(fid);
                if ((o_list = make_obj_list(fid,
                                            (list_opts.name == TRUE ? CHECK_LABEL : 0) |
                                            (list_opts.desc == TRUE ? CHECK_DESC : 0) |
                                            CHECK_GROUP | CHECK_SPECIAL)) != NULL)
                  {

                          /* print out filename, etc. */
                      printf("File: %s\n", f_name);
		      printfilever(fid);

                          /* check for file label */
                      if (list_opts.name == TRUE)


                           print_file_label(an_id);




                          /* check for file descriptions */
                      if (list_opts.desc == TRUE)


                           print_file_desc(f_name, an_id);




                          /* sort list */
                      sort_obj_list(o_list, list_opts.order);

                          /* print out list header according to options */
                      print_list_header(&list_opts);

                          /* Special case for short output */
                      if (list_opts.verbosity == VSHORT)
                        {
                            uint16      last_tag = 0;

                            o_info = get_next_obj(o_list, 0);	/* get first DD object */
                            while (o_info != NULL)
                              {
                                  if ((list_opts.limit == LGROUP || list_opts.limit == LNONE) || list_opts.limit_tag == o_info->tag)
                                    {
                                        if (o_info->tag != last_tag)
                                          {
                                              printf("%s%-*s: (tag %d)\n", (last_tag == 0 ? "" : "\n"),
                                                     TAGNAME_FIELD_WIDTH,
                                                     ((s = HDgettagsname(o_info->tag)) == NULL ? HDstrdup("Unknown") : s),
                                                     o_info->tag);
                                              last_tag = o_info->tag;
                                              printf("\tRef nos: ");
                                              HDfree(s);	/* free tagname string */
                                          }		/* end if */
                                        printf("%d ", o_info->ref);
                                    }	/* end if */
                                  o_info = get_next_obj(o_list, 1);		/* advance to the next DD object */
                              }		/* end while */
                            printf("\n");
                        }	/* end if */
                      else
                        {
                                /* Loop through all the objects in the file */
                            o_info = get_next_obj(o_list, 0);	/* get first DD object */
                            while (o_info != NULL)
                              {
                                  switch (list_opts.limit)
                                    {
                                    default:
                                    case LNONE:
                                    case LGROUP:
                                        print_list_obj(&list_opts, o_info, obj_num,an_id);
                                        break;

                                    case LTAGNUM:
                                    case LTAGNAME:
                                        if (list_opts.limit_tag == o_info->tag)
                                            print_list_obj(&list_opts, o_info, obj_num, an_id);
                                        break;
                                    }	/* end switch */
                                  obj_num++;
                                  o_info = get_next_obj(o_list, 1);		/* advance to the next DD object */
                              }		/*end while */
                        }	/* end else */
                  }		/* end if */
                free_obj_list(o_list);

                if (vinit_done == TRUE)
                    Vfinish(fid);
                if (Hclose(fid) == FAIL)
                    HEprint(stderr, 0);
            }	/* end if */
          else
              printf("WARNING: Invalid file name: %s\n", f_name);
          f_name = get_next_file(f_list, 1);
      }		/* end while */
    free_file_list(f_list);
}	/* end do_list() */

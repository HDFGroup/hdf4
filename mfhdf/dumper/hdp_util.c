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
static char RcsId[] = "@(#)1.1";
#endif

/* hdp_util.c,v 1.1 1994/04/18 15:49:18 georgev Exp */

#include "hdp.h"

const char *unknown_tag = "Unknown Tag";

char       *
tagnum_to_name(intn num)
{
    char       *ret;

    ret = HDgettagsname(num);
    if (ret == NULL)
        ret = HDstrdup(unknown_tag);
    return (ret);
}	/* end tagnum_to_name() */

intn 
tagname_to_num(const char *name)
{
    return (HDgettagnum(name));
}	/* end tagname_to_num() */

/*
 *  Routines to create a list of file names from the command line
 */
/* assumes that curr_arg is pointing to the first file name */
filelist_t *
make_file_list(intn curr_arg, intn argc, char *argv[])
{
    intn        i;
    filelist_t *ret;

    if (curr_arg > argc)	/* consistency check */
        return (NULL);

    ret = (filelist_t *) HDmalloc(sizeof(filelist_t));
    if (ret == NULL)
      {
          printf("Failure to allocate memory\n");
          return (NULL);
      }
    ret->file_arr = (char **) HDmalloc(sizeof(char *) * ((argc - curr_arg) + 1));
    if (ret->file_arr == NULL)
      {
          printf("Failure to allocate memory\n");
          HDfree(ret);
          return (NULL);
      }		/* end if */

    ret->max_files = (argc - curr_arg);
    ret->curr_file = 0;
    for (i = 0; curr_arg < argc; i++, curr_arg++)
        ret->file_arr[i] = HDstrdup(argv[curr_arg]);
    return (ret);
}	/* end make_file_list() */

char       *
get_next_file(filelist_t * f_list, intn advance)
{
    if (advance)
        f_list->curr_file++;
    if (f_list->curr_file >= f_list->max_files)
        return (NULL);
    return (f_list->file_arr[f_list->curr_file]);
}	/* end get_next_file() */

void 
free_file_list(filelist_t * f_list)
{
    intn        i;

    for (i = 0; i < f_list->max_files; i++)
        HDfree(f_list->file_arr[i]);
    HDfree(f_list->file_arr);
    HDfree(f_list);
}	/* end free_file_list() */

/*
 *  Routines to manipulate group lists
 */
groupinfo_t *
make_group_list(int32 fid, uint16 tag, uint16 ref)
{
    intn        nobj;
    intn        i;
    groupinfo_t *ret;
    int32       gid;

    if (tag == DFTAG_RIG || tag == DFTAG_SDG || tag == DFTAG_NDG)
      {
          if ((gid = DFdiread(fid, tag, ref)) == FAIL)
              return (NULL);
          if ((nobj = DFdinobj(gid)) == FAIL)
              return (NULL);
          if ((ret = (groupinfo_t *) HDmalloc(sizeof(groupinfo_t))) == NULL)
            {
                printf("Failure to allocate memory \n");
                return (NULL);
            }
          ret->max_dds = nobj;
          ret->curr_dd = 0;
          if (nobj > 0)
            {
                if ((ret->dd_arr = (DFdi *) HDmalloc(sizeof(DFdi) * nobj)) == NULL)
                  {
                      printf("Failure to allocate memory \n");
                      HDfree(ret);
                      return (NULL);
                  }		/* end if */
                for (i = 0; i < nobj; i++)
                  {
                      if (DFdiget(gid, &ret->dd_arr[i].tag, &ret->dd_arr[i].ref) == FAIL)
                        {
                            HDfree(ret->dd_arr);
                            HDfree(ret);
                            return (NULL);
                        }	/* end if */
                  }		/* end for */
            }	/* end if */
          else
            {	/* paranoia sets in... */
                ret->max_dds = ret->curr_dd = 0;
                ret->dd_arr = NULL;
            }	/* end else */
      }		/* end if */
    else
      {		/* check for Vgroup? */
          int32       vkey;

/* Yes, I know this wastes time, but at least it allows uniform access */
/* to both types of groups in HDF files... */
          if (vinit_done == FALSE)
            {	/* check whether we've already init'ed Vsets */
                vinit_done = TRUE;
                Vinitialize(fid);
            }	/* end if */
          if ((vkey = Vattach(fid, ref, "r")) != FAIL)
            {
                if ((nobj = Vntagrefs(vkey)) != FAIL)
                  {
                      int32      *temp_tag;
                      int32      *temp_ref;

                      if ((temp_tag = (int32 *) HDmalloc(sizeof(int32) * nobj)) == NULL)
                        {
                            printf("Failure to allocate memory \n");
                            Vdetach(vkey);
                            return (NULL);
                        }	/* end if */
                      if ((temp_ref = (int32 *) HDmalloc(sizeof(int32) * nobj)) == NULL)
                        {
                            printf("Failure to allocate memory \n");

                            Vdetach(vkey);
                            HDfree(temp_tag);
                            return (NULL);
                        }	/* end if */

                      if (Vgettagrefs(vkey, temp_tag, temp_ref, nobj) == FAIL)
                        {
                            Vdetach(vkey);
                            HDfree(temp_tag);
                            HDfree(temp_ref);
                            return (NULL);
                        }	/* end if */

                      if ((ret = (groupinfo_t *) HDmalloc(sizeof(groupinfo_t))) == NULL)
                        {
                            printf("Failure to allocate memory \n");

                            Vdetach(vkey);
                            HDfree(temp_tag);
                            HDfree(temp_ref);
                            return (NULL);
                        }	/* end if */
                      ret->max_dds = nobj;
                      ret->curr_dd = 0;
                      if ((ret->dd_arr = (DFdi *) HDmalloc(sizeof(DFdi) * nobj)) == NULL)
                        {
                            printf("Failure to allocate memory \n");

                            Vdetach(vkey);
                            HDfree(temp_tag);
                            HDfree(temp_ref);
                            HDfree(ret);
                            return (NULL);
                        }	/* end if */

                      for (i = 0; i < nobj; i++)
                        {
                            ret->dd_arr[i].tag = (uint16) temp_tag[i];
                            ret->dd_arr[i].ref = (uint16) temp_ref[i];
                        }	/* end for */

                      HDfree(temp_tag);
                      HDfree(temp_ref);
                  }		/* end if */
                else	/* bad vkey? */
                    return (NULL);
                Vdetach(vkey);	/* release the Vgroup */
            }	/* end if */
          else	/* failed to attach */
              return (NULL);
      }		/* end else */
    return (ret);
}	/* end make_group_list() */

DFdi       *
get_next_group(groupinfo_t * g_list, intn advance)
{
    if (advance)
        g_list->curr_dd++;
    if (g_list->curr_dd >= g_list->max_dds)
        return (NULL);
    return (&g_list->dd_arr[g_list->curr_dd]);
}	/* end get_next_group() */

int32 
get_group_max(groupinfo_t * g_list)
{
    if (g_list != NULL)
        return (g_list->max_dds);
    return (FAIL);
}	/* end get_group_max() */

void 
free_group_list(groupinfo_t * g_list)
{
    HDfree(g_list->dd_arr);
    HDfree(g_list);
}	/* end free_group_list() */

/*
 *  Routines to manipulate tag/ref lists
 */

objlist_t  *
make_obj_list(int32 fid, uint32 options)
{
    intn        nobj;			/* number of DDs in the file */
    int32       status;			/* status of various HDF calls */
    int32       aid;			/* temporary AID to use while getting DD info */
    int16       tmp_spec;		/* temporary storage for special status */
    objlist_t  *obj_ret;		/* pointer to the dd list to return */
    objinfo_t  *temp_obj;		/* temporary pointer to a working DD object */
    sp_info_block_t info;		/* temp. storage for special elem. info */
    intn        n, m;			/* local counting variable */

    if ((nobj = Hnumber(fid, DFTAG_WILDCARD)) == FAIL)
        return (NULL);

    if ((obj_ret = (objlist_t *) HDmalloc(sizeof(objlist_t))) == NULL)
      {
          printf("Failure to allocate memory \n");
          return (NULL);
      }

    obj_ret->max_obj = nobj;	/* set the number of objects */
    obj_ret->curr_obj = 0;
    if ((obj_ret->raw_obj_arr = (objinfo_t *) HDmalloc(sizeof(objinfo_t) * nobj)) == NULL)
      {
          printf("Failure to allocate memory \n");
          HDfree(obj_ret);
          return (NULL);
      }		/* end if */

	/* Clear array of dd/object information */
    HDmemset(obj_ret->raw_obj_arr, 0, sizeof(objinfo_t) * nobj);

	/* Read all the tag/ref's in the file into an array */
    aid = Hstartread(fid, DFTAG_WILDCARD, DFREF_WILDCARD);
    if (aid == FAIL)
      {
          HEprint(stderr, 0);
          HDfree(obj_ret->raw_obj_arr);
          HDfree(obj_ret);
          return (NULL);
      }		/* end if */

    for (n = 0, status = SUCCEED; (n < nobj) && (status != FAIL); n++)
      {
          Hinquire(aid, NULL, &(obj_ret->raw_obj_arr[n].tag),
                   &(obj_ret->raw_obj_arr[n].ref), &(obj_ret->raw_obj_arr[n].length),
                   &(obj_ret->raw_obj_arr[n].offset), NULL, NULL, &tmp_spec);
          if (options & CHECK_SPECIAL)
            {	/* are we looking for spec. elem. ? */
                obj_ret->raw_obj_arr[n].is_special = (tmp_spec != 0);
                if (obj_ret->raw_obj_arr[n].is_special)
                  {		/* get the special info. */
                      if ((status = HDget_special_info(aid, &info)) == FAIL)
                        {
                            obj_ret->raw_obj_arr[n].is_special = 0;
                        }	/* end if */
                      else
                        {	/* copy over special information we found */
                            if ((obj_ret->raw_obj_arr[n].spec_info = (sp_info_block_t *) HDmalloc(sizeof(sp_info_block_t))) == NULL)
                              {
                                  printf("Failure to allocate memory \n");
                                  obj_ret->raw_obj_arr[n].is_special = 0;
                              }
                            else
                                HDmemcpy(obj_ret->raw_obj_arr[n].spec_info, &info, sizeof(sp_info_block_t));
                        }	/* end else */
                  }		/* end if */
            }	/* end if */
          status = Hnextread(aid, DFTAG_WILDCARD, DFREF_WILDCARD, DF_CURRENT);
      }		/* end for */

    if (Hendaccess(aid) == FAIL)
      {
          HEprint(stderr, 0);
          HDfree(obj_ret->raw_obj_arr);
          HDfree(obj_ret);
          return (NULL);
      }		/* end if */

	/* Post-process the list of dd/objects, adding more information */
	/*  Also set up the pointers for the sorted list to be manipulated later */
    if ((obj_ret->srt_obj_arr = (objinfo_t **) HDmalloc(sizeof(objinfo_t *) * nobj)) == NULL)
      {
          printf("Failure to allocate memory \n");
          HDfree(obj_ret->raw_obj_arr);
          HDfree(obj_ret);
          return (NULL);
      }		/* end if */

	/* Loop for more information */
    for (n = 0; n < nobj; n++)
      {
          temp_obj = obj_ret->srt_obj_arr[n] = &obj_ret->raw_obj_arr[n];

              /* set the index value to a flag for later */
          temp_obj->index = (-1);

              /* check for a group */
          if (options & CHECK_GROUP)
            {	/* are we looking for groups ? */
                if (temp_obj->tag == DFTAG_RIG || temp_obj->tag == DFTAG_SDG
                    || temp_obj->tag == DFTAG_NDG || temp_obj->tag == DFTAG_VG)
                  {
                      temp_obj->is_group = TRUE;
                      if ((temp_obj->group_info = make_group_list(fid, temp_obj->tag, temp_obj->ref)) == NULL)
                        {
                            HDfree(obj_ret->raw_obj_arr);
                            HDfree(obj_ret);
                            return (NULL);
                        }	/* end if */
                  }		/* end if */
            }	/* end if */

              /* check for a annotations */
          if (options & CHECK_LABEL)
            {	/* are we looking for annotations ? 
                * No longer checked here, now checked in print_list_obj() -GV  */
            }		/* end if */

          if (options & CHECK_DESC)
            {	/* are we looking for annotations ? 
                 * No longer checked here, now checked in print_list_obj() -GV */
            }		/* end if */
      }		/* end for */

	/* Loop once more to figure out the index information */
    for (n = 0, temp_obj = &obj_ret->raw_obj_arr[0]; n < nobj; n++, temp_obj++)
      {
          if (temp_obj->index == (-1))
            {	/* first object of this type in the file */
                int32       temp_index;
                objinfo_t  *temp2_obj;	/* temporary pointer to a working DD object */

                temp_obj->index = temp_index = 0;

                    /* look for other objects of this tag */
                for (m = n, temp2_obj = temp_obj + 1; m+1 < nobj; m++, temp2_obj++)
                  {
                      if (temp2_obj->tag == temp_obj->tag)
                          temp2_obj->index = ++temp_index;	/* set next index */
                  } 		/* end for */
            }	/* end if */
      }		/* end for */

    obj_ret->options = options;
    return (obj_ret);
}	/* end make_dd_list() */

objinfo_t  *
get_next_obj(objlist_t * o_list, intn advance)
{
    if (advance)
        o_list->curr_obj++;
    if (o_list->curr_obj >= o_list->max_obj)
        return (NULL);
    return (o_list->srt_obj_arr[o_list->curr_obj]);
}	/* end get_next_obj() */

objinfo_t  *
goto_nth_obj(objlist_t * o_list, intn n)
{
    if (n >= 0 && n < o_list->max_obj)
        o_list->curr_obj = n;
    return (o_list->srt_obj_arr[o_list->curr_obj]);
}	/* end goto_nth_obj() */

void 
reset_obj_list(objlist_t * o_list)
{
    if (o_list != NULL)
        o_list->curr_obj = 0;
}	/* end reset_obj_list() */

void 
free_obj_list(objlist_t * o_list)
{
    intn        i;				/* local counting variable */
    objinfo_t  *temp_obj;		/* temporary pointer to a working DD object */

    for (i = 0, temp_obj = o_list->raw_obj_arr; i < o_list->max_obj; i++, temp_obj++)
      {
          if (temp_obj->is_group)
              free_group_list(temp_obj->group_info);
          if (temp_obj->is_special)
              HDfree(temp_obj->spec_info);
      }		/* end for */
    HDfree(o_list->srt_obj_arr);
    HDfree(o_list->raw_obj_arr);
    HDfree(o_list);
}	/* end free_obj_list() */

int 
sort_obj_list_by_tag(const void *p1, const void *p2)
{
    const objinfo_t *a = (const objinfo_t *) *((void **) p1);
    const objinfo_t *b = (const objinfo_t *) *((void **) p2);

    if (a->tag > b->tag)
        return (1);
    if (a->tag < b->tag)
        return (-1);
    if (a->ref > b->ref)
        return (1);
    if (a->ref < b->ref)
        return (-1);
    return (0);
}	/* end sort_obj_info_by_tag() */

#if 0 /* No longer possible since objects can have more than one label 
       * -GV 6/12/97 */
int 
sort_obj_list_by_name(const void *p1, const void *p2)
{
    const objinfo_t *a = (const objinfo_t *) *((void **) p1);
    const objinfo_t *b = (const objinfo_t *) *((void **) p2);

	/* Any label has priority over no label, else sort alphabetically */
    if (a->has_label)
      {
          if (b->has_label)
              return (HDstrcmp(a->lab_info, b->lab_info));
          else
              return (1);
      }		/* end if */
    else
      {
          if (b->has_label)
              return (-1);
          else
              return (0);
      }		/* end else */
}	/* end sort_obj_info_by_tag() */
#endif

void 
sort_obj_list(objlist_t * o_list, sort_t sort_type)
{
    switch (sort_type)
      {
#if 0 /* No longer possible since objects can have more than one label 
       * -GV 6/12/97 */
      case ONAME:	/* sort by name order */
          qsort(o_list->srt_obj_arr, o_list->max_obj, sizeof(objinfo_t *), sort_obj_list_by_name);
          break;
#endif
      case OGROUP:		/* sort by group order */
          break;	/* not currently implemented */

      case OTAG:	/* sort by tag order */
          qsort(o_list->srt_obj_arr, o_list->max_obj, sizeof(objinfo_t *), sort_obj_list_by_tag);
          break;

      case OFILE:	/* sort by file order */
      default:
          break;
      }		/* end switch() */
}	/* end sort_obj_list() */

/* Misc. utility functions */
int 
int32_compare(const void *a, const void *b)
{
    if (*(const int32 *) a > *(const int32 *) b)
        return (1);
    else if (*(const int32 *) a < *(const int32 *) b)
        return (-1);
    else
        return (0);
}	/* end int32_compare() */

void 
sort(int32 *chosen, int32 choices)
{
    qsort((void *) chosen, choices, sizeof(int32), int32_compare);
}

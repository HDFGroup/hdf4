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

/*-----------------------------------------------------------------------------
 * File:     mfan.c
 * Author:   GeorgeV
 * Purpose:  Multi-file read and write annotations: labels and descriptions 
 *           of data items and file
 * Invokes:  
 * Comments:
 *           Currently the id returned by ANstart() is the same as the
 *           one returned by Hopen().
 *           Currenntly as least 6 TBBT trees are created to handle 
 *           annotations(1 for file_ids,1 for all ann_ids, 4 for file_ann, 
 *           file_desc, data_ann and data_desc) which is not pretty but it 
 *           does make search/find of annotations much easier.Currently the 
 *           implementation uses TBBT trees.
 *
 *           NOTE1: These could be removed in favor of using the atom stuff
 *                 in order to more unify the code with the library.
 *
 *           NOTE2: Note that any mention of file ID's except in ANStart() 
 *                  should now refer to annotation interface ID.
 *
 * Contents: 
 *
 *  Private Routines:
 *  -----------------
 *  NOTES: TYPE here refers to file/data label/description types 
 *         They are AN_FILE_LABEL, AN_FILE_DESC, AN_DATA_LABEL, AN_DATA_DESC
 *         The tag/ref refers to data tag/ref
 *
 *  ANinit      - Intialize the annotation interface
 *  ANdestroy   - Un-intialize the annotation interface
 *
 *  ANIfidcmp    - compare two annotation interface id's (used in file TBBTtree)
 *  ANIanncmp    - compare two annotation handles(ann_id's) 
 *                  (used in annotation TBBTtree)
 *  ANIaddentry:  - add entry to corresponding annotation TBBTtree
 *  ANIcreate_ann_tree - create annotation TBBTtree 
 *  ANIfind:      - return annotation handle(ann_id) if found of given TYPE/ref
 *  ANInumann:    - return number of annotations that match TYPE/tag/ref
 *  ANIannlist:   - return list of handles(ann_id's) that match TYPE/tag/ref
 *  ANIannlen:    - get length of annotation given handle(ann_id)
 *  ANIreadann:   - read annotation given handle(ann_id)
 *  ANIwriteann:  - write annotation given handle(ann_id)
 *  ANIcreate     - create a new file/label annotation 
 *                  and return a handle(ann_id)
 *
 *  Public API routines:
 *  --------------------
 *  ANstart     - open file for annotation handling, returns an_id
 *  ANfileinfo  - get number of file/data annotations in file. Indices returned
 *                are used in ANselect() calls.
 *  ANend       - end access to annotation handling on file
 *  ANcreate    - create a new data annotation and return a handle(ann_id)
 *  ANcreatef   - create a new file annotation and return a handle(ann_id)
 *  ANselect    - returns an annotation handle(ann_id) from index for 
 *                a particular annotation TYPE. This handle is then used for
 *                calls like ANwriteann(), ANreadann(), ANannlen(),..etc
 *  ANnumann:   - return number of annotations that match TYPE/tag/ref
 *  ANannlist:  - return list of handles(ann_id's) that match TYPE/tag/ref
 *  ANannlen:   - get length of annotation given handle(ann_id)
 *  ANreadann:  - read annotation given handle(ann_id)
 *  ANwriteann: - write annotation given handle(ann_id)
 *  ANendaccess - end access to annotation using handle(ann_id)
 *
 *  Public Miscellaneous API Routines
 *  ---------------------------------
 *  ANget_tagref - get tag/ref pair to annotation type and index
 *  ANid2tagref  - get tag/ref pair to annotation ID
 *  ANtagref2id  - get annotation id given tag/ref
 *  ANatype2tag - annotation type to corresponding annotation TAG
 *  ANtag2atype - annotation TAG to corresponding annotation type
 *
 *---------------------------------------------------------------------------*/

#ifndef MFAN_C  /* define main annoation source file */
#define MFAN_C

#include "mfan.h"
#include "atom.h"

/* Whether we've installed the library termination function yet for this 
   interface */
PRIVATE intn library_terminate = FALSE;

/* Function Prototypes */
extern VOID ANfreedata(VOIDP data);
extern VOID ANfreekey(VOIDP key);
extern VOID dumpnodeKey(VOID *key, VOID *data);
extern VOID dumpentryKey(VOID *key, VOID *data);
extern VOID dumpfileKey(VOID *key, VOID *data);
extern intn ANIfidcmp(VOIDP i, VOIDP j, intn value);
extern intn ANIanncmp(VOIDP i, VOIDP j, intn value);
PRIVATE intn ANIstart(void);

/*-----------------------------------------------------------------------------
 *                          Internal Routines
 *---------------------------------------------------------------------------*/

/* ------------------------ Routines for TBBT ------------------------------*/
/* free data - used by tbbt routines */
VOID
ANfreedata(VOIDP data)
{
    HDfree(data);
} /* ANfreekey() */

/* free key - used by tbbt routines */
VOID
ANfreekey(VOIDP key)
{
    HDfree(key);
} /* ANfreekey() */

/* Following 3 routines are used for debugging purposes to dump 
 * key/data pairs from the TBBT trees 
 * eg. tbbt_dump(tree, dumpnodeKey, 0)*/
VOID
dumpnodeKey(VOID *key, VOID *data)
{
    ANnode *node = NULL;

    printf("key = %4.4d, data = ",*(int*)key);
    if (data != NULL) 
      {
          node = (ANnode *) data;
          printf("file_id=%ld, ann_key=%ld, new_ann=%d\n", (long)node->file_id,
                 (long)node->ann_key, node->new_ann);
      }
    else 
        printf("(NULL)\n");
    return;
}

VOID
dumpentryKey(VOID *key, VOID *data)
{
    ANentry *node = NULL;

    printf("key = %4.4d, data = ",*(int*)key);
    if (data != NULL) 
      {
          node = (ANentry *) data;
          printf("ann_id=%ld, annref=%d, elmtag=%d, elmref=%d\n", (long)node->ann_id,
                 node->annref, node->elmtag, node->elmref);
      }
    else 
        printf("(NULL)\n");
    return;
}

VOID
dumpfileKey(VOID *key, VOID *data)
{
    ANfile *node = NULL;

    printf("key = %4.4d, data = ",*(int*)key);
    if (data != NULL) 
      {
          node = (ANfile *) data;
          printf("filename=%s, accesmode=%ld \n", node->filename,
                 (long)node->access_mode);
      }
    else 
        printf("(NULL)\n");
    return;
}

/* ------------------------ Private Internal Routines ---------------------*/

/* ------------------------------- ANIfidcmp -------------------------------- 
 NAME
	ANIfidcmp -- compare two annotation interface id's

 DESCRIPTION
    Compares two annotation interface id's. Used by trees for file id's. 

 RETURNS
    Returns 0 if i=j, -1 if i > j and 1 if i < j

 AUTHOR
    GeorgeV.

 NOTE:- these file ID's are now refered to as annotation interface ID's

--------------------------------------------------------------------------- */
intn
ANIfidcmp(VOIDP i,   /* IN: annotation interfaced id (int32) */
          VOIDP j,   /* IN: annotation interfaced id (int32) */
          intn value)
{
    /* shut compiler up */
    value=value;

    if (*(int32 *)i == *(int32 *)j) 
        return 0;
    if (*(int32 *)i > *(int32 *)j) 
        return -1; 
    else 
        return 1;
} /* ANIfidcmp */

/* ------------------------------- ANIanncmp -------------------------------- 
 NAME
	ANIanncmp -- compare two annotation keys or id's

 DESCRIPTION
        Compares two annotation keys. Used by tree for annotations
        Also used to compare annotation id's since also 32 bit value.

 RETURNS
        Returns 0 if i=j, -1 if i > j and 1 if i < j

 AUTHOR
    GeorgeV.
--------------------------------------------------------------------------- */
intn 
ANIanncmp(VOIDP i,   /* IN: annotation key(tag,ref) */
          VOIDP j,   /* IN: annotation key(tag,ref) */
          intn value)
{
    /* shut compiler up */
    value=value;

    if (*(int32 *)i == *(int32 *)j) 
        return 0;
    if (*(int32 *)i > *(int32 *)j) 
        return -1; 
    else 
        return 1;
} /* ANIanncmp */

/*--------------------------------------------------------------------------
 NAME
    ANIstart - AN-level initialization routine.

 DESCRIPTION
    Register the shut-down routine (ANPshutdown) for call with atexit

 RETURNS
    Returns SUCCEED/FAIL

 AUTHOR
    GeorgeV.

--------------------------------------------------------------------------*/
PRIVATE intn 
ANIstart(void)
{
    CONSTR(FUNC, "ANIstart");    /* for HERROR */
    intn        ret_value = SUCCEED;

    /* Don't call this routine again... */
    library_terminate = TRUE;

    /* Install atexit() library cleanup routine */
    if (HPregister_term_func(&ANdestroy) != 0)
        HGOTO_ERROR(DFE_CANTINIT, FAIL);

  done:
    if(ret_value == FAIL)   
      { /* Error condition cleanup */

      } /* end if */

    /* Normal function cleanup */

    return(ret_value);
} /* end ANIstart() */

/* ------------------------------- ANinit -------------------------------- 
 NAME
	ANinit -- Initialize Annotation Interface

 DESCRIPTION
    Initializes the annotation interface i.e. creates trees
    to hold all open files using annotation interface.

 RETURNS
    SUCCEED or FAIL

 AUTHOR
    GeorgeV.

--------------------------------------------------------------------------- */
PRIVATE int32
ANinit(void)
{
    CONSTR(FUNC, "ANinit");
    int32 ret_value = SUCCEED;
    
    /* Clear error stack */
    HEclear();

    /* Perform global, one-time initialization */
    if (library_terminate == FALSE)
        if(ANIstart()==FAIL)
            HGOTO_ERROR(DFE_CANTINIT, FAIL);

    /* Check to see if we need to create RB tree for file id's */
    if (ANfilelist == NULL)
        if ((ANfilelist = tbbtdmake(ANIfidcmp, sizeof(int32))) == NULL)
            HE_REPORT_GOTO("failed to create TBBT tree for file_ids", FAIL);

    /* Check to see if we need to create RB tree for annotations id's */
    if (ANnodelist == NULL)
        if ((ANnodelist = tbbtdmake(ANIanncmp, sizeof(int32))) == NULL)
            HE_REPORT_GOTO("failed to create TBBT tree for ann_ids", FAIL);

  done:
    if(ret_value == FAIL)   
      { /* Error condition cleanup */

      } /* end if */

    /* Normal function cleanup */

    return ret_value;
} /* ANinit() */

/* ------------------------------- ANdestroy -------------------------------- 
 NAME
	ANdestroy -- Un-Initialize Annotation Interface

 DESCRIPTION
    Unallocates global annotaton node list and file list.

 RETURNS
    SUCCEED or FAIL

 AUTHOR
    GeorgeV.

--------------------------------------------------------------------------- */
intn
ANdestroy(void)
{
#ifdef LATER
    CONSTR(FUNC, "ANdestroy");
#endif /* LATER */
    int32    ret_value = SUCCEED;

    /* Clear error stack */
    HEclear();

    /* Check to see if we need to destroy RB tree for file id's */
    if (ANfilelist != NULL)
        tbbtdfree(ANfilelist, ANfreedata, ANfreekey);

    /* Check to see if we need to destroy RB tree for annotations id's */
    if (ANnodelist != NULL)
        tbbtdfree(ANnodelist, ANfreedata, ANfreekey);

    /* reset pointers */
    ANfilelist = NULL;
    ANnodelist = NULL;
 
    return ret_value;
} /* ANdestroy () */

/*--------------------------------------------------------------------------
 NAME
       ANIaddentry -- add entry to corresponding annotation RB tree

 DESCRIPTION
       Adds annotation to correct tree

 RETURNS
       annotation ID if successful and FAIL (-1) otherwise

 AUTHOR
    GeorgeV.

 -------------------------------------------------------------------------*/
PRIVATE int32
ANIaddentry(int32 an_id,    /* IN: annotation interface id */
            ann_type type,  /* IN: annotation type 
                               AN_DATA_LABEL for data labels, 
                               AN_DATA_DESC for data descriptions,
                               AN_FILE_LABEL for file labels,
                               AN_FILE_DESC for file descritpions.*/ 
            uint16 ann_ref, /* IN: ref of annotation */
            uint16 elmtag,  /* IN: tag of item of which this is annotation */
            uint16 elmref,  /* IN: ref of item of which this is annotation */
            intn new_ann    /* IN: new annotation flag */)
{
    CONSTR(FUNC, "ANIaddentry");
    TBBT_NODE *entry = NULL;
    uint16  ann_tag;
    int32   *ann_id  = NULL;
    int32   *ann_key = NULL;
    ANfile  *file_entry = NULL;
    ANentry *ann_entry  = NULL;
    ANnode  *ann_node   = NULL;
    int32    ret_value  = SUCCEED;
 
    /* Clear error stack */
    HEclear();

    /* Valid file id */
    if (HAatom_group(an_id)!=FIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* First find file node from file tree */
    if ((entry = tbbtdfind(ANfilelist, &an_id, NULL)) == NULL)
        HE_REPORT_GOTO("failed to find an_id", FAIL);

    file_entry = (ANfile *) entry->data; /* get file entry from node */

    /* Check for empty annotation tree of 'type'? */   
    if (file_entry->an_num[type] == -1)
      {
          if ((file_entry->an_tree[type] = 
               (TBBT_TREE *) tbbtdmake(ANIanncmp,sizeof(int32))) == NULL)
              HE_REPORT_GOTO("failed to create annotation tree", FAIL);

          file_entry->an_num[type] = 0;
      }

    /* Which type of annotation file/data label or desc? */
    switch((ann_type)type)
      {
      case AN_DATA_LABEL:
          ann_tag = DFTAG_DIL;
          break;
      case AN_DATA_DESC:
          ann_tag = DFTAG_DIA;
          break;
      case AN_FILE_LABEL:
          ann_tag = DFTAG_FID;
          break;
      case AN_FILE_DESC:
          ann_tag = DFTAG_FD;
          break;
      default:
          HE_REPORT_GOTO("Bad annotation type for this call",FAIL);
      }

    /* allocate space for key */
    if ((ann_key = (int32 *)HDmalloc(sizeof(int32))) == NULL)
        HGOTO_ERROR(DFE_NOSPACE, FAIL);

    /* allocate space for ann_id */
    if ((ann_id = (int32 *)HDmalloc(sizeof(int32))) == NULL)
        HGOTO_ERROR(DFE_NOSPACE, FAIL);

    /* Create 32bit key from type/ref 
     *  ----------------------------
     *  | type(16bits)| ref(16bits) |
     *  -----------------------------*/
    *ann_key = AN_CREATE_KEY(type, ann_ref);

    /* Initialize annotation entry */
    if ((ann_entry = HDmalloc(sizeof(ANentry))) == NULL)
        HGOTO_ERROR(DFE_NOSPACE, FAIL);

    ann_entry->annref  = ann_ref;
    ann_entry->ann_id  = *ann_id = num_anns++;

    if (type == AN_DATA_LABEL || type == AN_DATA_DESC)
      {
          ann_entry->elmtag = (uint16) elmtag;
          ann_entry->elmref = (uint16) elmref;
      }
    else
      {
          ann_entry->elmtag = ann_tag;
          ann_entry->elmref = ann_ref;
      }

    /* Add annotation entry to 'type' tree */
    if (tbbtdins(file_entry->an_tree[type], ann_entry, ann_key) == NULL)
        HE_REPORT_GOTO("failed to insert annotation into 'type' tree", FAIL);

    /* increment number of annotatiosn of 'type' */
    file_entry->an_num[type] += 1;

    /* Initialize annotation node */
    if ((ann_node = HDmalloc(sizeof(ANnode))) == NULL)
        HGOTO_ERROR(DFE_NOSPACE, FAIL);

    ann_node->file_id = an_id;
    ann_node->ann_key = *ann_key;
    ann_node->new_ann = new_ann;

    /* Add annotation node to ann_id RB tree */
    if (tbbtdins(ANnodelist, ann_node, ann_id) == NULL)
        HE_REPORT_GOTO("failed to insert annotation into ann_id tree", FAIL);

    /* return annotation id */
    ret_value = ann_entry->ann_id;

  done:
    if(ret_value == FAIL)   
      { /* Error condition cleanup */
          if (ann_key != NULL)
              HDfree(ann_key);
          if (ann_id != NULL)
              HDfree(ann_id);
          if (ann_entry != NULL)
              HDfree(ann_entry);
          if (ann_node != NULL)
              HDfree(ann_node);
      } /* end if */

    /* Normal function cleanup */

    return ret_value;
} /* ANIaddentry */

/*--------------------------------------------------------------------------
 NAME
     ANIcreate_ann_tree --  create an annotation tree of 'type' for given file 

 DESCRIPTION
     Creates either a label or descritption annotation TBBTtree. 

 RETURNS
     Number of annotations of 'type' in file if successful and 
     FAIL (-1) otherwise

 AUTHOR
    GeorgeV.

 -------------------------------------------------------------------------*/
PRIVATE intn
ANIcreate_ann_tree(int32 an_id,  /* IN: annotation interface id */
                   ann_type type /* IN: AN_DATA_LABEL for data labels, 
                                    AN_DATA_DESC for data descriptions,
                                    AN_FILE_LABEL for file labels,
                                    AN_FILE_DESC for file descritpions.*/)
{
    CONSTR(FUNC, "ANIcreate_ann_tree");
    TBBT_NODE *entry = NULL;
    uint8   datadi[4];
    int32   more_anns;
    int32   aid;
    int32   nanns, i;
    int32   *ann_key = NULL; 
    int32   *ann_id  = NULL; 
    uint16  ann_tag;
    uint16  ann_ref;
    uint8   *dptr = NULL;
    ANfile  *file_entry = NULL;
    ANentry *ann_entry  = NULL;
    ANnode  *ann_node   = NULL;
    intn     ret_value = SUCCEED;

    /* Clear error stack */
    HEclear();

    /* Valid file id */
    if (HAatom_group(an_id)!=FIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* First find file node from file tree */
    if ((entry = tbbtdfind(ANfilelist, &an_id, NULL)) == NULL)
        HE_REPORT_GOTO("failed to find an_id", FAIL);

    file_entry = (ANfile *) entry->data; /* get file entry from node */

    /* Check for empty annotation tree of 'type'? */   
    if (file_entry->an_num[type] == -1)
      {
          if ((file_entry->an_tree[type] = 
               (TBBT_TREE *) tbbtdmake(ANIanncmp,sizeof(int32))) == NULL)
              HE_REPORT_GOTO("failed to create annotation tree", FAIL);
          file_entry->an_num[type] = 0;
      }
    else
      {
          ret_value = file_entry->an_num[type];  /* tree already created */
          goto done;
      }

    /* Which type of annotation data/file label or desc? */
    switch((ann_type)type)
      {
      case AN_DATA_LABEL:
          ann_tag = DFTAG_DIL;
          break;
      case AN_DATA_DESC:
          ann_tag = DFTAG_DIA;
          break;
      case AN_FILE_LABEL:
          ann_tag = DFTAG_FID;
          break;
      case AN_FILE_DESC:
          ann_tag = DFTAG_FD;
          break;
      default:
          HE_REPORT_GOTO("Bad annotation type for this call",FAIL);
      }

    /* Get number of annotations of 'type' in file */
    if ((nanns = Hnumber(an_id, ann_tag)) == 0)
      { /* ZERO annotations of 'type' in file */
          file_entry->an_num[type] = 0;
          ret_value =  file_entry->an_num[type];  
          goto done;
      }

    /* start read on 'type' annotations  in file 
     * note that so far an_id == file_id */
    if ((aid = Hstartread(an_id, ann_tag, DFREF_WILDCARD)) == FAIL)
      {
          Hendaccess(aid);
          HE_REPORT_GOTO("Hstartread failed to read annotation", FAIL);
      }
    else
        more_anns = SUCCEED; 

    /* Process annotations of 'type' in file */
    for (i = 0; (i < nanns) && (more_anns != FAIL); i++)
      { /* see if annotation is there */
          if (FAIL == Hinquire(aid, (int32 *) NULL, (uint16 *) NULL, &ann_ref,
                               (int32 *) NULL, (int32 *) NULL, (int32 *) NULL,
                               (int16 *) NULL, (int16 *) NULL))
            {
                file_entry->an_num[type] = nanns;
                ret_value =  file_entry->an_num[type];  
                goto done;
            }

          /* if data annotation, read data annotation tag/ref */
          if(type != AN_FILE_LABEL && type != AN_FILE_DESC)
            {
                if ((int32) FAIL == Hread(aid, (int32) 4, datadi))
                  {
                      file_entry->an_num[type] = nanns;
                      ret_value =  file_entry->an_num[type];  
                      goto done;
                  }
            }

          /* allocate space for key */
          if ((ann_key = (int32 *)HDmalloc(sizeof(int32))) == NULL)
              HGOTO_ERROR(DFE_NOSPACE, FAIL);

          /* allocate space for ann_id */
          if ((ann_id = (int32 *)HDmalloc(sizeof(int32))) == NULL)
              HGOTO_ERROR(DFE_NOSPACE, FAIL);

          /* Allocate & Initialize annotation entry 
           *  decode data tag/ref */
          if ((ann_entry = HDmalloc(sizeof(ANentry))) == NULL)
              HGOTO_ERROR(DFE_NOSPACE, FAIL);

          ann_entry->annref  = ann_ref;
          ann_entry->ann_id  = *ann_id = num_anns++;

          /* Check if data annotation to decode data tag/ref */
          if(type != AN_FILE_LABEL && type != AN_FILE_DESC)
            {
                dptr = (uint8 *) &(datadi[0]);
                UINT16DECODE(dptr, ann_entry->elmtag);
                UINT16DECODE(dptr, ann_entry->elmref);
            }
          else
            {
                ann_entry->elmtag = ann_tag;
                ann_entry->elmref = ann_ref;
            }

          /* Create key from tag/ref pair 
           *  ----------------------------
           *  | tag(16bits) | ref(16bits) |
           *  -----------------------------*/
          *ann_key = AN_CREATE_KEY(type, ann_ref);

          /* Add annotation entry to 'type' tree */
          if (tbbtdins(file_entry->an_tree[type], ann_entry, ann_key) == NULL)
              HE_REPORT_GOTO("failed to insert annotation into 'type' tree", FAIL);

          /* Initialize annotation node */
          if ((ann_node = HDmalloc(sizeof(ANnode))) == NULL)
              HGOTO_ERROR(DFE_NOSPACE, FAIL);

          ann_node->file_id = an_id;
          ann_node->ann_key = *ann_key;
          ann_node->new_ann = 0;       /* not a newly created annotation */

          /* Add annotation node to ann_id RB tree */
          if (tbbtdins(ANnodelist, ann_node, ann_id) == NULL)
              HE_REPORT_GOTO("failed to insert annotation into ann_id tree", FAIL);
        
          /* set read on next annotation */
          more_anns = Hnextread(aid, ann_tag, DFREF_WILDCARD, DF_CURRENT);
      } /* end for "more_anns" */
    
    /* Finish access*/
    Hendaccess(aid);

    file_entry->an_num[type] = nanns;
    ret_value =  file_entry->an_num[type];  

  done:
    if(ret_value == FAIL)   
      { /* Error condition cleanup */
          if (ann_key != NULL)
              HDfree(ann_key);
          if (ann_id != NULL)
              HDfree(ann_id);
          if (ann_entry != NULL)
              HDfree(ann_entry);
          if (ann_node != NULL)
              HDfree(ann_node);
      } /* end if */

    /* Normal function cleanup */

    return ret_value;
} /* ANIcreate_ann_tree */

#if NOT_USED_YET
/*--------------------------------------------------------------------------
 NAME
     ANIfind -- find annotation id for given annotation type and ref number

 DESCRIPTION
     Find annotation id for the given annotation type and ref number

 RETURNS
    Annotation id if successful and FAIL (-1) otherwise

 AUTHOR
    GeorgeV.

 -------------------------------------------------------------------------*/ 
PRIVATE int32
ANIfind(int32 an_id,   /* IN: annotation interface id */
        ann_type type, /* IN: AN_DATA_LABEL for data labels, 
                          AN_DATA_DESC for data descriptions,
                          AN_FILE_LABEL for file labels,
                          AN_FILE_DESC for file descritpions.*/ 
        uint16 ann_ref /* IN: ref of annotation */)
{
    CONSTR(FUNC, "ANIfind");
    TBBT_NODE *entry = NULL;
    ANfile  *file_entry = NULL;
    ANentry *ann_entry  = NULL;
    int32   ann_key;
    int32   ret_value = SUCCEED;

    /* Clear error stack */
    HEclear();

    /* Valid file id */
    if (HAatom_group(an_id)!=FIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* First find file node from file tree */
    if ((entry = tbbtdfind(ANfilelist, &an_id, NULL)) == NULL)
        HE_REPORT_GOTO("failed to find an_id", FAIL);

    file_entry = (ANfile *) entry->data; /* get file entry from node */

    /* Check for empty annotation tree of 'type'? */   
    if (file_entry->an_num[type] == -1)
      {
          if ((file_entry->an_tree[type] = 
               (TBBT_TREE *) tbbtdmake(ANIanncmp,sizeof(int32))) == NULL)
              HE_REPORT_GOTO("failed to create annotation tree", FAIL);

          file_entry->an_num[type] = 0; /* intialize after allocation */
      }

    /* Create key from type/ref pair 
     *  ----------------------------
     *  | type(16bits) | ref(16bits) |
     *  -----------------------------*/
    ann_key = AN_CREATE_KEY(type, ann_ref);

    /* See if annotation of 'type' with ref exits */
    if ((entry = tbbtdfind(file_entry->an_tree[type], &ann_key, NULL)) == NULL)
        HE_REPORT_GOTO("failed to find annotation of 'type'", FAIL);

    /* get annotation entry from node */
    ann_entry = (ANentry *) entry->data; 

    /* return annotation id */
    ret_value = ann_entry->ann_id;

  done:
    if(ret_value == FAIL)   
      { /* Error condition cleanup */

      } /* end if */

    /* Normal function cleanup */

    return ret_value;
} /* ANIfind */
#endif /* NOT_USED_YET */

/*--------------------------------------------------------------------------
 NAME
   ANInumann -- find number of annotation of 'type' that 
                 match the given element tag/ref 

 DESCRIPTION
   Find number of annotation of 'type' for the given element 
   tag/ref pair. Doesn't handle file lables/descs yet.

 RETURNS
   Number of annotation found if successful and FAIL (-1) otherwise

 AUTHOR
    GeorgeV.

 -------------------------------------------------------------------------*/ 
PRIVATE intn
ANInumann(int32 an_id,     /* IN: annotation interface id */
          ann_type type,   /* IN: AN_DATA_LABEL for data labels, 
                              AN_DATA_DESC for data descriptions,
                              AN_FILE_LABEL for file labels,
                              AN_FILE_DESC for file descritpions.*/
          uint16 elem_tag, /* IN: tag of item of which this is annotation */
          uint16 elem_ref  /* IN: ref of item of which this is annotation */)
{
    CONSTR(FUNC, "ANInumann");
    TBBT_NODE *entry = NULL;
    ANfile  *file_entry = NULL;
    ANentry *ann_entry  = NULL;
    intn    nanns = 0; 
    intn    ret_value = SUCCEED;

    /* Clear error stack */
    HEclear();

    /* Valid file id */
    if (HAatom_group(an_id)!=FIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* First find file node from file tree */
    if ((entry = tbbtdfind(ANfilelist, &an_id, NULL)) == NULL)
        HE_REPORT_GOTO("failed to find an_id", FAIL);

    file_entry = (ANfile *) entry->data; /* get file entry from node */

    /* Empty annotation tree */
    if (file_entry->an_num[type] == -1)
      {
          if (ANIcreate_ann_tree(an_id, type) == FAIL)
              HGOTO_ERROR(DFE_BADCALL, FAIL);
      }

    /* Traverse the list looking for a match */
    for(entry = tbbtfirst((TBBT_NODE *)*(file_entry->an_tree[type])); 
        entry != NULL; entry = tbbtnext(entry))
      {
          ann_entry = (ANentry *) entry->data; /* get annotation entry from node */
          if ((ann_entry->elmref == elem_ref) && (ann_entry->elmtag == elem_tag))
            {
                nanns++; /* increment ref counter if match */
            }
      }

    /* return number of annotation references found for tag/ref */
    ret_value = nanns;

  done:
    if(ret_value == FAIL)   
      { /* Error condition cleanup */

      } /* end if */

    /* Normal function cleanup */

    return ret_value;
} /* ANInumann */

/*--------------------------------------------------------------------------
 NAME
   ANIannlist -- generate list of annotation ids of 'type' that 
                 match the given element tag/ref 

 DESCRIPTION
       Find and generate list of annotation ids of 'type' for the given 
       element tag/ref pair

 RETURNS
       number of annotations ids found if successful and FAIL (-1) otherwise

 AUTHOR
    GeorgeV.

 ------------------------------------------------------------------------*/ 
PRIVATE intn
ANIannlist(int32 an_id,     /* IN: annotation interface id */
           ann_type type,   /* IN: AN_DATA_LABEL for data labels, 
                               AN_DATA_DESC for data descriptions,
                               AN_FILE_LABEL for file labels,
                               AN_FILE_DESC for file descritpions.*/
           uint16 elem_tag, /* IN: tag of item of which this is annotation*/
           uint16 elem_ref, /* IN: ref of item of which this is annotation */
           int32 ann_list[] /* OUT: array of ann_id's that match criteria. */)
{
    CONSTR(FUNC, "ANIannlist");
    TBBT_NODE *entry = NULL;
    ANfile  *file_entry = NULL;
    ANentry *ann_entry  = NULL;
    intn    nanns = 0;
    intn    ret_value = SUCCEED;

    /* Clear error stack */
    HEclear();

    /* Valid file id */
    if (HAatom_group(an_id)!=FIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* First find file node from file tree */
    if ((entry = tbbtdfind(ANfilelist, &an_id, NULL)) == NULL)
        HE_REPORT_GOTO("failed to find an_id", FAIL);

    file_entry = (ANfile *) entry->data; /* get file entry from node */

    /* Empty annotation tree */
    if (file_entry->an_num[type] == -1)
      {
          if (ANIcreate_ann_tree(an_id, type) == FAIL)
              HGOTO_ERROR(DFE_BADCALL, FAIL);
      }

    /* Traverse the list looking for a match */
    for(entry = tbbtfirst((TBBT_NODE *)*(file_entry->an_tree[type])); 
        entry != NULL; entry = tbbtnext(entry))
      {
          ann_entry = (ANentry *) entry->data; /* get annotation entry from node */
          if ((ann_entry->elmref == elem_ref) && (ann_entry->elmtag == elem_tag))
            { /* save ref of ann match in list */
                ann_list[nanns++] = ann_entry->ann_id; 
            }
      }

    /* return number of annotation id's found for tag/ref */
    ret_value = nanns;

  done:
    if(ret_value == FAIL)   
      { /* Error condition cleanup */

      } /* end if */

    /* Normal function cleanup */

    return ret_value;
} /* ANIannlist */

/*--------------------------------------------------------------------------
 NAME
       ANIannlen -- get length of annotation givne annotation id

 DESCRIPTION
       Uses the annotation id to find ann_key & file_id

 RETURNS
       length of annotation if successful and FAIL (-1) otherwise

 AUTHOR
    GeorgeV.

 ------------------------------------------------------------------------*/
PRIVATE int32
ANIannlen(int32 ann_id /*  IN: annotation id */)
{
#ifdef LATER
    CONSTR(FUNC, "ANIannlen");
#endif /* LATER */
    TBBT_NODE *entry = NULL;
    ANnode  *ann_node   = NULL;
    int32  file_id;
    int32  type;
    int32  ann_key;
    uint16 ann_tag;
    uint16 ann_ref;
    int32  ann_length=(-1);
    int32  ret_value = SUCCEED;

    /* Clear error stack */
    HEclear();

    /* First find file node from annotation node tree */
    if ((entry = tbbtdfind(ANnodelist, &ann_id, NULL)) == NULL)
        HE_REPORT_GOTO("failed to find ann_id", FAIL);

    ann_node = (ANnode *) entry->data; /* get ann node from node */

    /* get file id and annotation key */
    file_id = ann_node->file_id;
    ann_key = ann_node->ann_key;
    type    = AN_KEY2TYPE(ann_key);
    ann_ref = AN_KEY2REF(ann_key);

    /* Valid file id */
    if (file_id == FAIL)
        HE_REPORT_GOTO("bad file_id", FAIL);

    /* set type annotation tag */
    switch((int32)type)
      {
      case AN_DATA_LABEL:
          ann_tag = DFTAG_DIL;
          break;
      case AN_DATA_DESC:
          ann_tag = DFTAG_DIA;
          break;
      case AN_FILE_LABEL:
          ann_tag = DFTAG_FID;
          break;
      case AN_FILE_DESC:
          ann_tag = DFTAG_FD;
          break;
      default:
          HE_REPORT_GOTO("Bad annotation type for this call",FAIL);
      }

    if (ann_tag == DFTAG_DIL || ann_tag == DFTAG_DIA)
      {  /* 4=length of data tag/ref for data label or description */
          if ((ann_length = Hlength(file_id, ann_tag, ann_ref)) == FAIL) 
              HE_REPORT_GOTO("Failed to find annotation length", FAIL);
          ann_length -= 4;
      }
    else if (ann_tag == DFTAG_FID || ann_tag == DFTAG_FD)
      {  /* not data tag/ref for file label or description */
          if ((ann_length = Hlength(file_id, ann_tag, ann_ref)) == FAIL) 
              HE_REPORT_GOTO("Failed to find annotation length", FAIL);
      }

    /* return the length */
    ret_value = (ann_length);

  done:
    if(ret_value == FAIL)   
      { /* Error condition cleanup */

      } /* end if */

    /* Normal function cleanup */

    return ret_value;
} /* ANIannlen */

/*--------------------------------------------------------------------------
 NAME
       ANIreadann -- read annotation given ann_id

 DESCRIPTION
       Gets tag and ref of annotation.  Finds DD for that annotation.
       Reads the annotation, taking care of NULL terminator, if necessary.

 RETURNS
       SUCCEED (0) if successful and FAIL (-1) otherwise

 AUTHOR
    GeorgeV.

 ------------------------------------------------------------------------*/
PRIVATE intn
ANIreadann(int32 ann_id, /* IN: annotation id (handle) */ 
           char *ann,    /* OUT: space to return annotation in */
           int32 maxlen  /* IN: size of space to return annotation in */)
{
    CONSTR(FUNC, "ANIreadann");
    TBBT_NODE *entry = NULL;
    ANnode  *ann_node   = NULL;
    int32   file_id;
    int32   type;
    int32   ann_key;
    int32   aid;
    int32   ann_len;
    uint16  ann_tag;
    uint16  ann_ref;
    uint8   datadi[4] = {0,0,0,0};   /* to read in and discard data/ref! */
    intn    ret_value = SUCCEED;

    /* Clear error stack */
    HEclear();

    /* First find file node from file tree */
    if ((entry = tbbtdfind(ANnodelist, &ann_id, NULL)) == NULL)
        HE_REPORT_GOTO("failed to find ann_id", FAIL);

    ann_node = (ANnode *) entry->data; /* get ann node from node */

    /* get file id and annotation key */
    file_id = ann_node->file_id;
    ann_key = ann_node->ann_key;
    type    = AN_KEY2TYPE(ann_key);
    ann_ref = AN_KEY2REF(ann_key);

    /* Valid file id */
    if (file_id == FAIL)
        HGOTO_ERROR(DFE_BADCALL, FAIL);

    /* set type tag */
    switch((int32)type)
      {
      case AN_DATA_LABEL:
          ann_tag = DFTAG_DIL;
          break;
      case AN_DATA_DESC:
          ann_tag = DFTAG_DIA;
          break;
      case AN_FILE_LABEL:
          ann_tag = DFTAG_FID;
          break;
      case AN_FILE_DESC:
          ann_tag = DFTAG_FD;
          break;
      default:
          HE_REPORT_GOTO("Bad annotation type for this call",FAIL);
      }

    /* find DD for that annotation using tag/ref */
    aid = Hstartread(file_id, ann_tag, ann_ref);
    if (aid == FAIL)
      {
          Hendaccess(aid);
          HE_REPORT_GOTO("Failed to get access to annotation",FAIL);
      }
    if (FAIL == Hinquire(aid, (int32 *) NULL, (uint16 *) NULL, (uint16 *) NULL, 
                         &ann_len, (int32 *) NULL, (int32 *) NULL, 
                         (int16 *) NULL, (int16 *) NULL))
      {
          Hendaccess(aid);
          HE_REPORT_GOTO("Failed to get annotation",FAIL);
      }

    /* first four bytes were tag/ref if data annotation, so they don't count */
    if (ann_tag == DFTAG_DIL || ann_tag == DFTAG_DIA)
        ann_len -= 4;    
#ifdef AN_DEBUG
    printf("ANIreadann: from Hinquire, ann_len=%d, maxlen=%d\n", 
           ann_len, maxlen);
#endif
    /* Check length of space provided
     * if not enough space, truncate annotation 
     * Labels need space for null terminator, Descriptions don't */
    if (ann_tag == DFTAG_FID || ann_tag == DFTAG_DIL)
      { /* Labels */
          if (ann_len > maxlen - 1)
              ann_len = maxlen - 1;
      }
    else
      { /* Descriptions */
          if (ann_len > maxlen)
              ann_len = maxlen;
      }

    /* If data label or description need to read past tag/ref */
    if (ann_tag == DFTAG_DIL || ann_tag == DFTAG_DIA)
      { /* Data label/description */
          if ((int32) FAIL == Hread(aid, (int32) 4, datadi))
            {     /* go past tag/ref */
                Hendaccess(aid);
                HE_REPORT_GOTO("Failed to go past tag/ref",FAIL);
            }
      }

    /* read itslef annotation now..*/
    if ((int32) FAIL == Hread(aid, ann_len, ann))
      {     /* read the annotation */
          Hendaccess(aid);
          HE_REPORT_GOTO("Failed to read annotation",FAIL);
      }

    /* If Label need to NULL terminate string */
    if (ann_tag == DFTAG_FID || ann_tag == DFTAG_DIL)
        ann[ann_len] = '\0';     
#ifdef AN_DEBUG
    printf("ANIreadann: ann_len=%d, ann=%s\n", ann_len,ann);
#endif
    /* Close access to annotation object */
    Hendaccess(aid);

  done:
    if(ret_value == FAIL)   
      { /* Error condition cleanup */

      } /* end if */

    /* Normal function cleanup */

    return ret_value;
} /* ANIreadann */

/*--------------------------------------------------------------------------
 NAME
       ANIwriteann -- write annotation given ann_id

 DESCRIPTION
       Checks for pre-existence of given annotation, replacing old one if it
       exists. Writes out annotation.

 RETURNS
       SUCCEED (0) if successful and FAIL (-1) otherwise

 AUTHOR
    GeorgeV.

 ------------------------------------------------------------------------*/
PRIVATE intn
ANIwriteann(int32 ann_id,    /* IN: annotation id */
            const char *ann, /* IN: annotation to write */
            int32 ann_len    /* IN: length of annotation */)
{
    CONSTR(FUNC, "ANIwriteann");
    TBBT_NODE *fentry = NULL;
    TBBT_NODE *entry  = NULL;
    ANfile  *file_entry = NULL;
    ANentry *ann_entry  = NULL;
    ANnode  *ann_node   = NULL;
    int32   file_id;
    int32   type;
    int32   ann_key;
    int32   aid;
    int     newflag = 0;
    uint16  ann_tag;
    uint16  ann_ref;
    uint16  elem_tag;
    uint16  elem_ref;
    uint8   datadi[4] = {0,0,0,0};      /* to hold data tag/ref for writing */
    uint8   *ptr = NULL;
    intn    ret_value = SUCCEED;

    /* Clear error stack */
    HEclear();

    /* First find node from ann_id tree */
    if ((entry = tbbtdfind(ANnodelist, &ann_id, NULL)) == NULL)
        HE_REPORT_GOTO("failed to find ann_id", FAIL);

    ann_node = (ANnode *) entry->data; /* get ann node from node */

    /* get file id and annotation key */
    file_id = ann_node->file_id;
    ann_key = ann_node->ann_key;
    type    = AN_KEY2TYPE(ann_key);
    ann_ref = AN_KEY2REF(ann_key);

    /* Valid file id */
    if (file_id == FAIL)
        HGOTO_ERROR(DFE_BADCALL, FAIL);

    /* set type tag */
    switch((int32)type)
      {
      case AN_DATA_LABEL:
          ann_tag = DFTAG_DIL;
          break;
      case AN_DATA_DESC:
          ann_tag = DFTAG_DIA;
          break;
      case AN_FILE_LABEL:
          ann_tag = DFTAG_FID;
          break;
      case AN_FILE_DESC:
          ann_tag = DFTAG_FD;
          break;
      default:
          HE_REPORT_GOTO("Bad annotation type for this call",FAIL);
      }

    /* First find file node from file tree */
    if ((fentry = tbbtdfind(ANfilelist, &file_id, NULL)) == NULL)
        HE_REPORT_GOTO("failed to find file_id", FAIL);

    file_entry = (ANfile *) fentry->data; /* get file entry from node */

    /* Get annotation entry so that we can extract tag/ref of element 
     * Note that for file labels and descriptions the tag/ref contain
     * DFTAG_XXX and annotation reference number */
    if ((entry = tbbtdfind(file_entry->an_tree[type], &ann_key, NULL)) == NULL)
        HE_REPORT_GOTO("failed to retrieve annotation of 'type' tree", FAIL);

    ann_entry = (ANentry *) entry->data;

    elem_tag = ann_entry->elmtag;
    elem_ref = ann_entry->elmref;
    newflag  = ann_node->new_ann;

    /* is this a new annotation */
    if (newflag == 1)
      { 
          ann_node->new_ann = 0; /* set new annotation entry to false */
      }

    /* If annotation exists, delete it and rewrite new annotation  */
    if (newflag == 0)
      {  /* does prev annotation exist? */
          if (Hdeldd(file_id, ann_tag, ann_ref) == FAIL)
              HE_REPORT_GOTO("Unable to replace old annotation",FAIL);
      }

    if (ann_tag == DFTAG_DIL || ann_tag == DFTAG_DIA)
      { /* Data label/description 
         * Note: cannot use Hputelement because need to write data tag/ref */
          aid = Hstartwrite(file_id, ann_tag, ann_ref, ann_len + 4);
          if (aid == FAIL)
            {
                Hendaccess(aid);
                HE_REPORT_GOTO("Failed to start write access on annotation",FAIL);
            }

          /* Setup up to write annotation tag/ref */
          ptr = (uint8 *) &(datadi[0]);   /* first, write the object's tag/ref */
          UINT16ENCODE(ptr, elem_tag);
          UINT16ENCODE(ptr, elem_ref);

          /* write annotation tag/ref of element */
          if ((int32) FAIL == Hwrite(aid, (int32) 4, datadi))
            {
                Hendaccess(aid);
                HE_REPORT_GOTO("Failed to write tag/ref of annotation",FAIL);
            }

#ifdef AN_DEBUG
          printf("ANIwriteann: ann_len=%d, ann=%s\n", ann_len,ann);
#endif
          /* then write the annotation itself */
          if ((int32) FAIL == Hwrite(aid, ann_len, (const VOIDP)ann))
            {     
                Hendaccess(aid);
                HE_REPORT_GOTO("Failed to write annotation",FAIL);
            }

          Hendaccess(aid); /* end access to annotation */
      }
    else
      { /* file label/description 
         * write out file label/description */
          if (FAIL == Hputelement(file_id, ann_tag, ann_ref, (VOIDP)ann, ann_len))
              HE_REPORT_GOTO("Failed to write file annotation",FAIL);
#ifdef AN_DEBUG
          printf("ANIwriteann: fann_len=%d, fann=%s\n", ann_len,ann);
#endif
      }

  done:
    if(ret_value == FAIL)   
      { /* Error condition cleanup */

      } /* end if */

    /* Normal function cleanup */

    return ret_value;
} /* ANIwriteann */

/* ------------------------------- ANIcreate ------------------------------- 
 NAME
	ANIcreate - create a new annotation and return a handle(id)

 DESCRIPTION
    Creates an annotation, returns an 'an_id' to work with the new 
    annotation which can either be a label or description.

 RETURNS
    An ID to an annotation which can either be a label or description

 AUTHOR
    GeorgeV.

--------------------------------------------------------------------------- */
PRIVATE intn 
ANIcreate(int32 file_id,   /* IN: file ID */
          uint16 elem_tag, /* IN: tag of item to be assigned annotation */
          uint16 elem_ref, /* IN: reference number of itme to be assigned ann */ 
          ann_type type    /* IN: AN_DATA_LABEL for data labels, 
                              AN_DATA_DESC for data descriptions,
                              AN_FILE_LABEL for file labels,
                              AN_FILE_DESC for file descritpions.*/)
{
    CONSTR(FUNC, "ANIcreate");    /* for HERROR */
    int32   ann_id;
    uint16  ann_tag;
    uint16  ann_ref;
    intn    ret_value = SUCCEED;

    /* Clear error stack */
    HEclear();
  
    /* Valid file id */
    if (HAatom_group(file_id)!=FIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* deal with type */
    switch((ann_type)type)
      {
      case AN_DATA_LABEL:
          ann_tag = DFTAG_DIL;
          ann_ref = Htagnewref(file_id,ann_tag);
          break;
      case AN_DATA_DESC:
          ann_tag = DFTAG_DIA;
          ann_ref = Htagnewref(file_id,ann_tag);
          break;
      case AN_FILE_LABEL:
          /* for file label set elmement tag/ref to ann_tag & ref */
          ann_tag = DFTAG_FID;
          ann_ref = Htagnewref(file_id,ann_tag);
          elem_tag = ann_tag;
          elem_ref = ann_ref;
          break;
      case AN_FILE_DESC:
          /* for file desc set elmement tag/ref to ann_tag & ref */
          ann_tag = DFTAG_FD;
          ann_ref = Htagnewref(file_id,ann_tag);
          elem_tag = ann_tag;
          elem_ref = ann_ref;
          break;
      default:
          HE_REPORT_GOTO("Bad annotation type for this call",FAIL);
      }

    /* Check tag and ref */
    if (!elem_tag)
        HGOTO_ERROR(DFE_BADTAG, FAIL);
    if (!elem_ref)
        HGOTO_ERROR(DFE_BADREF, FAIL);
  
    /* put new annotation tag/ref into 'type' TBBTtree */
    if ((ann_id = ANIaddentry(file_id, type, ann_ref, elem_tag, elem_ref, 1)) == FAIL)
        HE_REPORT_GOTO("Failed to add annotation to RB tree",FAIL);

    ret_value = ann_id;

  done:
    if(ret_value == FAIL)   
      { /* Error condition cleanup */

      } /* end if */

    /* Normal function cleanup */

    return ret_value;
} /* ANIcreate() */

/* --------------------- Exported Multi-file Interface ----------------------*/

/* ------------------------------- ANstart -------------------------------- 
 NAME
	ANstart -- open file for annotation handling

 DESCRIPTION
    Start annotation handling on the file return a annotation ID to the file.

 RETURNS
    A file ID or FAIL. Note that we use 'an_id' which is the same
    as the file id.

 AUTHOR
    GeorgeV.

--------------------------------------------------------------------------- */
EXPORT int32
ANstart(int32 file_id /* IN: file to start annotation access on*/)
{
    CONSTR(FUNC, "ANstart");
    int32  *an_id    = NULL;
    ANfile *file_entry = NULL;
    int32   ret_value = SUCCEED;

#ifdef HAVE_PABLO
    TRACE_ON(AN_mask, ID_ANstart);
#endif /* HAVE_PABLO */

    /* Clear error stack */
    HEclear();

    /* check for valid file id */
    if (HAatom_group(file_id)!=FIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* Check to see if we need to call ANInit, needs to moved to 
     * overall initialization call */
    if (ANfilelist == NULL || ANnodelist == NULL)
        ANinit();

    /* Check to see if this file_id is already in file tree */
    if (tbbtdfind(ANfilelist, &an_id, NULL) == NULL)
      { /* file is not in file tree so create a new entry */

          /* allocate space for file handle */
          if ((an_id = HDmalloc(sizeof(int32))) == NULL)
              HGOTO_ERROR(DFE_NOSPACE, FAIL);

          *an_id = file_id;

          /* Intialize file entry */
          if ((file_entry = HDmalloc(sizeof(ANfile))) == NULL)
              HGOTO_ERROR(DFE_NOSPACE, FAIL);
          file_entry->access_mode = 0;
          file_entry->an_tree[AN_DATA_LABEL] = NULL;   /* data label ann tree */
          file_entry->an_tree[AN_DATA_DESC]  = NULL;   /* data description ann tree */
          file_entry->an_tree[AN_FILE_LABEL] = NULL;   /* file label ann tree */
          file_entry->an_tree[AN_FILE_DESC]  = NULL;   /* file description ann tree */
          file_entry->an_num[AN_DATA_LABEL] = -1;   
          file_entry->an_num[AN_DATA_DESC]  = -1;   
          file_entry->an_num[AN_FILE_LABEL] = -1;   
          file_entry->an_num[AN_FILE_DESC]  = -1;   
          file_entry->filename = NULL;

          /* Add file entry to file tree */
          if (tbbtdins(ANfilelist, file_entry, an_id) == NULL)
              HE_REPORT_GOTO("Failed to insert file_id into TBBTtree", FAIL);

          /* return file id */
          ret_value = ((int32)*an_id);
      }
    else 
      { /* file already added to file tree so return file_id*/
          ret_value = file_id;
      }

  done:
    if(ret_value == FAIL)   
      { /* Error condition cleanup */
          if (an_id != NULL)
              HDfree(an_id);
          if(file_entry != NULL)
              HDfree(file_entry);
      } /* end if */

    /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(AN_mask, ID_ANstart);
#endif /* HAVE_PABLO */

    return ret_value;
} /* ANstart() */

/*--------------------------------------------------------------------------
 NAME
    ANfileinfo - Report high-level information about the ANxxx interface for a given file.

 DESCRIPTION
    Reports general information about the number of file and object(i.e. data)
    annotations in the file. This routine is generally used to find
    the range of acceptable indices for ANselect calls.

 RETURNS
    SUCCEED/FAIL

 AUTHOR
    GeorgeV.

--------------------------------------------------------------------------*/
EXPORT intn 
ANfileinfo(int32 an_id,         /* IN:  annotation interface id */
           int32 *n_file_label, /* OUT: the # of file labels */
           int32 *n_file_desc,  /* OUT: the # of file descriptions */
           int32 *n_obj_label,  /* OUT: the # of object labels */
           int32 *n_obj_desc    /* OUT: the # of object descriptions */)
{
    CONSTR(FUNC, "ANfileinfo");    /* for HERROR */
    TBBT_NODE *entry = NULL;
    ANfile  *file_entry = NULL;
    intn    ret_value = SUCCEED;

#ifdef HAVE_PABLO
    TRACE_ON(AN_mask, ID_ANfileinfo);
#endif /* HAVE_PABLO */

    /* Clear error stack */
    HEclear();

    /* Valid file id */
    if (HAatom_group(an_id)!=FIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* First find file node from file tree */
    if ((entry = tbbtdfind(ANfilelist, &an_id, NULL)) == NULL)
        HE_REPORT_GOTO("failed to find an_id", FAIL);

    file_entry = (ANfile *) entry->data; /* get file entry from node */

    /* Empty file label annotation tree? */
    if (file_entry->an_num[AN_FILE_LABEL] == -1)
      {
          if ((*n_file_label = ANIcreate_ann_tree(an_id, AN_FILE_LABEL)) == FAIL)
              HE_REPORT_GOTO("failed to create file label annotation TBBTtree", FAIL);
      }
    else
        *n_file_label = file_entry->an_num[AN_FILE_LABEL];

    /* Empty file descritpiton annotation tree? */
    if (file_entry->an_num[AN_FILE_DESC] == -1)
      {
          if ((*n_file_desc = ANIcreate_ann_tree(an_id, AN_FILE_DESC)) == FAIL)
              HE_REPORT_GOTO("failed to create file desc annotation TBBTtree", FAIL);
      }
    else
        *n_file_desc = file_entry->an_num[AN_FILE_DESC];

    /* Empty label annotation tree? */
    if (file_entry->an_num[AN_DATA_LABEL] == -1)
      {
          if ((*n_obj_label = ANIcreate_ann_tree(an_id, AN_DATA_LABEL)) == FAIL)
              HE_REPORT_GOTO("failed to create data label annotation TBBTtree", FAIL);
      }
    else
        *n_obj_label = file_entry->an_num[AN_DATA_LABEL];

    /* Empty descritpiton annotation tree? */
    if (file_entry->an_num[AN_DATA_DESC] == -1)
      {
          if ((*n_obj_desc = ANIcreate_ann_tree(an_id, AN_DATA_DESC)) == FAIL)
              HE_REPORT_GOTO("failed to create data desc annotation TBBTtree", FAIL);
      }
    else
        *n_obj_desc = file_entry->an_num[AN_DATA_DESC];

  done:
    if(ret_value == FAIL)   
      { /* Error condition cleanup */

      } /* end if */

    /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(AN_mask, ID_ANfileinfo);
#endif /* HAVE_PABLO */

    return ret_value;
} /* ANfileinfo() */

/* -------------------------------- ANend ---------------------------------
 NAME
	ANend -- End annotation access to file file

 DESCRIPTION
    End annotation access to file.

 RETURNS
    SUCCEED / FAIL
--------------------------------------------------------------------------- */
EXPORT int32
ANend(int32 an_id /* IN: Annotation ID of file to close */)
{
    CONSTR(FUNC,"ANend");
    TBBT_NODE *fentry = NULL;
    TBBT_NODE *aentry = NULL;
    TBBT_NODE *entry  = NULL;
    ANfile  *file_entry = NULL;
    ANentry *ann_entry  = NULL;
    ANnode  *ann_node   = NULL;
    VOID    *ann_key = NULL;
    VOID    *kp = NULL;
    int32   ret_value = SUCCEED;

#ifdef HAVE_PABLO
    TRACE_ON(AN_mask, ID_ANend);
#endif /* HAVE_PABLO */
  
    /* Clear error stack */
    HEclear();

    /* Valid file id */
    if (HAatom_group(an_id)!=FIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* First find file node from file tree */
    if ((fentry = tbbtdfind(ANfilelist, &an_id, NULL)) == NULL)
        HE_REPORT_GOTO("failed to find an_id", FAIL);

    file_entry = (ANfile *) fentry->data; /* get file entry from node */

    if (file_entry->filename != NULL)
        HDfree(file_entry->filename);  /* free file name */
  
    /* NEED to delete trees of annotations attached to node 
     * NOTE: This could be written shorter using a for loop....
     * or something....  */

    /* free file label annotation rb tree */
    if (file_entry->an_tree[AN_FILE_LABEL] != NULL) 
      { /* Traverse tree puling ann_id's to delete from ANnodelist */
          for(aentry = tbbtfirst((TBBT_NODE *)*(file_entry->an_tree[AN_FILE_LABEL])); 
              aentry != NULL;
              aentry = tbbtnext(aentry))
            { /* get annotation entry from node */
                ann_entry = (ANentry *) aentry->data; 

                /* See if annotation exits */
                if ((entry = tbbtdfind(ANnodelist, &(ann_entry->ann_id), NULL)) 
                    == NULL)
                    HE_REPORT_GOTO("Failed to find annotation with ann_id", FAIL);

                /* delete node from ANndoelist */
                if ((ann_node =(ANnode *) tbbtrem((TBBT_NODE **)ANnodelist, entry, 
                                                  &ann_key))== NULL)
                    HGOTO_ERROR(DFE_BADCALL, FAIL);      

                if(ann_node != NULL)
                    HDfree(ann_node); /* free node */
                if(ann_key != NULL)
                    HDfree(ann_key);  /* free key */

            } /* end for 'entry */
          /* finally free tree */
          tbbtdfree(file_entry->an_tree[AN_FILE_LABEL], ANfreedata, ANfreekey);  
      }

    /* free file desc annotation rb tree */
    if (file_entry->an_tree[AN_FILE_DESC] != NULL) 
      { /* Traverse tree puling ann_id's to delete from ANnodelist */
          for(aentry = tbbtfirst((TBBT_NODE *)*(file_entry->an_tree[AN_FILE_DESC])); 
              aentry != NULL;
              aentry = tbbtnext(aentry))
            { /* get annotation entry from node */
                ann_entry = (ANentry *) aentry->data; 

                /* See if annotation exits */
                if ((entry = tbbtdfind(ANnodelist, &(ann_entry->ann_id), NULL)) 
                    == NULL)
                    HE_REPORT_GOTO("Failed to find annotation with ann_id", FAIL);

                /* delete node from ANndoelist */
                if ((ann_node =(ANnode *) tbbtrem((TBBT_NODE **)ANnodelist, entry, 
                                                  &ann_key))== NULL)
                    HGOTO_ERROR(DFE_BADCALL, FAIL);      

                if (ann_node != NULL)
                    HDfree(ann_node);   /* free node */
                if (ann_key != NULL)
                    HDfree(ann_key);    /* free key */

            } /* end for 'entry */
          /* finally free tree */
          tbbtdfree(file_entry->an_tree[AN_FILE_DESC], ANfreedata, ANfreekey);  
      }

    /* free label annotation rb tree */
    if (file_entry->an_tree[AN_DATA_LABEL] != NULL) 
      { /* Traverse tree puling ann_id's to delete from ANnodelist */
          for(aentry = tbbtfirst((TBBT_NODE *)*(file_entry->an_tree[AN_DATA_LABEL])); 
              aentry != NULL;
              aentry = tbbtnext(aentry))
            { /* get annotation entry from node */
                ann_entry = (ANentry *) aentry->data; 

                /* See if annotation exits */
                if ((entry = tbbtdfind(ANnodelist, &(ann_entry->ann_id), NULL)) 
                    == NULL)
                    HE_REPORT_GOTO("Failed to find annotation with ann_id", FAIL);

                /* delete node from ANndoelist */
                if ((ann_node =(ANnode *) tbbtrem((TBBT_NODE **)ANnodelist, entry, 
                                                  &ann_key))== NULL)
                    HGOTO_ERROR(DFE_BADCALL, FAIL);      

                if (ann_node != NULL)
                    HDfree(ann_node);   /* free node */
                if (ann_key != NULL)
                    HDfree(ann_key);    /* free key */

            } /* end for 'entry */
          /* finally free tree */
          tbbtdfree(file_entry->an_tree[AN_DATA_LABEL], ANfreedata, ANfreekey);  
      }

    /* free desc annotation rb tree */
    if (file_entry->an_tree[AN_DATA_DESC] != NULL) 
      { /* Traverse tree puling ann_id's to delete from ANnodelist */
          for(aentry = tbbtfirst((TBBT_NODE *)*(file_entry->an_tree[AN_DATA_DESC])); 
              aentry != NULL;
              aentry = tbbtnext(aentry))
            { /* get annotation entry from node */
                ann_entry = (ANentry *) aentry->data; 

                /* See if annotation exits */
                if ((entry = tbbtdfind(ANnodelist, &(ann_entry->ann_id), NULL)) 
                    == NULL)
                    HE_REPORT_GOTO("Failed to find annotation with ann_id", FAIL);

                /* delete node from ANndoelist */
                if ((ann_node =(ANnode *) tbbtrem((TBBT_NODE **)ANnodelist, entry, 
                                                  &ann_key))== NULL)
                    HGOTO_ERROR(DFE_BADCALL, FAIL);      

                if (ann_node != NULL)
                    HDfree(ann_node); /* free node */
                if (ann_key != NULL)
                    HDfree(ann_key);       /* free key */

            } /* end for 'entry */
          /* finally free tree */
          tbbtdfree(file_entry->an_tree[AN_DATA_DESC], ANfreedata, ANfreekey);  
      }

    /* Now we can delete the node itself */
    if ((file_entry =(ANfile *) tbbtrem((TBBT_NODE **)ANfilelist, fentry, &kp))
        == NULL)
        HGOTO_ERROR(DFE_BADCALL, FAIL);      

    /* free file data & key */
    if (file_entry != NULL)
        HDfree(file_entry);
    if(kp != NULL)
        HDfree(kp);

  done:
    if(ret_value == FAIL)   
      { /* Error condition cleanup */

      } /* end if */

    /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(AN_mask, ID_ANend);
#endif /* HAVE_PABLO */

    return ret_value;
} /* ANend() */

/* ------------------------------------------------------------------------ 
 NAME
	ANcreate - create a new element annotation and return a handle(id)

 DESCRIPTION
    Creates a data annotation, returns an 'an_id' to work with the new 
    annotation which can either be a label or description.

 RETURNS
    An ID to an annotation which can either be a label or description

 AUTHOR
    GeorgeV.

--------------------------------------------------------------------------- */
EXPORT int32
ANcreate(int32 an_id,     /* IN: annotation interface ID */
         uint16 elem_tag, /* IN: tag of item to be assigned annotation */ 
         uint16 elem_ref, /* IN: reference number of itme to be assigned ann */ 
         ann_type type    /* IN: AN_DATA_LABEL for data labels, 
                             AN_DATA_DESC for data descriptions*/)
{
#ifdef LATER
    CONSTR(FUNC, "ANcreate");    /* for HERROR */
#endif /* LATER */
    int32    ret_value;

#ifdef HAVE_PABLO
    TRACE_ON(AN_mask, ID_ANcreate);
#endif /* HAVE_PABLO */

    ret_value = (ANIcreate(an_id, elem_tag, elem_ref, type));

#ifdef HAVE_PABLO
    TRACE_OFF(AN_mask, ID_ANcreate);
#endif /* HAVE_PABLO */

    return ret_value;
} /* ANcreate() */

/* ------------------------------------------------------------------------ 
 NAME
	ANcreatef - create a new file annotation and return a handle(id)

 DESCRIPTION
    Creates a file annotation, returns an 'an_id' to work with the new 
    file annotation which can either be a label or description.

 RETURNS
    An ID to an annotation which can either be a file label or description        

 AUTHOR
    GeorgeV.

--------------------------------------------------------------------------- */
EXPORT int32
ANcreatef(int32 an_id,  /* IN: annotation interface ID */
          ann_type type /* IN:  AN_FILE_LABEL for file labels,
                           AN_FILE_DESC for file descritpions.*/)
{
#ifdef LATER
    CONSTR(FUNC, "ANcreatef");    /* for HERROR */
#endif /* LATER */
    uint16 ann_tag;
    uint16 ann_ref;
    int32  ret_value = SUCCEED;

#ifdef HAVE_PABLO
    TRACE_ON(AN_mask, ID_ANcreatef);
#endif /* HAVE_PABLO */

    /* deal with type */
    switch((ann_type)type)
      {
      case AN_FILE_LABEL:
          ann_tag = DFTAG_FID;
          ann_ref = 0; /* fake ref, will be replaced in ANIcreate() */
          break;
      case AN_FILE_DESC:
          ann_tag = DFTAG_FD;
          ann_ref = 0; /* fake ref, will be replaced in ANIcreate() */
          break;
      default:
          HE_REPORT_GOTO("Bad annotation type for this call",FAIL);
      }

    ret_value = (ANIcreate(an_id, ann_tag, ann_ref, type));

  done:
    if(ret_value == FAIL)   
      { /* Error condition cleanup */

      } /* end if */

    /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(AN_mask, ID_ANcreatef);
#endif /* HAVE_PABLO */

    return ret_value;
} /* ANcreateann() */

/* ------------------------------- ANselect ------------------------------- 
 NAME
	ANselect -- get an annotation ID from index of 'type'

 DESCRIPTION
    The position index is ZERO based

 RETURNS
    An ID to an annotation type which can either be a label or description 

 AUTHOR
    GeorgeV.

--------------------------------------------------------------------------- */
EXPORT int32
ANselect(int32 an_id,  /* IN: annotation interface ID */
         int32 index,  /* IN: index of annottion to get ID for */
         ann_type type /* IN: AN_DATA_LABEL for data labels, 
                          AN_DATA_DESC for data descriptions,
                          AN_FILE_LABEL for file labels,
                          AN_FILE_DESC for file descritpions.*/)
{
    CONSTR(FUNC, "ANselect");    /* for HERROR */
    TBBT_NODE *fentry = NULL;
    TBBT_NODE *entry  = NULL;
    ANfile  *file_entry = NULL;
    ANentry *ann_entry  = NULL;
    int32   ret_value = SUCCEED;

#ifdef HAVE_PABLO
    TRACE_ON(AN_mask, ID_ANselect);
#endif /* HAVE_PABLO */

    /* Clear error stack */
    HEclear();

    /* Valid file id */
    if (HAatom_group(an_id)!=FIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* First find file node from file tree */
    if ((fentry = tbbtdfind(ANfilelist, &an_id, NULL)) == NULL)
        HE_REPORT_GOTO("failed to find an_id", FAIL);

    file_entry = (ANfile *) fentry->data; /* get file entry from node */

    /* Empty annotation tree */
    if (file_entry->an_num[type] == -1)
      {
          if (ANIcreate_ann_tree(an_id, type) == FAIL)
              HGOTO_ERROR(DFE_BADCALL, FAIL);
      }

    /* check index and adjust to 1 based for tbbtindx()*/
    if(index >= 0 && index <= file_entry->an_num[type])
        index++;
    else
        HE_REPORT_GOTO("bad index", FAIL);

    /* find 'index' entry */
    if ((entry = tbbtindx((TBBT_NODE *)*(file_entry->an_tree[type]), index)) == NULL)
        HE_REPORT_GOTO("failed to find 'index' entry", FAIL);

    ann_entry = (ANentry *) entry->data; 

    /* return ann_id */
    ret_value = ann_entry->ann_id;

  done:
    if(ret_value == FAIL)   
      { /* Error condition cleanup */

      } /* end if */

    /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(AN_mask, ID_ANselect);
#endif /* HAVE_PABLO */

    return ret_value;
} /* ANselect() */

/*--------------------------------------------------------------------------
 NAME
   ANnumann -- find number of annotation of 'type' that 
               match the given element tag/ref 

 DESCRIPTION
   Find number of annotation of 'type' for the given element 
   tag/ref pair.

 RETURNS
   Number of annotation found if successful and FAIL (-1) otherwise

 AUTHOR
    GeorgeV.

 ------------------------------------------------------------------------*/ 
EXPORT intn
ANnumann(int32 an_id,     /* IN: annotation interface id */
         ann_type type,   /* IN: AN_DATA_LABEL for data labels, 
                             AN_DATA_DESC for data descriptions,
                             AN_FILE_LABEL for file labels,
                             AN_FILE_DESC for file descritpions.*/
         uint16 elem_tag, /* IN: tag of item of which this is annotation */
         uint16 elem_ref  /* IN: ref of item of which this is annotation */ )
{
#ifdef LATER
    CONSTR(FUNC, "ANnumann");
#endif /* LATER */
    intn   ret_value;

#ifdef HAVE_PABLO
    TRACE_ON(AN_mask, ID_ANnumann);
#endif /* HAVE_PABLO */

    ret_value = ANInumann(an_id, type, elem_tag, elem_ref);

#ifdef HAVE_PABLO
    TRACE_OFF(AN_mask, ID_ANnumann);
#endif /* HAVE_PABLO */

    return ret_value;
} /* ANnumann() */

/*--------------------------------------------------------------------------
 NAME
   ANannlist -- generate list of annotation ids of 'type' that 
                 match the given element tag/ref 

 DESCRIPTION
   Find and generate list of annotation ids of 'type' for the given 
   element tag/ref pair

 RETURNS
   Number of annotations ids found if successful and FAIL (-1) otherwise

 AUTHOR
    GeorgeV.

 ------------------------------------------------------------------------*/ 
EXPORT intn
ANannlist(int32 an_id,       /* IN: annotation interface id */
          ann_type type,     /* IN: AN_DATA_LABEL for data labels, 
                                AN_DATA_DESC for data descriptions,
                                AN_FILE_LABEL for file labels,
                                AN_FILE_DESC for file descritpions.*/
          uint16 elem_tag,   /* IN: tag of item of which this is annotation */
          uint16 elem_ref,   /* IN: ref of item of which this is annotation */
          int32 ann_list[]   /* OUT: array of ann_id's that match criteria. */)
{
#ifdef LATER
    CONSTR(FUNC, "ANannlist");
#endif /* LATER */
    intn  ret_value;

#ifdef HAVE_PABLO
    TRACE_ON(AN_mask, ID_ANannlist);
#endif /* HAVE_PABLO */

    ret_value = ANIannlist(an_id, type, elem_tag, elem_ref, ann_list);

#ifdef HAVE_PABLO
    TRACE_OFF(AN_mask, ID_ANannlist);
#endif /* HAVE_PABLO */

    return ret_value;
} /* ANannlist() */

/*--------------------------------------------------------------------------
 NAME
       ANannlen -- get length of annotation givne annotation id

 DESCRIPTION
       Uses the annotation id to find ann_key & file_id

 RETURNS
       length of annotation if successful and FAIL (-1) otherwise

 AUTHOR
    GeorgeV.

 ------------------------------------------------------------------------*/
EXPORT int32
ANannlen(int32 ann_id /* IN: annotation id */)
{
#ifdef LATER
    CONSTR(FUNC, "ANannlen");
#endif /* LATER */
    int32  ret_value;

#ifdef HAVE_PABLO
    TRACE_ON(AN_mask, ID_ANannlen);
#endif /* HAVE_PABLO */

    ret_value =  ANIannlen(ann_id);
#ifdef HAVE_PABLO
    TRACE_OFF(AN_mask, ID_ANannlen);
#endif /* HAVE_PABLO */

    return ret_value;
} /* ANannlen() */

/*--------------------------------------------------------------------------
 NAME
       ANwriteann -- write annotation given ann_id

 DESCRIPTION
       Checks for pre-existence of given annotation, replacing old one if it
       exists. Writes out annotation.

 RETURNS
       SUCCEED (0) if successful and FAIL (-1) otherwise

 AUTHOR
    GeorgeV.

 ------------------------------------------------------------------------*/
EXPORT int32
ANwriteann(int32 ann_id,     /* IN: annotation id */
           const char *ann,  /* IN: annotation to write */
           int32 annlen      /* IN: length of annotation */)
{
#ifdef LATER
    CONSTR(FUNC, "ANwriteann");    /* for HERROR */
#endif /* LATER */
    int32  ret_value;

#ifdef HAVE_PABLO
    TRACE_ON(AN_mask, ID_ANwriteann);
#endif /* HAVE_PABLO */

    ret_value = ANIwriteann(ann_id, ann, annlen);
#ifdef HAVE_PABLO
    TRACE_OFF(AN_mask, ID_ANwriteann);
#endif /* HAVE_PABLO */

    return ret_value;
} /* ANwriteann() */

/*--------------------------------------------------------------------------
 NAME
       ANreadann -- read annotation given ann_id

 DESCRIPTION
       Gets tag and ref of annotation.  Finds DD for that annotation.
       Reads the annotation, taking care of NULL terminator, if necessary.

 RETURNS
       SUCCEED (0) if successful and FAIL (-1) otherwise

 AUTHOR
    GeorgeV.

 ------------------------------------------------------------------------*/
EXPORT int32
ANreadann(int32 ann_id,  /* IN: annotation id (handle) */
          char *ann,     /* OUT: space to return annotation in */
          int32 maxlen   /* IN: size of space to return annotation in */)
{
#ifdef LATER
    CONSTR(FUNC, "ANreadann");    /* for HERROR */
#endif /* LATER */
    int32   ret_value;

#ifdef HAVE_PABLO
    TRACE_ON(AN_mask, ID_ANreadann);
#endif /* HAVE_PABLO */

    ret_value = ANIreadann(ann_id, ann, maxlen);
#ifdef HAVE_PABLO
    TRACE_OFF(AN_mask, ID_ANreadann);
#endif /* HAVE_PABLO */

    return ret_value;
} /* ANreadann() */

/* ----------------------------------------------------------------------- 
 NAME
	ANendaccess -- end access to an annotation given it's id

 DESCRIPTION
        Terminates access to an annotation. For now does nothing

 RETURNS
        SUCCEED(0) or FAIL(-1)

 AUTHOR
    GeorgeV.

--------------------------------------------------------------------------- */
EXPORT intn
ANendaccess(int32 ann_id /* IN: annotation id */)
{
#ifdef LATER
    CONSTR(FUNC, "ANendaccess");    /* for HERROR */
#endif /* LATER */
    intn  ret_value = SUCCEED;
#ifdef HAVE_PABLO
    TRACE_ON(AN_mask, ID_ANendaccess);
#endif /* HAVE_PABLO */
#ifdef HAVE_PABLO
    TRACE_OFF(AN_mask, ID_ANendaccess);
#endif /* HAVE_PABLO */

    /* shut compiler up */
    ann_id=ann_id;

    return ret_value;
} /* ANendaccess() */

/* -------------------------------------------------------------- 
 NAME
	ANget_tagref - get tag/ref pair to annotation ID

 DESCRIPTION
    The position index is zero based

 RETURNS
    A tag/ref pairt to an annotation type which can either be a 
    label or description given the annotation ID

 AUTHOR
    GeorgeV.

--------------------------------------------------------------------------- */
EXPORT int32
ANget_tagref(int32 an_id,   /* IN: annotation interface ID */
             int32 index,   /* IN: index of annottion to get tag/ref for */
             ann_type type, /* IN: AN_DATA_LABEL for data labels, 
                               AN_DATA_DESC for data descriptions,
                               AN_FILE_LABEL for file labels,
                               AN_FILE_DESC for file descritpions.*/
             uint16 *tag,   /* OUT: Tag for annotation */
             uint16 *ref    /* OUT: ref for annotation */)
{
    CONSTR(FUNC, "ANget_tagref");    /* for HERROR */
    TBBT_NODE *fentry = NULL;
    TBBT_NODE *entry  = NULL;
    ANfile  *file_entry = NULL;
    ANentry *ann_entry  = NULL;
    int32   ret_value = SUCCEED;

    /* Clear error stack */
    HEclear();

    /* Valid file id */
    if (HAatom_group(an_id)!=FIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* First find file node from file tree */
    if ((fentry = tbbtdfind(ANfilelist, &an_id, NULL)) == NULL)
        HE_REPORT_GOTO("failed to find an_id", FAIL);

    file_entry = (ANfile *) fentry->data; /* get file entry from node */

    /* Empty annotation tree */
    if (file_entry->an_num[type] == -1)
      {
          if (ANIcreate_ann_tree(an_id, type) == FAIL)
              HGOTO_ERROR(DFE_BADCALL, FAIL);
      }

    /* check index and adjust to 1 based for tbbtindx()*/
    if(index >= 0 && index <= file_entry->an_num[type])
        index++;
    else
        HE_REPORT_GOTO("bad index", FAIL);

    /* find 'index' entry */
    if ((entry = tbbtindx((TBBT_NODE *)*(file_entry->an_tree[type]), index)) == NULL)
        HE_REPORT_GOTO("failed to find 'index' entry", FAIL);

    ann_entry = (ANentry *) entry->data; 

    *ref = ann_entry->annref;
    switch((int32)type)
      {
      case AN_DATA_LABEL:
          *tag = DFTAG_DIL;
          break;
      case AN_DATA_DESC:
          *tag = DFTAG_DIA;
          break;
      case AN_FILE_LABEL:
          *tag = DFTAG_FID;
          break;
      case AN_FILE_DESC:
          *tag = DFTAG_FD;
          break;
      default:
          HE_REPORT_GOTO("Bad annotation type for this call",FAIL);
      }

  done:
    if(ret_value == FAIL)   
      { /* Error condition cleanup */

      } /* end if */

    /* Normal function cleanup */

    return ret_value;
} /* ANget_tagref() */

/*--------------------------------------------------------------------------
 NAME
    ANid2tagref -- get tag/ref given annotation id

 DESCRIPTION
    Uses the annotation id to find ann_node entry which contains ann_ref

 RETURNS
    SUCCEED(0) if successful and FAIL (-1) otherwise.  

 AUTHOR
    GeorgeV.

 ------------------------------------------------------------------------*/
int32
ANid2tagref(int32 ann_id,  /* IN: annotation id */
            uint16 *tag,   /* OUT: Tag for annotation */
            uint16 *ref    /* OUT: ref for annotation */)
{
#ifdef LATER
    CONSTR(FUNC, "ANid2tagref");
#endif /* LATER */
    TBBT_NODE *entry = NULL;
    ANnode  *ann_node   = NULL;
    int32  file_id;
    int32  type;
    int32  ann_key;
    uint16 ann_ref;
    int32  ret_value = SUCCEED;

    /* Clear error stack */
    HEclear();

    /* Valid annotation id */
    /* First find file node from file tree */
    if ((entry = tbbtdfind(ANnodelist, &ann_id, NULL)) == NULL)
        HE_REPORT_GOTO("failed to find ann_id", FAIL);

    ann_node = (ANnode *) entry->data; /* get ann node from node */

    /* get file id and annotation key */
    file_id = ann_node->file_id;
    ann_key = ann_node->ann_key;
    type    = AN_KEY2TYPE(ann_key);
    ann_ref = AN_KEY2REF(ann_key);

    /* Valid file id */
    if (file_id == FAIL)
        HE_REPORT_GOTO("bad file_id", FAIL);

    *ref = ann_ref;
    /* set type annotation tag */
    switch((int32)type)
      {
      case AN_DATA_LABEL:
          *tag = DFTAG_DIL;
          break;
      case AN_DATA_DESC:
          *tag = DFTAG_DIA;
          break;
      case AN_FILE_LABEL:
          *tag = DFTAG_FID;
          break;
      case AN_FILE_DESC:
          *tag = DFTAG_FD;
          break;
      default:
          HE_REPORT_GOTO("Bad annotation type for this call",FAIL);
      }

  done:
    if(ret_value == FAIL)   
      { /* Error condition cleanup */

      } /* end if */

    /* Normal function cleanup */

    return ret_value;
} /* ANid2tagref */

/*--------------------------------------------------------------------------
 NAME
       ANtagref2id -- get annotation id given tag/ref

 DESCRIPTION
       Gets the annotation id of the annotation given the tag/ref of
       the annotation itself and the annotation interface id.

 RETURNS
       Annotation id of annotation if successful and FAIL(-1) otherwise. 

 AUTHOR
    GeorgeV.

 ------------------------------------------------------------------------*/
int32
ANtagref2id(int32 an_id,    /* IN  Annotation interface id */
            uint16 ann_tag, /* IN: Tag for annotation */
            uint16 ann_ref  /* IN: ref for annotation */)
{
    CONSTR(FUNC, "ANtagref2id");
    TBBT_NODE *entry = NULL;
    ANfile   *file_entry = NULL;
    ANentry  *ann_entry  = NULL;
    int32    ann_key;
    ann_type type;
    int32    ret_value = SUCCEED;

    /* Clear error stack */
    HEclear();

    /* Valid file id */
    if (HAatom_group(an_id)!=FIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* First find file node from file tree */
    if ((entry = tbbtdfind(ANfilelist, &an_id, NULL)) == NULL)
        HE_REPORT_GOTO("failed to find an_id", FAIL);

    file_entry = (ANfile *) entry->data; /* get file entry from node */

    /* set type given annotation tag */
    switch((uint16)ann_tag)
      {
      case DFTAG_DIL:
          type = AN_DATA_LABEL;
          break;
      case DFTAG_DIA:
          type = AN_DATA_DESC;
          break;
      case  DFTAG_FID:
          type = AN_FILE_LABEL;
          break;
      case  DFTAG_FD:
          type = AN_FILE_DESC;
          break;
      default:
          HE_REPORT_GOTO("Bad annotation type for this call",FAIL);
      }

    /* Check for empty annotation tree of 'type'? */   
    if (file_entry->an_num[type] == -1)
      {
          if (ANIcreate_ann_tree(an_id, type) == FAIL)
              HGOTO_ERROR(DFE_BADCALL, FAIL);
      }

    /* Create key from type/ref pair 
     *  ----------------------------
     *  | type(16bits) | ref(16bits) |
     *  -----------------------------*/
    ann_key = AN_CREATE_KEY(type, ann_ref);

    /* See if annotation of 'type' with ref exits */
    if ((entry = tbbtdfind(file_entry->an_tree[type], &ann_key, NULL)) == NULL)
        HE_REPORT_GOTO("failed to find annotation of 'type'", FAIL);

    /* get annotation entry from node */
    ann_entry = (ANentry *) entry->data; 

    /* return annotation id */
    ret_value = ann_entry->ann_id;

  done:
    if(ret_value == FAIL)   
      { /* Error condition cleanup */

      } /* end if */

    /* Normal function cleanup */

    return ret_value;
} /* ANtagref2id */

/*-------------------------------------------------------------------- 
 NAME
     ANatype2tag - annotation type to corresponding annotation TAG

 DESCRIPTION
     Translate annotation type to corresponding TAG

 RETURNS
     Returns TAG corresponding to annotatin type

 AUTHOR
    GeorgeV.

--------------------------------------------------------------------*/
EXPORT uint16
ANatype2tag(ann_type atype /* IN: Annotation type */)
{   /* Switch on annotation type "atype" */
#ifdef LATER
    CONSTR(FUNC, "ANatype2tag");    /* for HERROR */
#endif /* LATER */
    uint16 ann_tag;

    switch((ann_type)atype) 
      {
      case AN_FILE_LABEL: ann_tag = DFTAG_FID; break;
      case AN_FILE_DESC:  ann_tag = DFTAG_FD;  break;
      case AN_DATA_LABEL: ann_tag = DFTAG_DIL; break;
      case AN_DATA_DESC:  ann_tag = DFTAG_DIA; break;
      default: ann_tag = 5;
      } /* switch */
    return ann_tag;
} /* ANatype2tag */

/*-------------------------------------------------------------------- 
 NAME
     ANtag2atype - annotation TAG to corresponding annotation type

 DESCRIPTION
     Translate annotation TAG to corresponding atype

 RETURNS
     Returns type corresponding to annotatin TAG

 AUTHOR
    GeorgeV.

--------------------------------------------------------------------*/
EXPORT ann_type
ANtag2atype(uint16 atag /* IN: annotation tag */)
{   /* Switch on annotation tag */
#ifdef LATER
    CONSTR(FUNC, "ANtag2atype");    /* for HERROR */
#endif /* LATER */
    ann_type atype;

    switch((uint16)atag) 
      {
      case DFTAG_FID: atype = AN_FILE_LABEL; break;
      case DFTAG_FD:  atype = AN_FILE_DESC;  break;
      case DFTAG_DIL: atype = AN_DATA_LABEL; break;
      case DFTAG_DIA: atype = AN_DATA_DESC;  break;
          /* This will cause warnings on some compiliers */
      default: atype = AN_UNDEF;
      } /* switch */
    return atype;
} /* ANtag2atype */

#endif /* MFAN_C */

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
 *           implementation uses TBBT trees and there exists code for 
 *           Red-Black tree implmentation.
 *
 *           NOTES: Note that any mention of file ID's except in ANStart() 
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
 *  ANIfidcmp     - compare two annotation interface id's (used in file RBtree)
 *  ANIanncmp     - compare two annotation handles(ann_id's) 
 *                  (used in annotation RBtree)
 *  ANIaddentry:  - add entry to corresponding annotation RBtree
 *  ANIcreate_ann_tree - create annotation RBtree 
 *  ANIfind:      - return annotation handle(ann_id) if found of given TYPE/ref
 *  ANInumann:    - return number of annotations that match TYPE/tag/ref
 *  ANIannlist:   - return list of handles(ann_id's) that match TYPE/tag/ref
 *  ANIannlen:    - get length of annotation given handle(ann_id)
 *  ANIreadann:   - read annotation given handle(ann_id)
 *  ANIwriteann:  - write annotation given handle(ann_id)
 *  ANIcreate     - create a new file/label annotation 
 *                  and return a handle(ann_id)
 *
 *  High-Level routines:
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
 *  Miscellaneous Routines
 *  ----------------------
 *  ANget_tagref - get tag/ref pair to annotation type and index
 *  ANid2tagref  - get tag/ref pair to annotation ID
 *  atype2tag - annotation type to corresponding annotation TAG
 *  tag2atype - annotation TAG to corresponding annotation type
 *
 *---------------------------------------------------------------------------*/

#ifndef MFAN_C  /* define main annoation source file */
#define MFAN_C

#include "mfan.h"

/* Function Prototypes */
extern VOID ANfreedata(VOIDP data);
extern VOID ANfreekey(VOIDP key);
extern VOID dumpnodeKey(VOID *key, VOID *data);
extern VOID dumpentryKey(VOID *key, VOID *data);
extern VOID dumpfileKey(VOID *key, VOID *data);
extern intn ANIfidcmp(VOIDP i, VOIDP j, intn value);
extern intn ANIanncmp(VOIDP i, VOIDP j, intn value);

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
 USAGE
	int32 ANIfidcmp(i, j)
        int32 *i;        IN: annotation interfaced id
        int32 *j;        IN: annotation interfaced id
 RETURNS
        Returns 0 if i=j, -1 if i > j and 1 if i < j
 DESCRIPTION
        Compares two annotation interface id's. Used by trees for file id's. 

 NOTE:- these file ID's are now refered to as annotation interface ID's

--------------------------------------------------------------------------- */
#ifdef HAVE_RBTREE
int32 
ANIfidcmp(int32 *i, int32 *j)
{
  if (*i == *j) 
    return 0;
  if (*i > *j) 
    return -1; 
  else 
    return 1;
} /* ANIfidcmp */
#else /* use tbbt */
intn
ANIfidcmp(VOIDP i, VOIDP j, intn value)
{
  if (*(int32 *)i == *(int32 *)j) 
    return 0;
  if (*(int32 *)i > *(int32 *)j) 
    return -1; 
  else 
    return 1;
} /* ANIfidcmp */
#endif /* use tbbt */

/* ------------------------------- ANIanncmp -------------------------------- 
 NAME
	ANIanncmp -- compare two annotation keys or id's
 USAGE
	int32 ANIanncmp(i, j)
        int32 *i;        IN: annotation key(tag,reg)
        int32 *j;        IN: annotation key(tag,reg)
 RETURNS
        Returns 0 if i=j, -1 if i > j and 1 if i < j
 DESCRIPTION
        Compares two annotation keys. Used by tree for annotations
        Also used to compare annotation id's since also 32 bit value.
--------------------------------------------------------------------------- */
#ifdef HAVE_RBTREE
int32 
ANIanncmp(int32 *i, int32 *j)
{
  if (*i == *j) 
    return 0;
  if (*i > *j) 
    return -1; 
  else 
    return 1;
} /* ANIanncmp */
#else /* use tbbt */
intn 
ANIanncmp(VOIDP i, VOIDP j, intn value)
{
  if (*(int32 *)i == *(int32 *)j) 
    return 0;
  if (*(int32 *)i > *(int32 *)j) 
    return -1; 
  else 
    return 1;
} /* ANIanncmp */
#endif /* use tbbt */

/* ------------------------------- ANinit -------------------------------- 
 NAME
	ANinit -- Initialize Annotation Interface
 USAGE
	int32 ANinit(void)

 RETURNS
        SUCCEED or FAIL
 DESCRIPTION
        Initializes the annotation interface i.e. creates trees
        to hold all open files using annotation interface.
--------------------------------------------------------------------------- */
PRIVATE int32
ANinit(void)
{
#ifdef LATER
  CONSTR(FUNC, "ANinit");
#endif /* LATER */
    
  /* Clear error stack */
  HEclear();

#ifdef HAVE_RBTREE
  /* Check to see if we need to create RB tree for file id's */
  if (ANfilelist == NULL)
    if ((ANfilelist = make_rb()) == NULL)
      HE_REPORT_RETURN("failed to create RBtree for file_ids", FAIL);

  /* Check to see if we need to create RB tree for annotations id's */
  if (ANnodelist == NULL)
    if ((ANnodelist = make_rb()) == NULL)
      HE_REPORT_RETURN("failed to create RBtree for ann_ids", FAIL);

#else /* use tbbt */

  /* Check to see if we need to create RB tree for file id's */
  if (ANfilelist == NULL)
    if ((ANfilelist = tbbtdmake(ANIfidcmp, sizeof(int32))) == NULL)
      HE_REPORT_RETURN("failed to create TBBT tree for file_ids", FAIL);

  /* Check to see if we need to create RB tree for annotations id's */
  if (ANnodelist == NULL)
    if ((ANnodelist = tbbtdmake(ANIanncmp, sizeof(int32))) == NULL)
      HE_REPORT_RETURN("failed to create TBBT tree for ann_ids", FAIL);
#endif /* use tbbt */

  return SUCCEED;
} /* ANinit() */

/* ------------------------------- ANdestroy -------------------------------- 
 NAME
	ANdestroy -- Un-Initialize Annotation Interface
 USAGE
	int32 ANdestroy(void)

 RETURNS
        SUCCEED or FAIL
 DESCRIPTION
        Unallocates global annotaton node list and file list.
--------------------------------------------------------------------------- */
int32
ANdestroy(void)
{
#ifdef LATER
  CONSTR(FUNC, "ANdestroy");
#endif /* LATER */

  /* Clear error stack */
  HEclear();

#ifdef HAVE_RBTREE
  /* Check to see if we need to destory RB tree for file id's */
  if (ANfilelist != NULL)
    rb_free_tree(ANfilelist);

  /* Check to see if we need to destroy RB tree for annotations id's */
  if (ANnodelist == NULL)
    rb_free_tree(ANnodelist);

  /* reset pointers */
  ANfilelist = NULL;
  ANnodelist = NULL;  
#else /* use tbbt */

  /* Check to see if we need to destroy RB tree for file id's */
  if (ANfilelist != NULL)
    tbbtdfree(ANfilelist, ANfreedata, ANfreekey);

  /* Check to see if we need to destroy RB tree for annotations id's */
  if (ANnodelist != NULL)
    tbbtdfree(ANnodelist, ANfreedata, ANfreekey);

  /* reset pointers */
  ANfilelist = NULL;
  ANnodelist = NULL;
#endif /* use tbbt */
 
  return SUCCEED;
} /* ANdestroy () */

/*--------------------------------------------------------------------------
 NAME
       ANIaddentry -- add entry to corresponding annotation RB tree
 USAGE
       int16  ANIaddentry(an_id, type, ann_ref, elmtag, elmref, new_ann)
       int32  an_id;  IN: annotation interface id
       ann_type  type:  IN: AN_DATA_LABEL for data labels, 
                            AN_DATA_DESC for data descriptions,
                            AN_FILE_LABEL for file labels,
                            AN_FILE_DESC for file descritpions.
       uint16 ann_ref:  IN: ref of annotation
       uint16 elmtag,:  IN: tag of item of which this is annotation
       uint16 elmref;   IN: ref of item of which this is annotation
       intn   new_ann;  IN: new annotation flag

 RETURNS
       annotation ID if successful and FAIL (-1) otherwise
 DESCRIPTION
       Adds annotation to correct tree
 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
 *------------------------------------------------------------------------*/
PRIVATE int32
ANIaddentry(int32 an_id, ann_type type, uint16 ann_ref, 
            uint16 elmtag, uint16 elmref, intn new_ann)
{
    CONSTR(FUNC, "ANIaddentry");
#ifdef HAVE_RBTREE
    Rb_node entry;
#else
    TBBT_NODE *entry = NULL;
#endif
    uint16  ann_tag;
    int32   *ann_id  = NULL;
    int32   *ann_key = NULL;
    ANfile  *file_entry = NULL;
    ANentry *ann_entry  = NULL;
    ANnode  *ann_node   = NULL;
 
    /* Clear error stack */
    HEclear();

    /* Valid file id */
    if (an_id == FAIL)
        HRETURN_ERROR(DFE_BADCALL, FAIL);

#ifdef HAVE_RBTREE
    /* First find file node from file RB tree */
    if ((entry = rb_find_gkey(ANfilelist, &an_id, ANIfidcmp)) == NULL)
        HRETURN_ERROR(DFE_BADCALL, FAIL);

    if (entry == ANfilelist)
      HE_REPORT_RETURN("failed to find an_id", FAIL);
    else
      file_entry = (ANfile *) rb_val(entry); /* get file entry from node */

    /* Check for empty annotation tree of 'type'? */   
    if (file_entry->an_num[type] == -1)
      {
        if ((file_entry->an_tree[type] = (Rb_node) make_rb()) == NULL)
          HE_REPORT_RETURN("failed to create annotation RBtree", FAIL);
        
        file_entry->an_num[type] = 0;
      }

#else /* use tbbt */
    /* First find file node from file tree */
    if ((entry = tbbtdfind(ANfilelist, &an_id, NULL)) == NULL)
      HE_REPORT_RETURN("failed to find an_id", FAIL);

    file_entry = (ANfile *) entry->data; /* get file entry from node */

    /* Check for empty annotation tree of 'type'? */   
    if (file_entry->an_num[type] == -1)
      {
        if ((file_entry->an_tree[type] = 
             (TBBT_TREE *) tbbtdmake(ANIanncmp,sizeof(int32))) == NULL)
          HE_REPORT_RETURN("failed to create annotation tree", FAIL);

        file_entry->an_num[type] = 0;
      }
#endif /* use tbbt */

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
        HE_REPORT_RETURN("Bad annotation type for this call",FAIL);
      }

    /* allocate space for key */
    if ((ann_key = (int32 *)HDmalloc(sizeof(int32))) == NULL)
        HRETURN_ERROR(DFE_NOSPACE, FAIL);

    /* allocate space for ann_id */
    if ((ann_id = (int32 *)HDmalloc(sizeof(int32))) == NULL)
        HRETURN_ERROR(DFE_NOSPACE, FAIL);

    /* Create 32bit key from type/ref 
    *  ----------------------------
    *  | type(16bits)| ref(16bits) |
    *  -----------------------------*/
    *ann_key = AN_CREATE_KEY(type, ann_ref);

    /* Initialize annotation entry */
    if ((ann_entry = HDmalloc(sizeof(ANentry))) == NULL)
        HRETURN_ERROR(DFE_NOSPACE, FAIL);

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

#ifdef HAVE_RBTREE
    /* Add annotation entry to 'type' RB tree */
    if (rb_insertg(file_entry->an_tree[type], ann_key, ann_entry, ANIanncmp) == NULL)
      HE_REPORT_RETURN("failed to insert annotation into 'type' RBtree", FAIL);
#else  /* use tbbt */
    /* Add annotation entry to 'type' tree */
    if (tbbtdins(file_entry->an_tree[type], ann_entry, ann_key) == NULL)
      HE_REPORT_RETURN("failed to insert annotation into 'type' tree", FAIL);
#endif /* use tbbt */

    /* increment number of annotatiosn of 'type' */
    file_entry->an_num[type] += 1;

    /* Initialize annotation node */
    if ((ann_node = HDmalloc(sizeof(ANnode))) == NULL)
        HRETURN_ERROR(DFE_NOSPACE, FAIL);

    ann_node->file_id = an_id;
    ann_node->ann_key = *ann_key;
    ann_node->new_ann = new_ann;
#ifdef HAVE_RBTREE
    /* Add annotation node to ann_id RB tree */
    if (rb_insertg(ANnodelist, ann_id, ann_node, ANIanncmp) == NULL)
      HE_REPORT_RETURN("failed to insert annotation into ann_id RBtree", FAIL);
#else /* use tbbt */
    /* Add annotation node to ann_id RB tree */
    if (tbbtdins(ANnodelist, ann_node, ann_id) == NULL)
      HE_REPORT_RETURN("failed to insert annotation into ann_id tree", FAIL);
#endif /* use tbbt */

    /* return annotation id */
    return ann_entry->ann_id;
} /* ANIaddentry */

/*--------------------------------------------------------------------------
 NAME
      ANIcreate_ann_tree --  create an annotation tree of 'type' for given file 
 USAGE
       intn   ANIcreate_ann_tree(an_id, type)
       int32  an_id;   IN: annotation interface id
       int    type:      IN: AN_DATA_LABEL for data labels, 
                             AN_DATA_DESC for data descriptions,
                             AN_FILE_LABEL for file labels,
                             AN_FILE_DESC for file descritpions.
 RETURNS
       Number of annotations of 'type' in file if successful and 
       FAIL (-1) otherwise
 DESCRIPTION
       Creates either a label or descritption annotation RBtree. 
 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
 *------------------------------------------------------------------------*/
PRIVATE intn
ANIcreate_ann_tree(int32 an_id, ann_type type)
{
    CONSTR(FUNC, "ANIcreate_ann_tree");
#ifdef HAVE_RBTREE
    Rb_node entry;
#else 
    TBBT_NODE *entry = NULL;
#endif
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

    /* Clear error stack */
    HEclear();

    /* Valid file id */
    if (an_id == FAIL)
        HRETURN_ERROR(DFE_BADCALL, FAIL);

#ifdef HAVE_RBTREE
    /* First find file node to from file RB tree */
    if ((entry = rb_find_gkey(ANfilelist, &an_id, ANIfidcmp)) == NULL)
        HRETURN_ERROR(DFE_BADCALL, FAIL);

    if (entry == ANfilelist)
      HE_REPORT_RETURN("failed to find an_id", FAIL);
    else
      file_entry = (ANfile *) rb_val(entry); /* get file entry from node */

    /* Check for empty annotation tree? */   
    if (file_entry->an_num[type] == -1)
      {
        if ((file_entry->an_tree[type] = (Rb_node) make_rb()) == NULL)
          HE_REPORT_RETURN("failed to creat RBtree for annotation type", FAIL);
        file_entry->an_num[type] = 0;
      }
    else
      return file_entry->an_num[type];  /* tree already created */

#else /* use tbbt */

    /* First find file node from file tree */
    if ((entry = tbbtdfind(ANfilelist, &an_id, NULL)) == NULL)
      HE_REPORT_RETURN("failed to find an_id", FAIL);

    file_entry = (ANfile *) entry->data; /* get file entry from node */

    /* Check for empty annotation tree of 'type'? */   
    if (file_entry->an_num[type] == -1)
      {
        if ((file_entry->an_tree[type] = 
             (TBBT_TREE *) tbbtdmake(ANIanncmp,sizeof(int32))) == NULL)
          HE_REPORT_RETURN("failed to create annotation tree", FAIL);
        file_entry->an_num[type] = 0;
      }
    else
      return file_entry->an_num[type];  /* tree already created */

#endif /* use tbbt */

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
        HE_REPORT_RETURN("Bad annotation type for this call",FAIL);
      }

    /* Get number of annotations of 'type' in file */
    if ((nanns = Hnumber(an_id, ann_tag)) == 0)
      { /* ZERO annotations of 'type' in file */
        file_entry->an_num[type] = 0;
        return  file_entry->an_num[type];  
      }

    /* start read on 'type' annotations  in file 
    * note that so far an_id == file_id */
    if ((aid = Hstartread(an_id, ann_tag, DFREF_WILDCARD)) == FAIL)
      {
        Hendaccess(aid);
        HE_REPORT_RETURN("Hstartread failed to read annotation", FAIL);
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
            return  file_entry->an_num[type];  
          }

        /* if data annotation, read data annotation tag/ref */
        if(type != AN_FILE_LABEL && type != AN_FILE_DESC)
          {
            if ((int32) FAIL == Hread(aid, (int32) 4, datadi))
              {
                file_entry->an_num[type] = nanns;
                return  file_entry->an_num[type];  
              }
          }

        /* allocate space for key */
        if ((ann_key = (int32 *)HDmalloc(sizeof(int32))) == NULL)
          HRETURN_ERROR(DFE_NOSPACE, FAIL);

        /* allocate space for ann_id */
        if ((ann_id = (int32 *)HDmalloc(sizeof(int32))) == NULL)
          HRETURN_ERROR(DFE_NOSPACE, FAIL);

        /* Allocate & Initialize annotation entry 
        *  decode data tag/ref */
        if ((ann_entry = HDmalloc(sizeof(ANentry))) == NULL)
          HRETURN_ERROR(DFE_NOSPACE, FAIL);

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

#ifdef HAVE_RBTREE
        /* Add annotation entry to 'type' RB tree */
        if (rb_insertg(file_entry->an_tree[type], ann_key, ann_entry, 
                       ANIanncmp) == NULL)
          HE_REPORT_RETURN("failed to insert annotation into 'type' RBtree", 
                           FAIL);
#else  /* use tbbt */
        /* Add annotation entry to 'type' tree */
        if (tbbtdins(file_entry->an_tree[type], ann_entry, ann_key) == NULL)
          HE_REPORT_RETURN("failed to insert annotation into 'type' tree", FAIL);
#endif /* use tbbt */

        /* Initialize annotation node */
        if ((ann_node = HDmalloc(sizeof(ANnode))) == NULL)
          HRETURN_ERROR(DFE_NOSPACE, FAIL);

        ann_node->file_id = an_id;
        ann_node->ann_key = *ann_key;
        ann_node->new_ann = 0;       /* not a newly created annotation */

#ifdef HAVE_RBTREE
        /* Add annotation node to ann_id RB tree */
        if (rb_insertg(ANnodelist, ann_id, ann_node, ANIanncmp) == NULL)
          HE_REPORT_RETURN("failed to insert annotation into ann_id RBtree", 
                           FAIL);
#else /* use tbbt */
        /* Add annotation node to ann_id RB tree */
        if (tbbtdins(ANnodelist, ann_node, ann_id) == NULL)
          HE_REPORT_RETURN("failed to insert annotation into ann_id tree", FAIL);
#endif /* use tbbt */
        
        /* set read on next annotation */
        more_anns = Hnextread(aid, ann_tag, DFREF_WILDCARD, DF_CURRENT);
      } /* end for "more_anns" */
    
    /* Finish access*/
    Hendaccess(aid);

    file_entry->an_num[type] = nanns;
    return  file_entry->an_num[type];  
} /* ANIcreate_ann_tree */

#if NOT_USED_YET
/*--------------------------------------------------------------------------
 NAME
     ANIfind -- find annotation id for given annotation type and ref number
 USAGE
       int16  ANIfind(an_id, type, ann_ref)
       int32  an_id;   IN: annotation interface id
       ann_type  type:   IN: AN_DATA_LABEL for data labels, 
                             AN_DATA_DESC for data descriptions,
                             AN_FILE_LABEL for file labels,
                             AN_FILE_DESC for file descritpions.
       uint16 ann_ref;   IN: ref of annotation
 RETURNS
       annotation id if successful and FAIL (-1) otherwise
 DESCRIPTION
       Find annotation id for the given annotation type and ref number

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
 *------------------------------------------------------------------------*/ 
PRIVATE int32
ANIfind(int32 an_id, ann_type type, uint16 ann_ref)
{
    CONSTR(FUNC, "ANIfind");
#ifdef HAVE_RBTREE
    Rb_node entry;
#else
    TBBT_NODE *entry = NULL;
#endif
    ANfile  *file_entry = NULL;
    ANentry *ann_entry  = NULL;
    int32   ann_key;

    /* Clear error stack */
    HEclear();

    /* Valid file id */
    if (an_id == FAIL)
      HRETURN_ERROR(DFE_BADCALL, FAIL);

#ifdef HAVE_RBTREE
    /* Find node containing key(an_id) */
    if ((entry = rb_find_gkey(ANfilelist, &an_id, ANIfidcmp)) == NULL)
      HRETURN_ERROR(DFE_NOMATCH, FAIL);

    if (entry == ANfilelist)
      HE_REPORT_RETURN("failed to find an_id", FAIL);
    else
      file_entry = (ANfile *) rb_val(entry); /* get file entry from node */

    /* Empty annotation tree */
    if (file_entry->an_num[type] == -1)
      {
        if (ANIcreate_ann_tree(an_id, type) == FAIL)
          HRETURN_ERROR(DFE_BADCALL, FAIL);
      }
#else /* use tbbt */
    /* First find file node from file tree */
    if ((entry = tbbtdfind(ANfilelist, &an_id, NULL)) == NULL)
      HE_REPORT_RETURN("failed to find an_id", FAIL);

    file_entry = (ANfile *) entry->data; /* get file entry from node */

    /* Check for empty annotation tree of 'type'? */   
    if (file_entry->an_num[type] == -1)
      {
        if ((file_entry->an_tree[type] = 
             (TBBT_TREE *) tbbtdmake(ANIanncmp,sizeof(int32))) == NULL)
          HE_REPORT_RETURN("failed to create annotation tree", FAIL);

        file_entry->an_num[type] = 0; /* intialize after allocation */
      }
#endif /* use tbbt */

    /* Create key from type/ref pair 
    *  ----------------------------
    *  | type(16bits) | ref(16bits) |
    *  -----------------------------*/
    ann_key = AN_CREATE_KEY(type, ann_ref);

#ifdef HAVE_RBTREE
    /* See if annotation of 'type' with ref exits */
    if ((entry = rb_find_gkey(file_entry->an_tree[type],&ann_key, ANIanncmp)) == NULL)
      HRETURN_ERROR(DFE_NOMATCH, FAIL);

    /* if not found return FAIL 
     * else get annotation entry from node */
    if (entry == file_entry->an_tree[type])
      HE_REPORT_RETURN("failed to find annotation of 'type'", FAIL);
    else
      ann_entry == (ANentry *) rb_val(entry); 
#else /*  use tbbt */
    /* See if annotation of 'type' with ref exits */
    if ((entry = tbbtdfind(file_entry->an_tree[type], &ann_key, NULL)) == NULL)
      HE_REPORT_RETURN("failed to find annotation of 'type'", FAIL);

    /* get annotation entry from node */
    ann_entry = (ANentry *) entry->data; 
#endif /* user tbbt */

    /* return annotation id */
    return ann_entry->ann_id;
} /* ANIfind */
#endif /* NOT_USED_YET */

/*--------------------------------------------------------------------------
 NAME
   ANInumann -- find number of annotation of 'type' that 
                 match the given element tag/ref 
 USAGE
       intn  ANInumann(an_id, type, elm_tag, elem_ref)
       int32  an_id;   IN: annotation interface id
       int    type:      IN: AN_DATA_LABEL for data labels, 
                             AN_DATA_DESC for data descriptions,
                             AN_FILE_LABEL for file labels,
                             AN_FILE_DESC for file descritpions.
       uint16 elem_tag,: IN: tag of item of which this is annotation
       uint16 elem_ref;  IN: ref of item of which this is annotation
 RETURNS
       number of annotation found if successful and FAIL (-1) otherwise
 DESCRIPTION
       Find number of annotation of 'type' for the given element 
       tag/ref pair. Doesn't handle file lables/descs yet.
 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
 *------------------------------------------------------------------------*/ 
PRIVATE intn
ANInumann(int32 an_id, ann_type type, uint16 elem_tag, uint16 elem_ref)
{
    CONSTR(FUNC, "ANInumann");
#ifdef HAVE_RBTREE
    Rb_node entry;
#else
    TBBT_NODE *entry = NULL;
#endif
    ANfile  *file_entry = NULL;
    ANentry *ann_entry  = NULL;
    intn    nanns = 0; 

    /* Clear error stack */
    HEclear();

    /* Valid file id */
    if (an_id == FAIL)
      HRETURN_ERROR(DFE_BADCALL, FAIL);

#ifdef HAVE_RBTREE
    /* Find node containing key(an_id) */
    if ((entry = rb_find_gkey(ANfilelist, &an_id, ANIfidcmp)) == NULL)
      HRETURN_ERROR(DFE_NOMATCH, FAIL);

    if (entry == ANfilelist)
      HE_REPORT_RETURN("failed to find an_id", FAIL);
    else
      file_entry = (ANfile *) rb_val(entry); /* get file entry from node */
#else  /* use tbbt */
    /* First find file node from file tree */
    if ((entry = tbbtdfind(ANfilelist, &an_id, NULL)) == NULL)
      HE_REPORT_RETURN("failed to find an_id", FAIL);

    file_entry = (ANfile *) entry->data; /* get file entry from node */
#endif /* use tbbt */

    /* Empty annotation tree */
    if (file_entry->an_num[type] == -1)
      {
        if (ANIcreate_ann_tree(an_id, type) == FAIL)
          HRETURN_ERROR(DFE_BADCALL, FAIL);
      }

#ifdef HAVE_RBTREE
    /* Traverse the list looking for a match */
    for(entry = rb_first(file_entry->an_tree[type]); 
        entry != nil(file_entry->an_tree[type]); entry = rb_next(entry))
      {
        ann_entry = (ANentry *) rb_val(entry); /* get annotation entry from node */
        if ((ann_entry->elmref == elem_ref) && (ann_entry->elmtag == elem_tag))
          {
            nanns++; /* increment ref counter if match */
          }
      }
#else /* use tbbt */
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
#endif /* use tbbt */

    /* return number of annotation references found for tag/ref */
    return nanns;
} /* ANInumann */

/*--------------------------------------------------------------------------
 NAME
   ANIannlist -- generate list of annotation ids of 'type' that 
                 match the given element tag/ref 
 USAGE
       intn  ANIannlist(an_id, type, elm_tag, elem_ref)
       int32  an_id;   IN: annotation interface id
       ann_type  type:   IN: AN_DATA_LABEL for data labels, 
                             AN_DATA_DESC for data descriptions,
                             AN_FILE_LABEL for file labels,
                             AN_FILE_DESC for file descritpions.
       uint16 elem_tag,: IN: tag of item of which this is annotation
       uint16 elem_ref;  IN: ref of item of which this is annotation
       int32  ann_list[]; OUT: array of ann_id's that match criteria.
 RETURNS
       number of annotations ids found if successful and FAIL (-1) otherwise
 DESCRIPTION
       Find and generate list of annotation ids of 'type' for the given 
       element tag/ref pair
 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
 *------------------------------------------------------------------------*/ 
PRIVATE intn
ANIannlist(int32 an_id, ann_type type, uint16 elem_tag, uint16 elem_ref,
           int32 ann_list[])
{
    CONSTR(FUNC, "ANIannlist");
#ifdef HAVE_RBTREE
    Rb_node entry;
#else
    TBBT_NODE *entry = NULL;
#endif
    ANfile  *file_entry = NULL;
    ANentry *ann_entry  = NULL;
    intn    nanns = 0;

    /* Clear error stack */
    HEclear();

    /* Valid file id */
    if (an_id == FAIL)
      HRETURN_ERROR(DFE_BADCALL, FAIL);

#ifdef HAVE_RBTREE
    /* Find node containing key(an_id) */
    if ((entry = rb_find_gkey(ANfilelist, &an_id, ANIfidcmp)) == NULL)
      HRETURN_ERROR(DFE_NOMATCH, FAIL);

    if (entry == ANfilelist)
      HE_REPORT_RETURN("failed to find an_id", FAIL);
    else
      file_entry = (ANfile *) rb_val(entry); /* get file entry from node */
#else  /* use tbbt */
    /* First find file node from file tree */
    if ((entry = tbbtdfind(ANfilelist, &an_id, NULL)) == NULL)
      HE_REPORT_RETURN("failed to find an_id", FAIL);

    file_entry = (ANfile *) entry->data; /* get file entry from node */
#endif /* use tbbt */

    /* Empty annotation tree */
    if (file_entry->an_num[type] == -1)
      {
        if (ANIcreate_ann_tree(an_id, type) == FAIL)
          HRETURN_ERROR(DFE_BADCALL, FAIL);
      }

#ifdef HAVE_RBTREE
    /* Traverse the list looking for a match */
    for(entry = rb_first(file_entry->an_tree[type]); 
        entry != nil(file_entry->an_tree[type]); entry = rb_next(entry))
      {
        ann_entry = (ANentry *) rb_val(entry); /* get annotation entry from node */
        if ((ann_entry->elmref == elem_ref) && (ann_entry->elmtag == elem_tag))
          { /* save ref of ann match in list */
            ann_list[nanns++] = ann_entry->ann_id; 
          }
      }

#else /* use tbbt */
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
#endif /* use tbbt */

    /* return number of annotation id's found for tag/ref */
    return nanns;
} /* ANIannlist */

/*--------------------------------------------------------------------------
 NAME
       ANIannlen -- get length of annotation givne annotation id
 USAGE
       int32 ANIannlen(ann_id)
       int32 ann_id;   IN: annotation id
 RETURNS
       length of annotation if successful and FAIL (-1) otherwise
 DESCRIPTION
       Uses the annotation id to find ann_key & file_id
 GLOBAL VARIABLES

 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
 *------------------------------------------------------------------------*/
PRIVATE int32
ANIannlen(int32 ann_id)
{
#ifdef LATER
    CONSTR(FUNC, "ANIannlen");
#endif /* LATER */
#ifdef HAVE_RBTREE
    Rb_node entry;
#else
    TBBT_NODE *entry = NULL;
#endif
    ANnode  *ann_node   = NULL;
    int32  file_id;
    int32  type;
    int32  ann_key;
    uint16 ann_tag;
    uint16 ann_ref;
    int32  ann_length=(-1);

    /* Clear error stack */
    HEclear();

    /* Valid annotation id */

#ifdef HAVE_RBTREE
    /* See if annotation of 'type' with ref exits */
    if ((entry = rb_find_gkey(ANnodelist, &ann_id, ANIanncmp)) == NULL)
      HE_REPORT_RETURN("Failed to find annotation with ann_id", FAIL);

    /* if not found return FAIL 
     * else get annotation node from node */
    if (entry == ANnodelist)
      HE_REPORT_RETURN("Failed to find annotation with an_id", FAIL);
    else
      ann_node = (ANnode *) rb_val(entry);

#else  /* use tbbt */
    /* First find file node from file tree */
    if ((entry = tbbtdfind(ANnodelist, &ann_id, NULL)) == NULL)
      HE_REPORT_RETURN("failed to find ann_id", FAIL);

    ann_node = (ANnode *) entry->data; /* get ann node from node */
#endif /* use tbbt */

    /* get file id and annotation key */
    file_id = ann_node->file_id;
    ann_key = ann_node->ann_key;
    type    = AN_KEY2TYPE(ann_key);
    ann_ref = AN_KEY2REF(ann_key);

    /* Valid file id */
    if (file_id == FAIL)
      HE_REPORT_RETURN("bad file_id", FAIL);

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
        HE_REPORT_RETURN("Bad annotation type for this call",FAIL);
      }

    if (ann_tag == DFTAG_DIL || ann_tag == DFTAG_DIA)
      {  /* 4=length of data tag/ref for data label or description */
        if ((ann_length = Hlength(file_id, ann_tag, ann_ref)) == FAIL) 
          HE_REPORT_RETURN("Failed to find annotation length", FAIL);
        ann_length -= 4;
      }
    else if (ann_tag == DFTAG_FID || ann_tag == DFTAG_FD)
      {  /* not data tag/ref for file label or description */
        if ((ann_length = Hlength(file_id, ann_tag, ann_ref)) == FAIL) 
          HE_REPORT_RETURN("Failed to find annotation length", FAIL);
      }

    /* return the length */
    return (ann_length);
} /* ANIannlen */

/*--------------------------------------------------------------------------
 NAME
       ANIreadann -- read annotation given ann_id
 USAGE
       intn ANIgetann(ann_id, ann, maxlen)
       int32 ann_id;   IN: annotation id (handle)
       char *ann;     OUT: space to return annotation in
       int32 maxlen;   IN: size of space to return annotation in
 RETURNS
       SUCCEED (0) if successful and FAIL (-1) otherwise
 DESCRIPTION
       Gets tag and ref of annotation.  Finds DD for that annotation.
       Reads the annotation, taking care of NULL terminator, if necessary.
 GLOBAL VARIABLES

 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
 *------------------------------------------------------------------------*/
PRIVATE intn
ANIreadann(int32 ann_id, char *ann, int32 maxlen)
{
    CONSTR(FUNC, "ANIreadann");
#ifdef HAVE_RBTREE
    Rb_node entry;
#else
    TBBT_NODE *entry = NULL;
#endif
    ANnode  *ann_node   = NULL;
    int32   file_id;
    int32   type;
    int32   ann_key;
    int32   aid;
    int32   ann_len;
    uint16  ann_tag;
    uint16  ann_ref;
    uint8   datadi[4] = {0,0,0,0};   /* to read in and discard data/ref! */

    /* Clear error stack */
    HEclear();

    /* Valid annotation id */
#ifdef HAVE_RBTREE
    /* See if annotation of 'type' with ref exits */
    if ((entry = rb_find_gkey(ANnodelist, &ann_id, ANIanncmp)) == NULL)
      HE_REPORT_RETURN("Failed to find annotation with ann_id", FAIL);

    /* if not found return FAIL 
     * else get annotation node from node */
    if (entry == ANnodelist)
      HE_REPORT_RETURN("Failed to find annotation with an_id", FAIL);
    else
      ann_node = (ANnode *) rb_val(entry);

#else  /* use tbbt */
    /* First find file node from file tree */
    if ((entry = tbbtdfind(ANnodelist, &ann_id, NULL)) == NULL)
      HE_REPORT_RETURN("failed to find ann_id", FAIL);

    ann_node = (ANnode *) entry->data; /* get ann node from node */
#endif /* use tbbt */

    /* get file id and annotation key */
    file_id = ann_node->file_id;
    ann_key = ann_node->ann_key;
    type    = AN_KEY2TYPE(ann_key);
    ann_ref = AN_KEY2REF(ann_key);

    /* Valid file id */
    if (file_id == FAIL)
        HRETURN_ERROR(DFE_BADCALL, FAIL);

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
        HE_REPORT_RETURN("Bad annotation type for this call",FAIL);
      }

    /* find DD for that annotation using tag/ref */
    aid = Hstartread(file_id, ann_tag, ann_ref);
    if (aid == FAIL)
      {
          Hendaccess(aid);
          HE_REPORT_RETURN("Failed to get access to annotation",FAIL);
      }
    if (FAIL == Hinquire(aid, (int32 *) NULL, (uint16 *) NULL, (uint16 *) NULL, 
                         &ann_len, (int32 *) NULL, (int32 *) NULL, 
                         (int16 *) NULL, (int16 *) NULL))
      {
          Hendaccess(aid);
          HE_REPORT_RETURN("Failed to get annotation",FAIL);
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
            HE_REPORT_RETURN("Failed to go past tag/ref",FAIL);
          }
      }

    /* read itslef annotation now..*/
    if ((int32) FAIL == Hread(aid, ann_len, ann))
      {     /* read the annotation */
          Hendaccess(aid);
          HE_REPORT_RETURN("Failed to read annotation",FAIL);
      }

    /* If Label need to NULL terminate string */
    if (ann_tag == DFTAG_FID || ann_tag == DFTAG_DIL)
      ann[ann_len] = '\0';     
#ifdef AN_DEBUG
        printf("ANIreadann: ann_len=%d, ann=%s\n", ann_len,ann);
#endif
    /* Close access to annotation object */
    Hendaccess(aid);

    return SUCCEED;
} /* ANIreadann */

/*--------------------------------------------------------------------------
 NAME
       ANIwriteann -- write annotation given ann_id
 USAGE
       intn ANIwriteann(ann_id, ann, ann_len)
       char *ann_id;   IN: annotation id
       char *ann;     IN: annotation to write
       int32 ann_len;  IN: length of annotation

 RETURNS
       SUCCEED (0) if successful and FAIL (-1) otherwise
 DESCRIPTION
       Checks for pre-existence of given annotation, replacing old one if it
       exists. Writes out annotation.
 GLOBAL VARIABLES

 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
 *------------------------------------------------------------------------*/
PRIVATE intn
ANIwriteann(int32 ann_id, char *ann, int32 ann_len)
{
    CONSTR(FUNC, "ANIwriteann");
#ifdef HAVE_RBTREE
    Rb_node fentry;
    Rb_node entry;
#else
    TBBT_NODE *fentry = NULL;
    TBBT_NODE *entry  = NULL;
#endif
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

    /* Clear error stack */
    HEclear();

    /* Valid annotation id */

#ifdef HAVE_RBTREE
    /* See if annotation of 'type' with ref exits */
    if ((entry = rb_find_gkey(ANnodelist, &ann_id, ANIanncmp)) == NULL)
      HE_REPORT_RETURN("Failed to find annotation with ann_id", FAIL);

    /* if not found return FAIL 
     * else get annotation node from node */
    if (entry == ANnodelist)
      HE_REPORT_RETURN("Failed to find annotation with an_id", FAIL);
    else
      ann_node = (ANnode *) rb_val(entry);

#else  /* use tbbt */
    /* First find node from ann_id tree */
    if ((entry = tbbtdfind(ANnodelist, &ann_id, NULL)) == NULL)
      HE_REPORT_RETURN("failed to find ann_id", FAIL);

    ann_node = (ANnode *) entry->data; /* get ann node from node */
#endif /* use tbbt */

    /* get file id and annotation key */
    file_id = ann_node->file_id;
    ann_key = ann_node->ann_key;
    type    = AN_KEY2TYPE(ann_key);
    ann_ref = AN_KEY2REF(ann_key);

    /* Valid file id */
    if (file_id == FAIL)
        HRETURN_ERROR(DFE_BADCALL, FAIL);

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
        HE_REPORT_RETURN("Bad annotation type for this call",FAIL);
      }

#ifdef HAVE_RBTREE
    /* Find node containing key(file_id) */
    if ((fentry = rb_find_gkey(ANfilelist, &file_id, ANIfidcmp)) == NULL)
      HRETURN_ERROR(DFE_NOMATCH, FAIL);

    if (fentry == ANfilelist)
      HE_REPORT_RETURN("failed to find file_id", FAIL);
    else
      file_entry = (ANfile *) rb_val(fentry); /* get file entry from node */

    /* Get annotation entry so that we can extract tag/ref of element 
    * Note that for file labels and descriptions the tag/ref contain
    * DFTAG_XXX and annotation reference number */
    if ((entry = rb_find_gkey(file_entry->an_tree[type], &ann_key, ANIanncmp)) == NULL)
      HE_REPORT_RETURN("failed to retrieve annotation of 'type' RBtree", FAIL);

    /* if not found return FAIL 
     * else get annotation entry from node */
    if (entry == file_entry->an_tree[type])
      HE_REPORT_RETURN("Failed to find annotation with ann_key ", FAIL);
    else
      ann_entry = (ANentry *) rb_val(entry);

#else  /* use tbbt */
    /* First find file node from file tree */
    if ((fentry = tbbtdfind(ANfilelist, &file_id, NULL)) == NULL)
      HE_REPORT_RETURN("failed to find file_id", FAIL);

    file_entry = (ANfile *) fentry->data; /* get file entry from node */

    /* Get annotation entry so that we can extract tag/ref of element 
    * Note that for file labels and descriptions the tag/ref contain
    * DFTAG_XXX and annotation reference number */
    if ((entry = tbbtdfind(file_entry->an_tree[type], &ann_key, NULL)) == NULL)
        HE_REPORT_RETURN("failed to retrieve annotation of 'type' tree", FAIL);

    ann_entry = (ANentry *) entry->data;
#endif /* use tbbt */

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
          HE_REPORT_RETURN("Unable to replace old annotation",FAIL);
      }

    if (ann_tag == DFTAG_DIL || ann_tag == DFTAG_DIA)
      { /* Data label/description 
         * Note: cannot use Hputelement because need to write data tag/ref */
        aid = Hstartwrite(file_id, ann_tag, ann_ref, ann_len + 4);
        if (aid == FAIL)
          {
            Hendaccess(aid);
            HE_REPORT_RETURN("Failed to start write access on annotation",FAIL);
          }

        /* Setup up to write annotation tag/ref */
        ptr = (uint8 *) &(datadi[0]);   /* first, write the object's tag/ref */
        UINT16ENCODE(ptr, elem_tag);
        UINT16ENCODE(ptr, elem_ref);

        /* write annotation tag/ref of element */
        if ((int32) FAIL == Hwrite(aid, (int32) 4, datadi))
          {
            Hendaccess(aid);
            HE_REPORT_RETURN("Failed to write tag/ref of annotation",FAIL);
          }

#ifdef AN_DEBUG
        printf("ANIwriteann: ann_len=%d, ann=%s\n", ann_len,ann);
#endif
        /* then write the annotation itself */
        if ((int32) FAIL == Hwrite(aid, ann_len, ann))
          {     
            Hendaccess(aid);
            HE_REPORT_RETURN("Failed to write annotation",FAIL);
          }

        Hendaccess(aid); /* end access to annotation */
      }
    else
      { /* file label/description 
         * write out file label/description */
        if (FAIL == Hputelement(file_id, ann_tag, ann_ref, (uint8 *) ann, ann_len))
          HE_REPORT_RETURN("Failed to write file annotation",FAIL);
#ifdef AN_DEBUG
        printf("ANIwriteann: fann_len=%d, fann=%s\n", ann_len,ann);
#endif
      }

    return SUCCEED;
} /* ANIwriteann */

/* ------------------------------- ANIcreate ------------------------------- 
 NAME
	ANIcreate - create a new annotation and return a handle(id)
 USAGE
	int32 ANIcreate(int32 file_id, uint16 tag, uint16 ref, char *ann, 
                      int32 ann_len, intn type )
        int32 file_id;  IN: file ID
        uint16 tag;     IN: tag of item to be assigned annotation
        uint16 ref;     IN: reference number of itme to be assigned ann
        ann_type  type: IN: AN_DATA_LABEL for data labels, 
                             AN_DATA_DESC for data descriptions,
                             AN_FILE_LABEL for file labels,
                             AN_FILE_DESC for file descritpions.
 RETURNS
        An ID to an annotation which can either be a label or description
 DESCRIPTION
        Creates an annotation, returns an 'an_id' to work with the new 
        annotation which can either be a label or description.
--------------------------------------------------------------------------- */
PRIVATE intn 
ANIcreate(int32 file_id, uint16 elem_tag, uint16 elem_ref, ann_type type )
{
  CONSTR(FUNC, "ANIcreate");    /* for HERROR */
  int32   ann_id;
  uint16  ann_tag;
  uint16  ann_ref;

  /* Clear error stack */
  HEclear();
  
  /* Valid file id */
  if (file_id == FAIL)
    HRETURN_ERROR(DFE_BADCALL, FAIL);

  /* new annotation */
  ann_ref = Hnewref(file_id);
  if (ann_ref == 0)
    HE_REPORT_RETURN("Failed create new ref for annotation",FAIL);

  /* deal with type */
  switch((ann_type)type)
    {
    case AN_DATA_LABEL:
      ann_tag = DFTAG_DIL;
      break;
    case AN_DATA_DESC:
      ann_tag = DFTAG_DIA;
      break;
    case AN_FILE_LABEL:
      /* for file label set elmement tag/ref to ann_tag & ref */
      ann_tag = DFTAG_FID;
      elem_tag = ann_tag;
      elem_ref = ann_ref;
      break;
    case AN_FILE_DESC:
      /* for file desc set elmement tag/ref to ann_tag & ref */
      ann_tag = DFTAG_FD;
      elem_tag = ann_tag;
      elem_ref = ann_ref;
      break;
    default:
      HE_REPORT_RETURN("Bad annotation type for this call",FAIL);
    }

  /* Check tag and ref */
  if (!elem_tag)
    HRETURN_ERROR(DFE_BADTAG, FAIL);
  if (!elem_ref)
    HRETURN_ERROR(DFE_BADREF, FAIL);
  
  /* put new annotation tag/ref into 'type' RBtree */
  if ((ann_id = ANIaddentry(file_id, type, ann_ref, elem_tag, elem_ref, 1)) == FAIL)
    HE_REPORT_RETURN("Failed to add annotation to RB tree",FAIL);

  return ann_id;
} /* ANIcreate() */

/* --------------------- Exported Multi-file Interface ----------------------*/

/* ------------------------------- ANstart -------------------------------- 
 NAME
	ANstart -- open file for annotation handling
 USAGE
	int32 ANstart(file_id, path, HDFmode)
        int32  file_id;         IN: file to start annotation access on
 RETURNS
        A file ID or FAIL
 DESCRIPTION
        Start annotation handling on the file return a annotation ID to the file.
--------------------------------------------------------------------------- */
EXPORT int32
ANstart(int32 file_id)
{
  CONSTR(FUNC, "ANstart");
  int32  *an_id    = NULL;
  ANfile *file_entry = NULL;

  /* Clear error stack */
  HEclear();

  if (file_id == FAIL)
    return FAIL;

  /* Check to see if we need to call ANInit, needs to moved to 
   * overall initialization call */
  if (ANfilelist == NULL || ANnodelist == NULL)
    ANinit();

  /* allocate space for file handle */
  if ((an_id = HDmalloc(sizeof(int32))) == NULL)
    HRETURN_ERROR(DFE_NOSPACE, FAIL);

  *an_id = file_id;

#if 0
  /* Open file */
  *file_id = Hopen(filename, acc_mode, 0);
  if (*file_id == FAIL)
    return FAIL;
#endif
  /* Intialize file entry */
  if ((file_entry = HDmalloc(sizeof(ANfile))) == NULL)
    HRETURN_ERROR(DFE_NOSPACE, FAIL);
#if 0
  file_entry->access_mode = acc_mode;
#endif
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
#if 0
  if ((file_entry->filename = HDmalloc(strlen(filename)+1)) == NULL)
    HRETURN_ERROR(DFE_NOSPACE, FAIL);
  HDstrcpy(file_entry->filename,filename);
#endif

#ifdef HAVE_RBTREE
  /* Add file entry to file RBtree */
  if (rb_insertg(ANfilelist, an_id, file_entry, ANIfidcmp) == NULL)
    HE_REPORT_RETURN("Failed to insert file_id into RBtree", FAIL);

#else  /* use tbbt */
  /* Add file entry to file tree */
  if (tbbtdins(ANfilelist, file_entry, an_id) == NULL)
    HE_REPORT_RETURN("Failed to insert file_id into RBtree", FAIL);

#endif /* use tbbt */
  /* return file id */
  return ((int32)*an_id);
} /* ANstart() */

/*--------------------------------------------------------------------------
 NAME
    ANfileinfo
 PURPOSE
    Report high-level information about the ANxxx interface for a given file.
 USAGE
    intn ANfileinfo(an_id, n_file_label, n_file_desc, n_obj_label, n_obj_desc)
        int32 an_id;        IN:  annotation interface id
        int32 *n_file_label;  OUT: the # of file labels
        int32 *n_file_desc;   OUT: the # of file descriptions
        int32 *n_obj_label;   OUT: the # of object labels
        int32 *n_obj_desc;    OUT: the # of object descriptions
 RETURNS
    SUCCEED/FAIL
 DESCRIPTION
    Reports general information about the number of file and object(i.e. data)
    annotations in the file. This routine is generally used to find
    the range of acceptable indices for ANselect calls.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
EXPORT intn 
ANfileinfo(int32 an_id, int32 *n_file_label, int32 *n_file_desc,
           int32 *n_obj_label, int32 *n_obj_desc)
{
  CONSTR(FUNC, "ANfileinfo");    /* for HERROR */
#ifdef HAVE_RBTREE
  Rb_node entry;
#else
  TBBT_NODE *entry = NULL;
#endif
  ANfile  *file_entry = NULL;

  /* Clear error stack */
  HEclear();

  /* Valid file id */
  if (an_id == FAIL)
    HRETURN_ERROR(DFE_BADCALL, FAIL);

#ifdef HAVE_RBTREE
  /* Find node containing key(an_id) */
  if ((entry = rb_find_gkey(ANfilelist, &an_id, ANIfidcmp)) == NULL)
    HRETURN_ERROR(DFE_NOMATCH, FAIL);

  if (entry == ANfilelist)
    HE_REPORT_RETURN("failed to find an_id", FAIL);
  else
    file_entry = (ANfile *) rb_val(entry); /* get file entry from node */
#else  /* use tbbt */
  /* First find file node from file tree */
  if ((entry = tbbtdfind(ANfilelist, &an_id, NULL)) == NULL)
    HE_REPORT_RETURN("failed to find an_id", FAIL);

  file_entry = (ANfile *) entry->data; /* get file entry from node */
#endif /* use tbbt */

  /* Empty file label annotation tree? */
  if (file_entry->an_num[AN_FILE_LABEL] == -1)
    {
      if ((*n_file_label = ANIcreate_ann_tree(an_id, AN_FILE_LABEL)) == FAIL)
        HE_REPORT_RETURN("failed to create file label annotation RBtree", FAIL);
    }
  else
    *n_file_label = file_entry->an_num[AN_FILE_LABEL];

  /* Empty file descritpiton annotation tree? */
  if (file_entry->an_num[AN_FILE_DESC] == -1)
    {
      if ((*n_file_desc = ANIcreate_ann_tree(an_id, AN_FILE_DESC)) == FAIL)
        HE_REPORT_RETURN("failed to create file desc annotation RBtree", FAIL);
    }
  else
    *n_file_desc = file_entry->an_num[AN_FILE_DESC];

  /* Empty label annotation tree? */
  if (file_entry->an_num[AN_DATA_LABEL] == -1)
    {
      if ((*n_obj_label = ANIcreate_ann_tree(an_id, AN_DATA_LABEL)) == FAIL)
        HE_REPORT_RETURN("failed to create data label annotation RBtree", FAIL);
    }
  else
    *n_obj_label = file_entry->an_num[AN_DATA_LABEL];

  /* Empty descritpiton annotation tree? */
  if (file_entry->an_num[AN_DATA_DESC] == -1)
    {
      if ((*n_obj_desc = ANIcreate_ann_tree(an_id, AN_DATA_DESC)) == FAIL)
        HE_REPORT_RETURN("failed to create data desc annotation RBtree", FAIL);
    }
  else
    *n_obj_desc = file_entry->an_num[AN_DATA_DESC];

    return SUCCEED;
} /* ANfileinfo() */

/* -------------------------------- ANend ---------------------------------
 NAME
	ANend -- End annotation access to file file
 USAGE
	int32 ANend(an_id)
        int32 an_id;               IN: Annotation ID of file to close
 RETURNS
        SUCCEED / FAIL
 DESCRIPTION
        End annotation access to file.
--------------------------------------------------------------------------- */
EXPORT int32
ANend(int32 an_id)
{
  CONSTR(FUNC,"ANend");
#ifdef HAVE_RBTREE
  Rb_node fentry;
  Rb_node aentry;
  Rb_node entry;
#else
  TBBT_NODE *fentry = NULL;
  TBBT_NODE *aentry = NULL;
  TBBT_NODE *entry  = NULL;
#endif
  ANfile  *file_entry = NULL;
  ANentry *ann_entry  = NULL;
  ANnode  *ann_node   = NULL;
  VOID    *ann_key = NULL;
  VOID    *kp = NULL;
  
  /* Clear error stack */
  HEclear();

  /* Valid file id */
  if (an_id == FAIL)
    HRETURN_ERROR(DFE_BADCALL, FAIL);

#ifdef HAVE_RBTREE
    /* Find node containing key(an_id) */
    if ((fentry = rb_find_gkey(ANfilelist, &an_id, ANIfidcmp)) == NULL)
      HRETURN_ERROR(DFE_NOMATCH, FAIL);

    if (fentry == ANfilelist)
      HE_REPORT_RETURN("failed to find an_id", FAIL);
    else
      file_entry = (ANfile *) rb_val(fentry); /* get file entry from node */
#else  /* use tbbt */
    /* First find file node from file tree */
    if ((fentry = tbbtdfind(ANfilelist, &an_id, NULL)) == NULL)
      HE_REPORT_RETURN("failed to find an_id", FAIL);

    file_entry = (ANfile *) fentry->data; /* get file entry from node */
#endif /* use tbbt */

  if (file_entry->filename != NULL)
    HDfree(file_entry->filename);  /* free file name */
  
  /* NEED to delete trees of annotations attached to node 
  * NOTE: This could be written shorter using a for loop....
  * or something....  */

#ifdef HAVE_RBTREE
  /* free file label annotation rb tree */
  if (file_entry->an_tree[AN_FILE_LABEL] != NULL) 
    { /* Traverse tree puling ann_id's to delete from ANnodelist */
      for(aentry = rb_first(file_entry->an_tree[AN_FILE_LABEL]); 
          aentry != nil(file_entry->an_tree[AN_FILE_LABEL]); 
          aentry = rb_next(aentry))
        { /* get annotation entry from node */
          ann_entry = (ANentry *) rb_val(aentry); 

          /* See if annotation exits */
          if ((entry = rb_find_gkey(ANnodelist,ann_entry->ann_id, ANIanncmp)) 
              == NULL)
            HE_REPORT_RETURN("Failed to find annotation with ann_id", FAIL);

          /* if not found return FAIL 
           * else get annotation node from node */
          if (entry == ANnodelist)
            HE_REPORT_RETURN("Failed to find annotation with an_id", FAIL);
          else
            {
              ann_node = (ANnode *) rb_val(entry);
              ann_key  = (int32 *) rb_key(entry);
            }

          /* free data & key */
          HDfree(ann_node);
          HDfree(ann_key);

          /* delete node from ANndoelist, frees it too */
          if (rb_delete_node(entry) == -1)
            HRETURN_ERROR(DFE_BADCALL, FAIL);      
        } /* end for 'entry */
      /* finally free tree */
      rb_free_tree(file_entry->an_tree[AN_FILE_LABEL]);  
    }

  /* free file desc annotation rb tree */
  if (file_entry->an_tree[AN_FILE_DESC] != NULL)
    { /* Traverse tree puling ann_id's to delete from ANnodelist */
      for(aentry = rb_first(file_entry->an_tree[AN_FILE_DESC]); 
          aentry != nil(file_entry->an_tree[AN_FILE_DESC]); 
          aentry = rb_next(aentry))
        { /* get annotation entry from node */
          ann_entry = (ANentry *) rb_val(aentry); 

          /* See if annotation exits */
          if ((entry = rb_find_gkey(ANnodelist,ann_entry->ann_id, ANIanncmp)) 
              == NULL)
            HE_REPORT_RETURN("Failed to find annotation with ann_id", FAIL);

          /* if not found return FAIL 
           * else get annotation node from node */
          if (entry == ANnodelist)
            HE_REPORT_RETURN("Failed to find annotation with an_id", FAIL);
          else
            {
              ann_node = (ANnode *) rb_val(entry);
              ann_key  = (int32 *) rb_key(entry);
            }

          /* free data & key */
          HDfree(ann_node);
          HDfree(ann_key);

          /* delete node from ANndoelist */
          if (rb_delete_node(entry) == -1)
            HRETURN_ERROR(DFE_BADCALL, FAIL);      
        } /* end for 'entry */
      /* finally free tree itself */
      rb_free_tree(file_entry->an_tree[AN_FILE_DESC]); 
    }

  /* free label annotation rb tree */
  if (file_entry->an_tree[AN_DATA_LABEL] != NULL)
    { /* Traverse tree puling ann_id's to delete from ANnodelist */
      for(aentry = rb_first(file_entry->an_tree[AN_DATA_LABEL]); 
          aentry != nil(file_entry->an_tree[AN_DATA_LABEL]); 
          aentry = rb_next(aentry))
        { /* get annotation entry from node */
          ann_entry = (ANentry *) rb_val(aentry); 

          /* See if annotation exits */
          if ((entry = rb_find_gkey(ANnodelist,ann_entry->ann_id, ANIanncmp)) 
              == NULL)
            HE_REPORT_RETURN("Failed to find annotation with ann_id", FAIL);

          /* if not found return FAIL 
           * else get annotation node from node */
          if (entry == ANnodelist)
            HE_REPORT_RETURN("Failed to find annotation with an_id", FAIL);
          else
            {
              ann_node = (ANnode *) rb_val(entry);
              ann_key  = (int32 *) rb_key(entry);
            }

          /* free data & key */
          HDfree(ann_node);
          HDfree(ann_key);

          /* delete node from ANndoelist */
          if (rb_delete_node(entry) == -1)
            HRETURN_ERROR(DFE_BADCALL, FAIL);      
        } /* end for 'entry */
      /* finally free tree itself */
      rb_free_tree(file_entry->an_tree[AN_DATA_LABEL]);  
    }

  /* free desc annotation rb tree */
  if (file_entry->an_tree[AN_DATA_DESC] != NULL)
    { /* Traverse tree puling ann_id's to delete from ANnodelist */
      for(aentry = rb_first(file_entry->an_tree[AN_DATA_DESC]); 
          aentry != nil(file_entry->an_tree[AN_DATA_DESC]); 
          aentry = rb_next(aentry))
        { /* get annotation entry from node */
          ann_entry = (ANentry *) rb_val(aentry); 

          /* See if annotation exits */
          if ((entry = rb_find_gkey(ANnodelist,ann_entry->ann_id, ANIanncmp)) 
              == NULL)
            HE_REPORT_RETURN("Failed to find annotation with ann_id", FAIL);

          /* if not found return FAIL 
           * else get annotation node from node */
          if (entry == ANnodelist)
            HE_REPORT_RETURN("Failed to find annotation with an_id", FAIL);
          else
            {
              ann_node = (ANnode *) rb_val(entry);
              ann_key  = (int32 *) rb_key(entry);
            }

          /* free data & key */
          HDfree(ann_node);
          HDfree(ann_key);

          /* delete node from ANndoelist */
          if (rb_delete_node(entry) == -1)
            HRETURN_ERROR(DFE_BADCALL, FAIL);      
        } /* end for 'entry */
      /* finally free tree itself */
      rb_free_tree(file_entry->an_tree[AN_DATA_DESC]); 
    }

  HDfree(file_entry);             /* free file entry */

  /* Now we can delete the node itself */
  if (rb_delete_node(fentry) == -1)
    HRETURN_ERROR(DFE_BADCALL, FAIL);

#else /* use tbbt */

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
            HE_REPORT_RETURN("Failed to find annotation with ann_id", FAIL);

          /* delete node from ANndoelist */
          if ((ann_node =(ANnode *) tbbtrem((TBBT_NODE **)ANnodelist, entry, 
                                            &ann_key))== NULL)
            HRETURN_ERROR(DFE_BADCALL, FAIL);      

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
            HE_REPORT_RETURN("Failed to find annotation with ann_id", FAIL);

          /* delete node from ANndoelist */
          if ((ann_node =(ANnode *) tbbtrem((TBBT_NODE **)ANnodelist, entry, 
                                            &ann_key))== NULL)
            HRETURN_ERROR(DFE_BADCALL, FAIL);      

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
            HE_REPORT_RETURN("Failed to find annotation with ann_id", FAIL);

          /* delete node from ANndoelist */
          if ((ann_node =(ANnode *) tbbtrem((TBBT_NODE **)ANnodelist, entry, 
                                            &ann_key))== NULL)
            HRETURN_ERROR(DFE_BADCALL, FAIL);      

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
            HE_REPORT_RETURN("Failed to find annotation with ann_id", FAIL);

          /* delete node from ANndoelist */
          if ((ann_node =(ANnode *) tbbtrem((TBBT_NODE **)ANnodelist, entry, 
                                            &ann_key))== NULL)
            HRETURN_ERROR(DFE_BADCALL, FAIL);      

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
    HRETURN_ERROR(DFE_BADCALL, FAIL);      

  /* free file data & key */
  if (file_entry != NULL)
    HDfree(file_entry);
  if(kp != NULL)
    HDfree(kp);
#endif /* use tbbt */

#if 0
  /* Close file and set return value */
  return Hclose(file_id);
#endif
  return SUCCEED;
} /* ANend() */

/* ------------------------------------------------------------------------ 
 NAME
	ANcreate - create a new element annotation and return a handle(id)
 USAGE
	int32 ANcreate(an_id, tag, ref, ann, ann_len, type )
        int32 an_id;    IN: annotation interface ID
        uint16 tag;     IN: tag of item to be assigned annotation
        uint16 ref;     IN: reference number of itme to be assigned ann
        ann_type  type: IN: AN_DATA_LABEL for data labels, 
                            AN_DATA_DESC for data descriptions,
 RETURNS
        An ID to an annotation which can either be a label or description
 DESCRIPTION
        Creates a data annotation, returns an 'an_id' to work with the new 
        annotation which can either be a label or description.
--------------------------------------------------------------------------- */
EXPORT int32
ANcreate(int32 an_id, uint16 elem_tag, uint16 elem_ref, ann_type type)
{
#ifdef LATER
  CONSTR(FUNC, "ANcreate");    /* for HERROR */
#endif /* LATER */

  return (ANIcreate(an_id, elem_tag, elem_ref, type));
} /* ANcreate() */

/* ------------------------------------------------------------------------ 
 NAME
	ANcreatef - create a new file annotation and return a handle(id)
 USAGE
	int32 ANcreatef(an_id,ann_type type )
        int32 an_id;    IN: annotation interface ID
        ann_type  type: IN:  AN_FILE_LABEL for file labels,
                             AN_FILE_DESC for file descritpions.
 RETURNS
        An ID to an annotation which can either be a file label or description
 DESCRIPTION
        Creates a file annotation, returns an 'an_id' to work with the new 
        file annotation which can either be a label or description.
        
--------------------------------------------------------------------------- */
EXPORT int32
ANcreatef(int32 an_id, ann_type type)
{
#ifdef LATER
  CONSTR(FUNC, "ANcreatef");    /* for HERROR */
#endif /* LATER */
  uint16 ann_tag;
  uint16 ann_ref;

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
      HE_REPORT_RETURN("Bad annotation type for this call",FAIL);
    }

  return (ANIcreate(an_id, ann_tag, ann_ref, type));
} /* ANcreateann() */

/* ------------------------------- ANselect ------------------------------- 
 NAME
	ANselect -- get an annotation ID from index of 'type'
 USAGE
	int32 ANselect(file_id, index)
        int32 an_id;    IN: annotation interface ID
        int32 index;    IN: index of annottion to get ID for
        ann_type  type: IN: AN_DATA_LABEL for data labels, 
                            AN_DATA_DESC for data descriptions,
                            AN_FILE_LABEL for file labels,
                            AN_FILE_DESC for file descritpions.
 RETURNS
        An ID to an annotation type which can either be a label or description 
 DESCRIPTION
        The position index is ZERO based
--------------------------------------------------------------------------- */
EXPORT int32
ANselect(int32 an_id, int32 index, ann_type type)
{
  CONSTR(FUNC, "ANselect");    /* for HERROR */
#ifdef HAVE_RBTREE
  Rb_node fentry;
  Rb_node entry;
#else
  TBBT_NODE *fentry = NULL;
  TBBT_NODE *entry  = NULL;
#endif
  ANfile  *file_entry = NULL;
  ANentry *ann_entry  = NULL;
#ifdef HAVE_RBTREE
  intn    i;
#endif /* HAVE_RBTREE */

  /* Clear error stack */
  HEclear();

  /* Valid file id */
  if (an_id == FAIL)
    HRETURN_ERROR(DFE_BADCALL, FAIL);

#ifdef HAVE_RBTREE
  /* Find node containing key(an_id) */
  if ((fentry = rb_find_gkey(ANfilelist, &an_id, ANIfidcmp)) == NULL)
    HRETURN_ERROR(DFE_NOMATCH, FAIL);
  
  if (fentry == ANfilelist)
    HE_REPORT_RETURN("failed to find an_id", FAIL);
  else
    file_entry = (ANfile *) rb_val(fentry); /* get file entry from node */
#else  /* use tbbt */
  /* First find file node from file tree */
  if ((fentry = tbbtdfind(ANfilelist, &an_id, NULL)) == NULL)
    HE_REPORT_RETURN("failed to find an_id", FAIL);

  file_entry = (ANfile *) fentry->data; /* get file entry from node */
#endif /* use tbbt */

  /* Empty annotation tree */
  if (file_entry->an_num[type] == -1)
    {
      if (ANIcreate_ann_tree(an_id, type) == FAIL)
        HRETURN_ERROR(DFE_BADCALL, FAIL);
    }

#ifdef HAVE_RBTREE
  /* Traverse the list till 'index' */
  for(entry = rb_first(file_entry->an_tree[type]), i = 0; 
      entry != nil(file_entry->an_tree[type]) && i != index; 
      entry = rb_next(entry), i++);

  if (entry == file_entry->an_tree[type])
    HE_REPORT_RETURN("failed to find annoation of 'type' with 'index'", FAIL);
  else
    ann_entry = (ANentry *) rb_val(entry); 

#else  /* use tbbt */

  /* check index and adjust to 1 based for tbbtindx()*/
  if(index >= 0 && index <= file_entry->an_num[type])
    index++;
  else
    HE_REPORT_RETURN("bad index", FAIL);

  /* find 'index' entry */
  if ((entry = tbbtindx((TBBT_NODE *)*(file_entry->an_tree[type]), index)) == NULL)
    HE_REPORT_RETURN("failed to find 'index' entry", FAIL);

  ann_entry = (ANentry *) entry->data; 
#endif /* use tbbt */

  /* return ann_id */
  return ann_entry->ann_id;
} /* ANselect() */

/*--------------------------------------------------------------------------
 NAME
   ANnumann -- find number of annotation of 'type' that 
                 match the given element tag/ref 
 USAGE
       intn  ANnumann(an_id, type, elm_tag, elem_ref)
       int32  an_id;     IN: annotation interface id
       int    type:      IN: AN_DATA_LABEL for data labels, 
                             AN_DATA_DESC for data descriptions,
                             AN_FILE_LABEL for file labels,
                             AN_FILE_DESC for file descritpions.
       uint16 elem_tag,: IN: tag of item of which this is annotation
       uint16 elem_ref;  IN: ref of item of which this is annotation
 RETURNS
       number of annotation found if successful and FAIL (-1) otherwise
 DESCRIPTION
       Find number of annotation of 'type' for the given element 
       tag/ref pair.
 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
 *------------------------------------------------------------------------*/ 
EXPORT intn
ANnumann(int32 an_id, ann_type type, uint16 elem_tag, uint16 elem_ref)
{
#ifdef LATER
  CONSTR(FUNC, "ANnumann");
#endif /* LATER */

  return ANInumann(an_id, type, elem_tag, elem_ref);
} /* ANnumann() */

/*--------------------------------------------------------------------------
 NAME
   ANannlist -- generate list of annotation ids of 'type' that 
                 match the given element tag/ref 
 USAGE
       intn  ANannlist(an_id, type, elm_tag, elem_ref)
       int32  an_id;     IN: annotation interface id
       ann_type  type:   IN: AN_DATA_LABEL for data labels, 
                             AN_DATA_DESC for data descriptions,
                             AN_FILE_LABEL for file labels,
                             AN_FILE_DESC for file descritpions.
       uint16 elem_tag,: IN: tag of item of which this is annotation
       uint16 elem_ref;  IN: ref of item of which this is annotation
       int32  ann_list[]; OUT: array of ann_id's that match criteria.
 RETURNS
       number of annotations ids found if successful and FAIL (-1) otherwise
 DESCRIPTION
       Find and generate list of annotation ids of 'type' for the given 
       element tag/ref pair
 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
 *------------------------------------------------------------------------*/ 
EXPORT intn
ANannlist(int32 an_id, ann_type type, uint16 elem_tag, uint16 elem_ref,
           int32 ann_list[])
{
#ifdef LATER
  CONSTR(FUNC, "ANannlist");
#endif /* LATER */

  return ANIannlist(an_id, type, elem_tag, elem_ref, ann_list);
} /* ANannlist() */

/*--------------------------------------------------------------------------
 NAME
       ANannlen -- get length of annotation givne annotation id
 USAGE
       int32 ANannlen(ann_id)
       int32 ann_id;   IN: annotation id
 RETURNS
       length of annotation if successful and FAIL (-1) otherwise
 DESCRIPTION
       Uses the annotation id to find ann_key & file_id
 GLOBAL VARIABLES

 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
 *------------------------------------------------------------------------*/
EXPORT int32
ANannlen(int32 ann_id)
{
#ifdef LATER
  CONSTR(FUNC, "ANannlen");
#endif /* LATER */

  return ANIannlen(ann_id);
} /* ANannlen() */

/*--------------------------------------------------------------------------
 NAME
       ANwriteann -- write annotation given ann_id
 USAGE
       intn ANwriteann(ann_id, ann, ann_len)
       char *ann_id;   IN: annotation id
       char *ann;     IN: annotation to write
       int32 ann_len;  IN: length of annotation

 RETURNS
       SUCCEED (0) if successful and FAIL (-1) otherwise
 DESCRIPTION
       Checks for pre-existence of given annotation, replacing old one if it
       exists. Writes out annotation.
 GLOBAL VARIABLES

 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
 *------------------------------------------------------------------------*/
EXPORT int32
ANwriteann(int32 ann_id, char *ann, int32 annlen)
{
#ifdef LATER
  CONSTR(FUNC, "ANwriteann");    /* for HERROR */
#endif /* LATER */

  return ANIwriteann(ann_id, ann, annlen);
} /* ANwriteann() */

/*--------------------------------------------------------------------------
 NAME
       ANreadann -- read annotation given ann_id
 USAGE
       intn ANreadann(ann_id, ann, maxlen)
       int32 ann_id;   IN: annotation id (handle)
       char *ann;     OUT: space to return annotation in
       int32 maxlen;   IN: size of space to return annotation in
 RETURNS
       SUCCEED (0) if successful and FAIL (-1) otherwise
 DESCRIPTION
       Gets tag and ref of annotation.  Finds DD for that annotation.
       Reads the annotation, taking care of NULL terminator, if necessary.
 GLOBAL VARIABLES

 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
 *------------------------------------------------------------------------*/
EXPORT int32
ANreadann(int32 ann_id, char *ann, int32 maxlen)
{
#ifdef LATER
  CONSTR(FUNC, "ANreadann");    /* for HERROR */
#endif /* LATER */

  return ANIreadann(ann_id, ann, maxlen);
} /* ANreadann() */

/* ----------------------------------------------------------------------- 
 NAME
	ANendaccess -- end access to an annotation given it's id
 USAGE
	intn ANendaccess(ann_id)
        int32 ann_id;    IN: annotation id
 RETURNS
        SUCCEED or FAIL
 DESCRIPTION
        Terminates access to an annotation. For now does nothing
--------------------------------------------------------------------------- */
EXPORT intn
ANendaccess(int32 ann_id)
{
#ifdef LATER
  CONSTR(FUNC, "ANendaccess");    /* for HERROR */
#endif /* LATER */

  return SUCCEED;
} /* ANendaccess() */

/* -------------------------------------------------------------- 
 NAME
	ANget_tagref - get tag/ref pair to annotation ID
 USAGE
	int32 ANget_tagref(an_id, index, type, tag, ref)
        int32 an_id;   IN: annotation interface ID
        int32 index;     IN: index of annottion to get tag/ref for
        ann_type  type:  IN: AN_DATA_LABEL for data labels, 
                             AN_DATA_DESC for data descriptions,
                             AN_FILE_LABEL for file labels,
                             AN_FILE_DESC for file descritpions.
        uint16 tag;     OUT: Tag for annotation
        uint16 ref;     OUT: ref for annotation
 RETURNS
        A tag/ref pairt to an annotation type which can either be a 
        label or description given the annotation ID
 DESCRIPTION
        The position index is zero based
--------------------------------------------------------------------------- */
EXPORT int32
ANget_tagref(int32 an_id, int32 index, ann_type type, uint16 *tag, uint16 *ref)
{
  CONSTR(FUNC, "ANget_tagref");    /* for HERROR */
#ifdef HAVE_RBTREE
  Rb_node fentry;
  Rb_node entry;
#else
  TBBT_NODE *fentry = NULL;
  TBBT_NODE *entry  = NULL;
#endif
  ANfile  *file_entry = NULL;
  ANentry *ann_entry  = NULL;
#ifdef HAVE_RBTREE
  intn    i;
#endif /* HAVE_RBTREE */

  /* Clear error stack */
  HEclear();

  /* Valid file id */
  if (an_id == FAIL)
    HRETURN_ERROR(DFE_BADCALL, FAIL);

#ifdef HAVE_RBTREE
  /* Find node containing key(an_id) */
  if ((fentry = rb_find_gkey(ANfilelist, &an_id, ANIfidcmp)) == NULL)
    HRETURN_ERROR(DFE_NOMATCH, FAIL);
  
  if (fentry == ANfilelist)
    HE_REPORT_RETURN("failed to find an_id", FAIL);
  else
    file_entry = (ANfile *) rb_val(fentry); /* get file entry from node */
#else  /* use tbbt */
  /* First find file node from file tree */
  if ((fentry = tbbtdfind(ANfilelist, &an_id, NULL)) == NULL)
    HE_REPORT_RETURN("failed to find an_id", FAIL);

  file_entry = (ANfile *) fentry->data; /* get file entry from node */
#endif /* use tbbt */

  /* Empty annotation tree */
  if (file_entry->an_num[type] == -1)
    {
      if (ANIcreate_ann_tree(an_id, type) == FAIL)
        HRETURN_ERROR(DFE_BADCALL, FAIL);
    }

#ifdef HAVE_RBTREE
  /* Traverse the list till 'index' */
  for(entry = rb_first(file_entry->an_tree[type]), i = 0; 
      entry != nil(file_entry->an_tree[type]) && i != index; 
      entry = rb_next(entry), i++);

  if (entry == file_entry->an_tree[type])
    HE_REPORT_RETURN("failed to find annoation of 'type' with 'index'", FAIL);
  else
    ann_entry = (ANentry *) rb_val(entry); 

#else  /* use tbbt */

  /* check index and adjust to 1 based for tbbtindx()*/
  if(index >= 0 && index <= file_entry->an_num[type])
    index++;
  else
    HE_REPORT_RETURN("bad index", FAIL);

  /* find 'index' entry */
  if ((entry = tbbtindx((TBBT_NODE *)*(file_entry->an_tree[type]), index)) == NULL)
    HE_REPORT_RETURN("failed to find 'index' entry", FAIL);

  ann_entry = (ANentry *) entry->data; 
#endif /* use tbbt */

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
      HE_REPORT_RETURN("Bad annotation type for this call",FAIL);
    }

  return SUCCEED;
} /* ANget_tagref() */

/*--------------------------------------------------------------------------
 NAME
       ANid2tagref -- get tag/ref given annotation id
 USAGE
       int32 ANid2tagref(ann_id, tag, ref)
       int32 ann_id;   IN: annotation id
       uint16 tag;     OUT: Tag for annotation
       uint16 ref;     OUT: ref for annotation
 RETURNS
       SUCCEED if successful and FAIL (-1) otherwise. 
 DESCRIPTION
       Uses the annotation id to find ann_node entry which contains ann_ref
 GLOBAL VARIABLES

 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
 *------------------------------------------------------------------------*/
int32
ANid2tagref(int32 ann_id, uint16 *tag, uint16 *ref)
{
#ifdef LATER
    CONSTR(FUNC, "ANid2tagref");
#endif /* LATER */
#ifdef HAVE_RBTREE
    Rb_node entry;
#else
    TBBT_NODE *entry = NULL;
#endif
    ANnode  *ann_node   = NULL;
    int32  file_id;
    int32  type;
    int32  ann_key;
    uint16 ann_ref;

    /* Clear error stack */
    HEclear();

    /* Valid annotation id */

#ifdef HAVE_RBTREE
    /* See if annotation of 'type' with ref exits */
    if ((entry = rb_find_gkey(ANnodelist, &ann_id, ANIanncmp)) == NULL)
      HE_REPORT_RETURN("Failed to find annotation with ann_id", FAIL);

    /* if not found return FAIL 
     * else get annotation node from node */
    if (entry == ANnodelist)
      HE_REPORT_RETURN("Failed to find annotation with an_id", FAIL);
    else
      ann_node = (ANnode *) rb_val(entry);

#else  /* use tbbt */
    /* First find file node from file tree */
    if ((entry = tbbtdfind(ANnodelist, &ann_id, NULL)) == NULL)
      HE_REPORT_RETURN("failed to find ann_id", FAIL);

    ann_node = (ANnode *) entry->data; /* get ann node from node */
#endif /* use tbbt */

    /* get file id and annotation key */
    file_id = ann_node->file_id;
    ann_key = ann_node->ann_key;
    type    = AN_KEY2TYPE(ann_key);
    ann_ref = AN_KEY2REF(ann_key);

    /* Valid file id */
    if (file_id == FAIL)
      HE_REPORT_RETURN("bad file_id", FAIL);

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
        HE_REPORT_RETURN("Bad annotation type for this call",FAIL);
      }

    return SUCCEED;
} /* ANid2tagref */

/*-------------------------------------------------------------------- 
 NAME
     atype2tag - annotation type to corresponding annotation TAG
 DESCRIPTION
     translate annotation type to corresponding TAG
 RETURNS
     returns TAG correspondign to annotatin type
--------------------------------------------------------------------*/
EXPORT uint16
atype2tag(ann_type atype)
{   /* Switch on annotation type "atype" */
#ifdef LATER
  CONSTR(FUNC, "atype2tag");    /* for HERROR */
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
} /* atype2tag */

/*-------------------------------------------------------------------- 
 NAME
     tag2atype - annotation TAG to corresponding annotation type
 DESCRIPTION
     translate annotation TAG to corresponding atype
 RETURNS
     returns type correspondign to annotatin TAG
--------------------------------------------------------------------*/
EXPORT ann_type
tag2atype(uint16 atag)
{   /* Switch on annotation tag */
#ifdef LATER
  CONSTR(FUNC, "tag2atype");    /* for HERROR */
#endif /* LATER */
  ann_type atype;

  switch((uint16)atag) 
    {
    case DFTAG_FID: atype = AN_FILE_LABEL; break;
    case DFTAG_FD:  atype = AN_FILE_DESC;  break;
    case DFTAG_DIL: atype = AN_DATA_LABEL; break;
    case DFTAG_DIA: atype = AN_DATA_DESC;  break;
    default: atype = -1;
    } /* switch */
  return atype;
} /* tag2atype */

#endif /* MFAN_C */

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
 * Contents:
 *
 *  Private Routines:
 *  -----------------
 *  NOTES: TYPE here refers to file/data label/description types 
 *         tag/ref refers to data tag/ref
 *
 *  ANinit        - Intialize the annotation interface
 *  ANIfidcmp     - compare two file id's (used in file RBtree)
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
 *  ANstart     - open file for annotation handling, returns file id
 *  ANfilefinfo - get number of file/data annotations in file. Indices returned
 *                are used in ANselect() calls.
 *  ANend       - end access to annotation handling
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
 *---------------------------------------------------------------------------*/

#ifndef MFAN_C
#define MFAN_C
#include "hdf.h"
#include "hfile.h"
#include "mfan.h"

/*-----------------------------------------------------------------------------
 *                          Internal Routines
 *---------------------------------------------------------------------------*/

/* ------------------------------- ANinit -------------------------------- 
 NAME
	ANinit -- Initialize Annotation Interface
 USAGE
	int32 ANstart(void)

 RETURNS
        SUCCEED or FAIL
 DESCRIPTION
        Initializes the annotation interface i.e. creates red-black tree
        to hold all open files using annotation interface.
--------------------------------------------------------------------------- */
PRIVATE int32
ANinit(void)
{
    CONSTR(FUNC, "ANinit");
    
    /* Clear error stack */
    HEclear();

    /* Check to see if we need to create RB tree for file id's */
    if (ANfilelist == NULL)
      if ((ANfilelist = make_rb()) == NULL)
        HE_REPORT_RETURN("failed to create RBtree for file_ids", FAIL);

    /* Check to see if we need to create RB tree for annotations id's */
    if (ANnodelist == NULL)
      if ((ANnodelist = make_rb()) == NULL)
        HE_REPORT_RETURN("failed to create RBtree for ann_ids", FAIL);

   return SUCCEED;
} /* ANinit() */

/* ------------------------------- ANIfidcmp -------------------------------- 
 NAME
	ANIfidcmp -- compare two file id's
 USAGE
	int32 ANIfidcmp(i, j)
        int32 i;        IN: file id
        int32 j;        IN: file id
 RETURNS
        Returns 0 if i=j, -1 if i > j and 1 if i < j
 DESCRIPTION
        Compares two file id's. Used by Red-black tree for file id's.
--------------------------------------------------------------------------- */
PRIVATE int32 
ANIfidcmp(int32 i, int32 j)
{
  if (i == j) 
    return 0;
  if (i > j) 
    return -1; 
  else 
    return 1;
} /* ANIfidcmp */

/* ------------------------------- ANIanncmp -------------------------------- 
 NAME
	ANIanncmp -- compare two annotation keys or id's
 USAGE
	int32 ANIanncmp(i, j)
        int32 i;        IN: annotation key(tag,reg)
        int32 j;        IN: annotation key(tag,reg)
 RETURNS
        Returns 0 if i=j, -1 if i > j and 1 if i < j
 DESCRIPTION
        Compares two annotation keys. Used by Red-black tree for annotations
        Also used to compare annotation id's since also 32 bit value.
--------------------------------------------------------------------------- */
PRIVATE int32 
ANIanncmp(int32 i, int32 j)
{
  if (i == j) 
    return 0;
  if (i > j) 
    return -1; 
  else 
    return 1;
} /* ANIanncmp */

/*--------------------------------------------------------------------------
 NAME
       ANIaddentry -- add entry to corresponding annotation RB tree
 USAGE
       int16  ANIaddentry(file_id, type, ann_ref, elmtag, elmref, new_ann)
       int32  file_id;  IN: file id
       ann_type  type:  IN: AN_DATA_LABEL for data labels, 
                            AN_DATA_DESC for data descriptions,
                            AN_FILE_LABEL for file labels,
                            AN_FILE_DESC for file descritpions.
       uint16 ann_ref: IN: ref of annotation
       uint16 elmtag,: IN: tag of item of which this is annotation
       uint16 elmref;  IN: ref of item of which this is annotation
       intn   new_ann; IN: new annotation flag

 RETURNS
       annotation ID if successful and FAIL (-1) otherwise
 DESCRIPTION
       Adds annotation to correct RB tree
 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
 *------------------------------------------------------------------------*/
PRIVATE int32
ANIaddentry(int32 file_id, ann_type type, uint16 ann_ref, 
            uint16 elmtag, uint16 elmref, intn new_ann)
{
    CONSTR(FUNC, "ANIaddentry");
    int32   ann_id;
    int32   ann_key;
    uint16  ann_tag;
    Rb_node entry;
    ANfile  *file_entry;
    ANentry *ann_entry;
    ANnode  *ann_node;
 
    /* Clear error stack */
    HEclear();

    /* Valid file id */
    if (file_id == FAIL)
        HRETURN_ERROR(DFE_BADCALL, FAIL);

    /* First find file node from file RB tree */
    if ((entry = rb_find_gkey(ANfilelist, file_id, ANIfidcmp)) == NULL)
        HRETURN_ERROR(DFE_BADCALL, FAIL);

    if (entry == ANfilelist)
      HE_REPORT_RETURN("failed to find file_id", FAIL);
    else
      file_entry = (ANfile *) rb_val(entry); /* get file entry from node */

    /* Check for empty annotation tree of 'type'? */   
    if (file_entry->an_num[type] == -1)
      if ((file_entry->an_tree[type] = (Rb_node) mk_rb()) == NULL)
        HE_REPORT_RETURN("failed to create annotation RBtree", FAIL);

    /* Which type of annotation file/data label or desc? */
    switch(type)
      {
        AN_DATA_LABEL:
            ann_tag = DFTAG_DIL;
            break;
        AN_DATA_DESC:
            ann_tag = DFTAG_DIA;
            break;
        AN_FILE_LABEL:
            ann_tag = DFTAG_FID;
            break;
        AN_FILE_DESC:
            ann_tag = DFTAG_FD;
            break;
      default:
            HE_REPORT_RETURN("Bad annotation type for this call",FAIL);
      }

    /* Create 32bit key from type/ref 
    *  ----------------------------
    *  | type(16bits)| ref(16bits) |
    *  -----------------------------*/
    ann_key = AN_CREATE_KEY(type, ann_ref);

    /* Initialize annotation entry */
    if ((ann_entry = HDgetspace(sizeof(ANentry))) == NULL)
        HRETURN_ERROR(DFE_NOSPACE, FAIL);

    ann_entry->annref  = ann_ref;
    ann_entry->ann_id  = ann_id = num_anns++;

    if (type != AN_FILE_LABEL && type != AN_FILE_DESC)
      {
        ann_entry->elmtag = elmtag;
        ann_entry->elmref = elmref;
      }
    else
      {
        ann_entry->elmtag = ann_tag;
        ann_entry->elmref = ann_ref;
      }

    /* Add annotation entry to 'type' RB tree */
    if (rb_insertg(file_entry->an_tree[type], ann_key, ann_entry, ANIanncmp) == NULL)
      HE_REPORT_RETURN("failed to insert annotation into 'type' RBtree", FAIL);

    /* Initialize annotation node */
    if ((ann_node = HDgetspace(sizeof(ANnode))) == NULL)
        HRETURN_ERROR(DFE_NOSPACE, FAIL);

    ann_node->file_id = file_id;
    ann_node->ann_key = ann_key;
    ann_node->new_ann = new_ann;

    /* Add annotation node to ann_id RB tree */
    if (rb_insertg(ANnodelist, ann_id, ann_node, ANIanncmp) == NULL)
      HE_REPORT_RETURN("failed to insert annotation into ann_id RBtree", FAIL);

    /* return annotation id */
    return ann_entry->ann_id;
} /* ANIaddentry */

/*--------------------------------------------------------------------------
 NAME
      ANIcreate_ann_tree --  create an annotation tree of 'type' for given file 
 USAGE
       intn   ANIcreate_ann_tree(file_id, type)
       int32  file_id;   IN: file id
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
ANIcreate_ann_tree(int32 file_id, ann_type type)
{
    CONSTR(FUNC, "ANIcreate_ann_tree");
    uint8   datadi[4];
    int32   more_anns;
    int32   aid;
    int32   nanns, i;
    int32   ann_key; 
    int32   ann_id; 
    uint16  ann_tag;
    uint16  ann_ref;
    uint8   *dptr;
    Rb_node entry;
    ANfile  *file_entry;
    ANentry *ann_entry;
    ANnode  *ann_node;

    /* Clear error stack */
    HEclear();

    /* Valid file id */
    if (file_id == FAIL)
        HRETURN_ERROR(DFE_BADCALL, FAIL);

    /* First find file node to from file RB tree */
    if ((entry = rb_find_gkey(ANfilelist, file_id, ANIfidcmp)) == NULL)
        HRETURN_ERROR(DFE_BADCALL, FAIL);

    if (entry == ANfilelist)
      HE_REPORT_RETURN("failed to find file_id", FAIL);
    else
      file_entry = (ANfile *) rb_val(entry); /* get file entry from node */

    /* Check for empty annotation tree? */   
    if (file_entry->an_num[type] == -1)
      {
        if ((file_entry->an_tree[type] = (Rb_node) mk_rb()) == NULL)
          HE_REPORT_RETURN("failed to creat RBtree for annotation type", FAIL);
      }
    else
      return file_entry->an_num[type];  /* tree already created */

    /* Which type of annotation data/file label or desc? */
    switch(type)
      {
        AN_DATA_LABEL:
            ann_tag = DFTAG_DIL;
            break;
        AN_DATA_DESC:
            ann_tag = DFTAG_DIA;
            break;
        AN_FILE_LABEL:
            ann_tag = DFTAG_FID;
            break;
        AN_FILE_DESC:
            ann_tag = DFTAG_FD;
            break;
      default:
            HE_REPORT_RETURN("Bad annotation type for this call",FAIL);
      }

    /* Get number of annotations of 'type' in file */
    if ((nanns = Hnumber(file_id, ann_tag)) == 0)
      { /* ZERO annotations of 'type' in file */
        file_entry->an_num[type] = 0;
        return  file_entry->an_num[type];  
      }

    /* start read on 'type' annotations  in file */
    if ((aid = Hstartread(file_id, ann_tag, DFREF_WILDCARD)) == FAIL)
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

        /* Allocate & Initialize annotation entry 
        *  decode data tag/ref */
        if ((ann_entry = HDgetspace(sizeof(ANentry))) == NULL)
          HRETURN_ERROR(DFE_NOSPACE, FAIL);

        ann_entry->annref  = ann_ref;
        ann_entry->ann_id  = ann_id = num_anns++;

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
        ann_key = AN_CREATE_KEY(type, ann_ref);

        /* Add annotation entry to file RB tree */
        if (rb_insertg(file_entry->an_tree[type], ann_key, ann_entry, 
                       ANIanncmp) == NULL)
          {
         HE_REPORT_RETURN("failed to insert annotation of 'type' into RBtree"
                          , FAIL);
          }

        /* Initialize annotation node */
        if ((ann_node = HDgetspace(sizeof(ANnode))) == NULL)
          HRETURN_ERROR(DFE_NOSPACE, FAIL);

        ann_node->file_id = file_id;
        ann_node->ann_key = ann_key;
        ann_node->new_ann = 0;       /* not a newly created annotation */

        /* Add annotation node to ann_id RB tree */
        if (rb_insertg(ANnodelist, ann_id, ann_node, ANIanncmp) == NULL)
          HE_REPORT_RETURN("failed to insert annotation into ann_id RBtree", FAIL);
        
        /* set read on next annotation */
        more_anns = Hnextread(aid, ann_tag, DFREF_WILDCARD, DF_CURRENT);
      } /* end for "more_anns" */
    
    /* Finish access*/
    Hendaccess(aid);

    file_entry->an_num[type] = nanns;
    return  file_entry->an_num[type];  
} /* ANIcreate_ann_tree */

/*--------------------------------------------------------------------------
 NAME
     ANIfind -- find annotation id for given annotation type and ref number
 USAGE
       int16  ANIfind(file_id, type, ann_ref)
       int32  file_id;   IN: file id
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
ANIfind(int32 file_id, ann_type type, uint16 ann_ref)
{
    CONSTR(FUNC, "ANIfind");
    int32   ann_key;
    Rb_node entry;
    ANfile  *file_entry = NULL;
    ANentry *ann_entry = NULL;

    /* Clear error stack */
    HEclear();

    /* Valid file id */
    if (file_id == FAIL)
      HRETURN_ERROR(DFE_BADCALL, FAIL);

    /* Find node containing key(file_id) */
    if ((entry = rb_find_gkey(ANfilelist, file_id, ANIfidcmp)) == NULL)
      HRETURN_ERROR(DFE_NOMATCH, FAIL);

    if (entry == ANfilelist)
      HE_REPORT_RETURN("failed to find file_id", FAIL);
    else
      file_entry = (ANfile *) rb_val(entry); /* get file entry from node */

    /* Empty annotation tree */
    if (file_entry->an_num[type] == -1)
      {
        if (ANIcreate_ann_tree(file_id, type) == FAIL)
          HRETURN_ERROR(DFE_BADCALL, FAIL);
      }

    /* Create key from type/ref pair 
    *  ----------------------------
    *  | type(16bits) | ref(16bits) |
    *  -----------------------------*/
    ann_key = AN_CREATE_KEY(type, ann_ref);

    /* See if annotation of 'type' with ref exits */
    if ((entry = rb_find_gkey(file_entry->an_tree[type],ann_key, ANIanncmp)) == NULL)
      HRETURN_ERROR(DFE_NOMATCH, FAIL);

    /* if not found return FAIL 
     * else get annotation entry from node */
    if (entry == file_entry->an_tree[type])
      HE_REPORT_RETURN("failed to find annotation of 'type'", FAIL);
    else
      ann_entry == (ANentry *) rb_val(entry); 

    /* return annotation id */
    return ann_entry->ann_id;
} /* ANIfind */

/*--------------------------------------------------------------------------
 NAME
   ANInumann -- find number of annotation of 'type' that 
                 match the given element tag/ref 
 USAGE
       intn  ANInumann(file_id, type, elm_tag, elem_ref)
       int32  file_id;   IN: file id
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
PRIVATE intn
ANInumann(int32 file_id, ann_type type, uint16 elem_tag, uint16 elem_ref)
{
    CONSTR(FUNC, "ANInumann");
    int32  ann_key;
    intn   nanns = 0;
    Rb_node entry;
    ANfile  *file_entry;
    ANentry *ann_entry;

    /* Clear error stack */
    HEclear();

    /* Valid file id */
    if (file_id == FAIL)
      HRETURN_ERROR(DFE_BADCALL, FAIL);

    /* Find node containing key(file_id) */
    if ((entry = rb_find_gkey(ANfilelist, file_id, ANIfidcmp)) == NULL)
      HRETURN_ERROR(DFE_NOMATCH, FAIL);

    if (entry == ANfilelist)
      HE_REPORT_RETURN("failed to find file_id", FAIL);
    else
      file_entry = (ANfile *) rb_val(entry); /* get file entry from node */

    /* Empty annotation tree */
    if (file_entry->an_num[type] == -1)
      {
        if (ANIcreate_ann_tree(file_id, type) == FAIL)
          HRETURN_ERROR(DFE_BADCALL, FAIL);
      }

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

    /* return number of annotation references found for tag/ref */
    return nanns;
} /* ANInumann */

/*--------------------------------------------------------------------------
 NAME
   ANIannlist -- generate list of annotation ids of 'type' that 
                 match the given element tag/ref 
 USAGE
       intn  ANIannlist(file_id, type, elm_tag, elem_ref)
       int32  file_id;   IN: file id
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
ANIannlist(int32 file_id, ann_type type, uint16 elem_tag, uint16 elem_ref,
           int32 ann_list[])
{
    CONSTR(FUNC, "ANIannlist");
    int32  ann_key;
    intn   nanns = 0;
    Rb_node entry;
    ANfile  *file_entry;
    ANentry *ann_entry;

    /* Clear error stack */
    HEclear();

    /* Valid file id */
    if (file_id == FAIL)
      HRETURN_ERROR(DFE_BADCALL, FAIL);

    /* Find node containing key(file_id) */
    if ((entry = rb_find_gkey(ANfilelist, file_id, ANIfidcmp)) == NULL)
      HRETURN_ERROR(DFE_NOMATCH, FAIL);

    if (entry == ANfilelist)
      HE_REPORT_RETURN("failed to find file_id", FAIL);
    else
      file_entry = (ANfile *) rb_val(entry); /* get file entry from node */

    /* Empty annotation tree */
    if (file_entry->an_num[type] == -1)
      {
        if (ANIcreate_ann_tree(file_id, type) == FAIL)
          HRETURN_ERROR(DFE_BADCALL, FAIL);
      }

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
    CONSTR(FUNC, "ANIannlen");
    Rb_node entry;
    ANfile  *file_entry;
    ANentry *ann_entry;
    ANnode  *ann_node;
    int32  file_id;
    int32  type;
    int32  ann_key;
    uint16 ann_tag;
    uint16 ann_ref;
    int32  ann_length;

    /* Clear error stack */
    HEclear();

    /* Valid annotation id */

    /* See if annotation of 'type' with ref exits */
    if ((entry = rb_find_gkey(ANnodelist,ann_id, ANIanncmp)) == NULL)
      HE_REPORT_RETURN("Failed to find annotation with ann_id", FAIL);

    /* if not found return FAIL 
     * else get annotation node from node */
    if (entry == ANnodelist)
      HE_REPORT_RETURN("Failed to find annotation with an_id", FAIL);
    else
      ann_node = (ANnode *) rb_val(entry);

    /* get file id and annotation key */
    file_id = ann_node->file_id;
    ann_key = ann_node->ann_key;
    type    = AN_KEY2TYPE(ann_key);
    ann_ref = AN_KEY2REF(ann_key);

    /* Valid file id */
    if (file_id == FAIL)
      HE_REPORT_RETURN("bad file_id", FAIL);

   /* set type annotation tag */
    switch(type)
      {
        AN_DATA_LABEL:
            ann_tag = DFTAG_DIL;
            break;
        AN_DATA_DESC:
            ann_tag = DFTAG_DIA;
            break;
        AN_FILE_LABEL:
            ann_tag = DFTAG_FID;
            break;
        AN_FILE_DESC:
            ann_tag = DFTAG_FD;
            break;
      default:
            HE_REPORT_RETURN("Bad annotation type for this call",FAIL);
      }

    if (ann_tag == DFTAG_DIL || ann_tag == DFTAG_DIA)
      {  /* 4=length of data tag/ref for data label or description */
        if ((ann_length = Hlength(file_id, ann_tag, ann_ref) - 4) == FAIL) 
          HE_REPORT_RETURN("Failed to find annotation length", FAIL);
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
       uint8 *ann;     OUT: space to return annotation in
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
ANIreadann(int32 ann_id, uint8 *ann, int32 maxlen)
{
    CONSTR(FUNC, "ANIreadann");
    Rb_node entry;
    ANfile  *file_entry;
    ANentry *ann_entry;
    ANnode  *ann_node;
    int32   file_id;
    int32   type;
    int32   ann_key;
    int32   aid;
    int32   ann_len;
    uint16  ann_tag;
    uint16  ann_ref;
    uint8   datadi[4];      /* to read in and discard data/ref! */

    /* Clear error stack */
    HEclear();

    /* Valid annotation id */

    /* See if annotation of 'type' with ref exits */
    if ((entry = rb_find_gkey(ANnodelist,ann_id, ANIanncmp)) == NULL)
      HE_REPORT_RETURN("Failed to find annotation with ann_id", FAIL);

    /* if not found return FAIL 
     * else get annotation node from node */
    if (entry == ANnodelist)
      HE_REPORT_RETURN("Failed to find annotation with an_id", FAIL);
    else
      ann_node = (ANnode *) rb_val(entry);

    /* get file id and annotation key */
    file_id = ann_node->file_id;
    ann_key = ann_node->ann_key;
    type    = AN_KEY2TYPE(ann_key);
    ann_ref = AN_KEY2REF(ann_key);

    /* Valid file id */
    if (file_id == FAIL)
        HRETURN_ERROR(DFE_BADCALL, FAIL);

   /* set type tag */
    switch(type)
      {
        AN_DATA_LABEL:
            ann_tag = DFTAG_DIL;
            break;
        AN_DATA_DESC:
            ann_tag = DFTAG_DIA;
            break;
        AN_FILE_LABEL:
            ann_tag = DFTAG_FID;
            break;
        AN_FILE_DESC:
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
       uint8 *ann;     IN: annotation to write
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
ANIwriteann(int32 ann_id, uint8 *ann, int32 ann_len)
{
    CONSTR(FUNC, "ANIwriteann");
    Rb_node entry;
    ANfile  *file_entry;
    ANentry *ann_entry;
    ANnode  *ann_node;
    int32   file_id;
    int32   type;
    int32   ann_key;
    int32   aid;
    int     newflag = 0;
    uint16  ann_tag;
    uint16  ann_ref;
    uint16  elem_tag;
    uint16  elem_ref;
    uint8   datadi[4];      /* to hold data tag/ref for writing */
    uint8   *ptr;
    int32   new_ann;

    /* Clear error stack */
    HEclear();

    /* Valid annotation id */

    /* See if annotation of 'type' with ref exits */
    if ((entry = rb_find_gkey(ANnodelist,ann_id, ANIanncmp)) == NULL)
      HE_REPORT_RETURN("Failed to find annotation with ann_id", FAIL);

    /* if not found return FAIL 
     * else get annotation node from node */
    if (entry == file_entry->an_tree[type])
      HE_REPORT_RETURN("Failed to find annotation with an_id", FAIL);
    else
      ann_node = (ANnode *) rb_val(entry);

    /* get file id and annotation key */
    file_id = ann_node->file_id;
    ann_key = ann_node->ann_key;
    type    = AN_KEY2TYPE(ann_key);
    ann_ref = AN_KEY2REF(ann_key);

    /* Valid file id */
    if (file_id == FAIL)
        HRETURN_ERROR(DFE_BADCALL, FAIL);

   /* set type tag */
    switch(type)
      {
        AN_DATA_LABEL:
            ann_tag = DFTAG_DIL;
            break;
        AN_DATA_DESC:
            ann_tag = DFTAG_DIA;
            break;
        AN_FILE_LABEL:
            ann_tag = DFTAG_FID;
            break;
        AN_FILE_DESC:
            ann_tag = DFTAG_FD;
            break;
      default:
            HE_REPORT_RETURN("Bad annotation type for this call",FAIL);
      }

    /* Get annotation entry so that we can extract tag/ref of element 
    * Note that for file labels and descriptions the tag/ref contain
    * DFTAG_XXX and annotation reference number */
    if (rb_find_gkey(file_entry->an_tree[type], ann_key, ann_entry, ANIanncmp) == NULL)
      HE_REPORT_RETURN("failed to retrieve annotation of 'type' RBtree", FAIL);

    /* if not found return FAIL 
     * else get annotation entry from node */
    if (entry == file_entry->an_tree[type])
      HE_REPORT_RETURN("Failed to find annotation with ann_key ", FAIL);
    else
      ann_entry = (ANentry *) rb_val(entry);

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
      }

#if 0
    /* Note: don't need this any more sice ANIcreate takes care of 
     *  adding it to the tree
     * put annotation tag/ref into 'type' RB tree if new */
    if (newflag)
      {
        if (FAIL == ANIaddentry(type, ann_ref, elm_tag, elm_ref, new_flag))
          {
            Hendaccess(aid);
            HE_REPORT_RETURN("Failed to add annotation to RB tree",FAIL);
          }
      }
#endif

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
    int32       ann_id;
    int         newflag = 0;
    uint16      ann_tag;
    uint16      ann_ref;
    uint8       datadi[4];      /* to hold data tag/ref for writing */
    uint8      *ptr;

    /* Clear error stack */
    HEclear();

    /* Valid file id */
    if (file_id == FAIL)
      HRETURN_ERROR(DFE_BADCALL, FAIL);

#if 0 
    /* NOTE: Don't need this since for now since we creat always 
     *  creates a new annotation 
     * check if this tag/ref already has this type of annotation */
    if ((ann_ref = ANIfind(file_id, type, elem_tag, elem_ref)) == FAIL)
      HE_REPORT_RETURN("Failed to find annotation",FAIL);


    if (ann_ref == 0)
      { /* new annotation */
        ann_ref = Hnewref(file_id);
        if (ann_ref == 0)
          HE_REPORT_RETURN("Failed create new ref for annotation",FAIL);
        newflag = 1;  /* remember to add ann tag/ref to directory */
      }

    /* put annotation tag/ref into 'type' RB tree if new */
    if (newflag)
      {
        if ((ann_id = ANIaddentry(type, ann_ref, elm_tag, elm_ref)) == FAIL)
            HE_REPORT_RETURN("Failed to add annotation to RB tree",FAIL);
      }
#endif

    /* new annotation */
    ann_ref = Hnewref(file_id);
    if (ann_ref == 0)
      HE_REPORT_RETURN("Failed create new ref for annotation",FAIL);

   /* deal with type */
    switch(type)
      {
        AN_DATA_LABEL:
            ann_tag = DFTAG_DIL;
            break;
        AN_DATA_DESC:
            ann_tag = DFTAG_DIA;
            break;
        AN_FILE_LABEL:
            /* for file label set elmement tag/ref to ann_tag & ref */
            ann_tag = DFTAG_FID;
            elem_tag = ann_tag;
            elem_ref = ann_ref;
            break;
        AN_FILE_DESC:
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
	int32 ANstart(path, HDFmode)
        char * filename;        IN: file name to open
        int32  acc_mode;        IN: access mode to open file with
 RETURNS
        A file ID or FAIL
 DESCRIPTION
        Open a file by calling Hopen() and return a  file ID to the file.
--------------------------------------------------------------------------- */
EXPORT int32
ANstart(const char *filename, int32 acc_mode)
{
    CONSTR(FUNC, "ANstart");
    int32  file_id;
    ANfile *file_entry;

#ifdef ANDEBUG
    fprintf(stderr, "ANstart: I've been called\n");
#endif
    /* Clear error stack */
    HEclear();

    /* Check to see if we need to call ANInit, needs to moved to 
     * overall initialization call */
    if (ANfilelist == NULL || ANnodelist == NULL)
      ANinit();

    /* Open file */
     file_id = Hopen(filename, acc_mode, 0);
     if (file_id == FAIL)
         return FAIL;

    /* Intialize file entry */
    if ((file_entry = HDgetspace(sizeof(ANfile))) == NULL)
        HRETURN_ERROR(DFE_NOSPACE, FAIL);
    file_entry->access_mode = acc_mode;
    file_entry->an_tree[AN_DATA_LABEL] = NULL;   /* data label ann tree */
    file_entry->an_tree[AN_DATA_DESC]  = NULL;   /* data description ann tree */
    file_entry->an_tree[AN_FILE_LABEL] = NULL;   /* file label ann tree */
    file_entry->an_tree[AN_FILE_DESC]  = NULL;   /* file description ann tree */
    file_entry->an_num[AN_DATA_LABEL] = -1;   
    file_entry->an_num[AN_DATA_DESC]  = -1;   
    file_entry->an_num[AN_FILE_LABEL] = -1;   
    file_entry->an_num[AN_FILE_DESC]  = -1;   
    if ((file_entry->filename = HDgetspace(strlen(filename)+1)) == NULL)
        HRETURN_ERROR(DFE_NOSPACE, FAIL);
    HDstrcpy(file_entry->filename,filename);

    /* Add file entry to file RBtree */
    if (rb_insertg(ANfilelist, file_id, file_entry, ANIfidcmp) == NULL)
      HE_REPORT_RETURN("Failed to insert file_id into RBtree", FAIL);

    /* return file id */
    return file_id;
} /* ANstart() */

/*--------------------------------------------------------------------------
 NAME
    ANfileinfo
 PURPOSE
    Report high-level information about the ANxxx interface for a given file.
 USAGE
    intn ANfileinfo(file_id, n_file_label, n_file_desc, n_obj_label, n_obj_desc)
        int32 file_id;        IN:  file ID to get information about
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
ANfileinfo(int32 file_id, int32 *n_file_label, int32 *n_file_desc,
           int32 *n_obj_label, int32 *n_obj_desc)
{
    CONSTR(FUNC, "ANfileinfo");    /* for HERROR */
    uint32  ann_key;
    Rb_node entry;
    ANfile  *file_entry;
    ANentry *ann_entry;

    /* Clear error stack */
    HEclear();

    /* Valid file id */
    if (file_id == FAIL)
        HRETURN_ERROR(DFE_BADCALL, FAIL);

    /* Find node containing key(file_id) */
    if ((entry = rb_find_gkey(ANfilelist, file_id, ANIfidcmp)) == NULL)
        HRETURN_ERROR(DFE_NOMATCH, FAIL);
    if (entry == ANfilelist)
      HE_REPORT_RETURN("failed to find file_id", FAIL);
    else
      file_entry = (ANfile *) rb_val(entry); /* get file entry from node */

    /* Empty file label annotation tree? */
    if (file_entry->an_num[AN_FILE_LABEL] == -1)
      {
        if ((*n_file_label = ANIcreate_ann_tree(file_id, AN_FILE_LABEL)) == FAIL)
          HE_REPORT_RETURN("failed to create file label annotation RBtree", FAIL);
      }
    else
      *n_file_label = file_entry->an_num[AN_FILE_LABEL];

    /* Empty file descritpiton annotation tree? */
    if (file_entry->an_num[AN_FILE_DESC] == -1)
      {
        if ((*n_file_desc = ANIcreate_ann_tree(file_id, AN_FILE_DESC)) == FAIL)
          HE_REPORT_RETURN("failed to create file desc annotation RBtree", FAIL);
      }
    else
      *n_file_desc = file_entry->an_num[AN_FILE_DESC];

    /* Empty label annotation tree? */
    if (file_entry->an_num[AN_DATA_LABEL] == -1)
      {
        if ((*n_obj_label = ANIcreate_ann_tree(file_id, AN_DATA_LABEL)) == FAIL)
          HE_REPORT_RETURN("failed to create data label annotation RBtree", FAIL);
      }
    else
      *n_obj_label = file_entry->an_num[AN_DATA_LABEL];

    /* Empty descritpiton annotation tree? */
    if (file_entry->an_num[AN_DATA_DESC] == -1)
      {
        if ((*n_obj_desc = ANIcreate_ann_tree(file_id, AN_DATA_DESC)) == FAIL)
          HE_REPORT_RETURN("failed to create data desc annotation RBtree", FAIL);
      }
    else
      *n_obj_desc = file_entry->an_num[AN_DATA_DESC];

    return SUCCEED;
} /* ANfileinfo() */


/* -------------------------------- ANend ---------------------------------
 NAME
	ANend -- close a file
 USAGE
	int32 ANend(file_id)
        int32 file_id;               IN: file ID of file to close
 RETURNS
        SUCCEED / FAIL
 DESCRIPTION
        Close the file

--------------------------------------------------------------------------- */
EXPORT int32
ANend(int32 file_id)
{
  CONSTR(FUNC,"ANend");
  int32   ret;
  Rb_node entry;
  ANfile  *file_entry;

#ifdef ANDEBUG
    fprintf(stderr, "ANend: I've been called\n");
#endif

    /* Clear error stack */
    HEclear();

    /* Valid file id */
    if (file_id == FAIL)
        HRETURN_ERROR(DFE_BADCALL, FAIL);

    /* First find file node to delete from file RB tree */
    if ((entry = rb_find_gkey(ANfilelist, file_id, ANIfidcmp)) == NULL)
        HRETURN_ERROR(DFE_BADCALL, FAIL);

    if (entry == ANfilelist)
      HE_REPORT_RETURN("Failed to find file_id", FAIL);
    else
      file_entry = (ANfile *) rb_val(entry); /* get file entry from node */

    if (file_entry->filename != NULL)
      HDfreespace(file_entry->filename);  /* free file name */

   /* need to delete RB trees of annotations attached to node */
   /* free file label annotation rb tree */
    if (file_entry->an_tree[AN_FILE_LABEL] != NULL)
      rb_free_tree(file_entry->an_tree[AN_FILE_LABEL]);  

    /* free file desc annotation rb tree */
    if (file_entry->an_tree[AN_FILE_DESC] != NULL)
      rb_free_tree(file_entry->an_tree[AN_FILE_DESC]); 

    /* free label annotation rb tree */
    if (file_entry->an_tree[AN_DATA_LABEL] != NULL)
      rb_free_tree(file_entry->an_tree[AN_DATA_LABEL]);  

    /* free desc annotation rb tree */
    if (file_entry->an_tree[AN_DATA_DESC] != NULL)
      rb_free_tree(file_entry->an_tree[AN_DATA_DESC]); 
    HDfreespace(file_entry);             /* free file entry */

    /* Now we can delete the node itself */
    if (rb_delete_node(entry) == -1)
        HRETURN_ERROR(DFE_BADCALL, FAIL);

    /* Close file and set return value */
    ret = Hclose(file_id);

    return ret;
} /* ANend() */

/* ------------------------------------------------------------------------ 
 NAME
	ANcreate - create a new element annotation and return a handle(id)
 USAGE
	int32 ANcreate(int32 file_id, uint16 tag, uint16 ref, char *ann, 
                      int32 ann_len, intn type )
        int32 file_id;  IN: file ID
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
ANcreate(int32 file_id, uint16 elem_tag, uint16 elem_ref, ann_type type)
{
    CONSTR(FUNC, "ANcreate");    /* for HERROR */

    return (ANIcreate(file_id, elem_tag, elem_ref, type));
} /* ANcreate() */

/* ------------------------------------------------------------------------ 
 NAME
	ANcreatef - create a new file annotation and return a handle(id)
 USAGE
	int32 ANcreatef(int32 file_id,ann_type type )
        int32 file_id;  IN: file ID
        ann_type  type: IN:  AN_FILE_LABEL for file labels,
                             AN_FILE_DESC for file descritpions.
 RETURNS
        An ID to an annotation which can either be a file label or description
 DESCRIPTION
        Creates a file annotation, returns an 'an_id' to work with the new 
        file annotation which can either be a label or description.
        
--------------------------------------------------------------------------- */
EXPORT int32
ANcreatef(int32 file_id, ann_type type)
{
    CONSTR(FUNC, "ANcreatef");    /* for HERROR */
    uint16 ann_tag;
    uint16 ann_ref;

   /* deal with type */
    switch(type)
      {
        AN_FILE_LABEL:
            ann_tag = DFTAG_FID;
            ann_ref = 0; /* fake ref, will be replaced in ANIcreate() */
            break;
        AN_FILE_DESC:
            ann_tag = DFTAG_FD;
            ann_ref = 0; /* fake ref, will be replaced in ANIcreate() */
            break;
      default:
            HE_REPORT_RETURN("Bad annotation type for this call",FAIL);
      }

    return (ANIcreate(file_id, ann_tag, ann_ref, type));
} /* ANcreateann() */

/* ------------------------------- ANselect ------------------------------- 
 NAME
	ANselect -- get an annotation ID from index of 'type'
 USAGE
	int32 ANselect(file_id, index)
        int32 file_id;   IN: file ID
        int32 index;     IN: index of annottion to get ID for
        ann_type  type: IN: AN_DATA_LABEL for data labels, 
                            AN_DATA_DESC for data descriptions,
                            AN_FILE_LABEL for file labels,
                            AN_FILE_DESC for file descritpions.
 RETURNS
        An ID to an annotation type which can either be a label or description 
 DESCRIPTION
        The position index is zero based
--------------------------------------------------------------------------- */
EXPORT int32
ANselect(int32 file_id, int32 index, ann_type type)
{
    CONSTR(FUNC, "ANselect");    /* for HERROR */
    int32   ann_key;
    intn    i;
    Rb_node entry;
    ANfile  *file_entry;
    ANentry *ann_entry;

    /* Clear error stack */
    HEclear();

    /* Valid file id */
    if (file_id == FAIL)
      HRETURN_ERROR(DFE_BADCALL, FAIL);

    /* Find node containing key(file_id) */
    if ((entry = rb_find_gkey(ANfilelist, file_id, ANIfidcmp)) == NULL)
      HRETURN_ERROR(DFE_NOMATCH, FAIL);

    if (entry == ANfilelist)
      HE_REPORT_RETURN("failed to find file_id", FAIL);
    else
      file_entry = (ANfile *) rb_val(entry); /* get file entry from node */

    /* Empty annotation tree */
    if (file_entry->an_num[type] == -1)
      {
        if (ANIcreate_ann_tree(file_id, type) == FAIL)
          HRETURN_ERROR(DFE_BADCALL, FAIL);
      }

    /* Traverse the list till 'index' */
    for(entry = rb_first(file_entry->an_tree[type]), i = 0; 
        entry != nil(file_entry->an_tree[type]) && i != index; 
        entry = rb_next(entry), i++)

    if (entry == file_entry->an_tree[type])
      HE_REPORT_RETURN("failed to find annoation of 'type' with 'index'", FAIL);
    else
      ann_entry = (ANentry *) rb_val(entry); 

    /* return ann_id */
    return ann_entry->ann_id;
} /* ANselect() */

/*--------------------------------------------------------------------------
 NAME
   ANnumann -- find number of annotation of 'type' that 
                 match the given element tag/ref 
 USAGE
       intn  ANnumann(file_id, type, elm_tag, elem_ref)
       int32  file_id;   IN: file id
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
ANnumann(int32 file_id, ann_type type, uint16 elem_tag, uint16 elem_ref)
{
    CONSTR(FUNC, "ANnumann");
    return ANInumann(file_id, type, elem_tag, elem_ref);
} /* ANnumann() */

/*--------------------------------------------------------------------------
 NAME
   ANannlist -- generate list of annotation ids of 'type' that 
                 match the given element tag/ref 
 USAGE
       intn  ANannlist(file_id, type, elm_tag, elem_ref)
       int32  file_id;   IN: file id
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
ANannlist(int32 file_id, ann_type type, uint16 elem_tag, uint16 elem_ref,
           int32 ann_list[])
{
    CONSTR(FUNC, "ANannlist");
    return ANIannlist(file_id, type, elem_tag, elem_ref, ann_list);
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
    CONSTR(FUNC, "ANannlen");
    return ANIannlen(ann_id);
} /* ANannlen() */

/*--------------------------------------------------------------------------
 NAME
       ANwriteann -- write annotation given ann_id
 USAGE
       intn ANwriteann(ann_id, ann, ann_len)
       char *ann_id;   IN: annotation id
       uint8 *ann;     IN: annotation to write
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
ANwriteann(int32 ann_id, uint8 *ann, int32 annlen)
{
    CONSTR(FUNC, "ANwriteann");    /* for HERROR */

    return ANwriteann(ann_id, ann, annlen);
} /* ANwriteann() */

/*--------------------------------------------------------------------------
 NAME
       ANreadann -- read annotation given ann_id
 USAGE
       intn ANreadann(ann_id, ann, maxlen)
       int32 ann_id;   IN: annotation id (handle)
       uint8 *ann;     OUT: space to return annotation in
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
ANreadann(int32 ann_id, uint8 *ann, int32 maxlen)
{
    CONSTR(FUNC, "ANreadann");    /* for HERROR */

    return ANIreadann(ann_id, ann, maxlen);
} /* ANreadann() */

/* ----------------------------------------------------------------------- 
 NAME
	ANendaccess -- end access to an annotation given it's id
 USAGE
	intn ANendaccess(ann_id)
        int32 an_id;    IN: annotation id
 RETURNS
        SUCCEED or FAIL
 DESCRIPTION
        Terminates access to an annotation. For now does nothing
--------------------------------------------------------------------------- */
EXPORT intn
ANendaccess(int32 an_id)
{
    CONSTR(FUNC, "ANendaccess");    /* for HERROR */

} /* ANendaccess() */

#endif /* MFAN_C */

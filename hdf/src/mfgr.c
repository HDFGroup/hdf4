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
static char RcsId[] = "$Revision$";
#endif

/* $Id$ */

/*
FILE
    mfgr.c

PURPOSE
    Multi-file access to "generic" raster images

REMARKS
    These functions perform manipulate "generic" raster images.  These
    raster images are composed of "pixels" which can have multiple
    components of data-types other than 8-bit unsigned integers.  Each
    image can have multiple palettes associated with it and other 'attributes'
    in the same "name=value" style as the SD*() routines have.

DESIGN
        These routines are modeled loosely after the previous single-file
    DFGR*() routines and the current SD*() routines.  There is a table of
    pointers to sets of raster images stored in the files which have been
    initialized, similar to the current Vgroup/Vdata implementation.  The table
    contains entries for each file which contain pointers to set of information
    about the "global" attributes (for this interface) and to the set of raster
    images in the file.
        Each set of information for each file is stored in memory using
    the tbbt*() routines.
        Each raster image (RI) in the file will be stored in one Vgroup which
    will contain all the RIs created as well as the "global" raster attributes.
        Each RI in the file will be stored as a Vgroup containing the image data
    for each RI as well as the palettes and dataset attributes for that image.
    Also, each RI Vgroup will have a corresponding RIG created with it and
    maintained in parallel, for backward compatibility.

BUGS/LIMITATIONS
   Currently the following design limitations are still in place:
   1 - Cannot have pixels or palette entries which contain mixed variable
        types, ie. all the pixel/palette components must be of the same
        number type.
   2 - Currently all the components must be of valid HDF number types,
        fractional bytes (ie. 6-bit components) or 'plain' multiple byte values
        are not handled, although they can be packed into the next larger
        sized number type in order to hold them.

EXPORTED ROUTINES

File/Interface Functions:
int32 GRstart(int32 hdf_file_id)
    - Initializes the GR interface for a particular file. Returns a 'grid' to
        specify the GR group to operate on.
intn GRfileinfo(int32 grid, int32 *n_datasets, int32 *n_attrs)
    - Returns information about the datasets and "global" attributes for the
        GR interface.
intn GRend(int32 grid)
    - Terminates multi-file GR access for a file.

Image I/O Functions:
int32 GRcreate(int32 grid,char *name,int32 ncomp,int32 nt,int32 il,int32 dimsizes[2])
    - Defines a raster image in a file.  Returns a 'riid' to work with the new
        raster image.
int32 GRselect(int32 grid,int32 index)
    - Selects an existing RI to operate on.
int32 GRnametoindex(int32 grid,char *name)
    - Maps a RI name to an index which is returned.
intn GRgetiminfo(int32 riid,char *name,int32 *ncomp,int32 *nt,int32 *il,int32 dimsizes[2],int32 *n_attr)
    - Gets information about an RI which has been selected/created.
intn GRwriteimage(int32 riid,int32 start[2],int32 stride[2],int32 count[2],VOIDP data)
    - Writes image data to an RI.  Partial dataset writing and subsampling is
        allowed, but only with the dimensions of the dataset (ie. no UNLIMITED
        dimension support)
intn GRreadimage(int32 riid,int32 start[2],int32 stride[2],int32 count[2],VOIDP data)
    - Read image data from an RI.  Partial reads and subsampling are allowed.
intn GRendaccess(int32 riid)
    - End access to an RI.

Dimension Functions:
int32 GRgetdimid(int32 riid,int32 index)
    - Get a dimension id ('dimid') for an RI to assign atrributes to. [Later]
intn GRsetdimname(int32 dimid,char *name)
    - Set the name of a dimension. [Later]
int32 GRdiminfo(int32 dimid,char *name,int32 *size,int32 *n_attr)
    - Get information about the dimensions attributes and size. [Later]

ID/Ref/Index Functions:
uint16 GRidtoref(int32 riid)
    - Maps an riid to a reference # for annotating or including in a Vgroup.
int32 GRreftoindex(int32 hdf_file_id,uint16 ref)
    - Maps the reference # of an RI into an index which can be used with
        GRselect.

Interlace Request Functions:
intn GRreqlutil(int32 riid,intn il)
    - Request that the next LUT read from an RI have a particular interlace.
intn GRreqimageil(int32 riid,intn il)
    - Request that the image read from an RI have a particular interlace.

LUT/Palette I/O Functions:
int32 GRgetlutid(int32 riid,int32 index)
    - Get a palette id ('palid') for an RI.
intn GRgetlutinfo(int32 lutid,int32 *ncomp,int32 *nt,int32 *il,int32 *nentries)
    - Gets information about a palette.
intn GRwritelut(int32 lutid,int32 ncomps,int32 nt,int32 il,int32 nentries,VOIDP data)
    - Writes out a palette for an RI.
intn GRreadlut(int32 palid,VOIDP data)
    - Reads a palette from an RI.

Special Element Functions:
int32 GRsetexternalfile(int32 riid,char *filename,int32 offset)
    - Makes the image data of an RI into an external element special element.
intn GRsetaccesstype(int32 riid,uintn accesstype)
    - Sets the access for an RI to be either serial or parallel I/O.
intn GRsetcompress(int32 riid,int32 comp_type,comp_info *cinfo)
    - Makes the image data of an RI into a compressed special element.

Attribute Functions:
intn GRsetattr(int32 dimid|riid|grid,char *name,int32 attr_nt,int32 count,VOIDP data)
    - Write an attribute for an object.
int32 GRattrinfo(int32 dimid|riid|grid,int32 index,char *name,int32 *attr_nt,int32 *count)
    - Get attribute information for an object.
intn GRgetattr(int32 dimid|riid|grid,int32 index,VOIDP data)
    - Read an attribute for an object.
int32 GRfindattr(int32 dimid|riid|grid,char *name)
    - Get the index of an attribute with a given name for an object.

LOCAL ROUTINES
intn GRIil_convert(const VOIDP inbuf,gr_interlace_t inil,VOIDP outbuf,
        gr_interlace_t outil,int32 dims[2],int32 ncomp,int32 nt);
    - Copy a pixel buffer from one interlace to another.

AUTHOR
   Quincey Koziol

MODIFICATION HISTORY
   10/20/95  - Starting writing specs & coding prototype
    3/ 8/96  - Modifications to remove compiled limits on the # of files
 */

#define MFGR_MASTER
#include "mfgr.h"       /* Multi-file raster information */

/* Local pre-processor macros */
#define XDIM    0
#define YDIM    1

/*
   * --------------------------------------------------------------------
   * PRIVATE  data structure and routines.
   * --------------------------------------------------------------------
 */
static TBBT_TREE *gr_tree=NULL;

PRIVATE intn GRIupdatemeta(int32 hdf_file_id,ri_info_t *img_ptr);

PRIVATE intn GRIupdateRIG(int32 hdf_file_id,ri_info_t *img_ptr);

PRIVATE intn GRIupdateRI(int32 hdf_file_id,ri_info_t *img_ptr);

PRIVATE intn GRIup_attr_data(int32 hdf_file_id,at_info_t *attr_ptr);

/*--------------------------------------------------------------------------
 NAME
    rigcompare
 PURPOSE
    Compare two B-tree keys for equality.
 USAGE
    intn rigcompare(k1, k2, cmparg)
        VOIDP k1, k2;               IN: ptrs to the keys for the tree elements
        intn cmparg;                IN/OUT: unused
 RETURNS
    Returns negative for k2>k1, positive for k2<k1 and 0 for k2=k1
 DESCRIPTION
    Similar to memcmp function, this routine just compares tree keys for
    greater/equal/lesser status.
 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS

  *** Only called by B-tree routines, should _not_ be called externally ***

 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
intn rigcompare(VOIDP k1, VOIDP k2, intn cmparg)
{
    /* shut compiler up */
    cmparg = cmparg;

    return ((intn) ((*(int32 *) k1) - (*(int32 *) k2)));    /* valid for integer keys */
}   /* rigcompare */

/* ---------------------------- GRIgrdestroynode ------------------------- */
/*
   Frees B-Tree gr_info_t nodes

   *** Only called by B-tree routines, should _not_ be called externally ***
 */
VOID
GRIgrdestroynode(VOIDP n)
{
    gr_info_t      *gr_ptr=(gr_info_t *)n;

    /* clear out the tbbt's */
    tbbtdfree(gr_ptr->grtree, GRIridestroynode, NULL);
    tbbtdfree(gr_ptr->gattree, GRIattrdestroynode, NULL);

    HDfree(gr_ptr);
}   /* GRIgrdestroynode */

/*--------------------------------------------------------------------------
 NAME
    GRIattrdestroynode
 PURPOSE
    Frees B-Tree attribute nodes.
 USAGE
    VOID GRIattrdestroynode(n)
        VOIDP n;               IN: ptr to the attr node to delete
 RETURNS
    none
 DESCRIPTION
    Called internally by the tbbt*() routines, this routine is used when
    deleting trees.
 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS

  *** Only called by B-tree routines, should _not_ be called externally ***

 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
VOID GRIattrdestroynode(VOIDP n)
{
    at_info_t *at_ptr=(at_info_t *)n;

    if(at_ptr->name!=NULL)
        HDfree(at_ptr->name);
    if(at_ptr->data!=NULL)
        HDfree(at_ptr->data);

    HDfree(at_ptr);
}   /* GRIattrdestroynode */
       
/*--------------------------------------------------------------------------
 NAME
    GRIridestroynode
 PURPOSE
    Frees B-Tree raster-image nodes.
 USAGE
    VOID GRIridestroynode(n)
        VOIDP n;               IN: ptr to the attr node to delete
 RETURNS
    none
 DESCRIPTION
    Called internally by the tbbt*() routines, this routine is used when
    deleting trees.
 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS

  *** Only called by B-tree routines, should _not_ be called externally ***

 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
VOID GRIridestroynode(VOIDP n)
{
    ri_info_t *ri_ptr=(ri_info_t *)n;

    if(ri_ptr->name!=NULL)
        HDfree(ri_ptr->name);
    if(ri_ptr->ext_name!=NULL)
        HDfree(ri_ptr->ext_name);
    tbbtdfree(ri_ptr->lattree, GRIattrdestroynode, NULL);
    if(ri_ptr->fill_value!=NULL)
        HDfree(ri_ptr->fill_value);

    HDfree(ri_ptr);
}   /* GRIridestroynode */
       
/* -------------------------- Get_grfile ------------------------ */
/*
   Looks in the TBBT gr_tree for the file ID of a file.
   Returns a pointer to the gr_info_t for that file on success, otherwise NULL.
 */
PRIVATE gr_info_t *
Get_grfile(HFILEID f)
{
    VOIDP *t;       /* vfile_t pointer from tree */
    int32 key=(int32)f;

    t = (VOIDP *) tbbtdfind(gr_tree, (VOIDP) &key, NULL);
    return((gr_info_t *)(t==NULL ? NULL : *t));
} /* end Get_grfile() */

        
/* -------------------------- New_grfile ------------------------ */
/*
   Creates gr_info_t structure and adds it to the tree
   Returns a pointer to the gr_info_t for that file on success, otherwise NULL.
 */
PRIVATE gr_info_t *
New_grfile(HFILEID f)
{
    gr_info_t *g;
    
    /* Allocate the gr_info_t structure */
    if (NULL == (g = (gr_info_t *) HDcalloc(1,sizeof(gr_info_t))))
      return(NULL);

    /* Assign the file ID & insert into the tree */
    g->hdf_file_id=f;
    tbbtdins(gr_tree, (VOIDP) g, NULL);    /* insert the vg instance in B-tree */

    return(g);
} /* end New_grfile() */

/*--------------------------------------------------------------------------
 NAME
    GRIget_image_list
 PURPOSE
    Find all the unique raster images in the file
 USAGE
    intn GRIget_image_list(hdf_file_id,gr_info)
        int32 hdf_file_id;          IN: file ID from Hopen
        gr_info_t *gr_info;         IN: # of unique images found
 RETURNS
    Return SUCCEED/FAIL
 DESCRIPTION
    Sifts through all the images in a file and compiles a list of all the
    unique ones.
 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
   o Count the maximum number of possible raster images in the file
   o Search through the file for raster images
      - Look for RI8s
      - Look for RIGs
      - Look for an RI Vgroup, then read in RIs & "global" attributes.
   o Eliminate duplicate images
   o Throw all the remaining RI8s, RIGs, and RIs into an internal table with
      appropriate information about each of them
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
static intn GRIget_image_list(int32 file_id,gr_info_t *gr_ptr)
{
    CONSTR(FUNC, "GRIget_image_list");
    uint16      gr_ref;         /* ref # of the Vgroup containing new-style RIs */
    intn        curr_image;     /* current image gathering information about */
    intn        nimages;        /* total number of potential images */
    int32       nri, nci, nri8, nci8, nvg;   /* number of RIs, CIs, RI8s & CI8s & Vgroups */
    struct image_info {
        uint16 grp_tag,grp_ref;         /* tag/ref of the group the image is in */
        uint16 img_tag,img_ref;         /* tag/ref of the image itself */
        uint16 aux_ref;                 /* ref of aux. info about an image */
        int32 offset;                   /* offset of the image data */
    } *img_info;
    uint16      find_tag, find_ref;     /* storage for tag/ref pairs found */
    int32       find_off, find_len;     /* storage for offset/lengths of tag/refs found */
    intn        i, j;           /* local counting variable */
    intn        ret_value = SUCCEED;

    HEclear();

    /* In a completely psychotic file, there could be RIGs with no corresponding
       RI8s and also RI8s with no corresponding RIGs, so assume the worst
       case and then run through them all to eliminate matched pairs */
    nri = Hnumber(file_id, DFTAG_RI);     /* count the number of RI and CIs */
    if (nri == FAIL)
        HGOTO_ERROR(DFE_INTERNAL, FAIL);
    nci = Hnumber(file_id, DFTAG_CI);     /* count the number of RI and CIs */
    if (nci == FAIL)
        HGOTO_ERROR(DFE_INTERNAL, FAIL);
    nri8 = Hnumber(file_id, DFTAG_RI8);     /* add the number of RI8 and CI8s */
    if (nri8 == FAIL)
        HGOTO_ERROR(DFE_INTERNAL, FAIL);
    nci8 = Hnumber(file_id, DFTAG_CI8);
    if (nci8 == FAIL)
        HGOTO_ERROR(DFE_INTERNAL, FAIL);
    nvg = Hnumber(file_id, RI_TAG);
    if (nvg == FAIL)
        HGOTO_ERROR(DFE_INTERNAL, FAIL);
    nimages = (intn) (nri + nci + nri8 + nci8 + nvg);

#ifdef QAK
printf("%s: nri=%ld, nci=%ld, nri8=%ld, nci8=%ld, nvg=%ld\n",FUNC,(long)nri,(long)nci,(long)nri8,(long)nci8,(long)nvg);
#endif /* QAK */
    /* if there are no images just close the file and get out */
    if (nimages == 0)
      {
        ret_value = (SUCCEED);
        goto done;
      }

    /* Get space to store the image offsets */
    if ((img_info = (struct image_info *) HDmalloc(nimages * sizeof(struct image_info))) == NULL)
        HGOTO_ERROR(DFE_NOSPACE, FAIL);
    HDmemset(img_info,0,nimages*sizeof(struct image_info));    

    /* search through the GR group for raster images & global attributes */
    curr_image = 0;
    if((gr_ref=(uint16)Vfind(file_id,GR_NAME))!=0)
      {
          int32       gr_key;         /* Vgroup key of the GR Vgroup */

          gr_ptr->gr_ref=gr_ref; /* squirrel this away for later use */
          if((gr_key=Vattach(file_id,(int32)gr_ref,"r"))!=FAIL)
            {
                int32 nobjs=Vntagrefs(gr_key); /* The number of objects in the Vgroup */
                int32 img_key;  /* Vgroup key of an image */
                int32 grp_tag,grp_ref;  /* a tag/ref in the Vgroup */
                int32 img_tag,img_ref;  /* image tag/ref in the Vgroup */
                char textbuf[VGNAMELENMAX + 1];    /* buffer to store the name in */
                
                for(i=0; i<nobjs; i++)
                  {
                      if(Vgettagref(gr_key,i,&grp_tag,&grp_ref)==FAIL)
                          continue;
                      
                      switch(grp_tag)
                        {
                            case DFTAG_VG:  /* should be an image */
                                if((img_key=Vattach(file_id,grp_ref,"r"))!=FAIL)
                                  {
                                    if(Vgetclass(img_key,textbuf)!=FAIL)
                                      {
                                        if(!HDstrcmp(textbuf,RI_NAME))
                                          { /* found an image, whew! */
                                            for(j=0; j<Vntagrefs(img_key); j++)
                                              {
                                                  if(Vgettagref(img_key,j,&img_tag,&img_ref)==FAIL)
                                                      continue;
                                                  if(img_tag==DFTAG_RI)
                                                    {
                                                        img_info[curr_image].grp_tag=(uint16)grp_tag;
                                                        img_info[curr_image].grp_ref=(uint16)grp_ref;
                                                        img_info[curr_image].img_tag=(uint16)img_tag;
                                                        img_info[curr_image].img_ref=(uint16)img_ref;
                                                        img_info[curr_image].offset = Hoffset(file_id, (uint16)img_tag, (uint16)img_ref);     /* store offset */
                                                        curr_image++;
                                                        break;
                                                    } /* end if */
                                              } /* end for */
                                          } /* end if */
                                      } /* end if */
                                    Vdetach(img_key);
                                  } /* end if */
                                break;

                            case DFTAG_VH:  /* must be a "global" attaribute */
                                  {
                                      at_info_t *new_attr;  /* attribute to add to the set of local attributes */
                                      int32 at_key;         /* VData key for the attribute */

                                      if((new_attr=(at_info_t *)HDmalloc(sizeof(at_info_t)))==NULL)
                                          HGOTO_ERROR(DFE_NOSPACE,FAIL);
                                      new_attr->ref=(uint16)grp_ref;
                                      new_attr->index=gr_ptr->gattr_count;
                                      new_attr->data_modified=FALSE;
                                      new_attr->new_at=FALSE;
                                      new_attr->data=NULL;
                                      if((at_key=VSattach(file_id,(int32)grp_ref,"r"))!=FAIL)
                                        {
                                            char *fname;

                                            /* Make certain the attribute only has one field */
                                            if(VFnfields(at_key)!=1)
                                              {
                                                VSdetach(at_key);
                                                HDfree(new_attr);
                                                break;
                                              } /* end if */
                                            new_attr->nt=VFfieldtype(at_key,0);
                                            new_attr->len=VFfieldorder(at_key,0);
                                            if(new_attr->len==1)
                                                new_attr->len=VSelts(at_key);

                                            /* Get the name of the attribute */
                                            if((fname=VFfieldname(at_key,0))==NULL)
                                              {
                                                sprintf(textbuf,"Attribute #%d",(int)new_attr->index);
                                                if((new_attr->name=(char *)HDmalloc(HDstrlen(textbuf)+1))==NULL)
                                                  {
                                                    VSdetach(at_key);
                                                    HDfree(new_attr);
                                                    HGOTO_ERROR(DFE_NOSPACE,FAIL);
                                                  } /* end if */
                                                HDstrcpy(new_attr->name,textbuf);
                                              } /* end if */
                                            else
                                              {
                                                if((new_attr->name=(char *)HDmalloc(HDstrlen(fname)+1))==NULL)
                                                  {
                                                    VSdetach(at_key);
                                                    HDfree(new_attr);
                                                    HGOTO_ERROR(DFE_NOSPACE,FAIL);
                                                  } /* end if */
                                                HDstrcpy(new_attr->name,fname);
                                              } /* end else */
                                                
                                            tbbtdins(gr_ptr->gattree, (VOIDP) new_attr, NULL);    /* insert the attr instance in B-tree */ 

                                            VSdetach(at_key);
                                        } /* end if */

                                      gr_ptr->gattr_count++;
                                  } /* end case */
                                break;

                            default:
                                break;
                        } /* end switch */
                  } /* end for */
              Vdetach(gr_key);
            } /* end if */
      } /* end if */

    /* Get information about the RIGs in the file */
    find_tag = find_ref = 0;
    while (Hfind(file_id, DFTAG_RIG, DFREF_WILDCARD, &find_tag, &find_ref, &find_off, &find_len, DF_FORWARD) == SUCCEED)
      {
          uint16      elt_tag, elt_ref;   /* tag/ref of items in a RIG */
          int32       group_id;       /* group ID for looking at RIG's */

          /* read RIG into memory */
          if ((group_id = DFdiread(file_id, DFTAG_RIG, find_ref)) == FAIL)
              HGOTO_ERROR(DFE_INTERNAL, FAIL);
          elt_tag = elt_ref = 0;    /* initialize bogus tag/ref */
          while (!DFdiget(group_id, &elt_tag, &elt_ref))
            {   /* get next tag/ref */
                if (elt_tag == DFTAG_CI || elt_tag == DFTAG_RI)
                  {   
                      if (elt_tag > 0 && elt_ref > 0) /* make certain we found an image */
                        {     /* store the information about the image */
                            img_info[curr_image].grp_tag=DFTAG_RIG;
                            img_info[curr_image].grp_ref=find_ref;
                            img_info[curr_image].img_tag=elt_tag;
                            img_info[curr_image].img_ref=elt_ref;
                            img_info[curr_image].offset = Hoffset(file_id, elt_tag, elt_ref);     /* store offset */
                            curr_image++;
                        }     /* end if */
                      break;  /* break out of reading the group */
                  } /* end if */
            } /* end while */
      } /* end while */

    /* go through the RI8s */
    find_tag = find_ref = 0;
    while (Hfind(file_id, DFTAG_RI8, DFREF_WILDCARD, &find_tag, &find_ref, &find_off, &find_len, DF_FORWARD) == SUCCEED)
      {
          img_info[curr_image].grp_tag=DFTAG_NULL;
          img_info[curr_image].grp_ref=DFREF_WILDCARD;
          img_info[curr_image].img_tag=find_tag;
          img_info[curr_image].img_ref=find_ref;
          img_info[curr_image].offset = find_off;   /* store offset */
          curr_image++;
      }     /* end while */

    /* go through the CI8s */
    find_tag = find_ref = 0;
    while (Hfind(file_id, DFTAG_CI8, DFREF_WILDCARD, &find_tag, &find_ref, &find_off, &find_len, DF_FORWARD) == SUCCEED)
      {
          img_info[curr_image].grp_tag=DFTAG_NULL;
          img_info[curr_image].grp_ref=DFREF_WILDCARD;
          img_info[curr_image].img_tag=find_tag;
          img_info[curr_image].img_ref=find_ref;
          img_info[curr_image].offset = find_off;   /* store offset */
          curr_image++;
      } /* end while */

    /* Eliminate duplicate images by using the offset of the image data */
    /* Here's a table for how the images will be eliminated: */
    /*
                                    img_info[i].grp_tag
                                    VG              RIG             NULL(RI8)
                               +-----------------+-----------------+--------+
                            VG | j=NULL          | i=NULL          | i=NULL |
                               |                 | j.aux=i.grp_ref |        |
                               +-----------------+-----------------+--------+
    img_info[j].grp_tag     RIG| j=NULL          | j=NULL          | i=NULL |
                               | i.aux=j.grp_ref |                 |        |
                               +-----------------+-----------------+--------+
                      NULL(RI8)| j=NULL          | j=NULL          | j=NULL |
                               +-----------------+-----------------+--------+
    */

    nimages = curr_image;   /* reset the number of images we really have */
#ifdef QAK
printf("before duplicate elimination\n");
for (i = 0; i < curr_image; i++)
  {
    printf("%d: tag=%u, ref=%u, offset=%ld\n",(int)i,(unsigned)img_info[i].img_tag,(unsigned)img_info[i].img_ref,(long)img_info[i].offset);
  } /* end for */
#endif /* QAK */
    for (i = 0; i < curr_image; i++)
      {     /* go through the images looking for duplicates */
          if(img_info[i].img_tag!=DFTAG_NULL)
              for (j = i+1; j < curr_image; j++)
                {
                    if(img_info[j].img_tag!=DFTAG_NULL)
                        if (img_info[i].offset == img_info[j].offset)
                          {
                              /* eliminate the oldest tag from the match */
                              switch(img_info[i].img_tag) {
                                  case DFTAG_RI:
                                  case DFTAG_CI: /* Newer style raster image, found in RIG & Vgroup */
                                      if(img_info[j].grp_tag==DFTAG_RIG)
                                        {
                                          img_info[j].img_tag=DFTAG_NULL;
                                          if(img_info[i].grp_tag==DFTAG_VG)
                                              img_info[i].aux_ref=img_info[j].grp_ref;
                                        } /* end if */
                                      else
                                          if(img_info[i].grp_tag==DFTAG_VG)
                                              img_info[j].img_tag=DFTAG_NULL;
                                          else
                                            {
                                              img_info[j].img_tag=DFTAG_NULL;
                                              if(img_info[i].grp_tag==DFTAG_RIG)
                                                  img_info[j].aux_ref=img_info[i].grp_ref;
                                            } /* end else */
                                      break;

                                  case DFTAG_RI8:
                                  case DFTAG_CI8: /* Eldest style raster image, no grouping */
                                      if(img_info[j].img_tag!=DFTAG_RI8 && img_info[j].img_tag!=DFTAG_CI8)
                                          img_info[i].img_tag=DFTAG_NULL;
                                      else
                                          img_info[j].img_tag=DFTAG_NULL;
                                      break;

                                  default: /* an image which was eliminated from the list of images */
                                    break;
                                } /* end switch */
                              nimages--;  /* if duplicate found, decrement the number of images */
                          } /* end if */
                } /* end for */
      } /* end for */
#ifdef QAK
printf("after duplicate elimination\n");
for (i = 0; i < curr_image; i++)
  {
    printf("%d: tag=%u, ref=%u, offset=%ld\n",(int)i,(unsigned)img_info[i].img_tag,(unsigned)img_info[i].img_ref,(long)img_info[i].offset);
  } /* end for */
#endif /* QAK */

    /* Ok, now sort through the file for information about each image found */
    for(i=0; i<curr_image; i++)
      {
          if(img_info[i].img_tag!=DFTAG_NULL)
            {
              switch(img_info[i].grp_tag) {
                  case DFTAG_VG: /* New style raster image, found in a Vgroup */
                      {
                          ri_info_t *new_image; /* ptr to the image to read in */
                          int32 img_key;            /* Vgroup key of an image */
                          int32 img_tag,img_ref;    /* image tag/ref in the Vgroup */
                          char textbuf[VGNAMELENMAX + 1];    /* buffer to store the name in */
                          uint8 ntstring[4];        /* buffer to store NT info */
                          uint8 GRtbuf[64];         /* local buffer for reading RIG info */

                          if((img_key=Vattach(file_id,img_info[i].grp_ref,"r"))!=FAIL)
                            {
                                if((new_image=(ri_info_t *)HDmalloc(sizeof(ri_info_t)))==NULL)
                                  {
                                    HDfree((VOIDP) img_info);   /* free offsets */
                                    Hclose(file_id);
                                    HGOTO_ERROR(DFE_NOSPACE,FAIL);
                                  } /* end if */

                                /* Initialize all the fields in the image structure to zeros */
                                HDmemset(new_image,0,sizeof(ri_info_t));

                                /* Get the name of the image */
                                if(Vgetname(img_key,textbuf)==FAIL)
                                    sprintf(textbuf,"Raster Image #%d",(int)i);
                                if((new_image->name=(char *)HDmalloc(HDstrlen(textbuf)+1))==NULL)
                                    HGOTO_ERROR(DFE_NOSPACE,FAIL);
                                HDstrcpy(new_image->name,textbuf);

                                /* Initialize the local attribute tree */
                                new_image->lattr_count = 0;
                                new_image->lattree = tbbtdmake(rigcompare, sizeof(int32));
                                if (new_image->lattree == NULL)
                                    HGOTO_ERROR(DFE_NOSPACE, FAIL);
                                new_image->ri_ref=img_info[i].grp_ref;
                                if(img_info[i].aux_ref!=0)
                                    new_image->rig_ref=img_info[i].aux_ref;
                                else
                                    new_image->rig_ref=DFREF_WILDCARD;

                                for(j=0; j<Vntagrefs(img_key); j++)
                                  {
                                      if(Vgettagref(img_key,j,&img_tag,&img_ref)==FAIL)
                                          continue;

                                      /* parse this tag/ref pair */
                                      switch(img_tag) {
                                          case DFTAG_RI:
                                          case DFTAG_CI:    /* Regular or compressed image data */
                                              new_image->img_tag=(uint16)img_tag;
                                              new_image->img_ref=(uint16)img_ref;
                                              break;

                                          case DFTAG_LUT:   /* Palette */
                                              new_image->lut_tag=(uint16)img_tag;
                                              new_image->lut_ref=(uint16)img_ref;
                                              break;

                                          case DFTAG_LD:    /* Palette dimensions */
                                              if (Hgetelement(file_id, (uint16)img_tag, (uint16)img_ref, GRtbuf) != FAIL)
                                                {
                                                    uint8      *p;

                                                    p = GRtbuf;
                                                    INT32DECODE(p, new_image->lut_dim.xdim);
                                                    INT32DECODE(p, new_image->lut_dim.ydim);
                                                    UINT16DECODE(p, new_image->lut_dim.nt_tag);
                                                    UINT16DECODE(p, new_image->lut_dim.nt_ref);
                                                    INT16DECODE(p, new_image->lut_dim.ncomps);
                                                    INT16DECODE(p, new_image->lut_dim.il);
                                                    UINT16DECODE(p, new_image->lut_dim.comp_tag);
                                                    UINT16DECODE(p, new_image->lut_dim.comp_ref);
                                                }
                                              else
                                                  HGOTO_ERROR(DFE_READERROR, FAIL);
                                               
                                               /* read NT */
                                              if (Hgetelement(file_id, new_image->lut_dim.nt_tag, new_image->lut_dim.nt_ref, ntstring) == FAIL)
                                                  HGOTO_ERROR(DFE_READERROR, FAIL);

                                              /* check for any valid NT */
                                              if (ntstring[1] == DFNT_NONE)
                                                  break;
                                              
                                              /* set NT info */
                                              new_image->lut_dim.dim_ref = (uint16)img_ref;
                                              new_image->lut_dim.nt = (int32)ntstring[1];
                                              new_image->lut_dim.file_nt_subclass = (int32)ntstring[3];
                                              if ((new_image->lut_dim.file_nt_subclass != DFNTF_HDFDEFAULT)
                                                    && (new_image->lut_dim.file_nt_subclass!= DFNTF_PC)
                                                    && (new_image->lut_dim.file_nt_subclass!= DFKgetPNSC(new_image->lut_dim.nt, DF_MT)))
                                                  break; /* unknown subclass */
                                              if (new_image->lut_dim.file_nt_subclass!= DFNTF_HDFDEFAULT)
                                                {     /* if native or little endian */
                                                    if (new_image->lut_dim.file_nt_subclass!= DFNTF_PC)   /* native */
                                                        new_image->lut_dim.nt |= (uint32)DFNT_NATIVE;
                                                    else  /* little endian */
                                                        new_image->lut_dim.nt |= (uint32)DFNT_LITEND;
                                                }     /* end if */
                                              break;

                                          case DFTAG_ID:    /* Image description info */
                                              if (Hgetelement(file_id, (uint16)img_tag, (uint16)img_ref, GRtbuf) != FAIL)
                                                {
                                                    uint8      *p;

                                                    p = GRtbuf;
                                                    INT32DECODE(p, new_image->img_dim.xdim);
                                                    INT32DECODE(p, new_image->img_dim.ydim);
                                                    UINT16DECODE(p, new_image->img_dim.nt_tag);
                                                    UINT16DECODE(p, new_image->img_dim.nt_ref);
                                                    INT16DECODE(p, new_image->img_dim.ncomps);
                                                    INT16DECODE(p, new_image->img_dim.il);
                                                    UINT16DECODE(p, new_image->img_dim.comp_tag);
                                                    UINT16DECODE(p, new_image->img_dim.comp_ref);
                                                }
                                              else
                                                  HGOTO_ERROR(DFE_READERROR, FAIL);
                                               
                                               /* read NT */
                                              if (Hgetelement(file_id, new_image->img_dim.nt_tag, new_image->img_dim.nt_ref, ntstring) == FAIL)
                                                  HGOTO_ERROR(DFE_READERROR, FAIL);

                                              /* check for any valid NT */
                                              if (ntstring[1] == DFNT_NONE)
                                                  break;
                                              
                                              /* set NT info */
                                              new_image->img_dim.dim_ref=(uint16)img_ref;
                                              new_image->img_dim.nt = (int32)ntstring[1];
                                              new_image->img_dim.file_nt_subclass = (int32)ntstring[3];
                                              if ((new_image->img_dim.file_nt_subclass != DFNTF_HDFDEFAULT)
                                                    && (new_image->img_dim.file_nt_subclass!= DFNTF_PC)
                                                    && (new_image->img_dim.file_nt_subclass!= DFKgetPNSC(new_image->img_dim.nt, DF_MT)))
                                                  break; /* unknown subclass */
                                              if (new_image->img_dim.file_nt_subclass!= DFNTF_HDFDEFAULT)
                                                {     /* if native or little endian */
                                                    if (new_image->img_dim.file_nt_subclass!= DFNTF_PC)   /* native */
                                                        new_image->img_dim.nt |= (uint32)DFNT_NATIVE;
                                                    else  /* little endian */
                                                        new_image->img_dim.nt |= (uint32)DFNT_LITEND;
                                                }     /* end if */
                                              break;

                                          case DFTAG_VH:    /* Attribute information */
                                              {
                                                  at_info_t *new_attr;  /* attribute to add to the set of local attributes */
                                                  int32 at_key;         /* VData key for the attribute */

                                                  if((new_attr=(at_info_t *)HDmalloc(sizeof(at_info_t)))==NULL)
                                                      HGOTO_ERROR(DFE_NOSPACE,FAIL);
                                                  new_attr->ref=(uint16)img_ref;
                                                  new_attr->index=new_image->lattr_count;
                                                  new_attr->data_modified=FALSE;
                                                  new_attr->new_at=FALSE;
                                                  new_attr->data=NULL;
                                                  if((at_key=VSattach(file_id,(int32)img_ref,"r"))!=FAIL)
                                                    {
                                                        char *fname;

                                                        /* Make certain the attribute only has one field */
                                                        if(VFnfields(at_key)!=1)
                                                          {
                                                            VSdetach(at_key);
                                                            HDfree(new_attr);
                                                            break;
                                                          } /* end if */
                                                        new_attr->nt=VFfieldtype(at_key,0);
                                                        new_attr->len=VFfieldorder(at_key,0);
                                                        if(new_attr->len==1)
                                                            new_attr->len=VSelts(at_key);

                                                        /* Get the name of the attribute */
                                                        if((fname=VFfieldname(at_key,0))==NULL)
                                                          {
                                                            sprintf(textbuf,"Attribute #%d",(int)new_attr->index);
                                                            if((new_attr->name=(char *)HDmalloc(HDstrlen(textbuf)+1))==NULL)
                                                              {
                                                                VSdetach(at_key);
                                                                HDfree(new_attr);
                                                                HGOTO_ERROR(DFE_NOSPACE,FAIL);
                                                              } /* end if */
                                                            HDstrcpy(new_attr->name,textbuf);
                                                          } /* end if */
                                                        else
                                                          {
                                                            if((new_attr->name=(char *)HDmalloc(HDstrlen(fname)+1))==NULL)
                                                              {
                                                                VSdetach(at_key);
                                                                HDfree(new_attr);
                                                                HGOTO_ERROR(DFE_NOSPACE,FAIL);
                                                              } /* end if */
                                                            HDstrcpy(new_attr->name,fname);
                                                          } /* end else */
                                                
                                                        tbbtdins(new_image->lattree, (VOIDP) new_attr, NULL);    /* insert the attr instance in B-tree */ 

                                                        VSdetach(at_key);
                                                    } /* end if */

                                                  new_image->lattr_count++;
                                              } /* end case */
                                              break;

                                          default:          /* Unknown tag */
                                              break;
                                        } /* end switch */
                                  } /* end for */
                              new_image->index=gr_ptr->gr_count;
                              new_image->gr_ptr=gr_ptr; /* point up the tree */
                              tbbtdins(gr_ptr->grtree, (VOIDP) new_image, NULL);    /* insert the new image into B-tree */ 
                              gr_ptr->gr_count++;
                              Vdetach(img_key);
                            } /* end if */
                      } /* end case */
                      break;

                  case DFTAG_RIG:   /* Older style raster image, found in RIG */
                      {
                          int32       GroupID;
                          uint16      elt_tag, elt_ref;
                          ri_info_t *new_image; /* ptr to the image to read in */
                          char textbuf[VGNAMELENMAX + 1];    /* buffer to store the name in */
                          uint8 ntstring[4];        /* buffer to store NT info */
                          uint8 GRtbuf[64];         /* local buffer for reading RIG info */

                          /* read RIG into memory */
                          if ((GroupID = DFdiread(file_id, DFTAG_RIG, img_info[i].grp_ref)) == FAIL)
                              HGOTO_ERROR(DFE_READERROR, FAIL);

                          if((new_image=(ri_info_t *)HDmalloc(sizeof(ri_info_t)))==NULL)
                            {
                              HDfree((VOIDP) img_info);   /* free offsets */
                              Hclose(file_id);
                              HGOTO_ERROR(DFE_NOSPACE,FAIL);
                            } /* end if */

                          /* Initialize all the fields in the image structure to zeros */
                          HDmemset(new_image,0,sizeof(ri_info_t));

                          /* Get the name of the image */
                          sprintf(textbuf,"Raster Image #%d",(int)i);
                          if((new_image->name=(char *)HDmalloc(HDstrlen(textbuf)+1))==NULL)
                              HGOTO_ERROR(DFE_NOSPACE,FAIL);
                          HDstrcpy(new_image->name,textbuf);

                          /* Initialize the local attribute tree */
                          new_image->lattree = tbbtdmake(rigcompare, sizeof(int32));
                          if (new_image->lattree == NULL)
                              HGOTO_ERROR(DFE_NOSPACE, FAIL);
                          new_image->ri_ref=DFREF_WILDCARD;
                          new_image->rig_ref=img_info[i].grp_ref;

                          while (!DFdiget(GroupID, &elt_tag, &elt_ref))
                            {     /* get next tag/ref */
                                switch (elt_tag)
                                  {   /* process tag/ref */
                                      case DFTAG_RI:
                                      case DFTAG_CI:    /* Regular or compressed image data */
                                          new_image->img_tag=elt_tag;
                                          new_image->img_ref=elt_ref;
                                          break;

                                      case DFTAG_LUT:   /* Palette */
                                          new_image->lut_tag=elt_tag;
                                          new_image->lut_ref=elt_ref;
                                          break;

                                      case DFTAG_LD:    /* Palette dimensions */
                                          if (Hgetelement(file_id, elt_tag, elt_ref, GRtbuf) != FAIL)
                                            {
                                                uint8      *p;

                                                p = GRtbuf;
                                                INT32DECODE(p, new_image->lut_dim.xdim);
                                                INT32DECODE(p, new_image->lut_dim.ydim);
                                                UINT16DECODE(p, new_image->lut_dim.nt_tag);
                                                UINT16DECODE(p, new_image->lut_dim.nt_ref);
                                                INT16DECODE(p, new_image->lut_dim.ncomps);
                                                INT16DECODE(p, new_image->lut_dim.il);
                                                UINT16DECODE(p, new_image->lut_dim.comp_tag);
                                                UINT16DECODE(p, new_image->lut_dim.comp_ref);
                                            }
                                          else
                                              HGOTO_ERROR(DFE_READERROR, FAIL);
                                               
                                           /* read NT */
                                          if (Hgetelement(file_id, new_image->lut_dim.nt_tag, new_image->lut_dim.nt_ref, ntstring) == FAIL)
                                              HGOTO_ERROR(DFE_READERROR, FAIL);

                                          /* check for any valid NT */
                                          if (ntstring[1] == DFNT_NONE)
                                              break;
                                              
                                          /* set NT info */
                                          new_image->lut_dim.dim_ref = elt_ref;
                                          new_image->lut_dim.nt = (int32)ntstring[1];
                                          new_image->lut_dim.file_nt_subclass = (int32)ntstring[3];
                                          if ((new_image->lut_dim.file_nt_subclass != DFNTF_HDFDEFAULT)
                                                && (new_image->lut_dim.file_nt_subclass!= DFNTF_PC)
                                                && (new_image->lut_dim.file_nt_subclass!= DFKgetPNSC(new_image->lut_dim.nt, DF_MT)))
                                              break; /* unknown subclass */
                                          if (new_image->lut_dim.file_nt_subclass!= DFNTF_HDFDEFAULT)
                                            {     /* if native or little endian */
                                                if (new_image->lut_dim.file_nt_subclass!= DFNTF_PC)   /* native */
                                                    new_image->lut_dim.nt |= (uint32)DFNT_NATIVE;
                                                else  /* little endian */
                                                    new_image->lut_dim.nt |= (uint32)DFNT_LITEND;
                                            }     /* end if */
                                          break;

                                        case DFTAG_ID:    /* Image description info */
                                            if (Hgetelement(file_id, elt_tag, elt_ref, GRtbuf) != FAIL)
                                              {
                                                  uint8      *p;

                                                  p = GRtbuf;
                                                  INT32DECODE(p, new_image->img_dim.xdim);
                                                  INT32DECODE(p, new_image->img_dim.ydim);
                                                  UINT16DECODE(p, new_image->img_dim.nt_tag);
                                                  UINT16DECODE(p, new_image->img_dim.nt_ref);
                                                  INT16DECODE(p, new_image->img_dim.ncomps);
                                                  INT16DECODE(p, new_image->img_dim.il);
                                                  UINT16DECODE(p, new_image->img_dim.comp_tag);
                                                  UINT16DECODE(p, new_image->img_dim.comp_ref);
                                              }
                                            else
                                                HGOTO_ERROR(DFE_GETELEM, FAIL);
                                               
                                             /* read NT */
                                            if (Hgetelement(file_id, new_image->img_dim.nt_tag, new_image->img_dim.nt_ref, ntstring) == FAIL)
                                                HGOTO_ERROR(DFE_GETELEM, FAIL);

                                            /* check for any valid NT */
                                            if (ntstring[1] == DFNT_NONE)
                                                break;
                                              
                                            /* set NT info */
                                            new_image->img_dim.dim_ref=elt_ref;
                                            new_image->img_dim.nt = (int32)ntstring[1];
                                            new_image->img_dim.file_nt_subclass = (int32)ntstring[3];
                                            if ((new_image->img_dim.file_nt_subclass != DFNTF_HDFDEFAULT)
                                                  && (new_image->img_dim.file_nt_subclass!= DFNTF_PC)
                                                  && (new_image->img_dim.file_nt_subclass!= DFKgetPNSC(new_image->img_dim.nt, DF_MT)))
                                                break; /* unknown subclass */
                                            if (new_image->img_dim.file_nt_subclass!= DFNTF_HDFDEFAULT)
                                              {     /* if native or little endian */
                                                  if (new_image->img_dim.file_nt_subclass!= DFNTF_PC)   /* native */
                                                      new_image->img_dim.nt |= (uint32)DFNT_NATIVE;
                                                  else  /* little endian */
                                                      new_image->img_dim.nt |= (uint32)DFNT_LITEND;
                                              }     /* end if */
                                            break;

                                      default:    /* ignore unknown tags */
                                          break;
                                  } /* end switch */
                            } /* end while */
                        new_image->index=gr_ptr->gr_count;
                        new_image->gr_ptr=gr_ptr; /* point up the tree */
                        tbbtdins(gr_ptr->grtree, (VOIDP) new_image, NULL);    /* insert the new image into B-tree */ 
                        gr_ptr->gr_count++;
                      } /* end case */
                      break;

                  case DFTAG_NULL:  /* Eldest style raster image, no grouping */
                      {
                          ri_info_t *new_image; /* ptr to the image to read in */
                          char textbuf[VGNAMELENMAX + 1];    /* buffer to store the name in */
                          uint8 GRtbuf[64];         /* local buffer for reading RIG info */

                          if((new_image=(ri_info_t *)HDmalloc(sizeof(ri_info_t)))==NULL)
                            {
                              HDfree((VOIDP) img_info);   /* free offsets */
                              Hclose(file_id);
                              HGOTO_ERROR(DFE_NOSPACE,FAIL);
                            } /* end if */

                          /* Initialize all the fields in the image structure to zeros */
                          HDmemset(new_image,0,sizeof(ri_info_t));

                          /* Get the name of the image */
                          sprintf(textbuf,"Raster Image #%d",(int)i);
                          if((new_image->name=(char *)HDmalloc(HDstrlen(textbuf)+1))==NULL)
                              HGOTO_ERROR(DFE_NOSPACE,FAIL);
                          HDstrcpy(new_image->name,textbuf);

                          /* Initialize the local attribute tree */
                          new_image->lattree = tbbtdmake(rigcompare, sizeof(int32));
                          if (new_image->lattree == NULL)
                              HGOTO_ERROR(DFE_NOSPACE, FAIL);
                          new_image->ri_ref=DFREF_WILDCARD;
                          new_image->rig_ref=DFREF_WILDCARD;

                          /* Get tag/ref for image */
                          new_image->img_tag=img_info[i].img_tag;
                          new_image->img_ref=img_info[i].img_ref;

                          /* Get dimension information */
                          if (Hgetelement(file_id, DFTAG_ID8, new_image->img_ref, GRtbuf) != FAIL)
                            {
                                uint8      *p;
                                uint16      u;

                                p = GRtbuf;
                                UINT16DECODE(p, u);
                                new_image->img_dim.xdim=(int32)u;
                                UINT16DECODE(p, u);
                                new_image->img_dim.ydim=(int32)u;
                            }   /* end if */
                          else
                              HGOTO_ERROR(DFE_GETELEM, FAIL);

                          /* only 8-bit images, so fill in rest of dim info */
                          new_image->img_dim.dim_ref=DFREF_WILDCARD;
                          new_image->img_dim.ncomps=1;
                          new_image->img_dim.nt=DFNT_UINT8;
                          new_image->img_dim.file_nt_subclass=DFNTF_HDFDEFAULT;
                          new_image->img_dim.il=MFGR_INTERLACE_PIXEL;
                          new_image->img_dim.nt_tag=DFTAG_NULL;
                          new_image->img_dim.nt_ref=DFREF_WILDCARD;
                          new_image->img_dim.comp_tag=DFTAG_NULL;
                          new_image->img_dim.comp_ref=DFREF_WILDCARD;

                          /* Get palette information */
                          if(Hexist(file_id, DFTAG_IP8, new_image->img_ref)==SUCCEED)
                            {
                                new_image->lut_tag=DFTAG_IP8;
                                new_image->lut_ref=new_image->img_ref;

                                /* set palette dimensions too */
                                new_image->lut_dim.dim_ref = DFREF_WILDCARD;
                                new_image->lut_dim.xdim=256;
                                new_image->lut_dim.ydim=1;
                                new_image->img_dim.ncomps=1;
                                new_image->img_dim.nt=DFNT_UINT8;
                                new_image->img_dim.file_nt_subclass=DFNTF_HDFDEFAULT;
                                new_image->img_dim.il=MFGR_INTERLACE_PIXEL;
                                new_image->img_dim.nt_tag=DFTAG_NULL;
                                new_image->img_dim.nt_ref=DFREF_WILDCARD;
                                new_image->img_dim.comp_tag=DFTAG_NULL;
                                new_image->img_dim.comp_ref=DFREF_WILDCARD;
                            } /* end if */
                          else
                                new_image->lut_tag=new_image->lut_ref=DFREF_WILDCARD;

                        new_image->index=gr_ptr->gr_count;
                        new_image->gr_ptr=gr_ptr; /* point up the tree */
                        tbbtdins(gr_ptr->grtree, (VOIDP) new_image, NULL);    /* insert the new image into B-tree */ 
                        gr_ptr->gr_count++;
                      } /* end case */
                      break;

                  default: /* an image which was eliminated from the list of images */
                    break;
                } /* end switch */
            } /* end if */
      } /* end for */

    HDfree((VOIDP) img_info);   /* free offsets */

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */

  return ret_value;
} /* end GRIget_image_list() */

/*--------------------------------------------------------------------------
 NAME
    GRIil_convert
 PURPOSE
    Convert a buffer from one interlace to another.
 USAGE
    intn GRIil_convert(inbuf,inil,outbuf,outil,dims,ncomp,nt)
        const VOIDP inbuf;          IN: input buffer
        gr_interlace_t inil;        IN: input buffer's interlace scheme
        VOIDP outbuf;               IN: output buffer
        gr_interlace_t outil;       IN: output buffer's requested interlace scheme
        int32 dims[2];              IN: dimensions of the buffers
        int32 ncomp;                IN: both buffer's number of components per pixel
        int32 nt;                   IN: both buffer's pixel number-type
 RETURNS
    Returns SUCCEED/FAIL
 DESCRIPTION
    This routine converts between PIXEL, LINE & COMPONENT interlacing schemes.
    All data written to the disk is written in PIXEL interlacing and converted
    to/from the user's buffers.
 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
    This routine does no parameter checking, it's assumed to be done at a
    higher layer.

    This routine also does not handle the case where the inbuf==outbuf, i.e.
    switching the interlace 'in-place'.  More work would be required to enable
    this to be done.
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
intn GRIil_convert(const VOIDP inbuf,gr_interlace_t inil,VOIDP outbuf,
        gr_interlace_t outil,int32 dims[2],int32 ncomp,int32 nt)
{
    CONSTR(FUNC, "GRIil_convert");    /* for HERROR */
    intn ret_value=SUCCEED;
    uintn pixel_size=DFKNTsize((nt|DFNT_NATIVE)&(~DFNT_LITEND))*ncomp;
    uintn comp_size=DFKNTsize((nt|DFNT_NATIVE)&(~DFNT_LITEND));
    VOIDP *in_comp_ptr=NULL;    /* an array of pointers to each input component */
    VOIDP *out_comp_ptr=NULL;   /* an array of pointers to each output component */
    int32 *in_pixel_add=NULL;   /* an array of increments for each input pixel moved */
    int32 *out_pixel_add=NULL;  /* an array of increments for each output pixel moved */
    int32 *in_line_add=NULL;    /* an array of increments for each input line moved */
    int32 *out_line_add=NULL;   /* an array of increments for each output line moved */
    intn i,j,k;       /* local counting variables */

#ifdef QAK
printf("%s: inil=%d, outil=%d\n",FUNC,(int)inil,(int)outil);
printf("%s: dim[XDIM]=%d, dim[YDIM]=%d\n",FUNC,(int)dims[XDIM],(int)dims[YDIM]);
printf("%s: ncomp=%d, nt=%d\n",FUNC,(int)ncomp,(int)nt);
printf("%s: pixel_size=%d, comp_size=%d\n",FUNC,(int)pixel_size,(int)comp_size);
#endif /* QAK */
    if(inil==outil)     /* check for trivial input=output 'conversion' */
        HDmemcpy(outbuf,inbuf,dims[XDIM]*dims[YDIM]*pixel_size);
    else
      {
          /* allocate pixel pointer arrays */
          if((in_comp_ptr=HDmalloc(sizeof(VOIDP)*ncomp))==NULL)
              HGOTO_ERROR(DFE_NOSPACE, FAIL);
          if((out_comp_ptr=HDmalloc(sizeof(VOIDP)*ncomp))==NULL)
              HGOTO_ERROR(DFE_NOSPACE, FAIL);

          /* allocate pixel increment arrays */
          if((in_pixel_add=HDmalloc(sizeof(int32)*ncomp))==NULL)
              HGOTO_ERROR(DFE_NOSPACE, FAIL);
          if((out_pixel_add=HDmalloc(sizeof(int32)*ncomp))==NULL)
              HGOTO_ERROR(DFE_NOSPACE, FAIL);

          /* allocate line increment arrays */
          if((in_line_add=HDmalloc(sizeof(int32)*ncomp))==NULL)
              HGOTO_ERROR(DFE_NOSPACE, FAIL);
          if((out_line_add=HDmalloc(sizeof(int32)*ncomp))==NULL)
              HGOTO_ERROR(DFE_NOSPACE, FAIL);

          /* Set up the input buffer pointers and adders */
          switch(inil)
            {
                case MFGR_INTERLACE_PIXEL:
                    for(i=0; i<ncomp; i++)
                      {
                        in_comp_ptr[i]=((uint8 *)inbuf)+i*comp_size;
                        in_pixel_add[i]=pixel_size;
                        in_line_add[i]=0;
                      } /* end for */
                    break;

                case MFGR_INTERLACE_LINE:
                    for(i=0; i<ncomp; i++)
                      {
                        in_comp_ptr[i]=((uint8 *)inbuf)+i*dims[XDIM]*comp_size;
                        in_pixel_add[i]=comp_size;
                        in_line_add[i]=(ncomp-1)*dims[XDIM]*comp_size;
                      } /* end for */
                    break;

                case MFGR_INTERLACE_COMPONENT:
                    for(i=0; i<ncomp; i++)
                      {
                        in_comp_ptr[i]=((uint8 *)inbuf)+i*dims[YDIM]*dims[XDIM]*comp_size;
                        in_pixel_add[i]=comp_size;
                        in_line_add[i]=0;
                      } /* end for */
                    break;

                default:
                    HGOTO_ERROR(DFE_ARGS, FAIL);
            } /* end switch */

          /* Set up the output buffer pointers and adders */
          switch(outil)
            {
                case MFGR_INTERLACE_PIXEL:
                    for(i=0; i<ncomp; i++)
                      {
                        out_comp_ptr[i]=((uint8 *)outbuf)+i*comp_size;
                        out_pixel_add[i]=pixel_size;
                        out_line_add[i]=0;
                      } /* end for */
                    break;

                case MFGR_INTERLACE_LINE:
                    for(i=0; i<ncomp; i++)
                      {
                        out_comp_ptr[i]=((uint8 *)outbuf)+i*dims[XDIM]*comp_size;
                        out_pixel_add[i]=comp_size;
                        out_line_add[i]=(ncomp-1)*dims[XDIM]*comp_size;
                      } /* end for */
                    break;

                case MFGR_INTERLACE_COMPONENT:
                    for(i=0; i<ncomp; i++)
                      {
                        out_comp_ptr[i]=((uint8 *)outbuf)+i*dims[YDIM]*dims[XDIM]*comp_size;
                        out_pixel_add[i]=comp_size;
                        out_line_add[i]=0;
                      } /* end for */
                    break;

                default:
                    HGOTO_ERROR(DFE_ARGS, FAIL);
            } /* end switch */

#ifdef QAK
for(i=0; i<ncomp; i++)
  {
      printf("%s: in_pixel_add[%d]=%d, in_line_add[%d]=%d\n",FUNC,(int)i,(int)in_pixel_add[i],(int)i,(int)in_line_add[i]);
      printf("%s: out_pixel_add[%d]=%d, out_line_add[%d]=%d\n",FUNC,(int)i,(int)out_pixel_add[i],(int)i,(int)out_line_add[i]);
  } /* end for */
#endif /* QAK */
        /* now just push pixels from one buffer to another */
        for(i=0; i<dims[YDIM]; i++)
          {
              for(j=0; j<dims[XDIM]; j++)
                {
                    for(k=0; k<ncomp; k++)
                      {
                          HDmemcpy(out_comp_ptr[k],in_comp_ptr[k],comp_size);
                          out_comp_ptr[k]=((uint8 *)out_comp_ptr[k])+out_pixel_add[k];
                          in_comp_ptr[k]=((uint8 *)in_comp_ptr[k])+in_pixel_add[k];
                      } /* end for */
                } /* end for */

              /* wrap around the end of the line of pixels */
              /* (only necessary if one of the buffers is in 'line' interlace) */
              if(inil==MFGR_INTERLACE_LINE || outil==MFGR_INTERLACE_LINE)
                  for(k=0; k<ncomp; k++)
                    {
                        out_comp_ptr[k]=((uint8 *)out_comp_ptr[k])+out_line_add[k];
                        in_comp_ptr[k]=((uint8 *)in_comp_ptr[k])+in_line_add[k];
                    } /* end for */
          } /* end for */
      } /* end else */

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */

    /* Free arrays allocated during this routine */
    /* (common for both error and normal returns) */
    if(in_comp_ptr!=NULL)
        HDfree(in_comp_ptr);
    if(out_comp_ptr!=NULL)
        HDfree(out_comp_ptr);
    if(in_pixel_add!=NULL)
        HDfree(in_pixel_add);
    if(out_pixel_add!=NULL)
        HDfree(out_pixel_add);
    if(in_line_add!=NULL)
        HDfree(in_line_add);
    if(out_line_add!=NULL)
        HDfree(out_line_add);

  return ret_value;
} /* end GRIil_convert() */

/*--------------------------------------------------------------------------
 NAME
    GRstart
 PURPOSE
    Initialize the GR*() interface for a given HDF file.
 USAGE
    int32 GRstart(hdf_file_id)
        int32 hdf_file_id;          IN: file ID from Hopen
 RETURNS
    Return grid on success or FAIL
 DESCRIPTION
    Initializes the GR*() interface to operate on the HDF file which was
    specified by hdf_file_id.  This routine must be called before any further
    GR*() routines are called for a file.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
int32 GRstart(int32 hdf_file_id)
{
    CONSTR(FUNC, "GRstart");    /* for HERROR */
    gr_info_t *gr_ptr;          /* ptr to the new GR information for a file */
    int32  ret_value = SUCCEED;

#ifdef HAVE_PABLO
    TRACE_ON(GR_mask, ID_GRstart);
#endif /* HAVE_PABLO */
    /* clear error stack and check validity of file id */
    HEclear();

    /* check the validity of the file ID */
    if(!HDvalidfid(hdf_file_id))
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* Check if GR file tree has been allocated */
    if (gr_tree == NULL)
      {
          if ((gr_tree = tbbtdmake(rigcompare, sizeof(int32))) == NULL)
              HGOTO_ERROR(DFE_NOSPACE, FAIL);

          /* Initialize the atom groups for GRs and RIs */
          HAinit_group(GRIDGROUP,GRATOM_HASH_SIZE);
          HAinit_group(RIIDGROUP,GRATOM_HASH_SIZE);
      } /* end if */

    /* Grab the existing gr_info_t structure first, otherwise create a new one */
    if ((gr_ptr = Get_grfile(hdf_file_id)) == NULL)
        if ((gr_ptr = New_grfile(hdf_file_id)) == NULL)
            HGOTO_ERROR(DFE_FNF, FAIL);

    if (gr_ptr->access++)
        HGOTO_DONE(SUCCEED);

    /* Fire up the Vset interface */
    if(Vstart(hdf_file_id)==FAIL)
        HGOTO_ERROR(DFE_CANTINIT,FAIL);

    /* Initialize the starting information for the interface */
    gr_ptr->hdf_file_id=hdf_file_id;
    gr_ptr->gr_ref=DFREF_WILDCARD;
    gr_ptr->gr_count=0;
    gr_ptr->grtree = tbbtdmake(rigcompare, sizeof(int32));
    if (gr_ptr->grtree == NULL)
        HGOTO_ERROR(DFE_NOSPACE, FAIL);
    gr_ptr->gr_modified=0;

    gr_ptr->gattr_count=0;
    gr_ptr->gattree = tbbtdmake(rigcompare, sizeof(int32));
    if (gr_ptr->gattree == NULL)
        HGOTO_ERROR(DFE_NOSPACE, FAIL);
    gr_ptr->gattr_modified=0;
    gr_ptr->attr_cache=GR_ATTR_THRESHHOLD;

    /* Go get all the images and attributes in the file */
    if(GRIget_image_list(hdf_file_id,gr_ptr)==FAIL)
        HGOTO_ERROR(DFE_INTERNAL,FAIL);

    /* Return handle to the GR interface to the user */
    ret_value=HAregister_atom(GRIDGROUP,gr_ptr);

#ifdef QAK
/* Dump the tree */
printf("%s: id=%ld\n",FUNC,(long)GRSLOT2ID(hdf_file_id,next_gr));
tbbtdump(gr_ptr->gattree,0);
#endif /* QAK */

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(GR_mask, ID_GRstart);
#endif /* HAVE_PABLO */
  return ret_value;
} /* end GRstart() */

/*--------------------------------------------------------------------------
 NAME
    GRfileinfo
 PURPOSE
    Report high-level information about the GR*() interface for a given file.
 USAGE
    intn GRfileinfo(grid, n_datasets, n_attrs)
        int32 grid;                 IN: GR ID to get information about
        int32 *n_datasets;          OUT: the # of GR datasets in a file
        int32 *n_attrs;             OUT: the # of "global" GR attributes
 RETURNS
    SUCCEED/FAIL
 DESCRIPTION
    Reports general information about the number of datasets and "global"
    attributes for the GR interface.  This routine is generally used to find
    the range of acceptable indices for GRselect calls.
 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
intn GRfileinfo(int32 grid,int32 *n_datasets,int32 *n_attrs)
{
    CONSTR(FUNC, "GRfileinfo");    /* for HERROR */
    gr_info_t *gr_ptr;          /* ptr to the GR information for a file */
    intn  ret_value = SUCCEED;

#ifdef HAVE_PABLO
    TRACE_ON(GR_mask, ID_GRfileinfo);
#endif /* HAVE_PABLO */
    /* clear error stack and check validity of file id */
    HEclear();

    /* check the validity of the GR ID */
    if (HAatom_group(grid)!=GRIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);
    
    /* locate GR's object in hash table */
    if (NULL == (gr_ptr = (gr_info_t *) HAatom_object(grid)))
        HGOTO_ERROR(DFE_NOVS, FAIL);

/* Get the number of datasets & global attributes from the memory structures */
    if(n_datasets!=NULL)
        *n_datasets=gr_ptr->gr_count;
    if(n_attrs!=NULL)
        *n_attrs=gr_ptr->gattr_count;
        
done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(GR_mask, ID_GRfileinfo);
#endif /* HAVE_PABLO */
  return ret_value;
} /* end GRfileinfo() */

/*--------------------------------------------------------------------------
 NAME
    GRIupdatemeta
 PURPOSE
    Internal routine to update the meta-data for an image
 USAGE
    intn GRIupdatemeta(hdf_file_id,img_ptr)
        int32 hdf_file_id;          IN: the file ID for the HDF file.
        ri_info_t *img_ptr;         IN: pointer to the image info for the
                                        image to write.
 RETURNS
    SUCCEED/FAIL

 DESCRIPTION
    Write out the meta-information about an image (& palette) to an HDF file.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
    Shared by both GRIupdateRIG() and GRIupdateRI() calls.
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
PRIVATE intn GRIupdatemeta(int32 hdf_file_id,ri_info_t *img_ptr)
{
    CONSTR(FUNC, "GRIupdatemeta");   /* for HERROR */
    uint8 GRtbuf[64];   /* local buffer for reading RIG info */
    uint8 ntstring[4];  /* temporary storage for the number type information */
    uint8 *p;           /* temporary pointer into buffer */
    intn  ret_value = SUCCEED;

    HEclear();
    if (!HDvalidfid(hdf_file_id) || img_ptr==NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* Weird test below to allow for tag/ref values of zero.  (I'll spare */
    /*  everyone my rant about why DFTAG_NULL should have been made zero */
    /*  instead of one... QAK) */
    if(img_ptr->img_dim.nt_tag<=DFTAG_NULL)
        img_ptr->img_dim.nt_tag=DFTAG_NT;
    if(img_ptr->img_dim.nt_ref==DFREF_WILDCARD)
        img_ptr->img_dim.nt_ref=Htagnewref(hdf_file_id,img_ptr->img_dim.nt_tag);
    
    /* Write out the raster image's number-type record */
    ntstring[0] = DFNT_VERSION;     /* version */
    ntstring[1] = (uint8)img_ptr->img_dim.nt;       /* type */
    ntstring[2] = DFKNTsize(img_ptr->img_dim.nt)*8; /* width: RIG data is 8-bit chars */
    ntstring[3] = DFNTC_BYTE;       /* class: data are numeric values */
    if (Hputelement(hdf_file_id, img_ptr->img_dim.nt_tag,
            img_ptr->img_dim.nt_ref, ntstring, (int32) 4) == FAIL)
        HGOTO_ERROR(DFE_PUTELEM, FAIL);
    
    /* Check for a palette with this image */
    if(img_ptr->lut_ref>DFREF_WILDCARD)
      {
          /* Write out the palette number-type */
          if(img_ptr->lut_dim.nt_tag<=DFTAG_NULL)
              img_ptr->lut_dim.nt_tag=DFTAG_NT;
          if(img_ptr->lut_dim.nt_ref==DFREF_WILDCARD)
              img_ptr->lut_dim.nt_ref=Htagnewref(hdf_file_id,img_ptr->lut_dim.nt_tag);
          ntstring[0] = DFNT_VERSION;     /* version */
          ntstring[1] = DFNT_UCHAR;       /* type */
          ntstring[2] = 8;                /* width: RIG data is 8-bit chars */
          ntstring[3] = DFNTC_BYTE;       /* class: data are numeric values */
          if (Hputelement(hdf_file_id, img_ptr->lut_dim.nt_tag,
                  img_ptr->lut_dim.nt_ref, ntstring, (int32)4) == FAIL)
              HGOTO_ERROR(DFE_PUTELEM, FAIL);

          /* Write out the palette dimensions */
          p = GRtbuf;
          INT32ENCODE(p, img_ptr->lut_dim.xdim);
          INT32ENCODE(p, img_ptr->lut_dim.ydim);
          UINT16ENCODE(p, img_ptr->lut_dim.nt_tag);
          UINT16ENCODE(p, img_ptr->lut_dim.nt_ref);
          INT16ENCODE(p, img_ptr->lut_dim.ncomps);
/* Currently all data is written out in 'pixel' interlace, so force the */
/* interlace stored on disk to match, instead of the interlacing that the */
/* user created the image with. -QAK  */
#ifdef LATER
          INT16ENCODE(p, img_ptr->lut_dim.il);
#else /* LATER */
          INT16ENCODE(p, (int16)MFGR_INTERLACE_PIXEL);
#endif /* LATER */
          UINT16ENCODE(p, img_ptr->lut_dim.comp_tag);
          UINT16ENCODE(p, img_ptr->lut_dim.comp_ref);
          if(img_ptr->lut_dim.dim_ref==DFREF_WILDCARD)
              img_ptr->lut_dim.dim_ref=Htagnewref(hdf_file_id,DFTAG_LD);
          if (Hputelement(hdf_file_id, DFTAG_LD, img_ptr->lut_dim.dim_ref, GRtbuf, (int32)(p-GRtbuf)) == FAIL)
              HGOTO_ERROR(DFE_PUTELEM, FAIL);
      } /* end if */

    /* Write out the image dimensions */
    p = GRtbuf;
    INT32ENCODE(p, img_ptr->img_dim.xdim);
    INT32ENCODE(p, img_ptr->img_dim.ydim);
    UINT16ENCODE(p, img_ptr->img_dim.nt_tag);
    UINT16ENCODE(p, img_ptr->img_dim.nt_ref);
    INT16ENCODE(p, img_ptr->img_dim.ncomps);
/* Currently all data is written out in 'pixel' interlace, so force the */
/* interlace stored on disk to match, instead of the interlacing that the */
/* user created the image with. -QAK  */
#ifdef LATER
    INT16ENCODE(p, img_ptr->img_dim.il);
#else /* LATER */
    INT16ENCODE(p, (int16)MFGR_INTERLACE_PIXEL);
#endif /* LATER */
    UINT16ENCODE(p, img_ptr->img_dim.comp_tag);
    UINT16ENCODE(p, img_ptr->img_dim.comp_ref);
    if(img_ptr->img_dim.dim_ref==DFREF_WILDCARD)
        img_ptr->img_dim.dim_ref=Htagnewref(hdf_file_id,DFTAG_ID);
    if (Hputelement(hdf_file_id, DFTAG_ID, img_ptr->img_dim.dim_ref, GRtbuf, (int32)(p-GRtbuf)) == FAIL)
        HGOTO_ERROR(DFE_PUTELEM, FAIL);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */

  return ret_value;
} /* end GRIupdatemeta() */

/*--------------------------------------------------------------------------
 NAME
    GRIupdateRIG
 PURPOSE
    Internal routine to update the RIG for an image
 USAGE
    intn GRIupdateRIG(hdf_file_id,img_ptr)
        int32 hdf_file_id;          IN: the file ID for the HDF file.
        ri_info_t *img_ptr;         IN: pointer to the image info for the
                                        image to write.
 RETURNS
    SUCCEED/FAIL

 DESCRIPTION
    Write out the RIG structure to an HDF file.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
PRIVATE intn GRIupdateRIG(int32 hdf_file_id,ri_info_t *img_ptr)
{
    CONSTR(FUNC, "GRIupdateRIG");   /* for HERROR */
    int32 GroupID;      /* RIG id for group interface */
    intn   ret_value = SUCCEED;

    HEclear();
    if (!HDvalidfid(hdf_file_id) || img_ptr==NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* Don't write out a RIG, unless it is compatible with the older RIGS */
    /*  which already exist.  This is to guarantee compatibility with older */
    /*  software, both application and in the library */
    if(img_ptr->img_dim.nt!=DFNT_UINT8 || (img_ptr->img_dim.ncomps!=1
            && img_ptr->img_dim.ncomps!=3))
      {
        ret_value =(SUCCEED); /* lie and say it's ok - QAK */
        goto done;
      }

#ifdef QAK
printf("%s: writing RIG\n",FUNC);
#endif /* QAK */

    /* Write out the image/palette meta-information */
    if (GRIupdatemeta(hdf_file_id,img_ptr)==FAIL)
        HGOTO_ERROR(DFE_INTERNAL, FAIL);

    /* Write out the RIG itself */
    if ((GroupID = DFdisetup(6)) == FAIL)    /* max 6 tag/refs in set */
        HGOTO_ERROR(DFE_GROUPSETUP, FAIL);

    /* add image dimension tag/ref to RIG */
    if (DFdiput(GroupID, DFTAG_ID, (uint16) img_ptr->img_dim.dim_ref) == FAIL)
        HGOTO_ERROR(DFE_PUTGROUP, FAIL);

    /* add image data tag/ref to RIG */
    if (DFdiput(GroupID, img_ptr->img_tag, img_ptr->img_ref) == FAIL)
        HGOTO_ERROR(DFE_PUTGROUP, FAIL);

    /* Check if we should write palette information */
    if(img_ptr->lut_ref>DFREF_WILDCARD)
      {
          /* add palette dimension tag/ref to RIG */
          if (DFdiput(GroupID, DFTAG_LD, (uint16) img_ptr->lut_dim.dim_ref) == FAIL)
              HGOTO_ERROR(DFE_PUTGROUP, FAIL);

          /* add palette data tag/ref to RIG */
          if (DFdiput(GroupID, img_ptr->lut_tag, img_ptr->lut_ref) == FAIL)
              HGOTO_ERROR(DFE_PUTGROUP, FAIL);
      } /* end if */

    /* write out RIG */
    if(img_ptr->rig_ref==DFTAG_WILDCARD)
        img_ptr->rig_ref=Htagnewref(hdf_file_id,DFTAG_RIG);
    if(DFdiwrite(hdf_file_id, GroupID, DFTAG_RIG, img_ptr->rig_ref)==FAIL)
        HGOTO_ERROR(DFE_GROUPWRITE, FAIL);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */

  return ret_value;
} /* end GRIupdateRIG() */

/*--------------------------------------------------------------------------
 NAME
    GRIupdateRI
 PURPOSE
    Internal routine to update the RI Vgroup for an image
 USAGE
    intn GRIupdateRI(hdf_file_id,img_ptr)
        int32 hdf_file_id;          IN: the file ID for the HDF file.
        ri_info_t *img_ptr;         IN: pointer to the image info for the
                                        image to write.
 RETURNS
    SUCCEED/FAIL

 DESCRIPTION
    Write out the RI Vgroup to an HDF file.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
PRIVATE intn GRIupdateRI(int32 hdf_file_id,ri_info_t *img_ptr)
{
    CONSTR(FUNC, "GRIupdateRI");   /* for HERROR */
    int32 GroupID;      /* RI vgroup id */
    intn  ret_value = SUCCEED;

    HEclear();
    if (!HDvalidfid(hdf_file_id) || img_ptr==NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

#ifdef QAK
printf("%s: check 1.0\n",FUNC);
#endif /* QAK */
    /* Write out the image/palette meta-information */
    if (GRIupdatemeta(hdf_file_id,img_ptr)==FAIL)
        HGOTO_ERROR(DFE_INTERNAL, FAIL);

    /* Write out the RI Vgroup itself */
    if ((GroupID = Vattach(hdf_file_id,(img_ptr->ri_ref>DFREF_WILDCARD ?
            img_ptr->ri_ref : -1),"w")) == FAIL)
        HGOTO_ERROR(DFE_CANTATTACH, FAIL);

#ifdef QAK
printf("%s: check 2.0, GroupID=%ld\n",FUNC,(long)GroupID);
#endif /* QAK */
    /* grab the ref. # of the new Vgroup */
    if(img_ptr->ri_ref==DFREF_WILDCARD)
        img_ptr->ri_ref=(uint16)VQueryref(GroupID);

    /* Set the name of the RI */
    if(img_ptr->name!=NULL)
        if(Vsetname(GroupID,img_ptr->name)==FAIL)
            HGOTO_ERROR(DFE_BADVSNAME,FAIL);

    /* Set the class of the RI group */
    if(Vsetclass(GroupID,RI_NAME)==FAIL)
        HGOTO_ERROR(DFE_BADVSNAME,FAIL);

    /* add image dimension tag/ref to RIG */
    if (Vaddtagref(GroupID, DFTAG_ID, (uint16) img_ptr->img_dim.dim_ref) == FAIL)
        HGOTO_ERROR(DFE_CANTADDELEM, FAIL);

    /* If we don't have a tag for the image, just use DFTAG_RI for now */
    if(img_ptr->img_tag==DFTAG_NULL)
        img_ptr->img_tag=DFTAG_RI;

    /* add image data tag/ref to RIG */
    if (Vaddtagref(GroupID, img_ptr->img_tag, img_ptr->img_ref) == FAIL)
        HGOTO_ERROR(DFE_CANTADDELEM, FAIL);

    /* Check if we should write palette information */
    if(img_ptr->lut_ref>DFREF_WILDCARD)
      {
          /* add palette dimension tag/ref to RIG */
          if (Vaddtagref(GroupID, DFTAG_LD, (uint16) img_ptr->lut_dim.dim_ref) == FAIL)
              HGOTO_ERROR(DFE_CANTADDELEM, FAIL);

          /* add palette data tag/ref to RIG */
          if (Vaddtagref(GroupID, img_ptr->lut_tag, img_ptr->lut_ref) == FAIL)
              HGOTO_ERROR(DFE_CANTADDELEM, FAIL);
      } /* end if */

    /* write out RIG */
    if(Vdetach(GroupID)==FAIL)
        HGOTO_ERROR(DFE_CANTDETACH, FAIL);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */

  return ret_value;
} /* end GRIupdateRI() */

/*--------------------------------------------------------------------------
 NAME
    GRIup_attr_data
 PURPOSE
    Internal routine to update/create the attribute data
 USAGE
    intn GRIup_attr_data(hdf_file_id,attr_ptr)
        int32 hdf_file_id;          IN: the file ID for the HDF file.
        at_info_t *attr_ptr;        IN: pointer to the attribute info for the
                                        attr. to write.
 RETURNS
    SUCCEED/FAIL

 DESCRIPTION
    Write out the data for an attribute to an HDF file.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
PRIVATE intn GRIup_attr_data(int32 hdf_file_id,at_info_t *attr_ptr)
{
    CONSTR(FUNC, "GRIup_attr_data");   /* for HERROR */
    intn   ret_value = SUCCEED;

    HEclear();
    if (!HDvalidfid(hdf_file_id) || attr_ptr==NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

#ifdef QAK
printf("%s: attr_ptr->ref=%u\n",FUNC,attr_ptr->ref);
#endif /* QAK */
    /* Write out the attribute data */
    if (attr_ptr->ref==DFREF_WILDCARD)  /* create a new attribute */
      {
        if((attr_ptr->ref=(uint16)VHstoredata(hdf_file_id,attr_ptr->name,attr_ptr->data,
                attr_ptr->len,attr_ptr->nt,RIGATTRNAME,RIGATTRCLASS))==(uint16)FAIL)
            HGOTO_ERROR(DFE_VSCANTCREATE,FAIL);
        attr_ptr->new_at=TRUE;
      } /* end if */
    else    /* update an existing one */
      {
        int32 AttrID;       /* attribute Vdata id */

        if((AttrID=VSattach(hdf_file_id,attr_ptr->ref,"w"))==FAIL)
            HGOTO_ERROR(DFE_CANTATTACH,FAIL);
        if(VSsetfields(AttrID,attr_ptr->name)==FAIL)
          {
            VSdetach(AttrID);
            HGOTO_ERROR(DFE_BADFIELDS,FAIL);
          } /* end if */
        if(VSwrite(AttrID,attr_ptr->data,attr_ptr->len,FULL_INTERLACE)==FAIL)
          {
            VSdetach(AttrID);
            HGOTO_ERROR(DFE_VSWRITE,FAIL);
          } /* end if */
        if(VSdetach(AttrID)==FAIL)
            HGOTO_ERROR(DFE_CANTDETACH,FAIL);
      } /* end else */

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */

  return ret_value;
} /* end GRIup_attr_data() */

/*--------------------------------------------------------------------------
 NAME
    GRend
 PURPOSE
    Terminate the GR*() interface for a given HDF file.
 USAGE
    intn GRend(grid)
        int32 grid;          IN: GR ID from GRstart
 RETURNS
    SUCCEED/FAIL

 DESCRIPTION
    Terminates access to the GR*() interface for a specific file.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
intn GRend(int32 grid)
{
    CONSTR(FUNC, "GRend");      /* for HERROR */
    int32 hdf_file_id;          /* HDF file ID */
    int32 GroupID;              /* VGroup ID for the GR group */
    gr_info_t *gr_ptr;          /* ptr to the GR information for this grid */
    filerec_t *file_rec;        /* File record */
    VOIDP      *t1;
    intn   ret_value = SUCCEED;

#ifdef HAVE_PABLO
    TRACE_ON(GR_mask, ID_GRend);
#endif /* HAVE_PABLO */
    /* clear error stack and check validity of file id */
    HEclear();

    /* check the validity of the GR ID */
    if (HAatom_group(grid)!=GRIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);
    
    /* locate GR's object in hash table */
    if (NULL == (gr_ptr = (gr_info_t *) HAatom_object(grid)))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    if (--gr_ptr->access)
        HGOTO_DONE(SUCCEED);

    hdf_file_id=gr_ptr->hdf_file_id;
    file_rec = HAatom_object(hdf_file_id);

    if(((file_rec->access)&DFACC_WRITE)!=0)
      {
        /* Check if the GR group exists, and create it if not */
        if(gr_ptr->gr_ref==DFREF_WILDCARD)
          {
            if((GroupID=Vattach(gr_ptr->hdf_file_id,-1,"w"))==FAIL)
                HGOTO_ERROR(DFE_CANTATTACH,FAIL);
            if((gr_ptr->gr_ref=(uint16)VQueryref(GroupID))==(uint16)FAIL)
                HGOTO_ERROR(DFE_NOVALS,FAIL);
            if(Vsetname(GroupID,GR_NAME)==FAIL)
                HGOTO_ERROR(DFE_BADVSNAME,FAIL);
            if(Vsetclass(GroupID,GR_NAME)==FAIL)
                HGOTO_ERROR(DFE_BADVSNAME,FAIL);
          } /* end if */
        else
          {
            if((GroupID=Vattach(gr_ptr->hdf_file_id,gr_ptr->gr_ref,"w"))==FAIL)
                HGOTO_ERROR(DFE_CANTATTACH,FAIL);
          } /* end else */

        /* Write out the information for RIs which have been changed */
#ifdef QAK
printf("%s: gr_modified=%d, gr_count=%ld\n",FUNC,gr_ptr->gr_modified,(long)gr_ptr->gr_count);
#endif /* QAK */
        if(gr_ptr->gr_modified==TRUE && gr_ptr->gr_count>0)
          {
              VOIDP      *t2;
              ri_info_t *img_ptr;   /* ptr to the image */

#ifdef QAK
printf("%s: gr_modified=%d, gr_count=%ld\n",FUNC,gr_ptr->gr_modified,(long)gr_ptr->gr_count);
#endif /* QAK */
              if (NULL == (t2 = (VOIDP *) tbbtfirst((TBBT_NODE *) * (gr_ptr->grtree))))
                {
                    HGOTO_ERROR(DFE_NOTINTABLE, FAIL);
                } /* end if */
              else
                  img_ptr = (ri_info_t *) * t2;   /* get actual pointer to the ri_info_t */

              /* cycle through all of the images in memory */
              while (t2!=NULL)
                {
#ifdef QAK
printf("%s: data_modified=%d, meta_modified=%d, attr_modified=%d\n",FUNC,img_ptr->data_modified,img_ptr->meta_modified,img_ptr->attr_modified);
printf("%s: ri_ref=%u\n",FUNC,img_ptr->ri_ref);
#endif /* QAK */
                    /* check if the image data has been modified */
                    if(img_ptr->data_modified==TRUE)
                      {
                        /* do nothing currently, we are synchronously updating the image data */
                          img_ptr->data_modified=FALSE;
                      } /* end if */

                    /* check if the image meta-info has been modified */
                    if(img_ptr->meta_modified==TRUE)
                      {
                          /* write out the RI/RIG information again */
                          if(GRIupdateRIG(gr_ptr->hdf_file_id,img_ptr)==FAIL)
                              HGOTO_ERROR(DFE_INTERNAL, FAIL);
                          if(GRIupdateRI(gr_ptr->hdf_file_id,img_ptr)==FAIL)
                              HGOTO_ERROR(DFE_INTERNAL, FAIL);
                          img_ptr->meta_modified=FALSE;
                      } /* end if */

                    /* check if the local attributes has been modified */
                    if(img_ptr->attr_modified==TRUE && img_ptr->lattr_count>0)
                      {
                          VOIDP      *t3;
                          at_info_t *attr_ptr;   /* ptr to the attribute */

                          if (NULL == (t3 = (VOIDP *) tbbtfirst((TBBT_NODE *) * (img_ptr->lattree))))
                            {
                                HGOTO_ERROR(DFE_NOTINTABLE, FAIL);
                            } /* end if */
                          else
                              attr_ptr = (at_info_t *) * t3;   /* get actual pointer to the at_info_t */

                          /* cycle through all of the local attributes in memory */
                          while (t3!=NULL)
                            {
                                /* check if the attribute data has been modified */
                                if(attr_ptr->data_modified==TRUE)
                                  {
                                      if(GRIup_attr_data(gr_ptr->hdf_file_id,attr_ptr)==FAIL)
                                          HGOTO_ERROR(DFE_INTERNAL, FAIL);
                                      attr_ptr->data_modified=FALSE;
                                  } /* end if */

#ifdef QAK
printf("%s: ri_ref=%u, atptr->ref=%u\n",FUNC,img_ptr->ri_ref,attr_ptr->ref);
#endif /* QAK */
                                /* check if the attribute was added to the group */
                                if(attr_ptr->new_at==TRUE)
                                  {
                                      int32 lGroupID;  /* ID of the Vgroup */

                                      if((lGroupID=Vattach(gr_ptr->hdf_file_id,img_ptr->ri_ref,"w"))==FAIL)
                                          HGOTO_ERROR(DFE_CANTATTACH,FAIL);
                                      if(Vaddtagref(lGroupID,ATTR_TAG,attr_ptr->ref)==FAIL)
                                          HGOTO_ERROR(DFE_CANTADDELEM,FAIL);
                                      if(Vdetach(lGroupID)==FAIL)
                                          HGOTO_ERROR(DFE_CANTDETACH,FAIL);
                                      attr_ptr->new_at=FALSE;
                                  } /* end if */

                                /* get the next local attribute in the tree/list */
                                if (NULL != (t3 = (VOIDP *) tbbtnext((TBBT_NODE *) t3)))     /* get the next node in the tree */
                                    attr_ptr = (at_info_t *) * t3;     /* get actual pointer to the at_info_t */
                            } /* end while */
                          img_ptr->attr_modified=FALSE;
                      } /* end if */

                    /* Check if the RI is already in the GR, add it if not */
                    if(Vinqtagref(GroupID,RI_TAG,img_ptr->ri_ref)==FALSE)
                        if(Vaddtagref(GroupID,RI_TAG,img_ptr->ri_ref)==FAIL)
                            HGOTO_ERROR(DFE_CANTADDELEM,FAIL);

                    /* get the next image in the tree/list */
                    if (NULL != (t2 = (VOIDP *) tbbtnext((TBBT_NODE *) t2)))     /* get the next node in the tree */
                        img_ptr = (ri_info_t *) * t2;     /* get actual pointer to the ri_info_t */
                } /* end while */
          } /* end if */

#ifdef QAK
printf("%s: gr_ptr=%p, gr_ptr->gattr_modified=%d, gr_ptr->gattr_count=%ld\n",FUNC,gr_ptr,gr_ptr->gattr_modified,gr_ptr->gattr_count);
#endif /* QAK */
        /* Write out the information for the global attributes which have been changed */
        if(gr_ptr->gattr_modified==TRUE && gr_ptr->gattr_count>0)
          {
              VOIDP      *t2;
              at_info_t *attr_ptr;   /* ptr to the attribute */

              if (NULL == (t2 = (VOIDP *) tbbtfirst((TBBT_NODE *) * (gr_ptr->gattree))))
                {
                    HGOTO_ERROR(DFE_NOTINTABLE, FAIL);
                } /* end if */
              else
                  attr_ptr = (at_info_t *) * t2;   /* get actual pointer to the at_info_t */

#ifdef QAK
printf("%s: attr_ptr=%p\n",FUNC,attr_ptr);
#endif /* QAK */
              /* cycle through all of the global attributes in memory */
              while (t2!=NULL)
                {
#ifdef QAK
printf("%s: attr_ptr->data_modified=%d\n",FUNC,attr_ptr->data_modified);
#endif /* QAK */
                    /* check if the attribute data has been modified */
                    if(attr_ptr->data_modified==TRUE)
                      {
                        if(GRIup_attr_data(gr_ptr->hdf_file_id,attr_ptr)==FAIL)
                            HGOTO_ERROR(DFE_INTERNAL, FAIL);
                        attr_ptr->data_modified=FALSE;

#ifdef QAK
printf("%s: attr_ptr->new_at=%d\n",FUNC,attr_ptr->new_at);
#endif /* QAK */
                        /* check if the attribute was a new attribute */
                        if(attr_ptr->new_at==TRUE)
                          {
#ifdef QAK
printf("%s: GroupID=%ld\n",FUNC,(long)GroupID);
#endif /* QAK */
                            if(Vaddtagref(GroupID,ATTR_TAG,attr_ptr->ref)==FAIL)
                                HGOTO_ERROR(DFE_CANTADDELEM,FAIL);
                            attr_ptr->new_at=FALSE;
                          } /* end if */
                      } /* end if */

                    /* get the next global attribute in the tree/list */
                    if (NULL != (t2 = (VOIDP *) tbbtnext((TBBT_NODE *) t2)))     /* get the next node in the tree */
                        attr_ptr = (at_info_t *) * t2;     /* get actual pointer to the at_info_t */
                } /* end while */
            gr_ptr->gattr_modified=FALSE;
          } /* end if */

        /* Let go of the GR Vgroup */
        if(Vdetach(GroupID)==FAIL)
            HGOTO_ERROR(DFE_CANTDETACH,FAIL);
      } /* end if */

    /* Free all the memory we've allocated */
    tbbtdfree(gr_ptr->grtree, GRIridestroynode, NULL);
    tbbtdfree(gr_ptr->gattree, GRIattrdestroynode, NULL);

    /* Close down the entry for this file in the GR tree */
    /* Find the node in the tree */
    if (( t1 = (VOIDP *) tbbtdfind(gr_tree, (VOIDP) &hdf_file_id, NULL)) == NULL)
        HGOTO_DONE(FAIL);

    /* Delete the node and free the gr_info_t stucture */
    tbbtrem((TBBT_NODE **) gr_tree, (TBBT_NODE *) t1, NULL);
    HDfree(gr_ptr);

    /* Close down the Vset routines we started */
    if(Vend(hdf_file_id)==FAIL)
        HGOTO_ERROR(DFE_CANTSHUTDOWN,FAIL);

    if (NULL == HAremove_atom(grid))
        HGOTO_ERROR(DFE_INTERNAL, FAIL);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(GR_mask, ID_GRend);
#endif /* HAVE_PABLO */
  return ret_value;
} /* end GRend() */

/*--------------------------------------------------------------------------
 NAME
    GRselect
 PURPOSE
    Select a raster image to operate on.
 USAGE
    int32 GRselect(grid,index)
        int32 grid;          IN: GR ID from GRstart
        int32 index;         IN: Which raster image to select (indexed from 0)
 RETURNS
    A valid riid (Raster-Image ID) on success, or FAIL.

 DESCRIPTION
    Selects a raster image from the file to work on.  This ID is needed for
    all operations on the image dataset, including reading/writing data,
    annotations, etc.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
int32 GRselect(int32 grid,int32 index)
{
    CONSTR(FUNC, "GRselect");   /* for HERROR */
    gr_info_t *gr_ptr;          /* ptr to the GR information for this grid */
    ri_info_t *ri_ptr;          /* ptr to the image to work with */
    VOIDP *t;                   /* temp. ptr to the image found */
    int32  ret_value = SUCCEED;

#ifdef HAVE_PABLO
    TRACE_ON(GR_mask, ID_GRselect);
#endif /* HAVE_PABLO */
    /* clear error stack and check validity of file id */
    HEclear();

    /* check the validity of the GR ID */
    if (HAatom_group(grid)!=GRIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);
    
    /* locate GR's object in hash table */
    if (NULL == (gr_ptr = (gr_info_t *) HAatom_object(grid)))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    /* check the index range validity */
    if(!VALIDRIINDEX(index,gr_ptr))
        HGOTO_ERROR(DFE_ARGS, FAIL);

    if((t = (VOIDP *) tbbtdfind(gr_ptr->grtree, (VOIDP) &index, NULL))==NULL)
        HGOTO_ERROR(DFE_RINOTFOUND,FAIL);
    ri_ptr=(ri_info_t *)*t;

    ri_ptr->access++;

    ret_value=HAregister_atom(RIIDGROUP,ri_ptr);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(GR_mask, ID_GRselect);
#endif /* HAVE_PABLO */
  return ret_value;
} /* end GRselect() */

/*--------------------------------------------------------------------------
 NAME
    GRcreate

 PURPOSE
    Create a new raster image.

 USAGE
    int32 GRcreate(grid, name, ncomp, nt, il, dimsizes)
        int32 grid;         IN: GR ID from GRstart
        char *name;         IN: Name of raster image to create
        int32 ncomp;        IN: Number of components in image
        int32 nt;           IN: Number type of each component
        int32 il;           IN: Interlace of the components in the image
        int32 dimsizes[2];  IN: Dimensions of the new image

 RETURNS
    A valid riid (Raster-Image ID) on success, or FAIL.

 DESCRIPTION
    Creates a new raster image in a file.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
    All components must be the same number-type.

 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
int32 GRcreate(int32 grid,const char *name,int32 ncomp,int32 nt,int32 il,int32 dimsizes[2])
{
    CONSTR(FUNC, "GRcreate");   /* for HERROR */
    int32 GroupID;              /* ID of the Vgroup created */
    gr_info_t *gr_ptr;          /* ptr to the GR information for this grid */
    ri_info_t *ri_ptr;          /* ptr to the image to work with */
    int32    ret_value = SUCCEED;

#ifdef HAVE_PABLO
    TRACE_ON(GR_mask, ID_GRcreate);
#endif /* HAVE_PABLO */
    /* clear error stack */
    HEclear();

    /* check the validity of the args */
    if (HAatom_group(grid)!=GRIDGROUP || name==NULL || ncomp<1
            || (il!=MFGR_INTERLACE_PIXEL && il!=MFGR_INTERLACE_LINE 
            && il!=MFGR_INTERLACE_COMPONENT) || dimsizes==NULL 
            || dimsizes[0]<=0 || dimsizes[1]<=0)
        HGOTO_ERROR(DFE_ARGS, FAIL);
    
    /* locate GR's object in hash table */
    if (NULL == (gr_ptr = (gr_info_t *) HAatom_object(grid)))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    /* Allocate space for the new image information */
    if((ri_ptr=(ri_info_t *)HDmalloc(sizeof(ri_info_t)))==NULL)
        HGOTO_ERROR(DFE_NOSPACE,FAIL);
    HDmemset(ri_ptr,0,sizeof(ri_info_t));
    
    /* Allocate space for the name and copy it */
    if((ri_ptr->name=(char *)HDmalloc(HDstrlen(name)+1))==NULL)
        HGOTO_ERROR(DFE_NOSPACE,FAIL);
    HDstrcpy(ri_ptr->name,name);

    /* Assign image information */
    ri_ptr->index=gr_ptr->gr_count;
#ifdef OLD_WAY
    ri_ptr->ri_ref=DFREF_WILDCARD;
#else /* OLD_WAY */
    if ((GroupID = Vattach(gr_ptr->hdf_file_id,-1,"w")) == FAIL)
        HGOTO_ERROR(DFE_CANTATTACH, FAIL);
    ri_ptr->ri_ref=(uint16)VQueryref(GroupID);
    if(Vdetach(GroupID)==FAIL)
        HGOTO_ERROR(DFE_CANTDETACH, FAIL);
#endif /* OLD_WAY */
    ri_ptr->rig_ref=DFREF_WILDCARD;
    ri_ptr->gr_ptr=gr_ptr;
    ri_ptr->img_dim.dim_ref=DFREF_WILDCARD;
    ri_ptr->img_dim.xdim=dimsizes[XDIM];
    ri_ptr->img_dim.ydim=dimsizes[YDIM];
    ri_ptr->img_dim.ncomps=ncomp;
    ri_ptr->img_dim.nt=nt;
    ri_ptr->img_dim.file_nt_subclass=DFNTF_HDFDEFAULT;
    ri_ptr->img_dim.il=il;
    ri_ptr->img_dim.nt_tag=ri_ptr->img_dim.nt_ref=DFREF_WILDCARD;
    ri_ptr->img_dim.comp_tag=ri_ptr->img_dim.comp_ref=DFREF_WILDCARD;
    ri_ptr->img_tag=ri_ptr->img_ref=DFREF_WILDCARD;
    ri_ptr->lut_tag=ri_ptr->lut_ref=DFREF_WILDCARD;
    ri_ptr->im_il=MFGR_INTERLACE_PIXEL;
    ri_ptr->lut_il=MFGR_INTERLACE_PIXEL;
    ri_ptr->data_modified=FALSE;
    ri_ptr->meta_modified=TRUE;
    ri_ptr->attr_modified=FALSE;
    ri_ptr->lattr_count=0;
    ri_ptr->lattree = tbbtdmake(rigcompare, sizeof(int32));
    if(ri_ptr->lattree==NULL)
        HGOTO_ERROR(DFE_NOSPACE, FAIL);
    ri_ptr->comp_img=FALSE;
    ri_ptr->ext_img=FALSE;
    ri_ptr->acc_img=FALSE;
    ri_ptr->fill_img=TRUE;
    ri_ptr->fill_value=NULL;
    ri_ptr->store_fill=FALSE;
    ri_ptr->access++;

    /* insert the new image in the global image tree */
    tbbtdins(gr_ptr->grtree, (VOIDP) ri_ptr, NULL);    /* insert the new image into B-tree */ 

    /* indicate that the GR info has changed */
    gr_ptr->gr_modified=TRUE;
    gr_ptr->gr_count++;

    ret_value=HAregister_atom(RIIDGROUP,ri_ptr);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(GR_mask, ID_GRcreate);
#endif /* HAVE_PABLO */
  return ret_value;
} /* end GRcreate() */

/*--------------------------------------------------------------------------
 NAME
    GRnametoindex
 PURPOSE
    Map a raster image name to an index.
 USAGE
    int32 GRnametoindex(grid,name)
        int32 grid;          IN: GR ID from GRstart
        char *name;          IN: Name of raster image to search for
 RETURNS
    A valid index on success, or FAIL.

 DESCRIPTION
    Searches for a raster image based on the name provided.  This routine
    maps from names of raster images to indices inside the GR group.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
int32 GRnametoindex(int32 grid,char *name)
{
    CONSTR(FUNC, "GRnametoindex");   /* for HERROR */
    gr_info_t *gr_ptr;          /* ptr to the GR information for this grid */
    ri_info_t *ri_ptr;          /* ptr to the image to work with */
    VOIDP *t;                   /* temp. ptr to the image found */
    int32   ret_value = SUCCEED;

#ifdef HAVE_PABLO
    TRACE_ON(GR_mask, ID_GRnametoindex);
#endif /* HAVE_PABLO */
    /* clear error stack and check validity of file id */
    HEclear();

    /* check the validity of the GR ID */
    if (HAatom_group(grid)!=GRIDGROUP || name==NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);
    
    /* locate GR's object in hash table */
    if (NULL == (gr_ptr = (gr_info_t *) HAatom_object(grid)))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    if((t = (VOIDP *) tbbtfirst((TBBT_NODE *)* (gr_ptr->grtree)))==NULL)
        HGOTO_ERROR(DFE_RINOTFOUND,FAIL);
    do {
        ri_ptr=(ri_info_t *)*t;
        if(ri_ptr!=NULL && HDstrcmp(ri_ptr->name,name)==0)  /* ie. the name matches */
            HGOTO_DONE(ri_ptr->index);
    } while((t= (VOIDP *) tbbtnext((TBBT_NODE *)t))!=NULL);

    ret_value = (FAIL);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(GR_mask, ID_GRnametoindex);
#endif /* HAVE_PABLO */
  return ret_value;
} /* end GRnametoindex() */

/*--------------------------------------------------------------------------
 NAME
    GRgetiminfo

 PURPOSE
    Gets information about a raster image.

 USAGE
    intn GRgetiminfo(riid,name,ncomp,nt,il,dimsizes,n_attr)
        int32 riid;         IN: RI ID from GRselect/GRcreate
        char *name;         OUT: name of raster image
        int32 *ncomp;       OUT: number of components in image
        int32 *nt;          OUT: number type of components
        int32 *il;          OUT: interlace of the image
        int32 *dimsizes;    OUT: size of each dimension
        int32 *n_attr;      OUT: the number of attributes for the image

 RETURNS
    SUCCEED/FAIL

 DESCRIPTION
    Looks up information about an image which has been selected or created
    with the GR routines.  Each of the parameters can be NULL, in which case
    that piece of information will not be retrieved.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
intn GRgetiminfo(int32 riid,char *name,int32 *ncomp,int32 *nt,int32 *il,
    int32 dimsizes[2],int32 *n_attr)
{
    CONSTR(FUNC, "GRgetiminfo");   /* for HERROR */
    gr_info_t *gr_ptr;          /* ptr to the GR information for this grid */
    ri_info_t *ri_ptr;          /* ptr to the image to work with */
    intn  ret_value = SUCCEED;

#ifdef HAVE_PABLO
    TRACE_ON(GR_mask, ID_GRgetiminfo);
#endif /* HAVE_PABLO */
    /* clear error stack and check validity of args */
    HEclear();

    /* check the validity of the RI ID */
    if (HAatom_group(riid)!=RIIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);
    
    /* locate RI's object in hash table */
    if (NULL == (ri_ptr = (ri_info_t *) HAatom_object(riid)))
        HGOTO_ERROR(DFE_NOVS, FAIL);
    gr_ptr=ri_ptr->gr_ptr;

    if(name!=NULL)
        HDstrcpy(name, ri_ptr->name);

    if(ncomp!=NULL)
        *ncomp=ri_ptr->img_dim.ncomps;

    if(nt!=NULL)
        *nt=ri_ptr->img_dim.nt;

    if(il!=NULL)
        *il=ri_ptr->img_dim.il;

    if(dimsizes!=NULL)
      {
          dimsizes[0]=ri_ptr->img_dim.xdim;
          dimsizes[1]=ri_ptr->img_dim.ydim;
      } /* end if */

    if(n_attr!=NULL)
        *n_attr=ri_ptr->lattr_count;

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(GR_mask, ID_GRgetiminfo);
#endif /* HAVE_PABLO */
  return ret_value;
} /* end GRgetiminfo() */

/*--------------------------------------------------------------------------
 NAME
    GRwriteimage

 PURPOSE
    Writes raster data to an image

 USAGE
    intn GRwriteimage(riid,start,stride,edge,data)
        int32 riid;         IN: RI ID from GRselect/GRcreate
        int32 start[2];     IN: array containing the offset in the image of the
                                image data to write out
        int32 stride[2];    IN: array containing interval of data being written
                                along each edge.  strides of 0 are illegal
                                (and generate an error)
                                ie. stride of 1 in each dimension means
                                writing contiguous data, stride of 2 means
                                writing every other element out along an edge.
        int32 count[2];     IN: number of elements to write out along each edge.
        VOIDP data;         IN: pointer to the data to write out.

 RETURNS
    SUCCEED/FAIL

 DESCRIPTION
    Writes image data to an RI.  Partial dataset writing and subsampling is
        allowed, but only within the dimensions of the dataset (ie. no UNLIMITED
        dimension support)

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
    If the stride parameter is set to NULL, a stride of 1 will be assumed.

 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
intn GRwriteimage(int32 riid,int32 start[2],int32 in_stride[2],int32 count[2],VOIDP data)
{
    CONSTR(FUNC, "GRwriteimage");   /* for HERROR */
    int32 hdf_file_id;          /* HDF file ID */
    int32 stride[2];            /* pointer to the stride array */
    gr_info_t *gr_ptr;          /* ptr to the GR information for this grid */
    ri_info_t *ri_ptr;          /* ptr to the image to work with */
    intn solid_block=FALSE;     /* whether the image data is a solid block of data */
    intn whole_image=FALSE;     /* whether we are writing out the whole image */
    VOIDP *img_data;            /* pointer to the converted image data to write */
    uintn pixel_mem_size,       /* size of a pixel in memory */
        pixel_disk_size;        /* size of a pixel on disk */
    intn convert=FALSE;         /* true if machine NT != NT to be written */
    uint8 platnumsubclass;      /* class of this NT for this platform */
    intn new_image=FALSE;       /* whether we are writing a new image out */
    intn switch_interlace=FALSE;/* whether the memory interlace needs to be switched around */
    intn ret_value = SUCCEED;

#ifdef HAVE_PABLO
    TRACE_ON(GR_mask, ID_GRwriteimage);
#endif /* HAVE_PABLO */
    /* clear error stack and check validity of args */
    HEclear();

#ifdef QAK
printf("%s: check 1\n",FUNC);
#endif /* QAK */
    /* check the basic validity of the args (stride is OK to be NULL) */
    if (HAatom_group(riid)!=RIIDGROUP || start==NULL /* || in_stride==NULL */ || count==NULL
            || data==NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);
    
    /* Get the correct parameters into the stride array */
    if(in_stride==NULL)
          stride[XDIM]=stride[YDIM]=1;
    else
      {
          stride[XDIM]=in_stride[XDIM];
          stride[YDIM]=in_stride[YDIM];
      } /* end else */

    /* Sanity check the start, stride, and count args */
    if((start[XDIM]<0 || start[YDIM]<0) || (stride[XDIM]<1 || stride[YDIM]<1)
            || (count[XDIM]<1 || count[YDIM]<1))
        HGOTO_ERROR(DFE_BADDIM, FAIL);

#ifdef QAK
printf("%s: check 2, stride[XDIM,YDIM]=%ld, %ld\n",FUNC,stride[XDIM],stride[YDIM]);
#endif /* QAK */
#ifdef QAK
printf("%s: riid=%ld\n",FUNC,(long)riid);
printf("%s: start={%ld,%ld}\n",FUNC,(long)start[XDIM],(long)start[YDIM]);
printf("%s: stride={%ld,%ld}\n",FUNC,(long)stride[XDIM],(long)stride[YDIM]);
printf("%s: count={%ld,%ld}\n",FUNC,(long)count[XDIM],(long)count[YDIM]);
printf("%s: data=%p\n",FUNC,data);
#endif /* QAK */

    /* locate RI's object in hash table */
    if (NULL == (ri_ptr = (ri_info_t *) HAatom_object(riid)))
        HGOTO_ERROR(DFE_NOVS, FAIL);
    gr_ptr=ri_ptr->gr_ptr;
    hdf_file_id=gr_ptr->hdf_file_id;

#ifdef QAK
printf("%s: stride[XDIM,YDIM]=%ld, %ld\n",FUNC,stride[XDIM],stride[YDIM]);
printf("%s: start[XDIM,YDIM]=%ld, %ld\n",FUNC,start[XDIM],start[YDIM]);
printf("%s: count[XDIM,YDIM]=%ld, %ld\n",FUNC,count[XDIM],count[YDIM]);
#endif /* QAK */
    if(stride[XDIM]==1 && stride[YDIM]==1)
      { /* solid block of data */
          solid_block=TRUE;
          if((start[XDIM]==0 && start[YDIM]==0) && (count[XDIM]==ri_ptr->img_dim.xdim
                && count[YDIM]==ri_ptr->img_dim.ydim))
              whole_image=TRUE;
          else
              whole_image=FALSE;
      } /* end if */
    else /* block of data spread out with strides */
          solid_block=FALSE;

    if(ri_ptr->img_dim.il!=MFGR_INTERLACE_PIXEL)
        switch_interlace=TRUE;

#ifdef QAK
printf("%s: check 3\n",FUNC);
#endif /* QAK */
    /* Get the size of the pixels in memory and on disk */
    pixel_mem_size=ri_ptr->img_dim.ncomps*DFKNTsize((ri_ptr->img_dim.nt | DFNT_NATIVE) & (~DFNT_LITEND));
    pixel_disk_size=ri_ptr->img_dim.ncomps*DFKNTsize(ri_ptr->img_dim.nt);

    /* Get number-type and conversion information */
    platnumsubclass = DFKgetPNSC(ri_ptr->img_dim.nt & (~DFNT_LITEND), DF_MT);
#ifdef QAK
printf("%s: file_nt_subclass=%x, platnumsubclass=%x\n",FUNC,(unsigned)ri_ptr->img_dim.file_nt_subclass,(unsigned)platnumsubclass);
printf("%s: pixel_mem_size=%u, pixel_disk_size=%u\n",FUNC,(unsigned)pixel_mem_size,(unsigned)pixel_disk_size);
#endif /* QAK */
    convert = (ri_ptr->img_dim.file_nt_subclass != platnumsubclass) ||
	(pixel_mem_size != pixel_disk_size);  /* is conversion necessary? */

    if(convert || switch_interlace==TRUE)
      {   /* convert image data to HDF disk format */
          /* Allocate space for the conversion buffer */
          if((img_data=HDmalloc(pixel_disk_size*count[XDIM]*count[YDIM]))==NULL)
              HGOTO_ERROR(DFE_NOSPACE,FAIL);

          if(switch_interlace==TRUE)
            {
              VOIDP pixel_buf;  /* buffer for the pixel interlaced data */

              /* Allocate space for the conversion buffer */
              if((pixel_buf=HDmalloc(pixel_mem_size*count[XDIM]*count[YDIM]))==NULL)
                  HGOTO_ERROR(DFE_NOSPACE,FAIL);

              GRIil_convert(data,ri_ptr->img_dim.il,pixel_buf,MFGR_INTERLACE_PIXEL,
                  count,ri_ptr->img_dim.ncomps,ri_ptr->img_dim.nt);

              /* convert the pixel data into the HDF disk format */
              DFKconvert(pixel_buf,img_data,ri_ptr->img_dim.nt,
                  ri_ptr->img_dim.ncomps*count[XDIM]*count[YDIM],DFACC_WRITE,0,0);

              HDfree(pixel_buf);
            } /* end if */
          else /* convert the pixel data into the HDF disk format */
              DFKconvert(data,img_data,ri_ptr->img_dim.nt,
                  ri_ptr->img_dim.ncomps*count[XDIM]*count[YDIM],DFACC_WRITE,0,0);
      } /* end if */
    else /* no conversion necessary, just use the user's buffer */
        img_data=data;

    if(ri_ptr->img_tag==DFTAG_NULL || ri_ptr->img_ref==DFREF_WILDCARD)
        new_image=TRUE;
    else
        new_image=FALSE;

#ifdef QAK
printf("%s: check 4\n",FUNC);
#endif /* QAK */
#ifdef QAK
printf("%s: solid_block=%d\n",FUNC,(int)solid_block);
printf("%s: whole_image=%d\n",FUNC,(int)whole_image);
printf("%s: pixel_mem_size=%d\n",FUNC,(int)pixel_mem_size);
printf("%s: pixel_disk_size=%d\n",FUNC,(int)pixel_disk_size);
printf("%s: convert=%d\n",FUNC,(int)convert);
printf("%s: new_image=%d\n",FUNC,(int)new_image);
#endif /* QAK */
/* QAK */
    if(ri_ptr->comp_img==TRUE)
      {   /* create a compressed image */
          if(ri_ptr->ext_img==TRUE)
            { /* compressed, external image data */
            } /* end if */
          else
            { /* regular compressed image data */
            } /* end else */
      } /* end if */
    else 
      {   /* create an uncompressed image */
#ifdef QAK
fprintf(stderr,"%s: check 5, new_image=%d, whole_image=%d, solid_block=%d\n",FUNC,new_image,whole_image,solid_block);
#endif /* QAK */
          if(new_image==TRUE)
            { /* Create the tag/ref for the new image */
                ri_ptr->img_tag=DFTAG_RI;
                ri_ptr->img_ref=Htagnewref(hdf_file_id,ri_ptr->img_tag);
            } /* end if */

#ifdef QAK
fprintf(stderr,"%s: check 5.5, img_tag=%d, img_ref=%d\n",FUNC,(int)ri_ptr->img_tag,(int)ri_ptr->img_ref);
#endif /* QAK */
          if(whole_image==TRUE)
            { /* write the whole image out */
                if(Hputelement(hdf_file_id,ri_ptr->img_tag,ri_ptr->img_ref,
                        (uint8 *)img_data,pixel_disk_size*count[XDIM]*count[YDIM])==FAIL)
                    HGOTO_ERROR(DFE_PUTELEM,FAIL);
            } /* end if */
          else
            { /* write only part of the image out */
                int32 img_offset; /* current offset in the image data */
                VOIDP tmp_data;   /* temp. pointer to the image data to write */
                int32 aid;
                intn fill_image=FALSE;  /* whether to create fill values, etc. */
                VOIDP fill_line=NULL;   /* array containing the fill pixels for an entire line of the image */
                int32 fill_lo_size=0,   /* number of bytes in the "low" block */
                    fill_hi_size=0,     /* number of bytes in the "high" block */
                    fill_line_size=0;   /* number of bytes in the "line" block */

#ifdef QAK
printf("%s: check 6, ri_ptr->fill_img=%d, tag=%u, ref=%u\n",FUNC,(int)ri_ptr->fill_img,(unsigned)ri_ptr->img_tag,(unsigned)ri_ptr->img_ref);
#endif /* QAK */
                if((aid=Hstartaccess(hdf_file_id,ri_ptr->img_tag,ri_ptr->img_ref,
                        DFACC_WRITE))==FAIL)
                    HGOTO_ERROR(DFE_BADAID,FAIL);
                  
                img_offset=((ri_ptr->img_dim.xdim*start[YDIM])+start[XDIM])*pixel_disk_size;

                /* check if this is a new image, and if we need to write fill pixels */
                if(new_image==TRUE && ri_ptr->fill_img==TRUE)
                  { /* create fill value arrays for various blocks */
                      VOIDP fill_pixel; /* converted value for the filled pixel */
                      int32 at_index;   /* attribute index for the fill value */

#ifdef QAK
printf("%s: check 6.5, creating new image and need to fill it, ri_ptr->fill_value=%p, convert=%d\n",FUNC,ri_ptr->fill_value,(int)convert);
#endif /* QAK */
                      if((fill_pixel=(VOIDP)HDmalloc(pixel_disk_size))==NULL)
                          HGOTO_ERROR(DFE_NOSPACE,FAIL);

                      /* create correct disk version of fill pixel */
                      if(ri_ptr->fill_value!=NULL)
                        {
                            DFKconvert(ri_ptr->fill_value,fill_pixel,
                                ri_ptr->img_dim.nt,ri_ptr->img_dim.ncomps,
                                DFACC_WRITE,0,0);
                        } /* end if */
                      else  /* create default pixel fill value of all zero components */
                        {
                          /* Try to find a fill value attribute */
                          if((at_index=GRfindattr(riid,FILL_ATTR))!=FAIL)
                            { /* Found a fill value attribute */
                                if((ri_ptr->fill_value=(VOIDP)HDmalloc(pixel_mem_size))==NULL)
                                    HGOTO_ERROR(DFE_NOSPACE,FAIL);
                                if(GRgetattr(riid,at_index,ri_ptr->fill_value)==FAIL)
                                    HGOTO_ERROR(DFE_BADATTR,FAIL);
#ifdef QAK
printf("%s: check 6.6, found a fill value, nt=%d, ncomps=%d\n",FUNC,(int)ri_ptr->img_dim.nt,(int)ri_ptr->img_dim.ncomps);
#endif /* QAK */
                                DFKconvert(ri_ptr->fill_value,fill_pixel,
                                    ri_ptr->img_dim.nt,ri_ptr->img_dim.ncomps,
                                    DFACC_WRITE,0,0);
                            } /* end if */
                          else
                              HDmemset(fill_pixel,0,pixel_disk_size);
                        } /* end else */

                      /* check for "low" pixel runs */
                      if(start[XDIM]>0)
                          fill_lo_size=pixel_disk_size*start[XDIM];

                      /* check for "high" pixel runs */
                      if((start[XDIM]+((count[XDIM]-1)*stride[XDIM])+1)<ri_ptr->img_dim.xdim)
                          fill_hi_size=pixel_disk_size*(ri_ptr->img_dim.xdim-
                              (start[XDIM]+((count[XDIM]-1)*stride[XDIM])+1));

#ifdef QAK
printf("%s: check 6.75, xdim=%ld, ydim=%ld, fill_lo_size=%ld, fill_hi_size=%ld\n",FUNC,(long)ri_ptr->img_dim.xdim,(long)ri_ptr->img_dim.ydim,(long)fill_lo_size,(long)fill_hi_size);
#endif /* QAK */

                      /* create the "line" pixel block */
                      /* allocate space for the "line" block */
                      fill_line_size=pixel_disk_size*ri_ptr->img_dim.xdim;
                      if((fill_line=(VOIDP)HDmalloc(fill_line_size))==NULL)
                            HGOTO_ERROR(DFE_NOSPACE,FAIL);
                      HDmemfill(fill_line,fill_pixel,pixel_disk_size,ri_ptr->img_dim.xdim);

                      fill_image=TRUE;  /* set flag to write out fill pixels */
                      ri_ptr->store_fill=TRUE;  /* set flag to store fill value attribute */

                      /* Get rid of the fill pixel */
                      if(ri_ptr->fill_value==NULL)
                          ri_ptr->fill_value=fill_pixel;
                      else
                          HDfree(fill_pixel);
                  } /* end if */

#ifdef QAK
printf("%s: check 7, fill_image=%d\n",FUNC,fill_image);
#endif /* QAK */
                tmp_data=img_data;
                if(solid_block==TRUE)
                  {   /* write out runs of data in the image */
                      int32 pix_len;    /* length of current row's pixel run */
                      intn i;           /* temporary loop variable */

                      pix_len=pixel_disk_size*count[XDIM];
                        
                      if(fill_image==TRUE)
                        {   /* surround the block to write with fill values */
#ifdef QAK
printf("%s: check 8\n",FUNC);
#endif /* QAK */
#ifdef QAK
                            if(Hseek(aid,0,DF_START)==FAIL)
                                HGOTO_ERROR(DFE_SEEKERROR,FAIL);
#endif /* QAK */

#ifdef QAK
printf("%s: check 8.5\n",FUNC);
#endif /* QAK */
                            /* write out lines "below" the block */
                            if(start[YDIM]>0)
                              { /* fill in the lines leading up the block */
                                for(i=0; i<start[YDIM]; i++)
                                    if(Hwrite(aid,fill_line_size,fill_line)==FAIL)
                                        HGOTO_ERROR(DFE_WRITEERROR,FAIL);
                              } /* end if */

#ifdef QAK
printf("%s: check 9\n",FUNC);
#endif /* QAK */
                            /* write prelude of low pixels */
                            if(fill_lo_size>0)
                                if(Hwrite(aid,fill_lo_size,fill_line)==FAIL)
                                    HGOTO_ERROR(DFE_WRITEERROR,FAIL);

#ifdef QAK
printf("%s: check 10, fill_lo_size=%ld, fill_hi_size=%ld, pix_len=%ld\n",FUNC,(long)fill_lo_size,(long)fill_hi_size,(long)pix_len);
printf("%s: *tmp_data=%u\n",FUNC,(unsigned)*(uint8 *)tmp_data);
#endif /* QAK */
                            /* write out the block */
                            for(i=0; i<count[YDIM]; i++)
                              {
                                  if(Hwrite(aid,pix_len,tmp_data)==FAIL)
                                      HGOTO_ERROR(DFE_WRITEERROR,FAIL);

                                  /* This next write wraps around the high side */
                                  /* of the block and writes the next low side */
                                  /* at the same time. */
                                  if((fill_hi_size+fill_lo_size)>0
                                          && i<(count[YDIM]-1))
                                      if(Hwrite(aid,(fill_hi_size+fill_lo_size),fill_line)==FAIL)
                                          HGOTO_ERROR(DFE_WRITEERROR,FAIL);
                                  tmp_data=(VOIDP)((char *)tmp_data+pix_len);
                              } /* end for */
                            
#ifdef QAK
printf("%s: check 11\n",FUNC);
#endif /* QAK */
                            /* Finish the last chunk of high side fill values */
                            if(fill_hi_size>0)
                                if(Hwrite(aid,fill_hi_size,fill_line)==FAIL)
                                    HGOTO_ERROR(DFE_WRITEERROR,FAIL);

#ifdef QAK
printf("%s: check 12\n",FUNC);
#endif /* QAK */
                            /* write out lines "above" the block */
                            if((start[YDIM]+((count[YDIM]-1)*stride[YDIM])+1)
                                <ri_ptr->img_dim.ydim)
                              {
                                for(i=start[YDIM]+((count[YDIM]-1)*stride[YDIM])+1;
                                        i<ri_ptr->img_dim.ydim; i++)
                                    if(Hwrite(aid,fill_line_size,fill_line)==FAIL)
                                        HGOTO_ERROR(DFE_WRITEERROR,FAIL);
                              } /* end if */
#ifdef QAK
printf("%s: check 12.4\n",FUNC);
#endif /* QAK */
                        } /* end if */
                      else
                        {   /* don't worry about fill values */
#ifdef QAK
printf("%s: check 12.5\n",FUNC);
#endif /* QAK */
#ifdef QAK
#endif /* QAK */
                            for(i=0; i<count[YDIM]; i++)
                              {
                                  if(Hseek(aid,img_offset,DF_START)==FAIL)
                                      HGOTO_ERROR(DFE_SEEKERROR,FAIL);
                                  if(Hwrite(aid,pix_len,tmp_data)==FAIL)
                                      HGOTO_ERROR(DFE_WRITEERROR,FAIL);
                                  img_offset+=pixel_disk_size*ri_ptr->img_dim.xdim;
                                  tmp_data=(VOIDP)((char *)tmp_data+pix_len);
                              } /* end for */
                        } /* end else */
                  } /* end if */
                else
                  {   /* sub-sampling, seek to each data element and write it out */
                      intn i,j,k;       /* temporary loop variables */
                      int32 fill_stride_size=0; /* # of pixels in the stride block */
                      intn fill_xdim=FALSE, /* whether to fill in the X dimension */
                          fill_ydim=FALSE;  /* whether to fill in the Y dimension */

#ifdef QAK
printf("%s: check 13\n",FUNC);
#endif /* QAK */
                      /* check if we need to insert fill pixels between strides */
                      if(fill_image==TRUE)
                        {   /* create the "stride" pixel block */
                            if(stride[XDIM]>1)
                              {
                                /* allocate space for the "stride" block */
                                fill_stride_size=pixel_disk_size*(stride[XDIM]-1);
                                fill_xdim=TRUE;
                              } /* end if */
                            if(stride[YDIM]>1)
                                fill_ydim=TRUE;
#ifdef QAK
printf("%s: check 14\n",FUNC);
#endif /* QAK */

                            /* write fills and sub-sampled data */
#ifdef QAK
                            if(Hseek(aid,0,DF_START)==FAIL)
                                HGOTO_ERROR(DFE_SEEKERROR,FAIL);
#endif /* QAK */

                            /* write out lines "below" the block */
                            if(start[YDIM]>0)
                              { /* fill in the lines leading up the block */
#ifdef QAK
printf("%s: check 14.1 start[YDIM]=%d, writing %ld bytes\n",FUNC,(int)start[YDIM],(long)fill_line_size);
#endif /* QAK */
                                for(i=0; i<start[YDIM]; i++)
                                  {
#ifdef QAK
printf("%s: check 14.15 i=%d\n",FUNC,i);
#endif /* QAK */
                                    if(Hwrite(aid,fill_line_size,fill_line)==FAIL)
                                        HGOTO_ERROR(DFE_WRITEERROR,FAIL);
                                  } /* end for */
                              } /* end if */

                            /* write prelude of low pixels */
                            if(fill_lo_size>0)
                              {
#ifdef QAK
printf("%s: check 14.2 writing prelude of %d bytes\n",FUNC,(int)fill_lo_size);
#endif /* QAK */
                                if(Hwrite(aid,fill_lo_size,fill_line)==FAIL)
                                    HGOTO_ERROR(DFE_WRITEERROR,FAIL);
                              } /* end if */

#ifdef QAK
printf("%s: check 14.5, fill_lo_size=%ld, fill_hi_size=%ld\n",FUNC,(long)fill_lo_size,(long)fill_hi_size);
printf("%s: check 14.55, pixel_disk_size=%ld\n",FUNC,(long)pixel_disk_size);
printf("%s: check 14.6, fill_xdim=%ld, fill_ydim=%ld, fill_stride_size=%ld, fill_line_size=%ld\n",FUNC,(long)fill_xdim,(long)fill_ydim,(long)fill_stride_size,(long)fill_line_size);
printf("%s: check 14.61, count[YDIM]=%ld, count[XDIM]=%ld\n",FUNC,(long)count[YDIM],(long)count[XDIM]);
#endif /* QAK */
                            for(i=0; i<count[YDIM]; i++)
                              {
                                  for(j=0; j<count[XDIM]; j++)
                                    {
                                      if(Hwrite(aid,pixel_disk_size,tmp_data)==FAIL)
                                          HGOTO_ERROR(DFE_WRITEERROR,FAIL);
                                      if(fill_xdim==TRUE && j<(count[XDIM]-1))
                                        {
#ifdef QAK
printf("%s: check 14.63, writing fill_stride_size=%ld bytes\n",FUNC,(long)fill_stride_size);
#endif /* QAK */
                                          if(Hwrite(aid,fill_stride_size,fill_line)==FAIL)
                                              HGOTO_ERROR(DFE_WRITEERROR,FAIL);
                                        } /* end if */
                                      tmp_data=(VOIDP)((char *)tmp_data+pixel_disk_size);
                                    } /* end for */
  
                                  /* Fill in the y-dim stride lines */
                                  if(fill_ydim==TRUE)
                                      for(k=1; k<stride[YDIM]; k++)
                                        {
#ifdef QAK
printf("%s: check 14.65, k=%d\n",FUNC,(int)k);
#endif /* QAK */
                                          if(Hwrite(aid,fill_line_size,fill_line)==FAIL)
                                              HGOTO_ERROR(DFE_WRITEERROR,FAIL);
                                        } /* end for */

                                  /* This next write wraps around the high side */
                                  /* of the block and writes the next low side */
                                  /* at the same time. */
                                  if((fill_hi_size+fill_lo_size)>0
                                          && i<(count[YDIM]-1))
                                    {
#ifdef QAK
printf("%s: check 14.7, i=%d\n",FUNC,(int)i);
#endif /* QAK */
                                      if(Hwrite(aid,(fill_hi_size+fill_lo_size),fill_line)==FAIL)
                                          HGOTO_ERROR(DFE_WRITEERROR,FAIL);
                                    } /* end if */
                              } /* end for */

                            /* Finish the last chunk of high side fill values */
                            if(fill_hi_size>0)
                              {
#ifdef QAK
printf("%s: check 14.75, fill_hi_size=%d\n",FUNC,(int)fill_hi_size);
#endif /* QAK */
                                if(Hwrite(aid,fill_hi_size,fill_line)==FAIL)
                                    HGOTO_ERROR(DFE_WRITEERROR,FAIL);
                              } /* end if */

#ifdef INCORRECT
                            /* write out lines "above" the block */
                            if((start[YDIM]+((count[YDIM]-1)*stride[YDIM])+1)
                                <ri_ptr->img_dim.ydim)
                              {
printf("%s: check 14.8, ri_ptr->img_dim.xdim=%d, ri_ptr->img_dim.ydim=%d\n",FUNC,(int)ri_ptr->img_dim.xdim,(int)ri_ptr->img_dim.ydim);
                                for(i=start[YDIM]+((count[YDIM]-1)*stride[YDIM])+1;
                                        i<ri_ptr->img_dim.ydim; i++)
                                  {
printf("%s: check 14.81, i=%d\n",FUNC,i);
                                    if(Hwrite(aid,fill_line_size,fill_line)==FAIL)
                                        HGOTO_ERROR(DFE_WRITEERROR,FAIL);
                                  } /* end for */
                              } /* end if */
#endif /* QAK */

                        } /* end if */
                      else
                        {   /* don't worry about fill values */
                          int32 stride_add; /* amount to add for stride amount */

                          stride_add=pixel_disk_size*stride[XDIM];

#ifdef QAK
printf("%s: check 15, stride_add=%ld, img_offset=%ld\n",FUNC,(long)stride_add,(long)img_offset);
#endif /* QAK */
                            for(i=0; i<count[YDIM]; i++)
                              {
                                  int32 local_offset;
  
                                  local_offset=img_offset;
#ifdef QAK
printf("%s: check 15.5, local_offset=%ld\n",FUNC,(long)local_offset);
#endif /* QAK */
                                  for(j=0; j<count[XDIM]; j++)
                                    {
                                      if(Hseek(aid,local_offset,DF_START)==FAIL)
                                          HGOTO_ERROR(DFE_SEEKERROR,FAIL);
                                      if(Hwrite(aid,pixel_disk_size,tmp_data)==FAIL)
                                          HGOTO_ERROR(DFE_WRITEERROR,FAIL);
                                      local_offset+=stride_add;
                                      tmp_data=(VOIDP)((char *)tmp_data+pixel_disk_size);
                                    } /* end for */
  
                                  img_offset+=ri_ptr->img_dim.xdim*stride[YDIM]*pixel_disk_size;
                              } /* end for */
                        } /* end else */
                  } /* end else */
                  
                if(fill_line!=NULL)     /* free the fill-value pixels if we used 'em */
                    HDfree(fill_line);

                if(Hendaccess(aid)==FAIL)
                    HGOTO_ERROR(DFE_CANTENDACCESS,FAIL);
            } /* end else */
          ri_ptr->data_modified=TRUE;
      } /* end else */

    /* mark the image as being modified */
    ri_ptr->data_modified=TRUE;
    gr_ptr->gr_modified=TRUE;

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(GR_mask, ID_GRwriteimage);
#endif /* HAVE_PABLO */
  return ret_value;
} /* end GRwriteimage() */

/*--------------------------------------------------------------------------
 NAME
    GRreadimage

 PURPOSE
    Read raster data for an image

 USAGE
    intn GRreadimage(riid,start,stride,edge,data)
        int32 riid;         IN: RI ID from GRselect/GRcreate
        int32 start[2];     IN: array containing the offset in the image of the
                                image data to read in
        int32 stride[2];    IN: array containing interval of data being read
                                along each edge.  strides of 0 are illegal
                                (and generate an error)
                                ie. stride of 1 in each dimension means
                                reading contiguous data, stride of 2 means
                                reading every other element out along an edge.
        int32 count[2];     IN: number of elements to read in along each edge.
        VOIDP data;         IN: pointer to the data to read in.

 RETURNS
    SUCCEED/FAIL

 DESCRIPTION
    Read image data from an RI.  Partial dataset reading and subsampling is
        allowed.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
    If the stride parameter is set to NULL, a stride of 1 will be assumed.

 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
intn GRreadimage(int32 riid,int32 start[2],int32 in_stride[2],int32 count[2],VOIDP data)
{
    CONSTR(FUNC, "GRreadimage");   /* for HERROR */
    int32 hdf_file_id;          /* HDF file ID */
    gr_info_t *gr_ptr;          /* ptr to the GR information for this grid */
    ri_info_t *ri_ptr;          /* ptr to the image to work with */
    int32 stride[2];            /* pointer to the stride array */
    intn solid_block=FALSE;     /* whether the image data is a solid block of data */
    intn whole_image=FALSE;     /* whether we are writing out the whole image */
    VOIDP *img_data;            /* pointer to the converted image data to write */
    uintn pixel_disk_size;      /* size of a pixel on disk */
    uintn pixel_mem_size;       /* size of a pixel in memory */
    intn convert;               /* true if machine NT != NT to be written */
    uint8 platnumsubclass;      /* class of this NT for this platform */
    intn  ret_value = SUCCEED;

#ifdef HAVE_PABLO
    TRACE_ON(GR_mask, ID_GRreadimage);
#endif /* HAVE_PABLO */
    /* clear error stack and check validity of args */
    HEclear();

    /* check the basic validity of the args (stride is OK to be NULL) */
    if (HAatom_group(riid)!=RIIDGROUP || start==NULL /* || in_stride==NULL */ || count==NULL
            || data==NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);
    
    /* Get the correct parameters into the stride array */
    if(in_stride==NULL)
          stride[XDIM]=stride[YDIM]=1;
    else
      {
          stride[XDIM]=in_stride[XDIM];
          stride[YDIM]=in_stride[YDIM];
      } /* end else */

    /* Sanity check the start, stride, and count args */
    if((start[XDIM]<0 || start[YDIM]<0) || (stride[XDIM]<1 || stride[YDIM]<1)
            || (count[XDIM]<1 || count[YDIM]<1))
        HGOTO_ERROR(DFE_BADDIM, FAIL);

#ifdef QAK
fprintf(stderr,"%s: riid=%ld\n",FUNC,(long)riid);
fprintf(stderr,"%s: start={%ld,%ld}\n",FUNC,(long)start[XDIM],(long)start[YDIM]);
fprintf(stderr,"%s: stride={%ld,%ld}\n",FUNC,(long)stride[XDIM],(long)stride[YDIM]);
fprintf(stderr,"%s: count={%ld,%ld}\n",FUNC,(long)count[XDIM],(long)count[YDIM]);
fprintf(stderr,"%s: data=%p\n",FUNC,data);
#endif /* QAK */
    /* locate RI's object in hash table */
    if (NULL == (ri_ptr = (ri_info_t *) HAatom_object(riid)))
        HGOTO_ERROR(DFE_NOVS, FAIL);
    gr_ptr=ri_ptr->gr_ptr;
    hdf_file_id=gr_ptr->hdf_file_id;

    if(stride[XDIM]==1 && stride[YDIM]==1)
      { /* solid block of data */
          solid_block=TRUE;
          if((start[XDIM]==0 && start[YDIM]==0) && (count[XDIM]==ri_ptr->img_dim.xdim
                && count[YDIM]==ri_ptr->img_dim.ydim))
              whole_image=TRUE;
          else
              whole_image=FALSE;
      } /* end if */
    else /* block of data spread out with strides */
        solid_block=FALSE;

    /* Get the size of the pixels in memory and on disk */
    pixel_disk_size=ri_ptr->img_dim.ncomps*DFKNTsize(ri_ptr->img_dim.nt);
    pixel_mem_size=ri_ptr->img_dim.ncomps*DFKNTsize((ri_ptr->img_dim.nt | DFNT_NATIVE) & (~DFNT_LITEND));

    /* Get number-type and conversion information */
    platnumsubclass = DFKgetPNSC(ri_ptr->img_dim.nt & (~DFNT_LITEND), DF_MT);
#ifdef QAK
printf("%s: file_nt_subclass=%x, platnumsubclass=%x\n",FUNC,(unsigned)ri_ptr->img_dim.file_nt_subclass,(unsigned)platnumsubclass);
printf("%s: pixel_disk_size=%u\n",FUNC,(unsigned)pixel_disk_size);
#endif /* QAK */
    convert = (pixel_disk_size != pixel_mem_size) || (ri_ptr->img_dim.file_nt_subclass != platnumsubclass);  /* is conversion necessary? */

#ifdef QAK
fprintf(stderr,"%s: solid_block=%d\n",FUNC,(int)solid_block);
fprintf(stderr,"%s: whole_image=%d\n",FUNC,(int)whole_image);
fprintf(stderr,"%s: pixel_disk_size=%d\n",FUNC,(int)pixel_disk_size);
fprintf(stderr,"%s: ri_ptr->img_tag=%d, ri_ptr->img_ref=%d\n",FUNC,(int)ri_ptr->img_tag,(int)ri_ptr->img_ref);
fprintf(stderr,"%s: convert=%d\n",FUNC,(int)convert);
#endif /* QAK */
    if(ri_ptr->img_tag==DFTAG_NULL || ri_ptr->img_ref==DFREF_WILDCARD)
      { /* Fake an image for the user by using the pixel fill value */
          VOIDP fill_pixel;         /* converted value for the filled pixel */
          int32 at_index;

#ifdef QAK
fprintf(stderr,"%s: faking an image for the user\n",FUNC);
#endif /* QAK */
          if((fill_pixel=(VOIDP)HDmalloc(pixel_mem_size))==NULL)
              HGOTO_ERROR(DFE_NOSPACE,FAIL);

          /* Try to find a fill value attribute */
          if((at_index=GRfindattr(riid,FILL_ATTR))!=FAIL)
            { /* Found a fill value attribute */
#ifdef QAK
printf("%s: going after a fill value\n",FUNC);
#endif /* QAK */
                if(GRgetattr(riid,at_index,fill_pixel)==FAIL)
                    HGOTO_ERROR(DFE_BADATTR,FAIL);
#ifdef QAK
printf("%s: got the fill value\n",FUNC);
#endif /* QAK */
            } /* end if */
          else /* no fill value attribute */
              HDmemset(fill_pixel,0,pixel_mem_size);

          /* Fill the user's buffer with the fill value */
          HDmemfill(data,fill_pixel,pixel_mem_size,count[XDIM]*count[YDIM]);
          HDfree(fill_pixel);
      } /* end if */
    else
      { /* an image exists in the file */
#ifdef QAK
fprintf(stderr,"%s: image exists\n",FUNC);
#endif /* QAK */
          if(convert)
            {   /* convert image data to HDF disk format */
                /* Allocate space for the conversion buffer */
                if((img_data=HDmalloc(pixel_disk_size*count[XDIM]*count[YDIM]))==NULL)
                    HGOTO_ERROR(DFE_NOSPACE,FAIL);
            } /* end if */
          else /* no conversion necessary, just use the user's buffer */
              img_data=data;

#ifdef QAK
printf("%s: check 1 whole_image=%d, solid_block=%d\n",FUNC,(int)whole_image,(int)solid_block);
#endif /* QAK */
          if(whole_image==TRUE)
            { /* read the whole image in */
#ifdef QAK
fprintf(stderr,"%s: check 1.3, tag=%u, ref=%u\n",FUNC,(unsigned)ri_ptr->img_tag,(unsigned)ri_ptr->img_ref);
#endif /* QAK */
                if(Hgetelement(hdf_file_id,ri_ptr->img_tag,ri_ptr->img_ref,
                        (uint8 *)img_data)==FAIL)
                    HGOTO_ERROR(DFE_GETELEM,FAIL);
            } /* end if */
          else
            { /* read only part of the image in */
                int32 img_offset; /* current offset in the image data */
                VOIDP tmp_data;   /* temp. pointer to the image data read in */
                int32 aid;

#ifdef QAK
fprintf(stderr,"%s: check 1.5\n",FUNC);
#endif /* QAK */
                if((aid=Hstartaccess(hdf_file_id,ri_ptr->img_tag,ri_ptr->img_ref,
                        DFACC_READ))==FAIL)
                    HGOTO_ERROR(DFE_BADAID,FAIL);
                  
                img_offset=((ri_ptr->img_dim.xdim*start[YDIM])+start[XDIM])*pixel_disk_size;

                tmp_data=img_data;
                if(solid_block==TRUE)
                  {   /* read in runs of data in the image */
                      int32 pix_len;    /* length of current row's pixel run */
                      intn i;           /* temporary loop variable */

#ifdef QAK
printf("%s: check 2\n",FUNC);
#endif /* QAK */
                      pix_len=pixel_disk_size*count[XDIM];
                        
                      /* read in the block */
                      for(i=0; i<count[YDIM]; i++)
                        {
                            if(Hseek(aid,img_offset,DF_START)==FAIL)
                                HGOTO_ERROR(DFE_SEEKERROR,FAIL);
                            if(Hread(aid,pix_len,tmp_data)==FAIL)
                                HGOTO_ERROR(DFE_READERROR,FAIL);
                            img_offset+=pixel_disk_size*ri_ptr->img_dim.xdim;
                            tmp_data=(VOIDP)((char *)tmp_data+pix_len);
                        } /* end for */
                  } /* end if */
                else
                  {   /* sub-sampling, seek to each data element and read it in */
                      intn i,j;         /* temporary loop variables */
                      int32 stride_add; /* amount to add for stride amount */

#ifdef QAK
printf("%s: check 3\n",FUNC);
#endif /* QAK */
                      stride_add=pixel_disk_size*stride[XDIM];

                      for(i=0; i<count[YDIM]; i++)
                        {
                            int32 local_offset;

                            local_offset=img_offset;
                            for(j=0; j<count[XDIM]; j++)
                              {
                                if(Hseek(aid,local_offset,DF_START)==FAIL)
                                    HGOTO_ERROR(DFE_SEEKERROR,FAIL);
                                if(Hread(aid,pixel_disk_size,tmp_data)==FAIL)
                                    HGOTO_ERROR(DFE_READERROR,FAIL);
                                local_offset+=stride_add;
                                tmp_data=(VOIDP)((char *)tmp_data+pixel_disk_size);
                              } /* end for */

                            img_offset+=ri_ptr->img_dim.xdim*stride[YDIM]*pixel_disk_size;
                        } /* end for */
                  } /* end else */
                if(Hendaccess(aid)==FAIL)
                    HGOTO_ERROR(DFE_CANTENDACCESS,FAIL);
            } /* end else */
                  
#ifdef QAK
printf("%s: convert=%d\n",FUNC,(int)convert);
#endif /* QAK */
          if(convert)
            { /* convert the pixel data into the HDF disk format */
#ifdef QAK
printf("%s: check 4\n",FUNC);
#endif /* QAK */
              DFKconvert(img_data,data,ri_ptr->img_dim.nt,
                  ri_ptr->img_dim.ncomps*count[XDIM]*count[YDIM],DFACC_READ,0,0);
              HDfree(img_data);
            } /* end if */
      } /* end else */

      /* Check whether we need to convert the buffer to the user's */
      /*    requested interlace scheme. */
      /* Note: This is implemented in a horribly ugly & slow manner, but I'm */
      /*        in a bit of a hurry right now - QAK */
      if(ri_ptr->im_il!=MFGR_INTERLACE_PIXEL)
        {
          VOIDP pixel_buf;  /* buffer for the pixel interlaced data */

          /* Allocate space for the conversion buffer */
          if((pixel_buf=HDmalloc(pixel_mem_size*count[XDIM]*count[YDIM]))==NULL)
              HGOTO_ERROR(DFE_NOSPACE,FAIL);

          GRIil_convert(data,MFGR_INTERLACE_PIXEL,pixel_buf,ri_ptr->im_il,
              count,ri_ptr->img_dim.ncomps,ri_ptr->img_dim.nt);

          HDmemcpy(data,pixel_buf,pixel_mem_size*count[XDIM]*count[YDIM]);

          HDfree(pixel_buf);
        } /* end if */

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(GR_mask, ID_GRreadimage);
#endif /* HAVE_PABLO */
  return ret_value;
} /* end GRreadimage() */

/*--------------------------------------------------------------------------
 NAME
    GRendaccess

 PURPOSE
    End access to an RI.

 USAGE
    intn GRendaccess(riid)
        int32 riid;         IN: RI ID from GRselect/GRcreate

 RETURNS
    SUCCEED/FAIL

 DESCRIPTION
    End access to an RI.  Further attempts to access the RI ID will result in
    an error.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
intn GRendaccess(int32 riid)
{
    CONSTR(FUNC, "GRendaccess");   /* for HERROR */
    ri_info_t *ri_ptr;          /* ptr to the image to work with */
    intn  ret_value = SUCCEED;

#ifdef HAVE_PABLO
    TRACE_ON(GR_mask, ID_GRendaccess);
#endif /* HAVE_PABLO */
    /* clear error stack and check validity of args */
    HEclear();

    /* check the validity of the RI ID */
    if (HAatom_group(riid)!=RIIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);
    
    /* locate RI's object in hash table */
    if (NULL == (ri_ptr = (ri_info_t *) HAatom_object(riid)))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    if(!(ri_ptr->access>0))
        HGOTO_ERROR(DFE_CANTENDACCESS,FAIL);

    /* Check for writing a fill value attribute out */
    if(ri_ptr->store_fill==TRUE)
      { /* store the fill value attribute before letting go of the image */
          if(GRsetattr(riid,FILL_ATTR,ri_ptr->img_dim.nt,ri_ptr->img_dim.ncomps,ri_ptr->fill_value)==FAIL)
              HGOTO_ERROR(DFE_WRITEERROR,FAIL);
          ri_ptr->store_fill=FALSE;
      } /* end if */

    /* Reduce the number of accesses to the RI */
    ri_ptr->access--;

    /* Delete the atom for the RI ID */
    if(NULL==HAremove_atom(riid))
        HGOTO_ERROR(DFE_NOVS, FAIL);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(GR_mask, ID_GRendaccess);
#endif /* HAVE_PABLO */
  return ret_value;
} /* end GRendaccess() */

/*--------------------------------------------------------------------------
 NAME
    GRidtoref

 PURPOSE
    Maps an RI ID to a reference # for annotating or including in a Vgroup.

 USAGE
    uint16 GRidtoref(riid)
        int32 riid;         IN: RI ID from GRselect/GRcreate

 RETURNS
    A valid reference # on success or FAIL

 DESCRIPTION
    Maps an riid to a reference # for annotating or including in a Vgroup.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
uint16 GRidtoref(int32 riid)
{
    CONSTR(FUNC, "GRidtoref");   /* for HERROR */
    ri_info_t *ri_ptr;          /* ptr to the image to work with */
    uint16  ret_value = 0; /* FAIL? */

#ifdef HAVE_PABLO
    TRACE_ON(GR_mask, ID_GRidtoref);
#endif /* HAVE_PABLO */
    /* clear error stack and check validity of args */
    HEclear();

    /* check the validity of the RI ID */
    if (HAatom_group(riid)!=RIIDGROUP)
        HGOTO_ERROR(DFE_ARGS, (uint16)FAIL);
    
    /* locate RI's object in hash table */
    if (NULL == (ri_ptr = (ri_info_t *) HAatom_object(riid)))
        HGOTO_ERROR(DFE_NOVS, FAIL);

#ifdef OLD_WAY
    ret_value = (ri_ptr->ri_ref!=DFREF_WILDCARD ? ri_ptr->ri_ref : ri_ptr->rig_ref);
#else /* OLD_WAY */
/* Changed to handle case when ri_ptr->rig_ref is DFREF_WILDCARD */
    if(ri_ptr->ri_ref!=DFREF_WILDCARD)
        ret_value=ri_ptr->ri_ref;
    else
      {
        if(ri_ptr->rig_ref==DFREF_WILDCARD)
            HGOTO_ERROR(DFE_INTERNAL,(uint16)FAIL);
        ret_value=ri_ptr->rig_ref;
      } /* end else */
#endif /* OLD_WAY */

done:
  if(ret_value == 0)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(GR_mask, ID_GRidtoref);
#endif /* HAVE_PABLO */
  return ret_value;
} /* end GRidtoref() */

/*--------------------------------------------------------------------------
 NAME
    GRreftoindex

 PURPOSE
    Maps the reference # of an RI into an index which can be used with GRselect.

 USAGE
    int32 GRreftoindex(grid,ref)
        int32 grid;         IN: GR ID from GRstart
        uint16 ref;         IN: reference number of raster image to map to index

 RETURNS
    A valid index # on success or FAIL

 DESCRIPTION
    Maps the reference # of an RI into an index which can be used with GRselect.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
int32 GRreftoindex(int32 grid,uint16 ref)
{
    CONSTR(FUNC, "GRreftoindex");   /* for HERROR */
    gr_info_t *gr_ptr;          /* ptr to the GR information for this grid */
    ri_info_t *ri_ptr;          /* ptr to the image to work with */
    VOIDP *t;                   /* temp. ptr to the image found */
    int32   ret_value = SUCCEED;

#ifdef HAVE_PABLO
    TRACE_ON(GR_mask, ID_GRreftoindex);
#endif /* HAVE_PABLO */
    /* clear error stack and check validity of args */
    HEclear();

    /* check the validity of the RI ID */
    if (HAatom_group(grid)!=GRIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);
    
    /* locate GR's object in hash table */
    if (NULL == (gr_ptr = (gr_info_t *) HAatom_object(grid)))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    if((t = (VOIDP *) tbbtfirst((TBBT_NODE *) *(gr_ptr->grtree)))==NULL)
        HGOTO_ERROR(DFE_RINOTFOUND,FAIL);
    do {
        ri_ptr=(ri_info_t *)*t;
        if(ri_ptr!=NULL && ri_ptr->ri_ref==ref)  /* the ref matches */
            HGOTO_DONE(ri_ptr->index);
    } while((t= (VOIDP *) tbbtnext((TBBT_NODE *)t))!=NULL);

    ret_value = (FAIL);

done:
  if(ret_value == 0)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(GR_mask, ID_GRreftoindex);
#endif /* HAVE_PABLO */
  return ret_value;
} /* end GRreftoindex() */

/*--------------------------------------------------------------------------
 NAME
    GRreqlutil

 PURPOSE
    Request that the next LUT read from an RI have a particular interlace.

 USAGE
    intn GRreqlutil(riid,il)
        int32 riid;         IN: RI ID from GRselect/GRcreate
        intn il;            IN: interlace for next LUT.  From the following
                                values (found in mfgr.h): 
                      MFGR_INTERLACE_PIXEL      - pixel interlacing
                      MFGR_INTERLACE_LINE       - line interlacing
                      MFGR_INTERLACE_COMPONENT  - component/plane interlacing

 RETURNS
    SUCCEED/FAIL

 DESCRIPTION
    Request that the next LUT read from an RI have a particular interlace.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
intn GRreqlutil(int32 riid,intn il)
{
    CONSTR(FUNC, "GRreqlutil");   /* for HERROR */
    ri_info_t *ri_ptr;          /* ptr to the image to work with */
    intn  ret_value = SUCCEED;

#ifdef HAVE_PABLO
    TRACE_ON(GR_mask, ID_GRreqlutil);
#endif /* HAVE_PABLO */
    /* clear error stack and check validity of args */
    HEclear();

    /* check the validity of the RI ID */
    if (HAatom_group(riid)!=RIIDGROUP || il<(intn)MFGR_INTERLACE_PIXEL ||
            il>(intn)MFGR_INTERLACE_COMPONENT)
        HGOTO_ERROR(DFE_ARGS, FAIL);
    
    /* locate RI's object in hash table */
    if (NULL == (ri_ptr = (ri_info_t *) HAatom_object(riid)))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    /* Assign interlacing scheme */
    ri_ptr->lut_il=(gr_interlace_t)il;   

done:
  if(ret_value == 0)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_ON(GR_mask, ID_GRreqlutil);
#endif /* HAVE_PABLO */
  return ret_value;
} /* end GRreqlutil() */

/*--------------------------------------------------------------------------
 NAME
    GRreqimageil

 PURPOSE
    Request that the image read from an RI have a particular interlace.

 USAGE
    intn GRreqimageil(riid,il)
        int32 riid;         IN: RI ID from GRselect/GRcreate
        intn il;            IN: interlace for next RI.  From the following
                                values (found in mfgr.h): 
                      MFGR_INTERLACE_PIXEL      - pixel interlacing
                      MFGR_INTERLACE_LINE       - line interlacing
                      MFGR_INTERLACE_COMPONENT  - component/plane interlacing

 RETURNS
    SUCCEED/FAIL

 DESCRIPTION
    Request that the image read from an RI have a particular interlace.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
intn GRreqimageil(int32 riid,intn il)
{
    CONSTR(FUNC, "GRreqimageil");   /* for HERROR */
    ri_info_t *ri_ptr;          /* ptr to the image to work with */
    intn  ret_value = SUCCEED;

#ifdef HAVE_PABLO
    TRACE_ON(GR_mask, ID_GRreqimageil);
#endif /* HAVE_PABLO */
    /* clear error stack and check validity of args */
    HEclear();

    /* check the validity of the RI ID */
    if (HAatom_group(riid)!=RIIDGROUP || il<(intn)MFGR_INTERLACE_PIXEL ||
            il>(intn)MFGR_INTERLACE_COMPONENT)
        HGOTO_ERROR(DFE_ARGS, FAIL);
    
    /* locate RI's object in hash table */
    if (NULL == (ri_ptr = (ri_info_t *) HAatom_object(riid)))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    /* Assign interlacing scheme */
    ri_ptr->im_il=(gr_interlace_t)il;   

done:
  if(ret_value == 0)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(GR_mask, ID_GRreqimageil);
#endif /* HAVE_PABLO */
  return ret_value;
} /* end GRreqimageil() */

/*--------------------------------------------------------------------------
 NAME
    GRgetlutid

 PURPOSE
    Get a LUT id ('lutid') for an RI.

 USAGE
    int32 GRgetlutid(riid,index)
        int32 riid;         IN: RI ID from GRselect/GRcreate
        int32 lut_index;    IN: Which LUT image to select (indexed from 0)

 RETURNS
    Valid LUT ID on success, FAIL on failure

 DESCRIPTION
    Get a LUT id ('lutid') for accessing LUTs in an RI.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
    Currently only supports one LUT per image, at index 0 and LUTID==RIID.

 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
int32 GRgetlutid(int32 riid,int32 lut_index)
{
    CONSTR(FUNC, "GRgetlutid");   /* for HERROR */
    int32  ret_value = SUCCEED;

#ifdef HAVE_PABLO
    TRACE_ON(GR_mask, ID_GRgetlutid);
#endif /* HAVE_PABLO */
    /* clear error stack and check validity of args */
    HEclear();

    /* check the validity of the RI ID */
    if (HAatom_group(riid)!=RIIDGROUP || lut_index!=0)
        HGOTO_ERROR(DFE_ARGS, FAIL);
    
    ret_value =(riid);

done:
  if(ret_value == 0)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(GR_mask, ID_GRgetlutid);
#endif /* HAVE_PABLO */
  return ret_value;
} /* end GRgetlutid() */

/*--------------------------------------------------------------------------
 NAME
    GRgetlutinfo

 PURPOSE
    Gets information about a LUT.

 USAGE
    intn GRgetlutinfo(lutid,ncomp,nt,il,nentries)
        int32 lutid;        IN: LUT ID from GRgetlutid
        int32 *ncomp;       OUT: number of components in LUT
        int32 *nt;          OUT: number type of components
        int32 *il;          OUT: interlace of the LUT
        int32 *nentries;    OUT: the number of entries for the LUT

 RETURNS
    SUCCEED/FAIL

 DESCRIPTION
    Gets information about a LUT.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
intn GRgetlutinfo(int32 lutid,int32 *ncomp,int32 *nt,int32 *il,int32 *nentries)
{
    CONSTR(FUNC, "GRgetlutinfo");   /* for HERROR */
    ri_info_t *ri_ptr;          /* ptr to the image to work with */
    intn  ret_value = SUCCEED;

#ifdef HAVE_PABLO
    TRACE_ON(GR_mask, ID_GRgetlutinfo);
#endif /* HAVE_PABLO */
    /* clear error stack and check validity of args */
    HEclear();

    /* check the validity of the RI ID */
    if (HAatom_group(lutid)!=RIIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);
    
    /* locate LUT's object in hash table */
    if (NULL == (ri_ptr = (ri_info_t *) HAatom_object(lutid)))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    if(ri_ptr->lut_ref==DFREF_WILDCARD) /* check for no palette defined currently */
      {
          if(ncomp!=NULL)
              *ncomp=0;
          if(nt!=NULL)
              *nt=DFNT_NONE;
          if(il!=NULL)
              *il=-1;
          if(nentries!=NULL)
              *nentries=0;
      } /* end if */
    else        /* we've got a valid palette currently */
      {
          if(ncomp!=NULL)
              *ncomp=ri_ptr->lut_dim.ncomps;
          if(nt!=NULL)
              *nt=ri_ptr->lut_dim.nt;
          if(il!=NULL)
              *il=ri_ptr->lut_dim.il;
          if(nentries!=NULL)  /* xdim for LUTs is the number of entries */
              *nentries=ri_ptr->lut_dim.xdim;
      } /* end else */

done:
  if(ret_value == 0)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(GR_mask, ID_GRgetlutinfo);
#endif /* HAVE_PABLO */
  return ret_value;
} /* end GRgetlutinfo() */

/*--------------------------------------------------------------------------
 NAME
    GRwritelut

 PURPOSE
    Writes out a LUT for an RI.

 USAGE
    intn GRwritelut(riid,ncomps,nt,il,nentries,data)
        int32 lutid;        IN: LUT ID from GRgetlutid
        int32 ncomp;        IN: number of components in LUT
        int32 nt;           IN: number type of components
        int32 il;           IN: interlace of the LUT
        int32 nentries;     IN: the number of entries for the LUT
        VOIDP data;         IN: LUT data to write out

 RETURNS
    SUCCEED/FAIL

 DESCRIPTION
    Writes out a LUT for an RI.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
intn GRwritelut(int32 lutid,int32 ncomps,int32 nt,int32 il,int32 nentries,VOIDP data)
{
    CONSTR(FUNC, "GRwritelut");   /* for HERROR */
    int32 hdf_file_id;          /* file ID from Hopen */
    ri_info_t *ri_ptr;          /* ptr to the image to work with */
    intn  ret_value = SUCCEED;

#ifdef HAVE_PABLO
    TRACE_ON(GR_mask, ID_GRwritelut);
#endif /* HAVE_PABLO */
    /* clear error stack and check validity of args */
    HEclear();

    /* check the validity of the args (how to check il?) */
    if (HAatom_group(lutid)!=RIIDGROUP || ncomps<1 || (DFKNTsize(nt)==FAIL)
            || nentries<1 || data==NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);
    
    /* locate LUT's object in hash table */
    if (NULL == (ri_ptr = (ri_info_t *) HAatom_object(lutid)))
        HGOTO_ERROR(DFE_NOVS, FAIL);
    hdf_file_id=ri_ptr->gr_ptr->hdf_file_id;

    /* Check if this is compatible with older-style palettes */
    if(ncomps==3 && nt==DFNT_UINT8 && il==MFGR_INTERLACE_PIXEL && nentries==256)
      {
          /* Check if LUT exists already */
          if(ri_ptr->lut_tag!=DFTAG_NULL && ri_ptr->lut_ref>DFREF_WILDCARD)
            {   /* LUT already exists */
                if(Hputelement(hdf_file_id,ri_ptr->lut_tag,ri_ptr->lut_ref,
                        data,ncomps*nentries*DFKNTsize(nt))==FAIL)
                    HGOTO_ERROR(DFE_PUTELEM,FAIL);
            } /* end if */
          else
            {   /* LUT does not exist */
                ri_ptr->lut_tag=DFTAG_LUT;
                ri_ptr->lut_ref=Htagnewref(hdf_file_id,ri_ptr->lut_tag);
                ri_ptr->lut_dim.dim_ref=DFREF_WILDCARD;
                ri_ptr->lut_dim.xdim=256;
                ri_ptr->lut_dim.ydim=1;
                ri_ptr->lut_dim.ncomps=3;
                ri_ptr->lut_dim.nt=DFNT_UINT8;
                ri_ptr->lut_dim.file_nt_subclass=DFNTF_HDFDEFAULT;
                ri_ptr->lut_dim.il=MFGR_INTERLACE_PIXEL;
                ri_ptr->lut_dim.nt_tag=DFTAG_NULL;
                ri_ptr->lut_dim.nt_ref=DFREF_WILDCARD;
                ri_ptr->lut_dim.comp_tag=DFTAG_NULL;
                ri_ptr->lut_dim.comp_ref=DFREF_WILDCARD;
                if(Hputelement(hdf_file_id,ri_ptr->lut_tag,ri_ptr->lut_ref,
                        data,ncomps*nentries*DFKNTsize(nt))==FAIL)
                    HGOTO_ERROR(DFE_PUTELEM,FAIL);

                ri_ptr->meta_modified=TRUE;
            } /* end else */
      } /* end if */
    else
      {     /* currently, we are not going to support non-standard palettes */
          HGOTO_ERROR(DFE_UNSUPPORTED,FAIL);
      } /* end else */

done:
  if(ret_value == 0)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(GR_mask, ID_GRwritelut);
#endif /* HAVE_PABLO */
  return ret_value;
} /* end GRwritelut() */

/*--------------------------------------------------------------------------
 NAME
    GRreadlut

 PURPOSE
    Reads a LUT from an RI.

 USAGE
    intn GRreadlut(lutid,data)
        int32 lutid;        IN: LUT ID from GRgetlutid
        VOIDP data;         IN: buffer for LUT data read in

 RETURNS
    SUCCEED/FAIL

 DESCRIPTION
    Reads a LUT from an RI.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
intn GRreadlut(int32 lutid,VOIDP data)
{
    CONSTR(FUNC, "GRreadlut");   /* for HERROR */
    int32 hdf_file_id;          /* file ID from Hopen */
    ri_info_t *ri_ptr;          /* ptr to the image to work with */
    intn  ret_value = SUCCEED;

#ifdef HAVE_PABLO
    TRACE_ON(GR_mask, ID_GRreadlut);
#endif /* HAVE_PABLO */
    /* clear error stack and check validity of args */
    HEclear();

    /* check the validity of the RI ID */
    if (HAatom_group(lutid)!=RIIDGROUP || data==NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);
    
    /* locate LUT's object in hash table */
    if (NULL == (ri_ptr = (ri_info_t *) HAatom_object(lutid)))
        HGOTO_ERROR(DFE_NOVS, FAIL);
    hdf_file_id=ri_ptr->gr_ptr->hdf_file_id;

    if(ri_ptr->lut_tag!=DFTAG_NULL && ri_ptr->lut_ref!=DFREF_WILDCARD)
      {
          if(Hgetelement(hdf_file_id,ri_ptr->lut_tag,ri_ptr->lut_ref,data)==FAIL)
              HGOTO_ERROR(DFE_GETELEM,FAIL);
      } /* end if */
    
    /* Re-format the palette into the user's requested interlacing */
    if(ri_ptr->lut_il!=MFGR_INTERLACE_PIXEL)
      {
          uintn pixel_mem_size;       /* size of a pixel in memory */
          VOIDP pixel_buf;  /* buffer for the pixel interlaced data */
          int32 count[2];   /* "dimension" info */

          pixel_mem_size=ri_ptr->lut_dim.ncomps*DFKNTsize((ri_ptr->lut_dim.nt | DFNT_NATIVE) & (~DFNT_LITEND));

          /* Allocate space for the conversion buffer */
          if((pixel_buf=HDmalloc(pixel_mem_size*ri_ptr->lut_dim.xdim))==NULL)
              HGOTO_ERROR(DFE_NOSPACE,FAIL);

          count[XDIM]=1;
          count[YDIM]=ri_ptr->lut_dim.xdim;
          GRIil_convert(data,MFGR_INTERLACE_PIXEL,pixel_buf,ri_ptr->lut_il,
              count,ri_ptr->lut_dim.ncomps,ri_ptr->lut_dim.nt);

          HDmemcpy(data,pixel_buf,pixel_mem_size*ri_ptr->lut_dim.xdim);

          HDfree(pixel_buf);
      } /* end if */

done:
  if(ret_value == 0)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(GR_mask, ID_GRreadlut);
#endif /* HAVE_PABLO */
  return ret_value;
} /* end GRreadlut() */

/*--------------------------------------------------------------------------
 NAME
    GRsetexternalfile

 PURPOSE
    Makes the image data of an RI into an external element special element.

 USAGE
    intn GRsetexternalfile(riid,filename,offset)
        int32 riid;         IN: RI ID from GRselect/GRcreate
        char *filename;     IN: name of the external file
        int32 offset;       IN: offset in the external file to store the image

 RETURNS
    SUCCEED/FAIL

 DESCRIPTION
    Makes the image data of an RI into an external element special element.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
intn GRsetexternalfile(int32 riid,char *filename,int32 offset)
{
    CONSTR(FUNC, "GRsetexternalfile");   /* for HERROR */
    ri_info_t *ri_ptr;          /* ptr to the image to work with */
    intn  ret_value = SUCCEED;

#ifdef HAVE_PABLO
    TRACE_ON(GR_mask, ID_GRsetexternalfile);
#endif /* HAVE_PABLO */
    /* clear error stack and check validity of args */
    HEclear();

    /* check the validity of the RI ID */
    if (HAatom_group(riid)!=RIIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);
    
    /* locate RI's object in hash table */
    if (NULL == (ri_ptr = (ri_info_t *) HAatom_object(riid)))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    if((ri_ptr->ext_name=(char *)HDmalloc(HDstrlen(filename)+1))==NULL)
        HGOTO_ERROR(DFE_NOSPACE,FAIL);

    /* Mark the image as external and cache parameters */
    ri_ptr->ext_img=TRUE;
    HDstrcpy(ri_ptr->ext_name,filename);
    ri_ptr->ext_offset=offset;

done:
  if(ret_value == 0)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(GR_mask, ID_GRsetexternalfile);
#endif /* HAVE_PABLO */
  return ret_value;
} /* end GRsetexternalfile() */

/*--------------------------------------------------------------------------
 NAME
    GRsetaccesstype

 PURPOSE
    Sets the access for an RI to be either serial or parallel I/O.

 USAGE
    intn GRsetaccesstype(riid,accesstype)
        int32 riid;         IN: RI ID from GRselect/GRcreate
        uintn accesstype;   IN: access type for image data, from the following
                                values:
                                    DFACC_SERIAL - for serial access
                                    DFACC_PARALLEL - for parallel access

 RETURNS
    SUCCEED/FAIL

 DESCRIPTION
    Sets the access for an RI to be either serial or parallel I/O.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
intn GRsetaccesstype(int32 riid,uintn accesstype)
{
    CONSTR(FUNC, "GRsetaccesstype");   /* for HERROR */
    ri_info_t *ri_ptr;          /* ptr to the image to work with */
    intn  ret_value = SUCCEED;

#ifdef HAVE_PABLO
    TRACE_ON(GR_mask, ID_GRsetaccesstype);
#endif /* HAVE_PABLO */
    /* clear error stack and check validity of args */
    HEclear();

    /* check the validity of the RI ID */
    if (HAatom_group(riid)!=RIIDGROUP || (accesstype!=DFACC_DEFAULT && accesstype!=DFACC_SERIAL
            && accesstype!=DFACC_PARALLEL))
        HGOTO_ERROR(DFE_ARGS, FAIL);
    
    /* locate RI's object in hash table */
    if (NULL == (ri_ptr = (ri_info_t *) HAatom_object(riid)))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    /* Mark the image as having an access-mode and cache args */
    ri_ptr->acc_img=TRUE;
    ri_ptr->acc_type=accesstype;

done:
  if(ret_value == 0)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(GR_mask, ID_GRsetaccesstype);
#endif /* HAVE_PABLO */
  return ret_value;
} /* end GRsetaccesstype() */

/*--------------------------------------------------------------------------
 NAME
    GRsetcompress

 PURPOSE
    Compressed the image data of an RI.

 USAGE
    intn GRsetcompress(riid,comp_type,cinfo)
        int32 riid;         IN: RI ID from GRselect/GRcreate
        int32 comp_type;    IN: type of compression, from list in hcomp.h
        comp_info *cinfo;   IN: compression specific information

 RETURNS
    SUCCEED/FAIL

 DESCRIPTION
    Compressed the image data of an RI.
    (Makes the image data of an RI into a compressed special element)

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
intn GRsetcompress(int32 riid,int32 comp_type,comp_info *cinfo)
{
    CONSTR(FUNC, "GRsetcompress");   /* for HERROR */
    ri_info_t *ri_ptr;          /* ptr to the image to work with */
    intn  ret_value = SUCCEED;

#ifdef HAVE_PABLO
    TRACE_ON(GR_mask, ID_GRsetcompress);
#endif /* HAVE_PABLO */
    /* clear error stack and check validity of args */
    HEclear();

    /* check the validity of the RI ID */
    if (HAatom_group(riid)!=RIIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);
    
    /* locate RI's object in hash table */
    if (NULL == (ri_ptr = (ri_info_t *) HAatom_object(riid)))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    /* Mark the image as being compressed and cache args */
    ri_ptr->comp_img=TRUE;
    ri_ptr->comp_type=comp_type;
    HDmemcpy(&(ri_ptr->cinfo),cinfo,sizeof(comp_info));

done:
  if(ret_value == 0)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(GR_mask, ID_GRsetcompress);
#endif /* HAVE_PABLO */
  return ret_value;
} /* end GRsetcompress() */

/*--------------------------------------------------------------------------
 NAME
    GRsetattr

 PURPOSE
    Write an attribute for an object.

 USAGE
    intn GRsetattr(dimid|riid|grid,name,attr_nt,count,data)
        int32 dimid|riid|grid;  IN: DIM|RI|GR ID
        char *name;             IN: name of attribute
        int32 attr_nt;          IN: number-type of attribute
        int32 count;            IN: number of entries of the attribute
        VOIDP data;             IN: attribute data to write

 RETURNS
    SUCCEED/FAIL

 DESCRIPTION
    Write an attribute for an object (function will figure out ID type).

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
    Currently does not allow changing NT of an existing attribute.

 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
intn GRsetattr(int32 id,const char *name,int32 attr_nt,int32 count,const VOIDP data)
{
    CONSTR(FUNC, "GRsetattr");   /* for HERROR */
    int32 hdf_file_id;          /* HDF file ID from Hopen */
    gr_info_t *gr_ptr;          /* ptr to the GR information for this grid */
    ri_info_t *ri_ptr=NULL;     /* ptr to the image to work with */
    VOIDP *t;                   /* temp. ptr to the image found */
    TBBT_TREE *search_tree;     /* attribute tree to search through */
    at_info_t *at_ptr=NULL;     /* ptr to the attribute to work with */
    int32 at_size;              /* size in bytes of the attribute data */
    int32 *update_count;        /* pointer to the count of attributes in a tree */
    uintn *update_flag;         /* pointer to the flag to indicate an attribute tree is changed */
    uintn found=FALSE;          /* boolean for indicating the attribute exists already */
    intn is_riid=FALSE;         /* whether we had a RIID */
    intn ret_value = SUCCEED;

#ifdef HAVE_PABLO
    TRACE_ON(GR_mask, ID_GRsetattr);
#endif /* HAVE_PABLO */
    /* clear error stack and check validity of args */
    HEclear();

#ifdef QAK
printf("%s: entering\n",FUNC);
printf("%s: id=%ld, name=%p, data=%p, count=%ld\n",FUNC,(long)id,name,data,(long)count);
#endif /* QAK */
    /* Make sure that count is less than MAX_ORDER(Vdata)
           and total size is less than MAX_FIELD_SIZE(Vdata) */
    if ((count > MAX_ORDER) ||
        ((count * DFKNTsize(attr_nt)) > MAX_FIELD_SIZE))
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* check the validity of the args */
    if ((HAatom_group(id)!=RIIDGROUP && HAatom_group(id)!=GRIDGROUP) || data==NULL || name==NULL
            || count<=0 || DFKNTsize(attr_nt)==FAIL)
        HGOTO_ERROR(DFE_ARGS, FAIL);
    
    if (HAatom_group(id)==GRIDGROUP)
      {
#ifdef QAK
printf("%s: got a GRID\n",FUNC);
#endif /* QAK */
          /* locate GR's object in hash table */
          if (NULL == (gr_ptr = (gr_info_t *) HAatom_object(id)))
              HGOTO_ERROR(DFE_NOVS, FAIL);

          hdf_file_id=gr_ptr->hdf_file_id;
          search_tree=gr_ptr->gattree;
          update_flag=&(gr_ptr->gattr_modified);
#ifdef QAK
printf("%s: gr_ptr->gattr_count=%ld\n",FUNC,(long)gr_ptr->gattr_count);
#endif /* QAK */
          update_count=&(gr_ptr->gattr_count);
      } /* end if */
    else if (HAatom_group(id)==RIIDGROUP)
      {
          /* Need this flag for later */
          is_riid=TRUE;

#ifdef QAK
printf("%s: got a RIID, riid=%lx\n",FUNC,(long unsigned)id);
#endif /* QAK */
          /* locate RI's object in hash table */
          if (NULL == (ri_ptr = (ri_info_t *) HAatom_object(id)))
              HGOTO_ERROR(DFE_NOVS, FAIL);
          gr_ptr=ri_ptr->gr_ptr;
#ifdef QAK
printf("%s: got a RIID, ri_ptr=%p, gr_ptr=%p\n",FUNC,ri_ptr,gr_ptr);
#endif /* QAK */

          hdf_file_id=gr_ptr->hdf_file_id;
#ifdef QAK
printf("%s: hdf_file_id=%lx\n",FUNC,(long unsigned)hdf_file_id);
#endif /* QAK */
          search_tree=ri_ptr->lattree;
          update_flag=&(ri_ptr->attr_modified);
          update_count=&(ri_ptr->lattr_count);
      } /* end if */
    else    /* shouldn't get here, but what the heck... */
        HGOTO_ERROR(DFE_ARGS, FAIL);

#ifdef QAK
printf("%s: check 1, search_tree=%p\n",FUNC,search_tree);
#endif /* QAK */
    /* Search for an attribute with the same name */
    if((t = (VOIDP *) tbbtfirst((TBBT_NODE *)*search_tree))!=NULL)
      {
#ifdef QAK
printf("%s: t=%p\n",FUNC,t);
#endif /* QAK */
          do {
              at_ptr=(at_info_t *)*t;
#ifdef QAK
printf("%s: at_ptr=%p\n",FUNC,at_ptr);
if(at_ptr!=NULL)
{
    printf("%s: at_ptr->name=%p, at_ptr->index=%ld, name=%p\n",FUNC,at_ptr->name,(long)at_ptr->index,name);
    if(at_ptr->name!=NULL && name!=NULL)
        printf("%s: at_ptr->name=%s, name=%s\n",FUNC,at_ptr->name,name);
}
#endif /* QAK */
              if(at_ptr!=NULL && HDstrcmp(at_ptr->name,name)==0)  /* ie. the name matches */
                {
                    found=TRUE;
                    break;
                } /* end if */
#ifdef QAK
printf("%s: check 1.9: found=%d\n",FUNC,found);
#endif /* QAK */
          } while((t= (VOIDP *)tbbtnext((TBBT_NODE *)t))!=NULL);
      } /* end if */

#ifdef QAK
printf("%s: check 2: found=%d\n",FUNC,found);
#endif /* QAK */
    if(found==TRUE) /* attribute already exists, just update it */
      {
          int32 new_at_size;          /* size in bytes of the new attribute data */

#ifdef QAK
printf("%s: found existing attribute\n",FUNC);
#endif /* QAK */
          /* Catch the user if he tries to change the NT */
          if(attr_nt!=at_ptr->nt)
              HGOTO_ERROR(DFE_ARGS, FAIL);

          /* Calc. old & new attr. sizes */
          new_at_size=count*DFKNTsize((attr_nt | DFNT_NATIVE) & (~DFNT_LITEND));
          at_size=at_ptr->len*DFKNTsize((at_ptr->nt | DFNT_NATIVE) & (~DFNT_LITEND));

          if(new_at_size>gr_ptr->attr_cache)    /* check if data is cacheable */
            {   /* not cacheable, write directly out to disk and throw away old in-memory copy */
                int32 AttrID;       /* attribute Vdata id */

                /* Update data on disk */
                if((AttrID=VSattach(hdf_file_id,at_ptr->ref,"w"))==FAIL)
                    HGOTO_ERROR(DFE_CANTATTACH,FAIL);
                if(VSsetfields(AttrID,at_ptr->name)==FAIL)
                  {
                    VSdetach(AttrID);
                    HGOTO_ERROR(DFE_BADFIELDS,FAIL);
                  } /* end if */
                if(VSwrite(AttrID,data,count,FULL_INTERLACE)==FAIL)
                  {
                    VSdetach(AttrID);
                    HGOTO_ERROR(DFE_VSWRITE,FAIL);
                  } /* end if */
                if(VSdetach(AttrID)==FAIL)
                    HGOTO_ERROR(DFE_CANTDETACH,FAIL);

                /* Update in-memory fields */
                at_ptr->len=count;
                at_ptr->data_modified=FALSE;

                /* Toss the old data, it's not valid and it's un-cacheable now */
                if(at_ptr->data!=NULL)
                    HDfreenclear(at_ptr->data);
            }  /* end if */
          else
            {
                /* check if we need a bigger buffer */
                if(new_at_size>at_size || at_ptr->data==NULL)
                  {
                      if(at_ptr->data!=NULL)
                          HDfree(at_ptr->data);
                      if((at_ptr->data=(VOIDP)HDmalloc(new_at_size))==NULL)
                          HGOTO_ERROR(DFE_NOSPACE,FAIL);
                  } /* end if */
                HDmemcpy(at_ptr->data,data,new_at_size);

                /* Update in-memory fields */
                at_ptr->len=count;
                at_ptr->data_modified=TRUE;
                *update_flag=TRUE;  /* flag the tree as changed */
            } /* end else */
      } /* end if */
    else    /* a new attribute */
      {
#ifdef QAK
printf("%s: check 2.5: creating new attribute\n",FUNC);
#endif /* QAK */
        if((at_ptr=(at_info_t *)HDmalloc(sizeof(at_info_t)))==NULL)
            HGOTO_ERROR(DFE_NOSPACE,FAIL);

        /* Fill in fields for the new attribute */
#ifdef QAK
printf("%s:1: gr_ptr->gattr_count=%ld\n",FUNC,(long)gr_ptr->gattr_count);
#endif /* QAK */
        at_ptr->index=*update_count;  /* get the index and update the tree's count */
        at_ptr->nt=attr_nt;
        at_ptr->len=count;

        /* allocate space for the attribute name & copy it */
        if((at_ptr->name=(char *)HDmalloc(HDstrlen(name)+1))==NULL)
            HGOTO_ERROR(DFE_NOSPACE,FAIL);
        HDstrcpy(at_ptr->name,name);

        /* calc. the attr size to see if it is worth caching */
        at_size=at_ptr->len*DFKNTsize((at_ptr->nt | DFNT_NATIVE) & (~DFNT_LITEND));
        if(at_size<gr_ptr->attr_cache)
          { /* cacheable */
              /* allocate space for the attribute name & copy it */
              if((at_ptr->data=(char *)HDmalloc(at_size))==NULL)
                  HGOTO_ERROR(DFE_NOSPACE,FAIL);
              HDmemcpy(at_ptr->data,data,at_size);
              at_ptr->data_modified=TRUE;
              at_ptr->ref=DFREF_WILDCARD;
          } /* end if */
        else
          { /* non-cacheable */
#ifdef QAK
printf("%s: hdf_file_id=%lx\n",FUNC,(long unsigned)hdf_file_id);
#endif /* QAK */
              if((at_ptr->ref=(uint16)VHstoredata(hdf_file_id,at_ptr->name,data,
                      at_ptr->len,at_ptr->nt,RIGATTRNAME,RIGATTRCLASS))==(uint16)FAIL)
                  HGOTO_ERROR(DFE_VSCANTCREATE,FAIL);
              at_ptr->data=NULL;
              at_ptr->data_modified=FALSE;
          } /* end else */
        at_ptr->new_at=TRUE;

        /* Add the attribute to the attribute tree */
        if(tbbtdins(search_tree, (VOIDP)at_ptr, NULL)==NULL)
            HGOTO_ERROR(DFE_TBBTINS,FAIL);

        /* flag the attribute tree as being modified */
        *update_flag=TRUE;
        (*update_count)++;  /* get the index and update the tree's count */
#ifdef QAK
printf("%s:2: gr_ptr->gattr_count=%ld\n",FUNC,(long)gr_ptr->gattr_count);
#endif /* QAK */
      } /* end else */

    /* set this flag also, if we set local attributes */
    if(is_riid==TRUE) 
      {
        if(found==FALSE)    /* when adding new attributes, make certain the RI gets updated */
            ri_ptr->meta_modified=TRUE;
        gr_ptr->gr_modified=TRUE;
      } /* end if */

#ifdef QAK
printf("%s: check 3\n",FUNC);
#endif /* QAK */

done:
  if(ret_value == 0)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(GR_mask, ID_GRsetattr);
#endif /* HAVE_PABLO */
  return ret_value;
} /* end GRsetattr() */

/*--------------------------------------------------------------------------
 NAME
    GRattrinfo

 PURPOSE
    Get attribute information for an object.

 USAGE
    intn GRattrinfo(dimid|riid|grid,index,name,attr_nt,count)
        int32 dimid|riid|grid;  IN: DIM|RI|GR ID
        int32 index;            IN: index of the attribute for info
        char *name;             OUT: name of attribute
        int32 attr_nt;          OUT: number-type of attribute
        int32 count;            OUT: number of entries of the attribute

 RETURNS
    SUCCEED/FAIL

 DESCRIPTION
    Get attribute information for an object.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
intn GRattrinfo(int32 id,int32 index,char *name,int32 *attr_nt,int32 *count)
{
    CONSTR(FUNC, "GRattrinfo");   /* for HERROR */
    gr_info_t *gr_ptr;          /* ptr to the GR information for this grid */
    ri_info_t *ri_ptr;          /* ptr to the image to work with */
    VOIDP *t;                   /* temp. ptr to the image found */
    TBBT_TREE *search_tree;     /* attribute tree to search through */
    at_info_t *at_ptr;          /* ptr to the attribute to work with */
    intn   ret_value = SUCCEED;

#ifdef HAVE_PABLO
    TRACE_ON(GR_mask, ID_GRattrinfo);
#endif /* HAVE_PABLO */
    /* clear error stack and check validity of args */
    HEclear();

    /* check the validity of the ID, the index is checked below */
    if (HAatom_group(id)!=RIIDGROUP && HAatom_group(id)!=GRIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);
    
    if (HAatom_group(id)==GRIDGROUP)
      {
#ifdef QAK
printf("%s: found a GRID\n",FUNC);
#endif /* QAK */
          /* locate GR's object in hash table */
          if (NULL == (gr_ptr = (gr_info_t *) HAatom_object(id)))
              HGOTO_ERROR(DFE_NOVS, FAIL);

          if(index<0 || index>=gr_ptr->gattr_count)
              HGOTO_ERROR(DFE_ARGS, FAIL);

          search_tree=gr_ptr->gattree;
      } /* end if */
    else if (HAatom_group(id)==RIIDGROUP)
      {
#ifdef QAK
printf("%s: found a RIID\n",FUNC);
#endif /* QAK */
          /* locate RI's object in hash table */
          if (NULL == (ri_ptr = (ri_info_t *) HAatom_object(id)))
              HGOTO_ERROR(DFE_NOVS, FAIL);

          if(index<0 || index>=ri_ptr->lattr_count)
              HGOTO_ERROR(DFE_ARGS, FAIL); 
          search_tree=ri_ptr->lattree;
      } /* end if */
    else    /* shouldn't get here, but what the heck... */
        HGOTO_ERROR(DFE_ARGS, FAIL);

    if((t = (VOIDP *) tbbtdfind(search_tree, (VOIDP) &index, NULL))==NULL)
        HGOTO_ERROR(DFE_RINOTFOUND,FAIL);
    at_ptr=(at_info_t *)*t;

    if(name!=NULL)
        HDstrcpy(name,at_ptr->name);
    if(attr_nt!=NULL)
        *attr_nt=at_ptr->nt;
    if(count!=NULL)
        *count=at_ptr->len;

done:
  if(ret_value == 0)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(GR_mask, ID_GRattrinfo);
#endif /* HAVE_PABLO */
  return ret_value;
} /* end GRattrinfo() */

/*--------------------------------------------------------------------------
 NAME
    GRgetattr

 PURPOSE
    Read an attribute for an object.

 USAGE
    intn GRgetattr(dimid|riid|grid,index,data)
        int32 dimid|riid|grid;  IN: DIM|RI|GR ID
        int32 index;            IN: index of the attribute for info
        VOIDP data;             OUT: data read for attribute

 RETURNS
    SUCCEED/FAIL

 DESCRIPTION
    Read an attribute for an object.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
intn GRgetattr(int32 id,int32 index,VOIDP data)
{
    CONSTR(FUNC, "GRgetattr");   /* for HERROR */
    int32 hdf_file_id;          /* HDF file ID from Hopen */
    gr_info_t *gr_ptr;          /* ptr to the GR information for this grid */
    ri_info_t *ri_ptr;          /* ptr to the image to work with */
    VOIDP *t;                   /* temp. ptr to the image found */
    TBBT_TREE *search_tree;     /* attribute tree to search through */
    at_info_t *at_ptr;          /* ptr to the attribute to work with */
    int32 at_size;              /* size in bytes of the attribute data */
    intn  ret_value = SUCCEED;

#ifdef HAVE_PABLO
    TRACE_ON(GR_mask, ID_GRgetattr);
#endif /* HAVE_PABLO */
    /* clear error stack and check validity of args */
    HEclear();

    /* check the validity of the ID & data ptr, the index is checked below */
    if ((HAatom_group(id)!=RIIDGROUP && HAatom_group(id)!=GRIDGROUP) || data==NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);
    
    if (HAatom_group(id)==GRIDGROUP)
      {
          /* locate GR's object in hash table */
          if (NULL == (gr_ptr = (gr_info_t *) HAatom_object(id)))
              HGOTO_ERROR(DFE_NOVS, FAIL);

          if(index<0 || index>=gr_ptr->gattr_count)
              HGOTO_ERROR(DFE_ARGS, FAIL);

          search_tree=gr_ptr->gattree;
      } /* end if */
    else if (HAatom_group(id)==RIIDGROUP)
      {
          /* locate RI's object in hash table */
          if (NULL == (ri_ptr = (ri_info_t *) HAatom_object(id)))
              HGOTO_ERROR(DFE_NOVS, FAIL);
          gr_ptr=ri_ptr->gr_ptr;

          if(index<0 || index>=ri_ptr->lattr_count)
              HGOTO_ERROR(DFE_ARGS, FAIL);

          search_tree=ri_ptr->lattree;
      } /* end if */
    else    /* shouldn't get here, but what the heck... */
        HGOTO_ERROR(DFE_ARGS, FAIL);

    hdf_file_id=gr_ptr->hdf_file_id;
    if((t = (VOIDP *) tbbtdfind(search_tree, (VOIDP) &index, NULL))==NULL)
        HGOTO_ERROR(DFE_RINOTFOUND,FAIL);
    at_ptr=(at_info_t *)*t;

    /* Calculate the size of the attribute data */
    at_size=at_ptr->len*DFKNTsize((at_ptr->nt | DFNT_NATIVE) & (~DFNT_LITEND));

    /* Check if the attribute has been read in yet, and get it if not */
    if(at_ptr->data==NULL)
      {
        int32 AttrID;       /* attribute Vdata id */

        /* Grab some memory for the attribute data */
        if((at_ptr->data=HDmalloc(at_size))==NULL)
            HGOTO_ERROR(DFE_NOSPACE,FAIL);
            
        if((AttrID=VSattach(hdf_file_id,at_ptr->ref,"r"))==FAIL)
            HGOTO_ERROR(DFE_CANTATTACH,FAIL);
        if(VSsetfields(AttrID,at_ptr->name)==FAIL)
          {
            VSdetach(AttrID);
            HGOTO_ERROR(DFE_BADFIELDS,FAIL);
          } /* end if */
        if(VSread(AttrID,at_ptr->data,at_ptr->len,FULL_INTERLACE)==FAIL)
          {
            VSdetach(AttrID);
            HGOTO_ERROR(DFE_VSWRITE,FAIL);
          } /* end if */
        if(VSdetach(AttrID)==FAIL)
            HGOTO_ERROR(DFE_CANTDETACH,FAIL);
      } /* end if */

    /* Copy the attribute into the user's buffer */
    HDmemcpy(data,at_ptr->data,at_size);

    /* If the attribute is too large to keep in memory, chuck it again */
    if(at_size>gr_ptr->attr_cache)
        HDfreenclear(at_ptr->data);

done:
  if(ret_value == 0)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(GR_mask, ID_GRgetattr);
#endif /* HAVE_PABLO */
  return ret_value;
} /* end GRgetattr() */

/*--------------------------------------------------------------------------
 NAME
    GRfindattr

 PURPOSE
    Get the index of an attribute with a given name for an object.

 USAGE
    int32 GRfindattr(int32 dimid|riid|grid,char *name)
        int32 dimid|riid|grid;  IN: DIM|RI|GR ID
        char *name;             IN: name of attribute to search for

 RETURNS
    Valid index for an attribute on success, FAIL on failure

 DESCRIPTION
    Get the index of an attribute with a given name for an object.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
int32 GRfindattr(int32 id,const char *name)
{
    CONSTR(FUNC, "GRfindattr");   /* for HERROR */
    gr_info_t *gr_ptr;          /* ptr to the GR information for this grid */
    ri_info_t *ri_ptr;          /* ptr to the image to work with */
    VOIDP *t;                   /* temp. ptr to the image found */
    TBBT_TREE *search_tree;     /* attribute tree to search through */
    at_info_t *at_ptr;          /* ptr to the attribute to work with */
    int32 ret_value = SUCCEED;

#ifdef HAVE_PABLO
    TRACE_ON(GR_mask, ID_GRfindattr);
#endif /* HAVE_PABLO */
    /* clear error stack and check validity of args */
    HEclear();

    /* check the validity of the RI ID */
    if (HAatom_group(id)!=RIIDGROUP && HAatom_group(id)!=GRIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);
    
    if (HAatom_group(id)==GRIDGROUP)
      {
          /* locate GR's object in hash table */
          if (NULL == (gr_ptr = (gr_info_t *) HAatom_object(id)))
              HGOTO_ERROR(DFE_NOVS, FAIL);

          search_tree=gr_ptr->gattree;
      } /* end if */
    else if (HAatom_group(id)==RIIDGROUP)
      {
          /* locate RI's object in hash table */
          if (NULL == (ri_ptr = (ri_info_t *) HAatom_object(id)))
              HGOTO_ERROR(DFE_NOVS, FAIL);

          search_tree=ri_ptr->lattree;
      } /* end if */
    else    /* shouldn't get here, but what the heck... */
        HGOTO_ERROR(DFE_ARGS, FAIL);

    if((t = (VOIDP *) tbbtfirst((TBBT_NODE *)*search_tree))==NULL)
        HGOTO_ERROR(DFE_RINOTFOUND,FAIL);
    do {
        at_ptr=(at_info_t *)*t;
        if(at_ptr!=NULL && HDstrcmp(at_ptr->name,name)==0)  /* ie. the name matches */
            HGOTO_DONE(at_ptr->index);
    } while((t= (VOIDP *) tbbtnext((TBBT_NODE *)t))!=NULL);

    ret_value = (FAIL);

done:
  if(ret_value == 0)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(GR_mask, ID_GRfindattr);
#endif /* HAVE_PABLO */
  return ret_value;
} /* end GRfindattr() */

/*--------------------------------------------------------------------------
 NAME
    GRPshutdown
 PURPOSE
    Terminate various static buffers.
 USAGE
    intn GRPshutdown()
 RETURNS
    Returns SUCCEED/FAIL
 DESCRIPTION
    Free various buffers allocated in the GR routines.
 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
    Should only ever be called by the "atexit" function HDFend
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
intn GRPshutdown(void)
{
    if(gr_tree!=NULL)
      {
          /* Free the vfile tree */
          tbbtdfree(gr_tree, GRIgrdestroynode, NULL);

          /* Destroy the atom groups for GRs and RIs */
          HAdestroy_group(GRIDGROUP);
          HAdestroy_group(RIIDGROUP);

          gr_tree=NULL;
      } /* end if */
    return(SUCCEED);
} /* end GRPshutdown() */

/*

API functions to finish:

Misc. stuff left to do:
    Deal with special elements for images.
    GRrename for images.
    GRsetflags to suppress writing fill data and to suppress fillvalue attr.
    Read/Write Interlaced data (of line & plane types, pixel is working).

Features not supported:
    Add in full support for multiple palettes with each RI.
    Add in support for named palettes with each RI.
    Add in support for palettes with non-standard formats.
    Deletion of attributes or images (would require changing the way index
        numbers are handled)

*/

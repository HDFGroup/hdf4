/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF/releases/.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*-----------------------------------------------------------------------------
 * File:     mfanf.c
 * Purpose:  C-stubs for multi-file Fortran annotation routines
 * Invokes:  C-Routines in "mfan.c"
 * Contents: SEE annotation source/header files "mfan.c" and "mfan.h"
 *           for more info on the annotation interface.
 *
 *  NOTES: TYPE here refers to file/data label/description types
 *         They are AN_FILE_LABEL, AN_FILE_DESC, AN_DATA_LABEL, AN_DATA_DESC
 *         THE tag/ref refers to data tag/ref. The fortran equivalents
 *         are defined in 'hdf.inc'.
 *
 *  C-stubs directly callable by Fortran Users
 *  ------------------------------------------
 *    afstart    - start annotation access on file and return annotaton id
 *    affileinfo - get number of file/data annotations in file.
 *                 Indices returned are used in afselect() calls.
 *    afend      - end access to annotation handling on file
 *    afcreate   - create a new data annotation and return a handle
 *    affcreate  - create a new file annotation and return a handle
 *    afselect   - returns an annotation handle(ann_id) from index for
 *                 a particular annotation TYPE. This handle is then used for
 *                 calls like afwriteann(), afreadann(), afannlen(),..etc
 *    afnumann   - return number of annotations that match TYPE/tag/ref
 *    afannlist  - return list of handles that match TYPE/tag/ref
 *    afannlen   - get length of annotation given handle
 *    afwriteann - write annotation given handle
 *    afreadann  - read annotation given handle
 *    afendaccess - end access to annotation using handle
 *    afgettagref - get tag/ref pair to annotation ID
 *    afidtagref  - get tag/ref given annotation id
 *    aftagrefid  - get annotation id given tag/ref
 *    afatypetag  - annotation type to corresponding annotation TAG
 *    aftagatype  - annotation TAG to corresponding annotation type
 *---------------------------------------------------------------------------*/

#include "hdf_priv.h"
#include "hproto_fortran.h"

/*-----------------------------------------------------------------------------
  FUNCTION NAMING CONVENTION:
  ---------------------------
  This file contains the HDF-style C stubs for the multi-file Annotation
  interface. They call the corresponding C-functions in "mfan.c"

  The basic routines called by fortran will be of the form afxxxx
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name:    afstart
 * Purpose: Open file for annotation handling
 * Inputs:  file_id: id of HDF file
 * Returns: annotation interface handle on SUCCEED and FAIL otherwise
 * Users:
 * Invokes: ANstart()
 *---------------------------------------------------------------------------*/
intf
nafstart(intf *file_id)
{
    intf ret;

    ret = ANstart((int32)*file_id);

    return ret;
}

/*-----------------------------------------------------------------------------
 * Name:    affileinfo
 * Purpose: Get number of file/data annotations in file.
 * Inputs:  IN an_id:     annotation interface handle
 *          OUT num_flabel: number of file labels in file
 *          OUT num_fdesc:  number of file descriptions in file
 *          OUT num_olabel: number of data labels in file
 *          OUT num_odesc:  number of data descriptions in file
 * Returns: see ANfileinfo()
 * Users:   Fortran Users
 * Invokes: ANfileinfo()
 *---------------------------------------------------------------------------*/
intf
naffileinfo(intf *an_id, intf *num_flabel, intf *num_fdesc, intf *num_olabel, intf *num_odesc)
{
    intf  ret;
    int32 nflabel, nfdesc, nolabel, nodesc;

    ret = ANfileinfo((int32)*an_id, &nflabel, &nfdesc, &nolabel, &nodesc);

    /* fill in values to return */
    *num_flabel = nflabel;
    *num_fdesc  = nfdesc;
    *num_olabel = nolabel;
    *num_odesc  = nodesc;

    return ret;
}

/*-----------------------------------------------------------------------------
 * Name:    afend
 * Purpose: End access to annotation handling on file
 * Inputs:  file_id:
 * Returns: see ANend()
 * Users:   Fortran Users
 * Invokes: ANend()
 *---------------------------------------------------------------------------*/
intf
nafend(intf *an_id)
{
    return (intf)ANend((int32)*an_id);
}

/*-----------------------------------------------------------------------------
 * Name:    afcreate
 * Purpose: Create a new data annotation and return an annotation handle
 * Inputs:  an_id: annotation interface handle
 *          etag:    tag of data to annotate
 *          eref:    ref of data to annotate
 *          atype:   annotation type AN_DATA_LABEL, AN_DATA_DESC
 * Returns: see ANcreate()
 * Users:   Fortran Users
 * Invokes: ANcreate()
 *---------------------------------------------------------------------------*/
intf
nafcreate(intf *an_id, intf *etag, intf *eref, intf *atype)
{
    return (intf)ANcreate((int32)*an_id, (uint16)*etag, (uint16)*eref, (ann_type)*atype);
}

/*-----------------------------------------------------------------------------
 * Name:    affcreate
 * Purpose: Create a new file annotation and return an annotation handle
 * Inputs:  an_id: annottion interface handle
 *          atype:   annotation type AN_FILE_LABEL, AN_DATA_DESC
 * Returns: see ANcreatef()
 * Users:   Fortran Users
 * Invokes: ANcreatf()
 *---------------------------------------------------------------------------*/
intf
naffcreate(intf *an_id, intf *atype)
{
    return (intf)ANcreatef((int32)*an_id, (ann_type)*atype);
}

/*-----------------------------------------------------------------------------
 * Name:    afselect
 * Purpose: returns an annotation handle(ann_id) from index for
 *          a particular annotation TYPE. This handle is then used for
 *          calls like afwriteann(), afreadann(), afannlen(),..etc
 * Inputs:  an_id: annotation interface handle
 *          index:   index for particular annotation type. Usually based on
 *                   number of a particular type obtained from affileinfo()call.
 *                   ZERO based.
 *          atype:   annotation type AN_FILE_LABEL, AN_FILE_DESC, AN_DATA_LABEL
 *                   AN_DATA_DESC
 * Returns: see ANselect()
 * Users:   Fortran Users
 * Invokes: ANselect()
 *---------------------------------------------------------------------------*/
intf
nafselect(intf *an_id, intf *index, intf *atype)
{
    return (intf)ANselect((int32)*an_id, (int32)*index, (ann_type)*atype);
}

/*-----------------------------------------------------------------------------
 * Name:    afnumann
 * Purpose: Return number of annotations that match TYPE/tag/ref
 * Inputs:  an_id: annotation interface handle
 *          atype:   annotation type AN_DATA_LABEL, AN_DATA_DESC
 *          etag:    data tag to match
 *          eref:    data ref to match
 * Returns: see ANnumann()
 * Users:   Fortran Users
 * Invokes: ANnumann()
 *---------------------------------------------------------------------------*/
intf
nafnumann(intf *an_id, intf *atype, intf *etag, intf *eref)
{
    return (intf)ANnumann((int32)*an_id, (ann_type)*atype, (uint16)*etag, (uint16)*eref);
}

/*-----------------------------------------------------------------------------
 * Name:    afannlist
 * Purpose: Return list of handles that match TYPE/tag/ref
 * Inputs:  IN an_id: annotation interface handle
 *          IN atype:   annotation type AN_DATA_LABEL, AN_DATA_DESC
 *          IN etag:    data tag to match
 *          IN eref:    data ref to match
 *          OUT alist[]: list of annotation handles found that match tag/ref
 * Returns: number of annotations found that match else FAIL. see ANannlist()
 * Users:   Fortran Users
 * Invokes: ANnumann(), ANannlist()
 *---------------------------------------------------------------------------*/
intf
nafannlist(intf *an_id, intf *atype, intf *etag, intf *eref, intf alist[])
{
    intf   ret;
    int32 *tempanlist;
    intf   nanns;
    int    i;

    /* Get number of annotations that match tag/ref pair */
    nanns = ANnumann((int32)*an_id, (ann_type)*atype, (uint16)*etag, (uint16)*eref);
    if (nanns < 0)
        HE_REPORT_RETURN("ANnumann: failed to any annotations", FAIL);

    /* create annlist with true int32s to maintain compatibility
    ** with machines that allocate less than 32 bits per int. */
    if ((tempanlist = (int32 *)malloc(nanns * sizeof(int32))) == NULL)
        HRETURN_ERROR(DFE_NOSPACE, FAIL);

    /* Get list of annotation handles to return */
    ret = ANannlist((int32)*an_id, (ann_type)*atype, (uint16)*etag, (uint16)*eref, tempanlist);
    if (ret < 0)
        HE_REPORT_RETURN("ANannlist:failed to any annotations", FAIL);

    /* move annotation handles into caller's alist */
    for (i = 0; i < nanns; i++)
        alist[i] = tempanlist[i];

    free(tempanlist);

    return ret;
} /* nafannlist() */

/*-----------------------------------------------------------------------------
 * Name:    afannlen
 * Purpose: Get length of annotation given handle
 * Inputs:  an_id:annotation handle
 * Returns: see ANannlen()
 * Users:   Fortran Users
 * Invokes: ANannlen()
 *---------------------------------------------------------------------------*/
intf
nafannlen(intf *an_id)
{
    return (intf)ANannlen((int32)*an_id);
}

/*-----------------------------------------------------------------------------
 * Name:    afwriteann
 * Purpose: Write annotation given handle
 * Inputs:  ann_id: annotation handle
 *          ann:   annotation to write out
 *          annlen:length of annotation
 * Returns: see ANwriteann()
 * Users:   Fortran Users
 * Invokes: ANwriteann()
 *---------------------------------------------------------------------------*/
intf
nafwriteann(intf *ann_id, _fcd ann, intf *annlen)
{
    char *iann = NULL;
    intf  status;

    /* Convert fortran string to C-String */
    iann = HDf2cstring(ann, (int)*annlen);
    if (!iann)
        return FAIL;

    status = ANwriteann((int32)*ann_id, (char *)_fcdtocp(ann), (int32)*annlen);

    free(iann); /* free allocated space by HDf2cstring */

    return status;
}

/*-----------------------------------------------------------------------------
 * Name:    afreadann
 * Purpose: Read annotation given handle
 * Inputs:  ann_id:  annotation handle
 *          ann:    annotation read
 *          maxlen: maximum space allocated for "ann"
 * Returns: see ANreadann() (SUCCEED (0) if successful, else FAIL (-1))
 * Users:   Fortran Users
 * Invokes: ANreadann()
 *---------------------------------------------------------------------------*/
intf
nafreadann(intf *ann_id, _fcd ann, intf *maxlen)
{
    char *iann = NULL;
    int   status;

    /* Allocate space for fortran string */
    if (*maxlen)
        iann = (char *)malloc((uint32)*maxlen + 1);

    if (!iann)
        HRETURN_ERROR(DFE_NOSPACE, FAIL);

    status = ANreadann((int32)*ann_id, iann, (int32)*maxlen);

    /* C-String to Fortran String */
    HDpackFstring(iann, _fcdtocp(ann), (int)*maxlen);

    free(iann);

    return status;
}

/*-----------------------------------------------------------------------------
 * Name:    afendaccess
 * Purpose: End access to annotation using handle
 * Inputs:  ann_id:annotation handle
 * Returns: see ANendaccess()
 * Users:   Fortran Users
 * Invokes: ANendaccess()
 *---------------------------------------------------------------------------*/
intf
nafendaccess(intf *ann_id)
{
    return (intf)ANendaccess((int32)*ann_id);
}

/*-----------------------------------------------------------------------------
 * Name:    afgettagref
 * Purpose:
 * Inputs:
 * Returns: see ANget_tagref()
 * Users:   Fortran Users
 * Invokes: ANget_tagref()
 *---------------------------------------------------------------------------*/
intf
nafgettagref(intf *an_id, intf *index, intf *type, intf *tag, intf *ref)
{
    intf   ret;
    uint16 otag, oref;

    ret = (intf)ANget_tagref((int32)*an_id, (int32)*index, (ann_type)*type, &otag, &oref);

    *tag = otag;
    *ref = oref;

    return ret;
}

/*-----------------------------------------------------------------------------
 * Name:    afidtagref
 * Purpose:
 * Inputs:
 * Returns: see ANid2tagref()
 * Users:   Fortran Users
 * Invokes: ANid2tagerf()
 *---------------------------------------------------------------------------*/
intf
nafidtagref(intf *ann_id, intf *tag, intf *ref)
{
    intf   ret;
    uint16 otag, oref;

    ret = (intf)ANid2tagref((int32)*ann_id, &otag, &oref);

    *tag = otag;
    *ref = oref;

    return ret;
}

/*-----------------------------------------------------------------------------
 * Name:    aftagrefid
 * Purpose:
 * Inputs:
 * Returns: see ANtagref2id()
 * Users:   Fortran Users
 * Invokes: ANtagref2id()
 *---------------------------------------------------------------------------*/
intf
naftagrefid(intf *an_id, intf *tag, intf *ref)
{
    return (intf)ANtagref2id((int32)*an_id, (uint16)*tag, (uint16)*ref);
}

/*-----------------------------------------------------------------------------
 * Name:    afatypetag
 * Purpose:
 * Inputs:
 * Returns: see ANatype2tag()
 * Users:   Fortran Users
 * Invokes: ANatype2tag()
 *---------------------------------------------------------------------------*/
intf
nafatypetag(intf *atype)
{
    return (intf)ANatype2tag((ann_type)*atype);
}

/*-----------------------------------------------------------------------------
 * Name:    aftagatype
 * Purpose:
 * Inputs:
 * Returns: see ANtag2atype()
 * Users:   Fortran Users
 * Invokes: ANtag2atype()
 *---------------------------------------------------------------------------*/
intf
naftagatype(intf *tag)
{
    return (intf)ANtag2atype((uint16)*tag);
}

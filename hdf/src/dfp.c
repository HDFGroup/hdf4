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
 * File:     dfp.c
 * Purpose:  read and write palettes
 * Invokes:  df.c
 * Contents:
 *  DFPgetpal    : retrieve next palette
 *  DFPputpal    : write palette to file
 *  DFPaddpal    : add palette to file
 *  DFPnpals     : number of palettes in HDF file
 *  DFPreadref   : get palette with this reference number next
 *  DFPwriteref  : put palette with this reference number next
 *  DFPrestart   : forget info about last file accessed - restart from beginning
 *  DFPlastref   : return reference number of last element read or written
 *  DFPIopen     : open/reopen file
 *---------------------------------------------------------------------------*/

#include "hdf_priv.h"

/* remember that '0' is invalid ref number */
static uint16 Readref  = 0;
static uint16 Writeref = 0;
static uint16 Refset   = 0; /* Ref of palette to get next */
static uint16 Lastref  = 0; /* Last ref read/written */

static char Lastfile[DF_MAXFNLEN + 1] = ""; /* last file opened */

static int32 DFPIopen(const char *filename, int acc_mode);

/*--------------------------------------------------------------------------
 NAME
    DFPgetpal -- get next palette from file
 USAGE
    int DFPgetpal(filename,palette)
        char *filename;         IN: name of HDF file
        void * palette;          OUT: ptr to the buffer to store the palette in
 RETURNS
    SUCCEED on success, FAIL on failure.
 DESCRIPTION
    Gets the next palette from the file specified.
 GLOBAL VARIABLES
    Lastref, Refset
--------------------------------------------------------------------------*/
int
DFPgetpal(const char *filename, void *palette)
{
    int32 file_id;
    int32 aid;
    int32 length;
    int   ret_value = SUCCEED;

    HEclear();

    if (!palette)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    if ((file_id = DFPIopen(filename, DFACC_READ)) == FAIL)
        HGOTO_ERROR(DFE_BADOPEN, FAIL);

    if (Refset) {
        aid = Hstartread(file_id, DFTAG_IP8, Refset);
        if (aid == FAIL)
            aid = Hstartread(file_id, DFTAG_LUT, Refset);
    }
    else if (Readref) {
        aid = Hstartread(file_id, DFTAG_IP8, Readref);
        if (aid == FAIL)
            aid = Hstartread(file_id, DFTAG_LUT, Readref);
        if (aid != FAIL && (Hnextread(aid, DFTAG_IP8, DFREF_WILDCARD, DF_CURRENT) == FAIL)) {
            if (Hnextread(aid, DFTAG_LUT, DFREF_WILDCARD, DF_CURRENT) == FAIL) {
                Hendaccess(aid);
                aid = FAIL;
            }
        }
    }
    else {
        aid = Hstartread(file_id, DFTAG_IP8, DFREF_WILDCARD);
        if (aid == FAIL)
            aid = Hstartread(file_id, DFTAG_LUT, DFREF_WILDCARD);
    }

    Refset = 0;
    /* on error, close file and return -1 */
    if (aid == FAIL) {
        ret_value = (HDerr(file_id));
        goto done;
    }

    if (Hinquire(aid, (int32 *)NULL, (uint16 *)NULL, &Readref, &length, (int32 *)NULL, (int32 *)NULL,
                 (int16 *)NULL, (int16 *)NULL) == FAIL) {
        Hendaccess(aid);
        ret_value = HDerr(file_id);
        goto done;
    }

    /* read palette */
    if (Hread(aid, length, (uint8 *)palette) == FAIL) {
        Hendaccess(aid);
        ret_value = (HDerr(file_id));
        goto done;
    }

    Hendaccess(aid);

    Lastref = Readref;

    ret_value = Hclose(file_id);

done:
    return ret_value;
} /* end DFPgetpal() */

/*--------------------------------------------------------------------------
 NAME
    DFPputpal -- Write palette to file
 USAGE
    int DFPputpal(filename,palette,overwrite,filemode)
        char *filename;         IN: name of HDF file
        void * palette;          IN: ptr to the buffer retrieve the palette from
        int overwrite;         IN: whether to (1) overwrite last palette written,
                                    or (0) write it as a fresh palette
        char *filemode;         IN: if "a" append palette to file, "w" create
                                    new file
 RETURNS
    SUCCEED on success, FAIL on failure.
 DESCRIPTION
    Stores a palette in an HDF file, with options for creating new file or appending,
    and overwriting last palette written.
 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
    To overwrite, the filename must be the same as for the previous call.
--------------------------------------------------------------------------*/
int
DFPputpal(const char *filename, const void *palette, int overwrite, const char *filemode)
{
    int32 file_id;
    int   ret_value = SUCCEED;

    HEclear();

    if (!palette)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    if (overwrite && strcmp(filename, Lastfile))
        HGOTO_ERROR(DFE_BADCALL, FAIL);

    file_id = DFPIopen(filename, (*filemode == 'w') ? DFACC_CREATE : DFACC_WRITE);
    if (file_id == FAIL)
        HGOTO_ERROR(DFE_BADOPEN, FAIL);

    /* if we want to overwrite, Lastref is the ref to write.  If not, if
       Writeref is set, we use that ref.  If not we get a fresh ref. The
       ref to write is placed in Lastref */
    if (!overwrite)
        Lastref = (uint16)(Writeref ? Writeref : Htagnewref(file_id, DFTAG_IP8));
    if (Lastref == 0)
        HGOTO_ERROR(DFE_NOREF, FAIL);

    Writeref = 0; /* don't know ref to write after this */

    /* write out palette */
    if (Hputelement(file_id, DFTAG_IP8, Lastref, (const uint8 *)palette, (int32)768) < 0) {
        ret_value = (HDerr(file_id));
        goto done;
    }

    /* Check for the tag/ref before creating it willy-nilly */
    if (Hexist(file_id, DFTAG_LUT, Lastref) == FAIL)
        Hdupdd(file_id, DFTAG_LUT, Lastref, DFTAG_IP8, Lastref);

    ret_value = Hclose(file_id);

done:
    return ret_value;
} /* end DFPputpal() */

/*--------------------------------------------------------------------------
 NAME
    DFPaddpal -- Append palette to file
 USAGE
    int DFPaddpal(filename,palette)
        char *filename;         IN: name of HDF file
        void * palette;          IN: ptr to the buffer retrieve the palette from
 RETURNS
    SUCCEED on success, FAIL on failure.
 DESCRIPTION
    Appends a palette in an HDF file.
--------------------------------------------------------------------------*/
int
DFPaddpal(const char *filename, const void *palette)
{
    int ret_value;

    ret_value = (DFPputpal(filename, palette, 0, "a"));

    return ret_value;
} /* end DFPaddpal() */

/*--------------------------------------------------------------------------
 NAME
    DFPnpals -- determine # of palettes in a file
 USAGE
    int DFPnpals(filename)
        char *filename;         IN: name of HDF file
 RETURNS
    SUCCEED on success, FAIL on failure.
 DESCRIPTION
    Determines the number of unique palettes in a file.
--------------------------------------------------------------------------*/
int
DFPnpals(const char *filename)
{
    int32  file_id;
    int    curr_pal;           /* current palette count */
    int32  nip8, nlut;         /* number of IP8s & number of LUTs */
    int    npals;              /* total number of palettes */
    uint16 find_tag, find_ref; /* storage for tag/ref pairs found */
    int32  find_off, find_len; /* storage for offset/lengths of tag/refs found */
    int32 *pal_off;            /* storage for an array of palette offsets */
    int    i, j;               /* local counting variable */
    int    ret_value = SUCCEED;

    HEclear();

    /* should use reopen if same file as last time - more efficient */
    if ((file_id = DFPIopen(filename, DFACC_READ)) == FAIL)
        HGOTO_ERROR(DFE_BADOPEN, FAIL);

    /* count number of IPs */
    if ((nip8 = Hnumber(file_id, DFTAG_IP8)) == FAIL) {
        ret_value = (HDerr(file_id));
        goto done;
    }

    /* count number of LUTs */
    if ((nlut = Hnumber(file_id, DFTAG_LUT)) == FAIL) {
        ret_value = (HDerr(file_id));
        goto done;
    }
    npals = (int)(nip8 + nlut);

    /* if no palettes just return zero and get out */
    if (npals == 0) {
        if (Hclose(file_id) == FAIL) {
            ret_value = FAIL;
            goto done;
        }

        ret_value = npals;
        goto done;
    }

    /* Get space to store the palette offsets */
    if ((pal_off = (int32 *)malloc((size_t)npals * sizeof(int32))) == NULL)
        HGOTO_ERROR(DFE_NOSPACE, FAIL);

    /* go through the IP8s */
    curr_pal = 0;
    find_tag = find_ref = 0;
    while (Hfind(file_id, DFTAG_IP8, DFREF_WILDCARD, &find_tag, &find_ref, &find_off, &find_len,
                 DF_FORWARD) == SUCCEED) {
        pal_off[curr_pal] = find_off; /* store offset */
        curr_pal++;
    }

    /* go through the LUTs */
    find_tag = find_ref = 0;
    while (Hfind(file_id, DFTAG_LUT, DFREF_WILDCARD, &find_tag, &find_ref, &find_off, &find_len,
                 DF_FORWARD) == SUCCEED) {
        pal_off[curr_pal] = find_off; /* store offset */
        curr_pal++;
    }

    npals = curr_pal;                /* reset the number of palettes we really have */
    for (i = 1; i < curr_pal; i++) { /* go through the palettes looking for duplicates */
        if (pal_off[i] != (-1))
            for (j = 0; j < i; j++) {
                if (pal_off[i] == pal_off[j]) {
                    npals--;           /* if duplicate found, decrement the number of palettes */
                    pal_off[j] = (-1); /* mark as used, so we don't count it too... */
                }
            }
    }

    /* free offsets */
    free(pal_off);

    if (Hclose(file_id) == FAIL)
        HGOTO_ERROR(DFE_CANTCLOSE, FAIL);

    ret_value = npals;

done:
    return ret_value;
} /* end DFPnpals() */

/*--------------------------------------------------------------------------
 NAME
    DFPreadref -- set ref # of palette to read next
 USAGE
    int DFPreadref(filename,ref)
        char *filename;         IN: name of HDF file
        uint16 ref;             IN: ref # of palette to read next
 RETURNS
    SUCCEED on success, FAIL on failure.
 DESCRIPTION
    Sets the ref # of the next palette to read from a file
 GLOBAL VARIABLES
    Refset
--------------------------------------------------------------------------*/
int
DFPreadref(const char *filename, uint16 ref)
{
    int32 file_id;
    int32 aid;
    int   ret_value = SUCCEED;

    HEclear();

    if ((file_id = DFPIopen(filename, DFACC_READ)) == FAIL)
        HGOTO_ERROR(DFE_BADOPEN, FAIL);

    aid = Hstartread(file_id, DFTAG_IP8, ref);
    if (aid == FAIL) {
        aid = Hstartread(file_id, DFTAG_LUT, ref);
        if (aid == FAIL) {
            ret_value = (HDerr(file_id));
            goto done;
        }
    }

    Hendaccess(aid);
    Refset = ref;

    ret_value = (Hclose(file_id));

done:
    return ret_value;
} /* end DFPreadref() */

/*--------------------------------------------------------------------------
 NAME
    DFPwriteref -- set ref # of palette to write next
 USAGE
    int DFPwriteref(filename,ref)
        char *filename;         IN: name of HDF file
        uint16 ref;             IN: ref # of palette to write next
 RETURNS
    SUCCEED on success, FAIL on failure.
 DESCRIPTION
    Sets the ref # of the next palette to write to a file.  The filename is
    ignored -- this is probably a bug.  No matter what file the next palette
    is written to, it will have the reference number ref.
 GLOBAL VARIABLES
    Writeref
--------------------------------------------------------------------------*/
int
DFPwriteref(const char *filename, uint16 ref)
{
    int ret_value = SUCCEED;

    (void)filename;

    Writeref = ref;

    return ret_value;
} /* end DFPwriteref() */

/*--------------------------------------------------------------------------
 NAME
    DFPrestart -- restart reading/writing palettes from the start of a file
 USAGE
    int DFPrestart(void)
 RETURNS
    SUCCEED on success, FAIL on failure.
 DESCRIPTION
    Restart reading/writing palettes to a file.
 GLOBAL VARIABLES
    Lastfile
--------------------------------------------------------------------------*/
int
DFPrestart(void)
{
    int ret_value = SUCCEED;

    Lastfile[0] = '\0';

    return ret_value;
} /* end DFPrestart() */

/*--------------------------------------------------------------------------
 NAME
    DFPlastref -- returns last ref # read/written
 USAGE
    uint16 DFPlastref(void)
 RETURNS
    ref # on on success, 0 on failure.
 DESCRIPTION
    Return the last ref # read/written from a file.
 GLOBAL VARIABLES
    Lastref
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
uint16
DFPlastref(void)
{
    uint16 ret_value;

    ret_value = Lastref;

    return ret_value;
} /* end DFPlastref() */

/**************************************************************************/
/*----------------------- Internal routines ------------------------------*/
/**************************************************************************/

/*-----------------------------------------------------------------------------
 * Name:    DFPIopen
 * Purpose: open or reopen a file
 * Inputs:  filename: name of file to open
 *          acc_mode : access mode
 * Returns: file pointer on success, NULL on failure with DFerror set
 * Users:   HDF systems programmers, other DFP routines
 * Invokes: DFopen
 * Remarks: This is a hook for someday providing more efficient ways to
 *          reopen a file, to avoid re-reading all the headers
 *---------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
 NAME
    DFPIopen -- open/reopen file for palette interface
 USAGE
    int32 DFPIopen(filename,acc_mode)
        char *filename;         IN: name of HDF file
        int acc_mode;            IN: type of access to open file with
 RETURNS
    HDF file handle on success, FAIL on failure.
 DESCRIPTION
    Open/reopen a file for the DFP interface to work with.
 GLOBAL VARIABLES
    Refset, Readref, Lastfile
 COMMENTS, BUGS, ASSUMPTIONS
    This is a hook for someday providing more efficient ways to
    reopen a file, to avoid re-reading all the headers
--------------------------------------------------------------------------*/
static int32
DFPIopen(const char *filename, int acc_mode)
{
    int32 file_id;
    int32 ret_value = SUCCEED;

    /* use reopen if same file as last time - more efficient */
    if (strncmp(Lastfile, filename, DF_MAXFNLEN) || (acc_mode == DFACC_CREATE)) {
        /* treat create as different file */
        if ((file_id = Hopen(filename, acc_mode, 0)) == FAIL)
            HGOTO_ERROR(DFE_BADOPEN, FAIL);
        Refset  = 0; /* no ref to get set for this file */
        Readref = 0;
    }
    else if ((file_id = Hopen(filename, acc_mode, 0)) == FAIL)
        HGOTO_ERROR(DFE_BADOPEN, FAIL);

    /* remember filename, so reopen may be used next time if same file */
    strncpy(Lastfile, filename, DF_MAXFNLEN);
    Lastfile[DF_MAXFNLEN] = '\0';

    ret_value = file_id;

done:
    return ret_value;
} /* end DFPIopen() */

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

/*
 ** FILE
 **   dfstubs.c
 **   V3.X stubs for V4.0 "H-level" I/O routines.  First implemented: V3.2
 ** EXPORTED ROUTINES
 **   *DFopen -- open HDF file
 **   *DFclose -- close HDF file
 **   *DFdescriptors -- return a list of the data descriptors in the HDF file
 **   *DFnumber -- count the number of occurrences of a given tag in HDF file
 **   *DFsetfind -- set up a search
 **   *DFfind -- search for tag/ref combination
 **   DFaccess -- set up a read/write on a data element
 **   DFstart -- set up a read/write on a data element
 **   DFread -- read a portion of a data element
 **   DFseek -- seek a new position within a data element
 **   DFwrite -- write a portion of a data element
 **   DFupdate -- write out updated DDs to HDF file
 **   *DFstat -- provide status information about HDF file
 **   *DFgetelement -- read an entire data element
 **   *DFputelement -- write an entire data element
 **   *DFdup -- create an additional descriptor for a data element
 **   *DFdel -- delete a data element
 **   *DFnewref -- get an unused reference number
 **   *DFishdf -- is this an HDF file?
 **   *DFerrno -- return value of DFerror
 ** AUTHOR
 **   Doug Ilg
 */

#include "hdfi.h"
#include "df.h"

#define DFACC_APPEND  8
#define DFEL_ABSENT   0
#define DFEL_RESIDENT 1
#define DFSRCH_OLD    0
#define DFSRCH_NEW    1

/*
 *  Important Internal Variables
 */
static DF *DFlist = NULL; /* pointer to list of open DFs */

static int32  DFid        = 0;
static int32  DFaid       = 0;
static int    DFaccmode   = 0;
static int    DFelaccmode = 0;
static uint16 search_tag  = 0;
static uint16 search_ref  = 0;
static int    search_stat = DFSRCH_NEW;
static int32  search_aid  = 0;
static int    DFelstat    = DFEL_ABSENT;
static int32  DFelsize    = 0;
static int32  DFelseekpos = 0;
static uint16 acc_tag     = 0;
static uint16 acc_ref     = 0;
static char  *DFelement   = NULL;

int DFerror = 0; /* Error code for DF routines */

/* prototypes for internal routines */
static int DFIclearacc(void);
static int DFIcheck(DF *dfile);

/*
 ** NAME
 **   DFopen -- open HDF file
 ** USAGE
 **   DF *DFopen(name, acc_mode, ndds)
 **   char* name;             IN: name of file to open
 **   int acc_mode;           IN: DFACC_READ, DFACC_WRITE, DFACC_CREATE,
 **                               DFACC_ALL
 **   int ndds;               IN: number of DDs in a block
 ** RETURNS
 **   DF ptr to open file on success, NULL on failure with DFerror set
 ** DESCRIPTION
 **   Open an HDF file, if it exists.  If file does not exist and write
 **   access requested, create file.
 ** GLOBAL VARIABLES
 ** COMMENTS, BUGS, ASSUMPTIONS
 **   The pointer returned by DFopen is NOT a reference to a DF.  It is
 **   just a place keeper for the new type of file handle.  Any program that
 **   relies on the contents of a DF returned by DFopen must be re-written.
 ** EXAMPLES
 ** REVISION LOG
 */
DF *
DFopen(char *name, int acc_mode, int ndds)
{
    if (DFIcheck(DFlist) == 0) {
        DFerror = DFE_TOOMANY;
        return NULL;
    }
    else
        DFerror = DFE_NONE;

    DFaccmode = acc_mode | DFACC_READ;
    DFid      = Hopen(name, DFaccmode, (int16)ndds);

    if (DFid == -1) {
        DFerror = (int)HEvalue(1);
        return NULL;
    }
    else {
        /*
           DFlist = makedf(DFid);
         */
        DFlist = (DF *)&DFid;
        return DFlist;
    }
}

/*
 ** NAME
 **   DFclose -- close HDF file
 ** USAGE
 **   int DFclose(dfile)
 **   DF *dfile;              IN: pointer to an open DF file
 ** RETURNS
 **   0 on success, -1 on failure with DFerror set
 ** DESCRIPTION
 **   Write out updated DDs; close DF file.
 ** GLOBAL VARIABLES
 ** COMMENTS, BUGS, ASSUMPTIONS
 ** EXAMPLES
 ** REVISION LOG
 */
int
DFclose(DF *dfile)
{
    int ret;

    if (DFIcheck(dfile) != 0) {
        DFerror = DFE_NOTOPEN;
        return FAIL;
    }
    else
        DFerror = DFE_NONE;

    if (DFelstat == DFEL_RESIDENT) {
        Hputelement(DFid, acc_tag, acc_ref, (unsigned char *)DFelement, DFelsize);
        free(DFelement);
    }
    else
        Hendaccess(DFaid);

    if (search_stat == DFSRCH_OLD) {
        Hendaccess(search_aid);
        search_aid = 0;
    }

    ret = Hclose(DFid);
    if (ret == 0) {
        dfile     = 0;
        DFlist    = (DF *)NULL;
        DFid      = 0;
        DFaccmode = 0;
    }
    else {
        DFerror = (int)HEvalue(1);
    }

    return ret;
}

/*
 ** NAME
 **   DFdescriptors -- return a list of the data descriptors in the file
 ** USAGE
 **   int DFdescriptors(dfile, ptr, begin, num)
 **   DF *dfile;              IN: pointer to an open DF file
 **   DFdesc ptr[];           IN: pointer to space for the list of DDs
 **   int begin;              IN: starting DD number
 **   int num;                IN: number of entries
 ** RETURNS
 **   number of DDs returned in the list
 ** DESCRIPTION
 **   Fills in a list of all DDs in the file beginning with DD begin and
 **   including a maximum of num entries.  The number of DDs actually entered
 **   into the list is returned.
 ** GLOBAL VARIABLES
 ** COMMENTS, BUGS, ASSUMPTIONS
 ** EXAMPLES
 ** REVISION LOG
 */
int
DFdescriptors(DF *dfile, DFdesc ptr[], int begin, int num)
{
    int   i, ret;
    int32 aid;

    if (DFIcheck(dfile) != 0) {
        DFerror = DFE_NOTOPEN;
        return -1;
    }
    else
        DFerror = DFE_NONE;

    aid = Hstartread(DFid, DFTAG_WILDCARD, DFREF_WILDCARD);

    if (aid == FAIL) {
        DFerror = (int)HEvalue(1);
        return -1;
    }

    for (i = 2; i <= begin; i++) {
        ret = Hnextread(aid, DFTAG_WILDCARD, DFREF_WILDCARD, DF_CURRENT);
        if (ret == FAIL) {
            Hendaccess(aid);
            DFerror = (int)HEvalue(1);
            return -1;
        }
    }

    Hinquire(aid, NULL, &ptr[0].tag, &ptr[0].ref, &ptr[0].length, &ptr[0].offset, NULL, NULL, NULL);

    for (i = 1; i < num; i++) {
        ret = Hnextread(aid, DFTAG_WILDCARD, DFREF_WILDCARD, DF_CURRENT);
        if (ret == FAIL) {
            Hendaccess(aid);
            return i;
        }
        Hinquire(aid, NULL, &ptr[i].tag, &ptr[i].ref, &ptr[i].length, &ptr[i].offset, NULL, NULL, NULL);
    }
    Hendaccess(aid);

    return i;
}

/*
 ** NAME
 **   DFnumber -- return number of occurrences of given tag in the HDF file
 ** USAGE
 **   DFnumber(dfile, tag)
 **   DF *dfile;              IN: pointer to open DF file
 **   uint16 tag;             IN: tag to count occurrences of
 ** RETURNS
 **   Number of occurrences on success, -1 on failure with DFerror set.
 ** DESCRIPTION
 **   Returns the number of occurrences of the specified tag in the HDF file.
 **   If tag is DFTAG_WILDCARD, all tags are counted.
 ** GLOBAL VARIABLES
 ** COMMENTS, BUGS, ASSUMPTIONS
 ** EXAMPLES
 ** REVISION LOG
 */
int
DFnumber(DF *dfile, uint16 tag)
{
    int num;

    if (DFIcheck(dfile) != 0) {
        DFerror = DFE_NOTOPEN;
        return -1;
    }
    else
        DFerror = DFE_NONE;

    num = Hnumber(DFid, tag);
    return num;
}

/*
 ** NAME
 **   DFsetfind -- set up parameters for a wildcard find
 ** USAGE
 **   int DFsetfind(dfile, tag, ref)
 **   DF *dfile;              IN: pointer to open DF file
 **   uint16 tag;             IN: tag of element to search for (0 is wild)
 **   uint16 ref;             IN: ref of element to search for (0 is wild)
 ** RETURNS
 **   0 on success, -1 on failure
 ** DESCRIPTION
 **   Sets up parameters for a wildcard find on a tag/ref pair.
 ** GLOBAL VARIABLES
 ** COMMENTS, BUGS, ASSUMPTIONS
 ** EXAMPLES
 ** REVISION LOG
 */
int
DFsetfind(DF *dfile, uint16 tag, uint16 ref)
{
    if (DFIcheck(dfile) != 0) {
        DFerror = DFE_NOTOPEN;
        return -1;
    }
    else
        DFerror = DFE_NONE;

    search_tag = tag;
    search_ref = ref;

    search_stat = DFSRCH_NEW;

    return 0;
}

/*
 ** NAME
 **   DFfind -- perform wildcard searches
 ** USAGE
 **   int DFfind(dfile, ptr)
 **   DF *dfile;              IN: pointer to an open DF file
 **   DFdesc *ptr;            IN: pointer to put in DD when found
 ** RETURNS
 **   0 on success, -1 on failure
 ** DESCRIPTION
 **   If desired tag/ref is found, its DD is copied to *ptr.
 ** GLOBAL VARIABLES
 ** COMMENTS, BUGS, ASSUMPTIONS
 ** EXAMPLES
 ** REVISION LOG
 */
int
DFfind(DF *dfile, DFdesc *ptr)
{
    int ret;

    if (DFIcheck(dfile) != 0) {
        DFerror = DFE_NOTOPEN;
        return -1;
    }
    else
        DFerror = DFE_NONE;

    if (search_stat == DFSRCH_NEW) {
        search_aid  = Hstartread(DFid, search_tag, search_ref);
        search_stat = DFSRCH_OLD;
        ret         = 0;
    }
    else {
        ret = Hnextread(search_aid, search_tag, search_ref, DF_CURRENT);
    }

    if ((search_aid == FAIL) || (ret == FAIL)) {
        DFerror  = DFE_NOMATCH;
        ptr->tag = 0;
        ptr->ref = 0;
        return -1;
    }

    Hinquire(search_aid, NULL, &ptr->tag, &ptr->ref, &ptr->length, &ptr->offset, NULL, NULL, NULL);

    return 0;
}

/*
 ** NAME
 **   DFaccess -- set up a read/write on a data element
 ** USAGE
 **   int DFaccess(dfile, tag, ref, acc_mode)
 **   DF *dfile;              IN: pointer to open HDF file
 **   uint16 tag;             IN: tag of element
 **   uint16 ref;             IN: ref number of element
 **   char *acc_mode;         IN: "r", "w", or "a" (read, write, append)
 ** RETURNS
 **   0 on success, -1 on failure
 ** DESCRIPTION
 **   Set up read or write access on data element.
 ** GLOBAL VARIABLES
 ** COMMENTS, BUGS, ASSUMPTIONS
 **   This function needs to call DFupdate and Hendaccess if there is already
 **   an active access element with a different tag/ref.
 **   Also, set up globals "acc_tag" and "acc_ref" to keep tabs on the data
 **   being referenced, and "in_mem" to keep track of whether the data for
 **   an element to be appended to has been read into memory.
 ** EXAMPLES
 ** REVISION LOG
 */
int
DFaccess(DF *dfile, uint16 tag, uint16 ref, char *acc_mode)
{
    int accmode;
    /*
       DFdle *ptr;
       int dle_num, index, i;
     */

    if (DFIcheck(dfile) != 0) {
        DFerror = DFE_NOTOPEN;
        return -1;
    }
    else
        DFerror = DFE_NONE;

    switch (*acc_mode) {
        case 'r':
            accmode = DFACC_READ;
            break;
        case 'w':
            accmode = DFACC_WRITE;
            if (((DFaccmode & DFACC_WRITE) == 0) && ((DFaccmode & DFACC_CREATE) == 0)) {
                DFerror = DFE_BADACC;
                return -1;
            }
            break;
        case 'a':
            accmode = DFACC_APPEND;
            if (((DFaccmode & DFACC_WRITE) == 0) && ((DFaccmode & DFACC_CREATE) == 0)) {
                DFerror = DFE_BADACC;
                return -1;
            }
            break;
        default:
            DFerror = DFE_BADACC;
            return -1;
    }

    acc_tag     = tag;
    acc_ref     = ref;
    DFelaccmode = accmode;
    DFelstat    = DFEL_ABSENT;
    DFelseekpos = 0;
    DFelsize    = 0;

    switch (*acc_mode) {
        case 'r':
            DFelsize = Hlength(DFid, acc_tag, acc_ref);
            if (DFelsize <= 0) {
                DFIclearacc();
                DFerror = (int)HEvalue(1);
                return -1;
            }
            break;
            /* _maybe_ treat 'w' and 'a' in the same general 'a'-way */
        case 'w':
            DFelsize = Hlength(DFid, acc_tag, acc_ref);
            if (DFelsize == FAIL) {
                DFelsize = 0;
            }
            else
                DFelstat = DFEL_RESIDENT;
            break;
        case 'a':
            DFelsize = Hlength(DFid, acc_tag, acc_ref);
            if (DFelsize == FAIL) {
                DFIclearacc();
                DFerror = (int)HEvalue(1);
                return -1;
            }
            DFelseekpos = DFelsize;
            break;
    }

    return 0;
}

static int
DFIclearacc(void)
{
    Hendaccess(DFaid);
    DFaid       = 0;
    acc_tag     = 0;
    acc_ref     = 0;
    DFelsize    = 0;
    DFelseekpos = 0;
    DFelstat    = DFEL_ABSENT;
    DFelement   = NULL;

    return 0;
}

/*
 ** NAME
 **   DFstart -- set up a read/write on an access element
 ** USAGE
 **   int DFstart(dfile, tag, ref, acc_mode)
 **   DF *dfile;              IN: pointer to open  DF file
 **   uint16 tag;             IN: tag of element
 **   uint16 ref;             IN: ref number of element
 **   char *acc_mode;         IN: "r", "w", ro "a" (read, write, append)
 ** RETURNS
 **   0 on success, -1 on failure
 ** DESCRIPTION
 **   Set up a read or write access on data element.
 ** GLOBAL VARIABLES
 ** COMMENTS, BUGS, ASSUMPTIONS
 ** EXAMPLES
 ** REVISION LOG
 */
int
DFstart(DF *dfile, uint16 tag, uint16 ref, char *acc_mode)
{
    return DFaccess(dfile, tag, ref, acc_mode);
}

/*
 ** NAME
 **   DFread -- read a portion of a data element
 ** USAGE
 **   int32 DFread(dfile, ptr, len)
 **   DF *dfile;              IN: pointer to open DF file
 **   char *ptr;              IN: pointer to space to read data into
 **   int32 len;              IN: number of bytes to read
 ** RETURNS
 **   number of bytes read on success, -1 on failure
 ** DESCRIPTION
 **   Read bytes from a DF file (part of element specified by DFaccess)
 ** GLOBAL VARIABLES
 ** COMMENTS, BUGS, ASSUMPTIONS
 **   Space for data is assumed to be pre-allocated.
 ** EXAMPLES
 ** REVISION LOG
 */
int32
DFread(DF *dfile, char *ptr, int32 len)
{
    int32 ret;

    if (DFIcheck(dfile) != 0) {
        DFerror = DFE_NOTOPEN;
        return -1;
    }
    else
        DFerror = DFE_NONE;

    DFaid = Hstartread(DFid, acc_tag, acc_ref);
    ret   = Hseek(DFaid, DFelseekpos, 0);
    if (ret == FAIL) {
        Hendaccess(DFaid);
        DFerror = (int)HEvalue(1);
        return -1;
    }

    ret = Hread(DFaid, len, (unsigned char *)ptr);
    Hendaccess(DFaid);

    if (ret == FAIL) {
        DFerror = (int)HEvalue(1);
        return -1;
    }
    else {
        DFelseekpos += ret;
        return ret;
    }
}

/*
 ** NAME
 **   DFseek -- seek a new position within a data element
 ** USAGE
 **   int32 DFseek(dfile, offset)
 **   DF *dfile;              IN: pointer to open DF file
 **   int32 offset;           IN: offset from beginning of element
 ** RETURNS
 **   offset of actual position seek'ed to from beginning of element
 ** DESCRIPTION
 **   Seek position within element specified by DFaccess.
 ** GLOBAL VARIABLES
 ** COMMENTS, BUGS, ASSUMPTIONS
 ** EXAMPLES
 ** REVISION LOG
 */
int32
DFseek(DF *dfile, int32 offset)
{
    int ret;

    if (DFIcheck(dfile) != 0) {
        DFerror = DFE_NOTOPEN;
        return -1;
    }
    else
        DFerror = DFE_NONE;

    /* assuming no blank space can be forced by seeking past end of element
       and writing more data */
    if (offset > DFelsize) {
        DFerror = DFE_BADSEEK;
        return -1;
    }
    else {
        ret = Hseek(DFaid, offset, DF_START);
        if (ret == FAIL) {
            DFerror = (int)HEvalue(1);
            return -1;
        }
        DFelseekpos = offset;
    }

    return 0;
}

/*
 ** NAME
 **   DFwrite -- write a portion of a data element
 ** USAGE
 **   int32 DFwrite(dfile, ptr, len)
 **   DF *dfile;              IN: pointer to open DF file
 **   char *ptr;              IN: pointer to data to be written
 **   int32 len;              IN: number of bytes to be written
 ** RETURNS
 **   number of bytes written on success, -1 on failure
 ** DESCRIPTION
 **   Write bytes to DF file (part of element specified by DFaccess)
 ** GLOBAL VARIABLES
 ** COMMENTS, BUGS, ASSUMPTIONS
 **   This function should check the access mode in DFaccmode.  On write
 **   access, if(!in_mem) Hstartwrite, Hwrite, and set in_mem, otherwise just
 **   Hwrite.  On append access, if(!in_mem) Hstartread, Hinquire(oldsize),
 **   malloc(oldsize+writesize), Hread to malloc'd area, copy write request
 **   to end of malloc'd area, set in_mem, otherwise, realloc(area+writesize)
 **   copy write request to end of area.
 ** EXAMPLES
 ** REVISION LOG
 */
int32
DFwrite(DF *dfile, char *ptr, int32 len)
{
    int32 size, ret, newlen;

    if (DFIcheck(dfile) != 0) {
        DFerror = DFE_NOTOPEN;
        return -1;
    }
    if ((DFelaccmode != DFACC_WRITE) && (DFelaccmode != DFACC_APPEND)) {
        DFerror = DFE_BADACC;
        return -1;
    }
    else
        DFerror = DFE_NONE;

    size = DFelseekpos + len;
    if (DFelaccmode == DFACC_WRITE) {
        if (DFelstat == DFEL_ABSENT) {
            Hendaccess(DFaid);
            DFaid = Hstartwrite(DFid, acc_tag, acc_ref, len);
            Hseek(DFaid, DFelseekpos, DF_START);
            ret = Hwrite(DFaid, len, (unsigned char *)ptr);
        }
        else {
            if (size <= DFelsize) {
                Hendaccess(DFaid);
                DFaid = Hstartwrite(DFid, acc_tag, acc_ref, len);
                Hseek(DFaid, DFelseekpos, DF_START);
                ret = Hwrite(DFaid, len, (unsigned char *)ptr);
            }
            else {
                Hendaccess(DFaid);
                DFerror = DFE_NOTENOUGH;
                return -1;
            }
        }
    }
    else {
        newlen = size - Hlength(DFid, acc_tag, acc_ref);
        Hendaccess(DFaid);
        DFaid = HLcreate(DFid, acc_tag, acc_ref, newlen, (int32)4);
        Hseek(DFaid, DFelseekpos, DF_START);
        ret = Hwrite(DFaid, len, (unsigned char *)ptr);
    }

    Hendaccess(DFaid);
    DFelseekpos += len;
    DFelsize = size;
    DFelstat = DFEL_RESIDENT;

    return ret;
}

/*
 ** NAME
 **   DFupdate -- write out updated DDs to HDF file
 ** USAGE
 **   int DFupdate(dfile)
 **   DF *dfile;              IN: pointer to open DF file
 ** RETURNS
 **   0 on success, -1 on failure with DFerror set.
 ** DESCRIPTION
 **   This function only checks for valid input and returns.  It is included
 **   solely for compatibility with older programs.
 ** GLOBAL VARIABLES
 ** COMMENTS, BUGS, ASSUMPTIONS
 **   This function does nothing but check for valid input.
 **   However, this function should check to see if an appended-to data
 **   element is in memory and, if it is, write it out.
 ** EXAMPLES
 ** REVISION LOG
 */
int
DFupdate(DF *dfile)
{
    if (DFIcheck(dfile) != 0) {
        DFerror = DFE_NOTOPEN;
        return -1;
    }
    else
        DFerror = DFE_NONE;

    return 0;
}

/*
 ** NAME
 **   DFstat -- provide status information about HDF file
 ** USAGE
 **   int DFstat(dfile, dfinfo)
 **   DF *dfile;              IN: pointer to open DF file
 **   struct DFdata *dfinfo;  IN: pointer to space for info
 ** RETURNS
 **   0 on success, -1 on failure
 ** DESCRIPTION
 **   Fill dfinfo with status information about the HDF file.
 ** GLOBAL VARIABLES
 ** COMMENTS, BUGS, ASSUMPTIONS
 **   Currently, only the HDF version number is returned in dfinfo.
 **   Actually, nothing happens here now.
 ** EXAMPLES
 ** REVISION LOG
 */
int
DFstat(DF *dfile, DFdata *dfinfo)
{
    (void)dfinfo;

    if (DFIcheck(dfile) != 0) {
        DFerror = DFE_NOTOPEN;
        return -1;
    }
    else
        DFerror = DFE_NONE;

    return 0;
}

/*
 ** NAME
 **   DFgetelement -- read an entire data element
 ** USAGE
 **  int32 DFgetelement(dfile, tag, ref, ptr)
 **   DF *dfile;              IN: pointer to open DF file
 **   uint16 tag;             IN: tag of element
 **   uint16 ref;             IN: ref number of element
 **   char *ptr;              IN: pointer to space for data element
 ** RETURNS
 **   number of bytes read on success, -1 on failure
 ** DESCRIPTION
 **   Reads in a data element from an HDF file.
 ** GLOBAL VARIABLES
 ** COMMENTS, BUGS, ASSUMPTIONS
 **   Currently, this function returns 0 on success, not #bytes read.
 ** EXAMPLES
 ** REVISION LOG
 */
int32
DFgetelement(DF *dfile, uint16 tag, uint16 ref, char *ptr)
{
    if (DFIcheck(dfile) != 0) {
        DFerror = DFE_NOTOPEN;
        return -1;
    }
    else
        DFerror = DFE_NONE;

    if (Hgetelement(DFid, tag, ref, (unsigned char *)ptr) == -1) {
        DFerror = (int)HEvalue(1);
        return -1;
    }
    else
        return Hlength(DFid, tag, ref);
}

/*
 ** NAME
 **   DFputelement -- write an entire data element
 ** USAGE
 **   int DFputelement(dfile, tag, ref, ptr, len)
 **   DF *dfile;              IN: pointer to open DF file
 **   uint16 tag;             IN: tag of data element
 **   uint16 ref;             IN: ref number of data element
 **   char *ptr;              IN: pointer to data element
 **   int32 len;              IN: length of data element
 ** RETURNS
 **   Number of bytes written on success, -1 on failure
 ** DESCRIPTION
 **   Write an entire data element to HDF file.
 ** GLOBAL VARIABLES
 ** COMMENTS, BUGS, ASSUMPTIONS
 ** EXAMPLES
 ** REVISION LOG
 */
int32
DFputelement(DF *dfile, uint16 tag, uint16 ref, char *ptr, int32 len)
{
    if (DFIcheck(dfile) != 0) {
        DFerror = DFE_NOTOPEN;
        return -1;
    }
    else
        DFerror = DFE_NONE;

    if (Hputelement(DFid, tag, ref, (unsigned char *)ptr, len) == FAIL) {
        DFerror = (int)HEvalue(1);
        return -1;
    }
    else
        return Hlength(DFid, tag, ref);
}

/*
 ** NAME
 **   DFdup -- create an additional descriptor for a data element
 ** USAGE
 **   int DFdup(dfile, itag, iref, otag, oref)
 **   DF *dfile;              IN: pointer to open DF file
 **   uint16 itag;            IN: new tag of data element
 **   uint16 iref;            IN: new ref number of data element
 **   uint16 otag;            IN: current tag of data element
 **   uint16 oref;            IN: current ref number of data element
 ** RETURNS
 **   0 on success, -1 on failure
 ** DESCRIPTION
 **   Add a new tag/ref for existing data.
 ** GLOBAL VARIABLES
 ** COMMENTS, BUGS, ASSUMPTIONS
 ** EXAMPLES
 ** REVISION LOG
 */
int
DFdup(DF *dfile, uint16 itag, uint16 iref, uint16 otag, uint16 oref)
{
    if (DFIcheck(dfile) != 0) {
        DFerror = DFE_NOTOPEN;
        return -1;
    }
    else
        DFerror = DFE_NONE;

    if (Hdupdd(DFid, itag, iref, otag, oref) != SUCCEED) {
        DFerror = (int)HEvalue(1);
        return -1;
    }
    else
        return 0;
}

/*
 ** NAME
 **   DFdel -- delete a data element
 ** USAGE
 **   int DFdel(dfile, tag, ref)
 **   DF *dfile;              IN: pointer to open DF file
 **   uint16 tag;             IN: tag of element
 **   uint16 ref;             IN: ref number of element
 ** RETURNS
 **   0 on success, -1 on failure
 ** DESCRIPTION
 **   Delete a data element from HDF file.
 ** GLOBAL VARIABLES
 ** COMMENTS, BUGS, ASSUMPTIONS
 **   The data element is not actually deleted; it simply loses its DD.
 ** EXAMPLES
 ** REVISION LOG
 */
int
DFdel(DF *dfile, uint16 tag, uint16 ref)
{
    if (DFIcheck(dfile) != 0) {
        DFerror = DFE_NOTOPEN;
        return -1;
    }
    else
        DFerror = DFE_NONE;

    if (Hdeldd(DFid, tag, ref) != 0) {
        DFerror = (int)HEvalue(1);
        return -1;
    }
    else
        return 0;
}

/*
 ** NAME
 **   DFnewref -- get an unused reference number
 ** USAGE
 **   uint16 DFnewref(dfile)
 **   DF *dfile;              IN: pointer to open DF file
 ** RETURNS
 **   unused reference number, or 0 if no reference numbers are free
 ** DESCRIPTION
 **   Get an unused reference number.
 ** GLOBAL VARIABLES
 ** COMMENTS, BUGS, ASSUMPTIONS
 ** EXAMPLES
 ** REVISION LOG
 */
uint16
DFnewref(DF *dfile)
{
    uint16 ret;

    if (DFIcheck(dfile) != 0) {
        DFerror = DFE_NOTOPEN;
        return 0;
    }
    else
        DFerror = DFE_NONE;

    ret = Hnewref(DFid);
    if (ret == 0xffff) {
        DFerror = (int)HEvalue(1);
        return 0;
    }

    return ret;
}

/*
 ** NAME
 **   DFishdf -- is this an HDF file?
 ** USAGE
 **   int DFishdf(filename)
 **   char *filename;         IN: name of file to check
 ** RETURNS
 **   0 if it is an HDF file, -1 if it is not.
 ** DESCRIPTION
 **   Determine whether file is an HDF file.
 ** GLOBAL VARIABLES
 ** COMMENTS, BUGS, ASSUMPTIONS
 ** EXAMPLES
 ** REVISION LOG
 */
int
DFishdf(char *filename)
{
    int32 dummy;

    DFerror = DFE_NONE;

    dummy = Hopen(filename, DFACC_READ, 0);
    if (dummy == -1) {
        DFerror = (int)HEvalue(1);
        return -1;
    }
    else {
        Hclose(dummy);
        return 0;
    }
}

/*
 ** NAME
 **   DFerrno -- return value of DFerror
 ** USAGE
 **   int DFerrno()
 ** RETURNS
 **   Value of DFerror.
 ** DESCRIPTION
 **   Return value of DFerror.
 ** GLOBAL VARIABLES
 ** COMMENTS, BUGS, ASSUMPTIONS
 ** EXAMPLES
 ** REVISION LOG
 */
int
DFerrno(void)
{
    return DFerror;
}

/*-----------------------------------------------------------------------------
 * Name:    DFIcheck
 * Purpose: check if dfile argument represents a valid DF file
 * Inputs:  dfile: pointer to open DF file
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF systems programmers, several routines in this file
 *---------------------------------------------------------------------------*/

static int
DFIcheck(DF *dfile)
{
    DFerror = DFE_NONE;

    if ((dfile != (DF *)&DFid) || (DFid == 0)) {
        DFerror = DFE_DFNULL;
        return -1;
    }

    if ((DFaccmode & DFACC_ALL) != DFaccmode) {
        DFerror = DFE_BADACC;
        return -1;
    }
    else
        return 0;
}

/*-----------------------------------------------------------------------------
 * Name:    DFIerr
 * Purpose: Close a file and return on error. save DFerror
 * Inputs:  dfile: pointer to HDF file to close
 * Returns: -1
 * Users:   HDF systems programmers, for error handling
 * Invokes: DFclose
 * Remarks: Used to centralize some error handling
 *---------------------------------------------------------------------------*/

int
DFIerr(DF *dfile)
{
    int saveerror;

    saveerror = DFerror;
    if (dfile != NULL)
        (void)DFclose(dfile);
    DFerror = saveerror;
    return -1;
}

/*-----------------------------------------------------------------------------
 * The following functions are stubs for the old routines from "dfkit.c".
 *---------------------------------------------------------------------------*/

void *
DFIgetspace(uint32 qty)
{
    void *ret;

    ret     = (void *)malloc(qty);
    DFerror = (int)HEvalue(1);
    return ret;
}

void *
DFIfreespace(void *ptr)
{
    free(ptr);
    return NULL;
}

intn
DFIc2fstr(char *str, int len)
{
    return HDc2fstr(str, len);
}

char *
DFIf2cstring(_fcd fdesc, intn len)
{
    return HDf2cstring(fdesc, len);
}

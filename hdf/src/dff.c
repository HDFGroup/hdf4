#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.1  1992/08/25 21:40:44  koziol
Initial revision

*/
/*-----------------------------------------------------------------------------
 * File:    dfF.c
 * Purpose: C stubs for Fortran low level routines
 * Invokes: dfF.c
 * Contents: 
 *  dfiopen:   call DFopen to open HDF file
 *  dfclose:   call DFclose to close HDF file
 *  dfdesc:    call DFdescriptors to get contents of DDs
 *  dfdup:     call DFdup to create additional DD for item
 *  dfdel:     call DFdel to delete DD of item
 *  dfiaccess: call DFaccess to set up access to item
 *  dfstart:   call DFaccess to set up access to item
 *  dfread:    call DFread to read part of item
 *  dfseek:    call DFseek to move to offset within item
 *  dfwrite:   call DFwrite to write part of item
 *  dfupdate:  call DFupdate to write out changes
 *  dfget:     call DFgetelement to read item
 *  dfput:     call DFputelement to write item
 *  dfsfind:   call DFsetfind to set up search
 *  dffind:    call DFfind to find next matching item
 *  dferrno:   call DFerrno to return value of DFerror
 *  dfnewref:  call DFnewref to get unused ref no
 *  dfnumber:  call DFnumber to get number of occurrances of given tag
 *  dfstat:    call DFstat to get status info on file
 *  dfiishdf:  call DFishdf to get HDF string
 *---------------------------------------------------------------------------*/

#include "df.h"

#ifndef DF_FNAMES
#   define DF_FNAMES
#ifdef DF_CAPFNAMES
#   define ndfiaccess   FNAME(DFIACCESS)
#   define ndfiopen  FNAME(DFIOPEN)
#   define ndfclose  FNAME(DFCLOSE)
#   define ndfdesc   FNAME(DFDESC)
#   define ndfdup    FNAME(DFDUP)
#   define ndfdel    FNAME(DFDEL)
#   define ndfstart  FNAME(DFSTART)
#   define ndfread   FNAME(DFREAD)
#   define ndfseek   FNAME(DFSEEK)
#   define ndfwrite  FNAME(DFWRITE)
#   define ndfupdate FNAME(DFUPDATE)
#   define ndfget    FNAME(DFGET)
#   define ndfput    FNAME(DFPUT)
#   define ndfsfind  FNAME(DFSFIND)
#   define ndffind   FNAME(DFFIND)
#   define ndferrno  FNAME(DFERRNO)
#   define ndfnewref FNAME(DFNEWREF)
#   define ndfnumber FNAME(DFNUMBER)
#   define ndfstat   FNAME(DFSTAT)
#   define ndfiishdf FNAME(DFIISHDF)
#else   /* !DF_CAPFNAMES */
#   define ndfiaccess   FNAME(dfiaccess)
#   define ndfiopen  FNAME(dfiopen)
#   define ndfclose  FNAME(dfclose)
#   define ndfdesc   FNAME(dfdesc)
#   define ndfdup    FNAME(dfdup)
#   define ndfdel    FNAME(dfdel)
#   define ndfstart  FNAME(dfstart)
#   define ndfread   FNAME(dfread)
#   define ndfseek   FNAME(dfseek)
#   define ndfwrite  FNAME(dfwrite)
#   define ndfupdate FNAME(dfupdate)
#   define ndfget    FNAME(dfget)
#   define ndfput    FNAME(dfput)
#   define ndfsfind  FNAME(dfsfind)
#   define ndffind   FNAME(dffind)
#   define ndferrno  FNAME(dferrno)
#   define ndfnewref FNAME(dfnewref)
#   define ndfnumber FNAME(dfnumber)
#   define ndfstat   FNAME(dfstat)
#   define ndfiishdf FNAME(dfiishdf)
#endif /* DF_CAPFNAMES */
#endif /* DF_FNAMES */

/*-----------------------------------------------------------------------------
 * Name:    dfiopen
 * Purpose: call DFopen to open HDF file
 * Inputs:  name: name of file to open
 *      access: access mode - integer with value DFACC_READ etc. 
 *      defdds: default number of DDs per header block
 *      namelen: length of name
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFopen
 * Method:  Convert filename to C string, call DFopen
 *---------------------------------------------------------------------------*/

    FRETVAL(int32)
#ifdef PROTOTYPE
ndfiopen(_fcd name, int32 *access, int32 *defdds, intn *namelen)
#else
ndfiopen(name, access, defdds, namelen)
    _fcd name;
    intn *access, *defdds, *namelen;
#endif /* PROTOTYPE */
{
    char *fn;
    int32 ret;
    
    fn = DFIf2cstring(name, *namelen);
    ret = (int32) DFopen(fn, (intn)*access, (intn)*defdds);
    HDfreespace(fn);
    return(ret);
}


/*-----------------------------------------------------------------------------
 * Name:    dfclose
 * Purpose: Call DFclose to close HDF file
 * Inputs:  dfile: pointer to HDF file to close
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFclose
 *---------------------------------------------------------------------------*/

    FRETVAL(int)
#ifdef PROTOTYPE
ndfclose(int32 *dfile)
#else
ndfclose(dfile)
    int32 *dfile;
#endif /* PROTOTYPE */
{
    return(DFclose((DF *)*dfile));
}


/*-----------------------------------------------------------------------------
 * Name:    dfdesc
 * Purpose: Call DFdescriptors to obtain descriptors
 * Inputs:  dfile: pointer to HDF file
 *          ptr: pointer to array of size >= (4, num) to put descriptors in
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFdesc
 *---------------------------------------------------------------------------*/

    FRETVAL(int)
#ifdef PROTOTYPE
ndfdesc(int32 *dfile, int32 ptr[][4], int32 *begin, int32 *num)
#else
ndfdesc(dfile, ptr, begin, num)
    int32 *dfile, *begin, *num;
    int32 ptr[][4];
#endif /* PROTOTYPE */
{
    DFdesc *ptr1;
    int i, num_desc;

            /* allocate temporary space */
    ptr1 = (DFdesc *) HDgetspace((uint32)*num * sizeof(DFdesc));
    num_desc = DFdescriptors((DF*)*dfile, ptr1, (intn)*begin, (intn)*num);
    
    /* copy ptr1 array  ptr; note row/column inversion */
    for (i=0; i<num_desc; i++)
    {
        ptr[i][0] = (int32)ptr1[i].tag;
        ptr[i][1] = (int32)ptr1[i].ref;
        ptr[i][2] = ptr1[i].offset;
        ptr[i][3] = ptr1[i].length;
    }

    HDfreespace(ptr1);

    return num_desc;
}


/*-----------------------------------------------------------------------------
 * Name:    dfdup
 * Purpose: Call DFdup to create additional DD for item
 * Inputs:  dfile: pointer to HDF file
 *          tag, ref: attributes of new DD to add
 *          otag, oref: attributes of item to point to
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFdup
 *---------------------------------------------------------------------------*/

    FRETVAL(int)
#ifdef PROTOTYPE
ndfdup(int32 *dfile, int32 *tag, int32 *ref, int32 *otag, int32 *oref)
#else
ndfdup(dfile, tag, ref, otag, oref)
    int32 *dfile;
    int32 *tag, *ref, *oref, *otag;
#endif /* PROTOTYPE */
{

    return(DFdup((DF *)*dfile, (uint16)*tag, (uint16)*ref, (uint16)*otag,
    	   (uint16)*oref));
}


/*-----------------------------------------------------------------------------
 * Name:    dfdel
 * Purpose: Call DFdel to delete DD of item
 * Inputs:  dfile: pointer to HDF file
 *          tag, ref: attributes of DD to delete
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFdel
 *---------------------------------------------------------------------------*/

    FRETVAL(int)
#ifdef PROTOTYPE
ndfdel(int32 *dfile, int32 *tag, int32 *ref)
#else
ndfdel(dfile, tag, ref)
    int32 *dfile;
    int32 *tag, *ref;
#endif /* PROTOTYPE */
{
    return (DFdel((DF *)*dfile, (uint16)*tag, (uint16)*ref));
}


/*-----------------------------------------------------------------------------
 * Name:    dfiaccess
 * Purpose: Call DFaccess to set up access to item
 * Inputs:  dfile: pointer to HDF file
 *          tag, ref: attributes of item to access
 *          access: access mode
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFaccess
 *---------------------------------------------------------------------------*/

    FRETVAL(int32)
#ifdef PROTOTYPE
ndfiaccess(int32 *dfile, int32 *tag, int32 *ref, _fcd access, int32 *acclen)
#else
ndfiaccess(dfile, tag, ref, access, acclen)
    int32 *dfile;
    int32 *tag, *ref;
    _fcd access;
    int32 *acclen;
#endif /* PROTOTYPE */
{
    char *acc;
    int32 ret;

    acc = DFIf2cstring(access, *acclen);
    ret = (int32)DFaccess((DF *) *dfile, (uint16)*tag, (uint16)*ref, acc);
    HDfreespace(acc);
    return(ret);
}


#if 0
/*-----------------------------------------------------------------------------
 * Name:    dfstart
 * Purpose: Call DFaccess to set up access to item
 * Inputs:  dfile: pointer to HDF file
 *          tag, ref: attributes of item to access
 *          access: access mode
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFaccess
 *---------------------------------------------------------------------------*/

    FRETVAL(int)
#ifdef PROTOTYPE
ndfstart(int32 *dfile, int32 *tag, int32 *ref, char *access)
#else
ndfstart(dfile, tag, ref, access)
int32 *dfile;
int32 *tag, *ref;
char *access;
#endif /* PROTOTYPE */
{

    return(DFaccess((DF *)*dfile, (uint16)*tag, (uint16)*ref, access));
}
#endif /* 0 */

/*-----------------------------------------------------------------------------
 * Name:    dfread
 * Purpose: Call DFread to read part of item
 * Inputs:  dfile: pointer to HDF file
 *          ptr: pointer to space to read item into
 *          len: number of bytes to read
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFread
 *---------------------------------------------------------------------------*/

    FRETVAL(int32)
#ifdef PROTOTYPE
ndfread(int32 *dfile, _fcd ptr, int32 *len)
#else
ndfread(dfile, ptr, len)
    int32 *dfile, *len;
    _fcd ptr;
#endif /* PROTOTYPE */
{
    return (DFread((DF *) *dfile, (char *)_fcdtocp(ptr), *len));
}

/*-----------------------------------------------------------------------------
 * Name:    dfseek
 * Purpose: Call DFseek to move to offset within item
 * Inputs:  dfile: pointer to HDF file
 *	    offset: number of bytes from beginning of item to move to
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFseek
 *---------------------------------------------------------------------------*/

    FRETVAL(int32)
#ifdef PROTOTYPE
ndfseek(int32 *dfile, int32 *offset)
#else
ndfseek(dfile, offset)
    int32 *dfile, *offset;
#endif /* PROTOTYPE */
{

    return (DFseek((DF *)*dfile, *offset));
}


/*-----------------------------------------------------------------------------
 * Name:    dfwrite
 * Purpose: Call DFwrite to write part of item
 * Inputs:  dfile: pointer to HDF file
 *	    ptr: pointer to data to write
 *	    len: number of bytes to write
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFwrite
 *---------------------------------------------------------------------------*/

    FRETVAL(int32)
#ifdef PROTOTYPE
ndfwrite(int32 *dfile, _fcd ptr, int32 *len)
#else
ndfwrite(dfile, ptr, len)
    int32 *dfile, *len;
    _fcd ptr;
#endif /* PROTOTYPE */
{

    return (DFwrite((DF *)*dfile, (char *)_fcdtocp(ptr), *len));
}

/*-----------------------------------------------------------------------------
 * Name:    dfupdate
 * Purpose: Call DFupdate to write out changes
 * Inputs:  dfile: pointer to HDF file
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFupdate
 *---------------------------------------------------------------------------*/

    FRETVAL(int)
#ifdef PROTOTYPE
ndfupdate(int32 *dfile)
#else
ndfupdate(dfile)
    int32 *dfile;
#endif /* PROTOTYPE */
{

    return (DFupdate((DF *)*dfile));
}


/*-----------------------------------------------------------------------------
 * Name:    dfget
 * Purpose: Call DFget to read an element
 * Inputs:  dfile: pointer to HDF file
 *	    tag, ref: pointer to item to read
 *	    ptr: space to read item into
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFgetelement
 *---------------------------------------------------------------------------*/

    FRETVAL(int32)
#ifdef PROTOTYPE
ndfget(int32 *dfile, int32 *tag, int32 *ref, _fcd ptr)
#else
ndfget(dfile, tag, ref, ptr)
    int32 *dfile;
    int32 *tag, *ref;
    _fcd ptr;
#endif /* PROTOTYPE */
{
    return (DFgetelement((DF *)*dfile, (uint16)*tag, (uint16)*ref,
    	    (char *)_fcdtocp(ptr)));
}


/*-----------------------------------------------------------------------------
 * Name:    dfput
 * Purpose: Call DFput to write an element
 * Inputs:  dfile: pointer to HDF file
 *	    tag, ref: attributes of item to write
 *	    ptr: item to write
 *	    len: size of item
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFputelement
 *---------------------------------------------------------------------------*/

    FRETVAL(int32)
#ifdef PROTOTYPE
ndfput(int32 *dfile, int32 *tag, int32 *ref, _fcd ptr, int32 *len)
#else
ndfput(dfile, tag, ref, ptr, len)
    int32 *dfile;
    int32 *tag, *ref;
    int32 *len;
    _fcd ptr;
#endif /* PROTOTYPE */
{

    return (DFputelement((DF *)*dfile, (uint16)*tag, (uint16)*ref,
	    (char*)_fcdtocp(ptr), *len));
}

/*-----------------------------------------------------------------------------
 * Name:    dfsfind
 * Purpose: Call DFsetfind to set up search
 * Inputs:  dfile: pointer to HDF file
 *	    tag, ref: attributes of item to find
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFsetfind
 *---------------------------------------------------------------------------*/

    FRETVAL(int)
#ifdef PROTOTYPE
ndfsfind(int32 *dfile, int32 *tag, int32 *ref)
#else
ndfsfind(dfile, tag, ref)
    int32 *dfile;
    int32 *tag, *ref;
#endif /* PROTOTYPE */
{
    return (DFsetfind((DF *) *dfile, (uint16)*tag, (uint16)*ref));
}


/*-----------------------------------------------------------------------------
 * Name:    dffind
 * Purpose: Call DFfind to find next match
 * Inputs:  dfile: pointer to HDF file
 *	    itag, iref: attributes of item found
 *	    len: size of item
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFfind
 *---------------------------------------------------------------------------*/

    FRETVAL(int)
#ifdef PROTOTYPE
ndffind(int32 *dfile, int32 *itag, int32 *iref, int32 *len)
#else
ndffind(dfile, itag, iref, len)
    int32 *dfile;
    int32 *itag, *iref;
    int32 *len;
#endif /* PROTOTYPE */
{
    DFdesc *ptr1;
    int ret;

    ptr1 = HDgetspace((uint32)sizeof(DFdesc));
    ret = DFfind((DF *) *dfile, ptr1);

    *itag  = (int32)(ptr1->tag);
    *iref = (int32)(ptr1->ref);
    *len = ptr1->length;

    HDfreespace(ptr1);

    return(ret);
}


/*-----------------------------------------------------------------------------
 * Name:    dferrno
 * Purpose: Call DFerrno to get value of DFerror
 * Inputs:  none
 * Returns: value of DFerror
 * Users:   HDF Fortran programmers
 * Invokes: DFerrno
 *---------------------------------------------------------------------------*/

    FRETVAL(int)
#ifdef PROTOTYPE
ndferrno(void)
#else
ndferrno()
#endif /* PROTOTYPE */
{
    return(DFerrno());
}

/*-----------------------------------------------------------------------------
 * Name:    dfnewref
 * Purpose: Call DFnewref to get unused ref no
 * Inputs:  dfile: pointer to HDF file
 * Returns: int16: unused ref no
 * Users:   HDF Fortran programmers
 * Invokes: DFnewref
 *---------------------------------------------------------------------------*/

    FRETVAL(int32)
#ifdef PROTOTYPE
ndfnewref(int32 *dfile)
#else
ndfnewref(dfile)
int32 *dfile;
#endif /* PROTOTYPE */
{
    return(DFnewref((DF *) *dfile));
}


/*-----------------------------------------------------------------------------
 * Name:    dfnumber
 * Purpose: Call DFnumber to get unused ref no
 * Inputs:  dfile: pointer to HDF file
 *	    tag:   pointer to (int16)tag to count
 * Returns: int: number of occurances of given tag
 * Users:   HDF Fortran programmers
 * Invokes: DFnumber
 *---------------------------------------------------------------------------*/

    FRETVAL(int)
#ifdef PROTOTYPE
ndfnumber(int32 *dfile, int32 *tag)
#else
ndfnumber(dfile, tag)
int32 *dfile;
int32 *tag;
#endif /* PROTOTYPE */
{
    return(DFnumber((DF *) *dfile, (uint16)*tag));
}


/*-----------------------------------------------------------------------------
 * Name:    dfstat
 * Purpose: Call DFstat to get staus info on file
 * Inputs:  dfile:  pointer to HDF file
 *	    dfinfo: pointer to DFdata structure to fill in
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFstat
 *---------------------------------------------------------------------------*/

    FRETVAL(int)
#ifdef PROTOTYPE
ndfstat(int32 *dfile, DFdata *dfinfo)
#else
ndfstat(dfile, dfinfo)
int32 *dfile;
DFdata *dfinfo;
#endif /* PROTOTYPE */
{
    return(DFstat((DF *) *dfile, dfinfo));
}


/*-----------------------------------------------------------------------------
 * Name:    dfiishdf
 * Purpose: Call DFishdf to test file
 * Inputs:  name:    name of file to test
 *	    namelen: pointer to variable containing length of name string
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFishdf
 *---------------------------------------------------------------------------*/

    FRETVAL(int)
#ifdef PROTOTYPE
ndfiishdf(_fcd name, intn *namelen)
#else
ndfiishdf(name, namelen)
_fcd name;
intn *namelen;
#endif /* PROTOTYPE */
{
    char *fn;
    int ret;

    fn = DFIf2cstring(name, *namelen);
    ret = DFishdf(fn);
    HDfreespace(fn);
    return(ret);
}


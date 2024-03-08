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
 *  dfnumber:  call DFnumber to get number of occurrences of given tag
 *  dfstat:    call DFstat to get status info on file
 *  dfiishdf:  call DFishdf to get HDF string
 *---------------------------------------------------------------------------*/

#include "hdf_priv.h"
#include "df.h"
#include "hproto_fortran.h"

/*-----------------------------------------------------------------------------
 * Name:    dfiopen
 * Purpose: call DFopen to open HDF file
 * Inputs:  name: name of file to open
 *      acc_mode: access mode - integer with value DFACC_READ etc.
 *      defdds: default number of DDs per header block
 *      namelen: length of name
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFopen
 * Method:  Convert filename to C string, call DFopen
 * Note: DFopen actually return *DF.  In machines that a pointer
 *       is bigger than a Fortran INTEGER, this routine would fail.
 *       This is a design error and has no easy portable solution.
 *---------------------------------------------------------------------------*/

intf
ndfiopen(_fcd name, intf *acc_mode, intf *defdds, intf *namelen)
{
    char *fn;
    intf  ret;

    fn = DFIf2cstring(name, (int)*namelen);
    /* For compiler warning, see note above. */
    ret = (intf)DFopen(fn, (int)*acc_mode, (int)*defdds);
    free(fn);
    return ret;
}

/*-----------------------------------------------------------------------------
 * Name:    dfclose
 * Purpose: Call DFclose to close HDF file
 * Inputs:  dfile: pointer to HDF file to close
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFclose
 *---------------------------------------------------------------------------*/

intf
ndfclose(intf *dfile)
{
    return DFclose((DF *)*dfile);
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

intf
ndfdesc(intf *dfile, intf ptr[][4], intf *begin, intf *num)
{
    DFdesc *ptr1;
    int     i;
    intf    num_desc;

    /* allocate temporary space */
    if ((ptr1 = (DFdesc *)malloc((uint32)*num * sizeof(DFdesc))) == NULL)
        HRETURN_ERROR(DFE_NOSPACE, -1);
    ;
    num_desc = DFdescriptors((DF *)*dfile, ptr1, (int)*begin, (int)*num);

    /* copy ptr1 array  ptr; note row/column inversion */
    for (i = 0; i < num_desc; i++) {
        ptr[i][0] = (int32)ptr1[i].tag;
        ptr[i][1] = (int32)ptr1[i].ref;
        ptr[i][2] = ptr1[i].offset;
        ptr[i][3] = ptr1[i].length;
    }

    free(ptr1);

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

intf
ndfdup(intf *dfile, intf *tag, intf *ref, intf *otag, intf *oref)
{
    return DFdup((DF *)*dfile, (uint16)*tag, (uint16)*ref, (uint16)*otag, (uint16)*oref);
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

intf
ndfdel(intf *dfile, intf *tag, intf *ref)
{
    return DFdel((DF *)*dfile, (uint16)*tag, (uint16)*ref);
}

/*-----------------------------------------------------------------------------
 * Name:    dfiaccess
 * Purpose: Call DFaccess to set up access to item
 * Inputs:  dfile: pointer to HDF file
 *          tag, ref: attributes of item to access
 *          acc_mode: access mode
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFaccess
 *---------------------------------------------------------------------------*/

intf
ndfiaccess(intf *dfile, intf *tag, intf *ref, _fcd acc_mode, intf *acclen)
{
    char *acc;
    intf  ret;

    acc = DFIf2cstring(acc_mode, (int)*acclen);
    ret = (intf)DFaccess((DF *)*dfile, (uint16)*tag, (uint16)*ref, acc);
    free(acc);
    return ret;
}

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

intf
ndfread(intf *dfile, _fcd ptr, intf *len)
{
    return DFread((DF *)*dfile, (char *)_fcdtocp(ptr), *len);
}

/*-----------------------------------------------------------------------------
 * Name:    dfseek
 * Purpose: Call DFseek to move to offset within item
 * Inputs:  dfile: pointer to HDF file
 *      offset: number of bytes from beginning of item to move to
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFseek
 *---------------------------------------------------------------------------*/

intf
ndfseek(intf *dfile, intf *offset)
{
    return DFseek((DF *)*dfile, *offset);
}

/*-----------------------------------------------------------------------------
 * Name:    dfwrite
 * Purpose: Call DFwrite to write part of item
 * Inputs:  dfile: pointer to HDF file
 *      ptr: pointer to data to write
 *      len: number of bytes to write
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFwrite
 *---------------------------------------------------------------------------*/

intf
ndfwrite(intf *dfile, _fcd ptr, intf *len)
{
    return DFwrite((DF *)*dfile, (char *)_fcdtocp(ptr), *len);
}

/*-----------------------------------------------------------------------------
 * Name:    dfupdate
 * Purpose: Call DFupdate to write out changes
 * Inputs:  dfile: pointer to HDF file
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFupdate
 *---------------------------------------------------------------------------*/

intf
ndfupdate(intf *dfile)
{
    return DFupdate((DF *)*dfile);
}

/*-----------------------------------------------------------------------------
 * Name:    dfget
 * Purpose: Call DFget to read an element
 * Inputs:  dfile: pointer to HDF file
 *      tag, ref: pointer to item to read
 *      ptr: space to read item into
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFgetelement
 *---------------------------------------------------------------------------*/

intf
ndfget(intf *dfile, intf *tag, intf *ref, _fcd ptr)
{
    return DFgetelement((DF *)*dfile, (uint16)*tag, (uint16)*ref, (char *)_fcdtocp(ptr));
}

/*-----------------------------------------------------------------------------
 * Name:    dfput
 * Purpose: Call DFput to write an element
 * Inputs:  dfile: pointer to HDF file
 *      tag, ref: attributes of item to write
 *      ptr: item to write
 *      len: size of item
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFputelement
 *---------------------------------------------------------------------------*/

intf
ndfput(intf *dfile, intf *tag, intf *ref, _fcd ptr, intf *len)
{
    return DFputelement((DF *)*dfile, (uint16)*tag, (uint16)*ref, (char *)_fcdtocp(ptr), *len);
}

/*-----------------------------------------------------------------------------
 * Name:    dfsfind
 * Purpose: Call DFsetfind to set up search
 * Inputs:  dfile: pointer to HDF file
 *      tag, ref: attributes of item to find
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFsetfind
 *---------------------------------------------------------------------------*/

intf
ndfsfind(intf *dfile, intf *tag, intf *ref)
{
    return DFsetfind((DF *)*dfile, (uint16)*tag, (uint16)*ref);
}

/*-----------------------------------------------------------------------------
 * Name:    dffind
 * Purpose: Call DFfind to find next match
 * Inputs:  dfile: pointer to HDF file
 *      itag, iref: attributes of item found
 *      len: size of item
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFfind
 *---------------------------------------------------------------------------*/

intf
ndffind(intf *dfile, intf *itag, intf *iref, intf *len)
{
    DFdesc *ptr1;
    intf    ret;

    ptr1 = (DFdesc *)malloc((uint32)sizeof(DFdesc));
    if (ptr1 == NULL)
        HRETURN_ERROR(DFE_NOSPACE, -1);
    ret = DFfind((DF *)*dfile, ptr1);

    *itag = (int32)(ptr1->tag);
    *iref = (int32)(ptr1->ref);
    *len  = ptr1->length;

    free(ptr1);

    return ret;
}

/*-----------------------------------------------------------------------------
 * Name:    dferrno
 * Purpose: Call DFerrno to get value of DFerror
 * Inputs:  none
 * Returns: value of DFerror
 * Users:   HDF Fortran programmers
 * Invokes: DFerrno
 *---------------------------------------------------------------------------*/

intf
ndferrno(void)
{
    return DFerrno();
}

/*-----------------------------------------------------------------------------
 * Name:    dfnewref
 * Purpose: Call DFnewref to get unused ref no
 * Inputs:  dfile: pointer to HDF file
 * Returns: int16: unused ref no
 * Users:   HDF Fortran programmers
 * Invokes: DFnewref
 *---------------------------------------------------------------------------*/

intf
ndfnewref(intf *dfile)
{
    return (intf)DFnewref((DF *)*dfile);
}

/*-----------------------------------------------------------------------------
 * Name:    dfnumber
 * Purpose: Call DFnumber to get unused ref no
 * Inputs:  dfile: pointer to HDF file
 *      tag:   pointer to (int16)tag to count
 * Returns: int: number of occurrences of given tag
 * Users:   HDF Fortran programmers
 * Invokes: DFnumber
 *---------------------------------------------------------------------------*/

intf
ndfnumber(intf *dfile, intf *tag)
{
    return DFnumber((DF *)*dfile, (uint16)*tag);
}

/*-----------------------------------------------------------------------------
 * Name:    dfstat
 * Purpose: Call DFstat to get status info on file
 * Inputs:  dfile:  pointer to HDF file
 *      dfinfo: pointer to DFdata structure to fill in
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFstat
 *---------------------------------------------------------------------------*/

intf
ndfstat(intf *dfile, DFdata *dfinfo)
{
    return DFstat((DF *)*dfile, dfinfo);
}

/*-----------------------------------------------------------------------------
 * Name:    dfiishdf
 * Purpose: Call DFishdf to test file
 * Inputs:  name:    name of file to test
 *      namelen: pointer to variable containing length of name string
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFishdf
 *---------------------------------------------------------------------------*/

intf
ndfiishdf(_fcd name, intf *namelen)
{
    char *fn;
    intf  ret;

    fn  = DFIf2cstring(name, (int)*namelen);
    ret = DFishdf(fn);
    free(fn);
    return ret;
}

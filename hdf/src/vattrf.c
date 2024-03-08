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
 *
 * vattrf.c
 * Part of the HDF Vset attribute interface.
 *
 * C routines (short names) to be called from fortran
 *
 ******************************************************/

#include "hdf_priv.h"
#include "vg_priv.h"
#include "hproto_fortran.h"

/* ----------------- vsfcfdx ----------------------
 *  get field index
 *  VSfindex -- vsfcfdx -- vsffidx
 */

intf
nvsfcfdx(intf *vsid, _fcd fldnm, intf *findex, intf *fldnmlen)
{
    intf  ret;
    char *fld;

    fld = HDf2cstring(fldnm, (int)*fldnmlen);
    if (!fld)
        return FAIL;
    ret = (intf)VSfindex((int32)*vsid, fld, (int32 *)findex);
    free(fld);
    return ret;
}

/* -------------------------------------------------
 * vsfcsat -- set a numeric attr for a vdata or a field
 *            of a vdata
 * VSsetattr -- vsfcsat -- vsfsnat
 */

intf
nvsfcsat(intf *vsid, intf *findex, _fcd attrnm, intf *dtype, intf *count, intf *values, intf *attrnmlen)
{
    intf  ret;
    char *attrname;
    int32 cfindex;

    attrname = HDf2cstring(attrnm, (int)*attrnmlen);
    if (!attrname)
        return FAIL;
    cfindex = *findex;
    ret =
        (intf)VSsetattr((int32)*vsid, (int32)cfindex, attrname, (int32)*dtype, (int32)*count, (void *)values);
    free(attrname);
    return ret;
}

/* ----------------------------------------------------
 * vsfcsca -- set char attr for vdata or its field
 *
 * VSsetattr -- vsfcsca -- vsfscat
 */

intf
nvsfcsca(intf *vsid, intf *findex, _fcd attrnm, intf *dtype, intf *count, _fcd values, intf *attrnmlen)
{
    intf  ret;
    char *attrname;
    int32 cfindex;

    attrname = HDf2cstring(attrnm, (int)*attrnmlen);
    if (!attrname)
        return FAIL;
    cfindex = *findex;
    ret     = (intf)VSsetattr((int32)*vsid, (int32)cfindex, attrname, (int32)*dtype, (int32)*count,
                              (void *)_fcdtocp(values));
    free(attrname);
    return ret;
}

/* -------------------------------------------------------
 * vsfnats -- get total number of attributes of a vdata and
 *             its fields
 * VSnattrs -- vsfnats
 */

intf
nvsfnats(intf *vsid)
{
    intf ret;

    ret = (intf)VSnattrs((int32)*vsid);
    return ret;
}

/* -------------------------------------------------------
 * vsffnas -- get number of attributes of a vdata or of a field
 *
 * VSfnattrs -- vsffnas
 */

intf
nvsffnas(intf *vsid, intf *findex)
{
    intf  ret;
    int32 cfindex;

    cfindex = *findex;
    ret     = (intf)VSfnattrs((int32)*vsid, (int32)cfindex);
    return ret;
}

/* ---------------------------------------------------------
 * vsfcfda -- find attribute of a vdata of a field
 *               of the vdata
 *    VSfindattr -- vsfcfda -- vsffdat
 */

intf
nvsfcfda(intf *vsid, intf *findex, _fcd attrnm, intf *attrnmlen)
{
    intf  ret;
    char *attrname;
    int32 cfindex;

    attrname = HDf2cstring(attrnm, (int)*attrnmlen);
    if (!attrname)
        return FAIL;
    cfindex = *findex;

    ret = (intf)VSfindattr((int32)*vsid, (int32)cfindex, attrname);
    free(attrname);
    return ret;
}

/* ---------------------------------------------------------
 * vsfainf -- get attribute info
 * VSattrinfo -- vsfcain -- vsfainf
 */

intf
nvsfcain(intf *vsid, intf *findex, intf *aindex, _fcd attrname, intf *dtype, intf *count, intf *size,
         intf *attrnamelen)
{
    intf ret = FAIL;
    /* temporary variables */
    int32 cfindex;
    int32 tdtype, tcount, tsize;
    char *tattrname;

    cfindex = *findex;
    /* Allocate space for fortran strings */
    tattrname = (char *)malloc(*attrnamelen + 1);
    if (!tattrname)
        HRETURN_ERROR(DFE_NOSPACE, FAIL);

    ret = (intf)VSattrinfo((int32)*vsid, (int32)cfindex, (int32)*aindex, tattrname, &tdtype, &tcount, &tsize);
    if (ret != FAIL) {
        /* move values from temporary space to fortran variables */
        *dtype = tdtype;
        *count = tcount;
        *size  = tsize;
        /* convert C-string results back to Fortran strings */
        HDpackFstring(tattrname, _fcdtocp(attrname), (int)*attrnamelen);
    }
    free(tattrname);
    return ret;
}

/* ---------------------------------------------------------
 * vsfgnat -- get values of a numeric attribute
 * VSgetattr -- vsfgnat
 */

intf
nvsfgnat(intf *vsid, intf *findex, intf *aindex, intf *values)
{
    intf  ret;
    int32 cfindex;

    cfindex = *findex;
    ret     = (intf)VSgetattr((int32)*vsid, (int32)cfindex, (int32)*aindex, (void *)values);
    return ret;
}

/* --------------------------------------------------------
 * vsfgcat -- get values of a char type attribute
 * VSgetattr -- vsfgcat
 */

intf
nvsfgcat(intf *vsid, intf *findex, intf *aindex, _fcd values)
{
    intf  ret;
    int32 cfindex;

    cfindex = *findex;
    ret     = (intf)VSgetattr((int32)*vsid, cfindex, (int32)*aindex, (void *)_fcdtocp(values));
    return ret;
}

/* ---------------------------------------------------------
 * vsfisat -- test if a vdata is an attribute of other object
 * VSisattr -- vsfisat
 */

intf
nvsfisat(intf *vsid)
{
    intf ret;
    ret = (intf)VSisattr((int32)*vsid);
    return ret;
}

/* ---------------------------------------------------------
 * vfcsatt -- set a numeric attr for a vgroup
 * Vsetattr -- vfcsatt -- vfsnatt
 */

intf
nvfcsatt(intf *vgid, _fcd attrnm, intf *dtype, intf *count, intf *values, intf *attrnmlen)
{
    intf  ret;
    char *attrname;

    attrname = HDf2cstring(attrnm, (int)*attrnmlen);
    if (!attrname)
        return FAIL;
    ret = (intf)Vsetattr((int32)*vgid, attrname, (int32)*dtype, (int32)*count, (void *)values);
    free(attrname);
    return ret;
}

/* ----------------------------------------------------
 * vfcscat -- set char attr for vgroup
 *
 * Vsetattr -- vfcscat -- vfscatt
 */

intf
nvfcscat(intf *vgid, _fcd attrnm, intf *dtype, intf *count, _fcd values, intf *attrnmlen)
{
    intf  ret;
    char *attrname;

    attrname = HDf2cstring(attrnm, (int)*attrnmlen);
    if (!attrname)
        return FAIL;
    ret = (intf)Vsetattr((int32)*vgid, attrname, (int32)*dtype, (int32)*count, (void *)_fcdtocp(values));
    free(attrname);
    return ret;
}

/* -------------------------------------------------------
 * vfnatts -- get number of attributes of a vgroup
 * Vnattrs -- vfnatts
 */

intf
nvfnatts(intf *vgid)
{
    intf ret;

    ret = (intf)Vnattrs((int32)*vgid);
    return ret;
}

/* ---------------------------------------------------------
 * vfcfdat -- find attribute of a vgroup
 * Vfindattr -- vfcfdat -- vffdatt
 */

intf
nvfcfdat(intf *vgid, _fcd attrnm, intf *attrnmlen)
{
    intf  ret;
    char *attrname;

    attrname = HDf2cstring(attrnm, (int)*attrnmlen);
    if (!attrname)
        return FAIL;
    ret = (intf)Vfindattr((int32)*vgid, attrname);
    free(attrname);
    return ret;
}

/* ---------------------------------------------------------
 * vfainf -- get attribute info
 * Vattrinfo -- vfainfo
 */

intf
nvfainfo(intf *vgid, intf *aindex, _fcd attrname, intf *dtype, intf *count, intf *size)
{
    intf ret;
    ret = (intf)Vattrinfo((int32)*vgid, (int32)*aindex, _fcdtocp(attrname), (int32 *)dtype, (int32 *)count,
                          (int32 *)size);
    return ret;
}

/* ---------------------------------------------------------
 * vfgnatt -- get values of a numeric attribute
 * Vgetattr -- vfgnatt
 */

intf
nvfgnatt(intf *vgid, intf *aindex, intf *values)
{
    intf ret;
    ret = (intf)Vgetattr((int32)*vgid, *aindex, (void *)values);
    return ret;
}

/* --------------------------------------------------------
 * vfgcatt -- get values of a char type attribute
 * Vgetattr -- vfgcatt
 */

intf
nvfgcatt(intf *vgid, intf *aindex, _fcd values)
{
    intf ret;
    ret = (intf)Vgetattr((int32)*vgid, (int32)*aindex, (void *)_fcdtocp(values));
    return ret;
}

/* ---------------------------------------------------------
 * vfgver -- get version number of a vgroup
 * Vgetversion -- vfgver
 */

intf
nvfgver(intf *vgid)
{
    intf ret;
    ret = (intf)Vgetversion((int32)*vgid);
    return ret;
}

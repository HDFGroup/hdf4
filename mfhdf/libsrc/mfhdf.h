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

/* $Id$ */

#ifndef _MFSD_H_
#define _MFSD_H_

#ifndef HDF
#define HDF 1
#endif

#include "hdf.h"
#include "netcdf.h"

/* use this as marker for unlimited dimension */
#define SD_UNLIMITED NC_UNLIMITED

/* marker for ragged dimension */
#define SD_RAGGED    -1

#ifdef __cplusplus
extern "C" {
#endif

extern int32 SDstart
    PROTO((const char *name, int32 access));

extern intn SDend
    PROTO((int32 fid));

extern intn SDfileinfo
    PROTO((int32 fid, int32 *datasets, int32 *attrs));

extern int32 SDselect
    PROTO((int32 fid, int32 index));

extern intn SDgetinfo
    PROTO((int32 sdsid, char *name, int32 *rank, int32 *dimsizes, 
           int32 *nt, int32 *nattr));

#ifndef __CSTAR__
extern intn SDreaddata
    PROTO((int32 sdsid, int32 *start, int32 *stride, int32 *end, VOIDP data));
#endif

extern uint16 SDgerefnumber
    PROTO((int32 sdsid));

extern int32 SDnametoindex
    PROTO((int32 fid, char *name));

extern intn SDgetrange
    PROTO((int32 sdsid, VOIDP pmax, VOIDP pmin));

extern int32 SDcreate
    PROTO((int32 fid, char *name, int32 nt, int32 rank, int32 *dimsizes));

extern int32 SDgetdimid
    PROTO((int32 sdsid, intn number));

extern intn SDsetdimname
    PROTO((int32 id, char *name));

extern intn SDendaccess
    PROTO((int32 id));

extern intn SDsetrange
    PROTO((int32 sdsid, VOIDP pmax, VOIDP pmin));

extern intn SDsetattr
    PROTO((int32 id, char *name, int32 nt, int32 count, VOIDP data));

extern intn SDattrinfo
    PROTO((int32 id, int32 index, char *name, int32 *nt, int32 *count));

extern intn SDreadattr
    PROTO((int32 id, int32 index, VOIDP buf));

#ifndef __CSTAR__
extern intn SDwritedata
    PROTO((int32 sdsid, int32 *start, int32 *stride, int32 *end, VOIDP data));
#endif

extern intn SDsetdatastrs
    PROTO((int32 sdsid, char *l, char *u, char *f, char *c));

extern intn SDsetcal
    PROTO((int32 sdsid, float64 cal, float64 cale, float64 ioff,
               float64 ioffe, int32 nt));

extern intn SDsetfillvalue
    PROTO((int32 sdsid, VOIDP val));

extern intn SDgetfillvalue
    PROTO((int32 sdsid, VOIDP val));

extern intn SDgetdatastrs
    PROTO((int32 sdsid, char *l, char *u, char *f, char *c, intn len));

extern intn SDgetcal
    PROTO((int32 sdsid, float64 *cal, float64 *cale, float64 *ioff, 
               float64 *ioffe, int32 *nt));

extern intn SDsetdimstrs
    PROTO((int32 id, char *l, char *u, char *f));

extern intn SDsetdimscale
    PROTO((int32 id, int32 count, int32 nt, VOIDP data));

extern intn SDgetdimscale
    PROTO((int32 id, VOIDP data));

extern intn SDdiminfo
    PROTO((int32 id, char *name, int32 *size, int32 *nt, int32 *nattr));

extern intn SDgetdimstrs
    PROTO((int32 id, char *l, char *u, char *f, intn len));

extern intn SDsetexternalfile
    PROTO((int32 id, char *filename, int32 offset));

extern intn SDsetbigexternalfile
    PROTO((int32 id, char *filename, int32 offset));

extern int32 SDfindattr
    PROTO((int32 id, char *attrname));

extern int32 SDidtoref
    PROTO((int32 id));

extern int32 SDreftoindex
    PROTO((int32 fid, int32 ref));

extern int32 SDisrecord
    PROTO((int32 id));

extern intn SDiscoordvar
    PROTO((int32 id));

/* Define the FORTRAN names */

#ifndef MFSD_FNAMES
#   define  MFSD_FNAMES
#ifdef DF_CAPFNAMES
#   define nscstart    FNAME(SCSTART)
#   define nsfend      FNAME(SFEND)
#   define nsfendacc   FNAME(SFENDACC)
#   define nsffinfo    FNAME(SFFINFO)
#   define nsfselect   FNAME(SFSELECT)
#   define nscginfo    FNAME(SCGINFO)
#   define nscgainfo   FNAME(SCGAINFO)
#   define nscgdinfo   FNAME(SCGDINFO)
#   define nsfgcal     FNAME(SFGCAL)
#   define nsfscal     FNAME(SFSCAL)
#   define nsfgdscale  FNAME(SFGDSCALE)
#   define nsfsdscale  FNAME(SFSDSCALE)
#   define nsfgfill    FNAME(SFGFILL)
#   define nsfsfill    FNAME(SFSFILL)
#   define nsfgrange   FNAME(SFGRANGE)
#   define nsfsrange   FNAME(SFSRANGE)
#   define nscn2index  FNAME(SCN2INDEX)
#   define nsccreate   FNAME(SCCREATE)
#   define nscsdimstr  FNAME(SCSDIMSTR)
#   define nscsdimname FNAME(SCSDIMNAME)
#   define nscsdatstr  FNAME(SCSDATSTR)
#   define nsfdimid    FNAME(SFDIMID)
#   define nsfrattr    FNAME(SFRATTR)
#   define nsfrdata    FNAME(SFRDATA)
#   define nsfwdata    FNAME(SFWDATA)
#   define nscgdatstrs FNAME(SCGDATSTRS)
#   define nscgdimstrs FNAME(SCGDIMSTRS)
#   define nscsattr     FNAME(SCSATTR)
#   define nscfattr     FNAME(SCFATTR)
#   define nsfid2ref  FNAME(SFID2REF)
#   define nsfref2index FNAME(SFREF2INDEX)
#   define nsfiscvar   FNAME(SFISCVAR)
#   define nscsextf    FNAME(SCSEXTF)
#   define nsfsacct    FNAME(SFSACCT)
#else   /* DF_CAPFNAMES */
#   define nscstart    FNAME(scstart)
#   define nsfend      FNAME(sfend)
#   define nsfendacc   FNAME(sfendacc)
#   define nsffinfo    FNAME(sffinfo)
#   define nsfselect   FNAME(sfselect)
#   define nscginfo    FNAME(scginfo)
#   define nscgainfo   FNAME(scgainfo)
#   define nscgdinfo   FNAME(scgdinfo)
#   define nsfgcal     FNAME(sfgcal)
#   define nsfscal     FNAME(sfscal)
#   define nsfgdscale  FNAME(sfgdscale)
#   define nsfsdscale  FNAME(sfsdscale)
#   define nsfgfill    FNAME(sfgfill)
#   define nsfsfill    FNAME(sfsfill)
#   define nsfgrange   FNAME(sfgrange)
#   define nsfsrange   FNAME(sfsrange)
#   define nscn2index  FNAME(scn2index)
#   define nsccreate   FNAME(sccreate)
#   define nscsdimstr  FNAME(scsdimstr)
#   define nscsdimname FNAME(scsdimname)
#   define nscsdatstr  FNAME(scsdatstr)
#   define nsfdimid    FNAME(sfdimid)
#   define nsfrattr    FNAME(sfrattr)
#   define nsfrdata    FNAME(sfrdata)
#   define nsfwdata    FNAME(sfwdata)
#   define nscgdatstrs FNAME(scgdatstrs)
#   define nscgdimstrs FNAME(scgdimstrs)
#   define nscsattr    FNAME(scsattr)
#   define nscfattr    FNAME(scfattr)
#   define nsfid2ref   FNAME(sfid2ref)
#   define nsfref2index FNAME(sfref2index)
#   define nsfiscvar   FNAME(sfiscvar)
#   define nscsextf    FNAME(scsextf)
#   define nsfsacct    FNAME(sfsacct)
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif /* _MFSD_H_ */

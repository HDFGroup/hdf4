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

/* change this back if it causes problems on other machines than the Alhpa-QAK */
#ifdef OLD_WAY
#include "hdf.h"
#include "netcdf.h"
#else /* OLD_WAY */
#include "local_nc.h"
#endif /* OLD_WAY */

/* use this as marker for unlimited dimension */
#define SD_UNLIMITED NC_UNLIMITED
#define SD_NOFILL    NC_NOFILL
#define SD_FILL      NC_FILL
#define SD_DIMVAL_BW_COMP   1
#define SD_DIMVAL_BW_INCOMP  0
/* marker for ragged dimension */
#define SD_RAGGED    -1

#ifdef __cplusplus
extern "C" {
#endif

extern int32 SDstart
    (const char *name, int32 access);

extern intn SDend
    (int32 fid);

extern intn SDfileinfo
    (int32 fid, int32 *datasets, int32 *attrs);

extern int32 SDselect
    (int32 fid, int32 index);

extern intn SDgetinfo
    (int32 sdsid, char *name, int32 *rank, int32 *dimsizes, 
           int32 *nt, int32 *nattr);

#ifndef __CSTAR__
extern intn SDreaddata
    (int32 sdsid, int32 *start, int32 *stride, int32 *end, VOIDP data);
#endif

extern uint16 SDgerefnumber
    (int32 sdsid);

extern int32 SDnametoindex
    (int32 fid, char *name);

extern intn SDgetrange
    (int32 sdsid, VOIDP pmax, VOIDP pmin);

extern int32 SDcreate
    (int32 fid, char *name, int32 nt, int32 rank, int32 *dimsizes);

extern int32 SDgetdimid
    (int32 sdsid, intn number);

extern intn SDsetdimname
    (int32 id, char *name);

extern intn SDendaccess
    (int32 id);

extern intn SDsetrange
    (int32 sdsid, VOIDP pmax, VOIDP pmin);

extern intn SDsetattr
    (int32 id, char *name, int32 nt, int32 count, VOIDP data);

extern intn SDattrinfo
    (int32 id, int32 index, char *name, int32 *nt, int32 *count);

extern intn SDreadattr
    (int32 id, int32 index, VOIDP buf);

#ifndef __CSTAR__
extern intn SDwritedata
    (int32 sdsid, int32 *start, int32 *stride, int32 *end, VOIDP data);
#endif

extern intn SDsetdatastrs
    (int32 sdsid, char *l, char *u, char *f, char *c);

extern intn SDsetcal
    (int32 sdsid, float64 cal, float64 cale, float64 ioff,
               float64 ioffe, int32 nt);

extern intn SDsetfillvalue
    (int32 sdsid, VOIDP val);

extern intn SDgetfillvalue
    (int32 sdsid, VOIDP val);

extern intn SDgetdatastrs
    (int32 sdsid, char *l, char *u, char *f, char *c, intn len);

extern intn SDgetcal
    (int32 sdsid, float64 *cal, float64 *cale, float64 *ioff, 
               float64 *ioffe, int32 *nt);

extern intn SDsetdimstrs
    (int32 id, char *l, char *u, char *f);

extern intn SDsetdimscale
    (int32 id, int32 count, int32 nt, VOIDP data);

extern intn SDgetdimscale
    (int32 id, VOIDP data);

extern intn SDdiminfo
    (int32 id, char *name, int32 *size, int32 *nt, int32 *nattr);

extern intn SDgetdimstrs
    (int32 id, char *l, char *u, char *f, intn len);

extern intn SDsetexternalfile
    (int32 id, char *filename, int32 offset);

extern intn SDsetnbitdataset
    (int32 id, intn start_bit, intn bit_len, intn sign_ext, intn fill_one);

extern intn SDsetcompress
    (int32 id, int32 type, comp_info *c_info);

extern int32 SDfindattr
    (int32 id, char *attrname);

extern int32 SDidtoref
    (int32 id);

extern int32 SDreftoindex
    (int32 fid, int32 ref);

extern int32 SDisrecord
    (int32 id);

extern intn SDiscoordvar
    (int32 id);

extern intn SDsetaccesstype
    (int32 id, uintn accesstype);

extern intn SDsetblocksize
    (int32 sdsid, int32 block_size);

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


#ifndef _MFSD_H_
#define _MFSD_H_

#ifndef HDF
#define HDF 1
#endif

#include "hdf.h"
#include "netcdf.h"

#define SD_UNLIMITED NC_UNLIMITED

#ifdef __cplusplus
extern "C" {
#endif

extern int32 SDstart
    PROTO((char *name, int32 access));

extern int32 SDend
    PROTO((int32 fid));

extern int32 SDfileinfo
    PROTO((int32 fid, int32 *datasets, int32 *attrs));

extern int32 SDselect
    PROTO((int32 fid, intn index));

extern int32 SDgetinfo
    PROTO((int32 sdsid, char *name, int32 *rank, int32 *dimsizes, 
           int32 *nt, int32 *nattr));

extern int32 SDreaddata
    PROTO((int32 sdsid, int32 *start, int32 *stride, int32 *end, VOIDP data));

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

extern int32 SDsetdimname
    PROTO((int32 id, char *name));

extern int32 SDendaccess
    PROTO((int32 id));

extern intn SDsetrange
    PROTO((int32 sdsid, VOIDP pmax, VOIDP pmin));

extern intn SDsetattr
    PROTO((int32 id, char *name, int32 nt, intn count, VOIDP data));

extern intn SDattrinfo
    PROTO((int32 id, int32 index, char *name, int32 *nt, intn *count));

extern intn SDreadattr
    PROTO((int32 id, int32 index, VOIDP buf));

extern int32 SDwritedata
    PROTO((int32 sdsid, int32 *start, int32 *stride, int32 *end, VOIDP data));

extern int32 SDsetdatastrs
    PROTO((int32 sdsid, char *l, char *u, char *f, char *c));

extern int32 SDsetcal
    PROTO((int32 sdsid, float64 cal, float64 cale, float64 ioff,
               float64 ioffe, int32 nt));

extern int32 SDsetfillvalue
    PROTO((int32 sdsid, VOIDP val));

extern int32 SDgetfillvalue
    PROTO((int32 sdsid, VOIDP val));

extern int32 SDgetdatastrs
    PROTO((int32 sdsid, char *l, char *u, char *f, char *c, intn len));

extern int32 SDgetcal
    PROTO((int32 sdsid, float64 *cal, float64 *cale, float64 *ioff, 
               float64 *ioffe, int32 *nt));

extern int32 SDsetdimstrs
    PROTO((int32 id, char *l, char *u, char *f));

extern int32 SDsetdimscale
    PROTO((int32 id, int32 count, int32 nt, VOIDP data));

extern int32 SDgetdimscale
    PROTO((int32 id, VOIDP data));

extern int32 SDdiminfo
    PROTO((int32 id, char *name, int32 *size, int32 *nt, int32 *nattr));

extern int32 SDgetdimstrs
    PROTO((int32 id, char *l, char *u, char *f, intn len));


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
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif /* _MFSD_H_ */

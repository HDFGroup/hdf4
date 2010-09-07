/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the files COPYING and Copyright.html.  COPYING can be found at the root   *
 * of the source code distribution tree; Copyright.html can be found at      *
 * http://hdfgroup.org/products/hdf4/doc/Copyright.html.  If you do not have *
 * access to either file, you may request a copy from help@hdfgroup.org.     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef _MFSDF_H_
#define _MFSDF_H_

#include "H4api_adpt.h"

#include "mfhdf.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Define the FORTRAN names */

#if defined _WIN32 && !defined CMAKE_INTDIR /* windows specific cstub functions*/

#define nscend          H4_F77_FUNC(scend,SCEND)
#define nscendacc       H4_F77_FUNC(scendacc,SCENDACC)
#define nscfinfo        H4_F77_FUNC(scfinfo,SCFINFO)
#define nscselct        H4_F77_FUNC(scselect,SCSELCT)
#define nscdimid        H4_F77_FUNC(scdimid,SCDIMID)
#define nscgcal         H4_F77_FUNC(scgcal,SCGCAL)
#define nscscal         H4_F77_FUNC(scscal, SCSCAL)
#define nscgdscale      H4_F77_FUNC(scgdscale, SCGDSCALE)
#define nscsdscale      H4_F77_FUNC(scsdscale, SCSDSCALE)
#define nscgcfill       H4_F77_FUNC(scgcfill, SCGCFILL)
#define nscgfill        H4_F77_FUNC(scgfill, SCGFILL)
#define nscscfill       H4_F77_FUNC(scscfill, SCSCFILL)
#define nscsfill        H4_F77_FUNC(scsfill, SCSFILL)
#define nscsrange       H4_F77_FUNC(scsrange, SCSRANGE)
#define nscgrange       H4_F77_FUNC(scgrange, SCGRANGE)
#define nscrcatt        H4_F77_FUNC(scrcatt, SCRCATT)
#define nscrnatt        H4_F77_FUNC(scrnatt, SCRNATT)
#define nscrattr        H4_F77_FUNC(scrattr, SCRATTR)
#define nscrcdata       H4_F77_FUNC(scrcdata, SCRCDATA)
#define nscrdata        H4_F77_FUNC(scrdata, SCRDATA)
#define nscwcdata       H4_F77_FUNC(scwcdata, SCWCDATA)
#define nscwdata        H4_F77_FUNC(scwdata, SCWDATA)
#define nscid2ref       H4_F77_FUNC(scid2ref, SCID2REF)
#define nscref2index    H4_F77_FUNC(scref2index,SCREF2INDEX)
#define nscr2idx        H4_F77_FUNC(scr2idx,SCR2IDX)
#define nsciscvar       H4_F77_FUNC(sciscvar,SCISCVAR)
#define nscsnbit        H4_F77_FUNC(scsnbit,SCSNBIT)
#define nscsacct        H4_F77_FUNC(scsacct,SCSACCT)
#define nscsdmvc        H4_F77_FUNC(scsdmvc,SCSDMVC)
#define nscisdmvc       H4_F77_FUNC(scisdmvc,SCISDMVC)
#define nscsflmd        H4_F77_FUNC(scsflmd,SCSFLMD)
#define nscisrcrd       H4_F77_FUNC(scisrcrd,SCISRCRD)
#define nscsblsz        H4_F77_FUNC(scsblsz,SCSBLSZ)

HDFFCLIBAPI FRETVAL(intf) nscend(intf *file_id);
HDFFCLIBAPI FRETVAL(intf) nscendacc(intf *id);
HDFFCLIBAPI FRETVAL(intf) nscfinfo(intf *file_id, intf *datasets, intf *gattr);
HDFFCLIBAPI FRETVAL(intf) nscselct(intf *file_id, intf *index);
HDFFCLIBAPI FRETVAL(intf) nscdimid(intf *id, intf *index);
HDFFCLIBAPI FRETVAL(intf) nscgcal(intf *id, float64 *cal, float64 *cale, 
                float64 *ioff, float64 *ioffe, intf *nt);
HDFFCLIBAPI FRETVAL(intf) nscscal(intf *id, float64 *cal, float64 *cale,
                float64 *ioff, float64 *ioffe, intf *nt);
HDFFCLIBAPI FRETVAL(intf) nscsdscale(intf *id, intf *count, intf *nt, VOIDP values);
HDFFCLIBAPI FRETVAL(intf) nscgdscale(intf *id, VOIDP values);
HDFFCLIBAPI FRETVAL(intf) nscscfill(intf *id, _fcd val);
HDFFCLIBAPI FRETVAL(intf) nscgcfill(intf *id, _fcd val);
HDFFCLIBAPI FRETVAL(intf) nscsfill(intf *id, VOIDP val);
HDFFCLIBAPI FRETVAL(intf) nscgfill(intf *id, VOIDP val);
HDFFCLIBAPI FRETVAL(intf) nscgrange(intf *id, VOIDP max, VOIDP min);
HDFFCLIBAPI FRETVAL(intf) nscsrange(intf *id, VOIDP max, VOIDP min);
HDFFCLIBAPI FRETVAL(intf) nscrcatt(intf *id, intf *index, _fcd buf);
HDFFCLIBAPI FRETVAL(intf) nscrnatt(intf *id, intf *index, VOIDP buf);
HDFFCLIBAPI FRETVAL(intf) nscrattr(intf *id, intf *index, VOIDP buf);
HDFFCLIBAPI FRETVAL(intf) nscrdata(intf *id, intf *start, intf *stride, 
                       intf *end, VOIDP values);
HDFFCLIBAPI FRETVAL(intf) nscwdata(intf *id, intf *start, intf *stride, 
                       intf *end, VOIDP values);
HDFFCLIBAPI FRETVAL(intf) nscrcdata(intf *id, intf *start, intf *stride, 
                       intf *end, _fcd values);
HDFFCLIBAPI FRETVAL(intf) nscwcdata(intf *id, intf *start, intf *stride, 
                       intf *end, _fcd values);
HDFFCLIBAPI FRETVAL(intf) nscid2ref(intf *id);
HDFFCLIBAPI FRETVAL(intf) nscr2idx(intf *id, intf *ref);
HDFFCLIBAPI FRETVAL(intf) nsciscvar(intf *id);
HDFFCLIBAPI FRETVAL(intf) nscsnbit(intf *id, intf *start_bit, intf *bit_len,
                       intf *sign_ext, intf *fill_one);
HDFFCLIBAPI FRETVAL(intf) nscsacct(intf *id, intf *type);
HDFFCLIBAPI FRETVAL(intf) nscsdmvc(intf *id, intf *compmode);
HDFFCLIBAPI FRETVAL(intf) nscisdmvc(intf *id);
HDFFCLIBAPI FRETVAL(intf) nscsflmd(intf *id, intf *fillmode);
HDFFCLIBAPI FRETVAL(intf)  nscisrcrd(intf *id);
HDFFCLIBAPI FRETVAL(intf) nscsblsz(intf *id, intf *block_size);

#endif /* WIN32 && !CMAKE_INTDIR */

#   define nscstart      H4_F77_FUNC(scstart, SCSTART)
#   define nsfend        H4_F77_FUNC(sfend, SFEND)
#   define nsfendacc     H4_F77_FUNC(sfendacc, SFENDACC)
#   define nsffinfo      H4_F77_FUNC(sffinfo, SFFINFO)
#   define nsfselect     H4_F77_FUNC(sfselect, SFSELECT)
#   define nscginfo      H4_F77_FUNC(scginfo, SCGINFO)
#   define nscgainfo     H4_F77_FUNC(scgainfo, SCGAINFO)
#   define nscgdinfo     H4_F77_FUNC(scgdinfo, SCGDINFO)
#   define nsfgcal       H4_F77_FUNC(sfgcal, SFGCAL)
#   define nsfscal       H4_F77_FUNC(sfscal, SFSCAL)
#   define nsfgdscale    H4_F77_FUNC(sfgdscale, SFGDSCALE)
#   define nsfsdscale    H4_F77_FUNC(sfsdscale, SFSDSCALE)
#   define nsfgcfill     H4_F77_FUNC(sfgcfill, SFGCFILL)
#   define nsfgfill      H4_F77_FUNC(sfgfill, SFGFILL)
#   define nsfscfill     H4_F77_FUNC(sfscfill, SFSCFILL)
#   define nsfsfill      H4_F77_FUNC(sfsfill, SFSFILL)
#   define nsfsflmd      H4_F77_FUNC(sfsflmd, SFSFLMD)
#   define nsfgrange     H4_F77_FUNC(sfgrange, SFGRANGE)
#   define nsfsrange     H4_F77_FUNC(sfsrange, SFSRANGE)
#   define nscn2index    H4_F77_FUNC(scn2index, SCN2INDEX)
#   define nsccreate     H4_F77_FUNC(sccreate, SCCREATE)
#   define nscsdimstr    H4_F77_FUNC(scsdimstr, SCSDIMSTR)
#   define nscsdimname   H4_F77_FUNC(scsdimname, SCSDIMNAME)
#   define nscsdatstr    H4_F77_FUNC(scsdatstr, SCSDATSTR)
#   define nsfdimid      H4_F77_FUNC(sfdimid, SFDIMID)
#   define nsfrcatt      H4_F77_FUNC(sfrcatt, SFRCATT)
#   define nsfrnatt      H4_F77_FUNC(sfrnatt, SFRNATT)
#   define nsfrattr      H4_F77_FUNC(sfrattr, SFRATTR)
#   define nsfrcdata     H4_F77_FUNC(sfrcdata, SFRCDATA)
#   define nsfrdata      H4_F77_FUNC(sfrdata, SFRDATA)
#   define nsfwcdata     H4_F77_FUNC(sfwcdata, SFWCDATA)
#   define nsfwdata      H4_F77_FUNC(sfwdata, SFWDATA)
#   define nscgdatstrs   H4_F77_FUNC(scgdatstrs, SCGDATSTRS)
#   define nscgdimstrs   H4_F77_FUNC(scgdimstrs, SCGDIMSTRS)
#   define nscscatt      H4_F77_FUNC(scscatt, SCSCATT)
#   define nscsnatt      H4_F77_FUNC(scsnatt, SCSNATT)
#   define nscsattr      H4_F77_FUNC(scsattr, SCSATTR)
#   define nscfattr      H4_F77_FUNC(scfattr, SCFATTR)
#   define nsfid2ref     H4_F77_FUNC(sfid2ref, SFID2REF)
#   define nsfref2index  H4_F77_FUNC(sfref2index, SFREF2INDEX)
#   define nsfiscvar     H4_F77_FUNC(sfiscvar, SFISCVAR)
#   define nscsextf      H4_F77_FUNC(scsextf, SCSEXTF)
#   define nsfsacct      H4_F77_FUNC(sfsacct, SFSACCT)
#   define nsfsdmvc      H4_F77_FUNC(sfsdmvc, SFSDMVC)
#   define nsfisdmvc     H4_F77_FUNC(sfisdmvc, SFISDMVC)
#   define nsfisrcrd     H4_F77_FUNC(sfisrcrd, SFISRCRD)
#   define nscgichnk     H4_F77_FUNC(scgichnk, SCGICHNK)
#   define nscrcchnk     H4_F77_FUNC(scrcchnk, SCRCCHNK)
#   define nscrchnk      H4_F77_FUNC(scrchnk, SCRCHNK)
#   define nscscchnk     H4_F77_FUNC(scscchnk, SCSCCHNK)
#   define nscschnk      H4_F77_FUNC(scschnk, SCSCHNK)
#   define nscwcchnk     H4_F77_FUNC(scwcchnk, SCWCCHNK) 
#   define nscwchnk      H4_F77_FUNC(scwchnk, SCWCHNK)
#   define nscscompress  H4_F77_FUNC(scscompress, SCSCOMPRESS)
#   define nscgcompress  H4_F77_FUNC(scgcompress, SCGCOMPRESS)
#   define nsfsnbit      H4_F77_FUNC(sfsnbit, SFSNBIT)
#   define nsfsblsz      H4_F77_FUNC(sfsblsz, SFSBLSZ)
#   define nscchempty    H4_F77_FUNC(scchempty, SCCHEMPTY)
#   define nscgetfname   H4_F77_FUNC(scgetfname,SCGETFNAME)
#   define nscgetnamelen H4_F77_FUNC(scgetnamelen,SCGETNAMELEN)
#   define nscidtype     H4_F77_FUNC(scidtype,SCIDTYPE)
#   define nscgnvars_byname H4_F77_FUNC_(scgnvars_byname,SCGNVARS_BYNAME)
#   define nscn2indices  H4_F77_FUNC(scn2indices,SCN2INDICES)
#   define nscgmaxopenf  H4_F77_FUNC(scgmaxopenf,SCGMAXOPENF)
#   define nscgnumopenf  H4_F77_FUNC(scgnumopenf,SCGNUMOPENF)
#   define nscrmaxopenf  H4_F77_FUNC(scrmaxopenf,SCRMAXOPENF)

HDFFCLIBAPI FRETVAL(intf) nscstart(_fcd name, intf *access, intf *namelen);
HDFFCLIBAPI FRETVAL(intf) nscginfo(intf *id, _fcd name, intf *rank, 
                intf *dimsizes, intf *nt, intf *nattr, intf *len);
HDFFCLIBAPI FRETVAL(intf) nscn2index(intf *id, _fcd name, intf *namelen);
HDFFCLIBAPI FRETVAL(intf) nsccreate(intf *id, _fcd name, intf *nt, intf *rank, 
                        intf *dims, intf *namelen);
HDFFCLIBAPI FRETVAL(intf) nscsdimstr(intf *id, _fcd l, _fcd u, _fcd f, intf *ll, 
                        intf *ul, intf *fl);
HDFFCLIBAPI FRETVAL(intf) nscsdimname(intf *id, _fcd name, intf *len);
HDFFCLIBAPI FRETVAL(intf) nscsdatstr(intf *id, _fcd l, _fcd u, _fcd f, _fcd c, 
                        intf *ll, intf *ul, intf *fl, intf *cl);
HDFFCLIBAPI FRETVAL(intf) nscgdimstrs(intf *dim, _fcd label, _fcd unit, 
                      _fcd format,intf *llabel,intf *lunit, 
                      intf *lformat, intf *mlen);
HDFFCLIBAPI FRETVAL(intf) nscgdatstrs(intf *id, _fcd label, _fcd unit,
                 _fcd format, _fcd coord, intf *llabel, intf *lunit,
                   intf *lformat, intf *lcoord, intf *len);
HDFFCLIBAPI FRETVAL(intf) nscgainfo(intf *id, intf *number, _fcd name, intf *nt, 
                   intf *count, intf *len);
HDFFCLIBAPI FRETVAL(intf) nscgdinfo(intf *id, _fcd name, intf *sz, intf *nt, 
                   intf *nattr, intf *len);
HDFFCLIBAPI FRETVAL(intf) nscscatt(intf *id, _fcd name, intf *nt, intf *count,
                    _fcd data, intf *len);
HDFFCLIBAPI FRETVAL(intf) nscsnatt(intf *id, _fcd name, intf *nt, intf *count, VOIDP data, intf *len);
HDFFCLIBAPI FRETVAL(intf) nscsattr(intf *id, _fcd name, intf *nt, intf *count, 
                       VOIDP data, intf *len);
HDFFCLIBAPI FRETVAL(intf) nscsextf(intf *id, _fcd name, intf *offset,
                       intf *namelen);
HDFFCLIBAPI FRETVAL (intf) nscgichnk(intf *id, intf *dim_length, intf *flags);
HDFFCLIBAPI FRETVAL (intf) nscrcchnk(intf *id, intf *start, _fcd char_data);
HDFFCLIBAPI FRETVAL (intf) nscrchnk(intf *id, intf *start, VOIDP num_data);
HDFFCLIBAPI FRETVAL (intf) nscscchnk(intf *id, intf *maxcache, intf *flags);
HDFFCLIBAPI FRETVAL (intf) nscschnk(intf *id, intf *dim_length, intf *comp_type,
                intf *comp_prm);
HDFFCLIBAPI FRETVAL (intf) nscwcchnk(intf *id, intf *start, _fcd char_data);
HDFFCLIBAPI FRETVAL (intf) nscwchnk(intf *id, intf *start, VOIDP num_data);
HDFFCLIBAPI FRETVAL (intf) nscscompress(intf *id, intf *comp_type, intf *comp_prm);
HDFFCLIBAPI FRETVAL(intf) nscgcompress(intf *id, intf *comp_type, intf *comp_prm);
HDFFCLIBAPI FRETVAL(intf) nscfattr(intf *id, _fcd name, intf *namelen); 
HDFFCLIBAPI FRETVAL(intf) nscchempty(intf *id, intf *flag);

#ifdef __cplusplus
}
#endif

#endif /* _MFSDF_H_ */

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

#ifndef _MFSDF_H_
#define _MFSDF_H_

#include "H4api_adpt.h"

#include "mfhdf.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Define the FORTRAN names */

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
#   define nscgnvars_byname H4_F77_FUNC(scgnvars_byname,SCGNVARS_BYNAME)
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

HDFFCLIBAPI FRETVAL(intf) nscgnvars_byname(intf *sd_id, _fcd name, intf *namelen, intf *n_vars);
HDFFCLIBAPI FRETVAL(intf) nscn2indices(intf *sd_id, _fcd name, intf *namelen, intf *var_list, intf *type_list, intf *n_vars);
HDFFCLIBAPI FRETVAL(intf) nscgnumopenf(intf *cur_num);
HDFFCLIBAPI FRETVAL(intf) nscgmaxopenf(intf *cur_max, intf *sys_limit);
HDFFCLIBAPI FRETVAL(intf) nscrmaxopenf(intf *req_max);
HDFFCLIBAPI FRETVAL(intf) nscidtype(intf *obj_id, intf *obj_type);
HDFFCLIBAPI FRETVAL(intf) nscgetnamelen(intf *obj_id, intf *namelen);
HDFFCLIBAPI FRETVAL(intf) nscgetfname(intf *file_id, _fcd name, intf *namelen);

HDFFCLIBAPI FRETVAL(intf) nsfend(intf *file_id);
HDFFCLIBAPI FRETVAL(intf) nsfendacc(intf *id);
HDFFCLIBAPI FRETVAL(intf) nsffinfo(intf *file_id, intf *datasets, intf *gattr);
HDFFCLIBAPI FRETVAL(intf) nsfselect(intf *file_id, intf *index);
HDFFCLIBAPI FRETVAL(intf) nsfdimid(intf *id, intf *index);
HDFFCLIBAPI FRETVAL(intf) nsfgcal(intf *id, float64 *cal, float64 *cale, 
                float64 *ioff, float64 *ioffe, intf *nt);
HDFFCLIBAPI FRETVAL(intf) nsfscal(intf *id, float64 *cal, float64 *cale,
                float64 *ioff, float64 *ioffe, intf *nt);
HDFFCLIBAPI FRETVAL(intf) nsfsdscale(intf *id, intf *count, intf *nt, VOIDP values);
HDFFCLIBAPI FRETVAL(intf) nsfgdscale(intf *id, VOIDP values);
HDFFCLIBAPI FRETVAL(intf) nsfscfill(intf *id, _fcd val);
HDFFCLIBAPI FRETVAL(intf) nsfgcfill(intf *id, _fcd val);
HDFFCLIBAPI FRETVAL(intf) nsfsfill(intf *id, VOIDP val);
HDFFCLIBAPI FRETVAL(intf) nsfgfill(intf *id, VOIDP val);
HDFFCLIBAPI FRETVAL(intf) nsfgrange(intf *id, VOIDP max, VOIDP min);
HDFFCLIBAPI FRETVAL(intf) nsfsrange(intf *id, VOIDP max, VOIDP min);
HDFFCLIBAPI FRETVAL(intf) nsfrcatt(intf *id, intf *index, _fcd buf);
HDFFCLIBAPI FRETVAL(intf) nsfrnatt(intf *id, intf *index, VOIDP buf);
HDFFCLIBAPI FRETVAL(intf) nsfrattr(intf *id, intf *index, VOIDP buf);
HDFFCLIBAPI FRETVAL(intf) nsfrdata(intf *id, intf *start, intf *stride, 
                       intf *end, VOIDP values);
HDFFCLIBAPI FRETVAL(intf) nsfwdata(intf *id, intf *start, intf *stride, 
                       intf *end, VOIDP values);
HDFFCLIBAPI FRETVAL(intf) nsfrcdata(intf *id, intf *start, intf *stride, 
                       intf *end, _fcd values);
HDFFCLIBAPI FRETVAL(intf) nsfwcdata(intf *id, intf *start, intf *stride, 
                       intf *end, _fcd values);
HDFFCLIBAPI FRETVAL(intf) nsfid2ref(intf *id);
HDFFCLIBAPI FRETVAL(intf) nsfref2index(intf *id, intf *ref);
HDFFCLIBAPI FRETVAL(intf) nsfiscvar(intf *id);
HDFFCLIBAPI FRETVAL(intf) nsfsnbit(intf *id, intf *start_bit, intf *bit_len,
                       intf *sign_ext, intf *fill_one);
HDFFCLIBAPI FRETVAL(intf) nsfsacct(intf *id, intf *type);
HDFFCLIBAPI FRETVAL(intf) nsfsdmvc(intf *id, intf *compmode);
HDFFCLIBAPI FRETVAL(intf) nsfisdmvc(intf *id);
HDFFCLIBAPI FRETVAL(intf) nsfsflmd(intf *id, intf *fillmode);
HDFFCLIBAPI FRETVAL(intf) nsfisrcrd(intf *id);
HDFFCLIBAPI FRETVAL(intf) nsfsblsz(intf *id, intf *block_size);

#ifdef __cplusplus
}
#endif

#endif /* _MFSDF_H_ */

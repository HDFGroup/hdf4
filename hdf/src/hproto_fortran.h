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

#ifndef H4_H_PROTO_FORTRAN
#define H4_H_PROTO_FORTRAN

#include "H4api_adpt.h"
#include "df_priv.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 ** from dfanF.c
 */
#define ndaiganl       H4_F77_FUNC(daiganl, DAIGANL)
#define ndaigann       H4_F77_FUNC(daigann, DAIGANN)
#define ndaipann       H4_F77_FUNC(daipann, DAIPANN)
#define ndailist       H4_F77_FUNC(dailist, DAILIST)
#define ndalref        H4_F77_FUNC(dalref, DALREF)
#define ndaclear       H4_F77_FUNC(daclear, DACLEAR)
#define ndfanlastref   H4_F77_FUNC(dfanlastref, DFANLASTREF)
#define ndfanaddfds    H4_F77_FUNC(dfanaddfds, DFANADDFDS)
#define ndfangetfidlen H4_F77_FUNC(dfangetfidlen, DFANGETFIDLEN)
#define ndfangetfdslen H4_F77_FUNC(dfangetfdslen, DFANGETFDSLEN)
#define ndfangetfid    H4_F77_FUNC(dfangetfid, DFANGETFID)
#define ndfangetfds    H4_F77_FUNC(dfangetfds, DFANGETFDS)
#define ndaafds        H4_F77_FUNC(daafds, DAAFDS)
#define ndagfidl       H4_F77_FUNC(dagfidl, DAGFIDL)
#define ndagfdsl       H4_F77_FUNC(dagfdsl, DAGFDSL)
#define ndagfid        H4_F77_FUNC(dagfid, DAGFID)
#define ndagfds        H4_F77_FUNC(dagfds, DAGFDS)
#define ndaiafid       H4_F77_FUNC(daiafid, DAIAFID)

HDFFCLIBAPI intf ndaiganl(_fcd filename, intf *tag, intf *ref, intf *type, intf *fnlen);

HDFFCLIBAPI intf ndaigann(_fcd filename, intf *tag, intf *ref, _fcd annotation, intf *maxlen, intf *type,
                          intf *fnlen);

HDFFCLIBAPI intf ndaipann(_fcd filename, intf *tag, intf *ref, _fcd annotation, intf *annlen, intf *type,
                          intf *fnlen);

HDFFCLIBAPI intf ndailist(_fcd filename, intf *tag, intf reflist[], _fcd labellist, intf *listsize,
                          intf *maxlen, intf *startpos, intf *fnlen);

HDFFCLIBAPI intf ndalref(void);

HDFFCLIBAPI intf ndaclear(void);

HDFFCLIBAPI intf ndfanlastref(void);

HDFFCLIBAPI intf ndfanaddfds(intf *dfile, _fcd desc, intf *desclen);

HDFFCLIBAPI intf ndfangetfidlen(intf *dfile, intf *isfirst);

HDFFCLIBAPI intf ndfangetfdslen(intf *dfile, intf *isfirst);

HDFFCLIBAPI intf ndfangetfid(intf *dfile, _fcd id, intf *maxlen, intf *isfirst);

HDFFCLIBAPI intf ndfangetfds(intf *dfile, _fcd id, intf *maxlen, intf *isfirst);

HDFFCLIBAPI intf ndaafds(intf *dfile, _fcd desc, intf *desclen);

HDFFCLIBAPI intf ndagfidl(intf *dfile, intf *isfirst);

HDFFCLIBAPI intf ndagfdsl(intf *dfile, intf *isfirst);

HDFFCLIBAPI intf ndagfid(intf *dfile, _fcd id, intf *maxlen, intf *isfirst);

HDFFCLIBAPI intf ndagfds(intf *dfile, _fcd id, intf *maxlen, intf *isfirst);

HDFFCLIBAPI intf ndaiafid(intf *dfile, _fcd id, intf *idlen);

/*
 ** from dfr8F.c
 */
#define nd8spal         H4_F77_FUNC(d8spal, D8SPAL)
#define nd8first        H4_F77_FUNC(d8first, D8FIRST)
#define nd8igdim        H4_F77_FUNC(d8igdim, D8IGDIM)
#define nd8igimg        H4_F77_FUNC(d8igimg, D8IGIMG)
#define nd8ipimg        H4_F77_FUNC(d8ipimg, D8IPIMG)
#define nd8iaimg        H4_F77_FUNC(d8iaimg, D8IAIMG)
#define nd8irref        H4_F77_FUNC(d8irref, D8IRREF)
#define nd8iwref        H4_F77_FUNC(d8iwref, D8IWREF)
#define nd8inims        H4_F77_FUNC(d8inims, D8INIMS)
#define nd8lref         H4_F77_FUNC(d8lref, D8LREF)
#define ndfr8lastref    H4_F77_FUNC(dfr8lastref, DFR8LASTREF)
#define ndfr8setpalette H4_F77_FUNC(dfr8setpalette, DFR8SETPALETTE)
#define ndfr8restart    H4_F77_FUNC(dfr8restart, DFR8RESTART)
#define nd8scomp        H4_F77_FUNC(d8scomp, D8SCOMP)
#define ndfr8scompress  H4_F77_FUNC(dfr8scompress, DFR8SCOMPRESS)
#define nd8sjpeg        H4_F77_FUNC(d8sjpeg, D8SJPEG)
#define ndfr8sjpeg      H4_F77_FUNC(dfr8sjpeg, DFR8SJPEG)

HDFFCLIBAPI intf nd8spal(_fcd pal);

HDFFCLIBAPI intf nd8first(void);

HDFFCLIBAPI intf nd8igdim(_fcd filename, intf *xdim, intf *ydim, intf *ispal, intf *lenfn);

HDFFCLIBAPI intf nd8igimg(_fcd filename, _fcd image, intf *xdim, intf *ydim, _fcd pal, intf *lenfn);

HDFFCLIBAPI intf nd8ipimg(_fcd filename, _fcd image, intf *xdim, intf *ydim, intf *compress, intf *lenfn);

HDFFCLIBAPI intf nd8iaimg(_fcd filename, _fcd image, intf *xdim, intf *ydim, intf *compress, intf *lenfn);

HDFFCLIBAPI intf nd8irref(_fcd filename, intf *ref, intf *fnlen);

HDFFCLIBAPI intf nd8iwref(_fcd filename, intf *ref, intf *fnlen);

HDFFCLIBAPI intf nd8inims(_fcd filename, intf *fnlen);

HDFFCLIBAPI intf nd8lref(void);

HDFFCLIBAPI intf ndfr8lastref(void);

HDFFCLIBAPI intf ndfr8setpalette(_fcd pal);

HDFFCLIBAPI intf ndfr8restart(void);

HDFFCLIBAPI intf nd8scomp(intf *scheme);

HDFFCLIBAPI intf ndfr8scompress(intf *scheme);

HDFFCLIBAPI intf nd8sjpeg(intf *quality, intf *force_baseline);

HDFFCLIBAPI intf ndfr8sjpeg(intf *quality, intf *force_baseline);

/*
 ** from dfsdF.c
 */
#define ndsgdast         H4_F77_FUNC(dsgdast, DSGDAST)
#define ndsgdisc         H4_F77_FUNC(dsgdisc, DSGDISC)
#define ndsgrang         H4_F77_FUNC(dsgrang, DSGRANG)
#define ndssdims         H4_F77_FUNC(dssdims, DSSDIMS)
#define ndssdisc         H4_F77_FUNC(dssdisc, DSSDISC)
#define ndssrang         H4_F77_FUNC(dssrang, DSSRANG)
#define ndsclear         H4_F77_FUNC(dsclear, DSCLEAR)
#define ndsslens         H4_F77_FUNC(dsslens, DSSLENS)
#define ndsgdiln         H4_F77_FUNC(dsgdiln, DSGDILN)
#define ndsgdaln         H4_F77_FUNC(dsgdaln, DSGDALN)
#define ndsfirst         H4_F77_FUNC(dsfirst, DSFIRST)
#define ndspslc          H4_F77_FUNC(dspslc, DSPSLC)
#define ndseslc          H4_F77_FUNC(dseslc, DSESLC)
#define ndsgnt           H4_F77_FUNC(dsgnt, DSGNT)
#define ndssnt           H4_F77_FUNC(dssnt, DSSNT)
#define ndsigdim         H4_F77_FUNC(dsigdim, DSIGDIM)
#define ndsigdat         H4_F77_FUNC(dsigdat, DSIGDAT)
#define ndsipdat         H4_F77_FUNC(dsipdat, DSIPDAT)
#define ndsiadat         H4_F77_FUNC(dsiadat, DSIADAT)
#define ndsigdas         H4_F77_FUNC(dsigdas, DSIGDAS)
#define ndsigslc         H4_F77_FUNC(dsigslc, DSIGSLC)
#define ndsigdis         H4_F77_FUNC(dsigdis, DSIGDIS)
#define ndsisslc         H4_F77_FUNC(dsisslc, DSISSLC)
#define ndsisdas         H4_F77_FUNC(dsisdas, DSISDAS)
#define ndsisdis         H4_F77_FUNC(dsisdis, DSISDIS)
#define ndsirref         H4_F77_FUNC(dsirref, DSIRREF)
#define ndslref          H4_F77_FUNC(dslref, DSLREF)
#define ndsinum          H4_F77_FUNC(dsinum, DSINUM)
#define ndsip32s         H4_F77_FUNC(dsip32s, DSIP32S)
#define ndsscal          H4_F77_FUNC(dsscal, DSSCAL)
#define ndsgcal          H4_F77_FUNC(dsgcal, DSGCAL)
#define ndfsdgetdatastrs H4_F77_FUNC(dfsdgetdatastrs, DFSDGETDATASTRS)
#define ndfsdgetdimscale H4_F77_FUNC(dfsdgetdimscale, DFSDGETDIMSCALE)
#define ndfsdgetrange    H4_F77_FUNC(dfsdgetrange, DFSDGETRANGE)
#define ndfsdsetdims     H4_F77_FUNC(dfsdsetdims, DFSDSETDIMS)
#define ndfsdsetdimscale H4_F77_FUNC(dfsdsetdimscale, DFSDSETDIMSCALE)
#define ndfsdsetrange    H4_F77_FUNC(dfsdsetrange, DFSDSETRANGE)
#define ndfsdclear       H4_F77_FUNC(dfsdclear, DFSDCLEAR)
#define ndfsdsetlengths  H4_F77_FUNC(dfsdsetlengths, DFSDSETLENGTHS)
#define ndfsdgetdimlen   H4_F77_FUNC(dfsdgetdimlen, DFSDGETDIMLEN)
#define ndfsdgetdatalen  H4_F77_FUNC(dfsdgetdatalen, DFSDGETDATALEN)
#define ndfsdrestart     H4_F77_FUNC(dfsdrestart, DFSDRESTART)
#define ndfsdputslice    H4_F77_FUNC(dfsdputslice, DFSDPUTSLICE)
#define ndfsdendslice    H4_F77_FUNC(dfsdendslice, DFSDENDSLICE)
#define ndfsdsetnt       H4_F77_FUNC(dfsdsetnt, DFSDSETNT)
#define ndfsdgetnt       H4_F77_FUNC(dfsdgetnt, DFSDGETNT)
#define ndfsdlastref     H4_F77_FUNC(dfsdlastref, DFSDLASTREF)
#define ndsiwref         H4_F77_FUNC(dsiwref, DSIWREF)
#define ndssfill         H4_F77_FUNC(dssfill, DSSFILL)
#define ndsgfill         H4_F77_FUNC(dsgfill, DSGFILL)
#define ndsisslab        H4_F77_FUNC(dsisslab, DSISSLAB)
#define ndswslab         H4_F77_FUNC(dswslab, DSWSLAB)
#define ndseslab         H4_F77_FUNC(dseslab, DSESLAB)
#define ndsirslab        H4_F77_FUNC(dsirslab, DSIRSLAB)

HDFFCLIBAPI intf ndsgdisc(intf *dim, intf *maxsize, void *scale);

HDFFCLIBAPI intf ndsgrang(void *pmax, void *pmin);

HDFFCLIBAPI intf ndssdims(intf *rank, intf dimsizes[]);

HDFFCLIBAPI intf ndssdisc(intf *dim, intf *dimsize, void *scale);

HDFFCLIBAPI intf ndssrang(void *max, void *min);

HDFFCLIBAPI intf ndsclear(void);

HDFFCLIBAPI intf ndsslens(intf *maxlen_label, intf *maxlen_unit, intf *maxlen_format, intf *maxlen_coordsys);

HDFFCLIBAPI intf ndsgdiln(intf *dim, intf *llabel, intf *lunit, intf *lformat);

HDFFCLIBAPI intf ndsgdaln(intf *llabel, intf *lunit, intf *lformat, intf *lcoordsys);

HDFFCLIBAPI intf ndsfirst(void);

HDFFCLIBAPI intf ndspslc(intf windims[], void *data, intf dims[]);

HDFFCLIBAPI intf ndseslc(void);

HDFFCLIBAPI intf ndssnt(intf *numbertype);

HDFFCLIBAPI intf ndsgnt(intf *pnumbertype);

HDFFCLIBAPI intf ndsigdim(_fcd filename, intf *prank, intf sizes[], intf *maxrank, intf *lenfn);

HDFFCLIBAPI intf ndsigdat(_fcd filename, intf *rank, intf maxsizes[], void *data, intf *fnlen);

HDFFCLIBAPI intf ndsipdat(_fcd filename, intf *rank, intf dimsizes[], void *data, intf *fnlen);

HDFFCLIBAPI intf ndsiadat(_fcd filename, intf *rank, intf dimsizes[], void *data, intf *fnlen);

HDFFCLIBAPI intf ndsigslc(_fcd filename, intf winst[], intf windims[], void *data, intf dims[], intf *fnlen);

HDFFCLIBAPI intf ndsisslc(_fcd filename, intf *fnlen);

HDFFCLIBAPI intf ndsirref(_fcd filename, intf *ref, intf *fnlen);

HDFFCLIBAPI intf ndslref(void);

HDFFCLIBAPI intf ndsinum(_fcd filename, intf *len);

HDFFCLIBAPI intf ndsip32s(_fcd filename, intf *ref, intf *ispre32, intf *len);

HDFFCLIBAPI intf ndfsdgetdatastrs(_fcd label, _fcd unit, _fcd format, _fcd coordsys);

HDFFCLIBAPI intf ndfsdgetdimstrs(intf *dim, _fcd label, _fcd unit, _fcd format);

HDFFCLIBAPI intf ndfsdgetdimscale(intf *dim, intf *maxsize, void *scale);

HDFFCLIBAPI intf ndfsdgetrange(void *pmax, void *pmin);

HDFFCLIBAPI intf ndfsdsetdims(intf *rank, intf dimsizes[]);

HDFFCLIBAPI intf ndfsdsetdimscale(intf *dim, intf *dimsize, void *scale);

HDFFCLIBAPI intf ndfsdsetrange(void *max, void *min);

HDFFCLIBAPI intf ndfsdclear(void);

HDFFCLIBAPI intf ndfsdsetlengths(intf *maxlen_label, intf *maxlen_unit, intf *maxlen_format,
                                 intf *maxlen_coordsys);

HDFFCLIBAPI intf ndfsdgetdimlen(intf *dim, intf *llabel, intf *lunit, intf *lformat);

HDFFCLIBAPI intf ndfsdgetdatalen(intf *llabel, intf *lunit, intf *lformat, intf *lcoordsys);

HDFFCLIBAPI intf ndfsdrestart(void);

HDFFCLIBAPI intf ndfsdputslice(intf windims[], void *data, intf dims[]);

HDFFCLIBAPI intf ndfsdendslice(void);

HDFFCLIBAPI intf ndfsdsetnt(intf *numbertype);

HDFFCLIBAPI intf ndfsdgetnt(intf *pnumbertype);

HDFFCLIBAPI intf ndfsdlastref(void);

HDFFCLIBAPI intf ndsisdis(intf *dim, _fcd flabel, _fcd funit, _fcd fformat, intf *llabel, intf *lunit,
                          intf *lformat);

HDFFCLIBAPI intf ndsigdis(intf *dim, _fcd label, _fcd unit, _fcd format, intf *llabel, intf *lunit,
                          intf *lformat);

HDFFCLIBAPI intf ndsisdas(_fcd flabel, _fcd funit, _fcd fformat, _fcd fcoordsys, intf *isfortran,
                          intf *llabel, intf *lunit, intf *lformat, intf *lcoordsys);

HDFFCLIBAPI intf ndsigdas(_fcd label, _fcd unit, _fcd format, _fcd coordsys, intf *llabel, intf *lunit,
                          intf *lformat, intf *lcoord);

HDFFCLIBAPI intf ndsscal(float64 *cal, float64 *cal_err, float64 *ioff, float64 *ioff_err, intf *cal_type);

HDFFCLIBAPI intf ndsgcal(float64 *cal, float64 *cal_err, float64 *ioff, float64 *ioff_err, intf *cal_type);

HDFFCLIBAPI intf ndswref(_fcd filename, intf *fnlen, intf *ref);

HDFFCLIBAPI intf ndssfill(void *fill_value);

HDFFCLIBAPI intf ndsgfill(void *fill_value);

HDFFCLIBAPI intf ndssslab(_fcd filename, intf *fnlen);

HDFFCLIBAPI intf ndswslab(intf start[], intf stride[], intf cont[], void *data);

HDFFCLIBAPI intf ndseslab(void);

HDFFCLIBAPI intf ndsiwref(_fcd filename, intf *fnlen, intf *ref);

HDFFCLIBAPI intf ndsisslab(_fcd filename, intf *fnlen);

HDFFCLIBAPI intf ndsirslab(_fcd filename, intf *fnlen, intf start[], intf slab_size[], intf stride[],
                           void *buffer, intf buffer_size[]);

/*
 ** from dfpF.c
 */

#define ndpigpal    H4_F77_FUNC(dpigpal, DPIGPAL)
#define ndpippal    H4_F77_FUNC(dpippal, DPIPPAL)
#define ndpinpal    H4_F77_FUNC(dpinpal, DPINPAL)
#define ndpiwref    H4_F77_FUNC(dpiwref, DPIWREF)
#define ndpirref    H4_F77_FUNC(dpirref, DPIRREF)
#define ndprest     H4_F77_FUNC(dprest, DPREST)
#define ndplref     H4_F77_FUNC(dplref, DPLREF)
#define ndfprestart H4_F77_FUNC(dfprestart, DFPRESTART)
#define ndfplastref H4_F77_FUNC(dfplastref, DFPLASTREF)

HDFFCLIBAPI intf ndpigpal(_fcd filename, _fcd pal, intf *fnlen);

HDFFCLIBAPI intf ndpippal(_fcd filename, _fcd pal, intf *overwrite, _fcd filemode, intf *fnlen);

HDFFCLIBAPI intf ndpinpal(_fcd filename, intf *fnlen);

HDFFCLIBAPI intf ndpirref(_fcd filename, intf *ref, intf *fnlen);

HDFFCLIBAPI intf ndpiwref(_fcd filename, intf *ref, intf *fnlen);

HDFFCLIBAPI intf ndprest(void);

HDFFCLIBAPI intf ndplref(void);

HDFFCLIBAPI intf ndfprestart(void);

HDFFCLIBAPI intf ndfplastref(void);

/*
 ** from df24F.c
 */
#define nd2reqil       H4_F77_FUNC(d2reqil, D2REQIL)
#define ndf24reqil     H4_F77_FUNC(df24reqil, DF24REQIL)
#define nd2sdims       H4_F77_FUNC(d2sdims, D2SDIMS)
#define ndf24setdims   H4_F77_FUNC(df24setdims, DF24SETDIMS)
#define nd2setil       H4_F77_FUNC(d2setil, D2SETIL)
#define ndf24setil     H4_F77_FUNC(df24setil, DF24SETIL)
#define nd2first       H4_F77_FUNC(d2first, D2FIRST)
#define ndf24restart   H4_F77_FUNC(df24restart, DF24RESTART)
#define nd2igdim       H4_F77_FUNC(d2igdim, D2IGDIM)
#define nd2igimg       H4_F77_FUNC(d2igimg, D2IGIMG)
#define nd2iaimg       H4_F77_FUNC(d2iaimg, D2IAIMG)
#define nd2irref       H4_F77_FUNC(d2irref, D2IRREF)
#define nd2inimg       H4_F77_FUNC(d2inimg, D2INIMG)
#define nd2lref        H4_F77_FUNC(d2lref, D2LREF)
#define nd2scomp       H4_F77_FUNC(d2scomp, D2SCOMP)
#define ndf24scompress H4_F77_FUNC(df24scompress, DF24SCOMPRESS)
#define nd2sjpeg       H4_F77_FUNC(d2sjpeg, D2SJPEG)
#define ndf24sjpeg     H4_F77_FUNC(df24sjpeg, DF24SJPEG)

HDFFCLIBAPI intf nd2reqil(intf *il);

HDFFCLIBAPI intf nd2sdims(intf *xdim, intf *ydim);

HDFFCLIBAPI intf nd2igdim(_fcd filename, intf *pxdim, intf *pydim, intf *pil, intf *fnlen);

HDFFCLIBAPI intf nd2igimg(_fcd filename, _fcd image, intf *xdim, intf *ydim, intf *fnlen);

HDFFCLIBAPI intf nd2iaimg(_fcd filename, _fcd image, intf *xdim, intf *ydim, intf *fnlen, intf *newfile);

HDFFCLIBAPI intf nd2setil(intf *il);

HDFFCLIBAPI intf nd2first(void);

HDFFCLIBAPI intf ndf24reqil(intf *il);

HDFFCLIBAPI intf ndf24setdims(intf *xdim, intf *ydim);

HDFFCLIBAPI intf ndf24setil(intf *il);

HDFFCLIBAPI intf ndf24restart(void);

HDFFCLIBAPI intf nd2irref(_fcd filename, intf *ref, intf *fnlen);

HDFFCLIBAPI intf nd2inimg(_fcd filename, intf *fnlen);

HDFFCLIBAPI intf nd2lref(void);

HDFFCLIBAPI intf nd2scomp(intf *scheme);

HDFFCLIBAPI intf ndf24scompress(intf *scheme);

HDFFCLIBAPI intf nd2sjpeg(intf *quality, intf *force_baseline);

HDFFCLIBAPI intf ndf24sjpeg(intf *quality, intf *force_baseline);

/*
 ** from dfF.c
 */
#define ndfiaccess H4_F77_FUNC(dfiaccess, DFIACCESS)
#define ndfiopen   H4_F77_FUNC(dfiopen, DFIOPEN)
#define ndfclose   H4_F77_FUNC(dfclose, DFCLOSE)
#define ndfdesc    H4_F77_FUNC(dfdesc, DFDESC)
#define ndfdup     H4_F77_FUNC(dfdup, DFDUP)
#define ndfdel     H4_F77_FUNC(dfdel, DFDEL)
#define ndfstart   H4_F77_FUNC(dfstart, DFSTART)
#define ndfread    H4_F77_FUNC(dfread, DFREAD)
#define ndfseek    H4_F77_FUNC(dfseek, DFSEEK)
#define ndfwrite   H4_F77_FUNC(dfwrite, DFWRITE)
#define ndfupdate  H4_F77_FUNC(dfupdate, DFUPDATE)
#define ndfget     H4_F77_FUNC(dfget, DFGET)
#define ndfput     H4_F77_FUNC(dfput, DFPUT)
#define ndfsfind   H4_F77_FUNC(dfsfind, DFSFIND)
#define ndffind    H4_F77_FUNC(dffind, DFFIND)
#define ndferrno   H4_F77_FUNC(dferrno, DFERRNO)
#define ndfnewref  H4_F77_FUNC(dfnewref, DFNEWREF)
#define ndfnumber  H4_F77_FUNC(dfnumber, DFNUMBER)
#define ndfstat    H4_F77_FUNC(dfstat, DFSTAT)
#define ndfiishdf  H4_F77_FUNC(dfiishdf, DFIISHDF)

HDFFCLIBAPI intf ndfiopen(_fcd name, intf *acc_mode, intf *defdds, intf *namelen);

HDFFCLIBAPI intf ndfclose(intf *dfile);

HDFFCLIBAPI intf ndfdesc(intf *dfile, intf ptr[][4], intf *begin, intf *num);

HDFFCLIBAPI intf ndfdup(intf *dfile, intf *tag, intf *ref, intf *otag, intf *oref);

HDFFCLIBAPI intf ndfdel(intf *dfile, intf *tag, intf *ref);

HDFFCLIBAPI intf ndfiaccess(intf *dfile, intf *tag, intf *ref, _fcd acc_mode, intf *acclen);

HDFFCLIBAPI intf ndfstart(intf *dfile, intf *tag, intf *ref, char *acc_mode);

HDFFCLIBAPI intf ndfread(intf *dfile, _fcd ptr, intf *len);

HDFFCLIBAPI intf ndfseek(intf *dfile, intf *offset);

HDFFCLIBAPI intf ndfwrite(intf *dfile, _fcd ptr, intf *len);

HDFFCLIBAPI intf ndfupdate(intf *dfile);

HDFFCLIBAPI intf ndfget(intf *dfile, intf *tag, intf *ref, _fcd ptr);

HDFFCLIBAPI intf ndfput(intf *dfile, intf *tag, intf *ref, _fcd ptr, intf *len);

HDFFCLIBAPI intf ndfsfind(intf *dfile, intf *tag, intf *ref);

HDFFCLIBAPI intf ndffind(intf *dfile, intf *itag, intf *iref, intf *len);

HDFFCLIBAPI intf ndferrno(void);

HDFFCLIBAPI intf ndfnewref(intf *dfile);

HDFFCLIBAPI intf ndfnumber(intf *dfile, intf *tag);

HDFFCLIBAPI intf ndfstat(intf *dfile, DFdata *dfinfo);

HDFFCLIBAPI intf ndfiishdf(_fcd name, intf *namelen);

/*
 ** from dfutilF.c
 */
#define ndfindnr       H4_F77_FUNC(dfindnr, DFINDNR)
#define ndffindnextref H4_F77_FUNC(dffindnextref, DFFINDNEXTREF)

HDFFCLIBAPI intf ndfindnr(intf *dfile, intf *tag, intf *lref);

HDFFCLIBAPI intf ndffindnextref(intf *dfile, intf *tag, intf *lref);

/*
 ** from herrF.c
 */
#define nheprnt    H4_F77_FUNC(heprnt, HEPRNT)
#define nheprntc   H4_F77_FUNC(heprntc, HEPRNTC)
#define nhestringc H4_F77_FUNC(hestringc, HESTRINGC)

HDFFCLIBAPI void nheprnt(intf *print_levels);

HDFFCLIBAPI intf nheprntc(_fcd filename, intf *print_levels, intf *namelen);

HDFFCLIBAPI intf nhestringc(intf *error_code, _fcd error_message, intf *len);
/*
 ** from hfilef.c
 */
/*
 *  Not all HDF4 Fortran functions are real Fortran functions, they are C wrappers called from a
 *  Fortran program. Since on Windows we have to have a real Fortran function, additional C wrappers
 *  were added at the time when Windows port was done for multi-file interfaces: for example,
 *  hclose is a C wrapper on UNIX, while on Windows we have two functions: Fortran function hclose
 *  and C wrapper function hiclose called by hclose
 *                                                                                    EIP 2007-09-14
 */
#define nhiopen       H4_F77_FUNC(hiopen, HIOPEN)
#define nhclose       H4_F77_FUNC(hclose, HCLOSE)
#define nhnumber      H4_F77_FUNC(hnumber, HNUMBER)
#define nhxisdir      H4_F77_FUNC(hxisdir, HXISDIR)
#define nhxiscdir     H4_F77_FUNC(hxiscdir, HXISCDIR)
#define nhddontatexit H4_F77_FUNC(hddontatexit, HDDONTATEXIT)
#define nhglibverc    H4_F77_FUNC(hglibverc, HGLIBVERC)
#define nhgfilverc    H4_F77_FUNC(hgfilverc, HGFILVERC)
#define nhiishdf      H4_F77_FUNC(hiishdf, HIISHDF)
#define nhconfinfc    H4_F77_FUNC(hconfinfc, HCONFINFC)

HDFFCLIBAPI intf nhiopen(_fcd name, intf *acc_mode, intf *defdds, intf *namelen);

HDFFCLIBAPI intf nhclose(intf *file_id);

HDFFCLIBAPI intf nhnumber(intf *file_id, intf *tag);

HDFFCLIBAPI intf nhxisdir(_fcd dir, intf *dirlen);

HDFFCLIBAPI intf nhxiscdir(_fcd dir, intf *dirlen);

HDFFCLIBAPI intf nhddontatexit(void);

HDFFCLIBAPI intf nhglibverc(intf *major_v, intf *minor_v, intf *release, _fcd string, intf *len);

HDFFCLIBAPI intf nhgfilverc(intf *file_id, intf *major_v, intf *minor_v, intf *release, _fcd string,
                            intf *len);

HDFFCLIBAPI intf nhiishdf(_fcd name, intf *namelen);

HDFFCLIBAPI intf nhiclose(intf *file_id);

HDFFCLIBAPI intf nhinumbr(int32 file_id, uint16 tag);

HDFFCLIBAPI intf nhconfinfc(intf *coder_type, intf *info);
/*
 ** from dfufp2i.c
 */
#define nduif2i H4_F77_FUNC(duif2i, DUIF2I)

HDFFCLIBAPI int nduif2i(int32 *hdim, int32 *vdim, float32 *max, float32 *min, float32 hscale[],
                        float32 vscale[], float32 data[], _fcd palette, _fcd outfile, int *ct_method,
                        int32 *hres, int32 *vres, int *compress, int *lenfn);

HDFFCLIBAPI int DFUfptoimage(int32 hdim, int32 vdim, float32 max, float32 min, float32 *hscale,
                             float32 *vscale, float32 *data, uint8 *palette, char *outfile, int ct_method,
                             int32 hres, int32 vres, int compress);

/* for Multi-file fortran Annotation interface */

/*
 *  Not all HDF4 Fortran functions are real Fortran functions, they are C wrappers called from a
 *  Fortran program. Since on Windows we have to have a real Fortran function, additional C wrappers
 *  were added at the time when Windows port was done for multi-file interfaces: for example,
 *  hclose is a C wrapper on UNIX, while on Windows we have two functions: Fortran function hclose
 *  and C wrapper function hiclose called by hclose
 *                                                                                    EIP 2007-09-14
 */
#define nafstart     H4_F77_FUNC(afstart, AFSTART)
#define naffileinfo  H4_F77_FUNC(affileinfo, AFFILEINFO)
#define nafend       H4_F77_FUNC(afend, AFEND)
#define nafcreate    H4_F77_FUNC(afcreate, AFCREATE)
#define naffcreate   H4_F77_FUNC(affcreate, AFFCREATE)
#define nafselect    H4_F77_FUNC(afselect, AFSELECT)
#define nafnumann    H4_F77_FUNC(afnumann, AFNUMANN)
#define nafannlist   H4_F77_FUNC(afannlist, AFANNLIST)
#define nafannlen    H4_F77_FUNC(afannlen, AFANNLEN)
#define nafwriteann  H4_F77_FUNC(afwriteann, AFWRITEANN)
#define nafreadann   H4_F77_FUNC(afreadann, AFREADANN)
#define nafendaccess H4_F77_FUNC(afendaccess, AFENDACCESS)
#define nafgettagref H4_F77_FUNC(afgettagref, AFGETTAGREF)
#define nafidtagref  H4_F77_FUNC(afidtagref, AFIDTAGREF)
#define naftagrefid  H4_F77_FUNC(aftagrefid, AFTAGREFID)
#define nafatypetag  H4_F77_FUNC(afatypetag, AFATYPETAG)
#define naftagatype  H4_F77_FUNC(aftagatype, AFTAGATYPE)

/* Multi-file Annotation C-stubs for Fortran interface found in mfanf.c */

HDFFCLIBAPI intf nafstart(intf *file_id);

HDFFCLIBAPI intf naffileinfo(intf *an_id, intf *num_flabel, intf *num_fdesc, intf *num_olabel,
                             intf *num_odesc);

HDFFCLIBAPI intf nafend(intf *an_id);

HDFFCLIBAPI intf nafcreate(intf *an_id, intf *etag, intf *eref, intf *atype);

HDFFCLIBAPI intf naffcreate(intf *an_id, intf *atype);

HDFFCLIBAPI intf nafselect(intf *an_id, intf *idx, intf *atype);

HDFFCLIBAPI intf nafnumann(intf *an_id, intf *atype, intf *etag, intf *eref);

HDFFCLIBAPI intf nafannlist(intf *an_id, intf *atype, intf *etag, intf *eref, intf alist[]);

HDFFCLIBAPI intf nafannlen(intf *ann_id);

HDFFCLIBAPI intf nafwriteann(intf *ann_id, _fcd ann, intf *annlen);

HDFFCLIBAPI intf nafreadann(intf *ann_id, _fcd ann, intf *maxlen);

HDFFCLIBAPI intf nafendaccess(intf *ann_id);

HDFFCLIBAPI intf nafgettagref(intf *an_id, intf *idx, intf *type, intf *tag, intf *ref);

HDFFCLIBAPI intf nafidtagref(intf *ann_id, intf *tag, intf *ref);

HDFFCLIBAPI intf naftagrefid(intf *an_id, intf *tag, intf *ref);

HDFFCLIBAPI intf nafatypetag(intf *atype);

HDFFCLIBAPI intf naftagatype(intf *tag);

/* if defined Windows */
/* Multi-file Annotation C-stubs for Fortran interface found in mfanf.c */

HDFFCLIBAPI intf nafistart(intf *file_id);

HDFFCLIBAPI intf nafifinf(intf *an_id, intf *num_flabel, intf *num_fdesc, intf *num_olabel, intf *num_odesc);

HDFFCLIBAPI intf nafiend(intf *an_id);

HDFFCLIBAPI intf naficreat(intf *an_id, intf *etag, intf *eref, intf *atype);

HDFFCLIBAPI intf nafifcreat(intf *an_id, intf *atype);

HDFFCLIBAPI intf nafiselct(intf *an_id, intf *index, intf *atype);

HDFFCLIBAPI intf nafinann(intf *an_id, intf *atype, intf *etag, intf *eref);

HDFFCLIBAPI intf nafialst(intf *an_id, intf *atype, intf *etag, intf *eref, intf alist[]);

HDFFCLIBAPI intf nafialen(intf *ann_id);

HDFFCLIBAPI intf nafiwann(intf *ann_id, _fcd ann, intf *annlen);

HDFFCLIBAPI intf nafirann(intf *ann_id, _fcd ann, intf *maxlen);

HDFFCLIBAPI intf nafiendac(intf *ann_id);

HDFFCLIBAPI intf nafigtr(intf *an_id, intf *index, intf *type, intf *tag, intf *ref);

HDFFCLIBAPI intf nafiid2tr(intf *ann_id, intf *tag, intf *ref);

HDFFCLIBAPI intf nafitr2id(intf *an_id, intf *tag, intf *ref);

HDFFCLIBAPI intf nafitp2tg(intf *atype);

HDFFCLIBAPI intf nafitg2tp(intf *tag);

/* endif defined Windows */

/* Multi-file Annotation C-routines found in mfan.c */
HDFLIBAPI int32 ANstart(int32 file_id);

HDFLIBAPI intn ANfileinfo(int32 an_id, int32 *n_file_label, int32 *n_file_desc, int32 *n_obj_label,
                          int32 *n_obj_desc);

HDFLIBAPI int32 ANend(int32 an_id);

HDFLIBAPI int32 ANcreate(int32 an_id, uint16 elem_tag, uint16 elem_ref, ann_type type);

HDFLIBAPI int32 ANcreatef(int32 an_id, ann_type type);

HDFLIBAPI int32 ANselect(int32 an_id, int32 idx, ann_type type);

HDFLIBAPI intn ANnumann(int32 an_id, ann_type type, uint16 elem_tag, uint16 elem_ref);

HDFLIBAPI intn ANannlist(int32 an_id, ann_type type, uint16 elem_tag, uint16 elem_ref, int32 ann_list[]);

HDFLIBAPI int32 ANannlen(int32 ann_id);

HDFLIBAPI int32 ANwriteann(int32 ann_id, const char *ann, int32 annlen);

HDFLIBAPI int32 ANreadann(int32 ann_id, char *ann, int32 maxlen);

HDFLIBAPI intn ANendaccess(int32 ann_id);

HDFLIBAPI int32 ANget_tagref(int32 an_id, int32 idx, ann_type type, uint16 *ann_tag, uint16 *ann_ref);

HDFLIBAPI int32 ANid2tagref(int32 an_id, uint16 *ann_tag, uint16 *ann_ref);

HDFLIBAPI int32 ANtagref2id(int32 an_id, uint16 ann_tag, uint16 ann_ref);

HDFLIBAPI uint16 ANatype2tag(ann_type atype);

HDFLIBAPI ann_type ANtag2atype(uint16 atag);

/* Removed because this function is meant to be private.
HDFLIBAPI intn ANdestroy(void); */

/* for Multi-file fortran GR interface */
/*
 *  Not all HDF4 Fortran functions are real Fortran functions, they are C wrappers called from a
 *  Fortran program. Since on Windows we have to have a real Fortran function, additional C wrappers
 *  were added at the time when Windows port was done for multi-file interfaces: for example,
 *  hclose is a C wrapper on UNIX, while on Windows we have two functions: Fortran function hclose
 *  and C wrapper function hiclose called by hclose
 *                                                                                    EIP 2007-09-14
 */
#define nmgstart      H4_F77_FUNC(mgstart, MGSTART)
#define nmgfinfo      H4_F77_FUNC(mgfinfo, MGFINFO)
#define nmgend        H4_F77_FUNC(mgend, MGEND)
#define nmgicreat     H4_F77_FUNC(mgicreat, MGICREAT)
#define nmgselct      H4_F77_FUNC(mgselct, MGSELCT)
#define nmgin2ndx     H4_F77_FUNC(mgin2ndx, MGIN2NDX)
#define nmggiinf      H4_F77_FUNC(mggiinf, MGGIINF)
#define nmgwcimg      H4_F77_FUNC(mgwcimg, MGWCIMG)
#define nmgrcimg      H4_F77_FUNC(mgrcimg, MGRCIMG)
#define nmgwrimg      H4_F77_FUNC(mgwrimg, MGWRIMG)
#define nmgrdimg      H4_F77_FUNC(mgrdimg, MGRDIMG)
#define nmgendac      H4_F77_FUNC(mgendac, MGENDAC)
#define nmgid2rf      H4_F77_FUNC(mgid2rf, MGID2RF)
#define nmgr2idx      H4_F77_FUNC(mgr2idx, MGR2IDX)
#define nmgrltil      H4_F77_FUNC(mgrltil, MGRLTIL)
#define nmgrimil      H4_F77_FUNC(mgrimil, MGRIMIL)
#define nmggltid      H4_F77_FUNC(mggltid, MGGLTID)
#define nmgglinf      H4_F77_FUNC(mgglinf, MGGLINF)
#define nmgwrlut      H4_F77_FUNC(mgwrlut, MGWRLUT)
#define nmgwclut      H4_F77_FUNC(mgwclut, MGWCLUT)
#define nmgrdlut      H4_F77_FUNC(mgrdlut, MGRDLUT)
#define nmgrclut      H4_F77_FUNC(mgrclut, MGRCLUT)
#define nmgisxfil     H4_F77_FUNC(mgisxfil, MGISXFIL)
#define nmgiscatt     H4_F77_FUNC(mgiscatt, MGISCATT)
#define nmgisattr     H4_F77_FUNC(mgisattr, MGISATTR)
#define nmgatinf      H4_F77_FUNC(mgatinf, MGATINF)
#define nmggcatt      H4_F77_FUNC(mggcatt, MGGCATT)
#define nmggnatt      H4_F77_FUNC(mggnatt, MGGNATT)
#define nmggattr      H4_F77_FUNC(mggattr, MGGATTR)
#define nmgifndat     H4_F77_FUNC(mgifndat, MGIFNDAT)
#define nmgcgichnk    H4_F77_FUNC(mgcgichnk, MGCGICHNK)
#define nmgcrcchnk    H4_F77_FUNC(mgcrcchnk, MGCRCCHNK)
#define nmgcrchnk     H4_F77_FUNC(mgcrchnk, MGCRCHNK)
#define nmgcscchnk    H4_F77_FUNC(mgcscchnk, MGCSCCHNK)
#define nmgcschnk     H4_F77_FUNC(mgcschnk, MGCSCHNK)
#define nmgcwcchnk    H4_F77_FUNC(mgcwcchnk, MGCWCCHNK)
#define nmgcwchnk     H4_F77_FUNC(mgcwchnk, MGCWCHNK)
#define nmgcscompress H4_F77_FUNC(mgcscompress, MGCSCOMPRESS)
#define nmgcgcompress H4_F77_FUNC(mgcgcompress, MGCGCOMPRESS)
#define nmglt2rf      H4_F77_FUNC(mglt2rf, MGLT2RF)
#define nmgcgnluts    H4_F77_FUNC(mgcgnluts, MGCGNLUTS)

/* Multi-file GR C-stubs for FORTRAN interface found in mfgrf.c */

HDFFCLIBAPI intf /* !sl */
nmgiwimg(intf *riid, intf *start, intf *stride, intf *count, void *data);

HDFFCLIBAPI intf /* !sl */
nmgirimg(intf *riid, intf *start, intf *stride, intf *count, void *data);

HDFFCLIBAPI intf /* !sl */
nmgignat(intf *riid, intf *idx, void *data);

HDFFCLIBAPI intf nmgstart(intf *fid);

HDFFCLIBAPI intf nmgfinfo(intf *grid, intf *n_datasets, intf *n_attrs);

HDFFCLIBAPI intf nmgend(intf *grid);

HDFFCLIBAPI intf nmgicreat(intf *grid, _fcd name, intf *ncomp, intf *nt, intf *il, intf dimsizes[2],
                           intf *nlen);

HDFFCLIBAPI intf nmgselct(intf *grid, intf *idx);

HDFFCLIBAPI intf nmgin2ndx(intf *grid, _fcd name, intf *nlen);

HDFFCLIBAPI intf nmggiinf(intf *riid, _fcd name, intf *ncomp, intf *nt, intf *il, intf *dimsizes,
                          intf *nattr);

HDFFCLIBAPI intf nmgwcimg(intf *riid, intf *start, intf *stride, intf *count, _fcd data);

HDFFCLIBAPI intf nmgrcimg(intf *riid, intf *start, intf *stride, intf *count, _fcd data);

HDFFCLIBAPI intf nmgwrimg(intf *riid, intf *start, intf *stride, intf *count, void *data);

HDFFCLIBAPI intf nmgrdimg(intf *riid, intf *start, intf *stride, intf *count, void *data);

HDFFCLIBAPI intf nmgendac(intf *riid);

HDFFCLIBAPI intf nmgid2rf(intf *riid);

HDFFCLIBAPI intf nmgr2idx(intf *grid, intf *ref);

HDFFCLIBAPI intf nmgrltil(intf *riid, intf *il);

HDFFCLIBAPI intf nmgrimil(intf *riid, intf *il);

HDFFCLIBAPI intf nmggltid(intf *riid, intf *lut_index);

HDFFCLIBAPI intf nmgglinf(intf *lutid, intf *ncomp, intf *nt, intf *il, intf *nentries);

HDFFCLIBAPI intf nmgwrlut(intf *lutid, intf *ncomp, intf *nt, intf *il, intf *nentries, void *data);

HDFFCLIBAPI intf nmgwclut(intf *lutid, intf *ncomp, intf *nt, intf *il, intf *nentries, _fcd data);

HDFFCLIBAPI intf nmgrdlut(intf *lutid, void *data);

HDFFCLIBAPI intf nmgrclut(intf *lutid, _fcd data);

HDFFCLIBAPI intf nmgisxfil(intf *riid, _fcd filename, intf *offset, intf *nlen);

HDFFCLIBAPI intf nmgsactp(intf *riid, intf *accesstype);

HDFFCLIBAPI intf nmgiscatt(intf *riid, _fcd name, intf *nt, intf *count, _fcd data, intf *nlen);

HDFFCLIBAPI intf nmgisattr(intf *riid, _fcd name, intf *nt, intf *count, void *data, intf *nlen);

HDFFCLIBAPI intf nmgatinf(intf *riid, intf *idx, _fcd name, intf *nt, intf *count);

HDFFCLIBAPI intf nmggcatt(intf *riid, intf *idx, _fcd data);

HDFFCLIBAPI intf nmggnatt(intf *riid, intf *idx, void *data);

HDFFCLIBAPI intf nmggattr(intf *riid, intf *idx, void *data);

HDFFCLIBAPI intf nmgifndat(intf *riid, _fcd name, intf *nlen);

HDFFCLIBAPI intf nmgcgichnk(intf *id, intf *dim_length, intf *flags);

HDFFCLIBAPI intf nmgcrcchnk(intf *id, intf *start, _fcd char_data);

HDFFCLIBAPI intf nmgcrchnk(intf *id, intf *start, void *num_data);

HDFFCLIBAPI intf nmgcscchnk(intf *id, intf *maxcache, intf *flags);

HDFFCLIBAPI intf nmgcschnk(intf *id, intf *dim_length, intf *comp_type, intf *comp_prm);
HDFFCLIBAPI intf nmgcwcchnk(intf *id, intf *start, _fcd char_data);

HDFFCLIBAPI intf nmgcwchnk(intf *id, intf *start, void *num_data);

HDFFCLIBAPI intf nmgcscompress(intf *id, intf *comp_type, intf *comp_prm);

HDFFCLIBAPI intf nmgcgcompress(intf *id, intf *comp_type, intf *comp_prm);

HDFFCLIBAPI intf nmglt2rf(intf *id);

HDFFCLIBAPI intf nmgcgnluts(intf *id);

/*
 ** from vgF.c
 */
/*
 *  Not all HDF4 Fortran functions are real Fortran functions, they are C wrappers called from a
 *  Fortran program. Since on Windows we have to have a real Fortran function, additional C wrappers
 *  were added at the time when Windows port was done for multi-file interfaces: for example,
 *  hclose is a C wrapper on UNIX, while on Windows we have two functions: Fortran function hclose
 *  and C wrapper function hiclose called by hclose
 *                                                                                    EIP 2007-09-14
 */
#define ndfivopn    H4_F77_FUNC(dfivopn, DFIVOPN)
#define ndfvclos    H4_F77_FUNC(dfvclos, DFVCLOS)
#define nvatchc     H4_F77_FUNC(vatchc, VATCHC)
#define nvdtchc     H4_F77_FUNC(vdtchc, VDTCHC)
#define nvgnamc     H4_F77_FUNC(vgnamc, VGNAMC)
#define nvgclsc     H4_F77_FUNC(vgclsc, VGCLSC)
#define nvinqc      H4_F77_FUNC(vinqc, VINQC)
#define nvdeletec   H4_F77_FUNC(vdeletec, VDELETEC)
#define nvgidc      H4_F77_FUNC(vgidc, VGIDC)
#define nvgnxtc     H4_F77_FUNC(vgnxtc, VGNXTC)
#define nvsnamc     H4_F77_FUNC(vsnamc, VSNAMC)
#define nvsclsc     H4_F77_FUNC(vsclsc, VSCLSC)
#define nvinsrtc    H4_F77_FUNC(vinsrtc, VINSRTC)
#define nvisvgc     H4_F77_FUNC(visvgc, VISVGC)
#define nvisvsc     H4_F77_FUNC(visvsc, VISVSC)
#define nvsatchc    H4_F77_FUNC(vsatchc, VSATCHC)
#define nvsdtchc    H4_F77_FUNC(vsdtchc, VSDTCHC)
#define nvsqref     H4_F77_FUNC(vsqref, VSQREF)
#define nvsqtag     H4_F77_FUNC(vsqtag, VSQTAG)
#define nvsgver     H4_F77_FUNC(vsgver, VSGVER)
#define nvsseekc    H4_F77_FUNC(vsseekc, VSSEEKC)
#define nvsgnamc    H4_F77_FUNC(vsgnamc, VSGNAMC)
#define nvsgclsc    H4_F77_FUNC(vsgclsc, VSGCLSC)
#define nvsinqc     H4_F77_FUNC(vsinqc, VSINQC)
#define nvsfexc     H4_F77_FUNC(vsfexc, VSFEXC)
#define nvsfndc     H4_F77_FUNC(vsfndc, VSFNDC)
#define nvsgidc     H4_F77_FUNC(vsgidc, VSGIDC)
#define nvsdltc     H4_F77_FUNC(vsdltc, VSDLTC)
#define nvsapp      H4_F77_FUNC(vsapp, VSAPP)
#define nvssnamc    H4_F77_FUNC(vssnamc, VSSNAMC)
#define nvssclsc    H4_F77_FUNC(vssclsc, VSSCLSC)
#define nvssfldc    H4_F77_FUNC(vssfldc, VSSFLDC)
#define nvssintc    H4_F77_FUNC(vssintc, VSSINTC)
#define nvsfdefc    H4_F77_FUNC(vsfdefc, VSFDEFC)
#define nvssextfc   H4_F77_FUNC(vssextfc, VSSEXTFC)
#define nvfnflds    H4_F77_FUNC(vfnflds, VFNFLDS)
#define nvffnamec   H4_F77_FUNC(vffnamec, VFFNAMEC)
#define nvfftype    H4_F77_FUNC(vfftype, VFFTYPE)
#define nvffisiz    H4_F77_FUNC(vffisiz, VFFISIZ)
#define nvffesiz    H4_F77_FUNC(vffesiz, VFFESIZ)
#define nvffordr    H4_F77_FUNC(vffordr, VFFORDR)
#define nvsfrdc     H4_F77_FUNC(vsfrdc, VSFRDC)
#define nvsfrd      H4_F77_FUNC(vsfrd, VSFRD)
#define nvsreadc    H4_F77_FUNC(vsreadc, VSREADC)
#define nvsfwrt     H4_F77_FUNC(vsfwrt, VSFWRT)
#define nvsfwrtc    H4_F77_FUNC(vsfwrtc, VSFWRTC)
#define nvswritc    H4_F77_FUNC(vswritc, VSWRITC)
#define nvsgintc    H4_F77_FUNC(vsgintc, VSGINTC)
#define nvseltsc    H4_F77_FUNC(vseltsc, VSELTSC)
#define nvsgfldc    H4_F77_FUNC(vsgfldc, VSGFLDC)
#define nvssizc     H4_F77_FUNC(vssizc, VSSIZC)
#define nventsc     H4_F77_FUNC(ventsc, VENTSC)
#define nvlonec     H4_F77_FUNC(vlonec, VLONEC)
#define nvslonec    H4_F77_FUNC(vslonec, VSLONEC)
#define nvfindc     H4_F77_FUNC(vfindc, VFINDC)
#define nvfndclsc   H4_F77_FUNC(vfndclsc, VFNDCLSC)
#define nvhscdc     H4_F77_FUNC(vhscdc, VHSCDC)
#define nvhsdc      H4_F77_FUNC(vhsdc, VHSDC)
#define nvhscdmc    H4_F77_FUNC(vhscdmc, VHSCDMC)
#define nvhsdmc     H4_F77_FUNC(vhsdmc, VHSDMC)
#define nvhmkgpc    H4_F77_FUNC(vhmkgpc, VHMKGPC)
#define nvflocc     H4_F77_FUNC(vflocc, VFLOCC)
#define nvinqtrc    H4_F77_FUNC(vinqtrc, VINQTRC)
#define nvntrc      H4_F77_FUNC(vntrc, VNTRC)
#define nvnrefs     H4_F77_FUNC(vnrefs, VNREFS)
#define nvgttrsc    H4_F77_FUNC(vgttrsc, VGTTRSC)
#define nvqref      H4_F77_FUNC(vqref, VQREF)
#define nvqtag      H4_F77_FUNC(vqtag, VQTAG)
#define nvgttrc     H4_F77_FUNC(vgttrc, VGTTRC)
#define nvadtrc     H4_F77_FUNC(vadtrc, VADTRC)
#define nvfstart    H4_F77_FUNC(vfstart, VFSTART)
#define nvfend      H4_F77_FUNC(vfend, VFEND)
#define nvsqfnelt   H4_F77_FUNC(vsqfnelt, VSQFNELT)
#define nvsqfintr   H4_F77_FUNC(vsqfintr, VSQFINTR)
#define nvsqfldsc   H4_F77_FUNC(vsqfldsc, VSQFLDSC)
#define nvsqfvsiz   H4_F77_FUNC(vsqfvsiz, VSQFVSIZ)
#define nvsqnamec   H4_F77_FUNC(vsqnamec, VSQNAMEC)
#define nvsfccpk    H4_F77_FUNC(vsfccpk, VSFCCPK)
#define nvsfncpk    H4_F77_FUNC(vsfncpk, VSFNCPK)
#define nvdtrc      H4_F77_FUNC(vdtrc, VDTRC)
#define nvscfcls    H4_F77_FUNC(vscfcls, VSCFCLS)
#define nvscsetblsz H4_F77_FUNC(vscsetblsz, VSCSETBLSZ)
#define nvscsetnmbl H4_F77_FUNC(vscsetnmbl, VSCSETNMBL)
#define nvscgblinfo H4_F77_FUNC(vscgblinfo, VSCGBLINFO)
#define nvcgvgrp    H4_F77_FUNC(vcgvgrp, VCGVGRP)
#define nvscgvdatas H4_F77_FUNC(vscgvdatas, VSCGVDATAS)

HDFFCLIBAPI intf ndfivopn(_fcd filename, intf *acc_mode, intf *defdds, intf *namelen);

HDFFCLIBAPI intf ndfvclos(intf *file_id);

HDFFCLIBAPI intf nvatchc(intf *f, intf *vgid, _fcd accesstype);

HDFFCLIBAPI intf nvdtchc(intf *vkey);

HDFFCLIBAPI intf nvgnamc(intf *vkey, _fcd vgname);

HDFFCLIBAPI intf nvgclsc(intf *vkey, _fcd vgclass);

HDFFCLIBAPI intf nvinqc(intf *vkey, intf *nentries, _fcd vgname);

HDFFCLIBAPI intf nvdeletec(intf *f, intf *vkey);

HDFFCLIBAPI intf nvgidc(intf *f, intf *vgid);

HDFFCLIBAPI intf nvgnxtc(intf *vkey, intf *id);

HDFFCLIBAPI intf nvsnamc(intf *vkey, _fcd vgname, intf *vgnamelen);

HDFFCLIBAPI intf nvsclsc(intf *vkey, _fcd vgclass, intf *vgclasslen);

HDFFCLIBAPI intf nvinsrtc(intf *vkey, intf *vobjptr);

HDFFCLIBAPI intf nvisvgc(intf *vkey, intf *id);

HDFFCLIBAPI intf nvfstart(intf *f);

HDFFCLIBAPI intf nvfend(intf *f);

HDFFCLIBAPI intf nvisvsc(intf *vkey, intf *id);

HDFFCLIBAPI intf nvsatchc(intf *f, intf *vsref, _fcd accesstype);

HDFFCLIBAPI intf nvsdtchc(intf *vkey);

HDFFCLIBAPI intf nvsqref(intf *vkey);

HDFFCLIBAPI intf nvsqtag(intf *vkey);

HDFFCLIBAPI intf nvsgver(intf *vkey);

HDFFCLIBAPI intf nvsseekc(intf *vkey, intf *eltpos);

HDFFCLIBAPI intf nvsgnamc(intf *vkey, _fcd vsname, intf *vsnamelen);

HDFFCLIBAPI intf nvsgclsc(intf *vkey, _fcd vsclass, intf *vsclasslen);

HDFFCLIBAPI intf nvsinqc(intf *vkey, intf *nelt, intf *interlace, _fcd fields, intf *eltsize, _fcd vsname,
                         intf *fieldslen, intf *vsnamelen);

HDFFCLIBAPI intf nvsfexc(intf *vkey, _fcd fields, intf *fieldslen);

HDFFCLIBAPI intf nvsfndc(intf *f, _fcd name, intf *namelen);

HDFFCLIBAPI intf nvsgidc(intf *f, intf *vsref);

HDFFCLIBAPI intf nvsdltc(intf *f, intf *vsref);

HDFFCLIBAPI intf nvsapp(intf *vkey, intf *blk);

HDFFCLIBAPI intf nvssnamc(intf *vkey, _fcd vsname, intf *vsnamelen);

HDFFCLIBAPI intf nvssclsc(intf *vkey, _fcd vsclass, intf *vsclasslen);

HDFFCLIBAPI intf nvssfldc(intf *vkey, _fcd fields, intf *fieldslen);

HDFFCLIBAPI intf nvssintc(intf *vkey, intf *interlace);

HDFFCLIBAPI intf nvsfdefc(intf *vkey, _fcd field, intf *localtype, intf *order, intf *fieldlen);

HDFFCLIBAPI intf nvssextfc(intf *vkey, _fcd fname, intf *offset, intf *fnamelen);

HDFFCLIBAPI intf nvfnflds(intf *vkey);

HDFFCLIBAPI intf nvffnamec(intf *vkey, intf *idx, _fcd fname, intf *len);

HDFFCLIBAPI intf nvfftype(intf *vkey, intf *idx);

HDFFCLIBAPI intf nvffisiz(intf *vkey, intf *idx);

HDFFCLIBAPI intf nvffesiz(intf *vkey, intf *idx);

HDFFCLIBAPI intf nvffordr(intf *vkey, intf *idx);

HDFFCLIBAPI intf nvsfrdc(intf *vkey, _fcd cbuf, intf *nelt, intf *interlace);

HDFFCLIBAPI intf nvsfrd(intf *vkey, intf *buf, intf *nelt, intf *interlace);

HDFFCLIBAPI intf nvsreadc(intf *vkey, uint8 *buf, intf *nelt, intf *interlace);

HDFFCLIBAPI intf nvsfwrtc(intf *vkey, _fcd cbuf, intf *nelt, intf *interlace);

HDFFCLIBAPI intf nvsfwrt(intf *vkey, intf *buf, intf *nelt, intf *interlace);

HDFFCLIBAPI intf nvswritc(intf *vkey, uint8 *buf, intf *nelt, intf *interlace);

HDFFCLIBAPI intf nvsgintc(intf *vkey);

HDFFCLIBAPI intf nvseltsc(intf *vkey);

HDFFCLIBAPI intf nvsgfldc(intf *vkey, _fcd fields);

HDFFCLIBAPI intf nvssizc(intf *vkey, _fcd fields, intf *fieldslen);

HDFFCLIBAPI intf nventsc(intf *f, intf *vgid);

HDFFCLIBAPI intf nvlonec(intf *f, intf *idarray, intf *asize);

HDFFCLIBAPI intf nvslonec(intf *f, intf *idarray, intf *asize);

HDFFCLIBAPI intf nvfindc(intf *f, _fcd name, intf *namelen);

HDFFCLIBAPI intf nvfndclsc(intf *f, _fcd vgclass, intf *classlen);

HDFFCLIBAPI intf nvhscdc(intf *f, _fcd field, _fcd cbuf, intf *n, intf *datatype, _fcd vsname, _fcd vsclass,
                         intf *fieldlen, intf *vsnamelen, intf *vsclasslen);

HDFFCLIBAPI intf nvhsdc(intf *f, _fcd field, uint8 *buf, intf *n, intf *datatype, _fcd vsname, _fcd vsclass,
                        intf *fieldlen, intf *vsnamelen, intf *vsclasslen);

HDFFCLIBAPI intf nvhscdmc(intf *f, _fcd field, _fcd cbuf, intf *n, intf *datatype, _fcd vsname, _fcd vsclass,
                          intf *order, intf *fieldlen, intf *vsnamelen, intf *vsclasslen);

HDFFCLIBAPI intf nvhsdmc(intf *f, _fcd field, uint8 *buf, intf *n, intf *datatype, _fcd vsname, _fcd vsclass,
                         intf *order, intf *fieldlen, intf *vsnamelen, intf *vsclasslen);

HDFFCLIBAPI intf nvhmkgpc(intf *f, intf *tagarray, intf *refarray, intf *n, _fcd vgname, _fcd vgclass,
                          intf *vgnamelen, intf *vgclasslen);

HDFFCLIBAPI intf nvflocc(intf *vkey, _fcd field, intf *fieldlen);

HDFFCLIBAPI intf nvinqtrc(intf *vkey, intf *tag, intf *ref);

HDFFCLIBAPI intf nvntrc(intf *vkey);

HDFFCLIBAPI intf nvnrefs(intf *vkey, intf *tag);

HDFFCLIBAPI intf nvqref(intf *vkey);

HDFFCLIBAPI intf nvqtag(intf *vkey);

HDFFCLIBAPI intf nvgttrsc(intf *vkey, intf *tagarray, intf *refarray, intf *n);

HDFFCLIBAPI intf nvgttrc(intf *vkey, intf *which, intf *tag, intf *ref);

HDFFCLIBAPI intf nvadtrc(intf *vkey, intf *tag, intf *ref);

HDFFCLIBAPI intf nvdtrc(intf *vkey, intf *tag, intf *ref);

HDFFCLIBAPI intf nvsqfnelt(intf *vkey, intf *nelt);

HDFFCLIBAPI intf nvsqfintr(intf *vkey, intf *interlace);

HDFFCLIBAPI intf nvsqfldsc(intf *vkey, _fcd fields, intf *fieldslen);

HDFFCLIBAPI intf nvsqfvsiz(intf *vkey, intf *size);

HDFFCLIBAPI intf nvsqnamec(intf *vkey, _fcd name, intf *namelen);

HDFFCLIBAPI intf nvsfccpk(intf *vs, intf *packtype, _fcd buflds, intf *buf, intf *bufsz, intf *nrecs,
                          _fcd pckfld, _fcd fldbuf, intf *buflds_len, intf *fld_len);

HDFFCLIBAPI intf nvsfncpk(intf *vs, intf *packtype, _fcd buflds, intf *buf, intf *bufsz, intf *nrecs,
                          _fcd pckfld, intf *fldbuf, intf *buflds_len, intf *fld_len);

HDFFCLIBAPI intf nvscsetblsz(intf *id, intf *block_size);

HDFFCLIBAPI intf nvscsetnmbl(intf *id, intf *num_blocks);

HDFFCLIBAPI intf nvscgblinfo(intf *id, intf *block_size, intf *num_blocks);

HDFFCLIBAPI intf nvcgvgrp(intf *id, intf *start_vg, intf *vg_count, intf *refarray);

HDFFCLIBAPI intf nvscgvdatas(intf *id, intf *start_vd, intf *vd_count, intf *refarray);

HDFFCLIBAPI intf nvscfcls(intf *id, _fcd name, intf *namelen);

HDFFCLIBAPI intf nvfistart(intf *f);

HDFFCLIBAPI intf nvfiend(intf *f);

HDFFCLIBAPI intf nvsiqref(intf *vkey);

HDFFCLIBAPI intf nvsiqtag(intf *vkey);

HDFFCLIBAPI intf nvsigver(intf *vkey);

HDFFCLIBAPI intf nvfinflds(intf *vkey);

HDFFCLIBAPI intf nvfifnm(intf *vkey, intf *index, _fcd fname);

HDFFCLIBAPI intf nvfiftp(intf *vkey, intf *index);

HDFFCLIBAPI intf nvfifisz(intf *vkey, intf *index);
HDFFCLIBAPI intf nvfifesz(intf *vkey, intf *index);
HDFFCLIBAPI intf nvfifodr(intf *vkey, intf *index);

HDFFCLIBAPI intf nvsfirdc(intf *vkey, _fcd cbuf, intf *nelt, intf *interlace);

HDFFCLIBAPI intf nvsfird(intf *vkey, intf *buf, intf *nelt, intf *interlace);

HDFFCLIBAPI intf nvsfiwrc(intf *vkey, _fcd cbuf, intf *nelt, intf *interlace);

HDFFCLIBAPI intf nvsfiwr(intf *vkey, intf *buf, intf *nelt, intf *interlace);

HDFFCLIBAPI intf nvfirefs(intf *vkey, intf *tag);

HDFFCLIBAPI intf nvfiqref(intf *vkey);

HDFFCLIBAPI intf nvfiqtag(intf *vkey);

HDFFCLIBAPI intf nvsiqnelt(intf *vkey, intf *nelt);

HDFFCLIBAPI intf nvsiqintr(intf *vkey, intf *interlace);

HDFFCLIBAPI intf nvsqfldsc(intf *vkey, _fcd fields, intf *fieldslen);

HDFFCLIBAPI intf nvsiqvsz(intf *vkey, intf *ret_size);

/*
 ** from vattrf.c
 */
/*
 *  Not all HDF4 Fortran functions are real Fortran functions, they are C wrappers called from a
 *  Fortran program. Since on Windows we have to have a real Fortran function, additional C wrappers
 *  were added at the time when Windows port was done for multi-file interfaces: for example,
 *  hclose is a C wrapper on UNIX, while on Windows we have two functions: Fortran function hclose
 *  and C wrapper function hiclose called by hclose
 *                                                                                    EIP 2007-09-14
 */
#define nvsfcfdx H4_F77_FUNC(vsfcfdx, VSFCFDX)
#define nvsfcsat H4_F77_FUNC(vsfcsat, VSFCSAT)
#define nvsfcsca H4_F77_FUNC(vsfcsca, VSFCSCA)
#define nvsfnats H4_F77_FUNC(vsfnats, VSFNATS)
#define nvsffnas H4_F77_FUNC(vsffnas, VSFFNAS)
#define nvsfcfda H4_F77_FUNC(vsfcfda, VSFCFDA)
#define nvsfcain H4_F77_FUNC(vsfcain, VSFCAIN)
#define nvsfgnat H4_F77_FUNC(vsfgnat, VSFGNAT)
#define nvsfgcat H4_F77_FUNC(vsfgcat, VSFGCAT)
#define nvsfisat H4_F77_FUNC(vsfisat, VSFISAT)
#define nvfcsatt H4_F77_FUNC(vfcsatt, VFCSATT)
#define nvfcscat H4_F77_FUNC(vfcscat, VFCSCAT)
#define nvfnatts H4_F77_FUNC(vfnatts, VFNATTS)
#define nvfcfdat H4_F77_FUNC(vfcfdat, VFCFDAT)
#define nvfainfo H4_F77_FUNC(vfainfo, VFAINFO)
#define nvfgnatt H4_F77_FUNC(vfgnatt, VFGNATT)
#define nvfgcatt H4_F77_FUNC(vfgcatt, VFGCATT)
#define nvfgver  H4_F77_FUNC(vfgver, VFGVER)

HDFFCLIBAPI intf nvsfcfdx(intf *vsid, _fcd fldnm, intf *findex, intf *fldnmlen);
HDFFCLIBAPI intf nvsfcsat(intf *vsid, intf *findex, _fcd attrnm, intf *dtype, intf *count, intf *values,
                          intf *attrnmlen);
HDFFCLIBAPI intf nvsfcsca(intf *vsid, intf *findex, _fcd attrnm, intf *dtype, intf *count, _fcd values,
                          intf *attrnmlen);
HDFFCLIBAPI intf nvsfnats(intf *vsid);
HDFFCLIBAPI intf nvsffnas(intf *vsid, intf *findex);
HDFFCLIBAPI intf nvsfcfda(intf *vsid, intf *findex, _fcd attrnm, intf *attrnmlen);
HDFFCLIBAPI intf nvsfcain(intf *vsid, intf *findex, intf *aindex, _fcd attrname, intf *dtype, intf *count,
                          intf *size, intf *attrnamelen);
HDFFCLIBAPI intf nvsfgnat(intf *vsid, intf *findex, intf *aindex, intf *values);
HDFFCLIBAPI intf nvsfgcat(intf *vsid, intf *findex, intf *aindex, _fcd values);
HDFFCLIBAPI intf nvsfisat(intf *vsid);
HDFFCLIBAPI intf nvfcsatt(intf *vgid, _fcd attrnm, intf *dtype, intf *count, intf *values, intf *attrnmlen);
HDFFCLIBAPI intf nvfcscat(intf *vgid, _fcd attrnm, intf *dtype, intf *count, _fcd values, intf *attrnmlen);
HDFFCLIBAPI intf nvfnatts(intf *vgid);
HDFFCLIBAPI intf nvfcfdat(intf *vgid, _fcd attrnm, intf *attrnmlen);
HDFFCLIBAPI intf nvfainfo(intf *vgid, intf *aindex, _fcd attrname, intf *dtype, intf *count, intf *size);
HDFFCLIBAPI intf nvfgnatt(intf *vgid, intf *aindex, intf *values);
HDFFCLIBAPI intf nvfgcatt(intf *vgid, intf *aindex, _fcd values);
HDFFCLIBAPI intf nvfgver(intf *vgid);

/* Added for windows */
HDFFCLIBAPI intf nvsfainf(intf *vsid, intf *findex, intf *aindex, _fcd attrname, intf *dtype, intf *count,
                          intf *size);

HDFFCLIBAPI intf nvsfcnats(intf *vsid);

HDFFCLIBAPI intf nvsfcfnas(intf *vsid, intf *findex);

HDFFCLIBAPI intf nvsfcainf(intf *vsid, intf *findex, intf *aindex, _fcd attrname, intf *dtype, intf *count,
                           intf *size);

HDFFCLIBAPI intf nvsfcgna(intf *vsid, intf *findex, intf *aindex, intf *values);

HDFFCLIBAPI intf nvsfcgca(intf *vsid, intf *findex, intf *aindex, _fcd values);

HDFFCLIBAPI intf nvsfcisa(intf *vsid);

HDFFCLIBAPI intf nvfcnats(intf *vgid);

HDFFCLIBAPI intf nvfcainf(intf *vgid, intf *aindex, _fcd attrname, intf *dtype, intf *count, intf *size);

HDFFCLIBAPI intf nvfcgnat(intf *vgid, intf *aindex, intf *values);

HDFFCLIBAPI intf nvfcgcat(intf *vgid, intf *aindex, _fcd values);

HDFFCLIBAPI intf nvfcgver(intf *vgid);
/* End of windows */

/*
 ** from dfufp2i.c
 */

#define nduif2i H4_F77_FUNC(duif2i, DUIF2I)

HDFFCLIBAPI int nduif2i(int32 *hdim, int32 *vdim, float32 *max, float32 *min, float32 hscale[],
                        float32 vscale[], float32 data[], _fcd palette, _fcd outfile, int *ct_method,
                        int32 *hres, int32 *vres, int *compress, int *lenfn);

#ifdef __cplusplus
}
#endif

#endif /* H4_H_PROTO_FORTRAN */

/*
$Header$

$Log$
Revision 1.2  1992/08/27 19:55:26  likkai
change declaration of Vclose: now it returns intn instead of void.
change declaration of DFvsetclose: returns intn instead of int.

 * Revision 1.1  1992/08/25  21:40:44  koziol
 * Initial revision
 *
*/
#ifndef _H_PROTO
#define _H_PROTO

/*
** from hfile.c 
*/
extern int32 Hopen 
  PROTO((char *path, intn access, int16 ndds));

extern intn Hclose
  PROTO((int32 file_id));

extern int32 Hstartread
  PROTO((int32 file_id, uint16 tag, uint16 ref));

extern intn Hnextread
  PROTO((int32 access_id, uint16 tag, uint16 ref, int origin));

extern intn Hinquire
  PROTO((int32 access_id, int32 *pfile_id, uint16 *ptag, uint16 *pref, 
     int32 *plength, int32 *poffset, int32 *pposn, int16 *paccess,
     int16 *pspecial));

extern int32 Hstartwrite
  PROTO((int32 file_id, uint16 tag, uint16 ref, int32 length));

extern intn Hseek
  PROTO((int32 access_id, int32 offset, int origin));

extern int32 Hread
  PROTO((int32 access_id, int32 length, uint8 *data));

extern int32 Hwrite
  PROTO((int32 access_id, int32 length, uint8 *data));

extern int32 Hendaccess
  PROTO((int32 access_id));

extern int32 Hgetelement
  PROTO((int32 file_id, uint16 tag, uint16 ref, uint8 *data));

extern int Hputelement
  PROTO((int32 file_id, uint16 tag, uint16 ref, uint8 *data, int32 length));

extern int32 Hlength
  PROTO((int32 file_id, uint16 tag, uint16 ref));

extern int32 Hoffset
  PROTO((int32 file_id, uint16 tag, uint16 ref));

extern int Hdupdd
  PROTO((int32 file_id, uint16 tag, uint16 ref, uint16 old_tag, uint16 old_ref));

extern int Hdeldd
  PROTO((int32 file_id, uint16 tag, uint16 ref));

extern uint16 Hnewref
  PROTO((int32 file_id));

extern int Hsync
  PROTO((int32 file_id));

extern int Hnumber
  PROTO((int32 file_id, uint16 tag));

extern int Hgetlibversion
  PROTO((uint32 *major, uint32 *minor, uint32 *release, char *string));

extern int Hgetfileversion
  PROTO((int32 file_id, uint32 *major, uint32 *minor, uint32 *release, 
	 char *string));

#ifdef PC
extern int32 HDfreadbig
  PROTO((VOIDP buffer, int32 size,FILE *fp));

extern int32 HDfwritebig
  PROTO((VOIDP buffer, int32 size,FILE *fp));

#endif

extern uint16 HDmake_special_tag
  PROTO((uint16 tag));

extern bool HDis_special_tag
  PROTO((uint16 tag));

extern uint16 HDbase_tag
  PROTO((uint16 tag));

extern int HDerr
  PROTO((int32 file_id));

extern bool HDvalidfid
  PROTO((int32 file_id));

extern char *HDgettagname
  PROTO((uint16 tag));

extern int32 Hishdf
  PROTO((char *filename));

/*
** from hkit.c 
*/
extern char *HDstrncpy
  PROTO((register char *dest, register char *source, int32 len));

extern int32 HDspaceleft
  PROTO((void));

extern void *HDgetspace
  PROTO((uint32 qty));

extern void *HDfreespace
  PROTO((void *ptr));

extern intn HDc2fstr
  PROTO((char *str, int len));

extern char *HDf2cstring
  PROTO((_fcd fdesc, int len));

extern intn HDmemcopy
  PROTO((uint8 *from, uint8 *to, int32 length));

extern intn HDflush
    PROTO((int32 file_id));

/* 
** from hblocks.c 
*/
extern int32 HLcreate
  PROTO((int32 file_id, uint16 tag, uint16 ref, int32 block_length, 
	 int32 number_blocks));

/*
** from hextelt.c 
*/
extern int32 HXcreate
  PROTO((int32 file_id, uint16 tag, uint16 ref, char *extern_file_name));

/*
** from herr.c 
*/
extern char *HEstring
  PROTO((int16 error_code));

extern VOID HEpush
  PROTO((int16 error_code, char *function_name, char *file_name, int line));

#ifndef _H_ERR_MASTER_
extern VOID HEreport
  PROTO((char *, ...));
#endif /* _H_ERR_MASTER_ */

extern VOID HEprint
  PROTO((FILE *stream, int32 print_level));

extern int16 HEvalue
  PROTO((int32 level));

/* 
** from dfcomp.c 
*/
extern int DFputcomp
  PROTO((int32 file_id, uint16 tag, uint16 ref, uint8 *image, int32 xdim,
     int32 ydim, uint8 *palette, uint8 *newpal, int16 scheme));

extern int DFgetcomp
  PROTO((int32 file_id, uint16 tag, uint16 ref, uint8 *image, int32 xdim,
	 int32 ydim, uint16 scheme));

extern int32 DFCrle
  PROTO((VOIDP buf, VOIDP bufto, int32 len));

extern int32 DFCunrle
  PROTO((uint8 *buf, uint8 *bufto, int32 outlen, int resetsave));

/*
** from dfimcomp.c 
*/
extern VOID DFCimcomp
  PROTO((int32 xdim, int32 ydim, uint8 in[], uint8 out[], uint8 in_pal[],
     uint8 out_pal[], int mode));

extern VOID DFCunimcomp
  PROTO((int32 xdim, int32 ydim, uint8 in[], uint8 out[]));

/* 
** from dfgroup.c 
*/
extern int DFdiread
  PROTO((int32 file_id, uint16 tag, uint16 ref));

extern int DFdiget
  PROTO((uint16 *ptag, uint16 *pref));

extern int DFdisetup
  PROTO((int maxsize));

extern int DFdiput
  PROTO((uint16 tag, uint16 ref));

extern int DFdiwrite
  PROTO((int32 file_id, uint16 tag, uint16 ref));

/*
** from dfp.c 
*/
extern intn DFPgetpal
  PROTO((char *filename, VOIDP palette));

extern intn DFPputpal
  PROTO((char *filename, VOIDP palette, int overwrite, char *filemode));

extern int DFPaddpal
  PROTO((char *filename, VOIDP palette));

extern int DFPnpals
  PROTO((char *filename));

extern intn DFPreadref
  PROTO((char *filename, uint16 ref));

extern int DFPwriteref
  PROTO((char *filename, uint16 ref));

extern int DFPrestart
  PROTO((void));

extern int DFPlastref
  PROTO((void));

extern int32 DFPIopen
  PROTO((char *filename, int access));

/*
** from dfr8.c 
*/
extern intn DFR8getdims
  PROTO((char *filename, int32 *pxdim, int32 *pydim, int *pispal));

extern intn DFR8getimage
  PROTO((char *filename, uint8 *image, int32 xdim, int32 ydim, uint8 *pal));

extern int DFR8setpalette
  PROTO((uint8 *pal));

extern int DFR8putimage
  PROTO((char *filename, VOIDP image, int32 xdim, int32 ydim, uint16 compress));

extern int DFR8addimage
  PROTO((char *filename, VOIDP image, int32 xdim, int32 ydim, uint16 compress));

extern int DFR8nimages
  PROTO((char *filename));

extern intn DFR8readref
  PROTO((char *filename, uint16 ref));

extern int DFR8writeref
  PROTO((char *filename, uint16 ref));

extern int DFR8restart
  PROTO((void));

extern int DFR8lastref
  PROTO((void));

/*
** from dfgr.c 
*/
extern int DFGRgetlutdims
  PROTO((char *filename, int32 *pxdim, int32 *pydim, int *pncomps, int *pil));

extern int DFGRreqlutil
  PROTO((int il));

extern int DFGRgetlut
  PROTO((char *filename, VOIDP lut, int32 xdim, int32 ydim));

extern int DFGRgetimdims
  PROTO((char *filename, int32 *pxdim, int32 *pydim, int *pncomps, int *pil));

extern int DFGRreqimil
  PROTO((int il));

extern int DFGRgetimage
  PROTO((char *filename, VOIDP image, int32 xdim, int32 ydim));

extern int DFGRsetcompress
  PROTO((int scheme));

extern int DFGRsetlutdims
  PROTO((int32 xdim, int32 ydim, int ncomps, int il));

extern int DFGRsetlut
  PROTO((VOIDP lut, int32 xdim, int32 ydim));

extern int DFGRaddlut
  PROTO((char *filename, VOIDP lut, int32 xdim, int32 ydim));

extern int DFGRsetimdims
  PROTO((int32 xdim, int32 ydim, int ncomps, int il));

extern int DFGRaddimage
  PROTO((char *filename, VOIDP image, int32 xdim, int32 ydim));

extern int DFGRputimage
  PROTO((char *filename, VOIDP image, int32 xdim, int32 ydim));

extern int DFGRreadref
  PROTO((char *filename, uint16 ref));

extern int DFGRIgetdims
  PROTO((char *filename, int32 *pxdim, int32 *pydim, int *pncomps, 
	 int *pil, int type));

extern int DFGRIreqil
  PROTO((int il, int type));

extern int DFGRIgetimlut
  PROTO((char *filename, VOIDP imlut, int32 xdim, int32 ydim, int type, 
	 int isfortran));

extern int DFGRIsetdims
  PROTO((int32 xdim, int32 ydim, int ncomps, int type));

extern int DFGRIsetil
  PROTO((int il, int type));

extern int DFGRIrestart
  PROTO((void));

extern int DFGRIaddimlut
  PROTO((char *filename, VOIDP imlut, int32 xdim, int32 ydim, int type, 
	 int isfortran, int newfile));

/*
** from df24.c 
*/
extern int DF24getdims
  PROTO((char *filename, int32 *pxdim, int32 *pydim, int *pil));

extern int DF24reqil
  PROTO((int il));

extern int DF24getimage
  PROTO((char *filename, VOIDP image, int32 xdim, int32 ydim));

extern int DF24setdims
  PROTO((int32 xdim, int32 ydim));

extern int DF24setil
  PROTO((int il));

extern int DF24restart
  PROTO((void));

extern int DF24addimage
  PROTO((char *filename, VOIDP image, int32 xdim, int32 ydim));

extern int DF24putimage
  PROTO((char *filename, VOIDP image, int32 xdim, int32 ydim));

extern int DF24readref
  PROTO((char *filename, uint16 ref));

/*
** from dfan.c
*/

extern int32 DFANgetlablen
    PROTO((char *filename, uint16 tag, uint16 ref));

extern int DFANgetlabel
    PROTO((char *filename, uint16 tag, uint16 ref, char *label, int32 maxlen));

extern int32 DFANgetdesclen
    PROTO((char *filename, uint16 tag, uint16 ref));

extern int DFANgetdesc
    PROTO((char *filename, uint16 tag, uint16 ref, char *desc, int32 maxlen));

extern int32 DFANgetfidlen
    PROTO((int32 file_id, int isfirst));

extern int32 DFANgetfid
    PROTO((int32 file_id, char *id, int32 maxlen, int isfirst));

extern int32 DFANgetfdslen
    PROTO((int32 file_id, int isfirst));

extern int32 DFANgetfds
    PROTO((int32 file_id, char *desc, int32 maxlen, int isfirst));

extern int DFANputlabel
    PROTO((char *filename, uint16 tag, uint16 ref, char *label));

extern int DFANputdesc
    PROTO((char *filename, uint16 tag, uint16 ref, char *desc, int32 desclen));

extern int DFANaddfid
    PROTO((int32 file_id, char *id));

extern int DFANaddfds
    PROTO((int32 file_id, char *desc, int32 desclen));

extern uint16 DFANlastref
    PROTO((void));

extern int DFANlablist
    PROTO((char *filename, uint16 tag, uint16 reflist[], char *labellist,
                int listsize, int maxlen, int startpos));

extern uint16 DFANIlocate
  PROTO((int32 file_id, int type, uint16 tag, uint16 ref));

extern int DFANIaddentry
  PROTO((int type, uint16 annref, uint16 datatag, uint16 dataref));

extern int32 DFANIgetannlen
  PROTO((char *filename, uint16 tag, uint16 ref, int type));

extern intn DFANIgetann
  PROTO((char *filename, uint16 tag, uint16 ref, uint8 *ann,
                int32 maxlen, int type));

extern intn DFANIputann
  PROTO((char *filename, uint16 tag, uint16 ref, uint8 *ann, 
	 int32 annlen, int type));

extern int DFANIlablist
  PROTO((char *filename, uint16 tag, uint16 reflist[], uint8 *labellist,
	 int listsize, int maxlen, int startpos, int isfortran));

extern int DFANIaddfann
  PROTO((int32 file_id, char *ann, int32 annlen, int type));

extern int32 DFANIgetfannlen
  PROTO((int32 file_id, int type, int isfirst));

extern int32 DFANIgetfann
  PROTO((int32 file_id, char *ann, int32 maxlen, int type, int isfirst));

/*
** from dfsd.c (actually contained in dfsd.h because of use of private data
**              types in the prototypes)
*/

/*
** from dfconv.c
*/

extern int DFKNTsize
    PROTO((int32 number_type));

extern int32 DFKisnativeNT
    PROTO((int32 numbertype));

extern int8 DFKgetPNSC
    PROTO((int32 numbertype, int32 machinetype));

extern intn DFKsetNT
    PROTO((int32 ntype));

extern int32 DFKconvert
    PROTO((uint8 *source, uint8 *dest, int32 ntype, int32 num_elm,
            int16 access, int32 source_stride, int32 dest_stride));

/*
** from dfanF.c
*/
#ifndef DFAN_FNAMES
#   define  DFAN_FNAMES
#ifdef DF_CAPFNAMES
#   define ndaiganl  FNAME(DAIGANL)
#   define ndaigann  FNAME(DAIGANN)
#   define ndaipann  FNAME(DAIPANN)
#   define ndailist  FNAME(DAILIST)
#   define ndalref   FNAME(DALREF)
#   define ndfanlastref     FNAME(DFANLASTREF)

#   define ndfanaddfds      FNAME(DFANADDFDS)
#   define ndfangetfidlen   FNAME(DFANGETFIDLEN)
#   define ndfangetfdslen   FNAME(DFANGETFDSLEN)
#   define ndfangetfid      FNAME(DFANGETFID)
#   define ndfangetfds      FNAME(DFANGETFDS)
#   define ndaafds          FNAME(DAAFDS)
#   define ndagfidl         FNAME(DAGFIDL)
#   define ndagfdsl         FNAME(DAGFDSL)
#   define ndagfid          FNAME(DAGFID)
#   define ndagfds          FNAME(DAGFDS)
#   define ndaiafid         FNAME(DAIAFID)
#else   /* DF_CAPFNAMES */
#   define ndaiganl  FNAME(daiganl)
#   define ndaigann  FNAME(daigann)
#   define ndaipann  FNAME(daipann)
#   define ndailist  FNAME(dailist)
#   define ndalref   FNAME(dalref)
#   define ndfanlastref     FNAME(dfanlastref)

#   define ndfanaddfds      FNAME(dfanaddfds)
#   define ndfangetfidlen   FNAME(dfangetfidlen)
#   define ndfangetfdslen   FNAME(dfangetfdslen)
#   define ndfangetfid      FNAME(dfangetfid)
#   define ndfangetfds      FNAME(dfangetfds)
#   define ndaafds          FNAME(daafds)
#   define ndagfidl         FNAME(dagfidl)
#   define ndagfdsl         FNAME(dagfdsl)
#   define ndagfid          FNAME(dagfid)
#   define ndagfds          FNAME(dagfds)
#   define ndaiafid         FNAME(daiafid)
#endif /* DF_CAPFNAMES */
#endif /* DFAN_FNAMES */

extern FRETVAL(int32) ndaiganl
  PROTO((_fcd filename, int32 *tag, int32 *ref, int *type, int *fnlen));

extern FRETVAL(int32) ndaigann
  PROTO((_fcd filename, int32 *tag, int32 *ref, _fcd annotation, int32 *maxlen,
     int *type, int *fnlen));

extern FRETVAL(int32) ndaipann
  PROTO((_fcd filename, int32 *tag, int32 *ref, _fcd annotation, int32 *annlen,
     int *type, int *fnlen));

extern FRETVAL(int) ndailist
  PROTO((_fcd filename, int32 *tag, int32 reflist[], _fcd labellist,
    int *listsize, int *maxlen, int *startpos, int *fnlen));

extern FRETVAL(uint16) ndalref
  PROTO((void));

extern FRETVAL(uint16) ndfanlastref
  PROTO((void));

extern FRETVAL(int32) ndfanaddfds
  PROTO((int32 *dfile, _fcd desc, int32 *desclen));

extern FRETVAL(int32) ndfangetfidlen
  PROTO((int32 *dfile, int *isfirst));

extern FRETVAL(int32) ndfangetfdslen
  PROTO((int32 *dfile, int *isfirst));

extern FRETVAL(int32) ndfangetfid
  PROTO((int32 *dfile, _fcd id, int32 *maxlen, int *isfirst));

extern FRETVAL(int32) ndfangetfds
  PROTO((int32 *dfile, _fcd id, int32 *maxlen, int *isfirst));

extern FRETVAL(int32) ndaafds
  PROTO((int32 *dfile, _fcd desc, int32 *desclen));

extern FRETVAL(int32) ndagfidl
  PROTO((int32 *dfile, int *isfirst));

extern FRETVAL(int32) ndagfdsl
  PROTO((int32 *dfile, int *isfirst));

extern FRETVAL(int32) ndagfid
  PROTO((int32 *dfile, _fcd id, int32 *maxlen, int *isfirst));

extern FRETVAL(int32) ndagfds
  PROTO((int32 *dfile, _fcd id, int32 *maxlen, int *isfirst));

extern FRETVAL(int32) ndaiafid
  PROTO((int32 *dfile, _fcd id, int32 *idlen));

/*
** from dfr8F.c
*/
#ifndef DFR8_FNAMES
#   define DFR8_FNAMES
#ifdef DF_CAPFNAMES
#   define nd8spal   FNAME(D8SPAL)
#   define nd8first  FNAME(D8FIRST)
#   define nd8igdim  FNAME(D8IGDIM)
#   define nd8igimg  FNAME(D8IGIMG)
#   define nd8ipimg  FNAME(D8IPIMG)
#   define nd8iaimg  FNAME(D8IAIMG)
#   define nd8irref  FNAME(D8IRREF)
#   define nd8iwref  FNAME(D8IWREF)
#   define nd8inims  FNAME(D8INIMS)
#   define nd8lref   FNAME(D8LREF)
#   define ndfr8lastref      FNAME(DFR8LASTREF)
#   define ndfr8setpalette   FNAME(DFR8SETPALETTE)
#   define ndfr8restart  FNAME(DFR8RESTART)
#else   /* !DF_CAPFNAMES */
#   define nd8spal   FNAME(d8spal)
#   define nd8first  FNAME(d8first)
#   define nd8igdim  FNAME(d8igdim)
#   define nd8igimg  FNAME(d8igimg)
#   define nd8ipimg  FNAME(d8ipimg)
#   define nd8iaimg  FNAME(d8iaimg)
#   define nd8irref  FNAME(d8irref)
#   define nd8iwref  FNAME(d8iwref)
#   define nd8inims  FNAME(d8inims)
#   define nd8lref   FNAME(d8lref)
#   define ndfr8lastref      FNAME(dfr8lastref)
#   define ndfr8setpalette   FNAME(dfr8setpalette)
#   define ndfr8restart  FNAME(dfr8restart)
#endif /* DF_CAPFNAMES */
#endif /* DFR8_FNAMES */

extern  FRETVAL(int) nd8spal
    PROTO((_fcd pal));

extern  FRETVAL(int) nd8first
    PROTO((void));

extern  FRETVAL(int) nd8igdim
    PROTO((_fcd filename, int32 *xdim, int32 *ydim, int *ispal, int *lenfn));

extern  FRETVAL(int) nd8igimg
    PROTO((_fcd filename, _fcd image, int32 *xdim, int32 *ydim,
                _fcd pal, int *lenfn));

extern  FRETVAL(int) nd8ipimg
    PROTO((_fcd filename, _fcd image, int32 *xdim, int32 *ydim, int *compress,
                int *lenfn));

extern  FRETVAL(int) nd8iaimg
    PROTO((_fcd filename, _fcd image, int32 *xdim, int32 *ydim, int *compress,
                int *lenfn));

extern  FRETVAL(intn) nd8irref
    PROTO((_fcd filename, int32 *ref, int *fnlen));

extern FRETVAL(intn) nd8iwref
    PROTO((_fcd filename, int32 *ref, int *fnlen));

extern FRETVAL(int) nd8inims
    PROTO((_fcd filename, int *fnlen));

extern FRETVAL(int) nd8lref
    PROTO((void));

extern FRETVAL(int) ndfr8lastref
    PROTO((void));

extern  FRETVAL(int) ndfr8setpalette
    PROTO((_fcd pal));

extern  FRETVAL(int) ndfr8restart
    PROTO((void));

/*
** from dfsdF.c
*/
#ifndef DFSD_FNAMES
#   define DFSD_FNAMES
#ifdef DF_CAPFNAMES
#   define ndsgdast  FNAME(DSGDAST)
#   define ndsgdisc  FNAME(DSGDISC)
#   define ndsgrang  FNAME(DSGRANG)
#   define ndssdims  FNAME(DSSDIMS)
#   define ndssdisc  FNAME(DSSDISC)
#   define ndssrang  FNAME(DSSRANG)
#   define ndsclear  FNAME(DSCLEAR)
#   define ndsslens  FNAME(DSSLENS)
#   define ndsgdiln  FNAME(DSGDILN)
#   define ndsgdaln  FNAME(DSGDALN)
#   define ndsfirst  FNAME(DSFIRST)
#   define ndspslc   FNAME(DSPSLC)
#   define ndseslc   FNAME(DSESLC)
#   define ndsgnt    FNAME(DSGNT)
#   define ndssnt    FNAME(DSSNT)
#   define ndsigdim  FNAME(DSIGDIM)
#   define ndsigdat  FNAME(DSIGDAT)
#   define ndsipdat  FNAME(DSIPDAT)
#   define ndsiadat  FNAME(DSIADAT)
#   define ndsigdas  FNAME(DSIGDAS)
#   define ndsigslc  FNAME(DSIGSLC)
#   define ndsigdis  FNAME(DSIGDIS)
#   define ndsisslc  FNAME(DSISSLC)
#   define ndsisdas  FNAME(DSISDAS)
#   define ndsisdis  FNAME(DSISDIS)
#   define ndsirref  FNAME(DSIRREF)
#   define ndslref   FNAME(DSLREF)
#   define ndsinum   FNAME(DSINUM)
#   define ndfsdgetdatastrs  FNAME(DFSDGETDATASTRS)
#   define ndfsdgetdimscale  FNAME(DFSDGETDIMSCALE)
#   define ndfsdgetrange     FNAME(DFSDGETRANGE)
#   define ndfsdsetdims      FNAME(DFSDSETDIMS)
#   define ndfsdsetdimscale  FNAME(DFSDSETDIMSCALE)
#   define ndfsdsetrange     FNAME(DFSDSETRANGE)
#   define ndfsdclear        FNAME(DFSDCLEAR)
#   define ndfsdsetlengths   FNAME(DFSDSETLENGTHS)
#   define ndfsdgetdimlen    FNAME(DFSDGETDIMLEN)
#   define ndfsdgetdatalen   FNAME(DFSDGETDATALEN)
#   define ndfsdrestart      FNAME(DFSDRESTART)
#   define ndfsdputslice     FNAME(DFSDPUTSLICE)
#   define ndfsdendslice     FNAME(DFSDENDSLICE)
#   define ndfsdsetnt        FNAME(DFSDSETNT)
#   define ndfsdgetnt        FNAME(DFSDGETNT)
#   define ndfsdlastref      FNAME(DFSDLASTREF)
#else
#   define ndsgdast  FNAME(dsgdast)
#   define ndsgdisc  FNAME(dsgdisc)
#   define ndsgrang  FNAME(dsgrang)
#   define ndssdims  FNAME(dssdims)
#   define ndssdisc  FNAME(dssdisc)
#   define ndssrang  FNAME(dssrang)
#   define ndsclear  FNAME(dsclear)
#   define ndsslens  FNAME(dsslens)
#   define ndsgdiln  FNAME(dsgdiln)
#   define ndsgdaln  FNAME(dsgdaln)
#   define ndsfirst  FNAME(dsfirst)
#   define ndspslc   FNAME(dspslc)
#   define ndseslc   FNAME(dseslc)
#   define ndsgnt    FNAME(dsgnt)
#   define ndssnt    FNAME(dssnt)
#   define ndsigdim  FNAME(dsigdim)
#   define ndsigdat  FNAME(dsigdat)
#   define ndsipdat  FNAME(dsipdat)
#   define ndsiadat  FNAME(dsiadat)
#   define ndsigdas  FNAME(dsigdas)
#   define ndsigslc  FNAME(dsigslc)
#   define ndsigdis  FNAME(dsigdis)
#   define ndsisslc  FNAME(dsisslc)
#   define ndsisdas  FNAME(dsisdas)
#   define ndsisdis  FNAME(dsisdis)
#   define ndsirref  FNAME(dsirref)
#   define ndslref   FNAME(dslref)
#   define ndsinum   FNAME(dsinum)
#   define ndfsdgetdatastrs  FNAME(dfsdgetdatastrs)
#   define ndfsdgetdimscale  FNAME(dfsdgetdimscale)
#   define ndfsdgetrange     FNAME(dfsdgetrange)
#   define ndfsdsetdims      FNAME(dfsdsetdims)
#   define ndfsdsetdimscale  FNAME(dfsdsetdimscale)
#   define ndfsdsetrange     FNAME(dfsdsetrange)
#   define ndfsdclear        FNAME(dfsdclear)
#   define ndfsdsetlengths   FNAME(dfsdsetlengths)
#   define ndfsdgetdimlen    FNAME(dfsdgetdimlen)
#   define ndfsdgetdatalen   FNAME(dfsdgetdatalen)
#   define ndfsdrestart      FNAME(dfsdrestart)
#   define ndfsdputslice     FNAME(dfsdputslice)
#   define ndfsdendslice     FNAME(dfsdendslice)
#   define ndfsdsetnt        FNAME(dfsdsetnt)
#   define ndfsdgetnt        FNAME(dfsdgetnt)
#   define ndfsdlastref      FNAME(dfsdlastref)
#endif /* DF_CAPFNAMES */
#endif  /* DFSD_FNAMES */

extern  FRETVAL(int) ndsgdast
    PROTO((_fcd label, _fcd unit, _fcd format, _fcd coordsys));

extern FRETVAL(int) ndsigdis
    PROTO((int *dim, _fcd label, _fcd unit, _fcd format, int32 *llabel,
           int32 *lunit, int32 *lformat));

extern FRETVAL(int) ndsgdisc
    PROTO((int *dim, int32 *maxsize, void *scale));

extern FRETVAL(int) ndsgrang
    PROTO((void *pmax, void *pmin));

extern FRETVAL(int) ndssdims
    PROTO((int32 *rank, int32 dimsizes[]));

extern FRETVAL(int) ndssdisc
    PROTO((int *dim, int32 *dimsize, void *scale));

extern FRETVAL(int) ndssrang
    PROTO((void *max, void *min));

extern FRETVAL(int) ndsclear
    PROTO((void));

extern FRETVAL(int) ndsslens
    PROTO((int *maxlen_label, int *maxlen_unit, int *maxlen_format,
            int *maxlen_coordsys));

extern FRETVAL(int) ndsgdiln
    PROTO((int *dim, int *llabel, int *lunit, int *lformat));

extern FRETVAL(int) ndsgdaln
    PROTO((int *llabel, int *lunit, int *lformat, int *lcoordsys));

extern FRETVAL(int) ndsfirst
    PROTO((void));

extern FRETVAL(int) ndspslc
    PROTO((int32 windims[], void *data, int32 dims[]));

extern FRETVAL(int) ndseslc
    PROTO((void));

extern FRETVAL(int) ndssnt
    PROTO((int32 *numbertype));

extern FRETVAL(int) ndsgnt
    PROTO((int32 *pnumbertype));

extern FRETVAL(int) ndsigdim
    PROTO((_fcd filename, int32 *prank, int32 sizes[], int32 *maxrank, int32 *lenfn));

extern FRETVAL(int) ndsigdat
    PROTO((_fcd filename, int32 *rank, int32 maxsizes[], void *data, int32 *fnlen));

extern FRETVAL(int) ndsipdat
    PROTO((_fcd filename, int32 *rank, int32 dimsizes[], void *data, int32 *fnlen));

extern FRETVAL(int) ndsiadat
    PROTO((_fcd filename, int32 *rank, int32 dimsizes[], void *data, int32 *fnlen));

extern FRETVAL(int) ndsigslc
    PROTO((_fcd filename, int32 winst[], int32 windims[], void *data,
            int32 dims[], int *fnlen));

extern FRETVAL(int) ndsisslc
    PROTO((_fcd filename, int *fnlen));

extern FRETVAL(int) ndsirref
    PROTO((_fcd filename, int32 *ref, int *fnlen));

extern FRETVAL(int) ndslref
    PROTO((void));

extern FRETVAL(int) ndsinum
    PROTO((_fcd filename, int32 *len));

extern FRETVAL(int) ndfsdgetdatastrs
    PROTO((_fcd label, _fcd unit, _fcd format, _fcd coordsys));

extern FRETVAL(int) ndfsdgetdimscale
    PROTO((int *dim, int32 *maxsize, void *scale));

extern FRETVAL(int) ndfsdgetrange
    PROTO((void *pmax, void *pmin));

extern FRETVAL(int) ndfsdsetdims
    PROTO((int32 *rank, int32 dimsizes[]));

extern FRETVAL(int) ndfsdsetdimscale
    PROTO((int *dim, int32 *dimsize, void *scale));

extern FRETVAL(int) ndfsdsetrange
    PROTO((void *max, void *min));

extern FRETVAL(int) ndfsdclear
    PROTO((void));

extern FRETVAL(int) ndfsdsetlengths
    PROTO((int *maxlen_label, int *maxlen_unit, int *maxlen_format,
                int *maxlen_coordsys));

extern FRETVAL(int) ndfsdgetdimlen
    PROTO((int* dim, int *llabel, int *lunit, int *lformat));

extern FRETVAL(int) ndfsdgetdatalen
    PROTO((int *llabel, int *lunit, int *lformat, int *lcoordsys));

extern FRETVAL(int) ndfsdrestart
    PROTO((void));

extern FRETVAL(int) ndfsdputslice
    PROTO((int32 windims[], void *data, int32 dims[]));

extern FRETVAL(int) ndfsdendslice
    PROTO((void));

extern FRETVAL(int) ndfsdsetnt
    PROTO((int32 *numbertype));

extern FRETVAL(int) ndfsdgetnt
    PROTO((int32 *pnumbertype));

extern FRETVAL(int) ndfsdlastref
    PROTO((void));

/*
** from dfpF.c
*/

#ifndef DFP_FNAMES
#   define DFP_FNAMES
#ifdef DF_CAPFNAMES
#   define ndpigpal  FNAME(DPIGPAL)
#   define ndpippal  FNAME(DPIPPAL)
#   define ndpinpal  FNAME(DPINPAL)
#   define ndpiwref  FNAME(DPIWREF)
#   define ndpirref  FNAME(DPIRREF)
#   define ndprest   FNAME(DPREST)
#   define ndplref   FNAME(DPLREF)
#   define ndfprestart   FNAME(DFPRESTART)
#   define ndfplastref   FNAME(DFPLASTREF)
#else   /* !DF_CAPNAMES */
#   define ndpigpal  FNAME(dpigpal)
#   define ndpippal  FNAME(dpippal)
#   define ndpinpal  FNAME(dpinpal)
#   define ndpiwref  FNAME(dpiwref)
#   define ndpirref  FNAME(dpirref)
#   define ndprest   FNAME(dprest)
#   define ndplref   FNAME(dplref)
#   define ndfprestart   FNAME(dfprestart)
#   define ndfplastref   FNAME(dfplastref)
#endif /* DF_CAPFNAMES */
#endif /* DFP_FNAMES */

extern  FRETVAL(int) ndpigpal
    PROTO((_fcd filename, _fcd pal, int *fnlen));

extern FRETVAL(int) ndpippal
    PROTO((_fcd filename, _fcd pal, int *overwrite, _fcd filemode, int *fnlen));

extern FRETVAL(int) ndpinpal
    PROTO((_fcd filename, int *fnlen));

extern FRETVAL(int) ndpirref
    PROTO((_fcd filename, uint16 *ref, int *fnlen));

extern FRETVAL(int) ndpiwref
    PROTO((_fcd filename, uint16 *ref, int *fnlen));

extern FRETVAL(int) ndprest
    PROTO((void));

extern FRETVAL(int) ndplref
    PROTO((void));

extern FRETVAL(int) ndfprestart
    PROTO((void));

extern FRETVAL(int) ndfplastref
    PROTO((void));

/*
** from df24F.c
*/
#ifndef DF24_FNAMES
#   define DF24_FNAMES
#ifdef DF_CAPFNAMES
#   define nd2reqil  FNAME(D2REQIL)
#   define ndf24reqil    FNAME(DF24REQIL)
#   define nd2sdims  FNAME(D2SDIMS)
#   define ndf24setdims  FNAME(DF24SETDIMS)
#   define nd2setil  FNAME(D2SETIL)
#   define ndf24setil    FNAME(DF24SETIL)
#   define nd2first  FNAME(D2FIRST)
#   define ndf24restart  FNAME(DF24RESTART)
#   define nd2igdim  FNAME(D2IGDIM)
#   define nd2igimg  FNAME(D2IGIMG)
#   define nd2iaimg  FNAME(D2IAIMG)
#   define nd2irref  FNAME(D2IRREF)
#else
#   define nd2reqil  FNAME(d2reqil)
#   define ndf24reqil    FNAME(df24reqil)
#   define nd2sdims  FNAME(d2sdims)
#   define ndf24setdims  FNAME(df24setdims)
#   define nd2setil  FNAME(d2setil)
#   define ndf24setil    FNAME(df24setil)
#   define nd2first  FNAME(d2first)
#   define ndf24restart  FNAME(df24restart)
#   define nd2igdim  FNAME(d2igdim)
#   define nd2igimg  FNAME(d2igimg)
#   define nd2iaimg  FNAME(d2iaimg)
#   define nd2irref  FNAME(d2irref)
#endif /* DF_CAPFNAMES */
#endif /* DF24_FNAMES */

extern FRETVAL(int) nd2reqil
    PROTO((int *il));

extern FRETVAL(int) nd2sdims
    PROTO((int32 *xdim, int32 *ydim));

extern FRETVAL(int) nd2igdim
    PROTO((_fcd filename, int32 *pxdim, int32 *pydim, int *pil, int *fnlen));

extern FRETVAL(int) nd2igimg
    PROTO((_fcd filename, _fcd image, int32 *xdim, int32 *ydim, int *fnlen));

extern FRETVAL(int) nd2iaimg
    PROTO((_fcd filename, _fcd image, int32 *xdim, int32 *ydim,
            int *fnlen, int *newfile));

extern FRETVAL(int) nd2setil
    PROTO((int *il));

extern FRETVAL(int) nd2first
    PROTO((void));

extern FRETVAL(int) ndf24reqil
    PROTO((int *il));

extern FRETVAL(int) ndf24setdims
    PROTO((int32 *xdim, int32 *ydim));

extern FRETVAL(int) ndf24setil
    PROTO((int *il));

extern FRETVAL(int) ndf24restart
    PROTO((void));

extern FRETVAL(int) nd2irref
    PROTO((_fcd filename, int *ref, int *fnlen));

/*
** from dfF.c
*/
#ifndef DF_FNAMES
#   define DF_FNAMES
#ifdef DF_CAPFNAMES
#   define ndfiaccess FNAME(DFIACCESS)
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
#   define ndfiaccess FNAME(dfiaccess)
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

extern FRETVAL(int32) ndfiopen
    PROTO((_fcd name, int32 *access, int32 *defdds, intn *namelen));

extern FRETVAL(int) ndfclose
    PROTO((int32 *dfile));

extern FRETVAL(int) ndfdesc
    PROTO((int32 *dfile, int32 ptr[][4], int32 *begin, int32 *num));

extern FRETVAL(int) ndfdup
    PROTO((int32 *dfile, int32 *tag, int32 *ref, int32 *otag, int32 *oref));

extern FRETVAL(int) ndfdel
    PROTO((int32 *dfile, int32 *tag, int32 *ref));

extern FRETVAL(int32) ndfiaccess
    PROTO((int32 *dfile, int32 *tag, int32 *ref, _fcd access, int32 *acclen));

extern FRETVAL(int) ndfstart
    PROTO((int32 *dfile, int32 *tag, int32 *ref, char *access));

extern FRETVAL(int32) ndfread
    PROTO((int32 *dfile, _fcd ptr, int32 *len));

extern FRETVAL(int32) ndfseek
    PROTO((int32 *dfile, int32 *offset));

extern FRETVAL(int32) ndfwrite
    PROTO((int32 *dfile, _fcd ptr, int32 *len));

extern FRETVAL(int) ndfupdate
    PROTO((int32 *dfile));

extern FRETVAL(int32) ndfget
    PROTO((int32 *dfile, int32 *tag, int32 *ref, _fcd ptr));

extern FRETVAL(int32) ndfput
    PROTO((int32 *dfile, int32 *tag, int32 *ref, _fcd ptr, int32 *len));

extern FRETVAL(int) ndfsfind
    PROTO((int32 *dfile, int32 *tag, int32 *ref));

extern FRETVAL(int) ndffind
    PROTO((int32 *dfile, int32 *itag, int32 *iref, int32 *len));

extern FRETVAL(int) ndferrno
    PROTO((void));

extern FRETVAL(int32) ndfnewref
    PROTO((int32 *dfile));


extern FRETVAL(int) ndfnumber
    PROTO((int32 *dfile, int32 *tag));

#ifdef TEMP_OUT
extern FRETVAL(int) ndfstat
    PROTO((int32 *dfile, DFdata *dfinfo));
#endif

extern FRETVAL(int) ndfiishdf
    PROTO((_fcd name, intn *namelen));

/*
** from dfutil.c
*/
extern uint16 DFfindnextref
  PROTO((int32 file_id, uint16 tag, uint16 lref));

/*
** from dfutilF.c
*/
#ifndef DFUTIL_FNAMES
#   define DFUTIL_FNAMES
#ifdef DF_CAPFNAMES
#   define ndfindnr          FNAME(DFINDNR)
#   define ndffindnextref    FNAME(DFFINDNEXTREF)
#else
#   define ndfindnr          FNAME(dfindnr)
#   define ndffindnextref    FNAME(dffindnextref)
#endif /* DF_CAPFNAMES */
#endif /* DFUTIL_FNAMES */

extern FRETVAL(uint16) ndfindnr
    PROTO((int32 *dfile, uint16 *tag, uint16 *lref));

extern FRETVAL(uint16) ndffindnextref
    PROTO((int32 *dfile, uint16 *tag, uint16 *lref));

/*
** from herrF.c
*/
#ifndef HERR_FNAMES
#   define HERR_FNAMES
#ifdef DF_CAPFNAMES
#   define nheprnt   FNAME(HEPRNT)
#else
#   define nheprnt   FNAME(heprnt)
#endif /* DF_CAPFNAMES */
#endif  /* HERR_FNAMES */

extern FRETVAL(VOID) nheprnt
    PROTO((int32 print_levels));

/*
** from hfileF.c
*/
#ifndef HFILE_FNAMES
#   define HFILE_FNAMES
#ifdef DF_CAPFNAMES
#   define nhiopen   FNAME(HIOPEN)
#   define nhclose   FNAME(HCLOSE)
#else
#   define nhiopen   FNAME(hiopen)
#   define nhclose  FNAME(hclose)
#endif /* DF_CAPFNAMES */
#endif /* HFILE_FNAMES */

extern FRETVAL(int32) nhiopen
    PROTO((_fcd name, int *access, int16 *defdds, int *namelen));

extern FRETVAL(intn) nhclose
    PROTO((int32 *file_id));

/*
** from dfufp2im.c
*/
#ifndef DFUFP2I_FNAMES
#   define DFUFP2I_FNAMES
#ifdef DF_CAPFNAMES
#   define nduif2i       FNAME(DUIF2I)
#else
#   define nduif2i       FNAME(duif2i)
#endif /* DF_CAPFNAMES */
#endif /* DFUFP2I_FNAMES */

extern FRETVAL(int) nduif2i
    PROTO((int32 *hdim, int32 *vdim, float32 *max, float32 *min, float32 hscale[],
            float32 vscale[], float32 data[], uint8 *palette, _fcd outfile,
            int *ct_method, int32 *hres, int32 *vres, int *compress, int *lenfn));

extern int DFUfptoimage
    PROTO((int32 hdim, int32 vdim, float32 max, float32 min,
            float32 *hscale, float32 *vscale, float32 *data, uint8 *palette,
            char *outfile, int ct_method, int32 hres, int32 vres, int compress));

/* temp. fix for circular includes with vsets */
#ifdef PERM_OUT
/*
** from vconv.c
*/
extern int DIRECTtoIEEE
  PROTO((unsigned char *machfp, unsigned char *ieeefp, int mstep, int istep,
	int  n));

extern int DIRECTfromIEEE
  PROTO((unsigned char *ieeefp, unsigned char *machfp, int istep, int mstep,
	int n));

extern int bytetoIEEE
  PROTO((unsigned char *machfp, unsigned char *ieeefp, int mstep, int istep,
	int n));

extern int bytefromIEEE
  PROTO((unsigned char *ieeefp, unsigned char *machfp, int istep, int mstep,
	int n));

extern int inttoIEEE
  PROTO((unsigned char *src, unsigned char *dst, int sdelta, int ddelta,
	int n));

extern int intfromIEEE
  PROTO((unsigned char *src, unsigned char *dst, int sdelta, int ddelta,
	int n));

extern int longtoIEEE
  PROTO((unsigned char *src, unsigned char *dst, int sdelta, int ddelta,
	int n));

extern int longfromIEEE
  PROTO((unsigned char *src, unsigned char *dst, int sdelta, int ddelta,
	int n));

extern int vaxFtoIEEE
  PROTO((unsigned char *vaxfp, unsigned char *ieeefp, int vstep, int istep,
	int n));

extern int vaxFfromIEEE
  PROTO((unsigned char *ieeefp, unsigned char *vaxfp, int istep, int vstep,
	int n));

extern int floattoIEEE
  PROTO((unsigned char *machfp, unsigned char *ieeefp, int mstep, int istep,
	int n));

extern int floatfromIEEE
  PROTO((unsigned char *ieeefp, unsigned char *machfp, int istep, int mstep,
	int n));

extern int movebytes
  PROTO((VOIDP ss, VOIDP dd, int n));

extern int vicheckcompat
  PROTO((HFILEID f));

extern int vimakecompat
  PROTO((HFILEID f));

extern int vcheckcompat
  PROTO((char *fs));

extern int vmakecompat
  PROTO((char *fs));

extern int oldunpackvg
  PROTO((VGROUP *vg, unsigned char buf[], int *size));

extern int oldunpackvs
  PROTO((VDATA *vs, unsigned char buf[], int *size));


/*
** from vg.c
*/
extern int vnewref
  PROTO((HFILEID f));

extern int VSelts
  PROTO((VDATA *vs));

extern int VSgetinterlace
  PROTO((VDATA *vs));

extern int VSsetinterlace
  PROTO((VDATA *vs, int interlace));

extern int VSgetfields
  PROTO((VDATA *vs, char *fields));

extern int VSfexist
  PROTO((VDATA *vs, char *fields));

extern int VSsizeof
  PROTO((VDATA *vs, char *fields));

extern int matchnocase
  PROTO((char *strx, char *stry));

extern void VSdump
  PROTO((VDATA *vs));

extern void VSsetname
  PROTO((VDATA *vs, char *vsname));

extern void VSsetclass
  PROTO((VDATA *vs, char *vsclass));

extern void VSgetname
  PROTO((VDATA *vs, char *vsname));

extern int32 VSgetversion
  PROTO((VDATA *vs));

extern void VSgetclass
  PROTO((VDATA *vs, char *vsclass));

extern int VSinquire
  PROTO((VDATA *vs,int *nelt, int *interlace, char *fields, int *eltsize,
	char *vsname));

extern int VSlone
  PROTO((HFILEID f, int idarray[], int asize));

extern int Vlone
  PROTO((HFILEID f, int idarray[], int asize));

extern int Vfind
  PROTO((HFILEID f, char *vgname));

extern int VSfind
  PROTO((HFILEID f, char *vsname));


/*
** from vgp.c
*/
extern void setjj
  PROTO((void));

extern void setnojj
  PROTO((void));

extern int Load_vfile
  PROTO((HFILEID f));

extern void Remove_vfile
  PROTO((HFILEID f));

extern vfile_t *Get_vfile
  PROTO((HFILEID f));

extern vginstance_t *vginstance
  PROTO((HFILEID f, int vgid));

extern int vshowvgdir
  PROTO((HFILEID  f, char *text));

extern int vexistvg
  PROTO((HFILEID f, int vgid));

extern void vpackvg
  PROTO((VGROUP *vg, unsigned char buf[], int *size));

extern void vunpackvg
  PROTO((VGROUP *vg, unsigned char buf[], int *size));

extern VGROUP *Vattach
  PROTO((HFILEID f, int vgid, char *accesstype));

extern void Vdetach
  PROTO((VGROUP *vg));

extern int Vinsert
  PROTO((VGROUP *vg, VDATA *velt)); /* 2nd arg can also be (VGROUP *) */

extern int Vflocate
  PROTO((VGROUP *vg, char *field));

extern int Vinqtagref
  PROTO((VGROUP *vg, int tag, int ref));

extern int Vntagrefs
  PROTO((VGROUP *vg));

extern int Vgettagrefs
  PROTO((VGROUP *vg, int tagarray[], int refarray[], int n));

extern int Vgettagref
  PROTO((VGROUP *vg, int which, int *tag, int *ref));

extern int Vaddtagref
  PROTO((VGROUP *vg, int tag, int ref));

extern int vinsertpair
  PROTO((VGROUP *vg, int tag, int ref));

extern int Ventries
  PROTO((HFILEID f, int vgid));

extern void Vsetname
  PROTO((VGROUP *vg, char *vgname));

extern void Vsetclass
  PROTO((VGROUP *vg, char *vgclass));

extern int Visvg
  PROTO((VGROUP *vg, int id));

extern int Vgetid
  PROTO((HFILEID f, int vgid);

extern int Vgetnext
  PROTO((VGROUP *vg, int id));

extern void Vgetname
  PROTO((VGROUP *vg, char *vgname));

extern void Vgetclass
  PROTO((VGROUP *vg, char *vgclass));

extern int Vinquire
  PROTO((VGROUP *vg, int *nentries, char *vgname));

extern void Vinitialize
  PROTO((HFILEID f));


extern HFILEID Vopen
  PROTO((char *path, intn access, int16 ndds));

extern intn Vclose
  PROTO((HFILEID f));


/*
** from vparse.c
*/
extern int scanattrs
  PROTO((char *attrs, int *attrc, char ***attrv));


/*
** from vhi.c
*/
extern int VHstoredata
  PROTO((HFILEID f, char *field, unsigned char buf[], int32 n, int datatype,
	char *vsname, char *vsclass));

extern int VHstoredatam
  PROTO((HFILEID f, char *field, unsigned char buf[], int32 n, intdatatype,
	char *vsname, char *vsclass, int order));

extern int VHmakegroup
  PROTO((HFILEID  f, int tagarray[], int refarray[], int n, char *vgname,
	char *vgclass));


/*
** from vio.c
*/
extern int vshowvsdir
  PROTO((HFILEID f, char *text));

extern void vshowvg
  PROTO((VGROUP *vg));

extern void vshowvs
  PROTO((VDATA *vs));

extern vsinstance_t * vsinstance
  PROTO((HFILEID f, int vsid));

extern intn DFvsetclose
  PROTO((HFILEID f));

extern HFILEID DFvsetopen
  PROTO((char *fname, int  access, int defDDs));

extern int vexistvs
  PROTO((HFILEID f, int vsid));

extern void vpackvs
  PROTO((VDATA *vs, unsigned char buf[], int *size));

extern void vunpackvs
  PROTO((VDATA *vs, unsigned char buf[], int *size));

extern VDATA * VSattach
  PROTO((HFILEID f, int vsid, char *accesstype));

extern void VSdetach
  PROTO((VDATA *vs));

extern int Visvs
  PROTO((VGROUP *vg, int id));

extern int VSgetid
  PROTO((HFILEID f, int vsid));

extern zerofill
  PROTO(VOIDP x, int n));


/*
** from vsfld.c
*/
extern int SIZEOF
  PROTO((int x));

extern int VSsetfields
  PROTO((VDATA *vs, char *fields));

extern int VSfdefine
  PROTO((VDATA *vs, char *field, int localtype, int order));



/*
** from vrw.c
*/
extern int VSseek
  PROTO((VDATA *vs, int32 eltpos));

extern int VSread
  PROTO((VDATA *vs, unsigned char *buf, int32 nelt, int interlace));

extern int VSwrite
  PROTO((VDATA *vs, unsigned char *buf, int32 nelt, int interlace));

#endif /* PERM_OUT */
#endif /* _H_PROTO */

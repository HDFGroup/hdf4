/***************************************************************************
*
*
*                         NCSA HDF version 3.2r3
*                            December 1, 1992
*
* NCSA HDF Version 3.2 source code and documentation are in the public
* domain.  Specifically, we give to the public domain all rights for future
* licensing of the source code, all resale rights, and all publishing rights.
*
* We ask, but do not require, that the following message be included in all
* derived works:
*
* Portions developed at the National Center for Supercomputing Applications at
* the University of Illinois at Urbana-Champaign, in collaboration with the
* Information Technology Institute of Singapore.
*
* THE UNIVERSITY OF ILLINOIS GIVES NO WARRANTY, EXPRESSED OR IMPLIED, FOR THE
* SOFTWARE AND/OR DOCUMENTATION PROVIDED, INCLUDING, WITHOUT LIMITATION,
* WARRANTY OF MERCHANTABILITY AND WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE
*
****************************************************************************
*/
/*
$Header$

$Log$
Revision 1.11  1993/01/04 18:17:58  sxu
change dspre32 to dsip32s (for dfsdpre32sdg)

 * Revision 1.10  1992/11/05  18:59:26  chouck
 * Added (unix) wrapper to realloc()
 *
 * Revision 1.9  1992/11/02  16:35:41  koziol
 * Updates from 3.2r2 -> 3.3
 *
 * Revision 1.8  1992/10/22  22:53:32  chouck
 * Added group handle to group interface
 *
 * Revision 1.7  1992/10/08  18:27:07  chouck
 * Fixed prototype for DFfindnextref()
 *
 * Revision 1.6  1992/10/01  02:54:34  chouck
 * Added function DF24lastref()
 *
 * Revision 1.5  1992/09/15  19:58:20  koziol
 * CHanged major and minor -> majorv and minorv respectively
 *
 * Revision 1.4  1992/09/11  14:15:04  koziol
 * Changed Fortran stubs' parameter passing to use a new typedef, intf,
 * which should be typed to the size of an INTEGER*4 in whatever Fortran
 * compiler the C functions need to be compatible with.  (This is mostly
 * for the PC and possibly for the Mac)
 *
 * Revision 1.3  1992/08/31  16:14:52  chouck
 * Added Fortran support for calibration tags
 *
 * Revision 1.2  1992/08/27  19:55:26  likkai
 * change declaration of Vclose: now it returns intn instead of void.
 * change declaration of DFvsetclose: returns intn instead of int.
 *
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
  PROTO((char HUGE *path, intn access, int16 ndds));

extern intn Hclose
  PROTO((int32 file_id));

extern int32 Hstartread
  PROTO((int32 file_id, uint16 tag, uint16 ref));

extern intn Hnextread
  PROTO((int32 access_id, uint16 tag, uint16 ref, int origin));

extern intn Hinquire
  PROTO((int32 access_id, int32 HUGE *pfile_id, uint16 HUGE *ptag,
        uint16 HUGE *pref,int32 HUGE *plength, int32 HUGE *poffset,
        int32 HUGE *pposn, int16 HUGE *paccess, int16 HUGE *pspecial));

extern int32 Hstartwrite
  PROTO((int32 file_id, uint16 tag, uint16 ref, int32 length));

extern intn Hseek
  PROTO((int32 access_id, int32 offset, int origin));

extern int32 Hread
  PROTO((int32 access_id, int32 length, uint8 HUGE *data));

extern int32 Hwrite
  PROTO((int32 access_id, int32 length, uint8 HUGE *data));

extern int32 Hendaccess
  PROTO((int32 access_id));

extern int32 Hgetelement
  PROTO((int32 file_id, uint16 tag, uint16 ref, uint8 HUGE *data));

extern int Hputelement
  PROTO((int32 file_id, uint16 tag, uint16 ref, uint8 HUGE *data, int32 length));

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
  PROTO((uint32 HUGE *majorv, uint32 HUGE *minorv,
        uint32 HUGE *release, char HUGE *string));

extern int Hgetfileversion
  PROTO((int32 file_id, uint32 HUGE *majorv, uint32 HUGE *minorv,
        uint32 HUGE *release, char HUGE *string));

#if defined WIN3
extern int32 HDfreadbig
  PROTO((VOIDP buffer, int32 size,HFILE fp));

extern int32 HDfwritebig
  PROTO((VOIDP buffer, int32 size,HFILE fp));

#elif defined PC
extern int32 HDfreadbig
  PROTO((VOIDP buffer, int32 size,FILE HUGE *fp));

extern int32 HDfwritebig
  PROTO((VOIDP buffer, int32 size,FILE HUGE *fp));
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

extern char HUGE *HDgettagname
  PROTO((uint16 tag));

extern int32 Hishdf
  PROTO((char HUGE *filename));

/*
** from hkit.c 
*/
extern char HUGE *HIstrncpy
  PROTO((register char HUGE *dest, register char HUGE *source, int32 len));

extern int32 HDspaceleft
  PROTO((void));

extern void HUGE *HDgetspace
  PROTO((uint32 qty));

extern void HUGE *HDregetspace
  PROTO((VOIDP where, uint32 qty));

extern void HUGE *HDfreespace
  PROTO((void HUGE *ptr));

extern intn HDc2fstr
  PROTO((char HUGE *str, int len));

extern char HUGE *HDf2cstring
  PROTO((_fcd fdesc, int len));

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
  PROTO((int32 file_id, uint16 tag, uint16 ref, char HUGE *extern_file_name));

/*
** from herr.c 
*/
extern char HUGE *HEstring
  PROTO((int16 error_code));

extern VOID HEpush
  PROTO((int16 error_code, char HUGE *function_name, char HUGE *file_name,
        int line));

#ifndef _H_ERR_MASTER_
extern VOID HEreport
  PROTO((char HUGE *, ...));
#endif /* _H_ERR_MASTER_ */

extern VOID HEprint
  PROTO((FILE HUGE *stream, int32 print_level));

extern int16 HEvalue
  PROTO((int32 level));

/* 
** from dfcomp.c 
*/
extern int DFputcomp
  PROTO((int32 file_id, uint16 tag, uint16 ref, uint8 HUGE *image, int32 xdim,
     int32 ydim, uint8 HUGE *palette, uint8 HUGE *newpal, int16 scheme));

extern int DFgetcomp
  PROTO((int32 file_id, uint16 tag, uint16 ref, uint8 HUGE *image, int32 xdim,
	 int32 ydim, uint16 scheme));

/*
** from dfrle.c
*/
extern int32 DFCIrle
  PROTO((VOIDP buf, VOIDP bufto, int32 len));

extern int32 DFCIunrle
  PROTO((uint8 HUGE *buf, uint8 *bufto, int32 outlen, int resetsave));

/*
** from dfimcomp.c 
*/
extern VOID DFCIimcomp
  PROTO((int32 xdim, int32 ydim, uint8 HUGE in[], uint8 HUGE out[],
        uint8 HUGE in_pal[], uint8 HUGE out_pal[], int mode));

extern VOID DFCIunimcomp
  PROTO((int32 xdim, int32 ydim, uint8 HUGE in[], uint8 HUGE out[]));

/* 
** from dfgroup.c 
*/
extern int32 DFdiread
  PROTO((int32 file_id, uint16 tag, uint16 ref));

extern int DFdiget
  PROTO((int32 list, uint16 HUGE *ptag, uint16 HUGE *pref));

extern int32 DFdisetup
  PROTO((int maxsize));

extern int DFdiput
  PROTO((int32 list, uint16 tag, uint16 ref));

extern int DFdiwrite
  PROTO((int32 file_id, int32 list, uint16 tag, uint16 ref));

/*
** from dfp.c 
*/
extern intn DFPgetpal
  PROTO((char HUGE *filename, VOIDP palette));

extern intn DFPputpal
  PROTO((char HUGE *filename, VOIDP palette, int overwrite, char HUGE *filemode));

extern int DFPaddpal
  PROTO((char HUGE *filename, VOIDP palette));

extern int DFPnpals
  PROTO((char HUGE *filename));

extern intn DFPreadref
  PROTO((char HUGE *filename, uint16 ref));

extern int DFPwriteref
  PROTO((char HUGE *filename, uint16 ref));

extern int DFPrestart
  PROTO((void));

extern int DFPlastref
  PROTO((void));

extern int32 DFPIopen
  PROTO((char HUGE *filename, int access));

/*
** from dfr8.c 
*/
extern intn DFR8getdims
  PROTO((char HUGE *filename, int32 HUGE *pxdim, int32 HUGE *pydim,
        int HUGE *pispal));

extern intn DFR8getimage
  PROTO((char HUGE *filename, uint8 HUGE *image, int32 xdim, int32 ydim,
        uint8 HUGE *pal));

extern int DFR8setpalette
  PROTO((uint8 HUGE *pal));

extern int DFR8putimage
  PROTO((char HUGE *filename, VOIDP image, int32 xdim, int32 ydim, uint16 compress));

extern int DFR8addimage
  PROTO((char HUGE *filename, VOIDP image, int32 xdim, int32 ydim, uint16 compress));

extern int DFR8nimages
  PROTO((char HUGE *filename));

extern intn DFR8readref
  PROTO((char HUGE *filename, uint16 ref));

extern int DFR8writeref
  PROTO((char HUGE *filename, uint16 ref));

extern int DFR8restart
  PROTO((void));

extern int DFR8lastref
  PROTO((void));

/*
** from dfgr.c 
*/
extern int DFGRgetlutdims
  PROTO((char HUGE *filename, int32 HUGE *pxdim, int32 HUGE *pydim,
        int HUGE *pncomps, int HUGE *pil));

extern int DFGRreqlutil
  PROTO((int il));

extern int DFGRgetlut
  PROTO((char HUGE *filename, VOIDP lut, int32 xdim, int32 ydim));

extern int DFGRgetimdims
  PROTO((char HUGE *filename, int32 HUGE *pxdim, int32 HUGE *pydim,
        int HUGE *pncomps, int HUGE *pil));

extern int DFGRreqimil
  PROTO((int il));

extern int DFGRgetimage
  PROTO((char HUGE *filename, VOIDP image, int32 xdim, int32 ydim));

extern int DFGRsetcompress
  PROTO((int scheme));

extern int DFGRsetlutdims
  PROTO((int32 xdim, int32 ydim, int ncomps, int il));

extern int DFGRsetlut
  PROTO((VOIDP lut, int32 xdim, int32 ydim));

extern int DFGRaddlut
  PROTO((char HUGE *filename, VOIDP lut, int32 xdim, int32 ydim));

extern int DFGRsetimdims
  PROTO((int32 xdim, int32 ydim, int ncomps, int il));

extern int DFGRaddimage
  PROTO((char HUGE *filename, VOIDP image, int32 xdim, int32 ydim));

extern int DFGRputimage
  PROTO((char HUGE *filename, VOIDP image, int32 xdim, int32 ydim));

extern int DFGRreadref
  PROTO((char HUGE *filename, uint16 ref));

extern uint16 DFGRIlastref
    PROTO((void));

extern int DFGRIgetdims
  PROTO((char HUGE *filename, int32 HUGE *pxdim, int32 HUGE *pydim,
        int HUGE *pncomps, int HUGE *pil, int type));

extern int DFGRIreqil
  PROTO((int il, int type));

extern int DFGRIgetimlut
  PROTO((char HUGE *filename, VOIDP imlut, int32 xdim, int32 ydim, int type,
	 int isfortran));

extern int DFGRIsetdims
  PROTO((int32 xdim, int32 ydim, int ncomps, int type));

extern int DFGRIsetil
  PROTO((int il, int type));

extern int DFGRIrestart
  PROTO((void));

extern int DFGRIaddimlut
  PROTO((char HUGE *filename, VOIDP imlut, int32 xdim, int32 ydim, int type,
	 int isfortran, int newfile));

/*
** from df24.c 
*/
extern int DF24getdims
  PROTO((char HUGE *filename, int32 HUGE *pxdim, int32 HUGE *pydim,
        int HUGE *pil));

extern int DF24reqil
  PROTO((int il));

extern int DF24getimage
  PROTO((char HUGE *filename, VOIDP image, int32 xdim, int32 ydim));

extern int DF24setdims
  PROTO((int32 xdim, int32 ydim));

extern int DF24setil
  PROTO((int il));

extern int DF24restart
  PROTO((void));

extern int DF24addimage
  PROTO((char HUGE *filename, VOIDP image, int32 xdim, int32 ydim));

extern int DF24putimage
  PROTO((char HUGE *filename, VOIDP image, int32 xdim, int32 ydim));

extern int DF24readref
  PROTO((char HUGE *filename, uint16 ref));

extern uint16 DF24lastref
    PROTO((void));

/*
** from dfan.c
*/

extern int32 DFANgetlablen
    PROTO((char HUGE *filename, uint16 tag, uint16 ref));

extern int DFANgetlabel
    PROTO((char HUGE *filename, uint16 tag, uint16 ref, char HUGE *label,
            int32 maxlen));

extern int32 DFANgetdesclen
    PROTO((char HUGE *filename, uint16 tag, uint16 ref));

extern int DFANgetdesc
    PROTO((char HUGE *filename, uint16 tag, uint16 ref, char HUGE *desc,
            int32 maxlen));

extern int32 DFANgetfidlen
    PROTO((int32 file_id, int isfirst));

extern int32 DFANgetfid
    PROTO((int32 file_id, char HUGE *id, int32 maxlen, int isfirst));

extern int32 DFANgetfdslen
    PROTO((int32 file_id, int isfirst));

extern int32 DFANgetfds
    PROTO((int32 file_id, char HUGE *desc, int32 maxlen, int isfirst));

extern int DFANputlabel
    PROTO((char HUGE *filename, uint16 tag, uint16 ref, char HUGE *label));

extern int DFANputdesc
    PROTO((char HUGE *filename, uint16 tag, uint16 ref, char HUGE *desc,
            int32 desclen));

extern int DFANaddfid
    PROTO((int32 file_id, char HUGE *id));

extern int DFANaddfds
    PROTO((int32 file_id, char HUGE *desc, int32 desclen));

extern uint16 DFANlastref
    PROTO((void));

extern int DFANlablist
    PROTO((char HUGE *filename, uint16 tag, uint16 HUGE reflist[],
            char HUGE *labellist, int listsize, int maxlen, int startpos));

extern uint16 DFANIlocate
  PROTO((int32 file_id, int type, uint16 tag, uint16 ref));

extern int DFANIaddentry
  PROTO((int type, uint16 annref, uint16 datatag, uint16 dataref));

extern int32 DFANIgetannlen
  PROTO((char HUGE *filename, uint16 tag, uint16 ref, int type));

extern intn DFANIgetann
  PROTO((char HUGE *filename, uint16 tag, uint16 ref, uint8 HUGE *ann,
            int32 maxlen, int type));

extern intn DFANIputann
  PROTO((char HUGE *filename, uint16 tag, uint16 ref, uint8 HUGE *ann,
            int32 annlen, int type));

extern int DFANIlablist
  PROTO((char HUGE *filename, uint16 tag, uint16 HUGE reflist[],
            uint8 HUGE *labellist, int listsize, int maxlen, int startpos,
            int isfortran));

extern int DFANIaddfann
  PROTO((int32 file_id, char HUGE *ann, int32 annlen, int type));

extern int32 DFANIgetfannlen
  PROTO((int32 file_id, int type, int isfirst));

extern int32 DFANIgetfann
  PROTO((int32 file_id, char HUGE *ann, int32 maxlen, int type, int isfirst));

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
    PROTO((uint8 HUGE *source, uint8 HUGE *dest, int32 ntype, int32 num_elm,
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

extern FRETVAL(intf) ndaiganl
    PROTO((_fcd filename, intf HUGE *tag, intf HUGE *ref, intf HUGE *type,
            intf HUGE *fnlen));

extern FRETVAL(intf) ndaigann
    PROTO((_fcd filename, intf HUGE *tag, intf HUGE *ref, _fcd annotation,
        intf HUGE *maxlen, intf HUGE *type, intf HUGE *fnlen));

extern FRETVAL(intf) ndaipann
    PROTO((_fcd filename, intf HUGE *tag, intf HUGE *ref, _fcd annotation,
            intf HUGE *annlen, intf HUGE *type, intf HUGE *fnlen));

extern FRETVAL(intf) ndailist
    PROTO((_fcd filename, intf HUGE *tag, intf HUGE reflist[], _fcd labellist,
        intf HUGE *listsize, intf HUGE *maxlen, intf HUGE *startpos,
        intf HUGE *fnlen));

extern FRETVAL(intf) ndalref
    PROTO((void));

extern FRETVAL(int) ndfanlastref
    PROTO((void));

extern FRETVAL(intf) ndfanaddfds
    PROTO((intf HUGE *dfile, _fcd desc, intf HUGE *desclen));

extern FRETVAL(intf) ndfangetfidlen
    PROTO((intf HUGE *dfile, intf HUGE *isfirst));

extern FRETVAL(intf) ndfangetfdslen
    PROTO((intf HUGE *dfile, intf HUGE *isfirst));

extern FRETVAL(intf) ndfangetfid
    PROTO((intf HUGE *dfile, _fcd id, intf HUGE *maxlen, intf HUGE *isfirst));

extern FRETVAL(intf) ndfangetfds
    PROTO((intf HUGE *dfile, _fcd id, intf HUGE *maxlen, intf HUGE *isfirst));

extern FRETVAL(intf) ndaafds
    PROTO((intf HUGE *dfile, _fcd desc, intf HUGE *desclen));

extern FRETVAL(intf) ndagfidl
    PROTO((intf HUGE *dfile, intf HUGE *isfirst));

extern FRETVAL(intf) ndagfdsl
    PROTO((intf HUGE *dfile, intf HUGE *isfirst));

extern FRETVAL(intf) ndagfid
  PROTO((intf HUGE *dfile, _fcd id, intf HUGE *maxlen, intf HUGE *isfirst));

extern FRETVAL(intf) ndagfds
  PROTO((intf HUGE *dfile, _fcd id, intf HUGE *maxlen, intf HUGE *isfirst));

extern FRETVAL(intf) ndaiafid
  PROTO((intf HUGE *dfile, _fcd id, intf HUGE *idlen));

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

extern  FRETVAL(intf) nd8spal
    PROTO((_fcd pal));

extern  FRETVAL(intf) nd8first
    PROTO((void));

extern  FRETVAL(intf) nd8igdim
    PROTO((_fcd filename, intf HUGE *xdim, intf HUGE *ydim, intf HUGE *ispal,
            intf HUGE *lenfn));

extern  FRETVAL(intf) nd8igimg
    PROTO((_fcd filename, _fcd image, intf HUGE *xdim, intf HUGE *ydim,
            _fcd pal, intf HUGE *lenfn));

extern  FRETVAL(intf) nd8ipimg
    PROTO((_fcd filename, _fcd image, intf HUGE *xdim, intf HUGE *ydim,
            intf HUGE *compress, intf HUGE *lenfn));

extern  FRETVAL(intf) nd8iaimg
    PROTO((_fcd filename, _fcd image, intf HUGE *xdim, intf HUGE *ydim,
            intf HUGE *compress, intf HUGE *lenfn));

extern  FRETVAL(intf) nd8irref
    PROTO((_fcd filename, intf HUGE *ref, intf HUGE *fnlen));

extern FRETVAL(intf) nd8iwref
    PROTO((_fcd filename, intf HUGE *ref, intf HUGE *fnlen));

extern FRETVAL(intf) nd8inims
    PROTO((_fcd filename, intf HUGE *fnlen));

extern FRETVAL(intf) nd8lref
    PROTO((void));

extern FRETVAL(intf) ndfr8lastref
    PROTO((void));

extern  FRETVAL(intf) ndfr8setpalette
    PROTO((_fcd pal));

extern  FRETVAL(intf) ndfr8restart
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
#   define ndsip32s  FNAME(DSIP32S)	
#   define ndsscal   FNAME(DSSCAL)
#   define ndsgcal   FNAME(DSGCAL)
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
#   define ndsip32s  FNAME(dsip32s)
#   define ndsscal   FNAME(dsscal)
#   define ndsgcal   FNAME(dsgcal)
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

extern FRETVAL(intf) ndsgdisc
    PROTO((intf HUGE *dim, intf HUGE *maxsize, void HUGE *scale));

extern FRETVAL(intf) ndsgrang
    PROTO((void HUGE *pmax, void HUGE *pmin));

extern FRETVAL(intf) ndssdims
    PROTO((intf HUGE *rank, intf HUGE dimsizes[]));

extern FRETVAL(intf) ndssdisc
    PROTO((intf HUGE *dim, intf HUGE *dimsize, void HUGE *scale));

extern FRETVAL(intf) ndssrang
    PROTO((void HUGE *max, void HUGE *min));

extern FRETVAL(intf) ndsclear
    PROTO((void));

extern FRETVAL(intf) ndsslens
    PROTO((intf HUGE *maxlen_label, intf HUGE *maxlen_unit, \
            intf HUGE *maxlen_format, intf HUGE *maxlen_coordsys));

extern FRETVAL(intf) ndsgdiln
    PROTO((intf HUGE *dim, intf HUGE *llabel, intf HUGE *lunit,
            intf HUGE *lformat));

extern FRETVAL(intf) ndsgdaln
    PROTO((intf HUGE *llabel, intf HUGE *lunit, intf HUGE *lformat,
            intf HUGE *lcoordsys));

extern FRETVAL(intf) ndsfirst
    PROTO((void));

extern FRETVAL(intf) ndspslc
    PROTO((intf HUGE windims[], void HUGE *data, intf HUGE dims[]));

extern FRETVAL(intf) ndseslc
    PROTO((void));

extern FRETVAL(intf) ndssnt
    PROTO((intf HUGE *numbertype));

extern FRETVAL(intf) ndsgnt
    PROTO((intf HUGE *pnumbertype));

extern FRETVAL(intf) ndsigdim
    PROTO((_fcd filename, intf HUGE *prank, intf HUGE sizes[],
            intf HUGE *maxrank, intf HUGE *lenfn));

extern FRETVAL(intf) ndsigdat
    PROTO((_fcd filename, intf HUGE *rank, intf HUGE maxsizes[],
            void HUGE *data, intf HUGE *fnlen));

extern FRETVAL(intf) ndsipdat
    PROTO((_fcd filename, intf HUGE *rank, intf HUGE dimsizes[],
            void HUGE *data, intf HUGE *fnlen));

extern FRETVAL(intf) ndsiadat
    PROTO((_fcd filename, intf HUGE *rank, intf HUGE dimsizes[],
            void HUGE *data, intf HUGE *fnlen));

extern FRETVAL(intf) ndsigslc
    PROTO((_fcd filename, intf HUGE winst[], intf HUGE windims[],
            void HUGE *data, intf HUGE dims[], intf HUGE *fnlen));

extern FRETVAL(intf) ndsisslc
    PROTO((_fcd filename, intf HUGE *fnlen));

extern FRETVAL(intf) ndsirref
    PROTO((_fcd filename, intf HUGE *ref, intf HUGE *fnlen));

extern FRETVAL(intf) ndslref
    PROTO((void));

extern FRETVAL(intf) ndsinum
    PROTO((_fcd filename, intf HUGE *len));

extern FRETVAL(intf) ndsip32s
    PROTO((_fcd filename, intf HUGE *ref, intf HUGE *ispre32, intf HUGE *len));

extern FRETVAL(intf) ndfsdgetdatastrs
    PROTO((_fcd label, _fcd unit, _fcd format, _fcd coordsys));

extern FRETVAL(intf) ndfsdgetdimstrs
    PROTO((intf HUGE *dim, _fcd label, _fcd unit, _fcd format));

extern FRETVAL(intf) ndfsdgetdimscale
    PROTO((intf HUGE *dim, intf HUGE *maxsize, void HUGE *scale));

extern FRETVAL(intf) ndfsdgetrange
    PROTO((void HUGE *pmax, void HUGE *pmin));

extern FRETVAL(intf) ndfsdsetdims
    PROTO((intf HUGE *rank, intf HUGE dimsizes[]));

extern FRETVAL(intf) ndfsdsetdimscale
    PROTO((intf HUGE *dim, intf HUGE *dimsize, void HUGE *scale));

extern FRETVAL(intf) ndfsdsetrange
    PROTO((void HUGE *max, void HUGE *min));

extern FRETVAL(intf) ndfsdclear
    PROTO((void));

extern FRETVAL(intf) ndfsdsetlengths
    PROTO((intf HUGE *maxlen_label, intf HUGE *maxlen_unit,
            intf HUGE *maxlen_format, intf HUGE *maxlen_coordsys));

extern FRETVAL(intf) ndfsdgetdimlen
    PROTO((intf HUGE *dim, intf HUGE *llabel, intf HUGE *lunit,
            intf HUGE *lformat));

extern FRETVAL(intf) ndfsdgetdatalen
    PROTO((intf HUGE *llabel, intf HUGE *lunit, intf HUGE *lformat,
            intf HUGE *lcoordsys));

extern FRETVAL(intf) ndfsdrestart
    PROTO((void));

extern FRETVAL(intf) ndfsdputslice
    PROTO((intf HUGE windims[], void HUGE *data, intf HUGE dims[]));

extern FRETVAL(intf) ndfsdendslice
    PROTO((void));

extern FRETVAL(intf) ndfsdpre32
    PROTO((void));

extern FRETVAL(intf) ndfsdsetnt
    PROTO((intf HUGE *numbertype));

extern FRETVAL(intf) ndfsdgetnt
    PROTO((intf HUGE *pnumbertype));

extern FRETVAL(intf) ndfsdlastref
    PROTO((void));

extern FRETVAL(intf) ndsisdis
    PROTO((intf HUGE *dim, _fcd flabel, _fcd funit, _fcd fformat,
            intf HUGE *llabel, intf HUGE *lunit, intf HUGE *lformat));

extern FRETVAL(intf) ndsigdis
    PROTO((intf HUGE *dim, _fcd label, _fcd unit, _fcd format,
            intf HUGE *llabel, intf HUGE *lunit, intf HUGE *lformat));

extern FRETVAL(intf) ndsisdas
    PROTO((_fcd flabel, _fcd funit, _fcd fformat, _fcd fcoordsys,
        intf HUGE *isfortran, intf HUGE *llabel, intf HUGE *lunit,
        intf HUGE *lformat, intf HUGE *lcoordsys));

extern FRETVAL(intf) ndsigdas
    PROTO((_fcd label, _fcd unit, _fcd format, _fcd coordsys, intf HUGE *llabel,
        intf HUGE *lunit, intf HUGE *lformat, intf HUGE *lcoord));

extern FRETVAL(intf) ndsscal
    PROTO((float64 HUGE *cal, float64 HUGE *cal_err, float64 HUGE *ioff,
        float64 HUGE *ioff_err, intf HUGE *cal_type));

extern FRETVAL(intf) ndsgcal
    PROTO((float64 HUGE *cal, float64 HUGE *cal_err, float64 HUGE *ioff,
        float64 HUGE *ioff_err, intf HUGE *cal_type));

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

extern  FRETVAL(intf) ndpigpal
    PROTO((_fcd filename, _fcd pal, intf HUGE *fnlen));

extern FRETVAL(intf) ndpippal
    PROTO((_fcd filename, _fcd pal, intf HUGE *overwrite, _fcd filemode,
            intf HUGE *fnlen));

extern FRETVAL(intf) ndpinpal
    PROTO((_fcd filename, intf HUGE *fnlen));

extern FRETVAL(intf) ndpirref
    PROTO((_fcd filename, uint16 HUGE *ref, intf HUGE *fnlen));

extern FRETVAL(intf) ndpiwref
    PROTO((_fcd filename, uint16 HUGE *ref, intf HUGE *fnlen));

extern FRETVAL(intf) ndprest
    PROTO((void));

extern FRETVAL(intf) ndplref
    PROTO((void));

extern FRETVAL(intf) ndfprestart
    PROTO((void));

extern FRETVAL(intf) ndfplastref
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
#   define nd24lref  FNAME(D24LREF)
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
#   define nd24lref  FNAME(d24lref)
#endif /* DF_CAPFNAMES */
#endif /* DF24_FNAMES */

extern FRETVAL(intf) nd2reqil
    PROTO((intf HUGE *il));

extern FRETVAL(intf) nd2sdims
    PROTO((intf HUGE *xdim, intf HUGE *ydim));

extern FRETVAL(intf) nd2igdim
    PROTO((_fcd filename, intf HUGE *pxdim, intf HUGE *pydim, int HUGE *pil,
            int HUGE *fnlen));

extern FRETVAL(intf) nd2igimg
    PROTO((_fcd filename, _fcd image, intf HUGE *xdim, intf HUGE *ydim,
            intf HUGE *fnlen));

extern FRETVAL(intf) nd2iaimg
    PROTO((_fcd filename, _fcd image, intf HUGE *xdim, intf HUGE *ydim,
            intf HUGE *fnlen, intf HUGE *newfile));

extern FRETVAL(intf) nd2setil
    PROTO((intf HUGE *il));

extern FRETVAL(intf) nd2first
    PROTO((void));

extern FRETVAL(intf) ndf24reqil
    PROTO((intf HUGE *il));

extern FRETVAL(intf) ndf24setdims
    PROTO((intf HUGE *xdim, intf HUGE *ydim));

extern FRETVAL(intf) ndf24setil
    PROTO((intf HUGE *il));

extern FRETVAL(intf) ndf24restart
    PROTO((void));

extern FRETVAL(intf) nd2irref
    PROTO((_fcd filename, intf HUGE *ref, intf HUGE *fnlen));

extern FRETVAL(intf) nd24lref
         PROTO((void));

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

extern FRETVAL(intf) ndfiopen
    PROTO((_fcd name, intf HUGE *access, intf HUGE *defdds, intf HUGE *namelen));

extern FRETVAL(intf) ndfclose
    PROTO((intf HUGE *dfile));

extern FRETVAL(intf) ndfdesc
    PROTO((intf HUGE *dfile, intf HUGE ptr[][4], intf HUGE *begin,
            intf HUGE *num));

extern FRETVAL(intf) ndfdup
    PROTO((intf HUGE *dfile, intf HUGE *tag, intf HUGE *ref, intf HUGE *otag,
            intf HUGE *oref));

extern FRETVAL(intf) ndfdel
    PROTO((intf HUGE *dfile, intf HUGE *tag, intf HUGE *ref));

extern FRETVAL(intf) ndfiaccess
    PROTO((intf HUGE *dfile, intf HUGE *tag, intf HUGE *ref, _fcd access, intf HUGE *acclen));

extern FRETVAL(intf) ndfstart
    PROTO((intf HUGE *dfile, intf HUGE *tag, intf HUGE *ref, char HUGE *access));

extern FRETVAL(intf) ndfread
    PROTO((intf HUGE *dfile, _fcd ptr, intf HUGE *len));

extern FRETVAL(intf) ndfseek
    PROTO((intf HUGE *dfile, intf HUGE *offset));

extern FRETVAL(intf) ndfwrite
    PROTO((intf HUGE *dfile, _fcd ptr, intf HUGE *len));

extern FRETVAL(intf) ndfupdate
    PROTO((intf HUGE *dfile));

extern FRETVAL(intf) ndfget
    PROTO((intf HUGE *dfile, intf HUGE *tag, intf HUGE *ref, _fcd ptr));

extern FRETVAL(intf) ndfput
    PROTO((intf HUGE *dfile, intf HUGE *tag, intf HUGE *ref, _fcd ptr, intf HUGE *len));

extern FRETVAL(intf) ndfsfind
    PROTO((intf HUGE *dfile, intf HUGE *tag, intf HUGE *ref));

extern FRETVAL(intf) ndffind
    PROTO((intf HUGE *dfile, intf HUGE *itag, intf HUGE *iref, intf HUGE *len));

extern FRETVAL(intf) ndferrno
    PROTO((void));

extern FRETVAL(intf) ndfnewref
    PROTO((intf HUGE *dfile));

extern FRETVAL(intf) ndfnumber
    PROTO((intf HUGE *dfile, intf HUGE *tag));

#ifdef TEMP_OUT
extern FRETVAL(intf) ndfstat
    PROTO((intf HUGE *dfile, DFdata HUGE *dfinfo));
#endif

extern FRETVAL(intf) ndfiishdf
    PROTO((_fcd name, intf HUGE *namelen));

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

extern FRETVAL(intf) ndfindnr
    PROTO((intf HUGE *dfile, intf HUGE *tag, intf HUGE *lref));

extern FRETVAL(intf) ndffindnextref
    PROTO((intf HUGE *dfile, intf HUGE *tag, intf HUGE *lref));

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
    PROTO((intf HUGE *print_levels));

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

extern FRETVAL(intf) nhiopen
    PROTO((_fcd name, intf HUGE *access, intf HUGE *defdds, intf HUGE *namelen));

extern FRETVAL(intf) nhclose
    PROTO((intf HUGE *file_id));

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
    PROTO((int32 HUGE *hdim, int32 HUGE *vdim, float32 HUGE *max,
            float32 HUGE *min, float32 HUGE hscale[], float32 HUGE vscale[],
            float32 HUGE data[], uint8 HUGE *palette, _fcd outfile,
            int HUGE *ct_method, int32 HUGE *hres, int32 HUGE *vres,
            int HUGE *compress, int HUGE *lenfn));

extern int DFUfptoimage
    PROTO((int32 hdim, int32 vdim, float32 max, float32 min,
            float32 HUGE *hscale, float32 HUGE *vscale, float32 HUGE *data,
            uint8 HUGE *palette, char HUGE *outfile, int ct_method,
            int32 hres, int32 vres, int compress));

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

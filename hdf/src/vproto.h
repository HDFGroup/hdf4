/*
$Header$

$Log$
Revision 1.6  1993/01/19 05:56:31  koziol
Merged Hyperslab and JPEG routines with beginning of DEC ALPHA
port.  Lots of minor annoyances fixed.

 * Revision 1.5  1992/11/30  22:00:01  chouck
 * Added fixes for changing to Vstart and Vend
 *
 * Revision 1.4  1992/11/24  17:43:26  chouck
 * Fixed memory over-write when VGroups have lots of members
 *
 * Revision 1.3  1992/11/02  16:35:41  koziol
 * Updates from 3.2r2 -> 3.3
 *
 * Revision 1.2  1992/09/11  14:15:04  koziol
 * Changed Fortran stubs' parameter passing to use a new typedef, intf,
 * which should be typed to the size of an INTEGER*4 in whatever Fortran
 * compiler the C functions need to be compatible with.  (This is mostly
 * for the PC and possibly for the Mac)
 *
 * Revision 1.1  1992/08/25  21:40:44  koziol
 * Initial revision
 *
*/

/*
** from vconv.c
*/

extern int32 vicheckcompat
  PROTO((HFILEID f));

extern int32 vimakecompat
  PROTO((HFILEID f));

extern int32 vcheckcompat
  PROTO((char _HUGE *fs));

extern int32 vmakecompat
  PROTO((char _HUGE *fs));

extern void oldunpackvg
  PROTO((VGROUP _HUGE *vg, BYTE _HUGE buf[], int32 _HUGE *size));

extern void oldunpackvs
  PROTO((VDATA _HUGE *vs, BYTE _HUGE buf[], int32 _HUGE *size));

/*
** from vg.c
*/
extern uint16 vnewref
  PROTO((HFILEID f));

extern int32 VSelts
  PROTO((VDATA _HUGE *vs));

extern int32 VSgetinterlace
  PROTO((VDATA _HUGE *vs));

extern int32 VSsetinterlace
  PROTO((VDATA _HUGE *vs, int32 interlace));

extern int32 VSgetfields
  PROTO((VDATA _HUGE *vs, char _HUGE *fields));

extern int32 VSfexist
  PROTO((VDATA _HUGE *vs, char _HUGE *fields));

extern int32 VSsizeof
  PROTO((VDATA _HUGE *vs, char _HUGE *fields));

/* private */
extern int32 matchnocase
  PROTO((char _HUGE *strx, char _HUGE *stry));

extern void VSdump
  PROTO((VDATA _HUGE *vs));

extern void VSsetname
  PROTO((VDATA _HUGE *vs, char _HUGE *vsname));

extern void VSsetclass
  PROTO((VDATA _HUGE *vs, char _HUGE *vsclass));

extern void VSgetname
  PROTO((VDATA _HUGE *vs, char _HUGE *vsname));

extern void VSgetclass
  PROTO((VDATA _HUGE *vs, char _HUGE *vsclass));

extern int32 VSinquire
  PROTO((VDATA _HUGE *vs,int32 _HUGE *nelt, int32 _HUGE *interlace, char _HUGE *fields, int32 _HUGE *eltsize,
    char _HUGE *vsname));

extern int32 VSlone
  PROTO((HFILEID f, int32 _HUGE idarray[], int32 asize));

extern int32 Vlone
  PROTO((HFILEID f, int32 _HUGE idarray[], int32 asize));

extern int32 Vfind
  PROTO((HFILEID f, char _HUGE *vgname));

extern int32 VSfind
  PROTO((HFILEID f, char _HUGE *vsname));

extern void Vsetzap
    PROTO((void));

/*
** from vgp.c
*/
extern void setjj
  PROTO((void));

extern void setnojj
  PROTO((void));

extern void Vstart
  PROTO((HFILEID f));

extern intn Vend
  PROTO((HFILEID f));

/*private */
extern vginstance_t _HUGE *vginstance
  PROTO((HFILEID f, uint16 vgid));

extern int32 vexistvg
  PROTO((HFILEID f, uint16 vgid));

extern void vpackvg
  PROTO((VGROUP _HUGE *vg, BYTE _HUGE buf[], int32 _HUGE *size));

extern void vunpackvg
  PROTO((VGROUP _HUGE *vg, BYTE _HUGE buf[]));

extern VGROUP _HUGE *Vattach
  PROTO((HFILEID f, int32 vgid, char _HUGE *accesstype));

extern void Vdetach
  PROTO((VGROUP _HUGE *vg));

extern int32 Vinsert
  PROTO((VGROUP _HUGE *vg, VDATA _HUGE *velt));
  /* note: 2nd arg of Vinsert can also be (VGROUP *) */

extern int32 Vflocate
  PROTO((VGROUP _HUGE *vg, char _HUGE *field));

extern int32 Vinqtagref
  PROTO((VGROUP _HUGE *vg, int32 tag, int32 ref));

extern int32 Vntagrefs
  PROTO((VGROUP _HUGE *vg));

extern int32 Vgettagrefs
  PROTO((VGROUP _HUGE *vg, int32 _HUGE tagarray[], int32 _HUGE refarray[], int32 n));

extern int32 Vgettagref
  PROTO((VGROUP _HUGE *vg, int32 which, int32 _HUGE *tag, int32 _HUGE *ref));

extern int32 Vaddtagref
  PROTO((VGROUP _HUGE *vg, int32 tag, int32 ref));

extern int32 vinsertpair
  PROTO((VGROUP _HUGE *vg, uint16 tag, uint16 ref));

extern int32 Ventries
  PROTO((HFILEID f, int32 vgid));

extern void Vsetname
  PROTO((VGROUP _HUGE *vg, char _HUGE *vgname));

extern void Vsetclass
  PROTO((VGROUP _HUGE *vg, char _HUGE *vgclass));

extern int32 Visvg
  PROTO((VGROUP _HUGE *vg, int32 id));

extern int32 Vgetid
  PROTO((HFILEID f, int32 vgid));

extern int32 Vgetnext
  PROTO((VGROUP _HUGE *vg, int32 id));

extern void Vgetname
  PROTO((VGROUP _HUGE *vg, char _HUGE *vgname));

extern void Vgetclass
  PROTO((VGROUP _HUGE *vg, char _HUGE *vgclass));

extern int32 Vinquire
  PROTO((VGROUP _HUGE *vg, int32 _HUGE *nentries, char _HUGE *vgname));


/*
** from vparse.c
*/
extern int32 scanattrs
  PROTO((char _HUGE *attrs, int32 _HUGE *attrc, char _HUGE ***attrv));


/*
** from vhi.c
*/
extern int32 VHstoredata
  PROTO((HFILEID f, char _HUGE *field, BYTE _HUGE buf[], int32 n, int32 datatype,
    char _HUGE *vsname, char _HUGE *vsclass));

extern int32 VHstoredatam
  PROTO((HFILEID f, char _HUGE *field, BYTE _HUGE buf[], int32 n, int32 datatype,
    char _HUGE *vsname, char _HUGE *vsclass, int32 order));

extern int32 VHmakegroup
  PROTO((HFILEID  f, int32 _HUGE tagarray[], int32 _HUGE refarray[], int32 n, char _HUGE *vgname,
    char _HUGE *vgclass));


/*
** from vio.c
*/

extern vsinstance_t _HUGE * vsinstance
    PROTO((HFILEID f, uint16 vsid));

extern int32 DFvsetclose
    PROTO((HFILEID f));

extern HFILEID DFvsetopen
    PROTO((char _HUGE *fname, int16  access, int16 defDDs));

extern int32 vexistvs
    PROTO((HFILEID f, uint16 vsid));

extern void vpackvs
    PROTO((VDATA _HUGE *vs, BYTE _HUGE buf[], int32 _HUGE *size));

extern void vunpackvs
    PROTO((VDATA _HUGE *vs, BYTE _HUGE buf[], int32 _HUGE *size));

extern VDATA _HUGE * VSattach
    PROTO((HFILEID f, int32 vsid, char _HUGE *accesstype));

extern void VSdetach
    PROTO((VDATA _HUGE *vs));

extern int32 Visvs
    PROTO((VGROUP _HUGE *vg, int32 id));

extern int32 VSgetid
    PROTO((HFILEID f, int32 vsid));

/*
** from vsfld.c
*/

extern int16 SIZEOF
    PROTO((int16 x));

extern int16 HDFSIZEOF
    PROTO((int16 x));

extern int32 VSsetfields
  PROTO((VDATA _HUGE *vs, char _HUGE *fields));

extern int32 VSfdefine
  PROTO((VDATA _HUGE *vs, char _HUGE *field, int32 localtype, int32 order));

/*
** from vrw.c
*/
extern int32 VSseek
  PROTO((VDATA _HUGE *vs, int32 eltpos));

extern int32 VSread
  PROTO((VDATA _HUGE *vs, BYTE _HUGE buf[], int32 nelt, int32 interlace));

extern int32 VSwrite
  PROTO((VDATA _HUGE *vs, BYTE _HUGE buf[], int32 nelt, int32 interlace));

/*
** from vgF.c
*/
#ifndef VG_FNAMES
#   define VG_FNAMES
#ifdef DF_CAPFNAMES
#	define  ndfivopn FNAME(DFIVOPN)
#	define  ndfvclos FNAME(DFVCLOS)
#   define  nvatchc  FNAME(VATCHC)
#   define  nvdtchc  FNAME(VDTCHC)
#   define  nvgnamc  FNAME(VGNAMC)
#   define  nvgclsc  FNAME(VGCLSC)
#   define  nvinqc   FNAME(VINQC)
#   define  nvgidc   FNAME(VGIDC)
#   define  nvgnxtc  FNAME(VGNXTC)
#   define  nvsnamc  FNAME(VSNAMC)
#   define  nvsclsc  FNAME(VSCLSC)
#   define  nvinsrtc FNAME(VINSRTC)
#   define  nvisvgc  FNAME(VISVGC)
#   define  nvisvsc  FNAME(VISVSC)
#   define  nvsatchc FNAME(VSATCHC)
#   define  nvsdtchc FNAME(VSDTCHC)
#   define  nvsseekc FNAME(VSSEEKC)
#   define  nvsgnamc FNAME(VSGNAMC)
#   define  nvsgclsc FNAME(VSGCLSC)
#   define  nvsinqc  FNAME(VSINQC)
#   define  nvsfexc  FNAME(VSFEXC)
#   define  nvsgidc  FNAME(VSGIDC)
#   define  nvssnamc FNAME(VSSNAMC)
#   define  nvssclsc FNAME(VSSCLSC)
#   define  nvssfldc FNAME(VSSFLDC)
#   define  nvssintc FNAME(VSSINTC)
#   define  nvsfdefc FNAME(VSFDEFC)
#   define  nvsreadc FNAME(VSREADC)
#   define  nvswritc FNAME(VSWRITC)
#   define  nvsgintc FNAME(VSGINTC)
#   define  nvseltsc FNAME(VSELTSC)
#   define  nvsgfldc FNAME(VSGFLDC)
#   define  nvssizc  FNAME(VSSIZC)
#   define  nventsc  FNAME(VENTSC)
#   define  nsetjjc  FNAME(SETJJC)
#   define  nsetnojjc    FNAME(SETNOJJC)
#   define  nvlonec  FNAME(VLONEC)
#   define  nvslonec FNAME(VSLONEC)
#   define  nvhsdc   FNAME(VHSDC)
#   define  nvhsdmc  FNAME(VHSDMC)
#   define  nvhmkgpc FNAME(VHMKGPC)
#   define  nvflocc  FNAME(VFLOCC)
#   define  nvinqtrc FNAME(VINQTRC)
#   define  nvntrc   FNAME(VNTRC)
#   define  nvgttrsc FNAME(VGTTRSC)
#   define  nvgttrc  FNAME(VGTTRC)
#   define  nvadtrc  FNAME(VADTRC)
#   define  nvfstart FNAME(VFSTART)
#   define  nvfend   FNAME(VFEND)
#else   /* !DF_CAPFNAMES */
#	define  ndfivopn FNAME(dfivopn)
#	define  ndfvclos FNAME(dfvclos)
#   define  nvatchc  FNAME(vatchc)
#   define  nvdtchc  FNAME(vdtchc)
#   define  nvgnamc  FNAME(vgnamc)
#   define  nvgclsc  FNAME(vgclsc)
#   define  nvinqc   FNAME(vinqc)
#   define  nvgidc   FNAME(vgidc)
#   define  nvgnxtc  FNAME(vgnxtc)
#   define  nvsnamc  FNAME(vsnamc)
#   define  nvsclsc  FNAME(vsclsc)
#   define  nvinsrtc FNAME(vinsrtc)
#   define  nvisvgc  FNAME(visvgc)
#   define  nvisvsc  FNAME(visvsc)
#   define  nvsatchc FNAME(vsatchc)
#   define  nvsdtchc FNAME(vsdtchc)
#   define  nvsseekc FNAME(vsseekc)
#   define  nvsgnamc FNAME(vsgnamc)
#   define  nvsgclsc FNAME(vsgclsc)
#   define  nvsinqc  FNAME(vsinqc)
#   define  nvsfexc  FNAME(vsfexc)
#   define  nvsgidc  FNAME(vsgidc)
#   define  nvssnamc FNAME(vssnamc)
#   define  nvssclsc FNAME(vssclsc)
#   define  nvssfldc FNAME(vssfldc)
#   define  nvssintc FNAME(vssintc)
#   define  nvsfdefc FNAME(vsfdefc)
#   define  nvsreadc FNAME(vsreadc)
#   define  nvswritc FNAME(vswritc)
#   define  nvsgintc FNAME(vsgintc)
#   define  nvseltsc FNAME(vseltsc)
#   define  nvsgfldc FNAME(vsgfldc)
#   define  nvssizc  FNAME(vssizc)
#   define  nventsc  FNAME(ventsc)
#   define  nsetjjc  FNAME(setjjc)
#   define  nsetnojjc    FNAME(setnojjc)
#   define  nvlonec  FNAME(vlonec)
#   define  nvslonec FNAME(vslonec)
#   define  nvhsdc   FNAME(vhsdc)
#   define  nvhsdmc  FNAME(vhsdmc)
#   define  nvhmkgpc FNAME(vhmkgpc)
#   define  nvflocc  FNAME(vflocc)
#   define  nvinqtrc FNAME(vinqtrc)
#   define  nvntrc   FNAME(vntrc)
#   define  nvgttrsc FNAME(vgttrsc)
#   define  nvgttrc  FNAME(vgttrc)
#   define  nvadtrc  FNAME(vadtrc)
#   define  nvfstart FNAME(vfstart)
#   define  nvfend   FNAME(vfend)
#endif  /* DF_CAPFNAMES */
#endif  /* VG_FNAMES */

extern FRETVAL(intf) ndfivopn
    PROTO((_fcd filename, intf _HUGE *access, intf _HUGE *defdds, intf _HUGE *namelen));

extern FRETVAL(intf) ndfvclos
    PROTO((intf _HUGE *file_id));

extern FRETVAL(intf _HUGE *) nvatchc
    PROTO((HFILEID _HUGE *f, intf _HUGE *vgid, _fcd accesstype));

extern FRETVAL(void) nvdtchc
    PROTO((VGROUP _HUGE **vg));

extern FRETVAL(void) nvgnamc
    PROTO((VGROUP _HUGE **vg, _fcd vgname));

extern FRETVAL(void) nvgclsc
    PROTO((VGROUP _HUGE ** vg, _fcd vgclass));

extern FRETVAL(intf) nvinqc
    PROTO((VGROUP _HUGE ** vg, intf _HUGE *nentries, _fcd vgname));

extern FRETVAL(intf) nvgidc
    PROTO((HFILEID _HUGE *f, intf _HUGE *vgid));

extern FRETVAL(intf) nvgnxtc
    PROTO((VGROUP _HUGE **vg, intf _HUGE *id));

extern FRETVAL(void) nvsnamc
    PROTO((VGROUP _HUGE **vg, _fcd vgname, intf _HUGE *vgnamelen));

extern FRETVAL(void) nvsclsc
    PROTO((VGROUP _HUGE **vg, _fcd vgclass, intf _HUGE *vgclasslen));

extern FRETVAL(intf) nvinsrtc
    PROTO((VGROUP _HUGE **vg, intf _HUGE **vobjptr));

extern FRETVAL(intf) nvisvgc
    PROTO((VGROUP _HUGE **vg, intf _HUGE *id));

extern FRETVAL(intf) nvisvsc
    PROTO((VGROUP _HUGE **vg, intf _HUGE *id));

extern FRETVAL(intf _HUGE *) nvsatchc
    PROTO((HFILEID _HUGE *f, intf _HUGE *vsid, _fcd accesstype));

extern FRETVAL(void) nvsdtchc
    PROTO((VDATA _HUGE **vs));

extern FRETVAL(intf) nvsseekc
    PROTO((VDATA _HUGE **vs, intf _HUGE *eltpos));

extern FRETVAL(void) nvsgnamc
    PROTO((VDATA _HUGE **vs, _fcd vsname));

extern FRETVAL(void) nvsgclsc
    PROTO((VDATA _HUGE **vs, _fcd vsclass));

extern FRETVAL(intf) nvsinqc
    PROTO((VDATA _HUGE **vs, intf _HUGE *nelt ,intf _HUGE *interlace, _fcd fields, intf _HUGE *eltsize,
        _fcd vsname));

extern FRETVAL(intf) nvsfexc
    PROTO((VDATA _HUGE **vs, _fcd fields, intf _HUGE *fieldslen));

extern FRETVAL(intf) nvsgidc
    PROTO((HFILEID _HUGE *f, intf _HUGE *vsid));

extern FRETVAL(void) nvssnamc
    PROTO((VDATA _HUGE **vs, _fcd vsname,intn _HUGE *vsnamelen));

extern FRETVAL(void) nvssclsc
    PROTO((VDATA _HUGE **vs, _fcd vsclass, intf _HUGE *vsclasslen));

extern FRETVAL(intf) nvssfldc
    PROTO((VDATA _HUGE **vs, _fcd fields, intf _HUGE *fieldslen));

extern FRETVAL(intf) nvssintc
    PROTO((VDATA _HUGE **vs, intf _HUGE *interlace));

extern FRETVAL(intf) nvsfdefc
    PROTO((VDATA _HUGE **vs, _fcd field, intf _HUGE *localtype, intf _HUGE *order,
            intf _HUGE *fieldlen));

extern FRETVAL(intf) nvsreadc
    PROTO((VDATA _HUGE **vs, BYTE _HUGE *buf, intf _HUGE *nelt, intf _HUGE *interlace));

extern FRETVAL(intf) nvswritc
    PROTO((VDATA _HUGE **vs, BYTE _HUGE *buf, intf _HUGE *nelt, intf _HUGE *interlace));

extern FRETVAL(intf) nvsgintc
    PROTO((VDATA _HUGE **vs));

extern FRETVAL(intf) nvseltsc
    PROTO((VDATA _HUGE **vs));

extern FRETVAL(intf) nvsgfldc
    PROTO((VDATA _HUGE **vs, _fcd fields));

extern FRETVAL(intf) nvssizc
    PROTO((VDATA _HUGE **vs, _fcd fields, intf _HUGE *fieldslen));

extern FRETVAL(intf) nventsc
    PROTO((HFILEID _HUGE *f,intf _HUGE *vgid));

extern FRETVAL(void) nsetjjc
    PROTO((void));

extern FRETVAL(void) nsetnojjc
    PROTO((void));

extern FRETVAL(intf) nvlonec
    PROTO((HFILEID _HUGE *f, intf _HUGE **idarray, intf _HUGE *asize));

extern FRETVAL(intf) nvslonec
    PROTO((HFILEID _HUGE *f, intf _HUGE **idarray, intf _HUGE *asize));

extern FRETVAL(intf) nvhsdc
    PROTO((HFILEID _HUGE *f, _fcd field, BYTE _HUGE *buf, intf _HUGE *n, intf _HUGE *datatype,
        _fcd vsname, _fcd vsclass, intf _HUGE *fieldlen, intf _HUGE *vsnamelen,
        intf _HUGE *vsclasslen));

extern FRETVAL(intf) nvhsdmc
    PROTO((HFILEID _HUGE *f, _fcd field, BYTE _HUGE *buf, intf _HUGE *n, intf _HUGE *datatype,
        _fcd vsname, _fcd vsclass, intf _HUGE *order, intf _HUGE *fieldlen,
        intf _HUGE *vsnamelen, intf _HUGE *vsclasslen));

extern FRETVAL(intf) nvhmkgpc
    PROTO((HFILEID _HUGE *f, intf _HUGE *tagarray, intf _HUGE *refarray, intf _HUGE *n,
        _fcd vgname, _fcd vgclass,  intf _HUGE *vgnamelen, intf _HUGE *vgclasslen));

extern FRETVAL(intf) nvflocc
    PROTO((VGROUP _HUGE **vg, _fcd field, intf _HUGE *fieldlen));

extern FRETVAL(intf) nvinqtrc
    PROTO((VGROUP _HUGE **vg, intf _HUGE *tag, intf _HUGE *ref));

extern FRETVAL(intf) nvntrc
    PROTO((VGROUP _HUGE **vg));

extern FRETVAL(intf) nvgttrsc
    PROTO((VGROUP _HUGE **vg, intf _HUGE *tagarray, intf _HUGE *refarray, intf _HUGE *n));

extern FRETVAL(intf) nvgttrc
    PROTO((VGROUP _HUGE **vg, intf _HUGE *which, intf _HUGE *tag, intf _HUGE *ref));

extern FRETVAL(intf) nvadtrc
    PROTO((VGROUP _HUGE **vg, intf _HUGE *tag, intf _HUGE *ref));


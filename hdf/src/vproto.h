/*
$Header$

$Log$
Revision 1.8  1993/04/05 22:36:00  koziol
Fixed goofups made in haste when patching code.

 * Revision 1.7  1993/03/29  16:50:48  koziol
 * Updated JPEG code to new JPEG 4 code.
 * Changed VSets to use Threaded-Balanced-Binary Tree for internal
 * 	(in memory) representation.
 * Changed VGROUP * and VDATA * returns/parameters for all VSet functions
 * 	to use 32-bit integer keys instead of pointers.
 * Backed out speedups for Cray, until I get the time to fix them.
 * Fixed a bunch of bugs in the little-endian support in DFSD.
 *
 * Revision 1.6  1993/01/19  05:56:31  koziol
 * Merged Hyperslab and JPEG routines with beginning of DEC ALPHA
 * port.  Lots of minor annoyances fixed.
 *
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
  PROTO((VGROUP _HUGE *vg, uint8 _HUGE buf[], int32 _HUGE *size));

extern void oldunpackvs
  PROTO((VDATA _HUGE *vs, uint8 _HUGE buf[], int32 _HUGE *size));

/*
** from vg.c
*/
extern uint16 vnewref
  PROTO((HFILEID f));

#ifdef OLD_WAY
extern int32 VSelts
  PROTO((VDATA _HUGE *vs));
#else
extern int32 VSelts
  PROTO((int32 vkey));
#endif

#ifdef OLD_WAY
extern int32 VSgetinterlace
  PROTO((VDATA _HUGE *vs));
#else
extern int32 VSgetinterlace
  PROTO((int32 vkey));
#endif

#ifdef OLD_WAY
extern int32 VSsetinterlace
  PROTO((VDATA _HUGE *vs, int32 interlace));
#else
extern int32 VSsetinterlace
  PROTO((int32 vkey, int32 interlace));
#endif

#ifdef OLD_WAY
extern int32 VSgetfields
  PROTO((VDATA _HUGE *vs, char _HUGE *fields));
#else
extern int32 VSgetfields
  PROTO((int32 vkey, char _HUGE *fields));
#endif

#ifdef OLD_WAY
extern int32 VSfexist
  PROTO((VDATA _HUGE *vs, char _HUGE *fields));
#else
extern int32 VSfexist
  PROTO((int32 vkey, char _HUGE *fields));
#endif

#ifdef OLD_WAY
extern int32 VSsizeof
  PROTO((VDATA _HUGE *vs, char _HUGE *fields));
#else
extern int32 VSsizeof
  PROTO((int32 vkey, char _HUGE *fields));
#endif

#ifdef OLD_WAY
/* private */
extern int32 matchnocase
  PROTO((char _HUGE *strx, char _HUGE *stry));
#endif

#ifdef OLD_WAY
extern void VSdump
  PROTO((VDATA _HUGE *vs));
#else
extern void VSdump
  PROTO((int32 vkey));
#endif

#ifdef OLD_WAY
extern void VSsetname
  PROTO((VDATA _HUGE *vs, char _HUGE *vsname));
#else
extern void VSsetname
  PROTO((int32 vkey, char _HUGE *vsname));
#endif

#ifdef OLD_WAY
extern void VSsetclass
  PROTO((VDATA _HUGE *vs, char _HUGE *vsclass));
#else
extern void VSsetclass
  PROTO((int32 vkey, char _HUGE *vsclass));
#endif

#ifdef OLD_WAY
extern void VSgetname
  PROTO((VDATA _HUGE *vs, char _HUGE *vsname));
#else
extern void VSgetname
  PROTO((int32 vkey, char _HUGE *vsname));
#endif

#ifdef OLD_WAY
extern void VSgetclass
  PROTO((VDATA _HUGE *vs, char _HUGE *vsclass));
#else
extern void VSgetclass
  PROTO((int32 vkey, char _HUGE *vsclass));
#endif

#ifdef OLD_WAY
extern int32 VSinquire
  PROTO((VDATA _HUGE *vs,int32 _HUGE *nelt, int32 _HUGE *interlace, char _HUGE *fields, int32 _HUGE *eltsize,
    char _HUGE *vsname));
#else
extern int32 VSinquire
  PROTO((int32 vkey,int32 _HUGE *nelt, int32 _HUGE *interlace,
    char _HUGE *fields, int32 _HUGE *eltsize, char _HUGE *vsname));
#endif

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
extern intn vcompare
    PROTO((VOIDP k1,VOIDP k2,intn cmparg));

extern intn vcompareref
    PROTO((VOIDP k1,VOIDP k2,intn cmparg));

extern VOID vtfreenode
    PROTO((VOIDP n));

extern VOID vtfreekey
    PROTO((VOIDP k));

extern VOID Vinitialize
  PROTO((HFILEID f));

extern intn Vfinish
  PROTO((HFILEID f));

extern HFILEID Vopen
    PROTO(( char *path, intn access, int16 ndds));

extern intn Vclose
    PROTO((HFILEID f));

/*private */
#ifdef OLD_WAY
extern vginstance_t _HUGE *vginstance
  PROTO((HFILEID f, uint16 vgid));
#endif

extern int32 vexistvg
  PROTO((HFILEID f, uint16 vgid));

extern void vpackvg
  PROTO((VGROUP _HUGE *vg, uint8 _HUGE buf[], int32 _HUGE *size));

#ifdef OLD_WAY
extern void vunpackvg
  PROTO((VGROUP _HUGE *vg, uint8 _HUGE buf[]));
#endif

#ifdef OLD_WAY
extern VGROUP _HUGE *Vattach
  PROTO((HFILEID f, int32 vgid, char _HUGE *accesstype));
#else
extern int32 Vattach
  PROTO((HFILEID f, int32 vgid, char _HUGE *accesstype));
#endif

#ifdef OLD_WAY
extern void Vdetach
  PROTO((VGROUP _HUGE *vg));
#else
extern void Vdetach
  PROTO((int32 vkey));
#endif

#ifdef OLD_WAY
extern int32 Vinsert
  PROTO((VGROUP _HUGE *vg, VDATA _HUGE *velt));
  /* note: 2nd arg of Vinsert can also be (VGROUP *) */
#else
extern int32 Vinsert
  PROTO((int32 vkey, int32 vskey));
  /* note: 2nd arg of Vinsert can also be (VGROUP *) */
#endif

#ifdef OLD_WAY
extern int32 Vflocate
  PROTO((VGROUP _HUGE *vg, char _HUGE *field));
#else
extern int32 Vflocate
  PROTO((int32 vkey, char _HUGE *field));
#endif

#ifdef OLD_WAY
extern int32 Vinqtagref
  PROTO((VGROUP _HUGE *vg, int32 tag, int32 ref));
#else
extern int32 Vinqtagref
  PROTO((int32 vkey, int32 tag, int32 ref));
#endif

#ifdef OLD_WAY
extern int32 Vntagrefs
  PROTO((VGROUP _HUGE *vg));
#else
extern int32 Vntagrefs
  PROTO((int32 vkey));
#endif

#ifdef OLD_WAY
extern int32 Vgettagrefs
  PROTO((VGROUP _HUGE *vg, int32 _HUGE tagarray[], int32 _HUGE refarray[], int32 n));
#else
extern int32 Vgettagrefs
  PROTO((int32 vkey, int32 _HUGE tagarray[], int32 _HUGE refarray[], int32 n));
#endif

#ifdef OLD_WAY
extern int32 Vgettagref
  PROTO((VGROUP _HUGE *vg, int32 which, int32 _HUGE *tag, int32 _HUGE *ref));
#else
extern int32 Vgettagref
  PROTO((int32 vkey, int32 which, int32 _HUGE *tag, int32 _HUGE *ref));
#endif

extern int32 Vgetotag
  PROTO((int32 vkey, int32 _HUGE *tag));

extern int32 Vgetoref
  PROTO((int32 vkey, int32 _HUGE *ref));

#ifdef OLD_WAY
extern int32 Vaddtagref
  PROTO((VGROUP _HUGE *vg, int32 tag, int32 ref));
#else
extern int32 Vaddtagref
  PROTO((int32 vkey, int32 tag, int32 ref));
#endif

extern int32 vinsertpair
  PROTO((VGROUP _HUGE *vg, uint16 tag, uint16 ref));

extern int32 Ventries
  PROTO((HFILEID f, int32 vgid));

#ifdef OLD_WAY
extern void Vsetname
  PROTO((VGROUP _HUGE *vg, char _HUGE *vgname));
#else
extern int32 Vsetname
  PROTO((int32 vkey, char _HUGE *vgname));
#endif

#ifdef OLD_WAY
extern void Vsetclass
  PROTO((VGROUP _HUGE *vg, char _HUGE *vgclass));
#else
extern int32 Vsetclass
  PROTO((int32 vkey, char _HUGE *vgclass));
#endif

#ifdef OLD_WAY
extern int32 Visvg
  PROTO((VGROUP _HUGE *vg, int32 id));
#else
extern int32 Visvg
  PROTO((int32 vkey, int32 id));
#endif

#ifdef OLD_WAY
extern int32 Visvs
    PROTO((VGROUP _HUGE *vg, int32 id));
#else
extern int32 Visvs
    PROTO((int32 vkey, int32 id));
#endif

extern int32 Vgetid
  PROTO((HFILEID f, int32 vgid));

#ifdef OLD_WAY
extern int32 Vgetnext
  PROTO((VGROUP _HUGE *vg, int32 id));
#else
extern int32 Vgetnext
  PROTO((int32 vkey, int32 id));
#endif

#ifdef OLD_WAY
extern void Vgetname
  PROTO((VGROUP _HUGE *vg, char _HUGE *vgname));
#else
extern void Vgetname
  PROTO((int32 vkey, char _HUGE *vgname));
#endif

#ifdef OLD_WAY
extern void Vgetclass
  PROTO((VGROUP _HUGE *vg, char _HUGE *vgclass));
#else
extern void Vgetclass
  PROTO((int32 vkey, char _HUGE *vgclass));
#endif

#ifdef OLD_WAY
extern int32 Vinquire
  PROTO((VGROUP _HUGE *vg, int32 _HUGE *nentries, char _HUGE *vgname));
#else
extern int32 Vinquire
  PROTO((int32 vkey, int32 _HUGE *nentries, char _HUGE *vgname));
#endif


/*
** from vparse.c
*/
extern int32 scanattrs
  PROTO((char _HUGE *attrs, int32 _HUGE *attrc, char _HUGE ***attrv));


/*
** from vhi.c
*/
extern int32 VHstoredata
  PROTO((HFILEID f, char _HUGE *field, uint8 _HUGE buf[], int32 n, int32 datatype,
    char _HUGE *vsname, char _HUGE *vsclass));

extern int32 VHstoredatam
  PROTO((HFILEID f, char _HUGE *field, uint8 _HUGE buf[], int32 n, int32 datatype,
    char _HUGE *vsname, char _HUGE *vsclass, int32 order));

extern int32 VHmakegroup
  PROTO((HFILEID  f, int32 _HUGE tagarray[], int32 _HUGE refarray[], int32 n, char _HUGE *vgname,
    char _HUGE *vgclass));


/*
** from vio.c
*/

extern vsinstance_t _HUGE * vsinstance
    PROTO((HFILEID f, uint16 vsid));

extern VWRITELIST _HUGE * vswritelist
    PROTO((int32 vskey));

#ifdef QAK
extern int32 DFvsetclose
    PROTO((HFILEID f));

extern HFILEID DFvsetopen
    PROTO((char _HUGE *fname, int16  access, int16 defDDs));
#endif

extern int32 vexistvs
    PROTO((HFILEID f, uint16 vsid));

extern void vpackvs
    PROTO((VDATA _HUGE *vs, uint8 _HUGE buf[], int32 _HUGE *size));

#ifdef OLD_WAY
extern void vunpackvs
    PROTO((VDATA _HUGE *vs, uint8 _HUGE buf[], int32 _HUGE *size));
#endif

#ifdef OLD_WAY
extern VDATA _HUGE * VSattach
    PROTO((HFILEID f, int32 vsid, char _HUGE *accesstype));

extern void VSdetach
    PROTO((VDATA _HUGE *vs));
#else
extern int32 VSattach
    PROTO((HFILEID f, int32 vsid, char _HUGE *accesstype));

extern void VSdetach
    PROTO((int32 vkey));
#endif

extern int32 VSgetotag
  PROTO((int32 vkey, int32 _HUGE *tag));

extern int32 VSgetoref
  PROTO((int32 vkey, int32 _HUGE *ref));

extern int32 VSgetid
    PROTO((HFILEID f, int32 vsid));

extern int32 VSgetversion
    PROTO((int32 vkey));

/*
** from vsfld.c
*/

extern int16 SIZEOF
    PROTO((int16 x));

extern int16 HDFSIZEOF
    PROTO((int16 x));

#ifdef OLD_WAY
extern int32 VSsetfields
  PROTO((VDATA _HUGE *vs, char _HUGE *fields));

extern int32 VSfdefine
  PROTO((VDATA _HUGE *vs, char _HUGE *field, int32 localtype, int32 order));
#else
extern int32 VSsetfields
  PROTO((int32 vkey, char _HUGE *fields));

extern int32 VSfdefine
  PROTO((int32 vkey, char _HUGE *field, int32 localtype, int32 order));
#endif

/*
** from vrw.c
*/
#ifdef OLD_WAY
extern int32 VSseek
  PROTO((VDATA _HUGE *vs, int32 eltpos));
#else
extern int32 VSseek
  PROTO((int32 vkey, int32 eltpos));
#endif

#ifdef OLD_WAY
extern int32 VSread
  PROTO((VDATA _HUGE *vs, uint8 _HUGE buf[], int32 nelt, int32 interlace));
#else
extern int32 VSread
  PROTO((int32 vkey, uint8 _HUGE buf[], int32 nelt, int32 interlace));
#endif

#ifdef OLD_WAY
extern int32 VSwrite
  PROTO((VDATA _HUGE *vs, uint8 _HUGE buf[], int32 nelt, int32 interlace));
#else
extern int32 VSwrite
  PROTO((int32 vkey, uint8 _HUGE buf[], int32 nelt, int32 interlace));
#endif

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

extern FRETVAL(intf) nvatchc
    PROTO((HFILEID _HUGE *f, intf _HUGE *vgid, _fcd accesstype));

#ifdef OLD_WAY
extern FRETVAL(void) nvdtchc
    PROTO((VGROUP _HUGE **vg));

extern FRETVAL(void) nvgnamc
    PROTO((VGROUP _HUGE **vg, _fcd vgname));

extern FRETVAL(void) nvgclsc
    PROTO((VGROUP _HUGE ** vg, _fcd vgclass));

extern FRETVAL(intf) nvinqc
    PROTO((VGROUP _HUGE ** vg, intf _HUGE *nentries, _fcd vgname));
#else
extern FRETVAL(void) nvdtchc
    PROTO((intf _HUGE *vkey));

extern FRETVAL(void) nvgnamc
    PROTO((intf _HUGE *vkey, _fcd vgname));

extern FRETVAL(void) nvgclsc
    PROTO((intf _HUGE *vkey, _fcd vgclass));

extern FRETVAL(intf) nvinqc
    PROTO((intf _HUGE *vkey, intf _HUGE *nentries, _fcd vgname));
#endif

extern FRETVAL(intf) nvgidc
    PROTO((HFILEID _HUGE *f, intf _HUGE *vgid));

#ifdef OLD_WAY
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
#else
extern FRETVAL(intf) nvgnxtc
    PROTO((intf _HUGE *vkey, intf _HUGE *id));

extern FRETVAL(void) nvsnamc
    PROTO((intf _HUGE *vkey, _fcd vgname, intf _HUGE *vgnamelen));

extern FRETVAL(void) nvsclsc
    PROTO((intf _HUGE *vkey, _fcd vgclass, intf _HUGE *vgclasslen));

extern FRETVAL(intf) nvinsrtc
    PROTO((intf _HUGE *vkey, intf _HUGE *vobjptr));

extern FRETVAL(intf) nvisvgc
    PROTO((intf _HUGE *vkey, intf _HUGE *id));

extern FRETVAL(intf) nvisvsc
    PROTO((intf _HUGE *vkey, intf _HUGE *id));
#endif

extern FRETVAL(intf) nvsatchc
    PROTO((HFILEID _HUGE *f, intf _HUGE *vsid, _fcd accesstype));

#ifdef OLD_WAY
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
#else
extern FRETVAL(void) nvsdtchc
    PROTO((intf _HUGE *vkey));

extern FRETVAL(intf) nvsseekc
    PROTO((intf _HUGE *vkey, intf _HUGE *eltpos));

extern FRETVAL(void) nvsgnamc
    PROTO((intf _HUGE *vkey, _fcd vsname));

extern FRETVAL(void) nvsgclsc
    PROTO((intf _HUGE *vkey, _fcd vsclass));

extern FRETVAL(intf) nvsinqc
    PROTO((intf _HUGE *vkey, intf _HUGE *nelt ,intf _HUGE *interlace,
        _fcd fields, intf _HUGE *eltsize, _fcd vsname));

extern FRETVAL(intf) nvsfexc
    PROTO((intf _HUGE *vkey, _fcd fields, intf _HUGE *fieldslen));
#endif

extern FRETVAL(intf) nvsgidc
    PROTO((HFILEID _HUGE *f, intf _HUGE *vsid));

#ifdef OLD_WAY
extern FRETVAL(void) nvssnamc
    PROTO((VDATA _HUGE **vs, _fcd vsname,intf _HUGE *vsnamelen));

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
    PROTO((VDATA _HUGE **vs, uint8 _HUGE *buf, intf _HUGE *nelt, intf _HUGE *interlace));

extern FRETVAL(intf) nvswritc
    PROTO((VDATA _HUGE **vs, uint8 _HUGE *buf, intf _HUGE *nelt, intf _HUGE *interlace));

extern FRETVAL(intf) nvsgintc
    PROTO((VDATA _HUGE **vs));

extern FRETVAL(intf) nvseltsc
    PROTO((VDATA _HUGE **vs));

extern FRETVAL(intf) nvsgfldc
    PROTO((VDATA _HUGE **vs, _fcd fields));

extern FRETVAL(intf) nvssizc
    PROTO((VDATA _HUGE **vs, _fcd fields, intf _HUGE *fieldslen));
#else
extern FRETVAL(void) nvssnamc
    PROTO((intf _HUGE *vkey, _fcd vsname,intf _HUGE *vsnamelen));

extern FRETVAL(void) nvssclsc
    PROTO((intf _HUGE *vkey, _fcd vsclass, intf _HUGE *vsclasslen));

extern FRETVAL(intf) nvssfldc
    PROTO((intf _HUGE *vkey, _fcd fields, intf _HUGE *fieldslen));

extern FRETVAL(intf) nvssintc
    PROTO((intf _HUGE *vkey, intf _HUGE *interlace));

extern FRETVAL(intf) nvsfdefc
    PROTO((intf _HUGE *vkey, _fcd field, intf _HUGE *localtype,
            intf _HUGE *order, intf _HUGE *fieldlen));

extern FRETVAL(intf) nvsreadc
    PROTO((intf _HUGE *vkey, uint8 _HUGE *buf, intf _HUGE *nelt,
            intf _HUGE *interlace));

extern FRETVAL(intf) nvswritc
    PROTO((intf _HUGE *vkey, uint8 _HUGE *buf, intf _HUGE *nelt,
            intf _HUGE *interlace));

extern FRETVAL(intf) nvsgintc
    PROTO((intf _HUGE *vkey));

extern FRETVAL(intf) nvseltsc
    PROTO((intf _HUGE *vkey));

extern FRETVAL(intf) nvsgfldc
    PROTO((intf _HUGE *vkey, _fcd fields));

extern FRETVAL(intf) nvssizc
    PROTO((intf _HUGE *vkey, _fcd fields, intf _HUGE *fieldslen));
#endif

extern FRETVAL(intf) nventsc
    PROTO((HFILEID _HUGE *f,intf _HUGE *vgid));

extern FRETVAL(intf) nvlonec
    PROTO((HFILEID _HUGE *f, intf _HUGE **idarray, intf _HUGE *asize));

extern FRETVAL(intf) nvslonec
    PROTO((HFILEID _HUGE *f, intf _HUGE **idarray, intf _HUGE *asize));

extern FRETVAL(intf) nvhsdc
    PROTO((HFILEID _HUGE *f, _fcd field, uint8 _HUGE *buf, intf _HUGE *n, intf _HUGE *datatype,
        _fcd vsname, _fcd vsclass, intf _HUGE *fieldlen, intf _HUGE *vsnamelen,
        intf _HUGE *vsclasslen));

extern FRETVAL(intf) nvhsdmc
    PROTO((HFILEID _HUGE *f, _fcd field, uint8 _HUGE *buf, intf _HUGE *n, intf _HUGE *datatype,
        _fcd vsname, _fcd vsclass, intf _HUGE *order, intf _HUGE *fieldlen,
        intf _HUGE *vsnamelen, intf _HUGE *vsclasslen));

extern FRETVAL(intf) nvhmkgpc
    PROTO((HFILEID _HUGE *f, intf _HUGE *tagarray, intf _HUGE *refarray, intf _HUGE *n,
        _fcd vgname, _fcd vgclass,  intf _HUGE *vgnamelen, intf _HUGE *vgclasslen));

#ifdef OLD_WAY
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
#else
extern FRETVAL(intf) nvflocc
    PROTO((intf _HUGE *vkey, _fcd field, intf _HUGE *fieldlen));

extern FRETVAL(intf) nvinqtrc
    PROTO((intf _HUGE *vkey, intf _HUGE *tag, intf _HUGE *ref));

extern FRETVAL(intf) nvntrc
    PROTO((intf _HUGE *vkey));

extern FRETVAL(intf) nvgttrsc
    PROTO((intf _HUGE *vkey, intf _HUGE *tagarray, intf _HUGE *refarray, intf _HUGE *n));

extern FRETVAL(intf) nvgttrc
    PROTO((intf _HUGE *vkey, intf _HUGE *which, intf _HUGE *tag, intf _HUGE *ref));

extern FRETVAL(intf) nvadtrc
    PROTO((intf _HUGE *vkey, intf _HUGE *tag, intf _HUGE *ref));
#endif


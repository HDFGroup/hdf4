/*
$Header$

$Log$
Revision 1.2  1992/09/11 14:15:04  koziol
Changed Fortran stubs' parameter passing to use a new typedef, intf,
which should be typed to the size of an INTEGER*4 in whatever Fortran
compiler the C functions need to be compatible with.  (This is mostly
for the PC and possibly for the Mac)

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
  PROTO((char *fs));

extern int32 vmakecompat
  PROTO((char *fs));

extern void oldunpackvg
  PROTO((VGROUP *vg, BYTE buf[], int32 *size));

extern void oldunpackvs
  PROTO((VDATA *vs, BYTE buf[], int32 *size));

/*
** from vg.c
*/
extern uint16 vnewref
  PROTO((HFILEID f));

extern int32 VSelts
  PROTO((VDATA *vs));

extern int32 VSgetinterlace
  PROTO((VDATA *vs));

extern int32 VSsetinterlace
  PROTO((VDATA *vs, int32 interlace));

extern int32 VSgetfields
  PROTO((VDATA *vs, char *fields));

extern int32 VSfexist
  PROTO((VDATA *vs, char *fields));

extern int32 VSsizeof
  PROTO((VDATA *vs, char *fields));

/* private */
extern int32 matchnocase
  PROTO((char *strx, char *stry));

extern void VSdump
  PROTO((VDATA *vs));

extern void VSsetname
  PROTO((VDATA *vs, char *vsname));

extern void VSsetclass
  PROTO((VDATA *vs, char *vsclass));

extern void VSgetname
  PROTO((VDATA *vs, char *vsname));

extern void VSgetclass
  PROTO((VDATA *vs, char *vsclass));

extern int32 VSinquire
  PROTO((VDATA *vs,int32 *nelt, int32 *interlace, char *fields, int32 *eltsize,
	char *vsname));

extern int32 VSlone
  PROTO((HFILEID f, int32 idarray[], int32 asize));

extern int32 Vlone
  PROTO((HFILEID f, int32 idarray[], int32 asize));

extern int32 Vfind
  PROTO((HFILEID f, char *vgname));

extern int32 VSfind
  PROTO((HFILEID f, char *vsname));

extern void Vsetzap
    PROTO((void));

/*
** from vgp.c
*/
extern void setjj
  PROTO((void));

extern void setnojj
  PROTO((void));

extern void Vinitialize
  PROTO((HFILEID f));

extern void Vfinish
  PROTO((HFILEID f));

#if 0 /* was turned into a macro */
extern vfile_t *Get_vfile
  PROTO((HFILEID f));
#endif

/*private */
extern vginstance_t *vginstance
  PROTO((HFILEID f, uint16 vgid));

extern int32 vexistvg
  PROTO((HFILEID f, uint16 vgid));

extern void vpackvg
  PROTO((VGROUP *vg, BYTE buf[], int32 *size));

extern void vunpackvg
  PROTO((VGROUP *vg, BYTE buf[], int32 *size));

extern VGROUP *Vattach
  PROTO((HFILEID f, int32 vgid, char *accesstype));

extern void Vdetach
  PROTO((VGROUP *vg));

extern int32 Vinsert
  PROTO((VGROUP *vg, VDATA *velt)); 
  /* note: 2nd arg of Vinsert can also be (VGROUP *) */

extern int32 Vflocate
  PROTO((VGROUP *vg, char *field));

extern int32 Vinqtagref
  PROTO((VGROUP *vg, int32 tag, int32 ref));

extern int32 Vntagrefs
  PROTO((VGROUP *vg));

extern int32 Vgettagrefs
  PROTO((VGROUP *vg, int32 tagarray[], int32 refarray[], int32 n));

extern int32 Vgettagref
  PROTO((VGROUP *vg, int32 which, int32 *tag, int32 *ref));

extern int32 Vaddtagref
  PROTO((VGROUP *vg, int32 tag, int32 ref));

extern int32 vinsertpair
  PROTO((VGROUP *vg, uint16 tag, uint16 ref));

extern int32 Ventries
  PROTO((HFILEID f, int32 vgid));

extern void Vsetname
  PROTO((VGROUP *vg, char *vgname));

extern void Vsetclass
  PROTO((VGROUP *vg, char *vgclass));

extern int32 Visvg
  PROTO((VGROUP *vg, int32 id));

extern int32 Vgetid
  PROTO((HFILEID f, int32 vgid));

extern int32 Vgetnext
  PROTO((VGROUP *vg, int32 id));

extern void Vgetname
  PROTO((VGROUP *vg, char *vgname));

extern void Vgetclass
  PROTO((VGROUP *vg, char *vgclass));

extern int32 Vinquire
  PROTO((VGROUP *vg, int32 *nentries, char *vgname));


/*
** from vparse.c
*/
extern int32 scanattrs
  PROTO((char *attrs, int32 *attrc, char ***attrv));


/*
** from vhi.c
*/
extern int32 VHstoredata
  PROTO((HFILEID f, char *field, BYTE buf[], int32 n, int32 datatype,
	char *vsname, char *vsclass));

extern int32 VHstoredatam
  PROTO((HFILEID f, char *field, BYTE buf[], int32 n, int32 datatype,
	char *vsname, char *vsclass, int32 order));

extern int32 VHmakegroup
  PROTO((HFILEID  f, int32 tagarray[], int32 refarray[], int32 n, char *vgname,
	char *vgclass));


/*
** from vio.c
*/

extern vsinstance_t * vsinstance
  PROTO((HFILEID f, uint16 vsid));

extern int32 DFvsetclose
  PROTO((HFILEID f));

extern HFILEID DFvsetopen
  PROTO((char *fname, int16  access, int16 defDDs));

extern int32 vexistvs
  PROTO((HFILEID f, uint16 vsid));

extern void vpackvs
  PROTO((VDATA *vs, BYTE buf[], int32 *size));

extern void vunpackvs
  PROTO((VDATA *vs, BYTE buf[], int32 *size));

extern VDATA * VSattach
  PROTO((HFILEID f, int32 vsid, char *accesstype));

extern void VSdetach
  PROTO((VDATA *vs));

extern int32 Visvs
  PROTO((VGROUP *vg, int32 id));

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
  PROTO((VDATA *vs, char *fields));

extern int32 VSfdefine
  PROTO((VDATA *vs, char *field, int32 localtype, int32 order));

/*
** from vrw.c
*/
extern int32 VSseek
  PROTO((VDATA *vs, int32 eltpos));

extern int32 VSread
  PROTO((VDATA *vs, BYTE buf[], int32 nelt, int32 interlace));

extern int32 VSwrite
  PROTO((VDATA *vs, BYTE buf[], int32 nelt, int32 interlace));

/*
** from vgF.c
*/
#ifndef VG_FNAMES
#   define VG_FNAMES
#ifdef DF_CAPFNAMES
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
#else   /* !DF_CAPFNAMES */
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
#endif  /* DF_CAPFNAMES */
#endif  /* VG_FNAMES */

extern FRETVAL(intf *) nvatchc
    PROTO((HFILEID *f, intf *vgid, _fcd accesstype));

extern FRETVAL(void) nvdtchc
    PROTO((VGROUP **vg));

extern FRETVAL(void) nvgnamc
    PROTO((VGROUP **vg, _fcd vgname));

extern FRETVAL(void) nvgclsc
    PROTO((VGROUP ** vg, _fcd vgclass));

extern FRETVAL(intf) nvinqc
    PROTO((VGROUP** vg, intf *nentries, _fcd vgname));

extern FRETVAL(intf) nvgidc
    PROTO((HFILEID *f, intf *vgid));

extern FRETVAL(intf) nvgnxtc
    PROTO((VGROUP **vg, intf *id));

extern FRETVAL(void) nvsnamc
    PROTO((VGROUP **vg, _fcd vgname, intf *vgnamelen));

extern FRETVAL(void) nvsclsc
    PROTO((VGROUP **vg, _fcd vgclass, intf *vgclasslen));

extern FRETVAL(intf) nvinsrtc
    PROTO((VGROUP **vg, intf **vobjptr));

extern FRETVAL(intf) nvisvgc
    PROTO((VGROUP **vg, intf *id));

extern FRETVAL(intf) nvisvsc
    PROTO((VGROUP **vg, intf *id));

extern FRETVAL(intf *) nvsatchc
    PROTO((HFILEID *f, intf *vsid, _fcd accesstype));

extern FRETVAL(void) nvsdtchc
    PROTO((VDATA **vs));

extern FRETVAL(intf) nvsseekc
    PROTO((VDATA **vs, intf *eltpos));

extern FRETVAL(void) nvsgnamc
    PROTO((VDATA **vs, _fcd vsname));

extern FRETVAL(void) nvsgclsc
    PROTO((VDATA **vs, _fcd vsclass));

extern FRETVAL(intf) nvsinqc
    PROTO((VDATA **vs, intf *nelt ,intf *interlace, _fcd fields, intf *eltsize,
        _fcd vsname));

extern FRETVAL(intf) nvsfexc
    PROTO((VDATA **vs, _fcd fields, intf *fieldslen));

extern FRETVAL(intf) nvsgidc
    PROTO((HFILEID *f, intf *vsid));

extern FRETVAL(void) nvssnamc
    PROTO((VDATA **vs, _fcd vsname,intn *vsnamelen));

extern FRETVAL(void) nvssclsc
    PROTO((VDATA **vs, _fcd vsclass, intf *vsclasslen));

extern FRETVAL(intf) nvssfldc
    PROTO((VDATA **vs, _fcd fields, intf *fieldslen));

extern FRETVAL(intf) nvssintc
    PROTO((VDATA **vs, intf *interlace));

extern FRETVAL(intf) nvsfdefc
    PROTO((VDATA **vs, _fcd field, intf *localtype, intf *order,
            intf *fieldlen));

extern FRETVAL(intf) nvsreadc
    PROTO((VDATA **vs, BYTE *buf, intf *nelt, intf *interlace));

extern FRETVAL(intf) nvswritc
    PROTO((VDATA **vs, BYTE *buf, intf *nelt, intf *interlace));

extern FRETVAL(intf) nvsgintc
    PROTO((VDATA **vs));

extern FRETVAL(intf) nvseltsc
    PROTO((VDATA **vs));

extern FRETVAL(intf) nvsgfldc
    PROTO((VDATA **vs, _fcd fields));

extern FRETVAL(intf) nvssizc
    PROTO((VDATA **vs, _fcd fields, intf *fieldslen));

extern FRETVAL(intf) nventsc
    PROTO((HFILEID *f,intf *vgid));

extern FRETVAL(void) nsetjjc
    PROTO((void));

extern FRETVAL(void) nsetnojjc
    PROTO((void));

extern FRETVAL(intf) nvlonec
    PROTO((HFILEID *f, intf **idarray, intf *asize));

extern FRETVAL(intf) nvslonec
    PROTO((HFILEID *f, intf **idarray, intf *asize));

extern FRETVAL(intf) nvhsdc
    PROTO((HFILEID *f, _fcd field, BYTE *buf, intf *n, intf *datatype,
        _fcd vsname, _fcd vsclass, intf *fieldlen, intf *vsnamelen,
        intf *vsclasslen));

extern FRETVAL(intf) nvhsdmc
    PROTO((HFILEID *f, _fcd field, BYTE *buf, intf *n, intf *datatype,
        _fcd vsname, _fcd vsclass, intf *order, intf *fieldlen,
        intf *vsnamelen, intf *vsclasslen));

extern FRETVAL(intf) nvhmkgpc
    PROTO((HFILEID *f, intf *tagarray, intf *refarray, intf *n,
        _fcd vgname, _fcd vgclass,  intf *vgnamelen, intf *vgclasslen));

extern FRETVAL(intf) nvflocc
    PROTO((VGROUP **vg, _fcd field, intf *fieldlen));

extern FRETVAL(intf) nvinqtrc
    PROTO((VGROUP **vg, intf *tag, intf *ref));

extern FRETVAL(intf) nvntrc
    PROTO((VGROUP **vg));

extern FRETVAL(intf) nvgttrsc
    PROTO((VGROUP **vg, intf *tagarray, intf *refarray, intf *n));

extern FRETVAL(intf) nvgttrc
    PROTO((VGROUP **vg, intf *which, intf *tag, intf *ref));

extern FRETVAL(intf) nvadtrc
    PROTO((VGROUP **vg, intf *tag, intf *ref));


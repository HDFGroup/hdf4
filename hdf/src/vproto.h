/*
$Header$

$Log$
Revision 1.1  1992/08/25 21:40:44  koziol
Initial revision

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

extern FRETVAL(int32 *) nvatchc
    PROTO((HFILEID *f, int32 *vgid, _fcd accesstype));

extern FRETVAL(void) nvdtchc
    PROTO((VGROUP **vg));

extern FRETVAL(void) nvgnamc
    PROTO((VGROUP **vg, _fcd vgname));

extern FRETVAL(void) nvgclsc
    PROTO((VGROUP ** vg, _fcd vgclass));

extern FRETVAL(int32) nvinqc
    PROTO((VGROUP** vg, int32 *nentries, _fcd vgname));

extern FRETVAL(int32) nvgidc
    PROTO((HFILEID *f, int32 *vgid));

extern FRETVAL(int32) nvgnxtc
    PROTO((VGROUP **vg, int32 *id));

extern FRETVAL(void) nvsnamc
    PROTO((VGROUP **vg, _fcd vgname, intn *vgnamelen));

extern FRETVAL(void) nvsclsc
    PROTO((VGROUP **vg, _fcd vgclass, intn *vgclasslen));

extern FRETVAL(int32) nvinsrtc
    PROTO((VGROUP **vg, int32 **vobjptr));

extern FRETVAL(int32) nvisvgc
    PROTO((VGROUP **vg, int32 *id));

extern FRETVAL(int32) nvisvsc
    PROTO((VGROUP **vg, int32 *id));

extern FRETVAL(int32 *) nvsatchc
    PROTO((HFILEID *f, int32 *vsid, _fcd accesstype));

extern FRETVAL(void) nvsdtchc
    PROTO((VDATA **vs));

extern FRETVAL(int32) nvsseekc
    PROTO((VDATA **vs, int32 *eltpos));

extern FRETVAL(void) nvsgnamc
    PROTO((VDATA **vs, _fcd vsname));

extern FRETVAL(void) nvsgclsc
    PROTO((VDATA **vs, _fcd vsclass));

extern FRETVAL(int32) nvsinqc
    PROTO((VDATA **vs, int32 *nelt ,int32 *interlace, _fcd fields,
                int32 *eltsize, _fcd vsname));

extern FRETVAL(int32) nvsfexc
    PROTO((VDATA **vs, _fcd fields, intn *fieldslen));

extern FRETVAL(int32) nvsgidc
    PROTO((HFILEID *f, int32 *vsid));

extern FRETVAL(void) nvssnamc
    PROTO((VDATA **vs, _fcd vsname,intn *vsnamelen));

extern FRETVAL(void) nvssclsc
    PROTO((VDATA **vs, _fcd vsclass, intn *vsclasslen));

extern FRETVAL(int32) nvssfldc
    PROTO((VDATA **vs, _fcd fields, intn *fieldslen));

extern FRETVAL(int32) nvssintc
    PROTO((VDATA **vs, int32 *interlace));

extern FRETVAL(int32) nvsfdefc
    PROTO((VDATA **vs, _fcd field, int32 *localtype, int32 *order,
                intn *fieldlen));

extern FRETVAL(int32) nvsreadc
    PROTO((VDATA **vs, BYTE *buf, int32 *nelt, int32 *interlace));

extern FRETVAL(int32) nvswritc
    PROTO((VDATA **vs, BYTE *buf, int32 *nelt, int32 *interlace));

extern FRETVAL(int32) nvsgintc
    PROTO((VDATA **vs));

extern FRETVAL(int32) nvseltsc
    PROTO((VDATA **vs));

extern FRETVAL(int32) nvsgfldc
    PROTO((VDATA **vs, _fcd fields));

extern FRETVAL(int32) nvssizc
    PROTO((VDATA **vs, _fcd fields, intn *fieldslen));

extern FRETVAL(int32) nventsc
    PROTO((HFILEID *f,int32 *vgid));

extern FRETVAL(void) nsetjjc
    PROTO((void));

extern FRETVAL(void) nsetnojjc
    PROTO((void));

extern FRETVAL(int32) nvlonec
    PROTO((HFILEID *f, int32 **idarray, int32 *asize));

extern FRETVAL(int32) nvslonec
    PROTO((HFILEID *f, int32 **idarray, int32 *asize));

extern FRETVAL(int32) nvhsdc
    PROTO((HFILEID *f, _fcd field, BYTE *buf, int32 *n, int32 *datatype,
                _fcd vsname, _fcd vsclass, intn *fieldlen, 
                intn *vsnamelen, intn *vsclasslen));

extern FRETVAL(int32) nvhsdmc
    PROTO((HFILEID *f, _fcd field, BYTE *buf, int32 *n, int32 *datatype,
                _fcd vsname, _fcd vsclass, int32 *order, 
                intn *fieldlen, intn *vsnamelen, intn *vsclasslen));

extern FRETVAL(int32) nvhmkgpc
    PROTO((HFILEID *f, int32 *tagarray, int32 *refarray, int32 *n,
                _fcd vgname, _fcd vgclass,  intn *vgnamelen, 
                intn *vgclasslen));

extern FRETVAL(int32) nvflocc
    PROTO((VGROUP **vg, _fcd field, intn *fieldlen));

extern FRETVAL(int32) nvinqtrc
    PROTO((VGROUP **vg, int32 *tag, int32 *ref));

extern FRETVAL(int32) nvntrc
    PROTO((VGROUP **vg));

extern FRETVAL(int32) nvgttrsc
    PROTO((VGROUP **vg, int32 *tagarray, int32 *refarray, int32 *n));

extern FRETVAL(int32) nvgttrc
    PROTO((VGROUP **vg, int32 *which, int32 *tag, int32 *ref));

extern FRETVAL(int32) nvadtrc
    PROTO((VGROUP **vg, int32 *tag, int32 *ref));


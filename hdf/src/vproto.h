/****************************************************************************
 * NCSA HDF                                                                 *
 * Software Development Group                                               *
 * National Center for Supercomputing Applications                          *
 * University of Illinois at Urbana-Champaign                               *
 * 605 E. Springfield, Champaign IL 61820                                   *
 *                                                                          *
 * For conditions of distribution and use, see the accompanying             *
 * hdf/COPYING file.                                                        *
 *                                                                          *
 ****************************************************************************/

/* $Id$ */

#if defined c_plusplus || defined __cplusplus
extern      "C"
{
#endif                          /* c_plusplus || __cplusplus */

/*
   ** from vconv.c
 */
    extern int32 vicheckcompat
                (HFILEID f);

    extern int32 vimakecompat
                (HFILEID f);

    extern int32 vcheckcompat
                (char _HUGE * fs);

    extern int32 vmakecompat
                (char _HUGE * fs);

/*
   ** from vg.c
 */
    extern uint16 vnewref
                (HFILEID f);

    extern int32 VSelts
                (int32 vkey);

    extern int32 VSgetinterlace
                (int32 vkey);

    extern intn VSsetinterlace
                (int32 vkey, int32 interlace);

    extern int32 VSgetfields
                (int32 vkey, char _HUGE * fields);

    extern intn VSfexist
                (int32 vkey, char _HUGE * fields);

    extern int32 VSsizeof
                (int32 vkey, char _HUGE * fields);

    extern VOID VSdump
                (int32 vkey);

    extern int32 VSsetname
                (int32 vkey, const char _HUGE * vsname);

    extern int32 VSsetclass
                (int32 vkey, const char _HUGE * vsclass);

    extern int32 VSgetname
                (int32 vkey, char _HUGE * vsname);

    extern int32 VSgetclass
                (int32 vkey, char _HUGE * vsclass);

    extern intn VSinquire
                (int32 vkey, int32 _HUGE * nelt, int32 _HUGE * interlace,
           char _HUGE * fields, int32 _HUGE * eltsize, char _HUGE * vsname);

    extern int32 VSlone
                (HFILEID f, int32 _HUGE * idarray, int32 asize);

    extern int32 Vlone
                (HFILEID f, int32 _HUGE * idarray, int32 asize);

    extern int32 Vfind
                (HFILEID f, const char _HUGE * vgname);

    extern int32 VSfind
                (HFILEID f, const char _HUGE * vsname);

    extern int32 Vfindclass
                (HFILEID f, const char _HUGE * vgclass);

    extern int32 VSfindclass
                (HFILEID f, const char _HUGE * vsclass);

    extern VOID Vsetzap
                (void);

/*
   ** from vgp.c
 */
    extern intn vcompare
                (VOIDP k1, VOIDP k2, intn cmparg);

    extern intn vcompareref
                (VOIDP k1, VOIDP k2, intn cmparg);

    extern VOID vdestroynode
                (VOIDP n);

    extern VOID vtfreekey
                (VOIDP k);

    extern intn Vinitialize
                (HFILEID f);

    extern intn Vfinish
                (HFILEID f);

    extern HFILEID Vopen
                (char *path, intn acc_mode, int16 ndds);

    extern intn Vclose
                (HFILEID f);

    extern int32 vexistvg
                (HFILEID f, uint16 vgid);

    extern int32 Vattach
                (HFILEID f, int32 vgid, const char _HUGE * accesstype);

    extern int32 Vdetach
                (int32 vkey);

    extern int32 Vinsert
                (int32 vkey, int32 vskey);
    /* note: 2nd arg of Vinsert can also be (VGROUP *) */

    extern int32 Vflocate
                (int32 vkey, char _HUGE * field);

    extern intn Vinqtagref
                (int32 vkey, int32 tag, int32 ref);

    extern int32 Vntagrefs
                (int32 vkey);

    extern int32 Vnrefs
                (int32 vkey,int32 tag);

    extern int32 Vgettagrefs
                (int32 vkey, int32 _HUGE tagarray[], int32 _HUGE refarray[], int32 n);

    extern intn Vgettagref
                (int32 vkey, int32 which, int32 _HUGE * tag, int32 _HUGE * ref);

    extern int32 VQueryref
                (int32 vkey);

    extern int32 VQuerytag
                (int32 vkey);

    extern int32 Vaddtagref
                (int32 vkey, int32 tag, int32 ref);

    extern int32 Ventries
                (HFILEID f, int32 vgid);

    extern int32 Vsetname
                (int32 vkey, const char _HUGE * vgname);

    extern int32 Vsetclass
                (int32 vkey, const char _HUGE * vgclass);

    extern intn Visvg
                (int32 vkey, int32 id);

    extern intn Visvs
                (int32 vkey, int32 id);

    extern int32 Vgetid
                (HFILEID f, int32 vgid);

    extern int32 Vgetnext
                (int32 vkey, int32 id);

    extern int32 Vgetname
                (int32 vkey, char _HUGE * vgname);

    extern int32 Vgetclass
                (int32 vkey, char _HUGE * vgclass);

    extern intn Vinquire
                (int32 vkey, int32 _HUGE * nentries, char _HUGE * vgname);

    extern int32 Vdelete
                (int32 f, int32 ref);

    extern intn VPshutdown(void);

/*
   ** from vparse.c
 */
    extern int32 scanattrs
                (const char _HUGE * attrs, int32 _HUGE * attrc, char _HUGE *** attrv);

/*
   ** from vhi.c
 */
    extern int32 VHstoredata
                (HFILEID f, char _HUGE * field, uint8 _HUGE buf[], int32 n, int32 datatype,
                 char _HUGE * vsname, char _HUGE * vsclass);

    extern int32 VHstoredatam
                (HFILEID f, char _HUGE * field, uint8 _HUGE buf[], int32 n, int32 datatype,
                 char _HUGE * vsname, char _HUGE * vsclass, int32 order);

    extern int32 VHmakegroup
                (HFILEID f, int32 _HUGE tagarray[], int32 _HUGE refarray[], int32 n, char _HUGE * vgname,
                 char _HUGE * vgclass);

/*
   ** from vio.c
 */

    extern int32 vexistvs
                (HFILEID f, uint16 vsref);

    extern VOID vsdestroynode
                (VOIDP n);

    extern int32 VSattach
                (HFILEID f, int32 vsref, const char _HUGE * accesstype);

    extern int32 VSdetach
                (int32 vkey);

    extern int32 VSQuerytag
                (int32 vkey);

    extern int32 VSQueryref
                (int32 vkey);

    extern int32 VSgetid
                (HFILEID f, int32 vsref);

    extern int32 VSgetversion
                (int32 vkey);

    extern int32 VSdelete
                (int32 f, int32 ref);

    extern int32 VSappendable
                (int32 vkey, int32 blk);

/*
   ** from vsfld.c
 */

    extern int16 VSIZEOF
                (int16 x);

    extern int16 HDFSIZEOF
                (int16 x);

    extern intn VSsetfields
                (int32 vkey, const char _HUGE * fields);

    extern intn VSfdefine
                (int32 vkey, const char _HUGE * field, int32 localtype, int32 order);

    extern int32 VFnfields
                (int32 vkey);

    extern char *VFfieldname
                (int32 vkey, int32 index);

    extern int32 VFfieldtype
                (int32 vkey, int32 index);

    extern int32 VFfieldisize
                (int32 vkey, int32 index);

    extern int32 VFfieldesize
                (int32 vkey, int32 index);

    extern int32 VFfieldorder
                (int32 vkey, int32 index);

/*
   ** from vrw.c
 */
    extern intn VSPfreebuf(void);

    extern int32 VSseek
                (int32 vkey, int32 eltpos);

    extern int32 VSread
                (int32 vkey, uint8 _HUGE buf[], int32 nelt, int32 interlace);

    extern int32 VSwrite
                (int32 vkey, uint8 _HUGE buf[], int32 nelt, int32 interlace);

/*
   ** from vgF.c
 */
#ifndef VG_FNAMES
#   define VG_FNAMES
#ifdef DF_CAPFNAMES
#   define  ndfivopn FNAME(DFIVOPN)
#   define  ndfvclos FNAME(DFVCLOS)
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
#   define  nvsfndc  FNAME(VSFNDC)
#   define  nvsgidc  FNAME(VSGIDC)
#   define  nvsdltc  FNAME(VSDLTC)
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
#   define  nvsqfnelt   FNAME(VSQFNELT)
#   define  nvsqfintr   FNAME(VSQFINTR)
#   define  nvsqfldsc   FNAME(VSQFLDSC)
#   define  nvsqfvsiz   FNAME(VSQFVSIZ)
#   define  nvsqnamec   FNAME(VSQNAMEC)
#else                           /* !DF_CAPFNAMES */
#   define  ndfivopn FNAME(dfivopn)
#   define  ndfvclos FNAME(dfvclos)
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
#   define  nvsfndc  FNAME(vsfndc)
#   define  nvsgidc  FNAME(vsgidc)
#   define  nvsdltc  FNAME(vsdltc)
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
#   define  nvsqfnelt   FNAME(vsqfnelt)
#   define  nvsqfintr   FNAME(vsqfintr)
#   define  nvsqfldsc   FNAME(vsqfldsc)
#   define  nvsqfvsiz   FNAME(vsqfvsiz)
#   define  nvsqnamec   FNAME(vsqnamec)
#endif                          /* DF_CAPFNAMES */
#endif                          /* VG_FNAMES */

    extern      FRETVAL(intf) ndfivopn
                (_fcd filename, intf _HUGE * acc_mode, intf _HUGE * defdds, intf _HUGE * namelen);

    extern      FRETVAL(intf) ndfvclos
                (intf _HUGE * file_id);

    extern      FRETVAL(intf) nvatchc
                (HFILEID _HUGE * f, intf _HUGE * vgid, _fcd accesstype);

    extern      FRETVAL(intf) nvdtchc
                (intf _HUGE * vkey);

    extern      FRETVAL(intf) nvgnamc
                (intf _HUGE * vkey, _fcd vgname);

    extern      FRETVAL(intf) nvgclsc
                (intf _HUGE * vkey, _fcd vgclass);

    extern      FRETVAL(intf) nvinqc
                (intf _HUGE * vkey, intf _HUGE * nentries, _fcd vgname);

    extern      FRETVAL(intf) nvgidc
                (HFILEID _HUGE * f, intf _HUGE * vgid);

    extern      FRETVAL(intf) nvgnxtc
                (intf _HUGE * vkey, intf _HUGE * id);

    extern      FRETVAL(intf) nvsnamc
                (intf _HUGE * vkey, _fcd vgname, intf _HUGE * vgnamelen);

    extern      FRETVAL(intf) nvsclsc
                (intf _HUGE * vkey, _fcd vgclass, intf _HUGE * vgclasslen);

    extern      FRETVAL(intf) nvinsrtc
                (intf _HUGE * vkey, intf _HUGE * vobjptr);

    extern      FRETVAL(intf) nvisvgc
                (intf _HUGE * vkey, intf _HUGE * id);

    extern      FRETVAL(intf) nvisvsc
                (intf _HUGE * vkey, intf _HUGE * id);

    extern      FRETVAL(intf) nvsatchc
                (HFILEID _HUGE * f, intf _HUGE * vsref, _fcd accesstype);

    extern      FRETVAL(intf) nvsdtchc
                (intf _HUGE * vkey);

    extern      FRETVAL(intf) nvsseekc
                (intf _HUGE * vkey, intf _HUGE * eltpos);

    extern      FRETVAL(intf) nvsgnamc
                (intf _HUGE * vkey, _fcd vsname);

    extern      FRETVAL(intf) nvsgclsc
                (intf _HUGE * vkey, _fcd vsclass);

    extern      FRETVAL(intf) nvsinqc
                (intf _HUGE * vkey, intf _HUGE * nelt, intf _HUGE * interlace,
                 _fcd fields, intf _HUGE * eltsize, _fcd vsname);

    extern      FRETVAL(intf) nvsfexc
                (intf _HUGE * vkey, _fcd fields, intf _HUGE * fieldslen);

    extern      FRETVAL(intf) nvsfndc
                (HFILEID _HUGE * f, _fcd name, intf _HUGE * namelen);

    extern      FRETVAL(intf) nvsgidc
                (HFILEID _HUGE * f, intf _HUGE * vsref);

    extern      FRETVAL(intf) nvsdltc
                (HFILEID _HUGE * f, intf _HUGE * vsref);

    extern      FRETVAL(intf) nvssnamc
                (intf _HUGE * vkey, _fcd vsname, intf _HUGE * vsnamelen);

    extern      FRETVAL(intf) nvssclsc
                (intf _HUGE * vkey, _fcd vsclass, intf _HUGE * vsclasslen);

    extern      FRETVAL(intf) nvssfldc
                (intf _HUGE * vkey, _fcd fields, intf _HUGE * fieldslen);

    extern      FRETVAL(intf) nvssintc
                (intf _HUGE * vkey, intf _HUGE * interlace);

    extern      FRETVAL(intf) nvsfdefc
                (intf _HUGE * vkey, _fcd field, intf _HUGE * localtype,
                 intf _HUGE * order, intf _HUGE * fieldlen);

    extern      FRETVAL(intf) nvsreadc
                (intf _HUGE * vkey, uint8 _HUGE * buf, intf _HUGE * nelt,
                 intf _HUGE * interlace);

    extern      FRETVAL(intf) nvswritc
                (intf _HUGE * vkey, uint8 _HUGE * buf, intf _HUGE * nelt,
                 intf _HUGE * interlace);

    extern      FRETVAL(intf) nvsgintc
                (intf _HUGE * vkey);

    extern      FRETVAL(intf) nvseltsc
                (intf _HUGE * vkey);

    extern      FRETVAL(intf) nvsgfldc
                (intf _HUGE * vkey, _fcd fields);

    extern      FRETVAL(intf) nvssizc
                (intf _HUGE * vkey, _fcd fields, intf _HUGE * fieldslen);

    extern      FRETVAL(intf) nventsc
                (HFILEID _HUGE * f, intf _HUGE * vgid);

    extern      FRETVAL(intf) nvlonec
                (HFILEID _HUGE * f, intf _HUGE * idarray, intf _HUGE * asize);

    extern      FRETVAL(intf) nvslonec
                (HFILEID _HUGE * f, intf _HUGE * idarray, intf _HUGE * asize);

    extern      FRETVAL(intf) nvhsdc
                (HFILEID _HUGE * f, _fcd field, uint8 _HUGE * buf, intf _HUGE * n, intf _HUGE * datatype,
    _fcd vsname, _fcd vsclass, intf _HUGE * fieldlen, intf _HUGE * vsnamelen,
                 intf _HUGE * vsclasslen);

    extern      FRETVAL(intf) nvhsdmc
                (HFILEID _HUGE * f, _fcd field, uint8 _HUGE * buf, intf _HUGE * n, intf _HUGE * datatype,
       _fcd vsname, _fcd vsclass, intf _HUGE * order, intf _HUGE * fieldlen,
                 intf _HUGE * vsnamelen, intf _HUGE * vsclasslen);

    extern      FRETVAL(intf) nvhmkgpc
                (HFILEID _HUGE * f, intf _HUGE * tagarray, intf _HUGE * refarray, intf _HUGE * n,
                 _fcd vgname, _fcd vgclass, intf _HUGE * vgnamelen, intf _HUGE * vgclasslen);

    extern      FRETVAL(intf) nvflocc
                (intf _HUGE * vkey, _fcd field, intf _HUGE * fieldlen);

    extern      FRETVAL(intf) nvinqtrc
                (intf _HUGE * vkey, intf _HUGE * tag, intf _HUGE * ref);

    extern      FRETVAL(intf) nvntrc
                (intf _HUGE * vkey);

    extern      FRETVAL(intf) nvgttrsc
                (intf _HUGE * vkey, intf _HUGE * tagarray, intf _HUGE * refarray, intf _HUGE * n);

    extern      FRETVAL(intf) nvgttrc
                (intf _HUGE * vkey, intf _HUGE * which, intf _HUGE * tag, intf _HUGE * ref);

    extern      FRETVAL(intf) nvadtrc
                (intf _HUGE * vkey, intf _HUGE * tag, intf _HUGE * ref);

#if defined c_plusplus || defined __cplusplus
}
#endif                          /* c_plusplus || __cplusplus */

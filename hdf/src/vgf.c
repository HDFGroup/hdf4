#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.1  1992/08/25 21:40:44  koziol
Initial revision

*/
/*
*
* vgF.c
* Part of the HDF VSet interface.
*
* C routines (short names) to be called from fortran 
*
*
************************************************************************/

#include "vg.h"

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

/* 
**  remove trailing blanks from a string. input argument is a  string
**  and *MUST* be a variable and not a constant!! For internal use only!! 
**  Used only on Crays where the Fortran compiler will pad strings to the 
**  nearest 8-byte boundary.
*/

void
#ifdef PROTOTYPE
trimendblanks(char *ss) 
#else
trimendblanks(ss) 

	char *ss;
#endif
{

	int32 i,n;
    n = DFIstrlen(ss);
	for(i=n-1;i>=0;i--) {
		if(ss[i]!=' ') {
			ss[i+1]='\0';
			break;
		}
	}
}

/* ================================================== */
/*  VGROUP routines                                   */
/* ================================================== */

/* 
**  attach to a vgroup and returns its ptr
**  related: Vattach--vatchc--VFATCH
*/

    FRETVAL(int32 *)
#ifdef PROTOTYPE
nvatchc(HFILEID *f, int32 *vgid, _fcd accesstype)
#else
nvatchc(f, vgid, accesstype)

HFILEID	*f;
int32       *vgid;
_fcd        accesstype;                     /* assume one character only */
#endif
{
	VGROUP *vg;
	char	 *acc;

    acc = HDf2cstring (accesstype, 1);

	vg = (VGROUP*) Vattach(*f, *vgid, acc);
	(void) VFREESPACE(acc);

	if(vg==NULL)
		return( (int32*) -1);
	else 
		return( (int32*) vg);
}

/* ------------------------------------------------------------------ */

/* 
**  detach from a vgroup
**  related: Vdetach--vdtchc--VFDTCH
*/

    FRETVAL(void)
#ifdef PROTOTYPE
nvdtchc(VGROUP **vg)
#else
nvdtchc(vg)
VGROUP  **vg;
#endif
{
	Vdetach(*vg);
}
/* ------------------------------------------------------------------ */

/* 
**  get the name of a vgroup
**  related: Vgetname--vgnamc--VFGNAM
*/

    FRETVAL(void)
#ifdef PROTOTYPE
nvgnamc(VGROUP **vg, _fcd vgname)
#else
nvgnamc(vg, vgname)
VGROUP  **vg;
_fcd        vgname;             /* output */
#endif

{
	Vgetname (*vg, vgname);

}	/* VGNAMC */

/* ------------------------------------------------------------------ */
/* 
**  get the class name of a vgroup
**  related: Vgetclass--vgclsc--VFGCLS
*/

    FRETVAL(void)
#ifdef PROTOTYPE
nvgclsc(VGROUP ** vg, _fcd vgclass)
#else
nvgclsc(vg, vgclass)

	VGROUP 	**vg;
	_fcd 		vgclass;				/* output */
#endif

{
	 Vgetclass (*vg, vgclass);

}	/* VGCLSC */

/* ------------------------------------------------------------------ */
/* 
**  general inquiry on a vgroup 
**  related: Vinquire--vinqc--VFINQ
*/

    FRETVAL(int32)
#ifdef PROTOTYPE
nvinqc(VGROUP** vg, int32 *nentries, _fcd vgname)
#else
nvinqc(vg, nentries, vgname)

	VGROUP 	**vg;
	int32	 	*nentries;			/* output */
	_fcd   	vgname;				/* output */
#endif

{
	return( (int32) Vinquire(*vg, nentries, vgname) );

} /* VINQC */


/* ------------------------------------------------------------------ */
/* 
**  gets the id of the next vgroup in the file
**  related: Vgetid--vgidc--VFGID
*/

    FRETVAL(int32)
#ifdef PROTOTYPE
nvgidc(HFILEID *f, int32 *vgid)
#else
nvgidc(f,vgid)

	HFILEID		*f;
	int32 		*vgid;
#endif
{
	return( (int32) Vgetid (*f, *vgid) );
}

/* ------------------------------------------------------------------ */
/* 
**  gets the id of the next entry in the vgroup
**  related: Vgetnext--vgnxtc--VFGNXT
*/

    FRETVAL(int32)
#ifdef PROTOTYPE
nvgnxtc(VGROUP **vg, int32 *id)
#else
nvgnxtc(vg,id)

    VGROUP  **vg;
	int32		*id;
#endif
{
	return( Vgetnext(*vg, *id) );
}

/* ------------------------------------------------------------------ */
/* 
**  sets the name of the vgroup
**  related: Vsetname--vsnamc--VFSNAM
*/

    FRETVAL(void)
#ifdef PROTOTYPE
nvsnamc(VGROUP **vg, _fcd vgname, intn *vgnamelen)
#else
nvsnamc(vg, vgname, vgnamelen)

	VGROUP	**vg;
	_fcd	vgname;
	intn 	*vgnamelen;
#endif

{
	char *name;

    name = HDf2cstring (vgname, (intn)*vgnamelen);
	/* trimendblanks(name); */
	Vsetname (*vg, name);
    VFREESPACE (name);
}

/* ------------------------------------------------------------------ */
/* 
**  sets the class name of the vgroup
**  related: Vsetclass--vsclsc--VFSCLS
*/

    FRETVAL(void)
#ifdef PROTOTYPE
nvsclsc(VGROUP **vg, _fcd vgclass, intn *vgclasslen)
#else
nvsclsc(vg, vgclass, vgclasslen)

	VGROUP	**vg;
	_fcd	vgclass;
	intn 	*vgclasslen;
#endif

{
	char *class;
    class = HDf2cstring (vgclass, (intn)*vgclasslen);
	/* trimendblanks(class); */
	Vsetclass (*vg, class);
    VFREESPACE(class);
}

/* ------------------------------------------------------------------ */
/* 
**  inserts a vset object (ie a vgroup or vdata ptr) into the given vgroup
**  related: Vinsert--vinsrtc--VFINSRT
*/

    FRETVAL(int32)
#ifdef PROTOTYPE
nvinsrtc(VGROUP **vg, int32 **vobjptr)
#else
nvinsrtc(vg, vobjptr)

	VGROUP	**vg;
	int32		**vobjptr;
#endif
{
    return( (int32) Vinsert(*vg, (VDATA *)*vobjptr) );
}

/* ------------------------------------------------------------------ */
/* 
**  tests if a vset object (having id id) in a vgroup refers to a vgroup
**  related: Visvg--visvgc--VFISVG
*/

    FRETVAL(int32)
#ifdef PROTOTYPE
nvisvgc(VGROUP **vg, int32 *id)
#else
nvisvgc(vg, id)

	VGROUP	**vg;
	int32		*id;
#endif
{
	return( (int32) Visvg(*vg, *id) );
}

/* ------------------------------------------------------------------ */
/* 
**  tests if an id in a vgroup refers to a vdata
**  related: Visvs--visvsc--VFISVS
*/

    FRETVAL(int32)
#ifdef PROTOTYPE
nvisvsc(VGROUP **vg, int32 *id)
#else
nvisvsc(vg, id)

	VGROUP	**vg;
	int32		*id;
#endif
{
	return( (int32) Visvs(*vg, *id) );
}

/* ================================================== */
/*  VDATA routines                                    */
/* ================================================== */

/* 
**  attach to a vdata and returns its ptr
**  related: VSattach--vsatchc--VFATCH
*/

    FRETVAL(int32 *)
#ifdef PROTOTYPE
nvsatchc(HFILEID *f, int32 *vsid, _fcd accesstype)
#else
nvsatchc(f, vsid, accesstype)

	HFILEID		*f;
	int32		*vsid;
	_fcd		accesstype;
#endif

{
	VDATA *vs;
	char 	*acc;

    acc = HDf2cstring (accesstype, 1); /* 'r' or 'w' only */
	vs =  (VDATA*) VSattach(*f, *vsid, acc);
    VFREESPACE(acc);
	if ( vs == NULL)
		return((int32*) -1);
	else 
		return((int32*) vs);
}

/* ------------------------------------------------------------------ */
/*  
**  detach from a vdata
**  related: VSdetach--vsdtchc--VFDTCH
*/

    FRETVAL(void)
#ifdef PROTOTYPE
nvsdtchc(VDATA **vs)
#else
nvsdtchc(vs)

	VDATA 	**vs;
#endif

{
	VSdetach (*vs);
}

/* ------------------------------------------------------------------ */
/* 
**  seeks to a given element position in a vadata
**  related: VSseek--vsseekc--VSFSEEK
*/

    FRETVAL(int32)
#ifdef PROTOTYPE
nvsseekc(VDATA **vs, int32 *eltpos)
#else
nvsseekc(vs, eltpos)

	VDATA 	**vs;
	int32		*eltpos;
#endif

{
	return( (int32) VSseek(*vs, *eltpos) );
}

/* ------------------------------------------------------------------ */
/* 
**  gets the name of a vdata
**  related: VSgetname--vsgnamc--VSFGNAM
*/

    FRETVAL(void)
#ifdef PROTOTYPE
nvsgnamc(VDATA **vs, _fcd vsname)
#else
nvsgnamc(vs, vsname)

	VDATA 	**vs;
	_fcd 		vsname;
#endif

{
	VSgetname (*vs, vsname);
}	/* VSGNAMC */

/* ------------------------------------------------------------------ */
/* 
**  get the class name of a vdata
**  related: VSgetclass--vsgclsc--VSFGCLS
*/

    FRETVAL(void)
#ifdef PROTOTYPE
nvsgclsc(VDATA **vs, _fcd vsclass)
#else
nvsgclsc(vs, vsclass)

    VDATA   **vs;
	_fcd 		vsclass;					/* output */
#endif

{
	VSgetclass(*vs, vsclass);
}	/* VSGCLSC */

/* ------------------------------------------------------------------ */
/*
**  general inquiry on a vdata
**  related: VSinquire--vsinqc--VSFINQ
*/

    FRETVAL(int32)
#ifdef PROTOTYPE
nvsinqc(VDATA **vs, int32 *nelt ,int32 *interlace, _fcd fields, int32 *eltsize, _fcd vsname)
#else
nvsinqc(vs, nelt ,interlace, fields, eltsize, vsname)

	VDATA 	**vs;
	int32		*nelt, *interlace, *eltsize;			/* outputs */
	_fcd  	fields, vsname;							/* outputs */
#endif

{
	return( (int32) VSinquire (*vs, nelt, interlace, fields, eltsize, vsname) );
} 	/* VSINQC */


/* ------------------------------------------------------------------ */
/* 
**  tests if given fields exist in the vdata
**  related: VSfexist--vsfexc--VSFEX
*/

    FRETVAL(int32)
#ifdef PROTOTYPE
nvsfexc(VDATA **vs, _fcd fields, intn *fieldslen)
#else
nvsfexc(vs, fields, fieldslen)

	VDATA 	**vs;
	_fcd	fields;
	intn	*fieldslen;
#endif

{
	int32 	stat;
	char		*flds;

    flds = HDf2cstring (fields, (intn)*fieldslen );
	/* trimendblanks(flds); */
	stat =  (int32) VSfexist(*vs, flds);
    VFREESPACE(flds);

	return (stat);
}

/* ------------------------------------------------------------------ */
/* 
**  gets the id of the next vdata from the file
**  related: VSgetid--vsgidc--VSFGID
*/

    FRETVAL(int32)
#ifdef PROTOTYPE
nvsgidc(HFILEID *f, int32 *vsid)
#else
nvsgidc(f, vsid)

	HFILEID		*f;
	int32			*vsid;
#endif

{
	return( (int32) VSgetid(*f, *vsid) );
}

/* ------------------------------------------------------------------ */
/* 
**  sets the name of a vdata
**  related: VSsetname--vssnamc--VSFSNAM
*/

    FRETVAL(void)
#ifdef PROTOTYPE
nvssnamc(VDATA **vs, _fcd vsname,intn *vsnamelen)
#else
nvssnamc(vs, vsname, vsnamelen)

	VDATA  **vs;
	_fcd   vsname;
	intn   *vsnamelen;
#endif

{
	char   *name;

    name = HDf2cstring(vsname, (intn)*vsnamelen);
	/* trimendblanks (name); */
	VSsetname (*vs, name);
    VFREESPACE (name);
}

/* ------------------------------------------------------------------ */
/* 
**  sets the class name of the vdata
**  related: VSsetclass--vssclsc--VSFSCLS
*/

    FRETVAL(void)
#ifdef PROTOTYPE
nvssclsc(VDATA **vs, _fcd vsclass, intn *vsclasslen)
#else
nvssclsc(vs, vsclass, vsclasslen)

    VDATA	**vs;
    _fcd	vsclass;
    intn	*vsclasslen;
#endif

{
	char 	*class;

    class = HDf2cstring (vsclass, (intn)*vsclasslen);
	/* trimendblanks(class); */
	VSsetclass (*vs, class);
    VFREESPACE(class);
}

/* ------------------------------------------------------------------ */
/* 
**  sets the fields in a vdata for reading or writing
**  related: VSsetfields--vssfldc--VSFSFLD
*/

    FRETVAL(int32)
#ifdef PROTOTYPE
nvssfldc(VDATA **vs, _fcd fields, intn *fieldslen)
#else
nvssfldc(vs,fields, fieldslen)

	VDATA 	**vs;
	_fcd	fields;
	intn	*fieldslen;
#endif

{
	char 	*flds;
	int32	stat;

    flds = HDf2cstring (fields, (intn)*fieldslen);
	/* trimendblanks(flds); */
	stat =	(int32) VSsetfields (*vs, flds);
    VFREESPACE(flds);

	return(stat);
}

/* ------------------------------------------------------------------ */
/* 
**  sets the file interlace of a vdata
**  related: VSsetinterlace--vssintc--VSFSINT
*/

    FRETVAL(int32)
#ifdef PROTOTYPE
nvssintc(VDATA **vs, int32 *interlace)
#else
nvssintc(vs, interlace)

	VDATA  		**vs;
	int32	 		*interlace;
#endif

{
	return( (int32) VSsetinterlace (*vs, *interlace) );
}

/* ------------------------------------------------------------------ */
/* 
**  defines a new field to be used in the vdata
**  related: VSfdefine--vsfdefc--VSFFDEF
*/

    FRETVAL(int32)
#ifdef PROTOTYPE
nvsfdefc(VDATA **vs, _fcd field, int32 *localtype, int32 *order, intn *fieldlen)
#else
nvsfdefc(vs, field, localtype, order, fieldlen)

	VDATA 		**vs;
	_fcd			field;		
	int32 		*localtype, *order;
	intn			*fieldlen;
#endif

{
	int32 	stat;
	char 		*fld;

    fld  = HDf2cstring (field, (intn)*fieldlen);
	/* trimendblanks(fld); */
	stat =  (int32) VSfdefine(*vs, fld, *localtype, *order );
    VFREESPACE(fld);
	return (stat);
}

/* ------------------------------------------------------------------ */
/* 
**  reads from a vdata
**  related: VSread--vsreadc--VSFREAD
*/

    FRETVAL(int32)
#ifdef PROTOTYPE
nvsreadc(VDATA **vs, BYTE *buf, int32 *nelt, int32 *interlace)
#else
nvsreadc(vs, buf, nelt, interlace)

	VDATA	   	**vs;
	int32			*nelt, *interlace;
	BYTE			*buf;
#endif

{
	return( (int32) VSread(*vs, buf, *nelt, *interlace) );
}

/* ------------------------------------------------------------------ */
/* 
**  writes to a vdata
**  related: VSwrite--vswritc--VSFWRIT
*/

    FRETVAL(int32)
#ifdef PROTOTYPE
nvswritc(VDATA **vs, BYTE *buf, int32 *nelt, int32 *interlace)
#else
nvswritc(vs, buf, nelt, interlace)

	VDATA	   	**vs;
	int32			*nelt, *interlace;
	BYTE			*buf;
#endif

{
	return( (int32) VSwrite(*vs, buf, *nelt, *interlace) );
}

/* ======================================== */
/* miscellaneous VDATA inquiry routines */
/* ======================================== */
/* undocumented */

/* ------------------------------------------------------------------ */
/* 
**  gets the interlace of the vdata
**  related: VSgetinterlace--vsgintc--VSFGINT
*/

    FRETVAL(int32)
#ifdef PROTOTYPE
nvsgintc(VDATA **vs)
#else
nvsgintc(vs)

	VDATA 		**vs;
#endif

{
	return( (int32) VSgetinterlace(*vs) );
}

/* ------------------------------------------------------------------ */
/* 
**  gets the number of elements in the vdata
**  related: VSelts--vseltsc--VSFELTS
*/

    FRETVAL(int32)
#ifdef PROTOTYPE
nvseltsc(VDATA **vs)
#else
nvseltsc(vs)

	VDATA 		**vs;
#endif

{
	return( (int32) VSelts (*vs) );
}

/* ------------------------------------------------------------------ */
/* 
**  gets the fields in the vdata
**  related: VSgetfields--vsgfldc--VSFGFLD
*/

    FRETVAL(int32)
#ifdef PROTOTYPE
nvsgfldc(VDATA **vs, _fcd fields)
#else
nvsgfldc(vs, fields)

	VDATA 	**vs;
	_fcd  	fields;			/* output */
#endif

{
	return ( (int32) VSgetfields (*vs, fields) );

}	/* VSGFLDC */

/* ------------------------------------------------------------------ */
/* 
**  determines the (machine) size of the given fields
**  related: VSsizeof--vssizc--VSFSIZ
*/

    FRETVAL(int32)
#ifdef PROTOTYPE
nvssizc(VDATA **vs, _fcd fields, intn *fieldslen)
#else
nvssizc(vs, fields, fieldslen)

	VDATA 	**vs;
	_fcd	fields;
	intn	*fieldslen;
#endif

{
	char 	*flds;
	int32 	stat;

    flds = HDf2cstring (fields, (intn)*fieldslen);
	/* trimendblanks(flds); */
    stat =  VSsizeof(*vs, flds);
    VFREESPACE(flds);
	return (stat);
}

/* ------------------------------------------------------------------ */
/*
**  determines the no of entries in a vgroup
**  related: Ventries--ventsc--VFENTS
*/

    FRETVAL(int32)
#ifdef PROTOTYPE
nventsc(HFILEID *f,int32 *vgid)
#else
nventsc(f,vgid)

	HFILEID *f;
	int32 * vgid;
#endif

{
	return( (int32) Ventries (*f,*vgid) );
}

/* ======================================================= */
/* DEBUGGING ROUTINES                                      */
/* ======================================================= */
/*
**  enable debug
**  related: setjj--setjjc--SETFJJ
*/

    FRETVAL(void)
#ifdef PROTOTYPE
nsetjjc(void)
#else
nsetjjc()
#endif

{ 
	setjj(); 
}

/* ------------------------------------------------------------------ */
/*
**  disable debug
**  related: setnojj--setnojjc--SETFNJJ
*/

    FRETVAL(void)
#ifdef PROTOTYPE
nsetnojjc(void)
#else
nsetnojjc()
#endif

{ 
	setnojj(); 
}

/* ------------------------------------------------------------------ */
/* 
**  gets the refs of all lone vgroups in the file
**  related: Vlone--vlonec--VFLONE
*/

    FRETVAL(int32)
#ifdef PROTOTYPE
nvlonec(HFILEID *f, int32 **idarray, int32 *asize)
#else
nvlonec(f, idarray, asize)

	HFILEID		*f;
	int32	**idarray; 			/* output -- an integer array */
	int32	*asize;
#endif

{
	return( (int32) Vlone( *f, *idarray, *asize) );
}

/* ------------------------------------------------------------------ */
/*
**  gets the refs of all lone vdatas in the file
**  related: VSlone--vslonec--VSFLONE
*/

    FRETVAL(int32)
#ifdef PROTOTYPE
nvslonec(HFILEID *f, int32 **idarray, int32 *asize)
#else
nvslonec(f, idarray, asize)

	HFILEID		*f;
	int32	**idarray; 		/* output -- an integer array */
	int32	*asize;
#endif

{
	return( VSlone( *f, *idarray, *asize) );
}

/*
** ==================================================================
** HIGH-LEVEL VSET ROUTINES --- VHxxxxx()
** ==================================================================
*/

/*
**  store a simple dataset in a vdata 
**  related: VHstoredata--vhsdc--vhfsd
*/

    FRETVAL(int32)
#ifdef PROTOTYPE
nvhsdc(HFILEID *f, _fcd field, BYTE *buf, int32 *n, int32 *datatype, _fcd vsname, _fcd vsclass, intn *fieldlen, intn *vsnamelen, intn *vsclasslen)
#else
nvhsdc(f, field, buf, n, datatype, vsname, vsclass, fieldlen,
          vsnamelen, vsclasslen)

	HFILEID		*f;
	_fcd   	field, vsname, vsclass;
	int32	*n, *datatype;
	BYTE	*buf;
        intn    *fieldlen, *vsnamelen, *vsclasslen;
#endif

{
        char *fld, *name, *class;

        fld = HDf2cstring(field, *fieldlen);
        name = HDf2cstring(vsname, *vsnamelen);
        class = HDf2cstring(vsclass, *vsclasslen);

	return( (int32) VHstoredata (*f, fld, buf, *n, *datatype, 
                 name, class));
}

/* ------------------------------------------------------------------ */
/*
**  store an aggregate dataset in a vdata
**  related: VHstoredatam--vhsdmc--vhfsdm
*/

    FRETVAL(int32)
#ifdef PROTOTYPE
nvhsdmc(HFILEID *f, _fcd field, BYTE *buf, int32 *n, int32 *datatype,
        _fcd vsname, _fcd vsclass, int32 *order, intn *fieldlen,
        intn *vsnamelen, intn *vsclasslen)
#else
nvhsdmc(f, field, buf, n, datatype, vsname, vsclass, order,
           fieldlen, vsnamelen, vsclasslen)

	HFILEID	*f;
	_fcd  	field, vsname, vsclass;
	int32	*n, *datatype, *order;
	BYTE	*buf;
        intn    *fieldlen, *vsnamelen, *vsclasslen;
#endif

{
        char *fld, *name, *class;

        fld = HDf2cstring(field, *fieldlen);
        name = HDf2cstring(vsname, *vsnamelen);
        class = HDf2cstring(vsclass, *vsclasslen);

        return( (int32) VHstoredatam (*f, fld , buf, *n, 
          *datatype, name, class, *order));
}

/* ------------------------------------------------------------------ */
/*
**  make a new vgroup given several tag/ref pairs 
**  related: VHmakegroup--vhmkgpc--vhfmkgp
*/

    FRETVAL(int32)
#ifdef PROTOTYPE
nvhmkgpc(HFILEID *f, int32 *tagarray, int32 *refarray, int32 *n,
            _fcd vgname, _fcd vgclass, intn *vgnamelen, 
            intn *vgclasslen)
#else
nvhmkgpc(f, tagarray, refarray , n, vgname, vgclass, vgnamelen,
          vgclasslen)

	HFILEID	*f;
	_fcd   	vgname, vgclass;
	int32	*n, *tagarray, *refarray;
        intn    *vgnamelen, *vgclasslen;
#endif

{
        char *gname, *gclass;

        gname = HDf2cstring(vgname, *vgnamelen);
        gclass = HDf2cstring(vgclass, *vgclasslen);


	return( (int32) VHmakegroup (*f, tagarray, refarray, *n, gname, gclass));
}

/* ================================================================== */
/*
**  locate a field in a vdata that belongs to this VGROUP
**  related: Vflocate--vffloc--vflocc
*/

    FRETVAL(int32)
#ifdef PROTOTYPE
nvflocc(VGROUP **vg, _fcd field, intn *fieldlen)
#else
nvflocc(vg, field, fieldlen)

	VGROUP	**vg;
	_fcd   	field;
	intn	*fieldlen;
#endif

{
    char  *fld;
    int32 stat;

    fld = HDf2cstring (field, (intn)*fieldlen);
	/* trimendblanks(fld); */
	stat = (int32) Vflocate (*vg, fld);
    VFREESPACE(fld);

	return(stat);
}

/* ------------------------------------------------------------------ */
/* 
**  tests if a tag/ref pair is in a vgroup.
**  related: Vinqtagref--vinqtrc--vfinqtr
*/

    FRETVAL(int32)
#ifdef PROTOTYPE
nvinqtrc(VGROUP **vg, int32 *tag, int32 *ref)
#else
nvinqtrc(vg, tag, ref)

	VGROUP	**vg;
	int32 	*tag, *ref;
#endif

{
	return ( (int32) Vinqtagref ( *vg, *tag, *ref) );
}
/* ------------------------------------------------------------------ */
/* 
**  gets the number of tag/refs stored in a vgroup
**  related: Vntagrefs--vntrc--VFNTR
*/

    FRETVAL(int32)
#ifdef PROTOTYPE
nvntrc(VGROUP **vg)
#else
nvntrc(vg)

	VGROUP	**vg;
#endif

{
	return ( (int32) Vntagrefs (*vg) );
}
/* ------------------------------------------------------------------ */

/*
**  returns all the tag/ref pairs in a vgroup 
**  related: Vgettagrefs--vgttrsc--vfgttrs
*/

    FRETVAL(int32)
#ifdef PROTOTYPE
nvgttrsc(VGROUP **vg, int32 *tagarray, int32 *refarray, int32 *n)
#else
nvgttrsc(vg, tagarray, refarray, n)

	VGROUP	**vg;
	int32		*tagarray, *refarray; 		/* outputs - integer arrays */
	int32 	*n;
#endif

{
	return ( (int32)  Vgettagrefs (*vg, tagarray, refarray, *n) );
}

/* ------------------------------------------------------------------ */
/*
**  returns a specified tag/ref pair in a vgroup 
**  related: Vgettagref--vgttrc--vfgttr
*/

    FRETVAL(int32)
#ifdef PROTOTYPE
nvgttrc(VGROUP **vg, int32 *which, int32 *tag, int32 *ref)
#else
nvgttrc(vg, which, tag, ref)

	VGROUP	**vg;
	int32		*which;
	int32 	*tag, *ref;						/* outputs */
#endif
{
	return ( (int32) Vgettagref (*vg, *which, tag, ref) );
}
/* ------------------------------------------------------------------ */

/*
**  add a tag/ref pair to a vgroup 
**  related: Vaddtagref--vadtrc--VFADTR 
*/

    FRETVAL(int32)
#ifdef PROTOTYPE
nvadtrc(VGROUP **vg, int32 *tag, int32 *ref)
#else
nvadtrc(vg, tag, ref)

	VGROUP	**vg;
	int32 	*tag, *ref;
#endif
{
	return ( (int32) Vaddtagref ( *vg, *tag, *ref) );
}

/* ------------------------------------------------------------------ */


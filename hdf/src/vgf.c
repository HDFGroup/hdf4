#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.5  1993/01/19 05:56:17  koziol
Merged Hyperslab and JPEG routines with beginning of DEC ALPHA
port.  Lots of minor annoyances fixed.

 * Revision 1.4  1992/11/30  22:00:01  chouck
 * Added fixes for changing to Vstart and Vend
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
*
* vgF.c
* Part of the HDF VSet interface.
*
* C routines (short names) to be called from fortran 
*
*
************************************************************************/

#include "vg.h"

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

    n = HDstrlen(ss);
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

/*-----------------------------------------------------------------------------
 * Name:    dfivopn
 * Purpose: Fortran stub for dfvopen to call DFvsetopen to open HDF file
 * Inputs:  name: name of file to open
 *          access: access mode - integer with value DFACC_READ etc.
 *          defdds: default number of DDs per header block
 *          namelen: length of name
 * Returns: 0 on success, -1 on failure with error set
 * Users:   HDF Fortran programmers
 * Invokes: Hopen
 * Method:  Convert filename to C string, call Hopen
 *---------------------------------------------------------------------------*/

   FRETVAL(intf)
#ifdef PROTOTYPE
ndfivopn(_fcd name, intf *access, intf *defdds, intf *namelen)
#else
ndfivopn(name, access, defdds, namelen)
_fcd name;
intf *access;
intf *defdds;
intf *namelen;
#endif /* PROTOTYPE */
{
   char *fn;
   intf ret;

   fn = HDf2cstring(name, *namelen);
   ret = (intf) DFvsetopen(fn, *access, *defdds);
   HDfreespace(fn);
   return(ret);
}	/* end ndfivopn() */

/*-----------------------------------------------------------------------------
 * Name:    dfvclos
 * Purpose: Call DFvsetclose to close HDF file
 * Inputs:  file_id: handle to HDF file to close
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 * Invokes: Hclose
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
#ifdef PROTOTYPE
ndfvclos(intf *file_id)
#else
ndfvclos(file_id)
intf *file_id;
#endif /* PROTOTYPE */
{
   return(DFvsetclose(*file_id));
}	/* ndfvclos() */

/* 
**  attach to a vgroup and returns its ptr
**  related: Vattach--vatchc--VFATCH
*/

    FRETVAL(intf *)
#ifdef PROTOTYPE
nvatchc(HFILEID *f, intf *vgid, _fcd accesstype)
#else
nvatchc(f, vgid, accesstype)
    HFILEID *f;
    intf       *vgid;
    _fcd        accesstype;                     /* assume one character only */
#endif
{
	VGROUP *vg;
	char	 *acc;

    acc = HDf2cstring (accesstype, 1);

	vg = (VGROUP*) Vattach(*f, *vgid, acc);
    (void) HDfreespace(acc);

	if(vg==NULL)
        return( (intf *) -1);
	else 
        return( (intf *) vg);
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
    _fcd    vgname;             /* output */
#endif

{
	Vgetname (*vg, vgname);
}   /* VGNAMC */

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
    VGROUP  **vg;
	_fcd 		vgclass;				/* output */
#endif

{
	 Vgetclass (*vg, vgclass);
}   /* VGCLSC */

/* ------------------------------------------------------------------ */
/* 
**  general inquiry on a vgroup 
**  related: Vinquire--vinqc--VFINQ
*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nvinqc(VGROUP** vg, intf *nentries, _fcd vgname)
#else
nvinqc(vg, nentries, vgname)
    VGROUP  **vg;
    intf    *nentries;          /* output */
	_fcd   	vgname;				/* output */
#endif

{
    return( (intf) Vinquire(*vg, (int32 *)nentries, vgname) );
} /* VINQC */


/* ------------------------------------------------------------------ */
/* 
**  gets the id of the next vgroup in the file
**  related: Vgetid--vgidc--VFGID
*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nvgidc(HFILEID *f, intf *vgid)
#else
nvgidc(f,vgid)
    HFILEID     *f;
    intf        *vgid;
#endif
{
    return( (intf) Vgetid (*f, *vgid) );
}

/* ------------------------------------------------------------------ */
/* 
**  gets the id of the next entry in the vgroup
**  related: Vgetnext--vgnxtc--VFGNXT
*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nvgnxtc(VGROUP **vg, intf *id)
#else
nvgnxtc(vg,id)
    VGROUP  **vg;
    intf    *id;
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
nvsnamc(VGROUP **vg, _fcd vgname, intf *vgnamelen)
#else
nvsnamc(vg, vgname, vgnamelen)
    VGROUP  **vg;
	_fcd	vgname;
    intf    *vgnamelen;
#endif
{
	char *name;

    name = HDf2cstring (vgname, (intn)*vgnamelen);
	/* trimendblanks(name); */
	Vsetname (*vg, name);
    HDfreespace (name);
}

/* ------------------------------------------------------------------ */
/* 
**  sets the class name of the vgroup
**  related: Vsetclass--vsclsc--VFSCLS
*/

    FRETVAL(void)
#ifdef PROTOTYPE
nvsclsc(VGROUP **vg, _fcd vgclass, intf *vgclasslen)
#else
nvsclsc(vg, vgclass, vgclasslen)
    VGROUP  **vg;
	_fcd	vgclass;
    intf    *vgclasslen;
#endif
{
	char *class;

    class = HDf2cstring (vgclass, (intn)*vgclasslen);
	/* trimendblanks(class); */
	Vsetclass (*vg, class);
    HDfreespace (class);
}

/* ------------------------------------------------------------------ */
/* 
**  inserts a vset object (ie a vgroup or vdata ptr) into the given vgroup
**  related: Vinsert--vinsrtc--VFINSRT
*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nvinsrtc(VGROUP **vg, intf **vobjptr)
#else
nvinsrtc(vg, vobjptr)
    VGROUP  **vg;
    intf    **vobjptr;
#endif
{
    return( (intf) Vinsert(*vg, (VDATA *)*vobjptr) );
}

/* ------------------------------------------------------------------ */
/* 
**  tests if a vset object (having id id) in a vgroup refers to a vgroup
**  related: Visvg--visvgc--VFISVG
*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nvisvgc(VGROUP **vg, intf *id)
#else
nvisvgc(vg, id)

	VGROUP	**vg;
    intf    *id;
#endif
{
    return( (intf) Visvg(*vg, *id) );
}

/* ------------------------------------------------------------------ */
/* 
**  wrapper for Vstart
*/

    FRETVAL(void)
#ifdef PROTOTYPE
nvfstart(HFILEID *f)
#else
nvfstart(f)
    HFILEID *f;
#endif
{
    Vstart(*f);
} /* nvfstart */

/* ------------------------------------------------------------------ */
/* 
**  wrapper for Vend
*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nvfend(HFILEID *f)
#else
nvfend(f)
    HFILEID *f;
#endif
{
    return( (intf) Vend(*f));
} /* nvfend */

/* ------------------------------------------------------------------ */
/* 
**  tests if an id in a vgroup refers to a vdata
**  related: Visvs--visvsc--VFISVS
*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nvisvsc(VGROUP **vg, intf *id)
#else
nvisvsc(vg, id)
    VGROUP  **vg;
    intf    *id;
#endif
{
    return( (intf) Visvs(*vg, *id) );
}

/* ================================================== */
/*  VDATA routines                                    */
/* ================================================== */

/* 
**  attach to a vdata and returns its ptr
**  related: VSattach--vsatchc--VFATCH
*/

    FRETVAL(intf *)
#ifdef PROTOTYPE
nvsatchc(HFILEID *f, intf *vsid, _fcd accesstype)
#else
nvsatchc(f, vsid, accesstype)
    HFILEID     *f;
    intf        *vsid;
	_fcd		accesstype;
#endif
{
	VDATA *vs;
	char 	*acc;

    acc = HDf2cstring (accesstype, 1); /* 'r' or 'w' only */
	vs =  (VDATA*) VSattach(*f, *vsid, acc);
    HDfreespace(acc);
	if ( vs == NULL)
        return((intf *) -1);
	else 
        return((intf *) vs);
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
    VDATA   **vs;
#endif
{
	VSdetach (*vs);
}

/* ------------------------------------------------------------------ */
/* 
**  seeks to a given element position in a vadata
**  related: VSseek--vsseekc--VSFSEEK
*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nvsseekc(VDATA **vs, intf *eltpos)
#else
nvsseekc(vs, eltpos)
    VDATA   **vs;
    intf    *eltpos;
#endif
{
    return( (intf) VSseek(*vs, *eltpos) );
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
    VDATA   **vs;
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
    _fcd    vsclass;                    /* output */
#endif
{
	VSgetclass(*vs, vsclass);
}	/* VSGCLSC */

/* ------------------------------------------------------------------ */
/*
**  general inquiry on a vdata
**  related: VSinquire--vsinqc--VSFINQ
*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nvsinqc(VDATA **vs, intf *nelt ,intf *interlace, _fcd fields, intf *eltsize,
    _fcd vsname)
#else
nvsinqc(vs, nelt ,interlace, fields, eltsize, vsname)
    VDATA   **vs;
    intf    *nelt, *interlace, *eltsize;            /* outputs */
	_fcd  	fields, vsname;							/* outputs */
#endif
{
    return( (intf) VSinquire (*vs, (int32 *)nelt, (int32 *)interlace,
            fields, (int32 *)eltsize, vsname) );
} 	/* VSINQC */


/* ------------------------------------------------------------------ */
/* 
**  tests if given fields exist in the vdata
**  related: VSfexist--vsfexc--VSFEX
*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nvsfexc(VDATA **vs, _fcd fields, intf *fieldslen)
#else
nvsfexc(vs, fields, fieldslen)
    VDATA   **vs;
	_fcd	fields;
    intf    *fieldslen;
#endif
{
    intf    stat;
	char		*flds;

    flds = HDf2cstring (fields, (intn)*fieldslen );
	/* trimendblanks(flds); */
	stat =  (int32) VSfexist(*vs, flds);
    HDfreespace (flds);

	return (stat);
}

/* ------------------------------------------------------------------ */
/* 
**  gets the id of the next vdata from the file
**  related: VSgetid--vsgidc--VSFGID
*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nvsgidc(HFILEID *f, intf *vsid)
#else
nvsgidc(f, vsid)
    HFILEID *f;
    intf    *vsid;
#endif
{
    return( (intf) VSgetid(*f, *vsid) );
}

/* ------------------------------------------------------------------ */
/* 
**  sets the name of a vdata
**  related: VSsetname--vssnamc--VSFSNAM
*/

    FRETVAL(void)
#ifdef PROTOTYPE
nvssnamc(VDATA **vs, _fcd vsname,intf *vsnamelen)
#else
nvssnamc(vs, vsname, vsnamelen)
    VDATA  **vs;
	_fcd   vsname;
    intf   *vsnamelen;
#endif
{
	char   *name;

    name = HDf2cstring(vsname, (intn)*vsnamelen);
	/* trimendblanks (name); */
	VSsetname (*vs, name);
    HDfreespace (name);
}

/* ------------------------------------------------------------------ */
/* 
**  sets the class name of the vdata
**  related: VSsetclass--vssclsc--VSFSCLS
*/

    FRETVAL(void)
#ifdef PROTOTYPE
nvssclsc(VDATA **vs, _fcd vsclass, intf *vsclasslen)
#else
nvssclsc(vs, vsclass, vsclasslen)
    VDATA   **vs;
    _fcd	vsclass;
    intf    *vsclasslen;
#endif
{
	char 	*class;

    class = HDf2cstring (vsclass, (intn)*vsclasslen);
	/* trimendblanks(class); */
	VSsetclass (*vs, class);
    HDfreespace (class);
}

/* ------------------------------------------------------------------ */
/* 
**  sets the fields in a vdata for reading or writing
**  related: VSsetfields--vssfldc--VSFSFLD
*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nvssfldc(VDATA **vs, _fcd fields, intf *fieldslen)
#else
nvssfldc(vs,fields, fieldslen)
    VDATA   **vs;
	_fcd	fields;
    intf    *fieldslen;
#endif
{
	char 	*flds;
    intf    stat;

    flds = HDf2cstring (fields, (intn)*fieldslen);
	/* trimendblanks(flds); */
	stat =	(int32) VSsetfields (*vs, flds);
    HDfreespace (flds);

	return(stat);
}

/* ------------------------------------------------------------------ */
/* 
**  sets the file interlace of a vdata
**  related: VSsetinterlace--vssintc--VSFSINT
*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nvssintc(VDATA **vs, intf *interlace)
#else
nvssintc(vs, interlace)
    VDATA   **vs;
    intf    *interlace;
#endif
{
    return( (intf) VSsetinterlace (*vs, *interlace) );
}

/* ------------------------------------------------------------------ */
/* 
**  defines a new field to be used in the vdata
**  related: VSfdefine--vsfdefc--VSFFDEF
*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nvsfdefc(VDATA **vs, _fcd field, intf *localtype, intf *order, intf *fieldlen)
#else
nvsfdefc(vs, field, localtype, order, fieldlen)
    VDATA   **vs;
    _fcd    field;
    intf    *localtype, *order;
    intf    *fieldlen;
#endif
{
    intf    stat;
    char    *fld;

    fld  = HDf2cstring (field, (intn)*fieldlen);
	/* trimendblanks(fld); */
	stat =  (int32) VSfdefine(*vs, fld, *localtype, *order );
    HDfreespace(fld);
	return (stat);
}

/* ------------------------------------------------------------------ */
/* 
**  reads from a vdata
**  related: VSread--vsreadc--VSFREAD
*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nvsreadc(VDATA **vs, BYTE *buf, intf *nelt, intf *interlace)
#else
nvsreadc(vs, buf, nelt, interlace)
    VDATA       **vs;
    BYTE        *buf;
    intf        *nelt, *interlace;
#endif
{
    return( (intf) VSread(*vs, buf, *nelt, *interlace) );
}

/* ------------------------------------------------------------------ */
/* 
**  writes to a vdata
**  related: VSwrite--vswritc--VSFWRIT
*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nvswritc(VDATA **vs, BYTE *buf, intf *nelt, intf *interlace)
#else
nvswritc(vs, buf, nelt, interlace)
    VDATA       **vs;
    BYTE        *buf;
    intf        *nelt, *interlace;
#endif
{
    return( (intf) VSwrite(*vs, buf, *nelt, *interlace) );
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

    FRETVAL(intf)
#ifdef PROTOTYPE
nvsgintc(VDATA **vs)
#else
nvsgintc(vs)
    VDATA       **vs;
#endif
{
    return( (intf) VSgetinterlace(*vs) );
}

/* ------------------------------------------------------------------ */
/* 
**  gets the number of elements in the vdata
**  related: VSelts--vseltsc--VSFELTS
*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nvseltsc(VDATA **vs)
#else
nvseltsc(vs)
    VDATA       **vs;
#endif
{
    return( (intf) VSelts (*vs) );
}

/* ------------------------------------------------------------------ */
/* 
**  gets the fields in the vdata
**  related: VSgetfields--vsgfldc--VSFGFLD
*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nvsgfldc(VDATA **vs, _fcd fields)
#else
nvsgfldc(vs, fields)
    VDATA   **vs;
	_fcd  	fields;			/* output */
#endif
{
    return ( (intf) VSgetfields (*vs, fields) );
}   /* VSGFLDC */

/* ------------------------------------------------------------------ */
/* 
**  determines the (machine) size of the given fields
**  related: VSsizeof--vssizc--VSFSIZ
*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nvssizc(VDATA **vs, _fcd fields, intf *fieldslen)
#else
nvssizc(vs, fields, fieldslen)
    VDATA   **vs;
	_fcd	fields;
    intf    *fieldslen;
#endif
{
	char 	*flds;
    intf    stat;

    flds = HDf2cstring (fields, (intn)*fieldslen);
	/* trimendblanks(flds); */
    stat =  VSsizeof(*vs, flds);
    HDfreespace(flds);
	return (stat);
}

/* ------------------------------------------------------------------ */
/*
**  determines the no of entries in a vgroup
**  related: Ventries--ventsc--VFENTS
*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nventsc(HFILEID *f,intf *vgid)
#else
nventsc(f,vgid)
    HFILEID *f;
    intf    *vgid;
#endif
{
    return( (intf) Ventries (*f,*vgid) );
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

    FRETVAL(intf)
#ifdef PROTOTYPE
nvlonec(HFILEID *f, intf **idarray, intf *asize)
#else
nvlonec(f, idarray, asize)
    HFILEID     *f;
    intf    **idarray;          /* output -- an integer array */
    intf    *asize;
#endif
{
    return( (intf) Vlone( *f, (int32 *)*idarray, (int32)*asize) );
}

/* ------------------------------------------------------------------ */
/*
**  gets the refs of all lone vdatas in the file
**  related: VSlone--vslonec--VSFLONE
*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nvslonec(HFILEID *f, intf **idarray, intf *asize)
#else
nvslonec(f, idarray, asize)
    HFILEID     *f;
    intf    **idarray;      /* output -- an integer array */
    intf    *asize;
#endif
{
    return( VSlone( *f, (int32 *)*idarray, (int32)*asize) );
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

    FRETVAL(intf)
#ifdef PROTOTYPE
nvhsdc(HFILEID *f, _fcd field, BYTE *buf, intf *n, intf *datatype, _fcd vsname,
    _fcd vsclass, intf *fieldlen, intf *vsnamelen, intf *vsclasslen)
#else
nvhsdc(f, field, buf, n, datatype, vsname, vsclass, fieldlen, vsnamelen,
        vsclasslen)
    HFILEID     *f;
	_fcd   	field, vsname, vsclass;
    intf    *n, *datatype;
	BYTE	*buf;
    intf    *fieldlen, *vsnamelen, *vsclasslen;
#endif
{
    char *fld, *name, *class;

    fld = HDf2cstring(field, *fieldlen);
    name = HDf2cstring(vsname, *vsnamelen);
    class = HDf2cstring(vsclass, *vsclasslen);

    return( (intf) VHstoredata (*f, fld, buf, *n, *datatype,
                 name, class));
}

/* ------------------------------------------------------------------ */
/*
**  store an aggregate dataset in a vdata
**  related: VHstoredatam--vhsdmc--vhfsdm
*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nvhsdmc(HFILEID *f, _fcd field, BYTE *buf, intf *n, intf *datatype,
        _fcd vsname, _fcd vsclass, intf *order, intf *fieldlen,
        intf *vsnamelen, intf *vsclasslen)
#else
nvhsdmc(f, field, buf, n, datatype, vsname, vsclass, order,
           fieldlen, vsnamelen, vsclasslen)

	HFILEID	*f;
	_fcd  	field, vsname, vsclass;
    intf    *n, *datatype, *order;
	BYTE	*buf;
    intf    *fieldlen, *vsnamelen, *vsclasslen;
#endif
{
    char *fld, *name, *class;

    fld = HDf2cstring(field, *fieldlen);
    name = HDf2cstring(vsname, *vsnamelen);
    class = HDf2cstring(vsclass, *vsclasslen);

    return( (intf) VHstoredatam (*f, fld , buf, *n,
      *datatype, name, class, *order));
}

/* ------------------------------------------------------------------ */
/*
**  make a new vgroup given several tag/ref pairs 
**  related: VHmakegroup--vhmkgpc--vhfmkgp
*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nvhmkgpc(HFILEID *f, intf *tagarray, intf *refarray, intf *n, _fcd vgname,
    _fcd vgclass, intf *vgnamelen, intf *vgclasslen)
#else
nvhmkgpc(f, tagarray, refarray , n, vgname, vgclass, vgnamelen, vgclasslen)
    HFILEID *f;
	_fcd   	vgname, vgclass;
    intf    *n, *tagarray, *refarray;
    intf    *vgnamelen, *vgclasslen;
#endif
{
    char *gname, *gclass;

    gname = HDf2cstring(vgname, *vgnamelen);
    gclass = HDf2cstring(vgclass, *vgclasslen);

    return( (intf) VHmakegroup (*f, (int32 *)tagarray, (int32 *)refarray,
            *n, gname, gclass));
}

/* ================================================================== */
/*
**  locate a field in a vdata that belongs to this VGROUP
**  related: Vflocate--vffloc--vflocc
*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nvflocc(VGROUP **vg, _fcd field, intf *fieldlen)
#else
nvflocc(vg, field, fieldlen)
    VGROUP  **vg;
	_fcd   	field;
    intf    *fieldlen;
#endif
{
    char  *fld;
    intf  stat;

    fld = HDf2cstring (field, (intn)*fieldlen);
	/* trimendblanks(fld); */
	stat = (int32) Vflocate (*vg, fld);
    HDfreespace(fld);

	return(stat);
}

/* ------------------------------------------------------------------ */
/* 
**  tests if a tag/ref pair is in a vgroup.
**  related: Vinqtagref--vinqtrc--vfinqtr
*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nvinqtrc(VGROUP **vg, intf *tag, intf *ref)
#else
nvinqtrc(vg, tag, ref)
    VGROUP  **vg;
    intf    *tag, *ref;
#endif
{
    return ( (intf) Vinqtagref ( *vg, *tag, *ref) );
}
/* ------------------------------------------------------------------ */
/* 
**  gets the number of tag/refs stored in a vgroup
**  related: Vntagrefs--vntrc--VFNTR
*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nvntrc(VGROUP **vg)
#else
nvntrc(vg)
    VGROUP  **vg;
#endif
{
    return ( (intf) Vntagrefs (*vg) );
}
/* ------------------------------------------------------------------ */

/*
**  returns all the tag/ref pairs in a vgroup 
**  related: Vgettagrefs--vgttrsc--vfgttrs
*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nvgttrsc(VGROUP **vg, intf *tagarray, intf *refarray, intf *n)
#else
nvgttrsc(vg, tagarray, refarray, n)
    VGROUP  **vg;
    intf    *tagarray, *refarray;       /* outputs - integer arrays */
    intf    *n;
#endif
{
    return ( (intf)  Vgettagrefs (*vg, (int32 *)tagarray, (int32 *)refarray, *n) );
}

/* ------------------------------------------------------------------ */
/*
**  returns a specified tag/ref pair in a vgroup 
**  related: Vgettagref--vgttrc--vfgttr
*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nvgttrc(VGROUP **vg, intf *which, intf *tag, intf *ref)
#else
nvgttrc(vg, which, tag, ref)
    VGROUP  **vg;
    intf    *which;
    intf    *tag, *ref;                     /* outputs */
#endif
{
    return ( (intf) Vgettagref (*vg, *which, (int32 *)tag, (int32 *)ref) );
}
/* ------------------------------------------------------------------ */

/*
**  add a tag/ref pair to a vgroup 
**  related: Vaddtagref--vadtrc--VFADTR 
*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nvadtrc(VGROUP **vg, intf *tag, intf *ref)
#else
nvadtrc(vg, tag, ref)
    VGROUP  **vg;
    intf   *tag, *ref;
#endif
{
    return ( (intf) Vaddtagref ( *vg, *tag, *ref) );
}

/* ------------------------------------------------------------------ */


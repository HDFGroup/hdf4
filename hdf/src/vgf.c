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

#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif

/* $Id$ */

/*
   *
   * vgf.c
   * Part of the HDF VSet interface.
   *
   * C routines (short names) to be called from fortran
   *
   *
   *********************************************************************** */

#include "vg.h"

/*
   **  remove trailing blanks from a string. input argument is a  string
   **  and *MUST* be a variable and not a constant!! For internal use only!!
   **  Used only on Crays where the Fortran compiler will pad strings to the
   **  nearest 8-byte boundary.
 */

void
trimendblanks(char *ss)
{
    int32       i, n;

    n = HDstrlen(ss);
    for (i = n - 1; i >= 0; i--)
      {
          if (ss[i] != ' ')
            {
                ss[i + 1] = '\0';
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
ndfivopn(_fcd name, intf * acc_mode, intf * defdds, intf * namelen)
{
    char       *fn;
    intf        ret;

    fn = HDf2cstring(name, (intn) *namelen);
    if (!fn) return(FAIL);
    ret = (intf) Vopen(fn, (intn) *acc_mode, (int16) *defdds);
    HDfree(fn);
    return (ret);
}   /* end ndfivopn() */

/*-----------------------------------------------------------------------------
 * Name:    dfvclos
 * Purpose: Call DFvsetclose to close HDF file
 * Inputs:  file_id: handle to HDF file to close
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 * Invokes: Hclose
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
ndfvclos(intf * file_id)
{
    return (Vclose(*file_id));
}   /* ndfvclos() */

/*
   **  attach to a vgroup and returns its ptr
   **  related: Vattach--vatchc--VFATCH
 */

FRETVAL(intf)
nvatchc(HFILEID * f, intf * vgid, _fcd accesstype)
{
    int32       vkey;
    char       *acc;

    acc = HDf2cstring(accesstype, 1);
    if (!acc) return(FAIL);

    vkey = Vattach(*f, *vgid, acc);
    HDfree(acc);

    return (vkey);
}

/* ------------------------------------------------------------------ */

/*
   **  detach from a vgroup
   **  related: Vdetach--vdtchc--VFDTCH
 */

FRETVAL(intf)
nvdtchc(intf * vkey)
{
    return (Vdetach(*vkey));
}
/* ------------------------------------------------------------------ */

/*
   **  get the name of a vgroup
   **  related: Vgetname--vgnamc--VFGNAM
 */

FRETVAL(intf)
nvgnamc(intf * vkey, _fcd vgname)
{
    return (Vgetname(*vkey, vgname));
}   /* VGNAMC */

/* ------------------------------------------------------------------ */
/*
   **  get the class name of a vgroup
   **  related: Vgetclass--vgclsc--VFGCLS
 */

FRETVAL(intf)
nvgclsc(intf * vkey, _fcd vgclass)
{
    return (Vgetclass(*vkey, vgclass));
}   /* VGCLSC */

/* ------------------------------------------------------------------ */
/*
   **  general inquiry on a vgroup
   **  related: Vinquire--vinqc--VFINQ
 */

FRETVAL(intf)
nvinqc(intf * vkey, intf * nentries, _fcd vgname)
{
    return ((intf) Vinquire(*vkey, (int32 *) nentries, vgname));
}   /* VINQC */

/* ------------------------------------------------------------------ */
/*
   **  delete a vgroup from a file
   **  related: Vdelete--vdelete--
 */

FRETVAL(intf)
nvdelete(intf * f, intf * vkey)
{
    return ((intf) Vdelete((int32)*f, (int32)*vkey));
}   /* nvdelete */

/* ------------------------------------------------------------------ */
/*
   **  gets the id of the next vgroup in the file
   **  related: Vgetid--vgidc--VFGID
 */

FRETVAL(intf)
nvgidc(HFILEID * f, intf * vgid)
{
    return ((intf) Vgetid(*f, *vgid));
}

/* ------------------------------------------------------------------ */
/*
   **  gets the id of the next entry in the vgroup
   **  related: Vgetnext--vgnxtc--VFGNXT
 */

FRETVAL(intf)
nvgnxtc(intf * vkey, intf * id)
{
    return (Vgetnext(*vkey, *id));
}

/* ------------------------------------------------------------------ */
/*
   **  sets the name of the vgroup
   **  related: Vsetname--vsnamc--VFSNAM
 */

FRETVAL(intf)
nvsnamc(intf * vkey, _fcd vgname, intf * vgnamelen)
{
    char       *name;
    intf        ret;

    name = HDf2cstring(vgname, (intn) *vgnamelen);
    if (!name) return(FAIL);
    /* trimendblanks(name); */
    ret = (intf) Vsetname(*vkey, name);
    HDfree(name);

    return (ret);
}

/* ------------------------------------------------------------------ */
/*
   **  sets the class name of the vgroup
   **  related: Vsetclass--vsclsc--VFSCLS
 */

FRETVAL(intf)
nvsclsc(intf * vkey, _fcd vgclass, intf * vgclasslen)
{
    char       *class;
    intf        ret;

    class = HDf2cstring(vgclass, (intn) *vgclasslen);
    if (!class) return(FAIL);
    /* trimendblanks(class); */
    ret = (intf) Vsetclass(*vkey, class);
    HDfree(class);

    return (ret);
}

/* ------------------------------------------------------------------ */
/*
   **  inserts a vset object (ie a vgroup or vdata ptr) into the given vgroup
   **  related: Vinsert--vinsrtc--VFINSRT
 */

FRETVAL(intf)
nvinsrtc(intf * vkey, intf * vobjptr)
{
    return ((intf) Vinsert(*vkey, *vobjptr));
}

/* ------------------------------------------------------------------ */
/*
   **  tests if a vset object (having id id) in a vgroup refers to a vgroup
   **  related: Visvg--visvgc--VFISVG
 */

FRETVAL(intf)
nvisvgc(intf * vkey, intf * id)
{
    return ((intf) Visvg(*vkey, *id));
}

/* ------------------------------------------------------------------ */
/*
   **  wrapper for Vstart
 */

FRETVAL(intf)
nvfstart(HFILEID * f)
{
    return (Vstart(*f));
}   /* nvfstart */

/* ------------------------------------------------------------------ */
/*
   **  wrapper for Vend
 */

FRETVAL(intf)
nvfend(HFILEID * f)
{
    return ((intf) Vend(*f));
}   /* nvfend */

/* ------------------------------------------------------------------ */
/*
   **  tests if an id in a vgroup refers to a vdata
   **  related: Visvs--visvsc--VFISVS
 */

FRETVAL(intf)
nvisvsc(intf * vkey, intf * id)
{
    return ((intf) Visvs(*vkey, *id));
}

/* ================================================== */
/*  VDATA routines                                    */
/* ================================================== */

/*
   **  attach to a vdata and returns its ptr
   **  related: VSattach--vsatchc--VFATCH
 */

FRETVAL(intf)
nvsatchc(HFILEID * f, intf * vsid, _fcd accesstype)
{
    intf        vkey;
    char       *acc;

    acc = HDf2cstring(accesstype, 1);   /* 'r' or 'w' only */
    if (!acc) return(FAIL);
    vkey = VSattach(*f, *vsid, acc);
    HDfree(acc);

    return (vkey);
}

/* ------------------------------------------------------------------ */
/*
   **  detach from a vdata
   **  related: VSdetach--vsdtchc--VFDTCH
 */

FRETVAL(intf)
nvsdtchc(intf * vkey)
{
    return (VSdetach(*vkey));
}

/* ------------------------------------------------------------------ */
/*
   **  get the ref # of a vdata
   **  related: VSQueryref--vsqref--
 */

FRETVAL(intf)
nvsqref(intf * vkey)
{
    return ((intf)VSQueryref((int32)*vkey));
}

/* ------------------------------------------------------------------ */
/*
   **  get the tag # of a vdata
   **  related: VSQuerytag--vsqtag--
 */

FRETVAL(intf)
nvsqtag(intf * vkey)
{
    return ((intf)VSQuerytag((int32)*vkey));
}

/* ------------------------------------------------------------------ */
/*
   **  get the version # of a vdata
   **  related: VSgetversion--vsgver--
 */

FRETVAL(intf)
nvsgver(intf * vkey)
{
    return ((intf)VSgetversion((int32)*vkey));
}

/* ------------------------------------------------------------------ */
/*
   **  seeks to a given element position in a vadata
   **  related: VSseek--vsseekc--VSFSEEK
 */

FRETVAL(intf)
nvsseekc(intf * vkey, intf * eltpos)
{
    return ((intf) VSseek(*vkey, *eltpos));
}

/* ------------------------------------------------------------------ */
/*
   **  gets the name of a vdata
   **  related: VSgetname--vsgnamc--VSFGNAM
 */

FRETVAL(intf)
nvsgnamc(intf * vkey, _fcd vsname)
{
    return (VSgetname(*vkey, vsname));
}   /* VSGNAMC */

/* ------------------------------------------------------------------ */
/*
   **  get the class name of a vdata
   **  related: VSgetclass--vsgclsc--VSFGCLS
 */

FRETVAL(intf)
nvsgclsc(intf * vkey, _fcd vsclass)
{
    return (VSgetclass(*vkey, vsclass));
}   /* VSGCLSC */

/* ------------------------------------------------------------------ */
/*
   **  general inquiry on a vdata
   **  related: VSinquire--vsinqc--VSFINQ
 */

FRETVAL(intf)
nvsinqc(intf * vkey, intf * nelt, intf * interlace, _fcd fields, intf * eltsize,
        _fcd vsname)
{
    return ((intf) VSinquire(*vkey, (int32 *) nelt, (int32 *) interlace,
                             fields, (int32 *) eltsize, vsname));
}   /* VSINQC */

/* ------------------------------------------------------------------ */
/*
   **  tests if given fields exist in the vdata
   **  related: VSfexist--vsfexc--VSFEX
 */

FRETVAL(intf)
nvsfexc(intf * vkey, _fcd fields, intf * fieldslen)
{
    intf        ret;
    char       *flds;

    flds = HDf2cstring(fields, (intn) *fieldslen);
    if (!flds) return(FAIL);
    /* trimendblanks(flds); */
    ret = (int32) VSfexist(*vkey, flds);
    HDfree(flds);

    return (ret);
}

/* ------------------------------------------------------------------ */
/*
   **  looks for a named Vdata in a file
   **  related: VSfind--vsfndc--VSFFND
 */

FRETVAL(intf)
nvsfndc(HFILEID * f, _fcd name, intf * namelen)
{
    intf        ret;
    char       *cname;

    cname = HDf2cstring(name, (intn) *namelen);
    if (!cname) return(FAIL);
    /* trimendblanks(flds); */
    ret = (intf) VSfind(*f, cname);
    HDfree(cname);

    return (ret);
}

/* ------------------------------------------------------------------ */
/*
   **  gets the id of the next vdata from the file
   **  related: VSgetid--vsgidc--VSFGID
 */

FRETVAL(intf)
nvsgidc(HFILEID * f, intf * vsid)
{
    return ((intf) VSgetid(*f, *vsid));
}

/* ------------------------------------------------------------------ */
/*
   **  removes the vdata with id from the file
   **  related: VSdelete--vsdltc--VSFDLTE
 */

FRETVAL(intf)
nvsdltc(HFILEID * f, intf * vsid)
{
    return ((intf) VSdelete(*f, *vsid));
}

/* ------------------------------------------------------------------ */
/*
   **  make it possible to append unlimitedly to an existing vdata
   **  related: VSappendable--vsapp--
 */

FRETVAL(intf)
nvsapp(intf * vkey, intf *blk)
{
    return ((intf) VSappendable((int32)*vkey,(int32)*blk));
}

/* ------------------------------------------------------------------ */
/*
   **  sets the name of a vdata
   **  related: VSsetname--vssnamc--VSFSNAM
 */

FRETVAL(intf)
nvssnamc(intf * vkey, _fcd vsname, intf * vsnamelen)
{
    char       *name;
    intf        ret;

    name = HDf2cstring(vsname, (intn) *vsnamelen);
    if (!name) return(FAIL);
    /* trimendblanks (name); */
    ret = (intf) VSsetname(*vkey, name);
    HDfree(name);

    return (ret);
}

/* ------------------------------------------------------------------ */
/*
   **  sets the class name of the vdata
   **  related: VSsetclass--vssclsc--VSFSCLS
 */

FRETVAL(intf)
nvssclsc(intf * vkey, _fcd vsclass, intf * vsclasslen)
{
    char       *class;
    intf        ret;

    class = HDf2cstring(vsclass, (intn) *vsclasslen);
    if (!class) return(FAIL);
    /* trimendblanks(class); */
    ret = (intf) VSsetclass(*vkey, class);
    HDfree(class);

    return (ret);
}

/* ------------------------------------------------------------------ */
/*
   **  sets the fields in a vdata for reading or writing
   **  related: VSsetfields--vssfldc--VSFSFLD
 */

FRETVAL(intf)
nvssfldc(intf * vkey, _fcd fields, intf * fieldslen)
{
    char       *flds;
    intf        ret;

    flds = HDf2cstring(fields, (intn) *fieldslen);
    if (!flds) return(FAIL);
    /* trimendblanks(flds); */
    ret = (int32) VSsetfields(*vkey, flds);
    HDfree(flds);

    return (ret);
}

/* ------------------------------------------------------------------ */
/*
   **  sets the file interlace of a vdata
   **  related: VSsetinterlace--vssintc--VSFSINT
 */

FRETVAL(intf)
nvssintc(intf * vkey, intf * interlace)
{
    return ((intf) VSsetinterlace(*vkey, *interlace));
}

/* ------------------------------------------------------------------ */
/*
   **  defines a new field to be used in the vdata
   **  related: VSfdefine--vsfdefc--VSFFDEF
 */

FRETVAL(intf)
nvsfdefc(intf * vkey, _fcd field, intf * localtype, intf * order, intf * fieldlen)
{
    intf        ret;
    char       *fld;

    fld = HDf2cstring(field, (intn) *fieldlen);
    if (!fld) return(FAIL);
    /* trimendblanks(fld); */
    ret = (int32) VSfdefine(*vkey, fld, *localtype, *order);
    HDfree(fld);
    return (ret);
}

/*-----------------------------------------------------------------------------
 * Name:    vssextfc
 * Purpose: store data of a Vdata in an external file
 * Inputs:  id: vdata id
 *          name: name of external file
 *          offset: Number of bytes from the beginning of the
 *                    external file to where the data starts
 *          namelen: length of name
 * Returns: 0 on success, -1 on failure with error set
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
nvssextfc(intf *id, _fcd name, intf *offset, intf *namelen)
{
    char   *fn;
    intf    ret;

    fn = HDf2cstring(name, *namelen);
    if (!fn) return(FAIL);
    ret = (intf) VSsetexternalfile(*id, fn, *offset);
    HDfree((VOIDP)fn);
    return(ret);
}


/* ------------------------------------------------------------------ */
/*
   **  returns the number of fields in a vdata
   **  related: VFnfields--vfnflds--
 */

FRETVAL(intf)
nvfnflds(intf * vkey)
{
    return((intf) VFnfields((int32)*vkey));
}

/* ------------------------------------------------------------------ */
/*
   **  get the name of a given field in a vdata
   **  related: VFfieldname--vffname--
 */

FRETVAL(intf)
nvffname(intf * vkey, intf * index, _fcd fname)
{
    char *fieldname;

    if((fieldname=VFfieldname((int32)*vkey,(int32)*index))!=NULL)
      {
        HDstrcpy(fname,fieldname);
        return(SUCCEED);
      } /* end if */
    else
        return(FAIL);
}   /* vffname */

/* ------------------------------------------------------------------ */
/*
   **  get the number-type of a given field in a vdata
   **  related: VFfieldtype--vfftype--
 */

FRETVAL(intf)
nvfftype(intf * vkey, intf * index)
{
    return((intf)VFfieldtype((int32)*vkey,(int32)*index));
}   /* vfftype */

/* ------------------------------------------------------------------ */
/*
   **  get the internal (in memory) size of a given field in a vdata
   **  related: VFfieldisize--vffisiz--
 */

FRETVAL(intf)
nvffisiz(intf * vkey, intf * index)
{
    return((intf)VFfieldisize((int32)*vkey,(int32)*index));
}   /* vffisiz */

/* ------------------------------------------------------------------ */
/*
   **  get the external (on disk) size of a given field in a vdata
   **  related: VFfieldesize--vffesiz--
 */

FRETVAL(intf)
nvffesiz(intf * vkey, intf * index)
{
    return((intf)VFfieldesize((int32)*vkey,(int32)*index));
}   /* vffesiz */

/* ------------------------------------------------------------------ */
/*
   **  get the order of a given field in a vdata
   **  related: VFfieldorder--vffordr--
 */

FRETVAL(intf)
nvffordr(intf * vkey, intf * index)
{
    return((intf)VFfieldorder((int32)*vkey,(int32)*index));
}   /* vffordr */

/* ------------------------------------------------------------------ */
/*
   **  reads from a vdata
   **  related: VSread--vsreadc--VSFREAD
 */

FRETVAL(intf)
nvsreadc(intf * vkey, uint8 *buf, intf * nelt, intf * interlace)
{
    return ((intf) VSread(*vkey, buf, *nelt, *interlace));
}

/* ------------------------------------------------------------------ */
/*
   **  writes to a vdata
   **  related: VSwrite--vswritc--VSFWRIT
 */

FRETVAL(intf)
nvswritc(intf * vkey, uint8 *buf, intf * nelt, intf * interlace)
{
    return ((intf) VSwrite(*vkey, buf, *nelt, *interlace));
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
nvsgintc(intf * vkey)
{
    return ((intf) VSgetinterlace(*vkey));
}

/* ------------------------------------------------------------------ */
/*
   **  gets the number of elements in the vdata
   **  related: VSelts--vseltsc--VSFELTS
 */

FRETVAL(intf)
nvseltsc(intf * vkey)
{
    return ((intf) VSelts(*vkey));
}

/* ------------------------------------------------------------------ */
/*
   **  gets the fields in the vdata
   **  related: VSgetfields--vsgfldc--VSFGFLD
 */

FRETVAL(intf)
nvsgfldc(intf * vkey, _fcd fields)
{
    return ((intf) VSgetfields(*vkey, fields));
}   /* VSGFLDC */

/* ------------------------------------------------------------------ */
/*
   **  determines the (machine) size of the given fields
   **  related: VSsizeof--vssizc--VSFSIZ
 */

FRETVAL(intf)
nvssizc(intf * vkey, _fcd fields, intf * fieldslen)
{
    char       *flds;
    intf        ret;

    flds = HDf2cstring(fields, (intn) *fieldslen);
    if (!flds) return(FAIL);
    /* trimendblanks(flds); */
    ret = VSsizeof(*vkey, flds);
    HDfree(flds);
    return (ret);
}

/* ------------------------------------------------------------------ */
/*
   **  determines the no of entries in a vgroup
   **  related: Ventries--ventsc--VFENTS
 */

FRETVAL(intf)
nventsc(HFILEID * f, intf * vgid)
{
    return ((intf) Ventries(*f, *vgid));
}

/* ------------------------------------------------------------------ */
/*
   **  gets the refs of all lone vgroups in the file
   **  related: Vlone--vlonec--VFLONE
 */

FRETVAL(intf)
nvlonec(HFILEID * f, intf * idarray, intf * asize)
{
    return ((intf) Vlone(*f, (int32 *)idarray, (int32) *asize));
}

/* ------------------------------------------------------------------ */
/*
   **  gets the refs of all lone vdatas in the file
   **  related: VSlone--vslonec--VSFLONE
 */

FRETVAL(intf)
nvslonec(HFILEID * f, intf * idarray, intf * asize)
{
    return (VSlone(*f, (int32 *)idarray, (int32) *asize));
}

/* ------------------------------------------------------------------ */
/*
   **  gets the ref # of a vgroup for a given name
   **  related: Vfind--vfindc--VFIND
 */

FRETVAL(intf)
nvfindc(HFILEID _HUGE * f, _fcd name, intf _HUGE * namelen)
{
    char *tmp_name;
    intf ret;

    tmp_name = HDf2cstring(name, (intn) *namelen);
    if (!tmp_name) return(FAIL);

    ret = (intf) Vfind((int32)*f, tmp_name);
    HDfree(tmp_name);

    return (ret);
} /* end nvfindc() */

/* ------------------------------------------------------------------ */
/*
   **  gets the ref # of a vgroup for a given class
   **  related: Vfindclass--vfclassc--VFNDCLS
 */

FRETVAL(intf)
nvfndclsc(HFILEID _HUGE * f, _fcd class, intf _HUGE * classlen)
{
    char *t_class;
    intf ret;

    t_class = HDf2cstring(class, (intn) *classlen);
    if (!t_class) return(FAIL);

    ret = (intf) Vfindclass((int32)*f, t_class);
    HDfree(t_class);

    return (ret);
} /* end nvfndclsc() */

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
nvhsdc(HFILEID * f, _fcd field, uint8 *buf, intf * n, intf * datatype, _fcd vsname,
       _fcd vsclass, intf * fieldlen, intf * vsnamelen, intf * vsclasslen)
{
    char       *fld, *name, *class;
    intf        ret_val;

    fld = HDf2cstring(field, (intn) *fieldlen);
    if (!fld) return(FAIL);
    name = HDf2cstring(vsname, (intn) *vsnamelen);
    if (!name){
	HDfree(fld);
	return(FAIL);
    }
    class = HDf2cstring(vsclass, (intn) *vsclasslen);
    if (!class){
	HDfree(fld);
	HDfree(name);
	return(FAIL);
    }

    ret_val = (intf) VHstoredata(*f, fld, buf, *n, *datatype, name, class);
    HDfree(fld);
    HDfree(name);
    HDfree(class);

    return (ret_val);
}

/* ------------------------------------------------------------------ */
/*
   **  store an aggregate dataset in a vdata
   **  related: VHstoredatam--vhsdmc--vhfsdm
 */

FRETVAL(intf)
nvhsdmc(HFILEID * f, _fcd field, uint8 *buf, intf * n, intf * datatype,
        _fcd vsname, _fcd vsclass, intf * order, intf * fieldlen,
        intf * vsnamelen, intf * vsclasslen)
{
    char       *fld, *name, *class;
    intf        ret_val;

    fld = HDf2cstring(field, (intn) *fieldlen);
    if (!fld) return(FAIL);
    name = HDf2cstring(vsname, (intn) *vsnamelen);
    if (!name){
	HDfree(fld);
	return(FAIL);
    }
    class = HDf2cstring(vsclass, (intn) *vsclasslen);
    if (!class){
	HDfree(fld);
	HDfree(name);
	return(FAIL);
    }

    ret_val = (intf) VHstoredatam(*f, fld, buf, *n, *datatype, name, class, *order);
    HDfree(fld);
    HDfree(name);
    HDfree(class);

    return (ret_val);
}

/* ------------------------------------------------------------------ */
/*
   **  make a new vgroup given several tag/ref pairs
   **  related: VHmakegroup--vhmkgpc--vhfmkgp
 */

FRETVAL(intf)
nvhmkgpc(HFILEID * f, intf * tagarray, intf * refarray, intf * n, _fcd vgname,
         _fcd vgclass, intf * vgnamelen, intf * vgclasslen)
{
    char       *gname, *gclass;
    intf        ret_val;

    gname = HDf2cstring(vgname, (intn) *vgnamelen);
    if (!gname) return(FAIL);
    gclass = HDf2cstring(vgclass, (intn) *vgclasslen);
    if (!gclass){
	HDfree(gname);
	return(FAIL);
    }

    ret_val = (intf) VHmakegroup(*f, (int32 *) tagarray, (int32 *) refarray,
                                 *n, gname, gclass);
    HDfree(gname);
    HDfree(gclass);

    return (ret_val);
}

/* ================================================================== */
/*
   **  locate a field in a vdata that belongs to this VGROUP
   **  related: Vflocate--vffloc--vflocc
 */

FRETVAL(intf)
nvflocc(intf * vkey, _fcd field, intf * fieldlen)
{
    char       *fld;
    intf        ret;

    fld = HDf2cstring(field, (intn) *fieldlen);
    if (!fld) return(FAIL);
    /* trimendblanks(fld); */
    ret = (int32) Vflocate(*vkey, fld);
    HDfree(fld);

    return (ret);
}

/* ------------------------------------------------------------------ */
/*
   **  tests if a tag/ref pair is in a vgroup.
   **  related: Vinqtagref--vinqtrc--vfinqtr
 */

FRETVAL(intf)
nvinqtrc(intf * vkey, intf * tag, intf * ref)
{
    return ((intf) Vinqtagref(*vkey, *tag, *ref));
}
/* ------------------------------------------------------------------ */
/*
   **  gets the number of tag/refs stored in a vgroup
   **  related: Vntagrefs--vntrc--VFNTR
 */

FRETVAL(intf)
nvntrc(intf * vkey)
{
    return ((intf) Vntagrefs(*vkey));
}

/* ------------------------------------------------------------------ */
/*
   **  Returns the number of tags of a given type in a vgroup
   **  related: Vnrefs--vnrefs--
 */

FRETVAL(intf)
nvnrefs(intf _HUGE * vkey, intf _HUGE *tag)
{
    return((intf) Vnrefs((int32)*vkey, (int32)*tag));
} /* end nvnrefs() */

/* ------------------------------------------------------------------ */
/*
   **  Return the ref of this vgroup
   **  related: VQueryref--vqref--
 */

FRETVAL(intf)
nvqref(intf _HUGE * vkey)
{
    return((intf) VQueryref((int32)*vkey));
} /* end nvqref() */

/* ------------------------------------------------------------------ */
/*
   **  Return the ref of this vgroup
   **  related: VQuerytag--vqtag--
 */

FRETVAL(intf)
nvqtag(intf _HUGE * vkey)
{
    return((intf) VQuerytag((int32)*vkey));
} /* end nvqtag() */

/* ------------------------------------------------------------------ */

/*
   **  returns all the tag/ref pairs in a vgroup
   **  related: Vgettagrefs--vgttrsc--vfgttrs
 */

FRETVAL(intf)
nvgttrsc(intf * vkey, intf * tagarray, intf * refarray, intf * n)
{
    return ((intf) Vgettagrefs(*vkey, (int32 *) tagarray, (int32 *) refarray, *n));
}

/* ------------------------------------------------------------------ */
/*
   **  returns a specified tag/ref pair in a vgroup
   **  related: Vgettagref--vgttrc--vfgttr
 */

FRETVAL(intf)
nvgttrc(intf * vkey, intf * which, intf * tag, intf * ref)
{
    return ((intf) Vgettagref(*vkey, *which, (int32 *) tag, (int32 *) ref));
}
/* ------------------------------------------------------------------ */

/*
   **  tests if a tag/ref pair is in a vgroup.
   **  related: Vinqtagref--vinqtrc--vfinqtr
 */

FRETVAL(intf)
nvadtrc(intf * vkey, intf * tag, intf * ref)
{
    return ((intf) Vaddtagref(*vkey, *tag, *ref));
}
/* ------------------------------------------------------------------ */

/*
   **  checks the number of elements in a vgroup
   **  related: VSQuerycount--vsqfnelt
 */

FRETVAL(intf)
nvsqfnelt(intf * vkey, intf * nelt)
{
    int32       ret_nelt;
    intn        stat;

    stat = VSQuerycount((int32) *vkey, &ret_nelt);

    *nelt = (intf) ret_nelt;
    return ((intf) stat);
}
/* ------------------------------------------------------------------ */

/*
   **  checks the interlace of a vgroup
   **  related: VSQueryinterlace--vsqfintr
 */

FRETVAL(intf)
nvsqfintr(intf * vkey, intf * interlace)
{
    int32       ret_inter;
    intn        stat;

    stat = VSQueryinterlace((int32) *vkey, &ret_inter);

    *interlace = (intf) ret_inter;
    return ((intf) stat);
}
/* ------------------------------------------------------------------ */

/*
   **  gets the names of the fields of a vgroup
   **  related: VSQueryfields--vsqfflds
 */

FRETVAL(intf)
nvsqfldsc(intf * vkey, _fcd fields)
{
    return ((intf) VSQueryfields((int32) *vkey, fields));
}
/* ------------------------------------------------------------------ */

/*
   **  checks the size of an element of a vgroup
   **  related: VSQueryvsize--vsqfvsiz
 */

FRETVAL(intf)
nvsqfvsiz(intf * vkey, intf * size)
{
    int32       ret_size;
    intn        stat;

    stat = VSQueryvsize((int32) *vkey, &ret_size);

    *size = (intf) ret_size;
    return ((intf) stat);
}
/* ------------------------------------------------------------------ */

/*
   **  gets the names of a vgroup
   **  related: VSQueryname--vsqfname
 */

FRETVAL(intf)
nvsqnamec(intf * vkey, _fcd name)
{
    return ((intf) VSQueryname((int32) *vkey, name));
}
/* ------------------------------------------------------------------ */

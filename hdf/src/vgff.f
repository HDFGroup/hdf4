C****************************************************************************
C* NCSA HDF                                                                 *
C* Software Development Group                                               *
C* National Center for Supercomputing Applications                          *
C* University of Illinois at Urbana-Champaign                               *
C* 605 E. Springfield, Champaign IL 61820                                   *
C*                                                                          *
C* For conditions of distribution and use, see the accompanying             *
C* hdf/COPYING file.                                                        *
C*                                                                          *
C****************************************************************************
C
C $Id$
C
c **************************************************************************
c *
c * vgFf.f
c * Part of the HDF VSet interface.
c *
c * Contains fortran routines callable from fortran programs.
c *
c **************************************************************************


C------------------------------------------------------------------------------
C Name: dfvopen
C Purpose:  call dfivopn, open file
C Inputs:   path: Name of file to be opened
C           access: DFACC_READ, DFACC_WRITE, DFACC_CREATE,
C                      or any bitwise-or of the above.
C           ndds: Number of dds in header block if file needs to be created.
C Returns: 0 on success, FAIL on failure with error set
C Users:    Fortran stub routine
C Invokes: hiopen
C----------------------------------------------------------------------------*/

c   ============================================================
c	 VGROUP ROUTINES
c	============================================================

c	attachs to a vgroup 	
c	related: Vattach--vatchc--VFATCH

      integer function vfatch(f, vgid, accesstype)
      integer     f, vgid
      character*1 accesstype
      integer     vatchc

      vfatch = vatchc (f, vgid, accesstype)
      end

c	------------------------------------------------------------
c	detaches from a vgroup
c	related: Vdetach--vdtchc--VFDTCH

      subroutine  vfdtch (vg)
      integer     vg

      call vdtchc (vg)
      end

c	------------------------------------------------------------
c	general inquiry on a vgroup
c	related: Vgetname--vgnamc--VFGNAM

      subroutine vfgnam (vg, vgname)
      integer         vg
      character*(*)   vgname

      call vgnamc (vg, vgname)
      end
c	------------------------------------------------------------
c	get the class name of a vgroup
c	related: Vgetclass--vgclsc--VFGCLS

      subroutine vfgcls (vg, vgclass)

      integer			vg
      character*(*)	vgclass

      call vgclsc  (vg, vgclass)
      end
c   ------------------------------------------------------------
c	general inquiry on a vgroup
c	related: Vinquire--vinqc--VFINQ

      integer   function    vfinq (vg, nentries, vgname)
      integer			vg, nentries
      character*(*)	vgname
      integer			vinqc

      vfinq = vinqc (vg, nentries, vgname)
      end

c   ------------------------------------------------------------
c	gets the id of the next vgroup in the file
c	related: Vgetid--vgidc--VFGID

      integer   function    vfgid (f, vgid)
      integer		f, vgid
      integer		vgidc

      vfgid = vgidc (f, vgid)
      end

c	------------------------------------------------------------
c	gets the id of the next entry in the vgroup
c	related: Vgetnext--vgnxtc--VFGNXT

      integer   function    vfgnxt (vg, id)
      integer		vg, id
      integer		vgnxtc

      vfgnxt = vgnxtc (vg, id)
      end


c	------------------------------------------------------------
c	sets the name of the vgroup
c	related: Vsetname--vsnamc--VFSNAM

      subroutine vfsnam (vg, vgname)
      integer			vg
      character*(*)	vgname

      call vsnamc (vg, vgname, len(vgname))
      end
c   ------------------------------------------------------------
c	sets the class name of the vgroup
c	related: Vsetclass--vsclsc--VFSCLS

      subroutine vfscls (vg, vgclass)
      integer			vg
      character*(*)	vgclass

      call vsclsc  (vg, vgclass, len(vgclass))
      end

c	------------------------------------------------------------
c	inserts a vset entity (ie vgroup or vdata) into the given vgroup
c	related: Vinsert--vinsrtc--VFINSRT

      integer   function vfinsrt (vg, velt)
      integer		vg, velt
      integer		vinsrtc

      vfinsrt = vinsrtc (vg, velt)
      end

c     ------------------------------------------------------------
c     tests if an id in a vgroup is a vgroup
c     related: Visvg--visvgc--vfisvg

      integer	function	vfisvg (vg, id) 									
      integer		vg, id
      integer		visvgc

      vfisvg = visvgc (vg, id)
      end

c     ------------------------------------------------------------
c     tests if an id in a vgroup is a vdata
c     related: Visvs--visvsc--vfisvs

      integer	function	vfisvs (vg, id)
      integer		vg, id
      integer		visvsc

      vfisvs = visvsc (vg, id)
      end



c     ============================================================
c      VDATA ROUTINES
c     ============================================================

c     attach to a vdata
c     related: VSattach--vsatchc--vfatch

      integer	function	vsfatch (f, vsid, accesstype)
      integer		f, vsid
      character*1	accesstype
      integer		vsatchc

      vsfatch = vsatchc (f, vsid, accesstype)
      end

c     ------------------------------------------------------------
c     detach from a vdata
c     related: VSdetach--vsdtchc--vfdtch

      subroutine	vsfdtch (vs)
      integer		vs

      call vsdtchc (vs)
      end

c     ------------------------------------------------------------
c     seeks to a given element position in a vadata
c     related: VSseek--vsseekc--vsfseek

      integer	function	vsfseek (vs, eltpos )
      integer		vs, eltpos
      integer		vsseekc

      vsfseek = vsseekc (vs, eltpos)
      end

c     ------------------------------------------------------------
c     gets the name of a vdata
c     related: VSgetname--vsgnamc--vsfgnam

      subroutine  vsfgnam (vs, vsname)	
      integer			vs
      character*(*)	vsname

      call vsgnamc (vs, vsname)
      end

c     ------------------------------------------------------------
c     get the class name of a vdata
c     related: VSgetclass--vsgclsc--vsfgcls

      subroutine vsfgcls (vs, vsclass)			 
      integer			vs
      character*(*)	vsclass

      call vsgclsc  (vs, vsclass)
      end

c     ------------------------------------------------------------
c     general inquiry on a vdata
c     related: VSinquire--vsinqc--vsfinq

      integer function vsfinq (vs,nvs,ilace,fields,vsize,vsname) 
      integer			vs, nvs, ilace, vsize
      character*(*)	fields, vsname
      integer			vsinqc

      vsfinq = vsinqc (vs,nvs,ilace,fields,vsize,vsname) 
      end

c     ------------------------------------------------------------
c     tests if given fields exist in the vdata
c     related: VSfexist--vsfexc--vsfex

      integer	function vsfex (vs, fields)		
      integer			vs
      character*(*)	fields
      integer			vsfexc

      vsfex = vsfexc (vs, fields, len(fields))
      end

c     ------------------------------------------------------------
c     looks for a Vdata with a given name
c     related: VSfind--vsfind--vsffnd

      integer	function vsffnd (f, name)
      integer			f
      character*(*)	name
      integer                   vsfndc

      vsffnd = vsfndc (f, name, len(name))
      end

c  ------------------------------------------------------------
c     gets the id of the next vdata from the file
c     related: VSgetid--vsgidc--vsfgid

      integer  function vsfgid (f, vsid) 
      integer     f, vsid
      integer     vsgidc

      vsfgid = vsgidc (f, vsid)
      end

c     ------------------------------------------------------------
c     sets the name of a vdata
c     related: VSsetname--vssnamc--vsfsnam

      subroutine vsfsnam (vs, vsname)	
      integer			vs
      character*(*)	vsname

      call vssnamc (vs, vsname, len(vsname))
      end

c     ------------------------------------------------------------
c     set the class name of a vdata
c     related: VSsetclass--vssclsc--vsfscls

      subroutine vsfscls (vs, vsclass)			 
      integer			vs
      character*(*)	vsclass

      call vssclsc  (vs, vsclass, len(vsclass))
      end

c     ------------------------------------------------------------
c     sets the fields in a vdata for reading or writing
c     related: VSsetfields--vssfldc--vsfsfld

      integer	function	vsfsfld (vs, fields)		
      integer			vs
      character*(*)	fields
      integer		vssfldc

      vsfsfld = vssfldc (vs, fields, len(fields))
      end

c     ------------------------------------------------------------
c     sets the file interlace of a vdata
c     related: VSsetinterlace--vssintc--vsfsint

      integer 	function vsfsint (vs, interface)
      integer 		vs, interlace                         
      integer		vssintc

      vsfsint = vssintc (vs, interlace)
      end

c     ------------------------------------------------------------
c     defines a new field to be used in the vdata
c     related: VSfdefine--vsfdefc--vsffdef

      integer	function	vsffdef (vs, field, localtype, order)	

      integer			vs, localtype, order
      character*(*)	field
      integer			vsfdefc

      vsffdef = vsfdefc ( vs, field, localtype, order, len(field))

      end

c     ------------------------------------------------------------
c     reads from a vdata
c     related: VSread--vsreadc--vsfread

      integer	function	vsfread (vs, buf, nelts , interlace)

      integer			vs, nelts , interlace
      character*(*)	buf
      integer			vsreadc

      vsfread = vsreadc (vs, buf, nelts, interlace)
      end

c     ------------------------------------------------------------
c     writes to a vdata
c     related: VSwrite--vswritc--vsfwrit

      integer	function	vsfwrit (vs, buf, nelts, interlace)

      integer			vs, nelts, interlace
      character*(*)		buf	
      integer			vswritc

      vsfwrit = vswritc (vs, buf, nelts, interlace)
      end

c     ===========================================
c     MISCELLANEOUS USEFUL VDATA INQUIRY ROUTINES
c     ===========================================
c
c     undocumented


c     gets the interlace of a vdata
c     related: VSgetinterlace--vsgintc--vsfgint

      integer 	function vsfgint (vs)					
      integer 		vs
      integer		vsgintc

      vsfgint = vsgintc (vs)
      end

c     ------------------------------------------------------------
c     gets the number of elements in a vdata
c     related: VSelts--vseltsc--vsfelts

      integer 	function vsfelts (vs)			
      integer  vs	
      integer	vseltsc

      vsfelts = vseltsc (vs)
      end

c     ------------------------------------------------------------
c     gets the fields in the vdata
c     related: VSgetfields--vsgfldc--vsfgfld

      integer	function vsfgfld (vs, fields)	
      integer			vs
      character*(*) 	fields
      integer			vsgfldc

      vsfgfld = vsgfldc (vs, fields)
      end

c     ------------------------------------------------------------
c 	determines the (machine) size of the given fields
c     related: VSsizeof--vssizc--vsfsiz

      integer	function vsfsiz (vs, fields)		
      integer			vs
      character*(*) 	fields
      integer			vssizc

      vsfsiz = vssizc (vs, fields, len(fields))
      end

c     ------------------------------------------------------------
c     determines the no of entries in a vgroup
c     related: Ventries--ventsc--vfents

      integer 	function	vfents (f, vgid)		
      integer	f, vgid
      integer	ventsc 

      vfents = ventsc (f, vgid)
      end

c     ------------------------------------------------------------
c     gets the refs of all lone vgroups in the file
c     related: Vlone--vlonec--vflone

      integer function vflone (f, idarray, asize)
      integer			f
      integer			idarray(*)
      integer			asize
      integer			vlonec

      vflone = vlonec  (f, idarray, asize)
      end

c     ------------------------------------------------------------
c     gets the refs of all lone vdatas in the file
c     related: VSlone--vslonec--vsflone

      integer function vsflone (f, idarray, asize)
      integer			f
      integer			idarray(*)
      integer			asize
      integer			vslonec

      vsflone = vslonec  (f, idarray, asize)
      end

c     ------------------------------------------------------------
c     store a simple dataset in a vdata
c     related: VHstoredata--vhsdc--vhfsd

      integer function vhfsd(f,field,buf,n,dtype,vsname,vsclass)
      integer		f
      character*(*)  field
      integer		buf(*)	
      integer		n, dtype
      character*(*)  vsname, vsclass
      integer vhsdc 	

      vhfsd = vhsdc (f, field, buf, n, dtype, vsname, vsclass,
     1            len(field), len(vsname), len(vsclass))
      end

c     ------------------------------------------------------------
c     store an aggregate dataset in a vadata
c     related: VHstoredatam--vhsdmc--vhfsdm

      integer function vhfsdm (f,field,buf,n,dtype,vsname,vsclass,order)
      integer			f
      character*(*)  field
      integer			buf(*)	
      integer			n, dtype, order
      character*(*)  vsname, vsclass
      integer vhsdmc 		

      vhfsdm = vhsdmc (f, field, buf, n, dtype, vsname,vsclass, order,
     1             len(field), len(vsname), len(vsclass))
      end

c     ------------------------------------------------------------
c     make a new vgroup given several tag/ref pairs
c     related: VHmakegroup--vhmkgpc--vhfmkgp

      integer function vhfmkgp(f,tagarray,refarray,n,vgname,vgclass)
      integer		f, n
      integer		tagarray(*), refarray(*)
      character*(*)  vgname, vgclass
      integer 		vhmkgpc 	

      vhfmkgp = vhmkgpc (f, tagarray, refarray , n, vgname, vgclass,
     1                len(vgname), len(vgclass))
      end

c     ============================================================
c     More vgroup routines

c     locate a field in a vdata that belongs to this VGROUP	
c     related: Vflocate--vffloc--vflocc

      integer function vffloc  (vg, field)
      integer			vg
      character*(*)	field
      integer 			vflocc								
      vffloc = vflocc (vg, field, len(field))
      end

c     ------------------------------------------------------------
c     tests if a tag/ref pair is in a vgroup.
c     related: Vinqtagref--vinqtrc--vfinqtr

      integer function vfinqtr  (vg, tag, ref)
      integer		vg, tag, ref
      integer		vinqtrc									 
      vfinqtr = vinqtrc (vg, tag, ref)
      end

c     ------------------------------------------------------------
c     gets the number of tag/refs stored in a vgroup
c     related: Velts--veltsc--vfelts

      integer function vfntr (vg)
      integer		vg
      integer vntrc 
      vfntr = vntrc (vg)
      end

c     ------------------------------------------------------------
c     returns all the tag/ref pairs in a vgroup
c     related: Vgettagrefs--vgttrsc--vfgttrs

      integer function vfgttrs (vg, tagarray, refarray, n)
      integer		vg, n
      integer		tagarray(*), refarray(*)
      integer		vgttrsc									

      vfgttrs = vgttrsc (vg, tagarray, refarray, n)
      end
c     ------------------------------------------------------------
c     returns a specified tag/ref pair in a vgroup
c     related: Vgettagref--vgttrc--vfgttr

      integer function vfgttr (vg, which, tag, ref)
      integer		vg, which
      integer		tag, ref
      integer		vgttrc									

      vfgttr = vgttrc (vg, which, tag, ref)
      end

c     ------------------------------------------------------------
c     add a tag/ref pair to a vgroup
c     related: Vaddtagref--vadtrc--vfadtr

      integer function vfadtr	( vg, tag, ref)
      integer		vg, tag, ref
      integer vadtrc				

      vfadtr = vadtrc  ( vg, tag, ref)
      end
c     ============================================================



C
C $Header$
C
C $Log$
C Revision 1.1  1992/08/25 21:40:44  koziol
C Initial revision
C
C
c	**************************************************************************
c	* Likkai Ng May 1991 NCSA
c	*
c	* vgFf.f
c	* Part of the HDF VSet interface.
c	*
c	* Contains fortran routines callable from fortran programs.
c	*
c	**************************************************************************


c	============================================================
c	 VGROUP ROUTINES
c	============================================================

c	attachs to a vgroup 	
c	related: Vattach--vatchc--VFATCH

	integer	function VFATCH(f, vgid, accesstype)
	integer		f, vgid
	character*1	accesstype
	integer		VATCHC

	VFATCH = VATCHC (f, vgid, accesstype)
	end

c	------------------------------------------------------------
c	detaches from a vgroup
c	related: Vdetach--vdtchc--VFDTCH

	subroutine	VFDTCH (vg)	
	integer		vg

	call VDTCHC (vg)
	end

c	------------------------------------------------------------
c	general inquiry on a vgroup
c	related: Vgetname--vgnamc--VFGNAM

	subroutine VFGNAM (vg, vgname)			 
	integer			vg
	character*(*)	vgname

	call VGNAMC (vg, vgname)
	end
c	------------------------------------------------------------
c	get the class name of a vgroup
c	related: Vgetclass--vgclsc--VFGCLS

	subroutine VFGCLS (vg, vgclass)			 

	integer			vg
	character*(*)	vgclass

	call VGCLSC  (vg, vgclass)
	end
c	------------------------------------------------------------
c	general inquiry on a vgroup
c	related: Vinquire--vinqc--VFINQ

	integer	function	VFINQ (vg, nentries, vgname) 
	integer			vg, nentries
	character*(*)	vgname
	integer			VINQC

	VFINQ = VINQC (vg, nentries, vgname)
	end

c	------------------------------------------------------------
c	gets the id of the next vgroup in the file
c	related: Vgetid--vgidc--VFGID

	integer	function	VFGID (f, vgid)			
	integer		f, vgid
	integer		VGIDC

	VFGID = VGIDC (f, vgid)
	end

c	------------------------------------------------------------
c	gets the id of the next entry in the vgroup
c	related: Vgetnext--vgnxtc--VFGNXT

	integer	function	VFGNXT (vg, id)				
	integer		vg, id
	integer		VGNXTC

	VFGNXT = VGNXTC (vg, id)
	end


c	------------------------------------------------------------
c	sets the name of the vgroup
c	related: Vsetname--vsnamc--VFSNAM

	subroutine VFSNAM (vg, vgname)
	integer			vg
	character*(*)	vgname

	call VSNAMC (vg, vgname, len(vgname))
	end
c	------------------------------------------------------------
c	sets the class name of the vgroup
c	related: Vsetclass--vsclsc--VFSCLS

	subroutine VFSCLS (vg, vgclass)	
	integer			vg
	character*(*)	vgclass

	call VSCLSC  (vg, vgclass, len(vgclass))
	end

c	------------------------------------------------------------
c	inserts a vset entity (ie vgroup or vdata) into the given vgroup
c	related: Vinsert--vinsrtc--VFINSRT


	integer	function VFINSRT (vg, velt)	
	integer		vg, velt
	integer		VINSRTC

	VFINSRT = VINSRTC (vg, velt)
	end

c	------------------------------------------------------------
c	tests if an id in a vgroup is a vgroup
c	related: Visvg--visvgc--VFISVG

	integer	function	VFISVG (vg, id) 									
	integer		vg, id
	integer		VISVGC

	VFISVG = VISVGC (vg, id)
	end

c	------------------------------------------------------------
c	tests if an id in a vgroup is a vdata
c	related: Visvs--visvsc--VFISVS

	integer	function	VFISVS (vg, id)
	integer		vg, id
	integer		VISVSC

	VFISVS = VISVSC (vg, id)
	end



c	============================================================
c	 VDATA ROUTINES
c	============================================================

c	attach to a vdata
c	related: VSattach--vsatchc--VFATCH

	integer	function	VSFATCH (f, vsid, accesstype)
	integer		f, vsid
	character*1	accesstype
	integer		VSATCHC

	VSFATCH = VSATCHC (f, vsid, accesstype)
	end

c	------------------------------------------------------------
c	detach from a vdata
c	related: VSdetach--vsdtchc--VFDTCH

	subroutine	VSFDTCH (vs)
	integer		vs

	call VSDTCHC (vs)
	end

c	------------------------------------------------------------
c	seeks to a given element position in a vadata
c	related: VSseek--vsseekc--VSFSEEK

	integer	function	VSFSEEK (vs, eltpos )
	integer		vs, eltpos
	integer		VSSEEKC

	VSFSEEK = VSSEEKC (vs, eltpos)
	end

c	------------------------------------------------------------
c	gets the name of a vdata
c	related: VSgetname--vsgnamc--VSFGNAM

	subroutine  VSFGNAM (vs, vsname)	
	integer			vs
	character*(*)	vsname

	call VSGNAMC (vs, vsname)
	end

c	------------------------------------------------------------
c	get the class name of a vdata
c	related: VSgetclass--vsgclsc--VSFGCLS

	subroutine VSFGCLS (vs, vsclass)			 
	integer			vs
	character*(*)	vsclass

	call VSGCLSC  (vs, vsclass)
	end

c	------------------------------------------------------------
c	general inquiry on a vdata
c	related: VSinquire--vsinqc--VSFINQ

	integer function VSFINQ (vs,nvs,ilace,fields,vsize,vsname) 
	integer			vs, nvs, ilace, vsize
	character*(*)	fields, vsname
	integer			VSINQC

	VSFINQ = VSINQC (vs,nvs,ilace,fields,vsize,vsname) 
	end

c	------------------------------------------------------------
c	tests if given fields exist in the vdata
c	related: VSfexist--vsfexc--VSFEX

	integer	function VSFEX (vs, fields)		
	integer			vs
	character*(*)	fields
	integer			VSFEXC

	VSFEX = VSFEXC (vs, fields, len(fields))
	end

c  ------------------------------------------------------------
c	gets the id of the next vdata from the file
c	related: VSgetid--vsgidc--VSFGID

	integer  function VSFGID (f, vsid) 
	integer     f, vsid
	integer     VSGIDC

	VSFGID = VSGIDC (f, vsid)
	end

c	------------------------------------------------------------
c	sets the name of a vdata
c	related: VSsetname--vssnamc--VSFSNAM

	subroutine VSFSNAM (vs, vsname)	
	integer			vs
	character*(*)	vsname

	call VSSNAMC (vs, vsname, len(vsname))
	end

c	------------------------------------------------------------
c	set the class name of a vdata
c	related: VSsetclass--vssclsc--VSFSCLS

	subroutine VSFSCLS (vs, vsclass)			 
	integer			vs
	character*(*)	vsclass

	call VSSCLSC  (vs, vsclass, len(vsclass))
	end

c	------------------------------------------------------------
c	sets the fields in a vdata for reading or writing
c	related: VSsetfields--vssfldc--VSFSFLD

	integer	function	VSFSFLD (vs, fields)		
	integer			vs
	character*(*)	fields
	integer		VSSFLDC

	VSFSFLD = VSSFLDC (vs, fields, len(fields))
	end

c	------------------------------------------------------------
c	sets the file interlace of a vdata
c	related: VSsetinterlace--vssintc--VSFSINT

	integer 	function VSFSINT (vs, interface)
	integer 		vs, interlace                         
	integer		VSSINTC

	VSFSINT = VSSINTC (vs, interlace)
	end

c	------------------------------------------------------------
c	defines a new field to be used in the vdata
c	related: VSfdefine--vsfdefc--VSFFDEF

	integer	function	VSFFDEF (vs, field, localtype, order)	

	integer			vs, localtype, order
	character*(*)	field
	integer			VSFDEFC

	VSFFDEF = VSFDEFC ( vs, field, localtype, order, len(field))

	end

c	------------------------------------------------------------
c	reads from a vdata
c	related: VSread--vsreadc--VSFREAD

	integer	function	VSFREAD (vs, buf, nelts , interlace)

	integer			vs, nelts , interlace
	character*(*)	buf
	integer			VSREADC

	VSFREAD = VSREADC (vs, buf, nelts, interlace)
	end

c	------------------------------------------------------------
c	writes to a vdata
c	related: VSwrite--vswritc--VSFWRIT

	integer	function	VSFWRIT (vs, buf, nelts, interlace)

	integer			vs, nelts, interlace
	integer			buf(*)	
	integer			VSWRITC

	VSFWRIT = VSWRITC (vs, buf, nelts, interlace)
	end

c	===========================================
c	MISCELLANEOUS USEFUL VDATA INQUIRY ROUTINES
c	===========================================
c
c	undocumented


c	gets the interlace of a vdata
c	related: VSgetinterlace--vsgintc--VSFGINT

	integer 	function VSFGINT (vs)					
	integer 		vs
	integer		VSGINTC

	VSFGINT = VSGINTC (vs)
	end

c	------------------------------------------------------------
c	gets the number of elements in a vdata
c	related: VSelts--vseltsc--VSFELTS

	integer 	function VSFELTS (vs)			
	integer  vs	
	integer	VSELTSC

	VSFELTS = VSELTSC (vs)
	end

c	------------------------------------------------------------
c	gets the fields in the vdata
c	related: VSgetfields--vsgfldc--VSFGFLD

	integer	function VSFGFLD (vs, fields)	
	integer			vs
	character*(*) 	fields
	integer			VSGFLDC

	VSFGFLD = VSGFLDC (vs, fields)
	end

c	------------------------------------------------------------
c 	determines the (machine) size of the given fields
c	related: VSsizeof--vssizc--VSFSIZ

	integer	function VSFSIZ (vs, fields)		
	integer			vs
	character*(*) 	fields
	integer			VSSIZC

	VSFSIZ = VSSIZC (vs, fields, len(fields))
	end

c	------------------------------------------------------------
c	determines the no of entries in a vgroup
c	related: Ventries--ventsc--VFENTS

	integer 	function	VFENTS (f, vgid)		
	integer	f, vgid
	integer	VENTSC 

	VFENTS = VENTSC (f, vgid)
	end

c	------------------------------------------------------------
c	DEBUGGING ROUTINES
c	------------------------------------------------------------

c	enable debug
c	related: setjj--setjjc--SETFJJ

	integer 	function SETFJJ()			 
	integer  setjjc 

	SETFJJ = setjjc() 
	end

c	------------------------------------------------------------
c	disable debug
c	related: setnojj--setnojjc--SETFNJJ

	integer 	function SETFNJJ()			 
	integer  setnojjc 

	SETFNJJ = setnojjc() 
	end
c	------------------------------------------------------------
c	gets the refs of all lone vgroups in the file
c	related: Vlone--vlonec--VFLONE

	integer function VFLONE (f, idarray, asize)
	integer			f
	integer			idarray(*)
	integer			asize
	integer			VLONEC

	VFLONE = VLONEC  (f, idarray, asize)
	end

c	------------------------------------------------------------
c	gets the refs of all lone vdatas in the file
c	related: VSlone--vslonec--VSFLONE

	integer function VSFLONE (f, idarray, asize)
	integer			f
	integer			idarray(*)
	integer			asize
	integer			VSLONEC

	VSFLONE = VSLONEC  (f, idarray, asize)
	end

c	------------------------------------------------------------
c	store a simple dataset in a vdata
c	related: VHstoredata--vhsdc--vhfsd

	integer function VHFSD(f,field,buf,n,dtype,vsname,vsclass)
	integer		f
	character*(*)  field
	integer		buf(*)	
	integer		n, dtype
	character*(*)  vsname, vsclass
	integer VHSDC 	

	VHFSD = VHSDC (f, field, buf, n, dtype, vsname, vsclass,
     1            len(field), len(vsname), len(vsclass))
	end

c	------------------------------------------------------------
c	store an aggregate dataset in a vadata
c	related: VHstoredatam--vhsdmc--vhfsdm

	integer function VHFSDM (f,field,buf,n,dtype,vsname,vsclass,order)
	integer			f
	character*(*)  field
	integer			buf(*)	
	integer			n, dtype, order
	character*(*)  vsname, vsclass
	integer VHSDMC 		

	VHFSDM = VHSDMC (f, field, buf, n, dtype, vsname,vsclass, order,
     1             len(field), len(vsname), len(vsclass))
	end

c	------------------------------------------------------------
c	make a new vgroup given several tag/ref pairs
c	related: VHmakegroup--vhmkgpc--vhfmkgp

	integer function VHFMKGP(f,tagarray,refarray,n,vgname,vgclass)
	integer		f, n
	integer		tagarray(*), refarray(*)
	character*(*)  vgname, vgclass
	integer 		VHMKGPC 	

	VHFMKGP = VHMKGPC (f, tagarray, refarray , n, vgname, vgclass,
     1                len(vgname), len(vgclass))
	end

c	============================================================
c	More vgroup routines

c	locate a field in a vdata that belongs to this VGROUP	
c	related: Vflocate--vffloc--vflocc

	integer function VFFLOC  (vg, field)
	integer			vg
	character*(*)	field
	integer 			VFLOCC								
	VFFLOC = VFLOCC (vg, field, len(field))
	end

c	------------------------------------------------------------
c	tests if a tag/ref pair is in a vgroup.
c	related: Vinqtagref--vinqtrc--vfinqtr

	integer function VFINQTR  (vg, tag, ref)
	integer		vg, tag, ref
	integer		VINQTRC									 
	VFINQTR = VINQTRC (vg, tag, ref)
	end

c	------------------------------------------------------------
c	gets the number of tag/refs stored in a vgroup
c	related: Velts--veltsc--vfelts

	integer function VFNTR (vg)
	integer		vg
	integer VNTRC 
	VFNTR = VNTRC (vg)
	end

c	------------------------------------------------------------
c	returns all the tag/ref pairs in a vgroup
c	related: Vgettagrefs--vgttrsc--vfgttrs

	integer function VFGTTRS (vg, tagarray, refarray, n)
	integer		vg, n
	integer		tagarray(*), refarray(*)
	integer		VGTTRSC									

	VFGTTRS = VGTTRSC (vg, tagarray, refarray, n)
	end
c	------------------------------------------------------------
c	returns a specified tag/ref pair in a vgroup
c	related: Vgettagref--vgttrc--vfgttr

	integer function VFGTTR (vg, which, tag, ref)
	integer		vg, which
	integer		tag, ref
	integer		VGTTRC									

	VFGTTR = VGTTRC (vg, which, tag, ref)
	end

c	------------------------------------------------------------
c	add a tag/ref pair to a vgroup
c	related: Vaddtagref--vadtrc--VFADTR

	integer function VFADTR	( vg, tag, ref)
	integer		vg, tag, ref
	integer VADTRC				

	VFADTR = VADTRC  ( vg, tag, ref)
	end
c	============================================================


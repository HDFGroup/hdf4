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
C	dffunc.inc
C
C	Declarations of return values for HDF SDS functions
C
	integer DFSDadddata,		dsadata
	integer DFSDclear,		dsclear
	integer DFSDsetdimscale,	dssdisc
	integer DFSDendslice,		dseslc
	integer DFSDgetNT,		dsgnt
	integer DFSDgetdata,		dsgdata
	integer DFSDgetdatalen,		dsgdaln
	integer DFSDgetdatastrs,	dsgdast
	integer DFSDgetdimlen,		dsgdiln
	integer DFSDgetdims,		dsgdims
	integer DFSDgetdimscale,	dsgdisc
	integer DFSDgetdimstrs,		dsgdist
	integer DFSDgetmaxmin,		dsgmaxm
	integer DFSDgetrange,		dsgrang
	integer DFSDgetslice,		dsgslc
	integer DFSDlastref,		dslref
	integer DFSDnumber,		dsnum
	integer DFSDputdata,		dspdata
	integer DFSDputslice,		dspslc
	integer DFSDreadref,		dsrref
	integer DFSDrestart,		dsfirst
	integer DFSDsetNT,		dssnt
	integer DFSDsetdatastrs,	dssdast
	integer DFSDsetdims,		dssdims
	integer DFSDsetdimstrs,		dssdist
	integer DFSDsetlengths,		dsslens
	integer DFSDsetmaxmin,		dssmaxm
	integer DFSDsetorder,		dssodr
	integer DFSDsetrange,		dssrang
	integer DFSDstartslice,		dssslc
        integer dsgcal
        integer dsp32sd
        integer dsscal
        integer dseslab
        integer dsrslab
        integer dssslab
        integer dswslab
        integer dsgfill
        integer dssfill
        integer dswref
C
C	Declarations of return values for HDF Annotation functions
C
	integer DFANputlabel,		daplab
	integer DFANputdesc,		dapdesc
	integer DFANgetlablen,		dagllen
	integer DFANgetlabel,		daglab
	integer DFANgetdesclen,		dagdlen
	integer DFANgetdesc,		dagdesc
	integer DFANlablist,		dallist
	integer DFANaddfid,		daafid
	integer DFANaddfds,		daafds
	integer DFANgetfidlen,		dagfidl
	integer DFANgetfid,		dagfid
	integer DFANgetfdslen,		dagfdsl
	integer DFANgetfds,		dagfds
	integer DFANlastref,		dalref
        integer daclear
C
C	Declarations of return values for HDF Raster Image functions
C
	integer DFR8setpalette,		d8spal
	integer DFR8putimage,		d8pimg
	integer DFR8addimage,		d8aimg
	integer DFR8getdims,		d8gdims
	integer DFR8getimage,		d8gimg
	integer DFR8readref,		d8rref
	integer DFR8writeref,		d8wref
	integer DFR8restart,		d8first
	integer DFR8nimages,            d8nims
	integer DFR8lastref,            d8lref
	integer DFR8scompress,          d8scomp
	integer DFR8sjpeg,              d8sjpeg

	integer DF24setil,		d2setil
	integer DF24addimage,		d2aimg
	integer DF24putimage,		d2pimg
	integer DF24getimage,		d2gimg
	integer DF24getdims,		d2gdims
	integer DF24setdims,		d2sdims
	integer DF24readref,		d2rref
	integer DF24restart,		d2first
	integer DF24reqil,		d2reqil
        integer 			d2lref
	integer DF24scompress,          d2scomp
	integer DF24sjpeg,              d2sjpeg
	integer DF24nimages,		d2nimg

	integer DFPaddpal,		dpapal
	integer DFPgetpal,		dpgpal
	integer DFPputpal,		dpppal
	integer DFPnpals,		dpnpals
	integer DFPwriteref,		dpwref
	integer DFPreadref,		dprref
	integer DFPrestart,		dprest
	integer DFPlastref,		dplref

C
C	Declarations of return values for HDF Raster Image functions
C
	integer DFopen
	integer DFclose
	integer DFindnextref,		dfindnr
	integer DFsfind
	integer DFfind
	integer DFget
	integer DFput
	integer DFaccess
	integer DFread
	integer DFwrite
	integer DFseek
	integer DFupdate
	integer DFdup
	integer DFdel
	integer DFerrno
	integer DFishdf
	integer DFnewref
	integer DFnumber
	integer DFstat

C
C       Decls of SFxxx functions for Fortran multi-file interface
C
      integer sfstart
      integer sfn2index
      integer sfcreate
      integer sfsdmstr
      integer sfsdmname
      integer sfsdtstr
      integer sfgdtstr
      integer sfgdmstr
      integer sfginfo
      integer sfgainfo
      integer sfgdinfo
      integer sfsattr
      integer sffattr
      integer sfend
      integer sfendacc
      integer sffinfo
      integer sfselect
      integer sfdimid
      integer sfgcal
      integer sfscal
      integer sfsdscale
      integer sfgdscale
      integer sfsfill
      integer sfgfill
      integer sfgrange
      integer sfsrange
      integer sfrattr
      integer sfrdata
      integer sfwdata
      integer sfsextf
      integer sfsnbit
      integer sfsacct
      integer sfid2ref
      integer sfiscvar
      integer sfref2index


C
C	Declarations of return values for HDF Vgroup functions
C
      integer vfadtr 
      integer vfatch
      integer vfdtch
      integer vfend
      integer vfents
      integer vffloc
      integer vfgcls
      integer vfgid
      integer vfgnam
      integer vfgnxt
      integer vfgttr
      integer vfgttrs
      integer vfinq
      integer vfinqtr
      integer vfinsrt
      integer vfisvg
      integer vfisvs
      integer vflone
      integer vfntr
      integer vfscls
      integer vfsnam
      integer vfstart

C
C	Declarations of return values for HDF high level Vdata/Vgroup
C       functions
C
      integer vhfmkgp
      integer vhfsd
      integer vhfsdm

C
C	Declarations of return values for HDF Vdata functions
C
      integer vsfatch
      integer vsfdlte
      integer vsfdtch
      integer vsfelts
      integer vsfex
      integer vsffdef
      integer vsffnd
      integer vsfgcls
      integer vsfgfld
      integer vsfgid
      integer vsfgint
      integer vsfgnam
      integer vsfinq
      integer vsflone
      integer vsfread
      integer vsfscls
      integer vsfseek
      integer vsfsfld
      integer vsfsint
      integer vsfsiz
      integer vsfsnam
      integer vsfwrit

C
C	Declarations of return values for HDF Vdata Query functions
C
      integer vsqfnelt
      integer vsqfintr
      integer vsqfflds
      integer vsqfvsiz
      integer vsqfname

C
C	Declarations of return values for HDF low level H functions
C
      integer hclose
      integer heprnt
      integer hnumber
      integer hopen
      integer hxscdir
      integer hxsdir

C
C       Decls of MGxxx functions for Fortran multi-file GR interface
C

      integer mgstart
      integer mgfinfo
      integer mgend       
      integer mgcreat    
      integer mgselct     
      integer mgn2ndx    
      integer mggiinf     
      integer mgwrimg     
      integer mgrdimg     
      integer mgendac     
      integer mgid2rf     
      integer mgr2idx     
      integer mgrltil     
      integer mgrimil     
      integer mggltid     
      integer mgglinf     
      integer mgwrlut     
      integer mgrdlut     
      integer mgsxfil    
      integer mgssctp     
      integer mgsattr    
      integer mgatinf     
      integer mggattr     
      integer mgfndat    

C End of declarations


C
C $Id$
C
C	dffunctions.f
C
C	Declarations of return values for HDF SDS functions
C
	integer DFSDsetlengths,		dsslens
	integer DFSDsetdims,		dssdims
	integer DFSDsetdimstrs,		dssdist
	integer DFSDdsetdimscale,	dssdisc
	integer DFSDsetdatastrs,	dssdast
	integer DFSDsetmaxmin,		dssmaxm
	integer DFSDsetrange,		dssrang
	integer DFSDputdata,		dspdata
	integer DFSDadddata,		dsadata
	integer DFSDclear,		dsclear
	integer DFSDstartslice,		dssslc
	integer DFSDputslice,		dspslc
	integer DFSDendslice,		dseslc
	integer DFSDgetdims,		dsgdims
	integer DFSDgetdata,		dsgdata
	integer DFSDgetdimstrs,		dsgdist
	integer DFSDgetdimscale,	dsgdisc
	integer DFSDgetdatastrs,	dsgdast
	integer DFSDgetmaxmin,		dsgmaxm
	integer DFSDgetrange,		dsgrang
	integer DFSDgetdimlen,		dsgdiln
	integer DFSDgetdatalen,		dsgdaln
	integer DFSDreadref,		dsrref
	integer DFSDrestart,		dsfirst
	integer DFSDgetslice,		dsgslc
	integer DFSDsetNT,		dssnt
	integer DFSDgetNT,		dsgnt
	integer DFSDsetorder,		dssodr
	integer DFSDnumber,		dsnum
	integer DFSDlastref,		dslref
        integer dsp32sd,
        integer dsscal
        integer dsgcal
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
	integer DF24getimage,		d2gimg
	integer DF24getdims,		d2gdims
	integer DF24setdims,		d2sdims
	integer DF24readref,		d2rref
	integer DF24restart,		d2first
	integer DF24reqil,		d2reqil
        integer d24lref
	integer DF24scompress,          d2scomp
	integer DF24sjpeg,              d2sjpeg

	integer DFPaddpal,		dpapal
	integer DFPgetpal,		dpgpal
	integer DFPputpal,		dpppal
	integer DFPnpals,		dpnpals
	integer DFPwriteref,		dpwref
	integer DFPreadref,		dprref
	integer DFPrestart,		dprset
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

C End of declarations





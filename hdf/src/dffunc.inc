C
C $Header$
C
C $Log$
C Revision 1.1  1993/04/28 20:47:07  koziol
C Renamed constant.f to constant.i and added dffunc.i
C
c Revision 1.1  1993/03/02  02:20:16  sxu
c Initial revision
c
c Revision 1.2  1993/01/03  00:13:13  sxu
c Add file header
c
c Revision 1.1  1992/12/03  09:34:59  koziol
c Initial revision
c
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
	integer DFSDputdata,		dspdata
	integer DFSDadddata,		dsadata
	integer DFSDclear,		dsclear
	integer DFSDstartslice,		dssslc
	integer DFSDputslice,		dspslc
	integer DFSDendslice,		dsseslc
	integer DFSDgetdims,		dsgdims
	integer DFSDgetdata,		dsgdata
	integer DFSDgetdimstrs,		dsgdist
	integer DFSDgetdimscale,	dsgdisc
	integer DFSDgetdatastrs,	dsgdast
	integer DFSDgetmaxmin,		dsgmaxm
	integer DFSDreadref,		dsrref
	integer DFSDrestart,		dsfirst
	integer DFSDgetslice,		dsgslc
	integer DFSDsetNT,		dssnt
	integer DFSDgetNT,		dsgnt
	integer DFSDsetorder,		dssodr
	integer DFSDnumber
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
	integer DFANlastref
C
C	Declarations of return values for HDF Raster Image functions
C
	integer DFR8setpalette,		d8spal
	integer DFR8putimage,		d8pimg
	integer DFR8addimage,		d8aimg
	integer DFR8getdims,		d8gdims
	integer DFR8getimage,		d8gimg
	integer DFR8readref,		d8rref
	integer DFR8restart,		d8first
	integer DFR8nimages
	integer DFR8lastref

	integer DFR24setil,		d2setil
	integer DFR24addimage,		d2iaimg
	integer DFR24getdims,		d2igimg
	integer DFR24readref,		d2rref
	integer DFR24restart,		d24first
	integer DFR24requil,		d2reqil
        integer d24lref

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

C End of function declarations





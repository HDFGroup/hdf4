C***********************************************************
C * NCSA HDF
C   *
C * Software Development Group
C   *
C * National Center for Supercomputing Applications
C   *
C * University of Illinois at Urbana-Champaign
C   *
C * 605 E. Springfield, Champaign IL 61820
C   *
C *
C   *
C * For conditions of distribution and use, see the accompanying
C   *
C * hdf/COPYING file.
C   *
C *
C   *
C***************************************************
C
C $Id$ 
C
C **************************************************
C *
C * vattrff.f
C * Vset attribute Fortran routines
C *
C *************************************************

C --------------- vsffidx ------------------------
C    vsffidx -- find index of a named field in a vdata
C    VSfindex -- vsfcfdx -- vsffidx
C
       integer function vsffidx(vsid, fldnm, fldidx) 
       integer vsid
       character*(*) fldnm
       integer fldidx
       integer vsfcfdx

       vsffidx = vsfcfdx(vsid, fldnm, fldidx, 
     +                  len(fldnm))
       end
C --------------- vsfsat -------------------------
C    vsfsat -- set a numeric attribute for a vdata 
C              or a field of a vdata
C    VSsetattr -- vsfcsat -- vsfsat
C
       integer function vsfsat(vsid, findex,attrnm,dtype,
     +                        count, values)
       integer vsid, findex, dtype, count
       character*(*) attrnm
       integer  values
       integer vsfcsat
     
       vsfsat = vsfcsat(vsid, findex, attrnm, dtype,
     +                count, values, len(attrnm))
       end
C --------------- vsfscat -------------------------
C    vsfscat -- set a char type attribute for a vdata 
C              or a field of a vdata
C    VSsetattr -- vsfcsca -- vsfscat
C
       integer function vsfscat(vsid, findex,attrnm,dtype,
     +                      count, values)
       integer vsid, findex, dtype, count
       character*(*) attrnm
       character*(*) values
       integer vsfcsca
     
       vsfscat = vsfcsca(vsid, findex, attrnm, dtype,
     +         count, values, len(attrnm), len(values))
       end
C --------------- vsfgcat -------------------------
C    vsfgcat -- get a char type attribute for a vdata 
C              or a field of a vdata
C    VSgetattr -- vsfcgca -- vsfgcat
C
       integer function vsfgcat(vsid, findex,aindex,
     +                     values)
       integer vsid, findex, aindex
       character*(*) values
       integer vsfcgca
     
       vsfgcat = vsfcgca(vsid, findex, aindex, 
     +         values,  len(values))
       end
C --------------- vsffdat -------------------------
C    vsffdat -- find attribute of a vdata of a field
C               of the vdata
C    VSfindattr -- vsfcfda -- vsffdat
C
       integer function vsffdat(vsid, findex, attrnm)
       integer vsid, findex
       character*(*) attrnm
       integer vsfcfda

       vsffdat = vsfcfda(vsid, findex, attrnm, 
     +                   len(attrnm))
       end 
C --------------- vfsatt -------------------------
C    vfsatt -- set a numeric attr for a vgroup
C    Vsetattr -- vfcsatt -- vfsatt 
C
       integer function vfsatt(vgid, attrnm, dtype, 
     +                        count, values)
       integer vgid, dtype, count, values
       character*(*) attrnm
       integer vfcsatt

       vfsatt = vfcsatt(vgid, attrnm, dtype,count,
     +                  values, len(attrnm))
       end
C -------------- vfscatt -------------------------
C    vfscatt -- set a char attr for a vgroup
C    Vsetattr -- vfcscat -- vfscatt
C
       integer function vfscatt(vgid, attrnm, dtype,
     +                        count, values)
       integer vgid, dtype, count
       character*(*) attrnm, values
       integer vfcscat

       vfscatt = vfcscat(vgid, attrnm, dtype,count,
     +                  values, len(attrnm))
       end
C -------------- vfgcatt -------------------------
C    vfgcatt -- get a char attr for a vgroup
C    Vgetattr -- vfcgcat -- vfgcatt
C
       integer function vfgcatt(vgid, aindex, values)
       integer vgid, aindex
       character*(*) values
       integer vfcgcat

       vfgcatt = vfcgcat(vgid, aindex, values, len(values))
       end
C --------------- vffdatt ---------------------
C    vffdatt -- find an attribute of a vgroup
C    Vfindattr -- vfcfdat -- vffdatt
C
       integer function vffdatt(vg, attrnm)
       integer vg
       character*(*)   attrnm
       integer vfcfdat

       vffdatt = vfcfdat(vg, attrnm, len(attrnm))
       end
C --------------------------------------------

 

     

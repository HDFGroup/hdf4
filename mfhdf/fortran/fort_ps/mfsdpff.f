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
C mfsdpff.f, based on mfsdff.f,v 1.7
C
C----------------------------------------------------------------------
C     Name: sfstart
C     Purpose:  call scstart, open file
C     Inputs:   path: Name of file to be opened
C               access: DFACC_READ, DFACC_WRITE, DFACC_CREATE,
C               or any bitwise-or of the above.
C     Returns: 0 on success, FAIL on failure with error set
C     Users:    Fortran stub routine
C----------------------------------------------------------------------

      integer function sfstart(filename, access)

      character*(*) filename
      integer       access
C      integer       scstart
      INTERFACE
        INTEGER FUNCTION scstart(filename,access, nmlen)
          !MS$ATTRIBUTES C, reference, alias: '_SCSTART' :: scstart
          integer access, nmlen
          character*(*) filename
        END FUNCTION scstart
      END INTERFACE


      sfstart = scstart(filename, access, len(filename))
      return
      end
C----------------------------------------------------------------------
C     Name: sfend
C     Purpose:  call scend, close file
C     Inputs:   file_id: handle to HDF file to close
C     Returns: 0 on success, FAIL on failure with error set
C     Users:    HDF Fortran programmers
C----------------------------------------------------------------------

      integer function sfend(file_id)

      integer       file_id
C      integer       scend
      INTERFACE
        INTEGER FUNCTION scend(file_id)
          !MS$ATTRIBUTES C, reference, alias: '_SCEND' :: scend
          integer file_id
        END FUNCTION scend
      END INTERFACE

      sfend = scend(file_id)
      return
      end

C----------------------------------------------------------------------
C     Name: sfendacc
C     Purpose:  call scendacc, to end access to an sds
C     Inputs:   id: id of the data set
C     Returns: 0 on success, FAIL on failure with error set
C     Users:    HDF Fortran programmers
C----------------------------------------------------------------------

      integer function sfendacc(id)

      integer       id
C      integer       scendacc
      INTERFACE
        INTEGER FUNCTION scendacc(id)
          !MS$ATTRIBUTES C, reference, alias: '_SCENDACC' :: scendacc
          integer id
        END FUNCTION scendacc
      END INTERFACE

      sfendacc = scendacc(id)
      return
      end
C----------------------------------------------------------------------
C     Name: sffinfo
C     Purpose:  call scfinfo, get number datasets and attrs in the file
C     Inputs:   file_id: handle to HDF file to close
C     Outputs:  datasets: number of datasets
C               gattr:    number of global attrs
C     Returns: 0 on success, FAIL on failure with error set
C     Users:    HDF Fortran programmers
C----------------------------------------------------------------------

      integer function sffinfo(file_id, datasets, gattr)

      integer       file_id, datasets, gattr
C      integer       scfinfo
      INTERFACE
        INTEGER FUNCTION scfinfo(file_id,datasets,gattr)
          !MS$ATTRIBUTES C, reference, alias: '_SCFINFO' :: scfinfo
          integer file_id,datasets, gattr
        END FUNCTION scfinfo
      END INTERFACE

      sffinfo = scfinfo(file_id, datasets,gattr)
      return
      end

C----------------------------------------------------------------------
C     Name: sfselect
C     Purpose:  call scselct, to return a handle to a data set.
C     Inputs:   file_id:  handle to HDF file
C               index:    number of dataset to choose
C     Returns: sdsid on success, FAIL on failure with error set
C     Users:    Fortran stub routine
C----------------------------------------------------------------------

      integer function sfselect(file_id, index)

      integer       file_id, index
C      integer       scselct
      INTERFACE
        INTEGER FUNCTION scselct(file_id,index)
          !MS$ATTRIBUTES C, reference, alias: '_SCSELCT' :: scselct
          integer file_id, index
        END FUNCTION scselct
      END INTERFACE

      sfselect = scselct(file_id, index)
      return
      end

C----------------------------------------------------------------------
C     Name: sfdimid
C     Purpose:  call scdimid, get an id for a dim of a dataset
C     Inputs:   id: handle to a dataset
C               index: number of dimension to choose
C     Returns: 0 on success, FAIL on failure with error set
C     Users:    HDF Fortran programmers
C----------------------------------------------------------------------

      integer function sfdimid(id, index)

      integer       id, index
C      integer       scdimid
      INTERFACE
        INTEGER FUNCTION scdimid(id, index)
          !MS$ATTRIBUTES C, reference, alias: '_SCDIMID' :: scdimid
          integer id, index
        END FUNCTION scdimid
      END INTERFACE

      sfdimid = scdimid(id, index)
      return
      end
C----------------------------------------------------------------------
C     Name: sfgcal
C     Purpose:  call scgcal, get calibration info
C     Inputs:   id: handle to a dataset
C     Outputs:  cal, cale, ioff, ioffe: calibration factors and errors
C               nt: number type of data
C     Returns: 0 on success, FAIL on failure with error set
C     Users:    HDF Fortran programmers
C----------------------------------------------------------------------

      integer function sfgcal(id,cal,cale,ioff,ioffe,nt)

      integer       id, nt
      real*8    cal, cale, ioff, ioffe
C      integer       scgcal
      INTERFACE
        INTEGER FUNCTION scgcal(id,cal,cale,ioff,ioffe,nt)
          !MS$ATTRIBUTES C, reference, alias: '_SCGCAL' :: scgcal
          integer id, nt
          real*8  cal, cale, ioff, ioffe
        END FUNCTION scgcal
      END INTERFACE

      sfgcal = scgcal(id, cal,cale,ioff,ioffe,nt)
      return
      end
 
C----------------------------------------------------------------------
C     Name: sfscal
C     Purpose:  call scscal, set calibration info
C     Inputs:   id: handle to a dataset
C     Outputs:  cal, cale, ioff, ioffe: calibration factors and errors
C               nt: number type of data
C     Returns: 0 on success, FAIL on failure with error set
C     Users:    HDF Fortran programmers
C----------------------------------------------------------------------

      integer function sfscal(id,cal,cale,ioff,ioffe,nt)

      integer       id, nt
      real*8    cal, cale, ioff, ioffe
C      integer       scscal
      INTERFACE
        INTEGER FUNCTION scscal(id,cal,cale,ioff,ioffe,nt)
          !MS$ATTRIBUTES C, reference, alias: '_SCSCAL' :: scscal
          integer id, nt
          real*8  cal, cale, ioff, ioffe
        END FUNCTION scscal
      END INTERFACE

      sfscal = scscal(id, cal,cale,ioff,ioffe,nt)
      return
      end
 
C----------------------------------------------------------------------
C     Name: sfsdscale
C     Purpose:  call scsdscale, set dimension scale
C     Inputs:   id: handle to a dimension
C               count: size of the dimension
C               nt: number type of data
C               values: data
C     Returns: 0 on success, FAIL on failure with error set
C     Users:    HDF Fortran programmers
C----------------------------------------------------------------------

      integer function sfsdscale(id,count, nt, values)

      integer       id, count, nt, values
C      integer       scsdscale
      INTERFACE
        INTEGER FUNCTION scsdscale(id, count, nt , values)
          !MS$ATTRIBUTES C, reference, alias: '_SCSDSCALE' :: scsdscale
          integer id, count, nt, values
        END FUNCTION scsdscale
      END INTERFACE

      sfsdscale = scsdscale(id, count, nt, values)
      return
      end
C----------------------------------------------------------------------
C     Name: sfgdscale
C     Purpose:  call scgdscale, get dimension scale
C     Inputs:   id: handle to a dimension
C               values: data
C     Returns: 0 on success, FAIL on failure with error set
C     Users:    HDF Fortran programmers
C----------------------------------------------------------------------

      integer function sfgdscale(id, values)

      integer       id, values
C      integer       scgdscale
      INTERFACE
        INTEGER FUNCTION scgdscale(id, values)
          !MS$ATTRIBUTES C, reference, alias: '_SCGDSCALE' :: scgdscale
          integer id, values
        END FUNCTION scgdscale
      END INTERFACE

      sfgdscale = scgdscale(id, values)
      return
      end

C----------------------------------------------------------------------
C     Name: sfscfill
C     Purpose:  call scscfill, set char fill value
C     Inputs:   id: handle to a dataset
C               val: the fill value
C     Returns: 0 on success, FAIL on failure with error set
C     Users:    HDF Fortran programmers
C----------------------------------------------------------------------

      integer function sfscfill(id, val)

      integer       id
      character*1   val
C      integer       scscfill
      INTERFACE
        INTEGER FUNCTION scscfill(id, val)
          !MS$ATTRIBUTES C, reference, alias: '_SCSCFILL' :: scscfill
          integer id
          character*1 val
        END FUNCTION scscfill
      END INTERFACE

      sfscfill = scscfill(id, val)
      return
      end

C----------------------------------------------------------------------
C     Name: sfgcfill
C     Purpose:  call scgcfill, get char fill value
C     Inputs:   id: handle to a dataset
C               val: the fill value
C     Returns: 0 on success, FAIL on failure with error set
C     Users:    HDF Fortran programmers
C----------------------------------------------------------------------

      integer function sfgcfill(id, val)

      integer       id
      character*1   val
C      integer       scgcfill
      INTERFACE
        INTEGER FUNCTION scgcfill(id, val)
          !MS$ATTRIBUTES C, reference, alias: '_SCGCFILL' :: scgcfill
          integer id
          character*1 val
        END FUNCTION scgcfill
      END INTERFACE

      sfgcfill = scgcfill(id, val)
      return
      end

C----------------------------------------------------------------
C     Name: sfsfill
C     Purpose:  call scsfill, set numeric fill value
C     Inputs:   id: handle to a dataset
C               val: the fill value
C     Returns: 0 on success, FAIL on failure with error set
C     Users:    HDF Fortran programmers
C----------------------------------------------------------------

      integer function sfsfill(id, val)

      integer       id, val
C      integer       scsfill
      INTERFACE
        INTEGER FUNCTION scsfill(id, val)
          !MS$ATTRIBUTES C, reference, alias: '_SCSFILL' :: scsfill
          integer id, val
        END FUNCTION scsfill
      END INTERFACE

      sfsfill = scsfill(id, val)
      return
      end

C----------------------------------------------------------------------
C     Name: sfgfill
C     Purpose:  call scgfill, get numeric fill value
C     Inputs:   id: handle to a dataset
C               val: the fill value
C     Returns: 0 on success, FAIL on failure with error set
C     Users:    HDF Fortran programmers
C----------------------------------------------------------------------

      integer function sfgfill(id, val)

      integer       id, val
C      integer       scgfill
      INTERFACE
        INTEGER FUNCTION scgfill(id, val)
          !MS$ATTRIBUTES C, reference, alias: '_SCGFILL' :: scgfill
          integer id, val
        END FUNCTION scgfill
      END INTERFACE

      sfgfill = scgfill(id, val)
      return
      end

C----------------------------------------------------------------
C     Name: sfsrange
C     Purpose:  call scsrange, set the valid range info
C     Inputs:   id: handle to a dataset
C               max, min: the maximum and minimum values
C     Returns: 0 on success, FAIL on failure with error set
C     Users:    HDF Fortran programmers
C----------------------------------------------------------------

      integer function sfsrange(id, max, min)

      integer       id, max, min
C      integer       scsrange
      INTERFACE
        INTEGER FUNCTION scsrange(id, max, min)
          !MS$ATTRIBUTES C, reference, alias: '_SCSRANGE' :: scsrange
          integer id, max, min
        END FUNCTION scsrange
      END INTERFACE

      sfsrange = scsrange(id, max, min)
      return
      end

C----------------------------------------------------------------------
C     Name: sfgrange
C     Purpose:  call scgrange, get the valid range info
C     Inputs:   id: handle to a dataset
C               max, min:  the maximum and minimum values
C     Returns: 0 on success, FAIL on failure with error set
C     Users:    HDF Fortran programmers
C----------------------------------------------------------------------

      integer function sfgrange(id, max, min)

      integer       id, max, min
C      integer       scgrange
      INTERFACE
        INTEGER FUNCTION scgrange(id, max, min)
          !MS$ATTRIBUTES C, reference, alias: '_SCGRANGE' :: scgrange
          integer id, max, min
        END FUNCTION scgrange
      END INTERFACE

      sfgrange = scgrange(id, max, min)
      return
      end

C----------------------------------------------------------------------
C     Name: sdfn2index
C     Purpose:  call scn2index to return an index
C     Inputs:   id: file
C               name: name of data set
C     Returns: 0 on success, FAIL on failure with error set
C     Users:    Fortran stub routine
C----------------------------------------------------------------------

      integer function sfn2index(id, name)

      character*(*) name
      integer       id
c      integer       scn2index
      INTERFACE
        INTEGER FUNCTION scn2index(id, name, nmlen)
          !MS$ATTRIBUTES C, reference, alias: '_SCN2INDEX' :: scn2index
          integer id, nmlen
          character*(*) name
        END FUNCTION scn2index
      END INTERFACE

      sfn2index = scn2index(id, name, len(name))
      return
      end


C----------------------------------------------------------------------
C     Name: sfcreate
C     Purpose:  call sccreate to create a new dataset
C     Inputs:   id: file
C               name: name of data set
C               nt: number type
C               rank: rank
C               dims: sizes of dimensions
C     Returns: 0 on success, FAIL on failure with error set
C     Users:    Fortran stub routine
C----------------------------------------------------------------------

      integer function sfcreate(id, name, nt, rank, dims)

      character*(*) name
      integer       id, nt, rank
      integer       dims(*)
C      integer       sccreate
      INTERFACE
        INTEGER FUNCTION sccreate(id,name,nt,rank,dims,nmlen)
          !MS$ATTRIBUTES C, reference, alias: '_SCCREATE' :: sccreate
          integer id, nt, rank, dims(*), nmlen
          character*(*) name
        END FUNCTION sccreate
      END INTERFACE

      sfcreate = sccreate(id, name, nt, rank, dims, len(name))
      return
      end


C----------------------------------------------------------------------
C Name:     sfsdmstr
C Purpose:  set strings for this dimension
C Inputs:   id: dimension id
C           label,unit,format: strings to be set
C Returns:  0 on success, -1 on failure with error set
C Users:    HDF Fortran programmers
C----------------------------------------------------------------------

      integer function  sfsdmstr(id, label, unit, format)

      character*(*) label, unit, format
      integer len, id
C      integer scsdimstr
      INTERFACE
        INTEGER FUNCTION scsdimstr(id, label, unit, format,
     +                       llen,ulen, flen)
          !MS$ATTRIBUTES C, reference, alias: '_SCSDIMSTR' :: scsdimstr
          integer id, llen, ulen, flen
          character*(*) label, unit, format
        END FUNCTION scsdimstr
      END INTERFACE

      sfsdmstr = scsdimstr(id, label, unit, format,
     +     len(label), len(unit), len(format))
      
      return
      end

C----------------------------------------------------------------------
C Name:     sfsdmname
C Purpose:  set name of this dimension
C Inputs:   id: dimension id
C           name: the name 
C Returns:  0 on success, -1 on failure with error set
C Users:    HDF Fortran programmers
C----------------------------------------------------------------------

      integer function  sfsdmname(id, name)

      character*(*) name
      integer len, id
C      integer scsdimname
      INTERFACE
        INTEGER FUNCTION scsdimname(id, name, nmlen)
          !MS$ATTRIBUTES C,reference,alias:'_SCSDIMNAME' :: scsdimname
          integer id, nmlen
          character*(*) name
        END FUNCTION scsdimname
      END INTERFACE

      sfsdmname = scsdimname(id, name, len(name))
      
      return
      end
        

C----------------------------------------------------------------------
C Name:     sfsdtstr
C Purpose:  set strings for this data set
C Inputs:   id: dimension id
C           label,unit,format,coordsys: strings to be set
C Returns:  0 on success, -1 on failure with error set
C Users:    HDF Fortran programmers
C----------------------------------------------------------------------

      integer function sfsdtstr(id, l, u, f, c)

      character*(*) l, u, f, c
      integer len, id
C      integer scsdatstr
      INTERFACE
        INTEGER FUNCTION scsdatstr(id, l, u, f, c, llen, 
     +                         ulen, flen, clen)
          !MS$ATTRIBUTES C,reference,alias:'_SCSDATSTR' :: scsdatstr
          integer id, llen, ulen, flen, clen
	    character*(*) l,u,f,c
        END FUNCTION scsdatstr
      END INTERFACE

      sfsdtstr = scsdatstr(id, l, u, f, c,
     +     len(l), len(u), len(f), len(c))
     
      return
      end
   

C----------------------------------------------------------------------
C Name:     sfgdtstr
C Purpose:  get strings for this data set
C Inputs:   id: dimension id
C Outputs:  label,unit,format,coordsys: strings to be gotten
C Returns:  0 on success, -1 on failure with error set
C Users:    HDF Fortran programmers
C----------------------------------------------------------------------

      integer function sfgdtstr(id, label, unit, format, coord, mlen)

      character*(*) label, unit, format, coord
      integer len, id, mlen
C       integer scgdatstrs
      INTERFACE
        INTEGER FUNCTION scgdatstrs(id,label,unit,format,coord,
     +                               llen,ulen,flen, clen,mlen)
          !MS$ATTRIBUTES C,reference,alias:'_SCGDATSTRS' :: scgdatstrs
          integer id, llen, ulen, flen, clen, mlen
          character*(*) label, unit, format, coord
        END FUNCTION scgdatstrs
      END INTERFACE

      sfgdtstr = scgdatstrs(id, label, unit, format, coord,
     +     len(label), len(unit), len(format), len(coord), mlen)
      
      return
      end
   
C----------------------------------------------------------------------
C Name:     sfgdmstr
C Purpose:  get strings for this dimension
C Inputs:   id: dimension id
C Outputs:  label,unit,format: strings to be gotten
C Returns:  0 on success, -1 on failure with error set
C Users:    HDF Fortran programmers
C----------------------------------------------------------------------

      integer function  sfgdmstr(id, label, unit, format, mlen)

      character*(*) label, unit, format
      integer len, id
C      integer scgdimstrs
      INTERFACE
        INTEGER FUNCTION scgdimstrs(id,label,unit,format,
     + 	  llen,ulen,flen,mlen)
          !MS$ATTRIBUTES C,reference,alias:'_SCGDIMSTRS' :: scgdimstrs
          integer id, llen, ulen, flen, mlen
          character*(*) label, unit, format
        END FUNCTION scgdimstrs
      END INTERFACE

      sfgdmstr = scgdimstrs(id, label, unit, format,
     +     len(label), len(unit), len(format), mlen)
      
      return
      end
   
C----------------------------------------------------------------------
C Name:     sfginfo
C Purpose:  get basic info about this data set
C Inputs:   id: dataset id
C Outputs:  name, rank, nt, dims, nattrs
C Returns:  0 on success, -1 on failure with error set
C Users:    HDF Fortran programmers
C----------------------------------------------------------------------

      integer function  sfginfo(id, name, rank, dims, nt, nattr)

      character*(*) name
      integer rank, nt, nattr
      integer dims(*)
      integer len
c      integer scginfo
      INTERFACE
        INTEGER FUNCTION scginfo(id,name,rank,dims,nt,nattr,nmlen)
          !MS$ATTRIBUTES C, reference, alias: '_SCGINFO' :: scginfo
          integer id,rank,dims(*),nt,nattr,nmlen
          character*(*) name
        END FUNCTION scginfo
      END INTERFACE

      sfginfo = scginfo(id, name, rank, dims, nt, nattr, len(name))
      
      return
      end
   
C----------------------------------------------------------------------
C Name:     sfrcatt
C Purpose:  call scrcatt to get the contents of a char attribute
C Inputs:   id: dataset id
C           index: index of the attribute to read
C           buf: space to hold attribute values
C Returns:  0 on success, -1 on failure with error set
C Users:    HDF Fortran programmers
C----------------------------------------------------------------------

      integer function  sfrcatt(id, index, buf)

      character   buf(*)
      integer id, index
c      integer scrcatt
      INTERFACE
        INTEGER FUNCTION scrcatt(id, index, buf)
          !MS$ATTRIBUTES C, reference, alias: '_SCRCATT' :: scrcatt
          integer id, index
          character   buf(*)
        END FUNCTION scrcatt
      END INTERFACE

      sfrcatt = scrcatt(id, index, buf)

      return
      end

C----------------------------------------------------------------------
C Name:     sfrnatt
C Purpose:  call scrnatt to get the contents of a numeric attribute
C Inputs:   id: dataset id
C           index: index of the attribute to read
C           buf: space to hold attribute values
C Returns:  0 on success, -1 on failure with error set
C Users:    HDF Fortran programmers
C----------------------------------------------------------------------

      integer function  sfrnatt(id, index, buf)

      integer id, index, buf
c      integer scrnatt
      INTERFACE
        INTEGER FUNCTION scrnatt(id, index, buf)
          !MS$ATTRIBUTES C, reference, alias: '_SCRNATT' :: scrnatt
          integer id, index, buf
        END FUNCTION scrnatt
      END INTERFACE

      sfrnatt = scrnatt(id, index, buf)

      return
      end

C----------------------------------------------------------------------
C Name:     sfrattr
C Purpose:  call scrattr to get the contents of an attribute
C Inputs:   id: dataset id
C           index: index of the attribute to read
C           buf: space to hold attribute values
C Returns:  0 on success, -1 on failure with error set
C Users:    HDF Fortran programmers
C----------------------------------------------------------------------

      integer function  sfrattr(id, index, buf)

      character*(*) buf
      integer id, index
c      integer scrattr
      INTERFACE
        INTEGER FUNCTION scrattr(id, index, buf)
          !MS$ATTRIBUTES C, reference, alias: '_SCRATTR' :: scrattr
          integer id, index
          character*(*) buf
        END FUNCTION scrattr
      END INTERFACE

      sfrattr = scrattr(id, index, buf)

      return
      end

C----------------------------------------------------------------------
C Name:     sfrdata
C Purpose:  call scrdata to read a section of numeric data
C Inputs:   id: dataset id
C           start: start location
C           stride: stride along each dimension
C           end: number of values along each dim to read
C           values: data
C Returns:  0 on success, -1 on failure with error set
C Users:    HDF Fortran programmers
C----------------------------------------------------------------------

      integer function  sfrdata(id, start,stride, end,values)

      integer id, start(*), stride(*), end(*), values(*)
c      integer scrdata
      INTERFACE
        INTEGER FUNCTION scrdata(id,start, stride, end, values) 
          !MS$ATTRIBUTES C, reference, alias: '_SCRDATA' :: scrdata
          integer id, start(*), stride(*), end(*), values(*)
        END FUNCTION scrdata
      END INTERFACE

      sfrdata = scrdata(id,start, stride, end, values)

      return
      end

C----------------------------------------------------------------------
C Name:     sfwdata
C Purpose:  call scwdata to write a section of numeric data
C Inputs:   id: dataset id
C           start: start location
C           stride: stride along each dimension
C           end: number of values along each dim to write
C           values: data
C Returns:  0 on success, -1 on failure with error set
C Users:    HDF Fortran programmers
C----------------------------------------------------------------------

      integer function  sfwdata(id, start,stride, end,values)

      integer id, start(*), stride(*), end(*), values(*)
c      integer scwdata
      INTERFACE
        INTEGER FUNCTION scwdata(id,start, stride, end, values) 
          !MS$ATTRIBUTES C, reference, alias: '_SCWDATA' :: scwdata
          integer id, start(*), stride(*), end(*), values(*)
        END FUNCTION scwdata
      END INTERFACE

      sfwdata = scwdata(id,start, stride, end, values)

      return
      end

C----------------------------------------------------------------------
C Name:     sfrcdata
C Purpose:  call scrcdata to read a section of char data
C Inputs:   id: dataset id
C           start: start location
C           stride: stride along each dimension
C           end: number of values along each dim to read
C           values: data
C Returns:  0 on success, -1 on failure with error set
C Users:    HDF Fortran programmers
C----------------------------------------------------------------------

      integer function  sfrcdata(id, start,stride, end,values)

      integer id, start(*), stride(*), end(*)
      character*(*) values
c      integer scrcdata
      INTERFACE
        INTEGER FUNCTION scrcdata(id,start, stride, end, values) 
          !MS$ATTRIBUTES C, reference, alias: '_SCRCDATA' :: scrcdata
          integer id, start(*), stride(*), end(*)
          character *(*) values
        END FUNCTION scrcdata
      END INTERFACE

      sfrcdata = scrcdata(id,start, stride, end, values)

      return
      end

C----------------------------------------------------------------------
C Name:     sfwcdata
C Purpose:  call scwcdata to write a section of numeric data
C Inputs:   id: dataset id
C           start: start location
C           stride: stride along each dimension
C           end: number of values along each dim to write
C           values: data
C Returns:  0 on success, -1 on failure with error set
C Users:    HDF Fortran programmers
C----------------------------------------------------------------------

      integer function  sfwcdata(id, start,stride, end,values)

      integer id, start(*), stride(*), end(*)
      character*(*) values
c      integer scwcdata
      INTERFACE
        INTEGER FUNCTION scwcdata(id,start, stride, end, values) 
          !MS$ATTRIBUTES C, reference, alias: '_SCWCDATA' :: scwcdata
          integer id, start(*), stride(*), end(*)
          character*(*) values
        END FUNCTION scwcdata
      END INTERFACE

      sfwcdata = scwcdata(id,start, stride, end, values)

      return
      end


C----------------------------------------------------------------------
C Name:     sfgainfo
C Purpose:  get basic info about an attribute
C Inputs:   id: dataset id
C           number: attribute number
C Outputs:  name, nt, count
C Returns:  0 on success, -1 on failure with error set
C Users:    HDF Fortran programmers
C----------------------------------------------------------------------

      integer function  sfgainfo(id, number, name, nt, count)

      character*(*) name
      integer nt, number, len, count
C      integer scgainfo
      INTERFACE
        INTEGER FUNCTION scgainfo(id, number, name,nt, count,nmlen)
          !MS$ATTRIBUTES C, reference, alias: '_SCGAINFO' :: scgainfo
          integer id, number, nt, count, nmlen
          character*(*) name
        END FUNCTION scgainfo
      END INTERFACE

      sfgainfo = scgainfo(id, number, name, nt, count, len(name))
      
      return
      end

C----------------------------------------------------------------------
C Name:     sfgdinfo
C Purpose:  get basic info about a dimension
C Inputs:   id: dim id
C Outputs:  name, nt, sz, nattr
C Returns:  0 on success, -1 on failure with error set
C Users:    HDF Fortran programmers
C----------------------------------------------------------------------

      integer function  sfgdinfo(id, name, sz, nt, nattr)

      character*(*) name
      integer sz, nt, nattr, len
C      integer scgdinfo
      INTERFACE
        INTEGER FUNCTION scgdinfo(id,name, sz, nt, nattr,nmlen)
          !MS$ATTRIBUTES C, reference, alias: '_SCGDINFO' :: scgdinfo
          integer id, sz, nt, nattr, nmlen
          character*(*) name
        END FUNCTION scgdinfo
      END INTERFACE

      sfgdinfo = scgdinfo(id, name, sz, nt, nattr, len(name))
      
      return
      end

C----------------------------------------------------------------------
C Name:     sfscatt
C Purpose:  create (or modify an existing) char attribute
C Inputs:   id: id of object to attribute-ize
C Outputs:  name, nt, count, data
C Returns:  0 on success, -1 on failure with error set
C Users:    HDF Fortran programmers
C----------------------------------------------------------------------

      integer function  sfscatt(id,name,nt,count,data)

      character*(*) name
      character data(*)
      integer nt, count, len
C      integer scscatt
      INTERFACE
        INTEGER FUNCTION scscatt(id,name, nt, count,data, nmlen)
          !MS$ATTRIBUTES C, reference, alias: '_SCSCATT' :: scscatt
          integer id, nt, count, nmlen
          character*(*) name
          character     data(*)
        END FUNCTION scscatt
      END INTERFACE

      sfscatt = scscatt(id,name,nt,count,data,len(name))
      
      return
      end

C----------------------------------------------------------------------
C Name:     sfsnatt
C Purpose:  create (or modify an existing) numeric attribute
C Inputs:   id: id of object to attribute-ize
C Outputs:  name, nt, count, data
C Returns:  0 on success, -1 on failure with error set
C Users:    HDF Fortran programmers
C----------------------------------------------------------------------

      integer function  sfsnatt(id, name, nt, count, data)

      character*(*) name
      integer nt, data, count, len
C      integer scsnatt
      INTERFACE
        INTEGER FUNCTION scsnatt(id,name, nt, count,data, nmlen)
          !MS$ATTRIBUTES C, reference, alias: '_SCSNATT' :: scsnatt
          integer id, nt, count, data, nmlen
          character*(*) name
        END FUNCTION scsnatt
      END INTERFACE

      sfsnatt = scsnatt(id, name, nt, count, data, len(name))
      
      return
      end

C----------------------------------------------------------------------
C Name:     sfsattr
C Purpose:  for backward compatability. Calls scsattr to 
C           create (or modify an existing) attribute
C Inputs:   id: id of object to attribute-ize
C Outputs:  name, nt, count, data
C Returns:  0 on success, -1 on failure with error set
C Users:    HDF Fortran programmers
C Remarks:  This function should be phased out in the future.
C----------------------------------------------------------------------

      integer function  sfsattr(id, name, nt, count, data)

      character*(*) name, data
      integer nt, len, count
C      integer scsattr
      INTERFACE
        INTEGER FUNCTION scsattr(id,name, nt, count,data, nmlen)
          !MS$ATTRIBUTES C, reference, alias: '_SCSATTR' :: scsattr
          integer id, nt, count, nmlen
          character*(*) name, data
        END FUNCTION scsattr
      END INTERFACE

      sfsattr = scsattr(id, name, nt, count, data, len(name))
      
      return
      end

C----------------------------------------------------------------------
C     Name: sffattr
C     Purpose:  call scfattr, find an attribute
C     Inputs:   id: Object to look at
C               name: name of attribute
C     Returns: attribute number on success, FAIL on failure with error set
C     Users:    Fortran stub routine
C----------------------------------------------------------------------

      integer function sffattr(id, name)

      character*(*) name
      integer       id, len
C      integer       scfattr
      INTERFACE
        INTEGER FUNCTION scfattr(id,name, nmlen)
          !MS$ATTRIBUTES C, reference, alias: '_SCFATTR' :: scfattr
          integer id, nmlen
          character*(*) name
        END FUNCTION scfattr
      END INTERFACE

      sffattr = scfattr(id, name, len(name))
      return
      end

C----------------------------------------------------------------------
C     Name: sfid2ref
C     Purpose:  call scid2ref, map an id to ref
C     Inputs:   id: variable id
C     Returns: refernce number on success, FAIL on failure with errors
C              set
C     Users:    Fortran stub routine
C----------------------------------------------------------------------

      integer function sfid2ref(id)

      integer       id
C      integer       scid2ref
      INTERFACE
        INTEGER FUNCTION scid2ref(id)
          !MS$ATTRIBUTES C, reference, alias: '_SCID2REF' :: scid2ref
          integer id
        END FUNCTION scid2ref
      END INTERFACE

      sfid2ref = scid2ref(id)
      return
      end

C----------------------------------------------------------------------
C     Name: sfref2index
C     Purpose:  call scr2idx, map an ref to index
C     Inputs:   id: variable id
C               ref: reference number
C     Returns: index on success, FAIL on failure with error set
C     Users:    Fortran stub routine
C----------------------------------------------------------------------

      integer function sfref2index(id, ref)

      integer       id, ref
C      integer       scr2idx
      INTERFACE
        INTEGER FUNCTION scr2idx(id, ref)
          !MS$ATTRIBUTES C, reference, alias: '_SCR2IDX' :: scr2idx
          integer id, ref
        END FUNCTION scr2idx
      END INTERFACE

      sfref2index = scr2idx(id, ref)
      return
      end

C----------------------------------------------------------------------
C     Name: sfiscvar
C     Purpose:  call sciscvar to see if a dataset is a coordinate var 
C     Inputs:   id: variable id
C     Returns: TRUE/FALSE
C     Users:    Fortran stub routine
C----------------------------------------------------------------------

      integer function sfiscvar(id)

      integer       id
C      integer       sciscvar
      INTERFACE
        INTEGER FUNCTION sciscvar(id)
          !MS$ATTRIBUTES C, reference, alias: '_SCISCVAR' :: sciscvar
          integer id
        END FUNCTION sciscvar
      END INTERFACE

      sfiscvar = sciscvar(id)
      return
      end

C----------------------------------------------------------------------
C     Name: sfsextf
C     Purpose:  call scsextf to store data in an external file
C     Inputs:   id: sds id
C               fname: name of external file
C               offset: Number of bytes from the beginning of the
C                       external file to where the data starts
C     Returns: 0 on success, FAIL on failure with error set
C     Users:    Fortran stub routine
C----------------------------------------------------------------------

      integer function sfsextf(id, fname, offset)

      character*(*) fname
      integer       id, offset, len
C     integer       scsextf
      INTERFACE
        INTEGER FUNCTION scsextf(id, fname, offset, nmlen)
          !MS$ATTRIBUTES C, reference, alias: '_SCSEXTF' :: scsextf
          integer id, offset, nmlen
          character*(*) fname
        END FUNCTION scsextf
      END INTERFACE

      sfsextf = scsextf(id, fname, offset, len(fname))
      return
      end

C----------------------------------------------------------------------
C     Name: sfsnbit
C     Purpose:  call scsnbit, store data in n-bit data element
C     Inputs:  id: variable id
C              start_bit: starting bit offset
C              bit_len: # 0f bits to write
C              sign_ext: whether to use the top bit as a sign extender
C              fill_one: whether to fill the "background bits" with ones
C     Returns: SUCCEED on success, FAIL on failure with error set
C     Users:    Fortran stub routine
C----------------------------------------------------------------------

      integer function sfsnbit(id,start_bit,bit_len,sign_ext,fill_one)

      integer       id,start_bit,bit_len,sign_ext,fill_one 
C      integer       scsnbit
      INTERFACE
        INTEGER FUNCTION scsnbit(id,start_bit,bit_len,sign_ext,fill_one) 
          !MS$ATTRIBUTES C, reference, alias: '_SCSNBIT' :: scsnbit
          integer id,start_bit,bit_len,sign_ext,fill_one
        END FUNCTION scsnbit
      END INTERFACE

      sfsnbit = scsnbit(id,start_bit,bit_len,sign_ext,fill_one)
      return
      end

C----------------------------------------------------------------------
C     Name: sfsacct
C     Purpose:  call scsacct, set tje access type
C     Inputs:   id: variable id
C               type: the access type
C     Returns: SUCCEED on success, FAIL on failure with error set
C     Users:    Fortran stub routine
C----------------------------------------------------------------------

      integer function sfsacct(id, acctype)

      integer       id, acctype
C      integer       scsacct
      INTERFACE
        INTEGER FUNCTION scsacct(id, acctype)
          !MS$ATTRIBUTES C, reference, alias: '_SCSACCT' :: scsacct
          integer id, acctype
        END FUNCTION scsacct
      END INTERFACE

      sfsacct = scsacct(id, acctype)
      return
      end

C----------------------------------------------------------------------
C     Name: sfsdmvc
C     Purpose:  call scsdmvc, set dim value backward compatible type
C     Inputs:   id: dim id
C               compmode:  (defined in mfhdf.h)
C                  SD_DIMVAL_BW_COMP -- backward compatible 
C                  SD_DIMVAL_BW_INCOMP -- incompatible
C     Returns: SUCCEED on success, FAIL on failure with error set
C     Users:    Fortran stub routine
C----------------------------------------------------------------------

      integer function sfsdmvc(id, comptype)

      integer       id, comptype
C      integer       scsdmvc
      INTERFACE
        INTEGER FUNCTION scsdmvc(id, comptype)
          !MS$ATTRIBUTES C, reference, alias: '_SCSDMVC' :: scsdmvc
          integer id, comptype
        END FUNCTION scsdmvc
      END INTERFACE

      sfsdmvc = scsdmvc(id, comptype)
      return
      end

C----------------------------------------------------------------------
C     Name: sfisdmvc
C     Purpose:  call scisdmvc, get dim value backward compatibility 
C     Inputs:   id: dim id
C     Returns:  SD_DIMVAL_BW_COMP(1) -- backward compatible 
C               SD_DIMVAL_BW_INCOMP(0) -- incompatible
C               FAIL(-1) on failure with error set
C     Users:    Fortran stub routine
C----------------------------------------------------------------------

      integer function sfisdmvc(id)

      integer       id
C      integer       scisdmvc
      INTERFACE
        INTEGER FUNCTION scisdmvc(id)
          !MS$ATTRIBUTES C, reference, alias: '_SCISDMVC' :: scisdmvc
          integer id
        END FUNCTION scisdmvc
      END INTERFACE

      sfisdmvc = scisdmvc(id)
      return
      end

C----------------------------------------------------------------------
C     Name: sfsflmd
C     Purpose:  call scsflmd, set file fill mode
C     Inputs:   id: file id
C               fillmode: desired fill mode;
C                    SD_FILL(default) or SD_NOFILL. 
C     Returns: the current( before the new mode is set) fill mode;
C              FAIL on failure with error set
C     Users:    Fortran stub routine
C----------------------------------------------------------------------

      integer function sfsflmd(file_id, fillmode)

      integer       file_id, fillmode
C      integer       scsflmd
      INTERFACE
        INTEGER FUNCTION scsflmd(file_id, fillmode)
          !MS$ATTRIBUTES C, reference, alias: '_SCSFLMD' :: scsflmd
          integer file_id, fillmode
        END FUNCTION scsflmd
      END INTERFACE

      sfsflmd = scsflmd(file_id, fillmode)
      return
      end



C * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
C  Copyright by The HDF Group.                                               *
C  Copyright by the Board of Trustees of the University of Illinois.         *
C  All rights reserved.                                                      *
C                                                                            *
C  This file is part of HDF.  The full HDF copyright notice, including       *
C  terms governing use, modification, and redistribution, is contained in    *
C  the COPYING file, which can be found at the root of the source code       *
C  distribution tree, or in https://support.hdfgroup.org/ftp/HDF/releases/.  *
C  If you do not have access to either file, you may request a copy from     *
C  help@hdfgroup.org.                                                        *
C * * * * * * * * *  * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
C
C $Id$
C

C----------------------------------------------------------------------
C     Name: sdfstart
C     Purpose:  call scstart, open file
C     Inputs:   path: Name of file to be opened
C               access: DFACC_READ, DFACC_WRITE, DFACC_CREATE,
C               or any bitwise-or of the above.
C     Returns: 0 on success, FAIL on failure with error set
C     Users:    Fortran stub routine
C----------------------------------------------------------------------

      integer function sfstart(filename, access)

      character*(*) filename
      integer       access, scstart

      sfstart = scstart(filename, access, len(filename))
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
      integer       id, scn2index

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
      integer       id, nt, rank, sccreate
      integer       dims(*)

      sfcreate = sccreate(id, name, nt, rank, dims, len(name))
      return
      end


C----------------------------------------------------------------------
C Name:     sfsdmstr
C Purpose:  set strings for this dimension
C Inputs:   id: dimension id
C           label,unit,format: strings to be set
C Returns:  0 on success, -1 on failure with DFerror set
C Users:    HDF Fortran programmers
C----------------------------------------------------------------------

      integer function  sfsdmstr(id, label, unit, format)

      character*(*) label, unit, format
      integer len, id
      integer scsdimstr

      sfsdmstr = scsdimstr(id, label, unit, format,
     +     len(label), len(unit), len(format))
      
      return
      end

C----------------------------------------------------------------------
C Name:     sfsdmname
C Purpose:  set name of this dimension
C Inputs:   id: dimension id
C           name: the name 
C Returns:  0 on success, -1 on failure with DFerror set
C Users:    HDF Fortran programmers
C----------------------------------------------------------------------

      integer function  sfsdmname(id, name)

      character*(*) name
      integer len, id
      integer scsdimname

      sfsdmname = scsdimname(id, name, len(name))
      
      return
      end
        

C----------------------------------------------------------------------
C Name:     sfsdtstr
C Purpose:  set strings for this data set
C Inputs:   id: dimension id
C           label,unit,format,coordsys: strings to be set
C Returns:  0 on success, -1 on failure with DFerror set
C Users:    HDF Fortran programmers
C----------------------------------------------------------------------

      integer function sfsdtstr(id, l, u, f, c)

      character*(*) l, u, f, c
      integer len, id
      integer scsdatstr

      sfsdtstr = scsdatstr(id, l, u, f, c,
     +     len(l), len(u), len(f), len(c))
     
      return
      end
   

C----------------------------------------------------------------------
C Name:     sfgdtstr
C Purpose:  get strings for this data set
C Inputs:   id: dimension id
C Outputs:  label,unit,format,coordsys: strings to be gotten
C Returns:  0 on success, -1 on failure with DFerror set
C Users:    HDF Fortran programmers
C----------------------------------------------------------------------

      integer function sfgdtstr(id, label, unit, format, coord, mlen)

      character*(*) label, unit, format, coord
      integer len, id, mlen
      integer scgdatstrs

      sfgdtstr = scgdatstrs(id, label, unit, format, coord,
     +     len(label), len(unit), len(format), len(coord), mlen)
      
      return
      end
   
C----------------------------------------------------------------------
C Name:     sfgdmstr
C Purpose:  get strings for this dimension
C Inputs:   id: dimension id
C Outputs:  label,unit,format: strings to be gotten
C Returns:  0 on success, -1 on failure with DFerror set
C Users:    HDF Fortran programmers
C----------------------------------------------------------------------

      integer function  sfgdmstr(id, label, unit, format, mlen)

      character*(*) label, unit, format
      integer len, id
      integer scgdimstrs

      sfgdmstr = scgdimstrs(id, label, unit, format,
     +     len(label), len(unit), len(format), mlen)
      
      return
      end
   
C----------------------------------------------------------------------
C Name:     sfginfo
C Purpose:  get basic info about this data set
C Inputs:   id: dataset id
C Outputs:  name, rank, nt, dims, nattrs
C Returns:  0 on success, -1 on failure with DFerror set
C Users:    HDF Fortran programmers
C----------------------------------------------------------------------

      integer function  sfginfo(id, name, rank, dims, nt, nattr)

      character*(*) name
      integer rank, nt, nattr
      integer dims(*)
      integer scginfo, len

      sfginfo = scginfo(id, name, rank, dims, nt, nattr, len(name))
      
      return
      end
   
C----------------------------------------------------------------------
C Name:     sfgainfo
C Purpose:  get basic info about an attribute
C Inputs:   id: dataset id
C           number: attribute number
C Outputs:  name, nt, count
C Returns:  0 on success, -1 on failure with DFerror set
C Users:    HDF Fortran programmers
C----------------------------------------------------------------------

      integer function  sfgainfo(id, number, name, nt, count)

      character*(*) name
      integer nt, number
      integer scgainfo, len

      sfgainfo = scgainfo(id, number, name, nt, count, len(name))
      
      return
      end
   

C----------------------------------------------------------------------
C Name:     sfgdinfo
C Purpose:  get basic info about a dimension
C Inputs:   id: dim id
C Outputs:  name, nt, sz, nattr
C Returns:  0 on success, -1 on failure with DFerror set
C Users:    HDF Fortran programmers
C----------------------------------------------------------------------

      integer function  sfgdinfo(id, name, sz, nt, nattr)

      character*(*) name
      integer sz, nt, nattr
      integer scgdinfo, len

      sfgdinfo = scgdinfo(id, name, sz, nt, nattr, len(name))
      
      return
      end

C----------------------------------------------------------------------
C Name:     sfscatt
C Purpose:  create (or modify an existing) char attribute
C Inputs:   id: id of object to attribute-ize
C Outputs:  name, nt, count, data
C Returns:  0 on success, -1 on failure with DFerror set
C Users:    HDF Fortran programmers
C----------------------------------------------------------------------

      integer function  sfscatt(id,name,nt,count,data)

      character*(*) name, data
      integer nt
      integer scscatt, len

      sfscatt = scscatt(id,name,nt,count,data,len(name))
      
      return
      end

C----------------------------------------------------------------------
C Name:     sfsnatt
C Purpose:  create (or modify an existing) numeric attribute
C Inputs:   id: id of object to attribute-ize
C Outputs:  name, nt, count, data
C Returns:  0 on success, -1 on failure with DFerror set
C Users:    HDF Fortran programmers
C----------------------------------------------------------------------

      integer function  sfsnatt(id, name, nt, count, data)

      character*(*) name
      integer nt, data
      integer scsnatt, len

      sfsnatt = scsnatt(id, name, nt, count, data, len(name))
      
      return
      end

C----------------------------------------------------------------------
C Name:     sfsattr
C Purpose:  for backward compatability. Calls scsattr to 
C           create (or modify an existing) attribute
C Inputs:   id: id of object to attribute-ize
C Outputs:  name, nt, count, data
C Returns:  0 on success, -1 on failure with DFerror set
C Users:    HDF Fortran programmers
C Remarks:  This function should be phased out in the future.
C----------------------------------------------------------------------

      integer function  sfsattr(id, name, nt, count, data)

      character*(*) name, data
      integer nt
      integer scsattr, len

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
      integer       id, scfattr

      sffattr = scfattr(id, name, len(name))
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
      integer       id, offset, scsextf

      sfsextf = scsextf(id, fname, offset, len(fname))
      return
      end
C-------------------------------------------------------------------------
C        Name:      sfgichnk
C        Purpose:   get Info on SDS
C        Inputs:    id       - data set ID
C        Outputs:   dim_length - chunk dimensions (if any)
C                   comp_flag: 
C                               -1 - SDS is non-chunked
C                                0 - SDS is chunked without compression
C                                1 - SDS is chunked and compressed with
C                                    RLE, GZIP, SZIP or SKPHUFF 
C                                2 - SDS is chunked and compressed with
C                                    NBIT
C        Returns:   0 on success, -1 on failure
C        Calls:     scgichnk (C stub for SDgetchunkinfo function)
C-------------------------------------------------------------------------

         integer function sfgichnk(id, dim_length, comp_flag)

         integer id, dim_length(*), comp_flag
         integer scgichnk 

         sfgichnk = scgichnk(id, dim_length, comp_flag)
         return
         end

           
C-------------------------------------------------------------------------
C        Name:      sfrcchnk
C        Purpose:   read the specified chunk of CHARACTER data to the SDS 
C        Inputs:    id       - data set ID
C                   start    - chunk coordinates 
C                   char_data - buffer the data will be read into  
C        Returns:   0 on success, -1 on failure
C        Calls:     scrcchnk (C stub for SDreadchunk function)
C        Users:     HDF Fortran programmers
C-------------------------------------------------------------------------

         integer function sfrcchnk(id, start, char_data)
C
         integer id, start(*)
         character*(*) char_data(*)
         integer scrcchnk 
C
         sfrcchnk = scrcchnk(id, start, char_data) 
C
         return 
         end

           
C-------------------------------------------------------------------------
C        Name:      sfrchnk
C        Purpose:   read the specified chunk of NUMERIC data to the SDS 
C        Inputs:    id       - data set ID
C                   start    - chunk coordinates 
C                   num_data - buffer the  numeric data will be read into  
C        Returns:   0 on success, -1 on failure
C        Calls:     scrchnk (C stub for SDreadchunk function)
C        Users:     HDF Fortran programmers
C-------------------------------------------------------------------------

         integer function sfrchnk(id, start, num_data)
C
         integer id, start(*), num_data(*)
         integer scrchnk 
C
         sfrchnk = scrchnk(id, start, num_data) 
C
         return 
         end

           
C-------------------------------------------------------------------------
C        Name:      sfscchnk
C        Purpose:   set the maximum number of chunks to cache
C        Inputs:    id       - data set ID
C                   maxcache - max number of chunks to cache 
C                   flags    - flags =0, HDF_CACHEALL
C                              Currently only 0 can be passed.
C        Returns:   0 on success, -1 on failure
C        Calls:     scscchnk (C stub for SDsetchunkcache function)
C        Users:     HDF Fortran programmers
C-------------------------------------------------------------------------

         integer function sfscchnk(id, maxcache, flags)
C
         integer id, maxcache, flags 
         integer scscchnk 
C
         sfscchnk = scscchnk(id, maxcache, flags) 
C
         return 
         end

           

C-------------------------------------------------------------------------
C        Name:      sfschnk
C        Purpose:   make the SDS a chunked SDS
C        Inputs:    id       - data set ID
C                   dim_length - chunk dimensions
C                   comp_type - type of compression    
C                   supports the following compression types:
C                            ( see hcomp.h  file) 
C                            COMP_CODE_NONE = 0
C                            COMP_CODE_RLE = 1
C                            COMP_CODE_NBIT = 2 
C                            COMP_CODE_SKPHUFF = 3
C                            COMP_CODE_DEFLATE = 4 
C                            COMP_CODE_SZIP = 5
C                   comp_prm  - compression parameters array
C        For
C        Adaptive Huffman 
C                   comp_prm(1) = skphuff_skp_size
C        GZIP       
C                   comp_prm(1) = deflate_level
C        NBIT
C                   comp_prm(1) = nbit_sign_ext
C                   comp_prm(2) = nbit_fill_one
C                   comp_prm(3) = nbit_start_bit
C                   comp_prm(4) = nbit_bit_len 
C         
C        SZIP
C                   comp_prm(1) = options_mask
C                   comp_prm(2) = pixels_per_block 
C        NOTE: IT IS USER's responsibility to pass correct compression
C              parameters for each type of compression
C
C                       
C        Returns:   0 on success, -1 on failure
C        Calls:     scschnk (C stub for SDsetchunk function)
C-------------------------------------------------------------------------

         integer function sfschnk(id, dim_length, comp_type,comp_prm)

         integer id, dim_length(*), comp_type, comp_prm(*)
         integer scschnk 

         sfschnk = scschnk(id, dim_length, comp_type, comp_prm)
         return
         end

           
C-------------------------------------------------------------------------
C        Name:      sfwcchnk
C        Purpose:   write the specified chunk of CHARACTER data to the SDS 
C        Inputs:    id       - data set ID
C                   start    - chunk coordinates 
C                   char_data - buffer containing  data to be written  
C        Returns:   0 on success, -1 on failure
C        Calls:     scwcchnk (C stub for SDwritechunk function)
C        Users:     HDF Fortran programmers
C-------------------------------------------------------------------------

         integer function sfwcchnk(id, start, char_data)
C
         integer id, start(*) 
         character*(*) char_data(*)
         integer scwcchnk 
C
         sfwcchnk = scwcchnk(id, start, char_data) 
C
         return 
         end

           
C-------------------------------------------------------------------------
C        Name:      sfwchnk
C        Purpose:   write the specified chunk of NUMERIC data to the SDS 
C        Inputs:    id       - data set ID
C                   start    - chunk coordinates 
C                   num_data - buffer containing data to be written  
C        Returns:   0 on success, -1 on failure
C        Calls:     scwchnk (C stub for SDwritechunk function)
C        Users:     HDF Fortran programmers
C-------------------------------------------------------------------------

         integer function sfwchnk(id, start, num_data)
C
         integer id, start(*), num_data(*)
         integer scwchnk 
C
         sfwchnk = scwchnk(id, start, num_data) 
C
         return 
         end

           
C-------------------------------------------------------------------------
C        Name:      sfscompress
C        Purpose:   compress SDS 
C        Inputs:    id       - data set ID
C                   comp_type - type of compression
C                   supports the following compression types:
C                            ( see hcomp.h  file) 
C                            COMP_CODE_NONE = 0
C                            COMP_CODE_RLE =1
C                            COMP_CODE_SKPHUFF = 3
C                            COMP_CODE_DEFLATE = 4 
C                            COMP_CODE_SZIP =  5
C                   comp_prm - compression parameter array:
C                   comp_prm(1) = deflate_level for GZIP
C                   comp_prm(1) = skphuff_skp_size for ADAPTIVE HUFFMAN
C                   comp_prm(1) = options_mask for SZIP
C                   comp_prm(2) = bits_per_block for SZIP
C        NOTE: IT IS USER's responsibility to pass correct compression
C              parameters for each type of compression
C
C        Returns:   0 on success, -1 on failure
C        Calls:     scscompress (C stub for SDsetcompress function)
C-------------------------------------------------------------------------

         integer function sfscompress(id, comp_type,comp_prm)

         integer id, comp_type, comp_prm(*)
         integer scscompress 
         sfscompress = scscompress(id, comp_type, comp_prm)
         return
         end

C-------------------------------------------------------------------------
C        Name:      sfgcompress
C        Purpose:   get compression information about  SDS 
C        Inputs:    id       - data set ID
C        Output:    comp_type - type of compression
C                   supports the following compression types:
C                            ( see hcomp.h  file) 
C                            COMP_CODE_NONE = 0
C                            COMP_CODE_RLE =1
C                            COMP_CODE_SKPHUFF = 3
C                            COMP_CODE_DEFLATE = 4 
C                            COMP_CODE_SZIP =  5
C                   comp_prm - compression parameter array:
C                   comp_prm(1) = deflate_level for GZIP
C                   comp_prm(1) = skphuff_skp_size for ADAPTIVE HUFFMAN
C                   comp_prm(1) = options_mask for SZIP
C                   comp_prm(2) = bits_per_block for SZIP
C                   comp_prm(3) = pixels_per_scanline for SZIP
C                   comp_prm(4) = bits_per_pixel for SZIP
C                   comp_prm(5) = pixels for SZIP
C        NOTE: IT IS USER's responsibility to pass correct compression
C              parameters for each type of compression
C
C        Returns:   0 on success, -1 on failure
C        Calls:     scgcompress (C stub for SDsetcompress function)
C-------------------------------------------------------------------------

         integer function sfgcompress(id, comp_type,comp_prm)

         integer id, comp_type, comp_prm(*)
         integer scgcompress 
         sfgcompress = scgcompress(id, comp_type, comp_prm)
         return
         end


C-------------------------------------------------------------------------
C        Name:      sfchempty
C        Purpose:   checks whether an SDS is empty ( data has not been written) 
C        Inputs:    id       - data set ID
C        Outputs:   flag     - 1 if SDS is empty, 0 otherwise
C        Returns:   0 on success, -1 on failure
C        Calls:     scchempty (C stub for SDcheckempty function)
C        Users:     HDF Fortran programmers
C-------------------------------------------------------------------------

         integer function sfchempty(id, flag)
C
         integer id, flag 
         integer scchempty 
C
         sfchempty = scchempty(id, flag) 
C
         return 
         end
           
C----------------------------------------------------------------------
C     Name:     sfgetfname
C     Purpose:  Retrieves the name of the file given file identifier
C     Inputs:   file_id: file identifier
C     Input/Outputs: filename: character variable to hold 
C                    the name of the file
C     Returns:  the length of the name if successfull, -1 if fails
C     Calls:    scgetfname
C     Users:    Fortran stub routine
C----------------------------------------------------------------------

      integer function sfgetfname(file_id, filename)

      character*(*) filename
      integer       file_id
      integer       scgetfname

      sfgetfname = scgetfname(file_id, filename, len(filename))
      return
      end

C-------------------------------------------------------------------------
C        Name:      sfgetnamelen
C        Purpose:   Retrieves the length of the name of 
C                   a file, a dataset, or a dimension.
C        Inputs:    obj_id   - object identifier
C        Outputs:   length   - length of the object's name
C        Returns:   0 on success, -1 on failure
C        Calls:     scgetnamelen
C        Users:     Fortran stub routine
C-------------------------------------------------------------------------

      integer function sfgetnamelen(obj_id, length)
C
      integer obj_id, length
      integer scgetnamelen
C
      sfgetnamelen = scgetnamelen(obj_id, length) 
C
      return 
      end

C-------------------------------------------------------------------------
C        Name:      sfidtype
C        Purpose:   Returns the type of object
C        Inputs:    obj_id   - object identifier
C        Outputs:   obj_type - type of the object
C                       Can be one of the following
C                      -1 - if obj_id is not valid SD type
C                       0 - SD (file)
C                       1 - SDS (data set)
C                       2 - DIM_ID (dimension scale)
C                       see mfhdf.h file for definition of hdf_idtype_t
C        Returns:   0 on success, -1 on failure
C        Calls:     scidtype
C        Users:     Fortran stub routine
C-------------------------------------------------------------------------

      integer function sfidtype(obj_id, obj_type)
C
      integer obj_id, obj_type
      integer scidtype
C
      sfidtype = scidtype(obj_id, obj_type) 
C
      return 
      end
           
C----------------------------------------------------------------------
C     Name:     sfgnvars_byname
C     Purpose:  Gets the number of data sets having the same name
C     Inputs:   sd_id: SD interface identifier 
C               sds_name:  name of a data set
C     Output:   n_vars: number of data sets with name "name"
C     Returns:  0 on success, -1 on failure
C     Calls:    scgnvars_byname
C     Users:    Fortran stub routine
C----------------------------------------------------------------------

      integer function sfgnvars_byname(sd_id, sds_name, n_vars)

      character*(*) sds_name
      integer       sd_id, n_vars
      integer       scgnvars_byname

      sfgnvars_byname = scgnvars_byname(sd_id, sds_name, 
     +                  len(sds_name),
     +                  n_vars)
      return
      end

C----------------------------------------------------------------------
C     Name:     sfn2indices
C     Purpose:  Retrieves indices and types of all variables with 
C               the same name
C     Inputs:   sd_id: SD interface identifier 
C               sds_name:  name of a data set
C     Input/Output:   var_list: array to store indices 
C                     type_list: array to store variable type
C                                elements may have values 
C                                0 - for data set,
C                                1 - for dimension scale
C                                2 - unknown type
C     Returns:  0 on success, -1 on failure
C     Calls:    scn2indices
C     Users:    Fortran stub routine
C----------------------------------------------------------------------

      integer function sfn2indices(sd_id, sds_name, var_list, 
     +                            type_list, n_vars)

      character*(*) sds_name
      integer       sd_id
      integer       var_list(*), type_list(*)
      integer       n_vars
      integer       scn2indices

      sfn2indices = scn2indices(sd_id, sds_name, len(sds_name), 
     +                    var_list, type_list, n_vars)
      return
      end

C-------------------------------------------------------------------------
C        Name:      sfgmaxopenf
C        Purpose:   Retrieves current and maximum number of open files 
C        Outputs:   cur_max: current number of open files 
C                   sys_limit: maximum number of open files 
C        Returns:   0 on success, -1 on failure
C        Calls:     scgmaxopenf
C        Users:     Fortran stub routine
C-------------------------------------------------------------------------

      integer function sfgmaxopenf(cur_max, sys_limit)
C
      integer cur_max, sys_limit
      integer scgmaxopenf
C
      sfgmaxopenf = scgmaxopenf(cur_max, sys_limit) 
C
      return 
      end
           
C-------------------------------------------------------------------------
C        Name:      sfgnumopenf
C        Purpose:   Returns the number of files currently being opened
C        Outputs:   cur_num: current number of open files 
C        Returns:   0 on success, -1 on failure
C        Calls:     scgnumopenf
C        Users:     Fortran stub routine
C-------------------------------------------------------------------------

      integer function sfgnumopenf(cur_num)
C
      integer cur_num
      integer scgnumopenf
C
      sfgnumopenf = scgnumopenf(cur_num) 
C
      return 
      end
           
C-------------------------------------------------------------------------
C        Name:      sfrmaxopenf
C        Purpose:   Resets the maximum number of files can be opened 
C                   at the same time
C        Outputs:   req_max: requested maximum number of opened files allowed 
C        Returns:   0 on success, -1 on failure
C        Calls:     scrmaxopenf
C        Users:     Fortran stub routine
C-------------------------------------------------------------------------

      integer function sfrmaxopenf(req_max)
C
      integer req_max
      integer scrmaxopenf
C
      sfrmaxopenf = scrmaxopenf(req_max) 
C
      return 
      end
           
           
           
           

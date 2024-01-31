      program vdata_attributes 
      implicit none
C
C     Parameter declaration
C
      character*18 FILE_NAME
      character*14 VDATA_NAME
      character*4  FIELD_NAME
      character*10 VATTR_NAME
      character*6  FATTR_NAME
      integer      VATTR_N_VALUES, FATTR_N_VALUES 
C
      parameter (FILE_NAME    = 'General_Vdatas.hdf',
     +           VDATA_NAME   = 'Solid Particle',
     +           FIELD_NAME   = 'Mass',
     +           VATTR_NAME   = 'Site Ident',
     +           FATTR_NAME   = 'Scales')
      parameter (VATTR_N_VALUES = 3,
     +           FATTR_N_VALUES = 4)
   
      integer DFACC_WRITE, FULL_INTERLACE, HDF_VDATA
      integer DFNT_INT32, DFNT_CHAR8
      parameter (DFACC_WRITE    =  2,
     +           FULL_INTERLACE =  0,
     +           HDF_VDATA      = -1,
     +           DFNT_INT32     = 24,
     +           DFNT_CHAR8     =  4)
C
C     Function declaration
C
      integer hopen, hclose
      integer vfstart, vsffnd, vsfatch, vsfscat, vsfsnat, 
     +        vsffnas, vsffidx, vsfnats, vsfainf, vsfgcat, vsfgnat,
     +        vsfdtch, vfend 

C
C**** Variable declaration *******************************************
C
      integer   status
      integer   file_id, vdata_id, vdata_ref
      integer   field_index, n_vdattrs, n_fldattrs
      integer   vdata_type, vdata_n_values, vdata_size
      integer   field_type, field_n_values, field_size 
      character vd_attr(VATTR_N_VALUES)
      integer   fld_attr(FATTR_N_VALUES)
      character vattr_buf(VATTR_N_VALUES)
      integer   fattr_buf(FATTR_N_VALUES)
      character vattr_name_out(30), fattr_name_out(30)
      data vd_attr /'A', 'B', 'C'/
      data fld_attr /2, 4, 6, 8/
C
C**** End of variable declaration ************************************
C
C
C     Open the HDF file for writing.
C
      file_id = hopen(FILE_NAME, DFACC_WRITE, 0)
C
C     Initialize the VS interface.
C
      status = vfstart(file_id) 
C
C     Get the reference number of the vdata named VDATA_NAME.
C     
      vdata_ref = vsffnd(file_id, VDATA_NAME)
C
C     Attach to the vdata for writing. 
C
      vdata_id = vsfatch(file_id, vdata_ref, 'w') 
C
C     Attach an attribute to the vdata, as it is indicated by second parameter.
C
      status = vsfscat(vdata_id, HDF_VDATA, VATTR_NAME, DFNT_CHAR8,
     +                  VATTR_N_VALUES, vd_attr)
C
C     Get the index of the field FIELD_NAME within the vdata.
C
      status = vsffidx(vdata_id, FIELD_NAME, field_index)
C
C     Attach an attribute to the field with the index field_index.
C
      status = vsfsnat(vdata_id, field_index, FATTR_NAME, DFNT_INT32,
     +                  FATTR_N_VALUES, fld_attr)
      
C
C     Get the number of attributes attached to the vdata's first
C     field - should be 0.
C
      n_fldattrs = vsffnas(vdata_id, 0)
      write(*,*) 'Number of attributes of the first field' 
      write(*,*) ' of the vdata: ', n_fldattrs
C
C     Get the number of the attributes attached to the field specified by
C     index field_index - should be 1.
C
      n_fldattrs = vsffnas(vdata_id, field_index)
      write(*,*) 'Number of attributes of field ', FIELD_NAME,
     +           n_fldattrs
C
C     Get the total number of the field's and vdata's attributes - should be 2.
C
      n_vdattrs = vsfnats(vdata_id)
      write(*,*) 'Number of attributes of the vdata and its fields: ',
     +           n_vdattrs
C
C     Get information about the vdata's first attribute, indicated by 
C     the third parameter, which is the index of the attribute.
C
      status = vsfainf(vdata_id, HDF_VDATA, 0, vattr_name_out,
     +                 vdata_type, vdata_n_values, vdata_size)
C
C     Get information about the first attribute of the field specified by
C     field_index.
C
      status = vsfainf(vdata_id, field_index, 0, fattr_name_out, 
     +                 field_type, field_n_values, field_size)
C
C     Get the vdata's first attribute.
C
      status = vsfgcat(vdata_id, HDF_VDATA, 0, vattr_buf)
      write(*,*) 'Values of vdata attribute  ', vattr_buf
C
C     Get the first attribute of the field specified by field_index.
C
      status = vsfgnat(vdata_id, field_index, 0, fattr_buf)
      write(*,*)  'Values of the field attribute = ', fattr_buf
C       
C     Terminate access to the vdata and to the VS interface, and
C     close the HDF file.
C
      status = vsfdtch(vdata_id)
      status = vfend(file_id)
      status = hclose(file_id)
      end

      program annotation_info
      implicit none
C
C     Parameter declaration
C
      character*22 FILE_NAME
      character*9  VG_NAME
C
      parameter (FILE_NAME      = 'General_HDFobjects.hdf',
     +           VG_NAME        = 'AN Vgroup')
      integer    DFACC_READ
      parameter (DFACC_READ = 1)
      integer AN_FILE_LABEL, AN_DATA_LABEL, AN_DATA_DESC
      parameter (AN_FILE_LABEL = 2,
     +           AN_DATA_LABEL = 0,
     +           AN_DATA_DESC  = 1)
      integer DFTAG_DIA, DFTAG_FID, DFTAG_DIL
      parameter (DFTAG_DIA = 105,
     +           DFTAG_FID = 100,
     +           DFTAG_DIL = 104)
      integer DFTAG_VG
      parameter (DFTAG_VG = 1965)
C
C     Function declaration
C
      integer hopen, hclose
      integer afstart, afnumann, afannlist, afidtagref, aftagatype,
     +        afatypetag, afend
      integer vfstart, vfind 

C
C**** Variable declaration *******************************************
C
      integer status
      integer file_id, an_id
      integer n_annots, ann_index, annot_type, ann_tag, ann_ref
      integer ann_list(10) 
      integer vgroup_tag, vgroup_ref
C
C**** End of variable declaration ************************************
C
      annot_type = AN_DATA_DESC
      vgroup_tag = DFTAG_VG
C
C     Open the HDF file for reading.
C
      file_id = hopen(FILE_NAME, DFACC_READ, 0)
C
C     Initialize the V interface.
C
      status = vfstart(file_id)
C
C     Get the group named VG_NAME.
C
      vgroup_ref = vfind(file_id, VG_NAME)
C
C     Initialize the AN interface.
C
      an_id = afstart(file_id)

C
C     Get the number of object descriptions. 
C
      if (vgroup_ref .eq. -1) goto 100 
      n_annots = afnumann(an_id, annot_type, vgroup_tag, vgroup_ref)
C
C     Get the list of identifiers of the annotations attached to the
C     vgroup and of type annot_type. Identifiers are read into ann_list
C     buffer. One has to make sure that ann_list has the size big enough
C     to hold the list of identifiers.
C
      n_annots = afannlist(an_id, annot_type, vgroup_tag, vgroup_ref,
     +                     ann_list)
C
C     Get each annotation identifier from the list then display the
C     tag/ref number pair of the corresponding annotation.
C
      write(*,*) 'List of annotations of type AN_DATA_DESC'
      do 10 ann_index = 0, n_annots - 1
C
C     Get and display the ref number of the annotation from its
C     identifier.
C
      status = afidtagref(ann_list(ann_index+1), ann_tag, ann_ref) 
      write(*,*) 'Annotation index: ', ann_index
      if (ann_tag .eq. DFTAG_DIA) then
          write(*,*) 'tag = DFTAG_DIA (data description)'
      else
          write(*,*) ' tag = Incorrect'
      endif
      write(*,*) 'reference number = ', ann_ref
10    continue
C
C     Get and display an annotation type from an annotation tag.
C
      annot_type = aftagatype(DFTAG_FID)
      if (annot_type .eq. AN_FILE_LABEL) then
         write(*,*) 'Annotation type of DFTAG_FID (file label) is ',
     +               'AN_FILE_LABEL '
      else
         write(*,*) 'Annotation type of DFTAG_FID (file label) is ',
     +               'Incorrect'
      endif   
C
C     Get and display an annotation tag from an annotation type.
C
      ann_tag = afatypetag(AN_DATA_LABEL)
      if (ann_tag .eq. DFTAG_DIL ) then
         write(*,*) 'Annotation tag of AN_DATA_LABEL is ',
     +               'DFTAG_DIL (data label)'
      else
         write(*,*) 'Annotation type of DFTAG_FID (file label) is ',
     +               'Incorrect'
      endif   
C
C     Terminate access to the AN interface and close the HDF file.
C
100   continue
      status = afend(an_id)
      status = hclose(file_id)
      end

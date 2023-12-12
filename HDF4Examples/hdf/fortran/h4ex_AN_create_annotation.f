      program create_annotation
      implicit none
C
C     Parameter declaration
C
      character*22 FILE_NAME
      character*9  VG_NAME
      character*19 FILE_LABEL_TXT
      character*53 FILE_DESC_TXT
      character*16 DATA_LABEL_TXT
      character*54 DATA_DESC_TXT
C
      parameter (FILE_NAME      = 'General_HDFobjects.hdf',
     +           VG_NAME        = 'AN Vgroup',
     +           FILE_LABEL_TXT = 'General HDF objects',
     +           DATA_LABEL_TXT = 'Common AN Vgroup',
     +           FILE_DESC_TXT  = 
     + 'This is an HDF file that contains general HDF objects',
     +           DATA_DESC_TXT  = 
     + 'This is a vgroup that is used to test data annotations')
      integer DFACC_CREATE
      parameter (DFACC_CREATE = 4)
      integer AN_FILE_LABEL, AN_FILE_DESC, AN_DATA_LABEL, AN_DATA_DESC
      parameter (AN_FILE_LABEL = 2,
     +           AN_FILE_DESC  = 3,
     +           AN_DATA_LABEL = 0,
     +           AN_DATA_DESC  = 1)
C
C     Function declaration
C
      integer hopen, hclose
      integer afstart, affcreate, afwriteann, afcreate,
     +        afendaccess, afend
      integer vfstart, vfatch, vfsnam, vqref, vqtag, vfdtch, vfend

C
C**** Variable declaration ******************************************* 
C
      integer status
      integer file_id, an_id
      integer file_label_id, file_desc_id
      integer data_label_id, data_desc_id
      integer vgroup_id, vgroup_tag, vgroup_ref
C
C**** End of variable declaration ************************************
C
C
C     Create the HDF file.
C
      file_id = hopen(FILE_NAME, DFACC_CREATE, 0)
C
C     Initialize the AN interface.
C
      an_id = afstart(file_id)
C
C     Create the file label.
C
      file_label_id = affcreate(an_id, AN_FILE_LABEL)
C
C     Write the annotation to the file label.
C
      status = afwriteann(file_label_id, FILE_LABEL_TXT,
     +                    len(FILE_LABEL_TXT))       
C
C     Create file description.
C
      file_desc_id = affcreate(an_id, AN_FILE_DESC)
C
C     Write the annotation to the file description.
C
      status = afwriteann(file_desc_id, FILE_DESC_TXT,
     +                    len(FILE_DESC_TXT))
C
C     Create a vgroup in the file. Note that the vgroup's ref number is
C     set to -1 for creating and the access mode is 'w' for writing.
C
      status    = vfstart(file_id)
      vgroup_id = vfatch(file_id, -1, 'w')
      status    = vfsnam(vgroup_id, VG_NAME)      
C
C     Obtain the tag and reference number of the vgroup for subsequent
C     references.
C
      vgroup_ref = vqref(vgroup_id)
      vgroup_tag = vqtag(vgroup_id)
C
C     Create the data label for the vgroup identified by its tag and ref 
C     number.
C
      data_label_id = afcreate(an_id, vgroup_tag, vgroup_ref,
     +                          AN_DATA_LABEL)
C
C     Write the annotation text to the data label.
C
      status = afwriteann(data_label_id, DATA_LABEL_TXT, 
     +                    len(DATA_LABEL_TXT))
      
C
C     Create the data description for the vgroup identified by its tag and ref.
C
      data_desc_id = afcreate(an_id, vgroup_tag, vgroup_ref, 
     +                        AN_DATA_DESC)
C
C     Write the annotation text to the data description.
C
      status = afwriteann(data_desc_id, DATA_DESC_TXT,
     +                    len(DATA_DESC_TXT))       
C      
C     Terminate access to the vgroup and to the V interface.
C
      status = vfdtch(vgroup_id)
      status = vfend(file_id)
C
C     Terminate access to each annotation explicitly.
C
      status = afendaccess(file_label_id)
      status = afendaccess(file_desc_id)
      status = afendaccess(data_label_id)
      status = afendaccess(data_desc_id)
C
C     Terminate access to the AN interface and close the HDF file.
C
      status = afend(an_id)
      status = hclose(file_id)
      end 

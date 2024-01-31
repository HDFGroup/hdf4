      program  read_annotation
      implicit none
C
C     Parameter declaration
C
      character*22 FILE_NAME
C
      parameter (FILE_NAME = 'General_HDFobjects.hdf')
      integer    DFACC_READ
      parameter (DFACC_READ = 1)
      integer    AN_DATA_LABEL
      parameter (AN_DATA_LABEL = 0)
C
C     Function declaration
C
      integer hopen, hclose
      integer afstart, affileinfo, afselect, afannlen, afreadann,
     +        afendaccess, afend
C
C**** Variable declaration *******************************************
C
      integer status
      integer file_id, an_id, ann_id
      integer index, ann_length 
      integer n_file_labels, n_file_descs, n_data_labels, n_data_descs 
      character*256 ann_buf 
C
C**** End of variable declaration ************************************
C
C
C     Open the HDF file for reading.
C
      file_id = hopen(FILE_NAME, DFACC_READ, 0)
C
C     Initialize the AN interface.
C
      an_id = afstart(file_id)
C
C     Get the annotation information, i.e., the number of file labels,
C     file descriptions, data labels, and data descriptions.
C
      status = affileinfo(an_id, n_file_labels, n_file_descs,
     +                    n_data_labels, n_data_descs)
C
C     Get the data labels. Note that this DO loop can be used to obtain 
C     the contents of each kind of annotation with the appropriate number
C     of annotations and the type of annotation, i.e., replace
C     n_data_labels with n_file_labels, n_files_descs, or n_data_descs, and
C     AN_DATA_LABEL with AN_FILE_LABEL, AN_FILE_DESC, or AN_DATA_DESC, 
C     respectively.
C
      do 10 index = 0, n_data_labels-1
C
C     Get the identifier of the current data label.
C
      ann_id = afselect(an_id, index, AN_DATA_LABEL)
C
C     Get the length of the data label.
C
      ann_length = afannlen(ann_id)
C
C     Read and display the data label. The data label is read into buffer
C     ann_buf. One has to make sure that ann_buf has sufficient size to hold
C     the data label. Also note, that the third argument to afreadann is 
C     1 greater that the actual length of the data label (see comment to
C     C example).
C
      status = afreadann(ann_id, ann_buf, ann_length+1) 
      write(*,*) 'Data label index: ', index
      write(*,*) 'Data label contents: ', ann_buf(1:ann_length) 
10    continue
C
C     Terminate access to the current data label.
C
      status = afendaccess(ann_id)  
C
C     Terminate access to the AN interface and close the HDF file.
C
      status = afend(an_id)
      status = hclose(file_id)
      end


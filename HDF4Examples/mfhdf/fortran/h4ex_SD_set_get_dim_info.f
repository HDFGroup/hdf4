      program  dimension_info 
      implicit none
C
C     Parameter declaration.
C
      character*7  FILE_NAME
      character*11 SDS_NAME
      character*6  DIM_NAME_X
      character*6  DIM_NAME_Y
      integer      X_LENGTH, Y_LENGTH, RANK
      parameter   (FILE_NAME  = 'SDS.hdf',
     +             SDS_NAME   = 'SDStemplate',
     +             DIM_NAME_X  = 'X_Axis',
     +             DIM_NAME_Y  = 'Y_Axis',
     +             X_LENGTH = 5,
     +             Y_LENGTH = 16,
     +             RANK     = 2)
      integer      DFACC_WRITE, DFNT_INT16, DFNT_FLOAT64
      parameter   (DFACC_WRITE   = 2,
     +             DFNT_INT16   = 22,
     +             DFNT_FLOAT64 = 6)

C
C     Function declaration.
C
      integer sfstart, sfn2index, sfdimid, sfgdinfo
      integer sfsdscale, sfgdscale, sfsdmname, sfendacc
      integer sfend, sfselect
C
C**** Variable declaration *******************************************
C
      integer sd_id, sds_id, sds_index, status
      integer dim_index, dim_id
      integer n_values, n_attrs, data_type
      integer*2 data_X(X_LENGTH)
      integer*2 data_X_out(X_LENGTH)
      real*8    data_Y(Y_LENGTH)
      real*8    data_Y_out(Y_LENGTH)
      character*6 dim_name
      integer   i
C
C**** End of variable declaration ************************************
C
C
C     Initialize dimension scales.
C
      do 10 i = 1, X_LENGTH  
         data_X(i) = i - 1
10     continue

      do 20 i = 1, Y_LENGTH  
         data_Y(i) = 0.1 * (i - 1)
20     continue
C
C     Open the file and initialize SD interface.
C 
      sd_id = sfstart(FILE_NAME, DFACC_WRITE)
C
C     Get the index of the data set with the name specified in SDS_NAME. 
C
      sds_index = sfn2index(sd_id, SDS_NAME)
C
C     Select the data set corresponding to the returned index.
C
      sds_id = sfselect(sd_id, sds_index)
C
C     For each dimension of the data set,
C     get its dimension identifier and set dimension name
C     and dimension scales. Note that data type of dimension scale can
C     be different between dimensions and can be different from SDS data type.
C
      do 30 dim_index = 0, RANK - 1 
C
C        Select the dimension at position dim_index.
C
         dim_id = sfdimid(sds_id, dim_index)
C
C        Assign name and dimension scale to the dimension. 
C
         if (dim_index .eq. 0) then
            status = sfsdmname(dim_id, DIM_NAME_X) 
            n_values = X_LENGTH
            status = sfsdscale(dim_id, n_values, DFNT_INT16, data_X)
         end if
         if (dim_index .eq. 1) then
            status = sfsdmname(dim_id, DIM_NAME_Y)
            n_values = Y_LENGTH
            status = sfsdscale(dim_id, n_values, DFNT_FLOAT64, data_Y)
         end if
C
C      Get and display information about dimension and its scale values.
C      The following information is displayed:
C
C                    Information about 1 dimension :
C                    dimension name is X_Axis
C                    number of scale values is  5
C                    dimension scale data type is int16
C
C                    number of dimension attributes is   0
C                    Scale values are:
C                        0  1  2  3  4
C
C                    Information about 2 dimension :
C                    dimension name is Y_Axis
C                    number of scale values is  16
C                    dimension scale data type is float64
C                    number of dimension attributes is   0
C
C                    Scale values are:
C                        0.000      0.100      0.200      0.300
C                        0.400      0.500      0.600      0.700
C                        0.800      0.900      1.000      1.100
C                        1.200      1.300      1.400      1.500
C
       status = sfgdinfo(dim_id, dim_name, n_values, data_type, n_attrs)
C
       write(*,*) "Information about ", dim_index+1," dimension :"
       write(*,*) "dimension name is ", dim_name
       write(*,*) "number of scale values is", n_values
       if (data_type. eq. 22) then
           write(*,*) "dimension scale data type is int16"
       endif 
       if (data_type. eq. 6) then
           write(*,*) "dimension scale data type is float64"
       endif 
       write(*,*) "number of dimension attributes is ", n_attrs
C
       write(*,*) "Scale values are:"
       if (dim_index .eq. 0) then
          status = sfgdscale(dim_id, data_X_out)
          write(*,*) (data_X_out(i), i= 1, X_LENGTH)
       endif 
       if (dim_index .eq. 1)  then
          status = sfgdscale(dim_id, data_Y_out) 
          write(*,100) (data_Y_out(i), i= 1, Y_LENGTH)
100       format(4(1x,f10.3)/)
       endif 
30      continue
C
C     Terminate access to the data set.
C
      status = sfendacc(sds_id)
C
C     Terminate access to the SD interface and close the file.
C
      status = sfend(sd_id)
      end

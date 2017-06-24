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
         program test_szip
         implicit none
C
C------- Begin Variables declarations -----------------------------------
C
         integer   N_COMP_ARG, N_TYPES
         parameter (N_COMP_ARG = 5, N_TYPES = 4)
         integer   sd_id, sds_id(N_TYPES), sds_index(N_TYPES)
         integer   type(N_TYPES)
         integer   RANK, comp_type, comp_type_out
         integer   comp_arg(N_COMP_ARG), comp_prm_out(5)
         integer   d_dims(2)
         integer   start(2), stride(2), edges(2)
         integer   status
         integer   err_szip
         character*14 file
         character*17 name(4)
         integer   n, m, n_part, m_part
         integer   n_part_stride, m_part_stride
         integer   n_part_start, m_part_start
         integer   n_stride, m_stride
         integer   n_start, m_start
         integer   n_edges, m_edges
         integer   i, j, l, kl, kj, i_comp
C
C  SDS functions declarations
         include 'mffunc.inc'
         external hconfinf
         integer  hconfinf
C
C  Initial data declarations( change if you which to test larger arrays )
C
C  Data array dimensions n and m and RANK
C
         parameter (n = 12, m = 4, RANK = 2)

C
C  Part data dimensions n_part, m_part
C
         parameter (n_part = 5, m_part = 2)
C
C  Stride and start coordinates of part_data
C
         parameter (n_part_stride = 1, m_part_stride = 1)
         parameter (n_part_start = 2, m_part_start = 1)
C
C  Sart, stride  and edges parameters in each dimension
C
         parameter (n_start = 0, m_start = 0)
         parameter (n_stride = 1, m_stride = 1)
         parameter (n_edges = n, m_edges = m)
C
C  Data declaration
C
         integer*2 data2(n,m), data2_out(n_part,m_part)
         integer*4 data4(n,m), data4_out(n_part,m_part)
         real*4    rdata4(n,m), rdata4_out(n_part,m_part)
         real*8    rdata8(n,m), rdata8_out(n_part,m_part)
C
C  HDF parameters initialization
C
C
C  Read/Write parameters
C
         integer   DFACC_CREATE,
     .             DFACC_WRITE,
     .             DFACC_READ
         parameter (DFACC_CREATE = 4,
     .             DFACC_READ   =  1,
     .             DFACC_WRITE   = 2)
C
C  Data type parameters
C
         integer    DFNT_INT16,
     .              DFNT_INT32,
     .              DFNT_FLOAT32,
     .              DFNT_FLOAT64
         parameter (DFNT_INT16  = 22,
     .              DFNT_INT32   = 24,
     .              DFNT_FLOAT32 = 5,
     .              DFNT_FLOAT64 = 6)
C
C  Compression parametes
C
         integer   COMP_CODE_SZIP,
     .             SZ_EC_OPTION_MASK,
     .             SZ_NN_OPTION_MASK

         parameter (COMP_CODE_SZIP   = 5,
     .              SZ_EC_OPTION_MASK = 4,
     .              SZ_NN_OPTION_MASK = 32)
C
C  Compression arguments ( defined for clarity, actual values
C  will be passed to SFSCHUNK function via comp_arg parameter)
C
         integer pixels_per_block 
         parameter (pixels_per_block = 12)

C--------------------End of declarations------------------------------
C
C-------------------IS SZIP compression present with encoder?--------
         integer info
         err_szip = 0

         comp_type = COMP_CODE_SZIP
         status = hconfinf(comp_type, info)    
         if(status .LT. 0) then
            err_szip = err_szip + 1         
            goto 1111
         endif 
         if(info .EQ. 0 ) then
          print *,'Skipping test_szip: compression is not available'
          print *,' '
          goto 3333
         endif

       if(info .EQ. 1) then
         print *,'Skipping test_szip: szip encoding is not available'
          print *,' '
         goto 3333
       endif
            
C
C
C  We will write to five different files corresponding to the 
C  different compression types
C
C  NO compression
C 
         file = 'comp_szip.hdf'
         name(1) = 'szip_data_int16'
         name(2) = 'szip_data_int32'
         name(3) = 'szip_data_float32'
         name(4) = 'szip_data_float64'
         type(1) = DFNT_INT16
         type(2) = DFNT_INT32
         type(3) = DFNT_FLOAT32
         type(4) = DFNT_FLOAT64
C
C   Dimension sizes array initialization
C
         d_dims(1) = n
         d_dims(2) = m
C        
C   Initilize original array
C
         
         do 20 j = 1, m 
            do 10 i = 1, n
               data2(i,j) = 10*j + i
               data4(i,j) = 10*j + i
               rdata4(i,j) = 10*j + i
               rdata8(i,j) = 10*j + i
10          continue
20       continue
C
C  Initialize SD interfaces
C
        sd_id = sfstart (file, DFACC_CREATE)
C
C  Set compression type and compression arguments
C
C
C   Initialize compression arguments array
C
         comp_type   = COMP_CODE_SZIP
         comp_arg(1) = SZ_NN_OPTION_MASK
         comp_arg(2) = pixels_per_block
C
C  Main loop through different compression types
C
         do 1000 i_comp = 1, N_TYPES

C
C        Create the data set
C
         sds_id(i_comp) = sfcreate(sd_id, name(i_comp), 
     .                     type(i_comp), RANK, d_dims)
            if( sds_id(i_comp) .eq. -1 ) then
                print *, 'sfcreate failed for', i_comp, ' -th dataset'
                err_szip = err_szip + 1
            endif
         status = sfscompress(sds_id(i_comp), comp_type, comp_arg)
          if( status .ne. 0 ) then
            print *, 'sfscompress failed for', i_comp, ' -th dataset'
            err_szip = err_szip + 1
          endif
C
C   Write data to the file
C
                start(1) = n_start
                start(2) = m_start
                stride(1) = n_stride
                stride(2) = m_stride
                edges(1)  = n_edges
                edges(2)  = m_edges
                if (i_comp .eq. 1) then
                status = sfwdata(sds_id(i_comp), start, stride,
     .                           edges, data2)
                endif
                if (i_comp .eq. 2) then
                status = sfwdata(sds_id(i_comp), start, stride,
     .                           edges, data4)
                endif
                if (i_comp .eq. 3) then
                status = sfwdata(sds_id(i_comp), start, stride,
     .                           edges, rdata4)
                endif
                if (i_comp .eq. 4) then
                status = sfwdata(sds_id(i_comp), start, stride,
     .                           edges, rdata8)
                endif
            if( status .ne. 0 ) then
                print *, 'sfwdata failed for', i_comp, ' -th dataset'
                err_szip = err_szip + 1
            endif
         
         status = sfendacc(sds_id(i_comp))
            if( status .ne. 0 ) then
                print *, 'sfendacc failed for', i_comp, ' -th dataset'
                err_szip = err_szip + 1
            endif
1000      continue 
         status = sfend (sd_id)
            if( status .ne. 0 ) then
                print *, 'sfend failed'
                err_szip = err_szip + 1
            endif

C
C   Let's check what we have written 
C
C   Open files and restart SD interfaces
C
         sd_id = sfstart (file, DFACC_READ)
            if( sd_id .eq. -1 ) then
                print *, 'sfstart failed'
                err_szip = err_szip + 1
            endif
         do 2000 i = 1, N_TYPES
        
C
C  Find written dataset in each file using its name and index
C
            sds_index(i) = sfn2index (sd_id, name(i))
            if( sds_index(i) .eq. -1 ) then
                print *, 'sfn2index failed for', i, ' -th dataset'
                err_szip = err_szip + 1
            endif
            sds_id(i)    = sfselect (sd_id, sds_index(i))
            if( sds_id(i) .eq. -1 ) then
                print *, 'sfselect failed for', i, ' -th dataset'
                err_szip = err_szip + 1
            endif
C
C  Find out type of compression used and compression parameters.
C
            status = sfgcompress(sds_id(i), comp_type_out, comp_prm_out)
	    if (status .eq. -1) then
            print *, 'sfgcompress failed for', i, ' -th dataset'
                err_szip = err_szip + 1
            endif
                if (comp_type_out .ne. COMP_CODE_SZIP) then
            print *, 'wrong compression type for szip 
     .                compressed dataset'
                err_szip = err_szip + 1
                endif
C            write(*,*) comp_arg(1), comp_prm_out(1)
C            write(*,*) comp_arg(2), comp_prm_out(2)
C            write(*,*) comp_prm_out(3)
C            write(*,*) comp_prm_out(4)
C            write(*,*) comp_prm_out(5)
            
C                if ((comp_arg(1) .ne. comp_prm_out(1)) .or. 
C     .              (comp_arg(2) .ne. comp_prm_out(2))) then
C            print *, 'wrong compression parameter'
C                err_szip = err_szip + 1
C                endif

C
C   Read part of the data back using sfrdata function
C
         start(1) = n_part_start
         start(2) = m_part_start
         edges(1) = n_part
         edges(2) = m_part
         stride(1) = n_part_stride 
         stride(2) = m_part_stride 
                if (i .eq. 1) then
                status = sfrdata(sds_id(i), start, stride,
     .                           edges, data2_out)
                endif
                if (i .eq. 2) then
                status = sfrdata(sds_id(i), start, stride,
     .                           edges, data4_out)
                endif
                if (i .eq. 3) then
                status = sfrdata(sds_id(i), start, stride,
     .                           edges, rdata4_out)
                endif
                if (i .eq. 4) then
                status = sfrdata(sds_id(i), start, stride,
     .                           edges, rdata8_out)
                endif
             if (status .ne. 0) then
                 print *, 'sfrdata failed for reading part data for ',
     .           i, '-th dataset'
             err_szip = err_szip + 1
             endif
C
C   Compare output with actual data
C
         if (i .eq. 1) then
         do 204 j = 1, m_part
            do 203 l = 1, n_part
               kl = n_part_start + 1 + (l-1)*n_part_stride
               kj = m_part_start + 1 + (j-1)*m_part_stride
               if (data2(kl, kj) .ne. data2_out(l,j)) then
                  print *, 'sfrdata read wrong data for ', 
     .            i ,'-th dataset'
               err_szip = err_szip +1
               endif 
203         continue
204      continue
         endif
         if (i .eq. 2) then
         do 206 j = 1, m_part
            do 205 l = 1, n_part
               kl = n_part_start + 1 + (l-1)*n_part_stride
               kj = m_part_start + 1 + (j-1)*m_part_stride
               if (data4(kl, kj) .ne. data4_out(l,j)) then
                  print *, 'sfrdata read wrong data for ', 
     .            i ,'-th dataset'
               err_szip = err_szip +1
               endif 
205         continue
206      continue
         endif
         if (i .eq. 3) then
         do 208 j = 1, m_part
            do 207 l = 1, n_part
               kl = n_part_start + 1 + (l-1)*n_part_stride
               kj = m_part_start + 1 + (j-1)*m_part_stride
               if (rdata4(kl, kj) .ne. rdata4_out(l,j)) then
                  print *, 'sfrdata read wrong data for ', 
     .            i ,'-th dataset'
               err_szip = err_szip +1
               endif 
207         continue
208      continue
         endif
         if (i .eq. 4) then
         do 210 j = 1, m_part
            do 209 l = 1, n_part
               kl = n_part_start + 1 + (l-1)*n_part_stride
               kj = m_part_start + 1 + (j-1)*m_part_stride
               if (rdata8(kl, kj) .ne. rdata8_out(l,j)) then
                  print *, 'sfrdata read wrong data for ', 
     .            i ,'-th dataset'
               err_szip = err_szip +1
               endif 
209         continue
210      continue
         endif
         
C 
C  Terminate access to SDS, shutdown interfaces and close the files
C
           status = sfendacc(sds_id(i))
            if( status .ne. 0 ) then
                print *, 'sfendacc failed for', i, '-th dataset'
                err_szip = err_szip + 1
            endif
2000   continue
           status = sfend(sd_id)
            if( status .ne. 0 ) then
                print *, 'sfend failed'
                err_szip = err_szip + 1
            endif
1111     continue
         if (err_szip .ne. 0) then
            print *, 'test_szip failed with ', err_szip, ' errors'
            print *,' '
         endif
         if (err_szip .eq. 0) then
            print *, 'test_szip.................. PASSED'
            print *,' '
         endif
3333     continue
         end


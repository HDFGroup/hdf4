      program  vgroup_contents
      implicit none
C
C     Parameter declaration
C
      character*19 FILE_NAME
C
      parameter (FILE_NAME = 'General_Vgroups.hdf')
      integer DFACC_ READ
      parameter (DFACC_READ = 1)
C
C     Function declaration
C
      integer hopen, hclose
      integer vfstart, vfatch, vfgid, vntrc, vfgttr, vfisvg,
     +        vfisvs, vfdtch, vfend

C
C**** Variable declaration *******************************************
C
      integer status
      integer file_id
      integer vgroup_id, vgroup_ref,  vgroup_pos
      integer obj_index, num_of_pairs 
      integer obj_tag, obj_ref 
C
C**** End of variable declaration ************************************
C
C
C     Open the HDF file for reading.
C
      file_id = hopen(FILE_NAME, DFACC_READ, 0)
C
C     Initialize the V interface.
C
      status = vfstart(file_id)
C
C     Obtain each vgroup in the file by its reference number, get the
C     number of objects in the vgroup, and display the information
C     about that vgroup.
C
      vgroup_ref = -1
      vgroup_pos = 0
10    continue
C
C     Get the reference number of the next vgroup in the file.
C
      vgroup_ref = vfgid(file_id, vgroup_ref)
C
C     Attach to the vgroup or go to the end if no additional vgroup is found.
C
      if(vgroup_ref. eq. -1) goto 100
      vgroup_id = vfatch(file_id, vgroup_ref , 'r')
C
C     Get the total number of objects in the vgroup.
C
      num_of_pairs = vntrc(vgroup_id)
C
C     If the vgroup contains any object, print the tag/ref number
C     pair of each object in vgroup, in the order they appear in the
C     file, and indicate whether the object is a vdata, vgroup, or neither.
C
      if (num_of_pairs .gt. 0) then
         write(*,*) 'Vgroup # ', vgroup_pos, ' contains:'
         do 20 obj_index = 1, num_of_pairs
C
C     Get the tag/ref number pair of the object specified by its index 
C     and display them.
C
         status = vfgttr(vgroup_id, obj_index-1, obj_tag, obj_ref)
C
C     State whether the HDF object referred to by obj_ref is a vdata,
C     a vgroup, or neither.
C
         if( vfisvg(vgroup_id, obj_ref) .eq. 1) then
             write(*,*) 'tag = ', obj_tag, ' ref = ', obj_ref,
     +       '  <--- is a vgroup '
         else if ( vfisvs(vgroup_id, obj_ref) .eq. 1) then
             write(*,*) 'tag = ', obj_tag, ' ref = ', obj_ref,
     +       '  <--- is a vdata '
         else
             write(*,*) 'tag = ', obj_tag, ' ref = ', obj_ref,
     +       '  <--- neither vdata nor vgroup '
         endif
20       continue
      else
         write (*,*) 'Vgroup #', vgroup_pos, ' contains no HDF objects'
      endif
      write(*,*)
      vgroup_pos = vgroup_pos + 1
      goto 10 
100   continue      
C
C     Terminate access to the vgroup.
C
      status = vfdtch(vgroup_id)
C
C     Terminate access to the V interface and close the HDF file.
C
      status = vfend(file_id)
      status = hclose(file_id)
      end

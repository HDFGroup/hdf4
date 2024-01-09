      program  getinfo_about_vgroup
      implicit none
C
C     Parameter declaration
C
      character*19 FILE_NAME
C
      parameter (FILE_NAME = 'General_Vgroups.hdf')
      integer DFACC_READ
      parameter (DFACC_READ = 1)
      integer SIZE
      parameter(SIZE = 10)
C
C     Function declaration
C
      integer hopen, hclose
      integer vfstart, vfatch, vfgnam, vfgcls, vflone, vfdtch, vfend

C
C**** Variable declaration *******************************************
C
      integer status
      integer file_id
      integer vgroup_id
      integer lone_vg_number, num_of_lones
      character*64 vgroup_name, vgroup_class
      integer ref_array(SIZE)
      integer i
C
C**** End of variable declaration ************************************
C
C
C     Initialize ref_array.
C
      do 10 i = 1, SIZE
         ref_array(i) = 0
10    continue
C
C     Open the HDF file for reading.
C
      file_id = hopen(FILE_NAME, DFACC_READ, 0)
C
C     Initialize the V interface.
C
      status = vfstart(file_id)
C
C     Get and print the name and class name of all lone vgroups.
C     First, call vflone with num_of_lones set to 0 to get the number of
C     lone vgroups in the file and check whether size of ref_array is 
C     big enough to hold reference numbers of ALL lone groups.
C     If ref_array is not big enough, exit the program after displaying an
C     informative message.
C
      num_of_lones = 0
      num_of_lones = vflone(file_id, ref_array, num_of_lones)
      if (num_of_lones .gt. SIZE) then
      write(*,*) num_of_lones, 'lone vgroups is found'
      write(*,*) 'increase the size of ref_array to hold reference '
      write(*,*) 'numbers of all lone vgroups in the file'
      stop
      endif
C
C     If there are any lone groups in the file,
C
      if (num_of_lones .gt. 0) then
C
C     call vflone again to retrieve the reference numbers into ref_array.
C
      num_of_lones = vflone(file_id, ref_array, num_of_lones)
C
C     Display the name and class of each vgroup.
C
      write(*,*) 'Lone vgroups in the file are:'

      do 20 lone_vg_number = 1, num_of_lones
C
C     Attach to the current vgroup, then get and display its name and class.
C     Note: the current vgroup must be detached before moving to the next.  
C
      vgroup_name = ' '
      vgroup_class = ' '
      vgroup_id = vfatch(file_id, ref_array(lone_vg_number), 'r')
      status    = vfgnam(vgroup_id, vgroup_name)
      status    = vfgcls(vgroup_id, vgroup_class)
      write(*,*) 'Vgroup name ' ,  vgroup_name
      write(*,*) 'Vgroup class ' , vgroup_class
      write(*,*)
      status = vfdtch(vgroup_id)
20    continue

      endif
C
C     Terminate access to the V interface and close the HDF file.
C
      status = vfend(file_id)
      status = hclose(file_id)
      end

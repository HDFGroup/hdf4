      program  read_palette
      implicit none
C
C     Parameter declaration
C
      character*22 FILE_NAME
      character*18 IMAGE_NAME
      integer      N_ENTRIES
      integer      N_COMPS_PAL
C
      parameter (FILE_NAME   = 'Image_with_Palette.hdf',
     +           IMAGE_NAME  = 'Image with Palette',
     +           N_COMPS_PAL = 3,
     +           N_ENTRIES   = 256)
      integer DFACC_READ, DFNT_CHAR8, DFNT_UINT8, MFGR_INTERLACE_PIXEL
      parameter (DFACC_READ  = 1,
     +           DFNT_CHAR8  = 4,
     +           DFNT_UINT8  = 21,
     +           MFGR_INTERLACE_PIXEL = 0)
C
C     Function declaration
C
      integer hopen, hclose
      integer mgstart, mgn2ndx, mgselct, mggltid, mgglinf, 
     +        mgrclut, mgendac, mgend 
C
C**** Variable declaration *******************************************
C
      integer    file_id, gr_id, ri_id, ri_index, pal_id, pal_index
      integer    interlace_mode
      integer    data_type, n_comps, n_entries_out
      integer    status
      integer    i, j
      character  palette_data(N_COMPS_PAL, N_ENTRIES)
C
C**** End of variable declaration ************************************
C
C
C     Open the file.
C
      file_id = hopen(FILE_NAME, DFACC_READ, 0)
C
C     Initialize the GR interface.
C
      gr_id = mgstart(file_id)
C
C     Get the index of the image IMAGE_NAME.
C
      ri_index = mgn2ndx(gr_id, IMAGE_NAME)
C
C     Get the image identifier.
C
      ri_id = mgselct(gr_id, 0)
C
C     Get the identifier of the palette attached to the image.
C
      pal_index = 0
      pal_id = mggltid(ri_id, pal_index)
C
C     Obtain information about the palette.
C
      status = mgglinf(pal_id, n_comps, data_type, interlace_mode,
     +                 n_entries_out)
      write(*,*) ' Palette: ', n_comps, ' components;  ', 
     +           n_entries_out, ' entries'
C
C     Read the palette.
C
      status = mgrclut(pal_id, palette_data)
C
C     Display the palette data.
C
      write(*,*) "Palette data"
      do 10 i = 1, n_entries_out
         write(*,*) (ichar(palette_data(j,i)), j = 1, n_comps)
10    continue  
C
C     Terminate access to the raster image and to the GR interface,
C     and close the HDF file.
C
      status = mgendac(ri_id)
      status = mgend(gr_id)
      status = hclose(file_id)
      end

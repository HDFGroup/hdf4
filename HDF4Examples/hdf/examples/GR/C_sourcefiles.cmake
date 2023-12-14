#-----------------------------------------------------------------------------
# Define Sources, one file per application
#-----------------------------------------------------------------------------
set (h4examples
    h4ex_GR_create_and_write_chunked_image.c
    h4ex_GR_create_and_write_image.c
    h4ex_GR_write_palette.c
)
set (h4examples_use
    h4ex_GR_get_attribute.c
    h4ex_GR_image_info.c
    h4ex_GR_modify_image.c
    h4ex_GR_read_image.c
    h4ex_GR_read_palette.c
    h4ex_GR_set_attribute.c
)
set (h4examples_files
    General_RImages.hdf
    General_RImages2.hdf
    General_RImages3.hdf
    General_RImages4.hdf
    Image_with_Palette.hdf
)

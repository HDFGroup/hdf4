#-----------------------------------------------------------------------------
# Define Sources, one file per application
#-----------------------------------------------------------------------------
set (h4examples
    h4ex_VD_create_onefield_vdatas.c
    h4ex_VD_create_vdatas.c
    h4ex_VD_write_mixed_vdata_struct.c
    h4ex_VD_write_mixed_vdata.c
)
set (h4examples_use
    h4ex_VD_get_vdata_info.c
    h4ex_VD_locate_vdata.c
#    h4ex_VD_read_from_vdata.c #program will fail
    h4ex_VD_read_mixed_vdata.c
#    h4ex_VD_set_get_vdata_attr.c
    h4ex_VD_write_to_vdata.c
)
set (h4examples_files
    General_Vdatas.hdf
    General_Vdatas2.hdf
    General_Vdatas3.hdf
    Packed_Vdata.hdf
    Two_Vdatas.hdf
)

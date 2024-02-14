#-----------------------------------------------------------------------------
# Define Sources, one file per application
#-----------------------------------------------------------------------------
set (h4examples
    VD_create_onefield_vdatas
    VD_create_vdatas
    VD_write_mixed_vdata_struct
    VD_write_mixed_vdata
)
set (h4examples_use
    VD_get_vdata_info
    VD_locate_vdata
#    VD_read_from_vdata #program will fail
    VD_read_mixed_vdata
#    VD_set_get_vdata_attr
    VD_write_to_vdata
)
set (h4examples_files
    General_Vdatas.hdf
    General_Vdatas2.hdf
    General_Vdatas3.hdf
    Packed_Vdata.hdf
    Two_Vdatas.hdf
)

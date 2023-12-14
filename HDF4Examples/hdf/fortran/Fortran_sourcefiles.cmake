#-----------------------------------------------------------------------------
# Define Sources, one file per application
#-----------------------------------------------------------------------------
set (examples
    h4ex_VD_create_vdatas.f
    h4ex_VD_write_mixed_vdata.f
    h4ex_VD_write_to_vdata.f
    h4ex_VD_create_onefield_vdatas.f
    h4ex_VD_read_from_vdata.f
    h4ex_VD_read_mixed_vdata.f
    h4ex_VD_get_vdata_info.f
    h4ex_VD_locate_vdata.f
    h4ex_VG_create_vgroup.f
#    h4ex_VG_add_sds_to_vgroup.f
    h4ex_VG_insert_vdatas_to_vgroup.f
    h4ex_VG_set_get_vgroup_attr.f
    h4ex_VG_vgroup_contents.f
    h4ex_VG_get_vgroup_info.f
    h4ex_GR_create_and_write_image.f
    h4ex_GR_create_and_write_chunked_image.f
    h4ex_GR_modify_image.f
    h4ex_GR_set_attribute.f
    h4ex_GR_get_attribute.f
    h4ex_GR_write_palette.f
    h4ex_GR_image_info.f
    h4ex_GR_read_image.f
    h4ex_AN_create_annotation.f
    h4ex_AN_get_annotation_info.f
    h4ex_AN_read_annotation.f
    #h4ex_VD_set_get_vdata_attr.f
)
set (mf_examples
    h4ex_VG_add_sds_to_vgroup.f
)
set (skip_examples
    h4ex_VD_set_get_vdata_attr.f
)

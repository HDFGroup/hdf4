#-----------------------------------------------------------------------------
# Define Sources, one file per application
#-----------------------------------------------------------------------------
set (examples
    h4ex_SD_create_sds.f
    h4ex_SD_write_to_sds.f
    h4ex_SD_write_slab.f
    h4ex_SD_alter_sds_values.f
    h4ex_SD_unlimited_sds.f
    h4ex_SD_mv_sds_to_external.f
    h4ex_SD_read_from_sds.f
    h4ex_SD_read_subsets.f
    #h4ex_SD_get_info.f
    h4ex_SD_find_sds_by_name.f
    h4ex_SD_set_get_dim_info.f
    #h4ex_SD_dimscale_vs_sds.f
    h4ex_SD_set_attr.f
    #h4ex_SD_get_attr.f
    h4ex_SD_compress_sds.f
    h4ex_SD_chunking_example.f
)
set (skip_examples
    h4ex_SD_get_info.f
    h4ex_SD_dimscale_vs_sds.f
    h4ex_SD_get_attr.f
)

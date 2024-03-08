# HDF 4.2 to 4.3 Migration Guide

For the past few decades, HDF4 has deployed almost all of the library's header
files, both public and private. This deployment scheme makes even trivial
refactoring and bug fixing difficult to accomplish without breaking binary
compatibility when "private" headers change.

HDF 4.3.0 will finally separate the public and private APIs in both the single-
and multi-file libraries, allowing us to begin the process of cleaning up
several decades of accumulated technical debt.

This document lists the changes to what we deploy and how that may impact
code that uses HDF4. There is also a list of removed symbols, organized by
header file in the appendices.

Note that this is not a complete guide to changes in 4.3.0. It's only intended
to help people navigate the public/private API split. Please see the release
notes for a comprehensive list of other changes, e.g., build system changes.

## Changes in HDF 4.3.0

* Private header files are no longer distributed. A complete list can be found below.
* The error-handling macros have been removed from `herr.h`.
* Most of `hbitio.h` has been moved to a private header file.
* Most of `hfile.h` has been moved to a private header file.
* `hdf.h` no longer pulls in the `hdfi.h` internal header file. See below for the implications of this.

## Migration Steps

Most users should not have to make any changes to their applications, as long
as they only use the public API as documented in the HDF4 Reference Manual.
The single- and multi-file API calls are all unchanged and we have preserved
the legacy HDF 3 API (found in `df.h`).

If you HAVE used any of the undocumented functionality packeded with the library
(e.g., `linklist.h`'s linked lists), you will either have to pull those
source files out of an older version of the library for incorporation into
your own code, or stick with HDF 4.2.16. Note that we don't plan to have
additional HDF 4.2.x maintenance releases.

An additional problem you may encounter stems from the removal of the `hdfi.h`
internal header file from `hdf.h`. That internal header brought in a lot of OS,
POSIX, and C headers which will now be missing from your application. If you
relied on the implicit inclusion of `hdfi.h` to bring in things like `sys/types`,
you will have to add these to your source code, as `hdf.h` only includes what
it needs.

## Appendix: List of Removed Header Files

The following headers are no longer installed:

* atom.h
* bitvect.h
* cdeflate.h
* cnbit.h
* cnone.h
* crle.h
* cskphuff.h
* cszip.h
* dfan.h
* dfgr.h
* dfrig.h
* dfsd.h
* dfufp2i.h
* dynarray.h
* hchunks.h
* hcompi.h
* hconv.h
* hdfi.h
* hkit.h
* linklist.h
* local\_nc.h
* mfani.h
* mfgri.h
* mstdio.h
* tbbt.h

We also no longer deploy the XDR header:
* xdr.h

## Appendix: List of Removed Functions and Function-Like Macros

### atom.h
```
HAIswap_cache
HAatom_object

HAinit_group
HAdestroy_group
HAregister_atom
HAPatom_object
HAatom_group
HAremove_atom
HAsearch_atom
HAshutdown
```

### bitvect.h
```
bv_new
bv_delete
bv_set
```

### cdeflate.h
```
HCPcdeflate_stread
HCPcdeflate_stwrite
HCPcdeflate_seek
HCPcdeflate_inquire
HCPcdeflate_read
HCPcdeflate_write
HCPcdeflate_endaccess
```

### cnbit.h
```
HCPcnbit_stread
HCPcnbit_stwrite
HCPcnbit_seek
HCPcnbit_inquire
HCPcnbit_read
HCPcnbit_write
HCPcnbit_endaccess
```

### cnone.h
```
HCPcnone_stread
HCPcnone_stwrite
HCPcnone_seek
HCPcnone_inquire
HCPcnone_read
HCPcnone_write
HCPcnone_endaccess
```

### crle.h
```
HCPcrle_stread
HCPcrle_stwrite
HCPcrle_seek
HCPcrle_inquire
HCPcrle_read
HCPcrle_write
HCPcrle_endaccess
```

### cskphuff.h
```
HCPcskphuff_stread
HCPcskphuff_stwrite
HCPcskphuff_seek
HCPcskphuff_inquire
HCPcskphuff_read
HCPcskphuff_write
HCPcskphuff_endaccess
```

### cszip.h
```
HCPcszip_stread
HCPcszip_stwrite
HCPcszip_seek
HCPcszip_inquire
HCPcszip_read
HCPcszip_write
HCPcszip_endaccess
```

### dfgr.h
```
DFGRIopen
```

### dfsd.h
```
DFSDIopen
DFSDIsdginfo
DFSDIclear
DFSDIclearNT
DFSDIgetdata
DFSDIputdata
DFSDIgetslice
DFSDIputslice
DFSDIendslice
DFSDIrefresh
DFSDIisndg
DFSDIgetrrank
DFSDIgetwrank
DFSDIsetdimstrs
DFSDIsetdatastrs
```

### dfufp2i.h
```
process
generate_scale
convert_interp
pixrep_scaled
compute_offsets
pixrep_simple
DFUfptoimage
```

### dynarray.h
```
DAcreate_array
DAdestroy_array
DAsize_array
DAget_elem
DAset_elem
DAdel_elem
```

### glist.h
```
HDGLinitialize_list
HDGLinitialize_sorted_list
HDGLdestroy_list
HDGLadd_to_beginning
HDGLadd_to_end
HDGLadd_to_list
HDGLremove_from_beginning
HDGLremove_from_end
HDGLremove_from_list
HDGLremove_current
HDGLremove_all
HDGLpeek_at_beginning
HDGLpeek_at_end
HDGLfirst_in_list
HDGLcurrent_in_list
HDGLlast_in_list
HDGLnext_in_list
HDGLprevious_in_list
HDGLreset_to_beginning
HDGLreset_to_end
HDGLnum_of_objects
HDGLis_empty
HDGLis_in_list
HDGLcopy_list
HDGLperform_on_list
HDGLfirst_that
HDGLnext_that
HDGLprevious_that
HDGLlast_that
HDGLall_such_that
HDGLremove_all_such_that
HDGSinitialize_stack
HDGSdestroy_stack
HDGSpush
HDGSpop
HDGSpop_all
HDGSpeek_at_top
HDGScopy_stack
HDGQinitialize_queue
HDGQdestroy_queue
HDGQenqueue
HDGQdequeue
HDGQdequeue_all
HDGQpeek_at_head
HDGQpeek_at_tail
HDGQcopy_queue
```

### hchunks.h
```
HMCcreate
HMCgetcompress
HMCgetcomptype
HMCgetdatainfo
HMCgetdatasize
HMCsetMaxcache
HMCwriteChunk
HMCreadChunk
HMCPcloseAID
HMCPgetnumrecs
```

### hdfi.h
```
INT16ENCODE
UINT16ENCODE
INT32ENCODE
UINT32ENCODE
NBYTEENCODE
INT16DECODE
UINT16DECODE
INT32DECODE
UINT32DECODE
NBYTEDECODE
MIN
MAX
```

### herr.h
```
HERROR
HRETURN_ERROR
HGOTO_ERROR
HGOTO_FAIL
HCLOSE_GOTO_ERROR
HGOTO_DONE
HE_REPORT
HE_REPORT_RETURN
HE_CLOSE_REPORT_RETURN
HE_REPORT_GOTO
HE_CLOSE_REPORT_GOTO
```

### hfile.h
```
BADFREC
BASETAG
SPECIALTAG
MKSPECIALTAG
HIget_access_rec
HIrelease_accrec_node
HIgetspinfo
HPcompare_filerec_path
HPcompare_accrec_tagref
HPgetdiskblock
HPfreediskblock
HPisfile_in_use
HDcheck_empty
HDget_special_info
HDset_special_info
HP_read
HPseek
HP_write
HPread_drec
tagcompare
tagdestroynode
HLPstread
HLPstwrite
HLPseek
HLPread
HLPwrite
HLPinquire
HLPendaccess
HLPcloseAID
HLPinfo
HXPstread
HXPstwrite
HXPseek
HXPread
HXPwrite
HXPinquire
HXPendaccess
HXPcloseAID
HXPinfo
HXPreset
HXPsetaccesstype
HXPshutdown
HCPstread
HCPstwrite
HCPseek
HCPinquire
HCPread
HCPwrite
HCPendaccess
HCPcloseAID
HCPinfo
get_comp_len
HBPstread
HBPstwrite
HBPseek
HBPinquire
HBPread
HBPwrite
HBPendaccess
HBPcloseAID
HBPinfo
HRPstread
HRPstwrite
HRPseek
HRPinquire
HRPread
HRPwrite
HRPendaccess
HRPcloseAID
HRPinfo
HTPstart
HTPinit
HTPsync
HTPend
HTPcreate
HTPselect
HTPendaccess
HTPdelete
HTPupdate
HTPinquire
HTPis_special
HTPdump_dds
```

### hqueue.h
```
H4_CIRCLEQ_HEAD
H4_CIRCLEQ_ENTRY
H4_CIRCLEQ_INIT
H4_CIRCLEQ_INSERT_AFTER
H4_CIRCLEQ_INSERT_BEFORE
H4_CIRCLEQ_INSERT_HEAD
H4_CIRCLEQ_INSERT_TAIL
H4_CIRCLEQ_REMOVE
```

### linklist.h
```
HULcreate_list
HULdestroy_list
HULadd_node
HULsearch_node
HULfirst_node
HULnext_node
HULremove_node
```

### mcache.h
```
HASHKEY
mcache_open
mcache_filter
mcache_new
mcache_get
mcache_put
mcache_sync
mcache_close
mcache_get_pagesize
mcache_get_maxcache
mcache_set_maxcache
mcache_get_npages
mcache_stat
```

### mfan.h
```
AN_CREATE_KEY
AN_KEY2REF
AN_KEY2TYPE
```

### mfgr.h
```
VALIDRIINDEX
GRIil_convert
GRIgrdestroynode
GRIattrdestroynode
GRIridestroynode
```

### mstdio.h
```
HCPmstdio_stread
HCPmstdio_stwrite
HCPmstdio_seek
HCPmstdio_inquire
HCPmstdio_read
HCPmstdio_write
HCPmstdio_endaccess
```

### tbbt.h
```
tbbtdfind
tbbtfind
tbbtdless
tbbtless
tbbtindx
tbbtdins
tbbtins
tbbtrem
tbbtfirst
tbbtlast
tbbtnext
tbbtprev
tbbtdfree
tbbtfree
tbbtprint
tbbtdump
tbbtcount
tbbt_shutdown
```

### vgint.h
```
VSIget_vdata_node
VSIrelease_vdata_node
VSIgetvdatas
VSIget_vsinstance_node
VSIrelease_vsinstance_node
VIget_vgroup_node
VIrelease_vgroup_node
VIget_vginstance_node
VIrelease_vginstance_node
VPparse_shutdown
Get_vfile
vsinst
vginst
vswritelist
vpackvg
vinsertpair
vpackvs
VPgetinfo
VSPgetinfo
map_from_old_types
trimendblanks
```

### error.h
```
nc_serror
NCadvise
```

### local\_nc.h (NOTE: May be prefixed with sd_)
```
IS_RECVAR
nc_serror
NCadvise
NC_computeshapes
NC_xtypelen
NC_xlen_array
NC_xlen_attr
NC_xlen_cdf
NC_xlen_dim
NC_xlen_iarray
NC_xlen_string
NC_xlen_var
NCmemset
NC_arrayfill
NC_copy_arrayvals
NC_free_array
NC_free_attr
NC_free_cdf
NC_free_dim
NC_free_iarray
NC_free_string
NC_free_var
NC_incr_array
NC_dimid
NCcktype
NC_indefine
xdr_cdf
xdr_numrecs
xdr_shorts
xdr_NC_array
xdr_NC_attr
xdr_NC_dim
xdr_NC_fill
xdr_NC_iarray
xdr_NC_string
xdr_NC_var
NC_typelen
NC_check_id
NC_dup_cdf
NC_new_cdf
NC_new_array
NC_re_array
NC_new_attr
NC_findattr
NC_new_dim
NC_new_iarray
NC_new_string
NC_re_string
NC_hlookupvar
NC_new_var
NCvario
NCcoordck
xdr_NCvshort
NC_dcpy
NCxdrfile_sync
NCxdrfile_create
hdf_fill_array
hdf_get_data
hdf_get_vp_aid
hdf_map_type
hdf_unmap_type
hdf_get_ref
hdf_create_dim_vdata
hdf_create_compat_dim_vdata
hdf_write_attr
hdf_write_dim
hdf_write_var
hdf_write_xdr_cdf
hdf_conv_scales
hdf_read_dims
hdf_read_attrs
hdf_read_vars
hdf_read_xdr_cdf
hdf_xdr_cdf
hdf_vg_clobber
hdf_cdf_clobber
hdf_close
hdf_read_sds_dims
hdf_read_sds_cdf
SDPfreebuf
NCgenio
NC_var_shape
NC_reset_maxopenfiles
NC_get_maxopenfiles
NC_get_systemlimit
NC_get_numopencdfs
cdf_unmap_type
nssdc_read_cdf
nssdc_write_cdf
nssdc_xdr_cdf
HDiscdf
HDisnetcdf
HDisnetcdf64
```

## Appendix: List of Removed Symbols, Types, etc.

### atom.h
```
group_t
atom_t
HAsearch_func_t
atom_id_cache
atom_obj_cache
```

### bitvect.h
```
bv_bool
BV_DEFAULT_BITS
BV_CHUNK_SIZE
bv_struct_tag
```

### cdeflate.h
```
comp_coder_deflate_info_t
cdeflate_funcs
```

### cnbit.h
```
NBIT_BUF_SIZE
NBIT_MASK_SIZE
nbit_mask_info_t
comp_coder_nbit_info_t
cnbit_funcs
```

### cnone.h
```
comp_coder_none_info_t
cnone_funcs
```

### crle.h
```
RLE_BUF_SIZE
RLE_NIL
RLE_MIN_RUN
RLE_MAX_RUN
RLE_MIN_MIX
comp_coder_rle_info_t
crle_funcs
```

### cskphuff.h
```
SKPHUFF_MAX_CHAR
SUCCMAX
TWICEMAX
ROOT
comp_coder_skphuff_info_t
cskphuff_funcs
```

### cszip.h
```
H4_SZ_ALLOW_K13_OPTION_MASK
H4_SZ_CHIP_OPTION_MASK
H4_SZ_EC_OPTION_MASK
H4_SZ_LSB_OPTION_MASK
H4_SZ_MSB_OPTION_MASK
H4_SZ_NN_OPTION_MASK
H4_SZ_RAW_OPTION_MASK
SZ_H4_REV_2
comp_coder_szip_info_t
cszip_funcs
```

### dfan.h
```
DFAN_LABEL
DFAN_DESC
DFAN_LAB_BLKSIZE
DFAN_DESC_BLKSIZE
DFAN_DEFENTRIES
DFANdirentry
DFANdirhead
```

### dfgr.h
```
DFGRdr
DFGRrig
```

### dfrig.h
```
DFRdr
DFRrig
R8dim
```

### dfsd.h
```
DFSsdg
DFnsdgle
DFnsdg_t_hdr
```

### dfufp2i.h
```
Input
Output
```

### dynarray.h
```
dynarr_p
```

### glist.h
```
Generic_list_element
Generic_list_info
Generic_list
Generic_stack
Generic_queue
```

### hbitio.h
```
bitrec_t
maskc
maskl
```

### hchunks.h
```
DIM_DEF
DIM_DEF_PTR
HCHUNK_DEF
HCHUNK_DEF_PTR
chunked_funcs
```

### hcompi.h
```
comp_coder_info_t
comp_stateinfo_t
comp_state_cache_t
compinfo_t
```

### hconv.h
```
UI8_IN
UI8_OUT
SI16_IN
SI16_OUT
UI16_IN
UI16_OUT
SI32_IN
SI32_OUT
UI32_IN
UI32_OUT
F32_IN
F32_OUT
F64_IN
F64_OUT
LUI8_IN
LUI8_OUT
LSI16_IN
LSI16_OUT
LUI16_IN
LUI16_OUT
LSI32_IN
LSI32_OUT
LUI32_IN
LUI32_OUT
LF32_IN
LF32_OUT
LF64_IN
LF64_OUT
NUI8_IN
NUI8_OUT
NSI16_IN
NSI16_OUT
NUI16_IN
NUI16_OUT
NSI32_IN
NSI32_OUT
NUI32_IN
NUI32_OUT
NF32_IN
NF32_OUT
NF64_IN
NF64_OUT
fpx
float_uint_uchar
```

### hdfi.h
```
DFMT_LE
DFMT_BE
DF_MT
UNIXUNBUFIO
UNIXBUFIO
FILELIB
H4_HAVE_LP64
FNAME_POST_UNDERSCORE
hdf_pint_t
JMEMSYS
FRETVAL
FCALLKEYW
FNAME
```

### hfile.h
```
MAGICLEN
HDFMAGIC
DD_SZ
NDDS_SZ
OFFSET_SZ
INVALID_OFFSET
INVALID_LENGTH
HI_OPEN
HI_CREATE
HI_READ
HI_WRITE
HI_CLOSE
HI_FLUSH
HI_SEEK
HI_SEEK_CUR
HI_SEEKEND
HI_TELL
OPENERR
hdf_file_t
dd_t
version_t
ddblock_t
tag_info
fileop_t
filerec_t
DDLIST_DIRTY
FILE_END_DIRTY
accrec_t
sp_info_block_t
funclist_t
functab_t
FIDTYPE
AIDTYPE
GROUPTYPE
SDSTYPE
DIMTYPE
CDFTYPE
VGIDTYPE
VSIDTYPE
BITTYPE
GRIDTYPE
RIIDTYPE
diskblock_header
diskblock_tail
```

### hkit.h
```
tag_descript_t
tag_descriptions
nt_descript_t
nt_descriptions
```

### linklist.h
```
HUL_UNSORTED_LIST
HUL_SORTED_LIST
HULsearch_func_t
HULfind_func_t
node_info_t
list_head_t
```

### mcache.h
```
HASHSIZE
RET_SUCCESS
RET_ERROR
DEF_PAGESIZE
DEF_MAXCACHE
BKT
MCACHE_DIRTY
MCACHE_PINNED
ELEM_READ
ELEM_WRITTEN
ELEM_SYNC
L_ELEM
MCACHE_EXTEND
MCACHE
```

### mfan.h
```
ANnode
ANentry
ANATOM_HASH_SIZE
```

### mfgr.h
```
GRATOM_HASH_SIZE
RI_TAG
ATTR_TAG
GR_ATTR_THRESHHOLD
gr_info_t
at_info_t
dim_info_t
ri_info_t
```

### mstdio.h
```
comp_model_stdio_info_t
mstdio_funcs
```

### tbbt.h
```
TBBT_FAST_UINT16_COMPARE
TBBT_FAST_INT32_COMPARE
tbbt_node_private
tbbt_tree_private
TBBT_NODE
TBBT_TREE
```

### vgint.h
```
VGROUP
VDATA
VSUBGROUP
SYMDEF
VWRITELIST
DYN_VWRITELIST
vs_attr_t
vg_attr_t
DYN_VREADLIST
vgroup_desc
vdata_desc
vginstance_t
vsinstance_t
vfile_t
```

### local\_nc.h
```
FILENAME_MAX
NC_BYTE_SIZE
NC_CHAR_SIZE
NC_SHORT_SIZE
NC_LONG_SIZE
NC_FLOAT_SIZE
NC_DOUBLE_SIZE
NC_UNSPECIFIED_SIZE
Void
ATTR_TAG
DIM_TAG
VAR_TAG
DATA_TAG
BOGUS_TAG
DIMVAL_VERSION00
DIMVAL_VERSION01
BLOCK_MULT
MAX_BLOCK_SIZE
BLOCK_COUNT
MAX_VXR_ENTRIES
vix_t
NC_array
NC_string
NC_iarray
NC_dim
NC_attr
NC
NC_var
netCDF_FILE
HDF_FILE
CDF_FILE
cdf_routine_name
MAGICOFFSET
CDFMAGIC
HDFXMAGIC
NCMAGIC
NCMAGIC64
NCLINKMAGIC
```

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF/releases/.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef H4_H_PROTO
#define H4_H_PROTO

#include <stdio.h> /* for FILE */

#include "H4api_adpt.h"
#include "hdf.h"

/* Useful macros, which someday might become actual functions */
/* Wrappers for Hinquire. feb-2-92 */
#define HQueryfileid(aid, fileid)                                                                            \
    (Hinquire((int32)aid, (int32 *)fileid, (uint16 *)NULL, (uint16 *)NULL, (int32 *)NULL, (int32 *)NULL,     \
              (int32 *)NULL, (int16 *)NULL, (int16 *)NULL))

#define HQuerytagref(aid, tag, ref)                                                                          \
    (Hinquire((int32)aid, (int32 *)NULL, (uint16 *)tag, (uint16 *)ref, (int32 *)NULL, (int32 *)NULL,         \
              (int32 *)NULL, (int16 *)NULL, (int16 *)NULL))

#define HQuerylength(aid, length)                                                                            \
    (Hinquire((int32)aid, (int32 *)NULL, (uint16 *)NULL, (uint16 *)NULL, (int32 *)length, (int32 *)NULL,     \
              (int32 *)NULL, (int16 *)NULL, (int16 *)NULL))

#define HQueryoffset(aid, offset)                                                                            \
    (Hinquire((int32)aid, (int32 *)NULL, (uint16 *)NULL, (uint16 *)NULL, (int32 *)NULL, (int32 *)offset,     \
              (int32 *)NULL, (int16 *)NULL, (int16 *)NULL))

#define HQueryposition(aid, position)                                                                        \
    (Hinquire((int32)aid, (int32 *)NULL, (uint16 *)NULL, (uint16 *)NULL, (int32 *)NULL, (int32 *)NULL,       \
              (int32 *)position, (int16 *)NULL, (int16 *)NULL))

#define HQueryaccess(aid, access)                                                                            \
    (Hinquire((int32)aid, (int32 *)NULL, (uint16 *)NULL, (uint16 *)NULL, (int32 *)NULL, (int32 *)NULL,       \
              (int32 *)NULL, (int16 *)access, (int16 *)NULL))

#define HQueryspecial(aid, special)                                                                          \
    (Hinquire((int32)aid, (int32 *)NULL, (uint16 *)NULL, (uint16 *)NULL, (int32 *)NULL, (int32 *)NULL,       \
              (int32 *)NULL, (int16 *)NULL, (int16 *)special))

#ifdef __cplusplus
extern "C" {
#endif

/*
 ** from hfile.c
 */
HDFLIBAPI int32 Hopen(const char *path, int acc_mode, int16 ndds);

HDFLIBAPI int Hclose(int32 file_id);

HDFLIBAPI int32 Hstartread(int32 file_id, uint16 tag, uint16 ref);

HDFLIBAPI int Hnextread(int32 access_id, uint16 tag, uint16 ref, int origin);

HDFLIBAPI int Hexist(int32 file_id, uint16 search_tag, uint16 search_ref);

HDFLIBAPI int Hinquire(int32 access_id, int32 *pfile_id, uint16 *ptag, uint16 *pref, int32 *plength,
                       int32 *poffset, int32 *pposn, int16 *paccess, int16 *pspecial);

HDFLIBAPI int32 Hstartwrite(int32 file_id, uint16 tag, uint16 ref, int32 length);

HDFLIBAPI int32 Hstartaccess(int32 file_id, uint16 tag, uint16 ref, uint32 flags);

HDFLIBAPI int Hsetlength(int32 file_id, int32 length);

HDFLIBAPI int Happendable(int32 aid);

HDFLIBAPI int HPisappendable(int32 aid);

HDFLIBAPI int HPregister_term_func(hdf_termfunc_t term_func);

HDFLIBAPI int Hseek(int32 access_id, int32 offset, int origin);

HDFLIBAPI int32 Htell(int32 access_id);

HDFLIBAPI int32 Hread(int32 access_id, int32 length, void *data);

HDFLIBAPI int32 Hwrite(int32 access_id, int32 length, const void *data);

HDFLIBAPI int32 Htrunc(int32 access_id, int32 trunc_len);

HDFLIBAPI int Hendaccess(int32 access_id);

HDFLIBAPI int HDgetc(int32 access_id);

HDFLIBAPI int HDputc(uint8 c, int32 access_id);

HDFLIBAPI int32 Hgetelement(int32 file_id, uint16 tag, uint16 ref, uint8 *data);

HDFLIBAPI int32 Hputelement(int32 file_id, uint16 tag, uint16 ref, const uint8 *data, int32 length);

HDFLIBAPI int32 Hlength(int32 file_id, uint16 tag, uint16 ref);

HDFLIBAPI int32 Hoffset(int32 file_id, uint16 tag, uint16 ref);

HDFLIBAPI int Hsync(int32 file_id);

HDFLIBAPI int Hcache(int32 file_id, int cache_on);

HDFLIBAPI int Hgetlibversion(uint32 *majorv, uint32 *minorv, uint32 *releasev, char *string);

HDFLIBAPI int Hgetfileversion(int32 file_id, uint32 *majorv, uint32 *minorv, uint32 *release, char *string);

HDFLIBAPI int Hsetaccesstype(int32 access_id, unsigned accesstype);

HDFLIBAPI uint16 HDmake_special_tag(uint16 tag);

HDFLIBAPI int HDis_special_tag(uint16 tag);

HDFLIBAPI uint16 HDbase_tag(uint16 tag);

HDFLIBAPI int HDerr(int32 file_id);

HDFLIBAPI int HDvalidfid(int32 file_id);

HDFLIBAPI const char *HDgettagdesc(uint16 tag);

HDFLIBAPI char *HDgettagsname(uint16 tag);

HDFLIBAPI int HDgettagnum(const char *tag_name);

HDFLIBAPI char *HDgetNTdesc(int32 nt);

HDFLIBAPI const char *HDfidtoname(int32 fid);

HDFLIBAPI int Hgetntinfo(int32 numbertype, hdf_ntinfo_t *nt_info);

HDFLIBAPI int Hishdf(const char *filename);

HDFLIBAPI int Hfidinquire(int32 file_id, char **fname, int *acc_mode, int *attach);

HDFLIBAPI int Hshutdown(void);

HDFLIBAPI void HPend(void);

HDFLIBAPI int HDdont_atexit(void);

/*
 ** from hfiledd.c
 */
/******************************************************************************
 NAME
     Hdupdd - Duplicate a data descriptor

 DESCRIPTION
    Duplicates a data descriptor so that the new tag/ref points to the
    same data element pointed to by the old tag/ref.  Return FAIL if
    the given tag/ref are already in use.

 RETURNS
    returns SUCCEED (0) if successful, FAIL (-1) otherwise

*******************************************************************************/
HDFLIBAPI int Hdupdd(int32  file_id, /* IN: File ID the tag/refs are in */
                     uint16 tag,     /* IN: Tag of new tag/ref */
                     uint16 ref,     /* IN: Ref of new tag/ref */
                     uint16 old_tag, /* IN: Tag of old tag/ref */
                     uint16 old_ref  /* IN: Ref of old tag/ref */
);

/******************************************************************************
 NAME
     Hnumber - Determine the number of objects of a given type

 DESCRIPTION
    Determine how many objects of the given tag are in the file.
    tag may be set to DFTAG_WILDCARD to get back the total number
    of objects in the file.

    Note, a return value of zero is not a fail condition.

 RETURNS
    the number of objects of type 'tag' else FAIL

*******************************************************************************/
HDFLIBAPI int32 Hnumber(int32  file_id, /* IN: File ID the tag/refs are in */
                        uint16 tag      /* IN: Tag to count */
);

/******************************************************************************
 NAME
     Hnewref - Returns a ref that is guaranteed to be unique in the file

 DESCRIPTION
    Returns a ref number that can be used with any tag to produce a
    unique tag/ref.  Successive calls to Hnewref will generate a
    strictly increasing sequence until the highest possible ref had been
    returned, then Hnewref will return unused ref's starting from 1.

 RETURNS
    returns the ref number, 0 otherwise

*******************************************************************************/
HDFLIBAPI uint16 Hnewref(int32 file_id /* IN: File ID the tag/refs are in */
);

/******************************************************************************
 NAME
    Htagnewref  - returns a ref that is unique in the file for a given tag

 DESCRIPTION
    Returns a ref number that can be used with any tag to produce a
    unique tag/ref.  Successive calls to Hnewref will generate a
    strictly increasing sequence until the highest possible ref had been
    returned, then Hnewref will return unused ref's starting from 1.

 RETURNS
    returns the ref number, 0 otherwise

*******************************************************************************/
HDFLIBAPI uint16 Htagnewref(int32  file_id, /* IN: File ID the tag/refs are in */
                            uint16 tag      /* IN: Tag to search for a new ref for */
);

/******************************************************************************
 NAME
    Hfind - locate the next object of a search in an HDF file

 DESCRIPTION
    Searches for the `next' DD that fits the search tag/ref.  Wildcards
    apply.  If origin is DF_FORWARD, search from current position forwards
    in the file, otherwise DF_BACKWARD searches backward from the current
    position in the file.  If *find_tag and *find_ref are both set to
    0, this indicates the beginning of a search, and the search will
    start from the beginning of the file if the direction is DF_FORWARD
    and from the and of the file if the direction is DF_BACKWARD.

 RETURNS
    returns SUCCEED (0) if successful and FAIL (-1) otherwise

*******************************************************************************/
HDFLIBAPI int Hfind(int32   file_id,    /* IN: file ID to search in */
                    uint16  search_tag, /* IN: the tag to search for (can be DFTAG_WILDCARD) */
                    uint16  search_ref, /* IN: ref to search for (can be DFREF_WILDCARD) */
                    uint16 *find_tag,   /* IN: if (*find_tag==0) and (*find_ref==0) then start search */
                                        /* OUT: tag matching the search tag */
                    uint16 *find_ref,   /* IN: if (*find_tag==0) and (*find_ref==0) then start search */
                                        /* OUT: ref matching the search ref */
                    int32 *find_offset, /* OUT: offset of the data element found */
                    int32 *find_length, /* OUT: length of the data element found */
                    int    direction    /* IN: Direction to search in: */
                                        /*  DF_FORWARD searches forward from the current location */
                                        /*  DF_BACKWARD searches backward from the current location */
);

/******************************************************************************
 NAME
     HDcheck_tagref - Checks to see if tag/ref is in DD list i.e. created already

 DESCRIPTION
     Routine checks to see if tag/ref exists in the DD list i.e. has
     been created.

 RETURNS
     0-> tag/ref does not exist
     1-> tag/ref exists
    -1-> function failed

*******************************************************************************/
HDFLIBAPI int HDcheck_tagref(int32  file_id, /* IN: id of file */
                             uint16 tag,     /* IN: Tag to check */
                             uint16 ref /* IN: ref to check */);

/************************************************************************
NAME
   HDreuse_tagref -- reuse a data descriptor preserving tag/ref

DESCRIPTION
   Reuses the data descriptor of tag/ref in the dd list of the file.
   The tag/ref must already exist in the DD list.
   This routine is unsafe and may leave a file in a condition that is
   not usable by some routines.  Use with care. Not valid for
   special elements right now. Used for allowing the data to change
   and move somewhere else in the file for non-special elements.
   Must be carefully if apply to higher-level objects like GR's and SDS
   that are comprised of other objects.
   Useful when re-writing simple elements whose size changes while
   preserving the original tag/ref of the element since other elements
   might refer to this element by tag/ref e.g. in a Vgroup.

RETURNS
   returns SUCCEED (0) if successful, FAIL (-1) otherwise
************************************************************************/
HDFLIBAPI int HDreuse_tagref(int32  file_id, /* IN: id of file */
                             uint16 tag,     /* IN: tag of data descriptor to reuse */
                             uint16 ref /* IN: ref of data descriptor to reuse */);

/******************************************************************************
 NAME
     Hdeldd - Delete a data descriptor

 DESCRIPTION
    Deletes a data descriptor of tag/ref from the dd list of the file.
    This routine is unsafe and may leave a file in a condition that is
    not usable by some routines.  Use with care.
    For example, if this element is contained in a Vgroup, that group
    will *NOT* get updated to reflect that this element has been deleted.

 RETURNS
    returns SUCCEED (0) if successful, FAIL (-1) otherwise

*******************************************************************************/
HDFLIBAPI int Hdeldd(int32  file_id, /* IN: File ID the tag/refs are in */
                     uint16 tag,     /* IN: Tag of tag/ref to delete */
                     uint16 ref      /* IN: Ref of tag/ref to delete */
);

/*
 ** from hdfalloc.c
 */

HDFLIBAPI void *HDmemfill(void *dest, const void *src, uint32 item_size, uint32 num_items);

HDFLIBAPI char *HIstrncpy(char *dest, const char *source, int len);

HDFLIBAPI int32 HDspaceleft(void);

HDFLIBAPI int HDc2fstr(char *str, int len);

HDFLIBAPI char *HDf2cstring(_fcd fdesc, int len);

HDFLIBAPI int HDflush(int32 file_id);

HDFLIBAPI int HDpackFstring(char *src, char *dest, int len);

/*
 ** from hblocks.c
 */
HDFLIBAPI int32 HLcreate(int32 file_id, uint16 tag, uint16 ref, int32 block_length, int32 number_blocks);

HDFLIBAPI int HLconvert(int32 aid, int32 block_length, int32 number_blocks);

HDFLIBAPI int HDinqblockinfo(int32 aid, int32 *length, int32 *first_length, int32 *block_length,
                             int32 *number_blocks);

HDFLIBAPI int HLsetblockinfo(int32 aid, int32 block_size, int32 num_blocks);

HDFLIBAPI int HLgetblockinfo(int32 aid, int32 *block_size, int32 *num_blocks);

HDFLIBAPI int HLgetdatainfo(int32 file_id, uint8 *buf, unsigned start_block, unsigned info_count,
                            int32 *offsetarray, int32 *lengtharray);

/*
 ** from hextelt.c
 */
HDFLIBAPI int32 HXcreate(int32 file_id, uint16 tag, uint16 ref, const char *extern_file_name, int32 offset,
                         int32 start_len);

HDFLIBAPI int HXsetcreatedir(const char *dir);

HDFLIBAPI int HXsetdir(const char *dir);

/*
 ** from hcomp.c
 */
HDFLIBAPI int32 HCcreate(int32 file_id, uint16 tag, uint16 ref, comp_model_t model_type, model_info *m_info,
                         comp_coder_t coder_type, comp_info *c_info);

HDFLIBAPI int HCPgetcompinfo(int32 file_id, uint16 data_tag, uint16 data_ref, comp_coder_t *coder_type,
                             comp_info *c_info);

HDFLIBAPI int HCPgetcomptype(int32 file_id, uint16 data_tag, uint16 data_ref, comp_coder_t *coder_type);

HDFLIBAPI int HCPgetdatasize(int32 file_id, uint16 data_tag, uint16 data_ref, int32 *comp_size,
                             int32 *orig_size);

HDFPUBLIC int HCget_config_info(comp_coder_t coder_type, uint32 *compression_config_info);

HDFLIBAPI int32 HCPquery_encode_header(comp_model_t model_type, model_info *m_info, comp_coder_t coder_type,
                                       comp_info *c_info);

HDFLIBAPI int HCPencode_header(uint8 *p, comp_model_t model_type, model_info *m_info, comp_coder_t coder_type,
                               comp_info *c_info);

HDFLIBAPI int HCPdecode_header(uint8 *p, comp_model_t *model_type, model_info *m_info,
                               comp_coder_t *coder_type, comp_info *c_info);

/*
 ** from cszip.c
 */

HDFLIBAPI int HCPsetup_szip_parms(comp_info *c_info, int32 nt, int32 ncomp, int32 ndims, int32 *dims,
                                  int32 *cdims);
HDFLIBAPI int HCPrm_szip_special_bit(comp_info *c_info);
/*
 ** from hbuffer.c
 */
HDFLIBAPI int HBconvert(int32 aid);

/*
 ** from hcompri.c
 */
HDFLIBAPI int32 HRPconvert(int32 fid, uint16 tag, uint16 ref, int32 xdim, int32 ydim, int16 scheme,
                           comp_info *cinfo, unsigned pixel_size);

/*
 ** from herr.c
 */
HDFLIBAPI const char *HEstring(hdf_err_code_t error_code);

HDFLIBAPI void HEpush(hdf_err_code_t error_code, const char *function_name, const char *file_name, int line);

HDFLIBAPI void HEreport(const char *, ...);

HDFLIBAPI void HEprint(FILE *stream, int32 print_level);

HDFLIBAPI int16 HEvalue(int32 level);

HDFLIBAPI void HEclear(void);

HDFLIBAPI int HEshutdown(void);

/*
 ** from hbitio.c
 */
HDFLIBAPI int32 Hstartbitread(int32 file_id, uint16 tag, uint16 ref);

HDFLIBAPI int32 Hstartbitwrite(int32 file_id, uint16 tag, uint16 ref, int32 length);

HDFLIBAPI int Hbitappendable(int32 bitid);

HDFLIBAPI int Hbitwrite(int32 bitid, int count, uint32 data);

HDFLIBAPI int Hbitread(int32 bitid, int count, uint32 *data);

HDFLIBAPI int Hbitseek(int32 bitid, int32 byte_offset, int bit_offset);

HDFLIBAPI int Hgetbit(int32 bitid);

HDFLIBAPI int32 Hendbitaccess(int32 bitfile_id, int flushbit);

HDFLIBAPI int HPbitshutdown(void);

/*
 ** from hdatainfo.c
 */

/* Structure that holds a data descriptor */
typedef struct hdf_ddinfo_t {
    uint16 tag;
    uint16 ref;
    int32  offset;
    int32  length;
} hdf_ddinfo_t;

/*
 ** Public functions for getting raw data information - from hdatainfo.c
 */
HDFLIBAPI int ANgetdatainfo(int32 ann_id, int32 *offset, int32 *length);

HDFLIBAPI int HDgetdatainfo(int32 file_id, uint16 data_tag, uint16 data_ref, int32 *chk_coord,
                            unsigned start_block, unsigned info_count, int32 *offsetarray,
                            int32 *lengtharray);

HDFLIBAPI int VSgetdatainfo(int32 vsid, unsigned start_block, unsigned info_count, int32 *offsetarray,
                            int32 *lengtharray);

HDFLIBAPI int VSgetattdatainfo(int32 vsid, int32 findex, int attrindex, int32 *offset, int32 *length);

HDFLIBAPI int Vgetattdatainfo(int32 vgid, int attrindex, int32 *offset, int32 *length);

HDFLIBAPI int GRgetdatainfo(int32 riid, unsigned start_block, unsigned info_count, int32 *offsetarray,
                            int32 *lengtharray);

HDFLIBAPI int GRgetattdatainfo(int32 id, int32 attrindex, int32 *offset, int32 *length);

HDFLIBAPI int GRgetpalinfo(int32 gr_id, unsigned pal_count, hdf_ddinfo_t *palinfo_array);

/*
 ** from dfutil.c
 */
HDFLIBAPI uint16 DFfindnextref(int32 file_id, uint16 tag, uint16 lref);

/*
 ** from dfcomp.c
 */
HDFLIBAPI int DFputcomp(int32 file_id, uint16 tag, uint16 ref, const uint8 *image, int32 xdim, int32 ydim,
                        uint8 *palette, uint8 *newpal, int16 scheme, comp_info *cinfo);

HDFLIBAPI int DFgetcomp(int32 file_id, uint16 tag, uint16 ref, uint8 *image, int32 xdim, int32 ydim,
                        uint16 scheme);

/*
 ** from dfrle.c
 */
HDFLIBAPI int32 DFCIrle(const void *buf, void *bufto, int32 len);

HDFLIBAPI int32 DFCIunrle(uint8 *buf, uint8 *bufto, int32 outlen, int resetsave);

/*
 ** from dfimcomp.c
 */
HDFLIBAPI void DFCIimcomp(int32 xdim, int32 ydim, const uint8 *in, uint8 out[], uint8 in_pal[],
                          uint8 out_pal[], int mode);

HDFLIBAPI void DFCIunimcomp(int32 xdim, int32 ydim, uint8 in[], uint8 out[]);

/*
 ** from dfjpeg.c
 */

HDFLIBAPI int DFCIjpeg(int32 file_id, uint16 tag, uint16 ref, int32 xdim, int32 ydim, const void *image,
                       int16 scheme, comp_info *scheme_info);

/*
 ** from dfunjpeg.c
 */

HDFLIBAPI int DFCIunjpeg(int32 file_id, uint16 tag, uint16 ref, void *image, int32 xdim, int32 ydim,
                         int16 scheme);

/*
 ** from dfgroup.c
 */
HDFLIBAPI int32 DFdiread(int32 file_id, uint16 tag, uint16 ref);

HDFLIBAPI int DFdiget(int32 list, uint16 *ptag, uint16 *pref);

HDFLIBAPI int DFdinobj(int32 list);

HDFLIBAPI int32 DFdisetup(int maxsize);

HDFLIBAPI int DFdiput(int32 list, uint16 tag, uint16 ref);

HDFLIBAPI int DFdiwrite(int32 file_id, int32 list, uint16 tag, uint16 ref);

HDFLIBAPI void DFdifree(int32 groupID);

/*
 ** from dfp.c
 */
HDFLIBAPI int DFPgetpal(const char *filename, void *palette);

HDFLIBAPI int DFPputpal(const char *filename, const void *palette, int overwrite, const char *filemode);

HDFLIBAPI int DFPaddpal(const char *filename, const void *palette);

HDFLIBAPI int DFPnpals(const char *filename);

HDFLIBAPI int DFPreadref(const char *filename, uint16 ref);

HDFLIBAPI int DFPwriteref(const char *filename, uint16 ref);

HDFLIBAPI int DFPrestart(void);

HDFLIBAPI uint16 DFPlastref(void);

/*
 ** from dfr8.c
 */
HDFLIBAPI int DFR8setcompress(int32 scheme, comp_info *cinfo);

HDFLIBAPI int DFR8getdims(const char *filename, int32 *pxdim, int32 *pydim, int *pispal);

HDFLIBAPI int DFR8getimage(const char *filename, uint8 *image, int32 xdim, int32 ydim, uint8 *pal);

HDFLIBAPI int DFR8setpalette(uint8 *pal);

HDFLIBAPI int DFR8putimage(const char *filename, const void *image, int32 xdim, int32 ydim, uint16 compress);

HDFLIBAPI int DFR8addimage(const char *filename, const void *image, int32 xdim, int32 ydim, uint16 compress);

HDFLIBAPI int DFR8nimages(const char *filename);

HDFLIBAPI int DFR8readref(const char *filename, uint16 ref);

HDFLIBAPI int DFR8writeref(const char *filename, uint16 ref);

HDFLIBAPI int DFR8restart(void);

HDFLIBAPI uint16 DFR8lastref(void);

HDFLIBAPI int DFR8getpalref(uint16 *pal_ref);

HDFLIBAPI int DFR8Pshutdown(void);

/*
 ** from dfgr.c
 */
HDFLIBAPI int DFGRgetlutdims(const char *filename, int32 *pxdim, int32 *pydim, int *pncomps, int *pil);

HDFLIBAPI int DFGRreqlutil(int il);

HDFLIBAPI int DFGRgetlut(const char *filename, void *lut, int32 xdim, int32 ydim);

HDFLIBAPI int DFGRgetimdims(const char *filename, int32 *pxdim, int32 *pydim, int *pncomps, int *pil);

HDFLIBAPI int DFGRreqimil(int il);

HDFLIBAPI int DFGRgetimage(const char *filename, void *image, int32 xdim, int32 ydim);

HDFLIBAPI int DFGRsetcompress(int32 scheme, comp_info *cinfo);

HDFLIBAPI int DFGRsetlutdims(int32 xdim, int32 ydim, int ncomps, int il);

HDFLIBAPI int DFGRsetlut(void *lut, int32 xdim, int32 ydim);

HDFLIBAPI int DFGRaddlut(const char *filename, void *lut, int32 xdim, int32 ydim);

HDFLIBAPI int DFGRsetimdims(int32 xdim, int32 ydim, int ncomps, int il);

HDFLIBAPI int DFGRaddimage(const char *filename, void *image, int32 xdim, int32 ydim);

HDFLIBAPI int DFGRputimage(const char *filename, void *image, int32 xdim, int32 ydim);

HDFLIBAPI int DFGRreadref(const char *filename, uint16 ref);

HDFLIBAPI uint16 DFGRIlastref(void);

HDFLIBAPI int DFGRIgetdims(const char *filename, int32 *pxdim, int32 *pydim, int *pncomps, int *pil,
                           int type);

HDFLIBAPI int DFGRIreqil(int il, int type);

HDFLIBAPI int DFGRIgetimlut(const char *filename, void *imlut, int32 xdim, int32 ydim, int type,
                            int isfortran, int *compressed, uint16 *compr_type, int *has_pal);

HDFLIBAPI int DFGRIsetdims(int32 xdim, int32 ydim, int ncomps, int type);

HDFLIBAPI int DFGRIsetil(int il, int type);

HDFLIBAPI int DFGRIrestart(void);

HDFLIBAPI int DFGRIaddimlut(const char *filename, const void *imlut, int32 xdim, int32 ydim, int type,
                            int isfortran, int newfile);

HDFLIBAPI int DFGRPshutdown(void);

/*
 ** from df24.c
 */
HDFLIBAPI int DF24getdims(const char *filename, int32 *pxdim, int32 *pydim, int *pil);

HDFLIBAPI int DF24reqil(int il);

HDFLIBAPI int DF24getimage(const char *filename, void *image, int32 xdim, int32 ydim);

HDFLIBAPI int DF24setdims(int32 xdim, int32 ydim);

HDFLIBAPI int DF24setil(int il);

HDFLIBAPI int DF24setcompress(int32 type, comp_info *cinfo);

HDFLIBAPI int DF24restart(void);

HDFLIBAPI int DF24addimage(const char *filename, const void *image, int32 xdim, int32 ydim);

HDFLIBAPI int DF24putimage(const char *filename, const void *image, int32 xdim, int32 ydim);

HDFLIBAPI int DF24nimages(const char *filename);

HDFLIBAPI int DF24readref(const char *filename, uint16 ref);

HDFLIBAPI uint16 DF24lastref(void);

/*
 ** from dfan.c
 */

HDFLIBAPI int32 DFANgetlablen(const char *filename, uint16 tag, uint16 ref);

HDFLIBAPI int DFANgetlabel(const char *filename, uint16 tag, uint16 ref, char *label, int32 maxlen);

HDFLIBAPI int32 DFANgetdesclen(const char *filename, uint16 tag, uint16 ref);

HDFLIBAPI int DFANgetdesc(const char *filename, uint16 tag, uint16 ref, char *desc, int32 maxlen);

HDFLIBAPI int32 DFANgetfidlen(int32 file_id, int isfirst);

HDFLIBAPI int32 DFANgetfid(int32 file_id, char *id, int32 maxlen, int isfirst);

HDFLIBAPI int32 DFANgetfdslen(int32 file_id, int isfirst);

HDFLIBAPI int32 DFANgetfds(int32 file_id, char *desc, int32 maxlen, int isfirst);

HDFLIBAPI int DFANputlabel(const char *filename, uint16 tag, uint16 ref, char *label);

HDFLIBAPI int DFANputdesc(const char *filename, uint16 tag, uint16 ref, char *desc, int32 desclen);

HDFLIBAPI int DFANaddfid(int32 file_id, char *id);

HDFLIBAPI int DFANaddfds(int32 file_id, char *desc, int32 desclen);

HDFLIBAPI uint16 DFANlastref(void);

HDFLIBAPI int DFANlablist(const char *filename, uint16 tag, uint16 reflist[], char *labellist, int listsize,
                          int maxlen, int startpos);

HDFLIBAPI int DFANclear(void);

HDFLIBAPI int DFANIclear(void);

HDFLIBAPI uint16 DFANIlocate(int32 file_id, int type, uint16 tag, uint16 ref);

HDFLIBAPI int DFANIaddentry(int type, uint16 annref, uint16 datatag, uint16 dataref);

HDFLIBAPI int32 DFANIgetannlen(const char *filename, uint16 tag, uint16 ref, int type);

HDFLIBAPI int DFANIgetann(const char *filename, uint16 tag, uint16 ref, uint8 *ann, int32 maxlen, int type,
                          int isfortran);

HDFLIBAPI int DFANIputann(const char *filename, uint16 tag, uint16 ref, uint8 *ann, int32 annlen, int type);

HDFLIBAPI int DFANIlablist(const char *filename, uint16 tag, uint16 reflist[], uint8 *labellist, int listsize,
                           int maxlen, int startpos, int isfortran);

HDFLIBAPI int DFANIaddfann(int32 file_id, char *ann, int32 annlen, int type);

HDFLIBAPI int32 DFANIgetfannlen(int32 file_id, int type, int isfirst);

HDFLIBAPI int32 DFANIgetfann(int32 file_id, char *ann, int32 maxlen, int type, int isfirst);

HDFLIBAPI int DFANPshutdown(void);

/*
 ** from dfsd.c
 */

HDFLIBAPI int DFSDgetdims(const char *filename, int *prank, int32 sizes[], int maxrank);

HDFLIBAPI int DFSDgetdatastrs(char *label, char *unit, char *format, char *coordsys);

HDFLIBAPI int DFSDgetdimstrs(int dim, char *label, char *unit, char *format);

HDFLIBAPI int DFSDgetdatalen(int *llabel, int *lunit, int *lformat, int *lcoordsys);

HDFLIBAPI int DFSDgetdimlen(int dim, int *llabel, int *lunit, int *lformat);

HDFLIBAPI int DFSDgetdimscale(int dim, int32 maxsize, void *scale);

HDFLIBAPI int DFSDgetrange(void *pmax, void *pmin);

HDFLIBAPI int DFSDgetdata(const char *filename, int rank, int32 maxsizes[], void *data);

HDFLIBAPI int DFSDsetlengths(int maxlen_label, int maxlen_unit, int maxlen_format, int maxlen_coordsys);

HDFLIBAPI int DFSDsetdims(int rank, int32 dimsizes[]);

HDFLIBAPI int DFSDsetdatastrs(const char *label, const char *unit, const char *format, const char *coordsys);

HDFLIBAPI int DFSDsetdimstrs(int dim, const char *label, const char *unit, const char *format);

HDFLIBAPI int DFSDsetdimscale(int dim, int32 dimsize, void *scale);

HDFLIBAPI int DFSDsetrange(void *maxi, void *mini);

HDFLIBAPI int DFSDputdata(const char *filename, int rank, int32 dimsizes[], void *data);

HDFLIBAPI int DFSDadddata(const char *filename, int rank, int32 dimsizes[], void *data);

HDFLIBAPI int DFSDrestart(void);

HDFLIBAPI int32 DFSDndatasets(char *filename);

HDFLIBAPI int DFSDclear(void);

HDFLIBAPI uint16 DFSDlastref(void);

HDFLIBAPI int DFSDreadref(char *filename, uint16 ref);

HDFLIBAPI int DFSDgetslice(const char *filename, int32 winst[], int32 windims[], void *data, int32 dims[]);

HDFLIBAPI int DFSDstartslice(const char *filename);

HDFLIBAPI int DFSDputslice(int32 winend[], void *data, int32 dims[]);

HDFLIBAPI int DFSDendslice(void);

HDFLIBAPI int DFSDsetNT(int32 numbertype);

HDFLIBAPI int DFSDsetorder(int arrayorder);

HDFLIBAPI int DFSDgetNT(int32 *pnumbertype);

HDFLIBAPI int DFSDpre32sdg(char *filename, uint16 ref, int *ispre32);

HDFLIBAPI int DFSDsetcal(float64 cal, float64 cal_err, float64 ioff, float64 ioff_err, int32 cal_nt);

HDFLIBAPI int DFSDgetcal(float64 *pcal, float64 *pcal_err, float64 *pioff, float64 *pioff_err, int32 *cal_nt);

HDFLIBAPI int DFSDwriteref(const char *filename, uint16 ref);

HDFLIBAPI int DFSDsetfillvalue(void *fill_value);

HDFLIBAPI int DFSDgetfillvalue(void *fill_value);

HDFLIBAPI int DFSDstartslab(const char *filename);

HDFLIBAPI int DFSDwriteslab(int32 start[], int32 stride[], int32 count[], void *data);

HDFLIBAPI int DFSDendslab(void);

HDFLIBAPI int DFSDreadslab(const char *filename, int32 start[], int32 slab_size[], int32 stride[],
                           void *buffer, int32 buffer_size[]);

HDFLIBAPI int DFSDPshutdown(void);

/*
 ** from dfconv.c
 */

HDFLIBAPI int DFKNTsize(int32 number_type);

HDFLIBAPI int32 DFKisnativeNT(int32 numbertype);

HDFLIBAPI int32 DFKislitendNT(int32 numbertype);

HDFLIBAPI int8 DFKgetPNSC(int32 numbertype, int32 machinetype);

HDFLIBAPI int DFKsetNT(int32 ntype);

HDFLIBAPI int32 DFKconvert(void *source, void *dest, int32 ntype, int32 num_elm, int16 acc_mode,
                           int32 source_stride, int32 dest_stride);

/*
 ** from dfknat.c
 */

HDFLIBAPI int DFKnb1b(void *s, void *d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

HDFLIBAPI int DFKnb2b(void *s, void *d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

HDFLIBAPI int DFKnb4b(void *s, void *d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

HDFLIBAPI int DFKnb8b(void *s, void *d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

/*
 ** from dfkswap.c
 */

HDFLIBAPI int DFKsb2b(void *s, void *d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

HDFLIBAPI int DFKsb4b(void *s, void *d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

HDFLIBAPI int DFKsb8b(void *s, void *d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

/*
 ** Multi-file Annotation - from mfan.c
 */
HDFLIBAPI int32 ANstart(int32 file_id);

HDFLIBAPI int ANfileinfo(int32 an_id, int32 *n_file_label, int32 *n_file_desc, int32 *n_obj_label,
                         int32 *n_obj_desc);

HDFLIBAPI int32 ANend(int32 an_id);

HDFLIBAPI int32 ANcreate(int32 an_id, uint16 elem_tag, uint16 elem_ref, ann_type type);

HDFLIBAPI int32 ANcreatef(int32 an_id, ann_type type);

HDFLIBAPI int32 ANselect(int32 an_id, int32 idx, ann_type type);

HDFLIBAPI int ANnumann(int32 an_id, ann_type type, uint16 elem_tag, uint16 elem_ref);

HDFLIBAPI int ANannlist(int32 an_id, ann_type type, uint16 elem_tag, uint16 elem_ref, int32 ann_list[]);

HDFLIBAPI int32 ANannlen(int32 ann_id);

HDFLIBAPI int32 ANwriteann(int32 ann_id, const char *ann, int32 annlen);

HDFLIBAPI int32 ANreadann(int32 ann_id, char *ann, int32 maxlen);

HDFLIBAPI int ANendaccess(int32 ann_id);

HDFLIBAPI int32 ANget_tagref(int32 an_id, int32 idx, ann_type type, uint16 *ann_tag, uint16 *ann_ref);

HDFLIBAPI int32 ANid2tagref(int32 an_id, uint16 *ann_tag, uint16 *ann_ref);

HDFLIBAPI int32 ANtagref2id(int32 an_id, uint16 ann_tag, uint16 ann_ref);

HDFLIBAPI uint16 ANatype2tag(ann_type atype);

HDFLIBAPI ann_type ANtag2atype(uint16 atag);

/*
 ** Multi-file General Raster - from mfgr.c
 */
HDFLIBAPI int rigcompare(void *k1, void *k2, int cmparg);

HDFLIBAPI int32 GRstart(int32 hdf_file_id);

HDFLIBAPI int GRfileinfo(int32 grid, int32 *n_datasets, int32 *n_attrs);

HDFLIBAPI int GRend(int32 grid);

HDFLIBAPI int32 GRcreate(int32 grid, const char *name, int32 ncomp, int32 nt, int32 il, int32 dimsizes[2]);

HDFLIBAPI int32 GRselect(int32 grid, int32 idx);

HDFLIBAPI int32 GRnametoindex(int32 grid, const char *name);

HDFLIBAPI int GRgetiminfo(int32 riid, char *name, int32 *ncomp, int32 *nt, int32 *il, int32 dimsizes[2],
                          int32 *n_attr);

HDFLIBAPI int GRgetnluts(int32 riid);

HDFLIBAPI int GRwriteimage(int32 riid, int32 start[2], int32 stride[2], int32 count[2], void *data);

HDFLIBAPI int GRreadimage(int32 riid, int32 start[2], int32 stride[2], int32 count[2], void *data);

HDFLIBAPI int GRendaccess(int32 riid);

HDFLIBAPI uint16 GRidtoref(int32 riid);

HDFLIBAPI int32 GRreftoindex(int32 grid, uint16 ref);

HDFLIBAPI int GRreqlutil(int32 riid, int il);

HDFLIBAPI int GRreqimageil(int32 riid, int il);

HDFLIBAPI int32 GRgetlutid(int32 riid, int32 idx);

HDFLIBAPI uint16 GRluttoref(int32 lutid);

HDFLIBAPI int GRgetlutinfo(int32 riid, int32 *ncomp, int32 *nt, int32 *il, int32 *nentries);

HDFLIBAPI int GRwritelut(int32 riid, int32 ncomps, int32 nt, int32 il, int32 nentries, void *data);

HDFLIBAPI int GRreadlut(int32 lutid, void *data);

HDFLIBAPI int GRsetexternalfile(int32 riid, const char *filename, int32 offset);

HDFLIBAPI int GRsetaccesstype(int32 riid, unsigned accesstype);

HDFLIBAPI int GRsetcompress(int32 riid, comp_coder_t comp_type, comp_info *cinfo);

HDFLIBAPI int GRgetcompress(int32 riid, comp_coder_t *comp_type, comp_info *cinfo);

HDFLIBAPI int GRgetcompinfo(int32 riid, comp_coder_t *comp_type, comp_info *cinfo);

HDFLIBAPI int GRsetattr(int32 id, const char *name, int32 attr_nt, int32 count, const void *data);

HDFLIBAPI int GRattrinfo(int32 id, int32 idx, char *name, int32 *attr_nt, int32 *count);

HDFLIBAPI int GRgetattr(int32 id, int32 idx, void *data);

HDFLIBAPI int32 GRfindattr(int32 id, const char *name);

HDFLIBAPI int GRgetcomptype(int32 riid, comp_coder_t *comp_type);

HDFLIBAPI int GRPshutdown(void);

/* This function was added for hmap project only.  Feb-25-2011 */
HDFLIBAPI int GR2bmapped(int32 riid, int *tobe_mapped, int *created_byGR);

/*=== HDF_CHUNK_DEF same as in mfhdf.h - moved here  ====*/

/* Bit flags used for SDsetchunk(), SDgetchunkinfo()
   GRsetchunk() and GRgetchunkinfo(). Note that GRs do not support NBIT */
#define HDF_NONE  0x0
#define HDF_CHUNK 0x1
#define HDF_COMP  0x3
#define HDF_NBIT  0x5

/* Cache flags */
#define HDF_CACHEALL 0x1

/* Chunk Definition, Note that GRs need only 2 dimensions for the chunk_lengths */
typedef union hdf_chunk_def_u {
    /* Chunk Lengths only */
    int32 chunk_lengths[H4_MAX_VAR_DIMS]; /* chunk lengths along each dimension */

    struct {                                       /* For Compression info */
        int32      chunk_lengths[H4_MAX_VAR_DIMS]; /* chunk lengths along each dimension */
        int32      comp_type;                      /* Compression type */
        int32      model_type;                     /* Compression model type */
        comp_info  cinfo;                          /* Compression info struct */
        model_info minfo;                          /* Compression model info struct */
    } comp;

    struct {                                  /* For NBIT, Used by SDS and not by GR */
        int32 chunk_lengths[H4_MAX_VAR_DIMS]; /* chunk lengths along each dimension */
        int   start_bit;                      /* offset of the start bit in the data */
        int   bit_len;                        /* number of bits to store */
        int   sign_ext;                       /* whether to sign extend or not */
        int   fill_one;                       /* whether to fill with 1's or not (0's) */
    } nbit;

} HDF_CHUNK_DEF;

/*=== GR Chunking Routines  ====*/

/******************************************************************************
 NAME
      GRsetchunk   -- make GR a chunked GR

 DESCRIPTION
      This routine makes the GR a chunked GR according to the chunk
      definition passed in.

      The image currently cannot be special already.  i.e. NBIT,
      COMPRESSED, or EXTERNAL. This is an Error.

      The definition of the HDF_CHUNK_DEF union with relevant fields is:

      typedef union hdf_chunk_def_u
      {
         int32   chunk_lengths[2];  Chunk lengths along each dimension

         struct
          {
            int32     chunk_lengths[2]; Chunk lengths along each dimension
            int32     comp_type;                   Compression type
            comp_info cinfo;                       Compression info struct
          }comp;

      } HDF_CHUNK_DEF

      The simplest is the 'chunk_lengths' array specifying chunk
      lengths for each dimension where the 'flags' argument set to
      'HDF_CHUNK';

      COMPRESSION is set by using the 'HDF_CHUNK_DEF' structure to set the
      appropriate compression information along with the required chunk lengths
      for each dimension. The compression information is the same as
      that set in 'SDsetcompress()'. The bit-or'd'flags' argument' is set to
      'HDF_CHUNK | HDF_COMP'.

      See the example in pseudo-C below for further usage.

      The maximum number of Chunks in an HDF file is 65,535.

      The performance of the GRxxx interface with chunking is greatly
      affected by the users access pattern over the image and by
      the maximum number of chunks set in the chunk cache. The cache contains
      the Least Recently Used(LRU cache replacement policy) chunks. See the
      routine GRsetchunkcache() for further info on the chunk cache and how
      to set the maximum number of chunks in the chunk cache. A default chunk
      cache is always created.

      The following example shows the organization of chunks for a 2D image.
      e.g. 4x4 image with 2x2 chunks. The array shows the layout of
           chunks in the chunk array.

            4 ---------------------
              |         |         |
        Y     |  (0,1)  |  (1,1)  |
        ^     |         |         |
        |   2 ---------------------
        |     |         |         |
        |     |  (0,0)  |  (1,0)  |
        |     |         |         |
        |     ---------------------
        |     0         2         4
        ---------------> X

        --Without compression--:
        {
        HDF_CHUNK_DEF chunk_def;

        .......
        -- Set chunk lengths --
        chunk_def.chunk_lengths[0]= 2;
        chunk_def.chunk_lengths[1]= 2;

        -- Set Chunking --
        GRsetchunk(riid, chunk_def, HDF_CHUNK);
         ......
        }

        --With compression--:
        {
        HDF_CHUNK_DEF chunk_def;

        .......
        -- Set chunk lengths first --
        chunk_def.chunk_lengths[0]= 2;
        chunk_def.chunk_lengths[1]= 2;

        -- Set compression --
        chunk_def.comp.cinfo.deflate.level = 9;
        chunk_def.comp.comp_type = COMP_CODE_DEFLATE;

        -- Set Chunking with Compression --
        GRsetchunk(riid, chunk_def, HDF_CHUNK | HDF_COMP);
         ......
        }

 RETURNS
        SUCCEED/FAIL
******************************************************************************/
HDFLIBAPI int GRsetchunk(int32         riid,      /* IN: raster access id */
                         HDF_CHUNK_DEF chunk_def, /* IN: chunk definition */
                         int32         flags /* IN: flags */);

/******************************************************************************
 NAME
     GRgetchunkinfo -- get Info on GR

 DESCRIPTION
     This routine gets any special information on the GR. If its chunked,
     chunked and compressed or just a regular GR. Currently it will only
     fill the array of chunk lengths for each dimension as specified in
     the 'HDF_CHUNK_DEF' union. You can pass in a NULL for 'chunk_def'
     if don't want the chunk lengths for each dimension.
     Additionally if successful it will return a bit-or'd value in 'flags'
     indicating if the GR is:

     Chunked                  -> flags = HDF_CHUNK
     Chunked and compressed   -> flags = HDF_CHUNK | HDF_COMP
     Non-chunked              -> flags = HDF_NONE

     e.g. 4x4 array - Pseudo-C
     {
     int32   rcdims[3];
     HDF_CHUNK_DEF rchunk_def;
     int32   cflags;
     ...
     rchunk_def.chunk_lengths = rcdims;
     GRgetchunkinfo(riid, &rchunk_def, &cflags);
     ...
     }

 RETURNS
        SUCCEED/FAIL
******************************************************************************/
HDFLIBAPI int GRgetchunkinfo(int32          riid,      /* IN: Raster access id */
                             HDF_CHUNK_DEF *chunk_def, /* IN/OUT: chunk definition */
                             int32         *flags /* IN/OUT: flags */);

/******************************************************************************
 NAME
     GRwritechunk  -- write the specified chunk to the GR

 DESCRIPTION
     This routine writes a whole chunk of data to the chunked GR
     specified by chunk 'origin' for the given GR and can be used
     instead of GRwriteimage() when this information is known. This
     routine has less overhead and is much faster than using GRwriteimage().

     Origin specifies the coordinates of the chunk according to the chunk
     position in the overall chunk array.

     'datap' must point to a whole chunk of data.

     See GRsetchunk() for a description of the organization of chunks in an GR.

 RETURNS
        SUCCEED/FAIL
******************************************************************************/
HDFLIBAPI int GRwritechunk(int32       riid,   /* IN: raster access id */
                           int32      *origin, /* IN: origin of chunk to write */
                           const void *datap /* IN: buffer for data */);

/******************************************************************************
 NAME
     GRreadchunk   -- read the specified chunk to the GR

 DESCRIPTION
     This routine reads a whole chunk of data from the chunked GR
     specified by chunk 'origin' for the given GR and can be used
     instead of GRreadimage() when this information is known. This
     routine has less overhead and is much faster than using GRreadimage().

     Origin specifies the coordinates of the chunk according to the chunk
     position in the overall chunk array.

     'datap' must point to a whole chunk of data.

     See GRsetchunk() for a description of the organization of chunks in an GR.

 RETURNS
        SUCCEED/FAIL
******************************************************************************/
HDFLIBAPI int GRreadchunk(int32  riid,   /* IN: raster access id */
                          int32 *origin, /* IN: origin of chunk to read */
                          void  *datap /* IN/OUT: buffer for data */);

/******************************************************************************
NAME
     GRsetchunkcache -- maximum number of chunks to cache

DESCRIPTION
     Set the maximum number of chunks to cache.

     The cache contains the Least Recently Used(LRU cache replacement policy)
     chunks. This routine allows the setting of maximum number of chunks that
     can be cached, 'maxcache'.

     The performance of the GRxxx interface with chunking is greatly
     affected by the users access pattern over the image and by
     the maximum number of chunks set in the chunk cache. The number chunks
     that can be set in the cache is process memory limited. It is a good
     idea to always set the maximum number of chunks in the cache as the
     default heuristic does not take into account the memory available for
     the application.

     By default when the GR is promoted to a chunked element the
     maximum number of chunks in the cache 'maxcache' is set to the number of
     chunks along the last dimension.

     The values set here affects the current object's caching behaviour.

     If the chunk cache is full and 'maxcache' is greater then the
     current 'maxcache' value, then the chunk cache is reset to the new
     'maxcache' value, else the chunk cache remains at the current
     'maxcache' value.

     If the chunk cache is not full, then the chunk cache is set to the
     new 'maxcache' value only if the new 'maxcache' value is greater than the
     current number of chunks in the cache.

     Use flags argument of 'HDF_CACHEALL' if the whole object is to be cached
     in memory, otherwise pass in zero(0). Currently you can only
     pass in zero.

    See GRsetchunk() for a description of the organization of chunks in an GR.

RETURNS
     Returns the 'maxcache' value for the chunk cache if successful
     and FAIL otherwise
******************************************************************************/
HDFLIBAPI int GRsetchunkcache(int32 riid,     /* IN: raster access id */
                              int32 maxcache, /* IN: max number of chunks to cache */
                              int32 flags /* IN: flags = 0, HDF_CACHEALL */);

/* Vset interface functions (used to be in vproto.h) */

/* Useful macros, which someday might become actual functions */
/*
 * macros for VSinquire
 * all these macros should be public for users
 */
#define VSQuerycount(vs, count)                                                                              \
    (VSinquire(vs, (int32 *)count, (int32 *)NULL, (char *)NULL, (int32 *)NULL, (char *)NULL))

#define VSQueryinterlace(vs, intr)                                                                           \
    (VSinquire(vs, (int32 *)NULL, (int32 *)intr, (char *)NULL, (int32 *)NULL, (char *)NULL))

#define VSQueryfields(vs, flds)                                                                              \
    (VSinquire(vs, (int32 *)NULL, (int32 *)NULL, (char *)flds, (int32 *)NULL, (char *)NULL))

#define VSQueryvsize(vs, size)                                                                               \
    (VSinquire(vs, (int32 *)NULL, (int32 *)NULL, (char *)NULL, (int32 *)size, (char *)NULL))

#define VSQueryname(vs, name)                                                                                \
    (VSinquire(vs, (int32 *)NULL, (int32 *)NULL, (char *)NULL, (int32 *)NULL, (char *)name))

/*
 ** from vattr.c
 */
HDFLIBAPI int Vsetattr(int32 vgid, const char *attrname, int32 datatype, int32 count, const void *values);
HDFLIBAPI int Vnattrs(int32 vgid);
HDFLIBAPI int Vnattrs2(int32 vgid);
HDFLIBAPI int Vnoldattrs(int32 vgid);
HDFLIBAPI int Vfindattr(int32 vgid, const char *attrname);
HDFLIBAPI int Vattrinfo(int32 vgid, int attrindex, char *name, int32 *datatype, int32 *count, int32 *size);
HDFLIBAPI int Vattrinfo2 /* copy of Vattrinfo for old attributes */
    (int32 vgid, int attrindex, char *name, int32 *datatype, int32 *count, int32 *size, int32 *nfields,
     uint16 *refnum);
HDFLIBAPI int Vgetattr(int32 vgid, int attrindex, void *values);
HDFLIBAPI int Vgetattr2 /* copy of Vgetattr for old attributes */
    (int32 vgid, int attrindex, void *values);
HDFLIBAPI int32 Vgetversion(int32 vgid);
HDFLIBAPI int   VSfindex(int32 vsid, const char *fieldname, int32 *fldindex);
HDFLIBAPI int   VSsetattr(int32 vsid, int32 findex, const char *attrname, int32 datatype, int32 count,
                          const void *values);
HDFLIBAPI int   VSnattrs(int32 vsid);
HDFLIBAPI int   VSfnattrs(int32 vsid, int32 findex);
HDFLIBAPI int   VSfindattr(int32 vsid, int32 findex, const char *attrname);
HDFLIBAPI int   VSattrinfo(int32 vsid, int32 findex, int attrindex, char *name, int32 *datatype, int32 *count,
                           int32 *size);
HDFLIBAPI int   VSgetattr(int32 vsid, int32 findex, int attrindex, void *values);
HDFLIBAPI int   VSisattr(int32 vsid);
/*
 ** from vconv.c
 */
HDFLIBAPI int32 vicheckcompat(HFILEID f);

HDFLIBAPI int32 vimakecompat(HFILEID f);

HDFLIBAPI int32 vcheckcompat(char *fs);

HDFLIBAPI int32 vmakecompat(char *fs);

/*
 ** from vg.c
 */
HDFLIBAPI int32 VSelts(int32 vkey);

HDFLIBAPI int32 VSgetinterlace(int32 vkey);

HDFLIBAPI int VSsetinterlace(int32 vkey, int32 interlace);

HDFLIBAPI int32 VSgetfields(int32 vkey, char *fields);

HDFLIBAPI int VSfexist(int32 vkey, char *fields);

HDFLIBAPI int32 VSsizeof(int32 vkey, char *fields);

HDFLIBAPI void VSdump(int32 vkey);

HDFLIBAPI int32 VSsetname(int32 vkey, const char *vsname);

HDFLIBAPI int32 VSsetclass(int32 vkey, const char *vsclass);

HDFLIBAPI int32 VSgetname(int32 vkey, char *vsname);

HDFLIBAPI int32 VSgetclass(int32 vkey, char *vsclass);

HDFLIBAPI int VSinquire(int32 vkey, int32 *nelt, int32 *interlace, char *fields, int32 *eltsize,
                        char *vsname);

HDFLIBAPI int VSisinternal(const char *vsclass);

HDFLIBAPI int32 VSlone(HFILEID f, int32 *idarray, int32 asize);

HDFLIBAPI int32 Vlone(HFILEID f, int32 *idarray, int32 asize);

HDFLIBAPI int32 Vfind(HFILEID f, const char *vgname);

HDFLIBAPI int32 VSfind(HFILEID f, const char *vsname);

HDFLIBAPI int32 Vfindclass(HFILEID f, const char *vgclass);

HDFLIBAPI int32 VSfindclass(HFILEID f, const char *vsclass);

HDFLIBAPI int VSofclass(int32 id, const char *vsclass, unsigned start_vd, unsigned array_size,
                        uint16 *refarray);

HDFLIBAPI int VSgetvdatas(int32 id, unsigned start_vd, unsigned array_size, uint16 *refarray);

HDFLIBAPI int VSsetblocksize(int32 vkey, int32 block_size);

HDFLIBAPI int VSsetnumblocks(int32 vkey, int32 num_blocks);

HDFLIBAPI int VSgetblockinfo(int32 vkey, int32 *block_size, int32 *num_blocks);

HDFLIBAPI void Vsetzap(void);

/*
 ** from vgp.c
 */
HDFLIBAPI int vcompare(void *k1, void *k2, int cmparg);

HDFLIBAPI int vcompareref(void *k1, void *k2, int cmparg);

HDFLIBAPI void vdestroynode(void *n);

HDFLIBAPI void vtfreekey(void *k);

HDFLIBAPI int Vinitialize(HFILEID f);

HDFLIBAPI int Vfinish(HFILEID f);

HDFLIBAPI HFILEID Vopen(char *path, int acc_mode, int16 ndds);

HDFLIBAPI int Vclose(HFILEID f);

HDFLIBAPI int32 vexistvg(HFILEID f, uint16 vgid);

HDFLIBAPI int32 Vattach(HFILEID f, int32 vgid, const char *accesstype);

HDFLIBAPI int32 Vdetach(int32 vkey);

HDFLIBAPI int32 Vinsert(int32 vkey, int32 vskey);
/* note: 2nd arg of Vinsert can also be (VGROUP *) */

HDFLIBAPI int32 Vflocate(int32 vkey, char *field);

HDFLIBAPI int Vinqtagref(int32 vkey, int32 tag, int32 ref);

HDFLIBAPI int32 Vntagrefs(int32 vkey);

HDFLIBAPI int32 Vnrefs(int32 vkey, int32 tag);

HDFLIBAPI int32 Vgettagrefs(int32 vkey, int32 tagarray[], int32 refarray[], int32 n);

HDFLIBAPI int32 Vgetuservgs(int32 id, int32 start_ref, int32 n_vgs, int32 *refarray);

HDFLIBAPI int Vgettagref(int32 vkey, int32 which, int32 *tag, int32 *ref);

HDFLIBAPI int32 VQueryref(int32 vkey);

HDFLIBAPI int32 VQuerytag(int32 vkey);

HDFLIBAPI int32 Vaddtagref(int32 vkey, int32 tag, int32 ref);

HDFLIBAPI int32 Ventries(HFILEID f, int32 vgid);

HDFLIBAPI int32 Vsetname(int32 vkey, const char *vgname);

HDFLIBAPI int32 Vsetclass(int32 vkey, const char *vgclass);

HDFLIBAPI int Visvg(int32 vkey, int32 id);

HDFLIBAPI int Visvs(int32 vkey, int32 id);

HDFLIBAPI int32 Vgetid(HFILEID f, int32 vgid);

HDFLIBAPI int32 Vgetnext(int32 vkey, int32 id);

HDFLIBAPI int32 Vgetname(int32 vkey, char *vgname);

HDFLIBAPI int32 Vgetnamelen(int32 vkey, uint16 *name_len);

HDFLIBAPI int32 Vgetclassnamelen(int32 vkey, uint16 *classname_len);

HDFLIBAPI int32 Vgetclass(int32 vkey, char *vgclass);

HDFLIBAPI int Vinquire(int32 vkey, int32 *nentries, char *vgname);

HDFLIBAPI int32 Vdelete(int32 f, int32 ref);

HDFLIBAPI int Vgisinternal(int32 vkey);

HDFLIBAPI int Visinternal /* this function is replaced by Vgisinternal */
    (const char *classname);

HDFLIBAPI int Vgetvgroups(int32 id, unsigned start_vg, unsigned n_vgs, uint16 *refarray);

/*******************************************************************************
NAME
   Vdeletetagref - delete tag/ref pair in Vgroup

DESCRIPTION
    Deletes the given tag/ref pair from the Vgroup.  If the given tag/ref pair
    does not exist in the vgroup the routine will return FAIL. Users should use
    Vinqtagref() to check if the tag/ref pair exists before deleting.

RETURNS
    Returns SUCCEED if the tag/ref pair is deleted from Vgroup and
    FAIL if unsuccessful.

*******************************************************************************/
HDFLIBAPI int Vdeletetagref(int32 vkey, /* IN: vgroup key */
                            int32 tag,  /* IN: tag to delete in vgroup */
                            int32 ref /* IN: ref to delete in vgroup */);

HDFLIBAPI int VPshutdown(void);

/*
 ** from vparse.c
 */
HDFLIBAPI int32 scanattrs(const char *attrs, int32 *attrc, char ***attrv);

/*
 ** from vhi.c
 */
HDFLIBAPI int32 VHstoredata(HFILEID f, const char *field, const uint8 *buf, int32 n, int32 datatype,
                            const char *vsname, const char *vsclass);

HDFLIBAPI int32 VHstoredatam(HFILEID f, const char *field, const uint8 *buf, int32 n, int32 datatype,
                             const char *vsname, const char *vsclass, int32 order);

HDFLIBAPI int32 VHmakegroup(HFILEID f, int32 tagarray[], int32 refarray[], int32 n, const char *vgname,
                            const char *vgclass);

/*
 ** from vio.c
 */

HDFLIBAPI int VSPhshutdown(void);

HDFLIBAPI int32 vexistvs(HFILEID f, uint16 vsref);

HDFLIBAPI void vsdestroynode(void *n);

HDFLIBAPI void vfdestroynode(void *n);

HDFLIBAPI int32 VSattach(HFILEID f, int32 vsref, const char *accesstype);

HDFLIBAPI int32 VSdetach(int32 vkey);

HDFLIBAPI int32 VSQuerytag(int32 vkey);

HDFLIBAPI int32 VSQueryref(int32 vkey);

HDFLIBAPI int32 VSgetid(HFILEID f, int32 vsref);

HDFLIBAPI int32 VSgetversion(int32 vkey);

HDFLIBAPI int32 VSdelete(int32 f, int32 ref);

HDFLIBAPI int32 VSappendable(int32 vkey, int32 blk);

/*
 ** from vsfld.c
 */

HDFLIBAPI int VSsetfields(int32 vkey, const char *fields);

HDFLIBAPI int VSfdefine(int32 vkey, const char *field, int32 localtype, int32 order);

HDFLIBAPI int32 VFnfields(int32 vkey);

HDFLIBAPI char *VFfieldname(int32 vkey, int32 idx);

HDFLIBAPI int32 VFfieldtype(int32 vkey, int32 idx);

HDFLIBAPI int32 VFfieldisize(int32 vkey, int32 idx);

HDFLIBAPI int32 VFfieldesize(int32 vkey, int32 idx);

HDFLIBAPI int32 VFfieldorder(int32 vkey, int32 idx);

HDFLIBAPI int VSsetexternalfile(int32 vkey, const char *filename, int32 offset);

HDFLIBAPI int VSgetexternalfile(int32 vkey, unsigned name_len, char *filename, int32 *offset);

HDFLIBAPI int VSgetexternalinfo(int32 vkey, unsigned name_len, char *filename, int32 *offset, int32 *length);

HDFLIBAPI int VSfpack(int32 vsid, int packtype, const char *fields_in_buf, void *buf, int bufsz,
                      int n_records, const char *fields, void *fldbufpt[]);

/*
 ** from vrw.c
 */
HDFLIBAPI int VSPshutdown(void);

HDFLIBAPI int32 VSseek(int32 vkey, int32 eltpos);

HDFLIBAPI int32 VSread(int32 vkey, uint8 buf[], int32 nelt, int32 interlace);

HDFLIBAPI int32 VSwrite(int32 vkey, const uint8 buf[], int32 nelt, int32 interlace);

#ifdef __cplusplus
}
#endif

#endif /* H4_H_PROTO */

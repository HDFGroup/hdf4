
/****************************************************************************
 * NCSA HDF                                                                 *
 * Software Development Group                                               *
 * National Center for Supercomputing Applications                          *
 * University of Illinois at Urbana-Champaign                               *
 * 605 E. Springfield, Champaign IL 61820                                   *
 *                                                                          *
 * For conditions of distribution and use, see the accompanying             *
 * hdf/COPYING file.                                                        *
 *                                                                          *
 ****************************************************************************/

/* $Id$ */

#ifndef _H_PROTO
#define _H_PROTO

#if defined c_plusplus || defined __cplusplus
extern      "C"
{
#endif                          /* c_plusplus || __cplusplus */

/*
   ** from hfile.c
 */
    extern int32 Hopen
                (const char _HUGE * path, intn acc_mode, int16 ndds);

    extern intn Hclose
                (int32 file_id);

    extern int32 Hstartread
                (int32 file_id, uint16 tag, uint16 ref);

    extern intn Hnextread
                (int32 access_id, uint16 tag, uint16 ref, intn origin);

    extern intn Hfind
                (int32 file_id, uint16 search_tag, uint16 search_ref,
                 uint16 *find_tag, uint16 *find_ref, int32 *find_offset,
                 int32 *find_length, intn direction);

    extern intn Hexist
                (int32 file_id, uint16 search_tag, uint16 search_ref);

    extern intn Hinquire
                (int32 access_id, int32 _HUGE * pfile_id, uint16 _HUGE * ptag,
          uint16 _HUGE * pref, int32 _HUGE * plength, int32 _HUGE * poffset,
        int32 _HUGE * pposn, int16 _HUGE * paccess, int16 _HUGE * pspecial);

    extern int32 Hstartwrite
                (int32 file_id, uint16 tag, uint16 ref, int32 length);

    extern int32 Hstartaccess
                (int32 file_id, uint16 tag, uint16 ref, uint32 flags);

    extern intn Hsetlength
                (int32 file_id, int32 length);

    extern intn Happendable
                (int32 aid);

    extern intn HPisappendable
                (int32 aid);

    extern intn Hseek
                (int32 access_id, int32 offset, intn origin);

    extern int32 Hread
                (int32 access_id, int32 length, VOIDP data);

    extern int32 Hwrite
                (int32 access_id, int32 length, const VOIDP data);

    extern int32 Htrunc
                (int32 access_id, int32 trunc_len);

    extern intn Hendaccess
                (int32 access_id);

    extern intn HDgetc
                (int32 access_id);

    extern intn HDputc
                (uint8 c, int32 access_id);

    extern int32 Hgetelement
                (int32 file_id, uint16 tag, uint16 ref, uint8 _HUGE * data);

    extern int32 Hputelement
                (int32 file_id, uint16 tag, uint16 ref, uint8 _HUGE * data, int32 length);

    extern int32 Hlength
                (int32 file_id, uint16 tag, uint16 ref);

    extern int32 Hoffset
                (int32 file_id, uint16 tag, uint16 ref);

    extern int  Hdupdd
                (int32 file_id, uint16 tag, uint16 ref, uint16 old_tag, uint16 old_ref);

    extern int  Hdeldd
                (int32 file_id, uint16 tag, uint16 ref);

    extern uint16 Hnewref
                (int32 file_id);

    extern intn Hsync
                (int32 file_id);

    extern intn Hcache
                (int32 file_id, intn cache_on);

    extern int32 Hnumber
                (int32 file_id, uint16 tag);

    extern intn Hgetlibversion
                (uint32 _HUGE * majorv, uint32 _HUGE * minorv,
                 uint32 _HUGE * releasev, char _HUGE * string);

    extern intn Hgetfileversion
                (int32 file_id, uint32 _HUGE * majorv, uint32 _HUGE * minorv,
                 uint32 _HUGE * release, char _HUGE * string);

#if defined WIN3
    extern int32 HDfreadbig
                (VOIDP buffer, int32 size, HFILE fp);

    extern int32 HDfwritebig
                (VOIDP buffer, int32 size, HFILE fp);

#else                           /* !WIN3 */
#if defined PC
    extern int32 HDfreadbig
                (VOIDP buffer, int32 size, FILE _HUGE * fp);

    extern int32 HDfwritebig
                (VOIDP buffer, int32 size, FILE _HUGE * fp);
#endif                          /* PC */
#endif                          /* !WIN3 */

    extern uint16 HDmake_special_tag
                (uint16 tag);

    extern intn HDis_special_tag
                (uint16 tag);

    extern uint16 HDbase_tag
                (uint16 tag);

    extern int  HDerr
                (int32 file_id);

    extern intn HDvalidfid
                (int32 file_id);

    extern const char _HUGE *HDgettagname
                (uint16 tag);

    extern char _HUGE *HDgettagsname
                (uint16 tag);

    extern intn HDgettagnum
                (const char *tag_name);

    extern char _HUGE *HDgetNTdesc
                (int32 nt);

    extern const char _HUGE *HDfidtoname
                (int32 fid);

    extern intn Hishdf
                (const char _HUGE * filename);

    extern intn Hfidinquire
                (int32 file_id, char _HUGE ** fname, intn _HUGE * acc_mode,
                 intn _HUGE * attach);

/*
   ** from hdfalloc.c
 */

    extern VOIDP HDmemfill
                (VOIDP dest, const VOIDP src, uint32 item_size, uint32 num_items);

    extern char _HUGE *HIstrncpy
                (register char _HUGE * dest, register const char _HUGE * source, int32 len);

    extern int32 HDspaceleft
                (void);

#if (defined PC && !defined PC386) || defined MALLOC_CHECK
    extern VOIDP HDgetspace
                (uint32 qty);

    extern VOIDP HDregetspace
                (VOIDP where, uint32 qty);

    extern VOIDP HDclearspace
                (uint32 n, uint32 size);

    extern void HDfreespace
                (VOIDP ptr);

#endif                          /* (defined PC && !defined PC386) || defined MALLOC_CHECK */

#if defined PC & !defined PC386
    extern VOIDP fmemcpy_big
                (VOIDP dest, const VOIDP source, uint32 len);

    extern VOIDP fmemset_big
                (VOIDP dest, intn c, uint32 len);

    extern intn fmemcmp_big
                (VOIDP s1, VOIDP s2, uint32 len);

    extern VOIDP memcpy_big
                (VOIDP dest, VOIDP source, uint32 len);

    extern VOIDP memset_big
                (VOIDP dest, intn c, uint32 len);

    extern intn memcmp_big
                (VOIDP s1, VOIDP s2, uint32 len);

#endif                          /* WIN3 | PC */

#if defined VMS | (defined PC & !defined PC386) | defined macintosh | defined MIPSEL | defined NEXT
    extern char *HDstrdup
                (const char *s);

#endif

    extern intn HDc2fstr
                (char _HUGE * str, intn len);

    extern char _HUGE *HDf2cstring
                (_fcd fdesc, intn len);

    extern intn HDflush
                (int32 file_id);

    extern intn HDpackFstring
                (char _HUGE * src, char _HUGE * dest, intn len);

/*
   ** from hblocks.c
 */
    extern int32 HLcreate
                (int32 file_id, uint16 tag, uint16 ref, int32 block_length,
                 int32 number_blocks);

    intn        HLconvert
                (int32 aid, int32 block_length, int32 number_blocks);

    extern int  HDinqblockinfo
                (int32 aid, int32 *length, int32 *first_length, int32 *block_length,
                 int32 *number_blocks);

/*
   ** from hextelt.c
 */
    extern int32 HXcreate
                (int32 file_id, uint16 tag, uint16 ref, const char _HUGE * extern_file_name,
                 int32 offset, int32 start_len);

/*
   ** from hcomp.c
 */
    extern int32 HCcreate
                (int32 file_id, uint16 tag, uint16 ref,
                 comp_model_t model_type, model_info _HUGE * m_info,
                 comp_coder_t coder_type, comp_info _HUGE * c_info);

/*
   ** from hbigext.c
 */
    extern int32 HBcreate
                (int32 file_id, uint16 tag, uint16 ref, const char _HUGE * extern_file_name,
                 int32 offset, int32 start_len);

/*
   ** from herr.c
 */
    extern const char _HUGE *HEstring
                (hdf_err_code_t error_code);

    extern VOID HEpush
                (hdf_err_code_t error_code, const char _HUGE * function_name,
                 const char _HUGE * file_name, intn line);

/* #ifndef _H_ERR_MASTER_ */
    extern VOID HEreport
                (const char _HUGE *,...);
    /* #endif *//* _H_ERR_MASTER_ */

    extern VOID HEprint
                (FILE _HUGE * stream, int32 print_level);

    extern int16 HEvalue
                (int32 level);

    extern VOID HEclear
                (void);

/*
   ** from hbitio.c
 */
    extern int32 Hstartbitread
                (int32 file_id, uint16 tag, uint16 ref);

    extern int32 Hstartbitwrite
                (int32 file_id, uint16 tag, uint16 ref, int32 length);

    extern intn Hbitappendable
                (int32 bitid);

    extern intn Hbitwrite
                (int32 bitid, intn count, uint32 data);

    extern intn Hbitread
                (int32 bitid, intn count, uint32 *data);

    extern intn Hbitseek
                (int32 bitid, int32 byte_offset, intn bit_offset);

    extern intn Hgetbit
                (int32 bitid);

    extern intn Hputbit
                (int32 bitid, intn bit);

    extern int32 Hendbitaccess
                (int32 bitfile_id, intn flushbit);

/*
   ** from dfcomp.c
 */
    extern intn DFputcomp
                (int32 file_id, uint16 tag, uint16 ref, uint8 _HUGE * image,
        int32 xdim, int32 ydim, uint8 _HUGE * palette, uint8 _HUGE * newpal,
                 int16 scheme, comp_info * cinfo);

    extern int  DFgetcomp
                (int32 file_id, uint16 tag, uint16 ref, uint8 _HUGE * image,
                 int32 xdim, int32 ydim, uint16 scheme);

/*
   ** from dfrle.c
 */
    extern int32 DFCIrle
                (VOIDP buf, VOIDP bufto, int32 len);

    extern int32 DFCIunrle
                (uint8 _HUGE * buf, uint8 *bufto, int32 outlen, int resetsave);

/*
   ** from dfimcomp.c
 */
    extern VOID DFCIimcomp
                (int32 xdim, int32 ydim, uint8 _HUGE in[], uint8 _HUGE out[],
                 uint8 _HUGE in_pal[], uint8 _HUGE out_pal[], int mode);

    extern VOID DFCIunimcomp
                (int32 xdim, int32 ydim, uint8 _HUGE in[], uint8 _HUGE out[]);

/*
   ** from dfjpeg.c
 */

    extern intn DFCIjpeg
                (int32 file_id, uint16 tag, uint16 ref, int32 xdim, int32 ydim,
                 VOIDP image, int16 scheme, comp_info * scheme_info);

/*
   ** from dfunjpeg.c
 */

    extern intn DFCIunjpeg
                (int32 file_id, uint16 tag, uint16 ref, VOIDP image, int32 xdim,
                 int32 ydim, int16 scheme);

/*
   ** from dfgroup.c
 */
    extern int32 DFdiread
                (int32 file_id, uint16 tag, uint16 ref);

    extern intn DFdiget
                (int32 list, uint16 _HUGE * ptag, uint16 _HUGE * pref);

    extern intn DFdinobj
                (int32 list);

    extern int32 DFdisetup
                (int maxsize);

    extern intn DFdiput
                (int32 list, uint16 tag, uint16 ref);

    extern intn DFdiwrite
                (int32 file_id, int32 list, uint16 tag, uint16 ref);

/*
   ** from dfp.c
 */
    extern intn DFPgetpal
                (const char _HUGE * filename, VOIDP palette);

    extern intn DFPputpal
                (const char _HUGE * filename, VOIDP palette, intn overwrite, const char _HUGE * filemode);

    extern intn DFPaddpal
                (const char _HUGE * filename, VOIDP palette);

    extern intn DFPnpals
                (const char _HUGE * filename);

    extern intn DFPreadref
                (const char _HUGE * filename, uint16 ref);

    extern intn DFPwriteref
                (const char _HUGE * filename, uint16 ref);

    extern intn DFPrestart
                (void);

    extern uint16 DFPlastref
                (void);

/*
   ** from dfr8.c
 */
    extern int  DFR8setcompress
                (int32 scheme, comp_info * cinfo);

    extern intn DFR8getdims
                (const char _HUGE * filename, int32 _HUGE * pxdim, int32 _HUGE * pydim,
                 int _HUGE * pispal);

    extern intn DFR8getimage
                (const char _HUGE * filename, uint8 _HUGE * image, int32 xdim, int32 ydim,
                 uint8 _HUGE * pal);

    extern intn DFR8setpalette
                (uint8 _HUGE * pal);

    extern intn DFR8putimage
                (const char _HUGE * filename, VOIDP image, int32 xdim, int32 ydim, uint16 compress);

    extern intn DFR8addimage
                (const char _HUGE * filename, VOIDP image, int32 xdim, int32 ydim, uint16 compress);

    extern intn DFR8nimages
                (const char _HUGE * filename);

    extern intn DFR8readref
                (const char _HUGE * filename, uint16 ref);

    extern intn DFR8writeref
                (const char _HUGE * filename, uint16 ref);

    extern intn DFR8restart
                (void);

    extern uint16 DFR8lastref
                (void);

/*
   ** from dfgr.c
 */
    extern intn DFGRgetlutdims
                (const char _HUGE * filename, int32 _HUGE * pxdim, int32 _HUGE * pydim,
                 intn _HUGE * pncomps, intn _HUGE * pil);

    extern intn DFGRreqlutil
                (intn il);

    extern intn DFGRgetlut
                (const char _HUGE * filename, VOIDP lut, int32 xdim, int32 ydim);

    extern intn DFGRgetimdims
                (const char _HUGE * filename, int32 _HUGE * pxdim, int32 _HUGE * pydim,
                 intn _HUGE * pncomps, intn _HUGE * pil);

    extern intn DFGRreqimil
                (intn il);

    extern intn DFGRgetimage
                (const char _HUGE * filename, VOIDP image, int32 xdim, int32 ydim);

    extern intn DFGRsetcompress
                (int32 scheme, comp_info * cinfo);

    extern intn DFGRsetlutdims
                (int32 xdim, int32 ydim, intn ncomps, intn il);

    extern intn DFGRsetlut
                (VOIDP lut, int32 xdim, int32 ydim);

    extern intn DFGRaddlut
                (const char _HUGE * filename, VOIDP lut, int32 xdim, int32 ydim);

    extern intn DFGRsetimdims
                (int32 xdim, int32 ydim, intn ncomps, intn il);

    extern intn DFGRaddimage
                (const char _HUGE * filename, VOIDP image, int32 xdim, int32 ydim);

    extern intn DFGRputimage
                (const char _HUGE * filename, VOIDP image, int32 xdim, int32 ydim);

    extern intn DFGRreadref
                (const char _HUGE * filename, uint16 ref);

    extern uint16 DFGRIlastref
                (void);

    extern intn DFGRIgetdims
                (const char _HUGE * filename, int32 _HUGE * pxdim, int32 _HUGE * pydim,
                 intn _HUGE * pncomps, intn _HUGE * pil, intn type);

    extern intn DFGRIreqil
                (intn il, intn type);

    extern intn DFGRIgetimlut
                (const char _HUGE * filename, VOIDP imlut, int32 xdim, int32 ydim, intn type,
                 intn isfortran);

    extern intn DFGRIsetdims
                (int32 xdim, int32 ydim, intn ncomps, intn type);

    extern intn DFGRIsetil
                (intn il, intn type);

    extern intn DFGRIrestart
                (void);

    extern intn DFGRIaddimlut
                (const char _HUGE * filename, VOIDP imlut, int32 xdim, int32 ydim, intn type,
                 intn isfortran, intn newfile);

/*
   ** from df24.c
 */
    extern intn DF24getdims
                (const char _HUGE * filename, int32 _HUGE * pxdim, int32 _HUGE * pydim,
                 intn _HUGE * pil);

    extern intn DF24reqil
                (intn il);

    extern intn DF24getimage
                (const char _HUGE * filename, VOIDP image, int32 xdim, int32 ydim);

    extern intn DF24setdims
                (int32 xdim, int32 ydim);

    extern intn DF24setil
                (intn il);

    extern intn DF24setcompress
                (int32 type, comp_info * cinfo);

    extern intn DF24restart
                (void);

    extern intn DF24addimage
                (const char _HUGE * filename, VOIDP image, int32 xdim, int32 ydim);

    extern intn DF24putimage
                (const char _HUGE * filename, VOIDP image, int32 xdim, int32 ydim);

    extern intn DF24nimages
                (const char _HUGE * filename);

    extern intn DF24readref
                (const char _HUGE * filename, uint16 ref);

    extern uint16 DF24lastref
                (void);

/*
   ** from dfan.c
 */

    extern int32 DFANgetlablen
                (const char _HUGE * filename, uint16 tag, uint16 ref);

    extern intn DFANgetlabel
                (const char _HUGE * filename, uint16 tag, uint16 ref, char _HUGE * label,
                 int32 maxlen);

    extern int32 DFANgetdesclen
                (const char _HUGE * filename, uint16 tag, uint16 ref);

    extern intn DFANgetdesc
                (const char _HUGE * filename, uint16 tag, uint16 ref, char _HUGE * desc,
                 int32 maxlen);

    extern int32 DFANgetfidlen
                (int32 file_id, intn isfirst);

    extern int32 DFANgetfid
                (int32 file_id, char _HUGE * id, int32 maxlen, intn isfirst);

    extern int32 DFANgetfdslen
                (int32 file_id, intn isfirst);

    extern int32 DFANgetfds
                (int32 file_id, char _HUGE * desc, int32 maxlen, intn isfirst);

    extern intn DFANputlabel
                (const char _HUGE * filename, uint16 tag, uint16 ref, char _HUGE * label);

    extern intn DFANputdesc
                (const char _HUGE * filename, uint16 tag, uint16 ref, char _HUGE * desc,
                 int32 desclen);

    extern intn DFANaddfid
                (int32 file_id, char _HUGE * id);

    extern intn DFANaddfds
                (int32 file_id, char _HUGE * desc, int32 desclen);

    extern uint16 DFANlastref
                (void);

    extern intn DFANlablist
                (const char _HUGE * filename, uint16 tag, uint16 _HUGE reflist[],
         char _HUGE * labellist, intn listsize, intn maxlen, intn startpos);

    extern uint16 DFANIlocate
                (int32 file_id, int type, uint16 tag, uint16 ref);

    extern int  DFANIaddentry
                (int type, uint16 annref, uint16 datatag, uint16 dataref);

    extern int32 DFANIgetannlen
                (const char _HUGE * filename, uint16 tag, uint16 ref, int type);

    extern intn DFANIgetann
                (const char _HUGE * filename, uint16 tag, uint16 ref, uint8 _HUGE * ann,
                 int32 maxlen, int type);

    extern intn DFANIputann
                (const char _HUGE * filename, uint16 tag, uint16 ref, uint8 _HUGE * ann,
                 int32 annlen, int type);

    extern int  DFANIlablist
                (const char _HUGE * filename, uint16 tag, uint16 _HUGE reflist[],
            uint8 _HUGE * labellist, int listsize, int maxlen, int startpos,
                 int isfortran);

    extern int  DFANIaddfann
                (int32 file_id, char _HUGE * ann, int32 annlen, int type);

    extern int32 DFANIgetfannlen
                (int32 file_id, int type, int isfirst);

    extern int32 DFANIgetfann
                (int32 file_id, char _HUGE * ann, int32 maxlen, int type, int isfirst);

/*
   ** from dfsd.c
 */

    extern int  DFSDgetdims
                (const char _HUGE * filename, intn _HUGE * prank, int32 _HUGE sizes[], intn maxrank);

    extern int  DFSDgetdatastrs
                (char _HUGE * label, char _HUGE * unit, char _HUGE * format, char _HUGE * coordsys);

    extern int  DFSDgetdimstrs
                (int dim, char _HUGE * label, char _HUGE * unit, char _HUGE * format);

    extern int  DFSDgetdatalen
                (int _HUGE * llabel, int _HUGE * lunit, int _HUGE * lformat, int _HUGE * lcoordsys);

    extern int  DFSDgetdimlen
                (int dim, int _HUGE * llabel, int _HUGE * lunit, int _HUGE * lformat);

    extern int  DFSDgetdimscale
                (intn dim, int32 maxsize, VOIDP scale);

    extern int  DFSDgetrange
                (VOIDP pmax, VOIDP pmin);

    extern int  DFSDgetdata
                (const char _HUGE * filename, intn rank, int32 _HUGE maxsizes[], VOIDP data);

    extern int  DFSDsetlengths
                (int maxlen_label, int maxlen_unit, int maxlen_format,
                 int maxlen_coordsys);

    extern int  DFSDsetdims
                (intn rank, int32 _HUGE dimsizes[]);

    extern int  DFSDsetdatastrs
                (const char _HUGE * label, const char _HUGE * unit, const char _HUGE * format, const char _HUGE * coordsys);

    extern int  DFSDsetdimstrs
                (int dim, const char _HUGE * label, const char _HUGE * unit, const char _HUGE * format);

    extern int  DFSDsetdimscale
                (intn dim, int32 dimsize, VOIDP scale);

    extern int  DFSDsetrange
                (VOIDP maxi, VOIDP mini);

    extern int  DFSDputdata
                (const char _HUGE * filename, intn rank, int32 _HUGE dimsizes[], VOIDP data);

    extern int  DFSDadddata
                (const char _HUGE * filename, intn rank, int32 _HUGE dimsizes[], VOIDP data);

    extern int  DFSDrestart
                (void);

    extern int32 DFSDndatasets
                (char _HUGE * filename);

    extern int  DFSDclear
                (void);

    extern uint16 DFSDlastref
                (void);

    extern int  DFSDreadref
                (char _HUGE * filename, uint16 ref);

    extern int  DFSDgetslice
                (const char _HUGE * filename, int32 _HUGE winst[], int32 _HUGE windims[], VOIDP data,
                 int32 _HUGE dims[]);

    extern int  DFSDstartslice
                (const char _HUGE * filename);

    extern int  DFSDputslice
                (int32 _HUGE winend[], VOIDP data, int32 _HUGE dims[]);

    extern int  DFSDendslice
                (void);

    extern int  DFSDsetNT
                (int32 numbertype);

    extern int  DFSDsetorder
                (int arrayorder);

    extern int  DFSDgetNT
                (int32 _HUGE * pnumbertype);

    extern intn DFSDpre32sdg
                (char _HUGE * filename, uint16 ref, intn _HUGE * ispre32);

    extern int  DFSDsetcal
                (float64 cal, float64 cal_err, float64 ioff,
                 float64 ioff_err, int32 cal_nt);

    extern int  DFSDgetcal
                (float64 _HUGE * pcal, float64 _HUGE * pcal_err, float64 _HUGE * pioff,
                 float64 _HUGE * pioff_err, int32 _HUGE * cal_nt);

    extern int  DFSDwriteref
                (const char _HUGE * filename, uint16 ref);

    extern int  DFSDsetfillvalue
                (VOIDP fill_value);

    extern int  DFSDgetfillvalue
                (VOIDP fill_value);

    extern int  DFSDstartslab
                (const char _HUGE * filename);

    extern int  DFSDwriteslab
                (int32 _HUGE start[], int32 _HUGE stride[], int32 _HUGE count[],
                 VOIDP data);

    extern int  DFSDendslab
                (void);

    extern int  DFSDreadslab
                (const char *filename, int32 _HUGE start[], int32 _HUGE slab_size[],
             int32 _HUGE stride[], VOIDP buffer, int32 _HUGE buffer_size[]);

/*
   ** from dfconv.c
 */

    extern int  DFKNTsize
                (int32 number_type);

    extern int32 DFKisnativeNT
                (int32 numbertype);

    extern int32 DFKislitendNT
                (int32 numbertype);

    extern int8 DFKgetPNSC
                (int32 numbertype, int32 machinetype);

    extern intn DFKsetNT
                (int32 ntype);

    extern int32 DFKconvert
                (VOIDP source, VOIDP dest, int32 ntype, int32 num_elm,
                 int16 acc_mode, int32 source_stride, int32 dest_stride);

/*
   ** from dfknat.c
 */

    extern intn DFKnb1b
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKnb2b
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKnb4b
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKnb8b
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

/*
   ** from dfkswap.c
 */

    extern intn DFKsb2b
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKsb4b
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKsb8b
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

/*
   ** from dfkcray.c
 */

    extern intn DFKui2i
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKui2s
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKuo2i
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKuo2s
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKui4i
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKui4s
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKuo4i
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKuo4s
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKui4f
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKuo4f
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKui8f
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKuo8f
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKlui2i
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKlui2s
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKluo2i
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKluo2s
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKlui4i
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKlui4s
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKluo4i
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKluo4s
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKlui4f
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKluo4f
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKlui8f
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKluo8f
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

/*
   ** from dfkvms.c
 */

    extern intn DFKvi4f
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKvo4f
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKvi8f
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKvo8f
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKlvi4f
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKlvo4f
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKlvi8f
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKlvo8f
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

/*
   ** from dfkconv.c
 */

    extern intn DFKci4f
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKco4f
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKci8f
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKco8f
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKlci4f
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKlco4f
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKlci8f
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKlco8f
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

/*
   ** from dfkfuji.c
 */

    extern intn DFKpi4f
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKpo4f
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKpi8f
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKpo8f
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKlpi4f
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKlpo4f
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKlpi8f
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    extern intn DFKlpo8f
                (VOIDP s, VOIDP d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

/*
   ** from dfanF.c
 */
#ifndef DFAN_FNAMES
#   define  DFAN_FNAMES
#ifdef DF_CAPFNAMES
#   define ndaiganl  FNAME(DAIGANL)
#   define ndaigann  FNAME(DAIGANN)
#   define ndaipann  FNAME(DAIPANN)
#   define ndailist  FNAME(DAILIST)
#   define ndalref   FNAME(DALREF)
#   define ndfanlastref     FNAME(DFANLASTREF)

#   define ndfanaddfds      FNAME(DFANADDFDS)
#   define ndfangetfidlen   FNAME(DFANGETFIDLEN)
#   define ndfangetfdslen   FNAME(DFANGETFDSLEN)
#   define ndfangetfid      FNAME(DFANGETFID)
#   define ndfangetfds      FNAME(DFANGETFDS)
#   define ndaafds          FNAME(DAAFDS)
#   define ndagfidl         FNAME(DAGFIDL)
#   define ndagfdsl         FNAME(DAGFDSL)
#   define ndagfid          FNAME(DAGFID)
#   define ndagfds          FNAME(DAGFDS)
#   define ndaiafid         FNAME(DAIAFID)
#else                           /* DF_CAPFNAMES */
#   define ndaiganl  FNAME(daiganl)
#   define ndaigann  FNAME(daigann)
#   define ndaipann  FNAME(daipann)
#   define ndailist  FNAME(dailist)
#   define ndalref   FNAME(dalref)
#   define ndfanlastref     FNAME(dfanlastref)

#   define ndfanaddfds      FNAME(dfanaddfds)
#   define ndfangetfidlen   FNAME(dfangetfidlen)
#   define ndfangetfdslen   FNAME(dfangetfdslen)
#   define ndfangetfid      FNAME(dfangetfid)
#   define ndfangetfds      FNAME(dfangetfds)
#   define ndaafds          FNAME(daafds)
#   define ndagfidl         FNAME(dagfidl)
#   define ndagfdsl         FNAME(dagfdsl)
#   define ndagfid          FNAME(dagfid)
#   define ndagfds          FNAME(dagfds)
#   define ndaiafid         FNAME(daiafid)
#endif                          /* DF_CAPFNAMES */
#endif                          /* DFAN_FNAMES */

    extern      FRETVAL(intf) ndaiganl
                (_fcd filename, intf _HUGE * tag, intf _HUGE * ref, intf _HUGE * type,
                 intf _HUGE * fnlen);

    extern      FRETVAL(intf) ndaigann
                (_fcd filename, intf _HUGE * tag, intf _HUGE * ref, _fcd annotation,
                 intf _HUGE * maxlen, intf _HUGE * type, intf _HUGE * fnlen);

    extern      FRETVAL(intf) ndaipann
                (_fcd filename, intf _HUGE * tag, intf _HUGE * ref, _fcd annotation,
                 intf _HUGE * annlen, intf _HUGE * type, intf _HUGE * fnlen);

    extern      FRETVAL(intf) ndailist
                (_fcd filename, intf _HUGE * tag, intf _HUGE reflist[], _fcd labellist,
          intf _HUGE * listsize, intf _HUGE * maxlen, intf _HUGE * startpos,
                 intf _HUGE * fnlen);

    extern      FRETVAL(intf) ndalref
                (void);

    extern      FRETVAL(intf) ndfanlastref
                (void);

    extern      FRETVAL(intf) ndfanaddfds
                (intf _HUGE * dfile, _fcd desc, intf _HUGE * desclen);

    extern      FRETVAL(intf) ndfangetfidlen
                (intf _HUGE * dfile, intf _HUGE * isfirst);

    extern      FRETVAL(intf) ndfangetfdslen
                (intf _HUGE * dfile, intf _HUGE * isfirst);

    extern      FRETVAL(intf) ndfangetfid
                (intf _HUGE * dfile, _fcd id, intf _HUGE * maxlen, intf _HUGE * isfirst);

    extern      FRETVAL(intf) ndfangetfds
                (intf _HUGE * dfile, _fcd id, intf _HUGE * maxlen, intf _HUGE * isfirst);

    extern      FRETVAL(intf) ndaafds
                (intf _HUGE * dfile, _fcd desc, intf _HUGE * desclen);

    extern      FRETVAL(intf) ndagfidl
                (intf _HUGE * dfile, intf _HUGE * isfirst);

    extern      FRETVAL(intf) ndagfdsl
                (intf _HUGE * dfile, intf _HUGE * isfirst);

    extern      FRETVAL(intf) ndagfid
                (intf _HUGE * dfile, _fcd id, intf _HUGE * maxlen, intf _HUGE * isfirst);

    extern      FRETVAL(intf) ndagfds
                (intf _HUGE * dfile, _fcd id, intf _HUGE * maxlen, intf _HUGE * isfirst);

    extern      FRETVAL(intf) ndaiafid
                (intf _HUGE * dfile, _fcd id, intf _HUGE * idlen);

/*
   ** from dfr8F.c
 */
#ifndef DFR8_FNAMES
#   define DFR8_FNAMES
#ifdef DF_CAPFNAMES
#   define nd8spal   FNAME(D8SPAL)
#   define nd8first  FNAME(D8FIRST)
#   define nd8igdim  FNAME(D8IGDIM)
#   define nd8igimg  FNAME(D8IGIMG)
#   define nd8ipimg  FNAME(D8IPIMG)
#   define nd8iaimg  FNAME(D8IAIMG)
#   define nd8irref  FNAME(D8IRREF)
#   define nd8iwref  FNAME(D8IWREF)
#   define nd8inims  FNAME(D8INIMS)
#   define nd8lref   FNAME(D8LREF)
#   define ndfr8lastref      FNAME(DFR8LASTREF)
#   define ndfr8setpalette   FNAME(DFR8SETPALETTE)
#   define ndfr8restart  FNAME(DFR8RESTART)
#   define nd8scomp  FNAME(D8SCOMP)
#   define ndfr8scompress FNAME(DFR8SCOMPRESS)
#   define nd8sjpeg  FNAME(D8SJPEG)
#   define ndfr8sjpeg FNAME(DFR8SJPEG)
#else                           /* !DF_CAPFNAMES */
#   define nd8spal   FNAME(d8spal)
#   define nd8first  FNAME(d8first)
#   define nd8igdim  FNAME(d8igdim)
#   define nd8igimg  FNAME(d8igimg)
#   define nd8ipimg  FNAME(d8ipimg)
#   define nd8iaimg  FNAME(d8iaimg)
#   define nd8irref  FNAME(d8irref)
#   define nd8iwref  FNAME(d8iwref)
#   define nd8inims  FNAME(d8inims)
#   define nd8lref   FNAME(d8lref)
#   define ndfr8lastref      FNAME(dfr8lastref)
#   define ndfr8setpalette   FNAME(dfr8setpalette)
#   define ndfr8restart  FNAME(dfr8restart)
#   define nd8scomp  FNAME(d8scomp)
#   define ndfr8scompress FNAME(dfr8scompress)
#   define nd8sjpeg  FNAME(d8sjpeg)
#   define ndfr8sjpeg FNAME(dfr8sjpeg)
#endif                          /* DF_CAPFNAMES */
#endif                          /* DFR8_FNAMES */

    extern      FRETVAL(intf) nd8spal
                (_fcd pal);

    extern      FRETVAL(intf) nd8first
                (void);

    extern      FRETVAL(intf) nd8igdim
                (_fcd filename, intf _HUGE * xdim, intf _HUGE * ydim, intf _HUGE * ispal,
                 intf _HUGE * lenfn);

    extern      FRETVAL(intf) nd8igimg
                (_fcd filename, _fcd image, intf _HUGE * xdim, intf _HUGE * ydim,
                 _fcd pal, intf _HUGE * lenfn);

    extern      FRETVAL(intf) nd8ipimg
                (_fcd filename, _fcd image, intf _HUGE * xdim, intf _HUGE * ydim,
                 intf _HUGE * compress, intf _HUGE * lenfn);

    extern      FRETVAL(intf) nd8iaimg
                (_fcd filename, _fcd image, intf _HUGE * xdim, intf _HUGE * ydim,
                 intf _HUGE * compress, intf _HUGE * lenfn);

    extern      FRETVAL(intf) nd8irref
                (_fcd filename, intf _HUGE * ref, intf _HUGE * fnlen);

    extern      FRETVAL(intf) nd8iwref
                (_fcd filename, intf _HUGE * ref, intf _HUGE * fnlen);

    extern      FRETVAL(intf) nd8inims
                (_fcd filename, intf _HUGE * fnlen);

    extern      FRETVAL(intf) nd8lref
                (void);

    extern      FRETVAL(intf) ndfr8lastref
                (void);

    extern      FRETVAL(intf) ndfr8setpalette
                (_fcd pal);

    extern      FRETVAL(intf) ndfr8restart
                (void);

    extern      FRETVAL(intf) nd8scomp
                (intf * scheme);

    extern      FRETVAL(intf) ndfr8scompress
                (intf * scheme);

    extern      FRETVAL(intf) nd8sjpeg
                (intf * quality, intf * force_baseline);

    extern      FRETVAL(intf) ndfr8sjpeg
                (intf * quality, intf * force_baseline);

/*
   ** from dfsdF.c
 */
#ifndef DFSD_FNAMES
#   define DFSD_FNAMES
#ifdef DF_CAPFNAMES
#   define ndsgdast  FNAME(DSGDAST)
#   define ndsgdisc  FNAME(DSGDISC)
#   define ndsgrang  FNAME(DSGRANG)
#   define ndssdims  FNAME(DSSDIMS)
#   define ndssdisc  FNAME(DSSDISC)
#   define ndssrang  FNAME(DSSRANG)
#   define ndsclear  FNAME(DSCLEAR)
#   define ndsslens  FNAME(DSSLENS)
#   define ndsgdiln  FNAME(DSGDILN)
#   define ndsgdaln  FNAME(DSGDALN)
#   define ndsfirst  FNAME(DSFIRST)
#   define ndspslc   FNAME(DSPSLC)
#   define ndseslc   FNAME(DSESLC)
#   define ndsgnt    FNAME(DSGNT)
#   define ndssnt    FNAME(DSSNT)
#   define ndsigdim  FNAME(DSIGDIM)
#   define ndsigdat  FNAME(DSIGDAT)
#   define ndsipdat  FNAME(DSIPDAT)
#   define ndsiadat  FNAME(DSIADAT)
#   define ndsigdas  FNAME(DSIGDAS)
#   define ndsigslc  FNAME(DSIGSLC)
#   define ndsigdis  FNAME(DSIGDIS)
#   define ndsisslc  FNAME(DSISSLC)
#   define ndsisdas  FNAME(DSISDAS)
#   define ndsisdis  FNAME(DSISDIS)
#   define ndsirref  FNAME(DSIRREF)
#   define ndslref   FNAME(DSLREF)
#   define ndsinum   FNAME(DSINUM)
#   define ndsip32s  FNAME(DSIP32S)
#   define ndsscal   FNAME(DSSCAL)
#   define ndsgcal   FNAME(DSGCAL)
#   define ndfsdgetdatastrs  FNAME(DFSDGETDATASTRS)
#   define ndfsdgetdimscale  FNAME(DFSDGETDIMSCALE)
#   define ndfsdgetrange     FNAME(DFSDGETRANGE)
#   define ndfsdsetdims      FNAME(DFSDSETDIMS)
#   define ndfsdsetdimscale  FNAME(DFSDSETDIMSCALE)
#   define ndfsdsetrange     FNAME(DFSDSETRANGE)
#   define ndfsdclear        FNAME(DFSDCLEAR)
#   define ndfsdsetlengths   FNAME(DFSDSETLENGTHS)
#   define ndfsdgetdimlen    FNAME(DFSDGETDIMLEN)
#   define ndfsdgetdatalen   FNAME(DFSDGETDATALEN)
#   define ndfsdrestart      FNAME(DFSDRESTART)
#   define ndfsdputslice     FNAME(DFSDPUTSLICE)
#   define ndfsdendslice     FNAME(DFSDENDSLICE)
#   define ndfsdsetnt        FNAME(DFSDSETNT)
#   define ndfsdgetnt        FNAME(DFSDGETNT)
#   define ndfsdlastref      FNAME(DFSDLASTREF)
#   define ndsiwref          FNAME(DSIWREF)
#   define ndssfill          FNAME(DSSFILL)
#   define ndsgfill          FNAME(DSGFILL)
#   define ndsisslab         FNAME(DSISSLAB)
#   define ndswslab          FNAME(DSWSLAB)
#   define ndseslab          FNAME(DSESLAB)
#   define ndsirslab         FNAME(DSIRSLAB)
#else
#   define ndsgdast  FNAME(dsgdast)
#   define ndsgdisc  FNAME(dsgdisc)
#   define ndsgrang  FNAME(dsgrang)
#   define ndssdims  FNAME(dssdims)
#   define ndssdisc  FNAME(dssdisc)
#   define ndssrang  FNAME(dssrang)
#   define ndsclear  FNAME(dsclear)
#   define ndsslens  FNAME(dsslens)
#   define ndsgdiln  FNAME(dsgdiln)
#   define ndsgdaln  FNAME(dsgdaln)
#   define ndsfirst  FNAME(dsfirst)
#   define ndspslc   FNAME(dspslc)
#   define ndseslc   FNAME(dseslc)
#   define ndsgnt    FNAME(dsgnt)
#   define ndssnt    FNAME(dssnt)
#   define ndsigdim  FNAME(dsigdim)
#   define ndsigdat  FNAME(dsigdat)
#   define ndsipdat  FNAME(dsipdat)
#   define ndsiadat  FNAME(dsiadat)
#   define ndsigdas  FNAME(dsigdas)
#   define ndsigslc  FNAME(dsigslc)
#   define ndsigdis  FNAME(dsigdis)
#   define ndsisslc  FNAME(dsisslc)
#   define ndsisdas  FNAME(dsisdas)
#   define ndsisdis  FNAME(dsisdis)
#   define ndsirref  FNAME(dsirref)
#   define ndslref   FNAME(dslref)
#   define ndsinum   FNAME(dsinum)
#   define ndsip32s  FNAME(dsip32s)
#   define ndsscal   FNAME(dsscal)
#   define ndsgcal   FNAME(dsgcal)
#   define ndfsdgetdatastrs  FNAME(dfsdgetdatastrs)
#   define ndfsdgetdimscale  FNAME(dfsdgetdimscale)
#   define ndfsdgetrange     FNAME(dfsdgetrange)
#   define ndfsdsetdims      FNAME(dfsdsetdims)
#   define ndfsdsetdimscale  FNAME(dfsdsetdimscale)
#   define ndfsdsetrange     FNAME(dfsdsetrange)
#   define ndfsdclear        FNAME(dfsdclear)
#   define ndfsdsetlengths   FNAME(dfsdsetlengths)
#   define ndfsdgetdimlen    FNAME(dfsdgetdimlen)
#   define ndfsdgetdatalen   FNAME(dfsdgetdatalen)
#   define ndfsdrestart      FNAME(dfsdrestart)
#   define ndfsdputslice     FNAME(dfsdputslice)
#   define ndfsdendslice     FNAME(dfsdendslice)
#   define ndfsdsetnt        FNAME(dfsdsetnt)
#   define ndfsdgetnt        FNAME(dfsdgetnt)
#   define ndfsdlastref      FNAME(dfsdlastref)
#   define ndsiwref          FNAME(dsiwref)
#   define ndssfill          FNAME(dssfill)
#   define ndsgfill          FNAME(dsgfill)
#   define ndsisslab         FNAME(dsisslab)
#   define ndswslab          FNAME(dswslab)
#   define ndseslab          FNAME(dseslab)
#   define ndsirslab         FNAME(dsirslab)
#endif                          /* DF_CAPFNAMES */
#endif                          /* DFSD_FNAMES */

    extern      FRETVAL(intf) ndsgdisc
                (intf _HUGE * dim, intf _HUGE * maxsize, VOIDP scale);

    extern      FRETVAL(intf) ndsgrang
                (VOIDP pmax, VOIDP pmin);

    extern      FRETVAL(intf) ndssdims
                (intf _HUGE * rank, intf _HUGE dimsizes[]);

    extern      FRETVAL(intf) ndssdisc
                (intf _HUGE * dim, intf _HUGE * dimsize, VOIDP scale);

    extern      FRETVAL(intf) ndssrang
                (VOIDP max, VOIDP min);

    extern      FRETVAL(intf) ndsclear
                (void);

    extern      FRETVAL(intf) ndsslens
                (intf _HUGE * maxlen_label, intf _HUGE * maxlen_unit,
                 intf _HUGE * maxlen_format, intf _HUGE * maxlen_coordsys);

    extern      FRETVAL(intf) ndsgdiln
                (intf _HUGE * dim, intf _HUGE * llabel, intf _HUGE * lunit,
                 intf _HUGE * lformat);

    extern      FRETVAL(intf) ndsgdaln
                (intf _HUGE * llabel, intf _HUGE * lunit, intf _HUGE * lformat,
                 intf _HUGE * lcoordsys);

    extern      FRETVAL(intf) ndsfirst
                (void);

    extern      FRETVAL(intf) ndspslc
                (intf _HUGE windims[], VOIDP data, intf _HUGE dims[]);

    extern      FRETVAL(intf) ndseslc
                (void);

    extern      FRETVAL(intf) ndssnt
                (intf _HUGE * numbertype);

    extern      FRETVAL(intf) ndsgnt
                (intf _HUGE * pnumbertype);

    extern      FRETVAL(intf) ndsigdim
                (_fcd filename, intf _HUGE * prank, intf _HUGE sizes[],
                 intf _HUGE * maxrank, intf _HUGE * lenfn);

    extern      FRETVAL(intf) ndsigdat
                (_fcd filename, intf _HUGE * rank, intf _HUGE maxsizes[],
                 VOIDP data, intf _HUGE * fnlen);

    extern      FRETVAL(intf) ndsipdat
                (_fcd filename, intf _HUGE * rank, intf _HUGE dimsizes[],
                 VOIDP data, intf _HUGE * fnlen);

    extern      FRETVAL(intf) ndsiadat
                (_fcd filename, intf _HUGE * rank, intf _HUGE dimsizes[],
                 VOIDP data, intf _HUGE * fnlen);

    extern      FRETVAL(intf) ndsigslc
                (_fcd filename, intf _HUGE winst[], intf _HUGE windims[],
                 VOIDP data, intf _HUGE dims[], intf _HUGE * fnlen);

    extern      FRETVAL(intf) ndsisslc
                (_fcd filename, intf _HUGE * fnlen);

    extern      FRETVAL(intf) ndsirref
                (_fcd filename, intf _HUGE * ref, intf _HUGE * fnlen);

    extern      FRETVAL(intf) ndslref
                (void);

    extern      FRETVAL(intf) ndsinum
                (_fcd filename, intf _HUGE * len);

    extern      FRETVAL(intf) ndsip32s
                (_fcd filename, intf _HUGE * ref, intf _HUGE * ispre32, intf _HUGE * len);

    extern      FRETVAL(intf) ndfsdgetdatastrs
                (_fcd label, _fcd unit, _fcd format, _fcd coordsys);

    extern      FRETVAL(intf) ndfsdgetdimstrs
                (intf _HUGE * dim, _fcd label, _fcd unit, _fcd format);

    extern      FRETVAL(intf) ndfsdgetdimscale
                (intf _HUGE * dim, intf _HUGE * maxsize, VOIDP scale);

    extern      FRETVAL(intf) ndfsdgetrange
                (VOIDP pmax, VOIDP pmin);

    extern      FRETVAL(intf) ndfsdsetdims
                (intf _HUGE * rank, intf _HUGE dimsizes[]);

    extern      FRETVAL(intf) ndfsdsetdimscale
                (intf _HUGE * dim, intf _HUGE * dimsize, VOIDP scale);

    extern      FRETVAL(intf) ndfsdsetrange
                (VOIDP max, VOIDP min);

    extern      FRETVAL(intf) ndfsdclear
                (void);

    extern      FRETVAL(intf) ndfsdsetlengths
                (intf _HUGE * maxlen_label, intf _HUGE * maxlen_unit,
                 intf _HUGE * maxlen_format, intf _HUGE * maxlen_coordsys);

    extern      FRETVAL(intf) ndfsdgetdimlen
                (intf _HUGE * dim, intf _HUGE * llabel, intf _HUGE * lunit,
                 intf _HUGE * lformat);

    extern      FRETVAL(intf) ndfsdgetdatalen
                (intf _HUGE * llabel, intf _HUGE * lunit, intf _HUGE * lformat,
                 intf _HUGE * lcoordsys);

    extern      FRETVAL(intf) ndfsdrestart
                (void);

    extern      FRETVAL(intf) ndfsdputslice
                (intf _HUGE windims[], VOIDP data, intf _HUGE dims[]);

    extern      FRETVAL(intf) ndfsdendslice
                (void);

    extern      FRETVAL(intf) ndfsdsetnt
                (intf _HUGE * numbertype);

    extern      FRETVAL(intf) ndfsdgetnt
                (intf _HUGE * pnumbertype);

    extern      FRETVAL(intf) ndfsdlastref
                (void);

    extern      FRETVAL(intf) ndsisdis
                (intf _HUGE * dim, _fcd flabel, _fcd funit, _fcd fformat,
             intf _HUGE * llabel, intf _HUGE * lunit, intf _HUGE * lformat);

    extern      FRETVAL(intf) ndsigdis
                (intf _HUGE * dim, _fcd label, _fcd unit, _fcd format,
             intf _HUGE * llabel, intf _HUGE * lunit, intf _HUGE * lformat);

    extern      FRETVAL(intf) ndsisdas
                (_fcd flabel, _fcd funit, _fcd fformat, _fcd fcoordsys,
            intf _HUGE * isfortran, intf _HUGE * llabel, intf _HUGE * lunit,
                 intf _HUGE * lformat, intf _HUGE * lcoordsys);

    extern      FRETVAL(intf) ndsigdas
                (_fcd label, _fcd unit, _fcd format, _fcd coordsys, intf _HUGE * llabel,
             intf _HUGE * lunit, intf _HUGE * lformat, intf _HUGE * lcoord);

    extern      FRETVAL(intf) ndsscal
                (float64 _HUGE * cal, float64 _HUGE * cal_err, float64 _HUGE * ioff,
                 float64 _HUGE * ioff_err, intf _HUGE * cal_type);

    extern      FRETVAL(intf) ndsgcal
                (float64 _HUGE * cal, float64 _HUGE * cal_err, float64 _HUGE * ioff,
                 float64 _HUGE * ioff_err, intf _HUGE * cal_type);

    extern      FRETVAL(intf) ndswref
                (_fcd filename, intf _HUGE * fnlen, intf _HUGE * ref);

    extern      FRETVAL(intf) ndssfill
                (VOIDP fill_value);

    extern      FRETVAL(intf) ndsgfill
                (VOIDP fill_value);

    extern      FRETVAL(intf) ndssslab
                (_fcd filename, intf _HUGE * fnlen);

    extern      FRETVAL(intf) ndswslab
                (intf _HUGE start[], intf _HUGE stride[],
                 intf _HUGE cont[], VOIDP data);

    extern      FRETVAL(intf) ndseslab
                (void);

/*
   ** from dfpF.c
 */

#ifndef DFP_FNAMES
#   define DFP_FNAMES
#ifdef DF_CAPFNAMES
#   define ndpigpal  FNAME(DPIGPAL)
#   define ndpippal  FNAME(DPIPPAL)
#   define ndpinpal  FNAME(DPINPAL)
#   define ndpiwref  FNAME(DPIWREF)
#   define ndpirref  FNAME(DPIRREF)
#   define ndprest   FNAME(DPREST)
#   define ndplref   FNAME(DPLREF)
#   define ndfprestart   FNAME(DFPRESTART)
#   define ndfplastref   FNAME(DFPLASTREF)
#else                           /* !DF_CAPNAMES */
#   define ndpigpal  FNAME(dpigpal)
#   define ndpippal  FNAME(dpippal)
#   define ndpinpal  FNAME(dpinpal)
#   define ndpiwref  FNAME(dpiwref)
#   define ndpirref  FNAME(dpirref)
#   define ndprest   FNAME(dprest)
#   define ndplref   FNAME(dplref)
#   define ndfprestart   FNAME(dfprestart)
#   define ndfplastref   FNAME(dfplastref)
#endif                          /* DF_CAPFNAMES */
#endif                          /* DFP_FNAMES */

    extern      FRETVAL(intf) ndpigpal
                (_fcd filename, _fcd pal, intf _HUGE * fnlen);

    extern      FRETVAL(intf) ndpippal
                (_fcd filename, _fcd pal, intf _HUGE * overwrite, _fcd filemode,
                 intf _HUGE * fnlen);

    extern      FRETVAL(intf) ndpinpal
                (_fcd filename, intf _HUGE * fnlen);

    extern      FRETVAL(intf) ndpirref
                (_fcd filename, uint16 _HUGE * ref, intf _HUGE * fnlen);

    extern      FRETVAL(intf) ndpiwref
                (_fcd filename, uint16 _HUGE * ref, intf _HUGE * fnlen);

    extern      FRETVAL(intf) ndprest
                (void);

    extern      FRETVAL(intf) ndplref
                (void);

    extern      FRETVAL(intf) ndfprestart
                (void);

    extern      FRETVAL(intf) ndfplastref
                (void);

/*
   ** from df24F.c
 */
#ifndef DF24_FNAMES
#   define DF24_FNAMES
#ifdef DF_CAPFNAMES
#   define nd2reqil  FNAME(D2REQIL)
#   define ndf24reqil    FNAME(DF24REQIL)
#   define nd2sdims  FNAME(D2SDIMS)
#   define ndf24setdims  FNAME(DF24SETDIMS)
#   define nd2setil  FNAME(D2SETIL)
#   define ndf24setil    FNAME(DF24SETIL)
#   define nd2first  FNAME(D2FIRST)
#   define ndf24restart  FNAME(DF24RESTART)
#   define nd2igdim  FNAME(D2IGDIM)
#   define nd2igimg  FNAME(D2IGIMG)
#   define nd2iaimg  FNAME(D2IAIMG)
#   define nd2irref  FNAME(D2IRREF)
#   define nd2inimg  FNAME(D2INIMG)
#   define nd2lref   FNAME(D2LREF)
#   define nd2scomp  FNAME(D2SCOMP)
#   define ndf24scompress FNAME(DF24SCOMPRESS)
#   define nd2sjpeg  FNAME(D2SJPEG)
#   define ndf24sjpeg FNAME(DF24SJPEG)
#else
#   define nd2reqil  FNAME(d2reqil)
#   define ndf24reqil    FNAME(df24reqil)
#   define nd2sdims  FNAME(d2sdims)
#   define ndf24setdims  FNAME(df24setdims)
#   define nd2setil  FNAME(d2setil)
#   define ndf24setil    FNAME(df24setil)
#   define nd2first  FNAME(d2first)
#   define ndf24restart  FNAME(df24restart)
#   define nd2igdim  FNAME(d2igdim)
#   define nd2igimg  FNAME(d2igimg)
#   define nd2iaimg  FNAME(d2iaimg)
#   define nd2irref  FNAME(d2irref)
#   define nd2inimg  FNAME(d2inimg)
#   define nd2lref   FNAME(d2lref)
#   define nd2scomp  FNAME(d2scomp)
#   define ndf24scompress FNAME(df24scompress)
#   define nd2sjpeg  FNAME(d2sjpeg)
#   define ndf24sjpeg FNAME(df24sjpeg)
#endif                          /* DF_CAPFNAMES */
#endif                          /* DF24_FNAMES */

    extern      FRETVAL(intf) nd2reqil
                (intf _HUGE * il);

    extern      FRETVAL(intf) nd2sdims
                (intf _HUGE * xdim, intf _HUGE * ydim);

    extern      FRETVAL(intf) nd2igdim
                (_fcd filename, intf _HUGE * pxdim, intf _HUGE * pydim, intf _HUGE * pil,
                 intf _HUGE * fnlen);

    extern      FRETVAL(intf) nd2igimg
                (_fcd filename, _fcd image, intf _HUGE * xdim, intf _HUGE * ydim,
                 intf _HUGE * fnlen);

    extern      FRETVAL(intf) nd2iaimg
                (_fcd filename, _fcd image, intf _HUGE * xdim, intf _HUGE * ydim,
                 intf _HUGE * fnlen, intf _HUGE * newfile);

    extern      FRETVAL(intf) nd2setil
                (intf _HUGE * il);

    extern      FRETVAL(intf) nd2first
                (void);

    extern      FRETVAL(intf) ndf24reqil
                (intf _HUGE * il);

    extern      FRETVAL(intf) ndf24setdims
                (intf _HUGE * xdim, intf _HUGE * ydim);

    extern      FRETVAL(intf) ndf24setil
                (intf _HUGE * il);

    extern      FRETVAL(intf) ndf24restart
                (void);

    extern      FRETVAL(intf) nd2irref
                (_fcd filename, intf _HUGE * ref, intf _HUGE * fnlen);

    extern      FRETVAL(intf) nd2inimg
                (_fcd filename, intf _HUGE * fnlen);

    extern      FRETVAL(intf) nd2lref
                (void);

    extern      FRETVAL(intf) nd2scomp
                (intf * scheme);

    extern      FRETVAL(intf) ndf24scompress
                (intf * scheme);

    extern      FRETVAL(intf) nd2sjpeg
                (intf * quality, intf * force_baseline);

    extern      FRETVAL(intf) ndf24sjpeg
                (intf * quality, intf * force_baseline);

/*
   ** from dfF.c
 */
#ifndef DF_FNAMES
#   define DF_FNAMES
#ifdef DF_CAPFNAMES
#   define ndfiaccess FNAME(DFIACCESS)
#   define ndfiopen  FNAME(DFIOPEN)
#   define ndfclose  FNAME(DFCLOSE)
#   define ndfdesc   FNAME(DFDESC)
#   define ndfdup    FNAME(DFDUP)
#   define ndfdel    FNAME(DFDEL)
#   define ndfstart  FNAME(DFSTART)
#   define ndfread   FNAME(DFREAD)
#   define ndfseek   FNAME(DFSEEK)
#   define ndfwrite  FNAME(DFWRITE)
#   define ndfupdate FNAME(DFUPDATE)
#   define ndfget    FNAME(DFGET)
#   define ndfput    FNAME(DFPUT)
#   define ndfsfind  FNAME(DFSFIND)
#   define ndffind   FNAME(DFFIND)
#   define ndferrno  FNAME(DFERRNO)
#   define ndfnewref FNAME(DFNEWREF)
#   define ndfnumber FNAME(DFNUMBER)
#   define ndfstat   FNAME(DFSTAT)
#   define ndfiishdf FNAME(DFIISHDF)
#else                           /* !DF_CAPFNAMES */
#   define ndfiaccess FNAME(dfiaccess)
#   define ndfiopen  FNAME(dfiopen)
#   define ndfclose  FNAME(dfclose)
#   define ndfdesc   FNAME(dfdesc)
#   define ndfdup    FNAME(dfdup)
#   define ndfdel    FNAME(dfdel)
#   define ndfstart  FNAME(dfstart)
#   define ndfread   FNAME(dfread)
#   define ndfseek   FNAME(dfseek)
#   define ndfwrite  FNAME(dfwrite)
#   define ndfupdate FNAME(dfupdate)
#   define ndfget    FNAME(dfget)
#   define ndfput    FNAME(dfput)
#   define ndfsfind  FNAME(dfsfind)
#   define ndffind   FNAME(dffind)
#   define ndferrno  FNAME(dferrno)
#   define ndfnewref FNAME(dfnewref)
#   define ndfnumber FNAME(dfnumber)
#   define ndfstat   FNAME(dfstat)
#   define ndfiishdf FNAME(dfiishdf)
#endif                          /* DF_CAPFNAMES */
#endif                          /* DF_FNAMES */

    extern      FRETVAL(intf) ndfiopen
                (_fcd name, intf _HUGE * acc_mode, intf _HUGE * defdds, intf _HUGE * namelen);

    extern      FRETVAL(intf) ndfclose
                (intf _HUGE * dfile);

    extern      FRETVAL(intf) ndfdesc
                (intf _HUGE * dfile, intf _HUGE ptr[][4], intf _HUGE * begin,
                 intf _HUGE * num);

    extern      FRETVAL(intf) ndfdup
                (intf _HUGE * dfile, intf _HUGE * tag, intf _HUGE * ref, intf _HUGE * otag,
                 intf _HUGE * oref);

    extern      FRETVAL(intf) ndfdel
                (intf _HUGE * dfile, intf _HUGE * tag, intf _HUGE * ref);

    extern      FRETVAL(intf) ndfiaccess
                (intf _HUGE * dfile, intf _HUGE * tag, intf _HUGE * ref, _fcd acc_mode, intf _HUGE * acclen);

    extern      FRETVAL(intf) ndfstart
                (intf _HUGE * dfile, intf _HUGE * tag, intf _HUGE * ref, char _HUGE * acc_mode);

    extern      FRETVAL(intf) ndfread
                (intf _HUGE * dfile, _fcd ptr, intf _HUGE * len);

    extern      FRETVAL(intf) ndfseek
                (intf _HUGE * dfile, intf _HUGE * offset);

    extern      FRETVAL(intf) ndfwrite
                (intf _HUGE * dfile, _fcd ptr, intf _HUGE * len);

    extern      FRETVAL(intf) ndfupdate
                (intf _HUGE * dfile);

    extern      FRETVAL(intf) ndfget
                (intf _HUGE * dfile, intf _HUGE * tag, intf _HUGE * ref, _fcd ptr);

    extern      FRETVAL(intf) ndfput
                (intf _HUGE * dfile, intf _HUGE * tag, intf _HUGE * ref, _fcd ptr, intf _HUGE * len);

    extern      FRETVAL(intf) ndfsfind
                (intf _HUGE * dfile, intf _HUGE * tag, intf _HUGE * ref);

    extern      FRETVAL(intf) ndffind
                (intf _HUGE * dfile, intf _HUGE * itag, intf _HUGE * iref, intf _HUGE * len);

    extern      FRETVAL(intf) ndferrno
                (void);

    extern      FRETVAL(intf) ndfnewref
                (intf _HUGE * dfile);

    extern      FRETVAL(intf) ndfnumber
                (intf _HUGE * dfile, intf _HUGE * tag);

#ifdef TEMP_OUT
    extern      FRETVAL(intf) ndfstat
                (intf _HUGE * dfile, DFdata _HUGE * dfinfo);
#endif

    extern      FRETVAL(intf) ndfiishdf
                (_fcd name, intf _HUGE * namelen);

/*
   ** from dfutil.c
 */
    extern uint16 DFfindnextref
                (int32 file_id, uint16 tag, uint16 lref);

/*
   ** from dfutilF.c
 */
#ifndef DFUTIL_FNAMES
#   define DFUTIL_FNAMES
#ifdef DF_CAPFNAMES
#   define ndfindnr          FNAME(DFINDNR)
#   define ndffindnextref    FNAME(DFFINDNEXTREF)
#else
#   define ndfindnr          FNAME(dfindnr)
#   define ndffindnextref    FNAME(dffindnextref)
#endif                          /* DF_CAPFNAMES */
#endif                          /* DFUTIL_FNAMES */

    extern      FRETVAL(intf) ndfindnr
                (intf _HUGE * dfile, intf _HUGE * tag, intf _HUGE * lref);

    extern      FRETVAL(intf) ndffindnextref
                (intf _HUGE * dfile, intf _HUGE * tag, intf _HUGE * lref);

/*
   ** from herrF.c
 */
#ifndef HERR_FNAMES
#   define HERR_FNAMES
#ifdef DF_CAPFNAMES
#   define nheprnt   FNAME(HEPRNT)
#else
#   define nheprnt   FNAME(heprnt)
#endif                          /* DF_CAPFNAMES */
#endif                          /* HERR_FNAMES */

    extern      FRETVAL(VOID) nheprnt
                (intf _HUGE * print_levels);

/*
   ** from hfileF.c
 */
#ifndef HFILE_FNAMES
#   define HFILE_FNAMES
#ifdef DF_CAPFNAMES
#   define nhiopen   FNAME(HIOPEN)
#   define nhclose   FNAME(HCLOSE)
#   define nhnumber  FNAME(HNUMBER)
#else
#   define nhiopen   FNAME(hiopen)
#   define nhclose  FNAME(hclose)
#   define nhnumber FNAME(hnumber)
#endif                          /* DF_CAPFNAMES */
#endif                          /* HFILE_FNAMES */

    extern      FRETVAL(intf) nhiopen
                (_fcd name, intf _HUGE * acc_mode, intf _HUGE * defdds, intf _HUGE * namelen);

    extern      FRETVAL(intf) nhclose
                (intf _HUGE * file_id);

/*
   ** from dfufp2im.c
 */
#ifndef DFUFP2I_FNAMES
#   define DFUFP2I_FNAMES
#ifdef DF_CAPFNAMES
#   define nduif2i       FNAME(DUIF2I)
#else
#   define nduif2i       FNAME(duif2i)
#endif                          /* DF_CAPFNAMES */
#endif                          /* DFUFP2I_FNAMES */

    extern      FRETVAL(int) nduif2i
                (int32 _HUGE * hdim, int32 _HUGE * vdim, float32 _HUGE * max,
        float32 _HUGE * min, float32 _HUGE hscale[], float32 _HUGE vscale[],
                 float32 _HUGE data[], uint8 _HUGE * palette, _fcd outfile,
              int _HUGE * ct_method, int32 _HUGE * hres, int32 _HUGE * vres,
                 int _HUGE * compress, int _HUGE * lenfn);

    extern int  DFUfptoimage
                (int32 hdim, int32 vdim, float32 max, float32 min,
       float32 _HUGE * hscale, float32 _HUGE * vscale, float32 _HUGE * data,
                 uint8 _HUGE * palette, char _HUGE * outfile, int ct_method,
                 int32 hres, int32 vres, int compress);

/* temp. fix for circular includes with vsets */
#ifdef PERM_OUT
#include "vproto.h"
#endif                          /* PERM_OUT */

#if defined c_plusplus || defined __cplusplus
}
#endif                          /* c_plusplus || __cplusplus */

#endif                          /* _H_PROTO */

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

/*+ hfile.h
   *** Header for hfile.c, routines for low level data element I/O
   + */

#ifndef HFILE_H
#define HFILE_H

/* ------------------------------ Constants ------------------------------- */
/* Maximum number of files (number of slots for file records) */
#ifndef MAX_FILE
#if defined PC && !(defined PC386 || defined UNIX386)
#   define MAX_FILE 8
#else  /* !PC */
#   define MAX_FILE 16
#endif /* !PC */
#endif /* MAX_FILE */

/* Maximum length of external filename(s) (used in hextelt.c & hbigext.c) */
#ifndef MAX_PATH_LEN
#if defined PC && !(defined PC386 || defined UNIX386)
#define MAX_PATH_LEN 256
#else  /* non-DOS systems */
#define MAX_PATH_LEN 1024
#endif /* PATH_LEN defines */
#endif /* MAX_PATH_LEN */

#define MAX_
/* Maximum number of access elements */
#ifndef MAX_ACC
#   define MAX_ACC 256
#endif /* MAX_ACC */

/* Magic cookie for HDF data files */
#define MAGICLEN 4  /* length */
#define HDFMAGIC "\016\003\023\001"     /* ^N^C^S^A */

/* sizes of elements in a file.  This is necessary because
   the size of variables need not be the same as in the file
   (cannot use sizeof) */
#define DD_SZ 12    /* 2+2+4+4 */
#define NDDS_SZ 2
#define OFFSET_SZ 4

/* ndds (number of dd's in a block) default,
   so user need not specify */
#ifndef DEF_NDDS
#   define DEF_NDDS 16
#endif /* DEF_NDDS */

/* ndds minimum, to prevent excessive overhead of very small dd-blocks */
#ifndef MIN_NDDS
#   define MIN_NDDS 4
#endif /* MIN_NDDS */

/* largest number that will fit into 16-bit word ref variable */
#define MAX_REF ((uint16)32767)

/* invalid offset & length to indicate a partially defined element */
#define INVALID_OFFSET 0xFFFFFFFF
#define INVALID_LENGTH 0xFFFFFFFF

/* length of block and number of blocks for converting 'appendable' data */
/* elements into linked blocks (will eventually be replaced by the newer */
/* variable-length blocks */
#define HDF_APPENDABLE_BLOCK_LEN 4096
#define HDF_APPENDABLE_BLOCK_NUM 16

/* ----------------------------- Version Tags ----------------------------- */
/* Library version numbers */

#define LIBVER_MAJOR    4
#define LIBVER_MINOR    0
#define LIBVER_RELEASE  0
#define LIBVER_STRING   "NCSA HDF Version 4.0 Release 0, ?month? ?year?"
#define LIBVSTR_LEN    80   /* length of version string  */
#define LIBVER_LEN  92  /* 4+4+4+80 = 92 */
/* end of version tags */

/* -------------------------- File I/O Functions -------------------------- */
/* FILELIB -- file library to use for file access: 1 stdio, 2 fcntl
   default to stdio library i.e. UNIX buffered I/O */

#ifndef FILELIB
#   define FILELIB UNIXBUFIO    /* UNIX buffered I/O is the default */
#endif /* FILELIB */

#if (FILELIB == UNIXBUFIO)
/* using C buffered file I/O routines to access files */
typedef FILE *hdf_file_t;
#ifdef VMS
/* For VMS, use "mbc=64" to improve performance     */
#   define HI_OPEN(p, a)       (((a) & DFACC_WRITE) ? \
                                fopen((p), "r+", "mbc=64") : \
                                fopen((p), "r", "mbc=64"))
#else  /*  !VMS  */
#ifdef PC386
#   define HI_OPEN(p, a)       (((a) & DFACC_WRITE) ? \
                                fopen((p), "rb+") : fopen((p), "rb"))
#else /* !PC386 */
#   define HI_OPEN(p, a)       (((a) & DFACC_WRITE) ? \
                                fopen((p), "r+") : fopen((p), "r"))
#endif /* PC386 */
#endif /* !VMS */
#ifdef PC386
#   define HI_CREATE(p)        (fopen((p), "wb+"))
#else  /* PC386 */
#   define HI_CREATE(p)        (fopen((p), "w+"))
#endif /* PC386 */
#   define HI_READ(f, b, n)    (((n) == fread((b), 1, (size_t)(n), (f))) ? \
                                SUCCEED : FAIL)
#   define HI_WRITE(f, b, n)   (((n) == fwrite((b), 1, (size_t)(n), (f))) ? \
                                SUCCEED : FAIL)
#   define HI_CLOSE(f)   (fclose(f))
#   define HI_FLUSH(f)   (fflush(f)==0 ? SUCCEED : FAIL)
#   define HI_SEEK(f,o)  (fseek((f), (long)(o), SEEK_SET)==0 ? SUCCEED : FAIL)
#   define HI_SEEK_CUR(f,o)  (fseek((f), (long)(o), SEEK_CUR)==0 ? SUCCEED : FAIL)
#   define HI_SEEKEND(f) (fseek((f), (long)0, SEEK_END)==0 ? SUCCEED : FAIL)
#   define HI_TELL(f)    (ftell(f))
#   define OPENERR(f)    ((f) == (FILE *)NULL)
#endif /* FILELIB == UNIXBUFIO */

#if (FILELIB == UNIXUNBUFIO)
/* using UNIX unbuffered file I/O routines to access files */
typedef int hdf_file_t;
#   define HI_OPEN(p, a)       (((a) & DFACC_WRITE) ? \
                                    open((p), O_RDWR) : open((p), O_RDONLY))
#   define HI_CREATE(p)         (open((p), O_RDWR | O_CREAT | O_TRUNC))
#   define HI_CLOSE(f)          (close(f))
#   define HI_FLUSH(f)          (SUCCEED)
#   define HI_READ(f, b, n)     (read((f), (char *)(b), (n)))
#   define HI_WRITE(f, b, n)    (write((f), (char *)(b), (n)))
#   define HI_SEEK(f, o)        (lseek((f), (off_t)(o), SEEK_SET))
#   define HI_SEEKEND(f)        (lseek((f), (off_t)0, SEEK_END))
#   define HI_TELL(f)           (lseek((f), (off_t)0, SEEK_CUR))
#   define OPENERR(f)           (f < 0)
#endif /* FILELIB == UNIXUNBUFIO */

#if (FILELIB == MACIO)
/* using special routines to redirect to Mac Toolkit I/O */
typedef short hdf_file_t;
#   define HI_OPEN(x,y)         mopen(x,y)
#   define HI_CREATE(name)      mopen(name, DFACC_CREATE)
#   define HI_CLOSE(x)          mclose(x)
#   define HI_FLUSH(a)          (SUCCEED)
#   define HI_READ(a,b,c)       mread(a, (char *) b, (int32) c)
#   define HI_WRITE(a,b,c)      mwrite(a, (char *) b, (int32) c)
#   define HI_SEEK(x,y)         mlseek(x, (int32 )y, 0)
#   define HI_SEEKEND(x)        mlseek(x, 0L, 2)
#   define HI_TELL(x)           mlseek(x,0L,1)
#   define DF_OPENERR(f)        ((f) == -1)
#   define OPENERR(f)           (f < 0)
#endif /* FILELIB == MACIO */

#if (FILELIB == PCIO)
/* using special PC functions to enable reading/writing large chunks */
typedef FILE *hdf_file_t;
#   define HI_OPEN(p, a)       (((a) & DFACC_WRITE) ? \
                                fopen((p), "rb+") : fopen((p), "rb"))
#   define HI_CREATE(p)        (fopen((p), "wb+"))
/* Alias the HI_READ and HI_WRITE macros to functions which can handle */
/*  32-bits of data to read/write */
#   define HI_READ(f, b, n)    (((int32)(n) == HDfreadbig((b), (n), (f))) ? \
                                SUCCEED : FAIL)
#   define HI_WRITE(f, b, n)   (((int32)(n) == HDfwritebig((b), (n), (f))) ? \
                                SUCCEED : FAIL)
#   define HI_CLOSE(f)          (fclose(f))
#   define HI_FLUSH(f)          (fflush(f)==0 ? SUCCEED : FAIL)
#   define HI_SEEK(f,o)  (fseek((f), (long)(o), SEEK_SET)==0 ? SUCCEED : FAIL)
#   define HI_SEEKEND(f) (fseek((f), (long)0, SEEK_END)==0 ? SUCCEED : FAIL)
#   define HI_TELL(f)           (ftell(f))
#   define OPENERR(f)           ((f) == (FILE *)NULL)
#endif /* FILELIB == PCIO */

#if (FILELIB == WINIO)
/* using special MS Windows functions to enable reading/writing large chunks */
typedef HFILE hdf_file_t;
#   define HI_OPEN(p, a)       (((a) & DFACC_WRITE) ? \
                                _lopen((p), READ_WRITE) : _lopen((p), READ))
#   define HI_CREATE(p)        (_lcreat((p), 0))
/* Alias the HI_READ and HI_WRITE macros to functions which can handle */
/*  32-bits of data to read/write */
#   define HI_READ(f, b, n)    (((int32)(n) == HDfreadbig((b), (n), (f))) ? \
                                SUCCEED : FAIL)
#   define HI_WRITE(f, b, n)   (((int32)(n) == HDfwritebig((b), (n), (f))) ? \
                                SUCCEED : FAIL)
#   define HI_CLOSE(f)          (_lclose(f))
#   define HI_FLUSH(f)          (0)
#   define HI_SEEK(f, o)        (_llseek((f), (long)(o), SEEK_SET))
#   define HI_SEEKEND(f)        (_llseek((f), (long)0, SEEK_END))
#   define HI_TELL(f)           (_llseek((f),0l,SEEK_CUR))
#   define OPENERR(f)           ((f) == (HFILE)HFILE_ERROR)
#endif /* FILELIB == WINIO */

#if (FILELIB == WINNTIO)
/* using special Windows NT functions to enable reading/writing large chunks */
typedef HFILE hdf_file_t;
#   define HI_OPEN(p, a)       (((a) & DFACC_WRITE) ? \
                        _lopen((p), OF_READWRITE) : _lopen((p), OF_READ))
#   define HI_CREATE(p)        (_lcreat((p), 0))
#   define HI_READ(f, b, n)    (((int32)(n) == _hread((f), (b), (n))) ? \
                                SUCCEED : FAIL)
#   define HI_WRITE(f, b, n)   (((int32)(n) == _hwrite((f), (b), (n))) ? \
                                SUCCEED : FAIL)
#   define HI_CLOSE(f) (_lclose(f)==0 ? SUCCEED : FAIL)
#   define HI_FLUSH(f) (0)
#   define HI_SEEK(f, o)       (_llseek((f), (long)(o), 0))
#   define HI_SEEKEND(f) (_llseek((f), (long)0, 2))
#   define HI_TELL(f)  (_llseek((f),0l,1))
#   define OPENERR(f)  ((f) == (HFILE)HFILE_ERROR)
#endif /* FILELIB == WINNTIO */

/* ----------------------- Internal Data Structures ----------------------- */
/* The internal structure used to keep track of the files opened: an
   array of filerec_t structures, each has a linked list of ddblock_t.
   Each ddblock_t struct points to an array of dd_t structs. 

   File Header(4 bytes)
   ===================
   <--- 32 bits ----->
   ------------------
   |HDF magic number |
   ------------------

   HDF magic number - 0x0e031301 (Hexadecimal)

   Data Descriptor(DD - 12 bytes)
   ==============================
   <-  16 bits -> <- 16 bits -> <- 32 bits -> <- 32 bits ->
   --------------------------------------------------------
   |    Tag      | reference   |  Offset     |  Length    |
   |             | number      |             |            |
   --------------------------------------------------------
        \____________/
               V
        tag/ref (unique data indentifier in file)
   
   Tag  -- identifies the type of data, 16 bit unsigned integer whose
           value ranges from 1 - 65535. Tags are assigned by NCSA.
           Current tag assingments are:
           00001 - 32767  - reserved for NCSA use
           32768 - 64999  - user definable
           65000 - 65535  - reserved for expansion of format

   Refererence number - 16 bit unsigned integer whose assignment is not
          gauranteed to be consecutive. Provides a way to distinguish 
          elements with the same tag in the file.

   Offset - Specifies the byte offset of the data element from the 
            beginning of the file - 32 bit unsigned integer

   Length - Indicates the byte length of the data element
            32 bit unsigned integer

   Data Descriptor Header(DDH - 6 bytes)
   ====================================
   <-  16 bits -> <- 32 bits ->
   -----------------------------
   | Block Size  | Next Block  |
   -----------------------------

   Block Size - Indicates the number of DD's in the DD Block,
                16 bit unsigned integer value
   Next Block - Gives the offset of the next DD Block. The last DD Block has
                a ZERO(0) in the "Next Block" field in the DDH.
                32 bit unsigned integer value

   Data Descriptor Block(DDB - variable size)
   ==========================================
   <- DD Header -> <--------------- DD's --------------------->
   --------------------------------------------------------...-
   |Block | Next  | DD | DD | DD | DD | DD | DD | DD | DD |...|
   |Size  | Block |    |    |    |    |    |    |    |    |   |
   --------------------------------------------------------...-
   <-------------------------- DD Block ---------------------->

   Note: default number of DD's in a DD Block is 16

   HDF file layout
   =============================
   (one example)
   ---------------------------------------------------------------------
   | FH | DD Block | Data | DD Block | Data | DD Block | Data | .....
   ---------------------------------------------------------------------
 
*/

/* record of each data decsriptor */
typedef struct dd_t
  {
      uint16      tag;          /* Tag number of element i.e. type of data */
      uint16      ref;          /* Reference number of element */
      int32       length;       /* length of data element */
      int32       offset;       /* byte offset of data element from */
  }                             /* beginning of file */
dd_t;

/* version tags */
typedef struct version_t
  {
      uint32      majorv;       /* major version number */
      uint32      minorv;       /* minor version number */
      uint32      release;      /* release number */
      char        string[LIBVSTR_LEN + 1];  /* optional text description, len 80+1 */
      int16       modified;     /* indicates file was modified */
  }
version_t;

/* record of a block of data descriptors, mirrors structure of a HDF file.  */
typedef struct ddblock_t
  {
      intn        dirty;        /* boolean: should this DD block be flushed? */
      int32       myoffset;     /* offset of this DD block in the file */
      int16       ndds;         /* number of dd's in this block */
      int32       nextoffset;   /* offset to the next ddblock in the file */
      struct ddblock_t *next;   /* pointer to the next ddblock in memory */
      struct ddblock_t *prev;   /* Pointer to previous ddblock. */
      struct dd_t *ddlist;      /* pointer to array of dd's */
  }
ddblock_t;

/* hashing information */
#define HASH_MASK       0xff
#define HASH_BLOCK_SIZE 100
/* tag/ref structure */
typedef struct tag_ref_str
  {
      intn        tag;          /* tag for this element */
      intn        ref;          /* ref for this element */
      ddblock_t  *pblock;       /* ddblock this object is in */
      int32       pidx;         /* this object's index into dd block */
  }
tag_ref    , *tag_ref_ptr;

/* tag/ref list structure */
typedef struct tag_ref_list_str
  {
      int         count;        /* number of objects */
      tag_ref     objects[HASH_BLOCK_SIZE];     /* DDs */
      struct tag_ref_list_str *next;    /* next one in the list */
  }
tag_ref_list, *tag_ref_list_ptr;

/* File record structure */
typedef struct filerec_t
  {
      char       *path;         /* name of file */
      hdf_file_t  file;         /* either file descriptor or pointer */
      uint16      maxref;       /* highest ref in this file */
      intn        access;       /* access mode */
      intn        refcount;     /* reference count / times opened */
      intn        attach;       /* number of access elts attached */
      intn        version_set;  /* version tag stuff */
      version_t   version;      /* file version info */

      /* fast lookup of empty dd stuff */
      int32       null_idx;     /* index into null_block of NULL entry */
      struct ddblock_t *null_block;     /* last block a NULL entry was found in */

      /* DD block caching info */
      intn        cache;        /* boolean: whether caching is on */
      intn        dirty;        /* boolean: if dd list needs to be flushed */
      uint32      f_end_off;    /* offset of the end of the file */

      struct ddblock_t *ddhead; /* head of ddblock list */
      struct ddblock_t *ddlast; /* end of ddblock list */

      /* hash table stuff */
      tag_ref_list_ptr hash[HASH_MASK + 1];     /* hashed table of tag / refs */
  }
filerec_t;

/* bits for filerec_t 'dirty' flag */
#define DDLIST_DIRTY   0x01     /* mark whether to flush dirty DD blocks */
#define FILE_END_DIRTY 0x02     /* indicate that the file needs to be extended */

/* Each access element is associated with a tag/ref to keep track of
   the dd it is pointing at.  To facilitate searching for next dd's,
   instead of pointing directly to the dd, we point to the ddblock and
   index into the ddlist of that ddblock. */
typedef struct accrec_t
  {
      intn        appendable;   /* whether appends to the data are allowed */
      intn        flush;        /* whether the DD for this data should be flushed */
      /* when Hendaccess() is called */
      intn        used;         /* whether the access record is used */
      uint32      access;       /* access codes */
      intn        special;      /* special element ? */
      intn        new;          /* is a new element (i.e. no length set yet) */
      uintn       access_type;  /* I/O access type: serial/parallel/... */
      int32       file_id;      /* id of attached file */
      int32       idx;          /* index of dd into *block */
      int32       posn;         /* seek position with respect to */
      /* start of element */
      VOIDP       special_info; /* special element info? */
      struct ddblock_t *block;  /* ptr to ddblock that contains dd */
      struct funclist_t *special_func;  /* ptr to special function? */
  }
accrec_t;

/* Convenience Macros to access the DD for an access record */
#define ACCREC_TAG(a) ((a)->block->ddlist[(a)->idx].tag)
#define ACCREC_REF(a) ((a)->block->ddlist[(a)->idx].ref)
#define ACCREC_OFF(a) ((a)->block->ddlist[(a)->idx].offset)
#define ACCREC_LEN(a) ((a)->block->ddlist[(a)->idx].length)

/* this type is returned to applications programs or other special
   interfaces when they need to know information about a given
   special element.  This is all information that would not be returned
   via Hinquire().  This should probably be a union of structures. */
typedef struct sp_info_block_t
  {
      int16       key;          /* type of special element this is */

      /* external elements */
      int32       offset;       /* offset in the file */
      char       *path;         /* file name - should not be freed by user */

      /* linked blocks */
      int32       first_len;    /* length of first block */
      int32       block_len;    /* length of standard block */
      int32       nblocks;      /* number of blocks per chunk */

      /* compressed elements */
      int32       comp_type;    /* compression type */
      int32       model_type;   /* model type */
      int32       comp_size;    /* size of compressed information */

      /* variable-length linked blocks */
      int32       min_block;    /* the minimum block size */

  }
sp_info_block_t;

/* a function table record for accessing special data elements.
   special data elements of a key could be accessed through the list
   of functions in array pointed to by tab. */
typedef struct funclist_t
  {
      int32       (*stread) HPROTO((accrec_t * rec));
      int32       (*stwrite) HPROTO((accrec_t * rec));
      int32       (*seek) HPROTO((accrec_t * access_rec, int32 offset, intn origin));
      int32       (*inquire) HPROTO((accrec_t * access_rec, int32 *pfile_id,
                                 uint16 *ptag, uint16 *pref, int32 *plength,
                               int32 *poffset, int32 *pposn, int16 *paccess,
                                     int16 *pspecial));
      int32       (*read) HPROTO((accrec_t * access_rec, int32 length, VOIDP data));
      int32       (*write) HPROTO((accrec_t * access_rec, int32 length, const VOIDP data));
      intn        (*endaccess) HPROTO((accrec_t * access_rec));
      int32       (*info) HPROTO((accrec_t * access_rec, sp_info_block_t * info));
      int32       (*reset) HPROTO((accrec_t * access_rec, sp_info_block_t * info));
  }
funclist_t;

typedef struct functab_t
  {
      int16       key;          /* the key for this type of special elt */
      funclist_t *tab;          /* table of accessing functions */
  }
functab_t;

/* ---------------------- ID Types and Manipulation ----------------------- */
/* each id, what ever the type, will be represented with a 32-bit word,
   the most-significant 16 bits is a number unique for type.  The less-
   significant 16 bits is an id unique to each type; in this, we use the
   internal slot number. */

#define FIDTYPE   1
#define AIDTYPE   2
#define GROUPTYPE 3
#define SDSTYPE   4
#define DIMTYPE   5
#define CDFTYPE   6
#define VGIDTYPE  8     /* also defined in vg.h for Vgroups */
#define VSIDTYPE  9     /* also defined in vg.h for Vsets */
#define BITTYPE   10    /* For bit-accesses */
#define GRIDTYPE  11    /* for GR access */
#define RIIDTYPE  12    /* for RI access */
#define FSLOT2ID(s) ((int32)((((uint32)FIDTYPE & 0xffff) << 16)|((s) & 0xffff)))
#define VALIDFID(i) (((((uint32)(i) >> 16) & 0xffff) == FIDTYPE) && \
                     (((uint32)(i) & 0xffff) < MAX_FILE))
#define FID2SLOT(i) (VALIDFID(i) ? (uint32)(i) & 0xffff : -1)
#define FID2REC(i)  ((VALIDFID(i) ? &(file_records[(uint32)(i) & 0xffff]) : \
                      NULL))
#define BADFREC(r)  ((r)==NULL || (r)->refcount==0)
#define ASLOT2ID(s) ((((uint32)AIDTYPE & 0xffff) << 16) | ((s) & 0xffff))
#define VALIDAID(i) (((((uint32)(i) >> 16) & 0xffff) == AIDTYPE) && \
                     (((uint32)(i) & 0xffff) < MAX_ACC) && \
                     (access_records))
#define AID2SLOT(i) (VALIDAID(i) ? (uint32)(i) & 0xffff : -1)
#define AID2REC(i) ((VALIDAID(i) ? &(access_records[(uint32)(i) & 0xffff]) : NULL))

#define NO_ID     (uint32) 0

/* --------------------------- Special Elements --------------------------- */
/* The HDF tag space is divided as follows based on the 2 highest bits:
   00: NCSA reserved ordinary tags
   01: NCSA reserved special tags
   10, 11: User tags.

   It is relatively cheap to operate with special tags within the NCSA
   reserved tags range. For users to specify special tags and their
   corresponding ordinary tag, the pair has to be added to the
   special_table in hfile.c and SPECIAL_TABLE must be defined. */

#ifdef SPECIAL_TABLE
#define BASETAG(t) (HDbase_tag(t))
#define SPECIALTAG(t) (HDis_special_tag(t))
#define MKSPECIALTAG(t) (HDmake_special_tag(t))
#else
#define BASETAG(t)      (uint16)((~(t) & 0x8000) ? ((t) & ~0x4000) : (t))
#define SPECIALTAG(t)   (uint16)((~(t) & 0x8000) && ((t) & 0x4000))
#define MKSPECIALTAG(t) (uint16)((~(t) & 0x8000) ? ((t) | 0x4000) : DFTAG_NULL)
#endif /*SPECIAL_TABLE */

/* ----------------------- Library-Global Variables ----------------------- */
/* access records array.  defined in hfile.c */
extern accrec_t *access_records;

/* file records array.  defined in hfile.c */
#if defined(macintosh) | defined(THINK_C) | defined(DMEM)   /* Dynamic memory */
extern filerec_t *file_records;
#else  /* !macintosh */
extern filerec_t file_records[];
#endif /* !macintosh */

/* */
#define FILE_NDDS(file_rec) ((file_rec)->ddlast->ndds)

/* -------------------------- H-Layer Prototypes -------------------------- */
/*
   ** Functions to get information of special elt from other access records.
   **   defined in hfile.c
   ** These should really be HD... routines, but they are only used within
   **   the H-layer...
 */

#if defined c_plusplus || defined __cplusplus
extern      "C"
{
#endif                          /* c_plusplus || __cplusplus */

    extern int  HIget_access_slot
                (void);

    extern intn HIcount_dd(filerec_t * file_rec, uint16 cnt_tag, uint16 cnt_ref,
                           uintn *all_cnt, uintn *real_cnt);

    extern int  HIfind_dd
                (uint16 look_tag, uint16 look_ref, ddblock_t ** pblock, int32 *pidx,
                 intn direction);

    extern int  HInew_dd_block
                (filerec_t * file_rec, int16 ndds, char *FUNC);

    extern int  HIupdate_dd
                (filerec_t * file_rec, ddblock_t * block, int32 idx, char *FUNC);

    extern VOIDP HIgetspinfo
                (accrec_t * access_rec, uint16 tag, uint16 ref);

    extern int  HIlookup_dd
                (filerec_t * file_rec, uint16 look_tag, uint16 look_ref,
                 ddblock_t ** pblock, int32 *pidx);

    extern int  HIadd_hash_dd
                (filerec_t * file_rec, uint16 look_tag, uint16 look_ref,
                 ddblock_t * pblock, int32 pidx);

    extern int  HIdel_hash_dd
                (filerec_t * file_rec, uint16 look_tag, uint16 look_ref);

    extern int32 HPgetdiskblock
                (filerec_t * file_rec, int32 block_size, intn moveto);

    extern intn HPfreediskblock
                (filerec_t * file_rec, int32 block_offset, int32 block_size);

    extern int32 HDget_special_info
                (int32 access_id, sp_info_block_t * info_block);

/*
   ** from hblocks.c
 */
    extern int32 HLPstread
                (accrec_t * access_rec);

    extern int32 HLPstwrite
                (accrec_t * access_rec);

    extern int32 HLPseek
                (accrec_t * access_rec, int32 offset, int origin);

    extern int32 HLPread
                (accrec_t * access_rec, int32 length, VOIDP data);

    extern int32 HLPwrite
                (accrec_t * access_rec, int32 length, const VOIDP data);

    extern int32 HLPinquire
                (accrec_t * access_rec, int32 *pfile_id, uint16 *ptag, uint16 *pref,
               int32 *plength, int32 *poffset, int32 *pposn, int16 *paccess,
                 int16 *pspecial);

    extern intn HLPendaccess
                (accrec_t * access_rec);

    extern int32 HLPinfo
                (accrec_t * access_rec, sp_info_block_t * info_block);

/*
   ** from hextelt.c
 */
    extern int32 HXPstread
                (accrec_t * rec);

    extern int32 HXPstwrite
                (accrec_t * rec);

    extern int32 HXPseek
                (accrec_t * access_rec, int32 offset, int origin);

    extern int32 HXPread
                (accrec_t * access_rec, int32 length, VOIDP data);

    extern int32 HXPwrite
                (accrec_t * access_rec, int32 length, const VOIDP data);

    extern int32 HXPinquire
                (accrec_t * access_rec, int32 *pfile_id, uint16 *ptag, uint16 *pref,
               int32 *plength, int32 *poffset, int32 *pposn, int16 *paccess,
                 int16 *pspecial);

    extern intn HXPendaccess
                (accrec_t * access_rec);

    extern int32 HXPcloseAID
                (accrec_t * access_rec);

    extern int32 HXPinfo
                (accrec_t * access_rec, sp_info_block_t * info_block);

    extern int32 HXPreset
                (accrec_t * access_rec, sp_info_block_t * info_block);

    extern intn HXPsetaccesstype
                (accrec_t * access_rec);

/*
   ** from hbigext.c
 */
    extern int32 HBPstread
                (accrec_t * rec);

    extern int32 HBPstwrite
                (accrec_t * rec);

    extern int32 HBPseek
                (accrec_t * access_rec, int32 offset, int origin);

    extern int32 HBPread
                (accrec_t * access_rec, int32 length, VOIDP data);

    extern int32 HBPwrite
                (accrec_t * access_rec, int32 length, const VOIDP data);

    extern int32 HBPinquire
                (accrec_t * access_rec, int32 *pfile_id, uint16 *ptag, uint16 *pref,
               int32 *plength, int32 *poffset, int32 *pposn, int16 *paccess,
                 int16 *pspecial);

    extern intn HBPendaccess
                (accrec_t * access_rec);

    extern int32 HBPcloseAID
                (accrec_t * access_rec);

    extern int32 HBPinfo
                (accrec_t * access_rec, sp_info_block_t * info_block);

/*
   ** from hcomp.c
 */

    extern int32 HCPstread
                (accrec_t * rec);

    extern int32 HCPstwrite
                (accrec_t * rec);

    extern int32 HCPseek
                (accrec_t * access_rec, int32 offset, int origin);

    extern int32 HCPinquire
                (accrec_t * access_rec, int32 *pfile_id, uint16 *ptag, uint16 *pref,
               int32 *plength, int32 *poffset, int32 *pposn, int16 *paccess,
                 int16 *pspecial);

    extern int32 HCPread
                (accrec_t * access_rec, int32 length, VOIDP data);

    extern int32 HCPwrite
                (accrec_t * access_rec, int32 length, const VOIDP data);

    extern intn HCPendaccess
                (accrec_t * access_rec);

    extern int32 HCPcloseAID
                (accrec_t * access_rec);

    extern int32 HCPinfo
                (accrec_t * access_rec, sp_info_block_t * info_block);

/*
   ** from hvblocks.c
 */

    extern int32 HVPstread
                (accrec_t * rec);

    extern int32 HVPstwrite
                (accrec_t * rec);

    extern int32 HVPseek
                (accrec_t * access_rec, int32 offset, int origin);

    extern int32 HVPinquire
                (accrec_t * access_rec, int32 *pfile_id, uint16 *ptag, uint16 *pref,
               int32 *plength, int32 *poffset, int32 *pposn, int16 *paccess,
                 int16 *pspecial);

    extern int32 HVPread
                (accrec_t * access_rec, int32 length, VOIDP data);

    extern int32 HVPwrite
                (accrec_t * access_rec, int32 length, const VOIDP data);

    extern intn HVPendaccess
                (accrec_t * access_rec);

    extern int32 HVPinfo
                (accrec_t * access_rec, sp_info_block_t * info_block);

#ifdef MAC
    extern hdf_file_t mopen
                (char *filename, intn access);

    extern int32 mclose
                (hdf_file_t rn);

    extern int32 mlseek
                (hdf_file_t rn, int32 n, intn m);

    extern int32 mread
                (hdf_file_t rn, char *buf, int32 n);

    extern int32 mwrite
                (hdf_file_t rn, char *buf, int32 n);

#endif                          /* MAC */

#if defined c_plusplus || defined __cplusplus
}
#endif                          /* c_plusplus || __cplusplus */

#endif                          /* HFILE_H */

/*
$Header$

$Log$
Revision 1.6  1992/11/30 21:59:11  chouck
Added NO_ID for testing if an ID variable has an invalid content

 * Revision 1.5  1992/11/02  16:35:41  koziol
 * Updates from 3.2r2 -> 3.3
 *
 * Revision 1.4  1992/10/22  22:53:32  chouck
 * Added group handle to group interface
 *
 * Revision 1.3  1992/10/09  20:49:17  chouck
 * Added some patches to work with ThinkC I/O on the Mac
 *
 * Revision 1.2  1992/10/08  19:09:36  chouck
 * Changed file_t to hdf_file_t to make strict ANSI compliant
 *
 * Revision 1.1  1992/08/25  21:40:44  koziol
 * Initial revision
 *
*/
/*+ hfile.h
*** Header for hfile.c, routines for low level data element I/O
+*/

#ifndef HFILE_H
#define HFILE_H

/* maximum number of files (number of slots for file records) */

#ifndef MAX_FILE
#   define MAX_FILE 16
#endif

/* maximum number of access elements */

#ifndef MAX_ACC
#   define MAX_ACC 256
#endif

/* Magic cookie for HDF data files */

#define MAGICLEN 4             /* length */
#define HDFMAGIC "\016\003\023\001" /* ^N^C^S^A */

/* version tags */
/* Library version numbers */

#define LIBVER_MAJOR	3
#define LIBVER_MINOR    3
#define LIBVER_RELEASE	0
#define LIBVER_STRING   "NCSA HDF Version 3.3 Beta Release 1 Nov 1, 1992"
#define LIBVER_LEN	92	/* 4+4+4+80 = 92 */
/* end of version tags */

/* FILELIB -- file library to use for file access: 1 stdio, 2 fcntl
   default to stdio library */

#ifndef FILELIB
#   define FILELIB UNIXBUFIO /* UNIX buffered I/O is the default */
#endif

#if (FILELIB == UNIXBUFIO)
/* using C buffered file I/O routines to access files */
typedef FILE *hdf_file_t;
#ifdef VMS
/* For VMS, use "mbc=64" to improve performance     */
#   define HI_OPEN(p, a)       (((a) & DFACC_WRITE) ? \
                 fopen((p), "r+", "mbc=64") : fopen((p), "r", "mbc=64"))
#else  /*  !VMS  */
#   define HI_OPEN(p, a)       (((a) & DFACC_WRITE) ? \
                        fopen((p), "r+") : fopen((p), "r"))
#endif
#   define HI_CREATE(p)        (fopen((p), "w+"))
#   define HI_READ(f, b, n)    (((n) == fread((b), 1, (n), (f))) ? \
                                SUCCEED : FAIL)
#   define HI_WRITE(f, b, n)   (((n) == fwrite((b), 1, (n), (f))) ? \
                                SUCCEED : FAIL)
#   define HI_CLOSE(f) (fclose(f))
#   define HI_FLUSH(f) (fflush(f))
#   define HI_SEEK(f, o)       (fseek((f), (long)(o), SEEK_SET))
#   define HI_SEEKEND(f) (fseek((f), (long)0, SEEK_END))
#   define HI_TELL(f)  (ftell(f))
#   define OPENERR(f)  ((f) == (FILE *)NULL)
#endif /* FILELIB == UNIXBUFIO */

#if (FILELIB == UNIXUNBUFIO)
/* using UNIX unbuffered file I/O routines to access files */
typedef int hdf_file_t;
#   define HI_OPEN(p, a)       (((a) & DFACC_WRITE) ? \
                        open((p), O_RDWR) : open((p), O_RDONLY))
#   define HI_CREATE(p)        (open((p), O_RDWR | O_CREAT | O_TRUNC))
#   define HI_CLOSE(f) (close(f))
#   define HI_FLUSH(f) (SUCCEED)
#   define HI_READ(f, b, n)    (read((f), (b), (n)))
#   define HI_WRITE(f, b, n)   (write((f), (b), (n)))
#   define HI_SEEK(f, o)       (lseek((f), (off_t)(o), L_SET))
#   define HI_SEEKEND(f) (lseek((f), (off_t)0, L_XTND))
#   define HI_TELL(f)  (lseek((f), (off_t)0, L_INCR))
#   define OPENERR(f)  (f < 0)
#endif /* FILELIB == UNIXUNBUFIO */

#if (FILELIB == MACIO)
/* using special routines to redirect to Mac Toolkit I/O */
typedef short hdf_file_t;
#   define HI_OPEN(x,y) mopen(x,y)
#   define HI_CREATE(name) mopen(name, DFACC_CREATE)
#   define HI_CLOSE(x) mclose(x)
#   define HI_FLUSH(a) (SUCCEED)
#   define HI_READ(a,b,c) mread(a, (char *) b, (int32) c)
#   define HI_WRITE(a,b,c) mwrite(a, (char *) b, (int32) c)
#   define HI_SEEK(x,y) mlseek(x, (int32 )y, 0)
#   define HI_SEEKEND(x) mlseek(x, 0L, 2)
#   define HI_TELL(x) mlseek(x,0L,1)
#   define DF_OPENERR(f)	((f) == -1)
#   define OPENERR(f)  (f < 0)
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
#   define HI_CLOSE(f) (fclose(f))
#   define HI_FLUSH(f) (fflush(f))
#   define HI_SEEK(f, o)       (fseek((f), (long)(o), SEEK_SET))
#   define HI_SEEKEND(f) (fseek((f), (long)0, SEEK_END))
#   define HI_TELL(f)  (ftell(f))
#   define OPENERR(f)  ((f) == (FILE *)NULL)
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
#   define HI_CLOSE(f) (_lclose(f))
#   define HI_FLUSH(f) (0)
#   define HI_SEEK(f, o)       (_llseek((f), (long)(o), SEEK_SET))
#   define HI_SEEKEND(f) (_llseek((f), (long)0, SEEK_END))
#   define HI_TELL(f)  (_llseek((f),0l,SEEK_CUR))
#   define OPENERR(f)  ((f) == (HFILE)HFILE_ERROR)
#endif /* FILELIB == PCIO */

/* The internal structure used to keep track of the files opened: an
   array of filerec_t structures, each has a linked list of ddblock_t.
   Each ddblock_t struct points to an array of dd_t structs. */

/* record of each data decsriptor */

typedef struct dd_t {
    uint16 tag;
    uint16 ref;
    int32 length;
    int32 offset;
} dd_t;

/* version tags */
typedef struct version_t {
    uint32 majorv;		/* major version number */
    uint32 minorv;		/* minor version number */
    uint32 release;		/* release number */
    char string[81];		/* optional text description */
    int16 modified;		/* indicates file was modified */
} version_t;

/* record of a block of data descriptors, mirrors structure of a HDF file.  */
typedef struct ddblock_t {
    int16 ndds;                /* number of dd's in this block */
    int32 nextoffset;          /* offset to the next ddblock in the file */
    struct ddblock_t *next;    /* pointer to the next ddblock in memory */
    struct ddblock_t *prev;    /* Pointer to previous ddblock. */
    struct dd_t *ddlist;       /* pointer to array of dd's */
} ddblock_t;

/* hashing information */
#define HASH_MASK 0xff
typedef struct tag_ref_list_str {
  intn        tag;              /* tag for this element */
  intn        ref;              /* ref for this element */
  ddblock_t   *pblock;          /* ddblock this object is in */
  int32       pidx;             /* this object's index into dd block */
  struct tag_ref_list_str *next;  /* next one in the list */
  VOIDP       my_data;          /* data other interfaces can add / query */
} tag_ref_list, *tag_ref_list_ptr;

typedef struct filerec_t {
    char *path;                 /* name of file */
    hdf_file_t file;            /* either file descriptor or pointer */
    intn access;                /* access mode */
    intn refcount;              /* reference count / times opened */
    struct ddblock_t *ddhead;   /* head of ddblock list */
    struct ddblock_t *ddlast;   /* end of ddblock list */
    uint16 maxref;              /* highest ref in this file */
    intn attach;                /* number of access elts attached */

    /* version tag stuff */
    intn version_set;
    version_t version;		/* file version info */
    
    /* fast lookup of empty dd stuff */
    struct ddblock_t *null_block; /* last block a NULL entry was found in */
    int32             null_idx;   /* index into null_block of NULL entry */

    /* hash table stuff */
    tag_ref_list_ptr hash[HASH_MASK + 1];  /* hashed table of tag / refs */

} filerec_t;

/* Each access element is associated with a tag/ref to keep track of
   the dd it is pointing at.  To facilitate searching for next dd's,
   instead of pointing directly to the dd, we point to the ddblock and
   index into the ddlist of that ddblock. */

typedef struct accrec_t {
    int32 file_id;              /* id of attached file */
    struct ddblock_t *block;    /* ptr to ddblock that contains dd */
    int32 idx;                  /* index of dd into *block */
    int32 posn;                 /* seek position with respect to */
                                /* start of element */
    int16 access;               /* access codes */
    intn used;
    int16 special;
    VOIDP special_info;
    int32 (**special_func)();
} accrec_t;

/* a function table record for accessing special data elements.
   special data elements of a key could be accessed through the list
   of functions in array pointed to by tab. */

typedef struct functab_t {
    int16 key;                 /* the key for this type of special elt */
    int32 (**tab)();           /* table of accessing functions */
} functab_t;

/* sizes of elements in a file.  This is necessary because
   the size of variables need not be the same as in the file
   (cannot use sizeof) */

#define DD_SZ 12               /* 2+2+4+4 */
#define NDDS_SZ 2
#define OFFSET_SZ 4

/* ndds (number of dd's in a block) default,
   so user need not specify */

#ifndef DEF_NDDS
#   define DEF_NDDS 16
#endif

/* ndds minimum, to prevent excessive overhead of very small dd-blocks */

#ifndef MIN_NDDS
#   define MIN_NDDS 4
#endif

/* largest number that will fit into 16-bit word ref variable */

#define MAX_REF ((uint16)32767)

/* indices for special function table: the list of accessing functions
   are ordered so that the functions are always in the same position
   in a table */

#define SP_STREAD 0
#define SP_STWRITE 1
#define SP_SEEK 2
#define SP_INQUIRE 3
#define SP_READ 4
#define SP_WRITE 5
#define SP_END 6

/* macros */

/* each id, what ever the type, will be represented with a 32-bit word,
   the most-significant 16 bits is a number unique for type.  The less-
   significant 16 bits is an id unique to each type; in this, we use the
   internal slot number. */

#define FIDTYPE   1
#define AIDTYPE   2
#define GROUPTYPE 3
#define FSLOT2ID(s) ((((uint32)FIDTYPE & 0xffff) << 16) | ((s) & 0xffff))
#define VALIDFID(i) (((((uint32)(i) >> 16) & 0xffff) == FIDTYPE) && \
                    (((uint32)(i) & 0xffff) < MAX_FILE))
#define FID2SLOT(i) (VALIDFID(i) ? (uint32)(i) & 0xffff : -1)
#define FID2REC(i) ((VALIDFID(i) ? &(file_records[(uint32)(i) & 0xffff]) : NULL))
#define ASLOT2ID(s) ((((uint32)AIDTYPE & 0xffff) << 16) | ((s) & 0xffff))
#define VALIDAID(i) (((((uint32)(i) >> 16) & 0xffff) == AIDTYPE) && \
                    (((uint32)(i) & 0xffff) < MAX_ACC) && \
                    (access_records))
#define AID2SLOT(i) (VALIDAID(i) ? (uint32)(i) & 0xffff : -1)
#define AID2REC(i) ((VALIDAID(i) ? &(access_records[(uint32)(i) & 0xffff]) : NULL))

#define NO_ID     (uint32) 0

/* a tag is special if its tag belongs to a special set.  This test can be
   just ((t)==SPECIAL1 || (t)==SPECIAL2), or a full blown function.
   right now, no special tags yet */

#define BASETAG(t) (HDbase_tag(t))
#define SPECIALTAG(t) (HDis_special_tag(t))
#define MKSPECIALTAG(t) (HDmake_special_tag(t))

/* access records array.  defined in hfile.c */

extern accrec_t *access_records;

/* file records array.  defined in hfile.c */

extern filerec_t file_records[];

/* */
#define FILE_NDDS(file_rec) ((file_rec)->ddlast->ndds)


/* 
** Functions to get information of special elt from other access records.
**   defined in hfile.c 
** These should really be HD... routines, but they are only used within
**   the H-layer...
*/

extern int HIget_access_slot
  PROTO((void));

extern int HIfind_dd
  PROTO((uint16 look_tag, uint16 look_ref, ddblock_t **pblock, int32 *pidx));

extern int HInew_dd_block
  PROTO((filerec_t *file_rec, int16 ndds, char *FUNC));

extern int HIupdate_dd
  PROTO((filerec_t *file_rec, ddblock_t *block, int32 idx, char *FUNC));

extern VOIDP HIgetspinfo
  PROTO((accrec_t *access_rec, uint16 tag, uint16 ref));

extern int HIlookup_dd
  PROTO((filerec_t *file_rec, uint16 look_tag, uint16 look_ref,
         ddblock_t **pblock, int32 *pidx));

extern int HIadd_hash_dd
  PROTO((filerec_t *file_rec, uint16 look_tag, uint16 look_ref,
         ddblock_t *pblock, int32 pidx));

extern int HIdel_hash_dd
  PROTO((filerec_t *file_rec, uint16 look_tag, uint16 look_ref));

extern int32 HXIcloseAID
    PROTO((accrec_t *access_rec));
	
#ifdef MAC
extern hdf_file_t mopen
	PROTO((char * filename, intn access));
	
extern int mwrite
	PROTO((hdf_file_t rn, char *buf, intn n));
	
extern int mlseek
	PROTO((hdf_file_t rn, int32 n, intn m));

extern int32 mread
    PROTO((hdf_file_t rn, char *buf, int32 n));

extern int32 mwrite
    PROTO((hdf_file_t rn, char *buf, int32 n));

#endif

#endif /* HFILE_H */




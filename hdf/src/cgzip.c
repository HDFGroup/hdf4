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

#ifdef RCSID
static char RcsId[] = "$Revision$";
#endif

/* $Id$ */

/*
   FILE
   cgzip.c
   HDF gzip encoding I/O routines

   REMARKS

   DESIGN

   EXPORTED ROUTINES
   None of these routines are designed to be called by other users except
   for the modeling layer of the compression routines.

   AUTHOR
   Quincey Koziol

   MODIFICATION HISTORY
   2/10/95     Starting coding
 */

/* General HDF includes */
#include "hdf.h"
#include "hfile.h"

#define CGZIP_MASTER
#define CODER_CLIENT
/* HDF compression includes */
#include "hcompi.h"     /* Internal definitions for compression */

/* Internal Defines */
/* #define TESTING */

#ifdef LATER
/* declaration of the functions provided in this module */
PRIVATE int32 HCIcgzip_init(accrec_t * access_rec);

/*--------------------------------------------------------------------------
 NAME
    HCIcskphuff_splay -- Splay the tree around the source code passed

 USAGE
    void HCIcskphuff_splay(skphuff_info,plain)
    comp_coder_skphuff_info_t *skphuff_info;    IN:ptr to skphuff info
    uint8 plain;            IN: the source code to splay the tree around

 RETURNS
    None.

 DESCRIPTION
    Common code called by HCIcskphuff_encode and HCIcskphuff_decode

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
static void
HCIcskphuff_splay(comp_coder_skphuff_info_t * skphuff_info, uint8 plain)
{
    uintn       a, b;           /* children of nodes to semi-rotate */
    uint8       c, d;           /* pair of nodes to semi-rotate */
    uintn       skip_num;       /* the tree we are splaying */

    skip_num = skphuff_info->skip_pos;  /* get the tree number to splay */
    a = plain + SUCCMAX;    /* get the index for this source code in the up array */
    do
      {     /* walk up the tree, semi-rotating pairs */
          c = skphuff_info->up[skip_num][a];    /* find the parent of the node to semi-rotate around */
          if (c != ROOT)
            {   /* a pair remain above this node */
                d = skphuff_info->up[skip_num][c];  /* get the grand-parent of the node to semi-rotate around */
                b = skphuff_info->left[skip_num][d];

/* Exchange the children of the pair */
                if (c == b)
                  {
                      b = skphuff_info->right[skip_num][d];
                      skphuff_info->right[skip_num][d] = a;
                  }     /* end if */
                else
                    skphuff_info->left[skip_num][d] = a;

                if (a == skphuff_info->left[skip_num][c])
                    skphuff_info->left[skip_num][c] = b;
                else
                    skphuff_info->right[skip_num][c] = b;

                skphuff_info->up[skip_num][a] = d;
                skphuff_info->up[skip_num][b] = c;
                a = d;
            }   /* end if */
          else
            {   /* handle odd node at end */
                a = c;
            }   /* end else */
      }
    while (a != ROOT);
}   /* end HCIcskphuff_splay() */

/*--------------------------------------------------------------------------
 NAME
    HCIcskphuff_init -- Initialize a skipping huffman compressed data element.

 USAGE
    int32 HCIcskphuff_init(access_rec)
    accrec_t *access_rec;   IN: the access record of the data element

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Common code called by HCIcskphuff_staccess and HCIcskphuff_seek

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
PRIVATE int32
HCIcskphuff_init(accrec_t * access_rec)
{
    CONSTR(FUNC, "HCIcskphuff_init");
    compinfo_t *info;           /* special element information */
    comp_coder_skphuff_info_t *skphuff_info;    /* ptr to skphuff info */
    intn        i, j, k;        /* local counting var */

    info = (compinfo_t *) access_rec->special_info;

#ifdef TESTING
    printf("HCIcskphuff_init(): before Hbitseek() call\n");
#endif /* TESTING */
    if (Hbitseek(info->aid, 0, 0) == FAIL)  /* seek to beginning of element */
        HRETURN_ERROR(DFE_SEEKERROR, FAIL);

#ifdef TESTING
    printf("HCIcskphuff_init(): after Hbitseek() call\n");
#endif /* TESTING */
    skphuff_info = &(info->cinfo.coder_info.skphuff_info);

    /* Initialize RLE state information */
    skphuff_info->skip_pos = 0;     /* start in first byte */
    skphuff_info->offset = 0;   /* start at the beginning of the data */

    /* allocate pointers to the compression buffers */
    if ((skphuff_info->left = (uintn **) HDgetspace(sizeof(uintn *) * skphuff_info->skip_size)) == NULL)
                    HRETURN_ERROR(DFE_NOSPACE, FAIL);
    if ((skphuff_info->right = (uintn **) HDgetspace(sizeof(uintn *) * skphuff_info->skip_size)) == NULL)
                    HRETURN_ERROR(DFE_NOSPACE, FAIL);
    if ((skphuff_info->up = (uint8 **) HDgetspace(sizeof(uint8 *) * skphuff_info->skip_size)) == NULL)
                    HRETURN_ERROR(DFE_NOSPACE, FAIL);

#ifdef TESTING
    printf("HCIcskphuff_init(): halfway through allocating space\n");
#endif /* TESTING */
    /* allocate compression buffer for each skipping byte */
    for (i = 0; i < skphuff_info->skip_size; i++)
      {
          if ((skphuff_info->left[i] = (uintn *) HDgetspace(sizeof(uintn) * SUCCMAX)) == NULL)
                          HRETURN_ERROR(DFE_NOSPACE, FAIL);
          if ((skphuff_info->right[i] = (uintn *) HDgetspace(sizeof(uintn) * SUCCMAX)) == NULL)
                          HRETURN_ERROR(DFE_NOSPACE, FAIL);
          if ((skphuff_info->up[i] = (uint8 *) HDgetspace(sizeof(uint8) * TWICEMAX)) == NULL)
                          HRETURN_ERROR(DFE_NOSPACE, FAIL);
      }     /* end for */

#ifdef TESTING
    printf("HCIcskphuff_init(): after allocating space\n");
#endif /* TESTING */
    for (k = 0; k < skphuff_info->skip_size; k++)
      {
          for (i = 0; i < TWICEMAX; i++)    /* initialize the up pointers to point to their parent in the tree */
              skphuff_info->up[k][i] = i >> 1;

          for (j = 0; j < SUCCMAX; j++)
            {   /* initialize the left & right pointers correctly */
                skphuff_info->left[k][j] = j << 1;
                skphuff_info->right[k][j] = (j << 1) + 1;
            }   /* end for */
      }     /* end for */

#ifdef TESTING
    printf("HCIcskphuff_init(): after initializing arrays\n");
#endif /* TESTING */
    return (SUCCEED);
}   /* end HCIcskphuff_init() */

/*--------------------------------------------------------------------------
 NAME
    HCIcskphuff_decode -- Decode skipping Huffman compressed data into a buffer.

 USAGE
    int32 HCIcskphuff_decode(info,length,buf)
    compinfo_t *info;   IN: the info about the compressed element
    int32 length;       IN: number of bytes to read into the buffer
    uint8 *buf;         OUT: buffer to store the bytes read

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Common code called to decode skipping Huffman data from the file.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
PRIVATE int32
HCIcskphuff_decode(compinfo_t * info, int32 length, uint8 *buf)
{
    CONSTR(FUNC, "HCIcskphuff_decode");
    comp_coder_skphuff_info_t *skphuff_info;    /* ptr to skipping Huffman info */
    int32       orig_length;    /* original length to read */
    intn        bit;            /* bit from the file */
    uintn       a;
    uint8       plain;          /* the source code expanded from the file */

    skphuff_info = &(info->cinfo.coder_info.skphuff_info);

    orig_length = length;   /* save this for later */
    while (length > 0)
      {     /* decode until we have all the bytes we need */
#ifdef TESTING
printf("length=%ld\n",(long)length);
#endif /* TESTING */
          a = ROOT;     /* start at the root of the tree and find the leaf we need */

          do
            {   /* walk down once for each bit on the path */
#ifdef TESTING
intn bitcount=0;
printf("bitcount=%d\n",++bitcount);
#endif /* TESTING */
                if ((bit = Hgetbit(info->aid)) == 0)    /* get the next bit from the file */
                    a = skphuff_info->left[skphuff_info->skip_pos][a];
                else if (bit == 1)
                    a = skphuff_info->right[skphuff_info->skip_pos][a];
                else    /* bit==-1, i.e. an error */
                    HRETURN_ERROR(DFE_CDECODE, FAIL);
            }
          while (a <= MAXCHAR);

          plain = a - SUCCMAX;
          HCIcskphuff_splay(skphuff_info, plain);
          skphuff_info->skip_pos = (skphuff_info->skip_pos + 1) % skphuff_info->skip_size;
          *buf++ = plain;
          length--;
      }     /* end while */
    skphuff_info->offset += orig_length;    /* incr. abs. offset into the file */
    return (SUCCEED);
}   /* end HCIcskphuff_decode() */

/*--------------------------------------------------------------------------
 NAME
    HCIcskphuff_encode -- Encode data from a buffer into skipping Huffman
                            compressed data

 USAGE
    int32 HCIcskphuff_encode(info,length,buf)
    compinfo_t *info;   IN: the info about the compressed element
    int32 length;       IN: number of bytes to store from the buffer
    uint8 *buf;         OUT: buffer to get the bytes from

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Common code called to encode skipping Huffman data into a file.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
PRIVATE int32
HCIcskphuff_encode(compinfo_t * info, int32 length, uint8 *buf)
{
    CONSTR(FUNC, "HCIcskphuff_encode");
    comp_coder_skphuff_info_t *skphuff_info;    /* ptr to skipping Huffman info */
    int32       orig_length;    /* original length to write */
    uint8       stack_ptr = 0;  /* pointer to the position on the stack */
    intn        stack[MAXCHAR]; /* stack to store the bits generated */
    uintn       a;              /* variable to record the position in the tree */

    skphuff_info = &(info->cinfo.coder_info.skphuff_info);

    orig_length = length;   /* save this for later */
    while (length > 0)
      {     /* encode until we stored all the bytes */
          a = *buf + SUCCMAX;   /* find position in the up array */
          do
            {   /* walk up the tree, pushing bits */
                stack[stack_ptr] = (skphuff_info->right[skphuff_info->skip_pos][skphuff_info->up[skphuff_info->skip_pos][a]] == a);     /* push a 1 is this is the right node */
                stack_ptr++;
                a = skphuff_info->up[skphuff_info->skip_pos][a];
            }
          while (a != ROOT);

          do
            {   /* output the bits we have */
                stack_ptr--;
                if (Hputbit(info->aid, stack[stack_ptr]) == FAIL)
                    HRETURN_ERROR(DFE_CENCODE, FAIL);
            }
          while (stack_ptr != 0);
          HCIcskphuff_splay(skphuff_info, *buf);    /* semi-splay the tree around this node */
          skphuff_info->skip_pos = (skphuff_info->skip_pos + 1) % skphuff_info->skip_size;
          buf++;
          length--;
      }     /* end while */

    skphuff_info->offset += orig_length;    /* incr. abs. offset into the file */
    return (SUCCEED);
}   /* end HCIcskphuff_encode() */

/*--------------------------------------------------------------------------
 NAME
    HCIcskphuff_term -- Flush encoded data from internal buffer to skipping
                    Huffman compressed data

 USAGE
    int32 HCIcskphuff_term(info)
    compinfo_t *info;   IN: the info about the compressed element

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Common code called to flush skipping Huffman data into a file.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
PRIVATE int32
HCIcskphuff_term(compinfo_t * info)
{
#ifdef LATER
    CONSTR(FUNC, "HCIcskphuff_term");
#endif /* endif LATER */
    comp_coder_skphuff_info_t *skphuff_info;    /* ptr to skipping Huffman info */

    skphuff_info = &(info->cinfo.coder_info.skphuff_info);

    skphuff_info->skip_pos = 0;

    return (SUCCEED);
}   /* end HCIcskphuff_term() */

/*--------------------------------------------------------------------------
 NAME
    HCIcskphuff_staccess -- Start accessing a skipping Huffman compressed data element.

 USAGE
    int32 HCIcskphuff_staccess(access_rec, access)
    accrec_t *access_rec;   IN: the access record of the data element
    int16 access;           IN: the type of access wanted

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Common code called by HCIcskphuff_stread and HCIcskphuff_stwrite

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
PRIVATE int32
HCIcskphuff_staccess(accrec_t * access_rec, int16 acc_mode)
{
    CONSTR(FUNC, "HCIcskphuff_staccess");
    compinfo_t *info;           /* special element information */

    info = (compinfo_t *) access_rec->special_info;

#ifdef TESTING
    printf("HCIcskphuff_staccess(): before bitio calls\n");
#endif /* TESTING */
    /* need to check for not writing, as opposed to read access */
    /* because of the way the access works */
    if (!(acc_mode&DFACC_WRITE))
        info->aid = Hstartbitread(access_rec->file_id, DFTAG_COMPRESSED,
                                  info->comp_ref);
    else
        info->aid = Hstartbitwrite(access_rec->file_id, DFTAG_COMPRESSED,
                                   info->comp_ref, info->length);

#ifdef TESTING
    printf("HCIcskphuff_staccess(): after bitio calls\n");
#endif /* TESTING */
    if (info->aid == FAIL)
        HRETURN_ERROR(DFE_DENIED, FAIL);
    if ((acc_mode&DFACC_WRITE) && Hbitappendable(info->aid) == FAIL)
        HRETURN_ERROR(DFE_DENIED, FAIL);
    return (HCIcskphuff_init(access_rec));  /* initialize the skip-Huffman info */
}   /* end HCIcskphuff_staccess() */
#endif /* LATER */

/*--------------------------------------------------------------------------
 NAME
    HCPcgzip_stread -- start read access for compressed file

 USAGE
    int32 HCPcgzip_stread(access_rec)
    accrec_t *access_rec;   IN: the access record of the data element

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Start read access on a compressed data element using the gzip scheme.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
int32
HCPcgzip_stread(accrec_t * access_rec)
{
#ifdef LATER
    CONSTR(FUNC, "HCPcgzip_stread");
    int32       ret;

    if ((ret = HCIcskphuff_staccess(access_rec, DFACC_READ)) == FAIL)
        HRETURN_ERROR(DFE_CINIT, FAIL);
    return (ret);
#endif /* LATER */
}   /* HCPcgzip_stread() */

/*--------------------------------------------------------------------------
 NAME
    HCPcgzip_stwrite -- start write access for compressed file

 USAGE
    int32 HCPcgzip_stwrite(access_rec)
    accrec_t *access_rec;   IN: the access record of the data element

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Start write access on a compressed data element using the gzip scheme.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
int32
HCPcgzip_stwrite(accrec_t * access_rec)
{
#ifdef LATER
    CONSTR(FUNC, "HCPcgzip_stwrite");
    int32       ret;

#ifdef TESTING
    printf("HCPcskphuff_stwrite(): before call to HCIcskphuff_staccess()\n");
#endif
    if ((ret = HCIcskphuff_staccess(access_rec, DFACC_WRITE)) == FAIL)
        HRETURN_ERROR(DFE_CINIT, FAIL);
#ifdef TESTING
    printf("HCPcskphuff_stwrite(): after call to HCIcskphuff_staccess(), ret=%d\n", (int) ret);
#endif
    return (ret);
#endif /* LATER */
}   /* HCPcgzip_stwrite() */

/*--------------------------------------------------------------------------
 NAME
    HCPcgzip_seek -- Seek to offset within the data element

 USAGE
    int32 HCPcgzip_seek(access_rec,offset,origin)
    accrec_t *access_rec;   IN: the access record of the data element
    int32 offset;       IN: the offset in bytes from the origin specified
    intn origin;        IN: the origin to seek from [UNUSED!]

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Seek to a position with a compressed data element.  The 'origin'
    calculations have been taken care of at a higher level, it is an
    un-used parameter.  The 'offset' is used as an absolute offset
    because of this.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
int32
HCPcgzip_seek(accrec_t * access_rec, int32 offset, int origin)
{
#ifdef LATER
    CONSTR(FUNC, "HCPcgzip_seek");
    compinfo_t *info;           /* special element information */
    comp_coder_skphuff_info_t *skphuff_info;    /* ptr to skipping Huffman info */
    uint8      *tmp_buf;        /* pointer to throw-away buffer */

    /* shut compiler up */
    origin = origin;

    info = (compinfo_t *) access_rec->special_info;
    skphuff_info = &(info->cinfo.coder_info.skphuff_info);

    if (offset < skphuff_info->offset)
      {     /* need to seek from the beginning */
          if (HCIcskphuff_init(access_rec) == FAIL)
              HRETURN_ERROR(DFE_CINIT, FAIL);
      }     /* end if */

    if ((tmp_buf = (uint8 *) HDgetspace(TMP_BUF_SIZE)) == NULL)     /* get tmp buffer */
        HRETURN_ERROR(DFE_NOSPACE, FAIL);

    while (skphuff_info->offset + TMP_BUF_SIZE < offset)    /* grab chunks */
        if (HCIcskphuff_decode(info, TMP_BUF_SIZE, tmp_buf) == FAIL)
          {
              HDfreespace(tmp_buf);
              HRETURN_ERROR(DFE_CDECODE, FAIL);
          }     /* end if */
    if (skphuff_info->offset < offset)  /* grab the last chunk */
        if (HCIcskphuff_decode(info, offset - skphuff_info->offset, tmp_buf) == FAIL)
          {
              HDfreespace(tmp_buf);
              HRETURN_ERROR(DFE_CDECODE, FAIL);
          }     /* end if */

    HDfreespace(tmp_buf);
    return (SUCCEED);
#endif /* LATER */
}   /* HCPcgzip_seek() */

/*--------------------------------------------------------------------------
 NAME
    HCPcgzip_read -- Read in a portion of data from a compressed data element.

 USAGE
    int32 HCPcgzip_read(access_rec,length,data)
    accrec_t *access_rec;   IN: the access record of the data element
    int32 length;           IN: the number of bytes to read
    VOIDP data;             OUT: the buffer to place the bytes read

 RETURNS
    Returns the number of bytes read or FAIL

 DESCRIPTION
    Read in a number of bytes from the gzip compressed data element.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
int32
HCPcgzip_read(accrec_t * access_rec, int32 length, VOIDP data)
{
#ifdef LATER
    CONSTR(FUNC, "HCPcgzip_read");
    compinfo_t *info;           /* special element information */

    info = (compinfo_t *) access_rec->special_info;

    if (HCIcskphuff_decode(info, length, data) == FAIL)
        HRETURN_ERROR(DFE_CDECODE, FAIL);

    return (length);
#endif /* LATER */
}   /* HCPcgzip_read() */

/*--------------------------------------------------------------------------
 NAME
    HCPcgzip_write -- Write out a portion of data from a compressed data element.

 USAGE
    int32 HCPcgzip_write(access_rec,length,data)
    accrec_t *access_rec;   IN: the access record of the data element
    int32 length;           IN: the number of bytes to write
    VOIDP data;             IN: the buffer to retrieve the bytes written

 RETURNS
    Returns the number of bytes written or FAIL

 DESCRIPTION
    Write out a number of bytes to the gzip compressed data element.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
int32
HCPcgzip_write(accrec_t * access_rec, int32 length, const VOIDP data)
{
#ifdef LATER
    CONSTR(FUNC, "HCPcgzip_write");
    compinfo_t *info;           /* special element information */
    comp_coder_skphuff_info_t *skphuff_info;    /* ptr to skipping Huffman info */

    info = (compinfo_t *) access_rec->special_info;
    skphuff_info = &(info->cinfo.coder_info.skphuff_info);

    /* Don't allow random write in a dataset unless: */
    /*  1 - append onto the end */
    /*  2 - start at the beginning and rewrite (at least) the whole dataset */
    if ((info->length != skphuff_info->offset)
        && (skphuff_info->offset != 0 || length < info->length))
        HRETURN_ERROR(DFE_UNSUPPORTED, FAIL);

    if (HCIcskphuff_encode(info, length, data) == FAIL)
        HRETURN_ERROR(DFE_CENCODE, FAIL);

    return (length);
#endif /* LATER */
}   /* HCPcgzip_write() */

/*--------------------------------------------------------------------------
 NAME
    HCPcgzip_inquire -- Inquire information about the access record and data element.

 USAGE
    int32 HCPcgzip_inquire(access_rec,pfile_id,ptag,pref,plength,poffset,pposn,
            paccess,pspecial)
    accrec_t *access_rec;   IN: the access record of the data element
    int32 *pfile_id;        OUT: ptr to file id
    uint16 *ptag;           OUT: ptr to tag of information
    uint16 *pref;           OUT: ptr to ref of information
    int32 *plength;         OUT: ptr to length of data element
    int32 *poffset;         OUT: ptr to offset of data element
    int32 *pposn;           OUT: ptr to position of access in element
    int16 *paccess;         OUT: ptr to access mode
    int16 *pspecial;        OUT: ptr to special code

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Inquire information about the access record and data element.
    [Currently a NOP].

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
int32
HCPcgzip_inquire(accrec_t * access_rec, int32 *pfile_id, uint16 *ptag,
                    uint16 *pref, int32 *plength, int32 *poffset,
                    int32 *pposn, int16 *paccess, int16 *pspecial)
{
    /* shut compiler up */
    access_rec = access_rec;
    pfile_id = pfile_id;
    ptag = ptag;
    pref = pref;
    plength = plength;
    poffset = poffset;
    pposn = pposn;
    paccess = paccess;
    pspecial = pspecial;

    return (SUCCEED);
}   /* HCPcgzip_inquire() */

/*--------------------------------------------------------------------------
 NAME
    HCPcgzip_endaccess -- Close the compressed data element

 USAGE
    int32 HCPcgzip_endaccess(access_rec)
    accrec_t *access_rec;   IN: the access record of the data element

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Close the compressed data element and free encoding info.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
intn
HCPcgzip_endaccess(accrec_t * access_rec)
{
#ifdef LATER
    CONSTR(FUNC, "HCPcgzip_endaccess");
    compinfo_t *info;           /* special element information */

    info = (compinfo_t *) access_rec->special_info;

    /* flush out RLE buffer */
    if (access_rec->access&DFACC_WRITE)
        if (HCIcskphuff_term(info) == FAIL)
            HRETURN_ERROR(DFE_CTERM, FAIL);

    /* close the compressed data AID */
    if (Hendbitaccess(info->aid, 0) == FAIL)
        HRETURN_ERROR(DFE_CANTCLOSE, FAIL);

    return (SUCCEED);
#endif /* LATER */
}   /* HCPcgzip_endaccess() */

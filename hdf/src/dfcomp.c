#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.1  1992/08/25 21:40:44  koziol
Initial revision

*/
/*-----------------------------------------------------------------------------
 * File:    dfcomp.c
 * Purpose: File compression
 * Invokes: df.c dfimcomp.c df.h
 * Contents:
 *  DFputcomp: compress image and write it to HDF file
 *  DFgetcomp: read compressed image from HDF file and decompress it
 *  DFCrle: compress string using run length encoding
 *  DFCunrle: decompress string using run length encoding
 * Remarks: DFgetcomp and DFputcomp constitute a general compression interface
 *---------------------------------------------------------------------------*/

#include "hdf.h"
#include "herr.h"

#define R8_MAX_BLOCKS 32
#define R8_MAX_LENGTH 512

/*-----------------------------------------------------------------------------
 * Name:    DFputcomp
 * Purpose: Compress and write images to HDF file
 * Inputs:  file_id: pointer to HDF file
 *          tag, ref: tag, ref of compressed image for writing out
 *          image: image to be compressed
 *          xdim, ydim: dimensions of image
 *          palette: palette associated with image
 *          newpal: modified palette, produced if compression scheme is IMCOMP
 *          scheme: compression scheme to be used
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF programmers, DF8putrig, other routines
 * Invokes: DFCrle, DFCimcomp, DFaccess, DFwrite, DFIcheck
 * Remarks: IMCOMP modifies the palette associated with the image
 *          Hence the palette and newpal arguments
 *          This is a general compression interface - to be used anytime image
 *          compression is needed in HDF
 *          Note that rseq does its own compression, because that is part of
 *          the interactive color raster protocol
 *          The space needed for compression and decompression can be allocated
 *          statically or dynamically, depending on the DF_DYNAMIC flag, and
 *          for entire image or part of it (reused) depending on availability
 *          Accordingly, writing out is whole image, or row by row
 *          Note that compression is always row by row for RLE.
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DFputcomp(int32 file_id, uint16 tag, uint16 ref, uint8 *image, int32 xdim,
             int32 ydim, uint8 *palette, uint8 *newpal, int16 scheme)
#else
int DFputcomp(file_id, tag, ref, image, xdim, ydim, palette, newpal, scheme)
    int32 file_id;
    uint16 tag, ref;
    uint8 *image;
    int32 xdim, ydim;
    uint8 *palette;
    uint8 *newpal;
    int16 scheme;
#endif
{
    char *FUNC="DFputcomp";
    uint8 *buffer;             /* buffer to hold compressed image */
    uint8 *in;                 /* pointer to input for compression */
    uint8 *out;                /* pointer to space for compressed output */
    int32 cisize;              /* maximum size of compressed image */
    int32 crowsize;            /* maximum size of compressed row */
    intn  buftype;             /* buftype = 1: buffer enough for whole image*/
                               /* buftype = 2: buffer holds 1 row */
    int32 n;                   /* number of compressed bytes produced */
    int32 total;               /* total compressed bytes produced so far */
    int32 i;
    int ret=0;
    int32 aid;

    if (!HDvalidfid(file_id) || !tag || !ref
       || xdim <= 0 || ydim <= 0 || !image) {
       HERROR(DFE_ARGS);
       return FAIL;
    }

    switch (scheme) {
      case DFTAG_RLE:
       cisize = ydim*(xdim*121/120+1); /* 120 chars can compress to 121! */
       crowsize = xdim*121/120 + 128;

       /* allocate buffer for compression */
       buffer = HDgetspace((uint32)cisize);
       if (!buffer) {
           buffer = HDgetspace((uint32)crowsize);
           if (!buffer) {
               HERROR(DFE_NOSPACE);
               return FAIL;
           }
           buftype = 2;        /* compress and write out row by row */
       }
       else buftype = 1;       /* can hold whole image, then write */

       in = image;
       out = buffer;
       n = total = 0;          /* no bytes compressed so far */

       if (buftype == 2) {
           int32 num_blocks;
           int32 block_length;

           num_blocks = (ydim > (int32)R8_MAX_BLOCKS) ? (int32)R8_MAX_BLOCKS
                        : ydim;
           block_length = (xdim > (int32)R8_MAX_LENGTH) ? (int32)R8_MAX_LENGTH
                        : xdim;
           aid = HLcreate(file_id, tag, ref, block_length, num_blocks);
           if (aid == FAIL) {
               return FAIL;
           }
       }

       /* compress row by row */
       for (i=0; i<ydim; i++) {
           n = DFCrle((VOIDP)in, (VOIDP)out, xdim); /* compress row */
           in += xdim;                /* move input pointer */
           total += n;                /* keep running total */
           if (buftype==1)       /* can hold whole image */
               out = &buffer[total]; /* move out buffer pointer */
           else {                /* buffer too small, */
                                      /* write out what was produced */
               if (Hwrite(aid, n, buffer) == FAIL) {
                   ret = -1;       /* flag value */
                   break;
               }
               out = buffer;   /* reset output pointer */
           }
       }

       if (buftype==1) { /* write out entire image */
           ret = Hputelement(file_id, tag, ref, buffer, total);
           HDfreespace(buffer);
       }
       break;

      case DFTAG_IMC:
        if (!palette || !newpal) { /* need palette and newpal */
           HERROR(DFE_ARGS);
            return FAIL;
        }
        cisize = xdim*ydim/4;  /* IMCOMP always cuts to 1/4 */

        buffer = HDgetspace((uint32)cisize);
        if (!buffer) {
            HERROR(DFE_NOSPACE);
            return FAIL;
        }

        DFCimcomp(xdim, ydim, image, buffer, palette, newpal, 0);
        ret = Hputelement(file_id, tag, ref, buffer, cisize);

       HDfreespace(buffer);

        break;

    default:                   /* unknown compression scheme */
        HERROR(DFE_ARGS);
        return FAIL;
    }

    return(ret);
}

/*-----------------------------------------------------------------------------
 * Name:    DFgetcomp
 * Purpose: Read compressed image and decompress it
 * Inputs:  file_id: HDF file pointer
 *          tag, ref: id of image to be decompressed
 *          image: space to return decompressed image in
 *          xdim, ydim: dimensions of decompressed image
 *          scheme: compression scheme used
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF programmers, DF8getrig, other routines
 * Invokes: DFIcheck, DFIfind, DFaccess, DFread, DFCunrle, DFCunimcomp
 * Remarks: Will use dynamic/static memory allocation for buffer
 *          will read in image in parts if memory insufficient
 *          Decompression of rle is not necessarily row by row
 *          Other encodings can also be decoded with this
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DFgetcomp(int32 file_id, uint16 tag, uint16 ref, uint8 *image, int32 xdim,
             int32 ydim, uint16 scheme)
#else
int DFgetcomp(file_id, tag, ref, image, xdim, ydim, scheme)
    int32 file_id;
    uint16 tag, ref;
    uint8 *image;
    int32 xdim, ydim;
    uint16 scheme;
#endif
{
    char *FUNC="DFgetcomp";
    uint8 *buffer;
    uint8 *in;
    uint8 *out;
    int32 cisize, crowsize, buflen, bufleft; /* bufleft: bytes left in buffer*/

    int32 i;
    int32 totalread;
    int32 n;
    int32 aid;

    if (!HDvalidfid(file_id) || !tag || !ref
        || xdim <= 0 || ydim <= 0 || !image) {
       HERROR(DFE_ARGS);
       return FAIL;
    }

    aid = Hstartread(file_id, tag, ref);
    if (aid == FAIL) {
        HERROR(DFE_NOMATCH);
        return FAIL;
    }
    if (Hinquire(aid, (int32*)NULL, (uint16*)NULL, (uint16*)NULL, &cisize,
                (int32*)NULL, (int32*)NULL, (int16*)NULL, (int16*)NULL) == FAIL) {
       return FAIL;
    }

    switch (scheme) {
      case DFTAG_RLE:
       crowsize = xdim*121/120 + 128; /* max size of a row */

       buffer = HDgetspace((uint32)cisize);
       if (!buffer) {
           buffer = HDgetspace((uint32)crowsize);
           if (!buffer) {
               HERROR(DFE_NOSPACE);
               Hendaccess(aid);
               return FAIL;
           }
           buflen = crowsize;
       }
       else buflen = cisize;

       in = buffer;
       out = image;
       if ((n=Hread(aid, buflen, in))<0) {
           HDfreespace(buffer);
           Hendaccess(aid);
           return(-1);
       }
       totalread = n;
       bufleft = n;
       for (i=0; i<ydim; i++) {
           n = DFCunrle(in, out, xdim, !i); /* no of bytes used up */
           /* last arg=TRUE if i=0 - resets decompress */
           in += n;
           out += xdim;
           bufleft -= n;
           /* check if more bytes may be needed for next read */
           if ((bufleft<crowsize) && (totalread<cisize)) {
               memcpy(buffer, in, (size_t)bufleft);
               in = buffer;
               if ((n=Hread(aid,buflen-bufleft,(uint8 *)&in[bufleft]))<0) {
                   HDfreespace(buffer);
                   Hendaccess(aid);
                   return FAIL;
               }
               totalread += n;
               bufleft += n;
           }
       }

       Hendaccess(aid);
       HDfreespace(buffer);

       break;

      case DFTAG_IMC:
       crowsize = xdim;        /* size of compressed row */

       buffer = HDgetspace((uint32)cisize);
       if (!buffer) {
           buffer = HDgetspace((uint32)crowsize);
           if (!buffer) {
               HERROR(DFE_NOSPACE);
               Hendaccess(aid);
               return FAIL;
           }
           buflen = crowsize;
       }
       else buflen = cisize;
       if (buflen>=cisize) {
           if (Hread(aid, cisize, buffer) < cisize) {
               HDfreespace(buffer);
               Hendaccess(aid);
               return FAIL;
           }
           /* HDfreespace(buffer); */
           Hendaccess(aid);
           DFCunimcomp(xdim, ydim, buffer, image);
           HDfreespace(buffer);
           break;              /* go to end of switch */
       }

       in = buffer;            /* if can only read piecemeal */
       out = image;
       if ((n=Hread(aid, buflen, in))<0) {
           HDfreespace(buffer);
           Hendaccess(aid);
           return FAIL;
       }
       totalread = n;
       bufleft = n;
       for (i=0; i<ydim; i+=4) {
           DFCunimcomp(xdim, (int32)4, in, out);
           in += xdim;
           out += 4*xdim;
           bufleft -= xdim;
           if ((bufleft<crowsize) && (totalread<cisize)) {
               memcpy(buffer, in, (size_t)bufleft);
               in = buffer;
               if ((n=Hread(aid,buflen-bufleft,(uint8 *)&in[bufleft]))<0) {
                   HDfreespace(buffer);
                   Hendaccess(aid);
                   return FAIL;
               }
               totalread += n;
               bufleft += n;
           }
       }

       HDfreespace(buffer);
       Hendaccess(aid);

       break;

      default:                 /* unknown scheme */
       HERROR(DFE_ARGS);
       return FAIL;
    }

    return SUCCEED;
}

/*-----------------------------------------------------------------------------
 * Name:    DFCrle
 * Purpose: compress a string of bytes
 * Inputs:  buf: buffer containing data to be compressed
 *          bufto: space for compressed data - assumed big enough
 *          len: number of bytes to compress
 * Returns: number of compressed bytes on success, -1 on failure
 * Users:   HDF programmers, DFputcomp, other routines
 * Invokes: none
 * Remarks: Written for efficiency
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int32 DFCrle(VOIDP buf, VOIDP bufto, int32 len)
#else
int32 DFCrle(buf,bufto,len)
    VOIDP buf;
    VOIDP bufto;
    int32 len;
#endif
{
    register uint8 * p;
    register uint8 * q;
    register uint8 * cfoll;
    register uint8 * clead;
    uint8 * begp;
    int32 i;

    p = (uint8 *)buf;
    cfoll = (uint8 *)bufto;             /* place to copy to */
    clead = cfoll + 1;

    begp = p;
    while (len > 0) {           /* encode stuff until gone */

        q = p + 1;
        i = len-1;
        while (i && i+120 > len && *p == *q) {
            q++;
            i--;
        }

        if (q - p > 2) {        /* three in a row */
            if (p > begp) {
                *cfoll = (uint8)(p - begp);
                cfoll = clead;
            }
            *cfoll++ = (uint8)128 | (uint8)(q-p); /* len of seq */
            *cfoll++ = *p;      /* char of seq */
            len -= q-p;         /* subtract len of seq */
            p = q;
            clead = cfoll+1;
            begp = p;
        }
        else {
            *clead++ = *p++;    /* copy one char */
            len--;
            if (p - begp > 120) {
                *cfoll = (uint8)(p - begp);
                cfoll = clead++;
                begp = p;
            }
        }

    }
/*
 *  fill in last bytecount
 */
    if (p > begp)
        *cfoll = (uint8)(p - begp);
    else
        clead--;                    /* don't need count position */

    return((int32)((uint8*)clead - (uint8*)bufto)); /* how many encoded */
}

/*-----------------------------------------------------------------------------
 * Name:    DFCunrle
 * Purpose: decompress run length encoding
 * Inputs:  buf: buffer containing compressed data
 *          bufto: space for returning decompressed data
 *          outlen: number of *decompressed* bytes desired.
 *          resetsave: don't use any stored state info - used for fresh image
 * Returns: number of compressed bytes used up on success, -1 on failure
 * Users:   HDF programmers, DFgetcomp, other routines
 * Invokes: none
 * Remarks: has been modified so it will decompress even non-rowwise compression
 *          Hence the static storage stuff
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int32 DFCunrle(uint8 *buf, uint8 *bufto, int32 outlen, int resetsave)
#else
int32 DFCunrle(buf,bufto,outlen, resetsave)
    uint8 *buf;
    uint8 *bufto;
    int32 outlen;
    int resetsave;
#endif
{
    register int cnt;
    register uint8 * p;
    register uint8 * q;
    uint8 * endp;
    static uint8 save[255], *savestart=NULL, *saveend=NULL;
    /* save has a list of decompressed bytes not returned in
       previous call.  savestart and saveend specify the position
       at which this list starts and ends in the array save */

    p = (uint8 *)buf;
    endp = (uint8 *)bufto + outlen;
    q = (uint8 *)bufto;
    if (resetsave) savestart = saveend = save; /* forget saved state */
    while ((saveend>savestart) && (q<endp)) /* copy saved stuff */
        *q++ = *savestart++;
    if (savestart>=saveend) savestart = saveend = save;        /* all copied */
    while (q < endp) {
        cnt = *p++;            /* count field */
        if (!(cnt & 128)) {    /* is set of uniques */
            while (cnt--) {
                if (q<endp)
                    *q++ = *p++; /* copy unmodified */
                else
                    *saveend++ = *p++;
            }
        }
        else {
            cnt &= 127;                /* strip high bit */
            while (cnt--) {
                if (q<endp)
                    *q++ = *p;  /* copy unmodified */
                else
                    *saveend++ = *p;
            }
            p++;                /* skip that character */
        }
    }
    return((int32)(p - buf));
}

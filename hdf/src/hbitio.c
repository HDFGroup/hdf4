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
static char RcsId[] = "@(#)$Revision$";
#endif

/* $Id$ */

/*+
 FILE
       hbitio.c
       HDF bit level I/O routines
 REMARKS
        These functions operate on top of the "H" layer routines
        (i.e. they call Hstartread, Hstartwrite, Hread, Hseek, Hwrite, etc.)
        and depend on them for all actual I/O to data elements in the
        file.  This may be somewhat slow, but it prevents having
        to duplicate code for that access.
 EXPORTED ROUTINES
        Hstartbitread - open a dataset for bitfile dataset reading
        Hstartbitwrite - open a dataset for bitfile dataset writing
        Happendable - make a writable dataset appendable
        Hbitread - read bits from a bitfile dataset
        Hbitwrite - write bits to a bitfile dataset
        Hbitseek - seek to a given bit offset in a bitfile dataset
        Hendbitaccess - close off access to a bitfile dataset
 AUTHOR
       Quincey Koziol
 MODIFICATION HISTORY
    3/15/92     Starting writing
+*/

#define BITMASTER
#include "hdf.h"
#include "hfile.h"

/* Local Variables */

/* Array of records of information on each bitfile element.
   These will contain information like how to access the data element,
   where in the data element the current access should start from, etc.
   Allocated dynamically.
   See hbitio.h for definition. */

static struct bitrec_t *bitfile_records = NULL;

/* Local Function Declarations */
PRIVATE int HIget_bitfile_slot
    (void);

PRIVATE intn HIbitflush (bitrec_t *bitfile_rec,intn flushbit,bool writeout);

/* #define TESTING */
/* Actual Function Definitions */

/*--------------------------------------------------------------------------

 NAME
       Hstartbitread -- locate and position a bit-read access elt on a tag/ref
 USAGE
       int32 Hstartbitread(fileid, tag, ref)
       int fileid;             IN: id of file to attach access element to
       int tag;                IN: tag to search for
       int ref;                IN: ref to search for
 RETURNS
       returns id of bit-access element if successful, otherwise FAIL (-1)
 DESCRIPTION
        Calls Hstartread and initializes bit-level structures.
 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
int32 Hstartbitread(int32 file_id, uint16 tag, uint16 ref)
{
    CONSTR(FUNC,"Hstartbitread");   /* for HERROR */
    int bitslot;        /* slot in bit-access record array */
    int32 aid;          /* Access ID for the bit-level routines to use */
    struct bitrec_t *bitfile_rec;   /* Pointer to the bitfile record */

    /* clear error stack */
    HEclear();

    /* Try to get an AID */
    if((aid=Hstartread(file_id,tag,ref))==FAIL)
        HRETURN_ERROR(DFE_BADAID,FAIL);

    /* get a slot in the access record array */
    if((bitslot=HIget_bitfile_slot())==FAIL)
       HRETURN_ERROR(DFE_TOOMANY,FAIL);

    bitfile_rec = &(bitfile_records[bitslot]);
    bitfile_rec->acc_id=aid;
    bitfile_rec->bit_id=SLOT2BITID(bitslot);
    if(HQuerylength(aid,&bitfile_rec->max_offset)==FAIL)
        HRETURN_ERROR(DFE_INTERNAL,FAIL);
    bitfile_rec->byte_offset=0;

    bitfile_rec->mode='r';
    bitfile_rec->bytez=bitfile_rec->bytea+BITBUF_SIZE;

    /* pre-read the first block into the buffer */
    if(bitfile_rec->max_offset>bitfile_rec->byte_offset) {
        int32 read_size;    /* number of bytes to read into buffer */
        int32 n;            /* number of bytes actually read */

        read_size=MIN((bitfile_rec->max_offset-bitfile_rec->byte_offset),BITBUF_SIZE);
        if((n=Hread(bitfile_rec->acc_id,read_size,bitfile_rec->bytea))==FAIL)
            return(FAIL);   /* EOF? somebody pulled the rug out from under us! */
        bitfile_rec->buf_read=n;    /* keep track of the number of bytes in buffer */
        bitfile_rec->bytep=bitfile_rec->bytea;  /* set to the beginning of the buffer */
      } /* end if */
    else {
        bitfile_rec->bytep=bitfile_rec->bytez;  /* set to the end of the buffer to force read */
        bitfile_rec->buf_read=0;    /* set the number of bytes in buffer to 0 */
      } /* end else */
    bitfile_rec->block_offset=0;
    bitfile_rec->count=0;

    return SLOT2BITID(bitslot);
}   /* Hstartbitread() */

/*--------------------------------------------------------------------------

 NAME
       Hstartbitwrite -- set up a bit access elt for a write
 USAGE
       int32 Hstartbitwrite(fileid, tag, ref, len)
       int fileid;             IN: id of file to write to
       int tag;                IN: tag to write to
       int ref;                IN: ref to write to
       long length;            IN: the length of the data element (in bytes)
 RETURNS
       returns id of bit access element if successful and FAIL otherwise
 DESCRIPTION
       Set up a bit-write access elt to write out a data element.  Calls
       Hstartwrite for most initialization and just initializes the bit-
       level stuff here.
 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
int32 Hstartbitwrite(int32 file_id, uint16 tag, uint16 ref, int32 length)
{
    CONSTR(FUNC,"Hstartbitwrite");  /* for HERROR */
    int bitslot;            /* free access records array slot */
    bitrec_t *bitfile_rec;  /* access record */
    int32 aid;              /* Access ID for the bit-level routines to use */
    bool exists;            /* whether dataset exists already */

    /* clear error stack and check validity of file id */
    HEclear();

    exists=(Hexist(file_id,tag,ref)==SUCCEED) ? TRUE : FALSE;

    /* Try to get an AID */
    if((aid=Hstartwrite(file_id,tag,ref,length))==FAIL)
        HRETURN_ERROR(DFE_BADAID,FAIL);

    /* get empty slot in bit-access records */
    if((bitslot = HIget_bitfile_slot())==FAIL)
        HRETURN_ERROR(DFE_TOOMANY,FAIL);

    bitfile_rec = &(bitfile_records[bitslot]);
    bitfile_rec->acc_id=aid;
    bitfile_rec->bit_id=SLOT2BITID(bitslot);
    bitfile_rec->byte_offset=0;
    bitfile_rec->block_offset=0;
    if(exists==TRUE) {
        if(HQuerylength(aid,&bitfile_rec->max_offset)==FAIL)
            HRETURN_ERROR(DFE_INTERNAL,FAIL);

        /* pre-read the first block into the buffer */
        if(bitfile_rec->max_offset>bitfile_rec->byte_offset) {
            int32 read_size;    /* number of bytes to read into buffer */
            int32 n;            /* number of bytes actually read */

            read_size=MIN((bitfile_rec->max_offset-bitfile_rec->byte_offset),BITBUF_SIZE);
            if((n=Hread(bitfile_rec->acc_id,read_size,bitfile_rec->bytea))==FAIL)
                HRETURN_ERROR(DFE_READERROR,FAIL);   /* EOF? somebody pulled the rug out from under us! */
            bitfile_rec->buf_read=n;    /* keep track of the number of bytes in buffer */
            if(Hseek(bitfile_rec->acc_id,bitfile_rec->block_offset,DF_START)==FAIL)
                HRETURN_ERROR(DFE_SEEKERROR,FAIL);
          } /* end if */
      } /* end if */
    else {
        bitfile_rec->max_offset=0;
        bitfile_rec->buf_read=0;    /* set the number of bytes in buffer to 0 */
      } /* end else */
    bitfile_rec->mode='w';
    bitfile_rec->bytez=bitfile_rec->bytea+BITBUF_SIZE;
    bitfile_rec->bytep=bitfile_rec->bytea;  /* set to the beginning of the buffer */
    bitfile_rec->count=BITNUM;
    bitfile_rec->bits=0;

    return SLOT2BITID(bitslot);
}   /* end Hstartbitwrite() */

/*--------------------------------------------------------------------------

 NAME
       Hbitappendable -- make a bitio AID appendable
 USAGE
       intn Hbitappendable(bitid)
       int32 bitid;         IN: id of bit-element to make appendable
 RETURNS
        SUCCEED for success
        FAIL to indicate failure
 DESCRIPTION
       If a dataset is at the end of a file, allow Hbitwrite()s to write
       past the end of a file.  Allows expanding datasets without the use
       of linked blocks.
 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
intn Hbitappendable(int32 bitid)
{
    CONSTR(FUNC,"Hbitappendable");    /* for HERROR */
    bitrec_t *bitfile_rec;  /* access record */

    /* clear error stack and check validity of file id */
    HEclear();

    if((bitfile_rec = BITID2REC(bitid))==NULL)
        HRETURN_ERROR(DFE_ARGS,FAIL);

    /* Check for write access */
    if(bitfile_rec->mode!='w')
        HRETURN_ERROR(DFE_BADACC,FAIL);

    if(Happendable(bitfile_rec->acc_id)==FAIL)
        HRETURN_ERROR(DFE_NOTENOUGH,FAIL);
    return(SUCCEED);
}   /* end Hbitappendable() */

/*--------------------------------------------------------------------------

 NAME
       Hbitwrite -- write a number of bits out to a bit-element
 USAGE
       intn Hbitwrite(bitid, count, data)
       int32 bitid;         IN: id of bit-element to write to
       intn count;          IN: number of bits to write
       uint32 data;         IN: actual data bits to output
                            (bits to output must be in the low bits)
 RETURNS
       the number of bits written for successful write,
       FAIL to indicate failure
 DESCRIPTION
       Write a number of bits out to a bit-element.  This function
       buffers the bits and then writes them out when appropriate
       with Hwrite().
 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
intn Hbitwrite(int32 bitid, intn count, uint32 data)
{
    CONSTR(FUNC,"Hbitwrite"); /* for HERROR */
    bitrec_t *bitfile_rec;  /* access record */
    intn orig_count=count;  /* keep track of orig, number of bits to output */

    /* clear error stack and check validity of file id */
    HEclear();

    if(count<=0 || (bitfile_rec = BITID2REC(bitid))==NULL)
        HRETURN_ERROR(DFE_ARGS,FAIL);

#ifdef TESTING
printf("Hbitwrite(): bitid=%d count=%d, data=%x, bitfile_rec->count=%d\n",bitid,count,data,bitfile_rec->count);
#endif
    /* Check for write access */
    if(bitfile_rec->mode!='w')
        HRETURN_ERROR(DFE_BADACC,FAIL);

    if(count>DATANUM)
        count=DATANUM;

    data&=maskl[count];

    /* if the new bits will not fill up a byte, then just */
    /* merge the new bits into the current bits buffer */
    if(count<bitfile_rec->count) {
        bitfile_rec->bits|=data<<(bitfile_rec->count-=count);
        return(orig_count);
      } /* end if */

    /* fill up the current bits buffer and output the byte */
    *(bitfile_rec->bytep)=bitfile_rec->bits|data>>(count-=bitfile_rec->count);
    bitfile_rec->byte_offset++;
    if(++bitfile_rec->bytep==bitfile_rec->bytez) {
	int32 write_size;

#ifdef TESTING
printf("Hbitwrite(): past end of buffer on first chunk\n");
#endif
        write_size=bitfile_rec->bytez-bitfile_rec->bytea;
        bitfile_rec->bytep=bitfile_rec->bytea;
        if(Hwrite(bitfile_rec->acc_id,write_size,bitfile_rec->bytea)==FAIL)
            HRETURN_ERROR(DFE_WRITEERROR,FAIL);
        bitfile_rec->block_offset+=write_size;
        if(bitfile_rec->byte_offset>bitfile_rec->max_offset)
            bitfile_rec->max_offset=bitfile_rec->byte_offset;

#ifdef TESTING
printf("Hbitwrite(): max_offset=%d, byte_offset=%d, block_offset=%d\n",bitfile_rec->max_offset,bitfile_rec->byte_offset,bitfile_rec->block_offset);
#endif
        /* check if we should pre-read the next block into the buffer */
        if(bitfile_rec->max_offset>bitfile_rec->byte_offset) {
            int32 read_size;    /* number of bytes to read into buffer */
            int32 n;            /* number of bytes actually read */

#ifdef TESTING
printf("Hbitwrite(): pre-reading a block\n");
#endif
            read_size=MIN((bitfile_rec->max_offset-bitfile_rec->byte_offset),BITBUF_SIZE);
            if((n=Hread(bitfile_rec->acc_id,read_size,bitfile_rec->bytea))==FAIL)
                HRETURN_ERROR(DFE_READERROR,FAIL);   /* EOF? somebody pulled the rug out from under us! */
            bitfile_rec->buf_read=n;    /* keep track of the number of bytes in buffer */
            if(Hseek(bitfile_rec->acc_id,bitfile_rec->block_offset,DF_START)==FAIL)
                HRETURN_ERROR(DFE_SEEKERROR,FAIL);
          } /* end if */
      } /* end if */

    /* output any and all remaining whole bytes */
    while(count>=BITNUM) {
        *(bitfile_rec->bytep)=data>>(count-=BITNUM);
        bitfile_rec->byte_offset++;
        if(++bitfile_rec->bytep==bitfile_rec->bytez) {
            int32 write_size;

#ifdef TESTING
printf("Hbitwrite(): past end of buffer in loop\n");
#endif
            write_size=bitfile_rec->bytez-bitfile_rec->bytea;
            bitfile_rec->bytep=bitfile_rec->bytea;
            if(Hwrite(bitfile_rec->acc_id,write_size,bitfile_rec->bytea)==FAIL)
                HRETURN_ERROR(DFE_WRITEERROR,FAIL);
            bitfile_rec->block_offset+=write_size;
            if(bitfile_rec->byte_offset>bitfile_rec->max_offset)
                bitfile_rec->max_offset=bitfile_rec->byte_offset;

#ifdef TESTING
printf("Hbitwrite(): max_offset=%d, byte_offset=%d, block_offset=%d\n",bitfile_rec->max_offset,bitfile_rec->byte_offset,bitfile_rec->block_offset);
#endif
            /* check if we should pre-read the next block into the buffer */
            if(bitfile_rec->max_offset>bitfile_rec->byte_offset) {
                int32 read_size;    /* number of bytes to read into buffer */
                int32 n;            /* number of bytes actually read */

#ifdef TESTING
printf("Hbitwrite(): pre-reading a block in the loop\n");
#endif
                read_size=MIN((bitfile_rec->max_offset-bitfile_rec->byte_offset),BITBUF_SIZE);
                if((n=Hread(bitfile_rec->acc_id,read_size,bitfile_rec->bytea))==FAIL)
                    HRETURN_ERROR(DFE_READERROR,FAIL);   /* EOF? somebody pulled the rug out from under us! */
                bitfile_rec->buf_read=n;    /* keep track of the number of bytes in buffer */
                if(Hseek(bitfile_rec->acc_id,bitfile_rec->block_offset,DF_START)==FAIL)
                    HRETURN_ERROR(DFE_SEEKERROR,FAIL);
              } /* end if */
          } /* end if */
      } /* end while */

    /* put any remaining bits into the bits buffer */
    if((bitfile_rec->count=BITNUM-count)>0)
        bitfile_rec->bits=data<<bitfile_rec->count;

    return(orig_count);
}   /* end Hbitwrite() */

/*--------------------------------------------------------------------------

 NAME
       Hbitread -- read a number of bits from a bit-element
 USAGE
       intn Hbitread(bitid, count, data)
       int32 bitid;         IN: id of bit-element to write to
       intn count;          IN: number of bits to write
       uint32 *data;        IN: pointer to the bits to read
                            OUT: points to the bits read in
                            (bits input will be in the low bits)
 RETURNS
       the number of bits read for successful write,
       FAIL to indicate failure
 DESCRIPTION
       Read a number of bits from a bit-element.  This function
       buffers the bits and then reads them when appropriate
       with Hread().
 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
intn Hbitread(int32 bitid, intn count, uint32 *data)
{
    CONSTR(FUNC,"Hbitread"); /* for HERROR */
    bitrec_t *bitfile_rec;  /* access record */
    register uint32 l;
    uint32 b=0;         /* bits to return */
    uint16 orig_count;  /* the original number of bits to read in */
    int32 n;

    /* clear error stack and check validity of file id */
    HEclear();

    if(count<=0 || (bitfile_rec = BITID2REC(bitid))==NULL)
        HRETURN_ERROR(DFE_ARGS,FAIL);

    /* Check for write access */
    if(bitfile_rec->mode!='r')
        HRETURN_ERROR(DFE_BADACC,FAIL);

    if(count>DATANUM)    /* truncate the count if it's too large */
        count=DATANUM;

#ifdef TESTING
printf("Hbitread(): BITNUM=%d, count=%d, bitfile_rec->count=%d\n",BITNUM,count,bitfile_rec->count);
printf("Hbitread(): bitid=%d, data=%p\n",bitid,data);
#endif
    /* if the request can be satisfied with just the */
    /* buffered bits then do the shift and return */
    if(count<=bitfile_rec->count) {
        *data=(bitfile_rec->bits>>(bitfile_rec->count-=count)&(uint32)maskc[count]);
        return(count);
      } /* end if */

    /* keep track of the original number of bits to read in */
    orig_count=count;

    /* get all the buffered bits into the correct position first */
    if(bitfile_rec->count>0) {
        b=bitfile_rec->bits&maskc[bitfile_rec->count];
        b<<=(count-=bitfile_rec->count);
      } /* end if */

    /* bring in as many whole bytes as the request allows */
    while(count>=BITNUM) {
        if(bitfile_rec->bytep==bitfile_rec->bytez) {
#ifdef TESTING
printf("Hbitread(): reading next chunk in loop, bytep=%p, bytez=%p\n",bitfile_rec->bytep,bitfile_rec->bytez);
#endif
            n=Hread(bitfile_rec->acc_id,BITBUF_SIZE,bitfile_rec->bytea);
            if(n==FAIL) {       /* EOF */
                bitfile_rec->count=0;  /* make certain that we don't try to access the file->bits information */
                *data=b;         /* assign the bits read in */
#ifdef TESTING
printf("Hbitread(): !!!Error (a), fell off end of data\n");
#endif
                return(orig_count-count);   /* break out now */
              } /* end if */
            bitfile_rec->block_offset+=bitfile_rec->buf_read;    /* keep track of the number of bytes in buffer */
            bitfile_rec->bytez=n+(bitfile_rec->bytep=bitfile_rec->bytea);
            bitfile_rec->buf_read=n;    /* keep track of the number of bytes in buffer */
          } /* end if */
        l = *bitfile_rec->bytep++;
        b |= l << (count-=BITNUM);
        bitfile_rec->byte_offset++;
        if(bitfile_rec->byte_offset>bitfile_rec->max_offset)
            bitfile_rec->max_offset=bitfile_rec->byte_offset;
#ifdef TESTING
printf("Hbitread(): count=%d, l=%d, b=%d, max_offset=%d, byte_offset=%d\n",count,l,b,bitfile_rec->max_offset,bitfile_rec->byte_offset);
#endif
      } /* end while */

    /* split any partial request with the bits buffer */
    if(count>0) {
#ifdef TESTING
printf("Hbitread(): splitting partial request for bits\n");
#endif
        if(bitfile_rec->bytep==bitfile_rec->bytez) {
#ifdef TESTING
printf("Hbitread(): reading next chunk, bytep=%p, bytez=%p\n",bitfile_rec->bytep,bitfile_rec->bytez);
#endif
            n=Hread(bitfile_rec->acc_id,BITBUF_SIZE,bitfile_rec->bytea);
            if(n==FAIL) {          /* EOF */
#ifdef TESTING
printf("Hbitread(): !!!Error (b), fell off end of data\n");
#endif
                bitfile_rec->count=0;  /* make certain that we don't try to access the file->bits information */
                *data=b;         /* assign the bits read in */
                return(orig_count-count);      /* return now */
              } /* end if */
            bitfile_rec->block_offset+=bitfile_rec->buf_read;    /* keep track of the number of bytes in buffer */
            bitfile_rec->bytez=n+(bitfile_rec->bytep=bitfile_rec->bytea);
            bitfile_rec->buf_read=n;    /* keep track of the number of bytes in buffer */
#ifdef TESTING
printf("Hbitread(): after reading next chunk, n=%d, block_offset=%d\n",n,bitfile_rec->block_offset);
#endif
          } /* end if */
        bitfile_rec->count=(BITNUM-count);
        l=bitfile_rec->bits = *bitfile_rec->bytep++;
        b|=l>>bitfile_rec->count;
        bitfile_rec->byte_offset++;
        if(bitfile_rec->byte_offset>bitfile_rec->max_offset)
            bitfile_rec->max_offset=bitfile_rec->byte_offset;
#ifdef TESTING
printf("Hbitread(): max_offset=%d, byte_offset=%d\n",bitfile_rec->max_offset,bitfile_rec->byte_offset);
#endif
      } /* end if */
    else
        bitfile_rec->count=0;

    *data=b;
    return(orig_count);
}   /* end Hbitread() */

/*--------------------------------------------------------------------------

 NAME
       Hbitseek -- seek to a given bit position in a bit-element
 USAGE
       intn Hbitseek(bitid, offset)
       int32 bitid;         IN: id of bit-element to write to
       intn byte_offset;    IN: byte offset in the bit-element
       intn bit_offset;     IN: bit offset from the byte offset

 RETURNS
       returns FAIL (-1) if fail, SUCCEED (0) otherwise.
 DESCRIPTION
       Seek to a bit offset in a bit-element.
 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
        If seeking to the 15th bit in a bit-element, the call would look like:
            Hbitseek(bitid,1,7);

        Converting from a direct bit offset variable to this call looks like:
            Hbitseek(bitid,bit_offset/8,bit_offset%8);
     REVISION LOG
--------------------------------------------------------------------------*/
intn Hbitseek(int32 bitid, int32 byte_offset, intn bit_offset)
{
    CONSTR(FUNC,"Hbitseek");  /* for HERROR */
    bitrec_t *bitfile_rec;  /* access record */
    intn seek_pos;          /* position of block to seek to */
    int32 read_size;        /* number of bytes to read into buffer */
    int32 n;                /* number of bytes actually read */
    bool new_block;         /* whether to move to another block in the dataset */

    /* clear error stack and check validity of file id */
    HEclear();

#ifdef TESTING
printf("Hbitseek(): bitid=%d byte_offset=%d, bit_offset=%d\n",bitid,byte_offset,bit_offset);
#endif
    if(byte_offset<0 || bit_offset<0 || bit_offset>(BITNUM-1)
            || (bitfile_rec = BITID2REC(bitid))==NULL 
            || byte_offset>bitfile_rec->max_offset)
        HRETURN_ERROR(DFE_ARGS,FAIL);

    if(byte_offset < 0 || bit_offset < 0 || byte_offset > bitfile_rec->max_offset
            || bit_offset>(BITNUM-1))
        HRETURN_ERROR(DFE_ARGS,FAIL);

    /* determine whether we need to seek to another block in the file */
    new_block=(byte_offset<bitfile_rec->block_offset 
		|| byte_offset>=bitfile_rec->block_offset+BITBUF_SIZE) 
		? TRUE : FALSE;
#ifdef TESTING
printf("Hbitseek(): new_block=%s, block_offset=%d\n",(new_block ? "TRUE" : "FALSE"),bitfile_rec->block_offset);
#endif
    if(bitfile_rec->mode=='w')
        if(HIbitflush(bitfile_rec,-1,new_block)==FAIL) /* flush, but merge */
            HRETURN_ERROR(DFE_WRITEERROR,FAIL);

    if(new_block==TRUE) {
        seek_pos=(byte_offset/BITBUF_SIZE)*BITBUF_SIZE;
#ifdef TESTING
printf("Hbitseek(): seek_pos=%d\n",seek_pos);
#endif
        if(Hseek(bitfile_rec->acc_id,seek_pos,DF_START)==FAIL)
            HRETURN_ERROR(DFE_SEEKERROR,FAIL);

        read_size=MIN((bitfile_rec->max_offset-seek_pos),BITBUF_SIZE);
#ifdef TESTING
printf("Hbitseek(): read_size=%d\n",read_size);
#endif
        if((n=Hread(bitfile_rec->acc_id,read_size,bitfile_rec->bytea))==FAIL)
            HRETURN_ERROR(DFE_READERROR,FAIL);   /* EOF? somebody pulled the rug out from under us! */
        bitfile_rec->bytez=n+(bitfile_rec->bytep=bitfile_rec->bytea);
        bitfile_rec->buf_read=n;    /* keep track of the number of bytes in buffer */
        bitfile_rec->block_offset=seek_pos;
#ifdef TESTING
printf("Hbitseek(): bytez=%p, bytea=%p, bytep=%p, buf_read=%d, block_offset=%d\n",bitfile_rec->bytez,bitfile_rec->bytea,bitfile_rec->bytep,bitfile_rec->buf_read,bitfile_rec->block_offset);
#endif
        if(bitfile_rec->mode=='w')  /* if writing, return the file offset to it's original position */
            if(Hseek(bitfile_rec->acc_id,seek_pos,DF_START)==FAIL)
                HRETURN_ERROR(DFE_SEEKERROR,FAIL);
      } /* end if */

    bitfile_rec->byte_offset=byte_offset;

    /* set to the correct position in the buffer */
    bitfile_rec->bytep=bitfile_rec->bytea+(byte_offset-bitfile_rec->block_offset);
    if(bit_offset>0) {
        bitfile_rec->count=BITNUM-bit_offset;
        if(bitfile_rec->mode=='w') {  /* if writing, mask off bits not yet written */
            bitfile_rec->bits=*(bitfile_rec->bytep);
            bitfile_rec->bits&=maskc[bit_offset]<<bitfile_rec->count;
	  } /* end if */
	else {
            bitfile_rec->bits = *bitfile_rec->bytep++;
	  } /* end else */
      } /* end if */
    else {
        if(bitfile_rec->mode=='w') { /* if writing, mask off bits not yet written */
            bitfile_rec->count=BITNUM;
            bitfile_rec->bits=0;
	  } /* end if */
	else {
            bitfile_rec->count=0;
#ifdef QAK
            bitfile_rec->bits = *bitfile_rec->bytep++;
#endif
	  } /* end else */
      } /* end else */

#ifdef TESTING
printf("Hbitseek(): offset=%d, count=%d\n",(bitfile_rec->bytep-bitfile_rec->bytea),bitfile_rec->count);
#endif
    return(SUCCEED);
}   /* end Hbitseek() */

/*--------------------------------------------------------------------------

 NAME
       Hendbitaccess -- to dispose of a bitfile element
 USAGE
       int32 Hendbitaccess(bitfile_id,flushbit)
       int32 bitfile_id;        IN: id of bitfile element to dispose of
       intn flushbit;           IN: determines how to flush leftover bits
                                   (leftover bits are bits that have been
                                    buffered, but are less than the
                                    BITNUM (usually set to 8) number of
                                    bits)
                                    0 - flush with zeros
                                    1 - flush with ones
                                   -1 - throw away any leftover bits
 RETURNS
       returns SUCCEED (0) if successful, FAIL (-1) otherwise
 DESCRIPTION
       Used to dispose of a bitfile element.  Flushes any buffered bits
       to the dataset (if writing), and then calls Hendaccess.
 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
int32 Hendbitaccess(int32 bitfile_id,intn flushbit)
{
    CONSTR(FUNC,"Hendbitaccess");   /* for HERROR */
    bitrec_t *bitfile_rec;      /* bitfile record */

    /* check validity of access id */
    bitfile_rec = BITID2REC(bitfile_id);
    if(bitfile_rec==NULL || !bitfile_rec->used)
        HRETURN_ERROR(DFE_ARGS,FAIL);

    if(bitfile_rec->mode=='w')
        if(HIbitflush(bitfile_rec,flushbit,TRUE)==FAIL)
            HERROR(DFE_WRITEERROR);
    HDfreespace((VOIDP)bitfile_rec->bytea);    /* free the space for the buffer */
    bitfile_rec->used = FALSE;

    return(Hendaccess(bitfile_rec->acc_id));
}   /* end Hendbitaccess() */

/*--------------------------------------------------------------------------

 NAME
    HIbitflush -- flush the bits out to a writable bitfile
 USAGE
    intn HIbitflush(bitfile_rec,flushbit)
        bitrec_t *bitfile_rec;  IN: record of bitfile element to flush
        intn flushbit;          IN: determines how to flush leftover bits
                                   (leftover bits are bits that have been
                                    buffered, but are less than the
                                    BITNUM (usually set to 8) number of
                                    bits)
                                    0 - flush with zeros
                                    1 - flush with ones
                                   -1 - throw away any leftover bits
 RETURNS
    returns SUCCEED (0) if successful, FAIL (-1) otherwise
 DESCRIPTION
    Used to flush the buffer of a bitfile element, preserving the bits
    in the buffer which have not been modified.  The flushbits parameter
    is only used when the last bits written to the element are at the
    actual end of the dataset, not somewhere in the middle.
 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
    This routine does _not_ leave the bitfile in a position to continue
    I/O from the current point, additional modifications would have to be
    made in order to accomodate this.
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
PRIVATE intn HIbitflush(bitrec_t *bitfile_rec,intn flushbit,bool writeout)
{
    CONSTR(FUNC,"HIbitflush");
    intn write_size;            /* number of bytes to write out */

#ifdef TESTING
printf("HIbitflush(): max_offset=%d, byte_offset=%d, flushbit=%d\n",bitfile_rec->max_offset,bitfile_rec->byte_offset,flushbit);
#endif
    if(bitfile_rec->count<BITNUM) { /* check if there are any */
#ifdef TESTING
printf("HIbitflush(): extra bits to write\n");
#endif
        if(bitfile_rec->byte_offset>=bitfile_rec->max_offset) {
#ifdef TESTING
printf("HIbitflush(): longer than max_offset data\n");
#endif
            if(flushbit!=(-1))  /* only flush bits if asked and there are bits to flush */
                if(Hbitwrite(bitfile_rec->bit_id,bitfile_rec->count,(uint32)(flushbit ? 0xFF : 0))==FAIL)
                    HRETURN_ERROR(DFE_WRITEERROR,FAIL);
          } /* end if */
        else {  /* we are in the middle of a dataset and need to integrate */
#ifdef TESTING
printf("HIbitflush(): integrating change count=%d, bits=%d\n",bitfile_rec->count,bitfile_rec->bits);
#endif
            /* mask off a place for the new bits */
            *(bitfile_rec->bytep)&=~(maskc[BITNUM-bitfile_rec->count]<<bitfile_rec->count);

            /* merge in new bits */
            *(bitfile_rec->bytep)|=bitfile_rec->bits;

            bitfile_rec->bytep++;
            bitfile_rec->count=BITNUM;  /* reset count */
          } /* end else */
      } /* end if */
    if(writeout==TRUE) {     /* only write data out if necessary */
        write_size=bitfile_rec->bytez-bitfile_rec->bytea;
#ifdef TESTING
printf("HIbitflush(): write_size=%d\n",write_size);
#endif
        if(write_size>0)
            if(Hwrite(bitfile_rec->acc_id,write_size,bitfile_rec->bytea)==FAIL)
                HRETURN_ERROR(DFE_WRITEERROR,FAIL);
      } /* end if */

    return(SUCCEED);
} /* HIbitflush */

/*--------------------------------------------------------------------------
 HIget_bitfile_slot

 get a free bitfile record slot
--------------------------------------------------------------------------*/
PRIVATE int HIget_bitfile_slot(void)
{
    int i;                     /* temp index */
    CONSTR(FUNC,"HIget_bitfile_slot");

    /* Access records not allocated yet, allocate dynamically and initialize */
    if (!bitfile_records) {
        bitfile_records=(bitrec_t *)HDgetspace(MAX_BITFILE * sizeof(bitrec_t));
        if (!bitfile_records)
            return FAIL;
        for(i=0; i<MAX_BITFILE; i++)
            bitfile_records[i].used=FALSE;

        if((bitfile_records[0].bytea=(uint8 *)HDgetspace(BITBUF_SIZE))==NULL)
            HRETURN_ERROR(DFE_NOSPACE,FAIL);
        bitfile_records[0].used=TRUE;  /* use the first record */
        return(0);
      } /* end if */

    /* return the first unused record */
    for(i=0; i<MAX_BITFILE; i++)
        if(!bitfile_records[i].used) {
            if((bitfile_records[i].bytea=(uint8 *)HDgetspace(BITBUF_SIZE))==NULL)
                HRETURN_ERROR(DFE_NOSPACE,FAIL);
            bitfile_records[i].used=TRUE;
            return(i);
          } /* end if */

    return FAIL;
} /* HIget_bitfile_slot */


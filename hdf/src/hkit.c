#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.6  1993/01/26 19:42:48  koziol
Added support for reading and writing Little-Endian data on all
platforms.  This has been tested on: Cray, Sun, and PCs so far.

 * Revision 1.5  1993/01/19  05:56:03  koziol
 * Merged Hyperslab and JPEG routines with beginning of DEC ALPHA
 * port.  Lots of minor annoyances fixed.
 *
 * Revision 1.4  1992/11/05  18:59:26  chouck
 * Added (unix) wrapper to realloc()
 *
 * Revision 1.3  1992/11/02  16:35:41  koziol
 * Updates from 3.2r2 -> 3.3
 *
 * Revision 1.2  1992/08/26  19:44:25  chouck
 * Moved HDgettagname() into hkit.c and added calibration tag
 *
 * Revision 1.1  1992/08/25  21:40:44  koziol
 * Initial revision
 *
*/
#include <ctype.h>
#include "hdf.h"
#include "herr.h"
#include "hfile.h"

/*-----------------------------------------------------------------------------
 * Name:    HIstrncpy
 * Purpose: This function creates a string in dest that is at most
 *          'len' characters long.  The 'len' characters *include* the
 *          NULL terminatior which must be added for historical reasons.
 *          So if you have the string "Foo\0" you must call this copy
 *          function with len == 4
 * Inputs:  dest, source: destination and source for copy
 *          len: max length of the outgoing string
 * Returns: Address of dest.
 * Users:   HDF systems programmers
 * Remarks:
 *---------------------------------------------------------------------------*/

#if defined PROTOTYPE
char _HUGE *HIstrncpy(register char *dest,register char *source,int32 len)
#else
char _HUGE *HIstrncpy(dest, source, len)
register char *source, *dest;
int32 len;
#endif /* PROTOTYPE */
{
    char *destp;

    destp = dest;
    if (len == 0) return(destp);
    for(; (len > 1) && (*source != '\0');len--)
        *dest++ = *source++;
    *dest = '\0';       /* Force the last byte be '\0'   */
    return(destp);
}

#ifdef PC
#ifdef WIN3
int32 HDspaceleft(void)
{
/* return the largest amount of memory Windows can give us */
   return(GlobalCompact(0));
}
#else /* WIN3 */
int32 HDspaceleft(void)
{
    struct _heapinfo h_info;        /* structure for heap information, defined in <malloc.h> */
    int heap_status;                /* the current condition of the heap */
    int32 total_free,total_used;    /* variables to store the amount used and the amount free in the heap */

    total_free=0;
    total_used=0;
    h_info._pentry=NULL;
    for(;;) {
        heap_status=_heapwalk(&h_info);
        if(heap_status!=_HEAPOK)
            break;
        if(h_info._useflag==_USEDENTRY)
            total_used+=h_info._size;
        else
            total_free+=h_info._size;
    } /* end for */
    switch(heap_status) {
        case _HEAPEND:
        case _HEAPEMPTY:
            break;

        case _HEAPBADBEGIN:
            printf("%s block at %Fp of size %4.4X\n",(h_info._useflag==_USEDENTRY ? "USED" : "FREE"),h_info._pentry,h_info._size);
            printf("ERROR - heap is damaged\n\n");
            return((int32)-1);

        case _HEAPBADPTR:
            printf("%s block at %Fp of size %4.4X\n",(h_info._useflag==_USEDENTRY ? "USED" : "FREE"),h_info._pentry,h_info._size);
            printf("ERROR - bad pointer to heap\n\n");
            return((int32)-1);

        case _HEAPBADNODE:
            printf("%s block at %Fp of size %4.4X\n",(h_info._useflag==_USEDENTRY ? "USED" : "FREE"),h_info._pentry,h_info._size);
            printf("ERROR - bad node in heap\n\n");
            return((int32)-1);
    } /* end switch */
    return((int32)total_free);
} /* end HDspaceleft() */
#endif /* WIN3 */
#endif /* PC */


#ifdef PC
#ifdef WIN3
void _HUGE *HDgetspace(uint32 qty)
{
    char *FUNC="HDgetspace";

    HGLOBAL hTmp;
    HGLOBAL far *wfpTmp;
    
    hTmp=GlobalAlloc(GMEM_MOVEABLE|GMEM_DISCARDABLE|GMEM_ZEROINIT,qty+sizeof(HGLOBAL));
    if (!hTmp) {
        HERROR(DFE_NOSPACE);
        return(NULL);
      } /* end if */
    wfpTmp=(HGLOBAL far *) GlobalLock(hTmp);
    if (!wfpTmp) {
        GlobalFree(hTmp);
        HERROR(DFE_NOSPACE);
        return(NULL);
    }
    *wfpTmp=hTmp;
    wfpTmp++;
    return((void _HUGE *)wfpTmp);
}

void _HUGE *HDregetspace(VOIDP vfp, uint32 new_size)
{
    char *FUNC="HDregetspace";
    HGLOBAL new_handle;         /* handle of the new memory block */
    HGLOBAL hTmp;
    WORD far *wfpTmp;
    
    if (!vfp)
        return(NULL);
    hTmp = (HGLOBAL)(*(--((WORD far *) vfp)));
    if (!hTmp)
        return(NULL);
    if (GlobalUnlock(hTmp)) {
        HERROR(DFE_NOSPACE);
        return(NULL);
      } /* end if */
    if((new_handle=GlobalReAlloc(hTmp,new_size,GMEM_MOVEABLE|GMEM_DISCARDABLE|GMEM_ZEROINIT))!=NULL) {
        wfpTmp=(WORD far *) GlobalLock(new_handle);
        if (!wfpTmp) {
            GlobalFree(new_handle);
            HERROR(DFE_NOSPACE);
            return(NULL);
        }
        *wfpTmp=(WORD)hTmp;
        wfpTmp++;
        return(wfpTmp);
    } else
        return(NULL);
}

void _HUGE *HDfreespace(void *vfp)
{
    HGLOBAL hTmp;
    
    if (!vfp)
        return(NULL);
    hTmp=(HGLOBAL)(*(--((WORD far *) vfp)));
    if (!hTmp)
        return(NULL);
    GlobalUnlock(hTmp);
    GlobalFree(hTmp);

    return(NULL);
}
#else /* !WIN3 */
void _HUGE *HDgetspace(uint32 qty)
{
    char *FUNC="HDgetspace";
    char huge *p;

    qty+=sizeof(char)+sizeof(uint32);   /* increment the quantity to allocate */

    if(qty>=(int32)64000) {   /* see if we have to use halloc() to get a really large chunk of memory */
        p = halloc((int32)qty,(size_t)1);
        if (p==NULL) {
            HERROR(DFE_NOSPACE);
            return(NULL);
          } /* end if */
        *p++=1;     /* indicate that halloc() was used to acquire this memory */
      } /* end if */
    else {      /* we can get away with just malloc() */
        p = malloc((size_t)qty);
        if (p==NULL) {
            HERROR(DFE_NOSPACE);
            return(NULL);
          } /* end if */
        *p++=0;     /* indicate that malloc() was used to acquire this memory */
      } /* end else */
    *(uint32 *)p=qty;   /* save the size of the block */
    p+=sizeof(uint32);
    return(p);
}

#ifndef MIN
#define MIN(a,b)    (((a)<(b)) ? (a) : (b))
#endif

void _HUGE *HDregetspace(VOIDP ptr, uint32 qty)
{
    char *FUNC="HDregetspace";
    uint32 old_size;
    char *p=ptr;
    char *p2;

    qty+=sizeof(char)+sizeof(uint32);   /* increment the quantity to allocate */

    p-=sizeof(uint32);    /* decrement the pointer to free */
    old_size=*(uint32 *)p;
    p-=sizeof(char);
    if(qty>=(int32)64000) {   /* see if we have to use halloc() to get a really large chunk of memory */
        p2=halloc((int32)qty,(size_t)1);
        if(p2==NULL) {
            HERROR(DFE_NOSPACE);
            return(NULL);
          } /* end if */
        *p2++=1;     /* indicate that halloc() was used to acquire this memory */
        *(uint32 *)p2=qty;   /* save the size of the block */
      } /* end if */
    else {      /* we can get away with just malloc() */
        p2=malloc((size_t)qty);
        if(p2==NULL) {
            HERROR(DFE_NOSPACE);
            return(NULL);
          } /* end if */
        *p2++=0;     /* indicate that malloc() was used to acquire this memory */
      } /* end else */
    *(uint32 *)p2=qty;   /* save the size of the block */
    p2+=sizeof(uint32);
    HDmemcpy(p2,p+(sizeof(char)+sizeof(uint32)),
            MIN(UINT_MAX,(uint16)MIN(old_size,qty))-(sizeof(char)+sizeof(uint32)));
    if(*p)    /* check whether block of memory was allocated with halloc() */
        hfree(p);
    else       /* memory was allocated through malloc() */
        free(p);
    return(p2);
}

void _HUGE *HDfreespace(void *ptr)
{
    char *p=ptr;

    if(ptr==NULL)
        return(NULL);

    p-=(sizeof(char)+sizeof(uint32));    /* decrement the pointer to free */
    if(*p)   /* check whether block of memory was allocated with halloc() */
        hfree(p);
    else       /* memory was allocated through malloc() */
        free(p);
    return(NULL);
}

#endif /* WIN3 */
#else /* !PC */


#if defined PROTOTYPE
void *HDgetspace(uint32 qty)
#else
void *HDgetspace(qty)
uint32 qty;
#endif /* PROTOTYPE */
{
    char *FUNC="HDgetspace";
    char *p;

    p = (char *) malloc(qty);
    if (p== (char *) NULL) {
        HERROR(DFE_NOSPACE);
        HEreport("Attempted to allocate %d bytes", qty);
        return(NULL);
    }
    return(p);
}

#if defined PROTOTYPE
void *HDregetspace(VOIDP where, uint32 qty)
#else
void *HDregetspace(where, qty)
VOIDP where;
uint32 qty;
#endif /* PROTOTYPE */
{
    char *FUNC="HDregetspace";
    char *p;

    p = (char *) realloc(where, qty);
    if (p== (char *) NULL) {
        HERROR(DFE_NOSPACE);
        HEreport("Attempted to re-allocate %d bytes", qty);
        return(NULL);
    }
    return(p);
}

#if defined PROTOTYPE
void *HDfreespace(void *ptr)
#else
void *HDfreespace(ptr)
void *ptr;
#endif /* PROTOTYPE */
{
    if (ptr!=NULL) free(ptr);
    return(NULL);
}

#endif /* PC */

#if defined PROTOTYPE 
intn HDc2fstr(char *str, intn len)
#else
intn HDc2fstr(str, len)
char* str;
intn len;
#endif /* PROTOTYPE */
{
    int i;

    for(i=0; (str[i]); i++)
        /* EMPTY */;
    for(; i<len; i++) str[i] = ' ';
    return 0;
}

#if defined WIN3 || defined PC
#ifdef WIN3
/*--------------------------------------------------------------------------
**
** NAME
**  fmemcpy_big -- function specific to the PC to copy 32-bits of data
** USAGE
**  VOIDP fmemcpy_big (dst,src,len)
**  VOIDP dst;          IN: the buffer to put bytes into
**  VOIDP src;          IN: the source buffer to copy from
**  uint32 len;         IN: the number of bytes to copy
** RETURNS
**  returns the original pointer to dst
** DESCRIPTION
**  Because the IBM PC compilers use 16-bit number for memcpy, this function
**  blocks that up into 64kb blocks to copy at a time.
** GLOBAL VARIABLES
**  None
** COMMENTS, BUGS, ASSUMPTIONS
** EXAMPLES
** REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
VOIDP fmemcpy_big(VOIDP dst,VOIDP src,uint32 len)
#else
VOIDP fmemcpy_big(dst,src,len)
VOIDP dst;
VOIDP src;
uint32 len;
#endif
{
    uint8 *s,d;             /* alias for the buffers */

    if(len<=UINT_MAX)   /* if the size is small enough copy all at once */
        return(_fmemcpy(dst,src,(size_t)len));
    else {  /* number of bytes to read */
        s=(uint8 *)src;
        d=(uint8 *)dst;
        while(len>UINT_MAX) {
            _fmemcpy(d,s,UINT_MAX);
            s+=UINT_MAX;
            d+=UINT_MAX;
            len-=UINT_MAX;
          } /* end while */
        if(len>0)
            _fmemcpy(d,s,(size_t)len);
      } /* end else */
    return(dst);
}   /* end fmemcpy_big() */

/*--------------------------------------------------------------------------
**
** NAME
**  fmemset_big -- function specific to the PC to set 32-bits of data
** USAGE
**  VOIDP fmemset_big (src, c, len)
**  VOIDP src;          IN: the buffer to set to a value
**  intn c;             IN: the value to use to set
**  uint32 len;         IN: the number of bytes to set
** RETURNS
**  returns the original pointer to s
** DESCRIPTION
**  Because the IBM PC compilers use 16-bit number for memcpy, this function
**  blocks that up into 64kb blocks to set at a time.
** GLOBAL VARIABLES
**  None
** COMMENTS, BUGS, ASSUMPTIONS
** EXAMPLES
** REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
VOIDP fmemset_big(VOIDP src,intn c,uint32 len)
#else
VOIDP fmemset_big(src,c,len)
VOIDP src;
intn c;
uint32 len;
#endif
{
    uint8 *s;               /* alias for the buffers */

    if(len<=UINT_MAX)   /* if the size is small enough copy all at once */
        return(_fmemset(src,c,(size_t)len));
    else {  /* number of bytes to read */
        s=(uint8 *)src;
        while(len>UINT_MAX) {
            _fmemset(s,c,UINT_MAX);
            s+=UINT_MAX;
            len-=UINT_MAX;
          } /* end while */
        if(len>0)
            _fmemset(s,c,(size_t)len);
      } /* end else */
    return(src);
}   /* end fmemset_big() */

/*--------------------------------------------------------------------------
**
** NAME
**  fmemcmp_big -- function specific to the PC to compare 32-bits of data
** USAGE
**  VOIDP fmemcmp_big (s1,s2,len)
**  VOIDP s1;           IN: the first buffer
**  VOIDP s2;           IN: the second buffer
**  uint32 len;         IN: the number of bytes to copy
** RETURNS
**  returns a value less than, equal to, or greater than 0 indicating
**      that the object pointed to by s1 is less than, equal to, or greater
**      than the object pointed to by s2
** DESCRIPTION
**  Because the IBM PC compilers use 16-bit number for memcpy, this function
**  blocks that up into 64kb blocks to compare at a time.
** GLOBAL VARIABLES
**  None
** COMMENTS, BUGS, ASSUMPTIONS
** EXAMPLES
** REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
intn fmemcmp_big(VOIDP s1,VOIDP s2,uint32 len)
#else
intn fmemcmp_big(s1,s2,len)
VOIDP s1;
VOIDP s2;
uint32 len;
#endif
{
    intn ret_val;

    if(len<=UINT_MAX)   /* if the size is small enough copy all at once */
        return(_fmemcmp(s1,s2,(size_t)len));
    else {  /* number of bytes to read */
        while(len>UINT_MAX) {
            ret_val=_fmemcmp(s1,s2,UINT_MAX);
            if(ret_val!=0)
                return(ret_val);
            s1+=UINT_MAX;
            s2+=UINT_MAX;
            len-=UINT_MAX;
          } /* end while */
        if(len>0)
            return(_fmemcmp(s1,s2,(size_t)len));
      } /* end else */
    return(0);
}   /* end fmemcmp_big() */
#else   /* !WIN3 */
/*--------------------------------------------------------------------------
**
** NAME
**  memcpy_big -- function specific to the PC to copy 32-bits of data
** USAGE
**  VOIDP memcpy_big (dst,src,len)
**  VOIDP dst;          IN: the buffer to put bytes into
**  VOIDP src;          IN: the source buffer to copy from
**  uint32 len;         IN: the number of bytes to copy
** RETURNS
**  returns the original pointer to dst
** DESCRIPTION
**  Because the IBM PC compilers use 16-bit number for memcpy, this function
**  blocks that up into 64kb blocks to copy at a time.
** GLOBAL VARIABLES
**  None
** COMMENTS, BUGS, ASSUMPTIONS
** EXAMPLES
** REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
VOIDP memcpy_big(VOIDP dst,VOIDP src,uint32 len)
#else
VOIDP memcpy_big(dst,src,len)
VOIDP dst;
VOIDP src;
uint32 len;
#endif
{
    uint8 *s,*d;             /* alias for the buffers */

    if(len<=UINT_MAX)   /* if the size is small enough copy all at once */
        return(memcpy(dst,src,(size_t)len));
    else {  /* number of bytes to read */
        s=(uint8 *)src;
        d=(uint8 *)dst;
        while(len>UINT_MAX) {
            memcpy(d,s,UINT_MAX);
            s+=UINT_MAX;
            d+=UINT_MAX;
            len-=UINT_MAX;
          } /* end while */
        if(len>0)
            memcpy(d,s,(size_t)len);
      } /* end else */
    return(dst);
}   /* end memcpy_big() */

/*--------------------------------------------------------------------------
**
** NAME
**  memset_big -- function specific to the PC to set 32-bits of data
** USAGE
**  VOIDP memset_big (src, c, len)
**  VOIDP src;          IN: the buffer to set to a value
**  intn c;             IN: the value to use to set
**  uint32 len;         IN: the number of bytes to set
** RETURNS
**  returns the original pointer to s
** DESCRIPTION
**  Because the IBM PC compilers use 16-bit number for memcpy, this function
**  blocks that up into 64kb blocks to set at a time.
** GLOBAL VARIABLES
**  None
** COMMENTS, BUGS, ASSUMPTIONS
** EXAMPLES
** REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
VOIDP memset_big(VOIDP src,intn c,uint32 len)
#else
VOIDP memset_big(src,c,len)
VOIDP src;
intn c;
uint32 len;
#endif
{
    uint8 *s;               /* alias for the buffers */

    if(len<=UINT_MAX)   /* if the size is small enough copy all at once */
        return(memset(src,c,(size_t)len));
    else {  /* number of bytes to read */
        s=(uint8 *)src;
        while(len>UINT_MAX) {
            memset(s,c,UINT_MAX);
            s+=UINT_MAX;
            len-=UINT_MAX;
          } /* end while */
        if(len>0)
            memset(s,c,(size_t)len);
      } /* end else */
    return(src);
}   /* end memset_big() */

/*--------------------------------------------------------------------------
**
** NAME
**  memcmp_big -- function specific to the PC to compare 32-bits of data
** USAGE
**  VOIDP memcmp_big (s1,s2,len)
**  VOIDP s1;           IN: the first buffer
**  VOIDP s2;           IN: the second buffer
**  uint32 len;         IN: the number of bytes to copy
** RETURNS
**  returns a value less than, equal to, or greater than 0 indicating
**      that the object pointed to by s1 is less than, equal to, or greater
**      than the object pointed to by s2
** DESCRIPTION
**  Because the IBM PC compilers use 16-bit number for memcpy, this function
**  blocks that up into 64kb blocks to compare at a time.
** GLOBAL VARIABLES
**  None
** COMMENTS, BUGS, ASSUMPTIONS
** EXAMPLES
** REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
intn memcmp_big(VOIDP s1,VOIDP s2,uint32 len)
#else
intn memcmp_big(s1,s2,len)
VOIDP s1;
VOIDP s2;
uint32 len;
#endif
{
    intn ret_val;

    if(len<=UINT_MAX)   /* if the size is small enough copy all at once */
        return(memcmp(s1,s2,(size_t)len));
    else {  /* number of bytes to read */
        while(len>UINT_MAX) {
            ret_val=memcmp(s1,s2,UINT_MAX);
            if(ret_val!=0)
                return(ret_val);
            (uint8 *)s1+=UINT_MAX;
            (uint8 *)s2+=UINT_MAX;
            len-=UINT_MAX;
          } /* end while */
        if(len>0)
            return(memcmp(s1,s2,(size_t)len));
      } /* end else */
    return(0);
}   /* end memcmp_big() */
#endif  /* WIN3 */

#endif  /* WIN3 | PC */

#if defined PROTOTYPE
char _HUGE *HDf2cstring(_fcd fdesc, intn len)
#else
char _HUGE *HDf2cstring(fdesc, len)
    _fcd fdesc;
    intn len;
#endif /* PROTOTYPE */
{
    char *cstr, *str;
    int i;

    str = _fcdtocp(fdesc);
    for(i=len-1;i>=0 && (!isascii(str[i]) || !isgraph(str[i])); i--)
        /*EMPTY*/;
    cstr = (char *)HDgetspace(i+2);
    cstr[i+1] = '\0';
    for (; i>=0; i--) cstr[i] = str[i];
    return cstr;
}

/*--------------------------------------------------------------------------
 HIlookup_dd

 find the dd with tag and ref, by returning the block where the dd resides
 and the index of the dd in the ddblock ddlist.

 This function is different from HIfind_dd in that it does not understand
 any ordering in the file.  Wildcards sent to this routine (i.e. to
 get the 'next' widget get passed off to HIfind_dd().

 Return FAIL or SUCCEED
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int HIlookup_dd(filerec_t *file_rec, uint16 look_tag, uint16 look_ref, 
                ddblock_t **pblock, int32 *pidx)
#else
int HIlookup_dd(file_rec, look_tag, look_ref, pblock, pidx)
     filerec_t *file_rec;       /* the file */
     uint16 look_tag;           /* tag of dd to look for */
     uint16 look_ref;           /* ref of dd to look for */
     ddblock_t **pblock;        /* OUT: ddblock where dd is found */
     int32 *pidx;               /* OUT: index into ddlist where dd is found */
#endif
{ 
  char *FUNC="HIlookup_dd";       /* for HERROR */
  register intn tag, ref, key;
  register tag_ref_list_ptr p;

  if(look_tag == DFTAG_WILDCARD || look_ref == DFREF_WILDCARD)
    return (HIfind_dd(look_tag, look_ref, pblock, pidx));

  tag = (intn) look_tag;
  ref = (intn) look_ref;

  /*
   * Look for the normal version 
   */
  key = tag + ref;

  for(p = file_rec->hash[key & HASH_MASK]; p; p = p->next) {
    if(p->tag == tag && p->ref == ref) {
      *pblock = p->pblock;
      *pidx   = p->pidx;
      return SUCCEED;
    }
  }

  /*
   * Try looking for the special version of this tag
   */
  tag = (intn) MKSPECIALTAG(look_tag);
  key = tag + ref;

  for(p = file_rec->hash[key & HASH_MASK]; p; p = p->next) {
    if(p->tag == tag && p->ref == ref) {
      *pblock = p->pblock;
      *pidx   = p->pidx;
      return SUCCEED;
    }
  }

  return FAIL;

} /* HIlookup_dd */

/* ---------------------------- HIadd_hash_dd ----------------------------- */
/*
  Add a new dd into the hash table

  Return SUCCEED or FAIL
*/
#ifdef PROTOTYPE
int HIadd_hash_dd(filerec_t *file_rec, uint16 look_tag, uint16 look_ref, 
                ddblock_t *pblock, int32 pidx)
#else
int HIadd_hash_dd(file_rec, look_tag, look_ref, pblock, pidx)
     filerec_t *file_rec;      /* the file */
     uint16 look_tag;           /* tag of dd to add */
     uint16 look_ref;           /* ref of dd to add */
     ddblock_t *pblock;         /* ddblock where dd is  */
     int32 pidx;                /* index into ddlist where dd is */
#endif
{
  char *FUNC="HIadd_hash_dd";       /* for HERROR */
  register intn tag, ref, key;
  register tag_ref_list_ptr p;

  if(look_tag == DFTAG_NULL) return SUCCEED;

  tag = (intn) look_tag;
  ref = (intn) look_ref;
  key = tag + ref;

  if((p = (tag_ref_list_ptr) HDgetspace((uint32)sizeof(tag_ref_list)))==NULL)
    HRETURN_ERROR(DFE_NOSPACE, FAIL);
  
  p->pblock = pblock;
  p->pidx   = pidx;
  p->tag    = tag;
  p->ref    = ref;
  p->next   = file_rec->hash[key & HASH_MASK];        
  file_rec->hash[key & HASH_MASK] = p;
  
  return SUCCEED;

} /* HIadd_hash_dd */


/* ---------------------------- HIdel_hash_dd ----------------------------- */
/*
  Delete a dd from the hash table

  Return SUCCEED or FAIL
*/
#ifdef PROTOTYPE
int HIdel_hash_dd(filerec_t *file_rec, uint16 look_tag, uint16 look_ref)
#else
int HIdel_hash_dd(file_rec, look_tag, look_ref)
     filerec_t *file_rec;      /* the file */
     uint16 look_tag;           /* tag of dd to add */
     uint16 look_ref;           /* ref of dd to add */
#endif
{
  char *FUNC="HIdel_hash_dd";       /* for HERROR */
  register intn tag, ref, key;
  register tag_ref_list_ptr p, prev;

  tag = (intn) look_tag;
  ref = (intn) look_ref;
  key = tag + ref;
  
  p = file_rec->hash[key & HASH_MASK]; 

  if(!p) return SUCCEED;

  prev = NULL;
  for(p = file_rec->hash[key & HASH_MASK]; p; p = p->next) {
    if(p->tag == tag && p->ref == ref) {
      if(prev)
        prev->next = p->next;
      else
        file_rec->hash[key & HASH_MASK] = p->next;

      HDfreespace(p);
      return SUCCEED;
    }
    prev = p;
  }

  return SUCCEED;

} /* HIdel_hash_dd */


/*--------------------------------------------------------------------------
 HIfind_dd

 find the dd with tag and ref, by returning the block where the dd resides
 and the index of the dd in the ddblock ddlist.
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int HIfind_dd(uint16 look_tag, uint16 look_ref, ddblock_t **pblock, int32 *pidx)
#else
int HIfind_dd(look_tag, look_ref, pblock, pidx)
    uint16 look_tag;           /* tag of dd to look for */
    uint16 look_ref;           /* ref of dd to look for */
    ddblock_t **pblock;        /* IN: ddblock to start looking for the dd */
                               /* OUT: ddblock where dd is found */
    int32 *pidx;               /* IN: index before place in ddlist
                                  to start searching */
                               /* OUT: index into ddlist where dd is found */
#endif
{
    register int32 idx;             /* index into ddlist of current dd searched */
    register ddblock_t *block;      /* ptr to current ddblock searched */
    register dd_t *list;            /* ptr to current ddlist searched */

#ifndef oldspecial
    uint16 special_tag;                /* corresponding special tag */

    /* search for special version also */

    special_tag = MKSPECIALTAG(look_tag);
#endif

    /* start searching on the next dd */
    idx = *pidx + 1;
    for (block = *pblock; block; block = block->next) {

       list = block->ddlist;
       for (; idx < block->ndds; idx++) {

           /* skip the empty dd's */

           if (list[idx].tag == DFTAG_NULL && look_tag != DFTAG_NULL)
               continue;

           if(((look_tag == DFTAG_WILDCARD || list[idx].tag == look_tag)
#ifndef oldspecial
                || (special_tag != DFTAG_NULL && list[idx].tag == special_tag)
#endif
              ) && (look_ref == DFREF_WILDCARD || list[idx].ref == look_ref)) {

               /* we have a match !! */

               *pblock = block;
               *pidx = idx;
               return SUCCEED;
           }
       }

       /* start from beginning of the next dd list */

       idx = 0;
    }

    /* nothing found */

    return FAIL;
} /* HIfind_dd */

/* ------------------------------- HDflush -------------------------------- */
/*

  Force the system to flush the HDF file stream

  This should be primarily used for debugging

  The MAC does not really support fflush() so this routine just returns
	SUCCEED always on a MAC w/o really doing anything.

*/

#ifdef PROTOTYPE
intn HDflush(int32 file_id)
#else
intn HDflush(file_id)
    int32 file_id;             /* id of file to flush */
#endif
{
    char *FUNC="HDflush";       /* for HERROR */

#ifndef MAC

    filerec_t *file_rec;
    
    file_rec = FID2REC(file_id);
    if (!file_rec || file_rec->refcount == 0) {
        HERROR(DFE_ARGS);
        return FAIL;
    }
    
    fflush(file_rec->file);

#endif /* MAC */

    return SUCCEED;

} /* HDflush */


/* ---------------------------- HDpackFstring ----------------------------- */
/*

  HDpackFstring -- given a NULL terminated C string 'src' convert it to
  a space padded Fortran string 'dest' of length 'len'

*/
intn
#ifdef PROTOTYPE
HDpackFstring(char *src, char *dest, intn len)
#else
HDpackFstring(src, dest, len)
char *src, *dest;
intn len;
#endif
{

    intn sofar;
    
    for(sofar = 0; (sofar < len) && (*src != '\0'); sofar++)
        *dest++ = *src++;

    while(sofar++ < len)
        *dest++ = ' ';

    return SUCCEED;

}


/*--------------------------------------------------------------------------
 *  HDgettagname(tag) : map a tag to its corresponding name
 *                      return NULL if tag is unknown.
 *
 *  NOTE: Please keep tag names <= 30 characters - a 
 *        lot of pretty-printing code depends on it.
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
char _HUGE *HDgettagname(uint16 tag)
#else
char _HUGE *HDgettagname(tag)
     uint16 tag;
#endif /* PROTOTYPE */
{

  char *name;

  switch(tag) {
      
      /* Utility Tags */
  case DFTAG_NULL  :
      name = "No Data"; break;
  case DFTAG_VERSION :
      name = "Version Descriptor"; break;
  case DFTAG_LINKED :
      name = "Linked Blocks Indicator"; break;
  case DFTAG_FID   : 
      name = "File Identifier"; break;
  case DFTAG_FD    :   
      name = "File Description"; break;
  case DFTAG_TID   :
      name = "Tag Identifier"; break;
  case DFTAG_TD    : 
      name = "Tag Description"; break;
  case DFTAG_DIL   :
      name = "Data Id Label"; break;
  case DFTAG_DIA   :    
      name = "Data Id Annotation"; break;
  case DFTAG_NT    :    
      name = "Number type"; break;
  case DFTAG_MT    :   
      name = "Machine type"; break;
      
      /* raster-8 Tags */
  case DFTAG_ID8   :   
      name = "Image Dimensions-8"; break;
  case DFTAG_IP8   :   
      name = "Image Palette-8"; break;
  case DFTAG_RI8   :  
      name = "Raster Image-8"; break;
  case DFTAG_CI8   : 
      name = "RLE Compressed Image-8"; break;
  case DFTAG_II8   :  
      name = "Imcomp Image-8"; break;
      
      /* Raster Image Tags */
  case DFTAG_ID    :  
      name = "Image Dimensions"; break;
  case DFTAG_LUT   :  
      name = "Image Palette"; break;
  case DFTAG_RI    : 
      name = "Raster Image Data"; break;
  case DFTAG_CI    :  
      name = "Compressed Image"; break;
  case DFTAG_RIG   : 
      name = "Raster Image Group"; break;
  case DFTAG_LD    : 
      name = "Palette Dimension"; break;
  case DFTAG_MD    :
      name = "Matte Dimension"; break;
  case DFTAG_MA    :
      name = "Matte Data"; break;
  case DFTAG_CCN   :   
      name = "Color Correction"; break;
  case DFTAG_CFM   : 
      name = "Color Format"; break;
  case DFTAG_AR    :   
      name = "Aspect Ratio"; break;
  case DFTAG_DRAW  :
      name = "Sequenced images"; break;
  case DFTAG_RUN   :   
      name = "Runable program / script"; break;
  case DFTAG_XYP   : 
      name = "X-Y position"; break;
  case DFTAG_MTO   :  
      name = "M/c-Type override"; break;
      
      /* Tektronix */
  case DFTAG_T14   :   
      name = "TEK 4014 Data"; break;
  case DFTAG_T105  :
      name = "TEK 4105 data"; break;
      
      /* Compression Schemes */
  case DFTAG_RLE   : 
      name = "Run Length Encoding"; break;
  case DFTAG_IMCOMP : 
      name = "IMCOMP Encoding"; break;
  case DFTAG_JPEG :
      name = "24-bit JPEG Encoding"; break;
  case DFTAG_GREYJPEG :
      name = "8-bit JPEG Encoding"; break;
      
      /* Scientific / Numeric Data Sets */
  case DFTAG_SDG   : 
      name = "Scientific Data Group"; break;
  case DFTAG_NDG   : 
      name = "Numeric Data Group"; break;
  case DFTAG_SD    :
      name = "Scientific Data"; break;
  case DFTAG_SDD   : 
    name = "SciData description"; break;
  case DFTAG_SDL   :   
      name = "SciData labels"; break;
  case DFTAG_SDU   : 
      name = "SciData units"; break;
  case DFTAG_SDF   :  
      name = "SciData formats"; break;
  case DFTAG_SDS   :  
      name = "SciData scales"; break;
  case DFTAG_SDM   :  
      name = "SciData max/min"; break;
  case DFTAG_SDC   :  
      name = "SciData coordsys"; break;
  case DFTAG_SDT   :  
      name = "Transpose"; break;
  case DFTAG_SDLNK :  
      name = "Links related to the dataset"; break;
  case DFTAG_CAL   :  
      name = "Calibration information"; break;
      
      /* V Group Tags */
  case DFTAG_VG   :  
      name = "Vgroup"; break;
  case DFTAG_VH   : 
      name = "Vdata"; break;
  case DFTAG_VS   : 
      name = "Vdata Storage"; break;
  default:
      name = (char *) NULL;
      break;
  }
  
  return name;
  
}

#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.2  1992/08/26 19:44:25  chouck
Moved HDgettagname() into hkit.c and added calibration tag

 * Revision 1.1  1992/08/25  21:40:44  koziol
 * Initial revision
 *
*/
#include <ctype.h>
#include "hdf.h"
#include "herr.h"
#include "hfile.h"

/*-----------------------------------------------------------------------------
 * Name:    HDstrncpy
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
char *HDstrncpy(register char *dest,register char *source,int32 len)
#else
char *HDstrncpy(dest, source, len)
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

/*-----------------------------------------------------------------------------
 * Name:    HDmemcopy
 * Purpose: Copy bytes from one place to another
 * Inputs:  from, to: source and destination for copy
 *          length: number of bytes to copy
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF systems programmers, on machines without memcpy equivalents
 * Remarks: assumes non-overlapping
 *          Intended for machines on which memcppy etc. do not work
 *---------------------------------------------------------------------------*/

#if defined PROTOTYPE
intn HDmemcopy(uint8 *from, uint8 *to, int32 length)
#else
intn HDmemcopy( from, to, length)
uint8 *from, *to;
int32 length;
#endif /* PROTOTYPE */
{
    length++;
    while (--length) *to++ = *from++;
    return(0);
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
void *HDgetspace(uint32 qty)
{
    char *FUNC="HDgetspace";

    HANDLE mem_handle;  /* handle of the memory allocated */
    HANDLE *handle_addr;/* pointer to the HANDLE stored in the memory block */
    char huge *p1;
    char *p2;

    if (qty <= (uint32)128) {
        mem_handle=LocalAlloc(LMEM_MOVEABLE, (WORD)qty+sizeof(HANDLE)+1);
        if (mem_handle==NULL) {
            HERROR(DFE_NOSPACE);
            return(NULL);
        }
        handle_addr=(HANDLE *)(p2=(char *)LocalLock(mem_handle));
        if (handle_addr==NULL) {
            HERROR(DFE_NOSPACE);
            return(NULL);
        }
        *handle_addr=mem_handle;
        *((char *)handle_addr+sizeof(HANDLE))=0;
        return (p2+sizeof(HANDLE)+1);
    } else {
        mem_handle=GlobalAlloc(GMEM_MOVEABLE,(DWORD)(qty+sizeof(HANDLE)+1));
        if(mem_handle==NULL) {
            HERROR(DFE_NOSPACE);
            return(NULL);
        }
        /* lock down the block, use GlobalWire because we don't know how long */
        /*  this potentially large block will be used */
        handle_addr=(HANDLE *)(p1=(char huge *)GlobalWire(mem_handle));
        if (handle_addr==NULL) {
            HERROR(DFE_NOSPACE);
            return(NULL);
        }
        *handle_addr=mem_handle;    /* store the handle in the first part of the memory block */
        *((char *)handle_addr+sizeof(HANDLE))=1;
        return(p1+sizeof(HANDLE)+1);
    }
}

void *HDfreespace(void *ptr)
{
    HANDLE mem_handle;  /* handle of the memory allocated */
    HANDLE *handle_addr;/* pointer to the HANDLE stored in the memory block */

    if(ptr!=NULL) {
/* get the address of the handle, which is stored in the memory block itself */
        handle_addr=(HANDLE *)((char *)ptr-sizeof(HANDLE)-1);
        mem_handle=*handle_addr;    /* get the handle from the memory block */
        if (*((char *)ptr-1)) {
            GlobalUnWire(mem_handle);     /* unlock the block */
            GlobalFree(mem_handle);       /* free the block */
        } else {
            LocalUnlock(mem_handle);
            LocalFree(mem_handle);
        }
    } /* end if */
    return(NULL);
}
#else /* !WIN3 */
void *HDgetspace(uint32 qty)
{
    char *FUNC="HDgetspace";
    char huge *p;
    char *p2;

    qty++;    /* increment the quantity to allocate to allow for the extra byte we are going to include */

    if(qty>=(int32)64000) {   /* see if we have to use halloc() to get a really large chunk of memory */
        p = halloc((int32)qty,(size_t)1);
        if (p==NULL) {
            HERROR(DFE_NOSPACE);
            return(NULL);
          } /* end if */
        *p=1;           /* indicate that halloc() was used to acquire this memory */
        p++;
        return((char huge *)p);
      } /* end if */
    else {      /* we can get away with just malloc() */
        p2 = malloc((size_t)qty);
        if (p2==NULL) {
            HERROR(DFE_NOSPACE);
            return(NULL);
          } /* end if */
        *p2=0;           /* indicate that malloc() was used to acquire this memory */
        p2++;
        return((char huge *)p2);
      } /* end else */
}

void *HDfreespace(void *ptr)
{
    if(ptr==NULL)
        return(NULL);

    ptr = (char *)ptr-1;      /* decrement the pointer to free */
    if(*(char *)ptr)   /* check whether block of memory was allocated with halloc() */
        hfree(ptr);
    else       /* memory was allocated through malloc() */
        free(ptr);
    return(NULL);
}
#endif /* WIN3 */
#else /* PC */


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
intn HDc2fstr(char *str, int len)
#else
intn HDc2fstr(str, len)
char* str;
int len;
#endif /* PROTOTYPE */
{
    int i;

    for(i=0; (str[i]); i++)
        /* EMPTY */;
    for(; i<len; i++) str[i] = ' ';
    return 0;
}

#if defined PROTOTYPE
char *HDf2cstring(_fcd fdesc, intn len)
#else
char *HDf2cstring(fdesc, len)
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

  if(!(p = (tag_ref_list_ptr) HDgetspace((uint32)sizeof(tag_ref_list))))
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
char *HDgettagname(uint16 tag)
#else
char *HDgettagname(tag)
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

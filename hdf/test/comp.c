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

/*
 FILE
       comp.c
       Test HDF compressed data I/O routines

 REMARKS
        Currently these only really test RLE compression, at not very well
        at that...

 DESIGN
        These should be written to test all combinations of modeling and
        encoding layers.

 BUGS/LIMITATIONS

 EXPORTED ROUTINES

 AUTHOR
       Quincey Koziol

 MODIFICATION HISTORY
   10/19/93 - Threw this header in.
*/

/* $Id$ */

#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif

#include "tproto.h"

#define TESTFILE_NAME "tcomp.hdf"

#define BUFSIZE 4096

#ifdef QAK
/* different modeling layers to test */
comp_model_t test_models[]={COMP_MODEL_STDIO};

/* different compression layers to test */
comp_coder_t test_coders[]={COMP_CODE_NONE,COMP_CODE_RLE,COMP_CODE_NBIT);

#define NUM_OUTBUFS 1
static uint8 FAR *outbuf[NUM_OUTBUFS];
#endif

static uint8 FAR outbuf[BUFSIZE],
    FAR inbuf[BUFSIZE];

static uint8 FAR outbuf2[BUFSIZE],
    FAR inbuf2[BUFSIZE];

void test_comp()
{
    int32 fid;
    int32 aid1;
    int32 fileid, length, offset, posn;
    uint16 tag, ref, ref1, ref2, ref3;
    int16 acc_mode, special;
    register int i,j;
    int32 ret;
    intn errors = 0;
    model_info m_info;
    comp_info c_info;

    for (i=0; i<BUFSIZE; i++)
        outbuf[i] = (char) (i % 256);
    for(i=0; i<BUFSIZE; i++)
        for(j=0; j<130 && i<BUFSIZE; j++,i++)
            outbuf2[i]=(uint8)i;

    MESSAGE(5,printf("Creating a file %s\n", TESTFILE_NAME););
    fid = Hopen(TESTFILE_NAME, DFACC_CREATE, 0);
    CHECK(fid, FAIL, "Hopen");

#ifdef QAK
    MESSAGE(5,printf("Write an element and then promote to Linked Blocks\n"););
    ret = Hputelement(fid, (uint16) 1000, (uint16) 1,
                     (uint8 *) "element 1000 1 wrong ",
                      HDstrlen("element 1000 1 wrong ") + 1);
    CHECK(ret, FAIL, "Hputelement");

    aid1 = HLcreate(fid, 1000, 1, 10,10);
    CHECK(aid1, FAIL, "HLcreate");

    ret = Hseek(aid1, HDstrlen("element 1000 1 "), DF_START);
    CHECK(ret, FAIL, "Hseek");

    ret = Hwrite(aid1, HDstrlen("correct")+1, (uint8 *) "correct");
    if(ret != (int32)HDstrlen("correct") + 1) {
      fprintf(stderr, "ERROR: (%d) Hwrite returned the wrong length: %d\n", __LINE__,ret);
      errors++;
    }

    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");
#endif

    ref1 = Hnewref(fid);
    CHECK(ref1, 0, "Hnewref");

    MESSAGE(5,printf("Create a new element as a compressed element\n"););
    aid1 = HCcreate(fid, 1000, ref1, COMP_MODEL_STDIO, &m_info,
            COMP_CODE_RLE, &c_info);
    CHECK(aid1, FAIL, "HCcreate");

    ret = Hwrite(aid1, BUFSIZE/2, outbuf);
    if(ret != BUFSIZE/2) {
      fprintf(stderr, "ERROR(%d): Hwrite returned the wrong length: %d\n", __LINE__,(int)ret);
      HEprint(stdout,0);
      errors++;
    }

    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    ref2 = Hnewref(fid);
    CHECK(ref2, 0, "Hnewref");

    aid1 = HCcreate(fid, 1000, ref2, COMP_MODEL_STDIO, &m_info,
            COMP_CODE_RLE, &c_info);
    CHECK(aid1, FAIL, "HCcreate");

    ret = Hwrite(aid1, BUFSIZE/2, outbuf2);
    if(ret != BUFSIZE/2) {
      fprintf(stderr, "ERROR: (%d) Hwrite returned the wrong length: %d\n", __LINE__,(int)ret);
      HEprint(stdout,0);
      errors++;
    }

    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    MESSAGE(5,printf("Verifying data\n"););
    ret = Hgetelement(fid, (uint16) 1000, (uint16) ref1, inbuf);
    if(ret != BUFSIZE/2) {
      HEprint(stderr,0);
      fprintf(stderr, "ERROR: (%d) Hgetelement returned the wrong length: %d\n",__LINE__, (int)ret);
      errors++;
    }

    for (i=0; i < ret; i++) {
       if (inbuf[i] != outbuf[i]) {
           printf("Wrong data at %d, out %d in %d\n", i, outbuf[i], inbuf[i]);
           errors++;
         }
       inbuf[i] = '\0';
    }

    ret = Hgetelement(fid, (uint16) 1000, (uint16) ref2, inbuf2);
    if(ret != BUFSIZE/2) {
      HEprint(stderr,0);
      fprintf(stderr, "ERROR: (%d) Hgetelement returned the wrong length: %d\n", __LINE__,(int)ret);
      errors++;
    }

    for (i=0; i < ret; i++) {
       if (inbuf2[i] != outbuf2[i]) {
           printf("Wrong data at %d, out %d in %d\n", i, outbuf2[i], inbuf2[i]);
           errors++;
         }
       inbuf2[i] = '\0';
    }

    ref3 = Hnewref(fid);
    CHECK(ref3, 0, "Hnewref");

    aid1 = HCcreate(fid, 1020, ref3, COMP_MODEL_STDIO, &m_info,
            COMP_CODE_RLE, &c_info);
    CHECK(aid1, FAIL, "HCcreate");

    ret = Hwrite(aid1, BUFSIZE, outbuf);
    if(ret != BUFSIZE) {
      fprintf(stderr, "ERROR: (%d) Hwrite returned the wrong length: %d\n", __LINE__,(int)ret);
      errors++;
    }

    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    MESSAGE(5,printf("Closing and re-opening file %s\n", TESTFILE_NAME););
    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");

    fid = Hopen(TESTFILE_NAME, DFACC_RDWR, 0);
    CHECK(fid, FAIL, "Hopen");

    MESSAGE(5,printf("Verifying data\n"););

#ifdef QAK
    aid1 = Hstartread(fid, 1000, 1);
    CHECK(aid1, FAIL, "Hstartread");

    ret = Hinquire(aid1, &fileid, &tag, &ref, &length, &offset, &posn,
                  &acc_mode, &special);
    CHECK(ret, FAIL, "Hinquire");
    if(!special) {
      fprintf(stderr, "ERROR: Hinquire does not think element is special line %d\n",
              __LINE__);
      errors++;
    }

    ret = Hread(aid1, length, inbuf);
    if(ret != 23) {
      fprintf(stderr, "ERROR: (%d) Hread returned the wrong length: %d\n", _LINE__,ret);
      errors++;
    }

    if(HDstrcmp((const char *) inbuf, (const char *) "element 1000 1 correct")) {
      fprintf(stderr, "ERROR: (%d) Hread returned the wrong data\n",__LINE__);
      fprintf(stderr, "\t       Is: %s\n", inbuf);
      fprintf(stderr, "\tShould be: element 1000 1 correct\n");
      errors++;
    }

    ret = Hnextread(aid1, 1000, DFREF_WILDCARD, DF_CURRENT);
    CHECK(ret, FAIL, "Hnextread");
#else

    ret = Hstartread(fid, 1000, ref1);
    CHECK(ret, FAIL, "Hstartread");
#endif

    ret = Hinquire(aid1, &fileid, &tag, &ref, &length, &offset, &posn,
                  &acc_mode, &special);
    CHECK(ret, FAIL, "Hinquire");
    if(!special) {
      fprintf(stderr, "ERROR: Hinquire does not think element is special line %d\n",
              __LINE__);
      errors++;
    }

#ifdef QAK
    MESSAGE(5,printf("Writing to existing element\n"););
    aid2 = Hstartwrite(fid, 1000, 1, ref1);
    CHECK(aid2, FAIL, "Hstartwrite");

    ret = Hwrite(aid2, 4, (uint8 *) "ABCD");
    if(ret != 4) {
      fprintf(stderr, "ERROR: (%d) Hwrite returned the wrong length: %d\n", __LINE__,ret);
      errors++;
    }
#endif

    /* let's try to write to a read element for fun */
    ret = Hwrite(aid1, 4, (uint8 *) "ABCD");
    if(ret != FAIL) {
      fprintf(stderr, "ERROR: (%d) Hwrite allowed write to read access obj\n",__LINE__);
      errors++;
    }

    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    MESSAGE(5,printf("Closing the files\n"););
    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");
}

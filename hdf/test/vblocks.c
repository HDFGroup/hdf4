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

#include "tproto.h"
#define TESTFILE_NAME "tvblocks.hdf"

#define BUFSIZE 4096

#define HVCREATE_TAG 1000
#define HVCREATE_REF1 1
#define HVCREATE_REF2 2

#define HVCONVERT_TAG 1500

static uint8 FAR outbuf[BUFSIZE], FAR inbuf[BUFSIZE];

/* Tests:
    create new vblock
    change old data into vblock
    convert an AID into a vblock
    open a vblock and write "regular" data and vblock data
    open two vblocks and write to both
*/

void
test_hvblocks()
{
    int32       fid;
    int32       aid1;
    int i;
    int32       ret;
    intn        errors = 0;

    for (i = 0; i < BUFSIZE; i++)
        outbuf[i] = (char) (i % 256);

    MESSAGE(5, printf("Creating a file %s\n", TESTFILE_NAME);
        );
    fid = Hopen(TESTFILE_NAME, DFACC_CREATE, 0);
    CHECK(fid, FAIL, "Hopen");

/* Test 1 - Just create an empty v-len linked block */
    MESSAGE(5, printf("Create an element as a Variable-Len Linked Block\n"););
    aid1 = HVcreate(fid, (uint16) HVCREATE_TAG, (uint16) HVCREATE_REF1);
    CHECK(aid1, FAIL, "HVcreate");

    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

/* Test 2 - Create an empty v-len linked block and write data to it */
    MESSAGE(5, printf("Write to a Variable-Len Linked Block element\n"););
    aid1 = HVcreate(fid, (uint16) HVCREATE_TAG, (uint16) HVCREATE_REF2);
    CHECK(aid1, FAIL, "HVcreate");

    ret = Hwrite(aid1, BUFSIZE, outbuf);
    if (ret != (int32) BUFSIZE)
      {
          fprintf(stderr, "ERROR(%d): Hwrite returned the wrong length: %d\n", __LINE__,(int) ret);
          errors++;
      }

    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

/* Test 3 - Read data back from v-len linked block and verify */
    MESSAGE(5, printf("Read from a Variable-Len Linked Block element\n"););
    aid1 = Hstartread(fid, (uint16) HVCREATE_TAG, (uint16) HVCREATE_REF2);
    CHECK(aid1, FAIL, "HVcreate");

    ret = Hread(aid1, BUFSIZE, inbuf);
    if (ret != (int32) BUFSIZE)
      {
          fprintf(stderr, "ERROR(%d): Hwrite returned the wrong length: %d\n", __LINE__,(int) ret);
          errors++;
      }

    for (i = 0; i < ret; i++)
      {
          if (inbuf[i] != outbuf[i])
            {
                printf("Wrong data at %d, out %d in %d\n", (int)i, (int)outbuf[i], (int)inbuf[i]);
                errors++;
            }
          inbuf[i] = '\0';
      }

    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");

#ifdef QAK
    MESSAGE(5, printf("Create a new element as a Linked Block\n");
        );
    aid1 = HLcreate(fid, 1000, 4, 512, 2);
    CHECK(aid1, FAIL, "HLcreate");

    ret = Hwrite(aid1, BUFSIZE / 2, outbuf);
    if (ret != BUFSIZE / 2)
      {
          fprintf(stderr, #__LINE__"ERROR: Hwrite returned the wrong length: %d\n", (int) ret);
          errors++;
      }

    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    ret = Hnewref(fid);
    CHECK(ret, FAIL, "Hnewref");

    aid1 = HLcreate(fid, 1000, 2, 128, 16);
    CHECK(aid1, FAIL, "HLcreate");

    ret = Hwrite(aid1, HDstrlen("element 1000 2") + 1, (uint8 *) "element 1000 2");
    if (ret != (int32) HDstrlen("element 1000 2") + 1)
      {
          fprintf(stderr, #__LINE__"ERROR: Hwrite returned the wrong length: %d\n", (int) ret);
          errors++;
      }

    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    MESSAGE(5, printf("Verifying data\n");
        );
    ret = Hgetelement(fid, (uint16) 1000, (uint16) 4, inbuf);
    if (ret != BUFSIZE / 2)
      {
          fprintf(stderr, #__LINE__"ERROR: Hgetelement returned the wrong length: %d\n", (int) ret);
          errors++;
      }

    for (i = 0; i < ret; i++)
      {
          if (inbuf[i] != outbuf[i])
            {
                printf("Wrong data at %d, out %d in %d\n", i, outbuf[i], inbuf[i]);
                errors++;
            }
          inbuf[i] = '\0';
      }

    aid1 = HLcreate(fid, 1020, 2, 128, 4);
    CHECK(aid1, FAIL, "HLcreate");

    ret = Hwrite(aid1, BUFSIZE, outbuf);
    if (ret != BUFSIZE)
      {
          fprintf(stderr, #__LINE__"ERROR: Hwrite returned the wrong length: %d\n", (int) ret);
          errors++;
      }

    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    MESSAGE(5, printf("Closing and re-opening file %s\n", TESTFILE_NAME);
        );
    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");

    fid = Hopen(TESTFILE_NAME, DFACC_RDWR, 0);
    CHECK(fid, FAIL, "Hopen");

    ret = Hnewref(fid);
    CHECK(ret, FAIL, "Hnewref");

    MESSAGE(5, printf("Verifying data\n");
        );

    aid1 = Hstartread(fid, 1000, 1);
    CHECK(aid1, FAIL, "Hstartread");

    ret = Hinquire(aid1, &fileid, &tag, &ref, &length, &offset, &posn,
                   &acc_mode, &special);
    CHECK(ret, FAIL, "Hinquire");
    if (!special)
      {
          fprintf(stderr, "ERROR: Hinquire does not think element is special line %d\n",
                  __LINE__);
          errors++;
      }

    ret = Hread(aid1, length, inbuf);
    if (ret != 23)
      {
          fprintf(stderr, #__LINE__"ERROR: Hread returned the wrong length: %d\n", (int) ret);
          errors++;
      }

    if (HDstrcmp((const char *) inbuf, (const char *) "element 1000 1 correct"))
      {
          fprintf(stderr, #__LINE__"ERROR: Hread returned the wrong data\n");
          fprintf(stderr, "\t       Is: %s\n", (char *) inbuf);
          fprintf(stderr, "\tShould be: element 1000 1 correct\n");
          errors++;
      }

    ret = Hnewref(fid);
    CHECK(ret, FAIL, "Hnewref");

    ret = Hnextread(aid1, 1000, DFREF_WILDCARD, DF_CURRENT);
    CHECK(ret, FAIL, "Hnextread");

    ret = Hinquire(aid1, &fileid, &tag, &ref, &length, &offset, &posn,
                   &acc_mode, &special);
    CHECK(ret, FAIL, "Hinquire");
    if (!special)
      {
          fprintf(stderr, "ERROR: Hinquire does not think element is special line %d\n",
                  __LINE__);
          errors++;
      }

    ret = Hnextread(aid1, DFTAG_WILDCARD, DFREF_WILDCARD, DF_START);
    CHECK(ret, FAIL, "Hnextread");

    ret = Hinquire(aid1, &fileid, &tag, &ref, &length, &offset, &posn,
                   &acc_mode, &special);
    CHECK(ret, FAIL, "Hinquire");
    if (special)
      {
          fprintf(stderr, #__LINE__"ERROR: Hinquire thinks a non-special element is special\n");
          errors++;
      }

    ret = Hnextread(aid1, DFTAG_WILDCARD, 1151, DF_CURRENT);
    if (ret != FAIL)
      {
          fprintf(stderr, #__LINE__"ERROR: Hnextread found object with bogus ref\n");
          errors++;
      }

    ret = Hnextread(aid1, 1020, DFREF_WILDCARD, DF_CURRENT);
    CHECK(ret, FAIL, "Hnextread");

    ret = Hinquire(aid1, &fileid, &tag, &ref, &length, &offset, &posn,
                   &acc_mode, &special);
    CHECK(ret, FAIL, "Hinquire");
    if (!special)
      {
          fprintf(stderr, "ERROR: Hinquire does not think element is special line %d\n",
                  __LINE__);
          errors++;
      }

    MESSAGE(5, printf("Writing to existing element\n");
        );
    aid2 = Hstartwrite(fid, 1000, 1, 4);
    CHECK(aid2, FAIL, "Hstartwrite");

    ret = Hwrite(aid2, 4, (uint8 *) "ABCD");
    if (ret != 4)
      {
          fprintf(stderr, #__LINE__"ERROR: Hwrite returned the wrong length: %d\n", (int) ret);
          errors++;
      }

    /* let's try to write to a read element for fun */
    ret = Hwrite(aid1, 4, (uint8 *) "ABCD");
    if (ret != FAIL)
      {
          fprintf(stderr, #__LINE__"ERROR: Hwrite allowed write to read access obj\n");
          errors++;
      }

    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    ret = Hendaccess(aid2);
    CHECK(ret, FAIL, "Hendaccess");

    MESSAGE(5, printf("Open a second id for file %s\n", TESTFILE_NAME);
        );
    fid1 = Hopen(TESTFILE_NAME, DFACC_READ, 0);
    CHECK(fid1, FAIL, "Hopen");

    ret = Hnewref(fid1);
    CHECK(ret, FAIL, "Hnewref");

    MESSAGE(5, printf("Closing the files\n");
        );
    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");

    ret = Hclose(fid1);
    CHECK(ret, FAIL, "Hclose");

    MESSAGE(5, printf("Testing HLconvert function\n");
        );
    fid = Hopen(TESTFILE_NAME, DFACC_WRITE, 0);
    CHECK(fid, FAIL, "Hopen");

    ref = Hnewref(fid);
    CHECK(ret, FAIL, "Hnewref");

    aid = Hstartwrite(fid, HLCONVERT_TAG, ref, 5);
    CHECK(aid, FAIL, "Hstartwrite");

    ret = Hwrite(aid, 4, outbuf);
    CHECK(ret, FAIL, "Hwrite");

    ret = HLconvert(aid, 256, 10);
    CHECK(ret, FAIL, "HLconvert");

    ret = Hwrite(aid, 508, &outbuf[4]);
    CHECK(ret, FAIL, "Hwrite");

    ret = Hendaccess(aid);
    CHECK(ret, FAIL, "Hendaccess");

    aid = Hstartread(fid, HLCONVERT_TAG, ref);
    CHECK(aid, FAIL, "Hstartread");

    ret = Hread(aid, 512, inbuf);
    CHECK(ret, FAIL, "Hread");

    ret = Hendaccess(aid);
    CHECK(ret, FAIL, "Hendaccess");

    ret = Hclose(fid1);
    CHECK(ret, FAIL, "Hclose");

    if (HDmemcmp(inbuf, outbuf, 512))
      {
          fprintf(stderr, #__LINE__"Error when reading data from HLconvert buffer\n");
          errors++;
      }

    num_errs += errors;     /* increment global error count */
#endif /* QAK */
}

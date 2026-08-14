/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF/releases/.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
   test opening files and access elements until limits are reached
 */

#include "testhdf.h"
#include "hfile_priv.h"

/* This value can cause platform-specific problems as the number of files
 * that can be open at one time via fopen() is often capped by the OS.
 *
 * The current value doesn't seem to cause problems on any platform.
 */
#define BIG 127

#define TESTFILE_NAME  "thf"
#define TESTREF_NAME   "tref.hdf"
#define MAX_REF_TESTED MAX_REF
static int32 files[BIG];
static int32 accs[BIG];

static void test_open_limits(void);
static void test_ref_limits(void);

static void
test_open_limits(void)
{
    int   i;
    int32 ret;

    /* Initialize the tracking arrays */
    for (i = 0; i < BIG; i++)
        files[i] = accs[i] = FAIL;

#ifdef H4_HAVE_WIN32_API
    /* Windows can only have 512 stdio files open by default, so we need
     * to bump this to handle large values for BIG files open at once.
     *
     * Technically no longer necessary since BIG was adjusted to < 512, but
     * worth keeping around in case we need to set this in the future.
     */
    ret = _setmaxstdio(1024);
    CHECK(ret, FAIL, "_setmaxstdio");
#endif

    MESSAGE(5, puts("Opening many files of same name"););
    for (i = 0; i < BIG; i++) {
        files[i] = Hopen("thf.hdf", DFACC_RDWR, 0);
        if (files[i] < 0) {
            break;
        }
    }
    MESSAGE(5, printf("Opening stopped at %d/%d files\n", i, BIG););

    MESSAGE(5, puts("Closing all files"););
    for (i--; i >= 0; i--) {
        ret = Hclose(files[i]);
        CHECK(ret, FAIL, "Hclose");
        files[i] = FAIL;
        if (ret < 0)
            printf("Error closing file %d\n", i);
    }
    MESSAGE(5, puts("Closed files"););

    MESSAGE(5, puts("Opening many files of different names"););
    for (i = 0; i < BIG; i++) {
        char fname[100];
        sprintf(fname, "%s%1d.hdf", TESTFILE_NAME, i);
        files[i] = Hopen(fname, DFACC_ALL, 0);
        if (files[i] < 0) {
            break;
        }
    }
    MESSAGE(5, printf("Opening stopped at %d/%d files\n", i, BIG););

    MESSAGE(5, puts("Closing all files except first open"););
    for (i--; i > 0; i--) {
        ret = Hclose(files[i]);
        CHECK(ret, FAIL, "Hclose");
        files[i] = FAIL;
        if (ret < 0)
            printf("Error closing file %d\n", i);
    }
    MESSAGE(5, puts("Closed files"););

    MESSAGE(5, puts("Opening write access elements"););
    for (i = 0; i < BIG; i++) {
        accs[i] = Hstartwrite(files[0], (uint16)100, (uint16)(i + 1), 100L);
        if (accs[i] < 0)
            break;
    }
    MESSAGE(5, printf("Opening stopped at %d element\n", i););

    MESSAGE(5, puts("Closing access elements"););
    for (i--; i >= 0; i--) {
        ret = Hendaccess(accs[i]);
        CHECK(ret, FAIL, "Hendaccess");
        accs[i] = FAIL;
        if (ret < 0)
            printf("Error ending access %d\n", i);
    }
    MESSAGE(5, puts("Ended access"););

    ret = Hclose(files[0]);
    CHECK(ret, FAIL, "Hclose");
    files[0] = FAIL;

done:
    /* Release resources */
    for (i = 0; i < BIG; i++) {
        if (accs[i] != FAIL)
            Hendaccess(accs[i]);
        if (files[i] != FAIL)
            Hclose(files[i]);
    }
    return;
}

#define TAG1 ((uint16)1000)
#define TAG2 ((uint16)1001)

static void
test_ref_limits(void)
{
    int32 i;          /* local counting variable */
    int32 fid = FAIL; /* file ID */
    int32 iloop;
    int32 aid1 = FAIL, aid2 = FAIL;
    int32 aid = FAIL;

    MESSAGE(6, printf("Testing reference # limits\n"););
    MESSAGE(7, printf("Writing out data\n"););
    /* Write out MAX_REF number of data items for each tag */
    fid = Hopen(TESTREF_NAME, DFACC_CREATE, 512);
    CHECK(fid, FAIL, "Hopen");

    if (fid != FAIL) {
        iloop = MAX_REF_TESTED;
        for (i = 1; i <= (iloop / 2) + 5; i++) {
            uint16 ref;
            int32  data;
            int32  ret;

            /* Write out data to tag1 */
            ref = Htagnewref(fid, TAG1);
            CHECK(ref, 0, "Htagnewref");
            aid = Hstartwrite(fid, TAG1, ref, sizeof(int32));
            CHECK(aid, FAIL, "Hstartwrite");
            data = (int32)ref;
            ret  = Hwrite(aid, sizeof(int32), &data);
            CHECK(ret, FAIL, "Hwrite");
            ret = Hendaccess(aid);
            CHECK(ret, FAIL, "Hendaccess");
            aid = FAIL;

            /* lets be a little smatter here */
            if (ret == FAIL)
                break;

            /* Write out data to tag2 */
            ref = Htagnewref(fid, TAG2);
            CHECK(ref, 0, "Htagnewref");
            aid = Hstartwrite(fid, TAG2, ref, sizeof(int32));
            CHECK(aid, FAIL, "Hstartwrite");
            data = ref << 16;
            ret  = Hwrite(aid, sizeof(int32), &data);
            CHECK(ret, FAIL, "Hwrite");
            ret = Hendaccess(aid);
            CHECK(ret, FAIL, "Hendaccess");
            aid = FAIL;
            /* lets be a little smatter here */
            if (ret == FAIL)
                break;

        } /* end for */
        Hclose(fid);
        fid = FAIL;

        MESSAGE(7, printf("Verifying data\n"););

        /* Check the data written earlier */
        fid = Hopen(TESTREF_NAME, DFACC_READ, 0);
        CHECK(fid, FAIL, "Hopen");

        if (fid != FAIL) {
            uint16 ref;
            int32  data;
            int32  ret;

            /* Read in data from tag1 */
            aid1 = Hstartread(fid, TAG1, DFREF_WILDCARD);
            CHECK(aid1, FAIL, "Hstartread");
            ret = Hread(aid1, sizeof(int32), &data);
            CHECK(ret, FAIL, "Hread");
            ret = Hinquire(aid1, NULL, NULL, &ref, NULL, NULL, NULL, NULL, NULL);
            CHECK(ret, FAIL, "Hinquire");
            VERIFY((uint16)data, ref, "Hread");

            /* Read in data from tag2 */
            aid2 = Hstartread(fid, TAG2, DFREF_WILDCARD);
            CHECK(aid2, FAIL, "Hstartread");
            ret = Hread(aid2, sizeof(int32), &data);
            CHECK(ret, FAIL, "Hread");
            ret = Hinquire(aid2, NULL, NULL, &ref, NULL, NULL, NULL, NULL, NULL);
            CHECK(ret, FAIL, "Hinquire");
            VERIFY((uint32)data, (((uint32)ref) << 16), "Hread");

            while (Hnextread(aid1, TAG1, DFTAG_WILDCARD, DF_CURRENT) != FAIL) {
                ret = Hread(aid1, sizeof(int32), &data);
                CHECK(ret, FAIL, "Hread");
                ret = Hinquire(aid1, NULL, NULL, &ref, NULL, NULL, NULL, NULL, NULL);
                CHECK(ret, FAIL, "Hinquire");
                VERIFY((uint16)data, ref, "Hread");

                if (Hnextread(aid2, TAG2, DFTAG_WILDCARD, DF_CURRENT) != FAIL) {
                    ret = Hread(aid2, sizeof(int32), &data);
                    CHECK(ret, FAIL, "Hread");
                    ret = Hinquire(aid2, NULL, NULL, &ref, NULL, NULL, NULL, NULL, NULL);
                    CHECK(ret, FAIL, "Hinquire");
                    VERIFY((uint32)data, (((uint32)ref) << 16), "Hread");
                } /* end while */
            }     /* end while */
            ret = Hendaccess(aid1);
            CHECK(ret, FAIL, "Hendaccess");
            aid1 = FAIL;

            ret = Hendaccess(aid2);
            CHECK(ret, FAIL, "Hendaccess");
            aid2 = FAIL;

            Hclose(fid);
            fid = FAIL;
        } /* end if */
    }     /* end if */

done:
    /* Release resources */
    if (aid != FAIL)
        Hendaccess(aid);
    if (aid1 != FAIL)
        Hendaccess(aid1);
    if (aid2 != FAIL)
        Hendaccess(aid2);
    if (fid != FAIL)
        Hclose(fid);
    return;
}

void
test_hfile_limits(void)
{
    test_open_limits();
    test_ref_limits();
}

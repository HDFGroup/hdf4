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
    FILE - buffer.c
        Test HDF buffered data I/O routines

    DESIGN
        - Create a new data element and get a benchmark time for reading it in
            various ways.
        - Buffer the element and get times for the buffered element.
        - Make a new external element and get benchmark times
        - Buffer the external element and get times for the buffered element.
        - Make a new compressed element and get benchmark times
        - Buffer the compressed element and get times for the buffered element.
        - Make a new linked block element and get benchmark times
        - Buffer the linked block element and get times for the buffered element.
 */

#define TESTMASTER

#include "hdf_priv.h"
#include "tutils.h"
#include "hfile_priv.h"

#define TESTFILE_NAME "tbuffer.hdf"
#define EXTFILE_NAME  "tbuffer.dat"

/* Size of data elements to create */
#define ELEMSIZE 16384

/* define aliases for random number generation */
#define RAND    rand
#define SEED(a) srand((unsigned)(a))

/* Tag to use for creating the test elements */
#define BUFF_TAG 1000

/* Number of tests */
#define NUM_TESTS 4

/* Factor for converting seconds to microseconds */
#define FACTOR 1000000

/* Number of timing tests to run */
/* 0 - read/write entire buffer in one I/O operation */
/* 1 - read/write entire buffer one byte at a time forwards */
/* 2 - read/write entire buffer one byte at a time every other byte forwards */
/* 3 - read/write entire buffer one byte at a time backwards */
/* 4 - read/write entire buffer one byte at a time every other byte backwards */
#define NUM_TIMINGS 5
long read_time[NUM_TESTS][2];  /* 0 is unbuffered, 1 is buffered */
long write_time[NUM_TESTS][2]; /* 0 is unbuffered, 1 is buffered */

int32 elemsize; /* Actual number of elements in buffer */

/* I/O buffers */
uint8 *out_buf; /* Buffer for writing data */
uint8 *in_buf;  /* Buffer for reading data */

/* local function prototypes */
static void  init_buffer(void);
static void  usage(void);
static char *fixname(const char *base_name, char *fullname, size_t size);
static long  read_test(int32 aid);
static long  write_test(int32 aid, int num_timings);

/* Initialize output buffer */
static void
init_buffer(void)
{
    SEED(time(NULL));
    for (int j = 0; j < elemsize; j++) {
        out_buf[j] = (uint8)RAND();
    }
} /* init_buffers() */

static void
usage(void)
{
    printf("\nUsage: buffer [elemsize] \n\n");
    printf("where elemsize is the number of elements in buffer (default: 1000 in Cray, 16384 in other "
           "platforms)\n");
    printf("\n");
} /* end usage() */

/*
   Creates a file name from a file base name like 'test' and return it through
   the FULLNAME (at most SIZE characters counting the null terminator). The
   full name is created by prepending the contents of HDF4_TESTPREFIX
   (separated from the base name by a slash). Returns NULL if BASENAME or
   FULLNAME is the null pointer or if FULLNAME isn't large enough for the
   result.
*/
static char *
fixname(const char *base_name, char *fullname, size_t size)
{
    const char *prefix = NULL;
    char       *ptr, last = '\0';
    size_t      i, j;

    if (!base_name || !fullname || size < 1)
        return NULL;

    memset(fullname, 0, size);

    /* First use the environment variable, then try the constant */
    prefix = getenv("HDF4_TESTPREFIX");

#ifdef HDF4_TESTPREFIX
    if (!prefix)
        prefix = HDF4_TESTPREFIX;
#endif

    /* Prepend the prefix value to the base name */
    if (prefix && *prefix) {
        if (snprintf(fullname, size, "%s/%s", prefix, base_name) == (int)size)
            /* Buffer is too small */
            return NULL;
    }
    else {
        if (strlen(base_name) >= size)
            /* Buffer is too small */
            return NULL;
        else
            strcpy(fullname, base_name);
    }

    /* Remove any double slashes in the filename */
    for (ptr = fullname, i = j = 0; ptr && i < size; i++, ptr++) {
        if (*ptr != '/' || last != '/')
            fullname[j++] = *ptr;
        last = *ptr;
    }
    return fullname;

} /* end fixname() */

static long
read_test(int32 aid)
{
    struct timeval start_time, end_time; /* timing counts */
    long           acc_time;
    int32          ret;
    int            i;         /* local counting index */
    int            timing;    /* Which timing test we are on */
    int            err_count; /* number of incorrect array positions */

    acc_time = 0;
    for (timing = 0; timing < NUM_TIMINGS; timing++) {

        /* Seek to beginning of element */
        ret = Hseek(aid, 0, DF_START);
        CHECK(ret, FAIL, "Hseek");

        switch (timing) {
            case 0: /* Read entire buffer in one I/O operation */
                gettimeofday(&start_time, NULL);
                ret = Hread(aid, elemsize, in_buf);
                VERIFY(ret, elemsize, "Hread");
                gettimeofday(&end_time, NULL);
                break;

            case 1: /* Read entire buffer one byte at a time forwards */
                gettimeofday(&start_time, NULL);
                for (i = 0; i < elemsize; i++) {
                    /* Seek to correct location within element */
                    ret = Hseek(aid, i, DF_START);
                    CHECK(ret, FAIL, "Hseek");

                    ret = Hread(aid, 1, &in_buf[i]);
                    VERIFY(ret, 1, "Hread");
                }
                gettimeofday(&end_time, NULL);
                break;

            case 2: /* Read entire buffer one byte at a time every one byte forwards */
                gettimeofday(&start_time, NULL);
                for (i = 0; i < elemsize; i += 2) {
                    /* Seek to correct location within element */
                    ret = Hseek(aid, i, DF_START);
                    CHECK(ret, FAIL, "Hseek");

                    ret = Hread(aid, 1, &in_buf[i]);
                    VERIFY(ret, 1, "Hread");
                }
                for (i = 1; i < elemsize; i += 2) {
                    /* Seek to correct location within element */
                    ret = Hseek(aid, i, DF_START);
                    CHECK(ret, FAIL, "Hseek");

                    ret = Hread(aid, 1, &in_buf[i]);
                    VERIFY(ret, 1, "Hread");
                }
                gettimeofday(&end_time, NULL);
                break;

            case 3: /* Read entire buffer one byte at a time backwards */
                gettimeofday(&start_time, NULL);
                for (i = elemsize - 1; i >= 0; i--) {
                    /* Seek to correct location within element */
                    ret = Hseek(aid, i, DF_START);
                    CHECK(ret, FAIL, "Hseek");

                    ret = Hread(aid, 1, &in_buf[i]);
                    VERIFY(ret, 1, "Hread");
                }
                gettimeofday(&end_time, NULL);
                break;

            case 4: /* Read entire buffer one byte at a time every one byte backwards */
                gettimeofday(&start_time, NULL);
                for (i = elemsize - 1; i >= 0; i -= 2) {
                    /* Seek to correct location within element */
                    ret = Hseek(aid, i, DF_START);
                    CHECK(ret, FAIL, "Hseek");

                    ret = Hread(aid, 1, &in_buf[i]);
                    VERIFY(ret, 1, "Hread");
                }
                for (i = elemsize - 2; i >= 0; i -= 2) {
                    /* Seek to correct location within element */
                    ret = Hseek(aid, i, DF_START);
                    CHECK(ret, FAIL, "Hseek");

                    ret = Hread(aid, 1, &in_buf[i]);
                    VERIFY(ret, 1, "Hread");
                }
                gettimeofday(&end_time, NULL);
                break;
        } /* end switch */

        /* Verify buffer contents */
        for (err_count = 0, i = 0; i < elemsize; i++) {
            if (out_buf[i] != in_buf[i]) {
                printf("Position (%d) read in is (%d), should be (%d)\n", i, (int)in_buf[i], (int)out_buf[i]);
                num_errs++;
                err_count++;
                if (err_count > 10)
                    break;
            }
        }

        /* Clear input buffer */
        memset(in_buf, 0, (size_t)elemsize);

        /* Increment the total I/O time */
        acc_time += (end_time.tv_sec - start_time.tv_sec) * FACTOR + (end_time.tv_usec - start_time.tv_usec);
    } /* end for */

    return acc_time;
} /* end read_test() */

static long
write_test(int32 aid, int num_timings)
{
    struct timeval start_time, end_time; /* timing counts */
    long           acc_time;
    int32          ret;
    int            i;      /* local counting index */
    int            timing; /* Which timing test we are on */

    acc_time = 0;
    for (timing = 0; timing < num_timings; timing++) {

        /* Refresh output buffer with new values */
        init_buffer();

        /* Seek to beginning of element */
        ret = Hseek(aid, 0, DF_START);
        CHECK(ret, FAIL, "Hseek");

        switch (timing) {
            case 0: /* Write entire buffer in one I/O operation */
                gettimeofday(&start_time, NULL);
                ret = Hwrite(aid, elemsize, out_buf);
                VERIFY(ret, elemsize, "Hwrite");
                gettimeofday(&end_time, NULL);
                break;

            case 1: /* Write entire buffer one byte at a time forwards */
                gettimeofday(&start_time, NULL);
                for (i = 0; i < elemsize; i++) {
                    /* Seek to correct location within element */
                    ret = Hseek(aid, i, DF_START);
                    CHECK(ret, FAIL, "Hseek");

                    ret = Hwrite(aid, 1, &out_buf[i]);
                    VERIFY(ret, 1, "Hwrite");
                }
                gettimeofday(&end_time, NULL);
                break;

            case 2: /* Write entire buffer one byte at a time every one byte forwards */
                gettimeofday(&start_time, NULL);
                for (i = 0; i < elemsize; i += 2) {
                    /* Seek to correct location within element */
                    ret = Hseek(aid, i, DF_START);
                    CHECK(ret, FAIL, "Hseek");

                    ret = Hwrite(aid, 1, &out_buf[i]);
                    VERIFY(ret, 1, "Hwrite");
                }
                for (i = 1; i < elemsize; i += 2) {
                    /* Seek to correct location within element */
                    ret = Hseek(aid, i, DF_START);
                    CHECK(ret, FAIL, "Hseek");

                    ret = Hwrite(aid, 1, &out_buf[i]);
                    VERIFY(ret, 1, "Hwrite");
                }
                gettimeofday(&end_time, NULL);
                break;

            case 3: /* Write entire buffer one byte at a time backwards */
                gettimeofday(&start_time, NULL);
                for (i = elemsize - 1; i >= 0; i--) {
                    /* Seek to correct location within element */
                    ret = Hseek(aid, i, DF_START);
                    CHECK(ret, FAIL, "Hseek");

                    ret = Hwrite(aid, 1, &out_buf[i]);
                    VERIFY(ret, 1, "Hwrite");
                }
                gettimeofday(&end_time, NULL);
                break;

            case 4: /* Write entire buffer one byte at a time every one byte backwards */
                gettimeofday(&start_time, NULL);
                for (i = elemsize - 1; i >= 0; i -= 2) {
                    /* Seek to correct location within element */
                    ret = Hseek(aid, i, DF_START);
                    CHECK(ret, FAIL, "Hseek");

                    ret = Hwrite(aid, 1, &out_buf[i]);
                    VERIFY(ret, 1, "Hwrite");
                }
                for (i = elemsize - 2; i >= 0; i -= 2) {
                    /* Seek to correct location within element */
                    ret = Hseek(aid, i, DF_START);
                    CHECK(ret, FAIL, "Hseek");

                    ret = Hwrite(aid, 1, &out_buf[i]);
                    VERIFY(ret, 1, "Hwrite");
                }
                gettimeofday(&end_time, NULL);
                break;
        } /* end switch */

        /* Seek to beginning of element */
        ret = Hseek(aid, 0, DF_START);
        CHECK(ret, FAIL, "Hseek");

        /* Read buffer contents */
        ret = Hread(aid, elemsize, in_buf);
        VERIFY(ret, elemsize, "Hread");

        /* Verify buffer contents */
        for (i = 0; i < elemsize; i++) {
            if (out_buf[i] != in_buf[i]) {
                printf("Position (%d) read in is (%d), should be (%d)\n", i, (int)in_buf[i], (int)out_buf[i]);
                num_errs++;
                break;
            }
        }

        /* Clear input buffer */
        memset(in_buf, 0, (size_t)elemsize);

        /* Increment the total I/O time */
        acc_time += (end_time.tv_sec - start_time.tv_sec) * FACTOR + (end_time.tv_usec - start_time.tv_usec);
    } /* end for */

    return acc_time;
} /* end read_test() */

int
main(int argc, char *argv[])
{
    model_info m_info;
    comp_info  c_info;
    uint16     ref_num; /* reference number of the data written out */
    int32      fid;     /* file ID of HDF file for testing */
    int32      aid;     /* AID of element to test */
    int        test_num;
    int32      ret;
    char       hfilename[32];
    char       extfilename[32];
    int        CleanUp = 1;
    int        Cache   = 1;
    uint32     lmajor, lminor, lrelease;
    char       lstring[81];

    /* Un-buffer stdout */
    setbuf(stdout, NULL);

    if (argc > 2) {
        usage();
        exit(1);
    }
    else
        elemsize = (argc == 2) ? (int32)atol(argv[1]) : (int32)ELEMSIZE;

    if (elemsize <= 0) {
        usage();
        exit(1);
    }

    out_buf = malloc((size_t)elemsize * sizeof(uint8));
    in_buf  = malloc((size_t)elemsize * sizeof(uint8));

    Verbosity = 4; /* Default Verbosity is Low */

    Hgetlibversion(&lmajor, &lminor, &lrelease, lstring);

    printf("Built with HDF Library Version: %u.%u.%u, %s\n\n", (unsigned)lmajor, (unsigned)lminor,
           (unsigned)lrelease, lstring);

    MESSAGE(6, printf("Starting buffered element test (elemsize=%d)\n", elemsize);)

    if (Cache) /* turn on caching, unless we were instructed not to */
        Hcache(CACHE_ALL_FILES, TRUE);

    /* fill the buffer with interesting data to compress */
    init_buffer();

    fixname(TESTFILE_NAME, hfilename, sizeof hfilename);

    /* open the HDF file */
    fid = Hopen(hfilename, DFACC_ALL, 0);
    CHECK(fid, FAIL, "Hopen");

    /* Cycle through the different testing element types */
    /* Performing timings on each type of buffer and record results for output */
    /* if verbosity level is high enough */
    for (test_num = 0; test_num < NUM_TESTS; test_num++) {
        /* Get a new reference number */
        ref_num = Htagnewref(fid, BUFF_TAG);
        CHECK(ref_num, 0, "Htagnewref");

        /* Create the data element to perform the tests on */
        switch (test_num) {
            case 0: /* create plain data element */
                aid = Hstartaccess(fid, BUFF_TAG, ref_num, DFACC_RDWR);
                CHECK(aid, FAIL, "Hstartaccess");
                break;

            case 1: /* create external data element */
                fixname(EXTFILE_NAME, extfilename, sizeof extfilename);
                aid = HXcreate(fid, BUFF_TAG, ref_num, extfilename, 0, elemsize);
                CHECK(aid, FAIL, "HXcreate");
                break;

            case 2: /* create compressed data element */
                c_info.deflate.level = 9;
                aid = HCcreate(fid, BUFF_TAG, ref_num, COMP_MODEL_STDIO, &m_info, COMP_CODE_DEFLATE, &c_info);
                CHECK(aid, FAIL, "HCcreate");
                break;

            case 3: /* create linked-block data element */
                aid = HLcreate(fid, BUFF_TAG, ref_num, HDF_APPENDABLE_BLOCK_LEN, HDF_APPENDABLE_BLOCK_NUM);
                CHECK(aid, FAIL, "HLcreate");
                break;
        }

        /* Write the initial data to the data element */
        ret = Hwrite(aid, elemsize, out_buf);
        VERIFY(ret, elemsize, "Hwrite");

        /* Perform read timing tests on un-buffered data element */
        read_time[test_num][0] = read_test(aid);

        /* Perform write timing tests on un-buffered data element */
        /* Just write un-buffered compressed data in one block */
        write_time[test_num][0] = write_test(aid, (test_num == 2 ? 1 : NUM_TIMINGS));

        /* Convert element to a buffered element */
        ret = HBconvert(aid);
        CHECK(ret, FAIL, "HBconvert");

        /* Perform read timing tests on buffered data element */
        read_time[test_num][1] = read_test(aid);

        /* Perform write timing tests on un-buffered data element */
        write_time[test_num][1] = write_test(aid, NUM_TIMINGS);

        /* Close data element */
        ret = Hendaccess(aid);
        CHECK(ret, FAIL, "Hendaccess");

        MESSAGE(3, {
            printf("Unbuffered read time=%f seconds\n", ((double)read_time[test_num][0] / FACTOR));
            printf("Unbuffered write time=%f seconds\n", ((double)write_time[test_num][0] / FACTOR));
            printf("Buffered read time=%f seconds\n", ((double)read_time[test_num][1] / FACTOR));
            printf("Buffered write time=%f seconds\n", ((double)write_time[test_num][1] / FACTOR));
        })

    } /* end for */

    /* close the HDF file */
    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");

    /* Clean up files created */
    if (CleanUp) {
        remove(extfilename);
        remove(hfilename);
    }

    free(out_buf);
    free(in_buf);

    MESSAGE(6, printf("Finished buffered element test\n");)
    return num_errs;
} /* end main() */

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

/*
   test opening files and access elements until limits are reached
 */

#include "tproto.h"
#include "hfile.h"
#define BIG 600
#define TESTFILE_NAME "thf"
#define TESTREF_NAME "tref.hdf"
/* Change the define below for Maximum 'refs' tested on 
   the Macintosh if the 'hfile1' tests starts to croak */
#if defined macintosh || defined MAC || defined __MWERKS__ || defined SYMANTEC_C
#define     MAX_REF_TESTED 5000
#else
#define     MAX_REF_TESTED MAX_REF
#endif
static int32 FAR files[BIG];
static int32 FAR accs[BIG];

void
test_file_limits()
{
    int         i;
    int32       ret;
#ifdef QAK
    int32      *files, *accs;

    files = HDmalloc(BIG * sizeof(int32));
    accs = HDmalloc(BIG * sizeof(int32));
    if (!files || !accs)
      {
          fprintf(stderr, "Out of memory!\n");
          exit(1);
      }
#endif

    MESSAGE(5, puts("Opening many files of same name");
        );
    for (i = 0; i < BIG; i++)
      {
          files[i] = Hopen("thf.hdf", DFACC_RDWR, 0);
          if (files[i] < 0)
            {
/*            i++; */
              break;
            } /* end if */
      }
    MESSAGE(5, printf("Opening stopped at %d/%d files\n", i, BIG);
        );

    MESSAGE(5, puts("Closing all files");
        );
    for (i--; i >=0; i--)
      {
          ret = Hclose(files[i]);
          if (ret < 0)
              printf("Error closing file %d\n", i);
      }
    MESSAGE(5, puts("Closed files");
        );

    MESSAGE(5, puts("Opening many files of different names");
        );
    for (i = 0; i < BIG; i++)
      {
          char        fname[100];
          sprintf(fname, "%s%1d.hdf", TESTFILE_NAME, i);
          files[i] = Hopen(fname, DFACC_ALL, 0);
          if (files[i] < 0)
            {
/*            i++; */
              break;
            } /* end if */
      }
    MESSAGE(5, printf("Opening stopped at %d/%d files\n", i, BIG);
        );

    MESSAGE(5, puts("Closing all files except first open");
        );
    for (i--; i > 0; i--)
      {
          ret = Hclose(files[i]);
          if (ret < 0)
              printf("Error closing file %d\n", i);
      }
    MESSAGE(5, puts("Closed files");
        );

    MESSAGE(5, puts("Opening write access elements");
        );
    for (i = 0; i < BIG; i++)
      {
          accs[i] = Hstartwrite(files[0], (uint16) 100, (uint16) i, 100L);
          if (accs[i] < 0)
              break;
      }
    MESSAGE(5, printf("Opening stoped at %d element\n", i);
        );

    MESSAGE(5, puts("Closing access elements");
        );
    for (i--; i >= 0; i--)
      {
          ret = Hendaccess(accs[i]);
          if (ret < 0)
              printf("Error ending access %d\n", i);
      }
    MESSAGE(5, puts("Ended access");
        );

    ret = Hclose(files[0]);
#ifdef QAK
    HDfree((VOIDP) files);
    HDfree((VOIDP) accs);
#endif
} /* end test_file_limits() */

void
test_ref_limits()
{
    int32 i;                /* local counting variable */
    int32 fid;              /* file ID */
    int32 iloop;
    const uint16 tag1=1000, /* tags to create objects with */
        tag2=1001;

    MESSAGE(6, printf("Testing reference # limits\n"););
    MESSAGE(7, printf("Writing out data\n"););
    /* Write out MAX_REF number of data items for each tag */
    fid=Hopen(TESTREF_NAME, DFACC_ALL, 512);
    CHECK(fid, FAIL, "Hopen");

    if(fid!=FAIL)
      {
          iloop = MAX_REF_TESTED;
          for(i=1; i<=(iloop/2)+5; i++)
            {
                int32 aid;
                uint16 ref;
                int32 data;
                int32 ret;

                /* Write out data to tag1 */
                ref=Htagnewref(fid,tag1);
                CHECK(ref, 0, "Htagnewref");
                aid=Hstartwrite(fid,tag1,ref,sizeof(int32));
                CHECK(aid, FAIL, "Hstartwrite");
                data=ref;
                ret=Hwrite(aid,sizeof(int32),&data);
                CHECK(ret, FAIL, "Hwrite");
                ret=Hendaccess(aid);
                CHECK(ret, FAIL, "Hendaccess");

                /* lets be a little smatter here */
                if (ret == FAIL)
                    break;

                /* Write out data to tag2 */
                ref=Htagnewref(fid,tag2);
                CHECK(ref, 0, "Htagnewref");
                aid=Hstartwrite(fid,tag2,ref,sizeof(int32));
                CHECK(aid, FAIL, "Hstartwrite");
                data=ref<<16;
                ret=Hwrite(aid,sizeof(int32),&data);
                CHECK(ret, FAIL, "Hwrite");
                ret=Hendaccess(aid);
                CHECK(ret, FAIL, "Hendaccess");
                /* lets be a little smatter here */
                if (ret == FAIL)
                    break;

            } /* end for */
          Hclose(fid);
      } /* end if */

    MESSAGE(7, printf("Verifying data\n"););
    
    /* Check the data written earlier */
    fid=Hopen(TESTREF_NAME, DFACC_READ, 0);
    CHECK(fid, FAIL, "Hopen");

    if(fid!=FAIL)
      {
          uint16 ref;
          int32 aid;
          int32 data;
          int32 ret;

          /* Read in data from tag1 */
          aid=Hstartread(fid,tag1,DFREF_WILDCARD);
          CHECK(aid, FAIL, "Hstartread");
          ret=Hread(aid,sizeof(int32),&data);
          CHECK(ret, FAIL, "Hread");
          ret=Hinquire(aid,NULL,NULL,&ref,NULL,NULL,NULL,NULL,NULL);
          CHECK(ret, FAIL, "Hinquire");
          VERIFY((uint16)data,ref,"Hread");

          while(Hnextread(aid,tag1,DFTAG_WILDCARD,DF_CURRENT)!=FAIL)
            {
                ret=Hread(aid,sizeof(int32),&data);
                CHECK(ret, FAIL, "Hread");
                ret=Hinquire(aid,NULL,NULL,&ref,NULL,NULL,NULL,NULL,NULL);
                CHECK(ret, FAIL, "Hinquire");
                VERIFY((uint16)data,ref,"Hread");
            } /* end while */
          ret=Hendaccess(aid);
          CHECK(ret, FAIL, "Hendaccess");

          /* Read in data from tag2 */
          aid=Hstartread(fid,tag2,DFREF_WILDCARD);
          CHECK(aid, FAIL, "Hstartread");
          ret=Hread(aid,sizeof(int32),&data);
          CHECK(ret, FAIL, "Hread");
          ret=Hinquire(aid,NULL,NULL,&ref,NULL,NULL,NULL,NULL,NULL);
          CHECK(ret, FAIL, "Hinquire");
          VERIFY((uint32)data,(((uint32)ref)<<16),"Hread");

          while(Hnextread(aid,tag2,DFTAG_WILDCARD,DF_CURRENT)!=FAIL)
            {
                ret=Hread(aid,sizeof(int32),&data);
                CHECK(ret, FAIL, "Hread");
                ret=Hinquire(aid,NULL,NULL,&ref,NULL,NULL,NULL,NULL,NULL);
                CHECK(ret, FAIL, "Hinquire");
                VERIFY((uint32)data,(((uint32)ref)<<16),"Hread");
            } /* end while */
          ret=Hendaccess(aid);
          CHECK(ret, FAIL, "Hendaccess");

          Hclose(fid);
      } /* end if */
} /* end test_ref_limits() */

void
test_hfile1()
{
    test_file_limits();
    test_ref_limits();
}

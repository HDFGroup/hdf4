#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.7  1992/06/30 23:24:53  chouck
Fixed call to delete file under VMS

 * Revision 1.6  1992/06/02  16:04:38  dilg
 * Fixed error in expected return code from DFputelement().
 *
 * Revision 1.5  1992/06/01  19:41:48  dilg
 * Added explicit test for dangling aids.
 *
 * Revision 1.4  1992/05/07  16:37:55  dilg
 * Changed output file name frm "o1" to "tstubs.hdf"
 *
 * Revision 1.3  1992/04/28  19:36:55  dilg
 * Some minor cosmetic changes.
 *
 * Revision 1.2  1992/04/28  19:09:29  dilg
 * Corrected incorrect expected return value for DFwrite on line 347.
 *
 * Revision 1.1  1992/02/26  23:05:33  dilg
 * Initial revision
 *
*/
#include <stdio.h>
#include "df.h"

int nerrors = 0;
char ar0[] = "Testing...";
char ar1[] = "...one...";
char ar2[30] = "...two...";
char ar3[] = "...three";

main()
{
    DF *dfile;
    DFdata dfinfo;
    DFdesc dlist[5];
    char in[20];
    int a0size, a1size, a2size, a3size;
    int ret, err, i, nd;


    a0size = strlen(ar0) + 1;
    a1size = strlen(ar1) + 1;
    a2size = strlen(ar2) + 1;
    a3size = strlen(ar3) + 1;

    printf("This program will test the DF emulation layer\n");
    printf("of HDF 3.2 and beyond.  Many routines will be\n");
    printf("tested individually.  Each test will report its\n");
    printf("name and results.  If all goes well, all of the\n");
    printf("results will begin with 'Success'.  If a test\n");
    printf("fails, the results line will begin with '>>>Failure'.\n");
    printf("An error count is kept and printed out at the end.\n");
    printf("\nHit <return> to continue.");
    getchar();

#if defined PC || defined VMS
#ifdef VMS
    system("del tstubs.hdf;*");
#else
    system("del tstubs.hdf");
#endif
#else
    system("rm tstubs.hdf");
#endif /* PC || VMS */

    printf("\nTesting DFishdf... (should fail with error %d)\n", DFE_BADOPEN);
    ret = DFishdf("tstubs.hdf");
    if (ret == -1) {
	printf("Success:  DFishdf failed with DFerror = %d\n", DFerror);
    } else {
	printf("Failure at line %d:  ", __LINE__ - 5);
	printf("Non-existent file looks like HDF file.\n");
	nerrors++;
    }

    printf("\nTesting DFopen... (new file)\n");
    dfile = DFopen("tstubs.hdf", DFACC_WRITE|DFACC_CREATE, 0);
    if (dfile == (DF *)NULL) {
	printf("***ERROR %d opening file.\n", DFerror);
	exit(1);
    } else
	printf("Success!\n");

    printf("\nTesting DFclose...\n");
    ret = DFclose(dfile);
    if (ret == -1) {
	printf(">>>Close failed at line %d.\n", __LINE__ - 2);
	printf("   DFerror = %d\n", DFerror);
	nerrors++;
    } else
	printf("Success!\n");

    printf("\nTesting DFclose... (invalid file; should fail with error %d)\n",
	   DFE_NOTOPEN);
    ret = DFclose(dfile);
    if (ret == -1) {
	printf("Success: DFclose failed with DFerror = %d\n", DFerror);
    } else {
	printf(">>>Failure at line %d:  Close allowed on unopened file.\n",
		__LINE__ - 4);
	nerrors++;
    }

    printf("\nTesting DFerrno...\n");
    ret = DFerrno();
    if (ret != DFerror) {
	printf(">>>DFerrno failed at line %d.  Returned %d\n",__LINE__ - 2,ret);
	printf("   DFerror = %d\n", DFerror);
	nerrors++;
    } else
	printf("Success!\n");

    printf("\nTesting DFopen... (existing file)\n");
    dfile = DFopen("tstubs.hdf", DFACC_WRITE, 0);
    if (dfile == (DF *)NULL) {
	printf("***ERROR %d opening file.\n", DFerror);
	exit(1);
    } else
	printf("Success!\n");

    printf("\nTesting DFputelement...\n");
    ret = DFputelement(dfile, (uint16)255, (uint16)1, ar0, a0size);
    if (ret != a0size) {
	printf(">>>Write failed at line %d.\n", __LINE__ - 2);
	printf("   DFerror = %d\n", DFerror);
	nerrors++;
    } else
	printf("Success!\n");

    printf("\nTesting DFgetelement...\n");
    ret = DFgetelement(dfile, (uint16)255, (uint16)1, in);
    if (ret != a0size) {
	printf(">>>Read failed at line %d.\n", __LINE__ - 2);
	printf("   String read:  %-20s\n", in);
	printf("   DFerror = %d\n", DFerror);
	nerrors++;
    } else {
	err = 0;
	for (i=0; i<a0size; i++) {
	    if (in[i] != ar0[i])
		err = 1;
	}
	if (err == 1) {
	    printf(">>>Read failed at line %d:  strings differ.\n",__LINE__-13);
	    printf("   String written:  %-20s\n", ar0);
	    printf("   String read:     %-20s\n", in);
	    nerrors++;
	} else
	    printf("Success:  string read is the same as string written.\n");
    }
    for (i=0; i<20; i++)
	in[i] = (char)0;

    printf("\nTesting DFaccess (write)...\n");
    ret = DFaccess(dfile, (uint16)255, (uint16)3, "w");
    if (ret == -1) {
	printf(">>>Write access failed at line %d.\n", __LINE__ - 2);
	printf("   DFerror = %d\n", DFerror);
	nerrors++;
    } else
	printf("Success!\n");

    printf("\nTesting DFread... (should fail with error %d)\n", DFE_ARGS);
    ret = DFread(dfile, in, 5);
    if (ret == -1) {
	printf("Success:  DFread failed with DFerror = %d\n", DFerror);
    } else {
	printf(">>>Failure at line %d:  ", __LINE__ - 4);
	printf("Read allowed on element set up for write.\n");
	nerrors++;
    }
    for (i=0; i<20; i++)
	in[i] = (char)0;

    printf("\nTesting DFwrite...\n");
    ret = DFwrite(dfile, ar1, a1size);
    if (ret != a1size) {
	printf(">>>Write failed at line %d.  Wrote %d of %d bytes.\n",
		__LINE__ - 3, ret, a1size);
	printf("   DFerror = %d\n", DFerror);
	nerrors++;
    } else
	printf("Success!\n");

    printf("\nTesting DFaccess (read)...\n");
    ret = DFaccess(dfile, (uint16)255, (uint16)3, "r");
    if (ret == -1) {
	printf(">>>Read access failed at line %d.\n", __LINE__ - 2);
	printf("   DFerror = %d\n", DFerror);
	nerrors++;
    } else
	printf("Success!\n");

    printf("\nTesting DFwrite... (should fail with error %d)\n", DFE_BADACC);
    ret = DFwrite(dfile, in, 5);
    if (ret == -1) {
	printf("Success:  DFwrite failed with DFerror = %d\n", DFerror);
    } else {
	printf(">>>Failure at line %d:  ", __LINE__ - 4);
	printf("Write allowed on element set up for read.\n");
	nerrors++;
    }

    printf("\nTesting DFread...\n");
    ret = DFread(dfile, in, a1size);
    if (ret != a1size) {
	printf(">>>Read failed at line %d.  Read %d of %d bytes.\n",
		__LINE__ - 3, ret, a1size);
	printf("   String read:  %-20s\n", in);
	printf("   DFerror = %d\n", DFerror);
	nerrors++;
    } else {
	err = 0;
	for (i=0; i<a1size; i++) {
	    if (in[i] != ar1[i])
		err = 1;
	}
	if (err == 1) {
	    printf(">>>Read failed at line %d:  strings differ.\n",__LINE__-14);
	    printf("   String written:  %-20s\n", ar1);
	    printf("   String read:     %-20s\n", in);
	    nerrors++;
	} else
	    printf("Success:  string read is the same as string written.\n");
    }
    for (i=0; i<20; i++)
	in[i] = (char)0;

    printf("\nTesting DFnumber...\n");
    nd = DFnumber(dfile, (uint16)255);
    if (nd != 2) {
	printf(">>>DFnumber failed at line %d.  ", __LINE__ - 2);
	printf("Reported %d occurrances of tag 255\n", nd);
	printf("   rather than 2.\n");
	printf("   DFerror = %d\n", DFerror);
	nerrors++;
    } else
	printf("Success!\n");
    
    printf("\nTesting DFdescriptors...\n");
    ret = DFdescriptors(dfile, dlist, 0, 5);
    if (ret != (nd + 1)) {
	printf(">>>DFdescriptors failed at line %d.  Returned %d\n",
		__LINE__ - 3, ret);
	printf("   DFerror = %d\n", DFerror);
	nerrors++;
    } else
	if ((dlist[1].tag != 255) || (dlist[1].ref != 1) ||
	    (dlist[1].length != a0size) || (dlist[2].tag != 255) ||
	    (dlist[2].ref != 3) || (dlist[2].length != a1size)) {
	    printf(">>>DFdescriptors failed at line %d.  ", __LINE__ - 10);
	    printf("Descriptors have unexpected values\n");
	    nerrors++;
        } else
	    printf("Success!\n");

    printf("\nTesting DFupdate\n");
    ret = DFupdate(dfile);
    if (ret != 0) {
	printf(">>>DFupdate failed at line %d.\n", __LINE__ - 2);
	printf("   DFerror = %d\n", DFerror);
	nerrors++;
    } else
	printf("Success!\n");

    printf("\nTesting DFstat\n");
    ret = DFstat(dfile, &dfinfo);
    if (ret != 0) {
	printf(">>>DFstat failed at line %d.\n", __LINE__ - 2);
	printf("   DFerror = %d\n", DFerror);
	nerrors++;
    } else
	printf("Success!\n");
    
    printf("\nTesting DFnewref...\n");
    ret = DFnewref(dfile);
    if (ret != 4) {
	printf(">>>DFnewref failed at line %d.  ", __LINE__ - 2);
	printf("Returned ref. no. %d instead of 4\n", ret);
	printf("   DFerror = %d\n", DFerror);
	nerrors++;
    } else
	printf("Success!\n");

    printf("\nTesting DFdup...\n");
    ret = DFdup(dfile, (uint16)127, (uint16)7, (uint16)255, (uint16)3);
    if (ret == -1) {
	printf(">>>DFdup failed at line %d.\n", __LINE__ - 2);
	printf("   DFerror = %d\n", DFerror);
	nerrors++;
    } else {
	ret = DFnumber(dfile, (uint16)127);
	if (ret != 1) {
	    printf(">>>DFdup failed at line %d.  Duplicated tag not found.\n",
		   __LINE__ - 8);
	    printf("   DFerror = %d\n", DFerror);
	    nerrors++;
	} else
	    printf("Success!\n");
    }

    printf("\nTesting DFdel...\n");
    ret = DFdel(dfile, (uint16)127, (uint16)7);
    if (ret == -1) {
	printf(">>>DFdel failed at line %d.\n", __LINE__ - 2);
	printf("   DFerror = %d\n", DFerror);
	nerrors++;
    } else {
	ret = DFnumber(dfile, (uint16)127);
	if (ret != 0) {
	    printf(">>>DFdel failed at line %d.  ", __LINE__ - 8);
	    printf("Found %d instances of deleted tag.\n", ret);
	    printf("   DFerror = %d\n", DFerror);
	    nerrors++;
	} else
	    printf("Success!\n");
    }

    printf("\nTesting DFsetfind...\n");
    ret = DFsetfind(dfile, (uint16)254, (uint16)0);
    if (ret == -1) {
	printf(">>>DFsetfind failed at line %d.\n", __LINE__ - 2);
	printf("   DFerror = %d\n", DFerror);
	nerrors++;
    } else
	printf("Success!\n");

    printf("\nTesting DFfind...\n");
    ret = DFdup(dfile, (uint16)254, (uint16)4, (uint16)255, (uint16)3);
    if (ret != 0)
        printf(">>>DFdup 1 failed.\n");
    ret = DFdup(dfile, (uint16)254, (uint16)5, (uint16)255, (uint16)3);
    if (ret != 0)
        printf(">>>DFdup 2 failed.\n");
    ret = DFdup(dfile, (uint16)254, (uint16)6, (uint16)255, (uint16)3);
    if (ret != 0)
        printf(">>>DFdup 3 failed.\n");
    for (i=4; i<7; i++) {
    ret = DFfind(dfile, dlist);
    if (ret == -1) {
	printf(">>>DFfind failed at line %d.\n", __LINE__ - 2);
	printf("   DFerror = %d\n", DFerror);
	nerrors++;
    } else
	if ((dlist[0].tag != 254) || (dlist[0].ref != i) ||
	    (dlist[0].length != a1size)) {
	    printf(">>>DFfind failed at line %d.  ", __LINE__ - 8);
	    printf("Descriptor has unexpected values\n");
	    nerrors++;
        } else
	    printf("Success!\n");
    } /* for i */

    printf("\nTesting appending...\n");
    ret = DFputelement(dfile, (uint16)255, (uint16)7, ar2, a2size);
    if (ret != a2size) {
	printf(">>>Write failed at line %d.\n", __LINE__ - 2);
	printf("   DFerror = %d\n", DFerror);
	nerrors++;
    } else {
	ret = DFaccess(dfile, (uint16)255, (uint16)7, "a");
        if (ret != 0) {
	    printf(">>>DFaccess failed at line %d.\n", __LINE__ - 2);
	    printf("   DFerror = %d\n", DFerror);
	    nerrors++;
        } else {
	    ret = DFseek(dfile, a2size - 1);
            if (ret != 0) {
	    	printf(">>>DFseek failed at line %d.\n", __LINE__ - 2);
	    	printf("   DFerror = %d\n", DFerror);
	    	nerrors++;
            } else {
		ret = DFwrite(dfile, ar3, a3size);
                if (ret != a3size) {
	    	    printf(">>>DFwrite failed at line %d.\n", __LINE__ - 2);
	    	    printf("   DFerror = %d\n", DFerror);
	    	    nerrors++;
                } else {
		    strcat(ar2, ar3);
		    ret = DFgetelement(dfile, (uint16)255, (uint16)7, in);
    		    if (ret != strlen(ar2)+1) {
			printf(">>>Read failed at line %d.\n", __LINE__ - 2);
			printf("   String read:  %-20s\n", in);
			printf("   DFerror = %d\n", DFerror);
			nerrors++;
    		    } else {
			err = 0;
			for (i=0; i<strlen(ar2)+1; i++) {
			    if (in[i] != ar2[i])
				err = 1;
			}
			if (err == 1) {
			    printf(">>>Append failed at line %d:", __LINE__-13);
			    printf("  strings differ.\n");
			    printf("   String written:  %-20s\n", ar2);
			    printf("   String read:     %-20s\n", in);
			    nerrors++;
			} else {
		    	    printf("Success!\n");
			}
		    }
		}
	    }
	}
    }

    ret = DFclose(dfile);
    if (ret != 0) {
	printf(">>>Failure:  DFclose failed (probably due to open aids)\n");
	printf("   DFerror = %d\n", DFerror);
	HEprint(stderr, 0);
	nerrors++;
    }

    printf("\n\nTest Summary:\n");
    printf("   %d errors were encountered.\n", nerrors);
    if (nerrors != 0)
	printf("   Please check program output carefully.\n");
}

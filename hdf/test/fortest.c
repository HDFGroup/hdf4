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

#define TESTMASTER

#include "hdf.h"
#include "tutils.h"
#include "fortest.h"
#ifdef VMS
#include <processes.h>
#include <string.h>
#endif
#define NUMOFTESTS 20
#define VERSION "4.0beta"
#define BUILDDATE "Wed Nov 22 1995"

struct TestStruct
  {
      int         NumErrors;
      char        Description[64];
      int         SkipFlag;
      char        Name[16];
      char        Call[20];
  }
Test[NUMOFTESTS];

int
InitTest(const char *TheName, const char *TheCall, const char *TheDescr)
{
    static int  Index = 0;
    HDstrcpy(Test[Index].Description, TheDescr);
    HDstrcpy(Test[Index].Name, TheName);
    HDstrcpy(Test[Index].Call, TheCall);
    Test[Index].NumErrors = -1;
    Test[Index].SkipFlag = 0;
    Index++;
    return(Index);
}

void
CallFortranTest(char *TheCall)
{
#ifdef VMS
    static char TheProc[25];

    HDstrcpy(TheProc, "run []");
    HDstrcat(TheProc, TheCall);
    system(TheProc);
#else
    char TheLocalCall[80];

    HDstrcpy(TheLocalCall, "./");
    HDstrcat(TheLocalCall, TheCall);
    system(TheLocalCall);
#endif
}

int
main(int argc, char *argv[])
{
    int         CLLoop;         /* Command Line Loop */
    int         Loop, Loop1;
    int         Summary = 0;
    int         CleanUp = 1;
    int         ret;
    int         num_tests=0;
    uint32      lmajor, lminor, lrelease;
    char        lstring[81];
    char        verb_env[81];
    char        verb_tmp[81];
    FILE	*cmdfile, *fopen();
    char	*cmdfilename="fortest.arg";

#ifdef NO
    ret = Hgetlibversion(&lmajor, &lminor, &lrelease, lstring);
    printf("\nFORTEST V%s Built on: %s \n", VERSION, BUILDDATE);
    printf("HDF Library Version: %u.%ur%u, %s\n\n",
        (unsigned) lmajor, (unsigned) lminor, (unsigned) lrelease, lstring);
#endif

    num_tests=InitTest("slab", "slabwf", "");
    num_tests=InitTest("r24", "t24f", "");
    num_tests=InitTest("an", "tanf", "");
    num_tests=InitTest("anfile", "tanfilef", "");
    num_tests=InitTest("manf", "manf", "");
    num_tests=InitTest("mgrf", "mgrf", "");
    num_tests=InitTest("p", "tpf", "");
    num_tests=InitTest("r8", "tr8f", "");
    num_tests=InitTest("sdmms", "tsdmmsf", "");
    num_tests=InitTest("sdnmms", "tsdnmmsf", "");
    num_tests=InitTest("sdnnt", "tsdnntf", "");
    num_tests=InitTest("sdnt", "tsdntf", "");
    num_tests=InitTest("sdstr", "tsdstrf", "");
    num_tests=InitTest("vsetf", "tvsetf", "");
#ifndef DEC_ALPHA
    num_tests=InitTest("stubs", "tstubsf", "");
#else
    printf("   Skipping stubs\n");
#endif
    if ((cmdfile = fopen(cmdfilename, "w")) == NULL){
	printf("***Can't write to cmdfile(%s)***\n", cmdfilename);
	return(-1);
    }

    Verbosity = 3;  /* Default Verbosity is Low */
    for (CLLoop = 1; CLLoop < argc; CLLoop++)
      {
          if ((argc > CLLoop + 1) && ((HDstrcmp(argv[CLLoop], "-verbose") == 0) ||
                                      (HDstrcmp(argv[CLLoop], "-v") == 0)))
            {
                if (argv[CLLoop + 1][0] == 'l')
                    Verbosity = 5;
                else if (argv[CLLoop + 1][0] == 'm')
                    Verbosity = 7;
                else if (argv[CLLoop + 1][0] == 'h')
                    Verbosity = 9;
                else
                    Verbosity = atoi(argv[CLLoop + 1]);
		fprintf(cmdfile, "%s %d\n", VERBOSITY_STR, Verbosity);
            }
          if ((argc > CLLoop) && ((HDstrcmp(argv[CLLoop], "-summary") == 0) ||
                                  (HDstrcmp(argv[CLLoop], "-s") == 0)))
            {
                Summary = 1;
            }
          if ((argc > CLLoop) && (HDstrcmp(argv[CLLoop], "-help") == 0))
            {
                printf("Usage: fortest [-v[erbose] (l[ow]|m[edium]|h[igh]|0-9)] \n");
                printf("               [-[e]x[clude] name+] \n");
                printf("               [-o[nly] name+] \n");
                printf("               [-b[egin] name] \n");
                printf("               [-s[ummary]]  \n");
                printf("               [-c[leanno]]  \n");
                printf("\n\n");
                printf("verbose   controls the amount of information displayed\n");
                printf("exclude   to exclude tests by name\n");
                printf("only      to name tests which should be run\n");
                printf("begin     start at the name of the test givin\n");
                printf("summary   prints a summary of test results at the end\n");
                printf("cleanno   does not delete *.hdf files after execution of tests\n");
                printf("\n\n");
                printf("This program currently tests the following: \n\n");
                printf("%16s %s\n", "Name", "Description");
                printf("%16s %s\n", "----", "-----------");
                for (Loop = 0; Loop < num_tests; Loop++)
                  {
                      printf("%16s %s\n", Test[Loop].Name, Test[Loop].Description);
                  }
                printf("\n\n");
                exit(0);
            }
          if ((argc > CLLoop) && ((HDstrcmp(argv[CLLoop], "-cleanno") == 0) ||
                                  (HDstrcmp(argv[CLLoop], "-c") == 0)))
            {
                CleanUp = 0;
		fprintf(cmdfile, "%s %d\n", CLEAN_STR, CleanUp);
            }
          if ((argc > CLLoop + 1) && ((HDstrcmp(argv[CLLoop], "-exclude") == 0) ||
                                      (HDstrcmp(argv[CLLoop], "-x") == 0)))
            {
                Loop = CLLoop + 1;
                while ((Loop < argc) && (argv[Loop][0] != '-'))
                  {
                      for (Loop1 = 0; Loop1 < num_tests; Loop1++)
                        {
                            if (HDstrcmp(argv[Loop], Test[Loop1].Name) == 0)
                                Test[Loop1].SkipFlag = 1;
                        }
                      Loop++;
                  }
            }
          if ((argc > CLLoop + 1) && ((HDstrcmp(argv[CLLoop], "-begin") == 0) ||
                                      (HDstrcmp(argv[CLLoop], "-b") == 0)))
            {
                Loop = CLLoop + 1;
                while ((Loop < argc) && (argv[Loop][0] != '-'))
                  {
                      for (Loop1 = 0; Loop1 < num_tests; Loop1++)
                        {
                            if (HDstrcmp(argv[Loop], Test[Loop1].Name) != 0)
                                Test[Loop1].SkipFlag = 1;
                            if (HDstrcmp(argv[Loop], Test[Loop1].Name) == 0)
                                Loop1 = num_tests;
                        }
                      Loop++;
                  }
            }
          if ((argc > CLLoop + 1) && ((HDstrcmp(argv[CLLoop], "-only") == 0) ||
                                      (HDstrcmp(argv[CLLoop], "-o") == 0)))
            {
                for (Loop = 0; Loop < num_tests; Loop++)
                  {
                      Test[Loop].SkipFlag = 1;
                  }
                Loop = CLLoop + 1;
                while ((Loop < argc) && (argv[Loop][0] != '-'))
                  {
                      for (Loop1 = 0; Loop1 < num_tests; Loop1++)
                        {
                            if (HDstrcmp(argv[Loop], Test[Loop1].Name) == 0)
                                Test[Loop1].SkipFlag = 0;
                        }
                      Loop++;
                  }
            }
      }

    /*  printf("The Verbosity is %d \n",Verbosity); */
/*
    HDstrcpy(verb_env,FOR_VERB);
    HDstrcat(verb_env,"=");
    sprintf(verb_tmp,"%d",Verbosity);
    HDstrcat(verb_env,verb_tmp);
#ifndef vms
    HDputenv(verb_env);
#endif
*/
    for (Loop = 0; Loop < num_tests; Loop++)
      {
          if (Test[Loop].SkipFlag)
            {
		fprintf(cmdfile, "%s %s\n", SKIP_STR, Test[Loop].Name);
#ifdef NO
                MESSAGE(2, printf("Skipping -- %s (%s) \n",
                                  Test[Loop].Description, Test[Loop].Name);
                    );
#endif
            }
          else
            {
		fprintf(cmdfile, "%s %s\n", TEST_STR, Test[Loop].Name);
#ifdef NO
                MESSAGE(2, printf("Testing  -- %s (%s) \n",
                                  Test[Loop].Description, Test[Loop].Name);
                    );
                MESSAGE(5, printf("===============================================\n");
                    );
                Test[Loop].NumErrors = num_errs;
                CallFortranTest(Test[Loop].Call);
                Test[Loop].NumErrors = num_errs - Test[Loop].NumErrors;
                MESSAGE(5, printf("===============================================\n");
                    );
                MESSAGE(5, printf("There were %d errors detected.\n\n", Test[Loop].NumErrors);
                    );
#endif
            }
      }

    fclose(cmdfile);
#ifdef VMS
    {
        char *comprocfile="fortest.com";

        if ((cmdfile = fopen(comprocfile, "w")) == NULL){
           printf("***Can't write to cmdfile(%s)***\n", comprocfile);
           return(-1);
        }
        fprintf(cmdfile, "%s %s\n",
            "DEFINE/USER_MODE SYS$INPUT", cmdfilename);
        fprintf(cmdfile, "%s\n", "run []fortestF.exe");
        fclose(cmdfile);
        return(system("@fortest.com"));
    }
#else
        {
	char fortrancmd[100];
	HDstrcpy(fortrancmd, "./fortestF < ");
	HDstrcat(fortrancmd, cmdfilename);
	return(system(fortrancmd));
    }
#endif
#ifdef NO
    MESSAGE(2, printf("\n\n");
        )
        if (num_errs)
      {
          fprintf(stderr, "!!! %d Error(s) were detected !!!\n\n", num_errs);
      }
    else
      {
          fprintf(stderr, "All tests were successful. \n\n");
      }

    if (Summary)
      {
/*    printf("\n\n"); */
/*    printf("==========================================================\n"); */
          printf("Summary of Test Results:\n");
/*    printf("==========================================================\n\n"); */
          printf("Name of Test     Errors Description of Test\n");
          printf("---------------- ------ --------------------------------------\n");

          for (Loop = 0; Loop < num_tests; Loop++)
            {
                if (Test[Loop].NumErrors == -1)
                    printf("%16s %6s %s\n", Test[Loop].Name, "N/A",
                           Test[Loop].Description);
                else
                    printf("%16s %6d %s\n", Test[Loop].Name, Test[Loop].NumErrors,
                           Test[Loop].Description);
            }
          printf("\n\n");
/*    printf("==========================================================\n\n");
 */
      }

    if (CleanUp)
      {
          MESSAGE(2, printf("\nCleaning Up...\n\n");
              );
#ifndef VMS
          system("rm -f *.hdf");
#else
          system("delete *.hdf;*");
#endif
      }
    return (0);
#endif
}

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
#ifdef VMS
#include processes
#include string
#endif
#define NUMOFTESTS 16
#define VERSION "0.2beta"
#define BUILDDATE "Mon Feb 8 1993"

struct TestStruct {
  int  NumErrors;
  char Description[64];
  int  SkipFlag;
  char Name[16];
  char Call[20];
} Test[NUMOFTESTS] ;

void InitTest (const char *TheName,const char * TheCall,const char * TheDescr)
{
  static int Index=0;
  HDstrcpy(Test[Index].Description,TheDescr);
  HDstrcpy(Test[Index].Name,TheName);
  HDstrcpy(Test[Index].Call,TheCall);
  Test[Index].NumErrors = -1;
  Test[Index].SkipFlag = 0;
  Index++;
}

void CallFortranTest(char *TheCall) 
{
#ifdef VMS
  static  char TheProc[25];

  HDstrcpy(TheProc , "run ");   
  HDstrcat(TheProc, TheCall);
  system(TheProc);
#else
  system(TheCall);
#endif
}

int main (int argc, char *argv[]) 
{
  int CLLoop; /* Command Line Loop */
  int Loop, Loop1;
  int Summary=0;
  int CleanUp    = 1;
  int ret;
  uint32 lmajor, lminor, lrelease;
  char lstring[81];

  InitTest("slab1","./slab1wf","");
  InitTest("slab2","./slab2wf","");
  InitTest("slab3","./slab3wf","");
  InitTest("slab4","./slab4wf","");
  InitTest("slab","./slabwf","");
  InitTest("r24","./t24f","");
  InitTest("an","./tanf","");
  InitTest("anfile","./tanfilef","");
  InitTest("p","./tpf","");
  InitTest("r8","./tr8f","");
  InitTest("sdmms","./tsdmmsf","");
  InitTest("sdnmms","./tsdnmmsf","");
  InitTest("sdnnt","./tsdnntf","");
  InitTest("sdnt","./tsdntf","");
  InitTest("sdstr","./tsdstrf","");
#ifndef DEC_ALPHA
  InitTest("stubs","./tstubsf","");
#endif

  Verbosity = 4; /* Default Verbosity is Low */
  ret = Hgetlibversion(&lmajor, &lminor, &lrelease, lstring);

  printf("\nFORTEST V%s Built on: %s \n", VERSION, BUILDDATE );
  printf("HDF Library Version: %u.%ur%u, %s\n\n",
         (unsigned)lmajor, (unsigned)lminor, (unsigned)lrelease, lstring);
  for (CLLoop = 1; CLLoop < argc; CLLoop++) {
    if ((argc > CLLoop+1) && ((HDstrcmp(argv[CLLoop],"-verbose")==0) ||
        (HDstrcmp(argv[CLLoop],"-v")==0))) {
      if (argv[CLLoop+1][0] == 'l')
        Verbosity = 4;
      else if (argv[CLLoop+1][0] == 'm')
        Verbosity = 6;
      else if (argv[CLLoop+1][0] == 'h')
        Verbosity = 10;
      else 
        Verbosity = atoi(argv[CLLoop+1]); 
    }
    if ((argc > CLLoop) && ((HDstrcmp(argv[CLLoop],"-summary")==0) ||
        (HDstrcmp(argv[CLLoop],"-s")==0))) {
      Summary = 1;
    }
    if ((argc > CLLoop) && (HDstrcmp(argv[CLLoop],"-help")==0)) {
      printf("Usage: fortest [-v[erbose] (l[ow]|m[edium]|h[igh]|0-10)] \n");
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
      printf("%16s %s\n","Name","Description");
      printf("%16s %s\n","----","-----------");
      for (Loop = 0; Loop < NUMOFTESTS; Loop++) {
          printf("%16s %s\n",Test[Loop].Name,Test[Loop].Description);
      }
      printf("\n\n");
      exit(0);
    }
    if ((argc > CLLoop) && ((HDstrcmp(argv[CLLoop],"-cleanno")==0) ||
        (HDstrcmp(argv[CLLoop],"-c")==0))) {
      CleanUp = 0;
    }
    if ((argc > CLLoop+1) && ((HDstrcmp(argv[CLLoop],"-exclude") ==0) ||
        (HDstrcmp(argv[CLLoop],"-x")==0))) {
      Loop = CLLoop+1;
      while ((Loop < argc) && (argv[Loop][0] != '-')) {
        for (Loop1 = 0; Loop1 < NUMOFTESTS; Loop1++) {
          if (HDstrcmp(argv[Loop],Test[Loop1].Name) == 0)  
              Test[Loop1].SkipFlag = 1;
        }
        Loop++;
      }
    }
    if ((argc > CLLoop+1) && ((HDstrcmp(argv[CLLoop],"-begin") ==0) ||
        (HDstrcmp(argv[CLLoop],"-b")==0))) {
      Loop = CLLoop+1;
      while ((Loop < argc) && (argv[Loop][0] != '-')) {
        for (Loop1 = 0; Loop1 < NUMOFTESTS; Loop1++) {
          if (HDstrcmp(argv[Loop],Test[Loop1].Name) != 0)  
              Test[Loop1].SkipFlag = 1;
          if (HDstrcmp(argv[Loop],Test[Loop1].Name) == 0)
            Loop1 = NUMOFTESTS;
        }
        Loop++;
      }
    }
    if ((argc > CLLoop+1) && ((HDstrcmp(argv[CLLoop],"-only") ==0) ||
        (HDstrcmp(argv[CLLoop],"-o")==0))) {
      for (Loop = 0; Loop < NUMOFTESTS; Loop++) {
        Test[Loop].SkipFlag = 1;
      }
      Loop = CLLoop+1;
      while ((Loop < argc) && (argv[Loop][0] != '-')) {
        for (Loop1 = 0; Loop1 < NUMOFTESTS; Loop1++) {
          if (HDstrcmp(argv[Loop],Test[Loop1].Name) == 0)  
              Test[Loop1].SkipFlag = 0;
        }
        Loop++;
      }
    }
  }


  /*  printf("The Verbosity is %d \n",Verbosity); */

  for (Loop = 0; Loop < NUMOFTESTS; Loop++) {
    if (Test[Loop].SkipFlag) {
      MESSAGE(4,printf("Skipping -- %s \n",Test[Loop].Description););
    }
    else {
      MESSAGE(2,printf("Testing  -- %s (%s) \n",
                       Test[Loop].Description,Test[Loop].Name););
      MESSAGE(5,printf("===============================================\n"););
      Test[Loop].NumErrors = num_errs;
      CallFortranTest(Test[Loop].Call);
      Test[Loop].NumErrors = num_errs - Test[Loop].NumErrors;
      MESSAGE(5,printf("===============================================\n"););
      MESSAGE(5,printf("There were %d errors detected.\n\n",Test[Loop].NumErrors););
    }
  }

  MESSAGE(2,printf("\n\n");)
  if(num_errs) {
    fprintf(stderr, "!!! %d Error(s) were detected !!!\n\n", num_errs);
  } else {
    fprintf(stderr, "All tests were successful. \n\n");
  }


  if(Summary) {
/*    printf("\n\n"); */
/*    printf("==========================================================\n");*/
    printf("Summary of Test Results:\n"); 
/*    printf("==========================================================\n\n");*/
    printf("Name of Test     Errors Description of Test\n");
    printf("---------------- ------ --------------------------------------\n");

    for (Loop = 0; Loop < NUMOFTESTS; Loop++) {
      if (Test[Loop].NumErrors == -1)
        printf("%16s %6s %s\n",Test[Loop].Name,"N/A",
               Test[Loop].Description);
      else
        printf("%16s %6d %s\n",Test[Loop].Name,Test[Loop].NumErrors,
               Test[Loop].Description);
    }
    printf("\n\n");
/*    printf("==========================================================\n\n");
*/
  }

  if(CleanUp) {
    MESSAGE(2,printf("\nCleaning Up...\n\n"););
#ifndef VMS
    system("rm *.hdf");
#else
    system("delete *.hdf;*");
#endif
  }
  return(0);
}


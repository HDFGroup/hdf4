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

#define MAXNUMOFTESTS 20

/* Exportable variables */
int num_errs = 0;
int Verbocity = 0;

/* Internal Variables */
int Index=0;

/* ANY new test needs to have a prototype in tproto.h */
#include "tproto.h"

#ifdef TEST_PC
#define FAR far
#else
#define FAR /* */
#endif

#ifdef TEST_PC
FILE *dbg_file;
#endif

struct TestStruct {
  int  NumErrors;
  char Description[64];
  int  SkipFlag;
  char Name[16];
  VOID (*Call)();
} FAR Test[MAXNUMOFTESTS] ;

#ifdef PROTOTYPE
void InitTest (const char *TheName, VOID (*TheCall)(),const char *TheDescr)
#else
void InitTest (TheName, TheCall, TheDescr)
const char* TheName;
VOID (*TheCall)();
const char* TheDescr;
#endif
{
  if(Index>=MAXNUMOFTESTS) {
      printf("Uh-oh, too many tests added, increase MAXNUMOFTEST!\n");
      exit(0);
    } /* end if */
  HDstrcpy(Test[Index].Description,TheDescr);
  HDstrcpy(Test[Index].Name,TheName);
  Test[Index].Call = TheCall;
  Test[Index].NumErrors = -1;
  Test[Index].SkipFlag = 0;
  Index++;
}

#ifdef PROTOTYPE
main (int argc, char *argv[])
#else
main (argc, argv)
     int argc;
     char* argv[];
#endif
{
  int CLLoop; /* Command Line Loop */
  int Loop,Loop1;
  int Summary    = 0;
  int CleanUp    = 1;
  int ret;
  uint32 lmajor, lminor, lrelease;
  char lstring[81];

#ifdef TEST_PC
    dbg_file=fopen("test.dbg","w+");
#endif

  InitTest("vers",test_vers,"VERSION OF LIBRARY");
  InitTest("24bit",test_r24,"24BIT RASTER IMAGE INTERFACE");
  InitTest("8bit",test_r8,"8BIT RASTER IMAGE INTERFACE");
  InitTest("pal",test_pal,"PALETTE INTERFACE");
  InitTest("sdstr",test_tsdstr,"DATASTRINGS");
  InitTest("hblocks",test_hblocks,"HBLOCKS");
  InitTest("anot",test_an,"ANNOTATIONS");
  InitTest("anfile",test_anfile,"FILE ANNOTATIONS");
  InitTest("extelt",test_hextelt,"EXTERNAL ELEMENTS");
  InitTest("hfile",test_hfile,"HFILE");
  InitTest("hfile1",test_hfile1,"HFILE LIMITS");
  InitTest("sdmms",test_sdmms,"SDMMS");
  InitTest("sdnmms",test_sdnmms,"SDNMMS");
  InitTest("slabs",test_slab,"HYPERSLAB INTERFACE");
  InitTest("litend",test_litend,"LITTLE-ENDIAN INTERFACE");
  InitTest("vset",test_vsets,"VSET Interface Test");


  Verbocity = 4; /* Default Verbocity is Low */
  ret = Hgetlibversion(&lmajor, &lminor, &lrelease, lstring);

  printf("\nFor help use: testhdf -help\n");
  printf("Built with HDF Library Version: %d.%dr%d\n\n",
         lmajor, lminor, lrelease, lstring);
  for (CLLoop = 1; CLLoop < argc; CLLoop++) {
    if ((argc > CLLoop+1) && (HDstrcmp(argv[CLLoop],"-verbose")==0) ||
        (HDstrcmp(argv[CLLoop],"-v")==0)) {
      if (argv[CLLoop+1][0] == 'l')
        Verbocity = 4;
      else if (argv[CLLoop+1][0] == 'm')
        Verbocity = 6;
      else if (argv[CLLoop+1][0] == 'h')
        Verbocity = 10;
      else
        Verbocity = atoi(argv[CLLoop+1]);
    }
    if ((argc > CLLoop) && (HDstrcmp(argv[CLLoop],"-summary")==0) ||
        (HDstrcmp(argv[CLLoop],"-s")==0)) {
      Summary = 1;
    }
    if ((argc > CLLoop) && (HDstrcmp(argv[CLLoop],"-help")==0)) {
      printf("Usage: testhdf [-v[erbose] (l[ow]|m[edium]|h[igh]|0-10)] \n");
      printf("               [-[e]x[clude] name+] \n");
      printf("               [-o[nly] name+] \n");
      printf("               [-b[egin] name] \n");
      printf("               [-s[ummary]]  \n");
      printf("               [-c[leanoff]]  \n");
      printf("\n\n");
      printf("verbose   controls the amount of information displayed\n");
      printf("exclude   to exclude tests by name\n");
      printf("only      to name tests which should be run\n");
      printf("begin     start at the name of the test givin\n");
      printf("summary   prints a summary of test results at the end\n");
      printf("cleanoff  does not delete *.hdf files after execution of tests\n");
      printf("\n\n");
      printf("This program currently tests the following: \n\n");
      printf("%16s %s\n","Name","Description");
      printf("%16s %s\n","----","-----------");
      for (Loop = 0; Loop < Index; Loop++) {
          printf("%16s %s\n",Test[Loop].Name,Test[Loop].Description);
      }
      printf("\n\n");
      exit(0);
    }
    if ((argc > CLLoop) && (HDstrcmp(argv[CLLoop],"-cleanoff")==0) ||
        (HDstrcmp(argv[CLLoop],"-c")==0)) {
      CleanUp = 0;
    }
    if ((argc > CLLoop+1) && (HDstrcmp(argv[CLLoop],"-exclude") ==0) ||
        (HDstrcmp(argv[CLLoop],"-x")==0)) {
      Loop = CLLoop+1;
      while ((Loop < argc) && (argv[Loop][0] != '-')) {
        for (Loop1 = 0; Loop1 < Index; Loop1++) {
          if (HDstrcmp(argv[Loop],Test[Loop1].Name) == 0)  
              Test[Loop1].SkipFlag = 1;
        }
        Loop++;
      }
    }
    if ((argc > CLLoop+1) && (HDstrcmp(argv[CLLoop],"-begin") ==0) ||
        (HDstrcmp(argv[CLLoop],"-b")==0)) {
      Loop = CLLoop+1;
      while ((Loop < argc) && (argv[Loop][0] != '-')) {
        for (Loop1 = 0; Loop1 < Index; Loop1++) {
          if (HDstrcmp(argv[Loop],Test[Loop1].Name) != 0)  
              Test[Loop1].SkipFlag = 1;
          if (HDstrcmp(argv[Loop],Test[Loop1].Name) == 0)
            Loop1 = Index;
        }
        Loop++;
      }
    }
    if ((argc > CLLoop+1) && (HDstrcmp(argv[CLLoop],"-only") ==0) ||
        (HDstrcmp(argv[CLLoop],"-o")==0)) {
      for (Loop = 0; Loop < Index; Loop++) {
        Test[Loop].SkipFlag = 1;
      }
      Loop = CLLoop+1;
      while ((Loop < argc) && (argv[Loop][0] != '-')) {
        for (Loop1 = 0; Loop1 < Index; Loop1++) {
          if (HDstrcmp(argv[Loop],Test[Loop1].Name) == 0)
              Test[Loop1].SkipFlag = 0;
        }
        Loop++;
      }
    }
  }


  /*  printf("The Verbocity is %d \n",Verbocity); */

  for (Loop = 0; Loop < Index; Loop++) {
    if (Test[Loop].SkipFlag) {
      MESSAGE(4,printf("Skipping -- %s \n",Test[Loop].Description););
    }
    else {
      MESSAGE(2,printf("Testing  -- %s (%s) \n",
                       Test[Loop].Description,Test[Loop].Name););
      MESSAGE(5,printf("===============================================\n"););
      Test[Loop].NumErrors = num_errs;
      (*Test[Loop].Call)();
      Test[Loop].NumErrors = num_errs - Test[Loop].NumErrors;
      MESSAGE(5,printf("===============================================\n"););
      MESSAGE(5,printf("There were %d errors detected.\n\n",(int)Test[Loop].NumErrors););
    }
  }

  MESSAGE(2,printf("\n\n");)
  if(num_errs) {
    printf("!!! %d Error(s) were detected !!!\n\n", (int)num_errs);
  } else {
    printf("All tests were successful. \n\n");
  }


  if(Summary) {
/*    printf("\n\n"); */
/*    printf("==========================================================\n");*/
    printf("Summary of Test Results:\n");
/*    printf("==========================================================\n\n");*/
    printf("Name of Test     Errors Description of Test\n");
    printf("---------------- ------ --------------------------------------\n");

    for (Loop = 0; Loop < Index; Loop++) {
      if (Test[Loop].NumErrors == -1)
        printf("%16s %6s %s\n",Test[Loop].Name,"N/A",
               Test[Loop].Description);
      else
        printf("%16s %6d %s\n",Test[Loop].Name,(int)Test[Loop].NumErrors,
               Test[Loop].Description);
    }
    printf("\n\n");
/*    printf("==========================================================\n\n");
*/
  }

  if(CleanUp) {
    MESSAGE(2,printf("\nCleaning Up...\n\n"););
#ifdef PC
    remove("*.hdf");
#else
    system("rm -f *.hdf *.tmp");
#endif
  }
#ifdef TEST_PC
    fclose(dbg_file);
#endif
  return(0);
}


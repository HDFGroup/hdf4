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

#define HDP_MASTER
#include "hdp.h"

/* Print the usage message about this utility */
void 
usage(intn argc, char *argv[])
{
    printf("%s, version: 1.0, April 6, 1994\n\n", argv[0]);
    printf("Usage: hdp [-H] command [command options] <filename list>\n");
    printf("\t-H  Display usage information about the specified command.\n");
    printf("\t     If no command is specified, -H lists all commands.\n");
    printf("\t Commands:\n");
    printf("\t     list \tlists contents of files in <filename list>\n");
    printf("\t     dumpsds\tdisplays data of NDGs and SDGs in files listed in <filename list>\n");
    printf("\t     dumpvd\tdisplays data of vdatas in <filename list>. \n");
    printf("\t     dumpvg\tdisplays data of vgroups in <filename list>. \n");
    printf("\t     dumprig\tdisplays data of RIGs in <filename list>. \n");
}

int 
main(int argc, char *argv[])
{
    command_t   cmd;			/* command to perform */
    intn        curr_arg;		/* current cmd line argument */
    dump_opt_t  glob_opts;		/* global options for all commands */
    intn        j;				/* local counting variables */

    HDmemset(&glob_opts, 0, sizeof(dump_opt_t));

    if (argc < 2)
      {
          usage(argc, argv);
          exit(1);
      }		/* end if */

    curr_arg = 1;
/*  printf("Argument 0: %s\n",argv[0]);
    printf("Argument 1: %s\n",argv[1]);
    */
    while (curr_arg < argc && (argv[curr_arg][0] == '-'))
      {
          printf("\n");
          printf("Here executed!!\n");
          printf("\n");
              /*  while(curr_arg<argc && (argv[curr_arg][0]=='-' || argv[curr_arg][0]=='/')) {  */
          switch (argv[curr_arg][1])
            {
            case 'H':
                    /*     case 'h':  *//*    Print help for a given command */
                if (curr_arg < argc - 1)
                  {
                      glob_opts.help = TRUE;	/* for displaying options. */
                      break;
                  }
            default:
                usage(argc, argv);	/* Display the general usage. */
                exit(1);
            }	/* end switch */
          curr_arg++;
      }		/* end while */
    for (j = 0, cmd = HELP; j < (sizeof(commands) / sizeof(const char *)); j++, cmd++)
      {
          if (HDstrcmp(argv[curr_arg], commands[j]) == 0)
              break;
      }		/* end for */
/* printf("cmd=%d\n",(int)cmd);
   printf("command=%s\n",argv[curr_arg]);
   */
    curr_arg++;

	/* must be a legit command */
    switch (cmd)
      {
      case LIST:
          do_list(curr_arg, argc, argv, &glob_opts);
          break;

      case DUMPSDS:
          do_dumpsds(curr_arg, argc, argv, &glob_opts);
          break;

      case DUMPRIG:
          do_dumprig(curr_arg, argc, argv, &glob_opts);
          break;

      case DUMPVG:
          do_dumpvg(curr_arg, argc, argv, &glob_opts);
          break;

      case DUMPVD:
          do_dumpvd(curr_arg, argc, argv, &glob_opts);
          break;

      case HELP:
      case NONE:
          usage(argc, argv);
          break;

      default:
          printf("Invalid command!, cmd=%d\n", (int) cmd);
          exit(1);
          break;
      }		/* end switch */

    return (0);
}

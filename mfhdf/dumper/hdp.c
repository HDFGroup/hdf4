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
void usage(intn argc, char *argv[])
{
    printf("%s, version: 1.0, April 6, 1994\n\n",argv[0]);
    printf("Usage: hdp [-H] command [command options] <filename list>\n");
    printf("\t-H  Display usage information about the specified command.\n");
    printf("\t     If no command is specified, -H lists all commands.\n");
    printf("\t Commands:\n");
    printf("\t     list \tlists contents of files in <filename list>\n");
    printf("\t     dumpsds\tdisplays data of NDGs and SDGs in files listed in <filename list>\n");
    printf("\t     dumpvs\tdisplays data of vdatas in <filename list>. Not implemented yet.\n");
    printf("\t     dumpvg\tdisplays data of vgroups in <filename list>. Not implemented yet.\n");
    printf("\t     dumpris\tdisplays data of RIGs in <filename list>. Not implemented yet.\n");
} 

int main(int argc, char *argv[])
{
	command_t cmd;				/* command to perform */
	intn curr_arg;				/* current cmd line argument */
	dump_opt_t glob_opts;		/* global options for all commands */
	intn j;					/* local counting variables */

	HDmemset(&glob_opts,0,sizeof(dump_opt_t));

	if(argc<2) {
		usage(argc,argv);
		exit(1);
	  }	/* end if */

	curr_arg=1;
    while(curr_arg<argc && (argv[curr_arg][0]=='-' || argv[curr_arg][0]=='/')) {
        switch(argv[curr_arg][1]) {
			case 'H':
			case 'h':	/* Print help for a given command */
                                if (curr_arg < argc-1)  {
				    glob_opts.help=TRUE;
				    break;
				}
           default:    
       		usage(argc,argv);
       		exit(1);
       	}	/* end switch */
        curr_arg++;
      }	/* end while */
	for(j=0, cmd=HELP; j<(sizeof(commands)/sizeof(const char *)); j++,cmd++) {
		if(HDstrcmp(argv[curr_arg],commands[j])==0)
			break;
	  }	/* end for */
/* printf("cmd=%d\n",(int)cmd);
printf("command=%s\n",argv[curr_arg]);
*/
    curr_arg++;

	/* must be a legit command */
	switch(cmd) {	
		case LIST:
			do_list(curr_arg,argc,argv,&glob_opts);
			break;

		case DUMPSDS:
			do_dumpsds(curr_arg,argc,argv,&glob_opts);
			break;

		case DUMPRIG:
/*			do_dumprig(curr_arg,argc,argv,&glob_opts);
*/
                        printf("Not implemented \n");
			break;

		case DUMPVG:
/*			do_dumpvg(curr_arg,argc,argv,&glob_opts);
*/
                        printf("Not implemented \n");
			break;

		case DUMPVD:
/*			do_dumpvd(curr_arg,argc,argv,&glob_opts);
*/
                        printf("Not implemented \n");
			break;

		case HELP:
		case NONE:
			usage(argc,argv);
			break;

		default:
			printf("Invalid command!, cmd=%d\n",(int)cmd);
			exit(1);
			break;
	  }	/* end switch */
    
#ifdef QAK
    desc_buf=HDgetspace(sizeof(dd_t)*MAXBUFF);

    while(i < argc) {
        file_name = argv[i];
        fid = Hopen(file_name, DFACC_READ, -1);
        printf( "%s:  ", argv[i]);
        if(fid == FAIL) {
            if(HEvalue(1) == DFE_NOTDFFILE) {
                printf("\n\tNot an HDF file.\n");
                i++;
                continue;
            } else {
                printf("\n");
                fflush(stdout);
                HEprint(stderr, 0);
            }
        }
        
		aid = Hstartread(fid, DFTAG_WILDCARD, DFREF_WILDCARD);
		if(aid == FAIL) {
            HEprint(stderr, 0);
            i++;
            continue;	  
		}
        
		status = SUCCEED;
		for(n = 0; (n < MAXBUFF) && (status != FAIL); n++) {
            Hinquire(aid, NULL, &desc_buf[n].tag, &desc_buf[n].ref, &desc_buf[n].length,
                     &desc_buf[n].offset, NULL, NULL, NULL);
            status = Hnextread(aid, DFTAG_WILDCARD, DFREF_WILDCARD, DF_CURRENT);
		}
        
		if(n == MAXBUFF) 
            fprintf(stderr, 
                    "Warning:  File may have more DD's than hdfls can display\n");
        
		if(debug) {
            printf("\n");
            for (j=0; j<n; j++) {
                printf("%6d) tag %6d ref %6d ", j, desc_buf[j].tag, desc_buf[j].ref);
                printf(" offset %10ld length %10ld\n", desc_buf[j].offset, desc_buf[j].length);
            }
		}
		if (sort) 
			qsort( (char *)desc_buf, n, sizeof(dd_t), compare);
	
		v_init_done=FALSE;
		lprint(fid, desc_buf, n);
		if(v_init_done==TRUE)
			Vfinish(fid);
        
		if(Hendaccess(aid) == FAIL) 
            HEprint(stderr, 0);
        
        if (Hclose(fid) == FAIL) 
            HEprint(stderr, 0);
        
        i++;
        printf("\n");
    }

    HDfreespace(desc_buf);
#endif

    return(0);
}


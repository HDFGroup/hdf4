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


#include "hdp.h"
#ifndef MIPSEL
#include <math.h>
#endif /* MIPSEL */

extern void sort(int32 chosen[MAXCHOICES]);

static intn dvd(dump_info_t *dumpvd_opts, intn curr_arg, intn argc, 
		char *argv[], char *flds_chosen[MAXCHOICES], 
		int dumpallfields);

int32 dumpvd(int32 vd_id, int data_only, FILE *fp, char sep[2], 
		   int32 flds_indices[MAXCHOICES], int dumpallfields);

int32 VSref_index(int32 file_id, int32 vd_ref);

int32 VSstr_index(int32 file_id, char filter_str[MAXNAMELEN],
		  int name, int32 *find_ref, int32 *index);

int32 charcount(char *str, char onechar)
{
   int i, j, stop=0, count=0;

   j = HDstrlen(str);
   for (i=0; i<j; i++)
      if (str[i]==onechar)
	 count++;
   return(count);
}

void dumpvd_usage(intn argc,char *argv[])
{
	printf("Usage:\n");
	printf("%s dumpvd [-a|-i <index>|-n <name>|-r <ref>] [-dhv] [-o <filename> [-bx]] <filelist>\n",argv[0]);
	printf("\t-a\tDump all VDs in the file (default)\n");
	printf("\t-i <index>\tDump the <index>th SDS in the file \n");
	printf("\t-n <name>\tDump the VDS with name <name>\n");
	printf("\t-r <ref>\tDump the VDS with reference number <ref>\n");
	printf("\t-d\tDump data only, no tag/ref, formatted to input to hp2hdf\n");
	printf("\t-h\tDump header only, no annotation for elements nor data\n");
	printf("\t-v\tDump everything including all annotations (default)\n");
	printf("\t-o <filename>\tOutput to file <filename>\n");
	printf("\t-b\tBinary format of output\n");
	printf("\t-x\tAscii text format of output (default)\n");
}	/* end list_usage() */

void init_dumpvd_opts(dump_info_t *dumpvd_opts)
{
	dumpvd_opts->filter=DALL;		/* default dump all SDSs */
	/* dumpvd_opts->filter_num=FALSE; */      	/* not by reference nor by index */
	/* HDstrcpy(dumpvd_opts->filter_str, "\0"); */     /* not by name */
	dumpvd_opts->contents=DVERBOSE;	/* default dump all information */
	dumpvd_opts->dump_to_file=FALSE;	/* don't dump to output file*/
	dumpvd_opts->file_type=DASCII;		/* default output is ASCII file */
	HDstrcpy(dumpvd_opts->file_name, "\0");
}	/* end init_list_opts() */


intn parse_dumpvd_opts(dump_info_t *dumpvd_opts,intn *curr_arg,intn argc,
		       char *argv[], char *flds_chosen[MAXCHOICES], 
		       int *dumpallfields)
{
   int32 i, k, lastItem, number=1, count;
   char *tempPtr, *ptr, *string;

        if (*curr_arg >= argc) return(FAIL); 
	while ((*curr_arg < argc) && 
              (argv[*curr_arg][0]=='-')) { 
	    switch(argv[*curr_arg][1]) {
		case 'a':	/* dump all, default */
                    dumpvd_opts->filter = DALL;
                    (*curr_arg)++;
	 	    break;

		case 'i':	/* dump by index */
		    dumpvd_opts->filter=DINDEX;	
                    (*curr_arg)++;
		    string = (char*)HDmalloc(sizeof(char)*MAXNAMELEN);	
		    ptr = argv[*curr_arg];
		    dumpvd_opts->filter_num = (int*)HDmalloc(sizeof(int)*
					      (charcount(ptr, ',')+1));
		    lastItem = 0;
		    /* Extract each index from the index list and store it 
		       as an element into the "filter_num" array. */
		    for (i=0; !lastItem; i++) {
		       tempPtr = HDstrchr(ptr, ',');
		       if (tempPtr == NULL)
			  lastItem = 1;
		       else
			  *tempPtr = '\0';
		       HDstrcpy(string, ptr);
		       dumpvd_opts->filter_num[i] = atoi(string);
		       ptr = tempPtr + 1;
		    }
		    dumpvd_opts->filter_num[i] = 0;
                    (*curr_arg)++;
		    break;

                case 'r':       /* dump by reference */
                    dumpvd_opts->filter=DREFNUM;
		    (*curr_arg)++;
		    string = (char*)HDmalloc(sizeof(char)*MAXNAMELEN);	
                    ptr = argv[*curr_arg];
		    dumpvd_opts->filter_num = (int*)HDmalloc(sizeof(int)*
					      (charcount(ptr, ',')+1));
                    lastItem = 0;
                    for (i=0; !lastItem; i++) {
                       tempPtr = HDstrchr(ptr, ',');
                       if (tempPtr == NULL)
                          lastItem = 1;
                       else
                          *tempPtr = '\0';
		       string = (char*)HDmalloc(sizeof(char)*MAXNAMELEN);	
                       HDstrcpy(string, ptr);
                       dumpvd_opts->filter_num[i] = atoi(string);
                       ptr = tempPtr + 1;
		       /*printf("dumpvd_opt:%d\n", dumpvd_opts->filter_num[i]);
                        */ 
		    }
		    dumpvd_opts->filter_num[i] = 0;
                    (*curr_arg)++;
                    break;

		case 'n':	/* dump by names */
		    dumpvd_opts->filter=DNAME;	/* dump by name */
                    (*curr_arg)++;
		    lastItem = 0;
		    ptr = argv[*curr_arg];
		    for (i=0; !lastItem; i++) {
		       tempPtr = HDstrchr(ptr,',');
		       if (tempPtr == NULL)
			  lastItem = 1;
		       else 
			  *tempPtr = '\0';
		       string = (char*)HDmalloc(sizeof(char)*MAXNAMELEN);	
		       HDstrcpy(string, ptr);
		       dumpvd_opts->filter_str[i] = string;
		       ptr = tempPtr +1;
		    }
		    dumpvd_opts->filter_str[i] = NULL;
                    (*curr_arg)++;
		    break;

		case 'c':	/* dump by class */
		    dumpvd_opts->filter=DCLASS;
		    (*curr_arg)++;
		    lastItem = 0;
		    ptr = argv[*curr_arg];
		    for (i=0; !lastItem; i++) {
		       tempPtr = HDstrchr(ptr,',');
		       if (tempPtr == NULL)
			  lastItem = 1;
		       else
			  *tempPtr = '\0';
		       string = (char*)HDmalloc(sizeof(char)*MAXNAMELEN);
		       HDstrcpy(string,ptr);
		       dumpvd_opts->filter_str[i] = string;
		       ptr = tempPtr + 1;
		    }
		    dumpvd_opts->filter_str[i] = NULL;
		    (*curr_arg)++;
		    break;
		
		case 'f':	/* dump a subset of the fields */
		 if (!HDstrcmp(argv[1],"dumpvd")) {      
		    if (dumpvd_opts->filter==DALL)
		       dumpvd_opts->filter=DFIELDS; 
		    *dumpallfields = 0;
		    (*curr_arg)++;
		    lastItem = 0;
		    ptr = argv[*curr_arg];
		    for (i=0; !lastItem; i++) {
		       tempPtr = HDstrchr(ptr, ',');
		       if (tempPtr == NULL)
			  lastItem = 1;
		       else
			  *tempPtr = '\0';
		       string = (char*)HDmalloc(sizeof(char)*MAXNAMELEN); 
		       HDstrcpy(string, ptr); 
		       flds_chosen[i] = string;
		       ptr = tempPtr + 1;
		    }
		    flds_chosen[i] = NULL; 
		    (*curr_arg)++;
		 }
		 else {
		    printf("Warning: Invalid %s option %s\n", 
                           argv[1], argv[*curr_arg]);
		    return(FAIL);
		 }
		 break;

		case 'd':	/* dump data only */
                    dumpvd_opts->contents = DDATA;
                    (*curr_arg)++;
		    break;

		case 'h':	/* no annotations nor data */
                    dumpvd_opts->contents = DHEADER;
                    (*curr_arg)++;
		    break;

		case 'v':	/* dump all info */
                    dumpvd_opts->contents = DVERBOSE;
                    (*curr_arg)++;
                    break;

		case 'o':	/* specify output file */
                    dumpvd_opts->dump_to_file = TRUE; /* get filename */
                    HDstrcpy(dumpvd_opts->file_name, argv[++(*curr_arg)]);
                    if (++(*curr_arg)<argc)  { /* binary or ascii */
                        if (argv[*curr_arg][0] == '-') 
                            dumpvd_opts->file_type = (argv[*curr_arg][1] == 'b')?
                                     DBINARY : DASCII;
                        else (*curr_arg)--; 
                    }
                    (*curr_arg)++;
	            break;

		default:	/* invalid dumpvd option */
		    printf("Warning: Invalid dumpvd option %s\n", 
                             argv[*curr_arg]);
                    return(FAIL);
	  }	/* end switch */
      }	/* end while */
      return(SUCCEED);
}	/* end parse_dumpvd_opts */

void do_dumpvd(intn curr_arg,intn argc,char *argv[],dump_opt_t *glob_opts)
{
    dump_info_t dumpvd_opts;	/* dumpvd options */
    intn ret;
    char *flds_chosen[MAXCHOICES];
    int dumpallfields, i;

    flds_chosen[0] = NULL;
    dumpallfields = 1;
    if(glob_opts->help==TRUE) {
	dumpvd_usage(argc,argv);
	return;
    }	/* end if */
    init_dumpvd_opts(&dumpvd_opts);
    if(parse_dumpvd_opts(&dumpvd_opts,&curr_arg,argc,argv,flds_chosen,&dumpallfields)==FAIL) {
	dumpvd_usage(argc,argv);
	return;
    }	/* end if */
    switch (dumpvd_opts.filter)  {
        case DINDEX : 
            ret = dvd(&dumpvd_opts, curr_arg, argc, argv, flds_chosen, dumpallfields);
            if (ret == FAIL) {
                printf("Failure in dump %ith VD \n", dumpvd_opts.filter_num);
                exit(1);
            }
            break;
        case DREFNUM :
            ret = dvd(&dumpvd_opts, curr_arg, argc, argv, flds_chosen, dumpallfields);
            if (ret == FAIL) {
                printf("Failure in dump ref=%i VD \n", dumpvd_opts.filter_num);
                exit(1);
            }
            break;
        case DNAME :
            ret = dvd(&dumpvd_opts, curr_arg, argc, argv, flds_chosen, dumpallfields);
            if (ret == FAIL) {
                printf("Failure in dump name=%s VD \n", dumpvd_opts.filter_str);
                exit(1);
            }
            break;
	case DCLASS :
	    ret = dvd(&dumpvd_opts, curr_arg, argc, argv, flds_chosen, dumpallfields);
	    if (ret == FAIL) {
		printf("Failure in dump name=%s VD \n", dumpvd_opts.filter_str);
	        exit(1);
	    }
	    break;
        default:
            ret = dvd(&dumpvd_opts, curr_arg, argc, argv, flds_chosen, dumpallfields);
            if (ret == FAIL) {
                printf("Failure in dump all VDs \n");
                exit(1);
            }
            break;
        }  /* switch of filter  */
    return; 
}	/* end do_dumpvd() */


/* --------------------------------------------------------- 
*/
static int32 cn = 0;


int32 fmtbyte(unsigned char *x, FILE *ofp);
int32 fmtint8(VOIDP x, FILE *ofp);
int32 fmtuint8(VOIDP x, FILE *ofp);
int32 fmtint16(VOIDP x, FILE *ofp);
int32 fmtuint16(VOIDP x, FILE *ofp);
int32 fmtchar(VOIDP x, FILE *ofp);
int32 fmtuchar8(VOIDP x, FILE *ofp);
int32 fmtint(VOIDP x, FILE *ofp);
int32 fmtfloat32(VOIDP x, FILE *ofp);
int32 fmtint32(VOIDP x, FILE *ofp);
int32 fmtuint32(VOIDP x, FILE *ofp);
int32 fmtshort(VOIDP x, FILE *ofp);
int32 fmtfloat64(char *x, FILE *ofp);

#define BUFFER 1000000

int choose_vd(dump_info_t *dumpvd_opts, int32 vd_chosen[MAXCHOICES], 
	      int32 file_id, int *index_error)
{
   int32 i, k=0, index, find_ref, number;

   switch (dumpvd_opts->filter) {  /* Determine which VDs have been chosen. */
      case DINDEX:
         for (i=0; dumpvd_opts->filter_num[i]!=0; i++)  {
            vd_chosen[i] = dumpvd_opts->filter_num[i];
	    vd_chosen[i]--;
	 }
         break;
      case DREFNUM:
         for (i=0; dumpvd_opts->filter_num[i]!=0; i++) {
            index = VSref_index(file_id, dumpvd_opts->filter_num[i]);
	    if (index==-1) {
	       printf("Vdata with reference number %d: not found\n", 
		      dumpvd_opts->filter_num[i]); 
	       *index_error = 1;
	    }
	    else {
               vd_chosen[k] = index;
	       k++;
            }
         } /* for */
/*	 for (i=0; vd_chosen[i]!=-1; i++)
	    printf("%d\n", vd_chosen[i]);
*/
         break;
      case DNAME: 
         for (i=0; dumpvd_opts->filter_str[i]!=NULL; i++) {
	    find_ref = -1;
	    number = 0;
            Vstart(file_id); 
	    index = VSstr_index(file_id, dumpvd_opts->filter_str[i], 1, 
				&find_ref, &number);
            Vend(file_id);
	    if (index==-1) {
	       printf("Vdata with name %s: not found\n", 
		      dumpvd_opts->filter_str[i]);
	       *index_error = 1;
	    }
	    else  {
               vd_chosen[k] = index;
	       k++;
            }
         } /* for */  
         break;
      case DCLASS:
         for (i=0; dumpvd_opts->filter_str[i]!=NULL; i++) {
	    int32 found=0;
	    find_ref = -1;
	    number = 0;
	    Vstart(file_id);
	    while((index=VSstr_index(file_id, dumpvd_opts->filter_str[i], 0,
				     &find_ref, &number)) !=-1) {
               vd_chosen[k] = index;
	       found = 1;
	       k++;
            }
            if (!found) {
	       printf("Vdata with class %s: not found\n",
		      dumpvd_opts->filter_str[i]);
	       *index_error =1;
	    }
	    Vend(file_id);
         } /* for */
         break;
      case DFIELDS: /* Don't have to worry about which chosen fields yet. */
         break;
      case DALL:
         break;
      default:
         printf("Unknown filter option\n");
         exit(1);
   } /* switch */
} /* choose_vd */



static intn dvd(dump_info_t *dumpvd_opts, intn curr_arg, 
                intn argc, char *argv[], char *flds_chosen[MAXCHOICES], 
		int dumpallfields)  
{
    int32 file_id, vd_id, vd_chosen[MAXCHOICES], flds_indices[MAXCHOICES];
    int32 i, j, k, m, x;
    int32 nvf,interlace,vsize, index, len, lastItem;
    int32 vdata_ref=-1, vdata_tag, status;
    char *name, *label_str;
    char vdclass[VSNAMELENMAX],vdname[VSNAMELENMAX];
    char fields[FIELDNAMELENMAX], tempflds[FIELDNAMELENMAX];
    char *tempPtr, *ptr, string[MAXNAMELEN];
    char file_name[MAXFNLEN]; 
    FILE  *fp;
    int index_error=0, dumpall=0, data_only;

    while (curr_arg < argc) {  /* Loop until all specified files have been 
				  processed */
        HDstrcpy(file_name, argv[curr_arg]);
        curr_arg++;
        file_id = Hopen(file_name, DFACC_READ, 0);
        if (file_id == FAIL) {
           printf("Failure in open %s\n", file_name);
           exit(1);
        }
       
	for (i=0; i<MAXCHOICES; i++)
	   vd_chosen[i] = -1;

        /* Find out which VDs have been chosen. */
        choose_vd(dumpvd_opts, vd_chosen, file_id, &index_error);
/*
        for (i=0; i<5; i++)
	    printf("*** %d\n", vd_chosen[i]);
*/
	/* In case of index error, skip the current file. */
	if (index_error)
	   continue;
        /* Get output file name.  */
        if (dumpvd_opts->dump_to_file) 
	   fp = fopen(dumpvd_opts->file_name, "w");
        else 
	   fp = stdout;
        if (dumpvd_opts->contents!=DDATA)
           fprintf(fp, "File name: %s \n\n", file_name);
	Vstart(file_id);
	x = 0; /* "x" is used to index the array of "vd_chosen". */

	/* Determine if all VDs are to be dumped out. */
	if (vd_chosen[x]==-1) /* If so, set the corresponding flag. */
	   dumpall = 1;
	else { /* Otherwise, sort the indices of the chosen VDs in increasing 
	   	  order so that they will be dumped out in such order. */
	   sort(vd_chosen); 
/*
           int32 i, j, temp;
	   for (i=0; vd_chosen[i]!=-1; i++)
	      for (j=i; vd_chosen[j]!=-1; j++)
		 if (vd_chosen[i]>vd_chosen[j]) {
		    temp = vd_chosen[i];
		    vd_chosen[i] = vd_chosen[j];
		    vd_chosen[j] = temp;
                 }
*/
        }
/*
	for (i=0; vd_chosen[i]!=-1; i++)
	   printf("vd_chosen[%d]=%d\n", i, vd_chosen[i]);
*/
	/* Examine each VD. */
	for (i=0; (vdata_ref=VSgetid(file_id,vdata_ref)) != -1; i++) { 
	   int data_only, flds_match=0;
	   char sep[2];
           
	   /* Only dump the info of the chosen VDs or all of the VDs if none
	      has been selected. */
	   if ((!dumpall) && (i!=vd_chosen[x])) 
	      continue;
           
	   x++; /* One vdata has been located; so go to the next one in 
		   the array. */
           vd_id = VSattach(file_id,vdata_ref,"r");
	   if (vd_id == FAIL) {
	      printf("Cannot open vd id=%d\n", (int)vdata_ref);
	      continue;
           }

	   VSinquire(vd_id,&nvf,&interlace,fields, &vsize,vdname);
	   vdata_tag = VSQuerytag(vd_id);
	   if (HDstrlen(vdname) == 0)
	      HDstrcat(vdname,"");
           VSgetclass(vd_id,vdclass);

	   /* If one or more fields were specified by the user, then find out
	      what they were, determine their corresponding indices in 
	      "fields", and store these indices in the array "flds_indices" so
	      that they can be used to determine whether a field should be
	      dumped later on. */
           if (flds_chosen[0]!=NULL) { 
	      HDstrcpy(tempflds, fields);
	      ptr = tempflds;
	      for (j=0; j<MAXCHOICES; j++) /* Initialization */
	         flds_indices[j] = -1;
	      m = 0;
	      lastItem = 0;
	      /* Extract each field name from the list of fields of the 
		 current record. */
	      for (j=0; !lastItem; j++) {
	         tempPtr = HDstrchr(ptr, ',');
	         if (tempPtr == NULL)
		    lastItem = 1;
	         else
		    *tempPtr = '\0';
	         HDstrcpy(string, ptr);
	         ptr = tempPtr + 1;
		 /* Compare the extracted field name with each of the names
		    of the fields having been chosen. */
	         for (k=0; flds_chosen[k]!=NULL; k++) {
		    if (!HDstrcmp(flds_chosen[k], string)) {
		       flds_indices[m] = j;
		       m++;
		       flds_match = 1; 
		    }
	         } /* for (k...) */
	      }  /* for (j...) */
           } /* if  */

	   /* If no fields were chosen, all fields are to be dumped out, and
	      so all fileds match. */
	   if (flds_chosen[0]==NULL)
	      flds_match = 1;
           if (flds_match) {
	      int32 z, lastItem, count=0;
	      switch (dumpvd_opts->contents) {
	         case DVERBOSE: /* dump all information */
                    fprintf(fp, "Vdata: %d\n", (int) i);
		    fprintf(fp, "   tag = %d; ", (int) vdata_tag);
		    fprintf(fp, "reference = %d;\n", (int) vdata_ref);
		    fprintf(fp, "   number of records = %d; ",
		            (int) nvf); 
		    fprintf(fp, "interlace = %d;\n", (int) interlace);
		    fprintf(fp, "   fields = [");
		    
                    /* The list of field names can be very long and would 
		       look very messy when being displayed if it were to
		       be dumped out at once. The following block of 
		       operations is to display a list in a nice way even
		       if the list is long. */
		    lastItem = 0;
		    HDstrcpy(tempflds, fields);
		    ptr = tempflds;
		    for (z=0; !lastItem; z++) {
		       tempPtr = HDstrchr(ptr, ',');
		       if (tempPtr == NULL)
			  lastItem = 1;
                       else
			  *tempPtr = '\0';
		       HDstrcpy(string, ptr);
		       count += HDstrlen(string);
		       if (count>50) {
			  fprintf(fp, "\n               ");
			  count = 0;
		       }
		       fprintf(fp, "%s", string);
		       if (!lastItem)
			  fprintf(fp, ", ");
		       ptr = tempPtr + 1;
   	            }
		    fprintf(fp, "];\n");
		    
		    
		    fprintf(fp, "   record size (in bytes) = %d;\n",
			    vsize);
		    fprintf(fp, "   name = %s; class = %s;\n", 
		            vdname, vdclass);
		    /* read in all of the annotations */
		    len = DFANgetdesclen(file_name,vdata_tag,vdata_ref);
		    if (len != FAIL) {
		       label_str = (char *) HDmalloc((uint32) len + 1);
		       status = DFANgetdesc(file_name,vdata_tag,vdata_ref,
					    label_str,len+1);
		       label_str[len] = '\0';
		       if (status == FAIL)
		          printf("\t  Unable to read description.\n");
                       else
		          printf("\t  Description: %s\n",label_str);
                       HDfree(label_str);
                    } /* if (len != FAIL) */
	         case DHEADER: /* header only, no annotations nor data */
		    if (dumpvd_opts->contents == DHEADER) 
		       break;
	         case DDATA: /* data only */
		    if (dumpvd_opts->contents == DDATA) {
		       data_only = 1;
		       HDstrcpy(sep, "");
		    }
		    else {
		       data_only = 0;
		       HDstrcpy(sep, ";");
		    }
		    /* Only the chosen or all fields will be dumped out. */
		    dumpvd(vd_id, data_only, fp, sep, 
			   flds_indices, dumpallfields);
	      } /* switch */
	   }
	   VSdetach(vd_id);
         } /* for */
	 Vend(file_id);
      }      /* while (curr_arg < argc)  */
      return(0);
}     /* dvd */


int32 VSref_index(int32 file_id, int32 vd_ref)
{
   uint16 find_tag = 0;
   int32 find_ref = -1;
   int32 find_offset;
   int32 find_length;
   int32 search_file;
   int index = 0;
   int found = 0;

   Vstart(file_id);
   while (((find_ref=VSgetid(file_id,find_ref))!=-1) && (!found)) {
      if (find_ref == vd_ref)
	 found = 1;
      else
	 index++;
   }
   Vend(file_id);
   if (found)
      return(index);
   else
      return(-1);
} /* VSref_index */


int32 VSstr_index (int32 file_id, char filter_str[MAXNAMELEN], 
		   int name, int32 *find_ref, int32 *index)
{
   int found = 0;
   int32 vdata_id;
   char vdata_name[MAXNAMELEN];

   while ((!found) && ((*find_ref=VSgetid(file_id,*find_ref)) !=-1)) {
      vdata_id = VSattach(file_id, *find_ref, "r");
      if (name)
	 VSgetname(vdata_id, vdata_name);
      else 
	 VSgetclass(vdata_id, vdata_name);
      if (HDstrcmp(vdata_name, filter_str) == 0)
         found = 1;
      (*index)++;
      VSdetach(vdata_id);
   }
   if (found)
      return((*index)-1);
   else
      return(-1);
}


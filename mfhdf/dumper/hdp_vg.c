/***************************************************************************e
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
#include <math.h>
#include "vg.h"

#define NUM_VGS 20;

FILE *fopen();

struct node {
   int32 index,displayed;
   char name[MAXNAMELEN];
   char **children, **type;
}; 

void display (struct node *ptr, int32 level, struct node **list, 
	      int32 num_nodes, int32 firstchild);

extern void sort(int32 chosen[MAXCHOICES]);

static intn dvg(dump_info_t *dumpvg_opts, intn curr_arg, intn argc, 
		char *argv[]);

int32 Vref_index(int32 file_id, int32 vg_ref);

void vgdumpfull(int32 vg_id, int32 file_id, FILE *fp, struct node *aNode, 
		int32 skip);

int32 Vstr_index(int32 file_id, char filter_str[MAXNAMELEN], int name,
		 int32 *find_ref, int32 *index);

void dumpvg_usage(intn argc,char *argv[])
{
	printf("Usage:\n");
	printf("%s dumpvg [-a|-i <index>|-n <name>|-r <ref>] [-dhv] [-o <filename> [-bx]] <filelist>\n",argv[0]);
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

void init_dumpvg_opts(dump_info_t *dumpvg_opts)
{
	dumpvg_opts->filter=DALL;	/* default dump all SDSs */
	dumpvg_opts->filter_num=FALSE;  /* not by reference nor by index */
	dumpvg_opts->contents=DVERBOSE;	/* default dump all information */
	dumpvg_opts->dump_to_file=FALSE;/* don't dump to output file*/
	dumpvg_opts->file_type=DASCII;	/* default output is ASCII file */
	HDstrcpy(dumpvg_opts->file_name, "\0");
}	/* end init_list_opts() */


void do_dumpvg(intn curr_arg,intn argc,char *argv[],dump_opt_t *glob_opts)
{
    dump_info_t dumpvg_opts;	/* dumpvg options */
    intn ret;
    char *dummy1[MAXNAMELEN];
    int dummy2;

    if(glob_opts->help==TRUE) {
	dumpvg_usage(argc,argv);
	return;
    }	/* end if */
    init_dumpvg_opts(&dumpvg_opts);
    if(parse_dumpvd_opts(&dumpvg_opts,&curr_arg,argc,argv,dummy1,dummy2)
       ==FAIL) {
	dumpvg_usage(argc,argv);
	return;
    }	/* end if */
    switch (dumpvg_opts.filter)  {
        case DINDEX : 
            ret = dvg(&dumpvg_opts, curr_arg, argc, argv);
            if (ret == FAIL) {
                printf("Failure in dump %ith vgroup \n", dumpvg_opts.filter_num);
                exit(1);
            }
            break;
        case DREFNUM :
            ret = dvg(&dumpvg_opts, curr_arg, argc, argv);
            if (ret == FAIL) {
                printf("Failure in dump ref=%i vgroup \n", dumpvg_opts.filter_num);
                exit(1);
            }
            break;
        case DNAME :
            ret = dvg(&dumpvg_opts, curr_arg, argc, argv);
            if (ret == FAIL) {
                printf("Failure in dump name=%s vgroup \n", 
		       dumpvg_opts.filter_str);
                exit(1);
            }
            break;
        case DCLASS :
            ret = dvg(&dumpvg_opts, curr_arg, argc, argv);
            if (ret == FAIL) {
                printf("Failure in dump class=%s vgroup \n", 
		       dumpvg_opts.filter_str);
                exit(1);
            }
            break;
        default:
            ret = dvg(&dumpvg_opts, curr_arg, argc, argv);
            if (ret == FAIL) {
                printf("Failure in dump all vgroups \n");
                exit(1);
            }
            break;
        }  /* switch of filter  */
    return; 
}	/* end do_dumpvg() */


/* --------------------------------------------------------- 
* printing functions copied from vshow.c and used by vgdumpfull(). 
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


static intn dvg(dump_info_t *dumpvg_opts, intn curr_arg, 
                intn argc, char *argv[])  
{
    int32 file_id, vg_id, vg_chosen[MAXCHOICES];
    int32 i, j, k, n, x, y;
    int32 interlace,vsize, len, nvg, index, status;
    int32 vg_ref=-1, vg_tag;
    int32 level, num_nodes=0, max_vgs, num_vgs;
    int index_error=0, dumpall=0;
    char *name, *label_str;
    char file_name[MAXFNLEN]; 
    char vgclass[VGNAMELENMAX],vgname[VGNAMELENMAX],fields[FIELDNAMELENMAX];
    FILE  *fp;
    struct node **list, *ptr;

    while (curr_arg < argc)   { /* Examine each file. */
        HDstrcpy(file_name, argv[curr_arg]);
        curr_arg++;
        file_id = Hopen(file_name, DFACC_READ, 0);
        if (file_id == FAIL) {
           printf("Failure in open %s\n", file_name);
           exit(1);
        }
	
	for (i=0; i<MAXCHOICES; i++)
	   vg_chosen[i] = -1;
        k = 0;
        switch (dumpvg_opts->filter) {  /* Determine the vgroups to be dumped 
					   out. */
             int32 find_ref, number; 
	     case DINDEX:
		  for (i=0; dumpvg_opts->filter_num[i]!=0; i++) {
		     vg_chosen[i] = dumpvg_opts->filter_num[i];
		     vg_chosen[i]--;
                  } 
		  break;
             case DREFNUM:
		  for (i=0; dumpvg_opts->filter_num[i]!=0; i++) {
                     index = Vref_index(file_id, dumpvg_opts->filter_num[i]);
		     if (index==-1) {
		        printf("Vgroup with reference number %d: not found\n", 
			       dumpvg_opts->filter_num[i]); 
		        index_error = 1;
		     }
		     else {
                        vg_chosen[k] = index;
			k++;
                     }
		  }
                  break;
             case DNAME:
		  for (i=0; dumpvg_opts->filter_str[i]!=NULL; i++) {
		     find_ref =-1;
		     number = 0;
		     Vstart(file_id);
                     index = Vstr_index(file_id, dumpvg_opts->filter_str[i],
					1, &find_ref, &number);
    	             Vend(file_id);
		     if (index==-1) {
		        printf("Vgroup with name %s: not found\n", 
			       dumpvg_opts->filter_str[i]);
		        index_error = 1;
		     }
		     else {
                        vg_chosen[k] = index;
		        k++;
                     }
		  }
                  break;
	     case DCLASS:
		  for (i=0; dumpvg_opts->filter_str[i]!=NULL; i++) {
		     int32 found=0;
		     find_ref = -1;
		     number = 0;
		     Vstart(file_id);
		     while((index=Vstr_index(file_id,dumpvg_opts->filter_str[i],
					0, &find_ref, &number))!=-1) {
                        vg_chosen[k] = index;
			found = 1;
			k++;
                     }
		     if (!found) {
			printf("Vgroup with class %s: not found\n",
			       dumpvg_opts->filter_str[i]);
		        index_error = 1;
		     }
		     Vend(file_id);
		  }
		  break;
             case DALL:
                  break;
             default:
                  printf("Unknown filter option\n");
                  exit(1);
        } /* switch */

	if ((index_error) && (vg_chosen[0]==-1)) 
	   continue;
        /* Get the name of the output file. */
        if (dumpvg_opts->dump_to_file) 
	   fp = fopen(dumpvg_opts->file_name, "w");
        else 
	   fp = stdout;
        if (dumpvg_opts->contents!=DDATA)
           fprintf(fp, "File name: %s \n", file_name);
	Vstart(file_id);
	nvg = 0;
	x = 0;
	if (vg_chosen[x]==-1)
	   dumpall = 1;
        else
	   sort(vg_chosen);
       
	max_vgs = NUM_VGS;
	list = (struct node**)HDmalloc(sizeof(struct node*)*max_vgs);
	num_vgs = 0;
	for (i=0; (vg_ref=Vgetid(file_id,vg_ref))!=-1; i++) { 
	   int32 skip=FALSE;
	   content_t save=dumpvg_opts->contents;

	   if ((!dumpall) && (i!=vg_chosen[x])) {
	      skip = TRUE;
	      dumpvg_opts->contents = DDATA;
           }
           vg_id = Vattach(file_id,vg_ref,"r");
	   if (vg_id == FAIL) {
	      printf("Cannot open vg id=%d\n", (int)vg_ref);
	      continue;
           }
	   if (!skip)
	      x++;
	   Vinquire(vg_id,&n,vgname);
	   vg_tag = VQuerytag(vg_id);
	   if (HDstrlen(vgname)==0)
	      HDstrcat(vgname,"");
	   Vgetclass(vg_id, vgclass);
           num_vgs++; 
	   if (num_vgs>max_vgs) {
	      max_vgs += NUM_VGS;
	      list = HDregetspace(list,(uint32)sizeof(struct node)*max_vgs);
           }
	   list[i] = (struct node*)HDmalloc(sizeof(struct node)); 
	   num_nodes++;
	   
	   switch (dumpvg_opts->contents) {
	    int k;
	      case DVERBOSE: /* dump all information */
		 fprintf(fp, "\n");
	         fprintf(fp, "\nVgroup:%d\n", (int) i);
		 fprintf(fp, "     tag = %d; reference = %d;\n",
		         (int)vg_tag, (int)vg_ref);
		 fprintf(fp, "     name = %s; class = %s;\n", vgname, vgclass);
		 fprintf(fp, "     number of entries = %d;\n", (int) n); 
		 /* Read in all of the annotations. */
		 len = DFANgetdesclen(file_name,vg_tag,vg_ref);
		 if (len!=FAIL) {
		    label_str = (char *) HDmalloc((uint32) len + 1);
		    status = DFANgetdesc(file_name,vg_tag,vg_ref,
					 label_str,len+1);
		    label_str[len] = '\0';
		    if (status==FAIL)
		       printf("\t  Unable to read description.\n");
                    else
		       printf("\t  Description: %s\n",label_str);
                    HDfree(label_str);
                 } /* if (len != FAIL) */
	      case DHEADER: /* header only, no annotations nor data */
		 if (dumpvg_opts->contents == DHEADER) 
		    break;

	      case DDATA: /* data only */ 
		 dumpvg_opts->contents = save;
		 if ((dumpvg_opts->contents!=DDATA) && (!skip))
		    fprintf(fp, "Entries:-\n");
		 vgdumpfull(vg_id,file_id,fp, list[i], skip);
		 if (dumpvg_opts->contents==DDATA)
		    fprintf(fp, "\n");
           } /* switch */
	   Vdetach(vg_id);
	   list[i]->index = i; 
	   HDstrcpy(list[i]->name, vgname); 
	   list[i]->displayed = FALSE;
         } /* for */
	 Vend(file_id);
	 if (dumpvg_opts->contents!=DDATA) {
	    printf("\n\nGraphical representation of the file:-\n");
	    printf("(vg#: vgroup;   vd: vdata)\n\n");
	    for (y=0; y<num_nodes; y++) { 
	     int32 firstchild=FALSE, k;
	       level = -1;
	       ptr = list[y];
	       printf("   ");
	       display(ptr, level, list, num_nodes, firstchild);
	       printf("\n");
            } /* for */
         }
      }      /* while (curr_arg < argc)  */
      return(0);
}     /* dvg */


void vgdumpfull(int32 vg_id, int32 file_id, FILE *fp, struct node *aNode, 
		int32 skip)
{
   int32 vgt, vgotag, vgoref;
   int32 t, vsid, vg_tag, ne, tag;
   int found=0, num_entries;
   int32 vs, nv, vsotag, vsoref, interlace, vsize;
   char fields[FIELDNAMELENMAX], vsname[MAXNAMELEN], vsclass[VSNAMELENMAX];
   char vgname[VGNAMELENMAX], vgclass[VGNAMELENMAX];
   char *name;
   int32 z, lastItem, count=0;
   char *tempPtr, *ptr, string[MAXNAMELEN], tempflds[FIELDNAMELENMAX];

   num_entries = Vntagrefs(vg_id);
   aNode->children = (char**)HDmalloc(sizeof(char*)*num_entries);
   aNode->type = (char**)HDmalloc(sizeof(char*)*num_entries);
   
   for (t = 0; t<num_entries; t++) {
      Vgettagref(vg_id, t, &tag, &vsid);
      found = 1;
      if (tag == DFTAG_VG) {
         vgt = Vattach(file_id, vsid, "r");
         if (vgt == FAIL) {
	    printf("Cannot open vg id=&d\n", (int) vsid);
	    continue;
         } 
         Vinquire(vgt, &ne, vgname);
         if (HDstrlen(vgname) == 0)
	    HDstrcat(vgname, "NoName");
         vgotag = VQuerytag(vgt);
         vgoref = VQueryref(vgt);
         Vgetclass(vgt,vgclass);
	 if (!skip) {
            fprintf(fp, "     #%d (Vgroup)\n\ttag = %d;",
		    (int) t, (int) vgotag);
	    fprintf(fp, "reference = %d;\n\tnumber of entries = %d;\n",
	            (int)vgoref, (int)ne);
            fprintf(fp, "\tname = %s; class = %s\n", vgname, vgclass);
         } 
	 Vdetach(vgt);
	 aNode->children[t] = (char*)HDmalloc(sizeof(char)*HDstrlen(vgname));
	 HDstrcpy(aNode->children[t], vgname);
	 aNode->type[t] = (char*)HDmalloc(sizeof(char)*3);
	 HDstrcpy(aNode->type[t], "vg");
      } /* if */
      else if (tag == VSDESCTAG) {
         vs = VSattach(file_id, vsid, "r");
	 if (vs == FAIL) {
            printf("Cannot open vs id=%d\n", (int) vsid);
	    continue;
         }
	 VSinquire(vs, &nv, &interlace, fields, &vsize, vsname);
	 vsotag = VSQuerytag(vs);
	 vsoref = VSQueryref(vs);
	 if (HDstrlen(vsname) ==0)
	    HDstrcat(vsname, "NoName");
         VSgetclass(vs,vsclass);


	 if (!skip) {
	    fprintf(fp, "     #%d (Vdata)\n", (int) t);
	    fprintf(fp, "\ttag = %d; ", (int) vsotag);
	    fprintf(fp, "reference = %d; \n", (int) vsoref);
	    fprintf(fp, "\tnumber of records = %d; ", (int) nv);
	    fprintf(fp, "interlace = %d;\n", (int) interlace);
	    fprintf(fp, "\tfields = [");

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
	       if (tempPtr==NULL)
	          lastItem = 1;
               else
	          *tempPtr = '\0';
	       HDstrcpy(string, ptr);
	       count += HDstrlen(string);
	       if (count>50) {
	          fprintf(fp, "\n\t          ");
	          count = 0;
               }
	       fprintf(fp, "%s", string);
	       if (!lastItem)
	          fprintf(fp, ", ");
               ptr = tempPtr + 1;
            }
	    fprintf(fp, "];\n");
	    fprintf(fp, "\trecord size (in bytes) = %d;\n", vsize);
	    fprintf(fp, "\tname = %s; class = %s;\n", vsname, vsclass);
         }


	 VSdetach(vs);
	 aNode->children[t] = (char*)HDmalloc(sizeof(char)*HDstrlen(vsname));
	 HDstrcpy(aNode->children[t], vsname); 
	 
	 aNode->type[t] = (char*)HDmalloc(sizeof(char)*3);
	 HDstrcpy(aNode->type[t], "vd");
      }
      else {
	 name = HDgettagsname((uint16) tag);
	 if (!name)
	    name = "Unknown Tag";
         if (!skip) {
	    fprintf(fp, "     #%d (%s)\n", (int) t, name);
	    fprintf(fp, "\ttag = %d; reference = %d;\n", (int) tag, (int) vsid);
	 }
	 aNode->children[t] = (char*)HDmalloc(sizeof(char)*4);
         HDstrcpy(aNode->children[t], "***"); 
	 tempPtr = (char*)HDgettagdesc((uint16) tag);
	 if (!tempPtr) {
	    aNode->type[t] = (char*)HDmalloc(sizeof(char)*15);
	    HDstrcpy(aNode->type[t], "Unknown Object"); 
         }
	 else
	    aNode->type[t] = tempPtr + 6;
      }
   } /* for */
   aNode->children[num_entries] = NULL;
   if (!found)
      printf("     None.\n");
} /* vgdumpfull */


void display(struct node *ptr, int32 level, struct node **list, 
	     int32 num_nodes, int32 firstchild)
{
   char *name;
   int i, k, x, y, z, num=1, newline=TRUE;

   level++; 
   if (!firstchild)  
      for (k=0; k<level; k++)
         for (z=0; z<num; z++)
	    printf("\t");
   else 
      for (z=0; z<num; z++)
	 printf("\t");
   if (level>0)
      printf("-- ");
   printf("vg%d ", ptr->index);
   if (ptr->children[0]==NULL)
      printf("\n");
   if (!ptr->displayed) {
      for (i=0; ptr->children[i]!=NULL; i++) {
	 if (i==0)
	    firstchild = TRUE;
         else
	    firstchild = FALSE;
	 name = ptr->children[i];
	 if ((HDstrcmp(ptr->type[i], "vd")) && 
	     (HDstrcmp(ptr->children[i], "***"))) {
	    x = 0; 
	    while (HDstrcmp(name, list[x]->name)) 
	       x++;
	    display(list[x], level, list, num_nodes, firstchild);
         } /* if */ 
	 else { 
	    if (i>0) {
	       for (k=0; k<level+1; k++)
		  for (z=0; z<num; z++)
		     printf("\t");
            }
	    if (firstchild) 
	       for (z=0; z<num; z++)
		  printf("\t");
            printf("-- ");
	    /* printf("%s%d ", ptr->type[i], i); */
	    printf("%s  \n", ptr->type[i]);
	    newline = FALSE;
	 }
      } /* for (i...) */
      /* ptr->displayed = TRUE; */
   } /* if (!ptr->displayed) */
   else 
      printf("\n");
} /* display */


int32 Vref_index(int32 file_id, int32 vg_ref)
{
   int32 find_ref=-1;
   int32 vg_id;
   int index = 0;
   int found = 0;
  
   Vstart(file_id);
   while (((find_ref = Vgetid(file_id,find_ref))!=-1) && (!found)) {
      if (find_ref==vg_ref)
	 found = 1;
      else
	 index++;
   }
   Vend(file_id);
   if (found)
      return(index);
   else
      return(-1);
} /* Vref_index */


int32 Vstr_index (int32 file_id, char filter_str[MAXNAMELEN],
		  int name, int32 *find_ref, int32 *index)
{
   int found = 0;
   int32 vg_id;
   char vg_name[MAXNAMELEN];
   
   while ((!found) && ((*find_ref=Vgetid(file_id,*find_ref))!=-1)){
      vg_id = Vattach(file_id, *find_ref, "r");
      if (name)
         Vgetname(vg_id, vg_name);
      else
	 Vgetclass(vg_id, vg_name);
      if (HDstrcmp(vg_name, filter_str)==0)
         found = 1;
      (*index)++;
      Vdetach(vg_id);
   }
   if (found)
      return((*index)-1);
   else
      return(-1);
}


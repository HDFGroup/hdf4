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

#include "hdf.h"
#include "hfile.h"

#ifdef MAC
/* this isn't worth putting in hdfi.h */
/* otherwise, need to include stdlib.h, which */
/* in turn requires other includes - very messy, trust me */ 
void qsort(void *base, size_t nmemb, size_t size, 
        int (*compar) (const void*, const void *));
#endif

#ifdef PC
#define MAXBUFF 2048
#else
#define MAXBUFF 8192
#endif

dd_t *desc_buf;

intn
    debug    = FALSE,        /* Debugging is off by default */
    sort     = TRUE,         /* Sorting is on by default */
    longout  = FALSE,        /* short output by default */
    labels   = FALSE,        /* no label info by default */
    special  = FALSE,        /* no special info by default */
    groups   = FALSE;        /* no group info by default */
static intn v_init_done=FALSE;	/* whether the Vset interface has been */
								/* initialized for this file */
uint16  only_tag = DFTAG_NULL;  /* by default print info about all tags */

char * file_name;    /* name of current file being listed */

int compare
  (const VOID *, const VOID *);
int main
  (int, char *a[]);
void lprint
  (int32, dd_t *, int);
void print_item
  (int32 , dd_t *, intn );

int compare(const VOID * aa, const VOID * bb)
{
    const dd_t *a = (const dd_t *) aa;
    const dd_t *b = (const dd_t *) bb;
    
    if(a->tag > b->tag) return(1);
    if(a->tag < b->tag) return(-1);
    if(a->ref > b->ref) return(1);
    if(a->ref < b->ref) return(-1);
    return(0);
}

void print_item(int32 fid, dd_t *desc_list, intn n) 
{
    sp_info_block_t info;
    intn status;
    int32 len;
    char *name, *label_str;
    
    printf("\tRef no %6d\t%8ld bytes\n", desc_list[n].ref, desc_list[n].length);
    
    /* print out labels and annotations if desired */
    if(labels) { /* read in all of the labels */
        len = DFANgetlablen(file_name, desc_list[n].tag, desc_list[n].ref);
        if(len != FAIL) {
            label_str = (char *) HDgetspace((uint32) len + 1);
            status = DFANgetlabel(file_name, desc_list[n].tag, desc_list[n].ref, label_str, len + 1);
            label_str[len] = '\0';
            if(status == FAIL) 
                printf("\t  Unable to read label\n");
            else
                printf("\t  Label: %s\n", label_str);
            HDfreespace(label_str);
        }
        
        /* read in all of the annotations */
        len = DFANgetdesclen(file_name, desc_list[n].tag, desc_list[n].ref);
        if(len != FAIL) {
            label_str = (char *) HDgetspace((uint32) len + 1);
            status=DFANgetdesc(file_name,desc_list[n].tag,desc_list[n].ref,label_str,len+1);
            label_str[len] = '\0';
            if(status == FAIL) 
                printf("\t  Unable to read description\n");
            else
                printf("\t  Description: %s\n", label_str);
            HDfreespace(label_str);
        }
    }
    
    if((special) && (SPECIALTAG(desc_list[n].tag))) { /* print out special info if desired */
        int32 aid, ret; 
        
        aid = Hstartread(fid, desc_list[n].tag, desc_list[n].ref);
        ret = HDget_special_info(aid, &info);
        if((ret == FAIL) || (info.key == FAIL))
            return;
        
        switch(info.key) {
        case SPECIAL_LINKED:
            printf("\tLinked Block: first %ld standard %ld per unit %ld\n",
                   (long)info.first_len, (long)info.block_len, 
                   (long)info.nblocks);
            break;
            
        case SPECIAL_EXT:
            printf("\tExternal File: path %s  offset %ld\n", info.path, 
                   (long)info.offset);
            break;
            
        case SPECIAL_COMP:
            printf("\tCompressed Element: compression type: %s  modeling type %s\n", 
                   (info.comp_type==COMP_CODE_NONE ? "None" : 
                    (info.comp_type==COMP_CODE_RLE ? "Run-Length" : 
                     (info.comp_type==COMP_CODE_NBIT ? "N-Bit" : "Unknown" ))), 
                   (info.model_type==COMP_MODEL_STDIO ? "Standard" : "Unknown"));
            break;
            
        default:
            printf("\tDo not understand special element type %d\n", 
                   info.key);
            break;
        }
        Hendaccess(aid);
    }
    
    if((groups) && (desc_list[n].tag==DFTAG_RIG || desc_list[n].tag==DFTAG_SDG 
                    || desc_list[n].tag==DFTAG_NDG || desc_list[n].tag==DFTAG_VG)) { /* print groups */
        if(desc_list[n].tag!=DFTAG_VG) {	/* groups other than Vgroups */
            int32 GroupID;
            DFdi elmt;
            
            printf("\tContents:\n");
            if((GroupID=DFdiread(fid, desc_list[n].tag, desc_list[n].ref))>=0) {
                while(!DFdiget(GroupID,&elmt.tag,&elmt.ref)) {
                    name=(char *)HDgettagname(elmt.tag);
                    if(!name) 
                        name = "Unknown Tag";
                    printf("\t\t%-30s: (tag=%6d) ref=%d\n",name, 
                           elmt.tag,elmt.ref);
                } /* end while */
            }  /* end if */
            else
                printf("\t\tNone!\n");
        }	/* end if */
        else {	/* dump Vgroup tag/refs */
            int32 ntagrefs;
            int32 vkey;
            int32 *tag_arr, *ref_arr;
            intn i;
            
            if(v_init_done==FALSE) {	/* init the V routines */
                v_init_done=TRUE;
                Vinitialize(fid);
            }	/* end if */
            if((vkey=Vattach(fid,desc_list[n].ref,"r"))!=FAIL) {
                ntagrefs=Vntagrefs(vkey);
                printf("\tContents: %d items\n",(int)ntagrefs);
                if(ntagrefs>0) {
                    tag_arr = (int32 *) HDgetspace(sizeof(int32)*ntagrefs);
                    ref_arr = (int32 *) HDgetspace(sizeof(int32)*ntagrefs);
                    if(tag_arr==NULL || ref_arr==NULL) {
                        HDfreespace(tag_arr);
                        HDfreespace(ref_arr);
                    }	/* end if */
                    else {
                        if(Vgettagrefs(vkey,tag_arr,ref_arr,ntagrefs)!=FAIL) {
                            for(i=0; i<ntagrefs; i++) {
                                name=(char *)HDgettagname(tag_arr[i]);
                                if(!name) 
                                    name = "Unknown Tag";
                                printf("\t\t%-30s: (tag=%6d) ref=%d\n",
                                       name, (int)tag_arr[i],(int)ref_arr[i]);
                            }	/* end for */
                        }	/* end if */
                        HDfreespace(tag_arr);
                        HDfreespace(ref_arr);
                    }	/* end else */
                }	/* end if */
                Vdetach(vkey);
            }	/* end if */
        }	/* end else */
    } /* dumping groups */
} /* print_item */


void lprint(int32 fid, dd_t *desc_tmp, int num)
{
    intn j = 0, empty = 0;
    uint16 prev = 0;
    char *name;
    
    while (j <num) {
        if (desc_tmp[j].tag == DFTAG_NULL) {
            empty++;
            j++;
            continue;               /* don't print anything now */
        }
        
        /* 
         * skip this tag if the user only wants to see some tags and
         *  this is not one of them 
         */
        if(only_tag != DFTAG_NULL && only_tag != desc_tmp[j].tag) {
            j++;
            continue;
        }
        
        /*
         ** Find and print text description of this tag
         */
        name = (char *) HDgettagname(desc_tmp[j].tag);
        if(!name) 
			name = "Unknown Tag";
        printf("\n%-30s: (tag %d)\n", name, desc_tmp[j].tag);
        
        /*
         ** Print out reference number information
         */
        prev = desc_tmp[j].tag;
        if(longout) {
            while (desc_tmp[j].tag == prev && j < num) {
				print_item(fid,desc_tmp,j);
                j++;
            }
        } 
		else {
            printf("\tRef nos:");
            while (desc_tmp[j].tag == (uint16)prev && j < num) {
                printf(" %d",desc_tmp[j].ref);
                j++;
            }
        }
    }
    
    if(empty) 
		printf("\nEmpty (tag %d): %d slots\n", DFTAG_NULL, empty);
}

int main(int argc, char *argv[])
{
    int32 fid, aid;
    int i=1, j, n, status;
    
    while((i < argc) && (argv[i][0]=='-')){
        switch(argv[i][1]) {
        	case 'o':                 /* give non ordered output */
           	 	sort = FALSE;
           	 	break;

        	case 'd':                 /* go into debugging mode */
            	debug = TRUE;
            	break;

        	case 'v':                 /* print labels for elements */
            	labels = TRUE;
            	longout = TRUE;
            	break;

        	case 'l':                 /* give long output */
            	longout = TRUE;
            	break;

        	case 's':                 /* give info on special elements */
            	special = TRUE;
            	longout = TRUE;
            	break;

        	case 'g':                 /* give info on groups */
            	groups = TRUE;
            	longout = TRUE;
            	break;

        	case 't' :
            	if(argv[i][2] != '\0') 
                	only_tag = (uint16)atoi(&(argv[i][2]));
            	else 
                	only_tag = (uint16)atoi(&(argv[++i][0]));
            	break;

        	default:    
            	printf("Unknown option : -%c\n", argv[1][1]);
            	break;
        }
        i++;
    }
    
    /*
     * If a file name has not been supplied print the usage message
     */
    if(i == argc) {
        printf("%s,  version: 2.0   date: March 1, 1994\n",argv[0]);
        printf("%s [-o] [-l] [-d] [-v] [-g] [-t #] fn ....\n", argv[0]);
        printf("        This program displays information about the");
        printf(" data elements in\n");
        printf("        HDF file.\n");
        printf("    -d: offset & length info of each element in the file\n");
        printf("    -o: Ordered - display in reference number order\n");
        printf("    -l: Long format - display more information\n");
        printf("    -v: Verbose format - display text of annotations and labels.\n");
        printf("        (Verbose format automatically puts you in Long format).\n");
        printf("    -t #: List only information about a specific type of tag.\n");
        printf("          For example '%s -t 700 foo.hdf' \n", argv[0]);
        printf("          will list information only about Scientific Data\n");
        printf("          Groups.\n");
        printf("    -s: Give detailed descriptions of \"special elements\"\n");
	printf("    -g: List items in groups\n");
        exit (1);
    }
    
    desc_buf = (dd_t *) HDgetspace(sizeof(dd_t)*MAXBUFF);

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

    return(0);
}


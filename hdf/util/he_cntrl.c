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

/* ------ he-cntrl.c ------  
   This file contains much of the contol mechanisms for HDFed
   - Many of the command line functions
   - The parsing functions
   - predicate handling
*/

#include "he.h"


/* --- HEif --- executes commands if predicates are satisfied */
#ifdef PROTOTYPE
int HEif(HE_CMD *cmd)
#else
int HEif(cmd)
    HE_CMD *cmd;
#endif
{
    HE_PRED *pred;		/* predicates */
    HE_CMD *cmdTail;		/* last cmd we've seen in the sublist */

    if (cmd->argc == 2 && !strcmp(cmd->argv[1],"-help"))
    {
	puts("if [<predicates>]");
	puts("  <commands>*");
	puts("end");
	puts("\tExecutes commands if predicates are satisfied by element");
	return HE_OK;
    }

    /* parse the predicates on this command */
    pred = parsePred(cmd->argc, cmd->argv);
    if (!pred) return FAIL;

    /* execute the sub list only is the predicates are satisfied */
    if (satPred(currDesc(), pred))
    {
	/* go through sub-list until an end is encountered */
	for (cmdTail = cmd->sub; 
             cmdTail && strcmp(cmdTail->argv[0], "end");
	     cmdTail = cmdTail->next)
	    if (cmdTail->func)
		he_status = (*cmdTail->func) (cmdTail);
	    else
	    {
		fprintf(stderr, "Unrecognized command: %s\n", cmd->argv[0]);
		return FAIL;
	    }
    }
    return HE_OK;
}

/* --- HEselect --- step through all the elements in a file and execute if
   predicates are satisfied */
#ifdef PROTOTYPE
int HEselect(HE_CMD *cmd)
#else
int HEselect(cmd)
    HE_CMD *cmd;
#endif
{
    int t_currDesc;		/* tmp var */
    HE_PRED *pred;		/* predicate structure */
    HE_CMD *cmdTail;		/* last cmd we've seen in the sublist */

    if (cmd->argc == 2 && !strcmp(cmd->argv[1],"-help"))
    {
	puts("select [<predicates>]");
	puts("  <commands>*");
	puts("end");
	puts("\tSteps through all elements in the file that satisfies the");
	puts("\tpredicates and execute the commands on them.");
	return HE_OK;
    }

    /* generate predicate structure for this predicate list */
    pred = parsePred(cmd->argc, cmd->argv);
    if (!pred) return FAIL;

    /* save the curr desc index */
    t_currDesc = he_currDesc;

    /* step through all elements */
    for (he_currDesc = 0; he_currDesc < he_numDesc; he_currDesc++)
	if (currTag() != DFTAG_NULL && satPred(currDesc(), pred))
	    for (cmdTail = cmd->sub; strcmp(cmdTail->argv[0], "end");
		 cmdTail = cmdTail->next)
		if (cmdTail->func)
		    he_status = (*cmdTail->func) (cmdTail);
		else
		{
		    fprintf(stderr, "Unrecognized command: %s\n",
			    cmdTail->argv[0]);
		    he_status = FAIL;
		}

    /* restore he_currDesc */
    he_currDesc = t_currDesc;

    return HE_OK;
}

/* --- move to the next item --- */

extern HE_PRED *he_predicates;

#ifdef PROTOTYPE
int HEnext(HE_CMD *cmd)
#else
int HEnext(cmd)
    HE_CMD *cmd;
#endif
{
    int tmp;
    HE_PRED *predicates;
    HE_PRED *parsePred();

    if (cmd->argc == 2 && !strcmp(cmd->argv[1],"-help"))
    {
	puts("next [<predicates>]");
	puts("\tMove to the next element that satisfies the predicate");
	return HE_OK;
    }

    if (!fileOpen())
    {
	noFile();
        return HE_FAIL;
    }

    predicates = parsePred(cmd->argc, cmd->argv);

    /* the following code assumes that parsePred() returns NULL if error, */
    /* a predicate struct with no key if empty predicate list */
    if (!predicates) return HE_FAIL;

    /* replace this only if it is non-empty */
    if (predicates[0].key != 0)
    {
	if (he_predicates) free(he_predicates);
	he_predicates = predicates;
    }

    tmp = he_currDesc;
    do
    {
	if (tmp >= he_numDesc - 1)
	{
	    fprintf(stderr, "Reached end of file. Not moved.\n");
	    return HE_FAIL;
	}
	tmp++;
    } while (he_desc[tmp].tag == DFTAG_NULL ||
	     !satPred(he_desc + tmp, he_predicates));

    he_currDesc = tmp;

    return HE_OK;
}

/* ---------- Move to previous item -------------- */

#ifdef PROTOTYPE
int HEprev(HE_CMD *cmd)
#else
int HEprev(cmd)
    HE_CMD *cmd;
#endif
{
    int tmp;
    HE_PRED *predicates;
    HE_PRED *parsePred();

    if (cmd->argc == 2 && !strcmp(cmd->argv[1],"-help"))
    {
	puts("prev [<predicates>]");
	puts("\tMove to the next element that satisfies the predicate");
	return HE_OK;
    }

    if (!fileOpen())
    {
        noFile();
        return HE_FAIL;
    }

    predicates = parsePred(cmd->argc, cmd->argv);

    /* the following code assumes that parsePred() returns NULL if error,
       a predicate struct with no key if empty predicate list */
    if (!predicates) return HE_FAIL;

    if (predicates[0].key != 0)
    {
	if (he_predicates) free(he_predicates);
	he_predicates = predicates;
    }

    tmp = he_currDesc;
    do
    {
	if (tmp <= 0)
	{
	    fprintf(stderr, "Reached beginning of file. Not moved.\n");
	    return HE_FAIL;
	}
	tmp--;
    } while (he_desc[tmp].tag == DFTAG_NULL ||
	     !satPred(he_desc + tmp,he_predicates));
    he_currDesc = tmp;

    return HE_OK;
}

/* ----------- routines to call od on some objects ----------- */

#ifdef PROTOTYPE
int HEdump(HE_CMD *cmd)
#else
int HEdump(cmd)
    HE_CMD *cmd;
#endif
{
    register int i;
    int offset = 0, raw = 0;
    char *format = "-o";
    int32 length = 0;		/* zero is special, means all */

    for (i = 1; i < cmd->argc; i++)
	if (cmd->argv[i][0] == '-')
	    switch (findOpt(cmd->argv[i] + 1))
	    {
	    case HE_HELP:
            printf("dump [-offset <offset>] [-length <len>]\n");
            printf("\t[-decimal|-short|-byte|-hexidecimal|-float|-double|-ascii|\n");
                  printf("\t[-udecimal|-ushort|-octal|]\n");
            printf("\tDisplay the contents of the current object\n");
            printf("\t-offset            Start offset\n");
            printf("\t-length            Length (bytes) to look at\n");
            printf("\t-decimal           Decimal format [32 bit integers]\n");
            printf("\t-short             Decimal format   [16 bit integers]\n");
            printf("\t-byte              Decimal format    [8 bit integers]\n");
            printf("\t-hexidecimal       Hexidecimal format\n");
            printf("\t-float             Float format   [32 bit floats]\n");
            printf("\t-double            Float format  [64 bit floats]\n");
            printf("\t-ascii             Ascii format\n");
            printf("\t-udecimal          Unsigned Decimal format [32 bit integers]\n");
            printf("\t-ushort            Unsigned Decimal format   [16 bit integers]\n");
            printf("\t-octal             Octal format [Default]\n");
            return HE_OK;
	    case HE_OFFSET:
            offset = atoi(cmd->argv[++i]);
            break;
	    case HE_LENGTH:
            length = atoi(cmd->argv[++i]);
            if (length <= 0)
            {
                fprintf(stderr,"Illegal length: %s, ignored.\n",
                    cmd->argv[i]);
                length = 0;
            }
            break;
        case HE_DECIMAL:
            format = "-i";
            break;
        case HE_UDECIMAL:
            format = "-d";
            break;
	    case HE_SHORT:
            format = "-j";
            break;
        case HE_USHORT:
            format = "-s";
            break;
	    case HE_BYTE:
            format = "-b";
            break;
	    case HE_OCTAL:
            format = "-o";
            break;
	    case HE_HEX:
            format = "-x";
            break;
	    case HE_FLOAT:
            format = "-f";
            break;
	    case HE_DOUBLE:
            format = "-e";
            break;
	    case HE_ASCII:
            format = "-a";
            break;
        case HE_RAW:
            raw = DFNT_NATIVE;
            break;
	    case HE_NOTFOUND:
            unkOpt(cmd->argv[i]);
            return HE_FAIL;
	    case HE_AMBIG:
            ambigOpt(cmd->argv[i]);
            return HE_FAIL;
	    default:
            irrOpt(cmd->argv[i]);
            return HE_FAIL;
	    }
	else
	{
	    unkArg(cmd->argv[i]);
	    return HE_FAIL;
	}
    return dump(length, offset, format, raw);
}


#define MAX_LINE 60

/*
 * Run 'od' on a segment of the current data element
 */
#ifdef PROTOTYPE
int dump(int32 length, int offset, char *format, int raw_flag)
#else
int dump(length, offset, format, raw_flag)
    int32 length;			/* length of segment to look at */
    int offset;			/* offset from start of data element */
    char *format;		/* od arg, e.g. -x for hexidecimal */
    int raw_flag;
#endif
{
    int32 eltLength;
    register int32 i;
    register int len = 0;
    char *data;

    if (!fileOpen()) {
	noFile();
	return HE_FAIL;
    }

    eltLength = getElement(he_currDesc, &data);
    if (eltLength <= 0) {
	fprintf(stderr, "Unable to get element.\n");
	return HE_FAIL;
    }

    /* adjust the offset, negative offset implies starting from end. then
       check to see if offset is in range */
    if (offset < 0)
	offset += eltLength;
    if (offset < 0 || offset > eltLength) {
	fprintf(stderr,"Illegal offset. Setting offset to 0.\n");
	offset = 0;
    }

    /* adjust the length if it falls beyond the end of the element */
    if (length == 0 || length > (eltLength - offset))
	length = eltLength - offset;

    /*
     * Dump the data to the console
     */

    switch(format[1]) {

    case 'i':
        {
            register int32 *idata;
            idata = (int32 *) HDgetspace(length/4*sizeof(int32));

            DFKconvert((uint8 *)(data+offset), (uint8 *)idata, DFNT_INT32|raw_flag,
                       length/4, DFACC_READ, 0, 0); 
            printf("%8d: ", offset); 
            for(i = 0; i < length/4; i++) {
                printf("%11d ", idata[i]);
                if(++len > 4) {len = 0; printf("\n%8d: ", (offset + (i + 1) * 4));} 
            }  
            printf("\n");
            HDfreespace((VOIDP)idata);
        }
        break;

    case 'd':
        {
            register uint32 *idata;
            idata = (uint32 *) HDgetspace(length/4*sizeof(int32));

            DFKconvert((uint8 *)(data+offset), (uint8 *)idata, DFNT_UINT32|raw_flag,
                       length/4, DFACC_READ, 0, 0);
            printf("%8d: ", offset);
            for(i = 0; i < length/4; i++) {
                printf("%11u ", idata[i]);
                if(++len > 4) {len = 0; printf("\n%8d: ", (offset + (i + 1) * 4));}
            }
            printf("\n");
            HDfreespace((VOIDP)idata);
        }
        break;
    case 'j':
        {
            register int16 *sdata;
            sdata = (int16 *) HDgetspace(length/2*sizeof(int16));
            DFKconvert((uint8 *)(data + offset), (uint8 *)sdata, DFNT_INT16|raw_flag,
                       length/2, DFACC_READ, 0, 0);

            printf("%8d: ", offset);
            for(i = 0; i < length/2; i++) {
                printf("%10d ", sdata[i]);
                if(++len > 5) {len = 0; printf("\n%8d: ", (offset + (i + 1) * 2));}
            }
            printf("\n");
            HDfreespace((VOIDP)sdata);
        }
        break;

    case 's':
        {
            register uint16 *sdata;
            sdata = (uint16 *) HDgetspace(length/2*sizeof(uint16));

            DFKconvert((uint8 *)(data + offset), (uint8 *)sdata, DFNT_UINT16|raw_flag, 
                       length/2, DFACC_READ, 0, 0);
            printf("%8d: ", offset); 
            for(i = 0; i < length/2; i++) {
                printf("%10d ", sdata[i]);
                if(++len > 5) {len = 0; printf("\n%8d: ", (offset + (i + 1) * 2));} 
            } 
            printf("\n");
            HDfreespace((VOIDP)sdata);
        }
        break;

    case 'b':
        {
            register uint8 *bdata;
            bdata = (uint8 *) HDgetspace(length);
          
            DFKconvert((uint8 *)(data+offset), bdata, DFNT_UINT8|raw_flag,
                       length, DFACC_READ, 0,0);
            printf("%8d: ", offset); 
            for(i = 0; i < length; i++) {
                printf("%6d ", bdata[i]);
                if(++len > 7) {len = 0; printf("\n%8d: ", (offset + (i + 1)));} 
            }
            printf("\n");
            HDfreespace((VOIDP)bdata);
        }
        break;
        
    case 'x':
        {
            register intn *idata;
            intn sizeintn;
            sizeintn=sizeof(intn);
            idata= (intn *)(data+offset);
            printf("%8d: ", offset); 
            for(i = 0; i < length / sizeintn; i++) {
                printf("%10x ", idata[i]);
                if(++len > 5) {len = 0; printf("\n%8d: ", (offset + (i + 1) * sizeintn));} 
            }
            printf("\n"); 
            HDfreespace((VOIDP)idata);
        }
        break;

    case 'o':
        {
            register intn *idata;
            intn sizeintn;
            sizeintn=sizeof(intn);
            idata= (intn *)(data+offset);
            printf("%8d: ", offset);
            for(i = 0; i < length / 4; i++) {
                printf("%10o ", idata[i]);
                if(++len > 4) {len = 0; printf("\n%8d: ", (offset + (i + 1) * sizeintn));} 
            }
            printf("\n");
            HDfreespace((VOIDP)idata);
        }
        break;

    case 'a':
        {
            register char *cdata;
            cdata = (char *) (data + offset); 
            printf("%8d: ", offset);
            for(i = 0; i < length; i++) {
                if (cdata[i] != '\0')
                   printf("%c", cdata[i]);
                else 
                   printf(" ");
                if(++len > 40) {len = 0; printf("\n%8d: ", (offset + (i+1)));}  
            }
            printf("\n");
        }
        break;

    case 'f':
        {
            register float32 *fdata;
            fdata = (float32 *) HDgetspace(length/4*sizeof(float32));
  
            DFKconvert((uint8 *)(data + offset), (uint8 *)fdata, DFNT_FLOAT32|raw_flag,
                       length/4, DFACC_READ, 0, 0);

            printf("%8d: ", offset);
            for(i = 0; i < length / 4; i++) {
                printf("%15e", fdata[i]);
                if(++len > 3) {len = 0; printf("\n%8d: ", (offset + (i + 1) * 4));}
            }
            printf("\n");
            HDfreespace((VOIDP)fdata);
        }
        break;

    case 'e':
        {
            register float64 *fdata;
            fdata = (float64 *) HDgetspace(length/8*sizeof(float64));
  
            DFKconvert((uint8 *)(data + offset), (uint8 *)fdata, DFNT_FLOAT64|raw_flag,
                       length/8, DFACC_READ, 0, 0);

            printf("%8d: ", offset);
            for(i = 0; i < length / 8; i++) {
                printf("%30e", fdata[i]);
                if(++len > 1) {len = 0; printf("\n%8d: ", (offset + (i + 1) * 8));}
            }
            printf("\n"); 
            HDfreespace((VOIDP)fdata);
        }
        break;

    default:
        printf("Doing the default thang\n");
        break;

    }

    free(data);

    return HE_OK;
}

/* ------------------ Print 'info' ----------------- */

#ifdef PROTOTYPE
int HEinfo(HE_CMD *cmd)
#else
int HEinfo(cmd)
    HE_CMD *cmd;
#endif
{
  register int i;
  int all = NO;
  int longout = NO;
  int group = NO;
  int label = NO;
  
  for (i = 1; i < cmd->argc; i++)
    if (cmd->argv[i][0] == '-')
      switch (findOpt(cmd->argv[i] + 1))
        {
        case HE_HELP:
          puts("info [-all] [-long] [-group] [-label]");
          puts("\t-all\t\tShow info for all elements in file");
          puts("\t-long\t\tShow more info");
          puts("\t-group\t\tOrganize info in group(s)");
          puts("\t-label\t\tShow label if any");
          return HE_OK;
        case HE_LONGOUT:
          longout = YES;
          break;
        case HE_ALL:
          all = YES;
          break;
        case HE_LABEL:
          label = YES;
          break;
        case HE_DOGROUP:
          group = YES;
          break;
        case HE_NOTFOUND:
          unkOpt(cmd->argv[i]);
          return HE_FAIL;
        case HE_AMBIG:
          ambigOpt(cmd->argv[i]);
          return HE_FAIL;
        default:
          irrOpt(cmd->argv[i]);
          return HE_FAIL;
        }
    else
      {
        unkArg(cmd->argv[i]);
        return HE_FAIL;
      }
  return info(all, longout, group, label);
}

/*
 *Show info about data elements
 */
#ifdef PROTOTYPE
int info(int all, int longout, int group, int label)
#else
int info(all, longout, group, label)
    int all;			/* select all elements */
    int longout;		/* long output */
    int group;			/* arrange output in groups */
    int label;			/* show labels */
#endif
{
    int idx;
    int empty = 0;
    int start, end;
    int d;
    int *mark;
    register int i, j;

    if (!fileOpen()) {
      noFile();
      return HE_OK;
    }

    if (!group || (!isGrp(currTag()) && !all)) {
      if (all) {
        start = 0;
        end = he_numDesc - 1;
      }
      else
        start = end = he_currDesc;
      
      for (i = start; i <= end; i++)
        if (he_desc[i].tag == DFTAG_NULL)
          empty++;
        else
          {
            if (all && i == he_currDesc)
              printf("*");
            else
              printf(" ");
            printf("(%d) ", i+1); /* 1 based */
            infoDesc(i, longout, label);
          }
      
	if (empty > 0)
	    printf("Empty (tag %d) : %d slots.\n", DFTAG_NULL, empty);
    }
    else
    {
	mark = (int *) calloc(he_numDesc, sizeof(int));

	if (all)
	{
	    start = 0;
	    end = he_numGrp - 1;
	}
	else
	{
	    if (he_numGrp == 0)
	    {
		fprintf(stderr, "There is no group in this file.\n");
		return HE_FAIL;
	    }
	    start = end = currGrpNo();
	}
	for (i = start; i <= end; i++)
	{
	    printf("**Group %d:\n", i+1); /* 1 based */
	    idx = he_grp[i].desc;
	    infoDesc(idx, longout, label);

	    mark[idx] = YES;

	    for (j = 0; j < he_grp[i].size; j ++)
		if ((d = findDesc(he_grp[i].ddList + j)) >= 0)
		{
		    mark[d] = 1;
		    infoDesc(d, longout, 0);
		}
		else
		    fprintf(stderr,
			    "**Tag: %d, Ref: %d not in descriptors list!\n",
			    he_grp[i].ddList[j].tag,
			    he_grp[i].ddList[j].ref);
	}
	if (all)
	{
	    puts("\n**These do not belong to any group:");
	    for (i = 0; i < he_numDesc; i++)
		if (!mark[i])
		    if (he_desc[i].tag == DFTAG_NULL)
			empty++;
		    else
			infoDesc(i, longout, label);

	    if (empty > 0)
		printf("Empty (tag %d) : %d slots.\n", DFTAG_NULL, empty);
	}
    }
    return HE_OK;
}

#ifdef PROTOTYPE
void infoDesc(int desc, int longout, int label)
#else
void infoDesc(desc, longout, label)
    int desc;
    int longout;
    int label;
#endif
{
    char *s, *name;

    name = HDgettagname(he_desc[desc].tag);
    if(!name) name = "Unknown Tag";

    printf("\t%-30s: (Tag %d)", name, he_desc[desc].tag);

    if (longout)
	printf("\n\tRef: %d, Offset: %d, Length: %d (bytes)\n",
	       he_desc[desc].ref, he_desc[desc].offset, he_desc[desc].length);
    else
	printf(" Ref %d\n", he_desc[desc].ref);
    if (label)
    {
	getAnn(HE_LABEL, he_desc[desc].tag, he_desc[desc].ref, &s);
	if (s != NULL) printf("\tLabel: %s\n",s);
    }
}

/* ---------- HEdelete --- 'stub' function for delete */
#ifdef PROTOTYPE
int HEdelete(HE_CMD *cmd)
#else
int HEdelete(cmd)
    HE_CMD *cmd;
#endif
{
    if (cmd->argc < 2)
	return delete(he_currDesc);
    else
    {
	if (cmd->argv[1][0] != '-') {
	    unkArg(cmd->argv[1]);
            return HE_FAIL;
        }
	else
	    switch (findOpt(cmd->argv[1] + 1))
	    {
	    case HE_HELP:
		puts("delete");
		puts("\tDelete this element or group.");
		return HE_OK;
	    case HE_NOTFOUND:
		unkOpt(cmd->argv[1]);
		return HE_FAIL;
	    case HE_AMBIG:
		ambigOpt(cmd->argv[1]);
		return HE_FAIL;
	    default:
		irrOpt(cmd->argv[1]);
		return HE_FAIL;
	    }
    }
}

/* delete -- deletes a group and its elts or an elt from current hdf file */
#ifdef PROTOTYPE
int delete(int curr)
#else
int delete(curr)
    int curr;
#endif
{
    int ret;

    /* check if any file is open */
    if (!fileOpen())
    {
	noFile();
	return HE_FAIL;
    }

    /* call the actual routine to do it, the update the descriptor list */
    ret = recurseDel(curr);
    updateDesc();

    return ret;
}

/* recurseDel -- this function recursively tries to delete some elts from
   a file. if the current elt is a group, it will call itself on the elts
   of the group. else, it will check if any group references the elt, and
   actually deletes it if there is no other references */
#ifdef PROTOTYPE
int recurseDel(int curr)
#else
int recurseDel(curr)
    int curr;
#endif
{
    int d, currGrp;
    register int i;

    if (isGrp(he_desc[curr].tag))
    {
	/* if this is a group, do its elts then itself */

	/* find the index of the group */
	currGrp = desc2Grp(curr);

	/* step through the elts of this group */
	for ( i = 0; i < he_grp[currGrp].size ; i++)
	{
	    d =  findDesc(he_grp[currGrp].ddList + i);
	    if (d >= 0)
	    {
		/* null this so it will not report that this is a multiple
		 * copy of itself */
		he_grp[currGrp].ddList[i].tag = DFTAG_NULL;
		he_grp[currGrp].ddList[i].ref = 0;

		/* Try to delete this component */
		if (recurseDel(d) < 0) return HE_FAIL;
	    }
	}
	/* tried all components, now delete self */
	if (!hasReference(curr))
	    if (deleteDesc(curr) < 0)
		return HE_FAIL;
    }
    else
	/* not a group, actually delete this only if there is no group
	   referencing it */
	if (!hasReference(curr))
	    if (deleteDesc(curr) < 0)
		return HE_FAIL;
    return HE_OK;
}

/* some char const for the get/parse routines */
#define CR '\n'
#define SPACE ' '
#define TAB '\t'
#define QUOTE '"'
#define BREAK '\003'		/* C-c */
#define ESCAPE '\\'		/* Quote escape */
#define COMMENT '!'		/* Comment a line if first character */

/* max line size -- change this if you type longer than this in one line */
#define HE_LINE_SZ 512

/* command separator */
#define HE_SEPARATOR ';'

/* is this in batch mode or interactive? */
int he_batch = NO;

/* nesting when if/select is encountered */
int he_nestLevel = 0;

/* prompt is actually "he"<he_prompt><space> */
char *he_prompt = ">";
char he_nestChar = '>';

/* table to associate command to the function --
   add additional functions anywhere in the table BEFORE the
   Marker {0,0} entry */
struct {
    char *str;
    HE_FUNC func;
} he_funcTab[] =
{
{"open",     (HE_FUNC) HEopen},
{"close",    (HE_FUNC) HEclose},
{"next",     (HE_FUNC) HEnext},
{"prev",     (HE_FUNC) HEprev},
{"alias",    (HE_FUNC) HEalias},
{"unalias",  (HE_FUNC) HEunalias},
{"display",  (HE_FUNC) HEdisplay},
{"info",     (HE_FUNC) HEinfo},
{"list",     (HE_FUNC) HEinfo},
{"if",       (HE_FUNC) HEif},
{"select",   (HE_FUNC) HEselect},
{"wait",     (HE_FUNC) HEwait},
{"delete",   (HE_FUNC) HEdelete},
{"quit",     (HE_FUNC) HEquit},
{"dump",     (HE_FUNC) HEdump},
{"getr8",    (HE_FUNC) HEgetR8},
{"putr8",    (HE_FUNC) HEputR8},
{"put",      (HE_FUNC) HEput},
{"revert",   (HE_FUNC) HErevert},
{"write",    (HE_FUNC) HEwrite},
{"annotate", (HE_FUNC) HEannotate},
{"help",     (HE_FUNC) HEhelp},
{"end",      (HE_FUNC) 0},
{NULL,       (HE_FUNC) 0},			/* Marker */
};

#ifdef PROTOTYPE
HE_FUNC findFunc(char *word)
#else
HE_FUNC findFunc(word)
    char *word;
#endif /* PROTOTYPE */
{
    int len;
    int found = -1;
    register int i;

    len = strlen((const char *) word);

    for (i = 0; he_funcTab[i].str; i++)
	if (!strncmp(he_funcTab[i].str, (const char *) word, len))
	{
	    /* check for exact match */
	    if (strlen(he_funcTab[i].str) == len)
		return he_funcTab[i].func;

	    if (found < 0)
		found = i;
	    else
	    {
		fprintf(stderr,"Ambiguous command: %s.\n", word);
		return NULL;
	    }
	}

    if (found < 0) 
      return NULL;
    else
      return he_funcTab[found].func;
}

/* prompt -- printout prompt according to the nesting level */
#ifdef PROTOTYPE
void prompt(void)
#else
void prompt()
#endif
{

#ifndef MAC

    if (!he_nestLevel)
	printf("hdfed%s ", he_prompt);
    else
    {
	register int i;

	printf("     %s ", he_prompt);
	for (i = he_nestLevel; i; i--) putchar(he_nestChar);
	putchar(' ');
    }

#endif /* MAC */

}


/* reads a line of input into p */
/* Skips all initial spaces and empty commands */
/* always returns with at least a word in p, unless eof */
/* if eof and p is not empty, return HE_OK, else if no word, return EOF */
#ifdef PROTOTYPE
int getLine(register char *p)
#else
int getLine(p)
    register char *p;
#endif
{
    static int ch = 0;

    do
    {
	if (!he_batch && (ch != EOF)) prompt();
	ch = getc(stdin);
	if (ch == COMMENT)
	{
	    /* Skip this line */
	    do ch = getchar();
	    while ((ch != CR) && (ch != EOF));
	}
	else
	    while ((ch == SPACE) || (ch == TAB) || (ch == HE_SEPARATOR))
		ch = getchar();
	if (ch == EOF) return EOF;
    } while (ch == CR);

    while ((ch != EOF) && (ch != CR))
	switch(ch)
	{
	case ESCAPE:
	    ch = getchar();
	    if (!(ch == CR)) *p++ = ch;
	    ch = getchar();
	    break;
	case QUOTE:
	    ch = getchar();
	    while (ch != QUOTE)
	    {
		if (ch == ESCAPE) ch = getchar();
		*p++ = ch;
		ch = getchar();
	    }
	    ch = getchar();	/* Skip over the QUOTE */
	    break;
	case SPACE:
	case TAB:
	    *p++ = SPACE;
	    while ((ch == SPACE) || (ch == TAB)) ch = getchar();
	    break;
	case HE_SEPARATOR:
	    if (!isspace(*(p-1))) *p++ = SPACE;
	    *p++ = HE_SEPARATOR;
	    ch = SPACE;		/* Ensure next is a space */
	    break;
	default:
	    *p++ = ch;
	    ch = getchar();
	    break;
	}

    *p++ = '\0';
    return ch;
}

/* copy the next (space-delimited) word and advance the pointer as a
   side effect */
#ifdef PROTOTYPE
char *nextWord(char **p)
#else
char *nextWord(p)
    char **p;
#endif
{
    char *word;
    register char *s, *q;
    int len;

    q = *p;
    while (*q && isspace(*q)) q++;
    if (!(*q))
    {
	*p = q;
	return NULL;
    }

    s = q;
    while (*s && !isspace(*s)) s++;
    len = s - q;

    word = (char *) malloc(len + 1);
    strncpy(word, q, len);
    word[len] = '\0';

    *p = s;
    while (**p && (isspace(**p))) (*p)++;

    return word;
}

#ifdef PROTOTYPE
HE_CMD *parseCmd(char **p)
#else
HE_CMD *parseCmd(p)
    char **p;
#endif
{
    char *word;
    HE_CMD *cmd;
    HE_CMD *aliasCmd;
    HE_CMD *cmdTail;

    if (!(**p)) return NULL;

    cmd = (HE_CMD *) calloc(1, sizeof(HE_CMD));
    cmd->next = cmd->sub = (HE_CMD *) NULL;
    cmd->argc = 1;
    cmd->argv[0] = nextWord(p);

    if (aliasCmd = findAlias(cmd->argv[0]))
	cmd = aliasCmd;
    else
	cmd->func = findFunc(cmd->argv[0]);

    if (((HE_CMD*) cmd->func == (HE_CMD*) HEalias) || 
        ((HE_CMD*) cmd->func == (HE_CMD*) HEwait)) {
      /* let the alias command handle the parsing */
      cmd->argv[1] = copyStr(*p);
      cmd->argc = 2;
      
      **p = '\0';
    }
    else
    {
	cmdTail = cmd;
	while (cmdTail->next) cmdTail = cmdTail->next;

	for (word = nextWord(p); word && strcmp(word, ";");
	     word = nextWord(p), cmdTail->argc++)
	    cmdTail->argv[cmdTail->argc] = word;

	while (**p && (isspace(**p) || (**p == ';'))) (*p)++;
    }
    return cmd;
}

/* Inputs a line and returns cmd list of line */
#ifdef PROTOTYPE
HE_CMD *parse(void)
#else
HE_CMD *parse()
#endif
{
    static char line[HE_LINE_SZ];
    static char *ptr;
    int notDone = 1;
    HE_CMD *cmd;
    HE_CMD *cmdTail;

    if (getLine(line) == EOF) return NULL;
    ptr = line;

    cmdTail = cmd = parseCmd(&ptr);
    while (cmdTail->next) cmdTail = cmdTail->next;

    while (notDone)
    {
	cmdTail->next = parseCmd(&ptr);
	notDone = (cmdTail->next != NULL);
	while (cmdTail->next) cmdTail = cmdTail->next;
    }
    return cmd;
}

#ifdef PROTOTYPE
HE_CMD *getCmd(void)
#else
HE_CMD *getCmd()
#endif
{
    static HE_CMD *cmdList;
    HE_CMD *cmd;
    HE_CMD *cmdTail;

    if(!cmdList) 
      cmdList = parse();
    if(!cmdList) 
      return NULL;

    cmd = cmdList;
    if (cmdList) cmdList = cmdList->next;
    cmd->next = (HE_CMD *) NULL; /* Cut off links since these will be */
				 /* accessed later */

    if (cmd && (((HE_CMD*)cmd->func == (HE_CMD*)HEif) ||
                ((HE_CMD*)cmd->func == (HE_CMD*)HEselect)) &&
	!((cmd->argc > 1) && (cmd->argv[1][0] == '-') &&
	  (findOpt(cmd->argv[1]+1) == HE_HELP)))
    {
	he_nestLevel++;

	cmd->sub = getCmd();
	for (cmdTail = cmd->sub;
	     cmdTail && strcmp(cmdTail->argv[0], "end"); /* while != "end" */
	     cmdTail = cmdTail->next)
	    cmdTail->next = getCmd();

	he_nestLevel--;
    }
    return cmd;
}

#define HE_ALIAS_SZ 256

struct he_alias {
    char *str;
    HE_CMD *cmd;
} he_aliasTab[HE_ALIAS_SZ];

int he_numAlias = 0;

#ifdef PROTOTYPE
int setAlias(char *str, HE_CMD *cmd)
#else
int setAlias(str, cmd)
    char *str;
    HE_CMD *cmd;
#endif
{
    register int i;

    for (i = 0; i < he_numAlias; i++)
	if (!strcmp(str, he_aliasTab[i].str))
	{
	    he_aliasTab[i].cmd = cmd;
	    return HE_OK;
	}
    if (he_numAlias == HE_ALIAS_SZ)
    {
	fprintf(stderr, "Alias table full.\n");
	return HE_FAIL;
    }
    he_aliasTab[he_numAlias].str = str;
    he_aliasTab[he_numAlias++].cmd = cmd;

    return HE_OK;
}

#ifdef PROTOTYPE
HE_CMD *mkDupCmd(HE_CMD *cmd)
#else
HE_CMD *mkDupCmd(cmd)
    HE_CMD *cmd;
#endif
{
    register int i;
    HE_CMD *dupCmd;

    dupCmd = (HE_CMD *) calloc(1, sizeof(HE_CMD));
    dupCmd->func = cmd->func;
    dupCmd->argc = cmd->argc;
    dupCmd->next = dupCmd->sub = (HE_CMD *) NULL;
    for (i = 0; i < cmd->argc; i++)
	dupCmd->argv[i] = copyStr(cmd->argv[i]);

    return dupCmd;
}

#ifdef PROTOTYPE
HE_CMD *findAlias(char *str)
#else
HE_CMD *findAlias(str)
    char *str;
#endif
{
    register int i;
    HE_CMD *cmd;
    HE_CMD *dupCmd;
    HE_CMD *cmdTail;

    for (i = 0; i < he_numAlias; i++)
	if (!strcmp(str, he_aliasTab[i].str))
	{
	    cmd = he_aliasTab[i].cmd;
	    dupCmd = mkDupCmd(cmd);

	    cmd = cmd->next;
	    for (cmdTail = dupCmd; cmd;
		 cmd = cmd->next, cmdTail = cmdTail->next)
		cmdTail->next = mkDupCmd(cmd);

	    return dupCmd;
	}
    return NULL;
}

#ifdef PROTOTYPE
int HEunalias(HE_CMD *cmd)
#else
int HEunalias(cmd)
    HE_CMD *cmd;
#endif
{
    register int a, i, j;

    for (a = 1; a < cmd->argc; a++)
	for (i = 0; i < he_numAlias; i++)
	    if (!strcmp(cmd->argv[a], he_aliasTab[i].str))
	    {
		he_numAlias--;
		for (j = i; j < he_numAlias; j++)
		{
		    he_aliasTab[j].str = he_aliasTab[j+1].str;
		    he_aliasTab[j].cmd = he_aliasTab[j+1].cmd;
		}
		break;
	    }
    return HE_OK;
}

#ifdef PROTOTYPE
void printAlias(char *word, HE_CMD *cmd)
#else
void printAlias(word, cmd)
    char *word;
    HE_CMD *cmd;
#endif
{
    register int j;

    printf("%s:", word);
    for (; cmd; cmd = cmd->next)
    {
	printf("\t");
	for (j = 0; j < cmd->argc; j++) printf("%s ",cmd->argv[j]);
	puts("");
    }
}

#ifdef PROTOTYPE
int HEalias(HE_CMD *cmd)
#else
int HEalias(cmd)
    HE_CMD *cmd;
#endif
{
    char *s;
    char *word;
    register int i;
    HE_CMD *cmdTail;

    s = cmd->argv[1];
    word = nextWord(&s);

    if (!word)
    {
	for (i = 0; i < he_numAlias; i++)
	    printAlias(he_aliasTab[i].str, he_aliasTab[i].cmd);
	return HE_OK;
    }

    cmd = parseCmd(&s);
    if (!cmd)
    {
	cmd = findAlias(word);
	printAlias(word, cmd);
	return HE_OK;
    }
    for (cmdTail = cmd; cmdTail->next;  cmdTail = cmdTail->next)/*EMPTY*/
        ;
    while (cmdTail->next = parseCmd(&s))
    for (; cmdTail->next;  cmdTail = cmdTail->next)/*EMPTY*/
        ;
    return setAlias(word, cmd);
}

HE_PRED *he_predicates;

/* resetPred -- for setting the he_predicates to point to a pred
   of only group, this is the default when a file is opened */
#ifdef PROTOTYPE
int resetPred(void)
#else
int resetPred()
#endif
{
    if (he_predicates != NULL)
	free(he_predicates);

    he_predicates = (HE_PRED *) calloc(2, sizeof(HE_PRED));
    he_predicates[0].key = HEK_GROUP;
    he_predicates[1].key = 0;

    return HE_OK;
}

struct {
    char *str;
    int key;
} he_keyTab[] =
{
{"!=",		HEK_NEQUAL | HE_COMPARATOR},
{"<",		HEK_LESST | HE_COMPARATOR},
{"<=",		HEK_LEQUAL | HE_COMPARATOR},
{"==",		HEK_EQUAL | HE_COMPARATOR},
{">",		HEK_GRT | HE_COMPARATOR},
{">=",		HEK_GEQUAL | HE_COMPARATOR},
{"all",		HEK_ALL | HE_PREDICATE},
{"any",		HEK_ALL | HE_PREDICATE},
{"ref",		HEK_REF | HE_PREDICATE},
{"succeed",	HEK_SUCCEED | HE_PREDICATE},
{"fail",	HEK_FAIL | HE_PREDICATE},
{"tag",		HEK_TAG | HE_PREDICATE},
{"group",	HEK_GROUP | HE_PREDICATE},
/* Finish this later */
{NULL, 0},			/* Marker */
};

#ifdef PROTOTYPE
int findKey(char *word)
#else
int findKey(word)
    char *word;
#endif
{
    register int i;
    int len;
    int found = -1;

    len = strlen(word);

    for (i = 0; he_keyTab[i].str; i++)
	if (!strncmp(he_keyTab[i].str, word, len))
	{
	    /* if this is an exact match, just return */
	    if (strlen(he_keyTab[i].str) == len)
		return he_keyTab[i].key;
	    if (found < 0)
		found = i;
	    else
	    {
		fprintf(stderr, "Ambiguous: %s.\n", word);
		return HE_NOTFOUND;
	    }
	}
    if (found < 0)
    {
	fprintf(stderr, "Predicate/comparator: %s not found.\n", word);
	return HE_NOTFOUND;
    }
    return he_keyTab[found].key;
}

#ifdef PROTOTYPE
int isNumber(register char *s)
#else
int isNumber(s)
    register char *s;
#endif
{
    for (; *s; s++)
	if (!isdigit((int) *s)) return NO;

    return YES;
}

#define HE_PRED_SZ HE_ARG_SZ

#ifdef PROTOTYPE
HE_PRED *parsePred(int argc, char *argv[])
#else
HE_PRED *parsePred(argc, argv)
    int argc;
    char *argv[];
#endif
{
    HE_PRED *pred;
    int predNum = -1;
    int state = 0;
    int key;
    register int i;
    char *s;
    char *tok;

    pred = (HE_PRED *) calloc(HE_PRED_SZ, sizeof(HE_PRED));

    for ( i = 1; i < argc; i++)
    {
	s = argv[i];
	while (*s)
	{
	    if (state != 2) 
		tok = nextToken(&s);
	    else
		tok = nextWord(&s);
	    if (!tok) break;

	    if (state != 2)
	    {
		if ((key = findKey(tok)) == HE_NOTFOUND)
		{
		    free(pred);
		    return NULL;
		}
		free(tok);
	    }

	    switch(state)
	    {
	    case 0:
		/* Ready to accept a predicate */
		if (!(key & HE_PREDICATE))
		{
		    fprintf(stderr, "Parse error: %s.\n", argv[i]);
		    free(pred);
		    return NULL;
		}
		pred[++predNum].key = key & ~(HE_PREDICATE | HE_COMPARATOR);
		state = 1;
		break;

	    case 1:
		/* Can be a comparator for previous pred or a new predicate */
		if (key & HE_PREDICATE)
		{
		    pred[++predNum].key =
			key & ~(HE_PREDICATE | HE_COMPARATOR);

		}
		else if (key & HE_COMPARATOR)
		{
		    pred[predNum].Comp =
			key & ~(HE_PREDICATE | HE_COMPARATOR);
		    state = 2;
		}
		else
		{
		    fprintf(stderr, "Parse error: %s.\n", argv[i]);
		    free(pred);
		    return NULL;
		}
		break;

	    case 2:
		/* Looking for an argument */
		if (isNumber(tok))
		{
		    pred[predNum].argType = HE_NUMBER;
		    pred[predNum].arg.i = atoi(tok);
		}
		else
		{
		    pred[predNum].argType = HE_STRING;
		    pred[predNum].arg.str = copyStr(tok);
		}
		state = 0;
		break;

	    default:
		NOT_REACHED();
		break;
	    }
	}
    }
    pred[++predNum].key = 0;

    return pred;
}

#ifdef PROTOTYPE
int satPred(DFdesc *desc, HE_PRED pred[])
#else
int satPred(desc, pred)
    DFdesc *desc;
    HE_PRED pred[];
#endif
{
    int i;

    if (!pred) return YES;
    for (i = 0; pred[i].key; i++)
    {
	switch(pred[i].key)
	{
	case HEK_ALL:
	    return YES;		/* Always satisfied */
	case HEK_GROUP:
	    if (!isGrp(desc->tag))
		return NO;
	    break;
	case HEK_TAG:
	    if (pred[i].argType != HE_NUMBER)
	    {
		fprintf(stderr, "Argument to tag predicate not a number.");
		return NO;
	    }
	    if (!numCompare((int)desc->tag, pred[i].Comp, pred[i].arg.i))
		return NO;
	    break;
	case HEK_REF:
	    if (pred[i].argType != HE_NUMBER)
	    {
		fprintf(stderr, "Argument to ref predicate not a number.");
		return NO;
	    }
	    if (!numCompare((int)desc->ref, pred[i].Comp, pred[i].arg.i))
		return NO;
	    break;
	case HEK_SUCCEED:
	    return (he_status == 0);
	case HEK_FAIL:
	    return (he_status != 0);
	default:
	    NOT_REACHED();
	    break;
	}
    }

    return 1;
}

#ifdef PROTOTYPE
char *nextToken(char **p)
#else
char *nextToken(p)
    char **p;
#endif
{
    register char *s, *q;
    char *tok;

    if (!(**p)) return NULL;

    s = *p;

    if (isalnum(**p))
	while (isalnum(*s)) s++;
    else
	while (*s && !isalnum(*s)) s++;

    q = tok = (char *) malloc((s-(*p)) + 1);
    while (*p != s) *q++ = *(*p)++;
    *q = '\0';

    return tok;
}
    
#ifdef PROTOTYPE
int numCompare(int n1, int Comp, int n2)
#else
int numCompare(n1, Comp, n2)
    int n1, Comp, n2;
#endif
{
    switch(Comp)
    {
    case HEK_EQUAL:
	return (n1 == n2);

    case HEK_NEQUAL:
	return (n1 != n2);

    case HEK_GRT:
	return (n1 > n2);

    case HEK_GEQUAL:
	return (n1 >= n2);

    case HEK_LESST:
	return (n1 < n2);

    case HEK_LEQUAL:
	return (n1 <= n2);

    default:
	NOT_REACHED();
	break;
    }
    return NO;
}

/* end of cntrl.c */

#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.2  1992/07/14 17:50:30  mlivin
updated help with no pixrect, name of 'hdfed' not 'he'

 * Revision 1.1  1992/07/06  22:54:16  mlivin
 * Initial revision
 *
*/
/******************************************************************************
 * he - HDF editor
 *
 * This program has a long history, starting with its creation as a tool to
 * help HDF developers work at a low-level on HDF files.  It has evolved into
 * user-level program, though it is still designed for small editing tasks on
 * HDF files.
 *
 * he allows sophisticated HDF users to manipulate the elements in an HDF file.
 * These manipulations include selecting groups and showing information about
 * them, dumping them to the output, writing them to new files, deleting them,
 * inserting them, replacing, say, the palette of an r8 group, and editing the
 * text labels and descriptions of any element.
 *
 * he will NOT allow the user to *arbitrarily* modify binary data in the file
 * or any element, though it allows modification of tag and reference numbers
 * within strict constraints.  The user should not attempt to alter individual
 * bytes.  It is acceptable, however, to replace an element with another of
 * the same type.
 *
 * he can be used both interactively or in "batch" mode.  Here is a sample
 * batch program:
 *
 *      #!/bin/csh -f
 *      set file=$1
 *      shift
 *      he -batch $file -nobackup << EOF
 *      info -all -group $*
 *      close
 *      quit
 *      EOF
 *
 * This makes use of C-shell variable substitution to pass a filename to he,
 * invokes he, and then lists out on separate lines the commands to give once
 * he is running.  The $* trailing the info command is also C-shell variable
 * substitution.
 *
 * List of commands:       annotate   dump    if      open    putr8    unalias
 *                         close      getr8   info    prev    revert   wait
 *                         delete     help    next    put     select   write
 *                         display 
 *
 * Predicates are of the form TAG = 3 IMAGE_SIZE < 1000  LABEL = "abc"
 * Type <command> -help for usage of <command>.  DO NOT type the command
 * without arguments and expect help.  Some commands delete objects and do not
 * need any arguments.  If you are learning to use he, try it on an expendable
 * file.
 *****************************************************************************/
/* ------ he.c ------- main() main HDF interfacing routines */
#include "he.h"
#include "herr.h"

/* the return status of last command executed */
int he_status = HE_OK;

/* is this on the console or remote terminals?
   this should eventually be detected automatically */
int he_remote = YES;

/* is this batch mode or interactive? */
extern int he_batch;

#ifdef PROTOTYPE
int main(int argc, char *argv[])
#else
int main(argc, argv)
    int argc;
    char *argv[];
#endif
{
    int backup = YES;		/* Backup files when opening? */
    register int i;
    char *fileName = NULL;
    
    for (i = 1; i < argc; i++) {
	if (argv[i][0] == '-') {
	    switch (findOpt(argv[i] + 1)) {
            case HE_HELP:
		printf("he [<file>] [-nobackup] [-batch]\n");
		help();
		quit(0);
                break;
	    case HE_BATCH:
		he_batch = YES;
		break;
	    case HE_REMOTE:
		he_remote = YES;
		break;
	    case HE_NOBACKUP:
		backup = NO;
		break;
	    case HE_BACKUP:
		backup = YES;
		break;
	    case HE_NOTFOUND:
		unkOpt(argv[i]);
		quit(1);	/* does not return */
		break;
	    case HE_AMBIG:
		ambigOpt(argv[i]);
		quit(1);
		break;
	    default:
		irrOpt(argv[i]);
		quit(1);	/* does not return */
		break;
	    }
	}
	else
	{
	    /* Must be a filename */
	    if (!fileName)
		fileName = argv[i];
	    else
		fprintf(stderr, "Single file only. %s not open.\n", argv[i]);
	}
    }

    /* if there is a file name given in the command line, open it */
    if (fileName) he_status = openFile(fileName, backup);

    /* read, execute loop */
    cmdLoop();

    if (fileOpen()) close(YES);	/* close with keep */
    quit(0);
    return 0;
}

/* cmdLoop -- read commands and execute them */
#ifdef PROTOTYPE
void cmdLoop(void)
#else
void cmdLoop()
#endif
{
    HE_CMD *cmd;

    for (cmd = getCmd(); cmd; cmd = getCmd())
    {
	if (cmd->func)
	    he_status = (*cmd->func) (cmd);
	else
	{
	    fprintf(stderr, "Unknown command: %s.\n", cmd->argv[0]);
	    he_status = HE_FAIL;
	}
	deleteCmd(cmd);
    }
}

int he_currDesc;
int he_numDesc;
int he_numGrp;
int he_backup;
char *he_file;
DFdesc he_desc[HE_DESC_SZ];
HE_GROUP he_grp[HE_DESC_SZ];
	
#ifdef PROTOTYPE
int32 getElement(int desc, char **pdata)
#else
int32 getElement(desc, pdata)
    int desc;
    char **pdata;
#endif
{
    int32 length;
    int32 fid;

    length = he_desc[desc].length;

    /* alloc memory to read the element in */
    *pdata = (char *) malloc(length);
    if (*pdata == NULL) return FAIL;

    /* read in the element and check for error */
    if ((fid = Hopen(he_file, DFACC_READ, 0)) == (int32)NULL) {
      HEprint(stderr, 0);
      return FAIL;
    }
    if (Hgetelement(fid, he_desc[desc].tag, he_desc[desc].ref, (unsigned char *) (*pdata)) < 0) {
      free(*pdata);
      fprintf(stderr, "Cannot read element.\n");
      return FAIL;
    }
    Hclose(fid);
    return length;
}

#ifdef PROTOTYPE
int od(char *format, char *file)
#else
int od(format, file)
    char *format;
    char *file;
#endif
{
    /* fork a child and let the child run od */
    if (fork() == 0)
    {
	/* this is the child */
	if (execl("/bin/od", "od", format, file, 0) == -1)
	    fprintf(stderr, "Error while executing od.\n");

	/* return control to the parent */
	exit(0);
    }

    /* the parent waits for the child to die */
    wait(0);

    /* this is a bug because it always returns OK, will expand this as
       soon as the status return mechanism from wait is understood */
    return HE_OK;
}

/* the tmp directory, currently set for unix */
#define TDIR "/tmp/"

#ifdef PROTOTYPE
int getTmpName(char **pname)
#else
int getTmpName(pname)
    char **pname;
#endif
{
    int length;
    static int count = 0;
    char s[32];

    (void) sprintf(s, "%she%d.%d", TDIR, getpid(), count);
    count++;

    length = strlen(s);
    if (length <= 0) return FAIL;

    *pname = (char *) malloc(length+1);
    strcpy(*pname, s);

    return length;
}

#ifdef PROTOTYPE
int writeToFile(char *file, char *data, int32 length)
#else
int writeToFile(file, data, length)
    char *file;
    char *data;
    int32 length;
#endif
{
    FILE *fd;
    int written;

    fd = fopen(file, "w");
    if (fd == NULL) return FAIL;

    written = fwrite(data, sizeof(char), length, fd);
    if (written != length)
    {
	fprintf(stderr,"Error in write.\n");
	return FAIL;
    }
    fclose(fd);

    return HE_OK;
}

#ifdef PROTOTYPE
int removeFile(char *file)
#else
int removeFile(file)
    char *file;
#endif
{
    return unlink(file);
}

/* is a file currently opened */
#ifdef PROTOTYPE
int fileOpen(void)
#else
int fileOpen()
#endif
{
    return (he_file != NULL);
}

#ifdef PROTOTYPE
char *backupName(char *file)
#else
char *backupName(file)
    char *file;
#endif
{
    return catStr("$",file);
}

#ifdef PROTOTYPE
int backupFile(char *file)
#else
int backupFile(file)
    char *file;
#endif
{
    char *back;			/* backup file name */

    back = backupName(file);
    return copyFile(file, back);
}

#ifdef PROTOTYPE
int copyFile(char *from, char *to)
#else
int copyFile(from, to)
    char *from, *to;
#endif
{
    int read;
    char buf[HE_BUF_SZ];	/* copying buffer */
    FILE *fp;
    FILE *bfp;

    /* open the hdf file for backing up */
    if ((fp = fopen(from, "r")) == NULL)
    {
	fprintf(stderr,"Unable to open file: <%s>\n",from);
	return FAIL;
    }
    if ((bfp = fopen(to, "w")) == NULL)
    {
	fclose(fp);
	fprintf(stderr,"Unable to open backup file.\n");
	return FAIL;
    }
    /* copy the contents from hdf file to backup file */
    while ((read = fread(buf, 1, HE_BUF_SZ, fp)) > 0)
	fwrite(buf, 1, read, bfp);

    fclose(fp);
    fclose(bfp);

    return HE_OK;
}

#ifdef PROTOTYPE
int updateDesc(void)
#else
int updateDesc()
#endif
{
  uint32 fid;
  int32 aid, status;
  register int i, j;
  
  if(!(fid = Hopen(he_file, DFACC_READ, 0))) {
    printf("failed opening\n");
    HEprint(stdout, 0);
    return FAIL;
  }    
  
  aid = Hstartread(fid, DFTAG_WILDCARD, DFREF_WILDCARD);
  if(aid == FAIL) {
    HEprint(stderr, 0);
    return FAIL;
  }
  
  status = SUCCEED;
  for(i = 0; (i < HE_DESC_SZ) && (status != FAIL); i++) {
    Hinquire(aid, NULL, &he_desc[i].tag, &he_desc[i].ref, &he_desc[i].length,
             &he_desc[i].offset, NULL, (int16 *) NULL, (int16 *) NULL);
    status = Hnextread(aid, DFTAG_WILDCARD, DFREF_WILDCARD, DF_CURRENT);
  }
  he_numDesc = i;    
  
  /* get informations about the groups */
  he_numGrp = 0;
  for (i = 0; i < he_numDesc; i++) {
    if (isGrp(he_desc[i].tag)) {
      he_grp[he_numGrp].desc = i;
      he_grp[he_numGrp].size = he_desc[i].length / sizeof(tag_ref);
      he_grp[he_numGrp].ddList = (tag_ref_ptr) malloc(he_desc[i].length);
      
      if (!he_grp[he_numGrp].ddList) {
        fprintf(stderr, "Out of memory. Closing file.\n");
        closeFile(1);	/* keep the backup */
        return FAIL;
      }
      if (DFdiread(fid, he_desc[i].tag, he_desc[i].ref) < 0) {
        HEprint(stderr, 0);
        return FAIL;
      }
      for (j = 0; j < he_grp[he_numGrp].size; j++)
        DFdiget(&he_grp[he_numGrp].ddList[j].tag, 
                &he_grp[he_numGrp].ddList[j].ref);
      
      he_numGrp++;
    }
  }
  Hclose(fid);
  return SUCCEED;
}


#ifdef PROTOTYPE
int initFile(char *file)
#else
int initFile(file)
    char *file;
#endif
{
    if (he_file) free(he_file);
    he_file = copyStr(file);

    if (updateDesc() < 0)
	return FAIL;

    /* if there are groups in this file, go to the first group tag */
    /* otherwise, just go to the first element */
    if (he_numGrp > 0)
	he_currDesc = he_grp[0].desc;
    else
	he_currDesc = 0;

    return resetPred();
}

#ifdef PROTOTYPE
int closeFile(int keep)
#else
int closeFile(keep)
    int keep;
#endif
{
    register int i;
    char *back;

    if (!fileOpen())
    {
	fprintf(stderr, "No open file to close.\n");
	return FAIL;
    }
    /* free some dynamic storages */
    if (he_backup && !keep)
    {
	back = backupName(he_file);
	(void) removeFile(back);
	free(back);
    }
    free(he_file);
    he_file = NULL;
    for (i = 0; i < he_numGrp; i++)
	free(he_grp[i].ddList);

    return HE_OK;
}

#ifdef PROTOTYPE
int getR8(int xdim, int ydim, char *image, char *pal, int comp)
#else
int getR8(xdim, ydim, image, pal, comp)
    int xdim, ydim;
    char *image;
    char *pal;
    int comp;
#endif
{
    FILE *fp;
    int32 length;
    char *buf;

    if (!fileOpen())
    {
	noFile();
	return FAIL;
    }
    if (pal) 
	if (setPal(pal) < 0)
	    /* Error already signalled by setPal */
	    return FAIL;

    length = xdim * ydim;
    buf = (char *) malloc(length);

    if ((fp = fopen(image, "r")) == NULL)
    {
	fprintf(stderr,"Error opening image file: %s.\n", image);
	return FAIL;
    }
    if (fread(buf, xdim, ydim, fp) < ydim)
    {
	fprintf(stderr,"Error reading image file: %s.\n", image);
	return FAIL;
    }
    fclose(fp);

    if (DFR8addimage(he_file, buf, xdim, ydim, comp) < 0)
    {
	HEprint(stderr, 0);
	return FAIL;
    }
    free(buf);

    if (updateDesc() < 0)
	return FAIL;

    return HE_OK;
}

#ifdef PROTOTYPE
int setPal(char *pal)
#else
int setPal(pal)
    char *pal;
#endif
{
    FILE *fp;
    char reds[HE_COLOR_SZ], greens[HE_COLOR_SZ], blues[HE_COLOR_SZ];
    char palette[HE_PALETTE_SZ];
    register char *p;
    register int i;

    if ((fp = fopen(pal, "r")) == NULL)
    {
	fprintf(stderr,"Error opening palette file: %s.\n", pal);
	return FAIL;
    }
    if (fread(reds, 1, HE_COLOR_SZ, fp) < HE_COLOR_SZ ||
	fread(greens, 1, HE_COLOR_SZ, fp) < HE_COLOR_SZ ||
	fread(blues, 1, HE_COLOR_SZ, fp) < HE_COLOR_SZ)
    {
	fprintf(stderr, "Error reading palette file: %s.\n", pal);
	return FAIL;
    }

    /* convert sun palette to hdf palette */
    p = palette;
    for (i = 0; i < HE_COLOR_SZ; i++)
    {
	*p++ = reds[i];
	*p++ = greens[i];
	*p++ = blues[i];
    }

    if (DFR8setpalette((uint8*) palette) < 0)
    {
	fputs("Error setting palette.\n",stderr);
	return FAIL;
    }

    return HE_OK;
}

#ifdef PROTOTYPE
int findDesc(tag_ref_ptr dd)
#else
int findDesc(dd)
    tag_ref_ptr dd;
#endif
{
    register int i;

    for (i = 0; i < he_numDesc; i++)
	if ((he_desc[i].tag == dd->tag) && (he_desc[i].ref == dd->ref))
	    return i;

    return FAIL;
}

#ifdef PROTOTYPE
int desc2Grp(int desc)
#else
int desc2Grp(desc)
    int desc;
#endif
{
    register int i;

    for (i = 0; i < he_numGrp; i++)
	if (he_grp[i].desc == desc) return i;

    NOT_REACHED();
    return FAIL;
}

#ifdef PROTOTYPE
int hasReference(int desc)
#else
int hasReference(desc)
    int desc;
#endif
{
    register int i, j;

    for (i = 0; i < he_numGrp; i++)
	for (j = 0; j < he_grp[i].size ; j++)
	    if (he_grp[i].ddList[j].tag == he_desc[desc].tag &&
		he_grp[i].ddList[j].ref == he_desc[desc].ref)
		return YES;
    return NO;
}

#ifdef PROTOTYPE
int deleteDesc(int desc)
#else
int deleteDesc(desc)
    int desc;
#endif
{
    int32 fid;

    if ((fid = Hopen(he_file, DFACC_WRITE, 0)) == (int32)NULL) {
      HEprint(stderr, 0);
      return FAIL;
    }

    if (Hdeldd(fid, he_desc[desc].tag, he_desc[desc].ref) == FAIL)
    {
	HEprint(stderr, 0);
	return FAIL;
    }
    return Hclose(fid);
}

#ifdef PROTOTYPE
int getCurrRig(int32 *pXdim, int32 *pYdim, char **pPalette, char **pRaster)
#else
int getCurrRig(pXdim, pYdim, pPalette, pRaster)
    int32 *pXdim, *pYdim;
    char **pPalette;
    char **pRaster;
#endif
{
    int ispal;

    goTo(he_currDesc);

    if (DFR8getdims(he_file, pXdim, pYdim, &ispal)<0)
    {
	fprintf(stderr, "Error getting image group.\n");
	HEprint(stderr, 0);
	return FAIL;
    }
    if (ispal)
	*pPalette = (char *) malloc(HE_PALETTE_SZ);
    else
	*pPalette = (char *) NULL;
    *pRaster = (char *) malloc((*pXdim) * (*pYdim));

    if (DFR8getimage(he_file, (unsigned char *) *pRaster, *pXdim, *pYdim, 
                     (unsigned char *) *pPalette) == FAIL) 
    {
	fprintf(stderr, "Error getting image group.\n");
	HEprint(stderr, 0);
	return FAIL;
    }

    return HE_OK;
}

#ifdef PROTOTYPE
int putWithTempl(char *template, int n1, int n2, int n3, char *data, int length, int verbose)
#else
int putWithTempl(template, n1, n2, n3, data, length, verbose)
    char *template;
    int n1, n2, n3;
    char *data;
    int length;
    int verbose;
#endif
{
    char *file;
    int ret;

    convertTemplate(template, n1, n2, n3, &file);
    if (verbose)
	printf("Writing to file: %s\n", file);
    ret = writeToFile(file, data, length);
    free(file);

    return ret;
}

#ifdef PROTOTYPE
int revert(void)
#else
int revert()
#endif
{
    char *back;

    back = backupName(he_file);
    if (copyFile(back, he_file) < 0)
	return FAIL;
    return initFile(he_file);
}

#ifdef PROTOTYPE
int writeElt(char *file, uint16 ref, int elt)
#else
int writeElt(file, ref, elt)
    char *file;
    uint16 ref;
    int elt;
#endif
{
    int ret;
    char *data;
    int32 eltLength;
    char *p;
    uint16 rank;
    register int i;
    uint16 ntTag;
    uint16 ntRef;
    tag_ref_ptr ntDesc;
    int nt;

    eltLength = getElement(elt, &data);
    if (eltLength <= 0) {
      fprintf(stderr,"Cannot get element: tag %d ref %d.\n",
              he_desc[elt].tag, he_desc[elt].ref);
      return FAIL;
    }
    /* special case */

    if (he_desc[elt].tag == DFTAG_SDD) {

      /* lots of hack here */
      /* assume that the number types are of the same ref as this elt */
      p = data;
      
      /* get the rank */
      /* NOTE: UINT16READ and UINT16WRITE advances p */
      UINT16DECODE(p, rank);
      /* move to the NT of the data */
      p += (rank * 4);
      UINT16DECODE(p, ntTag);
      UINT16DECODE(p, ntRef);
      
      /* set up to write the number type element */
      ntDesc = (tag_ref_ptr) malloc(sizeof(tag_ref));
      ntDesc->tag = ntTag;
      ntDesc->ref = ntRef;
      nt = findDesc(ntDesc);
      free(ntDesc);
      writeElt(file, ref, nt);
      
      p -= 2;
      UINT16ENCODE(p, ref);
      /* do the NT of scales */
      for (i = 0; i < rank; i++) {
        p += 2;
        UINT16ENCODE(p, ref);
      }
    }

    ret = putElement(file, he_desc[elt].tag, ref, data, eltLength);
    free(data);
    return ret;
}

#ifdef PROTOTYPE
int putElement(char *file, uint16 tag, uint16 ref, char *data, int32 len)
#else
int putElement(file, tag, ref, data, len)
    char *file;
    uint16 tag;
    uint16 ref;
    char *data;
    int32 len;
#endif
{
    int ret;
    int32 fid;

    if ((fid = Hopen(file, DFACC_READ | DFACC_WRITE, 0)) == (uint32)NULL)
	/* a little tricky here */
	if (HEvalue(0) != DFE_FNF || (fid = Hopen(file, DFACC_ALL, 0)) == (uint32)NULL)
	{
	    HEprint(stderr, 0);
	    return FAIL;
	}
    if ((ret = Hputelement(fid, tag, ref, (unsigned char *) data, len)) < 0)
    {
	HEprint(stderr, 0);
	return ret;
    }
    return Hclose(fid);
}

#ifdef PROTOTYPE
int writeGrp(char *file)
#else
int writeGrp(file)
    char *file;
#endif
{
    register int i;
    uint16 ref;
    int grp;
    int elt;
    int ret;
    int32 fid;

    getNewRef(file, &ref);

    grp = currGrpNo();
    DFdisetup(he_grp[grp].size);
    for (i = 0; i < he_grp[grp].size; i++) {
      elt = findDesc(he_grp[grp].ddList + i);
      if (elt >= 0)
        writeElt(file, ref, elt);
      /* update the group dd list */
      DFdiput(he_grp[grp].ddList[i].tag, ref);
    }
    /* do the group now */

    if ((fid = Hopen(file, DFACC_READ | DFACC_WRITE, 0)) == (uint32)NULL)
    {
	HEprint(stderr, 0);
	return FAIL;
    }
    if ((ret = DFdiwrite(fid, currTag(), ref)) < 0)
    {
	HEprint(stderr, 0);
	return ret;
    }
    return Hclose(fid);
}

#ifdef PROTOTYPE
int getNewRef(char *file, uint16 *pRef)
#else
int getNewRef(file, pRef)
    char *file;
    uint16 *pRef;
#endif
{
    int32 fid;

    if ((fid = Hopen(file, DFACC_READ | DFACC_WRITE, 0)) == (uint32)NULL)
	/* a little tricky here */
	if (HEvalue(0) != DFE_FNF || (fid = Hopen(file, DFACC_ALL, 0)) == (uint32)NULL)
	{
	    HEprint(stderr, 0);
	    return FAIL;
	}
    *pRef = Hnewref(fid);
    return Hclose(fid);
}

#ifdef PROTOTYPE
int writeAnnot(char *file, uint16 tag, uint16 ref)
#else
int writeAnnot(file, tag, ref)
    char *file;
    uint16 tag;
    uint16 ref;
#endif
{
    char *data;
    int32 eltLength;
    intn tmp;
    char *p;
    uint16 newRef;

    while (tag == 0) {
      printf("Attach to what tag? (> 0)");
      scanf("%d", &tmp);
      tag = (uint16) tmp;
    }

    while (ref == 0) {
      printf("Attach to what ref? (> 0)");
      scanf("%d", &tmp);
      ref = (uint16) tmp;
    }

    eltLength = getElement(he_currDesc, &data);
    if (eltLength <= 0) {
      fprintf(stderr,"Cannot get element: tag %d ref %d.\n",
              he_desc[he_currDesc].tag, he_desc[he_currDesc].ref);
      return FAIL;
    }

    p = data;

    /*
     *  This is really ugly...
     */
    UINT16ENCODE(p, tag);
    UINT16ENCODE(p, ref);

    if (getNewRef(file, &newRef) < 0) {
      fprintf(stderr,"Error getting new ref number.\n");
      return FAIL;
    }
    
    return putElement(file, he_desc[he_currDesc].tag, newRef, data, eltLength);
}

#ifdef PROTOTYPE
int32 getAnn(int ann, int16 tag, int16 ref, char **pBuf)
#else
int32 getAnn(ann, tag, ref, pBuf)
    int ann;
    uint16 tag;
    uint16 ref;
    char **pBuf;
#endif
{
    int32 len;

    if (ann == HE_LABEL)
    {
	len = DFANgetlablen(he_file, tag, ref);
	if (len > 0)
	{
	    *pBuf = (char *) malloc(len+1);
	    DFANgetlabel(he_file, tag, ref, *pBuf, len+1);
	}
	else
	    *pBuf = NULL;
    }
    else
    {
	len = DFANgetdesclen(he_file, tag, ref);
	if (len > 0)
	{
	    *pBuf = (char *) malloc(len);
	    DFANgetdesc(he_file, tag, ref, *pBuf, len);
	}
	else
	    *pBuf = NULL;
    }
    return len;
}

#ifdef PROTOTYPE
int putAnn(int ann, uint16 tag, uint16 ref, char *buf, int32 len)
#else
int putAnn(ann, tag, ref, buf, len)
    int ann;
    uint16 tag;
    uint16 ref;
    char *buf;
    int32 len;
#endif
{
    int ret;

    if (ann == HE_LABEL)
	ret = DFANputlabel(he_file, tag, ref, buf);
    else
	ret = DFANputdesc(he_file, tag, ref, buf, len);
    if (ret < 0)
	HEprint(stderr, 0);

    return ret;
}

#ifdef PROTOTYPE
int32 readFromFile(char *file, char **pBuf)
#else
int32 readFromFile(file, pBuf)
    char *file;
    char **pBuf;
#endif
{
    FILE *fp;
    int32 soFar;
    int32 bufLen;
    int32 read;

    fp = fopen(file, "r");
    if (fp == NULL) return FAIL;

    soFar = 0;
    bufLen = 0;
    for (read = HE_BUF_SZ; read == HE_BUF_SZ; soFar += read)
    {
	bufLen += HE_BUF_SZ;
	if (bufLen == HE_BUF_SZ)
	    *pBuf = (char *) malloc(bufLen);
	else
	    *pBuf = (char *) realloc(*pBuf, bufLen);
	if (*pBuf == NULL) return FAIL;

	read = fread((*pBuf)+soFar, 1, HE_BUF_SZ, fp);
    }
    *pBuf = (char *) realloc(*pBuf, soFar + 1);
    (*pBuf)[soFar] = '\0';
    fclose(fp);
    return soFar;
}

/* ---- table for operators -------- */
struct {
    char *str;
    int key;
} he_optTab[] =
{
{"readonly", HE_RDONLY},
{"all", HE_ALL},
{"backup", HE_BACKUP},
{"batch", HE_BATCH},
{"help", HE_HELP},
{"longout", HE_LONGOUT},
{"nobackup", HE_NOBACKUP},
{"remote", HE_REMOTE},
{"verbose", HE_VERBOSE},
{"position", HE_POSITION},
{"expansion", HE_EXPANSION},
{"large", HE_LARGE},
{"offset", HE_OFFSET},
{"ascii", HE_ASCII},
{"octal", HE_OCTAL},
{"hexadecimal", HE_HEX},
{"decimal", HE_DECIMAL},
{"float", HE_FLOAT},
{"dimensions", HE_DIMS},
{"image", HE_IMAGE},
{"palette", HE_PALETTE},
{"raster", HE_RASTER},
{"rle", HE_RLE},
{"compress", HE_RLE},
{"imcomp", HE_IMCOMP},
{"group", HE_DOGROUP},
{"file", HE_FILE},
{"keep", HE_KEEP},
{"length", HE_LENGTH},
{"attachto", HE_ATTACHTO},
{"label", HE_LABEL},
{"descriptor", HE_DESCRIPTOR},
{"editor", HE_EDITOR},
{NULL, 0},
};

#ifdef PROTOTYPE
int findOpt(char *word)
#else
int findOpt(word)
    char *word;
#endif
{
    int len;
    int found = -1;
    register int i;

    len = strlen(word);

    for (i = 0; he_optTab[i].str; i++)
	if (!strncmp(he_optTab[i].str, word, len))
	{
	    /* exact match */
	    if (strlen(he_optTab[i].str) == len)
		return he_optTab[i].key;

	    if (found < 0)
		found = i;
	    else
		return HE_AMBIG;
	}

    if (found < 0)
	return HE_NOTFOUND;

    return he_optTab[found].key;
}

#ifdef PROTOTYPE
char * catStr(char *s, char *s1)
#else
char * catStr(s, s1)
    register char *s, *s1;
#endif
{
    char *t;

    t = (char *) malloc(strlen(s) + strlen(s1) + 1);
    strcpy(t, s);
    strcat(t, s1);
    return t;
}

#ifdef PROTOTYPE
char *copyStr(char *s)
#else
char *copyStr(s)
    register char *s;
#endif
{
    char *t;

    t = (char *) malloc(strlen(s) + 1);
    strcpy(t, s);
    return t;
}

#ifdef PROTOTYPE
int isGrp(uint16 tag)
#else
int isGrp(tag)
    uint16 tag;
#endif
{
    switch(tag)
    {
    case DFTAG_RIG:
    case DFTAG_SDG:
    case DFTAG_NDG:
      /* and other group tags */
      return 1;
    default:
      return 0;
    }
}

#ifdef PROTOTYPE
int HEquit(HE_CMD *cmd)
#else
int HEquit(cmd)
    HE_CMD *cmd;
#endif
{
    if (cmd->argc > 1)
    {
	puts("quit");
	puts("\tQuits this application.");
	return HE_OK;
    }
    return quit(0);
}

#ifdef PROTOTYPE
int quit(int status)
#else
int quit(status)
    int status;
#endif
{
    if (fileOpen())
    {
	if (closeFile(0) < 0)
	    return HE_FAIL;
    }
    exit(status);
    return SUCCEED;
}

#ifdef PROTOTYPE
int HEhelp(HE_CMD *dummy)
#else
int HEhelp(dummy)
     HE_CMD *dummy;
#endif
{
    help();
    return HE_OK;
}

#ifdef PROTOTYPE
void help(void)
#else
void help()
#endif
{
  /* print some help informations */
  printf("hdfed allows sophisticated HDF users to manipulate the elements in");
  printf(" an HDF file.\nThese manipulations include selecting groups and ");
  printf("showing information about\nthem, dumping them to the output, ");
  printf("writing them to new files, deleting them,\ninserting them, ");
  printf("replacing, say, the palette of an r8 group, and editing the\n");
  printf("text labels and descriptions of any element.\n\n");

  printf("hdfed will NOT allow the user to *arbitrarily* modify binary data ");
  printf("in the file or\nany element, though it allows modification of tag ");
  printf("and reference numbers within\nstrict constraints.  The user should");
  printf(" not attempt to alter individual bytes.  It\nis acceptable,");
  printf(" however, to replace an element with another of the same type.\n\n");

  printf("hdfed can be used both interactively or in 'batch' mode.  See the ");
  printf("\"HDF Calling\nInterfaces and Utilities\" manual section on hdfed ");
  printf("for an example.\n\n");

  printf("List of commands:");
  printf(    "\tannotate   dump    if      open    putr8    unalias\n");
  printf("\t\t\tclose      getr8   info    prev    revert   wait\n");
  printf("\t\t\tdelete     help    next    put     select   write\n");

  printf("\nPredicates for 'if' 'select' 'next' and 'prev' are of the form\n");
  printf("\ttag = 3 ref = 2 image_size < 1000 label = \"abc\"\n");
  printf("Type <command> -help for usage of <command>.  DO NOT type the");
  printf(" command without\narguments and expect help.  Some commands delete");
  printf(" objects and do not need any\narguments.  If you are learning to ");
  printf("use hdfed, try it on an expendable file.\n");

}

int HEwait(cmd)
    HE_CMD *cmd;
{
    int c;

    if (cmd->argv[1][0] == '-' && findOpt(cmd->argv[1] + 1) == HE_HELP) {
      printf("wait [<message>]\n");
      printf("\tPrints message and then wait for user to hit return\n");
      return HE_OK;
    }

    printf("%s\nPress return to continue.", cmd->argv[1]);
    do c = getchar();
    while (c != '\n');

    return HE_OK;
}

#ifdef PROTOTYPE
void deleteCmd(HE_CMD *cmd)
#else
void deleteCmd(cmd)
    HE_CMD *cmd;
#endif
{
    register int i;

    if (cmd == NULL) return;
    if (cmd->next != NULL) deleteCmd(cmd->next);
    if (cmd->sub != NULL) deleteCmd(cmd->sub);
    for (i = 0; i < cmd->argc; i++)
	if (cmd->argv[i] != NULL) free(cmd->argv[i]);
    free(cmd);
}

/* -------------- routines to manipulate templates --------------------- */

#define TEMPLATE_CHAR1 '#'
#define TEMPLATE_CHAR2 '@'
#define TEMPLATE_CHAR3 '%'

#ifdef PROTOTYPE
void convertTemplate(char *template, int n1, int n2, int n3, char **pname)
#else
void convertTemplate(template, n1, n2, n3, pname)
    char *template;
    int n1, n2, n3;
    char **pname;
#endif
{
    char s1[20], s2[20], s3[20];
    char *t;

    sprintf(s1, "%1d", n1);
    sprintf(s2, "%1d", n2);
    sprintf(s3, "%1d", n3);

    *pname = t = (char *) malloc(strlen(template) + 61);

    while (*template)
	switch(*template)
	{
	case TEMPLATE_CHAR1:
	    fillTemplate(&template, &t, s1, TEMPLATE_CHAR1);
	    break;
	case TEMPLATE_CHAR2:
	    fillTemplate(&template, &t, s2, TEMPLATE_CHAR2);
	    break;
	case TEMPLATE_CHAR3:
	    fillTemplate(&template, &t, s3, TEMPLATE_CHAR3);
	    break;
	default:
	    *t++ = *template++;
	}
    *t = '\0';
}

#ifdef PROTOTYPE
void fillTemplate(char **template, char **pout, char *s, char templateChar)
#else
void fillTemplate(template, pout, s, templateChar)
    char **template;
    char **pout;
    char *s;
    char templateChar;
#endif
{
    int templateLen, sLen;

    /* count length of template to replace */
    for (templateLen = 0; **template == templateChar;
	 (*template)++, templateLen++);
    sLen = strlen(s);

    /* fill with zero's if the space reserved in template is
       longer than the length of s */
    for (; templateLen > sLen; templateLen--)
	*(*pout)++ = '0';

    while (*s) *(*pout)++ = *s++;
}

/* end of he-main.c */

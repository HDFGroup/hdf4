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

/* --- he-file.c  --- file and annotation manipulation routines */
#include "he.h"
#ifdef VMS
#   include descrip
#endif

#ifdef PROTOTYPE
int HEannotate(HE_CMD *cmd)
#else
int HEannotate(cmd)
    HE_CMD *cmd;
#endif
{
    register int i;
    char *editor = NULL;
    int ann = HE_LABEL;

    for (i = 1; i < cmd->argc; i++)
	if (cmd->argv[i][0] == '-')
	    switch (findOpt(cmd->argv[i] + 1))
	    {
	    case HE_HELP:
		puts("annotate [-label|-descriptor] [-editor <editor>]");
		puts("\tEdit an annotation");
		puts("\t-label\t\tEdit label (default)");
		puts("\t-descriptor\tEdit descriptor");
		puts("\t-editor\t\tUse editor (default EDITOR env value)");
		return HE_OK;
	    case HE_LABEL:
		ann = HE_LABEL;
		break;
	    case HE_DESCRIPTOR:
		ann = HE_DESCRIPTOR;
		break;
	    case HE_EDITOR:
		if (++i < cmd->argc) editor = cmd->argv[i];
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
    return annotate(editor, ann);
}

/* Edit annontations (labels and descriptions) for the
 * current data element */
#ifdef PROTOTYPE
int annotate(char *editor, int ann)
#else
int annotate(editor, ann)
    char *editor;		/* editor to use */
    int ann;			/* HE_LABEL  or HE_DESCRIPTOR */
#endif
{

#if !defined MAC && !defined PC

    int32 len;			/* length of annotation */
    char *buf;			/* annotation buffer */
    char *file;			/* tmp file name */
    register int i;
    int ret;

    /* check if any hdf file is open
     */
    if (!fileOpen())
    {
	noFile();
	return HE_FAIL;
    }

    /* Get the annotation from hdf file
     */
    len = getAnn(ann, currTag(), he_desc[he_currDesc].ref, &buf);

    /* make sure some editor will be used
     * defaults to /usr/bin/ex
     * but should be made a compile time option
     */
/*    if (editor == NULL)
    {
	char *getenv();
	editor = getenv("EDITOR");
	if (editor == NULL) editor = "/usr/bin/ex";
    }
*/
    (void) getTmpName(&file);

    /* if there is prior annotation, put it into the tmp file
     */
    if (len > 0 && buf != NULL)
    {
	/* doctor the buffer a little for the editors
	 */
	if (ann == HE_LABEL)
	    buf[len] = '\n';
	else
	    len--;
	writeToFile(file, buf, len+1);
	free(buf);
    }

#ifndef VMS
   /* make sure some editor will be used
     * defaults to /usr/bin/ex
     * but should be made a comple time option
     */
    if (editor == NULL)
    {
        char *getenv();
        editor = getenv("EDITOR");
        if (editor == NULL) editor = "/usr/bin/ex";
    }
    if (fork() == 0)
    {
	/* this is the child */
	if (execl(editor, editor, file, 0) == -1)
	    fprintf(stderr, "Error while editing label.\n");

	/* return control to the parent if exec fails
	 */
	exit(0);
    }
#else	/* VMS  */
    if (vfork() == 0)
        /* this is the child */
    {
     intn ret_status;
     $DESCRIPTOR (input_file, file);
     $DESCRIPTOR (output_file, file);
     ret_status = TPU$EDIT(&input_file, &output_file);
     fprintf( "TPU$EDIT return status: %d. \n",ret_status);
     exit(0);
    }
#endif
    
    /* the parent waits for the child to die */
    wait(0);

    /* read in edited annotation
     */
    len = readFromFile(file, &buf);
    if (len <= 0 || buf == NULL) return len;

    /* doctor the buffer for standard format
     */
    if (ann == HE_LABEL)
    {
	/* take out control characters from the end */
	for (i = len; i >= 0 && !isgraph(buf[i]); i--)
	    ;
	buf[i+1] = '\0';
    }

    /* forget the tmp file
     */
    removeFile(file);

    /* write annotation to the hdf file
     */
    ret = putAnn(ann, currTag(), he_desc[he_currDesc].ref, buf, len);
    updateDesc();

    /* clean up
     */
    free(buf);
    return ret;

#else

	return 1;
	
#endif /* ndef MAC & PC */

}

extern int he_backup;

#ifdef PROTOTYPE
int HEopen(HE_CMD *cmd)
#else
int HEopen(cmd)
     HE_CMD *cmd;
#endif
{
  int backup = YES;
  register int i;
  char *file = NULL;
  
  for (i = 1; i < cmd->argc; i++)
    if (cmd->argv[i][0] != '-') {
      if (!file)
        file = cmd->argv[i];
      else
        {
          fprintf(stderr, "Only one file allowed.\n");
          return FAIL;
        }
    }
    else
      switch (findOpt(cmd->argv[i] + 1))
        {
        case HE_HELP:
          puts("open <file> [-nobackup]");
          puts("\t-nobackup\tDon't make a backup for this file.");
          return HE_OK;
        case HE_NOBACKUP:
          backup = 0;
          break;
        case HE_NOTFOUND:
          unkOpt(cmd->argv[i]);
          return FAIL;
        case HE_AMBIG:
          ambigOpt(cmd->argv[i]);
          return FAIL;
        default:
          irrOpt(cmd->argv[i]);
          return FAIL;
        }
  
  if (!file) {
    fprintf(stderr, "Please specify a file name.\n");
    return FAIL;
  }
  
  return openFile(file, backup);
}


/* openFile -- Internal open file routine. */
/* Called by:	HEopen, main */
/* Returns:	HE_OK and FAIL */
#ifdef PROTOTYPE
int openFile(char *file, int backup)
#else
int openFile(file, backup)
    char *file;
    int backup;
#endif
{
    ASSERT(file);

    /* only allow one file at a time */
    if (fileOpen())
    {
	fprintf(stderr,"File: %s is still open. Close before reopening.\n",
		he_file);
	return FAIL;
    }

    /* Make backup first if necessary */
    if (backup) {
      if (backupFile(file) < 0)
        return FAIL;
      he_backup = 1;
    }
    else
      he_backup = 0;
    
    if (initFile(file) < 0)
      return FAIL;
    
    return HE_OK;
  }


/* HEclose -- close current file*/
/* Called by HEdoCmd */
#ifdef PROTOTYPE
int HEclose(HE_CMD *cmd)
#else
int HEclose(cmd)
    HE_CMD *cmd;
#endif
{
    register int i;
    int keep = NO;

    for (i = 1; i < cmd->argc; i++)
	if (cmd->argv[i][0] != '-')
	{
	    unkArg(cmd->argv[i]);
	    return FAIL;
	}
	else
	    switch (findOpt(cmd->argv[i] + 1))
	    {
	    case HE_HELP:
		puts("close [-keep]");
		puts("\t-keep\t\tDon't delete the backup file.");
		return HE_OK;
	    case HE_KEEP:
		keep = YES;
		break;
	    case HE_NOTFOUND:
		unkOpt(cmd->argv[i]);
		return FAIL;
	    case HE_AMBIG:
		ambigOpt(cmd->argv[i]);
		return FAIL;
	    default:
		irrOpt(cmd->argv[i]);
		return FAIL;
	    }

    return closeFile(keep);
}

#ifdef PROTOTYPE
int HErevert(HE_CMD *cmd)
#else
int HErevert(cmd)
    HE_CMD *cmd;
#endif
{
    if (cmd->argc < 2)
	return revert();

    if (cmd->argv[1][0] != '-' || findOpt(cmd->argv[1]+1) == HE_HELP)
    {
	puts("revert");
	puts("\tDiscard all changes.");
	return HE_OK;
    }
    return HE_FAIL;
}

#ifdef PROTOTYPE
int HEwrite(HE_CMD *cmd)
#else
int HEwrite(cmd)
    HE_CMD *cmd;
#endif
{
    register int i;
    char *file;
    uint16 ref = 0;
    uint16 tag = 0;

    if (cmd->argc < 2 ||
	(cmd->argv[1][0] == '-' && findOpt(cmd->argv[1]+1) == HE_HELP))
    {
	puts("write <file> [-attachto <atag> <aref>]");
	puts("\tWrite an element or group into another HDF file");
	puts("\t-attchto\tONLY for writing annontations");
	puts("\t\t\tWhat element to attach annotation to");
	return HE_OK;
    }

    file = cmd->argv[1];
    for (i = 2; i < cmd->argc; i++)
	if (cmd->argv[i][0] != '-')
	    file = cmd->argv[i];
	else
	    switch (findOpt(cmd->argv[i] + 1))
	    {
	    case HE_ATTACHTO:
		tag = (uint16) atoi(cmd->argv[++i]);
		ref = (uint16) atoi(cmd->argv[++i]);
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
    return writ(file, tag, ref);
}

#ifdef PROTOTYPE
int writ(char *file, uint16 tag, uint16 ref)
#else
int writ(file, tag, ref)
    char *file;
    uint16 tag;
    uint16 ref;
#endif
{
    int ret;
    uint16 ref1;

    if (!fileOpen())
    {
	noFile();
	return HE_FAIL;
    }
    if (!strcmp(file, he_file))
    {
	fprintf(stderr,"Cannot write to self.\n");
	return HE_FAIL;
    }

    /* handle special cases */
    if (isAnnot(currTag()))
	return writeAnnot(file, tag, ref);
    if (isGrp(currTag()))
	return writeGrp(file);

    if (getNewRef(file, &ref1) < 0)
	return HE_FAIL;

    ret = writeElt(file, ref1, he_currDesc);
    return ret;
}

/* --------------------------- get a r8 from a file ----------------------- */

#ifdef PROTOTYPE
int HEgetR8(HE_CMD *cmd)
#else
int HEgetR8(cmd)
    HE_CMD *cmd;
#endif
{
    register int i;
    int xdim = 0, ydim = 0;
    char *image, *pal = NULL;
    int compress = 0;

    if (cmd->argc < 4 || 
	(cmd->argv[1][0] == '-' && findOpt(cmd->argv[1]+1) == HE_HELP))
    {
	puts("getr8 <image> <xdim> <ydim> [-palette <palette>] [-raster|-rle|-imcomp]");
	puts("\tGet a r8 group from raw files");
	puts("\t-palette\tRaw palette file");
	puts("\t-raster\t\tNo compression (default)");
	puts("\t-rle\t\tRun-length compression");
	puts("\t-imcomp\t\tImcomp compression");
	return HE_OK;
    }
    image = cmd->argv[1];
    xdim = atoi(cmd->argv[2]);
    ydim = atoi(cmd->argv[3]);

    for (i = 4; i < cmd->argc; i++)
	if (cmd->argv[i][0] == '-')
	    switch(findOpt(cmd->argv[i]+1))
	    {
	    case HE_PALETTE:
		pal = cmd->argv[++i];
		break;
	    case HE_RASTER:
		compress = 0;
		break;
	    case HE_RLE:
		compress = HE_RLE;
		break;
	    case HE_IMCOMP:
		compress = HE_IMCOMP;
		break;
	    case HE_NOTFOUND:
		unkOpt(cmd->argv[i]);
		return FAIL;
	    case HE_AMBIG:
		ambigOpt(cmd->argv[i]);
		return FAIL;
	    default:
		irrOpt(cmd->argv[i]);
		return FAIL;
	    }
	else
	{
	    unkArg(cmd->argv[i]);
	    return FAIL;
	}

    if (!image)
    {
	fprintf(stderr,"No image file specified.\n");
	return FAIL;
    }
    if (xdim == 0 || ydim == 0)
    {
	fprintf(stderr,"No dimensions specified.\n");
	return FAIL;
    }
    return getR8(xdim, ydim, image, pal, compress);
}

/* --------------- generic put routines ---------------------- */

#ifdef PROTOTYPE
int HEput(HE_CMD *cmd)
#else
int HEput(cmd)
    HE_CMD *cmd;
#endif
{
    register int i;
    int verbose = NO;
    char *template = "elt#.@";

    for (i = 1; i < cmd->argc; i++)
	if (cmd->argv[i][0] == '-')
	    switch (findOpt(cmd->argv[i] + 1))
	    {
	    case HE_HELP:
		puts("put [-file <file>] [-verbose]");
		puts("\tPut the raw binary of this element in a file");
		puts("\t-file\t\tOut file name (default \"elt#.@\")");
		puts("\t-verbose\tOutput diagnostic info");
		return HE_OK;
	    case HE_FILE:
		template = cmd->argv[++i];
		break;
	    case HE_VERBOSE:
		verbose = YES;
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
    return put(template, verbose);
}

#ifdef PROTOTYPE
int put(char *template, int verbose)
#else
int put(template, verbose)
    char *template;
    int verbose;
#endif
{
    int length;
    char *data;
    int ret;

    length = getElement(he_currDesc, &data);
    if ((length <= 0) || (data == NULL)) return HE_FAIL;
    ret = putWithTempl(template, he_currDesc, length, 1, data, length,
		       verbose);
    free(data);

    return ret;
}

/* ------------------ routines to put an r8 into a file --------------------- */

#ifdef PROTOTYPE
int HEputR8(HE_CMD *cmd)
#else
int HEputR8(cmd)
    HE_CMD *cmd;
#endif
{
    register int i;
    int verbose = NO;
    char *image = "img#.@.%";
    char * pal = "pal#";

    for (i = 1; i < cmd->argc; i++)
	if (cmd->argv[i][0] == '-')
	    switch (findOpt(cmd->argv[i] + 1))
	    {
	    case HE_HELP:
		puts("putr8 [-image <img>] [-palette <pal>] [-verbose]");
		puts("\tPut an r8 group into raw image and palette files");
		puts("\t-image\t\tImage file name template (default \"img#.@.%\")");
		puts("\t-palette\tPalette file name template (default \"pal#\")");
		puts("\t-verbose\tTo give output of steps taken");
		return HE_OK;
	    case HE_IMAGE:
		image = cmd->argv[++i];
		break;
	    case HE_PALETTE:
		pal = cmd->argv[++i];
		break;
	    case HE_VERBOSE:
		verbose = YES;
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
    return putR8(image, pal, verbose);
}

#ifdef PROTOTYPE
int putR8(char *image, char *pal, int verbose)
#else
int putR8(image, pal, verbose)
    char *image;
    char *pal;
    int verbose;
#endif
{
    int ret;
    int32 xdim, ydim;
    char *palette;
    char *raster;

    if (!fileOpen())
    {
	noFile();
	return HE_FAIL;
    }
    if (!isRig(currTag()))
    {
	fprintf(stderr,"Current element not an image group.");
	return HE_FAIL;
    }
    getCurrRig(&xdim, &ydim, &palette, &raster);
    if (raster == NULL)
    {
	fprintf(stderr,"Cannot find raster.\n");
	return HE_FAIL;
    }
    ret = putWithTempl(image, he_currDesc, xdim, ydim, raster, xdim*ydim,
		       verbose);
    free(raster);
    if (ret < 0) return HE_FAIL;
    if (palette != NULL)
    {
	register int i;
	char p[HE_PALETTE_SZ];

	for (i = 0; i < HE_COLOR_SZ; i++)
	{
	    p[i] = *palette++;
	    p[HE_COLOR_SZ + i] = *palette++;
	    p[2 * HE_COLOR_SZ + i] = *palette++;
	}
	ret = putWithTempl(pal, he_currDesc, xdim, ydim, p,
			   HE_PALETTE_SZ, verbose);
	free(palette);
	if (ret < 0) return HE_FAIL;
    }

    return HE_OK;
}

/* end of he-file.c */

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

/* $Id$ */

/* Prototypes for hdfed functions */
int HEalias
  PROTO((HE_CMD *cmd));

int HEannotate
  PROTO((HE_CMD *cmd));

int HEclose
  PROTO((HE_CMD *cmd));

int HEdelete
  PROTO((HE_CMD *cmd));

int HEdisplay
  PROTO((HE_CMD *cmd));

int HEdump
  PROTO((HE_CMD *cmd));

int HEgetR8
  PROTO((HE_CMD *cmd));

int HEhelp
  PROTO((HE_CMD *cmd));

int HEif
  PROTO((HE_CMD *cmd));

int HEinfo
  PROTO((HE_CMD *cmd));

int HEnext
  PROTO((HE_CMD *cmd));

int HEopen
  PROTO((HE_CMD *cmd));

int HEprev
  PROTO((HE_CMD *cmd));

int HEput
  PROTO((HE_CMD *cmd));

int HEputR8
  PROTO((HE_CMD *cmd));

int HEquit
  PROTO((HE_CMD *cmd));

int HErevert
  PROTO((HE_CMD *cmd));

int HEselect
  PROTO((HE_CMD *cmd));

int HEunalias
  PROTO((HE_CMD *cmd));

int HEwait
  PROTO((HE_CMD *cmd));

int HEwrite
  PROTO((HE_CMD *cmd));

int annotate
  PROTO((char *editor,int ann));

int backupFile
  PROTO((char *file));

char *backupName
  PROTO((char *file));

int bigImg
  PROTO((unsigned char *targ, unsigned char *src));

char * catStr
  PROTO((char *s1, char *s2));

int closeFile
  PROTO((int keep));

void cmdLoop
  PROTO((void));

void convertTemplate
  PROTO((char *template, int n1, int n2, int n3, char **pname));

int copyFile
  PROTO((char *from, char *to));

char *copyStr
  PROTO((char *str));

int delete
  PROTO((int curr));

void deleteCmd
  PROTO((HE_CMD *cmd));

int deleteDesc
  PROTO((int desc));

int desc2Grp
  PROTO((int desc));

int display
  PROTO((int c, int x, int y, int f, int l));

int dump
  PROTO((int32 length, int offset, char *foramt, int raw));

int fileOpen
  PROTO((void));

void fillTemplate
  PROTO((char **template, char **pout, char *s, char templateChar));

HE_CMD *findAlias
  PROTO((char *str));

int findDesc
  PROTO((tag_ref_ptr dd));

HE_FUNC findFunc
  PROTO((char *word));

int findKey
  PROTO((char *word));

int findOpt
  PROTO((char *word));

int32 getAnn
  PROTO((int ann, int16 tag, int16 ref, char **pBuf));

HE_CMD *getCmd
    PROTO((void));

int getCurrRig
  PROTO((int32 *pXdim, int32 *pYdim, char **pPalette, char **pRaster));

int32 getElement
  PROTO((int desc, char **pdata));

int getLine
  PROTO((register char *));

int getNewRef
  PROTO((char *file,uint16 *pRef));

int getPix
  PROTO((void));

int getR8
  PROTO((int xdim, int ydim, char *image, char *pal, int compress));

int getSpace
  PROTO((void));

int getTmpName
  PROTO((char **pname));

void goTo
  PROTO((int desc));

int hasReference
  PROTO((int desc));

void help
  PROTO((void));

int info
  PROTO((int all, int longout, int group, int label));

void infoDesc
  PROTO((int desc, int longout, int label));

int initFile
  PROTO((char *file));

int isGrp
  PROTO((uint16 tag));

int isNumber
  PROTO((register char *s));

int largeSet
  PROTO((void));

int main
  PROTO((int argc, char *argv[]));

HE_CMD *mkDupCmd
  PROTO((HE_CMD *cmd));

char *nextToken
  PROTO((char **p));

char *nextWord
  PROTO((char **p));

int numCompare
  PROTO((int n1,int Comp, int n2));

int od
  PROTO((char *format, char *file));

int openFile
  PROTO((char *file, int backup));

HE_CMD *parse
  PROTO((void));

HE_CMD *parseCmd
  PROTO((char **p));

HE_PRED *parsePred
  PROTO((int argc,char *argv[]));

int pixImage
  PROTO((int usepal));

void printAlias
  PROTO((char *word,HE_CMD *cmd));

void prompt
  PROTO((void));

int put
  PROTO((char *template, int verbose));

int putAnn
  PROTO((int ann, uint16 tag, uint16 ref, char *buf, int32 len));

int putElement
  PROTO((char *file, uint16 tag, uint16 ref, char *data, int32 len));

int putR8
  PROTO((char *image, char *pal, int verbose));

int putWithTempl
  PROTO((char *template, int n1, int n2, int n3, char *data, int length, int verbose));

int quit
  PROTO((int status));

int rImage
  PROTO((int usepal));

int32 readFromFile
  PROTO((char *file, char **pBuf));

int recurseDel
  PROTO((int curr));

int removeFile
  PROTO((char *file));

int resetPred
  PROTO((void));

int revert
  PROTO((void));

int rleIt
  PROTO((char *buf, char *bufto, int len));

int satPred
  PROTO((DFdesc *desc, HE_PRED pred[]));

int setAlias
  PROTO((char *str,HE_CMD *cmd));

int setPal
  PROTO((char *pal));

int updateDesc
  PROTO((void));

int writ
  PROTO((char *file, uint16 tag, uint16 ref));

int writeAnnot
  PROTO((char *file, uint16 tag, uint16 ref));

int writeElt
  PROTO((char *file, uint16 ref, int elt));

int writeGrp
  PROTO((char *file));

int writeToFile
  PROTO((char *file, char *data, int32 length));

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

/***********************************************************
*
* Test program:  manf
*
* Stores annotations in a file using Multi-file interface
* Writes several SDSs and corresponding RISs to a file.
* Writes labels and descriptions for all 2 out of 3 SDSs.
* Writes labels and descriptions for all RISs.
*
*************************************************************/

#define TESTFILE "tman.hdf"

#include "tproto.h"
#include "mfan.h"

#define MAXLEN_LAB    50
#define MAXLEN_DESC  500
#define ROWS          10
#define COLS          10
#define REPS           3   /* number of data sets to write to file */

/* File labels/desriptions to write */
static uint8 *file_lab[2] = {"File label #1: aaa", "File label #2: bbbbbb"};
static uint8 *file_desc[2]= {"File Descr #1: 1  2  3  4  5  6  7  8  9 10 11 12 13"
                           "\n        14 15 16 17 18 19 20 **END FILE DESCR**\n",
                           "File Descr #2: A B C D E F G H I J K L \n"
                           "               M N O  **END FILE DESCR**\n"};

/* Data labels /descriptions to write */
static uint8 *labsds[2] = {"Object label #1:  sds", 
                          "Object label #1.1:sds"};
static uint8 *labris[2] = {"Object label #2:  image", 
                          "Object label #2.1:image"};
static uint8 *descsds[2]={"Object Descr #1:   1  2  3  4  5  6  7  8  9 10 11 12 "
                         "\n       13 14 15 16 17 18 19 20 **END SDS DESCR**\n",
                         "Object Descr #1.1: 1  2  3  4  5  6  7  8  9 10 11 12 "
                         "\n       13 14 15 16 17 18 19 20 **END SDS DESCR**\n"};
static uint8 *descris[2] = {"Object Descr #2:   A B C D E F G H I J K L \n"
                           "                 M N O **END IMAGE DESCR **\n",
                           "Object Descr #2.1: A B C D E F G H I J K L \n"
                           "                 M N O **END IMAGE DESCR **\n"};

static VOID 
gen2Dfloat (int height, int width, float *data);

static VOID 
genimage (int height, int width, float *data, uint8 *image);

static VOID 
check_lab_desc (char *fname, uint16 tag, uint16 ref,uint8*label[],uint8 *desc[]);

static VOID
check_fann(char *fname);

/****************************************************************
**
**  check_fann:  Check file labels and descriptions in file
**
****************************************************************/
static VOID
check_fann(char *fname)
{
  int32 ret;
  int32 file_handle;
  int32 ann_handle;
  int32 nflabs, nfdescs, nolabs, nodescs;
  int32 ann_len;
  uint8 *ann_label = NULL;
  uint8 *ann_desc = NULL;
  intn  i;

  /* open file again */
  ret = file_handle = ANstart(fname, DFACC_READ);
  RESULT("ANstart");

  /* Get Info On Annotations In File */
  ret = ANfileinfo(file_handle, &nflabs, &nfdescs, &nolabs, &nodescs);
  RESULT("Anfileinfo");
#ifdef AN_DEBUG
  printf("There Are Nflabs=%d, Nfdescs=%d, Nolabs=%d, Nodescs=%d \n",nflabs,
         nfdescs, nolabs, nodescs);
#endif

  /* read file labels */
  for (i = 0; i < nflabs; i++)
    { /* select file label */
      ann_handle = ret = ANselect(file_handle, i, AN_FILE_LABEL);
      RESULT("ANselect");

      /* get file label length */
      ann_len = ret = ANannlen(ann_handle);
      RESULT("ANannlen");
        
      /* check ann length */
      if (ann_len != (int32) HDstrlen(file_lab[i]))
        {
          printf("\t>>>BAD FILE LABEL LENGTH.\n\t    IS: %d\n\tSHOULD BE: %d<<<\n",
                 (int) ann_len, (int) HDstrlen(file_lab[i]));
          num_errs++;
        }

      /* allocate space for label */
      if (ann_label == NULL)
        {
          if ((ann_label = (uint8 *)HDgetspace((ann_len+1)*sizeof(uint8))) 
              == NULL)
            {
              printf("Error: failed to allocate space to hold file label \n");
              exit(1);
            }
          HDmemset(ann_label,'\0', ann_len+1);
        }
      
      /* read label */
      ret = ANreadann(ann_handle, ann_label, ann_len+1);
      RESULT("ANreadann");
      ret = ANendaccess(ann_handle);
      RESULT("ANendaccess");      
      
      /* check label */
      if (HDstrncmp((const char *)ann_label, (const char *)file_lab[i],ann_len+1) != 0)
        {
          printf("\t>>>BAD FILE LABEL. \n\t       IS: %s; \n\tSHOULD BE: %s<<<\n",
                 ann_label, file_lab[i]);
          num_errs++;
        }
#ifdef AN_DEBUG
      printf("found ann_len=%d, file label=%s\n", strlen(ann_label),ann_label);
#endif
      HDfreespace(ann_label);
      ann_label = NULL;
    } /* end for nflabs */

  /* read file descriptions */
  for (i = 0; i < nfdescs; i++)
    { /* select file label */
      ann_handle = ret = ANselect(file_handle, i, AN_FILE_DESC);
      RESULT("ANselect");

      /* get file label length */
      ann_len = ret = ANannlen(ann_handle);
      RESULT("ANannlen");
        
      /* check ann length */
      if (ann_len != (int32) HDstrlen(file_desc[i]))
        {
          printf("\t>>>BAD FILE DESC LENGTH.\n\t    IS: %d\n\tSHOULD BE: %d<<<\n",
                 (int) ann_len, (int) HDstrlen(file_desc[i]));
          num_errs++;
        }

      /* allocate space for desc */
      if (ann_desc == NULL)
        {
          if ((ann_desc = (uint8 *)HDgetspace((ann_len+1)*sizeof(uint8))) 
              == NULL)
            {
              printf("Error: failed to allocate space to hold file desc  \n");
              exit(1);
            }
          HDmemset(ann_desc,'\0', ann_len+1);
        }
      
      /* read desc */
      ret = ANreadann(ann_handle, ann_desc, ann_len+1);
      RESULT("ANreadann");
      ret = ANendaccess(ann_handle);
      RESULT("ANendaccess");      
            
      /* check desc */
      if (HDstrncmp((const char *)ann_desc,(const char *)file_desc[i],ann_len+1) != 0)
        {
          printf("\t>>>BAD FILE DESC. \n\t       IS: %s; \n\tSHOULD BE: %s<<<\n",
                 ann_desc, file_desc[i]);
          num_errs++;
        }
#ifdef AN_DEBUG
      printf("found ann_len=%d, file desc=%s\n", strlen(ann_desc),ann_desc);
#endif
      HDfreespace(ann_desc);
      ann_desc = NULL;
    } /* end for nfdescs */

  /* Clean up */
  if (ann_label != NULL)
    HDfreespace(ann_label);
  if (ann_desc != NULL)
    HDfreespace(ann_desc);

  /* close file */
  ANend(file_handle);
} /* check_fann() */

/****************************************************************
**
**  gen2Dfloat:  generate 2-D data array
**
****************************************************************/
static      VOID
gen2Dfloat(int height, int width, float *data)
{
  int    i, j;
  float  *pdata;

  /* store one value per row, increasing by one for each row */
  pdata = data;
  for (i = 0; i < height; i++)
    for (j = 0; j < width; j++)
      *pdata++ = (float) i + 1;

} /* gen2Dfloat() */

/****************************************************************
**
**  genimage:  generate image from 2-D float array
**
****************************************************************/
static      VOID
genimage(int height, int width, float *data, uint8 *image)
{
  int    i, limit;
  float  *pdata, max, min, multiplier;
  uint8  *pimage;

  limit = height * width;
  pdata = data;
  max   = min = *pdata;
  for (i = 0; i < limit; i++, pdata++)
    {
      max = (max > *pdata) ? max : *pdata;
      min = (min < *pdata) ? min : *pdata;
    }
  /* store one value per row, increasing by one for each row */
  pdata  = data;
  pimage = image;
  multiplier = (float32) 255.0 / (max - min);
  for (i = 0; i < limit; i++)
    *image++ = (uint8) (((*pdata++) - min) * multiplier);
} /* geniamge() */

/****************************************************************
**
**  check_lab_desc:  read and compare label and description
**                   with expected ones
**
****************************************************************/
static      VOID
check_lab_desc(char *fname, uint16 tag, uint16 ref, uint8 *label[], uint8 *desc[])
{
  int32 ret;
  int32 file_handle;
  int32 nflabs, nfdescs, nolabs, nodescs;
  intn  num_dlabels, num_ddescs;
  int32  *dlabels, *ddescs;
  int32 ann_len;
  uint8  *ann_label = NULL;
  uint8  *ann_desc = NULL;
  int i;

  /* open file again */
  ret = file_handle = ANstart(fname, DFACC_READ);
  RESULT("ANstart");

  /* Get Info On Annotations In File */
  ret = ANfileinfo(file_handle, &nflabs, &nfdescs, &nolabs, &nodescs);
  RESULT("Anfileinfo");
#ifdef AN_DEBUG
  printf("There Are Nflabs=%d, Nfdescs=%d, Nolabs=%d, Nodescs=%d \n",nflabs,
         nfdescs, nolabs, nodescs);
#endif
  /* Get number of annotations with this tag/ref */
  num_dlabels = ret = ANnumann(file_handle, AN_DATA_LABEL, tag, ref);
  RESULT("ANnumann");
#ifdef AN_DEBUG
  printf("there are %d data labels for tag=%d, ref=%d \n", num_dlabels, 
         tag, ref);
#endif
  num_ddescs = ret = ANnumann(file_handle, AN_DATA_DESC, tag, ref);
  RESULT("ANnumann"); 
#ifdef AN_DEBUG
  printf("There Are %d Data Descriptions For Tag=%d, Ref=%d \n", 
         num_ddescs, tag, ref);
#endif
  /* allocate space for list of label annotation id's with this tag/ref */
  if (num_dlabels == 0)
    {
      printf("Error: no data labels found\n");
      num_errs++;
    }
  else
    {
      if ((dlabels = (int32 *)HDgetspace(num_dlabels * sizeof(int32))) == NULL)
        {
          printf("Error: failed to allocate space to hold data label ids\n");
          exit(1);
        }
    }

  /* allocate space for list of description annotation id's with this tag/ref */
  if (num_ddescs == 0)
    {
      printf("Error: no data descriptions found\n");
      num_errs++;
    }
  else
    {
      if ((ddescs = (int32 *)HDgetspace(num_ddescs * sizeof(int32))) == NULL)
        {
          printf("Error: failed to allocate space to hold data descs ids\n");
          exit(1);
        }
    }

  /* get list of label annotations id's with this tag/ref */
  ret = ANannlist(file_handle, AN_DATA_LABEL, tag, ref, dlabels);
  RESULT("ANannlist");
  if (ret != num_dlabels)
    printf("Error:ret!=nlabels there are %d data labels for tag=%d,ref=%d \n", 
           num_dlabels, tag, ref);

  /* get list of description annotations id's with this tag/ref */
  ret = ANannlist(file_handle, AN_DATA_DESC, tag, ref, ddescs);
  RESULT("ANannlist");
  if (ret != num_ddescs)
    printf("Error:ret!=ndescs there are %d data descss for tag=%d,ref=%d \n", 
           num_ddescs, tag, ref);

  /* loop through label list */
  for (i = 0; i < num_dlabels; i++)
    {
      ann_len = ret = ANannlen(dlabels[i]);
      RESULT("ANannlen");
        
      /* check ann length */
      if (ann_len != (int32) HDstrlen(label[i]))
        {
          printf("\t>>>BAD DATA LABEL LENGTH.\n\t    IS: %d\n\tSHOULD BE: %d<<<\n",
                 (int) ann_len, (int) HDstrlen(label[i]));
          num_errs++;
        }

      /* allocate space for label */
      if (ann_label == NULL)
        {
          if ((ann_label = (uint8 *)HDgetspace((ann_len+1)*sizeof(uint8))) 
              == NULL)
            {
              printf("Error: failed to allocate space to hold data label \n");
              exit(1);
            }
          HDmemset(ann_label,'\0', ann_len+1);
        }
      
      /* read label */
      ret = ANreadann(dlabels[i], ann_label, ann_len+1);
      RESULT("ANreadann");
      ret = ANendaccess(dlabels[i]);
      RESULT("ANendaccess");      

      /* check label */
      if (HDstrncmp((const char *)ann_label, (const char *)label[i], ann_len+1) != 0)
        {
          printf("\t>>>BAD DATA LABEL. \n\t       IS: %s; \n\tSHOULD BE: %s<<<\n",
                 ann_label, label[i]);
          num_errs++;
        }
#ifdef AN_DEBUG
      printf("found data_len=%d, data label=%s\n", strlen(ann_label),ann_label);
#endif
      HDfreespace(ann_label);
      ann_label = NULL;
    } /* end for labels */

  /* loop through desc list */
  for (i = 0; i < num_ddescs; i++)
    {
      /* get desc length */
      ann_len = ret = ANannlen(ddescs[i]);
      RESULT("ANannlen");

      /* check desc length */
      if (ann_len != (int32) HDstrlen(desc[i]))
        {
          printf("\t>>>BAD DATA DESC LENGTH.\n\t    IS: %d\n\tSHOULD BE: %d<<<\n",
                 (int) ann_len, (int) HDstrlen(desc[i]));
          num_errs++;
        }

      /* allocate space for descritpion */
      if (ann_desc == NULL)
        {
          if ((ann_desc = (uint8 *)HDgetspace((ann_len+1)*sizeof(uint8))) 
              == NULL)
            {
              printf("Error: failed to allocate space to hold data desc \n");
              exit(1);
              }
          HDmemset(ann_desc,'\0', ann_len+1);
        }

      /* read description */
      ret = ANreadann(ddescs[i], ann_desc, ann_len+1);
      RESULT("ANreadann");
      ret = ANendaccess(ddescs[i]);
      RESULT("ANendaccess");      

      /* check desc */
      if (HDstrncmp((const char *)ann_desc, (const char *)desc[i], ann_len) != 0)
        {
          printf("\t>>>BAD DATA DESC. \n\t       IS: %s; \n\tSHOULD BE: %s<<<\n",
                 ann_desc, desc[i]);
          num_errs++;
        }
#ifdef AN_DEBUG
      printf("found data desclen=%d, desc=%s\n", strlen(ann_desc),ann_desc);
#endif
      HDfreespace(ann_desc);
      ann_desc = NULL;
    } /* end for descs */
    
  /* free space */
  HDfreespace(dlabels);
  HDfreespace(ddescs);
  if (ann_label != NULL)
    HDfreespace(ann_label);
  if (ann_desc != NULL)
    HDfreespace(ann_desc);

  /* close file */
  ANend(file_handle);
} /* check_lab_desc() */

/****************************************************************
**
**  test_man(): Main annotation test routine
** 
**  NOTES: Keep in mind the order in which labels/descriptiosn
**         are written to the file to make sense of the indices
**         returned by ANfileinfo().
****************************************************************/
void
test_man()
{
  uint8  pal[768];
  uint8  *image, *newimage;
  uint16 refnum;
  int32  ret;
  intn   rank;
  int    i,j;
  int32  dimsizes[2];
  float  *data;
  int32   file_handle; /* file handle */
  int32   ann_handle;  /* annotation handle */

  /***** generate float array and image *****/
  data     = (float *) HDgetspace(ROWS * COLS * sizeof(float));
  image    = (uint8 *) HDgetspace(ROWS * COLS * sizeof(char));
  newimage = (uint8 *) HDgetspace(ROWS * COLS * sizeof(char));

  dimsizes[0] = ROWS;
  dimsizes[1] = COLS;

  /* generate images */
  gen2Dfloat(ROWS, COLS, data);
  genimage(ROWS, COLS, data, image);

  ret = DFSDsetdims(2, dimsizes);
  RESULT("DFSDsetdims");
    
  /* Start annotation Interface on file */
  ret = file_handle = ANstart(TESTFILE, DFACC_CREATE);
  RESULT("ANstart");

  /********  Write file labels and descriptions *********/
  MESSAGE(5, printf("***  Writing file labels and descriptions ***\n"););

  /* create and write file labels */
  for (i = 1; i >= 0; i--)
    {
      ret = ann_handle = ANcreatef(file_handle, AN_FILE_LABEL);
      RESULT("ANcreatef");
      ret = ANwriteann(ann_handle, file_lab[i], HDstrlen(file_lab[i]));
      RESULT("ANwriteann");
      ret = ANendaccess(ann_handle);
      RESULT("ANendaccess");
    }

  /* create and write file descriptions */
  for (i = 1; i >= 0; i--)
    {
      ret = ann_handle = ANcreatef(file_handle, AN_FILE_DESC);
      RESULT("ANcreatef");
      ret = ANwriteann(ann_handle, file_desc[i], HDstrlen(file_desc[i]));
      RESULT("ANwriteann");
      ret = ANendaccess(ann_handle);
      RESULT("ANendaccess");
    }

  /********  Write labels and descriptions *********/
  MESSAGE(5, printf("***  Writing labels and descriptions with SDS and RIS ***\n"););

  for (j = 0; j < REPS; j++)
    {
      /* write out scientific data set */
      ret = DFSDadddata(TESTFILE, 2, dimsizes, (VOIDP) data);
      RESULT("DFSDadddata");

      if ((j % 3) != 0)
        {   /* write out annotations for 2 out of every 3 */
          refnum = DFSDlastref();
          /* create and write data label */
          for (i = 1; i >=0; i--)
            {
              ret = ann_handle = ANcreate(file_handle, DFTAG_SDG, refnum, 
                                          AN_DATA_LABEL);
              RESULT("ANcreate");
              ret = ANwriteann(ann_handle, labsds[i], HDstrlen(labsds[i]));
              RESULT("ANwriteann");
              ret = ANendaccess(ann_handle);
              RESULT("ANendaccess");
            }

          /* create and write data description */
          for (i = 1; i >=0; i--)
            {
              ret = ann_handle = ANcreate(file_handle, DFTAG_SDG, refnum, 
                                          AN_DATA_DESC);
              RESULT("ANcreate");
              ret = ANwriteann(ann_handle, descsds[i], HDstrlen(descsds[i]));
              RESULT("ANwriteann");
              ret = ANendaccess(ann_handle);
              RESULT("ANendaccess");
            }
        }

      ret = DFR8addimage(TESTFILE, (VOIDP) image, COLS, ROWS, 0);
      RESULT("DFR8addimage");
      refnum = DFR8lastref();

      /* create and write image label */
      for (i = 1; i >=0; i--)
        {
          ret = ann_handle = ANcreate(file_handle, DFTAG_RIG, refnum, 
                                      AN_DATA_LABEL);
          RESULT("ANcreate");
          ret = ANwriteann(ann_handle, labris[i], HDstrlen(labris[i]));
          RESULT("ANwriteann");
          ret = ANendaccess(ann_handle);
          RESULT("ANendaccess");
        }

      /* create and write image description */
      for (i = 1; i >=0; i--)
        {
          ret = ann_handle = ANcreate(file_handle, DFTAG_RIG, refnum, 
                                      AN_DATA_DESC);
          RESULT("ANcreate");
          ret = ANwriteann(ann_handle, descris[i], HDstrlen(descris[i]));
          RESULT("ANwriteann");
          ret = ANendaccess(ann_handle);
          RESULT("ANendaccess");
        }
    } /* end for j */

  /* End writing annotations */
  ANend(file_handle);
  
  /********  Read labels and descriptions *********/
  MESSAGE(5, printf("*** Reading labels and descriptions for SDS and RIS ***\n"););

  for (j = 0; j < REPS; j++)
    {
      ret = DFSDgetdims(TESTFILE, &rank, dimsizes, 3);
      RESULT("DFSDgetdims");

      refnum = DFSDlastref();

      /* Check data set labels/descriptions */
      if ((j % 3) != 0)     /* read in annotations for 2 out of every 3 */
        check_lab_desc(TESTFILE, DFTAG_SDG, refnum, labsds, descsds);

      ret = DFR8getimage(TESTFILE, newimage, (int32) COLS, (int32) ROWS, pal);
      RESULT("DFR8getimage");
      refnum = DFR8lastref();

      /* Check image labels/descriptions */
      check_lab_desc(TESTFILE, DFTAG_RIG, refnum, labris, descris);
    }

  /***************** Read file labels and descriptions */
  MESSAGE(5, printf("*** Reading file label and descriptions ***\n"););
  check_fann(TESTFILE);

#if 0
  /* Destroy Annotation interface */
  ANdestroy();
#endif
  /* free up space */
  HDfreespace((VOIDP) data);
  HDfreespace((VOIDP) image);
  HDfreespace((VOIDP) newimage);
} /* test_man() */

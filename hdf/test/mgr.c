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
static char RcsId[] = "$Revision$";
#endif

/* $Id$ */

/***********************************************************
*
* Test program:  mgr
*
* Test the multi-file raster image interface
*
*************************************************************/

#define TESTFILE "tmgr.hdf"
#define DATAFILE "tmgr.dat"

#include "tproto.h"
#include "mfgr.h"

/* Local pre-processor macros */
#define XDIM    0
#define YDIM    1
#define MAX_IMG_NAME    64  /* Maximum length of image names for this test */

/* Local Data to verify image information in datafile */
const struct {
    char *name;
    int32 ncomp;
    int32 nt;
    int32 il;
    int32 dimsizes[2];
    int32 n_attr;
} datafile_info[]=
  {
    {"Raster Image #0", 3, DFNT_UCHAR8, MFGR_INTERLACE_PIXEL, {13,15}, 2},
    {"Raster Image #1", 3, DFNT_UCHAR8, MFGR_INTERLACE_LINE, {13,15}, 2},
    {"Raster Image #2", 3, DFNT_UCHAR8, MFGR_INTERLACE_COMPONENT, {13,15}, 2},
    {"Test Image #1", 4, DFNT_UINT16, MFGR_INTERLACE_PIXEL, {21,23}, 3},
    {"Test Image #2", 2, DFNT_FLOAT64, MFGR_INTERLACE_PIXEL, {17,19}, 3}
  };

#ifdef QAK
#define MAXLEN_LAB    50
#define MAXLEN_DESC  500
#define ROWS          10
#define COLS          10
#define REPS           3   /* number of data sets to write to file */

/* File labels/desriptions to write */
static char *file_lab[2] = {"File label #1: aaa", "File label #2: bbbbbb"};
static char *file_desc[2]= {"File Descr #1: 1  2  3  4  5  6  7  8  9 10 11 12 13"
                           "\n        14 15 16 17 18 19 20 **END FILE DESCR**\n",
                           "File Descr #2: A B C D E F G H I J K L \n"
                           "               M N O  **END FILE DESCR**\n"};

/* Data labels /descriptions to write */
static char *labsds[2] = {"Object label #1:  sds", 
                          "Object label #1.1:sds"};
static char *labris[2] = {"Object label #2:  image", 
                          "Object label #2.1:image"};
static char *descsds[2]={"Object Descr #1:   1  2  3  4  5  6  7  8  9 10 11 12 "
                         "\n       13 14 15 16 17 18 19 20 **END SDS DESCR**\n",
                         "Object Descr #1.1: 1  2  3  4  5  6  7  8  9 10 11 12 "
                         "\n       13 14 15 16 17 18 19 20 **END SDS DESCR**\n"};
static char *descris[2] = {"Object Descr #2:   A B C D E F G H I J K L \n"
                           "                 M N O **END IMAGE DESCR **\n",
                           "Object Descr #2.1: A B C D E F G H I J K L \n"
                           "                 M N O **END IMAGE DESCR **\n"};

static VOID 
gen2Dfloat (int height, int width, float *data);

static VOID 
genimage (int height, int width, float *data, uint8 *image);

static VOID 
check_lab_desc (char *fname, uint16 tag, uint16 ref,char* label[],char *desc[]);

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
  char *ann_label = NULL;
  char *ann_desc = NULL;
  intn  i;
  intn indx;

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
      indx = i;
      ann_handle = ret = ANselect(file_handle, indx, AN_FILE_LABEL);
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
          if ((ann_label = (char *)HDmalloc((ann_len+1)*sizeof(char))) 
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
      HDfree(ann_label);
      ann_label = NULL;
    } /* end for nflabs */

  /* read file descriptions */
  for (i = 0; i < nfdescs; i++)
    { /* select file label */
      indx = i;
      ann_handle = ret = ANselect(file_handle, indx, AN_FILE_DESC);
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
          if ((ann_desc = (char *)HDmalloc((ann_len+1)*sizeof(char))) 
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
      HDfree(ann_desc);
      ann_desc = NULL;
    } /* end for nfdescs */

  /* Clean up */
  if (ann_label != NULL)
    HDfree(ann_label);
  if (ann_desc != NULL)
    HDfree(ann_desc);

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
check_lab_desc(char *fname, uint16 tag, uint16 ref, char *label[], char *desc[])
{
  int32 ret;
  int32 file_handle;
  int32 nflabs, nfdescs, nolabs, nodescs;
  intn  num_dlabels, num_ddescs;
  int32  *dlabels, *ddescs;
  int32 ann_len;
  char  *ann_label = NULL;
  char  *ann_desc = NULL;
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
      if ((dlabels = (int32 *)HDmalloc(num_dlabels * sizeof(int32))) == NULL)
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
      if ((ddescs = (int32 *)HDmalloc(num_ddescs * sizeof(int32))) == NULL)
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
          if ((ann_label = (char *)HDmalloc((ann_len+1)*sizeof(char))) 
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
      HDfree(ann_label);
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
          if ((ann_desc = (char *)HDmalloc((ann_len+1)*sizeof(char))) 
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
      HDfree(ann_desc);
      ann_desc = NULL;
    } /* end for descs */
    
  /* free space */
  HDfree(dlabels);
  HDfree(ddescs);
  if (ann_label != NULL)
    HDfree(ann_label);
  if (ann_desc != NULL)
    HDfree(ann_desc);

  /* close file */
  ANend(file_handle);
} /* check_lab_desc() */

#endif /* QAK */

void dump_image(void *data, int32 xdim, int32 ydim, int32 ncomp, int32 nt)
{
    int32 nt_size=DFKNTsize(nt);
    int32 i,j,k;

    for(i=0; i<ydim; i++)
      {
          printf("%ld:",(long)i);
          for(j=0; j<xdim; j++)
            {
                printf("(");
                for(k=0; k<ncomp; k++)
                  {
                    switch(nt)
                      {
                          case DFNT_CHAR8:
                          case DFNT_UCHAR8:
#ifdef QAK
                            {
                                char *ptr=(char *)data;
                                printf("%c ",*ptr);
                            }
                            break;
#endif /* QAK */

                          case DFNT_UINT8:
                            {
                                unsigned char *ptr=(unsigned char *)data;
                                printf("%u ",(unsigned)*ptr);
                            }
                            break;

                          case DFNT_INT8:
                            {
                                char *ptr=(char *)data;
                                printf("%d ",(int)*ptr);
                            }
                            break;

                          case DFNT_UINT16:
                            {
                                uint16 *ptr=(uint16 *)data;
                                printf("%u ",(unsigned)*ptr);
                            }
                            break;

                          case DFNT_INT16:
                            {
                                int16 *ptr=(int16 *)data;
                                printf("%d ",(int)*ptr);
                            }
                            break;

                          case DFNT_UINT32:
                            {
                                uint32 *ptr=(uint32 *)data;
                                printf("%lu ",(unsigned long)*ptr);
                            }
                            break;

                          case DFNT_INT32:
                            {
                                int32 *ptr=(int32 *)data;
                                printf("%ld ",(long)*ptr);
                            }
                            break;

                          case DFNT_FLOAT32:
                            {
                                float32 *ptr=(float32 *)data;
                                printf("%f ",(double)*ptr);
                            }
                            break;

                          case DFNT_FLOAT64:
                            {
                                float64 *ptr=(float64 *)data;
                                printf("%f ",(double)*ptr);
                            }
                            break;

                          default:
                            printf("unknown NT: %ld\n",(long)nt);
                            break;

                      } /* end switch */
                    data=(void *)((char *)data+nt_size);
                  } /* end for */
                printf(")");

            } /* end for */
          printf("\n");
      } /* end for */
}   /* dump_image() */

/****************************************************************
**
**  test_mgr(): Main multi-file raster image test routine
** 
****************************************************************/
void
test_mgr_old()
{
#ifdef QAK
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
  data     = (float *) HDmalloc(ROWS * COLS * sizeof(float));
  image    = (uint8 *) HDmalloc(ROWS * COLS * sizeof(char));
  newimage = (uint8 *) HDmalloc(ROWS * COLS * sizeof(char));

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
  HDfree((VOIDP) data);
  HDfree((VOIDP) image);
  HDfree((VOIDP) newimage);
#else /* QAK */
    intn i,j,k;             /* local counting variable */
    int32 hdf_file_id;      /* HDF file ID */
    int32 grid;             /* GR interface ID */
    int32 riid;             /* RI interface ID */
    int32 ret;              /* generic return value */
    int32 n_datasets;       /* number of datasets */
    int32 n_attrs;          /* number of attributes */
    int32 ncomp;            /* number of components */
    int32 nt;               /* number-type */
    int32 il;               /* interlace */
    int32 dimsizes[2];      /* dimension sizes */
    char img_name[32];      /* image name space */
    char attr_name[32];     /* name of the attribute */
    char u8_attr[64];       /* uint8 attribute */
    uint16 *img1_data;      /* uint16 image pointer */
    int32 start[2];         /* starting location for I/O */
    int32 stride[2];        /* stride of I/O */
    int32 count[2];         /* count of I/O */

    /* Create the test file */
#ifdef QAK
    hdf_file_id=Hopen(TESTFILE,DFACC_ALL,0);
#else /* QAK */
    hdf_file_id=Hopen(TESTFILE,DFACC_RDWR,0);
#endif /* QAK */
    CHECK(hdf_file_id,FAIL,"Hopen");

    /* Try initializing the GR interface */
    grid=GRstart(hdf_file_id);
    CHECK(grid,FAIL,"GRstart");

    ret=(intn)GRfileinfo(grid,&n_datasets,&n_attrs);
    CHECK(ret,FAIL,"GRfileinfo");

printf("hdf_file_id=%ld\n",(long)hdf_file_id);
printf("grid=%ld: n_datasets=%ld, n_attrs=%ld\n",(long)grid,(long)n_datasets,(long)n_attrs);
    if(n_attrs>0)
      {
          for(i=0; i<n_attrs; i++)
            {
                ret=(intn)GRattrinfo(grid,i,attr_name,&nt,&ncomp);
                CHECK(ret,FAIL,"GRattrinfo");
if(ret==FAIL)
    HEprint(stderr,0);

                ret=(intn)GRgetattr(grid,i,u8_attr);
                CHECK(ret,FAIL,"GRgetattr");
                if(ret!=FAIL)
                    printf("Global Attribute #%d: Name=%s, Value=%s\n",i,attr_name,u8_attr);

            } /* end for */
      } /* end if */

    /* Shut down the GR interface */
    ret=GRend(grid);
    CHECK(ret,FAIL,"GRend");
if(ret==FAIL)
    HEprint(stderr,0);

    /* Close the file */
    ret=Hclose(hdf_file_id);
    CHECK(ret,FAIL,"Hclose");
if(ret==FAIL)
    HEprint(stderr,0);


    /* Ok, let's try playing with the images some */
    hdf_file_id=Hopen(TESTFILE,DFACC_RDWR,0);
    CHECK(hdf_file_id,FAIL,"Hopen");

    /* Try initializing the GR interface */
    grid=GRstart(hdf_file_id);
    CHECK(grid,FAIL,"GRstart");

    ret=(intn)GRfileinfo(grid,&n_datasets,&n_attrs);
    CHECK(ret,FAIL,"GRfileinfo");

    /* Attach to each image */
    for(i=0; i<n_datasets; i++)
      {
          riid=GRselect(grid,i);
          CHECK(riid,FAIL,"GRselect");
          if(riid==FAIL)
              HEprint(stderr,0);
          else
            {
                ret=(intn)GRgetiminfo(riid,NULL,&ncomp,&nt,&il,dimsizes,&n_attrs);
                CHECK(ret,FAIL,"GRfileinfo");

printf("%d: riid=%ld: ncomp=%ld, nt=%ld, il=%ld, dim[0]=%ld, dim[1]=%ld, n_attrs=%ld\n",i,(long)riid,(long)ncomp,(long)nt,(long)il,(long)dimsizes[0],(long)dimsizes[1],(long)n_attrs);

              /* Detach from the first image */
              ret=GRendaccess(riid);
              CHECK(ret,FAIL,"GRselect");
            } /* end else */
      } /* end for */

    /* Shut down the GR interface */
    ret=GRend(grid);
    CHECK(ret,FAIL,"GRend");
if(ret==FAIL)
    HEprint(stderr,0);

    /* Close the file */
    ret=Hclose(hdf_file_id);
    CHECK(ret,FAIL,"Hclose");
if(ret==FAIL)
    HEprint(stderr,0);


    /* Ok, now create some attributes in the images */
    hdf_file_id=Hopen(TESTFILE,DFACC_RDWR,0);
    CHECK(hdf_file_id,FAIL,"Hopen");

    /* Try initializing the GR interface */
    grid=GRstart(hdf_file_id);
    CHECK(grid,FAIL,"GRstart");

    /* Create an attribute for the file */
    HDstrcpy(attr_name,"Test1");
    HDstrcpy(u8_attr,"Attribute value 1");
    ret=GRsetattr(grid,attr_name,DFNT_UINT8,HDstrlen(u8_attr)+1,u8_attr);
    CHECK(ret,FAIL,"GRsetattr");

    /* Create another attribute for the file */
    HDstrcpy(attr_name,"Test2");
    HDstrcpy(u8_attr,"Attribute value 2");
    ret=GRsetattr(grid,attr_name,DFNT_UINT8,HDstrlen(u8_attr)+1,u8_attr);
    CHECK(ret,FAIL,"GRsetattr");

    ret=(intn)GRfileinfo(grid,&n_datasets,&n_attrs);
    CHECK(ret,FAIL,"GRfileinfo");

    /* Attach to each image */
    for(i=0; i<n_datasets; i++)
      {
          riid=GRselect(grid,i);
          CHECK(riid,FAIL,"GRselect");
          if(riid==FAIL)
              HEprint(stderr,0);
          else
            {
                /* Create an attribute for the image */
                HDstrcpy(attr_name,"Image1");
                HDstrcpy(u8_attr,"Attribute value 1");
                ret=GRsetattr(riid,attr_name,DFNT_UINT8,HDstrlen(u8_attr)+1,u8_attr);
                CHECK(ret,FAIL,"GRsetattr");

                /* Create another attribute for the image */
                HDstrcpy(attr_name,"Image2");
                HDstrcpy(u8_attr,"Attribute value 2");
                ret=GRsetattr(riid,attr_name,DFNT_UINT8,HDstrlen(u8_attr)+1,u8_attr);
                CHECK(ret,FAIL,"GRsetattr");

                ret=(intn)GRgetiminfo(riid,NULL,&ncomp,&nt,&il,dimsizes,&n_attrs);
                CHECK(ret,FAIL,"GRfileinfo");

printf("%d: riid=%ld: ncomp=%ld, nt=%ld, il=%ld, dim[0]=%ld, dim[1]=%ld, n_attrs=%ld\n",i,(long)riid,(long)ncomp,(long)nt,(long)il,(long)dimsizes[0],(long)dimsizes[1],(long)n_attrs);

                for(j=0; j<n_attrs; j++)
                  {
                    ret=(intn)GRattrinfo(riid,j,attr_name,&nt,&ncomp);
                    CHECK(ret,FAIL,"GRattrinfo");
if(ret==FAIL)
    HEprint(stderr,0);

                    ret=(intn)GRgetattr(riid,j,u8_attr);
                    CHECK(ret,FAIL,"GRgetattr");
                    if(ret!=FAIL)
                        printf("Image #%d Attribute #%d: Name=%s, Value=%s\n",i,j,attr_name,u8_attr);
                  } /* end for */

                /* Detach from the image */
                ret=GRendaccess(riid);
                CHECK(ret,FAIL,"GRendaccess");
            } /* end else */
      } /* end for */

    /* Shut down the GR interface */
    ret=GRend(grid);
    CHECK(ret,FAIL,"GRend");
if(ret==FAIL)
    HEprint(stderr,0);

    /* Close the file */
    ret=Hclose(hdf_file_id);
    CHECK(ret,FAIL,"Hclose");
if(ret==FAIL)
    HEprint(stderr,0);

    /* Ok, now create some new images in the file */
    hdf_file_id=Hopen(TESTFILE,DFACC_RDWR,0);
    CHECK(hdf_file_id,FAIL,"Hopen");

    /* Try initializing the GR interface */
    grid=GRstart(hdf_file_id);
    CHECK(grid,FAIL,"GRstart");

    dimsizes[0]=21;
    dimsizes[1]=23;
    ncomp=4;
    nt=DFNT_UINT16;
    riid=(intn)GRcreate(grid,"Test Image #1",ncomp,nt,0,dimsizes);
    CHECK(riid,FAIL,"GRcreate");

    img1_data=HDmalloc(dimsizes[0]*dimsizes[1]*DFKNTsize(nt)*ncomp);
    CHECK(img1_data,NULL,"HDmalloc");

    {
        uint16 *tptr=(uint16 *)img1_data;

        for(i=0; i<dimsizes[1]; i++)
            for(j=0; j<dimsizes[0]; j++)
                for(k=0; k<ncomp; k++)
                    *tptr++=j+k;
    }
            
    start[0]=start[1]=0;
    stride[0]=1;
    stride[1]=2;
    count[0]=dimsizes[0]/2;
    count[1]=dimsizes[1]/2;
    ret=GRwriteimage(riid,start,stride,count,img1_data);
    CHECK(ret,FAIL,"GRwriteimage");
if(ret==FAIL)
    HEprint(stderr,0);
    
    {
        uint16 *tptr=(uint16 *)img1_data;

        for(i=0; i<dimsizes[1]; i++)
            for(j=0; j<dimsizes[0]; j++)
                for(k=0; k<ncomp; k++)
                    *tptr++=2;
    }

    start[0]=start[1]=1;
    stride[0]=2;
    stride[1]=1;
    count[0]=dimsizes[0]/2;
    count[1]=dimsizes[1]/2;
    ret=GRwriteimage(riid,start,stride,count,img1_data);
    CHECK(ret,FAIL,"GRwriteimage");
if(ret==FAIL)
    HEprint(stderr,0);
    
    /* Get rid of the image data */
    HDfree(img1_data);

    /* Detach from the image */
    ret=GRendaccess(riid);
    CHECK(ret,FAIL,"GRendaccess");
if(ret==FAIL)
    HEprint(stderr,0);

    dimsizes[0]=17;
    dimsizes[1]=19;
    ncomp=2;
    nt=DFNT_FLOAT64;
    riid=(intn)GRcreate(grid,"Test Image #2",ncomp,nt,0,dimsizes);
    CHECK(riid,FAIL,"GRcreate");

    img1_data=HDmalloc(dimsizes[0]*dimsizes[1]*DFKNTsize(nt)*ncomp);
    CHECK(img1_data,NULL,"HDmalloc");

    {
        float64 *tptr=(float64 *)img1_data;

        for(i=0; i<dimsizes[1]; i++)
            for(j=0; j<dimsizes[0]; j++)
                for(k=0; k<ncomp; k++)
                    *tptr++=6.0;
    }
            
    start[0]=start[1]=0;
    stride[0]=stride[1]=1;
    count[0]=dimsizes[0]/2;
    count[1]=dimsizes[1]/2;
    ret=GRwriteimage(riid,start,stride,count,img1_data);
    CHECK(ret,FAIL,"GRwriteimage");
if(ret==FAIL)
    HEprint(stderr,0);
    
    {
        float64 *tptr=(float64 *)img1_data;

        for(i=0; i<dimsizes[1]; i++)
            for(j=0; j<dimsizes[0]; j++)
                for(k=0; k<ncomp; k++)
                    *tptr++=9.0;
    }
            
    start[0]=dimsizes[0]/2;
    start[1]=dimsizes[1]/2;
    stride[0]=stride[1]=1;
    count[0]=dimsizes[0]/2;
    count[1]=dimsizes[1]/2;
    ret=GRwriteimage(riid,start,stride,count,img1_data);
    CHECK(ret,FAIL,"GRwriteimage");
if(ret==FAIL)
    HEprint(stderr,0);
    
    /* Get rid of the image data */
    HDfree(img1_data);

    /* Detach from the image */
    ret=GRendaccess(riid);
    CHECK(ret,FAIL,"GRendaccess");
if(ret==FAIL)
    HEprint(stderr,0);

    ret=(intn)GRfileinfo(grid,&n_datasets,&n_attrs);
    CHECK(ret,FAIL,"GRfileinfo");

    /* Attach to each image */
    for(i=0; i<n_datasets; i++)
      {
          riid=GRselect(grid,i);
          CHECK(riid,FAIL,"GRselect");
          if(riid==FAIL)
              HEprint(stderr,0);
          else
            {
                /* Create an attribute for the image */
                HDstrcpy(attr_name,"Image1");
                HDstrcpy(u8_attr,"Attribute value 1");
                ret=GRsetattr(riid,attr_name,DFNT_UINT8,HDstrlen(u8_attr)+1,u8_attr);
                CHECK(ret,FAIL,"GRsetattr");

                /* Create another attribute for the image */
                HDstrcpy(attr_name,"Image2");
                HDstrcpy(u8_attr,"Attribute value 2");
                ret=GRsetattr(riid,attr_name,DFNT_UINT8,HDstrlen(u8_attr)+1,u8_attr);
                CHECK(ret,FAIL,"GRsetattr");

                ret=(intn)GRgetiminfo(riid,NULL,&ncomp,&nt,&il,dimsizes,&n_attrs);
                CHECK(ret,FAIL,"GRfileinfo");

printf("%d: riid=%ld: ncomp=%ld, nt=%ld, il=%ld, dim[0]=%ld, dim[1]=%ld, n_attrs=%ld\n",i,(long)riid,(long)ncomp,(long)nt,(long)il,(long)dimsizes[0],(long)dimsizes[1],(long)n_attrs);

                img1_data=HDmalloc(dimsizes[0]*dimsizes[1]*ncomp*DFKNTsize(nt));
                CHECK(img1_data,NULL,"HDmalloc");

                HDmemset(img1_data,0,dimsizes[0]*dimsizes[1]*ncomp*DFKNTsize(nt));

                start[0]=start[1]=0;
                stride[0]=stride[1]=1;
                ret=GRreadimage(riid,start,stride,dimsizes,img1_data);

                /* Dump the image out */
                dump_image(img1_data,dimsizes[0],dimsizes[1],ncomp,nt);

                HDfree(img1_data);

                for(j=0; j<n_attrs; j++)
                  {
                    ret=(intn)GRattrinfo(riid,j,attr_name,&nt,&ncomp);
                    CHECK(ret,FAIL,"GRattrinfo");
if(ret==FAIL)
    HEprint(stderr,0);

                    ret=(intn)GRgetattr(riid,j,u8_attr);
                    CHECK(ret,FAIL,"GRgetattr");
                    if(ret!=FAIL)
                        printf("Image #%d Attribute #%d: Name=%s, Value=%s\n",i,j,attr_name,u8_attr);
                  } /* end for */

                /* Detach from the image */
                ret=GRendaccess(riid);
                CHECK(ret,FAIL,"GRendaccess");
            } /* end else */
      } /* end for */

    /* Shut down the GR interface */
    ret=GRend(grid);
    CHECK(ret,FAIL,"GRend");
if(ret==FAIL)
    HEprint(stderr,0);

    /* Close the file */
    ret=Hclose(hdf_file_id);
    CHECK(ret,FAIL,"Hclose");
if(ret==FAIL)
    HEprint(stderr,0);
#endif /* QAK */
} /* test_mgr_old() */

/* Test outline:
    I. Interface Initialization
        A. GRstart
        B. GRend
        C. GRfileinfo
    II. Create Images
        A. GRcreate/GRselect/GRendaccess w/GRgetiminfo
        B. Write/Read images
            1. With no Data
                a. Default fill value
                b. user defined fill value
            2. With real Data
                a. New Image
                    1. With default fill value
                        aa. Whole image
                        bb. Sub-setted image
                        cc. Sub-sampled image
                    2. With user defined vill value
                        aa. Whole image
                        bb. Sub-setted image
                        cc. Sub-sampled image
                b. Existing Image
                    1. Whole image
                    2. Sub-setted image
                    3. Sub-sampled image
    III. ID/Ref/Index Functions
        A. GRidtoref
        B. GRreftoindex
    IV. Interlace Functions [Need to be implemented]
        A. GRreqlutil
        B. GRreqimageil
    V. Palette Functions
        A. GRgetlutid w/GRgetlutinfo
        B. Read/Write Palettes
            1. GRwritelut
            2. GRreadlut
    VI. Special Element Functions [Need to be implemented]
        A. GRsetexternalfile
        B. GRsetaccesstype
        C. GRsetcompress
    VII. Atribute Functions
        A. GRattrinfo
        B. Read/Write Attributes
            1. GRsetattr
            2. GRgetattr
        C. GRfindattr
        
*/

/****************************************************************
**
**  test_mgr_init(): Multi-file Raster Initialization Test Routine
** 
****************************************************************/
void
test_mgr_init()
{
    int32 fid;              /* HDF file ID */
    int32 grid;             /* GRID for the interface */
    int32 n_datasets;       /* number of datasets */
    int32 n_attrs;          /* number of attributes */
    int32 ret;              /* generic return value */

    /* Output message about test being performed */
    MESSAGE(6, printf("Testing Multi-file Raster Initialization routines\n"););

    MESSAGE(8, printf("Try creating a new file and checking it out\n"););

    /* Ok, now create a new file */
    fid=Hopen(TESTFILE,DFACC_RDWR,0);
    CHECK(fid,FAIL,"Hopen");

    /* Try initializing the GR interface */
    grid=GRstart(fid);
    CHECK(grid,FAIL,"GRstart");

    ret=(intn)GRfileinfo(grid,&n_datasets,&n_attrs);
    CHECK(ret,FAIL,"GRfileinfo");

    if(n_datasets!=0 || n_attrs!=0)
      {
          MESSAGE(3, printf("Number of datasets/attributes in new file incorrect\n"););
          num_errs++;
      } /* end if */

    /* Shut down the GR interface */
    ret=GRend(grid);
    CHECK(ret,FAIL,"GRend");

    /* Close the file */
    ret=Hclose(fid);
    CHECK(ret,FAIL,"Hclose");


    MESSAGE(8, printf("Try checking out an existing file\n"););

    /* Ok, now check an existing file */
    fid=Hopen(DATAFILE,DFACC_READ,0);
    CHECK(fid,FAIL,"Hopen");

    /* Try initializing the GR interface */
    grid=GRstart(fid);
    CHECK(grid,FAIL,"GRstart");

    ret=(intn)GRfileinfo(grid,&n_datasets,&n_attrs);
    CHECK(ret,FAIL,"GRfileinfo");

    if(n_datasets!=5 || n_attrs!=2)
      {
          MESSAGE(3, printf("Number of datasets/attributes in existing file incorrect\n"););
          num_errs++;
      } /* end if */

    /* Shut down the GR interface */
    ret=GRend(grid);
    CHECK(ret,FAIL,"GRend");

    /* Close the file */
    ret=Hclose(fid);
    CHECK(ret,FAIL,"Hclose");
}   /* end test_mgr_init() */

/****************************************************************
**
**  test_mgr_image(): Multi-file Raster Image I/O Test Routine
** 
        A. GRcreate/GRselect/GRendaccess w/GRgetiminfo
        B. Write/Read images
            1. With no Data
                a. Default fill value
                b. user defined fill value
            2. With real Data
                a. New Image
                    1. With default fill value
                        aa. Whole image
                        bb. Sub-setted image
                        cc. Sub-sampled image
                    2. With user defined vill value
                        aa. Whole image
                        bb. Sub-setted image
                        cc. Sub-sampled image
                b. Existing Image
                    1. Whole image
                    2. Sub-setted image
                    3. Sub-sampled image
****************************************************************/
void
test_mgr_image()
{
    int32 fid;              /* HDF file ID */
    int32 grid;             /* GRID for the interface */
    int32 n_datasets;       /* number of datasets */
    int32 n_attrs;          /* number of attributes */
    int32 ret;              /* generic return value */
    intn i,j,k;             /* local counting variables */

    /* Output message about test being performed */
    MESSAGE(6, printf("Testing Multi-file Raster Image I/O routines\n"););

    MESSAGE(8, printf("Try checking out the images in an existing file\n"););

    /* Open up the existing datafile and get the image information from it */
    fid=Hopen(DATAFILE,DFACC_READ,0);
    CHECK(fid,FAIL,"Hopen");

    /* Try initializing the GR interface */
    grid=GRstart(fid);
    CHECK(grid,FAIL,"GRstart");

    ret=(intn)GRfileinfo(grid,&n_datasets,&n_attrs);
    CHECK(ret,FAIL,"GRfileinfo");

    /* Walk through each existing dataset and verify it's contents */
    for(i=0; i<n_datasets; i++)
      {
          int32 riid;               /* RI ID for an image */
          char name[MAX_IMG_NAME];  /* storage for the image's name */
          int32 ncomp;              /* number of components */
          int32 nt;                 /* NT of the components */
          int32 il;                 /* interlace of the image data */
          int32 dimsizes[2];        /* dimension sizes of the image */
          int32 n_attr;             /* number of attributes with each image */

          /* Attach to the image */
          riid=GRselect(grid,i);
          CHECK(riid,FAIL,"GRselect");

          /* Get the Image information */
          *name='\0';
          ret=GRgetiminfo(riid,name,&ncomp,&nt,&il,dimsizes,&n_attr);
          CHECK(ret,FAIL,"GRgetiminfo");

          /* Check the name for correctness */
          if(HDstrcmp(name,datafile_info[i].name))
            {
                MESSAGE(3, printf("Name for image %d is: %s, should be %s\n",i,name,datafile_info[i].name););
                num_errs++;
            } /* end if */

          /* Check the # of components */
          if(ncomp!=datafile_info[i].ncomp)
            {
                MESSAGE(3, printf("Number of components for image %d is: %ld, should be %ld\n",i,(long)ncomp,(long)datafile_info[i].ncomp););
                num_errs++;
            } /* end if */

          /* Check the NT of components */
          if(nt!=datafile_info[i].nt)
            {
                MESSAGE(3, printf("NT of components for image %d is: %ld, should be %ld\n",i,(long)nt,(long)datafile_info[i].nt););
                num_errs++;
            } /* end if */

          /* Check the interlace of components */
          if(il!=datafile_info[i].il)
            {
                MESSAGE(3, printf("Interlace of components for image %d is: %ld, should be %ld\n",i,(long)il,(long)datafile_info[i].il););
                num_errs++;
            } /* end if */

          /* Check the x-dimension of the image */
          if(dimsizes[XDIM]!=datafile_info[i].dimsizes[XDIM])
            {
                MESSAGE(3, printf("X-dim of image %d is: %ld, should be %ld\n",i,(long)dimsizes[XDIM],(long)datafile_info[i].dimsizes[XDIM]););
                num_errs++;
            } /* end if */

          /* Check the y-dimension of the image */
          if(dimsizes[YDIM]!=datafile_info[i].dimsizes[YDIM])
            {
                MESSAGE(3, printf("Y-dim of image %d is: %ld, should be %ld\n",i,(long)dimsizes[YDIM],(long)datafile_info[i].dimsizes[YDIM]););
                num_errs++;
            } /* end if */

          /* Check the # of attributes of the image */
          if(n_attr!=datafile_info[i].n_attr)
            {
                MESSAGE(3, printf("# of attributes for image %d is: %ld, should be %ld\n",i,(long)n_attr,(long)datafile_info[i].n_attr););
                num_errs++;
            } /* end if */

          /* End access to the image */
          ret=GRendaccess(riid);
          CHECK(ret,FAIL,"GRendaccess");
      } /* end for */

    /* Shut down the GR interface */
    ret=GRend(grid);
    CHECK(ret,FAIL,"GRend");

    /* Close the file */
    ret=Hclose(fid);
    CHECK(ret,FAIL,"Hclose");
}   /* end test_mgr_image() */

/****************************************************************
**
**  test_mgr_index(): Multi-file Raster ID/Ref/Index Test Routine
** 
****************************************************************/
void
test_mgr_index()
{
    /* Output message about test being performed */
    MESSAGE(6, printf("Testing Multi-file Raster ID/Ref/Index routines\n"););
}   /* end test_mgr_index() */

/****************************************************************
**
**  test_mgr_lut(): Multi-file Raster LUT/Palette Test Routine
** 
****************************************************************/
void
test_mgr_lut()
{
    /* Output message about test being performed */
    MESSAGE(6, printf("Testing Multi-file Raster Palette routines\n"););
}   /* end test_mgr_lut() */

/****************************************************************
**
**  test_mgr_attr(): Multi-file Raster Attribute Test Routine
** 
****************************************************************/
void
test_mgr_attr()
{
    /* Output message about test being performed */
    MESSAGE(6, printf("Testing Multi-file Raster Attribute routines\n"););
}   /* end test_mgr_attr() */

/****************************************************************
**
**  test_mgr(): Main multi-file raster image test routine
** 
****************************************************************/
void
test_mgr()
{
    /*
        Each major outline portion has it's own main function:
        I. Interface Initialization - test_mgr_init
        II. Create Images           - test_mgr_image
        III. ID/Ref/Index Functions - test_mgr_index
        IV. Interlace Functions [Need to be implemented]
        V. Palette Functions        - test_mgr_lut
        VI. Special Element Functions [Need to be implemented]
        VII. Atribute Functions     - test_mgr_attr
    */

    /* Output message about test being performed */
    MESSAGE(5, printf("Testing Multi-file Raster routines\n"););

    test_mgr_init();
    test_mgr_image();
    test_mgr_index();
    test_mgr_lut();
    test_mgr_attr();
}   /* test_mgr() */


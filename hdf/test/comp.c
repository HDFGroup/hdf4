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

/*
 FILE
       comp.c
       Test HDF compressed data I/O routines

 REMARKS
        Currently these only really test RLE compression, at not very well
        at that...

 DESIGN
        These should be written to test all combinations of modeling and
        encoding layers.

 BUGS/LIMITATIONS

 EXPORTED ROUTINES

 AUTHOR
       Quincey Koziol

 MODIFICATION HISTORY
   10/19/93 - Threw this header in.
*/

/* $Id$ */

#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif

#include "tproto.h"

#define TESTFILE_NAME "tcomp.hdf"

#define BUFSIZE 4096

/* last ditch attempt to define this value... */
#ifndef UINT_MAX
#define UINT_MAX (unsigned)(-1)
#endif /* UINT_MAX */

/* make a guess about RAND_MAX if it isn't defined... */
#ifndef RAND_MAX
#define RAND_MAX (UINT_MAX)
#endif /* RAND_MAX */

/* define aliases for random number generation */
#define RAND rand
#define SEED(a) srand((unsigned)(a))

#define COMP_TAG 1000

/* different modeling layers to test */
comp_model_t test_models[]={COMP_MODEL_STDIO};

/* different compression layers to test */
comp_coder_t test_coders[]={
COMP_CODE_NONE,
COMP_CODE_RLE
/*,COMP_CODE_NBIT*/	/* n-bit testing is done in it's own module, nbit.c */
};

int32 test_ntypes[]={
DFNT_INT8 ,DFNT_UINT8 ,DFNT_INT16 ,DFNT_UINT16 ,DFNT_INT32 ,DFNT_UINT32 };

#define NUM_OUTBUFS 3	/* the number of different output buffers to test */
/* outbuf #1 is all zeros (very easy to compress) */
/* outbuf #2 is a fibonacci sequence (very hard to compress) */
/* outbuf #3 is random data (also hard to compress) */
static int8 FAR *outbuf_int8[NUM_OUTBUFS];
static uint8 FAR *outbuf_uint8[NUM_OUTBUFS];
static int16 FAR *outbuf_int16[NUM_OUTBUFS];
static uint16 FAR *outbuf_uint16[NUM_OUTBUFS];
static int32 FAR *outbuf_int32[NUM_OUTBUFS];
static uint32 FAR *outbuf_uint32[NUM_OUTBUFS];

/* only need one input buffer per type of data */
static int8 FAR *inbuf_int8;
static uint8 FAR *inbuf_uint8;
static int16 FAR *inbuf_int16;
static uint16 FAR *inbuf_uint16;
static int32 FAR *inbuf_int32;
static uint32 FAR *inbuf_uint32;

/* local function prototypes */
static void init_model_info(comp_model_t m_type,model_info *m_info,
		int32 test_ntype);
static void init_coder_info(comp_coder_t c_type,comp_info *c_info,
		int32 test_ntype);
static void allocate_buffers(void);
static void init_buffers(void);
static void free_buffers(void);
static uint16 write_data(int32 fid,comp_model_t m_type,model_info *m_info,
		comp_coder_t c_type,comp_info *c_info,intn test_num,
		int32 ntype);
static void read_data(int32 fid,uint16 ref_num,intn test_num,int32 ntype);

static void init_model_info(comp_model_t m_type,model_info *m_info,int32 test_ntype)
{
	/* shut the compiler up */
	m_type=m_type; m_info=m_info; test_ntype=test_ntype;

	switch(m_type) {
		case COMP_MODEL_STDIO:
		default:
			/* don't do anything for this case */
			break;
	  }	/* end switch */
}	/* end init_model_info() */

static void init_coder_info(comp_coder_t c_type,comp_info *c_info,int32 test_ntype)
{
	/* shut the compiler up */
	c_type=c_type; c_info=c_info; test_ntype=test_ntype;

	switch(c_type) {
		case COMP_CODE_RLE:
		case COMP_CODE_NONE:
		default:
			/* don't do anything for this case */
			break;
	  }	/* end switch */
}	/* end init_coder_info() */

static void allocate_buffers(void)
{
	intn i;

	for(i=0; i<NUM_OUTBUFS; i++) {
		outbuf_int8[i]=(int8 *)HDgetspace(BUFSIZE*sizeof(int8));
		outbuf_uint8[i]=(uint8 *)HDgetspace(BUFSIZE*sizeof(uint8));
		outbuf_int16[i]=(int16 *)HDgetspace(BUFSIZE*sizeof(int16));
		outbuf_uint16[i]=(uint16 *)HDgetspace(BUFSIZE*sizeof(uint16));
		outbuf_int32[i]=(int32 *)HDgetspace(BUFSIZE*sizeof(int32));
		outbuf_uint32[i]=(uint32 *)HDgetspace(BUFSIZE*sizeof(uint32));
	  }	/* end for */
	inbuf_int8=(int8 *)HDclearspace(BUFSIZE,sizeof(int8));
	inbuf_uint8=(uint8 *)HDclearspace(BUFSIZE,sizeof(uint8));
	inbuf_int16=(int16 *)HDclearspace(BUFSIZE,sizeof(int16));
	inbuf_uint16=(uint16 *)HDclearspace(BUFSIZE,sizeof(uint16));
	inbuf_int32=(int32 *)HDclearspace(BUFSIZE,sizeof(int32));
	inbuf_uint32=(uint32 *)HDclearspace(BUFSIZE,sizeof(uint32));
}	/* allocate_buffers() */

static void init_buffers(void)
{
	uint32 last_fib,curr_fib,next_fib;
	intn r;
	intn i,j;

	for(i=0; i<NUM_OUTBUFS; i++) {
		switch(i) {
			case 0:	/* all zero filled */
				HDmemset(outbuf_int8[i],0,BUFSIZE*sizeof(int8));
				HDmemset(outbuf_uint8[i],0,BUFSIZE*sizeof(uint8));
				HDmemset(outbuf_int16[i],0,BUFSIZE*sizeof(int16));
				HDmemset(outbuf_uint16[i],0,BUFSIZE*sizeof(uint16));
				HDmemset(outbuf_int32[i],0,BUFSIZE*sizeof(int32));
				HDmemset(outbuf_uint32[i],0,BUFSIZE*sizeof(uint32));
				break;

			case 1: /* fibonacci sequence */
				for(j=0,last_fib=0,curr_fib=1; j<BUFSIZE; j++) {
					outbuf_int8[i][j]=(int8)curr_fib;
					outbuf_uint8[i][j]=(uint8)curr_fib;
					outbuf_int16[i][j]=(int16)curr_fib;
					outbuf_uint16[i][j]=(uint16)curr_fib;
					outbuf_int32[i][j]=(int32)curr_fib;
					outbuf_uint32[i][j]=(uint32)curr_fib;
					next_fib=curr_fib+last_fib;
					last_fib=curr_fib;
					curr_fib=next_fib;
				  }	/* end for */
				break;

			case 2: /* random #'s */
				SEED(time(NULL));
				for(j=0; j<BUFSIZE; j++) {
					r=RAND();
					outbuf_int8[i][j]=(int8)(r-RAND_MAX/2);
					outbuf_uint8[i][j]=(uint8)r;
					outbuf_int16[i][j]=(int16)(r-RAND_MAX/2);
					outbuf_uint16[i][j]=(uint16)r;
					outbuf_int32[i][j]=(int32)(r-RAND_MAX/2);
					outbuf_uint32[i][j]=(uint32)r;
				  }	/* end for */
				break;
		  }	/* end switch */
	  }	/* end for */
}	/* init_buffers() */

static void free_buffers(void)
{
	intn i;

	for(i=0; i<NUM_OUTBUFS; i++) {
		HDfreespace(outbuf_int8[i]);
		HDfreespace(outbuf_uint8[i]);
		HDfreespace(outbuf_int16[i]);
		HDfreespace(outbuf_uint16[i]);
		HDfreespace(outbuf_int32[i]);
		HDfreespace(outbuf_uint32[i]);
	  }	/* end for */
	HDfreespace(inbuf_int8);
	HDfreespace(inbuf_uint8);
	HDfreespace(inbuf_int16);
	HDfreespace(inbuf_uint16);
	HDfreespace(inbuf_int32);
	HDfreespace(inbuf_uint32);
}	/* free_buffers() */

static uint16 write_data(int32 fid,comp_model_t m_type,model_info *m_info,
	comp_coder_t c_type,comp_info *c_info,intn test_num,int32 ntype)
{
	int32 aid;
	uint16 ret_ref;
	int32 err_ret;
	int32 write_size;
	VOIDP data_ptr;

	MESSAGE(8, printf("Writing data for test %d, ntype=%d, model_type=%d, coder_type=%d\n",(int)test_num,(int)ntype,(int)m_type,(int)c_type);)
	ret_ref=Hnewref(fid);
	aid=HCcreate(fid,COMP_TAG,ret_ref,m_type,m_info,c_type,c_info);
    CHECK(aid, FAIL, "HCcreate");
	if(aid==FAIL)
		return(0);

	switch(ntype) {
		case DFNT_INT8:
			data_ptr=(VOIDP)outbuf_int8[test_num];
			break;
		case DFNT_UINT8:
			data_ptr=(VOIDP)outbuf_uint8[test_num];
			break;
		case DFNT_INT16:
			data_ptr=(VOIDP)outbuf_int16[test_num];
			break;
		case DFNT_UINT16:
			data_ptr=(VOIDP)outbuf_uint16[test_num];
			break;
		case DFNT_INT32:
			data_ptr=(VOIDP)outbuf_int32[test_num];
			break;
		case DFNT_UINT32:
			data_ptr=(VOIDP)outbuf_uint32[test_num];
			break;
		default:
			return(0);
	  }	/* end switch */

	write_size=BUFSIZE*DFKNTsize(ntype);
    err_ret = Hwrite(aid, write_size, data_ptr);
    if(err_ret != write_size) {
      fprintf(stderr, "ERROR(%d): Hwrite returned the wrong length: %d\n", __LINE__,(int)err_ret);
      HEprint(stdout,0);
      num_errs++;
    }

    err_ret = Hendaccess(aid);
    CHECK(err_ret, FAIL, "Hendaccess");

	return(ret_ref);
}	/* end write_data() */

static void read_data(int32 fid,uint16 ref_num,intn test_num,int32 ntype)
{
	int32 aid;
	int32 err_ret;
	int32 read_size;
	VOIDP out_ptr;
	VOIDP in_ptr;

	MESSAGE(8, printf("Reading data for test %d, ntype=%d\n",(int)test_num,(int)ntype);)

	aid=Hstartread(fid,COMP_TAG,ref_num);
    CHECK(aid, FAIL, "Hstartread");
	if(aid==FAIL)
		return;

	switch(ntype) {
		case DFNT_INT8:
			out_ptr=(VOIDP)outbuf_int8[test_num];
			in_ptr=(VOIDP)inbuf_int8;
			break;
		case DFNT_UINT8:
			out_ptr=(VOIDP)outbuf_uint8[test_num];
			in_ptr=(VOIDP)inbuf_uint8;
			break;
		case DFNT_INT16:
			out_ptr=(VOIDP)outbuf_int16[test_num];
			in_ptr=(VOIDP)inbuf_int16;
			break;
		case DFNT_UINT16:
			out_ptr=(VOIDP)outbuf_uint16[test_num];
			in_ptr=(VOIDP)inbuf_uint16;
			break;
		case DFNT_INT32:
			out_ptr=(VOIDP)outbuf_int32[test_num];
			in_ptr=(VOIDP)inbuf_int32;
			break;
		case DFNT_UINT32:
			out_ptr=(VOIDP)outbuf_uint32[test_num];
			in_ptr=(VOIDP)inbuf_uint32;
			break;
		default:
			return;
	  }	/* end switch */

	read_size=BUFSIZE*DFKNTsize(ntype);
    err_ret = Hread(aid, read_size, in_ptr);
    if(err_ret != read_size) {
		fprintf(stderr, "ERROR(%d): Hwrite returned the wrong length: %d\n", __LINE__,(int)err_ret);
		HEprint(stdout,0);
		num_errs++;
      } /* end if */

	if(HDmemcmp(in_ptr,out_ptr,read_size)!=0) {
		char *s=HDgetNTdesc(ntype);

		fprintf(stderr, "ERROR: Data from test %d, number type %s differs\n",test_num,s);
		HDfreespace(s);
		num_errs++;
	  }	/* end if */

    err_ret = Hendaccess(aid);
    CHECK(err_ret, FAIL, "Hendaccess");
}	/* end read_data() */

void test_comp(void)
{
    model_info m_info;
    comp_info c_info;
    uint16 ref_num;		/* reference number of the data written out */
	int32 fid;			/* file ID of HDF file for testing */
    intn test_num,ntype_num,model_num,coder_num;
	int32 ret;

	MESSAGE(6, printf("Starting compression test\n");)

    /* allocate room for the input and output buffers */
    allocate_buffers();

    /* fill the buffers with interesting data to compress */
    init_buffers();

	/* open the HDF file */
	fid=Hopen(TESTFILE_NAME,DFACC_ALL,0);

    /* Cycle through the different testing data, the number types, */
    /* the different modeling layers and the different coding layers, */
    /* in that order */
    for(test_num=0; test_num<NUM_OUTBUFS; test_num++) {
		for(ntype_num=0; ntype_num<(sizeof(test_ntypes)/sizeof(test_ntypes[0]));
				ntype_num++) {
            for(model_num=0;
                    model_num<(sizeof(test_models)/sizeof(test_models[0]));
                    model_num++) {
				init_model_info(test_models[model_num],&m_info,test_ntypes[ntype_num]);
                for(coder_num=0;
						coder_num<(sizeof(test_coders)/sizeof(test_coders[0]));
						coder_num++) {
					init_coder_info(test_coders[coder_num],&c_info,test_ntypes[ntype_num]);

					ref_num=write_data(fid,test_models[model_num],&m_info,test_coders[coder_num],&c_info,test_num,test_ntypes[ntype_num]);
					read_data(fid,ref_num,test_num,test_ntypes[ntype_num]);
                  } /* end for */
              } /* end for */
          } /* end for */
      } /* end for */

	/* close the HDF file */
	ret=Hclose(fid);
    CHECK(ret, FAIL, "Hclose");

    /* free the input and output buffers */
    free_buffers();

	MESSAGE(6, printf("Finished compression test\n");)
}   /* end test_comp() */

#ifdef OLD_WAY
static uint8 FAR outbuf[BUFSIZE],
    FAR inbuf[BUFSIZE];

static uint8 FAR outbuf2[BUFSIZE],
    FAR inbuf2[BUFSIZE];

void test_comp()
{
    int32 fid;
    int32 aid1;
    int32 fileid, length, offset, posn;
    uint16 tag, ref, ref1, ref2, ref3;
    int16 acc_mode, special;
    register int i,j;
    int32 ret;
    intn errors = 0;
    model_info m_info;
    comp_info c_info;

    for (i=0; i<BUFSIZE; i++)
        outbuf[i] = (char) (i % 256);
    for(i=0; i<BUFSIZE; i++)
        for(j=0; j<130 && i<BUFSIZE; j++,i++)
            outbuf2[i]=(uint8)i;

    MESSAGE(5,printf("Creating a file %s\n", TESTFILE_NAME););
    fid = Hopen(TESTFILE_NAME, DFACC_CREATE, 0);
    CHECK(fid, FAIL, "Hopen");

#ifdef QAK
    MESSAGE(5,printf("Write an element and then promote to Linked Blocks\n"););
    ret = Hputelement(fid, (uint16) 1000, (uint16) 1,
                     (uint8 *) "element 1000 1 wrong ",
                      HDstrlen("element 1000 1 wrong ") + 1);
    CHECK(ret, FAIL, "Hputelement");

    aid1 = HLcreate(fid, 1000, 1, 10,10);
    CHECK(aid1, FAIL, "HLcreate");

    ret = Hseek(aid1, HDstrlen("element 1000 1 "), DF_START);
    CHECK(ret, FAIL, "Hseek");

    ret = Hwrite(aid1, HDstrlen("correct")+1, (uint8 *) "correct");
    if(ret != (int32)HDstrlen("correct") + 1) {
      fprintf(stderr, "ERROR: (%d) Hwrite returned the wrong length: %d\n", __LINE__,ret);
      errors++;
    }

    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");
#endif

    ref1 = Hnewref(fid);
    CHECK(ref1, 0, "Hnewref");

    MESSAGE(5,printf("Create a new element as a compressed element\n"););
    aid1 = HCcreate(fid, 1000, ref1, COMP_MODEL_STDIO, &m_info,
            COMP_CODE_RLE, &c_info);
    CHECK(aid1, FAIL, "HCcreate");

    ret = Hwrite(aid1, BUFSIZE/2, outbuf);
    if(ret != BUFSIZE/2) {
      fprintf(stderr, "ERROR(%d): Hwrite returned the wrong length: %d\n", __LINE__,(int)ret);
      HEprint(stdout,0);
      errors++;
    }

    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    ref2 = Hnewref(fid);
    CHECK(ref2, 0, "Hnewref");

    aid1 = HCcreate(fid, 1000, ref2, COMP_MODEL_STDIO, &m_info,
            COMP_CODE_RLE, &c_info);
    CHECK(aid1, FAIL, "HCcreate");

    ret = Hwrite(aid1, BUFSIZE/2, outbuf2);
    if(ret != BUFSIZE/2) {
      fprintf(stderr, "ERROR: (%d) Hwrite returned the wrong length: %d\n", __LINE__,(int)ret);
      HEprint(stdout,0);
      errors++;
    }

    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    MESSAGE(5,printf("Verifying data\n"););
    ret = Hgetelement(fid, (uint16) 1000, (uint16) ref1, inbuf);
    if(ret != BUFSIZE/2) {
      HEprint(stderr,0);
      fprintf(stderr, "ERROR: (%d) Hgetelement returned the wrong length: %d\n",__LINE__, (int)ret);
      errors++;
    }

    for (i=0; i < ret; i++) {
       if (inbuf[i] != outbuf[i]) {
           printf("Wrong data at %d, out %d in %d\n", i, outbuf[i], inbuf[i]);
           errors++;
         }
       inbuf[i] = '\0';
    }

    ret = Hgetelement(fid, (uint16) 1000, (uint16) ref2, inbuf2);
    if(ret != BUFSIZE/2) {
      HEprint(stderr,0);
      fprintf(stderr, "ERROR: (%d) Hgetelement returned the wrong length: %d\n", __LINE__,(int)ret);
      errors++;
    }

    for (i=0; i < ret; i++) {
       if (inbuf2[i] != outbuf2[i]) {
           printf("Wrong data at %d, out %d in %d\n", i, outbuf2[i], inbuf2[i]);
           errors++;
         }
       inbuf2[i] = '\0';
    }

    ref3 = Hnewref(fid);
    CHECK(ref3, 0, "Hnewref");

    aid1 = HCcreate(fid, 1020, ref3, COMP_MODEL_STDIO, &m_info,
            COMP_CODE_RLE, &c_info);
    CHECK(aid1, FAIL, "HCcreate");

    ret = Hwrite(aid1, BUFSIZE, outbuf);
    if(ret != BUFSIZE) {
      fprintf(stderr, "ERROR: (%d) Hwrite returned the wrong length: %d\n", __LINE__,(int)ret);
      errors++;
    }

    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    MESSAGE(5,printf("Closing and re-opening file %s\n", TESTFILE_NAME););
    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");

    fid = Hopen(TESTFILE_NAME, DFACC_RDWR, 0);
    CHECK(fid, FAIL, "Hopen");

    MESSAGE(5,printf("Verifying data\n"););

#ifdef QAK
    aid1 = Hstartread(fid, 1000, 1);
    CHECK(aid1, FAIL, "Hstartread");

    ret = Hinquire(aid1, &fileid, &tag, &ref, &length, &offset, &posn,
                  &acc_mode, &special);
    CHECK(ret, FAIL, "Hinquire");
    if(!special) {
      fprintf(stderr, "ERROR: Hinquire does not think element is special line %d\n",
              __LINE__);
      errors++;
    }

    ret = Hread(aid1, length, inbuf);
    if(ret != 23) {
      fprintf(stderr, "ERROR: (%d) Hread returned the wrong length: %d\n", _LINE__,ret);
      errors++;
    }

    if(HDstrcmp((const char *) inbuf, (const char *) "element 1000 1 correct")) {
      fprintf(stderr, "ERROR: (%d) Hread returned the wrong data\n",__LINE__);
      fprintf(stderr, "\t       Is: %s\n", inbuf);
      fprintf(stderr, "\tShould be: element 1000 1 correct\n");
      errors++;
    }

    ret = Hnextread(aid1, 1000, DFREF_WILDCARD, DF_CURRENT);
    CHECK(ret, FAIL, "Hnextread");
#else

    ret = Hstartread(fid, 1000, ref1);
    CHECK(ret, FAIL, "Hstartread");
#endif

    ret = Hinquire(aid1, &fileid, &tag, &ref, &length, &offset, &posn,
                  &acc_mode, &special);
    CHECK(ret, FAIL, "Hinquire");
    if(!special) {
      fprintf(stderr, "ERROR: Hinquire does not think element is special line %d\n",
              __LINE__);
      errors++;
    }

#ifdef QAK
    MESSAGE(5,printf("Writing to existing element\n"););
    aid2 = Hstartwrite(fid, 1000, 1, ref1);
    CHECK(aid2, FAIL, "Hstartwrite");

    ret = Hwrite(aid2, 4, (uint8 *) "ABCD");
    if(ret != 4) {
      fprintf(stderr, "ERROR: (%d) Hwrite returned the wrong length: %d\n", __LINE__,ret);
      errors++;
    }
#endif

    /* let's try to write to a read element for fun */
    ret = Hwrite(aid1, 4, (uint8 *) "ABCD");
    if(ret != FAIL) {
      fprintf(stderr, "ERROR: (%d) Hwrite allowed write to read access obj\n",__LINE__);
      errors++;
    }

    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    MESSAGE(5,printf("Closing the files\n"););
    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");
}
#endif /* OLD_WAY */


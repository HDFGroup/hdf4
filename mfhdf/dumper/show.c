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

/* Modified from vshow.c by Eric Tsui, 12/25/1994. */

#ifdef RCSID
static char *RcsId[] = "@(#)$Revision$";
#endif

#include "vg.h"

static int32 cn = 0;

/* ------------------------------------------------ */
/* Each of the following functions displays one element. */
int32
vfmtbyte(unsigned char *x, int display, FILE *fp)
{
   if (display)
     cn += fprintf(fp, "%02x ", *x);
   return (1);
} 

int32
vfmtchar(char *x, int display, FILE *fp)
{
   if (display) {
      cn++;
      /* putchar(*x); */
      fprintf(fp, "%c ", *x); 
   }
   return(1);
}

int32
vfmtint(char *x, int display, FILE *fp)
{
     int i=0;
     HDmemcpy(&i, x, sizeof(int32));
     if (display)
        cn+=fprintf(fp, "%d",i);
     return(1);
}

int32
vfmtfloat(char *x, int display, FILE *fp)
{
    float       f = 0;
    HDmemcpy(&f, x, sizeof(float32));
    if (display)
       cn += fprintf(fp, "%f", f);
    return (1);
}

int32
vfmtlong(char *x, int display, FILE *fp)
{
    long        l = 0;
    HDmemcpy(&l, x, sizeof(int32));
    if (display)
       cn += fprintf(fp, "%ld", l);
    return (1);
}

int32
vfmtshort(char *x, int display, FILE *fp)
{
     short s=0;
     HDmemcpy(&s, x, sizeof(int16));
     if (display)
        cn += fprintf(fp, "%ld",1);
     return(1);
}


int32
vfmtdouble(char *x, int display, FILE *fp)
{
    double      d = 0;
    HDmemcpy(&d, x, sizeof(float64));
    if (display)
       cn += fprintf(fp, "%f", d);
    return (1);
}


#define BUFFER 1000000

/* ------------------------------------------------ */

int32
dumpvd(int32 vd, int data_only, FILE *fp, char separater[2], 
       int flds_indices[100], int dumpallfields)
{
    char        vdname[100], fields[FIELDNAMELENMAX], flds[FIELDNAMELENMAX];
    int32       j, i, t, interlace, nv, vsize;
    uint8      *bb, *b;
    VWRITELIST *w;
    int32       (*vfmtfn[60]) ();
    int32       off[60];
    int32       order[60];

    int32       bufsize;        /* size of the buffer we are using */
    int32       chunk;          /* number of rows that will fit in the buffer */
    int32       done;           /* number of rows we have done */
    int32       count; 		/* number of rows to do this time through 
    				   the loop */

    int32       nf; 		/* number of fields in this Vdata */
    int32	x, display;
    int32	temp, addr_width, num_digits, address;
    int32 	num_flds, num_recs, cnt1, cnt2;

    VSinquire(vd, &nv, &interlace, fields, &vsize, vdname);

    if (nv * vsize > BUFFER) /* If the number of records in the vdata is 
				bigger than the buffer size, then divide
				the records into chunks. */
      {
          bufsize = BUFFER;
          chunk = BUFFER / vsize;
      }
    else /* Otherwise, all the records will be read in at one time. */
      {
          bufsize = nv * vsize;
          chunk = nv;
      }

    done = 0;
    bb = (uint8 *) HDgetspace(bufsize); /* Allocate space for the buffer. */
    if (bb == NULL)
      {
          printf("dumpvd malloc error\n");
          return (0);
      }
    VSsetfields(vd, fields);
    w = vswritelist(vd);

    nf = w->n;
    x = 0; /* Used for accessing the array storing the indices of the 
	      selected fields. */
    for (i = 0; i<nf; i++) /* Read in data of all the fields. */
      {
          order[i] = w->order[i];

	  /* Set offset for the next element. */
          off[i] = DFKNTsize(w->type[i] | DFNT_NATIVE); 
   
	  /* Display the header of a vdata if the user didn't specify the
	     data-only option. */
	  if (!data_only) {
	     if ((dumpallfields) || (flds_indices[x]==i)) {
                fprintf(fp, "- field index %d: [%s], type=%d, order=%d\n", 
	               (int) i, w->name[i], w->type[i], w->order[i]);
	     x++; 
	     }
	  } /* if */

	  /* Choose a function for displaying a piece of data of a 
	     particular type. */
          switch (w->type[i])
            {
                case DFNT_CHAR:
                    vfmtfn[i] = vfmtchar;
                    break;

                case DFNT_UINT8:
                case DFNT_INT8:
                    vfmtfn[i] = vfmtbyte;
                    break;

                case DFNT_UINT16:
                case DFNT_INT16:
                    vfmtfn[i] = vfmtshort;
                    break;

                case DFNT_UINT32:
                case DFNT_INT32:
                    vfmtfn[i] = vfmtlong;
                    break;

                case DFNT_FLOAT32:
                    vfmtfn[i] = vfmtfloat;
                    break;

                case DFNT_FLOAT64:
                    vfmtfn[i] = vfmtdouble;
                    break;

                default:
                    fprintf(stderr, "sorry, type [%d] not supported\n", 
			    (int) w->type[i]);
                    break;

            } /* switch */
      } /* for */
    cn = 0;
    done = count = 0;

    /* If not just the data will be dumped out, then put an address-type
       column on the left so that the user can recognize which record 
       he/she is looking at. */
    if (!data_only) {
       temp = nv/10;
       address = 0;
       addr_width = num_digits = 1;
       while (temp!=0) {
	  if (temp!=1)
             addr_width++;
	  temp = temp/10;
       }
       fprintf(fp, "Loc.");
       for (j=0; j<addr_width-3; j++)
          fprintf(fp, " ");
       fprintf(fp, "     Data\n");

       /* The address of the first record is 0. Also, fill in the extra 
	  space on the left with 0's. */
       while (num_digits <= addr_width) {
          fprintf(fp, "0");
          num_digits++;
          cn++; 
       }
       fprintf(fp, "      ");
       cn += 6;
       if (addr_width == 2) {
	  fprintf(fp, " ");
          cn++;
       }
       else if (addr_width == 1) {
          fprintf(fp, "  ");
          cn += 2;
       }
    } /* while */   
    num_flds = vsize/VSgetfields(vd, flds); 

    cnt1 = 0;
    cnt2 = 0;
    while (done != nv)
      {
          /* Determine the amount of data to be read this time. */
          if ((nv - done) > chunk)
              count = chunk;
          else
              count = nv - done;

          /* read and update bookkeeping */
          VSread(vd, bb, count, interlace);
          done += count;
          b = bb;

          /* Display the data. */
          for (j = 0; j < count; j++) /* each iteration causes one record 
					 to be printed */
            {   
		cnt1++;
		x = 0;
                for (i = 0; i < nf; i++) /* display all fields in one record */
                  {   
		      if ((!dumpallfields) && (flds_indices[x]!=i))
		         display = 0;
  		      else {
			 display = 1;
			 x++;
		      }
		    
                      for (t = 0; t < order[i]; t++) {
                            (vfmtfn[i]) (b, display, fp);
                            b += off[i];
			    if (display) {
                               fprintf(fp, " ");
                               cn++;
			       cnt2++;
			    }
                      }
		      if (display) {
                         fprintf(fp, " ");
                         cn++;
		         cnt2++;
		      }
                  } /* for i to nf-1 */


                  if (cnt2>0) {
		     address++;
		     /* "separator" is the symbol used for separating 
			different records. */
		     fprintf(fp, "%s ",separater);
	          }

		  /*
		  if (separater[0] == ";") {
		     fprintf(fp, " ");
		     cn += 2; 
		  }
		  else 
		     cn += 6; 
		  */

                if (!data_only)
                  {
		     if ((cnt1*cnt2) > 30)
                     /* if (cn > 50) */
                        {   
			    cnt1 = 0;
			    cnt2 = 0;
			    fprintf(fp, "\n");
                            cn = 0; 
		            
			    /* As long as there is data to be displayed,
			       fill in the extra space with 0's on the left
			       of each address. */
			    if (j<(count-1)) {
                               temp = address;
			       num_digits = 1;
			       while ((temp = temp/10)!=0)
			          num_digits++;
                               while (num_digits < addr_width) {
			          fprintf(fp, "0");
			          num_digits++;
			          cn++;  
                               }
			       fprintf(fp, "%d      ", address);
			       cn += 6+num_digits;
			       if (addr_width == 2) {
			          fprintf(fp, " ");
			          cn++; 
			       }
			       else if (addr_width == 1) {
			          fprintf(fp, "  ");
			         cn += 2; 
			       }
			   } /* if (!data_only) */
                        }
                  }
                else
                  {
                      fprintf(fp, "\n");
                  }
            } /* for (j=0; j<count; j++) */
      } /* while (done != nv) */

    /* ============================================ */

    HDfreespace((VOIDP) bb);
    fprintf(fp, "\n\n");

    return (1);

}   /* dumpvd */
/* ------------------------------------- */

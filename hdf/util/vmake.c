/*****************************************************************************
*
* vmake.c
*
*	HDF Vset utility.
*
*	vmake:  utility creates vsets. May be used in 3 ways:
*			(1)	add a new vgroup.
*			(2)	add a new vdata from ascii data.
*			(3)	link vgroups and vdatas to a vgroup.
*
*    USAGE:
*			(1) 	vmake file vgname
*			(2)	vmake file vsname {format} {ascii data stream}
*			(3)	vmake file -l vgref v1 v2 ... vn
*
*
******************************************************************************/

#include "vg.h"

/*
 * Prototypes of local functions
 */
int32 vsetlink
  PROTO((char *hfile, int32 vgid, int32 ids[], int32 n));

int32 vgadd
  PROTO((char *hfile, char *vgname));

int32 inpdata 
  PROTO((unsigned char**bp));

void vsadd
  PROTO((char *hfile, char *vsname, char *format));

int32 scanit
  PROTO((char *string, char ***fields, int32 **type, int32 **order));

int32 savfld
  PROTO((char *ss, int p1, int p2));

int32 compact
  PROTO((char *ss, char *dd));

int32 savtype
  PROTO((char *ss, int p1, int p2));

int32 separate
  PROTO((char *ss, char *fmt, int32 *num));

int show_help_msg
#ifdef CONVEX
    PROTO(());
#else
    PROTO((VOID));
#endif

/*
 *  Main entry point
 */
#ifdef PROTOTYPE
main(int ac,char **av)
#else
main(ac,av)
int ac;
char**av;
#endif
{

  char  *hfile, *vgname, *vsname, *fmt;

  if (ac <3 ) {
		show_help_msg();
		exit(0);
		}
  else if (ac==3)  {
      if (!HDstrcmp(av[2],"-l")) { show_help_msg() ; exit(0); }
  		hfile  = av[1];
  		vgname = av[2];
 		vgadd(hfile,vgname);
		}

  else if (ac==4)  {
      if (!HDstrcmp(av[2],"-l")) { show_help_msg() ; exit(0); }
  		hfile    = av[1];
  		vsname   = av[2];
  		fmt      = av[3];
 		vsadd(hfile,vsname,fmt);
		}

  else if (!HDstrcmp(av[2],"-l")) {
    int i;
	int32 n, vgref, ids[50];

	  	hfile 	= av[1];
		sscanf(av[3],"%d",&vgref);
		for(n=0,i=4;i<ac;i++,n++) { sscanf(av[i],"%d",&ids[n]); }
		vsetlink(hfile,vgref,ids,n);
		}
  else {
		show_help_msg();
		exit(0);
		}
    return(0);
} /* main */
  
void showfmttypes() {
	fprintf(stderr,"\tvalid fmt types: \n");
	fprintf(stderr,"\t  c - char    (char in HDF file)    \n");
	fprintf(stderr,"\t  b - byte    (int8 in HDF file)    \n"); 
	fprintf(stderr,"\t  s - short   (int16 in HDF file)   \n");
	fprintf(stderr,"\t  l - long    (int32 in HDF file)   \n");
	fprintf(stderr,"\t  f - float   (float32 in HDF file) \n");
	}

#if defined ( PROTOTYPE ) && ! defined ( CONVEX )
int show_help_msg(VOID)
#else
int show_help_msg()
#endif
{

  printf("\nvmake: creates vsets.\n");
  printf("\nUSAGE:\n");
  
  printf(" (1) vmake file vgname             (adds a new vgroup)\n");
  printf(" (2) vmake file vsname format      (adds a new vdata)\n");
  printf(" (3) vmake file -l vgref v1 .. vn  (links v1 v2 .. vn into vgref)\n");
  
  printf("\nwhere\n");
  printf("  vgref is the ref of a vgroup\n");
  printf("  v1,..,vn are refs of vgroups and vdatas\n");
  printf("  format is <field=fmt,field=fmt,..>\n");
  printf("    field is any text string\n");
  printf("    fmt is one of the following optionally preceded by a decimal.\n");
  showfmttypes();
  
  printf("\nTo create a vdata, vmake reads ascii data from stdin\n");
  
  printf("EXAMPLES:\n");
  printf("\t cat dat.txt | vmake hh.hdf \"triangles\" \"PLIST3=3l\"\n");
  printf("\t vmake abc.hdf \"xyvals\" \"X=d,Y=f\" < abc.dat\n");
  printf("\n");
  
  return (1);
  
} /* show_help_msg */

/* ------------------------------------------------------- */

#ifdef PROTOTYPE
int32 vsetlink(char *hfile, int32 vgid, int32 ids[], int32 n)
#else
int32 vsetlink(hfile,vgid,ids,n)
char * hfile;
int32 vgid, n, ids[];
#endif
{
  HFILEID f;
  int32 vgmain, vg;
  int32 vs;
  int32 err=0;
  int32 i;

	f = Hopen(hfile,DFACC_ALL,0);
	if (f == FAIL) {
	  fprintf(stderr,"cannot open %s.  \n",hfile); exit(0); 
	  }
        Vinitialize(f);
  vgmain = Vattach(f,vgid,"w");
  if(vgmain==FAIL) { fprintf(stderr, "0\n"); Vfinish(f); Hclose(f); exit(-1);}

    for(i=0;i<n;i++) {
        if( -1 != vexistvg(f,(uint16)ids[i])) {
            if ((vg=Vattach(f,ids[i],"r")) != FAIL)  {
                if (Vinsert(vgmain,vg)  == -1)  { /*  is really VGROUP* */
                    err = 1;
                    fprintf(stderr,"insert a vg (%d)fails!!\n",ids[i]);
                }
                Vdetach(vg);
            }
        }
        else if ( -1 != vexistvs(f,(uint16)ids[i])) {
            if ((vs= VSattach(f,ids[i],"r")) != FAIL) {
                if (Vinsert(vgmain,vs) == FAIL) {
                    err = 1;
                    fprintf(stderr,"insert a vs (%d)fails!!\n",ids[i]);
                }
                VSdetach(vs);
            }
        }
        else {
            fprintf(stderr,"no such vgroup or vdata [%d]\n",ids[i]);
            err= 1;
        }
    }

  Vdetach(vgmain);
  Vfinish(f);
  Hclose(f);

  if (err) exit(-1); 
  else fprintf(stderr,"1\n"); /* success */
  return (1);
} /* vsetlink */

/* ------------------------------------------------------- */
/* 
add a (new) vgroup to the file 
*/

#ifdef PROTOTYPE
int32 vgadd (char *hfile, char *vgname) 
#else
int32 vgadd (hfile,vgname) 
char * hfile;
char * vgname;
#endif
{
  HFILEID f;
  int32 ref; 
  int32 vg;

  f=Hopen(hfile,DFACC_ALL,0);
  if (f==FAIL) {
	  fprintf(stderr,"cannot open %s. \n",hfile); exit(0); 
	  }
  Vinitialize(f);
  vg = Vattach(f,-1,"w");
  if (vg==FAIL) { fprintf(stderr,"cannot attach vg\n"); exit(0); }
  ref = VQueryref(vg);
  Vsetname(vg,vgname);
  Vdetach(vg);
  Vfinish(f);
  Hclose(f);
  fprintf(stderr,"%d\n",ref);
  return(1);

} /* vgadd */

/* ------------------------------------------------------- */
/*
 add a (new) vdata to the file.
 Data will be ascii and will come in from stdin
 according to the format (c-style).
*/
#ifdef PROTOTYPE
void vsadd (char *hfile, char *vsname, char *format)
#else
void vsadd (hfile,vsname,format)
char * hfile;
char * vsname;
char * format;
#endif
{
  int32 stat, i,n, nwritten;
  unsigned char *buf;
  char **fields;
  int32 *type, *order, nfld;
  char allfields[100];
  HFILEID	f;
  int32 vs;
  int32 ref,ftype;

  nfld = scanit(format,&fields,&type,&order);
  if (nfld < 1) { fprintf(stderr,"bad fields\n"); exit(-1); }

  if ( (f=Hopen(hfile,DFACC_ALL,0))==FAIL) {
	  fprintf(stderr,"cannot open %s.  \n",hfile); exit(-1); 
	  }
  Vinitialize(f);
  vs = VSattach(f,-1,"w");
  ref = VSQueryref(vs);

printf("vsadd: ref is %d\n",ref);

  allfields[0] = '\0';
  for (i=0;i<nfld;i++) {
	  switch (type[i]) {
		  case 'c': ftype = DFNT_CHAR;       break;
		  case 's': ftype = DFNT_INT16;      break;
		  case 'f': ftype = DFNT_FLOAT32;    break;
		  case 'l': ftype = DFNT_INT32;      break;
		  case 'b': ftype = DFNT_INT8;       break; 
		  case 'D': ftype = DFNT_DOUBLE;     break; 

		  default:  fprintf(stderr,"bad type [%c]\n",type[i]); 
						showfmttypes();
						exit(-1);
						break;
		  }
	  stat = VSfdefine(vs,fields[i],ftype,order[i]);
      HDstrcat(allfields,fields[i]);
      HDstrcat(allfields,",");
     }

  i=HDstrlen(allfields); allfields[i-1]='\0'; /* remove last comma */

  VSsetname(vs,vsname);
  stat = VSsetfields(vs,allfields);

  nwritten = 0;
  while( (n = inpdata(&buf)) > 0) {
	 /*  printf("inpdata rets n=%d .. ",n); */
    stat = VSwrite(vs,buf,n,FULL_INTERLACE);
     printf("+%d  \n",stat); 
	 nwritten +=n;
	 if (stat < 1) fprintf(stderr,"Vswrite stat=%d\n",stat);
	 }
  VSdetach(vs);
  Vfinish(f);
  Hclose(f);
  fprintf(stderr,"%d\n",ref,nwritten);
  return;

} /* vsadd */

/* ------------------------------------------------------------------ */
/* This part of the code deals with formatting stdin input data.      */
/* ------------------------------------------------------------------ */

#include <stdio.h>


#define MAXVAR 32
static char *fldptr[MAXVAR];
static char flds[MAXVAR][100];
static char fmts[MAXVAR]; 
static int32  fords[MAXVAR]; 
static int32  ftyp[MAXVAR]; 
static int  ntotal = 0;


/* scanf functions */
#ifdef PROTOTYPE
static int32 inplong  (int32 *x)
#else
static int32 inplong  (x)
int32  *x;
#endif
{
int val, ret;

    ret = scanf ("%ld ",&val);
    *x = (int32)val;
    return(ret);
}

#ifdef PROTOTYPE
static int32 inpshort  (int16 *x)
#else
static int32 inpshort  (x)
short  *x;
#endif
{
int ret, val;

    ret = scanf ("%d ", &val);
    *x = (int16)val;
    return(ret);
}

#ifdef PROTOTYPE
static int32 inpbyte  (int8 *x)
#else
static int32 inpbyte  (x)
int8  *x;
#endif
{
int ret;
int val;

    ret = scanf ("%d ", &val);
    *x = (int8)val;
    return(ret);
}

#ifdef PROTOTYPE
static int32 inpfloat(float32 *x)
#else
static int32 inpfloat(x)
float32 *x;
#endif
{
int ret;
float val;

    ret = scanf ("%f ", &val);
    *x = (float32)val;
    return(ret);
}

#ifdef PROTOTYPE
static int32 inpchar (char *x)
#else
static int32 inpchar (x)
char *x;
#endif
{
    return(scanf ("%c ",x));
}


#define BUFSIZE 40000
unsigned char inpbuffer[BUFSIZE];

#ifdef PROTOTYPE
int32 inpdata (unsigned char **bp)
#else
int32 inpdata (bp)
     unsigned char**bp; 
#endif
{ 
  int32 totalsize, nread, t,i,j,k;
  unsigned char *b;
  int32 maxrec;
  int32 (*inpfn[MAXVAR])();
  int32 inpsiz[MAXVAR];

     for(i=0;i<ntotal;i++) {
	    switch(fmts[i]) {

		   case 'c':
			  inpfn[i]  = inpchar; inpsiz[i] = sizeof(char);
			  break;

		   case 'b':
			  inpfn[i]  = inpbyte; inpsiz[i] = sizeof(int8);
			  break;

		   case 's':
		          inpfn[i]  = inpshort; inpsiz[i] = sizeof(short);
			  break;

		   case 'l':
			  inpfn[i]  = inplong; inpsiz[i] = sizeof(long);
			  break;


		   case 'f':
			  inpfn[i] = inpfloat; inpsiz[i] = sizeof(float);
			  break;


			default: printf("inpdata: fmt routine for [%c] not ready\n",fmts[i]);
		   } 
	    }
     for (totalsize=0, i=0;i<ntotal;i++) totalsize  += (fords[i]*inpsiz[i]); 
	  maxrec = BUFSIZE/totalsize - 1; 

  /* begin reading in the ascii data from stdin */

	*bp = b = inpbuffer;
   for (nread=0, j=0;j<maxrec;j++,nread++) {
      for(i=0;i<ntotal;i++) {
		   for(k=0;k<fords[i];k++) {
			  t = (inpfn[i])(b);  
	        if (t==EOF)  return(nread); 
			  b+=inpsiz[i];
			  }
		   }
      }

   return (nread); /* no of recs read */

} /* inpdata */


#ifdef PROTOTYPE
int32 scanit (char *string,char ***fields,int32 **type,int32 **order)
#else
int32 scanit (string,fields,type,order)
 char *   string;
 char *** fields;
 int32  **  type;
 int32  **  order;
#endif
{
  int32 ns,i;
  int32 p1,p2;
  char ss[300];
  int32 c;

 compact(string,ss);
 ns = HDstrlen(ss); ss[ns++] = ',';

 p1 = p2 = 0;
 for(i=0;i<ns;i++) {
	 c = ss[i];
	 if(c== '=') {
		 p2 = i;
         savfld(ss,(int)p1,(int)(p2-1));
         p1 = p2+1;
		 }
	 else if(c== ',') {
		 p2 = i;
         savtype(ss,(int)p1,(int)(p2-1));
		 p1 = p2+1;
		 }
	 }
   for(i=0;i<ntotal;i++) {
	  fldptr[i] = flds[i];
	  ftyp[i]   = fmts[i];
	  }

   *type   =  ftyp;
   *order  =  fords;
	*fields =  fldptr;
   return (ntotal);

} /* scanit */

#ifdef PROTOTYPE
int32 compact (char *ss, char *dd) 
#else
int32 compact (ss,dd) 
     char *ss, *dd; 
#endif
{
  int i,t,n = HDstrlen(ss);
  for(t=0,i=0;i<n;i++)
    if(ss[i]!=' ') {
        dd[t++] = ss[i];
    }
  dd[t] = '\0';
  return (1);
}

/* ------------------------------------------------------------------ */
#ifdef PROTOTYPE
int32 savfld(char *ss, int p1, int p2)
#else
int32 savfld(ss,p1,p2) 
     char *ss; 
     int p1,p2; 
#endif
{
  int32 t=p2-p1+1;

  HDstrncpy(flds[ntotal],&ss[p1],t);
  flds[ntotal][t] = '\0';
  return (1);
} /* savfld */

#ifdef PROTOTYPE
int32 savtype (char *ss, int p1, int p2) 
#else
int32 savtype (ss,p1,p2) 
     char *ss; 
     int p1,p2; 
#endif
{
  char temp[20];
  int32 t=p2-p1+1;
  HDstrncpy(temp,&ss[p1],p2-p1+1);
  temp[t] = '\0';
  separate(temp,&fmts[ntotal],&fords[ntotal]);
  ntotal++;
  return (1);

  }

#ifdef PROTOTYPE
int32 separate(char *ss, char *fmt, int32 *num) 
#else
int32 separate(ss,fmt,num) 
char *ss; 
char *fmt; 
int32 *num; 
#endif
{
  int32 i,n;
  i=0;
  n=HDstrlen(ss);
  while(i<n) {
    if(ss[i]<'0' ||  ss[i] >'9') break; 
    i++;
  }
  if(i>0) sscanf(ss,"%d",num); else *num= 1;
  *fmt  = ss[i];
  return (1);
}

/* ------------------------------------------------------------------ */

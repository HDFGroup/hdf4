#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.1  1992/02/28 17:00:00  likkai
Initial revision

*/

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
*****************************************************************************
* 
*			  NCSA HDF Vset release 2.1
*					May 1991
* 				Jason Ng May 1991 NCSA
*
* NCSA HDF Vset release 2.1 source code and documentation are in the public
* domain.  Specifically, we give to the public domain all rights for future
* licensing of the source code, all resale rights, and all publishing rights.
* 
* We ask, but do not require, that the following message be included in all
* derived works:
* 
* Portions developed at the National Center for Supercomputing Applications at
* the University of Illinois at Urbana-Champaign.
* 
* THE UNIVERSITY OF ILLINOIS GIVES NO WARRANTY, EXPRESSED OR IMPLIED, FOR THE
* SOFTWARE AND/OR DOCUMENTATION PROVIDED, INCLUDING, WITHOUT LIMITATION,
* WARRANTY OF MERCHANTABILITY AND WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE
* 
******************************************************************************/

#include <vg.h>

#ifdef MAC
 int show_help_msg(void);
 int vgadd(char *, char *);
 int vsadd(char *,char *, char *);
 int vsetlink (char *, int, int*, int);
 int scanit (char*, char***, int**, int**);
 int getdata ( unsigned char**);  
 int compact (char *,char *);
 int savfld (char *,int, int);
 int savtype (char *,int, int);
 int separate(char *ss, char *, int*);

#endif

main(ac,av) int ac; char**av; {

  char  *hfile, *vgname, *vsname, *fmt;

  if (ac <3 ) {
		show_help_msg();
		exit(0);
		}
  else if (ac==3)  {
      if (!strcmp(av[2],"-l")) { show_help_msg() ; exit(0); }
  		hfile  = av[1];
  		vgname = av[2];
 		vgadd(hfile,vgname);
		}

  else if (ac==4)  {
      if (!strcmp(av[2],"-l")) { show_help_msg() ; exit(0); }
  		hfile    = av[1];
  		vsname   = av[2];
  		fmt      = av[3];
 		vsadd(hfile,vsname,fmt);
		}

  else if (!strcmp(av[2],"-l")) {

     	int i, n, vgref,ids[50];
	  	hfile 	= av[1];
		sscanf(av[3],"%d",&vgref);
		for(n=0,i=4;i<ac;i++,n++) { sscanf(av[i],"%d",&ids[n]); }
		vsetlink(hfile,vgref,ids,n);
		}

  else {
		show_help_msg();
		exit(0);
		}

  } /* main */
  
int show_help_msg()  {

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
    printf("    fmt is one of [d,l,f,c] optionally preceded by a decimal.\n");
    printf("\nTo create a vdata, vmake reads ascii data from stdin\n");

    printf("EXAMPLES:\n");
	 printf("\t cat dat.txt | vmake hh.hdf \"triangles\" \"PLIST3=3d\"\n");
	 printf("\t vmake abc.hdf \"xyvals\" \"X=d,Y=f\" < abc.dat\n");
	 printf("\n");

	 return (1);

 } /* show_help_msg */

/* ------------------------------------------------------- */

int vsetlink  (hfile,vgid,ids,n)
char * hfile;
int vgid, n, ids[];
{
  HFILEID f;
  VGROUP * vgmain, *vg;
  VDATA * vs;
  int err=0;
  int i;

	f = Hopen(hfile,DFACC_ALL,0);
	if (f == FAIL) {
	  fprintf(stderr,"cannot open %s.  \n",hfile); exit(0); 
	  }

  vgmain = (VGROUP*) Vattach(f,vgid,"w");
  if(vgmain==NULL) { fprintf(stderr, "0\n"); Hclose(f); exit(-1);}

  for(i=0;i<n;i++) {
	  if     ( -1 != vexistvg(f,ids[i])) {
			if ((vg=(VGROUP*) Vattach(f,ids[i],"r")) != NULL)  {
	         if (Vinsert(vgmain,(VDATA*) vg)  == -1)  { /*  is really VGROUP* */
					err = 1; 
				   fprintf(stderr,"insert a vg (%d)fails!!\n",ids[i]);	
					}
		      Vdetach(vg);
		      }
		 }
     else if ( -1 != vexistvs(f,ids[i])) {
	       if ((vs= (VDATA*) VSattach(f,ids[i],"r")) != NULL) { 
	         if (Vinsert(vgmain,(VDATA*) vs) == -1) { 
					err = 1;
				   fprintf(stderr,"insert a vs (%d)fails!!\n",ids[i]);	
					}
		      VSdetach(vs);
		      }
		 }
     else { fprintf(stderr,"no such vgroup or vdata [%d]\n",ids[i]);  err= 1; }
	  }

  Vdetach(vgmain);
  Hclose(f);

  if (err) exit(-1); 
  else fprintf(stderr,"1\n"); /* success */
  return (1);
} /* vsetlink */

/* ------------------------------------------------------- */
/* 
add a (new) vgroup to the file 
*/

int vgadd (hfile,vgname) 
char * hfile;
char * vgname;
{
  HFILEID f;
  int ref; 
  VGROUP * vg;

  f=Hopen(hfile,DFACC_ALL,0);
  if (f==FAIL) {
	  fprintf(stderr,"cannot open %s. \n",hfile); exit(0); 
	  }

  vg = (VGROUP*) Vattach(f,-1,"w");
  if (vg==NULL) { fprintf(stderr,"cannot attach vg\n"); exit(0); }
  ref = vg->oref;
  Vsetname(vg,vgname);
  Vdetach(vg);

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
vsadd (hfile,vsname,format)
char * hfile;
char * vsname;
char * format;

{
  int stat, i,n, nwritten;
  unsigned char *buf;
  char **fields;
  int *type, *order, nfld;
  char allfields[100];
  HFILEID	f;
  VDATA *vs;
  int ref,ftype;

  nfld = scanit(format,&fields,&type,&order);
  if (nfld < 1) { fprintf(stderr,"bad fields\n"); exit(-1); }

  if ( (f=Hopen(hfile,DFACC_ALL,0))==FAIL) {
	  fprintf(stderr,"cannot open %s.  \n",hfile); exit(-1); 
	  }

  vs = (VDATA*) VSattach(f,-1,"w");
  ref = vs->oref;

printf("vsadd: ref is %d\n",ref);

  allfields[0] = '\0';
  for (i=0;i<nfld;i++) {
	  switch (type[i]) {
		  case 'd': ftype = LOCAL_INTTYPE;   break;
		  case 'f': ftype = LOCAL_FLOATTYPE; break;
		  case 'c': ftype = LOCAL_CHARTYPE;  break;
		  case 'l': ftype = LOCAL_LONGTYPE;  break;
		  default:  fprintf(stderr,"bad type [%c]\n",type[i]); exit(-1); break;
		  }
	  stat = VSfdefine(vs,fields[i],ftype,order[i]);
	  strcat(allfields,fields[i]);
	  strcat(allfields,",");
     }

  i=strlen(allfields); allfields[i-1]='\0'; /* remove last comma */

  VSsetname(vs,vsname);
  stat = VSsetfields(vs,allfields);

  nwritten = 0;
  while( (n = getdata(&buf)) > 0) {
	 /*  printf("getdata rets n=%d .. ",n); */
    stat = VSwrite(vs,buf,n,FULL_INTERLACE);
     printf("+%d",stat); 
	 nwritten +=n;
	 if (stat < 1) fprintf(stderr,"Vswrite stat=%d\n",stat);
	 }
  VSdetach(vs);

  Hclose(f);
  fprintf(stderr,"%d\n",ref,nwritten);
  return (1);

} /* vsadd */

/* ------------------------------------------------------------------ */
/* This part of the code deals with formatting stdin input data.      */
/* ------------------------------------------------------------------ */

#include <stdio.h>


#define MAXVAR 32
static char *fldptr[MAXVAR];
static char flds[MAXVAR][100];
static char fmts[MAXVAR]; 
static int  fords[MAXVAR]; 
static int  ftyp[MAXVAR]; 
static int  ntotal = 0;


/* scanf functions */
static int getint  (x) int  *x; { return(scanf ("%d ",x)); }
static int getfloat(x) float*x; { return(scanf ("%f ",x)); }
static int getbyte (x) char *x; { return(scanf ("%c ",x)); }
static int getlong (x) long *x; { return(scanf ("%ld ",x)); }

/* printf functions */
static int putbyte (x) char *x; { putchar(*x);    return (1);   }
static int putint  (x) int  *x; { printf("%d ",*x); return (1);}
static int putfloat(x) float*x; { printf("%f ",*x); return (1);}
static int putlong (x) long *x; { printf("%ld ",*x); return (1);}


#define BUFSIZE 40000

int getdata (bp) unsigned char**bp; { 
  int totalsize, nread, t,i,j,k;
  unsigned char *b;
  int maxrec;
  int (*getfn[MAXVAR])();
  int (*putfn[MAXVAR])();
  int getsiz[MAXVAR];
  unsigned char getbuffer[BUFSIZE];

     for(i=0;i<ntotal;i++) {
	    switch(fmts[i]) {
		   case 'd':
		     putfn[i] = putint; getfn[i]  = getint; getsiz[i] = sizeof(int);
			  break;
		   case 'f':
			  putfn[i] = putfloat; getfn[i]  = getfloat; getsiz[i] = sizeof(float);
			  break;
		   case 'c':
			  putfn[i]  = putbyte; getfn[i]  = getbyte; getsiz[i] = sizeof(char);
			  break;
		   case 'l':
			  putfn[i]  = putlong; getfn[i]  = getlong; getsiz[i] = sizeof(long);
			  break;
		   } 
	    }
     for (totalsize=0, i=0;i<ntotal;i++) totalsize  += (fords[i]*getsiz[i]); 
	  maxrec = BUFSIZE/totalsize - 1; 

  /* begin reading in the ascii data from stdin */

	*bp = b = getbuffer;
   for (nread=0, j=0;j<maxrec;j++,nread++) {
      for(i=0;i<ntotal;i++) {
		   for(k=0;k<fords[i];k++) {
			  t = (getfn[i])(b);  
	        if (t==EOF)  return(nread); 
			  b+=getsiz[i];
			  }
		   }
      }

   return (nread); /* no of recs read */

} /* getdata */

 int scanit (string,fields,type,order)
 char *   string;
 char *** fields;
 int  **  type;
 int  **  order;
 {
  int ns,i;
  int p1,p2;
  char ss[300];
  int c;

 
 compact(string,ss);
 ns = strlen(ss); ss[ns++] = ','; 

 p1 = p2 = 0;
 for(i=0;i<ns;i++) {
	 c = ss[i];
	 if(c== '=') {
		 p2 = i;
		 savfld(ss,p1,p2-1);
		 p1 = p2+1;
		 }
	 else if(c== ',') {
		 p2 = i;
		 savtype(ss,p1,p2-1);
		 p1 = p2+1;
		 }
	 }
   for(i=0;i<ntotal;i++) {
	  fldptr[i] = flds[i];
	  switch(fmts[i]) {
		  case 'd':
				ftyp[i]   = 'd';
				break;
		  case 'f':
				ftyp[i]   = 'f';
				break;
		  case 'c':
				ftyp[i]   = 'c';
				break;
		  case 'l':
				ftyp[i]   = 'l';
				break;
		  } 
	  }

   *type   =  ftyp;
   *order  =  fords;
	*fields =  fldptr;
   return (ntotal);

 } /* scanit */

 int compact (ss,dd) char *ss, *dd; {
	int i,t,n = strlen(ss);
	for(t=0,i=0;i<n;i++) if(ss[i]!=' ') { dd[t++] = ss[i]; }
	dd[t] = '\0';
	return (1);
	}

/* ------------------------------------------------------------------ */

int savfld (ss,p1,p2) char *ss; int p1,p2; {
  int t=p2-p1+1;
  strncpy(flds[ntotal],&ss[p1],t);
  flds[ntotal][t] = '\0';
  return (1);

 } /* savfld */

int savtype (ss,p1,p2) char *ss; int p1,p2; {
  char temp[20];
  int t=p2-p1+1;
  strncpy(temp,&ss[p1],p2-p1+1); temp[t] = '\0';
  separate(temp,&fmts[ntotal],&fords[ntotal]);
  ntotal++;
  return (1);

  }

int separate(ss,fmt,num) char *ss; char *fmt; int*num; {
	int i,n;
	i=0;
	n=strlen(ss);
	while(i<n) {
	  if(ss[i]<'0' ||  ss[i] >'9') break; 
	  i++;
	  }
	if(i>0) sscanf(ss,"%d",num); else *num= 1;
	*fmt  = ss[i];
	return (1);
  }

/* ------------------------------------------------------------------ */

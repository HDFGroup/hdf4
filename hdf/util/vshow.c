/*****************************************************************************
*
* vshow.c
*
*	HDF Vset utility.
*
*	vshow: 	dumps out vsets in a hdf file.
*
*	Usage:  vshow [file] {+{n}}
*		the '+' option indicates a full dump on all vdatas.
*		'+n' means full dump only for the nth vdata.
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

#include "vg.h"

static int condensed;

#ifdef MAC
int vsdumpfull(VDATA *);
#endif

int32 vsdumpfull
  PROTO((VDATA * vs)); 

main(ac,av) int ac; 
char**av;
{
  VGROUP	   *vg, *vgt;
  VDATA  		*vs;
  HFILEID		f;
  int32 vgid = -1;
  int32 vsid = -1;
  int32 vsno = 0;
  int32	vstag;
  
  int32 i, t, nvg, n, ne, nv, interlace, vsize;
  int32 * lonevs; /* array to store refs of all lone vdatas */
  int32 nlone; /* total number of lone vdatas */
  
  char fields[50], vgname[50],vsname[50];
  char  vgclass[50],vsclass[50], *name;
  int32 fulldump = 0, start = 1;
  
  if (ac == 3) if(av[2][0]=='-'||av[2][0]=='+') {
    sscanf(&(av[2][1]),"%d",&vsno);
    if(vsno == 0) {
      printf("FULL DUMP\n"); 
    } else { 
      printf("FULL DUMP on vs#%d\n",vsno); 
    }
    fulldump = 1;
    if(av[2][0]=='+') condensed = 1; 
    else condensed = 0;
    start = 2;
  }

  if(ac < 2) {
    printf("%s: dumps HDF vsets info from hdf file\n",av[0]);
    printf("usage: %s file {+n} \n", av[0]);
    printf("\t +  gives full dump of all vdatas.\n"); 
    printf("\t +n gives full dump of vdata with id n.\n"); 
    exit(0);
  }

  if((f=Hopen(av[1],DFACC_READ,0))==FAIL) exit(0);
  printf("\nFILE: %s\n",av[1]);
  
  nvg=0;
  while( (vgid = Vgetid(f,vgid)) != -1) {
    vg = (VGROUP*) Vattach(f,vgid,"r");
    if(vg==NULL) {
      printf("cannot open vg id=%d\n",vgid);
    }
    Vinquire(vg,&n, vgname);
    Vgetclass(vg, vgclass); 
    if (DFIstrlen(vgname)==0)  strcat(vgname,"NoName");
    printf("\nvg:%d <%d/%d> (%s {%s}) has %d entries:\n",
           nvg, vg->otag, vg->oref, vgname, vgclass,n);
    
    for (t=0; t< Vntagrefs(vg); t++) {
      Vgettagref(vg, t, &vstag, &vsid);
      
      /* ------ V D A T A ---------- */
      if (vstag==VSDESCTAG)  {  
        vs = (VDATA *) VSattach(f,vsid,"r");

        if(vs==NULL) {
          printf("cannot open vs id=%d\n",vsid);
          continue;
        }

        VSinquire(vs, &nv,&interlace, fields, &vsize, vsname);
        if (DFIstrlen(vsname)==0)  strcat(vsname,"NoName");
        VSgetclass(vs,vsclass); 
        printf("  vs:%d <%d/%d> nv=%d i=%d fld [%s] vsize=%d (%s {%s})\n",
                t, vs->otag, vs->oref, nv, interlace, fields, vsize, vsname, vsclass);
        
        if(fulldump && vsno==0) vsdumpfull(vs);
        else if(fulldump && vsno==vs->oref) vsdumpfull(vs);
        
        VSdetach(vs);
      }
      else 
        if (vstag==VGDESCTAG)  {  
          /* ------ V G R O U P ----- */
          vgt = (VGROUP*) Vattach(f,vsid,"r");
          
          if(vgt==NULL) {
            printf("cannot open vg id=%d\n",vsid);
            continue;
          }
          
          Vinquire(vgt, &ne, vgname);
          if (DFIstrlen(vgname)==0)  strcat(vgname,"NoName");
          Vgetclass(vgt, vgclass);
          printf("  vg:%d <%d/%d> ne=%d (%s {%s})\n",
                 t, vgt->otag, vgt->oref, ne,  vgname, vgclass );
          Vdetach(vgt);
        }
        else { 
          name = HDgettagname(vstag);
          if(!name) name = "Unknown Tag";
          printf("  --:%d <%d/%d> %s\n", t, vstag, vsid, name);
        }
    } /* while */
    
    Vdetach(vg);
    nvg++;

  } /* while */
  
  if(nvg==0) {
    printf("No vgroups in this file\n");
  }
  
  nlone = VSlone(f,NULL,0);
  if(nlone > 0) {

    printf("Lone vdatas:\n");
    if (NULL == (lonevs = (int*) VGETSPACE (sizeof(int)*nlone))) {
      printf("%s: File has %d lone vdatas but ",av[0],nlone ); 
      printf("cannot alloc lonevs space. Quit.\n"); 
      exit(0);
    }

    VSlone(f,lonevs, nlone);
    for(i=0; i<nlone;i++) {
      vsid = lonevs[i];
      if( NULL == ( vs = (VDATA *) VSattach(f,lonevs[i],"r"))) {
        printf("cannot open vs id=%d\n",vsid);
        continue;
      }
      VSinquire (vs, &nv,&interlace, fields, &vsize, vsname);
      if (DFIstrlen(vsname)==0)  strcat(vsname,"NoName");
      VSgetclass (vs, vsclass);
      printf("L vs:%d <%d/%d> nv=%d i=%d fld [%s] vsize=%d (%s.%s)\n",
              vsid, vs->otag, vs->oref, nv, interlace, fields, vsize, vsname, vsclass);
      if (fulldump && vsno==0) vsdumpfull(vs);
      else if (fulldump && vsno==vs->oref) vsdumpfull(vs);
      VSdetach(vs);
    }
    VFREESPACE (lonevs);
  }

  Hclose(f);
  
} /* main */


/* ------------------------------------------------ */
/* printing functions used by vsdumpfull(). */
static int32 cn = 0;
int32 fmtbyte(x) unsigned char*x;  { cn += printf("%02x ",*x); return(1);  }

int32 fmtchar(x) char*x; 
{ 
  cn++; putchar(*x); return(1);
}

int32 fmtint(x) 
     char* x;
{	
  int aint;
  memcpy((unsigned char *)&aint, x, sizeof(int)); 
  cn += printf("%d",aint); 
  return(1);  
}

int32 fmtfloat(x) 
     char* x;
{
  float afloat;
  memcpy((unsigned char *) &afloat, x, sizeof(float)); 
  cn += printf("%f",afloat); 
  return(1);  
}

int32 fmtlong(x) 
     char* x;   
{	
  long along;
  memcpy((unsigned char *) &along, x, sizeof(long)); 
  cn += printf("%ld",along); 
  return(1);  
}

int32 fmtshort(x) 
     char* x;   
{	
  short ashort;
  memcpy((unsigned char *) &ashort, x, sizeof(short)); 
  cn += printf("%d",ashort); 
  return(1);  
}

int32 fmtdouble(x) char*x;
{	
  double adouble;
  memcpy((unsigned char *) &adouble, x, sizeof(double)); 
  cn += printf("%f",adouble); 
  return(1);  
}

/* ------------------------------------------------ */

int32 vsdumpfull(vs) VDATA * vs; 
{
  char vsname[100], fields[100];
  int32 j,i,t,interlace, nv, vsize;
  BYTE *bb, *b;
  VWRITELIST* w;
  int32 (*fmtfn[60])();
  int32 off[60];
  int32 order[60];
  
  int32 nf;
  
  VSinquire(vs, &nv,&interlace, fields, &vsize, vsname);
  bb = (char*) VGETSPACE (nv*vsize);
  if(bb==NULL) { 
    printf("vsdumpfull malloc error\n");
    return(0); 
  }

  VSsetfields(vs,fields);
  VSread(vs,bb,nv,interlace);
  
  w = &(vs->wlist);
  
  nf = w->n;
  for (i=0;i<w->n;i++) {
    printf("%d: fld [%s], type=%d, order=%d\n", i, w->name[i], w->type[i], w->order[i]);

    order[i] = w->order[i];
    off[i]   = DFKNTsize(w->type[i]);
    
    switch(w->type[i]) {
      
    case DFNT_CHAR:
      fmtfn[i] = fmtchar;
      break;
      
    case DFNT_UINT8:
    case DFNT_INT8:
      fmtfn[i] = fmtbyte;
      break;
      
    case DFNT_UINT16:
    case DFNT_INT16:
      fmtfn[i] = fmtshort;
      break;
      
    case DFNT_UINT32:
    case DFNT_INT32:
      fmtfn[i] = fmtlong;
      break;
      
    case DFNT_FLOAT32:
      fmtfn[i] = fmtfloat;
      break;
      
    case DFNT_FLOAT64:
      fmtfn[i] = fmtdouble;
      break;
      
    default: 
      fprintf(stderr,"sorry, type [%s] not supported\n", w->type[i]); 
      break;

    }
  }
  
  b= bb;
  cn=0;
  
  for(j=0;j<nv;j++) {
    for(i=0;i<nf;i++) {
      for(t=0;t<order[i];t++) { (fmtfn[i]) (b); b+=off[i]; }
      putchar(' ');
    }
    if (condensed) { if( cn > 70) { putchar('\n'); cn=0; } } 
    else putchar('\n');
  }
  
  /* ============================================ */
  
  VFREESPACE (bb);
  printf("\n\n");
  
  return(1);
  
} /* vsdumpfull */
/* ------------------------------------- */

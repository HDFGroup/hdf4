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
******************************************************************************/

#include "vg.h"

static int condensed;

int32 vsdumpfull
  PROTO((int32 vs)); 

main(ac,av) int ac; 
char**av;
{
  int32	vg, vgt;
  int32 vgotag,vgoref;
  int32 vs;
  int32 vsotag,vsoref;
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
  Vstart(f);
  printf("\nFILE: %s\n",av[1]);
  
  nvg=0;
  while( (vgid = Vgetid(f,vgid)) != -1) {
    vg = Vattach(f,vgid,"r");
    if(vg==NULL) {
      printf("cannot open vg id=%d\n",vgid);
    }
    Vinquire(vg,&n, vgname);
    Vgetotag(vg,&vgotag);
    Vgetoref(vg,&vgoref);
    Vgetclass(vg, vgclass); 
    if (HDstrlen(vgname)==0)  HDstrcat(vgname,"NoName");
    printf("\nvg:%d <%d/%d> (%s {%s}) has %d entries:\n",
           nvg, vgotag, vgoref, vgname, vgclass,n);
    
    for (t=0; t< Vntagrefs(vg); t++) {
      Vgettagref(vg, t, &vstag, &vsid);
      
      /* ------ V D A T A ---------- */
      if (vstag==VSDESCTAG)  {  
        vs = VSattach(f,vsid,"r");

        if(vs==NULL) {
          printf("cannot open vs id=%d\n",vsid);
          continue;
        }

        VSinquire(vs, &nv,&interlace, fields, &vsize, vsname);
	VSgetotag(vs, &vsotag);
	VSgetoref(vs, &vsoref);
        if (HDstrlen(vsname)==0)  HDstrcat(vsname,"NoName");
        VSgetclass(vs,vsclass); 
        printf("  vs:%d <%d/%d> nv=%d i=%d fld [%s] vsize=%d (%s {%s})\n",
                t, vsotag, vsoref, nv, interlace, fields, vsize, vsname, vsclass);
        
        if(fulldump && vsno==0) vsdumpfull(vs);
        else if(fulldump && vsno==vsoref) vsdumpfull(vs);
        
        VSdetach(vs);
      }
      else 
        if (vstag==VGDESCTAG)  {  
          /* ------ V G R O U P ----- */
          vgt = Vattach(f,vsid,"r");
          
          if(vgt==NULL) {
            printf("cannot open vg id=%d\n",vsid);
            continue;
          }
          
          Vinquire(vgt, &ne, vgname);
          if (HDstrlen(vgname)==0)  HDstrcat(vgname,"NoName");
	  Vgetotag(vgt,&vgotag);
	  Vgetoref(vgt,&vgoref);
          Vgetclass(vgt, vgclass);
          printf("  vg:%d <%d/%d> ne=%d (%s {%s})\n",
                 t, vgotag, vgoref, ne,  vgname, vgclass );
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
    if (NULL == (lonevs = (int32 *) HDgetspace(sizeof(int)*nlone))) {
      printf("%s: File has %d lone vdatas but ",av[0],nlone ); 
      printf("cannot alloc lonevs space. Quit.\n"); 
      exit(0);
    }

    VSlone(f,lonevs, nlone);
    for(i=0; i<nlone;i++) {
      vsid = lonevs[i];
      if( FAIL == ( vs = VSattach(f,lonevs[i],"r"))) {
        printf("cannot open vs id=%d\n",vsid);
        continue;
      }
      VSinquire (vs, &nv,&interlace, fields, &vsize, vsname);
      if (HDstrlen(vsname)==0)  HDstrcat(vsname,"NoName");
      VSgetotag(vs, &vsotag);
      VSgetoref(vs, &vsoref);
      VSgetclass (vs, vsclass);
      printf("L vs:%d <%d/%d> nv=%d i=%d fld [%s] vsize=%d (%s {%s})\n",
              vsid, vsotag, vsoref, nv, interlace, fields, vsize, vsname, vsclass);
      if (fulldump && vsno==0) vsdumpfull(vs);
      else if (fulldump && vsno==vsoref) vsdumpfull(vs);
      VSdetach(vs);
    }
    HDfreespace(lonevs);
  }

  Vend(f);
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
  int *i;
  i = (int *) x;
  cn += printf("%d", *i); 
  return(1);  
}

int32 fmtfloat(x) 
     char* x;
{
  float *f;
  f = (float *) x;
  cn += printf("%f", *f); 
  return(1);  
}

int32 fmtlong(x) 
     char* x;   
{	
  int32 *l;
  l = (int32 *) x;
  cn += printf("%d", *l);
  return(1);  
}

int32 fmtshort(x) 
     char* x;   
{	
  int16 *s;
  s = (int16 *) x;
  cn += printf("%d", *s);
  return(1);  
}

int32 fmtdouble(x) char*x;
{	
  double *d;
  d = (double *) x;
  cn += printf("%f", *d); 
  return(1);  
}

/* ------------------------------------------------ */

int32 vsdumpfull(vs) int32 vs; 
{
  char vsname[100], fields[100];
  int32 j,i,t,interlace, nv, vsize;
  uint8 *bb, *b;
  VWRITELIST* w;
  int32 (*fmtfn[60])();
  int32 off[60];
  int32 order[60];
  
  int32 nf;
  
  VSinquire(vs, &nv,&interlace, fields, &vsize, vsname);
  bb = (uint8 *) HDgetspace(nv*vsize);
  if(bb==NULL) { 
    printf("vsdumpfull malloc error\n");
    return(0); 
  }

  VSsetfields(vs,fields);
  VSread(vs,bb,nv,interlace);
  
  w = vswritelist(vs);
  
  nf = w->n;
  for (i=0; i < w->n; i++) {
    printf("%d: fld [%s], type=%d, order=%d\n", i, w->name[i], w->type[i], w->order[i]);

    order[i] = w->order[i];
    off[i]   = DFKNTsize(w->type[i] | DFNT_NATIVE);
    
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
  
  for(j = 0; j < nv; j++) {
    for(i = 0; i < nf; i++) {
      for(t=0; t < order[i]; t++) { 
        (fmtfn[i]) (b); 
        b += off[i]; 
        putchar(' ');
        cn++;
      }
      putchar(' ');
      cn++;
    }
    
    if (condensed) { 
      if(cn > 65) { 
        putchar('\n'); 
        cn=0; 
      } 
    } 
    else putchar('\n');
  }
  
  /* ============================================ */
  
  HDfreespace(bb);
  printf("\n\n");
  
  return(1);
  
} /* vsdumpfull */
/* ------------------------------------- */

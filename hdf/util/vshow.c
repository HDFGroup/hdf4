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

#include <vg.h>

static int condensed;
static char sbuf[80]; /* message buffer */
#define PMSG     fprintf(stderr,"%s",sbuf)

#ifdef MAC
int vsdumpfull(VDATA *);
#endif

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
	char  vgclass[50],vsclass[50];
	int32 fulldump = 0;

	if (ac==3) if(av[2][0]=='-'||av[2][0]=='+') {
		sscanf(&(av[2][1]),"%d",&vsno);
		if(vsno == 0) {
			sprintf(sbuf,"FULL DUMP\n"); 
			PMSG; 
		}
		else { 
			sprintf(sbuf,"FULL DUMP on vs#%d\n",vsno); 
			PMSG; 
		}
		fulldump = 1;
		if(av[2][0]=='+') condensed = 1; 
		else condensed = 0;
	}
	if(ac<2) {
		sprintf(sbuf,"%s: dumps HDF vsets info from hdf file\n",av[0]);
		PMSG;
		sprintf(sbuf,"usage: %s file {+n} \n", av[0]);
		PMSG;
		sprintf(sbuf,"\t +  gives full dump of all vdatas.\n"); 
		PMSG;
		sprintf(sbuf,"\t +n gives full dump of vdata with id n.\n"); 
		PMSG;
		exit(0);
	}
	if((f=Hopen(av[1],DFACC_READ,0))==FAIL) exit(0);
	sprintf(sbuf,"\nFILE: %s\n",av[1]);
	PMSG;

	nvg=0;
	while( (vgid = Vgetid(f,vgid)) != -1) {
		vg = (VGROUP*) Vattach(f,vgid,"r");
		if(vg==NULL) {
			sprintf(sbuf,"cannot open vg id=%d\n",vgid);
			PMSG;
		}
		Vinquire(vg,&n, vgname);
		Vgetclass(vg, vgclass); 
		if (strlen(vgname)==0)  strcat(vgname,"NoName");
		sprintf(sbuf,"\nvg:%d <%d/%d> (%s.%s) has %d entries:\n",
		    nvg, vg->otag, vg->oref, vgname, vgclass,n);
		PMSG;

		for (t=0; t< Vntagrefs(vg); t++) {
			Vgettagref(vg, t, &vstag, &vsid);
			
			if (vstag==VSDESCTAG)  {  /* ------ V D A T A ---------- */
				vs = (VDATA *) VSattach(f,vsid,"r");
				if(vs==NULL) {
					sprintf(sbuf,"cannot open vs id=%d\n",vsid);
					PMSG;
				}
				VSinquire(vs, &nv,&interlace, fields, &vsize, vsname);
				if (strlen(vsname)==0)  strcat(vsname,"NoName");
				VSgetclass(vs,vsclass); 
				sprintf(sbuf,"  vs:%d <%d/%d> nv=%d i=%d fld [%s] vsize=%d (%s.%s)\n",
				    t, vs->otag, vs->oref, nv, interlace, 
					fields, vsize, vsname, vsclass);
				PMSG;

				if(fulldump && vsno==0) vsdumpfull(vs);
				else if(fulldump && vsno==vs->oref) vsdumpfull(vs);

				VSdetach(vs);
			}
			else if (vstag==VGDESCTAG)  {  /* ------ V G R O U P ----- */
				vgt = (VGROUP*) Vattach(f,vsid,"r");
				if(vgt==NULL) {
					sprintf(sbuf,"cannot open vg id=%d\n",vsid);
					PMSG;
				}
				Vinquire(vgt, &ne, vgname);
				if (strlen(vgname)==0)  strcat(vgname,"NoName");
				Vgetclass(vgt, vgclass);
				sprintf(sbuf,"  vg:%d <%d/%d> ne=%d (%s.%s)\n",
				    t, vgt->otag, vgt->oref, ne,  vgname, vgclass );
				PMSG;
				Vdetach(vgt);
			}
			else {
				sprintf(sbuf,"  --:%d <%d/%d>  NON-VSET element\n", t,vstag,vsid);
				PMSG;
			}
		} /* while */
		Vdetach(vg);
		nvg++;
	} /* while */

	if(nvg==0) {
		sprintf(sbuf,"No vgroups in this file\n");
		PMSG;
		}

	nlone = VSlone(f,NULL,0);
	if(nlone > 0) {
		sprintf(sbuf,"Lone vdatas:\n"); PMSG;
		if (NULL == (lonevs = (int*) VGETSPACE (sizeof(int)*nlone))) {
			sprintf(sbuf,"%s: File has %d lone vdatas but ",av[0],nlone ); PMSG;
   		sprintf(sbuf,"cannot alloc lonevs space. Quit.\n"); PMSG; 
     		 exit(0);
  	 	 	}
		VSlone(f,lonevs, nlone);
		for(i=0; i<nlone;i++) {
			vsid = lonevs[i];
			if( NULL == ( vs = (VDATA *) VSattach(f,lonevs[i],"r"))) {
				sprintf(sbuf,"cannot open vs id=%d\n",vsid);
				PMSG;
				}
			VSinquire (vs, &nv,&interlace, fields, &vsize, vsname);
			if (strlen(vsname)==0)  strcat(vsname,"NoName");
			VSgetclass (vs, vsclass);
			sprintf(sbuf,"L vs:%d <%d/%d> nv=%d i=%d fld [%s] vsize=%d (%s.%s)\n",
					     vsid, vs->otag, vs->oref, nv, interlace, fields,
							 vsize, vsname, vsclass);
			PMSG;
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
int32 fmtchar(x) char*x;   	{ cn++; putchar(*x); return(1); }
int32 fmtint(x) int*x;   		{ cn += printf("%d ",*x); return(1);  }
int32 fmtfloat(x) float*x; 	{ cn += printf("%f ",*x); return(1);  }
int32 fmtlong(x) int*x;   	{ cn += printf("%ld ",*x); return(1);  }
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
		sprintf(sbuf,"vsdumpfull malloc error\n");
		PMSG; 
		return(0); 
	}

	VSsetfields(vs,fields);
	VSread(vs,bb,nv,interlace);

	w = &(vs->wlist);

	nf = w->n;
	for (i=0;i<w->n;i++) {
		printf("%d: fld [%s], type=%d, order=%d\n",
		    i, w->name[i], w->type[i], w->order[i]);
		switch(w->type[i]) {

			case LOCAL_FLOATTYPE:
				order[i] = w->order[i];
				off[i]   = sizeof(float);
				fmtfn[i] = fmtfloat;
				break;

			case LOCAL_INTTYPE:
				order[i] = w->order[i];
				off[i]   = sizeof(int);
				fmtfn[i] = fmtint;
				break;

			case LOCAL_BYTETYPE:
				order[i] = w->order[i];
				off[i]   = sizeof(unsigned char);
				fmtfn[i] = fmtbyte;
				break;

			case LOCAL_CHARTYPE:
				order[i] = w->order[i];
				off[i]   = sizeof(char);
				fmtfn[i] = fmtchar;
				break;

			case LOCAL_LONGTYPE:
				order[i] = w->order[i];
				off[i]   = sizeof(long);
				fmtfn[i] = fmtlong;
				break;
		}
	}

	b= bb;
	cn=0;

	for(j=0;j<nv;j++) {
		for(i=0;i<nf;i++) {
			for(t=0;t<order[i];t++) { (fmtfn[i]) (b); b+=off[i]; }
		}
		if (condensed) { if( cn > 70) { putchar('\n'); cn=0; } } 
		else putchar('\n');
	}

	/* ============================================ */

	VFREESPACE (bb);
	putchar('\n'); 
	putchar('\n');

	return(1);

} /* vsdumpfull */
/* ------------------------------------- */

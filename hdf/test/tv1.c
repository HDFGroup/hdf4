#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.5  1993/01/19 05:59:23  koziol
Merged Hyperslab and JPEG routines with beginning of DEC ALPHA
port.  Lots of minor annoyances fixed.

 * Revision 1.4  1992/11/30  22:01:15  chouck
 * Added fix for Vstart() and Vend()
 *
 * Revision 1.3  1992/05/27  21:51:19  chouck
 * Added a few casts to VSwrite() calls
 *
 * Revision 1.2  1992/05/18  22:11:07  sxu
 * modified constants for number types
 *
 * Revision 1.1  1992/03/01  22:29:07  dilg
 * Initial revision
 *
 * Revision 1.1  1992/02/29  19:55:07  likkai
 * Initial revision
 *
*/
/*
	=================================
	HDF VSET TEST PROGRAM
	Jason Ng Feb-28-92 NCSA
	=================================
*/

#include "vg.h"

#define FS "tv1.hdf"
     main(ac,av) 
int ac;
char**av;
{
    int num;
    
    if(ac!=2) num = 5;
    else sscanf(av[1],"%d",&num);
    
    printf("%s: tests the basic vgroup and vdata creation routines\n", av[0]);
    printf("creates %d vgroups and %d vdatas in %s\n",num,num,FS);
    createm(FS,num);
    printf("success: file %s created.\n", FS);
    printf("use the utility vshow to examine this file:\n");
    printf("\t\tvshow %s +\n",FS);
    printf("results should be as in %s.result\n",av[0]);
}

createm(fs,n) char*fs;
     int n;
{
    VGROUP * vg;
    VDATA * vs;
    HFILEID f;
    char ss[30];
    int i,ne=0;
    float buf[100];
    
    if( FAIL==(f=Hopen(fs,DFACC_ALL,0))) {
        printf("open err %s\n",fs);
        exit(0);
    }
 
    Vstart(f);

    for(i=0;i<n;i++) {
        vg = (VGROUP*) Vattach(f,-1,"w");
        sprintf(ss,"test_vgroup#%d",i);
        Vsetname(vg,ss);
        Vdetach(vg);
        printf("created VGROUP %s \n",ss);
    }
    
    for(i=0;i<n;i++) {
        ne +=5;
        vs = (VDATA*) VSattach(f,-1,"w");
        sprintf(ss,"vdata#%d",i);
        VSsetname(vs,ss);
        VSfdefine(vs,"PRESS",DFNT_FLOAT32,1);
        VSsetfields(vs,"PRESS");
        makefloatdata(buf,ne);
        VSwrite(vs, (unsigned char *) buf, ne, FULL_INTERLACE);
        VSdetach(vs);
        printf("created VDATA %s with %d elements\n",ss,ne);
    }

    Vend(f);

    Hclose(f);
}

makefloatdata(xx,n) 
     int n; 
     float xx[];
{
    int i;
    for(i=0;i<n;i++) xx[i] = n*100.0 + i*i;
}


#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.4  1992/11/19 17:15:13  chouck
HIlock() and HIunlock() are now static

 * Revision 1.3  1992/07/09  16:08:00  chouck
 * Minor fix to a few routine names for VMS
 *
 * Revision 1.2  1992/05/29  19:23:00  chouck
 * Modified to reflect current names
 *
 * Revision 1.1  1992/02/10  20:59:34  chouck
 * Initial revision
 *
*/

/*
  As the VMS linker ignores case, some function names have
  to be redefined.  BUT don't just include hdf.h as then
  we will get all the prototypes too.
*/
#ifdef VMS
#include "dfivms.h"
#endif

main()
{
    puts("This program is for checking the linkage of functions.");
    puts("Should not be run.");
    exit(1);

    HEstring();
    HEpush();
    HEprint();
    HEvalue();

    Hopen();
    Hclose();
    Hstartread();
    Hnextread();
    Hinquire();
    Hstartwrite();
    Hseek();
    Hread();
    Hwrite();
    Hendaccess();
    Hgetelement();
    Hputelement();
    Hlength();
    Hoffset();
    Hdupdd();
    HIupdate_dd();
    Hdeldd();
    Hnewref();
    Hishdf();
    Hsync();
    HIgetspinfo();
/*
    HIlock();
    HIunlock();
*/
    HDvalidfid();
    HDerr();
    Hnumber();

    HLcreate();
    HXcreate();

    DFPgetpal();
    DFPputpal();
    DFPaddpal();
    DFPnpals();
    DFPreadref();
    DFPwriteref();
    DFPrestart();
    DFPlastref();

    DFR8getdims();
    DFR8getimage();
    DFR8setpalette();
    DFR8putimage();
    DFR8addimage();
    DFR8nimages();
    DFR8readref();
    DFR8writeref();
    DFR8restart();
    DFR8lastref();

    DF24getdims();
    DF24reqil();
    DF24getimage();
    DF24setdims();
    DF24setil();
    DF24restart();
    DF24addimage();
    DF24putimage();
    DF24readref();
}


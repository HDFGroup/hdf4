#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.1  1992/02/10 20:59:34  chouck
Initial revision

*/
main()
{
    puts("This program is for checking the linkage of functions.");
    puts("Should not be run.");
    exit(1);

    HEstring();
    HEclear();
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
#if 0
    Hishdf();
#endif
    Hsync();
    HIgetspinfo();
    HIlock();
    HIunlock();
    HIvalidfid();
    HIerr();
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


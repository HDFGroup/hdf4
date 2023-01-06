/*********************************************************************
 *   Copyright 1993, UCAR/Unidata
 *   See netcdf/COPYRIGHT file for copying and redistribution conditions.
 *********************************************************************/

union generic {			/* used to hold any kind of fill_value */
    double doublev;
    float floatv;
    nclong longv;
    short shortv;
    char charv;
};

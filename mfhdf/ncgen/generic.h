/*********************************************************************
 *   Copyright 1993, UCAR/Unidata
 *   See netcdf/COPYRIGHT file for copying and redistribution conditions.
 *********************************************************************/

#ifndef H4NC_GENERIC_H
#define H4NC_GENERIC_H

union generic { /* used to hold any kind of fill_value */
    double doublev;
    float  floatv;
    nclong longv;
    short  shortv;
    char   charv;
};

#endif /* H4NC_GENERIC_H */

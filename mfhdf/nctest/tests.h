/*********************************************************************
 *   Copyright 1993, UCAR/Unidata
 *   See netcdf/COPYRIGHT file for copying and redistribution conditions.
 *********************************************************************/

#ifndef NCTEST_TESTS_H
#define NCTEST_TESTS_H

#ifdef __cplusplus
extern "C" {
#endif

extern void test_nccreate(char *);
extern void test_ncopen(char *);
extern void test_ncredef(char *);
extern void test_ncendef(char *);
extern void test_ncclose(char *);
extern void test_ncinquire(char *);
extern void test_ncsync(char *);
extern void test_ncabort(char *);
extern void test_ncdimdef(char *);
extern void test_ncdimid(char *);
extern void test_ncdiminq(char *);
extern void test_ncdimrename(char *);
extern void test_ncvardef(char *);
extern void test_ncvarid(char *);
extern void test_ncvarinq(char *);
extern void test_ncvarput1(char *);
extern void test_ncvarget1(char *);
extern void test_ncvarput(char *);
extern void test_ncvarget(char *);
extern void test_ncvarputg(char *);
extern void test_ncvargetg(char *);
extern void test_ncvarget_unlim(char *);
extern void test_ncrecinq(char *);
extern void test_ncrecput(char *);
extern void test_ncrecget(char *);
extern void test_ncvarrename(char *);
extern void test_ncattput(char *);
extern void test_ncattinq(char *);
extern void test_ncattget(char *);
extern void test_ncattcopy(char *, char *);
extern void test_ncattname(char *);
extern void test_ncattrename(char *);
extern void test_ncattdel(char *);
extern void test_nctypelen(void);
extern int  test_varputget(int);
extern int  test_varputgetg(int);
extern int  test_slabs(int);

#ifdef __cplusplus
}
#endif

#endif /* NCTEST_TESTS_H */

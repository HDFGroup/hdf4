/*
 *     Copyright 1988, University Corporation for Atmospheric Research
 *        Not for Resale. All copies to include this notice.
 */

#include "h4config.h"

/*
 * This program tests only the xdr library functionality required by netCDF.
 *  'xdr_vector' is not used by the netCDF, it is used here for convenience.
 */

#ifdef H4_HAVE_NETINET_IN_H
#  include <netinet/in.h>     /* for htonl() */
#else
#  ifdef H4_HAVE_WINSOCK2_H
#    include <winsock2.h>
#  endif
#endif

# define xdr_assert(ex)    {if (!(ex)){(void)fprintf(stderr,"Assertion failed: file \"%s\", line %d\n", __FILE__, __LINE__);}}

#ifndef    H4_NO_SYS_XDR_INC
#include <types.h>
#include <xdr.h>
#else
#include "types.h"
#include "xdr.h"
#endif

#define TESTFILE "test.xdr"
/* if this is NOT defined, then the program just reads the file */
#define CREATE

#ifdef __FreeBSD__
#define EPSILON .005
#else /* __FreeBSD__ */
#define EPSILON .0005
#endif /* __FreeBSD__ */

int main(ac,av)
int ac ;
char *av[] ;
{
    int ii ;
    const char *fname ;
    FILE *F ;
    XDR xdrs[1] ;
    size_t count ;
    u_int szof ;

    /* some random numbers, divisible by 4 and less than 32k */
    static u_int seeks[] =
    {
        16828 ,
        8448 ,
        20632 ,
        8124 ,
        16764 ,
        17232 ,
        3476 ,
        28168 , /* +(5 * sizeof(double) + (5 * sizeof(enum) */
        /* */
        28108 ,
        16796 ,
        10968 ,
        24104 ,
        30560 ,
        16880 ,
        17260 ,
        12556 ,
    } ;

    u_int poses[9] ;
    int jj = 0 ;

    static char text[32] ={ "Hiya sailor. New in town?" };
    char got_s[32] ;

    static unsigned char bytes[8] = { 254, 255, 0, 1, 2} ;
    unsigned char *bp , got_ab[8] ;

    static int ints[5] = { 5, 6, 7, 8, 9} ;
    int *ip , got_ip[5] ;

    static unsigned int u_ints[5] = {
        ((unsigned)65535),
        ((unsigned)65534),
        ((unsigned)0),
        ((unsigned)1),
        ((unsigned)2)
    } ;
    unsigned int *uip , got_uip[5] ;

    long lnum = 82555 ;

    static long longs[5] = { -4, -3, -2, -1, 0} ;
    long *lp , got_al[5] ;

    static unsigned long u_longs[5] = {
        ((unsigned)65535),
        ((unsigned)65534),
        ((unsigned)0),
        ((unsigned)1),
        ((unsigned)2)
    } ;
    unsigned long *ulp , got_aul[5] ;

    static float floats[5] = { 100.125, 100.25, 100.375, 100.5, 100.625 } ;
    float *fp , got_af[5] ;

    /* These all require 25 bits: 2^(25-i)+1/2^i, i = 2, 3, ..., 6 */
    static double doubles[5] = { 8388608.25, 4194304.125, 2097152.0625,
                    1048576.03125, 524288.015625 } ;

    double *dp , got_ad[5] ;

    typedef enum {ZERO_E, ONE_E, TWO_E, THREE_E, FOUR_E} encount ;
    static encount encounts[5] = {ZERO_E, ONE_E, TWO_E, THREE_E, FOUR_E} ;
    encount *ep , got_ep[5] ;


#ifdef MDEBUG
    malloc_debug(2) ;
#endif
    fname = TESTFILE ;
    if(ac > 1)
    {
        fname = av[1] ;
    }
#ifdef CREATE
/* Create */

#ifdef __STDC__
    F = fopen(fname,"wb") ;
#else
    F = fopen(fname,"w") ;
#endif
    if( F == NULL)
    {
        perror("fopen failed") ;
        exit(-1) ;
    }

    /* fill the file so seeks will work even on non-virtual machines */
    for(ii=0 ; ii< 28227 ; ii++)
    {
        xdr_assert( putc(0, F) != EOF ) ;
    }

    xdrstdio_create(xdrs, F, XDR_ENCODE) ;

    xdr_assert( xdr_setpos(xdrs, seeks[jj])) ;
    xdr_assert( xdr_opaque(xdrs, text, sizeof(text))) ;
    poses[jj++] = xdr_getpos(xdrs) ;

    xdr_assert( xdr_setpos(xdrs, seeks[jj])) ;
    xdr_assert( xdr_opaque(xdrs, (caddr_t)bytes, sizeof(bytes))) ;

    /* no setpos, just for variety */
    szof = sizeof(int) ;
    count = sizeof(ints)/sizeof(int) ;
    xdr_assert( xdr_vector(xdrs, (char *)ints, count, szof, xdr_int)) ;
    poses[jj++] = xdr_getpos(xdrs) ;

    xdr_assert( xdr_setpos(xdrs, seeks[jj])) ;
    szof = sizeof(unsigned int) ;
    count = sizeof(u_ints)/sizeof(unsigned int) ;
    xdr_assert( xdr_vector(xdrs, (char *)u_ints, count, szof, xdr_u_int)) ;
    poses[jj++] = xdr_getpos(xdrs) ;

    xdr_assert( xdr_setpos(xdrs, seeks[jj])) ;
    xdr_assert( xdr_long(xdrs, &lnum)) ;
    poses[jj++] = xdr_getpos(xdrs) ;

    xdr_assert( xdr_setpos(xdrs, seeks[jj])) ;
    szof = sizeof(long) ;
    count = sizeof(longs)/sizeof(long) ;
    xdr_assert( xdr_vector(xdrs, (char *)longs, count, szof, xdr_long)) ;
    poses[jj++] = xdr_getpos(xdrs) ;

    xdr_assert( xdr_setpos(xdrs, seeks[jj])) ;
    szof = sizeof(unsigned long) ;
    count = sizeof(u_longs)/sizeof(unsigned long) ;
    xdr_assert( xdr_vector(xdrs, (char *)u_longs, count, szof, xdr_u_long)) ;
    poses[jj++] = xdr_getpos(xdrs) ;

    xdr_assert( xdr_setpos(xdrs, seeks[jj])) ;
    szof = sizeof(float) ;
    count = sizeof(floats)/sizeof(float) ;
    xdr_assert( xdr_vector(xdrs, (char *)floats, count, szof, xdr_float)) ;
    poses[jj++] = xdr_getpos(xdrs) ;

    xdr_assert( xdr_setpos(xdrs, seeks[jj])) ;
    szof = sizeof(double) ;
    count = sizeof(doubles)/sizeof(double) ;
    xdr_assert( xdr_vector(xdrs, (char *)doubles, count, szof, xdr_double)) ;
    poses[jj++] = xdr_getpos(xdrs) ;

    /* again no setpos  */
    szof = sizeof(encount) ;
    count = sizeof(encounts)/sizeof(encount) ;
    xdr_assert( xdr_vector(xdrs, (char *)encounts, count, szof, xdr_enum)) ;
    poses[jj++] = xdr_getpos(xdrs) ;

#if 0
    /* stdio performance check */
    count = 8192 ;
    for( lnum = 0 ; lnum < count ; lnum++)
    {
        xdr_assert( xdr_long(xdrs, &lnum) ) ;
    }
#endif

/* flush, rewind  and reopen */

    xdr_assert(fflush((FILE *)xdrs->x_private) != EOF) ; /* xdr_destroy(xdrs) */

    xdr_assert(fclose(F) != EOF) ;
#endif /* CREATE */
#ifdef __STDC__
    F = fopen(fname,"rb") ;
#else
    F = fopen(fname,"r") ;
#endif
    if( F == NULL)
    {
        perror("fopen failed") ;
        exit(-1) ;
    }
    xdrstdio_create(xdrs, F, XDR_DECODE) ;

    jj = 0 ;

/* check contents */

    xdr_assert( xdr_setpos(xdrs, seeks[jj])) ;
    xdr_assert( xdr_opaque(xdrs, got_s, sizeof(text))) ;
    xdr_assert( poses[jj++] = xdr_getpos(xdrs) ) ;
    printf("string: %s\n", got_s) ;

    xdr_assert( xdr_setpos(xdrs, seeks[jj])) ;
    xdr_assert( xdr_opaque(xdrs, (caddr_t)got_ab, sizeof(bytes))) ;
    printf("unsigned bytes: ");
    for(ii = 0, bp = got_ab ;
            ii < (int)sizeof(bytes) ; ii++, bp++)
    {
        printf("%u ", *bp) ;
        xdr_assert( *bp == bytes[ii] ) ;
    }
    putchar('\n') ;

    szof = sizeof(int) ;
    count = sizeof(ints)/sizeof(int) ;
    xdr_assert( xdr_vector(xdrs, (char *)got_ip, count, szof, xdr_int)) ;
    xdr_assert( poses[jj++] = xdr_getpos(xdrs) ) ;
    printf("ints: ");
    for(ii = 0, ip = got_ip ;
            ii < (int)count ; ii++, ip++)
    {
        printf("%d ", *ip) ;
        xdr_assert( *ip == ints[ii] ) ;
    }
    putchar('\n') ;

    xdr_assert( xdr_setpos(xdrs, seeks[jj])) ;
    szof = sizeof(unsigned int) ;
    count = sizeof(u_ints)/sizeof(unsigned int) ;
    xdr_assert( xdr_vector(xdrs, (char *)got_uip, count, szof, xdr_u_int)) ;
    xdr_assert( poses[jj++] = xdr_getpos(xdrs) ) ;
    printf("unsigned ints: ");
    for(ii = 0, uip = got_uip ;
            ii < (int)count ; ii++, uip++)
    {
        printf("%u ", *uip) ;
        xdr_assert( *uip == u_ints[ii] ) ;
    }
    putchar('\n') ;

    xdr_assert( xdr_setpos(xdrs, seeks[jj])) ;
    xdr_assert( xdr_long(xdrs, got_al)) ;
    xdr_assert( poses[jj++] = xdr_getpos(xdrs) ) ;
    printf("LONG: %ld\n", *got_al) ;

    xdr_assert( xdr_setpos(xdrs, seeks[jj])) ;
    szof = sizeof(long) ;
    count = sizeof(longs)/sizeof(long) ;
    xdr_assert( xdr_vector(xdrs, (char *)got_al, count, szof, xdr_long)) ;
    xdr_assert( poses[jj++] = xdr_getpos(xdrs) ) ;
    printf("longs: ");
    for(ii = 0, lp = got_al ;
            ii < (int)count ; ii++, lp++)
    {
        printf("%d ", (int)*lp) ;
        xdr_assert( (int)*lp == (int)longs[ii] ) ;
    }
    putchar('\n') ;

    xdr_assert( xdr_setpos(xdrs, seeks[jj])) ;
    szof = sizeof(unsigned long) ;
    count = sizeof(u_longs)/sizeof(unsigned long) ;
    xdr_assert( xdr_vector(xdrs, (char *)got_aul, count, szof, xdr_u_long)) ;
    xdr_assert( poses[jj++] = xdr_getpos(xdrs) ) ;
    printf("unsigned longs: ");
    for(ii = 0, ulp = got_aul ;
            ii < (int)count ; ii++, ulp++)
    {
        printf("%lu ", *ulp) ;
        xdr_assert( *ulp == u_longs[ii] ) ;
    }
    putchar('\n') ;

    xdr_assert( xdr_setpos(xdrs, seeks[jj])) ;
    szof = sizeof(float) ;
    count = sizeof(floats)/sizeof(float) ;
    xdr_assert( xdr_vector(xdrs, (char *)got_af, count, szof, xdr_float)) ;
    xdr_assert( poses[jj++] = xdr_getpos(xdrs) ) ;
    printf("floats:\n");
    for(ii = 0, fp = got_af ;
            ii < (int)count ; ii++, fp++)
    {
        printf("\t% .6e\n", *fp) ;
        xdr_assert( *fp < floats[ii] + EPSILON &&  *fp > floats[ii] - EPSILON ) ;
    }
    putchar('\n') ;

    xdr_assert( xdr_setpos(xdrs, seeks[jj])) ;
    szof = sizeof(double) ;
    count = sizeof(doubles)/sizeof(double) ;
    xdr_assert( xdr_vector(xdrs, (char *)got_ad, count, szof, xdr_double)) ;
    xdr_assert( poses[jj++] = xdr_getpos(xdrs) ) ;
    printf("doubles:\n");
    for(ii = 0, dp = got_ad ;
            ii < (int)count ; ii++, dp++)
    {
        printf("\t% .12e\n", *dp) ;
        xdr_assert( (*dp < (doubles[ii] + (double)EPSILON)) && (*dp > (doubles[ii] - (double)EPSILON) )) ;
    }
    putchar('\n') ;

    szof = sizeof(encount) ;
    count = sizeof(encounts)/sizeof(encount) ;
    xdr_assert( xdr_vector(xdrs, (char *)got_ep, count, szof, xdr_enum)) ;
    printf("enums: ");
    for(ii = 0, ep = got_ep ;
            ii < (int)count ; ii++, ep++)
    {
        printf("%d ", (int)*ep) ;
        xdr_assert( *ep == encounts[ii] ) ;
    }
    putchar('\n') ;
    xdr_assert( poses[jj++] = xdr_getpos(xdrs) ) ;

#if 0
    /* stdio performance check */
    count = 8192 ;
    for( ii = 0 ; ii < count ; ii++)
    {
        xdr_assert( xdr_long(xdrs, got_al) ) ;
        xdr_assert( *got_al == ii ) ;
    }
#endif
    exit(0) ;
}

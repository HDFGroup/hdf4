#define CHECK(ret, val, where) \
{if (Verbocity>9) printf("   Call to HDF routine: %15s at line %4d in %s returned %d \n",where,__LINE__,__FILE__,ret);\
if(ret == val) {printf("*** UNEXPECTED RETURN from %s is %d\n", where, ret); num_errs++;}}

#define RESULT(a) \
{if (Verbocity>8) printf("   Call to HDF routine: %15s at line %4d in %s returned %d \n",a,__LINE__,__FILE__,ret); \
if(ret == FAIL) {printf("*** UNEXPECTED RETURN from %s is %d\n", a, ret); num_errs++;}}

#define MESSAGE(v,a) {if (Verbocity>v) {a}}


#ifdef PROTOTYPE
#define    PROTO(x) x
#else
#define    PROTO(x) ()
#endif

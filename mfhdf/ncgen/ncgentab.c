/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* First part of user prologue.  */
#line 8 "mfhdf/ncgen/ncgen.y"

#include <string.h>
#include <stdlib.h>
#include "ncgen.h"

int  yylex(void);
void yyerror(char *s);

typedef struct Symbol { /* symbol table entry */
    char          *name;
    struct Symbol *next;
    unsigned       is_dim : 1; /* appears as netCDF dimension */
    unsigned       is_var : 1; /* appears as netCDF variable */
    unsigned       is_att : 1; /* appears as netCDF attribute */
    int            dnum;       /* handle as a dimension */
    int            vnum;       /* handle as a variable */
} * YYSTYPE1;

#define YYSTYPE YYSTYPE1
YYSTYPE install(char *sname), lookup(char *sname);
YYSTYPE symlist; /* symbol table: linked list */

void        init_netcdf(void);               /* initializes netcdf counts (e.g. nvars) */
void        define_netcdf(char *netcdfname); /* generates all define mode stuff */
void        load_netcdf(void *rec_start);    /* generates variable puts */
void        close_netcdf(void);              /* generates close */
const char *nctype(nc_type);

void  derror(const char *fmt, ...);                      /* varargs message emitter */
void *emalloc(int size), *erealloc(void *ptr, int size); /* malloc that checks for memory exhausted */
void  clearout(void);                                    /* initializes symbol table */
void  nc_getfill(nc_type, union generic *);              /* to get fill value for various types */
void  nc_putfill(nc_type, void *, union generic *);      /* to get fill value for various types */
void  nc_fill(nc_type, long, void *, union generic);     /* fills a generic array with a value */
int   put_variable(void *);                              /* invoke nc calls or generate code to put */
                                                         /* variable values            */
extern int derror_count;                                 /* counts errors in netcdf definition */
extern int lineno;                                       /* line number for error messages */

static int    not_a_string;        /* whether last constant read was a string */
static char   termstring[MAXTRST]; /* last terminal string read */
static double double_val;          /* last double value read */
static float  float_val;           /* last float value read */
static nclong long_val;            /* last long value read */
static short  short_val;           /* last short value read */
static char   char_val;            /* last char value read */
static char   byte_val;            /* last byte value read */

static nc_type type_code;  /* holds declared type for variables */
static nc_type atype_code; /* holds derived type for attributes */
static char   *netcdfname; /* to construct netcdf file name */
static void   *att_space;  /* pointer to block for attribute values */
static nc_type valtype;    /* type code for list of attribute values  */

static char   *char_valp; /* pointers used to accumulate data values */
static char   *byte_valp;
static short  *short_valp;
static nclong *long_valp;
static float  *float_valp;
static double *double_valp;
static void   *rec_cur;   /* pointer to where next data value goes */
static void   *rec_start; /* start of space for a record of data */

#line 135 "mfhdf/ncgen/ncgentab.c"

#ifndef YY_CAST
#ifdef __cplusplus
#define YY_CAST(Type, Val)             static_cast<Type>(Val)
#define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type>(Val)
#else
#define YY_CAST(Type, Val)             ((Type)(Val))
#define YY_REINTERPRET_CAST(Type, Val) ((Type)(Val))
#endif
#endif
#ifndef YY_NULLPTR
#if defined __cplusplus
#if 201103L <= __cplusplus
#define YY_NULLPTR nullptr
#else
#define YY_NULLPTR 0
#endif
#else
#define YY_NULLPTR ((void *)0)
#endif
#endif

#include "ncgentab.h"
/* Symbol kind.  */
enum yysymbol_kind_t {
    YYSYMBOL_YYEMPTY        = -2,
    YYSYMBOL_YYEOF          = 0,  /* "end of file"  */
    YYSYMBOL_YYerror        = 1,  /* error  */
    YYSYMBOL_YYUNDEF        = 2,  /* "invalid token"  */
    YYSYMBOL_NC_UNLIMITED_K = 3,  /* NC_UNLIMITED_K  */
    YYSYMBOL_BYTE_K         = 4,  /* BYTE_K  */
    YYSYMBOL_CHAR_K         = 5,  /* CHAR_K  */
    YYSYMBOL_SHORT_K        = 6,  /* SHORT_K  */
    YYSYMBOL_LONG_K         = 7,  /* LONG_K  */
    YYSYMBOL_FLOAT_K        = 8,  /* FLOAT_K  */
    YYSYMBOL_DOUBLE_K       = 9,  /* DOUBLE_K  */
    YYSYMBOL_IDENT          = 10, /* IDENT  */
    YYSYMBOL_TERMSTRING     = 11, /* TERMSTRING  */
    YYSYMBOL_BYTE_CONST     = 12, /* BYTE_CONST  */
    YYSYMBOL_CHAR_CONST     = 13, /* CHAR_CONST  */
    YYSYMBOL_SHORT_CONST    = 14, /* SHORT_CONST  */
    YYSYMBOL_LONG_CONST     = 15, /* LONG_CONST  */
    YYSYMBOL_FLOAT_CONST    = 16, /* FLOAT_CONST  */
    YYSYMBOL_DOUBLE_CONST   = 17, /* DOUBLE_CONST  */
    YYSYMBOL_DIMENSIONS     = 18, /* DIMENSIONS  */
    YYSYMBOL_VARIABLES      = 19, /* VARIABLES  */
    YYSYMBOL_NETCDF         = 20, /* NETCDF  */
    YYSYMBOL_DATA           = 21, /* DATA  */
    YYSYMBOL_22_            = 22, /* '{'  */
    YYSYMBOL_23_            = 23, /* '}'  */
    YYSYMBOL_24_            = 24, /* ';'  */
    YYSYMBOL_25_            = 25, /* ','  */
    YYSYMBOL_26_            = 26, /* '='  */
    YYSYMBOL_27_            = 27, /* '('  */
    YYSYMBOL_28_            = 28, /* ')'  */
    YYSYMBOL_29_            = 29, /* ':'  */
    YYSYMBOL_YYACCEPT       = 30, /* $accept  */
    YYSYMBOL_ncdesc         = 31, /* ncdesc  */
    YYSYMBOL_32_1           = 32, /* $@1  */
    YYSYMBOL_33_2           = 33, /* $@2  */
    YYSYMBOL_34_3           = 34, /* $@3  */
    YYSYMBOL_dimsection     = 35, /* dimsection  */
    YYSYMBOL_dimdecls       = 36, /* dimdecls  */
    YYSYMBOL_dimdecline     = 37, /* dimdecline  */
    YYSYMBOL_dimdecl        = 38, /* dimdecl  */
    YYSYMBOL_dimd           = 39, /* dimd  */
    YYSYMBOL_dim            = 40, /* dim  */
    YYSYMBOL_vasection      = 41, /* vasection  */
    YYSYMBOL_vadecls        = 42, /* vadecls  */
    YYSYMBOL_vadecl         = 43, /* vadecl  */
    YYSYMBOL_vardecl        = 44, /* vardecl  */
    YYSYMBOL_type           = 45, /* type  */
    YYSYMBOL_varlist        = 46, /* varlist  */
    YYSYMBOL_varspec        = 47, /* varspec  */
    YYSYMBOL_48_4           = 48, /* $@4  */
    YYSYMBOL_var            = 49, /* var  */
    YYSYMBOL_dimspec        = 50, /* dimspec  */
    YYSYMBOL_dimlist        = 51, /* dimlist  */
    YYSYMBOL_vdim           = 52, /* vdim  */
    YYSYMBOL_attdecl        = 53, /* attdecl  */
    YYSYMBOL_54_5           = 54, /* $@5  */
    YYSYMBOL_att            = 55, /* att  */
    YYSYMBOL_avar           = 56, /* avar  */
    YYSYMBOL_attr           = 57, /* attr  */
    YYSYMBOL_attvallist     = 58, /* attvallist  */
    YYSYMBOL_aconst         = 59, /* aconst  */
    YYSYMBOL_attconst       = 60, /* attconst  */
    YYSYMBOL_datasection    = 61, /* datasection  */
    YYSYMBOL_datadecls      = 62, /* datadecls  */
    YYSYMBOL_datadecl       = 63, /* datadecl  */
    YYSYMBOL_64_6           = 64, /* $@6  */
    YYSYMBOL_constlist      = 65, /* constlist  */
    YYSYMBOL_dconst         = 66, /* dconst  */
    YYSYMBOL_67_7           = 67, /* $@7  */
    YYSYMBOL_const          = 68  /* const  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;

#ifdef short
#undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
#include <limits.h> /* INFRINGES ON USER NAME SPACE */
#if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#define YY_STDINT_H
#endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined               YY_STDINT_H
typedef int_least8_t   yytype_int8;
#else
typedef signed char    yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined                YY_STDINT_H
typedef int_least16_t  yytype_int16;
#else
typedef short          yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
#undef UINT_LEAST8_MAX
#undef UINT_LEAST16_MAX
#define UINT_LEAST8_MAX  255
#define UINT_LEAST16_MAX 65535
#endif

#if defined                  __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t  yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char  yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined                   __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int   yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
#if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#define YYPTRDIFF_T       __PTRDIFF_TYPE__
#define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
#elif defined PTRDIFF_MAX
#ifndef ptrdiff_t
#include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#endif
#define YYPTRDIFF_T       ptrdiff_t
#define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
#else
#define YYPTRDIFF_T       long
#define YYPTRDIFF_MAXIMUM LONG_MAX
#endif
#endif

#ifndef YYSIZE_T
#ifdef __SIZE_TYPE__
#define YYSIZE_T __SIZE_TYPE__
#elif defined size_t
#define YYSIZE_T size_t
#elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#define YYSIZE_T size_t
#else
#define YYSIZE_T unsigned
#endif
#endif

#define YYSIZE_MAXIMUM                                                                                       \
    YY_CAST(YYPTRDIFF_T,                                                                                     \
            (YYPTRDIFF_MAXIMUM < YY_CAST(YYSIZE_T, -1) ? YYPTRDIFF_MAXIMUM : YY_CAST(YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST(YYPTRDIFF_T, sizeof(X))

/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
#if defined YYENABLE_NLS && YYENABLE_NLS
#if ENABLE_NLS
#include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#define YY_(Msgid) dgettext("bison-runtime", Msgid)
#endif
#endif
#ifndef YY_
#define YY_(Msgid) Msgid
#endif
#endif

#ifndef YY_ATTRIBUTE_PURE
#if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#define YY_ATTRIBUTE_PURE __attribute__((__pure__))
#else
#define YY_ATTRIBUTE_PURE
#endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
#if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#define YY_ATTRIBUTE_UNUSED __attribute__((__unused__))
#else
#define YY_ATTRIBUTE_UNUSED
#endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if !defined lint || defined __GNUC__
#define YY_USE(E) ((void)(E))
#else
#define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && !defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
#if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                                                                  \
    _Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wuninitialized\"")
#else
#define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                                                                  \
    _Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wuninitialized\"")                     \
        _Pragma("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
#endif
#define YY_IGNORE_MAYBE_UNINITIALIZED_END _Pragma("GCC diagnostic pop")
#else
#define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
#define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
#define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
#define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && !defined __ICC && 6 <= __GNUC__
#define YY_IGNORE_USELESS_CAST_BEGIN                                                                         \
    _Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wuseless-cast\"")
#define YY_IGNORE_USELESS_CAST_END _Pragma("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
#define YY_IGNORE_USELESS_CAST_BEGIN
#define YY_IGNORE_USELESS_CAST_END
#endif

#define YY_ASSERT(E) ((void)(0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

#ifdef YYSTACK_USE_ALLOCA
#if YYSTACK_USE_ALLOCA
#ifdef __GNUC__
#define YYSTACK_ALLOC __builtin_alloca
#elif defined __BUILTIN_VA_ARG_INCR
#include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#elif defined _AIX
#define YYSTACK_ALLOC __alloca
#elif defined _MSC_VER
#include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#define alloca _alloca
#else
#define YYSTACK_ALLOC alloca
#if !defined _ALLOCA_H && !defined EXIT_SUCCESS
#include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
/* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif
#endif
#endif
#endif
#endif

#ifdef YYSTACK_ALLOC
/* Pacify GCC's 'empty if-body' warning.  */
#define YYSTACK_FREE(Ptr)                                                                                    \
    do { /* empty */                                                                                         \
        ;                                                                                                    \
    } while (0)
#ifndef YYSTACK_ALLOC_MAXIMUM
/* The OS might guarantee only one guard page at the bottom of the stack,
   and a page size can be as small as 4096 bytes.  So we cannot safely
   invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
   to allow for a few compiler-allocated temporary stack slots.  */
#define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#endif
#else
#define YYSTACK_ALLOC YYMALLOC
#define YYSTACK_FREE  YYFREE
#ifndef YYSTACK_ALLOC_MAXIMUM
#define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#endif
#if (defined __cplusplus && !defined EXIT_SUCCESS &&                                                         \
     !((defined YYMALLOC || defined malloc) && (defined YYFREE || defined free)))
#include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif
#endif
#ifndef YYMALLOC
#define YYMALLOC malloc
#if !defined malloc && !defined EXIT_SUCCESS
void *malloc(YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#endif
#endif
#ifndef YYFREE
#define YYFREE free
#if !defined free && !defined EXIT_SUCCESS
void  free(void *);     /* INFRINGES ON USER NAME SPACE */
#endif
#endif
#endif
#endif /* !defined yyoverflow */

#if (!defined yyoverflow && (!defined __cplusplus || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc {
    yy_state_t yyss_alloc;
    YYSTYPE    yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
#define YYSTACK_GAP_MAXIMUM (YYSIZEOF(union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
#define YYSTACK_BYTES(N) ((N) * (YYSIZEOF(yy_state_t) + YYSIZEOF(YYSTYPE)) + YYSTACK_GAP_MAXIMUM)

#define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
#define YYSTACK_RELOCATE(Stack_alloc, Stack)                                                                 \
    do {                                                                                                     \
        YYPTRDIFF_T yynewbytes;                                                                              \
        YYCOPY(&yyptr->Stack_alloc, Stack, yysize);                                                          \
        Stack      = &yyptr->Stack_alloc;                                                                    \
        yynewbytes = yystacksize * YYSIZEOF(*Stack) + YYSTACK_GAP_MAXIMUM;                                   \
        yyptr += yynewbytes / YYSIZEOF(*yyptr);                                                              \
    } while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
#ifndef YYCOPY
#if defined __GNUC__ && 1 < __GNUC__
#define YYCOPY(Dst, Src, Count) __builtin_memcpy(Dst, Src, YY_CAST(YYSIZE_T, (Count)) * sizeof(*(Src)))
#else
#define YYCOPY(Dst, Src, Count)                                                                              \
    do {                                                                                                     \
        YYPTRDIFF_T yyi;                                                                                     \
        for (yyi = 0; yyi < (Count); yyi++)                                                                  \
            (Dst)[yyi] = (Src)[yyi];                                                                         \
    } while (0)
#endif
#endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL 4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST 70

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS 30
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS 39
/* YYNRULES -- Number of rules.  */
#define YYNRULES 71
/* YYNSTATES -- Number of states.  */
#define YYNSTATES 101

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK 276

/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                                                     \
    (0 <= (YYX) && (YYX) <= YYMAXUTOK ? YY_CAST(yysymbol_kind_t, yytranslate[YYX]) : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] = {
    0, 2, 2,  2,  2, 2,  2, 2, 2, 2, 2, 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, 2, 2, 2,
    2, 2, 2,  2,  2, 2,  2, 2, 2, 2, 2, 2,  27, 28, 2,  2,  25, 2,  2,  2,  2,  2,  2,  2,  2, 2, 2, 2,
    2, 2, 29, 24, 2, 26, 2, 2, 2, 2, 2, 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, 2, 2, 2,
    2, 2, 2,  2,  2, 2,  2, 2, 2, 2, 2, 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, 2, 2, 2,
    2, 2, 2,  2,  2, 2,  2, 2, 2, 2, 2, 22, 2,  23, 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, 2, 2, 2,
    2, 2, 2,  2,  2, 2,  2, 2, 2, 2, 2, 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, 2, 2, 2,
    2, 2, 2,  2,  2, 2,  2, 2, 2, 2, 2, 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, 2, 2, 2,
    2, 2, 2,  2,  2, 2,  2, 2, 2, 2, 2, 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, 2, 2, 2,
    2, 2, 2,  2,  2, 2,  2, 2, 2, 2, 2, 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, 2, 2, 2,
    2, 2, 2,  2,  1, 2,  3, 4, 5, 6, 7, 8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] = {
    0,   103, 103, 105, 110, 101, 121, 122, 124, 125, 127, 128, 130, 136, 144, 155, 157, 158,
    160, 161, 163, 163, 165, 167, 168, 169, 170, 171, 172, 174, 175, 178, 177, 203, 205, 206,
    208, 209, 211, 231, 230, 262, 263, 269, 279, 285, 286, 288, 297, 303, 314, 320, 326, 332,
    338, 346, 347, 350, 351, 354, 353, 416, 417, 420, 420, 486, 511, 544, 569, 594, 619, 644};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST(yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name(yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] = {"\"end of file\"",
                                      "error",
                                      "\"invalid token\"",
                                      "NC_UNLIMITED_K",
                                      "BYTE_K",
                                      "CHAR_K",
                                      "SHORT_K",
                                      "LONG_K",
                                      "FLOAT_K",
                                      "DOUBLE_K",
                                      "IDENT",
                                      "TERMSTRING",
                                      "BYTE_CONST",
                                      "CHAR_CONST",
                                      "SHORT_CONST",
                                      "LONG_CONST",
                                      "FLOAT_CONST",
                                      "DOUBLE_CONST",
                                      "DIMENSIONS",
                                      "VARIABLES",
                                      "NETCDF",
                                      "DATA",
                                      "'{'",
                                      "'}'",
                                      "';'",
                                      "','",
                                      "'='",
                                      "'('",
                                      "')'",
                                      "':'",
                                      "$accept",
                                      "ncdesc",
                                      "$@1",
                                      "$@2",
                                      "$@3",
                                      "dimsection",
                                      "dimdecls",
                                      "dimdecline",
                                      "dimdecl",
                                      "dimd",
                                      "dim",
                                      "vasection",
                                      "vadecls",
                                      "vadecl",
                                      "vardecl",
                                      "type",
                                      "varlist",
                                      "varspec",
                                      "$@4",
                                      "var",
                                      "dimspec",
                                      "dimlist",
                                      "vdim",
                                      "attdecl",
                                      "$@5",
                                      "att",
                                      "avar",
                                      "attr",
                                      "attvallist",
                                      "aconst",
                                      "attconst",
                                      "datasection",
                                      "datadecls",
                                      "datadecl",
                                      "$@6",
                                      "constlist",
                                      "dconst",
                                      "$@7",
                                      "const",
                                      YY_NULLPTR};

static const char *
yysymbol_name(yysymbol_kind_t yysymbol)
{
    return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-57)

#define yypact_value_is_default(Yyn) ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) 0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] = {
    7,   -2,  38,  -57, -57, 21,  30,  -57, -57, 30,  -1,  -57, 15,  -57, 23,  12,  -57, 30,  6,   -4,  -57,
    -57, -57, -57, -57, -57, -57, -57, -57, -57, -57, -57, 33,  -4,  20,  -57, 35,  -57, -57, -57, 17,  26,
    -57, -57, 24,  -57, 25,  -57, -57, 27,  33,  35,  28,  -57, 35,  22,  0,   -57, -57, 35,  31,  -57, -57,
    30,  -57, -57, -57, -57, -57, -57, -57, -57, 29,  -57, -57, 32,  36,  -57, -57, -6,  -57, 0,   -57, -57,
    30,  -57, -57, 34,  -57, 18,  -57, -57, -57, -57, -57, -57, -57, -57, -57, -57, -57};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] = {
    0,  0,  0,  2,  1,  6,  0,  3,  15, 7,  0,  10, 0,  14, 16, 0,  8,  0,  0,  0,  4,  9,  11, 13, 12, 23,
    24, 25, 26, 27, 28, 33, 0,  17, 0,  20, 0,  43, 21, 39, 0,  55, 44, 42, 0,  18, 22, 29, 31, 0,  0,  0,
    0,  19, 0,  34, 0,  41, 59, 56, 0,  5,  30, 0,  32, 49, 50, 48, 51, 52, 53, 54, 40, 45, 47, 0,  0,  57,
    38, 0,  36, 0,  63, 58, 0,  35, 46, 60, 61, 0,  37, 63, 66, 67, 65, 68, 69, 70, 71, 64, 62};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] = {-57, -57, -57, -57, -57, -57, -57, 43,  39,  -57, -56, -57, -57,
                                      37,  -57, -57, -57, 3,   -57, -28, -57, -57, -23, -57, -57, -57,
                                      -41, 13,  -57, -19, -57, -57, -57, 5,   -57, -57, -26, -57, -57};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] = {0,  2,  5,  14, 41, 7,  9,  10, 11, 12, 13, 20, 33,
                                        34, 35, 36, 46, 47, 55, 37, 64, 79, 80, 38, 49, 39,
                                        40, 43, 72, 73, 74, 52, 59, 60, 75, 87, 88, 89, 99};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] = {25, 26, 27, 28, 29, 30, 31, 78, 48, 23, 58, 65,  66, 67, 68, 69, 70, 71,
                                      58, 84, 3,  24, 85, 16, 17, 32, 48, 1,  78, 92,  93, 94, 95, 96, 97, 98,
                                      21, 17, 4,  6,  8,  18, 19, 42, 45, 31, 50, 51,  53, 63, 54, 61, 15, 56,
                                      81, 77, 22, 62, 82, 91, 83, 90, 86, 57, 76, 100, 0,  0,  0,  0,  44};

static const yytype_int8 yycheck[] = {4,  5,  6,  7,  8,  9,  10, 63, 36, 3,  51, 11, 12, 13, 14, 15, 16, 17,
                                      59, 25, 22, 15, 28, 24, 25, 29, 54, 20, 84, 11, 12, 13, 14, 15, 16, 17,
                                      24, 25, 0,  18, 10, 26, 19, 10, 24, 10, 29, 21, 24, 27, 25, 23, 9,  26,
                                      25, 24, 17, 54, 26, 25, 24, 84, 81, 50, 59, 91, -1, -1, -1, -1, 33};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] = {
    0,  20, 31, 22, 0,  32, 18, 35, 10, 36, 37, 38, 39, 40, 33, 37, 24, 25, 26, 19, 41, 24, 38, 3,  15, 4,
    5,  6,  7,  8,  9,  10, 29, 42, 43, 44, 45, 49, 53, 55, 56, 34, 10, 57, 43, 24, 46, 47, 49, 54, 29, 21,
    61, 24, 25, 48, 26, 57, 56, 62, 63, 23, 47, 27, 50, 11, 12, 13, 14, 15, 16, 17, 58, 59, 60, 64, 63, 24,
    40, 51, 52, 25, 26, 24, 25, 28, 59, 65, 66, 67, 52, 25, 11, 12, 13, 14, 15, 16, 17, 68, 66};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] = {0,  30, 32, 33, 34, 31, 35, 35, 36, 36, 37, 37, 38, 38, 39, 40, 41, 41,
                                   42, 42, 43, 43, 44, 45, 45, 45, 45, 45, 45, 46, 46, 48, 47, 49, 50, 50,
                                   51, 51, 52, 54, 53, 55, 55, 56, 57, 58, 58, 59, 60, 60, 60, 60, 60, 60,
                                   60, 61, 61, 62, 62, 64, 63, 65, 65, 67, 66, 68, 68, 68, 68, 68, 68, 68};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] = {0, 2, 0, 0, 0, 9, 0, 2, 2, 3, 1, 3, 3, 3, 1, 1, 0, 2, 2, 3, 1, 1, 2, 1,
                                   1, 1, 1, 1, 1, 1, 3, 0, 3, 1, 0, 3, 1, 3, 1, 0, 4, 3, 2, 1, 1, 1, 3, 1,
                                   1, 1, 1, 1, 1, 1, 1, 0, 2, 2, 3, 0, 4, 1, 3, 0, 2, 1, 1, 1, 1, 1, 1, 1};

enum { YYENOMEM = -2 };

#define yyerrok   (yyerrstatus = 0)
#define yyclearin (yychar = YYEMPTY)

#define YYACCEPT goto yyacceptlab
#define YYABORT  goto yyabortlab
#define YYERROR  goto yyerrorlab
#define YYNOMEM  goto yyexhaustedlab

#define YYRECOVERING() (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                                                               \
    do                                                                                                       \
        if (yychar == YYEMPTY) {                                                                             \
            yychar = (Token);                                                                                \
            yylval = (Value);                                                                                \
            YYPOPSTACK(yylen);                                                                               \
            yystate = *yyssp;                                                                                \
            goto yybackup;                                                                                   \
        }                                                                                                    \
        else {                                                                                               \
            yyerror(YY_("syntax error: cannot back up"));                                                    \
            YYERROR;                                                                                         \
        }                                                                                                    \
    while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* Enable debugging if requested.  */
#if YYDEBUG

#ifndef YYFPRINTF
#include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#define YYFPRINTF fprintf
#endif

#define YYDPRINTF(Args)                                                                                      \
    do {                                                                                                     \
        if (yydebug)                                                                                         \
            YYFPRINTF Args;                                                                                  \
    } while (0)

#define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                                                        \
    do {                                                                                                     \
        if (yydebug) {                                                                                       \
            YYFPRINTF(stderr, "%s ", Title);                                                                 \
            yy_symbol_print(stderr, Kind, Value);                                                            \
            YYFPRINTF(stderr, "\n");                                                                         \
        }                                                                                                    \
    } while (0)

/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print(FILE *yyo, yysymbol_kind_t yykind, YYSTYPE const *const yyvaluep)
{
    FILE *yyoutput = yyo;
    YY_USE(yyoutput);
    if (!yyvaluep)
        return;
    YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
    YY_USE(yykind);
    YY_IGNORE_MAYBE_UNINITIALIZED_END
}

/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print(FILE *yyo, yysymbol_kind_t yykind, YYSTYPE const *const yyvaluep)
{
    YYFPRINTF(yyo, "%s %s (", yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name(yykind));

    yy_symbol_value_print(yyo, yykind, yyvaluep);
    YYFPRINTF(yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print(yy_state_t *yybottom, yy_state_t *yytop)
{
    YYFPRINTF(stderr, "Stack now");
    for (; yybottom <= yytop; yybottom++) {
        int yybot = *yybottom;
        YYFPRINTF(stderr, " %d", yybot);
    }
    YYFPRINTF(stderr, "\n");
}

#define YY_STACK_PRINT(Bottom, Top)                                                                          \
    do {                                                                                                     \
        if (yydebug)                                                                                         \
            yy_stack_print((Bottom), (Top));                                                                 \
    } while (0)

/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print(yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
{
    int yylno  = yyrline[yyrule];
    int yynrhs = yyr2[yyrule];
    int yyi;
    YYFPRINTF(stderr, "Reducing stack by rule %d (line %d):\n", yyrule - 1, yylno);
    /* The symbols being reduced.  */
    for (yyi = 0; yyi < yynrhs; yyi++) {
        YYFPRINTF(stderr, "   $%d = ", yyi + 1);
        yy_symbol_print(stderr, YY_ACCESSING_SYMBOL(+yyssp[yyi + 1 - yynrhs]), &yyvsp[(yyi + 1) - (yynrhs)]);
        YYFPRINTF(stderr, "\n");
    }
}

#define YY_REDUCE_PRINT(Rule)                                                                                \
    do {                                                                                                     \
        if (yydebug)                                                                                         \
            yy_reduce_print(yyssp, yyvsp, Rule);                                                             \
    } while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
#define YYDPRINTF(Args) ((void)0)
#define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
#define YY_STACK_PRINT(Bottom, Top)
#define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
#define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct(const char *yymsg, yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
    YY_USE(yyvaluep);
    if (!yymsg)
        yymsg = "Deleting";
    YY_SYMBOL_PRINT(yymsg, yykind, yyvaluep, yylocationp);

    YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
    YY_USE(yykind);
    YY_IGNORE_MAYBE_UNINITIALIZED_END
}

/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;

/*----------.
| yyparse.  |
`----------*/

int
yyparse(void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t  yyssa[YYINITDEPTH];
    yy_state_t *yyss  = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE  yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs  = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    int yyn;
    /* The return value of yyparse.  */
    int yyresult;
    /* Lookahead symbol kind.  */
    yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
    /* The variables used to return semantic value and location from the
       action routines.  */
    YYSTYPE yyval;

#define YYPOPSTACK(N) (yyvsp -= (N), yyssp -= (N))

    /* The number of symbols on the RHS of the reduced rule.
       Keep to zero when no symbol should be popped.  */
    int yylen = 0;

    YYDPRINTF((stderr, "Starting parse\n"));

    yychar = YYEMPTY; /* Cause a token to be read.  */

    goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
    /* In all cases, when you get here, the value and location stacks
       have just been pushed.  So pushing a state here evens the stacks.  */
    yyssp++;

/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
    YYDPRINTF((stderr, "Entering state %d\n", yystate));
    YY_ASSERT(0 <= yystate && yystate < YYNSTATES);
    YY_IGNORE_USELESS_CAST_BEGIN
    *yyssp = YY_CAST(yy_state_t, yystate);
    YY_IGNORE_USELESS_CAST_END
    YY_STACK_PRINT(yyss, yyssp);

    if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
        YYNOMEM;
#else
    {
        /* Get the current used size of the three stacks, in elements.  */
        YYPTRDIFF_T yysize = yyssp - yyss + 1;

#if defined yyoverflow
        {
            /* Give user a chance to reallocate the stack.  Use copies of
               these so that the &'s don't force the real ones into
               memory.  */
            yy_state_t *yyss1 = yyss;
            YYSTYPE    *yyvs1 = yyvs;

            /* Each stack pointer address is followed by the size of the
               data in use in that stack, in bytes.  This used to be a
               conditional around just the two extra args, but that might
               be undefined if yyoverflow is a macro.  */
            yyoverflow(YY_("memory exhausted"), &yyss1, yysize * YYSIZEOF(*yyssp), &yyvs1,
                       yysize * YYSIZEOF(*yyvsp), &yystacksize);
            yyss = yyss1;
            yyvs = yyvs1;
        }
#else /* defined YYSTACK_RELOCATE */
        /* Extend the stack our own way.  */
        if (YYMAXDEPTH <= yystacksize)
            YYNOMEM;
        yystacksize *= 2;
        if (YYMAXDEPTH < yystacksize)
            yystacksize = YYMAXDEPTH;

        {
            yy_state_t    *yyss1 = yyss;
            union yyalloc *yyptr =
                YY_CAST(union yyalloc *, YYSTACK_ALLOC(YY_CAST(YYSIZE_T, YYSTACK_BYTES(yystacksize))));
            if (!yyptr)
                YYNOMEM;
            YYSTACK_RELOCATE(yyss_alloc, yyss);
            YYSTACK_RELOCATE(yyvs_alloc, yyvs);
#undef YYSTACK_RELOCATE
            if (yyss1 != yyssa)
                YYSTACK_FREE(yyss1);
        }
#endif

        yyssp = yyss + yysize - 1;
        yyvsp = yyvs + yysize - 1;

        YY_IGNORE_USELESS_CAST_BEGIN
        YYDPRINTF((stderr, "Stack size increased to %ld\n", YY_CAST(long, yystacksize)));
        YY_IGNORE_USELESS_CAST_END

        if (yyss + yystacksize - 1 <= yyssp)
            YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

    if (yystate == YYFINAL)
        YYACCEPT;

    goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:
    /* Do appropriate processing given the current state.  Read a
       lookahead token if we need one and don't already have one.  */

    /* First try to decide what to do without reference to lookahead token.  */
    yyn = yypact[yystate];
    if (yypact_value_is_default(yyn))
        goto yydefault;

    /* Not known => get a lookahead token if don't already have one.  */

    /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
    if (yychar == YYEMPTY) {
        YYDPRINTF((stderr, "Reading a token\n"));
        yychar = yylex();
    }

    if (yychar <= YYEOF) {
        yychar  = YYEOF;
        yytoken = YYSYMBOL_YYEOF;
        YYDPRINTF((stderr, "Now at end of input.\n"));
    }
    else if (yychar == YYerror) {
        /* The scanner already issued an error message, process directly
           to error recovery.  But do not keep the error token as
           lookahead, it is too special and may lead us to an endless
           loop in error recovery. */
        yychar  = YYUNDEF;
        yytoken = YYSYMBOL_YYerror;
        goto yyerrlab1;
    }
    else {
        yytoken = YYTRANSLATE(yychar);
        YY_SYMBOL_PRINT("Next token is", yytoken, &yylval, &yylloc);
    }

    /* If the proper action on seeing token YYTOKEN is to reduce or to
       detect an error, take that action.  */
    yyn += yytoken;
    if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
        goto yydefault;
    yyn = yytable[yyn];
    if (yyn <= 0) {
        if (yytable_value_is_error(yyn))
            goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
    }

    /* Count tokens shifted since error; after three, turn off error
       status.  */
    if (yyerrstatus)
        yyerrstatus--;

    /* Shift the lookahead token.  */
    YY_SYMBOL_PRINT("Shifting", yytoken, &yylval, &yylloc);
    yystate = yyn;
    YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
    *++yyvsp = yylval;
    YY_IGNORE_MAYBE_UNINITIALIZED_END

    /* Discard the shifted token.  */
    yychar = YYEMPTY;
    goto yynewstate;

/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
    yyn = yydefact[yystate];
    if (yyn == 0)
        goto yyerrlab;
    goto yyreduce;

/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
    /* yyn is the number of a rule to reduce with.  */
    yylen = yyr2[yyn];

    /* If YYLEN is nonzero, implement the default value of the action:
       '$$ = $1'.

       Otherwise, the following line sets YYVAL to garbage.
       This behavior is undocumented and Bison
       users should not rely upon it.  Assigning to YYVAL
       unconditionally makes the parser a bit smaller, and it avoids a
       GCC warning that YYVAL may be used uninitialized.  */
    yyval = yyvsp[1 - yylen];

    YY_REDUCE_PRINT(yyn);
    switch (yyn) {
        case 2: /* $@1: %empty  */
#line 103 "mfhdf/ncgen/ncgen.y"
        {
            init_netcdf();
        }
#line 1258 "mfhdf/ncgen/ncgentab.c"
        break;

        case 3: /* $@2: %empty  */
#line 105 "mfhdf/ncgen/ncgen.y"
        {
            if (ndims > H4_MAX_NC_DIMS)
                derror("Too many dimensions");
        }
#line 1267 "mfhdf/ncgen/ncgentab.c"
        break;

        case 4: /* $@3: %empty  */
#line 110 "mfhdf/ncgen/ncgen.y"
        {
            if (derror_count == 0)
                define_netcdf(netcdfname);
        }
#line 1276 "mfhdf/ncgen/ncgentab.c"
        break;

        case 5: /* ncdesc: NETCDF '{' $@1 dimsection $@2 vasection $@3 datasection '}'  */
#line 116 "mfhdf/ncgen/ncgen.y"
        {
            if (derror_count == 0)
                close_netcdf();
        }
#line 1285 "mfhdf/ncgen/ncgentab.c"
        break;

        case 12: /* dimdecl: dimd '=' LONG_CONST  */
#line 131 "mfhdf/ncgen/ncgen.y"
        {
            if (long_val <= 0)
                derror("negative dimension size");
            dims[ndims].size = long_val;
            ndims++;
        }
#line 1295 "mfhdf/ncgen/ncgentab.c"
        break;

        case 13: /* dimdecl: dimd '=' NC_UNLIMITED_K  */
#line 137 "mfhdf/ncgen/ncgen.y"
        {
            if (rec_dim != -1)
                derror("only one NC_UNLIMITED dimension allowed");
            rec_dim          = ndims; /* the unlimited (record) dimension */
            dims[ndims].size = NC_UNLIMITED;
            ndims++;
        }
#line 1306 "mfhdf/ncgen/ncgentab.c"
        break;

        case 14: /* dimd: dim  */
#line 145 "mfhdf/ncgen/ncgen.y"
        {
            if (yyvsp[0]->is_dim == 1) {
                derror("duplicate dimension declaration for %s", yyvsp[0]->name);
            }
            yyvsp[0]->is_dim = 1;
            yyvsp[0]->dnum   = ndims;
            dims[ndims].name = (char *)emalloc(strlen(yyvsp[0]->name) + 1);
            (void)strcpy(dims[ndims].name, yyvsp[0]->name);
        }
#line 1320 "mfhdf/ncgen/ncgentab.c"
        break;

        case 23: /* type: BYTE_K  */
#line 167 "mfhdf/ncgen/ncgen.y"
        {
            type_code = NC_BYTE;
        }
#line 1326 "mfhdf/ncgen/ncgentab.c"
        break;

        case 24: /* type: CHAR_K  */
#line 168 "mfhdf/ncgen/ncgen.y"
        {
            type_code = NC_CHAR;
        }
#line 1332 "mfhdf/ncgen/ncgentab.c"
        break;

        case 25: /* type: SHORT_K  */
#line 169 "mfhdf/ncgen/ncgen.y"
        {
            type_code = NC_SHORT;
        }
#line 1338 "mfhdf/ncgen/ncgentab.c"
        break;

        case 26: /* type: LONG_K  */
#line 170 "mfhdf/ncgen/ncgen.y"
        {
            type_code = NC_LONG;
        }
#line 1344 "mfhdf/ncgen/ncgentab.c"
        break;

        case 27: /* type: FLOAT_K  */
#line 171 "mfhdf/ncgen/ncgen.y"
        {
            type_code = NC_FLOAT;
        }
#line 1350 "mfhdf/ncgen/ncgentab.c"
        break;

        case 28: /* type: DOUBLE_K  */
#line 172 "mfhdf/ncgen/ncgen.y"
        {
            type_code = NC_DOUBLE;
        }
#line 1356 "mfhdf/ncgen/ncgentab.c"
        break;

        case 31: /* $@4: %empty  */
#line 178 "mfhdf/ncgen/ncgen.y"
        {
            if (nvars >= H4_MAX_NC_VARS)
                derror("too many variables");
            nvdims = 0;
            /* make sure variable not re-declared */
            if (yyvsp[0]->is_var == 1) {
                derror("duplicate variable declaration for %s", yyvsp[0]->name);
            }
            yyvsp[0]->is_var = 1;
            yyvsp[0]->vnum   = nvars;
            vars[nvars].name = (char *)emalloc(strlen(yyvsp[0]->name) + 1);
            (void)strcpy(vars[nvars].name, yyvsp[0]->name);
            vars[nvars].type = type_code;
            /* set default fill value.  You can override this with
             * the variable attribute "_FillValue". */
            nc_getfill(type_code, &vars[nvars].fill_value);
            vars[nvars].has_data = 0; /* has no data (yet) */
        }
#line 1380 "mfhdf/ncgen/ncgentab.c"
        break;

        case 32: /* varspec: var $@4 dimspec  */
#line 198 "mfhdf/ncgen/ncgen.y"
        {
            vars[nvars].ndims = nvdims;
            nvars++;
        }
#line 1389 "mfhdf/ncgen/ncgentab.c"
        break;

        case 38: /* vdim: dim  */
#line 212 "mfhdf/ncgen/ncgen.y"
        {
            if (nvdims >= H4_MAX_VAR_DIMS) {
                derror("%s has too many dimensions", vars[nvars].name);
            }
            if (yyvsp[0]->is_dim == 1)
                dimnum = yyvsp[0]->dnum;
            else {
                derror("%s is not declared as a dimension", yyvsp[0]->name);
                dimnum = ndims;
            }
            if (rec_dim != -1 && dimnum == rec_dim && nvdims != 0) {
                derror("unlimited dimension must be first");
            }
            vars[nvars].dims[nvdims] = dimnum;
            nvdims++;
        }
#line 1411 "mfhdf/ncgen/ncgentab.c"
        break;

        case 39: /* $@5: %empty  */
#line 231 "mfhdf/ncgen/ncgen.y"
        {
            valnum  = 0;
            valtype = NC_UNSPECIFIED;
            /* get a large block for attributes, realloc later */
            att_space = emalloc(H4_MAX_NC_ATTSIZE);
            /* make all kinds of pointers point to it */
            char_valp   = (char *)att_space;
            byte_valp   = (char *)att_space;
            short_valp  = (short *)att_space;
            long_valp   = (nclong *)att_space;
            float_valp  = (float *)att_space;
            double_valp = (double *)att_space;
        }
#line 1429 "mfhdf/ncgen/ncgentab.c"
        break;

        case 40: /* attdecl: att $@5 '=' attvallist  */
#line 245 "mfhdf/ncgen/ncgen.y"
        {
            if (natts >= H4_MAX_NC_ATTRS)
                derror("too many attributes");
            atts[natts].var  = varnum;
            atts[natts].type = valtype;
            atts[natts].len  = valnum;
            /* shrink space down to what was really needed */
            att_space       = erealloc(att_space, valnum * nctypelen(valtype));
            atts[natts].val = att_space;
            if (NC_STREQ(atts[natts].name, _FillValue)) {
                nc_putfill(atts[natts].type, atts[natts].val, &vars[atts[natts].var].fill_value);
            }
            natts++;
        }
#line 1450 "mfhdf/ncgen/ncgentab.c"
        break;

        case 42: /* att: ':' attr  */
#line 264 "mfhdf/ncgen/ncgen.y"
        {
            varnum = -1; /* handle of "global" attribute */
        }
#line 1458 "mfhdf/ncgen/ncgentab.c"
        break;

        case 43: /* avar: var  */
#line 270 "mfhdf/ncgen/ncgen.y"
        {
            if (yyvsp[0]->is_var == 1)
                varnum = yyvsp[0]->vnum;
            else {
                derror("%s not declared as a variable, fatal error", yyvsp[0]->name);
                YYABORT;
            }
        }
#line 1471 "mfhdf/ncgen/ncgentab.c"
        break;

        case 44: /* attr: IDENT  */
#line 280 "mfhdf/ncgen/ncgen.y"
        {
            atts[natts].name = (char *)emalloc(strlen(yyvsp[0]->name) + 1);
            (void)strcpy(atts[natts].name, yyvsp[0]->name);
        }
#line 1480 "mfhdf/ncgen/ncgentab.c"
        break;

        case 47: /* aconst: attconst  */
#line 289 "mfhdf/ncgen/ncgen.y"
        {
            if (valtype == NC_UNSPECIFIED)
                valtype = atype_code;
            if (valtype != atype_code)
                derror("values for attribute must be all of same type");
        }
#line 1491 "mfhdf/ncgen/ncgentab.c"
        break;

        case 48: /* attconst: CHAR_CONST  */
#line 298 "mfhdf/ncgen/ncgen.y"
        {
            atype_code   = NC_CHAR;
            *char_valp++ = char_val;
            valnum++;
        }
#line 1501 "mfhdf/ncgen/ncgentab.c"
        break;

        case 49: /* attconst: TERMSTRING  */
#line 304 "mfhdf/ncgen/ncgen.y"
        {
            atype_code = NC_CHAR;
            {
                /* don't null-terminate attribute strings */
                int len = strlen(termstring);
                valnum += len;
                (void)strncpy(char_valp, termstring, len);
                char_valp += len;
            }
        }
#line 1516 "mfhdf/ncgen/ncgentab.c"
        break;

        case 50: /* attconst: BYTE_CONST  */
#line 315 "mfhdf/ncgen/ncgen.y"
        {
            atype_code   = NC_BYTE;
            *byte_valp++ = byte_val;
            valnum++;
        }
#line 1526 "mfhdf/ncgen/ncgentab.c"
        break;

        case 51: /* attconst: SHORT_CONST  */
#line 321 "mfhdf/ncgen/ncgen.y"
        {
            atype_code    = NC_SHORT;
            *short_valp++ = short_val;
            valnum++;
        }
#line 1536 "mfhdf/ncgen/ncgentab.c"
        break;

        case 52: /* attconst: LONG_CONST  */
#line 327 "mfhdf/ncgen/ncgen.y"
        {
            atype_code   = NC_LONG;
            *long_valp++ = long_val;
            valnum++;
        }
#line 1546 "mfhdf/ncgen/ncgentab.c"
        break;

        case 53: /* attconst: FLOAT_CONST  */
#line 333 "mfhdf/ncgen/ncgen.y"
        {
            atype_code    = NC_FLOAT;
            *float_valp++ = float_val;
            valnum++;
        }
#line 1556 "mfhdf/ncgen/ncgentab.c"
        break;

        case 54: /* attconst: DOUBLE_CONST  */
#line 339 "mfhdf/ncgen/ncgen.y"
        {
            atype_code     = NC_DOUBLE;
            *double_valp++ = double_val;
            valnum++;
        }
#line 1566 "mfhdf/ncgen/ncgentab.c"
        break;

        case 59: /* $@6: %empty  */
#line 354 "mfhdf/ncgen/ncgen.y"
        {
            valtype               = vars[varnum].type; /* variable type */
            valnum                = 0;                 /* values accumulated for variable */
            vars[varnum].has_data = 1;
            /* compute dimensions product (size of a "record") */
            var_size = nctypelen(valtype);
            if (vars[varnum].ndims == 0)
                var_len = 1;
            else if (vars[varnum].dims[0] == rec_dim) {
                var_len              = 1; /* one record for unlimited vars */
                netcdf_record_number = 0;
            }
            else
                var_len = dims[vars[varnum].dims[0]].size;
            for (dimnum = 1; dimnum < vars[varnum].ndims; dimnum++)
                var_len = var_len * dims[vars[varnum].dims[dimnum]].size;
            /* allocate memory for a record of variable data */
            if (var_len * var_size != (unsigned)(var_len * var_size)) {
                derror("too much data for this machine");
                exit(9);
            }
            rec_start = malloc((unsigned)(var_len * var_size));
            if (rec_start == 0) {
                derror("out of memory\n");
                exit(3);
            }
            rec_cur = rec_start;
            switch (valtype) {
                case NC_CHAR:
                    char_valp = (char *)rec_start;
                    break;
                case NC_BYTE:
                    byte_valp = (char *)rec_start;
                    break;
                case NC_SHORT:
                    short_valp = (short *)rec_start;
                    break;
                case NC_LONG:
                    long_valp = (nclong *)rec_start;
                    break;
                case NC_FLOAT:
                    float_valp = (float *)rec_start;
                    break;
                case NC_DOUBLE:
                    double_valp = (double *)rec_start;
                    break;
            }
        }
#line 1619 "mfhdf/ncgen/ncgentab.c"
        break;

        case 60: /* datadecl: avar $@6 '=' constlist  */
#line 403 "mfhdf/ncgen/ncgen.y"
        {
            if (valnum > 0 && valnum < var_len) { /* leftovers */
                nc_fill(valtype, var_len - valnum, rec_cur, vars[varnum].fill_value);
                /* put out record of var_len values */
                if (derror_count == 0)
                    put_variable(rec_start);
            }
            free((char *)rec_start);
        }
#line 1636 "mfhdf/ncgen/ncgentab.c"
        break;

        case 63: /* $@7: %empty  */
#line 420 "mfhdf/ncgen/ncgen.y"
        {
            if (valnum >= var_len) {
                derror("too many values for this variable");
                exit(4);
            }
            not_a_string = 1;
        }
#line 1648 "mfhdf/ncgen/ncgentab.c"
        break;

        case 64: /* dconst: $@7 const  */
#line 428 "mfhdf/ncgen/ncgen.y"
        {
            if (not_a_string) {
                switch (valtype) {
                    case NC_CHAR:
                        rec_cur = (void *)char_valp;
                        break;
                    case NC_BYTE:
                        rec_cur = (void *)byte_valp;
                        break;
                    case NC_SHORT:
                        rec_cur = (void *)short_valp;
                        break;
                    case NC_LONG:
                        rec_cur = (void *)long_valp;
                        break;
                    case NC_FLOAT:
                        rec_cur = (void *)float_valp;
                        break;
                    case NC_DOUBLE:
                        rec_cur = (void *)double_valp;
                        break;
                }
            }
            if (valnum >= var_len) {
                /* put out record of var_len elements */
                if (derror_count == 0)
                    put_variable(rec_start);
                /* if this variable is unbounded, reset for */
                /* next record */
                if (vars[varnum].dims[0] == rec_dim) {
                    valnum = 0;
                    netcdf_record_number++;
                    rec_cur = rec_start;
                    switch (valtype) {
                        case NC_CHAR:
                            char_valp = (char *)rec_start;
                            break;
                        case NC_BYTE:
                            byte_valp = (char *)rec_start;
                            break;
                        case NC_SHORT:
                            short_valp = (short *)rec_start;
                            break;
                        case NC_LONG:
                            long_valp = (nclong *)rec_start;
                            break;
                        case NC_FLOAT:
                            float_valp = (float *)rec_start;
                            break;
                        case NC_DOUBLE:
                            double_valp = (double *)rec_start;
                            break;
                    }
                }
            }
        }
#line 1709 "mfhdf/ncgen/ncgentab.c"
        break;

        case 65: /* const: CHAR_CONST  */
#line 487 "mfhdf/ncgen/ncgen.y"
        {
            atype_code = NC_CHAR;
            switch (valtype) {
                case NC_CHAR:
                    *char_valp++ = char_val;
                    break;
                case NC_BYTE:
                    *byte_valp++ = char_val;
                    break;
                case NC_SHORT:
                    *short_valp++ = char_val;
                    break;
                case NC_LONG:
                    *long_valp++ = char_val;
                    break;
                case NC_FLOAT:
                    *float_valp++ = char_val;
                    break;
                case NC_DOUBLE:
                    *double_valp++ = char_val;
                    break;
            }
            valnum++;
        }
#line 1738 "mfhdf/ncgen/ncgentab.c"
        break;

        case 66: /* const: TERMSTRING  */
#line 512 "mfhdf/ncgen/ncgen.y"
        {
            not_a_string = 0;
            atype_code   = NC_CHAR;
            {
                int len = strlen(termstring);

                valnum += len;
                if (valnum > var_len) {
                    derror("string won't fit in this variable");
                    exit(5);
                }
                switch (valtype) {
                    case NC_CHAR:
                        (void)strncpy(char_valp, termstring, len);
                        char_valp += len;
                        rec_cur = (void *)char_valp;
                        break;
                    case NC_BYTE:
                        (void)strncpy(byte_valp, termstring, len);
                        byte_valp += len;
                        rec_cur = (void *)byte_valp;
                        break;
                    case NC_SHORT:
                    case NC_LONG:
                    case NC_FLOAT:
                    case NC_DOUBLE:
                        derror("string value invalid for %s variable", nctype(valtype));
                        break;
                }
            }
        }
#line 1775 "mfhdf/ncgen/ncgentab.c"
        break;

        case 67: /* const: BYTE_CONST  */
#line 545 "mfhdf/ncgen/ncgen.y"
        {
            atype_code = NC_BYTE;
            switch (valtype) {
                case NC_CHAR:
                    *char_valp++ = byte_val;
                    break;
                case NC_BYTE:
                    *byte_valp++ = byte_val;
                    break;
                case NC_SHORT:
                    *short_valp++ = byte_val;
                    break;
                case NC_LONG:
                    *long_valp++ = byte_val;
                    break;
                case NC_FLOAT:
                    *float_valp++ = byte_val;
                    break;
                case NC_DOUBLE:
                    *double_valp++ = byte_val;
                    break;
            }
            valnum++;
        }
#line 1804 "mfhdf/ncgen/ncgentab.c"
        break;

        case 68: /* const: SHORT_CONST  */
#line 570 "mfhdf/ncgen/ncgen.y"
        {
            atype_code = NC_SHORT;
            switch (valtype) {
                case NC_CHAR:
                    *char_valp++ = short_val;
                    break;
                case NC_BYTE:
                    *byte_valp++ = short_val;
                    break;
                case NC_SHORT:
                    *short_valp++ = short_val;
                    break;
                case NC_LONG:
                    *long_valp++ = short_val;
                    break;
                case NC_FLOAT:
                    *float_valp++ = short_val;
                    break;
                case NC_DOUBLE:
                    *double_valp++ = short_val;
                    break;
            }
            valnum++;
        }
#line 1833 "mfhdf/ncgen/ncgentab.c"
        break;

        case 69: /* const: LONG_CONST  */
#line 595 "mfhdf/ncgen/ncgen.y"
        {
            atype_code = NC_LONG;
            switch (valtype) {
                case NC_CHAR:
                    *char_valp++ = long_val;
                    break;
                case NC_BYTE:
                    *byte_valp++ = long_val;
                    break;
                case NC_SHORT:
                    *short_valp++ = long_val;
                    break;
                case NC_LONG:
                    *long_valp++ = long_val;
                    break;
                case NC_FLOAT:
                    *float_valp++ = long_val;
                    break;
                case NC_DOUBLE:
                    *double_valp++ = long_val;
                    break;
            }
            valnum++;
        }
#line 1862 "mfhdf/ncgen/ncgentab.c"
        break;

        case 70: /* const: FLOAT_CONST  */
#line 620 "mfhdf/ncgen/ncgen.y"
        {
            atype_code = NC_FLOAT;
            switch (valtype) {
                case NC_CHAR:
                    *char_valp++ = float_val;
                    break;
                case NC_BYTE:
                    *byte_valp++ = float_val;
                    break;
                case NC_SHORT:
                    *short_valp++ = float_val;
                    break;
                case NC_LONG:
                    *long_valp++ = float_val;
                    break;
                case NC_FLOAT:
                    *float_valp++ = float_val;
                    break;
                case NC_DOUBLE:
                    *double_valp++ = float_val;
                    break;
            }
            valnum++;
        }
#line 1891 "mfhdf/ncgen/ncgentab.c"
        break;

        case 71: /* const: DOUBLE_CONST  */
#line 645 "mfhdf/ncgen/ncgen.y"
        {
            atype_code = NC_DOUBLE;
            switch (valtype) {
                case NC_CHAR:
                    *char_valp++ = double_val;
                    break;
                case NC_BYTE:
                    *byte_valp++ = double_val;
                    break;
                case NC_SHORT:
                    *short_valp++ = double_val;
                    break;
                case NC_LONG:
                    *long_valp++ = double_val;
                    break;
                case NC_FLOAT:
                    if (double_val == FILL_DOUBLE)
                        *float_valp++ = FILL_FLOAT;
                    else
                        *float_valp++ = double_val;
                    break;
                case NC_DOUBLE:
                    *double_valp++ = double_val;
                    break;
            }
            valnum++;
        }
#line 1923 "mfhdf/ncgen/ncgentab.c"
        break;

#line 1927 "mfhdf/ncgen/ncgentab.c"

        default:
            break;
    }
    /* User semantic actions sometimes alter yychar, and that requires
       that yytoken be updated with the new translation.  We take the
       approach of translating immediately before every use of yytoken.
       One alternative is translating here after every semantic action,
       but that translation would be missed if the semantic action invokes
       YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
       if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
       incorrect destructor might then be invoked immediately.  In the
       case of YYERROR or YYBACKUP, subsequent parser actions might lead
       to an incorrect destructor call or verbose syntax error message
       before the lookahead is translated.  */
    YY_SYMBOL_PRINT("-> $$ =", YY_CAST(yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

    YYPOPSTACK(yylen);
    yylen = 0;

    *++yyvsp = yyval;

    /* Now 'shift' the result of the reduction.  Determine what state
       that goes to, based on the state we popped back to and the rule
       number reduced by.  */
    {
        const int yylhs = yyr1[yyn] - YYNTOKENS;
        const int yyi   = yypgoto[yylhs] + *yyssp;
        yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp ? yytable[yyi] : yydefgoto[yylhs]);
    }

    goto yynewstate;

/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
    /* Make sure we have latest lookahead translation.  See comments at
       user semantic actions for why this is necessary.  */
    yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE(yychar);
    /* If not already recovering from an error, report this error.  */
    if (!yyerrstatus) {
        ++yynerrs;
        yyerror(YY_("syntax error"));
    }

    if (yyerrstatus == 3) {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        if (yychar <= YYEOF) {
            /* Return failure if at end of input.  */
            if (yychar == YYEOF)
                YYABORT;
        }
        else {
            yydestruct("Error: discarding", yytoken, &yylval);
            yychar = YYEMPTY;
        }
    }

    /* Else will try to reuse lookahead token after shifting the error
       token.  */
    goto yyerrlab1;

/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and the
       label yyerrorlab therefore never appears in user code.  */
    if (0)
        YYERROR;
    ++yynerrs;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    YYPOPSTACK(yylen);
    yylen = 0;
    YY_STACK_PRINT(yyss, yyssp);
    yystate = *yyssp;
    goto yyerrlab1;

/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
    yyerrstatus = 3; /* Each real token shifted decrements this.  */

    /* Pop stack until we find a state that shifts the error token.  */
    for (;;) {
        yyn = yypact[yystate];
        if (!yypact_value_is_default(yyn)) {
            yyn += YYSYMBOL_YYerror;
            if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror) {
                yyn = yytable[yyn];
                if (0 < yyn)
                    break;
            }
        }

        /* Pop the current state because it cannot handle the error token.  */
        if (yyssp == yyss)
            YYABORT;

        yydestruct("Error: popping", YY_ACCESSING_SYMBOL(yystate), yyvsp);
        YYPOPSTACK(1);
        yystate = *yyssp;
        YY_STACK_PRINT(yyss, yyssp);
    }

    YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
    *++yyvsp = yylval;
    YY_IGNORE_MAYBE_UNINITIALIZED_END

    /* Shift the error token.  */
    YY_SYMBOL_PRINT("Shifting", YY_ACCESSING_SYMBOL(yyn), yyvsp, yylsp);

    yystate = yyn;
    goto yynewstate;

/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
    yyresult = 0;
    goto yyreturnlab;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
    yyresult = 1;
    goto yyreturnlab;

/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
    yyerror(YY_("memory exhausted"));
    yyresult = 2;
    goto yyreturnlab;

/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
    if (yychar != YYEMPTY) {
        /* Make sure we have latest lookahead translation.  See comments at
           user semantic actions for why this is necessary.  */
        yytoken = YYTRANSLATE(yychar);
        yydestruct("Cleanup: discarding lookahead", yytoken, &yylval);
    }
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    YYPOPSTACK(yylen);
    YY_STACK_PRINT(yyss, yyssp);
    while (yyssp != yyss) {
        yydestruct("Cleanup: popping", YY_ACCESSING_SYMBOL(+*yyssp), yyvsp);
        YYPOPSTACK(1);
    }
#ifndef yyoverflow
    if (yyss != yyssa)
        YYSTACK_FREE(yyss);
#endif

    return yyresult;
}

#line 676 "mfhdf/ncgen/ncgen.y"

/* PROGRAMS */

/* get lexical input routine generated by lex  */
#include "ncgenyy.c"

void derror(const char *fmt, ...);

void yyerror(s) /* called for yacc syntax error */
    char *s;
{
    derror(s);
}

#ifndef yywrap
int
yywrap() /* returns 1 on EOF if no more input */
{
    return 1;
}
#endif /* yywrap() */

/* Symbol table operations for ncgen tool */

YYSTYPE
lookup(char *sname) /* find sname in symbol table (linear search) */
{
    YYSTYPE sp;
    for (sp = symlist; sp != (YYSTYPE)0; sp = sp->next)
        if (NC_STREQ(sp->name, sname)) {
            return sp;
        }
    return 0; /* 0 ==> not found */
}

YYSTYPE
install(char *sname) /* install sname in symbol table */
{
    YYSTYPE sp;

    sp       = (YYSTYPE)emalloc(sizeof(struct Symbol));
    sp->name = (char *)emalloc(strlen(sname) + 1); /* +1 for '\0' */
    (void)strcpy(sp->name, sname);
    sp->next   = symlist; /* put at front of list */
    sp->is_dim = 0;
    sp->is_var = 0;
    sp->is_att = 0;
    symlist    = sp;
    return sp;
}

void
clearout() /* reset symbol table to empty */
{
    YYSTYPE sp, tp;
    for (sp = symlist; sp != (YYSTYPE)0;) {
        tp = sp->next;
        free(sp->name);
        free((char *)sp);
        sp = tp;
    }
    symlist = 0;
}

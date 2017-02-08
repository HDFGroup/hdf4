#define SIMPLE_TEST(x) int main(){ x; return 0; }

#ifdef STDC_HEADERS
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <float.h>
int main() { return 0; }
#endif /* STDC_HEADERS */

#ifdef PRINTF_LL_WIDTH

#ifdef HAVE_LONG_LONG
#  define LL_TYPE long long
#else /* HAVE_LONG_LONG */
#  define LL_TYPE __int64
#endif /* HAVE_LONG_LONG */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
  char *llwidthArgs[] = { "I64", "l64", "l", "L", "q", "ll", NULL };
  char *s = malloc(128);
  char **currentArg = NULL;
  LL_TYPE x = (LL_TYPE)1048576 * (LL_TYPE)1048576;
  for (currentArg = llwidthArgs; *currentArg != NULL; currentArg++)
    {
    char formatString[64];
    sprintf(formatString, "%%%sd", *currentArg);
    sprintf(s, formatString, x);
    if (strcmp(s, "1099511627776") == 0)
      {
      printf("PRINTF_LL_WIDTH=[%s]\n", *currentArg);
      return 0;
      }
    }
  return 1;
}

#endif /* PRINTF_LL_WIDTH */

#ifdef TEST_LFS_WORKS
/* Return 0 when LFS is available and 1 otherwise.  */
#define _LARGEFILE_SOURCE
#define _LARGEFILE64_SOURCE
#define _LARGE_FILES
#define _FILE_OFFSET_BITS 64
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <stdio.h>

int main(int argc, char **argv)
{
  /* check that off_t can hold 2^63 - 1 and perform basic operations... */
#define OFF_T_64 (((off_t) 1 << 62) - 1 + ((off_t) 1 << 62))
  if (OFF_T_64 % 2147483647 != 1)
    return 1;

  /* stat breaks on SCO OpenServer */
  struct stat buf;
  stat( argv[0], &buf );
  if (!S_ISREG(buf.st_mode))
    return 2;

  FILE *file = fopen( argv[0], "r" );
  off_t offset = ftello( file );
  fseek( file, offset, SEEK_CUR );
  fclose( file );
  return 0;
}
#endif

#include <stdio.h>

#define MESG(I,...) do { \
    fprintf( stderr, "%4d %8s (%s:%s:%d) ", (I), "[Info]", __FILE__, __func__, __LINE__ ); \
    fprintf( stderr, __VA_ARGS__ ); fprintf( stderr, "\n" ); \
  } while (0)

#define PASS(I,...) do { \
    fprintf( stderr, "%4d %8s (%s:%s:%d) ", (I), "[Ok]", __FILE__, __func__, __LINE__ ); \
    fprintf( stderr, __VA_ARGS__ ); fprintf( stderr, "\n" ); \
  } while (0)

#define FAIL(I,...) do { \
    fprintf( stderr, "%4d %8s (%s:%s:%d) ", (I), "[FAILED]", __FILE__, __func__, __LINE__ ); \
    fprintf( stderr, __VA_ARGS__ ); fprintf( stderr, "\n" ); \
  } while (0)

#undef REGISTER
#define REGISTER(X) X

/* EOF */

#include <stdio.h>

#define MESG(...) do { \
    fprintf( stderr, "%4d %8s (%s:%s:%d) ", id__, "[Info]", __FILE__, __func__, __LINE__ ); \
    fprintf( stderr, __VA_ARGS__ ); fprintf( stderr, "\n" ); \
  } while (0)

#define PASS(...) do { \
    fprintf( stderr, "%4d %8s (%s:%s:%d) ", id__, "[Ok]", __FILE__, __func__, __LINE__ ); \
    fprintf( stderr, __VA_ARGS__ ); fprintf( stderr, "\n" ); \
  } while (0)

#define FAIL(...) do { \
    fprintf( stderr, "%4d %8s (%s:%s:%d) ", id__, "[FAILED]", __FILE__, __func__, __LINE__ ); \
    fprintf( stderr, __VA_ARGS__ ); fprintf( stderr, "\n" ); \
  } while (0)

#undef REGISTER
#define REGISTER(X) int (X)(int id__)

/* EOF */

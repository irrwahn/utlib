#include <stdio.h>

#define MESG(I,...) do { \
    fprintf( stderr, "  #%d (%s:%s:%d) ", (I), __FILE__, __func__, __LINE__ ); \
    fprintf( stderr, __VA_ARGS__ ); fprintf( stderr, "\n" ); \
    } while (0)

#define PASS(I,...) do { \
    fprintf( stderr, "  #%d (%s:%s:%d) ", (I), __FILE__, __func__, __LINE__ ); \
    fprintf( stderr, __VA_ARGS__ ); fprintf( stderr, " [Ok]\n" ); \
    } while (0)

#define FAIL(I,...) do { \
    fprintf( stderr, "  #%d (%s:%s:%d) ", (I), __FILE__, __func__, __LINE__ ); \
    fprintf( stderr, __VA_ARGS__ ); fprintf( stderr, " [FAILED]\n" ); \
    } while (0)

#define REGISTER(X) X

/* EOF */

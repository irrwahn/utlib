#include <stdio.h>

#define PASS(I,M) fprintf( stderr, "#%d Ok: %s.  (%s,%d,%s)\n", (I), (M), __func__, __LINE__, __FILE__ )
#define FAIL(I,M) fprintf( stderr, "#%d Failed: %s.  (%s,%d,%s) \n", (I), (M), __func__, __LINE__, __FILE__ )

#define REGISTER(X) X

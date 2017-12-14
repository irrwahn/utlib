#include <stdio.h>

typedef int (*testf_t)(int);

#undef REGISTER
#define REGISTER(X)     extern int X( int );
#include "tests.h"
#undef REGISTER

static const testf_t testf[] = {
#define REGISTER(X)     X,
#include "tests.h"
#undef REGISTER
    NULL
};

/* EOF */

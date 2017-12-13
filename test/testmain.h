#include <stdio.h>

typedef int (*testf_t)(int);


#undef REGISTER
#define REGISTER(X)     extern int (X)( int );
    #include "tests.h"

#undef REGISTER
#define REGISTER(X)     X,
    static  testf_t testf[] = {
        #include "tests.h"
        NULL
    };
#undef REGISTER

/* EOF */

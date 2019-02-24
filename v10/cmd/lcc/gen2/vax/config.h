#include "gen.h"

#ifdef vax
#ifndef V9
#define strtod(a,b) atof(a)
dclproto(extern double atof,(char *));
#endif
#endif

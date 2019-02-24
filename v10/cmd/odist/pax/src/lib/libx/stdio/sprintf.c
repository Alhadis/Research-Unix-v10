#include "sfhdr.h"
#include "stdio.h"

#undef	sprintf
#define _stdsprintf	sprintf

#include "stdsprintf.c"

#include "sfhdr.h"
#include "stdio.h"

#undef	vsprintf
#define _stdvsprintf	vsprintf

#include "stdvsprintf.c"

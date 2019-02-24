#include "complex.h"

void complex_error(int err, double)
{
	errno = err;
}

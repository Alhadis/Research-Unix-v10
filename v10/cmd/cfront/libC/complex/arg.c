
#include "complex.h"

double	arg(complex z)
{
	return atan2(z.im,z.re);
}

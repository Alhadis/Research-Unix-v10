/* floating-point mod function without infinity or NaN checking */
#include <math.h>
#include <errno.h>

extern int errno;

double
fmod (x, y)
	double x, y;
{
	int sign = 0, yexp;
	double r, yfr, ldexp(), frexp();

	if (y == 0)
		return x;

	if (y < 0)
		y = -y;

	yfr = frexp (y, &yexp);

	if (x < 0) {
		sign = 1;
		r = -x;
	} else
		r = x;

	while (r >= y) {
		int rexp;
		double rfr = frexp (r, &rexp);
		r -= ldexp (y, rexp - yexp - (rfr < yfr));
	}

	if (sign)
		r = -r;
	return r;
}

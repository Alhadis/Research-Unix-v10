double
fmod(x, y)
double x, y;
{
	extern double modf();
	double d;

	if (y == 0.0)
		return (x);
#ifndef	u3b
#ifndef	pdp11
	(void)
	/* This MUST be fixed for 5.0!  It is intended to be very temporary! */
#endif
#endif
	modf(x/y, &d);
	return (x - d * y);
}

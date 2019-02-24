static	double	p[10] =
{
	1.0e0,
	1.0e1,
	1.0e2,
	1.0e3,
	1.0e4,
	1.0e5,
	1.0e6,
	1.0e7,
	1.0e8,
	1.0e9,
};

double
pow10(e)
{
	int i;

	if(e < 0)
		return 1 / pow10(-e);
	if(e >= 10) {
		i = e/2;
		return pow10(i) * pow10(e-i);
	}
	return p[e];
}

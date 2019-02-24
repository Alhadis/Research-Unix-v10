#include <math.h>
double ipow();

main()
{
#if mips || sun
	flt(2,  -125,  128, 24, 0x34000000, 0x7f7fffff, 0x00800000);
	dbl(2, -1021, 1024, 53, 0x3cb00000, 0x00000000, 0x7fefffff, 0xffffffff, 0x00100000, 0x00000000);
#endif
#ifdef vax
	flt(2,  -127,  127, 24, 0x00003500, 0xffff7fff, 0x00000080);
	dbl(2,  -127,  127, 56, 0x00002500, 0x00000000, 0xffff7fff, 0xffffffff, 0x00000080, 0x00000000);
#endif
}

dbl(b, emin, emax, p, eps, max, min)
double eps, max, min;
{
	printf("#define DBL_DIG		%g\n", floor((p-1)*log10((double)b)+ispower10(b)));
	printf("#define DBL_EPSILON	%.20e\n", eps);
	printf("#define DBL_EPSILON	%.20e\n", ipow(b, 1-p));
	printf("#define DBL_MANT_DIG	%d\n", p);
	printf("#define DBL_MAX		%.20e\n", (1-ipow(b, -p))*ipow(b, emax-1)*b);
	printf("#define DBL_MAX		%.20e\n", max);
	printf("#define DBL_MAX_10_EXP	%g\n", floor(log10((1-ipow(b, -p))*ipow(b, emax-1)*b)));
	printf("#define DBL_MAX_EXP	%d\n", emax);
	printf("#define DBL_MIN		%.20e\n", ipow(b, emin-1));
	printf("#define DBL_MIN		%.20e\n", min);
	printf("#define DBL_MIN_10_EXP	%g\n", ceil(log10(ipow(b, emin-1))));
	printf("#define DBL_MIN_EXP	%d\n", emin);
	printf("\n");
}

flt(b, emin, emax, p, eps, max, min)
{
	printf("#define FLT_DIG		%g\n", floor((p-1)*log10((double)b)+ispower10(b)));
	printf("#define FLT_EPSILON	%.20e\n", *(float*)&eps);
	printf("#define FLT_EPSILON	%.20e\n", ipow(b, 1-p));
	printf("#define FLT_MANT_DIG	%d\n", p);
	printf("#define FLT_MAX		%.20e\n", (1-ipow(b, -p))*ipow(b, emax-1)*b);
	printf("#define FLT_MAX		%.20e\n", *(float*)&max);
	printf("#define FLT_MAX_10_EXP	%g\n", floor(log10((1-ipow(b, -p))*ipow(b, emax-1)*b)));
	printf("#define FLT_MAX_EXP	%d\n", emax);
	printf("#define FLT_MIN		%.20e\n", *(float*)&min);
	printf("#define FLT_MIN		%.20e\n", ipow(b, emin-1));
	printf("#define FLT_MIN_10_EXP	%g\n", ceil(log10(ipow(b, emin-1))));
	printf("#define FLT_MIN_EXP	%d\n", emin);
	printf("\n");
}

double ipow(x, n)
{
	double ac;

	if (n >= 0)
		for (ac = 1; n > 0; n--)
			ac *= x;
	else
		for (ac = 1; n < 0; n++)
			ac /= x;
	return ac;
}

ispower10(x)
{
	double p;

	for (p = 1; p < x; p *= 10)
		;
	return p == x;
}

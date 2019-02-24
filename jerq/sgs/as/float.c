static char *ID_float = "@(#) float.c: 1.3 6/21/83";

#include "systems.h"

#if	FLOAT

#include <ctype.h>
#include <symbols.h>	/* needed for instab.h */
#include "instab.h"
#define SINGLEBIAS 127
#define DOUBLEBIAS 1023

extern double
	lclatof(),
	floor(),
	pow(),
	log2();

short
ato3bf(string,lp)
char *string;
rexpr *lp;
{
	double	dbl, fexp;
	double	two16 = 65536.;
	long	sbit, exponent, mantissa;
	short	leadbit = 0;

/*
 *	Some of the cast operations, namely
 *	double -> float and double -> long  don't necessarily
 *	have the same implementation on various machines
 *	that is why the following two variables are defined.
 *	Any time there are used in this file it is to get
 *	around the cast operator.
 */
	double	dtmp;		/*  dummy var needed for casting */
	float	ftmp;		/*  dummy var needed for casting */
	long	i, nf;

	dbl = lclatof(string);
#if	DEBUG
printf("\nato3bs: string= %s   atof value= %.13e\n",string,dbl);
#endif
	if (dbl == 0.0) {
		lp->expval = 0x0L;
		lp->fdexpval2 = 0x0L;
		return(0);
	}
	if (dbl > 0.0)
		sbit = 0L;
	else {
		sbit = 1L;
		dbl *= -1.0;
	}
/*
 *	Find out how many two's are in the floating point number ('dbl')
 *	then divide 'dbl' by 2**(number of 2's) (by multiplying
 *	by its reciprical) in order to scale it down within the range
 *	1.0 =< dbl =< 2.0
 *	Note: Due to rounding and imprecise results the resulting
 *	number may be slightly < 1.0 or slightly > 2.0
 *	which is why we must check for these cases.
 */
	fexp = floor(log2(dbl));
/*
 * The following line of code is commented out due to some problems
 * with the precision of the pow function. It is replaced by the
 * following while loop and if stmt.
 *
	dbl *= pow((double)2.0,-fexp);
 */
	i = (long)fexp;
	nf = 0;
	if (i < 0) { nf = 1; i = -i; }
	dtmp = (double)1.0;
	while (i-- > 0L) dtmp *= (double)2.0;
	if (nf == 0 )
		dbl *= ((double)1.0 / dtmp);
	else
		dbl *= dtmp;

	if (dbl >= 2.0) {
		dbl /= (double)2.0;
		++fexp;
	}
	else if (dbl < 1.0) {
		dbl *= (double)2.0;
		--fexp;
	}
#if	DEBUG
printf("ato3bs: fexp= %f   dbl= %.13e   %lx %lx\n",fexp,dbl,dbl);
#endif

	if (fexp > -(DOUBLEBIAS-1))  /* normalized numb */
		exponent = (long)fexp + (long)DOUBLEBIAS;
	else {		/* denormalized numb, must adjust mantissa
			 * e.g. a number like 1.0E-1025 will be coded
			 *	as 0.001E-1022 with some precision lost
			 */
		exponent = 0L;
/*
		fexp += (double)(DOUBLEBIAS-1);
		dbl *= pow((double)2.0, fexp);
*/
#if	DEBUG
printf("ato3bs: denorm numb - dbl= %.13e  fexp= %f\n",dbl,fexp);
#endif
	}
	if (exponent != 0L) leadbit = 1;
#if	DEBUG
printf("ato3bs: leadbit= %d\n", leadbit);
#endif

/*
 *	Multiply by 2**20 to get the first 20 bits of the fraction
 *	necessary to fill in the first word.
 *	Then subtract out this number and multiply by 2**32 to
 *	get the remaining fractional part for the second word of the double
 */

	dtmp = (dbl - (double)leadbit) * (double)(1L<<20); /* mult by 2**20 */
	ftmp = floor(dtmp);		/* takes the place of a cast */
	mantissa = ftmp;	/* takes the place of a cast */
	lp->expval = (sbit << 31) | (exponent << 20) | mantissa;
#if	DEBUG
printf("ato3bs: sign, (exp<<20), mantissa= %ld  %lx  %lx\n",
		sbit, (exponent<<20), mantissa);
printf("ato3bs: dtmp,ftmp,mantissa= %.13e  %.13e  %ld\n",dtmp,ftmp,mantissa);
#endif
	dtmp -= mantissa;
	dtmp *= two16;
	ftmp = floor(dtmp);		/* takes the place of a cast */
	lp->fdexpval2 = ftmp;
	dtmp -= lp->fdexpval2;
	dtmp *= two16;
	ftmp = dtmp;		/* takes the place of a cast */
	mantissa = ftmp;	/* takes the place of a cast */
	lp->fdexpval2 = (lp->fdexpval2<<16) | mantissa;
#if	DEBUG
printf("ato3bs: result (val,val2)= %08lx  %08lx\n",lp->expval,lp->fdexpval2);
#endif
	return(0);
} /* ato3bf */

double
lclatof(p)
register char *p;
{
	register char c;
	double fl, flexp, exp5;
/*	'big' is (2 ^ number of significant bits in a fp number) */
	double big = 4503599627370496.;		/* 2^52 */
	double ldexp();
	register int eexp, exp, bexp;
	register short	neg, negexp ;

	neg = 1;
	while((c = *p++) == ' ')
		;
	if (c == '-')
		neg = -1;
	else if (c=='+')
		;
	else
		--p;

	exp = 0;
	fl = 0;
	while ((c = *p++), isdigit(c)) {
		if (fl<big)
			fl = 10*fl + (c-'0');
		else
			exp++;
	}

	if (c == '.') {
		while ((c = *p++), isdigit(c)) {
			if (fl<big) {
				fl = 10*fl + (c-'0');
				exp--;
			}
		}
	}

	negexp = 1;
	eexp = 0;
	if ((c == 'E') || (c == 'e')) {
		if ((c= *p++) == '+')
			;
		else if (c=='-')
			negexp = -1;
		else
			--p;

		while ((c = *p++), isdigit(c)) {
			eexp = 10*eexp+(c-'0');
		}
		if (negexp<0)
			eexp = -eexp;
		exp = exp + eexp;
	}

	negexp = 1;
	if (exp<0) {
		negexp = -1;
		exp = -exp;
	}

	flexp = 1;
	exp5 = 5;
	bexp = exp;
	while (1) {
		if (exp&01)
			flexp *= exp5;
		exp >>= 1;
		if (exp == 0)
			break;
		exp5 *= exp5;
	}
	if (negexp < 0)
		fl /= flexp;
	else
		fl *= flexp;
	fl = ldexp(fl, negexp*bexp);
	return((neg<0) ? -fl : fl);
} /* lclatof */

extern double log();

double
log2(x)
double x;
{
	return( log(x) / log((double)2.0) );
} /* log2 */

short
validfp(p)
register char *p;
{
	register int c;
	char	integer, decimal, frac, e, exp;

	/* check for sign */
	if ((*p == '-') || (*p == '+'))
		++p;

	/* check for integral part */
	integer = 0;
	while ((c = *p++), isdigit(c))
		integer = 1;

	/* check for decimal pt. */
	decimal = 0;
	if (c == '.') {
		decimal = 1;
		/* check for fraction */
		frac = 0;
		while ((c = *p++), isdigit(c))
			frac = 1;
	}

	/* check for exponent */
	e = 0;
	if ((c == 'E') || (c == 'e')) {
		e = 1;
		if ((*p == '-') || (*p == '+'))
			++p;
		exp = 0;
		while ((c = *p++), isdigit(c))
			exp = 1;
	}

	/*
	 * are all the necessary parts here?
	 * invalid fp numbers look like:
	 *	[digits]	(no decimal)
	 *	.		(no digits)
	 *	.[eE]
	 *	.[eE][digits]
	 *
	 *	The ENTIRE string must be valid so 'c' should = NULL
	 *
	 * All else is assumed valid
	 */
	if ((decimal && !integer && !frac) ||
		   (integer && !decimal && !e) ||
		   (e && !exp) || (c != '\0') )
		return(0);	/* bad fp number */
	else
		return(1);	/* valid fp number */
} /* validfp */

/*
 *
 *	round double fp number to single by:
 *		- get high 3 bits of second word, these will go into
 *			new single word since the exponent shrinks to 8 bits
 *		- get guard bits (next 2 bits after high 3 bits)
 *		- if both guard bits equal 1 then add 1 to hi3bits
 *			note: if this add produces a carry it should
 *				promulgate into the mantissa however for the
 *				first cut we won't promulgate it.
 *
 */
fpdtos(fpval)
register rexpr	*fpval;
{
	long	sign, exp, mantissa, hi3bits, guard;

	sign = fpval->expval & (1L<<31);
	exp = (fpval->expval>>20) & 0x7ffL;
	if (exp != 0L)
		exp = (exp - (long)DOUBLEBIAS) + (long)SINGLEBIAS;
#if	DEBUG
printf("fpdtos: sign, old,new exp= %lx   %lx   %lx\n",sign,
		((fpval->expval>>20) & 0x7ffL), exp);
#endif

	mantissa = fpval->expval & 0xfffffL;
	hi3bits = (fpval->fdexpval2>>29) & 0x7L;
	guard = (fpval->fdexpval2>>27) & 0x3L;
	if (guard & 0x3L) {	/* round up */
		if (!(hi3bits & 0x7L))	/* add will not produce a carry */
			hi3bits += 1;
	}
#if	DEBUG
printf("fpdtos: hi3bits,guard= %lx  %lx\n",hi3bits,guard);
printf("fpdtos: old, new man= %lx    %lx\n",mantissa,(mantissa<<3) | hi3bits);
#endif
	mantissa = (mantissa<<3) | hi3bits;

	fpval->expval = 0L;
	fpval->expval = sign | (exp<<23) | mantissa;
#if	DEBUG
printf("fpdtos: new expval= %lx\n",fpval->expval);
#endif
} /* fpdtos */
#endif

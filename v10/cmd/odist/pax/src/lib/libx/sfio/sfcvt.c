#include	"sfhdr.h"

/*	Convert a floating point value to ASCII
**	This function unifies fcvt() and ecvt() in libc.a.
**
**	Written by Kiem-Phong Vo (06/27/90)
*/

#define INTPART		(IDIGITS/2)
#define INFINITE	(_Sfi = 3, "Inf")
#define ZERO		(_Sfi = 1, "0")

#if __STDC__ || __cplusplus
char *_sfcvt(reg double dval, int n_digit, int* decpt, int* sign, int e_format)
#else
char *_sfcvt(dval,n_digit,decpt,sign,e_format)
reg double	dval;		/* value to convert */
int		n_digit;	/* number of digits wanted */
int		*decpt;		/* to return decimal point */
int		*sign;		/* to return sign */
int		e_format;	/* doing e-format */
#endif
{
#ifdef NATIVE	/* NATIVE *cvt() can do this faster than we can */
	reg char	*sp;
	extern char	*fcvt(), *ecvt();
	sp = e_format ? ecvt(dval,n_digit,decpt,sign) : fcvt(dval,n_digit,decpt,sign);
	_Sfi = strlen(sp);
	return sp;
#else
	reg long	n, v, w;
	reg char	*sp, *ep, *buf, *endsp;
	static char	*Buf;

	/* set up local buffer */
	if(!Buf && !(Buf = malloc(MAXDIGITS)))
		return INFINITE;

	*sign = *decpt = 0;
	if(dval == 0.)
		return ZERO;
	else if(*sign = (dval < 0.))	/* assignment = */
		dval = -dval;

	n = 0;
	if(dval >= (double)MAXLONG)
	{	/* scale to a small enough number to fit an int */
		v = MAXEXP10-1;
		do
		{	if(dval < _Sfpos10[v])
				v -= 1;
			else
			{
				dval *= _Sfneg10[v];
				if((n += (1<<v)) >= IDIGITS)
					return INFINITE;
			}
		} while(dval >= (double)MAXLONG);
	}
	else if(dval < 1e-1)
	{	/* scale to get rid of leading zeros */
		v = MAXEXP10-1;
		do
		{	if(dval > _Sfneg10[v])
				v -= 1;
			else
			{
				dval *= _Sfpos10[v];
				if((n -= (1<<v)) <= -IDIGITS)
					return ZERO;
			}
		} while(dval < 1e-1);
	}
	*decpt = (int)n;

	buf = sp = Buf+INTPART;
	if((v = (int)dval) != 0)
	{	/* translate the integer part */
		dval -= (double)v;

		sfucvt(v,sp,n,w,ep);

		n = buf-sp;
		if((*decpt += (int)n) >= IDIGITS)
			return INFINITE;
		buf = sp;
		sp = Buf+INTPART;
	}
	else	n = 0;

	/* remaining number of digits to compute; add 1 for later rounding */
	n = ((e_format || *decpt <= 0) ? 1 : *decpt+1) - n;
	if(n_digit > 0)
		n += n_digit;

	if((ep = (sp+n)) > (endsp = Buf+(MAXDIGITS-2)))
		ep = endsp; 
	if(sp > ep)
		sp = ep;
	else while(sp < ep)
	{	/* generate fractional digits */
		if(dval <= 0.)
		{	/* fill with 0's */
			do { *sp++ = '0'; } while(sp < ep);
			goto done;
		}
		*sp++ = (char)('0' + (n = (int)(dval *= 10.)));
		dval -= (double)n;
	}

	if(ep <= buf)
		ep = buf+1;
	else if(ep < endsp)
	{	/* round the last digit */
		*--sp += 5;
		while(*sp > '9')
		{
			*sp = '0';
			if(sp > buf)
				*--sp += 1;
			else
			{	/* next power of 10 */
				*sp = '1';
				*decpt += 1;
				if(!e_format)
				{	/* add one more 0 for %f precision */
					ep[-1] = '0';
					ep += 1;
				}
			}
		}
	}

done:
	*--ep = '\0';
	_Sfi = ep-buf;
	return buf;

#endif /*NATIVE*/
}

#include	"sfhdr.h"

/*	The engine for formatting data
**
**	Written by Kiem-Phong Vo (06/27/90)
*/

#define HIGHBIT		(~(((ulong)~0L) >> 1))

#define F_LEFT		00001	/* left justification (-) */
#define F_SIGN		00002	/* must set a sign - or + */
#define F_BLANK		00004	/* if not - and +, then prepend a blank */
#define F_ZERO		00010	/* zero padding on the left side */
#define F_ALTER		00020	/* various alternative formats (#) */
#define F_PAD		00040	/* some kind of padding is needed */

#define F_REPEAT	00100	/* repeat the pattern until precision is exhausted */
#define F_0X		00200	/* must add a zero for prefix of hex format */
#define F_MINUS		00400	/* has a minus sign */
#define F_PREFIX	(F_MINUS|F_SIGN|F_BLANK|F_0X)

#define F_LONG		01000	/* object is long */
#define F_FLOAT		02000	/* %fFeEgG format */
#define F_GFORMAT	04000	/* a %gG format */

#define FPRECIS		6	/* default precision for floats */

#define GETARG(elt,arge,argf,args,type,fmt) \
	{ if(!argf) \
		elt = va_arg(args,type); \
	  else if((*argf)(fmt,(char*)(&arge)) < 0) \
		goto pop_fa; \
	  else	elt = arge; \
	}

#if __STDC__ || __cplusplus
sfvprintf(Sfile_t* f, const char* form, va_list args)
#else
sfvprintf(f,form,args)
Sfile_t	*f;		/* file to print to */
char	*form;		/* format to use */
va_list	args;		/* arg list if argf == 0 */
#endif
{
	reg long	n, lval;
	reg char	*sp, *ssp, *d;
	reg long	v;
	reg int		flags;
	reg char	*ep, *endep, *endsp, *endd;
	reg int		precis, width, n_output, r;
	int		fmt, sign, decpt;
	double		dval;
	long		al;
	int		ai, *ap;
	char		*as;
	Argf_p		argf;
	Extf_p		extf;
	va_list		alist;
	reg Fa_t	*fa, *fast;
	char		buf[MAXDIGITS];
	char		data[SF_GRAIN];

	/* fast io system */
#define SFflush(f,c)	(SFopen(f), r = _sfflsbuf(f,(int)c), SFLOCK(f), r)
#define SFwrite(f,s,n)	(SFopen(f), r = sfwrite(f,(uchar*)s,(int)n), SFLOCK(f), r)
#define SFnputc(f,c,n)	(SFopen(f), r = sfnputc(f,(uchar)c,(int)n), SFLOCK(f), r)
#define SFSET(f)	(d = (char*)f->next, endd = (char*)f->endb)
#define SFINIT(f)	(SFSET(f), n_output = 0)
#define SFEND(f)	((n_output += (uchar*)d - f->next), (f->next = (uchar*)d))
#define SFPUTC(f,c) \
	{ if(d >= endd) \
		{ SFEND(f); if(SFflush(f,c) <  0) break; n_output += 1; SFSET(f); } \
	  else	{ *d++ = (char)c; } \
	}
#define SFNPUTC(f,c,n) \
	{ if((endd-d) < n) \
		{ SFEND(f); if(SFnputc(f,c,n) != n) break; n_output += (int)n; SFSET(f); } \
	  else	{ while(n--) *d++ = (char)c; } \
	}
#define SFWRITE(f,s,n) \
	{ if((endd-d) < n) \
		{ SFEND(f); if(SFwrite(f,s,n) != n) break; n_output += (int)n; SFSET(f); } \
	  else	MEMCPY(d,s,(int)n); \
	}

	/* make sure stream is in write mode and buffer is not NULL */
	if(f->mode != SF_WRITE && SFMODE(f,SF_WRITE) < 0)
		return -1;

	SFLOCK(f);

	if(f->data == NIL(uchar*))
	{	f->data = f->next = (uchar*)data;
		f->endw = f->endb = f->data+sizeof(data);
	}
	SFINIT(f);

	argf = NIL(Argf_p);
	extf = NIL(Extf_p);
	fast = NIL(Fa_t*);

loop_fa :
	while(n = *form++)
	{
		flags = 0;
		if(n != '%')
		{	/* collect the non-pattern chars */
			sp = ssp = (char*)(form-1);
			while((n = *++ssp) && n != '%')
				;
			form = endsp = ssp;
			goto do_output;
		}

		endep = ep = NIL(char*);
		endsp = sp = buf+(sizeof(buf)-1);
		width = precis = -1;

	loop_flags:	/* LOOP FOR FLAGS, WIDTH AND PRECISION */
		switch(fmt = *form++)
		{
		case '-' :
			flags |= F_LEFT;
			goto loop_flags;
		case ' ' :
			flags |= F_BLANK;
			goto loop_flags;
		case '+' :
			flags |= F_SIGN;
			goto loop_flags;
		case '#' :
			flags |= F_ALTER;
			goto loop_flags;
		case '.' :	/* done with width */
			if(width < 0)
				width = 0;
			goto loop_flags;
		case '*' :	/* variable width or precision */
			GETARG(lval,ai,argf,args,int,'d');
			if(width >= 0)
				precis = (int)lval;
			else if((width = (int)lval) < 0)
			{	/* left justification */
				width = (int)(-lval);
				flags |= F_LEFT;
			}
			flags |= F_PAD;
			goto loop_flags;
		case '0' :	/* defining width or precision */
			if(width < 0)
				flags |= F_ZERO;
			/* fall thru */
		case '1' : case '2' : case '3' :
		case '4' : case '5' : case '6' :
		case '7' : case '8' : case '9' :
			lval = fmt - '0';
			for(n = *form; isdigit(n); n = *++form)
				lval = lval*10 + (n - '0');
			if(width < 0)
				width = (int)lval;
			else	precis = (int)lval;
			flags |= F_PAD;
			goto loop_flags;
		case 'l' :
			flags |= F_LONG;
		case 'h' : /* no short */
			goto loop_flags;

			/* PRINTF DIRECTIVES */

		case '&' : /* change extension function */
			if(!argf)
				extf = va_arg(args,Extf_p);
			else if((*argf)('&',(char*)(&extf)) < 0)
				goto pop_fa;
			continue;
		case '@' : /* change argument getting function */
			if(!argf)
				argf = va_arg(args,Argf_p);
			else if((*argf)('@',(char*)(&argf)) < 0)
				goto pop_fa;
			continue;
		case ':' : /* stack a pair of format/arglist */
			if(!FAMALLOC(fa))
				goto done;
			fa->form = (char*)form;
			GETARG(form,form,argf,args,char*,'1');
			if(!form)
				form = "";
			GETARG(alist,alist,argf,args,va_list,'2');
			fa->args = args;
			fa->argf.p = argf;
			fa->extf.p = extf;
			args = alist;
			fa->next = fast;
			fast = fa;
			continue;

		default :	/* unknown directive */
			if(extf)
			{	/* call the extension function */
				GETARG(sp,as,argf,args,char*,fmt);
				n = (*extf)(sp,fmt,precis,(char*)(&as));
				if((sp = as) != NIL(char*))
					goto s_format;
			}

			/* treat as text */
			form -= 1;
			continue;

		case 's':	/* a string */
			GETARG(sp,as,argf,args,char*,'s');
			n = -1;
			if(!sp)
			{	/* standard error string for null pointer */
				endsp = (sp = "(null)") + 6;
				flags = 0;
			}
			else
			{	/* set other bound */
			s_format:
				if(n < 0)
				{	ssp = sp-1;
					while(*++ssp)
						;
					n = ssp-sp;
				}
				if(precis >= 0 && precis < n)
					n = precis;
				endsp = sp+n;
			}
			flags &= ~(F_SIGN|F_BLANK|F_ALTER);
			precis = 0;
			break;

		case 'n':	/* return current output length */
			GETARG(ap,ap,argf,args,int*,'n');
			SFEND(f);
			*ap = n_output;
			continue;
		case 'c':	/* a character */
			GETARG(fmt,fmt,argf,args,int,'d');
		case '%' :
			flags = (flags&~(F_SIGN|F_BLANK|F_ZERO))|F_REPEAT;
			if(precis <= 0)
				precis = 1;
			break;
		case 'i':	/* signed integer */
		case 'd':
			if(flags&F_LONG)
				{ GETARG(lval,al,argf,args,long,'D'); }
			else	{ GETARG(lval,ai,argf,args,int,'d');  }
			if(lval < 0)
			{
				flags |= F_MINUS;
				if(lval == HIGHBIT)
				{	/* avoid overflow */
					lval = ((ulong)HIGHBIT)/10;
					*--sp = (char)('0' +
						       ((ulong)HIGHBIT -
							(((ulong)HIGHBIT)/10)*10));
				}
				else	lval = -lval;
			}
			goto base_10;
		case 'p':	/* pointer value */
			GETARG(ssp,as,argf,args,char*,'p');
			lval = (long)ssp;
			flags = (flags&~(F_SIGN|F_BLANK))|F_ALTER;
			fmt = 'x';
			goto base_16;
		case 'u':	/* unsigned value */
		case 'o':
		case 'x':
		case 'X':
			if(flags&F_LONG)
				{ GETARG(lval,al,argf,args,long,'D'); }
			else	{ GETARG(lval,ai,argf,args,int,'d');  }
			flags &= ~(F_SIGN|F_BLANK);
			if(fmt == 'o')
			{	/* base 8 conversion */
				do	{ *--sp = (char)('0' + (lval&07)); }
				while(lval = (ulong)lval >> 3);
				if((flags&F_ALTER) && *sp != '0')
					*--sp = '0';
			}
			else if(fmt == 'u')
			{	/* base 10 unsigned conversion */
			base_10:
				sfucvt(lval,sp,n,v,ssp);
			}
			else
			{	/* base 16 conversion */
			base_16:
				if((flags&F_ALTER) && lval)
					flags |= F_0X;
				ssp = fmt == 'X' ? "0123456789ABCDEF":"0123456789abcdef";
				do
				{	 *--sp = ssp[lval&017];
				} while(lval = ((ulong)lval) >> 4);
			}
			precis -= (endsp-sp);
			break;
		case 'g':	/* ultimately becomes %e or %f */
		case 'G':
			GETARG(dval,dval,argf,args,double,'F');
			precis = precis < 0 ? FPRECIS : precis == 0 ? 1 : precis;
			ep = sfecvt(dval,min(precis,FDIGITS),&decpt,&sign);
			if(dval == 0.)
				decpt = 1;
			else if(*ep == 'I')
				goto infinite;

			if(!(flags&F_ALTER))
			{	/* zap trailing 0s */
				if((n = sfslen()) > precis)
					n = precis;
				while((n -= 1) >= 1 && ep[n] == '0')
					;
				n += 1;
			}
			else	n = precis;

			flags = (flags & ~F_ZERO) | F_GFORMAT;
			if(decpt < -3 || decpt > precis)
			{
				precis = (int)(n-1);
				goto e_format;
			}
			else
			{
				precis = (int)(n - decpt);
				goto f_format;
			}
		case 'e':	/* double value in scientific notation */
		case 'E':
			/* convert data into ascii */
			GETARG(dval,dval,argf,args,double,'F');
			n = (precis = precis < 0 ? FPRECIS : precis)+1;
			ep = sfecvt(dval,(int)min(n,FDIGITS),&decpt,&sign);

		e_format: /* now build the x.yyyy string */
			if(isalpha(*ep))
				goto infinite;
			sp = endsp = buf+1;	/* reserve space for sign */
			*endsp++ = *ep ? *ep++ : '0';

			if(precis > 0 || (flags&F_ALTER))
				*endsp++ = '.';
			ssp = endsp;
			endep = ep+precis;
			while((*endsp++ = *ep++) && ep <= endep)
				;
			precis -= (endsp -= 1) - ssp;

			/* build the exponent */
			ep = endep = buf+(sizeof(buf)-1);
			if(dval != 0.)
			{
				if((n = decpt - 1) < 0)
					n = -n;
				while(n > 9)
				{	lval = n; n /= 10;	
					*--ep = (char)('0' + (lval - n*10));
				}
			}
			else	n = 0;
			*--ep = (char)('0' + n);
			if(endep-ep <= 1)	/* at least 2 digits */
				*--ep = '0';

			/* the e/Exponent separator and sign */
			*--ep = (decpt > 0 || dval == 0.) ? '+' : '-';
			*--ep = isupper(fmt) ? 'E' : 'e';

			flags = (flags&~F_ZERO)|F_FLOAT;
			goto end_efg;
		case 'f':	/* float or double in xxx.yyy notation */
		case 'F':
			GETARG(dval,dval,argf,args,double,'F');
			precis = precis < 0 ? FPRECIS : precis;
			ep = sffcvt(dval,min(precis,FDIGITS),&decpt,&sign);

		f_format: /* data before the '.' */
			if(isalpha(*ep))
			{
			infinite:
				endsp = (sp = ep)+sfslen();
				ep = endep;
				precis = 0;
				goto end_efg;
			}

			endsp = sp = buf+1;	/* save a space for sign */
			endep = ep+decpt;
			while(ep < endep && (*endsp++ = *ep++))
				;
			if(endsp == sp)
				*endsp++ = '0';

			if(precis > 0 || (flags&F_ALTER))
				*endsp++ = '.';

			if((n = -decpt) > 0)
			{	/* output zeros for negative exponent */
				ssp = endsp + min(n,precis);
				precis -= (int)n;
				while(endsp < ssp)
					*endsp++ = '0';
			}

			ssp = endsp;
			endep = ep+precis;
			while((*endsp++ = *ep++) && ep <= endep)
				;
			precis -= (endsp -= 1) - ssp;
			ep = endep;
			flags |= F_FLOAT;
		end_efg:
			if(sign)
			{	/* if a %gG, output the sign now */
				if(flags&F_GFORMAT)
					*--sp = '-';
				else	flags |= F_MINUS;
			}
			break;
		}

		if(!flags)
			goto do_output;
		else if(flags&(F_MINUS|F_SIGN|F_BLANK))
			fmt = (flags&F_MINUS) ? '-' : (flags&F_SIGN) ? '+' : ' ';

		n = (endsp-sp) + (endep-ep) + (precis <= 0 ? 0 : precis) +
		    ((flags&F_0X) ? 2 : (flags&F_PREFIX) ? 1 : 0);
		if((lval = width-n) > 0)
		{	/* check for padding */
			if(!(flags&F_ZERO))
			{	/* right padding */
				if(flags&F_LEFT)
					lval = -lval;
				else if(flags&F_PREFIX)
				{	/* blank padding, output prefix now */
					*--sp = fmt;
					if((flags&F_0X))
						*--sp = '0';
					flags &= ~F_PREFIX;
				}
			}
		}
		else	lval = 0;

		if(flags&F_PREFIX)
		{	/* output prefix */
			if(flags&F_0X)
				SFPUTC(f,'0');
			SFPUTC(f,fmt);
			if(fmt != ' ')
				flags |= F_ZERO;
		}

		if((n = lval) > 0)
		{	/* left padding */
			v = (flags&F_ZERO) ? '0' : ' ';
			SFNPUTC(f,v,n);
		}

		if((n = precis) > 0 && ((flags&F_REPEAT) || !(flags&F_FLOAT)))
		{	/* repeated chars or padding for integer precision */
			v = (flags&F_REPEAT) ? fmt : '0';
			SFNPUTC(f,v,n);
			precis = 0;
		}

	do_output:
		if((n = endsp-sp) > 0)
			SFWRITE(f,sp,n);

		if(flags&(F_FLOAT|F_LEFT))
		{	/* F_FLOAT: right padding for float precision */
			if((n = precis) > 0)
				SFNPUTC(f,'0',n);

			/* F_FLOAT: the exponent of %eE */
			if((n = endep-(sp=ep)) > 0)
				SFWRITE(f,sp,n);

			/* F_LEFT: right padding */
			if((n = -lval) > 0)
				SFNPUTC(f,' ',n);
		}
	}

pop_fa:	if(fa = fast)
	{	/* pop the format stack and continue */
		form = fa->form;
		args = fa->args;
		argf = fa->argf.p;
		extf = fa->extf.p;
		fast = fa->next;
		FAFREE(fa);
		goto loop_fa;
	}

done:
	SFEND(f);

	r = f->next - f->data;
	if((d = (char*)f->data) == data)
		f->data = NIL(uchar*);
	f->next = f->data;
	f->endw = f->endb = f->data+f->size;

	if(r > 0 && (d == data || ((f->flags&SF_LINE) && !(f->flags&SF_STRING))))
	{	SFopen(f);
		(void) sfwrite(f,(uchar*)d,r);
	}
	else	f->next += r;

	SFOPEN(f);
	return n_output;
}

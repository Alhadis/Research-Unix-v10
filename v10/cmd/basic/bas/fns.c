#include <stdio.h>
#include "ctype.h"
#include "typedef.h"
#include "basic.h"
#include "tokens.h"

#define	MAXRAND		2147483647

double	sqrt(), exp(), log(), atan(), sin(), cos(), fabs();
double	popfloat(), cvtnumber();
static	char str[MAXSTRING];
Stkptr	pop(), pushstring();
char	*allocstr(), *fprint(), *strpop();


/*
 * function --- interpret builtin function calls
 */

function()
{
	switch (*inptr) {
	case LEFT:
		fnarg("si");
		break;
	case RIGHT:
		fnarg("si");
		break;
	case MID:
		fnarg("sii");
		break;
	case UPSHIFT:
	case UNBLANK:
	case SYSTEM:
	case LEN:
	case VAL:
	case ASCII:
		fnarg("s");
		break;
	case RND:
	case ARG:
	case CHR:
	case RAND:
		fnarg("i");
		break;
	case SIN:
	case COS:
	case TAN:
	case ATN:
	case SQR:
	case EXPFN:
	case LOG:
	case INTFN:
	case ABS:
	case STR:
	case SGN:
		fnarg("f");
		break;
	default:
		return(0);
		}
	return(1);
}


/*
 * fnarg --- do type checking and evaluation of builtin functions
 */

fnarg(args)
char	*args;
{
	register int	op, i;
	register Stkptr s;
	register char	*q;
	char		*p;
	double		x, t;
	int		k, nc, n, ch, len;
	
	op = *inptr++;
	for (p = args; *p; ) {
		expr();
		switch (*p++) {

		case 's':
			if (((Stkptr)stkptr)->k_type != STRINGEXPR)
				err("string argument required");
			break;
		case 'i':
		case 'f':
			if (((Stkptr)stkptr)->k_type != FLOATEXPR)
				err("numeric argument required");
			break;
		default:
			badtype();
			}
		if (*p)
			expectc(COMMA);
		}
	expectc(RPAR);
	switch (op) {
	case SYSTEM:
		p = strpop();		/* got a string in UNIX format */
		i = exec("/bin/sh", "sh", "-c", p, 0);
		pushint(i);
		break;
	case LEFT:
		if ((i = popint()) < 0)
			i = 0;
		s = (Stkptr)stkptr;
		if (i > s->k_un.k_str.s_len)
			i = s->k_un.k_str.s_len;
		n = 0;
		goto common;
	case RIGHT:
		if ((i = popint()) < 0)
			i = 0;
		s = (Stkptr)stkptr;
		if (i > s->k_un.k_str.s_len)
			i = s->k_un.k_str.s_len;
		n = s->k_un.k_str.s_len - i;
		goto common;
	case MID:
		if ((i = popint()) < 0)
			i = 0;
		if ((n = popint() - 1) < 0)
			n = 0;
		s = (Stkptr)stkptr;
		if (n > s->k_un.k_str.s_len)
			n = s->k_un.k_str.s_len;
		if (i > s->k_un.k_str.s_len - n)
			i = s->k_un.k_str.s_len - n;
	common:
		if (n > 0 || i < s->k_un.k_str.s_len) {
			if (isstring(s->k_un.k_str.s_ptr)) {
				p = allocstr(NULL, i, 0);
				if (i)
					move(i, s->k_un.k_str.s_ptr+n, p);
				s->k_un.k_str.s_ptr = p;
				}
			else
				s->k_un.k_str.s_ptr += n;
			s->k_un.k_str.s_len = i;
			}
		break;
	case UPSHIFT:
		p = strpop();
		len = strlen(p);
		for (k = 0; k <= len; k++)
			if (!isalpha(*(p+k)) || isupper(*(p+k)))
				str[k] = p[k];
			else
				str[k] = toupper(p[k]);
		s = pushstring(str, len);
		s->k_un.k_str.s_ptr = str;
		s->k_un.k_str.s_len = len;
		break;
	case UNBLANK:
		p = strpop();
		len = strlen(p);
		for (k = nc = 0; k <= len; k++)
			if (*(p+k) != ' ')
				str[nc++] = p[k];
		s = pushstring(str, nc - 1);
		s->k_un.k_str.s_ptr = str;
		s->k_un.k_str.s_len = nc - 1;
		break;
	case LEN:
		s = pop(ANYTYPE);
		pushint(s->k_un.k_str.s_len);
		break;
	case CHR:
		ch = popint();
		s = pushstring(&ch, 1);
		s->k_un.k_str.s_ptr = allocstr(&ch, 1, 1);
		break;
	case STR:
		p = fprint(popfloat());
		len = strlen(p);
		s = pushstring(p, len);
		s->k_un.k_str.s_ptr = allocstr(p, len, len);
		break;
	case ARG:
		len = popint();
		p = (len > argcnt)? "" : argvec[len];
		len = strlen(p);
		s = pushstring(p, len);
		s->k_un.k_str.s_ptr = allocstr(p, len, len);
		break;
	case ASCII:
		s = pop(ANYTYPE);
		pushint(*s->k_un.k_str.s_ptr);
		break;
	case VAL:
		s = pop(ANYTYPE);
		p = q = s->k_un.k_str.s_ptr;
		i = s->k_un.k_str.s_len;
		pushfloat(cvtnumber(&p, i));
		if (p - q != i)
			err("invalid number %.*s", i, q);
		break;
	case SIN:
		pushfloat(sin(popfloat()));
		break;
	case COS:
		pushfloat(cos(popfloat()));
		break;
	case TAN:
		x = popfloat();
		pushfloat(sin(x)/cos(x));
		break;
	case SGN:
		x = popfloat();
		if (x > 0)
			t = 1;
		else
			if (x < 0)
				t = -1;
			else
				t = 0;
		pushfloat(t);
		break;
	case SQR:
		if ((x = popfloat()) >= 0)
			pushfloat(sqrt(x));
		else
			err("sqr of negatives undefined. check");
		break;
	case LOG:
		if ((x = popfloat()) != 0)
			pushfloat(log(fabs(x)));
		else
			err("log of zero is undefined. check");
		break;
	case EXPFN:
		pushfloat(exp(popfloat()));
		break;
	case ATN:
		pushfloat(atan(popfloat()));
		break;
	case ABS:
		pushfloat(fabs(popfloat()));
		break;
	case INTFN:
		pushfloat((double)(int)popfloat());
		break;
	case RND:
		if ((i = popint()) != 0)
			srand(i);
		pushfloat((double)rand() / MAXRAND);
		break;
	case RAND:
		i = popint();
		srand((int)time(0) + i);
		pushfloat(x = ((double)rand() / MAXRAND));
		break;
	default:
		err("function not found");
		}
}

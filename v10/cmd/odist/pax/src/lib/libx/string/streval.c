/*
 * G. S. Fowler
 * D. G. Korn
 * AT&T Bell Laboratories
 *
 * long integer arithmetic expression evaluator
 * long constants may be represented as:
 *
 *	0ooo		octal
 *	0[xX]hhh	hexadecimal
 *	ddd		decimal
 *	n#ccc		base n, 2 <= b <= 36
 *
 * NOTE: all operands are evaluated as both the parse
 *	 and evaluation are done on the fly
 */

#include <ctype.h>

#define getchr()	(*cur.nextchr++)
#define peekchr()	(*cur.nextchr)
#define ungetchr()	(cur.nextchr--)

#define pushchr(s)	{struct vars old;old=cur;cur.nextchr=(s);cur.errmsg=0
#define popchr()	cur=old;}
#define error(msg)	return(seterror(msg))

struct vars			/* vars stacked per invocation		*/
{
	char*	nextchr;	/* next char in current expression	*/
	char*	errchr;		/* next char after error		*/
	char*	errmsg;		/* error message text			*/
};

static struct vars	cur;

static long	(*convert)();	/* external conversion routine		*/

static long	expr();		/* subexpression evaluator		*/
static long	seterror();	/* set error message string		*/

extern long	strton();

/*
 * evaluate an integer arithmetic expression in s
 *
 * (long)(*convert)(char* string, char** end) is a user supplied
 * conversion routine that is called when unknown chars are encountered
 * in s string points to the part to be converted and end is adjusted to
 * point to the next non-converted character; if string is 0 then end
 * points to an error message string
 *
 * NOTE: (*convert)() may call streval()
 */

long
streval(s, end, conv)
char*	s;
char**	end;
long	(*conv)();
{
	long	n;

	pushchr(s);
	convert = conv;
	n = expr(0);
	if (peekchr() == ':') (void)seterror("invalid use of :");
	if (cur.errmsg)
	{
		if (convert) (void)(*convert)((char*)0, &cur.errmsg);
		cur.nextchr = cur.errchr;
		n = 0;
	}
	if (end) *end = cur.nextchr;
	popchr();
	return(n);
}

/*   
 * evaluate a subexpression with precedence
 */

static long
expr(precedence)
register int	precedence;
{
	register int	c;
	register long	n;
	register long	x;
	char*		pos;
	int		operand = 1;

	while (isspace(c = getchr()));
	switch (c)
	{
	case 0:
		ungetchr();
		error("more tokens expected");
	case '-':
		n = -expr(12);
		break;
	case '+':
		n = expr(12);
		break;
	case '!':
		n = !expr(12);
		break;
	case '~':
		n = ~expr(12);
		break;
	default:
		ungetchr();
		n = 0;
		operand = 0;
		break;
	}
	for (;;)
	{
		switch (c = getchr())
		{
		case 0:
			goto done;
		case ')':
			if (!precedence) error("too many )'s");
			goto done;
		case '(':
			n = expr(1);
			if (getchr() != ')')
			{
				ungetchr();
				error("closing ) expected");
			}
		gotoperand:
			if (operand) error("operator expected");
			operand = 1;
			continue;
		case '?':
			if (precedence > 1) goto done;
			if (peekchr() == ':')
			{
				getchr();
				x = expr(2);
				if (!n) n = x;
			}
			else
			{
				x = expr(2);
				if (getchr() != ':')
				{
					ungetchr();
					error(": expected for ? operator");
				}
				if (n)
				{
					n = x;
					(void)expr(2);
				}
				else n = expr(2);
			}
			break;
		case ':':
			goto done;
		case '|':
			if (peekchr() == '|')
			{
				if (precedence > 2) goto done;
				getchr();
				x = expr(3);
				n = n || x;
			}
			else
			{
				if (precedence > 3) goto done;
				x = expr(4);
				n |= x;
			}
			break;
		case '^':
			if (precedence > 4) goto done;
			x = expr(5);
			n ^= x;
			break;
		case '&':
			if (peekchr() == '&')
			{
				if (precedence > 2) goto done;
				getchr();
				x = expr(3);
				n = n && x;
			}
			else
			{
				if (precedence > 5) goto done;
				x = expr(6);
				n &= x;
			}
			break;
		case '=':
		case '!':
			if (peekchr() != '=') error("operator syntax error");
			if (precedence > 6) goto done;
			getchr();
			x = expr(7);
			if (c == '=') n = n == x;
			else n = n != x;
			break;
		case '<':
		case '>':
			if (peekchr() == c)
			{
				if (precedence > 8) goto done;
				getchr();
				x = expr(9);
				if (c == '<') n <<= x;
				else n >>= x;
			}
			else
			{
				if (precedence > 7) goto done;
				if (peekchr() == '=')
				{
					getchr();
					x = expr(8);
					if (c == '<') n = n <= x;
					else n = n >= x;
				}
				else
				{
					x = expr(8);
					if (c == '<') n = n < x;
					else n = n > x;
				}
			}
			break;
		case '+':
		case '-':
			if (precedence > 9) goto done;
			x = expr(10);
			if (c == '+') n +=  x;
			else n -= x;
			break;
		case '*':
		case '/':
		case '%':
			if (precedence > 10) goto done;
			x = expr(11);
			if (c == '*') n *= x;
			else if (x == 0) error("divide by zero");
			else if (c == '/') n /= x;
			else n %= x;
			break;
		default:
			if (isspace(c)) continue;
			pos = --cur.nextchr;
			if (isdigit(c)) n = strton(cur.nextchr, &cur.nextchr, 0);
			else if (convert) n = (*convert)(cur.nextchr, &cur.nextchr);
			if (cur.nextchr == pos) error("syntax error");
			goto gotoperand;
		}
		if (cur.errmsg) return(0);
		if (!operand) error("operand expected");
	}
 done:
	ungetchr();
	if (!operand) error("operand expected");
	return(n);
}

/*
 * set error message string
 */

static long
seterror(msg)
char*	msg;
{
	if (!cur.errmsg) cur.errmsg = msg;
	cur.errchr = cur.nextchr;
	cur.nextchr = "";
	return(0);
}

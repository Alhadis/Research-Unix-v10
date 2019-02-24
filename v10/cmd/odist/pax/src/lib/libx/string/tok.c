/*
 * Glenn Fowler
 * AT&T Bell Laboratories
 *
 * token stream routines
 */

struct tok				/* token stream state		*/
{
	union
	{
	char*		end;		/* end ('\0') of last token	*/
	struct tok*	nxt;		/* next in free list		*/
	}		ptr;
	char		chr;		/* replace *end with this	*/
	char		flg;		/* ==0 => don't restore string	*/
};

typedef struct tok TOK;

static struct tok*	freelist;

extern char*		malloc();

/*
 * open a new token stream on s
 * if f==0 then string is not restored
 */

char*
tokopen(s, f)
register char*	s;
int		f;
{
	register TOK*	p;

	if (p = freelist) freelist = freelist->ptr.nxt;
	else if (!(p = (TOK*)malloc(sizeof(TOK)))) return(0);
	p->chr = *(p->ptr.end = s);
	p->flg = f;
	return((char*)p);
}

/*
 * close a token stream
 * restore the string to its original state
 */

void
tokclose(u)
char*	u;
{
	register TOK*	p = (TOK*)u;

	if (p->flg) *p->ptr.end = p->chr;
	p->ptr.nxt = freelist;
	freelist = p;
}

/*
 * return next space separated token
 * "\n" is returned as a token
 * 0 returned when no tokens remain
 * "..." and '...' quotes are honored with \ escapes
 */

char*
tokread(u)
char*	u;
{
	register TOK*	p = (TOK*)u;
	register char*	s;
	register char*	r;
	register int	q;
	register int	c;

	/*
	 * restore string on each call
	 */

	if (!p->chr) return(0);
	s = p->ptr.end;
	if (p->flg) *s = p->chr;
	else if (!*s) s++;

	/*
	 * skip leading space
	 */

	while (*s == ' ' || *s == '\t') s++;
	if (!*s)
	{
		p->ptr.end = s;
		p->chr = 0;
		return(0);
	}

	/*
	 * find the end of this token
	 */

	r = s;
	q = 0;
	for (;;) switch (c = *r++)
	{
	case '\n':
		if (s == (r - 1) && !q) r++;
		/*FALLTHROUGH*/
	case ' ':
	case '\t':
		if (q) break;
		/*FALLTHROUGH*/
	case 0:
		if (s == --r)
		{
			p->ptr.end = r;
			p->chr = 0;
		}
		else
		{
			p->chr = *(p->ptr.end = r);
			*r = 0;
		}
		return(s);
	case '\\':
		if (*r) r++;
		break;
	case '"':
	case '\'':
		if (c == q) q = 0;
		else if (!q) q = c;
		break;
	}
}

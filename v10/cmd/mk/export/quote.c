#include	"mk.h"

/*
	quoted stuff gets EBIT
*/

static
squote(fd, srcp, destp)
	char **srcp, **destp;
{
	register char *src = *srcp, *dest = *destp;

	while(*src != '\''){
		if(*src == 0){
			char *s;

			if(s = Frdline(fd)){
				inline++;
				*dest++ = '\n'|EBIT;
				strcpy(src, s);
				continue;
			} else {
				SYNERR(-1); Fprint(2, "missing closing '\n");
				break;
			}
		}
		*dest++ = *src++|EBIT;
	}
	*dest = 0;
	if(*src)
		src++;
	*srcp = src;
	*destp = dest;
}

static
dquote(fd, src, dest)
	char **src, **dest;
{
	char *p = *src, *s = *dest;
	char *t, carry;
	Symtab *sym;

	while(*p != '"'){
		switch(*p)
		{
		case 0:
			if(t = Frdline(fd)){
				inline++;
				*s++ = '\n'|EBIT;
				strcpy(p, t);
				continue;
			} else {
				SYNERR(-1); Fprint(2, "missing closing \"\n");
				break;
			}
			break;
		case '\\':
			switch(*++p)
			{
			default:
				*s++ = '\\'|EBIT;
			case '\\': case '"':
			case '\'': case '$':
				*s++ = *p|EBIT;
			}
			p++;
			break;
		case '$':
			varsub(&p, &s, EBIT);
			break;
		default:
			*s++ = *p++|EBIT;
		}
	}
	*s = 0;
	if(*p)
		p++;
	*src = p;
	*dest = s;
}

quote1(fd, src, dest)
	char *src, *dest;
{
	char *t;

	while(*src){
		switch(*src)
		{
		case '\'':
			src++;
			squote(fd, &src, &dest);
			break;
		case '\\':
			if(src[1]){
				*dest++ = *++src | EBIT;
				src++;
			} else if(t = Frdline(fd)){
				inline++;
				strcpy(src, t);
			}
			break;
		default:
			*dest++ = *src++;
		}
	}
	*dest = 0;
}

quote2(fd, src, dest)
	char *src, *dest;
{
	char *t;

	while(*src){
		switch(*src)
		{
		case '"':
			src++;
			dquote(fd, &src, &dest);
			break;
		case '\\':
			if(src[1]){
				*dest++ = *++src;
				src++;
			} else if(t = Frdline(fd)){
				inline++;
				strcpy(src, t);
			}
			break;
		case '$':
			varsub(&src, &dest, 0);
			break;
		default:
			*dest++ = *src++;
		}
	}
	*dest = 0;
}

static char *
subsub(val, ext, dest, bit)
	char *val, *ext, *dest;
{
	register char *s;
	char *a, *b, *c, *d;
	int na, nb, nc, nd, n;
	int destsub = 0;

	/* prepare literals */
	a = s = ext;
	for(; *s && (*s != '=') && !PERCENT(*s); s++)
		;
	na = s-a;
	if(PERCENT(*s)) s++;
	b = s;
	for(; *s && (*s != '='); s++)
		;
	nb = s-b;
	if(*s == '=') s++;
	c = s;
	for(; *s && !PERCENT(*s); s++)
		;
	nc = s-c;
	if(PERCENT(*s)) s++, destsub = 1;
	d = s;
	for(; *s; s++)
		;
	nd = s-d;
	/* break into words, do sub */
	while(*val){
		for(s = val; *s && !SEP(*s); s++)
			;
		/* substitute in val..s */
		if((memcmp(val, a, na) == 0) && (memcmp(s-nb, b, nb) == 0)){
#define	DEST(s,n)	if(n) memcpy(dest,s,n), dest += n
			DEST(c, nc);
			n = (s-nb) - (val+na);
			if(destsub)
				DEST(val+na, n);
			DEST(d, nd);
		} else
			while(val < s)
				*dest++ = (*val++&~EBIT)|bit;
		/* clean up */
		while(*s && SEP(*s))
			*dest++ = (*s++&~EBIT)|bit;
		val = s;
	}
	return(dest);
}

varsub(psrc, pdest, bit)
	char **psrc, **pdest;
{
	char *t, carry;
	Symtab *sym;
	char *src = *psrc, *dest = *pdest;
	char *v = 0, vc;

	if(*++src == '{'){
		for(t = ++src; *src != '}'; src++)
			if(*src == 0){
				SYNERR(-1);
				Fprint(2, "missing '}'\n");
				Exit();
			}
		v = shname(t);
		if(src == v)
			v = 0;
		else {
			if(*v != ':'){
				SYNERR(-1);
				Fprint(2, "bad variable name\n");
				Exit();
			}
			vc = *v;
			*v = 0;
		}
		*src++ = carry = 0;
	} else {
		src = shname(t = src);
		carry = *src;
		*src = 0;
	}
	if(sym = symlook(t, S_VAR, (char *)0)){
		if(v){
char buf[BIGBLOCK]; strcpy(buf, v+1); lex(-1, buf);
			dest = subsub(sym->value, buf, dest, bit);
			*v = vc;
		} else
			for(t = sym->value; *t; )
				*dest++ = *t++|bit;
	}
	if(carry)
		*src = carry;
	*psrc = src;
	*pdest = dest;
}

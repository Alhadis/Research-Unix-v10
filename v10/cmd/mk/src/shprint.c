#include	"mk.h"

shprint(s, env, dest)
	char *s, **env, *dest;
{
	register char *q = dest;

	while(*s)
		switch(*s)
		{
		case '\\':
			*q++ = *s++;
			*q++ = *s++;
			break;
		case '$':
			q = vexpand(&s, q, env);
			break;
		default:
			*q++ = *s++;
			break;
		}
	*q = 0;
}

char *
mygetenv(name, env)
	char *name, **env;
{
	register char *p, *q;

	for(; *env; env++){
		for(p = name, q = *env; *p == *q; p++, q++)
			if(*p == 0) break;
		if((*p == 0) && (*q == '='))
			return(q+1);
	}
	return((char *)0);
}

char *
vexpand(ww, dest, env)
	char **ww, *dest, **env;
{
	register char *s, *w = *ww;
	char carry, *q;

	assert("vexpand no $", *w == '$');
	if(*++w == '{'){
		for(s = ++w; *s != '}'; s++)
			if(*s == 0) break;
	} else
		s = shname(w);
	carry = *s;
	*s = 0;
	if(q = mygetenv(w, env)){
		while(*q)
			*dest++ = *q++;
	} else {	/* copy name */
		for(q = *ww; q != s;)
			*dest++ = *q++;
		if(carry == '}')
			*dest++ = carry;
	}
	*s = carry;
	if(carry == '}')
		s++;
	*ww = s;
	return(dest);
}

front(s)
	char *s;
{
	register char *t;
	register i, j;
	char *flds[512];
	char buf[BIGBLOCK];

	setfields(" \t\n");
	strcpy(buf, s);
	i = getfields(buf, flds, 512);
	if(i > 5){
		flds[4] = flds[i-1];
		flds[3] = "...";
		i = 5;
	}
	t = s;
	for(j = 0; j < i; j++){
		for(s = flds[j]; *s; *t++ = *s++);
		*t++ = ' ';
	}
	*t = 0;
}

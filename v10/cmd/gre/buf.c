#include	<libc.h>

#define	BUFSIZE	65536
static char buf[BUFSIZE], *next = &buf[BUFSIZE], *rnext = buf;
long cnt;

rd(b, e)
	register char **b, **e;
{
	int n, m;

	if((*b == 0) || (*b == *e))
		rnext = *b = *e = buf;
	if(rnext == &buf[BUFSIZE]){
		if(*b > buf){
			if(next < *b){
				/* fail(next, *b); */
			}
			memmove(buf, *b, n = rnext - *b);
			rnext -= (*b - buf);
			next = *b = buf;
		}
	}
	if((n = read(0, rnext, &buf[BUFSIZE] - rnext)) <= 0){
		return(n);
	}
	*e = (rnext += n);
	return(1);
}

match(b, e)
	register char **b, **e;
{
	char *s, *f;
	int eoffset, n;

	cnt++;
	for(s = *b; s >= buf;)
		if(*--s == '\n')
			break;
	s++;
	if(next != s){
		/* fail(next, s); */
	}
	for(f = *e;;){
		for(; f < rnext; f++)
			if(*f == '\n')
				goto done;
		eoffset = f-s;
		if((n = rd(&s, &f)) <= 0)
			return(n);
		f = s+eoffset;
	}
done:
	f++;
	/* succ(s, f) */
	next = *b = f;
	*e = rnext;
	return(1);
}

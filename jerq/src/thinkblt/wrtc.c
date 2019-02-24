#define NDATA	120

static char buf[NDATA+2];

static char *p = buf+2;

wrtc(c)
{
	*p++ = c;
	if (p >= &buf[sizeof buf])
		wrtflush();
}

wrts(s)
register char *s;
{
	register char *q = p;
	register c;
	while (c = *s++) {
		*q++ = c;
		if (q >= &buf[sizeof buf])
			p=q, wrtflush(), q=p;
	}
	p = q;
}

wrt(s, n)
register char *s;
{
	register char *q = p;
	register nxfr;
	while (n > 0) {
		if ((nxfr = &buf[sizeof buf]-q) > n)
			nxfr = n;
		memcpy(q, s, nxfr);
		q += nxfr;
		s += nxfr;
		n -= nxfr;
		if (q >= &buf[sizeof buf])
			p=q, wrtflush(), q=p;
	}
	p = q;
}

wrtflush()
{
	register n = p-buf-2;
	if (n > 0) {
		extern int outfd;
		buf[0] = n>>8;
		buf[1] = n;
		write(outfd, buf, n+2);
	}
	p = buf+2;
}

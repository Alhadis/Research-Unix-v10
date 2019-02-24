static char normbuf[128], specbuf[3*128 + 2];
static int lastn, lasts;

static
cmp(a, b)
	char *a,*b;
{
	if(*a != *b)
		return((int)*a - (int)*b);
	else
		return((int)*++a - (int)*++b);
}

static
bs(array, name, first, last)
	char *array, *name;
	int first, last;
{
	register char *u, *l, *m;

	u = &array[3*last];
	l = &array[3*first];
	if((cmp(name, u-3) > 0) || (cmp(name, l) < 0))
		return(0);
	/* invariant is l <= name < u */
	while(l != u-3)		/* ie adjacent */
	{
		m = &l[(((long)u-(long)l)/6)*3];
		if(cmp(name, m) < 0)
			u = m;
		else
			l = m;
	}
	if(cmp(name, l))
		return(0);
	else
		return(l[2]);
}

imap(name)
	char *name;
{
	if(lastn == 0)
	{
		int fd;
		char buf[256];

		sprintf(buf, "%s/IMAP", JERQFONT);
		if((fd = open(buf, 0)) == -1)
			return(0);
		(void)read(fd, normbuf, 1);
		lastn = normbuf[0] & 0xFF;
		(void)read(fd, normbuf, 3*lastn);
		(void)close(fd);
	}
	if(name[1] == 0)
		return(name[0] & 0x7F);
	else
		return(bs(normbuf, name, 0, lastn));
}

smap(name)
	char *name;
{
	if(lasts == 0)
	{
		int  fd;
		char buf[256];

		sprintf(buf, "%s/SMAP", JERQFONT);
		if((fd = open(buf, 0)) == -1)
			return(0);
		(void)read(fd, specbuf, 1);
		lasts = specbuf[0] & 0xFF;
		(void)read(fd, specbuf, 3*lasts);
		(void)close(fd);
	}
	return(bs(specbuf, name, 0, lasts));
}

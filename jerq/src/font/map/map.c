#ifndef	JERQFONT
#define	JERQFONT	"/usr/jerq/font"
#endif	JERQFONT

static char normal[128], special[512];
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

	u = &array[2*last];
	l = &array[2*first];
	if((cmp(name, u-2) > 0) || (cmp(name, l) < 0))
		return(0);
	/* invariant is l <= name < u */
	while(l != u-2)		/* ie adjacent */
	{
		m = (char *)((((long)l+(long)u) >> 2) << 1);
#ifdef	DEBUG
		printf("bs loop: name=%c%c; l=%d, u=%d;  l=%c%c  m=%c%c u=%c%c\n",
			name[0],name[1],l,u,l[0],l[1],m[0],m[1],u[0],u[1]);
#endif
		if(cmp(name, m) < 0)
			u = m;
		else
			l = m;
	}
	if(cmp(name, l))
		return(0);
	else
		return(1 + (l-array)/2);
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
		read(fd, normal, 1);
		lastn = normal[0] & 0xFF;
		read(fd, normal, 2*lastn);
		close(fd);
	}
	return(bs(normal, name, 0, lastn));
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
		read(fd, special, 1);
		lasts = special[0] & 0xFF;
		read(fd, special, 2*lasts);
		close(fd);
	}
	return(bs(special, name, 0, lasts));
}

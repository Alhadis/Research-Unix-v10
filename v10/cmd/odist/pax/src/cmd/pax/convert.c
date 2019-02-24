/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * pax conversion support
 */

#include "pax.h"

/*
 * convert string to lower case in place
 */

char*
strlower(s)
register char*	s;
{
	register int	c;
	register char*	t;

	for (t = s; c = *t; t++)
		if (isupper(c))
			*t = tolower(c);
	return(s);
}

/*
 * convert string to upper case in place
 */

char*
strupper(s)
register char*	s;
{
	register int	c;
	register char*	t;

	for (t = s; c = *t; t++)
		if (islower(c))
			*t = toupper(c);
	return(s);
}

/*
 * convert binary header shorts to long
 */

long
cpio_long(s)
register short*	s;
{
	union integral	u;

	u.l = 1;
	if (u.c[0])
	{
		u.s[0] = s[1];
		u.s[1] = s[0];
	}
	else
	{
		u.s[0] = s[0];
		u.s[1] = s[1];
	}
	return(u.l);
}

/*
 * convert long to binary header shorts
 */

void
cpio_short(s, n)
register short*	s;
long		n;
{
	union integral	u;

	u.l = 1;
	if (u.c[0])
	{
		u.l = n;
		s[0] = u.s[1];
		s[1] = u.s[0];
	}
	else
	{
		u.l = n;
		s[0] = u.s[0];
		s[1] = u.s[1];
	}
}

/*
 * convert local mode to cpio mode
 */

int
cpio_mode(f)
register struct fileinfo*	f;
{
	register int	type;

	switch (f->st->st_mode & S_IFMT)
	{
	case 0:
		type = 0;
		break;
	case S_IFIFO:
		type = CPIO_FIFO;
		break;
	case S_IFDIR:
		type = CPIO_DIR;
		break;
	case S_IFCHR:
		type = CPIO_CHR;
		break;
	case S_IFBLK:
		type = CPIO_BLK;
		break;
	case S_IFLNK:
		type = CPIO_LNK;
		break;
	case S_IFSOCK:
		type = CPIO_SOCK;
		break;
	default:
		error(1, "%s: unknown file type %07o -- regular file assumed", f->name, f->st->st_mode & S_IFMT);
		/*FALLTHROUGH*/
	case S_IFREG:
		type = CPIO_REG;
		break;
	}
	return((f->st->st_mode & ~S_IFMT) | type);
}

/*
 * compute tar_header checksum
 */

int
tar_checksum()
{
	register char*	p;
	register int	n;

	p = tar_header.chksum;
	while (p < &tar_header.chksum[sizeof(tar_header.chksum)]) *p++ = ' ';
	n = 0;
	p = tar_block;
	while (p < &tar_block[TAR_HEADER]) n += *p++;
	return(n);
}

/*
 * compute running s5r4 file content checksum
 */

long
asc_checksum(b, n, sum)
register unsigned char*	b;
int			n;
register unsigned long	sum;
{
	register unsigned char*	e;

	e = b + n;
	while (b < e) sum += *b++;
	return(sum);
}

/*
 * get label header number
 */

long
getlabnum(p, byte, width, base)
register char*	p;
int		byte;
int		width;
int		base;
{
	register char*	e;
	register int	c;
	long		n;

	p += byte - 1;
	c = *(e = p + width);
	*e = 0;
	n = strtol(p, (char*)0, base);
	*e = c;
	return(n);
}

/*
 * get label header string
 */

char*
getlabstr(p, byte, width, s)
register char*	p;
int		byte;
int		width;
register char*	s;
{

	register char*	e;
	char*		v;

	v = s;
	p += byte - 1;
	e = p + width;
	while (p < e && (*s = *p++) != ' ') s++;
	*s = 0;
	return(v);
}

#if !new_delta_format

/*
 * this is an obsolete version of the libx implementation
 */

#undef	HASHPART
#define HASHPART(b,h,c,l,r)	(h = ((h = (h << (l)) ^ (h >> (r)) ^ (c)) & (1 << (b)) ? ~h : h) & ((((1 << ((b) - 1)) - 1) << 2) | 3))

#undef	HASHLPART
#define HASHLPART(h,c)		HASHPART(31, h, c, 3, 2)

unsigned long
memsum(b, n, c)
char*			b;
int			n;
register unsigned long	c;
{
	register unsigned char*	p;
	register unsigned char*	e;

	p = (unsigned char*)b;
	e = p + n;
	while (p < e) HASHLPART(c, *p++ + 1);
	return(c);
}

#endif

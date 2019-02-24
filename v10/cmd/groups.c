/*
**	Print out current access groups.
*/

char *		Usage			= "[-h{eader}] [-s{ort}]";

#include	<sys/param.h>
#include	<grp.h>
#include	<stdio.h>
#include	<trace.h>

char *		format			= " %s";
short		groups[NGROUPS+1];
char *		grnames[NGROUPS+1];
char *		name;
int		noheader;
int		nosort;
int		Traceflag;

#define		NULLSTR			(char *)0
#define		SYSERROR		(-1)

extern char *	malloc();
extern char *	strcpy();
extern char *	strrchr();
extern int	strlen();
extern char	_sobuf[];

void		prgr(), syserror(), usage();
int		comp();
char *		newstr();

int
main(argc, argv)
	int	argc;
	char *	argv[];
{
	int	ngroups = NGROUPS;

	if ( (name = strrchr(*argv, '/')) != NULLSTR )
		name++;
	else
		name = *argv;

	while ( --argc > 0 )
	{
		if ( **++argv == '-' )
		{
			register int	c;

			while ( c = *++*argv )
			{
				switch ( c )
				{
				case 'h':	noheader = 1; format = "%s\n";
						continue;
				case 's':	nosort = 1;
						continue;
				case 't':	Traceflag = 1;
						continue;
				default:	usage("unreconised flag '%c'", c);
				}

				while ( (c = **argv) <= '9' && c >= '0' )
					++*argv;
				--*argv;
			}

break2:			;
		}
		else
			usage("unrecognised argument \"%s\"", *argv);
	}

	setbuf(stdout, _sobuf);

	if ( (ngroups = getgroups(NGROUPS, groups)) == SYSERROR )
		syserror("getgroups");

	prgr(ngroups);

	return 0;
}

void
prgr(n)
	register int		n;
{
	register short *	gp;
	register char **	cpp;
	register int		i;
	register struct group *	grp;
	register char *		name;
	char			space[20];

	groups[n++] = getgid();

	Trace(("prgr(%d), gid %d\n", n-1, groups[n-1]));

	if ( !noheader )
		printf("Access group%s:", n==1?"":"s");

	for ( gp = groups, cpp = grnames, i = n ; i-- ; )
	{
		Trace(("Got group id %d", *gp));

		if ( (grp = getgrgid(*gp++)) == (struct group *)0 )
		{
			(void)sprintf(space, "<%d>", gp[-1]);
			name = space;
		}
		else
			name = grp->gr_name;

		Trace((" name \"%s\"\n", name));

		if ( nosort )
			printf(format, name);
		else
			*cpp++ = newstr(name);
	}

	if ( !nosort )
	{
		qsort((char *)grnames, n, sizeof(char **), comp);

		for ( cpp = grnames, i = n ; i-- ; )
			printf(format, *cpp++);
	}

	if ( !noheader )
		printf(".\n");
}

void
syserror(s)
	char *	s;
{
	perror(s);
	exit(2);
}

void
usage(s, a)
	char *	s;
	char *	a;
{
	fprintf(stderr, "%s: ", name);
	fprintf(stderr, s, a);
	fprintf(stderr, ".\nUsage: \"%s %s\".\n", name, Usage);
	exit(1);
}

char *
Malloc(size)
	int		size;
{
	register char *	cp;

	while ( (cp = malloc((unsigned)size)) == NULLSTR )
		syserror("no memory for Malloc");

	return cp;
}

char *
newstr(s)
	char *	s;
{
	if ( s == NULLSTR )
		s = "";

	return strcpy(Malloc(strlen(s)+1), s);
}

int
comp(a1, a2)
	char **		a1;
	char **		a2;
{
	register char *	s1	= *a1;
	register char *	s2	= *a2;
	register int	c1;
	register int	c2;

	while ( c1 = *s1++ )
	{
		if ( (c2 = *s2++) == 0 )
			return 1;

		if ( (c1 |= 040) != (c2 |= 040) )
			return (c1&0xff) - (c2&0xff);
	}

	return -((*s2)&0xff);
}

#ifdef	GETGROUP
int
getgroups(n, gp)
	int			n;
	short *			gp;
{
	register int		i;
	int			count = 1;

	*gp++ = 0;

	for ( i = 1 ; i < n ; i++ )
	{
		if ( getgrgid(i) != (struct group *)0 )
		{
			*gp++ = i;
			count++;
		}
	}

	return count;
}
#endif

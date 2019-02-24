/* @(#) comm1.c: 1.1 12/22/83				*/

# include "mfile1.h"

# ifndef EXIT
# define EXIT exit
# endif

int nerrors = 0;  /* number of errors */
int nowarn = 0;  /* suppress warnings */

NODE *lastfree;  /* pointer to last free node; (for allocator) */

# ifndef MYWHERE
where(c)
{
	 /* print location of error. c is either 'u', 'c', or 'w' */
	fprintf( stderr, "%s:%d:", ftitle, lineno );
}
# endif

/* VARARGS1 */
uerror( s, a, b, c, d, e, f, g )
char *s; 
{
	/* nonfatal error message 
	** the routine where is different for pass 1 and pass 2;
	** it tells where the error took place 
	*/
	++nerrors;
	where('u');
	fprintf( stderr, s, a, b, c, d, e, f, g );
	fprintf( stderr, "\n" );
	if( nerrors > 30 ) cerror( "too many errors");
	fflush(stderr);
	nowarn = 1;
}

/* VARARGS1 */
cerror(s, a, b, c, d)
char *s; 
{
	 /* compiler error: die */
	where('c');
	if( nerrors && nerrors <= 30 )
	{
		/* give the compiler the benefit of the doubt */
		fprintf( stderr,
		"cannot recover from earlier errors: goodbye!\n" );
		fflush(stderr);
	}
	else 
	{
		fprintf( stderr, "compiler error: " );
		fprintf( stderr, s, a, b, c, d);
		fprintf( stderr, "\n" );
		fflush(stderr);
		fflush( stdout );
	}
	EXIT(1);
}

/* VARARGS1 */
werror( s, a, b )
char *s; 
{
	/* warning */
	if (nowarn) return;
	where('w');
	fprintf( stderr, "warning: " );
	fprintf( stderr, s, a, b );
	fprintf( stderr, "\n" );
	fflush(stderr);
}

tinit()
{
	 /* initialize expression tree search */
	register NODE *p;

	for (p=node; p < &node[TREESZ]; ++p ) {
		p->in.op = FREE;
	}
	lastfree = node;
}

NODE *
talloc()
{
	register NODE *p, *q;

	p = lastfree;
	do {
		if (p < &node[TREESZ-1])
			q = p+1;
		else
			q = node;
		if (p->in.op == FREE) {
			lastfree = q;
			return p;
		}
	} while ((p = q) != lastfree);
	cerror("out of tree space; simplify expression");
}

tcheck()
{
	 /* ensure that all nodes have been freed */
	register NODE *q; register n;

	if (nerrors == 0) {
		for (n=0, q=node; q < &node[TREESZ]; ++q)
			if (q->in.op != FREE)
				++n;
		if (n)
			cerror("wasted space: %d nodes", n);
	} else
		tinit();
	freestr();
}

tfree(p)
register NODE *p; 
{
	/* allow tree fragments to be freed, also */
	if (p == 0) return;
	switch (optype(p->tn.op)) {
	case BITYPE:
		tfree (p->in.right);
	case UTYPE:
		tfree (p->in.left);
	}
	p->in.op = FREE;
}


NODE node[TREESZ];

char	ftitle[100] = "\"\"";	/* title of the file */
extern int	lineno;		/* line number of the input file */


#define NTSTRBUF	60
#define TSTRSZ		2048
char	itstrbuf[TSTRSZ];
char	*tstrbuf[NTSTRBUF] = { itstrbuf };
char	**curtstr = tstrbuf;
int	tstrused;

char *
tstr( cp )			/* place copy of string into temp storage */
	register char *cp;	/* strings longer than TSTRSZ will break tstr */
{
	register int i = strlen( cp );
	register char *dp;
	extern char * malloc();
	extern char * strcpy();

	if ( tstrused + i >= TSTRSZ )
	{
		if ( ++curtstr >= &tstrbuf[NTSTRBUF] )
			cerror( "out of temporary string space" );
		tstrused = 0;
		if ( *curtstr == 0 )
		{
			if ( ( dp = (char *) malloc( TSTRSZ ) ) == 0 )
				cerror( "out of memory [tstr()]" );
			*curtstr = dp;
		}
	}
	strcpy( dp = *curtstr + tstrused, cp );
	tstrused += i + 1;
	return ( dp );
}
#include "dope.h"

mkdope()
{
	register struct dopest *q;

	for( q = indope; q->dopeop >= 0; ++q )
	{
		dope[q->dopeop] = q->dopeval;
		opst[q->dopeop] = q->opst;
	}
}

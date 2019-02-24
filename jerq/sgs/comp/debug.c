/*	@(#) debug.c: 1.2 2/27/84	*/
/* debug.c -- symbol table output routines for common assembler */

#include <signal.h>
#include "mfile1.h"
#include "macdefs.h"
#include "storclass.h"

FILE * outfile = stdout;

#define LINENO (lineno-startln+1)	/* local line number */

static int startln;			/* editor line number of opening { */
static int oldln;			/* last line number printed */
int gdebug;				/* -g flag (not used) */
static char startfn[100];		/* file name of the opening { */
static char fncname[BUFSIZ+1];		/* name of current function */
static int bb_flags[BCSZ];		/* remember whether or not bb
					** is needed
					*/

fix1def(p,dsflag)
struct symtab *p;
int dsflag;				/* 0 to permit output */
{
	/* print debugging info
	 *
	 * don't do params, externs, structure names or members
	 * at this time; they are delayed until more information
	 * is known about them
	 */

	switch( p->sclass ) {

	case USTATIC:
	case EXTERN:
	case EXTDEF:
	case STNAME:
	case UNAME:
	case ENAME:
	case MOS:
	case MOE:
	case MOU:
		return;
	default:
		if( p->sclass & FIELD ) return;
		}

	/* parameters */
	if( p->slevel == 1 ) return;

	/* static externs */
	if( p->sclass == STATIC && p->slevel == 0 ) return;

	prdef(p,dsflag);
	}

prdims( p ) struct symtab *p; {
	/* print debugging info for dimensions
	 */
	int temp, dtemp, dflag;
	OFFSZ tsize();

	dflag = 0; /* need to print dim */
	dtemp = p->dimoff;

	for( temp=p->stype; temp&TMASK; temp = DECREF(temp) ) {
		/* put out a dimension for each instance of ARY in type word */

		if( ISARY(temp) ) {
			if( !dflag ) {
				fprintf( outfile, "	.dim	%d", dimtab[dtemp++] );
				dflag = 1;
				}
			else
				fprintf( outfile, ",%d", dimtab[dtemp++] );
			}
		}

	if( dflag ) {
		fputs( ";\t.size\t", outfile );
		fprintf( outfile, /*CONFMT*/"%ld", tsize(p->stype, p->dimoff, p->sizoff ) / SZCHAR  );
		putc( ';', outfile );
		}
	}

/* local table of fakes for un-names structures
 * sizoff for .ifake is stored in mystrtab[i]
 */
#define FAKENM 99	/* maximum number of fakenames */
int mystrtab[FAKENM], ifake = 0;
struct symtab mytable;
	char tagname[BUFSIZ] = "";

prdef(p, mydsflg)
struct symtab *p;
int mydsflg;
{
	/* print symbol definition pseudos
	 */
	extern int dsflag;		/* the real one */
	TWORD pty, bpty;
	TWORD chgtype();
	char *strname();
	int sclass;
	int saveloc;

	if ( mydsflg )
		return;

	/* translate storage class */

	switch( p->sclass ){
	case AUTO:	sclass = C_AUTO; break;
	case EXTDEF:
	case EXTERN:	sclass = C_EXT; break;
	case STATIC:	sclass = C_STAT; break;
	case REGISTER:	sclass = blevel==1 ? C_REGPARM : C_REG; break;
	case ULABEL:
	case LABEL:	sclass = C_LABEL; break;
	case MOS:	sclass = C_MOS; break;
	case PARAM:	sclass = C_ARG; break;
	case STNAME:	sclass = C_STRTAG; break;
	case MOU:	sclass = C_MOU; break;
	case UNAME:	sclass = C_UNTAG; break;
	case TYPEDEF:	sclass = C_TPDEF; break;
	case ENAME:	sclass = C_ENTAG; break;
	case MOE:	sclass = C_MOE; break;
	default:	if( p->sclass & FIELD )
				sclass = C_FIELD;
			else
				cerror( "bad storage class %d", p->sclass );
			break;
		}

	/* print a .bb symbol if this is the first symbol in the block */

	if( blevel > 2 && !bb_flags[blevel] && sclass != C_LABEL ) {
		fprintf( outfile, "	.def	.bb;	.val	.;	.scl	%d;	.line	%d;	.endef\n", C_BLOCK, LINENO );
		bb_flags[blevel] = 1;   /* don't let another .bb print */
		}

	/* make sure that .defs in functions are in text section */
	if( blevel > 1 )
		saveloc = locctr( PROG );

	fprintf( outfile, "	.def	%s;", exname( p->sname ) );

	switch( sclass ) {	/* print val based on storage class */

	case C_AUTO:
	case C_MOS:
	case C_MOU:
	case C_ARG:
		/* offset in bytes */
		fprintf( outfile, "	.val	%d;", p->offset/SZCHAR );
		break;

	case C_FIELD:
	case C_MOE:
		/* offset in bits, or internal value of enum symbol */
		fprintf( outfile, "	.val	%d;", p->offset );
		break;

	case C_REGPARM:
	case C_REG:
		/* offset in bytes in savearea for reg vars */
		/* actual offset determination is deferred to the asembler */
		/* pointer vars in %aN indicated by adding 8 */
		fprintf( outfile, "	.val	%d;", OUTREGNO(p));
		break;

	case C_STAT:
	case C_LABEL:
	case C_EXT:
		/* actual or hidden name, depending on scope */
		if( p->slevel != 0 )
		{
			fprintf( outfile, "	.val	");
			fprintf( outfile, LABFMT, p->offset );
			putc( ';', outfile);
		}
		else
			fprintf( outfile, "	.val	%s;",
				exname( p->sname ) );
		break;

	default:
		break;
		}

	pty = chgtype(p->stype);	/* change type word from new style
					** to old
					*/
	bpty = BTYPE(p->stype);
	fprintf( outfile, "	.scl	%d;	.type	0%o;", sclass, pty );

	/* print tag and size info, size only for tags themselves;
	 * don't print if size not known
	 */

	if (dimtab[p->sizoff] > 0)
		switch( sclass ) {

		case C_STRTAG:
		case C_UNTAG:
		case C_ENTAG:
			fprintf( outfile, "	.size	%d;", (unsigned)dimtab[p->sizoff] / SZCHAR );
			break;

		default:	/* bpty is base type */
				/* check the real dsflag here--don't want */
				/* to put out a .tag for a function when */
				/* the structure hasn't been .def'ed */
			if( (bpty == STRTY || bpty == UNIONTY || bpty == ENUMTY) && !dsflag )
			{
				char buf[BUFSIZ];

				(void) strcpy( buf, strname( p->sizoff ) );
				if ( buf[0] == '$' )
					buf[0] = '_';
				fprintf( outfile, "	.tag	%s;",
					exname( buf ) );
				fprintf( outfile, "	.size	%d;",
					(unsigned) dimtab[p->sizoff] / SZCHAR );
				}
			break;
			}

	/* print line for block symbols */

	if( p->slevel > 2 )
		fprintf( outfile, "	.line	%d;", LINENO );

	/* look for dimensions to print */

	if( sclass != C_LABEL && !mydsflg )
		prdims( p );

	/* size of field is its length in bits */

	if( sclass == C_FIELD )
		fprintf( outfile, "	.size	%d;", p->sclass & FLDSIZ );

	fputs( "\t.endef\n", outfile );
	if( blevel > 1 )
		locctr( saveloc );
	}

str1end( dimst, dsflag )
int dimst;
int dsflag;				/* 0 to enable output */
{
	/* called at the end of a structure declaration
	 * this routine puts out the structure tag, its members
	 * and an eos.  dimst is the index in dimtab of the
	 * structure description
	 */
	int member, size, saveloc;
	struct symtab *memptr, *tagnm, *strfind();

	if( dsflag ) return;

	/* set locctr to text */
	saveloc = locctr( PROG );

	/* set up tagname */
	member = dimtab[dimst + 1];
	tagnm = strfind(dimst);

	if( tagnm == NULL ) {
		/* create a fake if there is no tagname */
		/* use the local symbol table */
		tagnm = &mytable;
		if( ifake == FAKENM )
			cerror( "fakename table overflow" );

		/* generate the fake name and enter into the fake table */
		{
			char buf[BUFSIZ];

			sprintf( buf, ".%dfake", ifake );
			mytable.sname = savestr( buf );	/* lives forever! */
		}
		mystrtab[ifake++] = dimst;
		memptr = &stab[dimtab[member]];

		/* fix up the fake's class, type, and sizoff based on class of its members */
		switch( memptr->sclass ) {
		case MOS:
			tagnm->sclass = STNAME;
			tagnm->stype = STRTY;
			break;
		case MOE:
			tagnm->sclass = ENAME;
			tagnm->stype = ENUMTY;
			break;
		case MOU:
			tagnm->sclass = UNAME;
			tagnm->stype = UNIONTY;
			break;
		default:
			if( memptr->sclass & FIELD ){
				tagnm->sclass = STNAME;
				tagnm->stype = STRTY;
				}
			else
				cerror( "can't identify type of fake tagname" );
			}
		tagnm->slevel = 0;;
		tagnm->sizoff = dimst;
		}

	/* print out the structure header */
	{
		char buf[BUFSIZ], *saveit;

		saveit = tagnm->sname;
		(void) strncpy( buf, saveit, BUFSIZ - 1 );
		buf[BUFSIZ - 1] = '\0';
		tagnm->sname = buf;
		if ( buf[0] == '$' )
			buf[0] = '_';
		prdef( tagnm, 0 );
		tagnm->sname = saveit;
	}

	/* print out members */
	while( dimtab[member] >= 0 ) {
		memptr = &stab[dimtab[member++]];
		{
			char buf[BUFSIZ], *saveit;

			saveit = memptr->sname;
			(void) strncpy( buf, saveit, BUFSIZ - 1 );
			buf[BUFSIZ - 1] = '\0';
			memptr->sname = buf;
			if ( buf[0] == '$' )
				buf[0] = '_';
			prdef( memptr, 0 );
			memptr->sname = saveit;
		}
	}

	/* print eos */
	size = (unsigned)dimtab[dimst] / SZCHAR;
	fprintf( outfile, "	.def	.eos;	.val	%d;	.scl	%d;", size, C_EOS );
	fprintf( outfile, "	.tag	%s;	.size	%d;	.endef\n",
		exname( tagnm->sname ), size );

	/* return to old locctr */
	locctr( saveloc );
	}

struct symtab *
strfind( key ) int key; {
	/* find the structure tag in the symbol table, 0 == not found
	 */
	struct symtab *sptr;
	char spc;
	for( sptr = stab; sptr < stab + SYMTSZ; ++sptr ) {
		spc = sptr->sclass;
		if( (spc == STNAME || spc == ENAME || spc == UNAME ) && sptr->sizoff == key && sptr->stype != TNULL )
			return( sptr );
		}
	/* not found */
	return( NULL );
	}

char *
strname( key ) int key; {
	/* return a pointer to the tagname,
	 * the fake table is used if not found by strfind
	 */
	int i;
	struct symtab *tagnm, *strfind();
	tagnm = strfind( key );
	if( tagnm != NULL )
		return( tagnm->sname );

	for( i = 0; i < FAKENM; ++i )
		if( mystrtab[i] == key ) {
			sprintf( tagname, ".%dfake", i );
			return( tagname );
			}

	cerror( "structure tagname not found" );
	return(NULL);
	}

/* chgtype -- change type word from pcc2 internal type to
** old style type for symbol table utilities.
**
** The old and new types are sufficiently similar to obtain the
** old from the new by extracting the basic type, shifting the
** remaining bits right 1, then OR-ing them back with the basic
** type.  That will probably not continue to be true.
*/

/* Define old types */

#define	O_TNULL		020
#define O_TVOID		040
#define	O_UNDEF		00
#define	O_FARG		01
#define	O_CHAR		02
#define	O_SHORT		03
#define	O_INT		04
#define	O_LONG		05
#define	O_FLOAT		06
#define	O_DOUBLE	07
#define	O_STRTY		010
#define	O_UNIONTY	011
#define	O_ENUMTY	012
#define	O_MOETY		013
#define	O_UCHAR		014
#define	O_USHORT	015
#define	O_UNSIGNED	016
#define	O_ULONG		017

TWORD
chgtype(ntype)
TWORD ntype;			/* new type */
{
    TWORD btype = BTYPE(ntype);	/* get basic type */
    TWORD other = ntype & ~((TWORD) BTMASK);
				/* get remaining bits */

    TWORD newtype;		/* derived "new" (i.e., old-style) type */

    switch ( btype )		/* convert basic type */
    {
    case TNULL:		newtype = O_TNULL; break;
    case FARG:		newtype = O_FARG; break;
    case CHAR:		newtype = O_CHAR; break;
    case SHORT:		newtype = O_SHORT; break;
    case INT:		newtype = O_INT; break;
    case LONG:		newtype = O_LONG; break;
    case FLOAT:		newtype = O_FLOAT; break;
    case DOUBLE:	newtype = O_DOUBLE; break;
    case STRTY:		newtype = O_STRTY; break;
    case UNIONTY:	newtype = O_UNIONTY; break;
    case ENUMTY:	newtype = O_ENUMTY; break;
    case MOETY:		newtype = O_MOETY; break;
    case UCHAR:		newtype = O_UCHAR; break;
    case USHORT:	newtype = O_USHORT; break;
    case UNSIGNED:	newtype = O_UNSIGNED; break;
    case ULONG:		newtype = O_ULONG; break;
    case VOID:		newtype = O_TVOID; break;
    case UNDEF:		newtype = O_UNDEF; break;
    default:	cerror("Unknown basic type\n");
    }
    return( (other >> 1) | newtype );	/* return changed type */
}


sdbln1(dlflag)
int dlflag;				/* 0 to allow printing */
{
    if ( !dlflag && lineno != oldln )
    {
	oldln = lineno;			/* remember last line no. output */
	if ( curloc == PROG && strcmp( startfn, ftitle ) == 0 )
	    fprintf( outfile, "	.ln	%d\n", LINENO );
    }
    return;
}


aobeg(){
	/* called before removing automatics from stab
	 */
#if defined(M32B) && defined(IMPREGAL)
	raftn();
#endif
	}

aocode(p) struct symtab *p; {
	/* called when automatic p removed from stab
	 */
	}

aoend1(dsflag)
int dsflag;				/* 0 to allow printing */
{
	/* called after removing all automatics from stab
	 * print .eb here if .bb printed for this block
	 */
# ifndef NOSYMB
	if( !dsflag && bb_flags[blevel] ) {
		fprintf( outfile, "	.def	.eb;	.val	.;	.scl	%d;	.line	%d;	.endef\n", C_BLOCK, LINENO );
		bb_flags[blevel] = 0;
		}
# endif
	}


bfdebug( a, n, dsflag )
int a[];
int n;
int dsflag;				/* 0 to allow extra output */
{
	register i;
	register struct symtab *p;

	/* remember name of current function */

	(void) strcpy( fncname, stab[curftn].sname );

	/* initialize line number counters */

	oldln = startln = lineno;
	strcpy( startfn, ftitle );

	/* do .bf symbol and defs for parameters
	 * parameters are delayed to here to two reasons:
	 *    1: they appear inside the .bf - .ef
	 *    2: the true declarations have been seen
	 */
	if (!dsflag)
	{
		int line;

		fprintf( outfile, "	.ln	1\n");
		line = lineno;	/* line number information for sdb */
		fprintf( outfile, "	.def	.bf;	.val	.;	.scl	%d;	.line	%d;	.endef\n", C_FCN ,line );
		for( i=0; i<n; ++i ) {
			p = &stab[a[i]];
			prdef(p,0);
		}
	}
}

/* output end of function debugging information */

efdebug()
{
	/* print end-of-function pseudo and its line number */

	fprintf( outfile, "	.def	.ef;	.val	.;	.scl	%d;	.line	%d;	.endef\n", C_FCN, LINENO );
	if( LINENO > 1 )
		fprintf( outfile, "	.ln	%d\n", LINENO );
}

eobl2dbg()
{
	/* print debug information at lexical end of function */

    fprintf( outfile, "\t.def	%s;	.val	.;	.scl	-1;	.endef\n", fncname);
    return;
}


/* called at end of job to output debug information */
ejsdb()
{
}

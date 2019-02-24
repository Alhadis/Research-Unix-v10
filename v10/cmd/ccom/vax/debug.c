# include "mfile1.h"
# include "debug.h"

/*	File where debugging information is collected and printed out */

/*	This file is for pi, mostly */

int gdebug, slineno;
extern int Oflag;

#define SYMTAB struct symtab

/* (year-80) month day(in dec), as 4 hexits */

static vernum = 0x4501;

static char verstr[] = "vaxpcc2";

static char strvalfmt[] = "	.stabs	\"%s\",0x%x,0,%d,%d\n";

static char strsymfmt[] = "	.stabs	\"%s\",0x%x,0,%d,%s\n";

static char nulvalfmt[] = "	.stabn	0x%x,0,%d,%d\n";

static char nulsymfmt[] = "	.stabn	0x%x,0,%d,%s\n";

static char dotfmt[]	= "	.stabd	0x%x,0,%d\n";

static char labstr[16];

static char *
maklab(val)
{
	sprintx(labstr, LABFMT, val);
	return labstr;
}

static
wasused( p )
register SYMTAB *p; 
{
	register i;

	switch( p->sclass ) {
	case STNAME:
	case UNAME:
	case ENAME:
		for (i = dimtab[p->sizoff+1]; i >= 0 && dimtab[i] >= 0; i++)
			if (wasused( &stab[dimtab[i]] ))
				return 1;
		return 0;
	default:
		return p->suse < 0;
	}
}

dbnargs( n )	/* number of argument bytes in call to a function */
{
	if (gdebug)
		printx(dotfmt, N_NARGS, n/SZCHAR );
}

dbfunbeg( p )	/* beginning of a function */
SYMTAB *p;
{
	dbfile(exname(p->sname));
	printx(strsymfmt, p->sname, N_BFUN, lineno, exname(p->sname));
	ppstab(p);
}

dbfunarg( p )	/* called for each argument of a function */
SYMTAB *p; 
{
	ppstab( p );		/* we can pick up the reg later */
}

dbfunret()	/* return from a function */
{
	if (gdebug) {
		dbfile(NULL);
		printx(dotfmt, N_RFUN, slineno ? slineno : lineno );
		slineno = 0;
	}
}

dbfunend( lab )	/* end of a function */
{
	register SYMTAB *p = &stab[curftn];

	printx("%s:", maklab(lab));
	dbfile(labstr);
	if(gdebug)
		printx(strsymfmt, p->sname, N_EFUN, lineno, labstr);
}	

dblbrac()	/* block level when a { is seen */
{
	if (gdebug)
		printx(dotfmt, N_LBRAC, blevel);
}

dbrbrac()	/* block level when a } is seen */
{
	if (gdebug)
		printx(dotfmt, N_RBRAC, blevel);
}

aobeg()	/* called before printing out the autos */
{
}

aocode(p)
register struct symtab *p; 
{
	/* called when automatic p removed from stab */

	switch( p->sclass ) {
	case REGISTER:
	case AUTO:
	case STATIC:
	case EXTDEF:
	case STNAME:
	case UNAME:
	case ENAME:
		if(!ISFTN(p->stype) && (gdebug || wasused(p)))
			ppstab( p );
		return;
	case EXTERN:
		/*	for now, do not report externs	*/
		return;
	}
}

aoend()
{
}

# ifndef OUTREGNO
# define OUTREGNO(p) ((p)->offset)
# endif

#define BYTOFF(p)	(((off = (p)->offset) < 0 ? -off : off)/SZCHAR)

ppstab( p )
register SYMTAB *p; 
{
	/* write out .stabs for the symbol p */
	register TWORD t;
	register off, i;

	if( p->stype == TNULL ) return;

	if(gdebug && p->sclass & FIELD) {
		printx(strvalfmt, p->sname, N_SFLD,
			((p->sclass&FLDSIZ)<<BTSHIFT)|p->stype, p->offset);
		return;
	}

	switch( p->sclass ) {
	case AUTO:
		printx(strvalfmt, p->sname, N_LSYM, p->stype, BYTOFF(p));
		break;
	case REGISTER:
		printx(strvalfmt, p->sname, N_RSYM, p->stype, OUTREGNO(p));
		break;
	case PARAM:
		printx(strvalfmt, p->sname, N_PSYM, p->stype, BYTOFF(p));
		break;
	case EXTERN:
	case EXTDEF:
		if(gdebug)
			printx(strvalfmt, p->sname, N_GSYM, p->stype, 0);
		break;
/*
	case EXTDEF:
		printx(strsymfmt, p->sname, N_GSYM, p->stype,
			exname(p->sname));
		break;
*/
	case STATIC:
		if (ISFTN(p->stype))
			printx(strvalfmt, p->sname, N_STFUN, p->stype, 0);
		else
			printx(strsymfmt, p->sname,
			    (p->sflags&SBSS) ? N_LCSYM : N_STSYM, p->stype,
			    p->slevel ? maklab(p->offset) : exname(p->sname));
		break;
	case STNAME:
	case UNAME:
	case ENAME:
		if(!gdebug)
			break;
		printx(strvalfmt, p->sname, N_BSTR, p->stype, 0);
		for (i = dimtab[p->sizoff+1]; i >= 0 && dimtab[i] >= 0; i++)
			ppstab( &stab[dimtab[i]] );
		printx(strvalfmt, p->sname, N_ESTR, p->stype,
			dimtab[p->sizoff]/SZCHAR);
		return;
	case MOS:
	case MOU:
		if(gdebug)
			printx(strvalfmt, p->sname, N_SSYM, p->stype, BYTOFF(p));
		break;
	case MOE:
		if(gdebug)
			printx(strvalfmt, p->sname, N_SSYM, p->stype, p->offset);
		return;
	case TYPEDEF: /* !? */
		return;
	default:
		fprintf(stderr,"unexpected stab class %d: %s, type = 0x%04x\n",
			p->sclass, p->sname, p->stype);
		fflush(stderr);
		return;
	}
	/* make another entry to describe structs, unions, enums */
	switch( BTYPE(p->stype) ) {
	case STRTY:
	case UNIONTY:
	case ENUMTY:
		if(gdebug)
			printx(strvalfmt, stab[dimtab[p->sizoff+3]].sname,
				N_TYID, 0, 0);
	}

	/* make other entries with the dimensions */
	if(!gdebug)
		return;
	for( t=p->stype, i=p->dimoff; t&TMASK; t = DECREF(t) ) 
	{
		if( ISARY(t) ) printx(nulvalfmt, N_DIM, 0, dimtab[i++]);
	}
}

static char orgfile[100], curfile[100], prtfile[100];

static int srcfilop = N_SO;

static char *
makstr(ip)
register char *ip;
{
	register c; register char *jp = prtfile;
	do {
		if ((c = *ip++) != '"')
			*jp++ = c;
	} while (c);
	return prtfile;
}

dbfile(pname)
char *pname;
{
	int seg;
	if (strcmp(curfile, ftitle) == 0)
		return 0;
	strcpy(curfile, ftitle);
	seg = locctr(PROG);
	if(!Oflag) {	/* Ln: .stabs...Ln confuses c2 */
		if (pname == NULL)
			printx("%s:", pname = maklab(getlab()));
		printx(strsymfmt, makstr(curfile), srcfilop,
		    slineno ? slineno : lineno, pname);
	}
	slineno = 0;
	if (srcfilop == N_SO) {	/* first file */
		printx(strvalfmt, verstr, N_VER, vernum, time(0));
		strcpy(orgfile, ftitle);
		srcfilop = N_SOL;
	}
	if (seg >= 0)
		locctr(seg);
	return 1;
}

dbline()
{
	int seg;
	if (blevel && !dbfile(NULL) && gdebug) {
		seg = locctr(PROG);
		printx(dotfmt, N_SLINE, slineno ? slineno : lineno);
		slineno = 0;
		if (seg >= 0)
			locctr(seg);
	}
}

ejsdb()
{
	/* called at the end of the entire file */
	register struct symtab *p;
	extern Pflag, bbcnt;
	int i;
	if(Pflag) {
		printx("\t.data\nlocprof:\t.long %d\n", bbcnt+4);
		printx("\t.long 0\n\t.long L%db\n", bbcnt);
		printx("\t.space %d\n", 4*bbcnt+4);
		printx("L%db:\t.byte ", bbcnt);
		for(i = 0; curfile[i]; i++)
			printx("0x%x,", curfile[i]);
		printx("0\n");
	}
	for( p = &stab[SYMTSZ]; --p>=stab; )
	{
		if( p->stype != TNULL && !ISFTN(p->stype) ) 
		{
			switch( p->sclass ) 
			{
			case EXTERN:
				if( p->suse > 0 ) continue;	/* unused */
			case EXTDEF:
			case STATIC:
			case STNAME:
			case UNAME:
			case ENAME:
				if (gdebug || wasused(p))
					ppstab(p);
				break;
			}
		}
	}
	printx("	.text\n");
	printx("%s:", maklab(getlab()));
	printx(strsymfmt, makstr(orgfile), N_ESO, lineno, labstr);
}

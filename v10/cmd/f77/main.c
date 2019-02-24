char xxxvers[] = "\n@(#) FORTRAN 77 PASS 1, VERSION 2.10.2,  27 JUNE 1989\n";
#define	VER	0x5625	/* for pi; 8YMDD */

#include "defs"
#include <signal.h>

#ifdef SDB
#	include <a.out.h>
#	ifndef N_SO
#		include <stab.h>
#	endif

int types3[] = {	/* for supplying type to N_GSYM stab line */
	0,	 /* TYUNKNOWN */
	0,	 /* TYADDR */
	0100|3,	 /* TYSHORT */
	0100|5,	 /* TYLONG */
	0100|6,	 /* TYREAL */
	0100|7,	 /* TYDREAL */
	0100|16, /* TYCOMPLEX */
	0100|16, /* TYDCOMPLEX */
	0100|4,	 /* TYLOGICAL */
	0100|16, /* TYCHAR */
	0100|4,	 /* TYSUBR */
	0	 /* TYERROR */
};

int complex_seen, dcomplex_seen;
#endif

LOCAL clfiles();
LOCAL FILEP opf();

int bugwarn;

main(argc, argv)
int argc;
char **argv;
{
	char *s;
	int k, retcode, *ip;
#if SDB
	char elab[10];
	int elnum;
#endif
	void flovflo();

#define DONE(c)	{ retcode = c; goto finis; }

	signal(SIGFPE, flovflo);  /* catch overflows */

#if HERE == PDP11
	ldfps(01200);	/* trap on overflow */
#endif



	--argc;
	++argv;

	while(argc>0 && argv[0][0]=='-')
	{
		for(s = argv[0]+1 ; *s ; ++s) switch(*s)
		{
		case 'w':
			if(s[1]=='6' && s[2]=='6')
			{
				ftn66flag = YES;
				s += 2;
			}
			else
				nowarnflag = YES;
			break;

		case 'U':
			shiftcase = NO;
			break;

		case 'u':
			undeftype = YES;
			break;

		case 'O':
			optimflag = YES;
			if( isdigit(s[1]) )
			{
				k = *++s - '0';
				if(k > MAXREGVAR)
				{
					warn1("-O%d: too many register variables", k);
					maxregvar = MAXREGVAR;
				}
				else
					maxregvar = k;
			}
			break;

		case 'b':
			bugwarn |= 1;
			break;

		case 'B':
			bugwarn |= 2;
			break;

		case 'd':
			debugflag = YES;
			break;

		case 'p':
			profileflag = YES;
			break;

		case 'C':
			checksubs = YES;
			break;

		case '6':
			no66flag = YES;
			noextflag = YES;
			break;

		case '1':
			onetripflag = YES;
			break;

#ifdef SDB
		case 'g':
			sdbflag = YES;
			break;
#endif

		case 'N':
			switch(*++s)
			{
			case 'q':
				ip = &maxequiv; 
				goto getnum;
			case 'x':
				ip = &maxext; 
				goto getnum;
			case 's':
				ip = &maxstno; 
				goto getnum;
			case 'c':
				ip = &maxctl; 
				goto getnum;
			case 'n':
				ip = &maxhash; 
				goto getnum;

			default:
				fatali("invalid flag -N%c", *s);
			}
getnum:
			k = 0;
			while( isdigit(*++s) )
				k = 10*k + (*s - '0');
			if(k <= 0)
				fatal("Table size too small");
			*ip = k;
			break;

		case 'I':
			if(*++s == '2') {
				tyint = TYSHORT;
#ifdef SDB
				types3[TYLOGICAL] = 0100|3;
#endif
			}
			else if(*s == '4')
			{
				shortsubs = NO;
				tyint = TYLONG;
#ifdef SDB
				types3[TYLOGICAL] = 0100|4;
#endif
			}
			else if(*s == 's')
				shortsubs = YES;
			else
				fatali("invalid flag -I%c\n", *s);
			tylogical = tyint;
			break;

		default:
			fatali("invalid flag %c\n", *s);
		}
		--argc;
		++argv;
	}

	if(argc != 4)
		fatali("arg count %d", argc);
	asmfile  = opf(argv[1]);
	initfile = opf(argv[2]);
	textfile = opf(argv[3]);

	initkey();
	if(inilex( copys(argv[0]) ))
		DONE(1);
	fprintf(diagfile, "%s:\n", argv[0]);

#ifdef SDB
#ifndef UCBPASS2
	for(s = argv[0] ; ; s += 8)
	{
		prstab(s,N_SO,0,0);
		if( strlen(s) < 8 )
			break;
	}
#else
	prstab(argv[0],N_SO,0,0);
#endif
	prstab("vaxf77",N_VER,VER,0);
#endif

	fileinit();
	procinit();
	if(k = yyparse())
	{
		fprintf(diagfile, "Bad parse, return code %d\n", k);
		DONE(1);
	}
	if(nerr > 0)
		DONE(1);
	if(parstate != OUTSIDE)
	{
		warn("missing END statement");
		endproc();
	}
	doext();
	preven(ALIDOUBLE);
	prtail();
#if SDB
	if (sdbflag)
		commstruct();	/* make commons appear to pi as global structs */
	sprintf(elab, "L%d", elnum = newlabel());
	putlabel(elnum);
	prstab(argv[0],N_ESO,lineno,elab);
#endif
#if FAMILY==PCC
	puteof();
#endif

	if(nerr > 0)
		DONE(1);
	DONE(0);


finis:
	done(retcode);
	return(retcode);
}



done(k)
int k;
{
	static int recurs	= NO;

	if(recurs == NO)
	{
		recurs = YES;
		clfiles();
	}
	exit(k);
}


LOCAL FILEP opf(fn)
char *fn;
{
	FILEP fp;
	if( fp = fopen(fn, "w") )
		return(fp);

	fatalstr("cannot open intermediate file %s", fn);
	/* NOTREACHED */
}



LOCAL clfiles()
{
	clf(&textfile);
	clf(&asmfile);
	clf(&initfile);
}


clf(p)
FILEP *p;
{
	if(p!=NULL && *p!=NULL && *p!=stdout)
	{
		if(ferror(*p))
			fatal("writing error");
		fclose(*p);
	}
	*p = NULL;
}




 void
flovflo()
{
	err("floating exception during constant evaluation");
#if HERE == VAX
	fatal("vax cannot recover from floating exception");
	/* vax returns a reserved operand that generates
	   an illegal operand fault on next instruction,
	   which if ignored causes an infinite loop.
	*/
#endif
	signal(SIGFPE, flovflo);
}

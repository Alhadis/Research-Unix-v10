static char ID[] = "@(#) ld00.c: 1.17 8/18/83";
#include "system.h"

#include <stdio.h>

#if TS || RT
#include <ar.h>
#else
#include <archive.h>
#endif

#include "structs.h"
#include "extrns.h"
#include "list.h"
#include "ldfcn.h"
#include "sgs.h"
#include "sgsmacros.h"
#include "paths.h"
#include "y.tab.h"

extern FILE *yyin;	/* current lex input file */
extern char inline[];	/* current input line up to current token */
extern int yychar;	/* current yacc lookahead token number */

extern int in_y_exp;	/*
			 *	See explanation in ld.yac.
			 *	We must initialize this var. to zero
			 *	before parsing an ifile.
			 */

#if FLEXNAMES
#	define STRINCRE 2048
#else
#	define STRINCRE 1024
#endif
#define MAXINFLS 16
#define MAXDLEN 64	/* maximum lib search directory length*/

#ifndef LLIBDIR
#define LLIBDIR LIBDIR
#endif
#ifndef NDELDIRS
#define NDELDIRS	1
#endif

extern char *realloc();

static int nldirs = NDELDIRS;  /* number of -L options used    */
static char *libdirs[MAXLDIRS] = {	/* directories searched for libraries */
			LLIBDIR
#ifdef LLIBDIR1
			,LLIBDIR1
#endif
			};


/*
 * A stack is kept of input ifile discriptors so that
 * ifiles may include other ifiles.
 *
 * NOTE: currently, this only works if the ifile name
 *		is the last statement in an ifile, because YACC does
 *		not return from a recursive call properly.
 */
typedef struct instr INSTR;
struct instr {
	FILE *infildes;		/* file descriptor */
	char *infilnm;		/* ptr to file name */
	int lastlnno;		/* last line number read in the file */
	};
char whichin = 0;		/* index int instack */
INSTR instack[MAXINFLS+1];	/* stack of input ifiles */
/*eject*/
filespec(filename)
char *filename;
{

/*
 * This routine is called when there is an input directive to load a 
 * file. It checks the type of file, to distinguish between an ifile
 * and an object file
 */

	FILE *filedes;
#if PORTAR || PORT5AR
	unsigned short	firstword;
	char		atype[SARMAG];
#else
	unsigned short	firstword,
			atype;
#endif

#if DEBUG
	if (dflag > 1)
		fprintf(stderr, "filespec : %s\n", filename);
#endif

	if ((filedes = fopen(filename,"r")) == NULL) {
		lderror(2,0,sname(curfilnm),"Can't open file %s for input", filename);
		return;
		}

	in_y_exp = 0;

	if (fread( (char *) &firstword, sizeof(firstword), 1, filedes) != 1)
		lderror(2,0,NULL,"Can't read 1st word of file %s",filename);
#if PORTAR || PORT5AR
	fseek(filedes,0L,0);
	if (fread(atype,SARMAG,1,filedes) != 1)
		atype[0] = '\0';
#else
	if ( sizeof(firstword) != sizeof(ARMAG) )
		if ( fread( (char *)&atype, sizeof(atype), 1, filedes) != 1)
			atype = firstword;
#endif

	if (ISMAGIC(firstword)) {
		bldldfil( savefn(filename), 0 );
		fclose(filedes);
		}
#if PORTAR || PORT5AR
	else if (strncmp(atype,ARMAG,SARMAG) == 0) {
#else
	else if ( (firstword == (unsigned short) ARMAG) ||
			((sizeof(firstword) != sizeof(ARMAG)) &&
			 (atype == (unsigned short) ARMAG)) ) {
#endif
		bldldfil( savefn(filename), 1 );
		fclose(filedes);
		}
	else if (BADMAGIC(firstword)) {
		yyerror("file %s is of unknown type: magic number = %6.2x",
			filename, firstword);
		fclose(filedes);
		}
	else {
		/*
		 * Assume that the file is an ifile
		 */
		instack[whichin].infilnm = curfilnm;
		instack[whichin].lastlnno = lineno;
		instack[whichin].infildes = filedes;
		curfilnm = savefn(filename);
		lineno = 1;
		if( ++whichin == MAXINFLS ) {
			yyerror("ifile nesting limit exceeded with file %s", filename);
			fclose(filedes);
			curfilnm = instack[--whichin].infilnm;
			lineno = instack[whichin].lastlnno++;
			return;
			}

		if (fseek(filedes, 0L, 0) != 0)
			lderror(2,0,NULL,"Can't seek to the beginning of file %s",
			filename);
		yyin = filedes;

#if DEBUG
		if( dflag )
			fprintf(stderr, "reading cmds from file %s (%.2o)\n",
			      filename, filedes);
#endif

		yyparse();
		close(filedes);

		yyin = instack[--whichin].infildes;
		curfilnm = instack[whichin].infilnm;
		lineno = instack[whichin].lastlnno++;
		yychar = -1;

#if DEBUG
		if( dflag )
			fprintf(stderr, "returning input to fildes %s (%.2o)\n",
				curfilnm, yyin);
#endif

		}
}
/*eject*/
bldldfil(fname, typflag)
char *fname;
int typflag;
{

/*
 * Build a LDFILE action item
 */

	ACTITEM *p;

	p = (ACTITEM *) myalloc(sizeof(ACTITEM));

	p->ldlbry.aiinflnm = curfilnm;
	p->ldlbry.aiinlnno = lineno;
	p->ldlbry.aitype = typflag ? AILDLBRY : AILDFILE;
	p->ldlbry.aifilnam = fname;

	listadd(l_AI, &ldfilist, p);
}




bldadscn(scname, fname, sp)
char *scname;
char *fname;
ACTITEM *sp;		/* ptr to DFNSCN act_item */
{

/*
 * Build a ADDSCN action item
 */

	ACTITEM *p;

	p = (ACTITEM *) myalloc(sizeof(ACTITEM));

	p->addscn.aitype = AIADDSCN;
	p->addscn.aiinflnm = curfilnm;
	p->addscn.aiinlnno = lineno;
	copy(p->addscn.ainame, scname, sizeof(p->addscn.ainame));
	p->addscn.aiscfile = fname;

	listadd(l_AI, &(sp->dfnscn.sectspec), p);
}




ACTITEM *
bldadfil(fname, sp)
char *fname;
ACTITEM *sp;		/* ptr to DFNSCN act_item */
{

/*
 * Build a ADFILE action item
 */

	ACTITEM *p;

	p = (ACTITEM *) myalloc(sizeof(ACTITEM));

	p->adfile.aitype = AIADFILE;
	p->adfile.aiinflnm = curfilnm;
	p->adfile.aiinlnno = lineno;
	p->adfile.aifilnam = fname;

	listadd(l_AI, &(sp->dfnscn.sectspec), p);

	return (p);
}




bldexp(ep, lp)
ENODE *ep;
LISTOWN *lp;
{

/*
 * Build an EVEXPR action item
 */

	ACTITEM *p;

	p = (ACTITEM *) myalloc(sizeof(ACTITEM));

	p->evexpr.aitype = AIEVEXPR;
	p->evexpr.aiinlnno = lineno;
	p->evexpr.aiinflnm = curfilnm;
	p->evexpr.aiexptr = ep;

	listadd(l_AI, lp, p);
}




bldsym(sym, value, op)
char *sym;
long value;
int op;
{

/*
 * Build an DFNSYM action item
 */

	ACTITEM *p;

	p = (ACTITEM *) myalloc(sizeof(ACTITEM));

	p->dfnsym.aitype = AIDFNSYM;
	p->dfnsym.aiinlnno = lineno;
	p->dfnsym.aiinflnm = curfilnm;
	p->dfnsym.aisymbol = savefn(sym);
	p->dfnsym.aideflag = op;
	p->dfnsym.aidefval = value;

	listadd(l_AI, &symlist, p);
}
/*eject*/
pflags(flgname, ifile) 
char *flgname;		/* ptr to x, extracted from -x */
int ifile;		/* set TRUE iff the flag comes from an ifile */
{

/*
 * Process flag specifications
 *
 * Note that flags can come from two sources:
 *	command line : everything is character strings
 *	       ifile : parser has already converted tokens to binary
 */

	register int type;


/*
 *	-a		: make the output file "executable":
 *				1. complain about unresolved references
 *				2. define several "_xxxx" symbols
 *
 *	-cn		: increment the partial execution flag.  'n'
 *				defaults to 1:
 *				0 : normal, complete ld execution
 *				1 : execute parser of PASS 1 only
 *				2 : execute PASS 1 only
 *				3 : execute through allocation (up
 *					to output())
 *				4 : complete ld execution, with
 *					/dev/null as the output
 *					file name
 *
 *	-dn (-b for u3b): increment the internal ld debug flag. 'n'
 *				defaults to 1
 *
 *	-e name		: generate a UNIX a.out header in the optional
 *				header field of the output file, and
 *				set the "entry point" entry to the 
 *				address of the symbol "name".  This
 *				flag forces the flag -X
 *
 *	-f 0xaabb	: use the two-byte value aabb to:
 *				1. fill "holes" between sections
 *				2. fill uninitialized .bss sections
 *
 *	-h nnn		: make the optional header field nnn bytes in
 *				size. Zero fill any unused bytes of this
 *				header
 *
 *	-i		: ***** b16ld and x86ld only *****
 *				use two regions, to give separate I and
 *				D space (64 K each)
 *
 *	-lx		: search the library libx.a in directory
 *				libdirs[n] for any symbols which can
 *				satisfy outstanding unresolved references.
 *
 *	-m		: generate a memory map
 *
 *	-o name		: use "name" as the name of the output file.
 *				The default value is given by the
 *				constant A.OUT, defined in the include
 *				file paths.h
 *
 *	-p[nnn]		: generate a patch list. The size of the
 *				optional header is increased by the
 *				size of the patch list
 *			  the output section size is set to nnn
 *				after allocation and patch list
 *				construction to save physical memory
 *
 *	-r		: keep relocation information in the output file
 *
 *	-s		: strip all relocation and symbol table 
 *				information from the output file
 *
 *	-t		: ****** UNIX only ******
 *			        do type checking

 *	-tv		: ***** b16ld/x86ld/n3bld only *****
 *				generate TV function linkage
 *
 *	-x (only u3b)	: do not preserve symbols in the output symbol
 *				table except static or external symbols
 *
 *	-u name		: add "name" to ld's symbol table, as an
 *				undefined symbol
 *
 *	-z		: ****** UNIX only ******
 *				leave memory around zero unconfigured ro catch
 *			  null pointers
 *
 *	-B nn		: generate "padding" output sections of nn bytes
 *				of zero following any output section
 *				which is either:
 *				1. of zero length
 *				2. all of uninitialzed .bss 
 *
 *	-F		: ****** UNIX only ******
 *				indicate in header file for paging, block
 *				for paging, memory configure for paging, etc.
 *
 *	-H		: change the type of all global symbol table
 *				entries to "static"
 *
 *	-L<path>	: provides alternate directories and orders
 *			  search path for libraries (equivalent to
 *			  -I flag for cpp includes). -L must precede
 *			  -l to be effective.
 *
 *	-M		: ****** UNIX only *****
 *				output message if external variables are
 *				multiply defined
 *
 *	-N		: squash text and data together
 *
 *	-S		: "silent" mode: only fatal errors will be 
 *				listed - not warnings or diagnostics
 *
 *	-V		: output current ld environment definition. '-V'
 *				flags are cumulative:
 *				1 : describe the ld version
 *				2 : describe the machine and os version
 *
 *	-VS nn		: put nn as version stamp in the optional header
 *
 *	-X		: generate, in the optional header field, a
 *				standard UNIX a.out header
 *				(except for u3b, where it means
 *				 elimate compiler generated labels)
 *
 *	-ild		: generate an "extra" output section for each
 *				unallocated area of configured memory
 *
 */

	switch (flgname[0]) {
#if !UNIX || M32
		case 'a':
			aflag++;
			break;
#endif
#if !ONEPROC
		case 'c':
			type = 1;
			if( flgname[1] )
				if( (type = stoi(&flgname[1])) == 0x8000 ) {
					yyerror("-c flag does not specify a number: %s", &flgname[1]);
					type = 1;
					}
			cflag += type;
			break;
#endif
#if DEBUG
		case 'd':
#if UNIX
			lderror(0,0,NULL,"-d flag is not implemented");
			break;
		case 'b':
#endif
			type = 1;
			if( flgname[1] )
				if( (type = stoi(&flgname[1])) == 0x8000 ) {
					yyerror("-%c flag does not specify a number: %s", flgname[0],flgname[1]);
					type = 1;
					}
			dflag += type;
			break;
#endif
		case 'e':
			if( ifile )
				if (yylex() != NAME)
					yyerror("-e flag does not specify a legal symbol name: %s", inline);
				else
#if FLEXNAMES
					strcpy(epsymbol, yylval.sptr);
#else
					copy(epsymbol, yylval.sptr, 8);
#endif
			else {
#if FLEXNAMES
				strcpy(epsymbol, *++argptr);
#else
				copy(epsymbol, *++argptr, 8);
#endif
				argcnt--;
				}
			Xflag++;
#if DEBUG
			if (dflag)
				fprintf(stderr, "a.out entry point symbol: %s\n",
					epsymbol);
#endif
			break;
		case 'f':
			if( ifile )
				if (yylex() != LONGINT)
					yyerror("-f flag does not specify a two-byte number: %s", inline);
				else
					globfill = (short) *yylval.lptr;
			else {
				if( (globfill = (short) stoi(*++argptr)) == (short) 0x8000 ) {
					yyerror("-f flag does not specify a two-byte number: %s", *argptr);
					globfill = 0;
					}
				argcnt--;
				}
#if DEBUG
			if (dflag)
				fprintf(stderr, "global fill: %.2x\n",
					globfill);
#endif
			break;
#if !UNIX
		case 'h':
			if( ifile )
				if (yylex() != LONGINT)
					yyerror("-h flag does not specify a numeric value: %s", inline);
				else 
					hflag = *yylval.lptr;
			else {
				if( (hflag = stoi(*++argptr)) == 0x8000 ) {
					yyerror("-h flag does not specify a numeric value: %s", *argptr);
					hflag = 0;
					}
				argcnt--;
				}
			if( hflag < 0 ) {
				yyerror("invalid value on -h flag: %.2x", hflag);
				hflag = 0;
				}
#if DEBUG
			if(dflag)
				fprintf(stderr, "optional header size: %d\n",
					hflag);
#endif
			break;
#endif
		case 'i':
#if IANDD
			if (flgname [1] == '\0')
				{
				iflag++;
				break;
				}
#endif
#if ILDOPT
			if (flgname [1] == 'l' && flgname [2] == 'd')
				{
				ildflag++;
				break;
				}

			yyerror("%s is an invalid flag",flgname);
#endif
			break;
		case 'l':
			if( flgname[1] != '\0' ) {
				library(flgname);
				}
			else  {
				specflags(flgname, argptr);
				}
			break;
		case 'm':
			mflag++;
			break;
#if !UNIX
		case 'p':
			pflag = -1;	/* use default patch size */
			if( flgname[1] ) {
				if( (pflag = stoi(&flgname[1])) == 0x8000 ) {
					yyerror("-p flag does not specify a number: %s", &flgname[1]);
					pflag = -1;
				} else {
					if (pflag <= 0) {
						yyerror("-p flag size out of range: %s", &flgname[1]);
						pflag = -1;
						}
					} /* else */
				} /* if flgname */
			break;
#endif
		case 'o':	
			if( ifile ) {
				type = yylex();
				if( (type == FILENAME)  ||  (type == NAME) )
					copy(outfilnm, yylval.sptr, 128);
				else
					yyerror("-o flag does not specify a valid file name: %s", inline);
				}
			else {
				copy(outfilnm, *++argptr, 128);
				argcnt--;
				}
			if( outfilnm[127] != '\0' ) {
				outfilnm[127] = '\0';
				yyerror("-o file name too large (> 128 char); truncated to (%s)", outfilnm);
				}
#if DEBUG
			if(dflag)
				fprintf(stderr, "output file name: %s\n",
					outfilnm);
#endif
			break;
		case 'r':
			rflag++;
			break;
		case 's':
			sflag++;
			break;
		case 't':
#if TRVEC
			if (flgname[1] == 'v') {
				tvflag = 1;
				magic = (unsigned short) TVMAGIC;
				break;
				}
#endif
#if COMMON
			tflag++;
#endif
			break;
		case 'u':	
			if( ifile )
				if( yylex() != NAME )
					yyerror("-u flag does not specify a legal symbol name: %s", inline);
				else
					bldsym(yylval.sptr, 0L, 0);
			else {
				bldsym(*++argptr, 0L, 0);
				argcnt--;
				}
			break;
#if PAGING
		case 'z':
			zflag++;
			break;
#endif
#if !UNIX
		case 'B':
			if( ifile )
				if (yylex() != LONGINT)
					yyerror("-B flag does not specify a number: %s", inline);
				else
					Bflag = *yylval.lptr;
			else {
				if( (Bflag = stoi(*++argptr)) == 0x8000 ) {
					yyerror("-B flag does not specify a number: %s", *argptr);
					Bflag = 0;
					}
				argcnt--;
				}
#if DEBUG
			if (dflag)
				fprintf(stderr, "pad-section size: %.2x\n",
					Bflag);
#endif
			break;
		case 'H':
			Hflag++;
			break;
#endif
#if PAGING
		case 'F':
			Fflag++;
			break;
#endif
#if COMMON
		case 'M':
			Mflag++;
			break;
#endif
		case 'N':
			Nflag++;
			break;
		case 'L':
			if ( nldirs >= MAXLDIRS )
				yyerror("too many -L options, %d allowed", MAXLDIRS-1);
			else if ( flgname[1] == NULL )
				yyerror("no directory given with -L");
			else if ( strlen(&flgname[1]) > MAXDLEN )
				yyerror("-L path too long(%s)", &flgname[1]);
			else {
				libdirs[nldirs] = myalloc(size(&flgname[1]));
				strcpy(libdirs[nldirs++], &flgname[1]);
				}
				break;
		case 'S':
			Sflag++;
			break;
		case 'V':
			if (flgname[1] == 'S')  {
				if( ifile )
					if (yylex() != LONGINT)
						yyerror("-VS flag does not specify a numeric value: %s", inline);
					else 
						VSflag = *yylval.lptr;
				else {
					if( (VSflag = stoi(*++argptr)) == 0x8000 ) {
						yyerror("-VS flag does not specify a numeric value: %s", *argptr);
						VSflag = 0;
						}
					argcnt--;
					}
				}
			else  {
				Vflag++;
				}
			break;
#if !UNIX
		case 'X':
			Xflag++;
			break;
#endif
		default:
			specflags(flgname, argptr);
		}
}
/*eject*/
library(fname)
char *fname;
{

/*
 * For a library flag of the form "-lx", build a file name of the form
 *
 *		"libdirs[n]/libx.a"
 *
 * where libdirs[0] is LIBDIR (defined in paths.h) and libdirs[n>0]
 * are defined using the -L option. Then build a LDFILE action item.
 */

	static char libname[MAXDLEN+16];	/* -lx file name */
	int dndx;
	char *p;
	FILE *lfd;

	if ( *(++fname) == '\0' ) {
		yyerror("the -l flag (specifying a default library) is not supported");
		return;
		}
	else
		p = fname;

	/*
	 *  Loop thru libdir[NDELDIRS] thru libdir[nldirs-1]
	 */
	for ( dndx = NDELDIRS; dndx < nldirs; dndx++)  {
		sprintf(libname, "%s%s%.8s%s", libdirs[dndx], "/lib", p, ".a");
		if ( (lfd = fopen(libname,"r")) != NULL)
			goto liblab;
		}

	/*
	 *  loop thru libdirs[0] thru libdirs[NDELDIRS-1]
	 */
	for ( dndx = 0; dndx < NDELDIRS; dndx++)  {
		sprintf(libname, "%s%s%.8s%s", libdirs[dndx], "/lib", p, ".a");
		if ( (lfd = fopen(libname,"r")) != NULL)
			goto liblab;
		}

	yyerror("can't find library lib%s.a", p);
	return;

	liblab:
		fclose(lfd);
		bldldfil(savefn(libname), 1);
}
/*eject*/
char *
savefn(name)
char *name;
{

/*
 * Save the name of an ifile or input *.o or archive file
 */

	char *strbeg;
	int length;

	length = strlen(name) + 1;
	if (strnext + length > strlimit)
	{
#if ONEPROC
		strbase = myalloc( STRINCRE );
		strnext = strbase;
		strlimit = strbase + STRINCRE;
#else
		lderror( 0, 0, NULL, "too many strings, out of space" );
		lderror( 2, 0, NULL, "%sld run is too large and complex", SGS );
#endif
	}
	strbeg = strnext;
	strncpy( strbeg, name, length );
	strnext += length;

	return( strbeg );
}

/* @(#) local.c: 1.5 6/26/84				*/

/* # include <ctype.h> -- optim.h takes care of this */
# include "optim.h"
# include "sgs.h"
# include "paths.h"

# define ASDATE ""
# define LINELEN 400

# define FATAL(S)	fatal((S), (char *) 0)

char fixstr[256];
char line[LINELEN];
char * linptr;		/* pointer to current line */
int fixlen;
char * deflab;
int opn;
int m;
int numauto;		/* number of bytes of automatic vars. */
int numnreg;		/* number of registers */
boolean indata;
int lineno = IDVAL;
int nusel = 0;		/* semantically useless instructions */
int nspinc = 0;		/* useless sp increments */
int nmc = 0;		/* move followed by compare */
int nmal = 0;		/* move followed by arithmetic or logical */
int nredcmp = 0;	/* redundant compares */
int nadpsh = 0;		/* addw3 or subw3 followed by push */
int nadmsh = 0;		/* addw3 or subw3 followed by mov */
int nadmv = 0;		/* replace addw3 or subw3 by mov */
extern int ndisc;

#ifdef IMPIL
zflag = false;		/* debug flag for in--line expansion */
#endif /* IMPIL */

# define numops 151

char *optbl[numops] = {
	"acjl","acjle","acjleu","acjlu",
	"addb2","addb3","addh2","addh3","addw2",
	"addw3","alsw2","alsw3","andb2","andb3",
	"andh2","andh3","andw2","andw3","arsw2",
	"arsw3","atjnzb","atjnzh","atjnzw","bitw",
	"call","cmpb","cmph","cmpw",
	"decpth","decptw","divw2",
	"divw3","extzv","faddd2","faddd3","fadds2",
	"fadds3","fcmpd","fdivd2","fdivd3","fdivs2",
	"fdivs3","fmuld2","fmuld3","fmuls2","fmuls3",
	"fsubd2","fsubd3","fsubs2","fsubs3",
	"incpth","incptw","inctst","insv",
	"jbc","jbs","je","jg","jge",
	"jgeu","jgu","jl","jle","jleu",
	"jlu","jmp","jne","jneg","jnneg",
	"jnpos","jnz","jpos","jsb","jz",
	"llsw2","llsw3","lrsw2","lrsw3","mcomb",
	"mcomh","mcomw","mnegb","mnegh","mnegw",
	"modw2","modw3","movab","movah","movaw",
	"movb","movbbh","movbbw","movbhw","movblb",
	"movblh","movblw","movd","movdd","movds","movdw","movh",
	"movsd","movsw","movtdw","movthb","movtsw",
	"movtwb","movtwh","movw","movwd","movws",
	"movzbh","movzbw","movzhw","mulw2","mulw3",
	"orb2","orb3","orh2","orh3","orw2",
	"orw3","pushaw","pushbb","pushbh",
	"pushd","pushw","pushzb","pushzh","ret","save","subb2",
	"subb3","subh2","subh3","subw2","subw3",
	"switch","switcht",
	"udivw2","udivw3","umodw2","umodw3","umulw2",
	"umulw3","xorb2","xorb3","xorh2","xorh3",
	"xorw2","xorw3"
};

FILE *stmpfile;	/* Temporary storage for strings which are in the text
		 * section. The strings are collected and printed at
		 * the end of a function.  Requirement for field update */
extern char *mktemp();
FILE *tmpopen();
char tmpname[50];	/* name of file for storing string */
FILE *atmpfile; /* Temporary storage for input while scanning for presence of
		 * #ASM in function */
char atmpname[50];
int asmflag = false; /* indicates whether an 'asm' has been encountered */
long asmotell; /* location in the output file where the last function ended */
int oaflag = false; /* indicates whether -OA flag was entered */
boolean instring = false;
boolean inswitch = false;
#ifdef IMPIL
boolean swflag = false;		/* switch table appears in function */
NODE   	*lblnode = NULL;	/* pointer to last label node */
char 	*pcdecode();
#endif /* IMPIL */

yylex()
{
    extern char * strchr();

    register char * s;			/* temporary string ptr */
    register char * start;		/* another temporary string ptr */
    register int i;			/* temporary int */

#ifdef IMPIL
    ilinit();
#endif /* IMPIL */

    asmopen(); 				/* open temp file for asm treatment */
    linptr = NULL;			/* start off with no line */

    while (linptr != NULL || 
	((linptr = fgets(line,LINELEN,stdin)) != NULL && putasm(line)))
    {
	switch (*linptr)		/* dispatch on first char in line */
	{
	case '\n':			/* ignore new line */
	    break;
	    
	case '#':			/* check for special comments */
	    s = linptr + 1;		/* look at comment only, not # */

	    if (strncmp(s,"STRBEG",6) == 0)
		instring = true;	/* declare we're in a string */
	    else if (strncmp(s,"STREND",6) == 0)
		instring = false;	/* now out of string-ness */
	    else if (strncmp(s,"SWBEG",5) == 0)
		inswitch = swflag = true;	/* now in a switch table */
	    else if (strncmp(s,"SWEND",5) == 0)
		inswitch = false;	/* out of switch table */
	    else if (strncmp(s,"ASM",3) == 0)
		asmflag = true;		/* 'asm' appears in function */
#ifdef IMPREGAL
	    else if (strncmp(s,"REGAL",5) == 0)
		ratable( line );	/* register allocation opt */
#endif /* IMPREGAL */
		
	    break;			/* in any case, ignore line further */
	    
	case '.':			/* compiler label */
	default:			/* ordinary label */
	    if (indata)			/* in data section, just print */
	    {
		printf("%s",linptr);
		break;
	    }

	    /* reach here on labels in text space */

	    s = strchr(linptr,':');	/* find : */
	    if (s == NULL)		/* error if not found */
	    { FATAL("Bad input format\n"); }

	    *s++ = '\0';		/* change : to null, skip past */

/* for N3B (DMERT), switch code and jump tables are in text space,
** but SWBEG, SWEND bracket just the table.  For m32 and u3b, jump tables are
** always in data space.
** Write labels found when "inswitch" to special file for N3B only.
*/

#ifdef N3B
	    if (instring || inswitch)	/* inside special string or switch... */
#else
	    if (instring)		/* inside special string?... */
#endif	/* def N3B */
	    {
		putstr(linptr);		/* write label to special file */
		putstr(":\n");		/* put : back in */
	    }
	    else			/* normal text space label */
	    {
		applbl(linptr,s-linptr); /* append label node */
		lastnode->uniqid = IDVAL;
	    }
	    linptr = s;			/* continue processing past label */
	    continue;			/* next iteration */
/* handle more usual case of line beginning with space/tab */
	case ' ':
	case '\t':
	    for (s = linptr; isspace(*s); s++) /* skip white space */
		;
		
	    switch(*s)			/* dispatch on next character */
	    {
	    case '\0':			/* line of white space */
	    case '#':			/* comment */
	    case '\n':			/* new line */
		break;			/* ignore */
		
	    case '.':			/* start of pseudo-op */
		pseudo(s);		/* do pseudo-ops */
		break;

	    default:			/* normal instruction */
		if (indata)		/* in data section this is weird case */
		    printf("%s",linptr); /* just write line */
		else			/* normal instruction in text */
		{
		    for (start = s; ! isspace(*s); s++)
			;		/* skip over instruction mnemonic */

		    *s++ = '\0';	/* demarcate with null */

		    if ((opn = lookup(start)) == OTHER)
			saveop(0,start,s-start,opn);
					/* save funny instruction */
		    else
			saveop(0,optbl[m],0,opn);
					/* save normal inst. (m set by
					** lookup)
					*/
		    for ( ; isspace(*s); s++ )
			;		/* span white space */
		    opn = 1;		/* now save operands */
		    op(s);		/* skip intervening spaces/tabs */
		}
		/* remember program name */
#ifdef IMPIL
		if( ntail.back->op == SAVE ) lblnode = ntail.back->back;
#endif /* IMPIL */
		break;
	    }   /* end space/tab case */
	break;
	}	/* end first character switch */

	linptr = NULL;			/* indicate we're done with line */
    }   /* end while */
    return;				/* just exit */
}
op (s)
	register char *s;
{
	register char *t;

	while (*s)
	{
		/* scan over leading space/tabs */
		while (*s == ' ' || *s == '\t')
			s++;

		t = s;			/* remember effective operand start */
		while(*s != ',' && *s != '\n')	/* find end of operand */
			s++;
		*s++ = '\0';
		saveop(opn++, t, s-t, 0);
		if (*t == '.') /* operand is a label */
			if (inswitch)
				addref(t, s-t);
	}
	lastnode->uniqid = lineno;
	lineno = IDVAL;
}


pseudo (s)
	register char *s;		/* points at pseudo-op */
{
	void peep();			/* peephole improver */
	extern int cflag;		/* independent part -c flag:
					** enable/disable common tail
					*/
	register int pop;		/* pseudo-op code */
	char savechar;			/* saved char that follows pseudo */
	char * word = s;		/* pointer to beginning of pseudo */

/* The idea here is to stick in a NUL after the pseudo-op, then replace
** the character we clobbered, rather than copy the pseudo-op somewhere else.
*/

	for ( ; ! isspace(*s); s++ )
	    ;				/* scan to white space char */
	savechar = *s;			/* mark end of pseudo-op */
	*s = '\0';

	/* if we're in a data section, we write all pseudo-ops to output,
	** except for .word's, which we must examine, and .text .
	*/

	pop = plookup(word);		/* identify pseudo-op */
	*s = savechar;			/* restore saved character */
	if (indata) {			/* check pseudo-op possibilities */
		if (pop == TEXT) {
			indata = false;	/* not in data section anymore */
			return;
		}
		else if (pop != WORD) {
			printf("%s",linptr); /* output the line */
			return;
		}
	}

	switch (pop) {			/* dispatch on pseudo-op in text */
		case BYTE:
			putstr(linptr);
			break;
		case GLOBL:
		case TV:
			line[(m=length(line))-2] = '\0'; /* remove newline */
			appfl(line, m-1);
			break;
		case TEXT:
			indata = false;
			break;
		case FIL:
		case BSS:
			printf("%s", line);
			break;
		case LN:
			getln(line);
			break;
		case ALIGN:
			line[(m=length(line))-2] = '\0'; /* remove NL */
			if (inswitch) {
#ifdef N3B			/* for DMERT, put aligns in switch to special
				** output file with switch table
				*/
				line[m-2] = '\n';	/* restore new line */
				putstr(linptr);
#else
				saveop(0, line+1, m-2, OTHER);
				opn = 1;
				lastnode->uniqid = lineno;
				lineno = IDVAL;
#endif	/* def N3B */
			}
			else
				appfl(line, m-1);
			break;
		case SET:
			if (*++s == '.' && *++s == 'F') {
				printf("\t.text\n");
				setauto(line);
				printf("%s", line);
			}
			else if (s--, *s == '.' && *++s == 'R') {
				setnreg(line);
				line[(m=length(line))-2] = '\0';
				appfl(line,m-1);
			}
			else {
				line[(m=length(line))-2] = '\0';
				appfl(line, m-1);
			}
			break;
		case DATA:
			printf("%s", line);
			indata = true;
			break;
		case DEF: {
			int found;

			line[(m=length(line))-2] = '\0'; /* remove newline */
#ifdef IMPIL
			while( isspace( *s ) ) s++;
			if (*s++ == '.' && *s++ == 'e' && 
				*s++ == 'f') { /* .ef */
				appfl( "*", 2 );
				if( lblnode == NULL ) 
					{ FATAL( "No label preceeding .bf pseudo op, file may already be optimized\n" ); }
				appfl( lblnode->ops[0],
					length( lblnode->ops[0]));
			}
			s--;
#endif /* IMPIL */
			found = 0;
			while (true) {
				while (*s++ != ';');
				if (*(s += 2) == 'e') /* endef */
					break;
/* There are two special cases for ".val".  The first is
** ".val ." .  Since we're going to move the .def line, we need
** to replace the '.' with a real label (which we make a hard label)
** first.  The second special case is ".val .Lxxx" (a local label).
** This arises in the compiler from a user-defined label.  The
** problem here is that if the optimizer discards the local label,
** there will be a reference to a non-existent label which will only
** show up at link time.  To avoid this, we delete (ignore) the whole
** line.
*/
				else if (*s++ == 'v') { /* .val */
					if (s[3] == '.')
					{
					    if (s[4] == ';')	/* first case */
						found = 1;
					    else if (s[4] == 'L')
						found = 3;	/* second */
					}
				}
				else if (*s == 'c') /* .scl */
					if (*(s+3) == '-' && *(s+4) == '1') {
						found = 2;
						break;
					}
			}
			switch(found)
			{
			case 1:			/* .val . */
				deflab = (char *) newlab();
				deffix(line, deflab);
				appfl(fixstr, fixlen+1);
				fixlbl(deflab);
				applbl(deflab, length(deflab));
				lastnode->op = HLABEL;
				lastnode-> uniqid = IDVAL;
				break;
			case 2:			/* end of function */
				cflag = -1;	/* turn off common tail
						** merging:  improve speed,
						** not space.
						*/
				if( !asmflag || oaflag ) {
#ifdef IMPREGAL
					numnreg = raoptim(numnreg, numauto);
					numauto = raautos(numauto);
#endif /* IMPREGAL */
#ifdef IMPIL
					ilmark();
#endif /* IMPIL */
					filter();
					optim();
					peep();	/* do peephole improvements */
#ifdef IMPIL
					ilstat(numnreg, numauto);
#endif /* IMPIL */
					chkauto();
				}
				prtext();
				prstr();
				printf("%s", line);
				printf("\n");
				asmchk();
				init();
				break;
			case 3:			/* .val .Lxxx */
				break;		/* ignore line */
			default:		/* other .defs */
				appfl(line, m-1);
				break;
			}
		}
			break;
		case WORD:
			s++;			/* point past tab/nl */

/* we have to deal with whether .word is within a switch (SWBEG/SWEND) or
** not, and whether or not it appears in a data section.
*/

			if (inswitch) {		/* always add reference */
				register char * t = strchr(s,'\n');

				*t = '\0';	/* change NL to null */
				addref(s,t+1-s); /* add the reference */
				*t = '\n';	/* restore NL */
			}

			if (indata) 		/* in data, flush to output */
				printf("%s",linptr); /* print line */
			else if (inswitch)  	/* text (implicit) and switch */
				putstr(linptr);	/* flush to special file */
			else {			/* text, not in switch */
				saveop(0,".word",6,OTHER);
				opn = 1;	/* doing first operand */
				op(s);
			}
			break;

		default:			/* all unrecognized text
						** pseudo-ops
						*/
			if (! (instring || inswitch))
			    printf("%s", linptr); /* flush to output */
			else
			    putstr(linptr);	/* in(switch|string):  to
						** special file
						*/
			break;
	}
}

int
plookup(s)	/* look up pseudo op code */
	char *s;

{
/* Note:  to improve the linear search speed, these pseudo-ops
** are ordered by frequency of occurrence in a sample of C programs.
*/

	static char *pops[numpops] =
		{
		    ".word", ".byte", ".set", ".def", ".align",
		    ".data", ".text", ".globl", ".ln", ".bss",
		    ".file", ".tv"
		};

	static int popcode[numpops] =
		{
		    WORD, BYTE, SET, DEF, ALIGN,
		    DATA, TEXT, GLOBL, LN, BSS,
		    FIL, TV
		};

	register int i;

	for (i = 0; i < numpops; i++)
		if (!strcmp(s, pops[i]))
			return(popcode[i]);
	return(POTHER);
}


yyinit(flags) char * flags; {

	indata = false;
	for (; *flags != '\0'; flags++) {
		switch( *flags ) {
		case 'V':
			fprintf(stderr,"%s: optimizer - %s %s\n",
				SGSNAME,RELEASE,ASDATE);
			break;
		case 'A':
			oaflag = true;
			break;
#ifdef IMPIL
		case 'z':
			zflag = true;
			break;
#endif /* IMPIL */
		default:
			fprintf(stderr,"Optimizer: invalid flag '%c'\n",*flags);
		}
	}
}

char *
yysflgs( p ) char *p; { /* parse flags with sub fields */
	
	switch( *p ) {
#ifdef IMPIL
	case 'y':
		p++; /* skip over 'y' to sub option */
		p = pcdecode( p );
		return( p );
#endif /* IMPIL */
	default:
		return( p );
	}
}

int
lookup(op) char *op; { /* look up op code ordinal */

	register int f,l,om,x;
	static unsigned char ocode[numops] = {
		ACJL,ACJLE,ACJLEU,ACJLU,
		ADDB2,ADDB3,ADDH2,ADDH3,ADDW2,
		ADDW3,ALSW2,ALSW3,ANDB2,ANDB3,
		ANDH2,ANDH3,ANDW2,ANDW3,ARSW2,
		ARSW3,ATJNZB,ATJNZH,ATJNZW,BITW,
		CALL,CMPB,CMPH,CMPW,
		DECPTH,DECPTW,DIVW2,
		DIVW3,EXTZV,FADDD2,FADDD3,FADDS2,
		FADDS3,FCMPD,FDIVD2,FDIVD3,FDIVS2,
		FDIVS3,FMULD2,FMULD3,FMULS2,FMULS3,
		FSUBD2,FSUBD3,FSUBS2,FSUBS3,
		INCPTH,INCPTW,INCTST,INSV,
		JBC,JBS,JE,JG,JGE,
		JGEU,JGU,JL,JLE,JLEU,
		JLU,JMP,JNE,JNEG,JNNEG,
		JNPOS,JNZ,JPOS,JSB,JZ,
		LLSW2,LLSW3,LRSW2,LRSW3,MCOMB,
		MCOMH,MCOMW,MNEGB,MNEGH,MNEGW,
		MODW2,MODW3,MOVAB,MOVAH,MOVAW,
		MOVB,MOVBBH,MOVBBW,MOVBHW,MOVBLB,
		MOVBLH,MOVBLW,MOVD,MOVDD,MOVDS,MOVDW,MOVH,
		MOVSD,MOVSW,MOVTDW,MOVTHB,MOVTSW,
		MOVTWB,MOVTWH,MOVW,MOVWD,MOVWS,
		MOVZBH,MOVZBW,MOVZHW,MULW2,MULW3,
		ORB2,ORB3,ORH2,ORH3,ORW2,
		ORW3,PUSHAW,PUSHBB,PUSHBH,
		PUSHD,PUSHW,PUSHZB,PUSHZH,RET,SAVE,SUBB2,
		SUBB3,SUBH2,SUBH3,SUBW2,SUBW3,
		SWITCH,SWITCHT,
		UDIVW2,UDIVW3,UMODW2,UMODW3,UMULW2,
		UMULW3,XORB2,XORB3,XORH2,XORH3,
		XORW2,XORW3
		};

	f = 0;
	l = numops;
	om = 0;
	m = (f+l)/2;
	while (m != om) {
		x = strcmp(op,optbl[m]);
		if (x == 0)
			return((int) ocode[m]);
		else if (x < 0)
			l = m-1;
		    else
			f = m+1;
		om = m;
		m = (f+l)/2;
		}
	return(OTHER);
	}

deffix(str,lbl) register char * str, * lbl; { /*change "val .;" to "val ..n;"*/

	register char * nc;
	register boolean scanning;

	scanning = true;
	nc = fixstr;
	fixlen = 0;
	while (scanning)
		if (*str == '.' && *(str+1) == ';') {
			str++;
			scanning = false;
			}
		    else {
			*nc++ = *str++;
			fixlen++;
			}
	while (*nc++ = *lbl++)
		fixlen++;
	nc--;
	while (*nc++ = *str++)
		fixlen++;
	}

fixlbl(str) register char * str; { /* add colon to end of label */

	while (*str++);
	*--str = ':';
	*++str = '\0';
	}

int
length(str) register char * str; { /* return length of string (including \0) */

	register int l;

	l = 1;
	while (*str++)
		l++;
	return (l);
	}

getln(str) register char * str; { /* extract line number */

	while (!isdigit(*str))
		str++;
	lineno = 0;
	while (isdigit(*str)) {
		lineno = lineno * 10 + *str - '0';
		str++;
		}
	}

prinst(p) register NODE *p; { /* print instruction */
	if (p->uniqid != IDVAL)
		printf("	.ln	%d\n", p->uniqid);
#ifdef	IMPIL
	if (p->op == CALL ) {
		printf("@	%d	%s	%s\n", 
			numauto + ( int ) p->ops[MAXOPS], p->op1 + 1, p->op2 );
		p->ops[MAXOPS] = NULL;
	}
#endif /* IMPIL */
	switch (p->op) {
		case LABEL:
			printf("%s:\n", p->opcode);
			break;
		case HLABEL:
			printf("%s\n", p->opcode);
			break;
		case MISC:
			printf("	%s\n", p->opcode);
			break;
		case RET:
			if (*(p->op1 + 1) == '-') {
				printf("	RET\n");
				break;
			}
		default:
			printf("	%s	", p->opcode);
			if (p->op1 != NULL)
				printf("%s", p->op1);
			if (p->op2 != NULL)
				printf(",%s", p->op2);
			if (p->op3 != NULL)
				printf(",%s", p->op3);
			if (p->op4 != NULL)
				printf(",%s", p->op4);
			printf("\n");
			break;
	}
}

boolean
ishlp(p) register NODE *p; { /* return true if a fixed label present */

	for (; (p->op == LABEL || p->op == HLABEL); p=p->forw)
		if (ishl(p))
			return(true);
	return(false);
	}

FILE *
tmpopen() {
	strcpy( tmpname, TMPDIR );
	strcat( tmpname, "/25ccXXXXXX" );
	return( fopen( mktemp( tmpname ), "w" ) );
	}

putstr(string)   char *string; {
	/* Place string from the text section into a temporary file
	 * to be output at the end of the function */

	if( stmpfile == NULL )
		stmpfile = tmpopen();
	fprintf(stmpfile,"%s",string);
	}

prstr() {
/* print the strings stored in stmpfile at the end of the function */

	if( stmpfile != NULL ) {
		register int c;

		stmpfile = freopen( tmpname, "r", stmpfile );
		if( stmpfile != NULL )
			while( (c=getc(stmpfile)) != EOF )
				putchar( c );
		else
			{
			fprintf( stderr, "optimizer error: ");
			fprintf( stderr, "lost temp file\n");
			}
		(void) fclose( stmpfile );	/* close and delete file */
		unlink( tmpname );
		stmpfile = NULL;
		}
}

/* opens the temp file for storing input while looking for 'asm' */
asmopen() {
	strcpy( atmpname, TMPDIR );
	strcat( atmpname, "/asmXXXXXX" );
	atmpfile = fopen( mktemp( atmpname ), "w" );
	asmotell = ftell( stdout );
}

/* writes to temp file for 'asm' processing */
int
putasm( line )
char *line;
{
	return( fputs( line, atmpfile ) != EOF );
}

/* checks for 'asm' in files and undoes code movement */
asmchk() 
{
	register c;
	long endotell;
	extern int vars;

	if( asmflag && !oaflag ) {
		if( freopen( atmpname, "r", atmpfile ) != NULL ) {
			endotell = ftell( stdout );
			fseek( stdout, asmotell, 0 ); /* This is okay as long 
				as IMPIL is defined because it 
				is not really stdout, it is the file used by
				in-line expansion.  That file is still used, 
				even when in-line expansion is suppressed. 
				If IMPIL is not defined, optim will not work
				correctly to a terminal, but it will work
				correctly to a file.  
				This should be fixed. (fbw) */
			while( ( c = getc( atmpfile ) ) != EOF ) putchar( c );
			while( ftell( stdout ) < endotell ) printf( "!\n" );
		}
		else fprintf( stderr, "optimizer error: asm temp file lost\n" );
	}
	freopen( atmpname, "w", atmpfile );
	asmotell = ftell( stdout );
	asmflag = false;
	vars=0; 	/* reinitialize for global reg allocation */
}

putp(p,c) register NODE *p; char *c; {  /* insert pointer into jump node */

	if (p->op == CALL)
		p->op2 = c;
	else if (p->op == JBC || p->op == JBS)
		p->op3 = c;
	else if (p->op == RET) {
		p->op1 = NULL;
		p->op2 = c;
	}
	else
		p->op1 = c;
	}

revbr(p) NODE *p; { /* reverse jump in node p */

	switch (p->op) {
	    case JZ: p->op = JNZ; p->opcode = "jnz"; break;
	    case JNZ: p->op = JZ; p->opcode = "jz"; break;
	    case JPOS: p->op = JNPOS; p->opcode = "jnpos"; break;
	    case JNPOS: p->op = JPOS; p->opcode = "jpos"; break;
	    case JNEG: p->op = JNNEG; p->opcode = "jnneg"; break;
	    case JNNEG: p->op = JNEG; p->opcode = "jneg"; break;
	    case JE: p->op = JNE; p->opcode = "jne"; break;
	    case JNE: p->op = JE; p->opcode = "je"; break;
	    case JL: p->op = JGE; p->opcode = "jge"; break;
	    case JLE: p->op = JG; p->opcode = "jg"; break;
	    case JG: p->op = JLE; p->opcode = "jle"; break;
	    case JGE: p->op = JL; p->opcode = "jl"; break;
	    case JLU: p->op = JGEU; p->opcode = "jgeu"; break;
	    case JLEU: p->op = JGU; p->opcode = "jgu"; break;
	    case JGU: p->op = JLEU; p->opcode = "jleu"; break;
	    case JGEU: p->op = JLU; p->opcode = "jlu"; break;
	    case JBC: p->op = JBS; p->opcode = "jbs"; break;
	    case JBS: p->op = JBC; p->opcode = "jbc"; break;
	    }
	}

char *
dst(p) NODE *p; { /* return pointer to dst operand string */

	switch (p->op) {
	    case ACJL:   case ACJLU: case ACJLEU: case ATJNZB: case ATJNZH:
	    case ATJNZW: case INCTST:
	    case INCPTH: case INCPTW: case DECPTH: case DECPTW:
		return (p->op1);
	    case MCOMB:  case MCOMH:  case MCOMW:  case MOVZBH: case MOVZBW:
	    case MOVZHW: case MOVAB:  case MOVAH:  case MOVAW:  case ANDB2:
	    case ANDH2:  case ANDW2:  case ORB2:   case ORH2:   case ORW2:
	    case XORB2:  case XORH2:  case XORW2:  case MOVB:   case MOVH:
	    case MOVW:   case MOVBBH: case MOVBBW: case MOVBHW: case MOVTHB:
	    case MOVTWB: case MOVTWH: case MNEGB:  case MNEGH:  case MNEGW:
	    case ADDB2:  case ADDH2:  case ADDW2:  case SUBB2:  case SUBH2:
	    case SUBW2:  case MULW2:  case UMULW2: case DIVW2:  case UDIVW2:
	    case MODW2:  case UMODW2: case LLSW2:  case LRSW2:  case ALSW2:
	    case ARSW2: case FADDD2: case FSUBD2: case FMULD2: case FDIVD2:
	    case FADDS2: case FSUBS2: case FMULS2: case FDIVS2:
	    case MOVWS:	case MOVWD: case MOVTSW: case MOVTDW:
	    case MOVSW: case MOVDW: case MOVSD: case MOVDS: case MOVDD:
	    case MOVD:
		return(p->op2);
	    case ANDB3:  case ANDH3: case ANDW3:  case ORB3:   case ORH3:
	    case ORW3:   case XORB3: case XORH3:  case XORW3:  case LLSW3:
	    case LRSW3:  case ADDB3: case ADDH3:  case ADDW3:  case SUBB3:
	    case SUBH3:  case SUBW3: case MULW3:  case UMULW3: case DIVW3:
	    case UDIVW3: case MODW3: case UMODW3: case ALSW3:  case ARSW3:
	    case FADDD3: case FSUBD3: case FMULD3: case FDIVD3:
	    case FADDS3: case FSUBS3: case FMULS3: case FDIVS3:
		return(p->op3);
	    case INSV: case EXTZV:
		return(p->op4);
	    default:
		return (NULL);
	    }
	}

int
stype(cop) int cop; { /* return implied type of op code */

	switch (cop) {
	    case MCOMB: case ANDB2: case ANDB3: case ORB2:   case ORB3:
	    case XORB2: case XORB3: case MOVB:  case MOVTHB: case MOVTWB:
	    case MNEGB: case ADDB2: case ADDB3: case SUBB2:  case SUBB3:
	    case CMPB:
		return (1);
	    case MCOMH:  case MOVZBH: case ANDH2: case ANDH3: case ORH2:
	    case ORH3:   case XORH2:  case XORH3: case MOVH:  case MOVBBH:
	    case MOVTWH: case MNEGH:  case ADDH2: case ADDH3: case SUBH2:
	    case SUBH3:  case CMPH:
		return (2);
	    default:
		return (4);
	    
	    case FADDD2: case FSUBD2: case FMULD2: case FDIVD2:
	    case FADDD3: case FSUBD3: case FMULD3: case FDIVD3:
	    case FCMPD: case MOVSD: case MOVWD: case MOVDD:
		return(8);
	    }
	}

boolean
samereg(cp1,cp2) char *cp1, *cp2; { /* return true if same register */

	if (*cp1 == '%' && *cp1 == *cp2 && *++cp1 == *++cp2 && *++cp1 == *++cp2)
		return(true);
	    else
		return(false);
	}

boolean
usesreg(cp1,cp2) register char *cp1, *cp2; { /*return true if cp2 used in cp1*/

	while(*cp1 != '\0') {
		if (*cp1 == *cp2 && *(cp1+1) == *(cp2+1) &&
		    *(cp1+2) == *(cp2+2))
			return(true);
		cp1++;
		}
	return(false);
	}

dstats() { /* print stats on machine dependent optimizations */

	fprintf(stderr,"%d semantically useless instructions(s)\n", nusel);
	fprintf(stderr,"%d useless move(s) before compare(s)\n", nmc);
	fprintf(stderr,"%d merged move-arithmetic/logical(s)\n", nmal);
	fprintf(stderr,"%d useless sp increment(s)\n", nspinc);
	fprintf(stderr,"%d redundant compare(s)\n", nredcmp);
	}

wrapup() { /* print unprocessed text and update statistics file */

	FILE *fopen(), *sp;
	int mc,mal,usel,spinc,redcmp,disc,inst;

	if (n0.forw != NULL) {
		printf("	.text\n");
		filter();
		prtext();
		prstr();
		}
#ifdef IMPIL
	ilfile();
#endif /* IMPIL */

	(void) fclose( atmpfile );	/* close and delete file */
	unlink( atmpname );

	sp = fopen("/g1/dap/optim/stats/statfile","r");
	if (sp != NULL) {
		fscanf(sp, "%d %d %d %d %d %d %d",
		   &mc,&mal,&usel,&spinc,&redcmp,&disc,&inst);
		fclose(sp);
		mc += nmc;
		mal += nmal;
		usel += nusel;
		spinc += nspinc;
		redcmp += nredcmp;
		disc += ndisc;
		inst += ninst;
		sp = fopen("/g1/dap/optim/stats/statfile","w");
		if (sp != NULL) {
			fprintf(sp, "%d %d %d %d %d %d %d \n",
			   mc,mal,usel,spinc,redcmp,disc,inst);
			fclose(sp);
			}
		}
	}

setauto(str) char *str; { /* set indicator for number of autos */

	while (*str++ != ',')
	    ;
	numauto = atoi(str);		/* remember # of bytes */
	return;
	}

setnreg(str) char *str; { /* set indicator for number of registers */

	while (*str++ != ',')
	    ;
	numnreg = atoi(str);		/* remember number of registers */
	return;
	}
chkauto()	 /* delete sp increment if no autos, or fewer than 15 words */
{
    NODE *p;

/* 5.0 UNIX microcode on the 3B20 supports a "save" instruction that
** can also allocate temporary space on the stack (up to 15 words).
** Standard IS25 "save" does not do the stack allocation.
*/

#ifndef IMPREGAL
#ifdef UCODE50
    if (numauto <= 4*15)		/* save handles up to 15 words */
#else
    if (numauto == 0)			/* if no auto's allocated */
#endif	/* def UCODE50 */
    {
	p = n0.forw;
	while (islabel( p ))
	    p = p->forw;
	if (    strcmp(p->opcode,"save") == 0
	    &&  p->forw->op == ADDW2
	    )				/* save followed by addw2 */
	{
#ifdef UCODE50				/* must add second operand to save */
	    p->op2 = getspace(1+2+1);	/* for "&dd\0" */
	    (void) sprintf(p->op2,"&%d",numauto/4);
					/* append operand with # of autos */
#endif	/* def UCODE50 */
	    p = p->forw;		/* point at ADDW2 */
	    DELNODE( p );		/* delete it */
	    nspinc++;			/* discarded sp increment */
	    ndisc++;			/* discarded instruction */
	}
    }
#endif  /* der IMPREGAL */
#ifndef IMPIL
    numauto = 0;			/* reset for next routine */
#endif /* IMPIL */
    return;
}

uses(p) NODE *p; { /* set register use bits */

	switch (p->op) {
	    case MOVBLB: case MOVBLH: case MOVBLW:
		return (0x7);
	    case MCOMB:  case MCOMH:  case MCOMW:  case MOVZBH: case MOVZBW:
	    case MOVZHW: case MOVAB:  case MOVAH:  case MOVAW:  case MOVB:
	    case MOVH:   case MOVW:   case MOVBBH: case MOVBBW: case MOVBHW:
	    case MOVTHB: case MOVTWB: case MOVTWH: case MNEGB:  case MNEGH:
	    case MNEGW:
	    case MOVWS: case MOVTSW: case MOVSW:
		return (scanreg(p->op1,false) | scanreg(p->op2,true) |
			scanreg(p->op3,false) | scanreg(p->op4,false));
	    case ANDB3:  case ANDH3: case ANDW3:  case ORB3:   case ORH3:
	    case ORW3:   case XORB3: case XORH3:  case XORW3:  case LLSW3:
	    case LRSW3:  case ADDB3: case ADDH3:  case ADDW3:  case SUBB3:
	    case SUBH3:  case SUBW3: case MULW3:  case UMULW3: case DIVW3:
	    case UDIVW3: case MODW3: case UMODW3: case ALSW3:  case ARSW3:
		return (scanreg(p->op1,false) | scanreg(p->op2,false) |
			scanreg(p->op3,true) | scanreg(p->op4,false));
	    case INSV: case EXTZV:
		return (scanreg(p->op1,false) | scanreg(p->op2,false) |
			scanreg(p->op3,false) | scanreg(p->op4,true));
	    case CALL:
		return (scanreg(p->op1,false) | scanreg(p->op2,false) |
			scanreg(p->op3,false) | scanreg(p->op4,false) |
			( 1 << 2 )); /* call must preserve %r2;
					ptr to returned structure */
	    case RET:
		return (scanreg(p->op1,false) | scanreg(p->op2,false) |
			scanreg(p->op3,false) | scanreg(p->op4,false) |
			RETREG); /* function return values */
	    case JSB:
		return (scanreg(p->op1,false) | scanreg(p->op2,false) |
			scanreg(p->op3,false) | scanreg(p->op4,false) |
			1 ); /* cludge for profiler:  %r0 is used to pass
				an argument to _mcount */
	    case FADDD2: case FSUBD2: case FMULD2: case FDIVD2:
	    case FCMPD:
		return( dscanreg(p->op1,false) | dscanreg(p->op2,false) |
			scanreg(p->op3,false) | scanreg(p->op4,false) );

	    case FADDD3: case FSUBD3: case FMULD3: case FDIVD3:
		return( dscanreg(p->op1,false) | dscanreg(p->op2,false) |
			dscanreg(p->op3,true) | scanreg(p->op4,false) );
	    
	    case MOVWD: case MOVSD:
		return( scanreg(p->op1,false) | dscanreg(p->op2,true) |
			scanreg(p->op3,false) | scanreg(p->op4,false) );
	    
	    case MOVDW: case MOVTDW: case MOVDS:
		return( dscanreg(p->op1,false) | scanreg(p->op2,true) |
			scanreg(p->op3,false) | scanreg(p->op4,false) );

	    case MOVDD: case MOVD:
		return( dscanreg(p->op1,false) | dscanreg(p->op2,true) |
			scanreg(p->op3,false) | scanreg(p->op4,false) );

	    default:
		return (scanreg(p->op1,false) | scanreg(p->op2,false) |
			scanreg(p->op3,false) | scanreg(p->op4,false) |
			( iscbr( p ) ? CONCODES : 0 ) );
	    }
	}

scanreg(cp,flag) register char *cp; int flag; { /* determine registers 
					referenced in operand */

	int reg,set;

	if (cp == NULL)
		return (0);
	if (flag && *cp == '%')
		return (0);
	set = 0;
	while (*cp != '\0') {
		if (*cp == '%') {
			cp++;
			switch (*cp) {
			    case 'r':
				cp++;
				reg = *cp - '0';
				break;
			    case 'a':
				cp++;
				reg = 9;
				break;
			    case 'f':
				cp++;
				reg = 10;
				break;
			    case 's':
				cp++;
				reg = 11;
				break;
			    }
			set = set | 1 << reg;
			}
		cp++;
		}
	return(set);
	}

/* determine registers used for double operand */

int
dscanreg(cp,flag)
char * cp;			/* operand string */
boolean flag;			/* true if register is destination */
{
    int temp = scanreg(cp,flag);	/* do normal scan */

    if (*cp == '%')		/* if direct register reference... */
	temp = 3*temp;		/* set bit and its left neighbor (reg+1) */
    
    return(temp);
}

sets(p) NODE *p; { /* set register destination bits */

	char *cp, *dst();
	int o;
	boolean dblflag = false;	/* true if register double operand */

	switch (o = p->op) {
	    case MOVBLB: case MOVBLH: case MOVBLW:
		return (0x0007);

/* Procedure calls could potentially clobber all of the scratch registers.
** We would like call to "kill" all of them, but we can only be reasonably
** safe by saying it kills those registers in which it returns values.
*/

	    case CALL:
		return(RETREG | CONCODES);

/* Register doubles set two registers at a time:  r and r+1 */

	    case FADDD2: case FSUBD2: case FMULD2: case FDIVD2:
	    case FADDD3: case FSUBD3: case FMULD3: case FDIVD3:
	    case MOVWD: case MOVSD: case MOVDD: case MOVD:
		dblflag = true;

	    default:
		cp = dst(p);
		if (cp != NULL)
		    return((dblflag ? 3*setreg(cp) : setreg(cp)) | CONCODES );
					/* return reg bit and its left
					** neighbor on double
					*/
		else
		    return ( ( ( o == RET )
			|| ( CMPB <= o && o <= ATJNZW )
			|| ( PUSHAW <= o && o <= PUSHBH ) ) ? CONCODES : 0 );
	    }
	}

setreg(cp) char *cp; { /* set index of register destination (if one) */

	if (*cp == '%')
	    switch (*++cp) {
		    case 'r':
			cp++;
			return (1 << (*cp - '0'));
		    case 'a':
			return (1 << 9);
		    case 'f':
			return (1 << 10);
		    case 's':
			return (1 << 11);
		    default:			/* unrecognized name */
			break;
		    }
	return(0);			/* for non-register or unrecognized */
	}

boolean
isdead(cp,p) char *cp; NODE *p; { /* true iff *cp is dead after p */

	int reg;

	if (*cp != '%')
		return(false);
	switch(*++cp) {
	    case 'r':
		reg = *++cp - '0';
		break;
	    case 'a':
		reg = 9;
		break;
	    case 'f':
		reg = 10;
		break;
	    case 's':
		reg = 11;
		break;
	    default:			/* unrecognized name */
		return(false);
	    }
	if ((p->nlive & (1 << reg)) == 0)
		return(true);
	    else
		return(false);
	}
char *
getp(p) NODE *p; { /* return pointer to jump destination operand */

	switch (p->op) {
	    case JMP:  case JSB:   case JZ:    case JNZ:
	    case JPOS: case JNPOS: case JNEG: case JNNEG: case JE:
	    case JNE:  case JL:    case JLE:  case JG:    case JGE:
	    case JLU:  case JLEU:  case JGU:  case JGEU:
		return(p->op1);
	    case RET:
	    case SWITCH:
	    case SWITCHT:
	        return(NULL);
	    case JBC: case JBS:
		return(p->op3);
	    }
	}

char *
newlab() { /* generate a new label */

	static int lbn = 0;
	char *c;

	c = (char *)getspace(lbn < 100 ? 6 : (lbn < 1000 ? 7 : 8));
	sprintf(c, "..%d\0", lbn);
	lbn++;
	return(c);
	}

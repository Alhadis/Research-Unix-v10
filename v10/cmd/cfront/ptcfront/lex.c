/*ident	"@(#)ctrans:src/lex.c	1.5.1.1" */
/***************************************************************************

	C++ source for cfront, the C++ compiler front-end
	written in the computer science research center of Bell Labs

	Copyright (c) 1984 AT&T, Inc. All Rights Reserved
	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T, INC.

lex.c:
	lexical analyser based on pcc's and cpre's scanners
	modified to handle classes:
	new keywords:	class
			public
			call
			etc.
	names are not entered in the symbol table by lex()
	names can be of arbitrary length
	error() is used to report errors
	{} and () must match
	numeric constants are not converted into internal representation
	but stored as strings

****************************************************************************/

#include "cfront.h"
#include "yystype.h"
#include "size.h"
#include "tqueue.h"
#include "template.h"

# define  CCTRANS(x) x
 
#ifndef NULL
#define NULL	0
#endif
 
char* strdup(const char* s1) 
/* string duplication
    returns pointer to a new string which is the duplicate of string
    pointed to by s1
    NULL is returned if new string can't be created
*/
{  
   char * s2;
 
   s2 = malloc((unsigned) strlen(s1)+1) ;
   return(s2==NULL ? NULL : strcpy(s2,s1) );
}
 
#define copy_if_need_be(s)  ((templp->in_progress || templp->parameters_in_progress) ? strdup(s) : (s))

	/* lexical actions */

#define A_ERR	0		/* illegal character */
#define A_LET	1		/* saw a letter */
#define A_DIG	2		/* saw a digit */
#define A_1C	3		/* return a single character */
#define A_STR 	4		/* string */
#define A_CC	5		/* character constant */
#define A_BCD	6		/* GCOS BCD constant */
#define A_SL	7		/* saw a / */
#define A_DOT 	8		/* saw a . */
#define A_2C	9		/* possible two character symbol */
#define A_WS	10		/* whitespace (not \n) */
#define A_NL	11		/* \n */
#define A_LC	12		/* { */
#define A_RC	13		/* } */
#define A_L	14		/* ( */
#define A_R	15		/* ) */
#define A_EOF	16
#define A_ASS	17
#define A_LT	18
#define A_GT	19		/* > */
#define A_ER	20
#define A_OR	21
#define A_AND	22
#define A_MOD	23
#define A_NOT	24
#define A_MIN	25
#define A_MUL	26
#define A_PL	27
#define A_COL	28		/* : */
#define A_SHARP	29		/* # */
#define A_DOLL  30              /* $ */

	/* character classes */

# define LEXLET 01
# define LEXDIG 02
/* no LEXOCT because 8 and 9 used to be octal digits */
# define LEXHEX 010
# define LEXWS 020
# define LEXDOT 040

const FIRSTCHUNK = 8*1024-8;
const BUFCHUNK = 4*1024-8;

	/* text buffer */
static char inbuf[FIRSTCHUNK/*TBUFSZ*/];
static char* txtmax = &inbuf[FIRSTCHUNK/*TBUFSZ*/-1];
static char* txtstart = 0;
static char* txtfree = 0;

static struct buf* bufhead;
static buf* freebuf;
//static bufs;

struct buf {
	buf* next;
	char chars[BUFCHUNK];
//	buf() { next=bufhead; bufhead=this; }
};

new_buf(char c)
{
//fprintf(stderr,"new_buf %d\n",bufs++);
	buf* pbuf;
	if (freebuf) {
		pbuf = freebuf;
		freebuf = freebuf->next;
	}
	else
		pbuf = new buf;	// allocate and register new chunk
	pbuf->next = bufhead;
	bufhead = pbuf;

	if (BUFCHUNK < txtmax-txtstart) error('l',"lexical token too long");

	// copy current token:
	char* p = txtstart;
	txtstart = txtfree = &pbuf->chars[0];
	while (p<txtmax) *txtfree++ = *p++;
	*txtfree++=c;
	txtmax = &pbuf->chars[BUFCHUNK-1];
	return 0;
}


#define pch(c) ((txtmax<=txtfree)?new_buf(c):(*txtfree++=c))
#define start_txt()	txtstart = txtfree
#define del_txt()	txtfree = txtstart

static int Nfile;// = 1;
static char* file_name;	// source file names

Linkage linkage = linkage_default; // linkage is default C++	
const LINKMAX = 10;
static Linkage lvec[LINKMAX] = { linkage_default };
static int lcount = 0;

void set_linkage(char* p)
{
	if (p==0 || *p == 0) {	// resume previous linkage
		if (lcount>0) linkage = lvec[--lcount];
	}
	else {
		if (LINKMAX<=++lcount) {
			error('l',"linkage directive nested too deep");
			--lcount;
		} else if (strcmp(p,"C")==0)
			lvec[lcount] = linkage = linkage_C;
		else if (strcmp(p,"C++")==0)
			lvec[lcount] = linkage = linkage_Cplusplus;
		else {
			error("%s linkage",p);
			--lcount;
		}
	}
}

static struct loc tloc;
FILE * out_file = stdout;
FILE * in_file = stdin;
Ptable ktbl;
Ptable keyword_table;

static int p_level = 0;		/* number of unmatched ``(''s */
static int b_level = 0;		/* number of unmatched ``{''s */

# ifdef ibm

# define CSMASK 0377
# define CSSZ 256

# else

# define CSMASK 0177
# define CSSZ 128

# endif

static short lxmask[CSSZ+1];

int saved = 0;	/* putback character, avoid ungetchar */
static int lxtitle();

// overload rt;
inline YYSTYPE rt(char* x) { YYSTYPE y; y.s  = x;        return y; }
inline YYSTYPE rt(int   x) { YYSTYPE y; y.t  = x;        return y; }
inline YYSTYPE rt(loc   x) { YYSTYPE y; y.l  = x;        return y; }
inline YYSTYPE rt(void* x) { YYSTYPE y; y.pn = Pname(x); return y; }

#define get(c)		(c=getc(in_file))
#define unget(c)	ungetc(c,in_file)

#define reti(a,b)	{ addtok(a, rt(b), tloc);          return a; }
#define retn(a,b)	{ addtok(a, rt((Pnode)b), tloc);   return a; }
#define rets(a,b)	{ addtok(a, rt(b), tloc);          return a; }
#define retl(a)		{ addtok(a, rt(tloc), tloc);       return a; }

// keys[] holds the external form for tokens with fixed representation 
// illegal tokens and those with variable representation have 0 entries
char* keys[MAXTOK+1];

static void 
new_key(char* s, TOK toknum, TOK yyclass)
/*
  make "s" a new keyword with the representation (token) "toknum"
  "yyclass" is the yacc token (for example new_key("int",INT,TYPE); )
  "yyclass==0" means yyclass=toknum;
*/
{
	Pname n = new name(s);

	keys[(toknum==LOC)?yyclass:toknum] = s;
	n = new name(s);
	Pname nn = keyword_table->insert(n,0);
	// if (Nold) error('i',"keyword %sD twice",s);
	nn->base = toknum;
	nn->syn_class = (yyclass) ? yyclass : toknum;
	delete n;
}

const int keyword_count = 67;
static void 
ktbl_init()
/*
	enter keywords into keyword table for use by lex()
	and into keyword representation table used for output
 
        ktbl is only for types. We put nothing in it. 
	keyword_table is for user-defined reserved words 
*/
{
	ktbl = new table(KTBLSIZE,0,0);
	keyword_table = new table(keyword_count,0,0);

	new_key("asm",ASM,0);
	new_key("auto",AUTO,TYPE);
	new_key("break",LOC,BREAK);
	new_key("case",LOC,CASE);
	new_key("continue",LOC,CONTINUE);
	new_key("char",CHAR,TYPE);
	new_key("do",LOC,DO);
	new_key("double",DOUBLE,TYPE);
	new_key("default",LOC,DEFAULT);
	new_key("enum",ENUM,0);
	new_key("else",LOC,ELSE);
	new_key("extern",EXTERN,TYPE);
	new_key("float",FLOAT,TYPE);
	new_key("for",LOC,FOR);
	new_key("goto",LOC,GOTO);
	new_key("catch",CATCH,CATCH);
	new_key("try",TRY,TRY);
	new_key("if",LOC,IF);
	new_key("int",INT,TYPE);
	new_key("long",LONG,TYPE);
	new_key("return",LOC,RETURN);
	new_key("register",REGISTER,TYPE);
	new_key("static",STATIC,TYPE);
	new_key("struct",STRUCT,AGGR);
	new_key("sizeof",SIZEOF,0);
	new_key("short",SHORT,TYPE);
	new_key("switch",LOC,SWITCH);
	new_key("typedef",TYPEDEF,TYPE);
	new_key("unsigned",UNSIGNED,TYPE);
	new_key("union",UNION,AGGR);
	new_key("void",VOID,TYPE);
	new_key("while",LOC,WHILE);

	new_key("class",CLASS,AGGR);
	new_key("const",CONST,TYPE);
	new_key("delete",LOC,DELETE);
	new_key("friend",FRIEND,TYPE);
	new_key("inline",INLINE,TYPE);
	new_key("new",NEW,0);
	new_key("operator",OPERATOR,0);
	new_key("overload",OVERLOAD,TYPE);
	new_key("private",PRIVATE,PR);
	new_key("protected",PROTECTED,PR);
	new_key("public",PUBLIC,PR);
	new_key("signed",SIGNED,TYPE);
	new_key("template",TEMPLATE,0);
	new_key("this",THIS,0);
	new_key("virtual",VIRTUAL,TYPE);
	new_key("volatile",VOLATILE,TYPE);

	new_key("__statement", STATEMENT, 0) ;
	new_key("__expression", EXPRESSION, 0) ;
	new_key("__template_test", TEMPLATE_TEST, 0) ;
}

loc last_line;
loc noloc = { 0, 0 };

void loc::putline()
{
	if (file==0 && line==0) return;
//   fix, Nfile, not MAXFILE; Nfile == MAXFILE*4
//	if (0<=file && file<MAXFILE) {
	if ( 0<=file && file <= Nfile ) {
		char* f = file_name;
		if (f==0) f = src_file_name;
		fprintf(out_file,line_format,line,f);
		last_line = *this;
	}
}

void loc::put(FILE* p)
{
//    fix, Nfile, not MAXFILE; Nfile == MAXFILE*4
//	if (0<=file && file<MAXFILE) {
	if ( 0<=file && file <= Nfile ) {
		char* f = file_name;
		if (f==0) f = src_file_name;
		fprintf(p,"\"%s\", line %d: ",f,line);
	}
}

void lxenter(register char* s, short m)
/* enter a mask into lxmask */
{
	register c;

	while( c= *s++ ) lxmask[c+1] |= m;

}


void lxget(register c, register m)
/*
	put 'c' back then scan for members of character class 'm'
	terminate the string read with \0
	txtfree points to the character position after that \0
*/
{
	pch(c);
	while ( (get(c), lxmask[c+1]&m) ) pch(c);
	unget(c);
	pch('\0');
}

struct LXDOPE {
	short lxch;	/* the character */
	short lxact;	/* the action to be performed */
	TOK   lxtok;	/* the token number to be returned */
} lxdope[] = {
#ifdef apollo
	'@',	A_ERR,	0,	/* illegal characters go here... */
#else
         '$',    A_DOLL, 0,
//       '$',    A_ERR,  0,      /* illegal characters go here... */
#endif
	'_',	A_LET,	0,	/* letters point here */
	'0',	A_DIG,	0,	/* digits point here */
	' ',	A_WS,	0,	/* whitespace goes here */
	'\n',	A_NL,	0,
	'"',	A_STR,	0,	/* character string */
	'\'',	A_CC,	0,	/* ASCII character constant */
	'`',	A_BCD,	0,	/* 'foreign' character constant, e.g. BCD */
	'(',	A_L,	LP,
	')',	A_R,	RP,
	'{',	A_LC,	LC,
	'}',	A_RC,	RC,
	'[',	A_1C,	LB,
	']',	A_1C,	RB,
	'*',	A_MUL,	MUL,
	'?',	A_1C,	QUEST,
	':',	A_COL,	COLON,
	'+',	A_PL,	PLUS,
	'-',	A_MIN,	MINUS,
	'/',	A_SL,	DIV,
	'%',	A_MOD,	MOD,
	'&',	A_AND,	AND,
	'|',	A_OR,	OR,
	'^',	A_ER,	ER,
	'!',	A_NOT,	NOT,
	'~',	A_1C,	COMPL,
	',',	A_1C,	CM,
	';',	A_1C,	SM,
	'.',	A_DOT,	DOT,
	'<',	A_LT,	LT,
	'>',	A_GT,	GT,
	'=',	A_ASS,	ASSIGN,
	'#',	A_SHARP, 0,
	EOF,	A_EOF,	EOFTOK
	};
/* note: EOF is used as sentinel, so must be <=0 and last entry in table */

static struct LXDOPE *lxcp[CSSZ+1];

void 
lex_init()
{
	register struct LXDOPE *p;
	register i;
	register char *cp;
	/* set up character classes */

	/* first clear lexmask */
	for(i=0; i<=CSSZ; i++) lxmask[i] = 0;

#ifdef apollo
	lxenter( "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_$", LEXLET );
#else
	lxenter( "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_", LEXLET );
#endif
	lxenter( "0123456789", LEXDIG );
	lxenter( "0123456789abcdefABCDEF", LEXHEX );
		/* \013 should become \v someday; \013 is OK for ASCII and EBCDIC */
	lxenter( " \t\r\b\f\013", LEXWS );
	lxmask['.'+1] |= LEXDOT;

	/* make lxcp point to appropriate lxdope entry for each character */

	/* initialize error entries */

	for( i= 0; i<=CSSZ; ++i ) lxcp[i] = lxdope;

	/* make unique entries */

	for( p=lxdope; ; ++p ) {
		lxcp[p->lxch+1] = p;
		if( p->lxch < 0 ) break;
	}

	/* handle letters, digits, and whitespace */
	/* by convention, first, second, and third places */

	cp = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	while( *cp ) lxcp[*cp++ + 1] = &lxdope[1];
	cp = "123456789";
	while( *cp ) lxcp[*cp++ + 1] = &lxdope[2];
	cp = "\t\b\r\f\013";
	while( *cp ) lxcp[*cp++ + 1] = &lxdope[3];

	file_name = src_file_name;
	// set both curloc and tloc so curloc is valid at program startup
//	curloc.file = tloc.file = 0;
	curloc.line = tloc.line = 1;

	ktbl_init();
	lex_clear();
	saved = lxtitle();
}

void lex_clear()
{
	// delete extra buffers:
	buf* p = bufhead;	
	bufhead = 0;
//if (p) {
//fprintf(stderr,"lex_clear\n");
//bufs=0;
//}
	while (p) {
		buf* pp = p;
		p = p->next;
		pp->next = freebuf;
		freebuf = pp;
	}

	// re-set to static buffer:
	txtstart = txtfree = inbuf;
	txtmax = &inbuf[FIRSTCHUNK/*TBUFSZ*/-1];
}

int int_val(char hex)
{
	switch (hex) {
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		return hex-'0';
	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
		return hex-'a'+10;
	case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
		return hex-'A'+10;
	}
}

void hex_to_oct()
/*
	\x has been seen on input (in char const or string) and \ printed
	read the following hexadecimal integer and replace it with an octal
*/
{
	int i = 0;
	int c;
	get(c);
	if (lxmask[c+1] & LEXHEX) {
		i = int_val(c);
		get(c);					// try for two
		if (lxmask[c+1] & LEXHEX) {
			i = (i<<4) + int_val(c);
			get(c);				// try for three
			if (lxmask[c+1] & LEXHEX)
				i = (i<<4) + int_val(c);
			else
				unget(c);
		}
		else
			unget(c);
	}
	else {
		error("hexadecimal digitE after \\x");
		unget(c);
	}

//	if (0377 < i) error('l',"hexadecimal constant too large");
	i &= 0377;

	pch(('0'+(i>>6)));
	pch(('0'+((i&070)>>3)));
	pch(('0'+(i&7)));
}


char * chconst()
/*
	read a character constant into inbuf
*/
{
	register c;
	int nch = 0;

	pch('\'');

	for(;;) {
		char* p;
		char cc = 0;

		switch (get(c)) {
		case '\'':
			goto ex;
		case EOF:
			error("eof in char constant");
			goto ex;
		case '\n':
			error("newline in char constant");
			goto ex;
		case '\\':
			if (SZ_INT == nch++) error('l',"char constant too long");
			pch(c);
			switch (get(c)){
			case '\n':
				++tloc.line;
			default:
				pch(c);
				break;
			case '4': case '5': case '6': case '7': // octal
				p = txtfree;
				cc = c-4;
			case '0': case '1': case '2': case '3':
				pch(c);
				get(c);  /* try for 2 */
				if( lxmask[c+1] & LEXDIG && c<'8'){
					pch(c);
					get(c);  /* try for 3 */
					if (lxmask[c+1] & LEXDIG && c<'8') {
						if (cc) *p = cc; // zap high bit
						pch(c);
					}
					else
						unget(c);
				}
				else
					unget(c);
				break;
			case 'x':	// hexadecimal
				hex_to_oct();
				break;
			};
			break;
		default:
			if (SZ_INT == nch++) error('l',"char constant too long");
			pch(c);
		}
	}
ex:
	if(nch==0)
		error("empty char constant");
	pch('\'');
	pch('\0');
	return txtstart;
}

void lxcom()
/* process a "block comment" */
{
	register c;

	for(;;)
	switch (get(c)) {
	case EOF:
		error('w',"eof in comment");
		return;
	case '\n':
		tloc.line++;
//		Nline++;
		break;
	case '*':
		if (get(c) == '/') return;
		unget(c);
		break;
	case '/':
		if (get(c) == '*') error('w',"``/*'' in comment");
		unget(c);
		break;
	}
}


void linecom()
// process a "line comment"
{
	register c;

	get(c);
#ifdef DBG
	if ( c=='@' && get(c)=='!' ) {
		while ( get(c) != '\n' && c != EOF ) pch(c);
		pch('\0');
		process_debug_flags(txtstart);
		del_txt();
	}
#endif
	for(;;get(c))
	switch (c) {
	case EOF:
		error('w',"eof in comment");
		return;
	case '\n':
		tloc.line++;
//		Nline++;
		saved = lxtitle();
		return;
	}
}

char eat_whitespace()
{

	for(;;) {
		register c = get(c);
	lx:

		switch (c) {
		case EOF:
			error('w',"unexpected comment");
			return EOF;
		case '/':
			switch (get(c)) {
			case '*':
				lxcom();
				break;
			case '/':
				linecom();
				break;
			default:
				unget(c);
				return '/';
			}
			break;
		case '\n':
			++tloc.line;
			c = lxtitle();
			goto lx;
		case ' ':
		case '\t':
			break;
		default:
			return c;		
		}
	}	
}

void get_string()
{
	int lxchar;

	for(;;)
	switch (get(lxchar)) {
 	case '\\':
  		pch('\\');
 		switch (get(lxchar)){
 		case '\n':
 			++tloc.line;
 		default:
  			pch(lxchar);
 			break;
 		case 'x':	// hexadecimal
 			hex_to_oct();
 			break;
 		};
  		break;
	case '"':
        {       char* p = txtstart;     // eat_whitespace() moves txtstart
                if ((lxchar = eat_whitespace()) == '"') {
                        // string catenation, break with
                        // newline to avoid merging characters
                        // (e.g. "\xAB" "C")
                        pch('\\');
                        pch('\n');

                        continue;       // eat '\"' and carry on
                };

                txtstart = p;
		unget(lxchar);
		pch(0);
		return;
        }
	case '\n':
		error("newline in string");
		pch(0);
		return;
	case EOF:
		error("eof in string");
		pch(0);
		return;
	default:
		pch(lxchar);
	}
}

TOK tlex()
{
	TOK ret;
	Pname n;

//	Ntoken++;

	for(;;) {
		register lxchar;
		register struct LXDOPE *p;

		start_txt();

		if (saved) {
			lxchar = saved;
			saved = 0;
		}
		else
			get(lxchar);

 		if (lxchar+1 >= CSSZ )
		 error( "illegal input character encountered: %d", lxchar );

		switch( (p=lxcp[lxchar+1])->lxact ){

		case A_1C:	// eat up a single character, and return an opcode 
			reti(p->lxtok,p->lxtok);

		case A_EOF:
			if (p_level || b_level+lcount)
				error("'%s' missing at end of input",(b_level+lcount) ? "}" : ")");
		
			reti(EOFTOK,0);

		case A_SHARP:
			// cope with header file not ended with '\n'
			unget('#');
			saved = lxtitle();
			continue;

		case A_ERR:
		{	if (' '<=lxchar && lxchar<='~')	// ASCII printable
				error("illegal character '%c' (ignored)",lxchar);
			else
				error("illegal character '0%o' (ignored)",lxchar);
			continue;
		}

		case A_DOLL:
                { // lex a name of the for $id for template tree formals
                	Pname fn ;
                        lxget( lxchar, LEXLET|LEXDIG ) ;

                        if (!templp->in_progress ||  !txtstart[1]) {
                          // no name string immediately follows, treat it                                  
                          // like an illegal character
                          error("illegal character '0%o' (ignored)",lxchar);                               
                          continue;
                        }  

                        txtstart++ ;
                        if(fn=templ_compilation::tree_parameter(txtstart)) {                              
                        	switch (fn->n_template_arg) {
                          		case template_expr_tree_formal:
                            		// retain the $ in the name
                            			retn(ID, strdup(--txtstart)) ;

                          		case template_stmt_tree_formal:
                            			retn(SM_PARAM, fn) ;
                          	}  
                        }  
                        error("%s wasn't a statement or expression formal", txtstart);
                        rets(ID, copy_if_need_be(txtstart));
                }

		case A_LET:	// collect an identifier and check for keyword
		{
			char ll;
			switch (ll = lxchar) {
		//	case 'l':
			case 'L':
				switch (get(lxchar)) {
				case '\'':
					error('s',"wide character constant");
					unget(lxchar);
					continue;
				case '"':
					error('s',"wide character string");
					unget(lxchar);
					continue;
				}
				unget(lxchar);
				lxchar = ll;
			}
		}

			lxget( lxchar, LEXLET|LEXDIG );
                        /* look for a keyword or a global type */
			if ((n = keyword_table->look(txtstart,0))  /* keyword */
				|| (n = ktbl->look(txtstart, 0)))  /* local type */
			{
				TOK x;
				del_txt();
				switch (x=n->base) {
				case TNAME:
					rets(ID,n->string);
				case LOC:
					retl(n->syn_class);
				case EXTERN:
					if ((lxchar = eat_whitespace()) == '\"') {
						// linkage directive
						get_string();
						rets(LINKAGE,txtstart);
					}
					unget(lxchar);
					reti(TYPE,EXTERN);
				case CATCH:
				// case TEMPLATE:
					error('s',"%k",n->syn_class);
					continue;
				case TRY:
				{
					static int warn_try;
					if (!warn_try) { 
						Pname n = keyword_table->look("try",0);
						n->n_key = DEFAULT;
						error('w',&tloc,"%k is a future reserved keyword",n->syn_class);
						warn_try++;
					}
					rets(ID,n->string);
				}
				default:
					reti(n->syn_class,x);
				}
			}
			// rets(ID,txtstart);
                        rets(ID, copy_if_need_be(txtstart)) ;

		case A_DIG:

			ret = ICON;

			if (lxchar=='0') {
				int pkchar;
				get(pkchar);
				if(pkchar=='x' || pkchar=='X') {	// hex
					pch(lxchar);
					lxget(pkchar,LEXHEX);
					txtfree--;
					if (txtfree-txtstart<3) // minimum "0Xd\0"
						error("hex digitX after \"0x\"");
					get(lxchar);
					goto getsuffix;
				}
				unget(pkchar);
			}

			lxget(lxchar,LEXDIG);
			txtfree--;

			if (get(lxchar) == '.') {
		getfp:
				lxget('.', LEXDIG );
				txtfree--;
				ret = FCON;
				get(lxchar);
			};
		
			if (lxchar=='e' || lxchar=='E') {
				pch(lxchar);
				get(lxchar);
				if(lxchar=='-' || lxchar=='+') {
					pch(lxchar);
					get(lxchar);
				}
				if (lxmask[lxchar+1] & LEXDIG) {
					lxget( lxchar, LEXDIG );
					txtfree--;
					get(lxchar);
				}
				else
					error("missing exponent digits?");
				ret = FCON;
			};

			if(*txtstart=='0' && ret==ICON) {
				char *bch = txtstart;
				while (++bch <= txtfree) {
					if(*bch=='8' || *bch=='9')
						error("%c used as octal digit",*bch);
				}
			}

		getsuffix:
			switch (lxchar) {
			case 'f':
			case 'F':
				if (ret==ICON)
					error("%c suffix for integer constant",lxchar);
				else
					pch(lxchar);
				break;
			case 'u':
			case 'U':
				if (ret==FCON) {
					error("%c suffix for floating constant",lxchar);
					break;
				}
				pch(lxchar);
				switch(get(lxchar)) {
				case 'l':
				case 'L':
					pch(lxchar);
					break;
				default:
					saved=lxchar;
					break;
				}
				break;
			case 'l':
			case 'L':
				pch(lxchar);
				if (ret==FCON) {
					break;
				}
				switch(get(lxchar)) {
				case 'u':
				case 'U':
					pch(lxchar);
					break;
				default:
					saved=lxchar;
					break;
				}
				break;
			default:
				saved = lxchar;
				break;
			};

			if(*txtstart=='0' && txtfree-txtstart==1)
				reti(ZERO,0);	// plain zero

			pch(0);
			rets(ret,txtstart);


		case A_DOT:
			switch (get(lxchar)) {
			case '.':	// look for ellipsis
				if (get(lxchar) != '.') {
					error("token .. ?");
					saved = lxchar;
				}
				reti(ELLIPSIS,0);
			case '*':
				reti (REFMUL,DOT);
			}

			if (lxmask[lxchar+1] & LEXDIG){// look for floating constant
				unget(lxchar);
				goto getfp;
			}
			saved = lxchar;
			reti(DOT,0);

		case A_STR:
			/* save string constant in buffer */
			get_string();
			rets(STRING,txtstart);

		case A_CC:
			/* character constant */
			rets(CCON,chconst());

		case A_BCD:
			{
				register i;
				int j;
	
				pch('`');
	
				for (i=0; i<7; ++i) {
					pch(get(j));
					if (j == '`' ) break;
				}
				pch(0);
				if (6<i)
					error('l',"bcd constant exceeds 6 characters" );
				rets(CCON,txtstart);
			}

		case A_SL:	/* / */
			switch (get(lxchar))  {
			case '*':
				lxcom();
				break;
			case '/':
				linecom();
				break;
			case '=':
				reti(ASOP,ASDIV);
			default:
				saved = lxchar;
				reti(DIVOP,DIV);
			}

		case A_WS:
			continue;

		case A_NL:
			++tloc.line;
//			Nline++;
			saved = lxtitle();
			continue;

		case A_LC:
			if (BLMAX <= b_level++) {
				error('l',"blocks too deeply nested");
				ext(3);
			}
			retl(LC);

		case A_RC:
			if (lcount+b_level-- <= 0) {
				error("unexpected '}'");
				b_level = 0;
			}
			retl(RC);

		case A_L:
			p_level++;
			reti(LP,0);

		case A_R:
			if (p_level-- <= 0) {
				error("unexpected ')'");
				p_level = 0;
			}
			reti(RP,0);

		case A_ASS:
			switch (get(lxchar)) {
			case '=':
				reti(EQUOP,EQ);
			default:
				saved = lxchar;
				reti(ASSIGN,ASSIGN);
			}

		case A_COL:
			switch (get(lxchar)) {
			case ':':
				reti(MEM,0);
			case '=':
				error("':=' is not a c++ operator");
				reti(ASSIGN,ASSIGN);
			default:
				saved = lxchar;
				reti(COLON,COLON);
			}
		case A_NOT:
			switch (get(lxchar)) {
			case '=':
				reti(EQUOP,NE);
			default:
				saved = lxchar;
				reti(NOT,NOT);
			}
		case A_GT:
			switch(get(lxchar)) {
			case '>':
				switch (get(lxchar)) {
				case '=':
					reti(ASOP,ASRS);
					break;
				default:
					saved = lxchar;
					reti(SHIFTOP,RS);
				}
			case '=':
				reti(RELOP,GE);
			default:
				saved = lxchar;
				reti(GT,GT);
			}
		case A_LT:
			switch (get(lxchar)) {
			case '<':
				switch (get(lxchar)) {
				case '=':
					reti(ASOP,ASLS);
				default:
					saved = lxchar;
					reti(SHIFTOP,LS);
				}
			case '=':
				reti(RELOP,LE);
			default:
				saved = lxchar;
				reti(LT,LT);
			}
		case A_AND:
			switch (get(lxchar)) {
			case '&':
				reti(ANDAND,ANDAND);
			case '=':
				reti(ASOP,ASAND);
			default:
				saved = lxchar;
				reti(AND,AND);
			}
		case A_OR:
			switch (get(lxchar)) {
			case '|':
				reti(OROR,OROR);
			case '=':
				reti(ASOP,ASOR);
			default:
				saved = lxchar;
				reti(OR,OR);
			}
		case A_ER:
			switch (get(lxchar)) {
			case '=':
				reti(ASOP,ASER);
			default:
				saved = lxchar;
				reti(ER,ER);
			}
		case A_PL:
			switch (get(lxchar)) {
			case '=':
				reti(ASOP,ASPLUS);
			case '+':
				reti(ICOP,INCR);
			default:
				saved = lxchar;
				reti(PLUS,PLUS);
			}
		case A_MIN:
			switch (get(lxchar)) {
			case '=':
				reti(ASOP,ASMINUS);
			case '-':
				reti(ICOP,DECR);
			case '>':
				if (get(lxchar) == '*')
					{reti(REFMUL,REF);}
				else
					saved = lxchar;
				reti(REF,REF);
			default:
				saved = lxchar;
				reti(MINUS,MINUS);
			}
		case A_MUL:
			switch (get(lxchar)) {
			case '=':
				reti(ASOP,ASMUL);
			case '/':
				error('w',"*/ not as end of comment");
			default:
				saved = lxchar;
				reti(MUL,MUL);
			}
		case A_MOD:
			switch (get(lxchar)) {
			case '=':
				reti(ASOP,ASMOD);
			default:
				saved = lxchar;
				reti(DIVOP,MOD);
			}
		default:
			{error('i',"lex act==%d getc()->%d",p,lxchar);}

		}

		error('i',"lex, main switch");
	}

}

int lxtitle()
/*
	called after a newline; set linenumber and file name
*/
{
	register c;

	for(;;)
	switch ( get(c) ) {
	default:		// e.g. not '\n', not '#'
		return c;
	case '\n':
		tloc.line++;
//		Nline++;
	ll:
		break;
	case '#': 	/* # lineno "filename" */
	{	int cl = tloc.line;
		tloc.line = 0;
		for(;;) 
		switch (get(c)) {
		case '"':
			start_txt();
			for(;;)
			switch (get(c)) {
			case '"':
				pch('\0');

				while (get(c) != '\n') ;  // skip to eol.. ignore anything more

				if (*txtstart) {	// stack file name
					if (curr_file == 0){
							char* p1 = new char[txtfree-txtstart + 1];
							(void) strcpy(p1,txtstart);
							file_name = p1;
						}
						else { //&& line is dummy #line "input.c"
							// ignore
						}
				del_txt();
				goto ll;
				}
			case '\n':
				error("unexpected end of line on '# line'");
			default:
				pch(c);
			}
		case ' ':
			break;

		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			tloc.line = tloc.line*10+c-'0'; 
			break;

		case 'l':	// look for "#line ..." and then ignore "line"
			if (get(c)=='i' && get(c)=='n' && get(c)=='e') break;
               case '\n':
                        tloc.putline();
                        goto ll;

		default: // pass #rubbish through
			tloc.line = cl;
			pch('#');
			pch(c);
			while (get(c) != '\n') pch(c);
			pch('\0');
			fprintf(out_file,"\n%s\n",txtstart);
			start_txt();
			tloc.line++;
//			Nline++;
			goto ll;
		}
	}
	}
}

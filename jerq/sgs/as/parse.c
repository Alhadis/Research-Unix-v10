
# line 2 "parse.y"
static char *ID_parsey = "@(#) parse.y: 1.23 5/8/84";
#include <stdio.h>
#include "systems.h"
#include "symbols.h"
#include "instab.h"
#include "gendefs.h"

#if	ER21FIX
static char * SWA_21 = "@(#) ER21FIX";
#endif

#if	ER16FIX
static char * SWA_16 = "@(#) ER16FIX";
#endif

#if	EXPFIX
static char * SWA_EXP = "@(#) EXPFIX";
#endif

#if	MLDVFIX
static char * SWA_BLDV = "@(#) BLDVFIX";
#endif

#if	M32RSTFIX
static char * SWA_RST = "@(#) M32RSTFIX";
#endif

#ifdef	CHIPFIX
static char * SWA_CHIP = "@(#) CHIPFIX";
#endif

static short
	expand,		/* expand byte to be generated */
	nbits,		/* number of bits of code to be generated */
	spctype,	/* distinguishes between .byte, .half, .word, .float, .double */
	width;		/* bit field width */
static short memtoreg = 0;	/* Indicator of first instruction in the
				 * 2 instruction sequence that manifests
				 * the Interrupt After TSTW chip bug.
				 */
static unsigned short
	action; /* action routine to be called */
static instr
	*newins; /* for looking up a new instruction name */
static long
	opcd;	/* opcode */
extern unsigned short
	line,	/* current line number */
	cline;	/* line number from `.ln' */
extern short
	workaround, /* software work around disable flag */
	opt,	/* optimize */
	bitpos;

#if	M32RSTFIX
extern short rstflag;
#endif	/* M32RSTFIX */

extern long
	dotbss,
	newdot; /* up-to-date value of "." */
extern symbol
	*dot;
extern FILE
#if !ONEPROC
	*fdstring,
#endif
	*fdin;
extern char
	cfile[];
extern upsymins *lookup();
extern int deflab();
extern int compare();

# line 76 "parse.y"
typedef union  {
	long uulong;
	rexpr uuexpr;
	symbol *uusymptr;
	instr *uuinsptr;
	addrmode uumode;
	char *uustrptr;
} YYSTYPE;
# define MONOP1 257
# define MONOP2 258
# define MONOP3 259
# define PUSHOP1 260
# define DYAOP1 261
# define DYAOP2 262
# define DYAOP3 263
# define DYAOP4 264
# define DYAOP5 265
# define DYAOP6 266
# define DYAOP7 267
# define DYAOP8 268
# define CMPOP1 269
# define CMPOP2 270
# define TRIOP1 271
# define TRIOP2 272
# define TRIOP3 273
# define FPDYAOP1 274
# define FPDYAOP2 275
# define FPTRIOP1 276
# define FPCMPOP1 277
# define SPRTOP1 278
# define SPRTOP2 279
# define SPRTOP3 280
# define SHFTOP1 281
# define JMPOP1 282
# define JSBOP1 283
# define JSBOP2 284
# define BSBOP1 285
# define JMBOP1 286
# define LOOPOP1 287
# define LOOPOP2 288
# define BLOCKOP 289
# define FLDOP1 290
# define FLDOP2 291
# define FLDOP4 292
# define ZEROP1 293
# define TESTOP1 294
# define CALOP1 295
# define CALOP2 296
# define SAVOP1 297
# define SAVOP2 298
# define RETOP1 299
# define PSGLOBAL 300
# define PSSET 301
# define PSALIGN 302
# define PSCOMM 303
# define PSIDENT 304
# define PSBSS 305
# define PSZERO 306
# define PSFILE 307
# define PSBYTE 308
# define PSHALF 309
# define PSWORD 310
# define PSFLOAT 311
# define PSDOUBLE 312
# define PSDEF 313
# define PSVAL 314
# define PSSCL 315
# define PSTYPE 316
# define PSTAG 317
# define PSLINE 318
# define PSSIZE 319
# define PSDIM 320
# define PSENDEF 321
# define PSLN 322
# define PSIL 323
# define SECTIONN 324
# define SIMOP 325
# define SP 326
# define QUOTE 327
# define SHARP 328
# define DOLLAR 329
# define REG 330
# define AMP 331
# define SQ 332
# define LP 333
# define RP 334
# define MUL 335
# define PLUS 336
# define COMMA 337
# define MINUS 338
# define ALPH 339
# define DIV 340
# define DIG 341
# define COLON 342
# define SEMI 343
# define QUEST 344
# define LB 345
# define RB 346
# define LC 347
# define RC 348
# define LT 349
# define GT 350
# define OR 351
# define NL 352
# define NOCHAR 353
# define ID 354
# define REGISTER 355
# define OPERSPEC 356
# define ERR 357
# define NUMBER 358
# define STRING 359
# define FPNUMBER 360
# define EXPTYPE 361
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern short yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
YYSTYPE yylval, yyval;
# define YYERRCODE 256

# line 1388 "parse.y"


#if	FLEXNAMES
char yytext[BUFSIZ];
#else
char yytext[27];
#endif

short	type[] = {
	EOF,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	SP,	NL,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	SP,	0,	QUOTE,	SHARP,	DOLLAR,	REG,	AMP,	SQ,
	LP,	RP,	MUL,	PLUS,	COMMA,	MINUS,	ALPH,	DIV,
	DIG,	DIG,	DIG,	DIG,	DIG,	DIG,	DIG,	DIG,
	DIG,	DIG,	COLON,	SEMI,	LT,	0,	GT,	QUEST,
	0,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,
	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,
	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,
	ALPH,	ALPH,	ALPH,	LB,	0,	RB,	0,	ALPH,
	0,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,
	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,
	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,
	ALPH,	ALPH,	ALPH,	LC,	OR,	RC,	0,	0,
};

yylex()
{
	upsymins istp;
	static short
		labfield = 1, /* indicates if might be in a label field */
		mnemseen = 0; /* indicates if mnemonic seen yet */
	register char *yychrptr;
	register short c;
	long val;
	register short ctype;
	short base;
#if	FLOAT
	rexpr fpval;			/* floating point constant */
	extern short validfp(), ato3bf();	/* fp service routines */
#endif

	while (type[(c = getc(fdin)) + 1] == SP)
		labfield = 0;

	switch (ctype = type[c+1]) {
	case DIG:
#if	FLOAT
		yychrptr = yytext;
		*yychrptr++ = c;	/* save first char */
#endif
		val = c - '0';
		if (c=='0') {
			c = getc(fdin);
			if (c=='x' || c=='X') {
				base = 16;
			} else {
#if	FLOAT
				if (c=='.' || c=='E' || c=='e')
					base = 10;	/* fp number */
				else
#endif
					base = 8;
				ungetc(c,fdin);
			}
		} else
			base = 10;

		while ( (type[(c = getc(fdin))+1] == DIG)
		    || ((base==16) &&
			((('a'<=c) && (c<='f'))||(('A'<=c) && (c<='F')))))
		{
			if (base==8)
				val <<= 3;
			else if (base==10) {
				val *= 10;
#if	FLOAT
				*yychrptr++ = c;	/* save for fp check */
#endif
			} else
				val <<= 4;
			if ('a'<=c && c<='f')
				val += 10 + c - 'a';
			else if ('A'<=c && c<='F')
				val += 10 + c - 'A';
			else	val += c - '0';
		}

#if	FLOAT
			/* check if its a fp number */
		if ((base==10) && (c=='.' || c=='E' || c=='e')) {
			do {
				if (yychrptr < &yytext[sizeof(yytext)-2])
					*yychrptr++ = c;
				if (c == 'E' || c == 'e') {
					/* exp for fp number, next char might
					   be a sign (PLUS/MINUS) so get it here */
					if (((c = getc(fdin))=='+') || (c=='-'))
						*yychrptr++ = c;
					else	ungetc(c,fdin);
				}
			} while (( (ctype = type[(c = getc(fdin))+1]) == DIG)
			     ||	(ctype == ALPH));

			ungetc(c,fdin);
			*yychrptr = '\0';
			if (validfp(yytext)) {
				if (ato3bf(yytext,&fpval))
					yyerror("fp conversion error");
				yylval.uuexpr = fpval;
				return(FPNUMBER);
			} else	return(ERR);

		} else {	/* if( (base==10)... */
#endif
			ungetc(c,fdin);
			yylval.uulong = val;
			return(NUMBER);
#if	FLOAT
		}
#endif

	case REG:
		yychrptr = yytext;
		while (((ctype = type[(c = getc(fdin))+1]) == ALPH)
			     ||	(ctype == DIG))
		{
#if	FLEXNAMES
			if (yychrptr < &yytext[BUFSIZ-1])
#else
			if (yychrptr < &yytext[NCPS])
#endif
				*yychrptr++ = c;
		}
		ungetc(c,fdin);
		*yychrptr = '\0';
		istp = *lookup(yytext,N_INSTALL,MNEMON);
		if ((istp.itp == NULL) || (istp.itp->tag == 0)
			|| (istp.itp->val != REGISTER-256))
			yyerror("Invalid register name");
		yylval.uuinsptr = istp.itp;
		return(REGISTER);
	case ALPH:
		yychrptr = yytext;
		do {
#if	FLEXNAMES
			if (yychrptr < &yytext[BUFSIZ-1])
#else
			if (yychrptr < &yytext[NCPS])
#endif
				*yychrptr++ = c;
		} while (((ctype = type[(c = getc(fdin))+1]) == ALPH)
			     ||	(ctype == DIG));
		ungetc(c,fdin);
		*yychrptr = '\0';
		if (labfield) {
			/* found a label; lookup USRNAME */
			istp = *lookup(yytext,INSTALL,USRNAME);
			labfield = 0;
		}
		else {
			if (mnemseen)
				istp = *lookup(yytext,INSTALL,USRNAME);
			else {
				istp = *lookup(yytext,N_INSTALL,MNEMON);
				if ((istp.stp==NULL) || (istp.stp->tag==0)) {
					yyerror("Invalid instruction name");
					return(ERR);
				}
				yylval.uuinsptr = istp.itp;
				mnemseen = 1; /* set for next time */
				return(istp.itp->val + 256);
			} /* else */
		}
		yylval.uusymptr = istp.stp;
		return(ID);
	case QUOTE:
		yylval.uustrptr = yychrptr = yytext;
		while ((c = getc(fdin)) != '"') {
#if !ONEPROC
			putc(c,fdstring);
#endif
			if (yychrptr < &yytext[sizeof(yytext)] - 1)
				*yychrptr++ = c;
		}
#if !ONEPROC
		putc('\n',fdstring);
#endif
		*yychrptr = '\0';
		return(STRING);
	case LC:
		mnemseen = 0;
		return(LC);
	case 0:
		yyerror("illegal character");
		return(NOCHAR);

	case SEMI:
		/* reinitialize for next statement */
		mnemseen = 0;
		labfield = 1;
		return(SEMI);
	case SHARP:
		/* comment; skip rest of line */
		while (getc(fdin) != '\n');
		/* same as in case NL */
	case NL:
		/* reinitialize for next statement */
		mnemseen = 0;
		labfield = 1;
		return(NL);
	case LT:
		/* start of operand descriptor */
		yychrptr = yytext;
		do {
			if (yychrptr < &yytext[NCPS])
				*yychrptr++ = c;
		} while (((ctype = type[(c = getc(fdin))+1]) == ALPH)
			     ||	(ctype == GT));
		ungetc(c,fdin);
		*yychrptr = '\0';
		istp = *lookup(yytext,N_INSTALL,MNEMON);
		if ((istp.itp == NULL) || (istp.itp->tag == 0)
			|| (istp.itp->val != OPERSPEC-256))
			yyerror("Invalid operand specifier.");
		yylval.uuinsptr = istp.itp;
		return(OPERSPEC);
	default:
		return(ctype);
	} /* switch */
} /* yylex */

fill(nbytes)
long nbytes;
{
	long fillval;

	fillval = (dot->styp == TXT)	? ((TXTFILL<<8)|TXTFILL)
					: ((FILL<<8)|FILL);
	while (nbytes >= 2) {
		generate(2*BITSPBY,NOACTION,fillval,NULLSYM);
		nbytes -= 2;
	}
	if (nbytes)
		generate(BITSPBY,NOACTION,fillval,NULLSYM);
} /* fill */

ckalign(size)
long size;
{
	long mod;
	if ((mod = newdot % size) != 0) {
		fill(size - mod);
	}
} /* ckalign */

fillbyte()
{
	if (bitpos)
		generate(BITSPBY-bitpos,NOACTION,FILL,NULLSYM);
} /* fillbyte */

bss(sym,val,alignment,valtype)
symbol *sym;
long val,alignment;
short valtype;
{
	long mod;

	if (valtype != ABS)
		yyerror("`.bss' size not absolute");
	if (val <= 0)
		yyerror("Invalid `.bss' size");
	if ((sym->styp & TYPE) != UNDEF)
		yyerror("multiply define label in `.bss'");
	if (mod = dotbss % alignment)
		dotbss += alignment - mod;
	sym->value = dotbss;
	dotbss += val;
	sym->styp |= BSS;
} /* bss */
short yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
-1, 2,
	0, 1,
	257, 7,
	258, 7,
	259, 7,
	260, 7,
	261, 7,
	262, 7,
	263, 7,
	264, 7,
	265, 7,
	266, 7,
	267, 7,
	268, 7,
	269, 7,
	270, 7,
	271, 7,
	272, 7,
	274, 7,
	275, 7,
	276, 7,
	277, 7,
	278, 7,
	279, 7,
	280, 7,
	281, 7,
	282, 7,
	283, 7,
	284, 7,
	285, 7,
	286, 7,
	287, 7,
	288, 7,
	289, 7,
	290, 7,
	291, 7,
	292, 7,
	293, 7,
	294, 7,
	295, 7,
	296, 7,
	297, 7,
	298, 7,
	299, 7,
	300, 7,
	301, 7,
	302, 7,
	303, 7,
	304, 7,
	305, 7,
	306, 7,
	307, 7,
	308, 7,
	309, 7,
	310, 7,
	311, 7,
	312, 7,
	313, 7,
	314, 7,
	315, 7,
	316, 7,
	317, 7,
	318, 7,
	319, 7,
	320, 7,
	321, 7,
	322, 7,
	323, 7,
	324, 7,
	325, 7,
	343, 7,
	352, 7,
	-2, 0,
	};
# define YYNPROD 150
# define YYLAST 1212
short yyact[]={

   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,
  17,  18,  19,  20,  21,  22, 235,  46,  47,  48,
  49,  32,  33,  34,  23,  24,  25,  26,  27,  28,
  29,  30,  31,  35,  36,  37,  38,  39,  40,  41,
  42,  44,  43,  54,  55,  58,  59,  52,  57,  56,
  64,  71,  72,  73,  74,  75,  65,  76,  77,  78,
  68,  79,  80,  81,  66,  70,  53,  51,  45,  89,
 225, 223, 141, 209, 185, 208, 103, 107,  97, 118,
 100, 194, 101, 247, 186, 107, 103, 168, 164, 251,
 100,   4, 101, 105,  98, 107, 184, 106, 159, 118,
 118, 105,  99, 163, 120, 106,  99, 311, 258, 189,
 187, 105,  99, 117, 158, 106, 118, 103, 118, 118,
 174, 100, 172, 101, 204, 202, 107, 203, 170, 205,
 169,  97, 103, 144, 145, 120, 100, 245, 101,  84,
 199, 107, 105,  99, 200, 251, 106, 143,  83, 107,
 116, 141, 290, 141, 280, 278, 107, 105, 107, 141,
 264, 106, 263, 252, 107, 105, 197, 140, 345, 106,
 141,  85, 105, 257, 105, 107, 106, 344, 106, 343,
 105, 195, 342, 324, 106, 204, 202, 325, 203,   5,
 205, 105, 323, 322, 255, 178, 204, 202, 326, 203,
 321, 205, 320, 319, 109, 254, 204, 202, 313, 203,
 318, 205,  91, 317, 316,  91,  91, 315,  91,  91,
  91,  91,  91,  91,  91,  91,  91,  91,  91, 314,
 246, 244, 138, 204, 202, 249, 203, 243, 205, 242,
  91, 201,  91, 204, 202, 312, 203, 241, 205, 240,
 160, 239,  91, 114, 204, 202, 253, 203, 238, 205,
 236,  96, 234, 233,  96,  96, 232,  96,  96,  96,
  96,  96,  96,  96,  96,  96,  96,  96, 231, 230,
 118, 118, 118, 118, 118, 118, 118, 118, 229,  96,
 228,  96, 227, 226, 224, 222, 221, 220, 219, 218,
 217,  96, 216, 215, 214, 213, 118, 212, 118, 118,
 118, 211, 210, 190, 265, 266, 267, 268, 269, 270,
 271, 272, 303, 182, 250, 183, 176, 161,  82,  69,
 175, 181,  63, 113, 303,  86,  62,  61,  60,  50,
 291,  95, 292,   6,  95,  95,   3,  95,  95,  95,
  95,  95,  95,  95,  95,  95,  95,  95,   2,   1,
 303, 162,  88, 152,  67,   0,   0,   0,   0,  95,
   0,  95,   0,   0,   0,   0, 166, 167,   0, 104,
   0,  95,   0,   0, 118, 118, 118, 118,   0,   0,
   0, 142, 179, 180, 118, 146, 147, 148,   0, 150,
   0,   0, 153, 154,   0, 156, 157,   0,   0,   0,
   0,   0,   0, 118, 118, 118,   0,   0, 329, 330,
 331, 332,   0,  91,  91,  91,  91,  91, 339,  91,
   0,  91,  91,   0,   0,   0,   0,  91,   0,   0,
   0,   0,   0,   0,   0,   0,   0, 347, 348, 349,
   0,   0,   0,   0,   0,   0,   0, 305,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0, 305, 305,
   0,   0,  96,  96,  96,  96,  96, 112,  96,   0,
  96,  96,   0, 305,   0,  94,  96, 207,  94,  94,
 237,  94,  94,  94,  94,  94,  94,  94,  94,  94,
  94,  94,   0,   0,   0,   0, 310,   0,   0,   0,
   0,   0,   0,  94,   0,  94,   0, 310, 310,   0,
   0,   0,   0,  91,   0,  94, 111,  91,  91,   0,
   0,   0,   0,   0,  93,   0,   0,  93,  93,   0,
  93,  93,  93,  93,  93,  93,  93,  93,  93,  93,
  93,   0,  95,  95,  95,  95,  95,   0,  95,   0,
  95,  95,  93,   0,  93,   0,  95, 293, 294,   0,
 300, 298,  96,   0,  93,   0,  96,  96,   0,   0,
   0,   0, 259, 260, 261, 262, 309,   0, 110,   0,
   0,   0,   0, 285, 286,   0,  92, 309, 309,  92,
  92,   0,  92,  92,  92,  92,  92,  92,  92,  92,
  92,  92,  92,   0,   0, 302,   0,   0,   0,   0,
   0,   0,   0,   0,  92,   0,  92,   0,   0,   0,
   0,   0,   0,   0,   0,   0,  92, 327, 328,   0,
   0,   0,   0, 302,   0,   0,   0,   0,   0,   0,
   0, 341,  95, 115,   0,   0,  95,  95,   0,   0,
   0,  90,   0,   0,  90,  90,   0,  90,  90,  90,
  90,  90,  90,  90,  90,  90,  90,  90,   0,   0,
   0,   0,   0,   0, 336,   0,   0,   0,   0,  90,
   0,  90,   0,   0, 119,   0,  94,  94,  94,  94,
  94,  90,  94, 108,  94,  94,   0, 123,   0,   0,
  94,   0,   0,   0,   0,   0,   0,   0,   0, 135,
 136, 137,   0,   0,   0,   0,   0,   0,   0,   0,
 308,   0,   0,   0,   0,   0, 155,   0,   0,   0,
   0, 308, 308,   0,   0,  93,  93,  93,  93,  93,
   0,  93,   0,  93,  93,   0,   0,   0,   0,  93,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0, 307,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
 307, 307,   0,   0,   0,   0,  94,   0,   0,   0,
  94,  94,   0,   0,   0,   0,   0,  92,  92,  92,
  92,  92,   0,  92,   0,  92,  92,   0,   0,   0,
   0,  92,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0, 306,   0,   0,   0,  93,  87,   0,   0,  93,
  93,   0, 306, 306,   0,   0,   0, 121, 122,   0,
 124, 125, 126, 127, 128, 129, 130, 131, 132, 133,
 134,   0,  90,  90,  90,  90,  90,   0,  90,   0,
  90,  90, 149,   0, 151, 102,  90,   0,   0,   0,
   0,   0,   0,   0, 165,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0, 304,  92,   0,   0,
   0,  92,  92, 139,   0,   0,   0, 304, 304,   0,
   0,   0,   0, 283, 284,   0,   0,   0, 288, 289,
   0, 139,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0, 171,   0, 173,   0, 177, 177, 177,   0,
   0,   0,   0, 188,   0,   0, 191,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0, 192,   0,   0,
   0,   0,  90,   0,   0,   0,  90,  90,   0,   0,
   0,   0,   0, 193,   0,   0, 196, 198,   0, 206,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0, 334, 335,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0, 196,   0,   0,
   0,   0,   0,   0,   0,   0,   0, 346,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0, 273, 274, 275, 276, 277,
   0, 279,   0, 281, 282,   0, 248,   0,   0, 287,
   0,   0,   0,   0,   0, 256, 196,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0, 295, 296, 297,
 177, 299,   0,   0,   0, 301,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0, 139, 333,   0,   0,   0, 337,
 338,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0, 340 };
short yypact[]={

-1000,-1000,-165,-257,-204,-171,-204,-253,-197,-212,
-253,-253,-197,-253,-253,-253,-253,-253,-253,-253,
-253,-253,-253,-253,-197,-197,-197,-180,-200,-212,
-212,-1000,-200,-200,-200,-253,-200,-253,-1000,-200,
-200,-197,-200,-200,-249,-180,-243,-253,-243,-243,
-1000,-1000,-272,-1000,-224,-226,-261,-232,-261,-234,
-163,-163,-163,-264,-275,-244,-1000,-261,-245, -24,
-261,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-261,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,-1000,-1000,-261,-280,-1000,
-174,-189, -92,-261,-1000,-1000,-1000,-261,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,-1000,-286,-1000,-1000,-1000,
-288,-1000, -25, -26, -30, -32, -33, -34, -35, -37,
 -38, -39, -40, -41, -42,-1000,-1000,-1000,-1000, -81,
-290,-261, -43,-291, -44, -45,-1000, -47, -49, -58,
 -59, -71,-1000,-1000, -74, -75,-1000,-1000,-1000,-345,
-1000, -77,-1000,-1000,-264, -79, -86, -88,-1000,-1000,
 -90, -81, -98, -81,-100,-106,-1000, -81,-205,-106,
-106,-107,-1000,-1000,-1000,-277,-1000,-1000, -81,-1000,
-261,-102, -81,-211,-185, -78,-129,-1000,-139,-261,
-182,-247,-261,-261,-261,-261, -81,-1000,-186,-188,
-212,-212,-212,-212,-212,-212,-212,-212,-253,-253,
-253,-253,-253,-193,-253,-194,-253,-253,-197,-197,
-200,-200,-253,-197,-197,-196,-212,-1000,-212,-243,
-243,-261,-261,-261,-163,-261,-264,-1000, -81,-261,
-1000,-1000,-253,-1000,-1000,-248, -81, -89,-126,-1000,
-1000,-1000,-1000,-197,-212,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,-108,-120,-123,-180,-124,
-200,-127,-134,-1000,-135,-137,-144,-145,-1000,-1000,
-249,-1000,-1000,-154,-1000, -81,-150, -81,-1000, -81,
-1000, -81,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-136,-267,-267,-212,-212,-212,-212,-253,-197,
-197,-200,-253,-253,-212,-261,-267,-1000,-1000,-1000,
-1000,-1000,-1000,-155,-1000,-1000,-158,-160,-169,-1000,
 -81,-1000,-197,-212,-212,-212,-1000,-1000,-1000,-1000 };
short yypgo[]={

   0, 364, 324, 885, 379, 325, 846, 103, 694, 362,
  69, 653, 363, 327, 361, 204, 588, 526, 477, 333,
 253, 359, 358, 346, 343, 328, 339, 338, 330, 337,
 336, 332, 331, 329, 326, 323 };
short yyr1[]={

   0,  21,  22,  22,  22,  25,  25,  23,  23,  24,
  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,
  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,
  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,
  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,
  24,  24,  24,  24,   6,   6,   6,   6,   6,   6,
   6,   6,   6,  13,  13,   7,   7,   8,   8,   8,
   8,   8,   8,   8,   2,   2,  12,  12,   9,   9,
  14,  10,  10,  11,  11,  11,  15,  15,  16,  16,
  17,  17,  18,  18,  19,  19,  20,  20,  20,   3,
   3,   3,   3,   3,   4,   4,   4,   4,  26,  26,
  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,
  26,  26,  26,  26,  26,  26,  26,  26,  26,   1,
   1,   1,   1,   1,  33,  33,  27,  29,  30,  31,
  31,  28,  28,  34,  34,  32,  32,  35,   5,   5 };
short yyr2[]={

   0,   1,   0,   4,   3,   1,   1,   0,   2,   0,
   2,   2,   2,   2,   4,   4,   4,   4,   4,   4,
   4,   4,   4,   4,   6,   6,   6,   2,   2,   2,
   2,   6,   8,   6,   1,   2,   4,   6,   8,   8,
   8,   1,   2,   4,   4,   2,   2,   2,   2,   4,
   4,   6,   4,   1,   1,   1,   1,   1,   1,   1,
   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
   1,   1,   1,   1,   1,   0,   1,   0,   3,   4,
   2,   1,   4,   3,   2,   4,   1,   4,   2,   4,
   2,   4,   3,   4,   5,   4,   6,   5,   4,   1,
   3,   3,   3,   3,   1,   1,   3,   2,   1,   2,
   1,   2,   4,   2,   6,   2,   4,   2,   2,   2,
   2,   2,   2,   1,   2,   2,   1,   2,   4,   1,
   1,   1,   1,   1,   2,   3,   1,   1,   1,   1,
   1,   1,   3,   1,   3,   1,   3,   1,   1,   2 };
short yychk[]={

-1000, -21, -22, -23, 256, 354, -24, 257, 258, 259,
 260, 261, 262, 263, 264, 265, 266, 267, 268, 269,
 270, 271, 272, 281, 282, 283, 284, 285, 286, 287,
 288, 289, 278, 279, 280, 290, 291, 292, 293, 294,
 295, 296, 297, 299, 298, 325, 274, 275, 276, 277,
 -26, 324, 304, 323, 300, 301, 306, 305, 302, 303,
 -27, -29, -30, -31, 307, 313, 321,  -1, 317, -33,
 322, 308, 309, 310, 311, 312, 314, 315, 316, 318,
 319, 320, -25, 352, 343, 342, -25,  -6,  -9, -10,
 -11, -15, -16, -17, -18, -19, -20, 331, 347, 355,
 333, 335,  -3, 329,  -4, 354, 358, 338,  -8, -15,
 -16, -17, -18, -19, -20, -11, 347,  -7, -10,  -8,
 347,  -6,  -6,  -8,  -6,  -6,  -6,  -6,  -6,  -6,
  -6,  -6,  -6,  -6,  -6,  -8,  -8,  -8, -15,  -3,
 347, 333,  -9, 347,  -7,  -7,  -9,  -9,  -9,  -6,
  -9,  -6, -12,  -9,  -9,  -8,  -9,  -9, -10, 347,
 -15, -13, -14,  -7, 331,  -6, -13, -13, 359, 354,
 354,  -3, 354,  -3, 354, -28, -34,  -3, 358, -28,
 -28, -32, -35,  -5, 360, 338, 359, 354,  -3, 354,
 337,  -3,  -3,  -3, 361, 355,  -3, 355,  -3, 329,
 333, 333, 336, 338, 335, 340,  -3,  -4, 361, 361,
 337, 337, 337, 337, 337, 337, 337, 337, 337, 337,
 337, 337, 337, 361, 337, 361, 337, 337, 337, 337,
 337, 337, 337, 337, 337, 361, 337,  -5, 337, 337,
 337, 337, 337, 337, 337, 342, 337, 360,  -3, 337,
  -2, 356, 348, 334, 334, 333,  -3, 355, 355,  -4,
  -4,  -4,  -4, 348, 348,  -7,  -7,  -7,  -7,  -7,
  -7,  -7,  -7,  -6,  -6,  -6,  -6,  -6, 348,  -6,
 348,  -6,  -6,  -8,  -8,  -9,  -9,  -6,  -8,  -8,
 348,  -7,  -7, -13, -13,  -3,  -3,  -3, -34,  -3,
 -35,  -3,  -9, -10, -11, -15, -16, -17, -18, -19,
 -20, 355, 334, 334, 337, 337, 337, 337, 337, 337,
 337, 337, 337, 337, 337, 337, 334,  -2,  -2,  -7,
  -7,  -7,  -7,  -6,  -8,  -8,  -9,  -6,  -6,  -7,
  -3,  -2, 337, 337, 337, 337,  -8,  -7,  -7,  -7 };
short yydef[]={

   2,  -2,  -2,   9,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,  34,   0,   0,   0,   0,   0,   0,  41,  77,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  53, 108,   0, 110,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0, 123,   0,   0, 126,
   0, 136, 137, 138, 139, 140, 129, 130, 131, 132,
 133,   0,   4,   5,   6,   8,   3,  10,  54,  55,
  56,  57,  58,  59,  60,  61,  62,   0,   0,  81,
   0,   0,  86,   0,  99, 104, 105,   0,  11,  67,
  68,  69,  70,  71,  72,  73,   0,  12,  65,  66,
   0,  13,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,  27,  28,  29,  30,  86,
   0,   0,   0,   0,   0,   0,  35,   0,   0,   0,
   0,   0,  42,  76,   0,   0,  45,  46,  47,   0,
  48,   0,  63,  64,   0,   0,   0,   0, 109, 111,
   0, 113,   0, 115,   0, 117, 141, 143, 105, 118,
 119, 120, 145, 147, 148,   0, 121, 122, 124, 125,
   0, 127, 134,  75,   0,   0,   0,  84,  90,   0,
   0,   0,   0,   0,   0,   0,  88, 107,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,  80,   0,   0,
   0,   0,   0,   0,   0,   0,   0, 149, 135,   0,
  78,  74,   0,  83, 106,   0,  92,   0,   0, 100,
 101, 102, 103,   0,   0,  14,  15,  16,  17,  18,
  19,  20,  21,  22,  23,   0,   0,   0,   0,   0,
   0,   0,   0,  36,   0,   0,   0,   0,  43,  44,
   0,  49,  50,   0,  52, 112,   0, 116, 142, 144,
 146, 128,  79,  82,  85,  87,  89,  91,  93,  95,
  98,   0,  75,  75,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,  75,  97,  94,  24,
  25,  26,  31,   0,  33,  37,   0,   0,   0,  51,
 114,  96,   0,   0,   0,   0,  32,  38,  39,  40 };
# ifdef YYDEBUG
# include "y.debug"
# endif

# define YYFLAG -1000
# define YYERROR goto yyerrlab
# define YYACCEPT return(0)
# define YYABORT return(1)

/*	parser for yacc output	*/

#ifdef YYDEBUG
int yydebug = 0; /* 1 for debugging */
#endif
YYSTYPE yyv[YYMAXDEPTH]; /* where the values are stored */
int yychar = -1; /* current input token number */
int yynerrs = 0;  /* number of errors */
short yyerrflag = 0;  /* error recovery flag */

yyparse()
{	short yys[YYMAXDEPTH];
	int yyj, yym;
	register YYSTYPE *yypvt;
	register int yystate, yyn;
	register short *yyps;
	register YYSTYPE *yypv;
	register short *yyxi;

	yystate = 0;
	yychar = -1;
	yynerrs = 0;
	yyerrflag = 0;
	yyps= &yys[-1];
	yypv= &yyv[-1];

yystack:    /* put a state and value onto the stack */
#ifdef YYDEBUG
	if(yydebug >= 3)
		if(yychar < 0 || yytoknames[yychar] == 0)
			printf("char %d in %s", yychar, yystates[yystate]);
		else
			printf("%s in %s", yytoknames[yychar], yystates[yystate]);
#endif
	if( ++yyps >= &yys[YYMAXDEPTH] ) { 
		yyerror( "yacc stack overflow" ); 
		return(1); 
	}
	*yyps = yystate;
	++yypv;
	*yypv = yyval;
yynewstate:
	yyn = yypact[yystate];
	if(yyn <= YYFLAG) goto yydefault; /* simple state */
	if(yychar<0) {
		yychar = yylex();
#ifdef YYDEBUG
		if(yydebug >= 2) {
			if(yychar <= 0)
				printf("lex EOF\n");
			else if(yytoknames[yychar])
				printf("lex %s\n", yytoknames[yychar]);
			else
				printf("lex (%c)\n", yychar);
		}
#endif
		if(yychar < 0)
			yychar = 0;
	}
	if((yyn += yychar) < 0 || yyn >= YYLAST)
		goto yydefault;
	if( yychk[ yyn=yyact[ yyn ] ] == yychar ){ /* valid shift */
		yychar = -1;
		yyval = yylval;
		yystate = yyn;
		if( yyerrflag > 0 ) --yyerrflag;
		goto yystack;
	}
yydefault:
	/* default state action */
	if( (yyn=yydef[yystate]) == -2 ) {
		if(yychar < 0) {
			yychar = yylex();
#ifdef YYDEBUG
			if(yydebug >= 2)
				if(yychar < 0)
					printf("lex EOF\n");
				else
					printf("lex %s\n", yytoknames[yychar]);
#endif
			if(yychar < 0)
				yychar = 0;
		}
		/* look through exception table */
		for(yyxi=yyexca; (*yyxi!= (-1)) || (yyxi[1]!=yystate);
			yyxi += 2 ) ; /* VOID */
		while( *(yyxi+=2) >= 0 ){
			if( *yyxi == yychar ) break;
		}
		if( (yyn = yyxi[1]) < 0 ) return(0);   /* accept */
	}
	if( yyn == 0 ){ /* error */
		/* error ... attempt to resume parsing */
		switch( yyerrflag ){
		case 0:   /* brand new error */
#ifdef YYDEBUG
			yyerror("syntax error\n%s", yystates[yystate]);
			if(yytoknames[yychar])
				yyerror("saw %s\n", yytoknames[yychar]);
			else if(yychar >= ' ' && yychar < '\177')
				yyerror("saw `%c'\n", yychar);
			else if(yychar == 0)
				yyerror("saw EOF\n");
			else
				yyerror("saw char 0%o\n", yychar);
#else
			yyerror( "syntax error" );
#endif
yyerrlab:
			++yynerrs;
		case 1:
		case 2: /* incompletely recovered error ... try again */
			yyerrflag = 3;
			/* find a state where "error" is a legal shift action */
			while ( yyps >= yys ) {
				yyn = yypact[*yyps] + YYERRCODE;
				if( yyn>= 0 && yyn < YYLAST && yychk[yyact[yyn]] == YYERRCODE ){
					yystate = yyact[yyn];  /* simulate a shift of "error" */
					goto yystack;
				}
				yyn = yypact[*yyps];
				/* the current yyps has no shift onn "error", pop stack */
#ifdef YYDEBUG
				if( yydebug ) printf( "error recovery pops state %d, uncovers %d\n", *yyps, yyps[-1] );
#endif
				--yyps;
				--yypv;
			}
			/* there is no state on the stack with an error shift ... abort */
yyabort:
			return(1);
		case 3:  /* no shift yet; clobber input char */
#ifdef YYDEBUG
			if( yydebug ) {
				printf("error recovery discards ");
				if(yytoknames[yychar])
					printf("%s\n", yytoknames[yychar]);
				else if(yychar >= ' ' && yychar < '\177')
					printf("`%c'\n", yychar);
				else if(yychar == 0)
					printf("EOF\n");
				else
					printf("char 0%o\n", yychar);
			}
#endif
			if( yychar == 0 ) goto yyabort; /* don't discard EOF, quit */
			yychar = -1;
			goto yynewstate;   /* try again in the same state */
		}
	}
	/* reduction by production yyn */
#ifdef YYDEBUG
	if(yydebug) {	char *s;
		printf("reduce %d in:\n\t", yyn);
		for(s = yystates[yystate]; *s; s++) {
			putchar(*s);
			if(*s == '\n' && *(s+1))
				putchar('\t');
		}
	}
#endif
	yyps -= yyr2[yyn];
	yypvt = yypv;
	yypv -= yyr2[yyn];
	yyval = yypv[1];
	yym=yyn;
	/* consult goto table to find next state */
	yyn = yyr1[yyn];
	yyj = yypgo[yyn] + *yyps + 1;
	if( yyj>=YYLAST || yychk[ yystate = yyact[yyj] ] != -yyn ) yystate = yyact[yypgo[yyn]];
	switch(yym){
		
case 3:
# line 130 "parse.y"
 {
			dot->value = newdot; /* syncronize */
		} break;
case 4:
# line 133 "parse.y"
 {
			yyerrok;
			dot->value = newdot; /* syncronize */
		} break;
case 5:
# line 138 "parse.y"
 {
			++line;
			generate(0,NEWSTMT,(long)line,NULLSYM);
		} break;
case 6:
# line 142 "parse.y"
 {
			generate(0,NEWSTMT,(long)line,NULLSYM);
		} break;
case 8:
# line 147 "parse.y"
 {
			if ((yypvt[-1].uusymptr->styp & TYPE) != UNDEF)
				yyerror("error: multiply defined label");
			yypvt[-1].uusymptr->value = newdot;
			yypvt[-1].uusymptr->styp |= dot->styp;
			if (dot->styp == TXT)
				deflab(yypvt[-1].uusymptr);
		} break;
case 10:
# line 157 "parse.y"
 {
			/*
			 * PUSHW pushw SWAP[BHW]I
			 * (TST|CLR|INC|DEC)[BHW]
			 *	instructions
			 */
#if	EXPFIX
			if (compare(yypvt[-1].uuinsptr->name,"PUSHW") == EQUAL && workaround)
				if (yypvt[-0].uumode.newtype != NOTYPE)
					yyerror
					("Expand byte invalid in PUSHW instruction");
#endif
			pswcheck(yypvt[-1].uuinsptr, &(yypvt[-0].uumode));
			if (compare(yypvt[-1].uuinsptr->name,"CLRW") == EQUAL ||
			    compare(yypvt[-1].uuinsptr->name,"DECW") == EQUAL ||
			    compare(yypvt[-1].uuinsptr->name,"INCW") == EQUAL ||
			    compare(yypvt[-1].uuinsptr->name,"TSTW") == EQUAL)
				intaft1check(&(yypvt[-0].uumode), memtoreg);

			generate(8,NOACTION,yypvt[-1].uuinsptr->opcode,NULLSYM);
			addrgen(yypvt[-1].uuinsptr,&(yypvt[-0].uumode),NOTYPE,1);

			if (compare(yypvt[-1].uuinsptr->name,"PUSHW") != EQUAL &&
			    compare(yypvt[-1].uuinsptr->name,"pushw") != EQUAL) {
				pcintcheck(yypvt[-1].uuinsptr,&(yypvt[-0].uumode));	/* don't fix PUSHW */
			}
			memtoreg = 0;
	} break;
case 11:
# line 185 "parse.y"
 {
			/* PUSHAW pushaw instruction */
#if	EXPFIX
			if (compare(yypvt[-1].uuinsptr->name,"pushaw") != EQUAL && workaround)
				if (yypvt[-0].uumode.newtype != NOTYPE)
					yyerror
					("Expand byte invalid in PUSHAW,SWAPWI,SWAPHI, and SWAPBI instructions");
#endif
			generate(8,NOACTION,yypvt[-1].uuinsptr->opcode,NULLSYM);
			addrgen(yypvt[-1].uuinsptr,&(yypvt[-0].uumode),NOTYPE,1);
			memtoreg = 0;
		} break;
case 12:
# line 197 "parse.y"
 {
			/* POPW instruction */
#if	EXPFIX
			if( ( yypvt[-0].uumode.newtype != NOTYPE ) && workaround )
				yyerror
				("Expand byte illegal in POPW Instruction.");
#endif
			generate(8,NOACTION,yypvt[-1].uuinsptr->opcode,NULLSYM);
			addrgen(yypvt[-1].uuinsptr,&(yypvt[-0].uumode),NOTYPE,1);
			memtoreg = 0;
		} break;
case 13:
# line 208 "parse.y"
 {
			/* push[zb][bh] instructions */
			pushopgen(yypvt[-1].uuinsptr,&(yypvt[-0].uumode));
			pcintcheck(yypvt[-1].uuinsptr,&(yypvt[-0].uumode));
			memtoreg = 0;
		} break;
case 14:
# line 214 "parse.y"
 {
			/*
			 * (AND|and)[BHW]2 (OR|or)[BHW]2 (XOR|xor)[BHW]2
			 * MOD[BHW]2 MUL[BHW]2 DIV[BHW]2 modw2 mulw2 divw2
			 * (MCOM|mcom)[BHW] MNEG[BHW]
			 *	instructions
			 */
#if	MLDVFIX
			mulchk3(yypvt[-3].uuinsptr,&(yypvt[-2].uumode),&(yypvt[-0].uumode),&(yypvt[-0].uumode));
#endif

			/* Interrupt After TSTW Chip Bug
			 * Check if instruction does a store to
			 * a register and is a multiple cycle
			 * ALU instruction (ie. multiply, divide,
			 * modulo, move negate). If so, then set
			 * memtoreg so that a NOP will be generated
			 * after this instruction if necessary.
			 */

			if ( pswcheck(yypvt[-3].uuinsptr, &(yypvt[-2].uumode)) )
				pswcheck(yypvt[-3].uuinsptr, &(yypvt[-0].uumode));

#ifdef	CHIPFIX
			if ((yypvt[-0].uumode.admode == REGMD) &&
			    (((yypvt[-3].uuinsptr->name[0] == 'M') && (yypvt[-3].uuinsptr->name[1] != 'C')) ||
			     ((yypvt[-3].uuinsptr->name[0] == 'm') && (yypvt[-3].uuinsptr->name[1] != 'c')) ||
			     (yypvt[-3].uuinsptr->name[0] == 'D') ||
			     (yypvt[-3].uuinsptr->name[0] == 'd')))
				memtoreg = 1;
			else if (compare(yypvt[-3].uuinsptr->name, "MCOMW") == EQUAL ||
				 compare(yypvt[-3].uuinsptr->name, "mcomw") == EQUAL) {
					intaft2check(&(yypvt[-2].uumode), &(yypvt[-0].uumode), memtoreg);
					memtoreg = mtoregcheck(&(yypvt[-2].uumode), &(yypvt[-0].uumode));
			}
			else
				memtoreg = mtoregcheck(&(yypvt[-2].uumode), &(yypvt[-0].uumode));
#endif
			generate(8,NOACTION,yypvt[-3].uuinsptr->opcode,NULLSYM);
			addrgen(yypvt[-3].uuinsptr,&(yypvt[-2].uumode),NOTYPE,1);
			addrgen(yypvt[-3].uuinsptr,&(yypvt[-0].uumode),NOTYPE,2);
			pcintcheck(yypvt[-3].uuinsptr,&(yypvt[-0].uumode));
		} break;
case 15:
# line 257 "parse.y"
 {
			/* MOVAW movaw MOVTRW instructions */
			generate(8,NOACTION,yypvt[-3].uuinsptr->opcode,NULLSYM);
			addrgen(yypvt[-3].uuinsptr,&(yypvt[-2].uumode),NOTYPE,1);
			addrgen(yypvt[-3].uuinsptr,&(yypvt[-0].uumode),NOTYPE,2);
			pcintcheck(yypvt[-3].uuinsptr,&(yypvt[-0].uumode));
			memtoreg = mtoregcheck(&(yypvt[-2].uumode), &(yypvt[-0].uumode));
		} break;
case 16:
# line 265 "parse.y"
 {
			/*
			 * [al][lr]sw2 instructions
			 *
			 * generate code using triadic opcode,
			 * repeating 2nd operand
			 */
			pswcheck(yypvt[-3].uuinsptr, &(yypvt[-2].uumode));
			shftopgen(yypvt[-3].uuinsptr,&(yypvt[-2].uumode),&(yypvt[-0].uumode),&(yypvt[-0].uumode));
			pcintcheck(yypvt[-3].uuinsptr,&(yypvt[-0].uumode));
			memtoreg = mtoregcheck(&(yypvt[-2].uumode), &(yypvt[-0].uumode));
		} break;
case 17:
# line 277 "parse.y"
 {
			/*
			 * (MOV|mov)[bhw] instructions
			 *
			 * if first operand is &0, use CLR opcodes
			 * stored in 2nd low byte of opcode
			 */
			/* The following call to opt21gen
			 * is not always executed out because of the
			 * Interrupt After TSTW chip bug
			 */
			pswcheck(yypvt[-3].uuinsptr, &(yypvt[-2].uumode));
			if (compare(yypvt[-3].uuinsptr->name, "MOVW") == EQUAL ||
			    compare(yypvt[-3].uuinsptr->name, "movw") == EQUAL) {
				if (yypvt[-0].uumode.admode != REGMD || memtoreg == 0)
					opt21gen(yypvt[-3].uuinsptr,&(yypvt[-2].uumode),&(yypvt[-0].uumode),0);
				else {
					intaft2check(&(yypvt[-2].uumode), &(yypvt[-0].uumode), memtoreg);
					generate(8,NOACTION,yypvt[-3].uuinsptr->opcode & 0xFFL,NULLSYM);
					addrgen(yypvt[-3].uuinsptr, &(yypvt[-2].uumode), NOTYPE, 1);
					addrgen(yypvt[-3].uuinsptr, &(yypvt[-0].uumode), NOTYPE, 2);
				}
			}
			else
				opt21gen(yypvt[-3].uuinsptr,&(yypvt[-2].uumode),&(yypvt[-0].uumode),0);
		memtoreg = mtoregcheck(&(yypvt[-2].uumode), &(yypvt[-0].uumode));
		pcintcheck(yypvt[-3].uuinsptr, &(yypvt[-0].uumode));
		} break;
case 18:
# line 305 "parse.y"
 {
			/* movb[bh|bw|hw]  movt[wb|hb|wh] instructions */

			pswcheck(yypvt[-3].uuinsptr, &(yypvt[-2].uumode));
			convgen(yypvt[-3].uuinsptr,&(yypvt[-2].uumode),&(yypvt[-0].uumode));
			pcintcheck(yypvt[-3].uuinsptr,&(yypvt[-0].uumode));
			memtoreg = mtoregcheck(&(yypvt[-2].uumode), &(yypvt[-0].uumode));
		} break;
case 19:
# line 313 "parse.y"
 {
			/*
			 * (ADD|SUB|add|sub)[bhw]2 instructions
			 *
			 * if first operand is &1, use INC (DEC) opcodes
			 * stored in 2nd low byte of opcode
			 */
			/* The following call to opt21gen
			 * is not always executed because of the
			 * Interrupt After TSTW chip bug
			 */
			pswcheck(yypvt[-3].uuinsptr, &(yypvt[-2].uumode));
			if (yypvt[-0].uumode.admode != REGMD || memtoreg == 0)
				opt21gen(yypvt[-3].uuinsptr,&(yypvt[-2].uumode),&(yypvt[-0].uumode),1);
			else {
				generate(8,NOACTION,yypvt[-3].uuinsptr->opcode & 0xFFL,NULLSYM);
				addrgen(yypvt[-3].uuinsptr, &(yypvt[-2].uumode), NOTYPE, 1);
				addrgen(yypvt[-3].uuinsptr, &(yypvt[-0].uumode), NOTYPE, 2);
			}
			memtoreg = mtoregcheck(&(yypvt[-2].uumode), &(yypvt[-0].uumode));
			pcintcheck(yypvt[-3].uuinsptr,&(yypvt[-0].uumode));
		} break;
case 20:
# line 335 "parse.y"
 {
			/* movz[bh|bw|hw] instructions */
			pswcheck(yypvt[-3].uuinsptr, &(yypvt[-2].uumode));
			generate(8,NOACTION,yypvt[-3].uuinsptr->opcode,NULLSYM);
			expand = (optype(yypvt[-3].uuinsptr->tag,1) == 0) ? NOTYPE : UHALF;
			addrgen(yypvt[-3].uuinsptr,&(yypvt[-2].uumode),expand,1);
			/* 2nd operand is signed (halfword/word) */
			expand = (optype(yypvt[-3].uuinsptr->tag,2) == 1) ? UHALF : UWORD;
			addrgen(yypvt[-3].uuinsptr,&(yypvt[-0].uumode),expand,2);
			pcintcheck(yypvt[-3].uuinsptr,&(yypvt[-0].uumode));
			memtoreg = mtoregcheck(&(yypvt[-2].uumode), &(yypvt[-0].uumode));
		} break;
case 21:
# line 347 "parse.y"
 {
			/* u(mul|div|mod)w2 instructions */
			generate(8,NOACTION,yypvt[-3].uuinsptr->opcode,NULLSYM);
			addrgen(yypvt[-3].uuinsptr,&(yypvt[-2].uumode),UWORD,1);
			addrgen(yypvt[-3].uuinsptr,&(yypvt[-0].uumode),NOTYPE,2);
			pcintcheck(yypvt[-3].uuinsptr,&(yypvt[-0].uumode));
			memtoreg = 1;
		} break;
case 22:
# line 355 "parse.y"
 {
			/*
			 * (cmp|CMP)[bhw] instructions
			 *
			 * For IS25 versions, interchange operands and
			 * if first operand is &0, use TST opcodes
			 * stored in 2nd low byte of opcode
			 */

			if ( pswcheck(yypvt[-3].uuinsptr, &(yypvt[-2].uumode)) )
				pswcheck(yypvt[-3].uuinsptr, &(yypvt[-0].uumode));
			if (yypvt[-3].uuinsptr->tag & IS25) {
				/* The following call to opt21gen
				 * is not always executed because of the
				 * Interrupt After TSTW chip bug
				 */
				if (yypvt[-2].uumode.admode != REGMD || memtoreg == 0 ||
				    compare(yypvt[-3].uuinsptr->name,"cmpw") != EQUAL)
					opt21gen(yypvt[-3].uuinsptr,&(yypvt[-0].uumode),&(yypvt[-2].uumode),0);
				else {
					generate(8,NOACTION,yypvt[-3].uuinsptr->opcode,NULLSYM);
					addrgen(yypvt[-3].uuinsptr,&(yypvt[-0].uumode),NOTYPE,1);
					addrgen(yypvt[-3].uuinsptr,&(yypvt[-2].uumode),NOTYPE,2);
				}
			}
			else {
				/* generate normal sequence */
				generate(8,NOACTION,yypvt[-3].uuinsptr->opcode,NULLSYM);
				addrgen(yypvt[-3].uuinsptr,&(yypvt[-2].uumode),NOTYPE,1);
				addrgen(yypvt[-3].uuinsptr,&(yypvt[-0].uumode),NOTYPE,2);
			}
			memtoreg = 0;
		} break;
case 23:
# line 388 "parse.y"
 {
			/* (BIT|bit)[bhw] instructions */
			if ( pswcheck(yypvt[-3].uuinsptr, &(yypvt[-2].uumode)) )
				pswcheck(yypvt[-3].uuinsptr, &(yypvt[-0].uumode));
			generate(8,NOACTION,yypvt[-3].uuinsptr->opcode,NULLSYM);
			addrgen(yypvt[-3].uuinsptr,&(yypvt[-2].uumode),NOTYPE,1);
			addrgen(yypvt[-3].uuinsptr,&(yypvt[-0].uumode),NOTYPE,2);
			memtoreg = 0;
		} break;
case 24:
# line 397 "parse.y"
 {
			/*
			 * (AND|and)[bhw]3 (OR|or)[bhw]3 (XOR|xor)[bhw]3
			 * (ADD|add)[bhw]3 (SUB|sub)[bhw]3
			 * (MOD|mod)[BHW]3 (MUL|mul)[BHW]3 (DIV|div)[BHW]3
			 * (AR|LL)S[BHW]3 ALSW3 ROTW
			 *	instructions
			 */
			if ( pswcheck(yypvt[-5].uuinsptr, &(yypvt[-4].uumode)) )
				pswcheck(yypvt[-5].uuinsptr, &(yypvt[-2].uumode));
#if	MLDVFIX
			mulchk3(yypvt[-5].uuinsptr,&(yypvt[-4].uumode),&(yypvt[-2].uumode),&(yypvt[-0].uumode));
#endif
#if	ER21FIX
			er21(yypvt[-5].uuinsptr,&(yypvt[-4].uumode),&(yypvt[-2].uumode),&(yypvt[-0].uumode));
#endif
#if	0	/* trimul software workaround */
			if ((yypvt[-5].uuinsptr->name[0] == 'M') || (yypvt[-5].uuinsptr->name[0] == 'm') ||
			    (yypvt[-5].uuinsptr->name[0] == 'D') || (yypvt[-5].uuinsptr->name[0] == 'd')) {
				trimulcheck(yypvt[-5].uuinsptr, &(yypvt[-4].uumode), &(yypvt[-2].uumode), &(yypvt[-0].uumode));
				memtoreg = 0;
			}
			else {
#endif
#if	ER16FIX
			if ( er16fix(yypvt[-5].uuinsptr, &(yypvt[-4].uumode), &(yypvt[-2].uumode), &(yypvt[-0].uumode)) )
			{
				memtoreg = 0;
			}
			else {
#endif
				generate(8,NOACTION,yypvt[-5].uuinsptr->opcode,NULLSYM);
				addrgen(yypvt[-5].uuinsptr,&(yypvt[-4].uumode),NOTYPE,1);
				addrgen(yypvt[-5].uuinsptr,&(yypvt[-2].uumode),NOTYPE,2);
				addrgen(yypvt[-5].uuinsptr,&(yypvt[-0].uumode),NOTYPE,3);
				pcintcheck(yypvt[-5].uuinsptr,&(yypvt[-0].uumode));
				memtoreg = mtoregcheck(&(yypvt[-2].uumode), &(yypvt[-0].uumode));
#if	ER16FIX
			}
#endif
#if	0	/* trimul software workaround */
			}
#endif
		} break;
case 25:
# line 441 "parse.y"
 {
			/* u(mul|div|mod)w3 instructions */
#if	0	/* trimul software workaround */
			if (yypvt[-5].uuinsptr->name[0] == 'u')
				trimulcheck(yypvt[-5].uuinsptr, &(yypvt[-4].uumode), &(yypvt[-2].uumode), &(yypvt[-0].uumode));
			else {
#endif
#if	ER16FIX
			if ( er16fix(yypvt[-5].uuinsptr, &(yypvt[-4].uumode), &(yypvt[-2].uumode), &(yypvt[-0].uumode)) )
				;
			else {
#endif
				generate(8,NOACTION,yypvt[-5].uuinsptr->opcode,NULLSYM);
				addrgen(yypvt[-5].uuinsptr,&(yypvt[-4].uumode),UWORD,1);
				addrgen(yypvt[-5].uuinsptr,&(yypvt[-2].uumode),NOTYPE,2);
				addrgen(yypvt[-5].uuinsptr,&(yypvt[-0].uumode),NOTYPE,3);
				pcintcheck(yypvt[-5].uuinsptr,&(yypvt[-0].uumode));
#if	ER16FIX
			}
#endif
#if	0	/* trimul software workaround */
			}
#endif
			memtoreg = 0;
		} break;
case 26:
# line 466 "parse.y"
 {
			/* [al][lr]sw3 instructions */
			if ( pswcheck(yypvt[-5].uuinsptr, &(yypvt[-4].uumode)) )
				pswcheck(yypvt[-5].uuinsptr, &(yypvt[-2].uumode));
			shftopgen(yypvt[-5].uuinsptr,&(yypvt[-4].uumode),&(yypvt[-2].uumode),&(yypvt[-0].uumode));
			pcintcheck(yypvt[-5].uuinsptr,&(yypvt[-0].uumode));
			memtoreg = mtoregcheck(&(yypvt[-2].uumode), &(yypvt[-0].uumode));
		} break;
case 27:
# line 474 "parse.y"
 {
			/*
			 * jmp j(n)e j(n)neg j(n)pos j(n)z 
			 * j[lg][eu] j[lg] j[lg]eu
			 * JMP B(N)E[BH] BR[BH]
			 * B[LG][EU][BH] B[LG]EU[BH] B[LG][BH]
			 * BVC[BH] BVS[BH] BCC[BH] BCS[BH]
			 * BEQL[BH]X BNEQ[BH]X
			 *	instructions
			 */

			jmpopgen(yypvt[-1].uuinsptr,&(yypvt[-0].uumode));
			memtoreg = 0;
		} break;
case 28:
# line 488 "parse.y"
 {
			/* jsb instruction */
			jsbopgen(yypvt[-1].uuinsptr,&(yypvt[-0].uumode));
			memtoreg = 0;
		} break;
case 29:
# line 493 "parse.y"
 {
			/* JSB instruction */
#if	EXPFIX
			if( ( yypvt[-0].uumode.newtype != NOTYPE ) && workaround )
				yyerror
				("Expand byte illegal in JSB Instruction.");
#endif
#if	INTEXPFIX
			expandc(yypvt[-1].uuinsptr, &(yypvt[-0].uumode), destmem);
#endif
			generate(8,NOACTION,yypvt[-1].uuinsptr->opcode,NULLSYM);
			addrgen(yypvt[-1].uuinsptr,&(yypvt[-0].uumode),NOTYPE,2);
			memtoreg = 0;
		} break;
case 30:
# line 507 "parse.y"
 {
			/* BSB[BH] instructions */
			generate(8,NOACTION,yypvt[-1].uuinsptr->opcode,NULLSYM);
			switch (yypvt[-1].uuinsptr->tag) {
			case INSTRB:
				generate(8,RELPC8,yypvt[-0].uumode.adexpr.expval,yypvt[-0].uumode.adexpr.symptr);
				break;
			case INSTRH:
				generate(16,RELPC16,yypvt[-0].uumode.adexpr.expval,yypvt[-0].uumode.adexpr.symptr);
				break;
			default:
				yyerror("assembler error (BSBOP1)");
				break;
			} /* switch */
			memtoreg = 0;
		} break;
case 31:
# line 523 "parse.y"
 {
			/*
			 * jb[cs] instructions
			 *
			 * jb(c|s)	&a,op1,op2
			 *
			 *	is encoded as
			 *
			 * bitw		&(1<<a),op1
			 * j(z|nz)	op2
			 *
			 */
			pswcheck(yypvt[-5].uuinsptr, &(yypvt[-2].uumode));
			generate(8,NOACTION,yypvt[-5].uuinsptr->opcode,NULLSYM); /* bitw opcode */
			if (yypvt[-4].uumode.adexpr.symptr == NULLSYM) {
				if (yypvt[-4].uumode.adexpr.expval<0L || yypvt[-4].uumode.adexpr.expval>31L)
					yyerror("Bit position out of range");
				yypvt[-4].uumode.adexpr.expval = 1L << yypvt[-4].uumode.adexpr.expval;
				addrgen(yypvt[-5].uuinsptr,&(yypvt[-4].uumode),NOTYPE,1);
			}
			else {
				/* generate long immediate with action routine */
				generate(8,NOACTION,(long)(CIMMD),NULLSYM);
				generate(32,SHIFTVAL,yypvt[-4].uumode.adexpr.expval,
					yypvt[-4].uumode.adexpr.symptr);
			}
			addrgen(yypvt[-5].uuinsptr,&(yypvt[-2].uumode),NOTYPE,2);

			generate(0,NEWSTMT,(long)line,NULLSYM);
			newins = (*lookup(
					((yypvt[-5].uuinsptr->name[2] == 'c')
						? "jz"	/* for jbc */
						: "jnz"	/* for jbs */),
					N_INSTALL,MNEMON)).itp;
			jmpopgen(newins,&(yypvt[-0].uumode));
			memtoreg = 0;
		} break;
case 32:
# line 560 "parse.y"
 {
			/* acj[l|lu|le|leu] instructions */
			loop1gen(yypvt[-7].uuinsptr,&(yypvt[-6].uumode),&(yypvt[-4].uumode),&(yypvt[-2].uumode),&(yypvt[-0].uumode));
			pcintcheck(yypvt[-7].uuinsptr,&(yypvt[-0].uumode));
			memtoreg = 0;
		} break;
case 33:
# line 566 "parse.y"
 {
			/* atjnz[bhw] instructions */
			loop2gen(yypvt[-5].uuinsptr,&(yypvt[-4].uumode),&(yypvt[-2].uumode),&(yypvt[-0].uumode));
			pcintcheck(yypvt[-5].uuinsptr,&(yypvt[-0].uumode));
			memtoreg = 0;
		} break;
case 34:
# line 572 "parse.y"
 {
			/* movbl[bh] */
			if (memtoreg != 0) {
				newins = (*lookup("NOP",N_INSTALL,MNEMON)).itp;
				generate(8, NOACTION, newins->opcode, NULLSYM);
				memtoreg = 0;
			}
			blockgen(yypvt[-0].uuinsptr);
		} break;
case 35:
# line 581 "parse.y"
 {
			/* SPOP */
			if( yypvt[-0].uumode.newtype != NOTYPE) {
				yyerror("Expand byte illegal in id word for Support Processor Instructions.");
				return;
			}
			if( yypvt[-0].uumode.expspec != NULLSPEC ) {
				yyerror("Operand specifier illegal in id word for Support Processor Instructions.");
				return;
			}
			generate(8,NOACTION,yypvt[-1].uuinsptr->opcode,NULLSYM);
			generate(32,NOACTION,swap_b4(yypvt[-0].uumode.adexpr.expval),NULLSYM);
		} break;
case 36:
# line 594 "parse.y"
 {
			/* SPOPRS, SPOPRD, SPOPRT, SPOPWS, SPOPWD, SPOPWT */
			if( yypvt[-2].uumode.newtype != NOTYPE) {
				yyerror("Expand byte illegal in id word for Support Processor Instructions.");
				return;
			}
			if( yypvt[-2].uumode.expspec != NULLSPEC ) {
				yyerror("Operand specifier illegal in id word for Support Processor Instructions.");
				return;
			}
#if	EXPFIX
			if( ( yypvt[-0].uumode.newtype != NOTYPE ) && workaround ) {
				yyerror("Expand byte illegal in Support Processor Instructions.");
			}
#endif
			generate(8,NOACTION,yypvt[-3].uuinsptr->opcode,NULLSYM);
			generate(32,NOACTION,swap_b4(yypvt[-2].uumode.adexpr.expval),NULLSYM);
			addrgen( yypvt[-3].uuinsptr, &(yypvt[-0].uumode), NOTYPE, 2 );
		} break;
case 37:
# line 613 "parse.y"
 {
			/* SPOPS2, SPOPD2, SPOPT2 */
			if( yypvt[-4].uumode.newtype != NOTYPE) {
				yyerror("Expand byte illegal in id word for Support Processor Instructions.");
				return;
			}
#if	EXPFIX
			if( ( yypvt[-2].uumode.newtype != NOTYPE ||
			    yypvt[-2].uumode.newtype != NOTYPE ) && workaround )
			{
				yyerror("Expand byte illegal in Support Processor Instructions.");
			}
#endif
			if( yypvt[-4].uumode.expspec != NULLSPEC ) {
				yyerror("Operand specifier illegal in id word for Support Processor Instructions.");
				return;
			}
			generate(8,NOACTION,yypvt[-5].uuinsptr->opcode,NULLSYM);
			generate(32,NOACTION,swap_b4(yypvt[-4].uumode.adexpr.expval),NULLSYM);
			addrgen( yypvt[-5].uuinsptr, &(yypvt[-2].uumode), NOTYPE, 2 );
			addrgen( yypvt[-5].uuinsptr, &(yypvt[-0].uumode), NOTYPE, 3 );
		} break;
case 38:
# line 635 "parse.y"
 {
			/* insv instruction */
			pswcheck(yypvt[-7].uuinsptr, &(yypvt[-6].uumode));
			generate(8,NOACTION,yypvt[-7].uuinsptr->opcode,NULLSYM);
			--(yypvt[-2].uumode.adexpr.expval); /* decrement field width */
			addrgen(yypvt[-7].uuinsptr,&(yypvt[-2].uumode),NOTYPE,3);
			addrgen(yypvt[-7].uuinsptr,&(yypvt[-4].uumode),NOTYPE,2);
			addrgen(yypvt[-7].uuinsptr,&(yypvt[-6].uumode),NOTYPE,1);
			addrgen(yypvt[-7].uuinsptr,&(yypvt[-0].uumode),NOTYPE,4);
			pcintcheck(yypvt[-7].uuinsptr,&(yypvt[-0].uumode));
#ifdef	CHIPFIX
			if (yypvt[-0].uumode.admode == REGMD)
				memtoreg = 1;
			else
				memtoreg = 0;
#endif
		} break;
case 39:
# line 652 "parse.y"
 {
			/* extzv instruction */
			pswcheck(yypvt[-7].uuinsptr, &(yypvt[-2].uumode));
			generate(8,NOACTION,yypvt[-7].uuinsptr->opcode,NULLSYM);
			--(yypvt[-4].uumode.adexpr.expval); /* decrement field width */
			addrgen(yypvt[-7].uuinsptr,&(yypvt[-4].uumode),NOTYPE,1);	/* xchange order of */
			addrgen(yypvt[-7].uuinsptr,&(yypvt[-6].uumode),NOTYPE,2);	/* oprnds 1 and 2 */
			addrgen(yypvt[-7].uuinsptr,&(yypvt[-2].uumode),NOTYPE,3);
			addrgen(yypvt[-7].uuinsptr,&(yypvt[-0].uumode),NOTYPE,4);
			pcintcheck(yypvt[-7].uuinsptr,&(yypvt[-0].uumode));
			memtoreg = mtoregcheck(&(yypvt[-2].uumode), &(yypvt[-0].uumode));
		} break;
case 40:
# line 664 "parse.y"
 {
			/* (INS|EXT)F[BHW] instructions */
			/* Interrupt After TSTW Chip Bug
			 * Check if instruction does a store to
			 * a register and is a multiple cycle
			 * ALU instruction (ie. insert field).
			 * If so, then set memtoreg so that a NOP
			 * will be generated after this instruction
			 * if necessary.
			 */
			if ( pswcheck(yypvt[-7].uuinsptr, &(yypvt[-6].uumode)) )
				if ( pswcheck(yypvt[-7].uuinsptr, &(yypvt[-4].uumode)) )
					pswcheck(yypvt[-7].uuinsptr, &(yypvt[-2].uumode));
#ifdef	CHIPFIX
			if ((yypvt[-0].uumode.admode == REGMD) &&
			    (yypvt[-7].uuinsptr->name[0] == 'I'))
				memtoreg = 1;
			else
				memtoreg = mtoregcheck(&(yypvt[-2].uumode), &(yypvt[-0].uumode));
#endif
			generate(8,NOACTION,yypvt[-7].uuinsptr->opcode,NULLSYM);
			addrgen(yypvt[-7].uuinsptr,&(yypvt[-6].uumode),NOTYPE,1);
			addrgen(yypvt[-7].uuinsptr,&(yypvt[-4].uumode),NOTYPE,2);
			addrgen(yypvt[-7].uuinsptr,&(yypvt[-2].uumode),NOTYPE,3);
			addrgen(yypvt[-7].uuinsptr,&(yypvt[-0].uumode),NOTYPE,4);
			pcintcheck(yypvt[-7].uuinsptr,&(yypvt[-0].uumode));
		} break;
case 41:
# line 691 "parse.y"
 {
			/*
			 * movblw rsb
			 * RET GATE RETG [CALL|RET]PS [DIS|ENB]VJMP
			 * R[LGN][EQ|EQU]
			 * REQ[L|LU] R[LSS|GTR](U)
			 * RC[CS] RV[CS] RSB
			 * MOVBLW MVERNO WAIT SLFTST
			 * BPT NOP STR(CPY|END)
			 * CFLUSH
			 *	instructions
			 */
			generate(yypvt[-0].uuinsptr->nbits,NOACTION,yypvt[-0].uuinsptr->opcode,NULLSYM);
			memtoreg = 0;
		} break;
case 42:
# line 706 "parse.y"
 {
			testgen( yypvt[-1].uuinsptr, &(yypvt[-0].uumode));
			memtoreg = 0;
		} break;
case 43:
# line 710 "parse.y"
 {
			/* call instruction */
			callgen(yypvt[-3].uuinsptr,&(yypvt[-2].uumode),&(yypvt[-0].uumode));
			memtoreg = 0;
		} break;
case 44:
# line 715 "parse.y"
 {
			/* CALL instruction */
#if	EXPFIX
			if( ( yypvt[-2].uumode.newtype != NOTYPE ||
			    yypvt[-0].uumode.newtype != NOTYPE ) && workaround )
				yyerror
				("Expand byte illegal in CALL Instruction.");
#endif
			generate(8,NOACTION,yypvt[-3].uuinsptr->opcode,NULLSYM);
			addrgen(yypvt[-3].uuinsptr,&(yypvt[-2].uumode),NOTYPE,1);
			addrgen(yypvt[-3].uuinsptr,&(yypvt[-0].uumode),NOTYPE,2);
			memtoreg = 0;
		} break;
case 45:
# line 728 "parse.y"
 {
			/* save instruction */
			generate(8,NOACTION,yypvt[-1].uuinsptr->opcode,NULLSYM);
			/* let the action routine do the translation */
			generate(8,SAVTRANS,yypvt[-0].uumode.adexpr.expval,yypvt[-0].uumode.adexpr.symptr);
			memtoreg = 0;
		} break;
case 46:
# line 735 "parse.y"
 {
			/* ret instruction */
#if M32RSTFIX
			if (rstflag == YES) {
				if (yypvt[-0].uumode.adexpr.expval < 0 || yypvt[-0].uumode.adexpr.expval > 6)
					werror("Invalid number of registers in `ret'");
				restorefix(&(yypvt[-0].uumode));
			}
			else {
#endif
			generate(8,NOACTION,(yypvt[-1].uuinsptr->opcode & 0xFFL),NULLSYM);
			/* let the action routine do the translation */
			generate(8,SAVTRANS,yypvt[-0].uumode.adexpr.expval,yypvt[-0].uumode.adexpr.symptr);
#if M32RSTFIX
			}
#endif
			/* generate the CRET instruction */
			generate(8,NOACTION,(long)(yypvt[-1].uuinsptr->opcode >> 8),NULLSYM);
			memtoreg = 0;
		} break;
case 47:
# line 755 "parse.y"
 {
			/* SAVE RESTORE instructions */
#if	EXPFIX
			if( yypvt[-0].uumode.newtype != NOTYPE && workaround )
				yyerror
				("Expand byte illegal in SAVE/RESTORE Instructions.");
#endif
#if M32RSTFIX
			if ((yypvt[-1].uuinsptr->name[0] == 'R' ) && (rstflag == YES))
				restorefix(&(yypvt[-0].uumode));
			else {
#endif
			generate(8,NOACTION,yypvt[-1].uuinsptr->opcode,NULLSYM);
			generate(8,NOACTION,(long)((CREGMD<<4)|yypvt[-0].uumode.adreg),NULLSYM);
#if M32RSTFIX
			}
#endif
			memtoreg = 0;
		} break;
case 48:
# line 774 "parse.y"
 {
			/* EXTOP instuction for the m32a simulator i/o routines */
			generate(8,NOACTION,yypvt[-1].uuinsptr->opcode,NULLSYM);
			generate(8,NOACTION,yypvt[-0].uumode.adexpr.expval,NULLSYM);
			memtoreg = 0;
		} break;
case 49:
# line 780 "parse.y"
 {
#if	FLOAT
			/*
			 * fadd[sd]2 fsub[sd]2 fmul[sd]2 fdiv[sd]2
			 * mov[sd]w mov[sd]h mov[sd][sd]
			 * movt[sd]w movt[sd]h
			 *	instructions
			 */
			generate(yypvt[-3].uuinsptr->nbits,NOACTION,yypvt[-3].uuinsptr->opcode,NULLSYM);
			if (yypvt[-2].uumode.admode == FPIMMD) {
				if (yypvt[-3].uuinsptr->tag != INSTRD)	/* change dbl -> single */
					fpdtos(&(yypvt[-2].uumode.adexpr));
			}
			addrgen(yypvt[-3].uuinsptr,&(yypvt[-2].uumode),NOTYPE,1);
			addrgen(yypvt[-3].uuinsptr,&(yypvt[-0].uumode),NOTYPE,2);
			memtoreg = 0;
#endif
		} break;
case 50:
# line 798 "parse.y"
 {
#if	FLOAT
			/* mov[hw][sd] instructions */
			generate(yypvt[-3].uuinsptr->nbits,NOACTION,yypvt[-3].uuinsptr->opcode,NULLSYM);
			addrgen(yypvt[-3].uuinsptr,&(yypvt[-2].uumode),NOTYPE,1);
			addrgen(yypvt[-3].uuinsptr,&(yypvt[-0].uumode),NOTYPE,2);
			memtoreg = 0;
#endif
		} break;
case 51:
# line 807 "parse.y"
 {
#if	FLOAT
			/* fadd[sd]3 fsub[sd]3 fmul[sd]3 fdiv[sd]3 */
			if (yypvt[-5].uuinsptr->tag == INSTRD) {	/* double operands */
				if ((yypvt[-4].uumode.admode==REGMD) && (yypvt[-2].uumode.admode==REGMD))
				/* see if register pairs are used correctly */
					if (abs(yypvt[-2].uumode.adreg - yypvt[-4].uumode.adreg) == 1)
					yyerror("invalid double register usage");
			}
			generate(yypvt[-5].uuinsptr->nbits,NOACTION,yypvt[-5].uuinsptr->opcode,NULLSYM);
			if (yypvt[-4].uumode.admode == FPIMMD) {
				if (yypvt[-5].uuinsptr->tag != INSTRD)	/* change dbl -> single */
					fpdtos(&(yypvt[-4].uumode.adexpr));
			}
			addrgen(yypvt[-5].uuinsptr,&(yypvt[-4].uumode),NOTYPE,1);
			if (yypvt[-2].uumode.admode == FPIMMD) {
				if (yypvt[-5].uuinsptr->tag != INSTRD)	/* change dbl -> single */
					fpdtos(&(yypvt[-2].uumode.adexpr));
			}
			addrgen(yypvt[-5].uuinsptr,&(yypvt[-2].uumode),NOTYPE,2);
			addrgen(yypvt[-5].uuinsptr,&(yypvt[-0].uumode),NOTYPE,3);
			memtoreg = 0;
#endif
		} break;
case 52:
# line 831 "parse.y"
 {
#if	FLOAT
			/* fcmp[sd] instructions */
			if (yypvt[-3].uuinsptr->tag == INSTRD) {	/* fcmpd opcode */
				if ((yypvt[-2].uumode.admode==REGMD) && (yypvt[-0].uumode.admode==REGMD))
				/* see if register pairs are used correctly */
					if (abs(yypvt[-0].uumode.adreg - yypvt[-2].uumode.adreg) == 1)
					yyerror("invalid double register usage");
			}
			generate(yypvt[-3].uuinsptr->nbits,NOACTION,yypvt[-3].uuinsptr->opcode,NULLSYM);
			if (yypvt[-2].uumode.admode == FPIMMD) {
				if (yypvt[-3].uuinsptr->tag != INSTRD)	/* change dbl -> single */
					fpdtos(&(yypvt[-2].uumode.adexpr));
			}
			addrgen(yypvt[-3].uuinsptr,&(yypvt[-2].uumode),NOTYPE,1);
			if (yypvt[-0].uumode.admode == FPIMMD) {
				if (yypvt[-3].uuinsptr->tag != INSTRD)	/* change dbl -> single */
					fpdtos(&(yypvt[-0].uumode.adexpr));
			}
			addrgen(yypvt[-3].uuinsptr,&(yypvt[-0].uumode),NOTYPE,2);
			memtoreg = 0;
#endif
		} break;
case 74:
# line 880 "parse.y"
 { 			/* optional operand specifier */
		/* operand specifier type is found in opcode field of instab */
		yyval.uulong = (yypvt[-0].uuinsptr->opcode);
		} break;
case 75:
# line 884 "parse.y"
 {
		yyval.uulong = NULLSPEC;
		} break;
case 77:
# line 889 "parse.y"
 {
			yyval.uumode.admode = IMMD;
			yyval.uumode.newtype = NOTYPE;
			yyval.uumode.adexpr.exptype = ABS;
			yyval.uumode.adexpr.expval = 0;
			yyval.uumode.adexpr.symptr = NULL;
			yyval.uumode.expspec = NULLSPEC;
		} break;
case 78:
# line 898 "parse.y"
 {
			yyval.uumode.admode = IMMD;
			yyval.uumode.newtype = NOTYPE;
			yyval.uumode.adexpr = yypvt[-1].uuexpr;
			yyval.uumode.expspec = yypvt[-0].uulong;
		} break;
case 79:
# line 904 "parse.y"
 {
			if (yypvt[-0].uumode.newtype != NOTYPE)
				yyerror("Multiple expanded type operand");
			yyval.uumode.newtype = (short)(yypvt[-2].uuinsptr->opcode);
			yyval.uumode.admode = IMMD;
			yyval.uumode.adexpr = yypvt[-0].uumode.adexpr;
			yyval.uumode.expspec = yypvt[-0].uumode.expspec;
		} break;
case 80:
# line 913 "parse.y"
 {	/* immed. mode for fp numbers only */
#if	FLOAT
			yyval.uumode.admode = FPIMMD;
			yyval.uumode.newtype = NOTYPE;
			yyval.uumode.adexpr = yypvt[-0].uuexpr;
#endif
		} break;
case 81:
# line 921 "parse.y"
 {
			yyval.uumode.admode = REGMD;
			yyval.uumode.newtype = NOTYPE;
			/* register number is in opcode field of instab */
			yyval.uumode.adreg = (short)(yypvt[-0].uuinsptr->opcode);
			yyval.uumode.expspec = NULLSPEC;
		} break;
case 82:
# line 928 "parse.y"
 {
			if (yypvt[-0].uumode.newtype != NOTYPE)
				yyerror("Multiple expanded type operand");
			yyval.uumode.newtype = (short)(yypvt[-2].uuinsptr->opcode);
			yyval.uumode.admode = REGMD;
			yyval.uumode.adreg = yypvt[-0].uumode.adreg;
			yyval.uumode.expspec = yypvt[-0].uumode.expspec;
		} break;
case 83:
# line 937 "parse.y"
 {
			yyval.uumode.admode = REGDFMD;
			yyval.uumode.newtype = NOTYPE;
			/* register number is in opcode field of instab */
			yyval.uumode.adreg = (short)(yypvt[-1].uuinsptr->opcode);
			yyval.uumode.expspec = NULLSPEC;
		} break;
case 84:
# line 944 "parse.y"
 {
			yyval.uumode.admode = REGDFMD;
			yyval.uumode.newtype = NOTYPE;
			/* register number is in opcode field of instab */
			yyval.uumode.adreg = (short)(yypvt[-0].uuinsptr->opcode);
			yyval.uumode.expspec = NULLSPEC;
		} break;
case 85:
# line 951 "parse.y"
 {
			if (yypvt[-0].uumode.newtype != NOTYPE)
				yyerror("Multiple expanded type operand");
			yyval.uumode.newtype = (short)(yypvt[-2].uuinsptr->opcode);
			yyval.uumode.admode = REGDFMD;
			yyval.uumode.adreg = yypvt[-0].uumode.adreg;
			yyval.uumode.adexpr = yypvt[-0].uumode.adexpr;
			yyval.uumode.expspec = yypvt[-0].uumode.expspec;
		} break;
case 86:
# line 961 "parse.y"
 {
			yyval.uumode.admode = EXADMD;
			yyval.uumode.newtype = NOTYPE;
			yyval.uumode.adexpr = yypvt[-0].uuexpr;
			yyval.uumode.expspec = NULLSPEC;
		} break;
case 87:
# line 967 "parse.y"
 {
			if (yypvt[-0].uumode.newtype != NOTYPE)
				yyerror("Multiple expanded type operand");
			yyval.uumode.newtype = (short)(yypvt[-2].uuinsptr->opcode);
			yyval.uumode.admode = EXADMD;
			yyval.uumode.adexpr = yypvt[-0].uumode.adexpr;
			yyval.uumode.expspec = yypvt[-0].uumode.expspec;
		} break;
case 88:
# line 976 "parse.y"
 {
			yyval.uumode.admode = ABSMD;
			yyval.uumode.newtype = NOTYPE;
			yyval.uumode.adexpr = yypvt[-0].uuexpr;
			yyval.uumode.expspec = NULLSPEC;
		} break;
case 89:
# line 982 "parse.y"
 {
			if (yypvt[-0].uumode.newtype != NOTYPE)
				yyerror("Multiple expanded type operand");
			yyval.uumode.newtype = (short)(yypvt[-2].uuinsptr->opcode);
			yyval.uumode.admode = ABSMD;
			yyval.uumode.adexpr = yypvt[-0].uumode.adexpr;
			yyval.uumode.expspec = yypvt[-0].uumode.expspec;
		} break;
case 90:
# line 991 "parse.y"
 {
			yyval.uumode.admode = EXADDFMD;
			yyval.uumode.newtype = NOTYPE;
			yyval.uumode.adexpr = yypvt[-0].uuexpr;
			yyval.uumode.expspec = NULLSPEC;
		} break;
case 91:
# line 997 "parse.y"
 {
			if (yypvt[-0].uumode.newtype != NOTYPE)
				yyerror("Multiple expanded type operand");
			yyval.uumode.newtype = (short)(yypvt[-2].uuinsptr->opcode);
			yyval.uumode.admode = EXADDFMD;
			yyval.uumode.adexpr = yypvt[-0].uumode.adexpr;
			yyval.uumode.expspec = yypvt[-0].uumode.expspec;
		} break;
case 92:
# line 1006 "parse.y"
 {
			yyval.uumode.admode = ABSDFMD;
			yyval.uumode.newtype = NOTYPE;
			yyval.uumode.adexpr = yypvt[-0].uuexpr;
			yyval.uumode.expspec = NULLSPEC;
		} break;
case 93:
# line 1012 "parse.y"
 {
			if (yypvt[-0].uumode.newtype != NOTYPE)
				yyerror("Multiple expanded type operand");
			yyval.uumode.newtype = (short)(yypvt[-2].uuinsptr->opcode);
			yyval.uumode.admode = ABSDFMD;
			yyval.uumode.adexpr = yypvt[-0].uumode.adexpr;
			yyval.uumode.expspec = yypvt[-0].uumode.expspec;
		} break;
case 94:
# line 1021 "parse.y"
 {
			yyval.uumode.admode = DSPMD;
			yyval.uumode.newtype = NOTYPE;
			/* register number is in opcode field of instab */
			yyval.uumode.adreg = (short)(yypvt[-2].uuinsptr->opcode);
			yyval.uumode.adexpr = yypvt[-4].uuexpr;
			yyval.uumode.expspec = yypvt[-0].uulong;
		} break;
case 95:
# line 1029 "parse.y"
 {
			if (yypvt[-0].uumode.newtype != NOTYPE)
				yyerror("Multiple expanded type operand");
			yyval.uumode.newtype = (short)(yypvt[-2].uuinsptr->opcode);
			yyval.uumode.admode = DSPMD;
			yyval.uumode.adreg = yypvt[-0].uumode.adreg;
			yyval.uumode.adexpr = yypvt[-0].uumode.adexpr;
			yyval.uumode.expspec = yypvt[-0].uumode.expspec;
		} break;
case 96:
# line 1039 "parse.y"
 {
			yyval.uumode.admode = DSPDFMD;
			yyval.uumode.newtype = NOTYPE;
			/* register number is in opcode field of instab */
			yyval.uumode.adreg = (short)(yypvt[-2].uuinsptr->opcode);
			yyval.uumode.adexpr = yypvt[-4].uuexpr;
			yyval.uumode.expspec = yypvt[-0].uulong;
		} break;
case 97:
# line 1047 "parse.y"
 {
			yyval.uumode.admode = DSPDFMD;
			yyval.uumode.newtype = NOTYPE;
			/* register number is in opcode field of instab */
			yyval.uumode.adreg = (short)(yypvt[-2].uuinsptr->opcode);
			yyval.uumode.adexpr.exptype = ABS;
			yyval.uumode.adexpr.expval = 0;
			yyval.uumode.adexpr.symptr = NULL;
			yyval.uumode.expspec = yypvt[-0].uulong;
		} break;
case 98:
# line 1057 "parse.y"
 {
			if (yypvt[-0].uumode.newtype != NOTYPE)
				yyerror("Multiple expanded type operand");
			yyval.uumode.newtype = (short)(yypvt[-2].uuinsptr->opcode);
			yyval.uumode.admode = DSPDFMD;
			yyval.uumode.adreg = yypvt[-0].uumode.adreg;
			yyval.uumode.adexpr = yypvt[-0].uumode.adexpr;
			yyval.uumode.expspec = yypvt[-0].uumode.expspec;
		} break;
case 100:
# line 1068 "parse.y"
 {
			if (yypvt[-2].uuexpr.symptr == NULL) {
				yyval.uuexpr.symptr = yypvt[-0].uuexpr.symptr;
				yyval.uuexpr.exptype = yypvt[-0].uuexpr.exptype;
			}
			else if (yypvt[-0].uuexpr.symptr == NULL) {
				yyval.uuexpr.symptr = yypvt[-2].uuexpr.symptr;
				yyval.uuexpr.exptype = yypvt[-2].uuexpr.exptype;
			}
			else {
				yyerror("Illegal addition");
			}

			yyval.uuexpr.expval = yypvt[-2].uuexpr.expval + yypvt[-0].uuexpr.expval;
		} break;
case 101:
# line 1083 "parse.y"
 {
			if (yypvt[-2].uuexpr.symptr == NULL) {
				if (yypvt[-0].uuexpr.symptr != NULL) {
					yyerror("Illegal subtraction");
				}
				yyval.uuexpr.symptr = NULL;
				yyval.uuexpr.exptype = ABS;
				yyval.uuexpr.expval = yypvt[-2].uuexpr.expval - yypvt[-0].uuexpr.expval;
			}
			else if (yypvt[-0].uuexpr.symptr == NULL) {
				yyval.uuexpr.symptr = yypvt[-2].uuexpr.symptr;
				yyval.uuexpr.exptype = yypvt[-2].uuexpr.exptype;
				yyval.uuexpr.expval = yypvt[-2].uuexpr.expval - yypvt[-0].uuexpr.expval;
			}
			else if (yypvt[-2].uuexpr.exptype == yypvt[-0].uuexpr.exptype) {
				yyval.uuexpr.symptr = NULL;
				yyval.uuexpr.exptype = ABS;
				yyval.uuexpr.expval
				= (yypvt[-2].uuexpr.symptr->value - yypvt[-0].uuexpr.symptr->value)
				+ (yypvt[-2].uuexpr.expval - yypvt[-0].uuexpr.expval);
			}
			else {
				yyerror("Illegal subtraction");
				yyval.uuexpr.symptr = NULL;
				yyval.uuexpr.exptype = ABS;
				yyval.uuexpr.expval = 0;
			}
		} break;
case 102:
# line 1111 "parse.y"
 {
			if (yypvt[-2].uuexpr.symptr != NULL || yypvt[-0].uuexpr.symptr != NULL) {
				yyerror("Illegal multiplication");
			}
			yyval.uuexpr.symptr = NULL;
			yyval.uuexpr.exptype = ABS;
			yyval.uuexpr.expval = yypvt[-2].uuexpr.expval * yypvt[-0].uuexpr.expval;
		} break;
case 103:
# line 1119 "parse.y"
 {
			if (yypvt[-2].uuexpr.symptr != NULL || yypvt[-0].uuexpr.symptr != NULL) {
				yyerror("Illegal division");
			}
			yyval.uuexpr.symptr = NULL;
			yyval.uuexpr.exptype = ABS;
			yyval.uuexpr.expval = yypvt[-2].uuexpr.expval / yypvt[-0].uuexpr.expval;
		} break;
case 104:
# line 1128 "parse.y"
 {
			if ((yyval.uuexpr.exptype = yypvt[-0].uusymptr->styp & TYPE) == ABS) {
				yyval.uuexpr.expval = yypvt[-0].uusymptr->value;
				yyval.uuexpr.symptr = NULL;
			}
			else {
				yyval.uuexpr.symptr = yypvt[-0].uusymptr;
				yyval.uuexpr.expval = 0;
			}
		} break;
case 105:
# line 1138 "parse.y"
 {
			yyval.uuexpr.exptype = ABS;
			yyval.uuexpr.expval = yypvt[-0].uulong;
			yyval.uuexpr.symptr = NULL;
		} break;
case 106:
# line 1143 "parse.y"
 {
			yyval.uuexpr = yypvt[-1].uuexpr;
		} break;
case 107:
# line 1146 "parse.y"
 {
		yyval.uuexpr.exptype = yypvt[-0].uuexpr.exptype;
		yyval.uuexpr.symptr = yypvt[-0].uuexpr.symptr;
		yyval.uuexpr.expval = - yypvt[-0].uuexpr.expval;
		} break;
case 108:
# line 1152 "parse.y"
 {
			cgsect((short)(yypvt[-0].uuinsptr->tag));
		} break;
case 109:
# line 1155 "parse.y"
 {
			comment(yypvt[-0].uustrptr);
		} break;
case 110:
# line 1158 "parse.y"
 {
			generate(0,SETEXPAND,NULLVAL,NULLSYM);
		} break;
case 111:
# line 1161 "parse.y"
 {
			yypvt[-0].uusymptr->styp |= EXTERN;
		} break;
case 112:
# line 1164 "parse.y"
 {
			if (yypvt[-2].uusymptr == NULL) /* in case of error */
				aerror("Unable to define identifier");
			else {
				/* reset all but EXTERN bit */
				if (yypvt[-2].uusymptr==dot)
					if ((yypvt[-2].uusymptr->styp != yypvt[-0].uuexpr.exptype))
						yyerror("Incompatible types");
					else {
						long incr;

						if ((incr = yypvt[-0].uuexpr.symptr->value + yypvt[-0].uuexpr.expval - yypvt[-2].uusymptr->value) < 0)
							yyerror("Cannot decrement '.'");
						else
							fill(incr);
					} /* $2->styp == $4.expval */
				else { /* $2!=dot */
					yypvt[-2].uusymptr->styp &= EXTERN;
					if ((yypvt[-2].uusymptr->styp |= yypvt[-0].uuexpr.exptype & TYPE) == UNDEF)
						yyerror("Illegal `.set' expression");
					yypvt[-2].uusymptr->value = yypvt[-0].uuexpr.expval;
					if (yypvt[-0].uuexpr.symptr != NULL) {
						yypvt[-2].uusymptr->value += yypvt[-0].uuexpr.symptr->value;
						yypvt[-2].uusymptr->maxval = yypvt[-0].uuexpr.symptr->maxval;
					}
				} /* else */
			} /* else */
		} break;
case 113:
# line 1192 "parse.y"
 {
			long val;

			if (yypvt[-0].uuexpr.exptype != ABS)
				yyerror("`.zero' size not absolute");
			if ((val = yypvt[-0].uuexpr.expval) < 0)
				yyerror("Invalid `.zero' size");
			if (dot->styp != DAT)
				yyerror("`.zero' valid only in .data");
			fill(val);
		} break;
case 114:
# line 1203 "parse.y"
 {
			if (yypvt[-0].uuexpr.exptype != ABS)
				aerror("`.bss' alignment not absolute");
			bss(yypvt[-4].uusymptr,yypvt[-2].uuexpr.expval,yypvt[-0].uuexpr.expval,yypvt[-2].uuexpr.exptype);
		} break;
case 115:
# line 1208 "parse.y"
 {
			if (yypvt[-0].uuexpr.exptype != ABS)
				yyerror("'.align' field not absolute");
			ckalign(yypvt[-0].uuexpr.expval);
		} break;
case 116:
# line 1213 "parse.y"
 {
			if ((yypvt[-2].uusymptr->styp & TYPE) != UNDEF)
				yyerror("Illegal attempt to redefine symbol");
			else {
				yypvt[-2].uusymptr->styp = (EXTERN | UNDEF);
				yypvt[-2].uusymptr->value = yypvt[-0].uuexpr.expval;
			};
		} break;
case 117:
# line 1221 "parse.y"
 {
			fillbyte();
		} break;
case 118:
# line 1224 "parse.y"
 {
			fillbyte();
			ckalign(2L);
		} break;
case 119:
# line 1228 "parse.y"
 {
			fillbyte();
			ckalign(4L);
		} break;
case 120:
# line 1232 "parse.y"
 {
			ckalign(4L);
		} break;
case 121:
# line 1235 "parse.y"
 {
			if (cfile[0] != '\0')
				yyerror("Only 1 `.file' allowed");
			if (strlen(yypvt[-0].uustrptr) > 14)
				yyerror("`.file' name too long (14 character limit)");
			else
				strcpy(cfile,yypvt[-0].uustrptr);
			generate(0,SETFILE,NULLVAL,NULLSYM);
		} break;
case 122:
# line 1244 "parse.y"
 {
			generate(0,DEFINE,NULLVAL,yypvt[-0].uusymptr);
		} break;
case 123:
# line 1247 "parse.y"
 {
			generate(0,ENDEF,NULLVAL,NULLSYM);
		} break;
case 124:
# line 1250 "parse.y"
 {
			generate(0,(unsigned short)yypvt[-1].uulong,yypvt[-0].uuexpr.expval,yypvt[-0].uuexpr.symptr);
		} break;
case 125:
# line 1253 "parse.y"
 {
			generate(0,SETTAG,NULLVAL,yypvt[-0].uusymptr);
		} break;
case 127:
# line 1257 "parse.y"
 {
			cline = (short)yypvt[-0].uuexpr.expval;
			generate(0,LLINENO,yypvt[-0].uuexpr.expval,yypvt[-0].uuexpr.symptr);
		} break;
case 128:
# line 1261 "parse.y"
 {
			cline = (short)yypvt[-2].uuexpr.expval;
			generate(0,LLINENUM,yypvt[-2].uuexpr.expval,yypvt[-2].uuexpr.symptr);
			generate(0,LLINEVAL,yypvt[-0].uuexpr.expval,yypvt[-0].uuexpr.symptr);
		} break;
case 129:
# line 1267 "parse.y"
 {
			yyval.uulong = SETVAL;
		} break;
case 130:
# line 1270 "parse.y"
 {
			yyval.uulong = SETSCL;
		} break;
case 131:
# line 1273 "parse.y"
 {
			yyval.uulong = SETTYP;
		} break;
case 132:
# line 1276 "parse.y"
 {
			yyval.uulong = SETLNO;
		} break;
case 133:
# line 1279 "parse.y"
 {
			yyval.uulong = SETSIZ;
		} break;
case 134:
# line 1283 "parse.y"
 {
			generate(0,SETDIM1,yypvt[-0].uuexpr.expval,yypvt[-0].uuexpr.symptr);
		} break;
case 135:
# line 1286 "parse.y"
 {
			generate(0,SETDIM2,yypvt[-0].uuexpr.expval,yypvt[-0].uuexpr.symptr);
		} break;
case 136:
# line 1290 "parse.y"
 {
			spctype = NBPW / 4;
		} break;
case 137:
# line 1294 "parse.y"
 {
			spctype = NBPW / 2;
			ckalign(2L);
		} break;
case 138:
# line 1299 "parse.y"
 {
			spctype = NBPW;
			ckalign(4L);
		} break;
case 139:
# line 1304 "parse.y"
 {
			spctype = NBPW;
			ckalign(4L);
		} break;
case 140:
# line 1308 "parse.y"
 {
			spctype = 2 * NBPW;
			ckalign(4L);
		} break;
case 141:
# line 1313 "parse.y"
 {
			dot->value = newdot; /* syncronize */
			generate(0,NEWSTMT,(long)line,NULLSYM);
		} break;
case 142:
# line 1317 "parse.y"
 {
			dot->value = newdot; /* syncronize */
			generate(0,NEWSTMT,(long)line,NULLSYM);
		} break;
case 143:
# line 1322 "parse.y"
 {
			width = spctype;
			if (bitpos + width > spctype)
				yyerror("Expression crosses field boundary");
			/*
			 * Figure out which action routine to use
			 * in case there is an unresolved symbol.
			 * If a full word is being used, then
			 * a relocatable may be specified.
			 * Otherwise it is restricted to being an
			 * absolute (forward reference).
			 */
			action = (width == NBPW) ? RELDAT32 : RESABS;
			generate(width,(yypvt[-0].uuexpr.symptr != NULLSYM)*action,
				yypvt[-0].uuexpr.expval,yypvt[-0].uuexpr.symptr);
		} break;
case 144:
# line 1338 "parse.y"
 {
			width = yypvt[-2].uulong;
			if (bitpos + width > spctype)
				yyerror("Expression crosses field boundary");
			action = (width == NBPW) ? RELDAT32 : RESABS;
			generate(width,(yypvt[-0].uuexpr.symptr != NULLSYM)*action,
				yypvt[-0].uuexpr.expval,yypvt[-0].uuexpr.symptr);
		} break;
case 145:
# line 1347 "parse.y"
 {
			dot->value = newdot; /* syncronize */
			generate(0,NEWSTMT,(long)line,NULLSYM);
		} break;
case 146:
# line 1351 "parse.y"
 {
			dot->value = newdot; /* syncronize */
			generate(0,NEWSTMT,(long)line,NULLSYM);
		} break;
case 147:
# line 1356 "parse.y"
 {
#if	FLOAT
			width = spctype;
			if (bitpos + width > spctype)
				yyerror("Expression crosses field boundary");
			if (spctype != NBPW) {	/* double fp number */
				generate(32,NOACTION,yypvt[-0].uuexpr.expval,yypvt[-0].uuexpr.symptr);
				generate(32,NOACTION,yypvt[-0].uuexpr.fdexpval2,yypvt[-0].uuexpr.symptr);
			} else {	/* single fp number */
				fpdtos(&(yypvt[-0].uuexpr));	/* convert dbl to single */
				generate(32,NOACTION,yypvt[-0].uuexpr.expval,yypvt[-0].uuexpr.symptr);
			}
#endif
		} break;
case 148:
# line 1371 "parse.y"
 {
#if	FLOAT
			yyval.uuexpr.exptype = ABS;
			yyval.uuexpr.symptr = NULL;
			yyval.uuexpr.expval = yypvt[-0].uuexpr.expval;
			yyval.uuexpr.fdexpval2 = yypvt[-0].uuexpr.fdexpval2;
#endif
		} break;
case 149:
# line 1379 "parse.y"
 {
#if	FLOAT
			yyval.uuexpr.exptype = ABS;
			yyval.uuexpr.symptr = NULL;
			yyval.uuexpr.expval = yypvt[-0].uuexpr.expval | (1L<<31);   /* set sign bit */
			yyval.uuexpr.fdexpval2 = yypvt[-0].uuexpr.fdexpval2;
#endif
		} break;
	}
	goto yystack;  /* stack new state and value */
}

/*
 * For trancendental debugger pi; compatible with sdb(1).
 *
 * This file gives definitions supplementing <a.out.h> for permanent
 * symbol table entries.  These must have one of the N_STAB bits on,
 * and are subject to relocation according to the masks in <a.out.h>.
 *
 * IF YOU ADD DEFINITIONS, ADD THEM TO nm.c as well
 *
 * stabs for text symbols (must be 0x?4):
 */
#define	N_BFUN	0x24		/* procedure: name,,0,lineno,address */
#define	N_FUN	0x24
#define	N_NARGS	0x34		/* function call: ,,0,nbytes,address */
#define	N_SLINE	0x44		/* src line: ,,0,lineno,address */
#define	N_SO	0x64		/* source file: name,,0,lineno,address */
#define	N_SOL	0x84		/* #include file: name,,0,lineno,address */
#define	N_ESO	0x94		/* end source file: name,,0,lineno,address */
#define	N_ENTRY	0xa4		/* alternate entry: name,,0,lineno,address */
#define	N_RFUN	0xb4		/* return from function:  ,,0,lineno,address */
#define	N_LBRAC	0xc4		/* left  bracket: ,,0,level,address */
#define	N_RBRAC	0xd4		/* right bracket: ,,0,level,address */
#define	N_EFUN	0xf4		/* end of function: name,,0,lineno,address */
/*
 * stabs for data symbols (must be 0x?6):
 */
#define	N_STSYM	0x26		/* static symbol: name,,0,type,address */
/*
 * stabs for bss symbols (must be 0x?8):
 */
#define	N_LCSYM	0x28		/* .lcomm symbol: name,,0,type,address */
#define	N_ECOML	0xe8		/* end common (local name): ,,address */
/*
 * stabs for non-relocated stuff:
 */
#define	N_GSYM	0x20		/* global symbol: name,,0,type,0 */
#define	N_FNAME	0x22		/* procedure name (f77 kludge): name,,0 */
#define N_STFUN	0x32		/* static function: name,,0,type,0 */
#define	N_RSYM	0x40		/* register sym: name,,0,type,register */
#define	N_BSTR	0x5c		/* begin structure: name,,0,type,length */
#define	N_ESTR	0x5e		/*  end  structure: name,,0,type,length */
#define	N_SSYM	0x60		/* structure elt: name,,0,type,offset */
#define	N_SFLD	0x70		/* structure field: name,,0,type,offset */
#define	N_LSYM	0x80		/* local sym: name,,0,type,offset */
#define	N_PSYM	0xa0		/* parameter: name,,0,type,offset */
/*		0xc0		/* former N_LBRAC */
/*		0xe0		/* former N_RBRAC */
#define	N_BCOMM	0xe2		/* begin common: name,, */
#define	N_ECOMM	0xe4		/* end common: name,, */
#define	N_VER	0xf0		/* symbol table version number */
#define	N_TYID	0xfa		/* struct, union, or enum name */
#define	N_DIM	0xfc		/* dimension for arrays */
#define	N_LENG	0xfe		/* second stab entry with length information */

/*
 * for the berkeley pascal compiler, pc(1):
 */
#define	N_PC	0x30		/* global pascal symbol: name,,0,subtype,line */

#ifdef	DESCSYMBS

/* this stuff is taken from mfile1.h */

/*	type names, used in symbol table building */
# define TNULL 0
# define FARG 1
# define CHAR 2
# define SHORT 3
# define INT 4
# define LONG 5
# define FLOAT 6
# define DOUBLE 7
# define STRTY 8
# define UNIONTY 9
# define ENUMTY 10
# define MOETY 11
# define UCHAR 12
# define USHORT 13
# define UNSIGNED 14
# define ULONG 15
# define VOID 16
# define UNDEF 17

/*	type modifiers */

# define PTR  040
# define FTN  0100
# define ARY  0140

/*	type packing constants */

# define MTMASK 03
# define BTMASK 037
# define BTSHIFT 5 
# define TSHIFT 2
# define TMASK (MTMASK<<BTSHIFT)
# define TMASK1 (MTMASK<<(BTSHIFT+TSHIFT))

# define BTYPE(x)  ((x)&BTMASK)   /* basic type of x */

#endif	DESCSYMBS

#ifdef	DESCNAMES

char *basenames[] = {
	"<null>",	/*  0 */
	"<arg>",	/*  1 function argument		*/
	"char",		/*  2 */
	"short",	/*  3 */
	"int",		/*  4 */
	"long",		/*  5 */
	"float",	/*  6 */
	"double",	/*  7 */
	"struct",	/*  8 */
	"union",	/*  9 */
	"enum",		/* 10 */
	"memen",	/* 11 member of enumeration	*/
	"u_char",	/* 12 unsigned character	*/
	"u_short",	/* 13 */
	"u_int",	/* 14 */
	"u_long",	/* 15 */
	"void",		/* 16 */
	"<undef>",	/* 17 */
	"<???>"
};

char qualchars[] = {
	'\0',	/* 0 none	*/
	'*',	/* 1 pointer	*/
	'(',	/* 2 function	*/
	'[',	/* 3 array	*/
};

#endif	DESCNAMES

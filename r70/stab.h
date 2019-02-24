/*
 * For transcendental debugger pi(1).
 *
 * This file gives definitions supplementing <a.out.h> for permanent
 * symbol table entries.  These must have one of the N_STAB bits on,
 * and are subject to relocation according to the masks in <a.out.h>.
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

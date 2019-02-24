#ifndef UNIV_H
#include "univ.h"
#endif
struct	exec {	/* a.out header */
	long	a_magic;	/* magic number */
	long	a_text; 	/* size of text segment */
	long	a_data; 	/* size of initialized data */
	long	a_bss;  	/* size of unitialized data */
	long	a_syms; 	/* size of symbol table */
	long	a_entry; 	/* entry point */
	long	a_textorg;	/* -b textorg */
	long	a_flag; 	/* relocation info stripped */
};

class	mlist {			/* symbol table entry */
public:
	char	situ[4];
	char	*name;
	char	type;    	/* type flag */
	char	other;
	unsigned short desc;	/* C type code */
	long	value;		/* value */
	void	mkid(){
			if( !situ[0] )
				name = "???";
			else if( !situ[3] )
				name = situ;
			else
				name = sf("%0.8s",situ);
		}
};

#define	N_UNDF	0
#define	N_ABS	02
#define	N_TEXT	04
#define	N_DATA	06
#define	N_BSS	010
#define N_CTEXT	012
#define N_RDATA	014
#define N_COMM	016
#define	N_EXT	01	/* external bit, or'ed in */
#define N_TYPE	036
#define N_FN	036
#define N_MORE	040
#define N_REG	0100	/* place holder of some sort */
#define	X2WDS	020	/* long (2-word) quantity */



/* values for n_dtype */
#define	S_GSYM	0x01		/* global sym: name,type,0 */
#define	S_STSYM	0x02		/* static symbol: name,type,address */
#define	S_LCSYM	0x03		/* .lcomm symbol: name,type,address */
#define	S_RSYM	0x04		/* register sym: name,register,offset */
#define	S_LSYM	0x05		/* local sym: name,type,offset */
#define	S_PSYM	0x06		/* parameter: name,type,offset */
#define	S_BSTR	0x07		/* begin structure: name,, */
#define	S_ESTR	0x08		/* end structure: name,, */
#define	S_SSYM	0x09		/* structure elt: name,type,struct_offset */
#define	S_BENUM	0x0a		/* begin enum: name,, */
#define	S_EENUM	0x0b		/* end enum: name,, */
#define	S_ENUM	0x0c		/* enum element: name,,value */
#define	S_BCOMM	0x0d		/* begin common: name,, */
#define	S_ECOMM	0x0e		/* end common: name,, */
#define	S_ECOML	0x0f		/* end common (local name): ,,address */
#define	S_LBRAC	0x10		/* left bracket: ,nesting level,address */
#define	S_RBRAC	0x11		/* right bracket: ,nesting level,address */
#define	S_BFUN	0x12		/* procedure: name,linenumber,address */
#define	S_ENTRY	0x13		/* alternate entry: name,linenumber,address */
#define	S_SLINE	0x14		/* src line: ,linenumber,address */
#define	S_SO	0x15		/* source file name: name,,address */
#define	S_SOL	0x16		/* #line source filename: name,,address */
#define	S_FNAME	0x17		/* procedure name (f77 kludge): name,,0 */
#define	S_LENG	0x18		/* additional entry with length: ,,length */
#define	S_EFUN	0x19		/* end of function: name,, */
#define	S_DLINE	0x1A		/* data src line: ,linenumber,address */
#define S_NARGS 0x20		/* number of args in the function call */
#define S_TYID	0x21		/* follows STRTY, etc: name,, */
#define S_DIM   0x22		/* ary dimension */

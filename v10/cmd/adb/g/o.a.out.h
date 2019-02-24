struct	exec {	/* a.out header */
	long	a_magic;	/* magic number */
	long	a_text; 	/* size of text segment */
	long	a_data; 	/* size of initialized data */
	long	a_bss;  	/* size of unitialized data */
	long	a_syms; 	/* size of symbol table */
	long	a_entry; 	/* entry point */
	long	a_unused;	/* not used */
	long	a_flag; 	/* relocation info stripped */
};

#define	A_MAGIC1	0407       	/* normal */
#define	A_MAGIC2	0410       	/* read-only text */
#define	A_MAGIC3	0411       	/* separated I&D */
#define	A_MAGIC4	0406       	/* mpx */

struct	nlist {	/* symbol table entry */
	char	n_name[16];	/* symbol name */
	char	n_type;    	/* type flag */
	char	n_dtype;
	unsigned short n_desc;	/* C type code */
	long	n_value;	/* value */
};

/* values for n_type */
#define	N_UNDF	0	/* undefined */
#define	N_ABS	02	/* absolute */
#define	N_TEXT	04	/* text symbol */
#define	N_DATA	06	/* data symbol */
#define	N_BSS	010	/* bss symbol */
#define N_TYPE	036
#define	N_EXT	01	/* external bit, or'ed in */

#define	FORMAT	"%04x"	/* to print a value */

/*
 * Quantities for relocation words
 */
/* modifiers applying to relocation bits */
#define	X2WDS	020	/* long (2-word) quantity */
#define	XPCREL	040	/* PC-relative */
#define	XOFFS	0100	/* relative to base of data */

/* Instruction types, for loader-optimizer */
/* Apply only if type is N_ABS */
#define	TMASK	03600	/* Mask for type */
#define	TBR0	00200	/* Short (8-bit) branch */
#define	TBR1	00400	/* medium (16-bit) branch */
#define	TEA0	00600	/* Long absolute addr follows immediately */
#define	TEA1	01000	/* Long absolute addr after 1 word */
#define	TEA2	01200	/* Long absolute addr after 2 words */
#define	TIM0	01400	/* Long, shrinkable immediate follows */
#define	TPC0	01600	/* Long absolute convertible to pc-rel */
#define	TPC1	02000	/* Long absolute convertible to pc-rel after 1 wd */
#define	TPC2	02200	/* Long absolute convertible to pc-rel after 2 wds */
#define	TMM	02400	/* movem instruction */

/* Possible second E.A. for mov instruction */
#define	TMASK1	074000
#define	TSHFT1	4

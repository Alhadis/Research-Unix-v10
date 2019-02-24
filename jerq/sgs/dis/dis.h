/*
 *	static	char	*dish_ID[] = "@(#) dis.h: 1.3 12/14/83";
 */

/*
 *	This is the header file for the m32A disassembler.
 *	The information contained in the first part of the file
 *	is common to each version, while the last part is dependent
 *	on the particular machine architecture being used.
 */

#define		NCPS	10	/* number of chars per symbol	*/
#define		NHEX	80	/* max # chars in object per line	*/
#define		NLINE	80	/* max # chars in mnemonic per line	*/
#define		FAIL	0
#define		TRUE	1
#define		FALSE	0
#define		LEAD	1
#define		NOLEAD	0
#define		TERM	0	/* used in tables.c to indicate		*/
				/* that the 'indirect' field of the	*/
				/* 'instable' terminates - no pointer.	*/
				/* Is also checked in 'dis_text()' in	*/
				/* bits.c.				*/

#ifdef AR32W
#define	LNNOBLKSZ	1024	/* size of blocks of line numbers		*/
#define	SYMBLKSZ	1024	/* size if blocks of symbol table entries	*/
#else
#define	LNNOBLKSZ	512	/* size of blocks of line numbers		*/
#define	SYMBLKSZ	512	/* size of blocks of symbol table entries	*/
#endif
#define		STRNGEQ 0	/* used in string compare operation	*/
				/* in 'disassembly' routine		*/

/*
 *	The following are constants that are used in the disassembly 
 *	of floating point immediate operands.
 */
#define		NOTFLOAT	0
#define		FPSINGLE	1
#define		FPDOUBLE	2
#define		TWO_23	8388608		/* 2 ** 23 used in conversion	
					 * of floating point object to	
					 * a decimal number in _utls.c 
					 */
#define		TWO_32	4294967296.	/* 2 ** 32 also used in floating
					 * point number conversion in
					 * _utls.c
					 */
#define		TWO_52	4503599627370496.	/* 2 ** 52 also used in
						 * floating point conversion
						 * routines.
						 */

#define		BIAS	127	/* bias on 8 bit exponent of floating	
				 * point number in _utls.c		
				 */
#define		DBIAS	1023	/* bias on 11 bit exponent of double	
				 * precision floating point number	
				 * in _utls.c				
				 */

/*
 * The following are the 7 posssible types of floating point immediate  
 * operands. These are the possible values of [s|d]fpconv() which 
 * are in _utls.c.								
 */

#define		NOTANUM		0
#define		NEGINF		1
#define		INFINITY	2
#define		ZERO		3
#define		NEGZERO		4
#define		FPNUM		5
#define		FPBIGNUM	6

/*
 *	This is the structure that will be used for storing all the
 *	op code information.  The structure values themselves are
 *	in 'tables.c'.
 */

struct	instable {
	char		name[NCPS];
	unsigned	class;
};
/*
 *	This is the structure that will be used for storing all the
 *	address modification information.  The structure values
 *	themselves are in 'tables.c'.
 */
struct	formtable {
	char		name[NCPS];
	unsigned	typ;
};
/*	NOTE:	the following information in this file must be changed
 *		between the different versions of the disassembler.
 *
 *	These are the instruction classes as they appear in
 *	'tables.c'.  Here they are given numerical values
 *	for use in the actual disassembly of an object file.
 */

#define UNKNOWN		0
#define OPRNDS0		1	/* instructions with no operands	*/
#define OPRNDS1		2	/* instructions with 1 operand		*/
#define OPRNDS2		3	/* instructions with 2 operands		*/
#define OPRNDS3		4	/* instructions with 3 operands		*/
#define OPRNDS4		5	/* instructions with 4 operands		*/
#define JUMP		6	/* "jump" instruction			*/
#define JUMPSB		7	/* "jumpsb" instruction			*/
#define JUMP1		8	/* 1 byte offset jump instruction	*/
#define JUMP2		9	/* 2 byte offset jump instruction	*/
#define	BSBB		10	/* "BSBB" instruction			*/
#define	BSBH		11	/* "BSBB" instruction			*/
#define NOOP8		12	/* 8 bit offset (no operands)		*/
#define NOOP16		13	/* 16 bit offset (no operands)		*/
#define CALL		14	/* "call" instruction			*/
#define EXT		15	/* "extendop" instruction		*/
#define AD1OP3		17	/* 4 operands but add 1 to 3nd operand!	*/
#define SFPOPS2		18	/* single floating point,  2 operands	*/
#define	SFPOPS3		19	/* single floating point,  3 operands	*/
#define	MACRO		20	/* macro-rom instruction		*/
#define DFPOPS2		21	/* double floating point, 2 operands	*/
#define	DFPOPS3		22	/* double floating point, 3 operands	*/
#define SPRTOP0		23	/* support processor inst. 0 operands   */
#define SPRTOP1		24	/* support processor inst. 1 operand    */
#define SPRTOP2		25	/* support processor inst. 2 operands   */
 
/*
 *	These are the address modification types as they appear in
 *	'tables.c'.  Here they are given numerical values for
 *	use in the actual disassembly of an object file.
 */

 
#define IM6	1	/* immediate 6 bit field			*/
#define R	2	/* register					*/
#define RDF	3	/* register deferred (indirect)		*/
#define R4	4	/* register and 4 bit displacement		*/
#define IMB	5	/* immediate byte				*/
#define IMH	6	/* immediate half word				*/
#define IMW	7	/* immediate word				*/
#define BDB	8	/* base (register) and byte displacement	*/
#define BDH	9	/* base (register) and half word displacement	*/
#define BDW	10	/* base (register) and word displacement	*/
#define BDBDF	11	/* base and byte displacement deferred		*/
#define BDHDF	12	/* base and half word displacement deferred	*/
#define BDWDF	13	/* base and word displacement deferred		*/
#define EXPSB	14	/* expanded signed byte				*/
#define EXPUB	15	/* expanded unsigned byte			*/
#define EXPSH	16	/* expanded signed half word			*/
#define EXPUH	17	/* expanded unsigned half word			*/
#define EXPSW	18	/* expanded signed word				*/
#define EXPUW	19	/* expanded unsigned word			*/
#define IM4	20	/* immediate 4 bit field			*/
#define ABAD	21	/* absolute address				*/
#define ABADDF	22	/* absolute address deferred			*/
#define	IMD	23	/* immediate double (floating point)		*/

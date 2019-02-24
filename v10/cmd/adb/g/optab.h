#define	MAXRAND	4

struct optab {
	unsigned short opcode;
	unsigned short mask;
	unsigned short op2;
	unsigned short mk2;
	char	*opname;
	char	flags;
	char	nrand;
	short	rand[MAXRAND];
};

/*
 * operand types
 */

#define	DSREG	00	/* special registers */
#define	DEA	01	/* E.A. to low order 6 bits */
#define	DRG	02	/* register to low order 3 bits */
#define	DRGL	03	/* register to bits 11-9 */
#define	DBR	04	/* branch offset (short) */
#define	DMQ	05	/* move-quick 8-bit value */
#define	DAQ	06	/* add-quick 3-bit value in 11-9 */
#define	DIM	07	/* Immediate value, according to size */
#define	DEAM	010	/* E.A. to bits 11-6 as in move */
#define	DBCC	011	/* branch address as in "dbcc" */
#define	DTRAP	012	/* immediate in low 4 bits */
#define D2L	013	/* register to bits 0-2 of next word */
#define D2H	014	/* register to bits 12-14 of next word */
#define DBL	015	/* qty in bits 0-5 of next word */
#define DBH	016	/* qty in bits 6-11 of next word */
#define DCR	017	/* control reg a bit combination in 0-11 */

#define	DMASK	017

/*
 * operand flags
 */

#define	ADEC	0100	/* funny auto-decrement */
#define	AINC	0200	/* funny auto-increment */
#define	AAREG	0400	/* address register */
#define	ADREG	01000	/* data register */
#define	AONE	02000	/* immediate always 1 */
#define	AWORD	04000	/* immediate always two-byte */

/*
 * special registers
 */

#define	C	0100	/* the condition code register */
#define	SR	0200	/* the status register */
#define	U	0400	/* the user stack pointer */

/*
 * flags
 */

#define	B	01	/* byte */
#define	W	02	/* word */
#define	L	04	/* long */
#define	NZ	010	/* no size */
#define	SZ	017

#define I2W	020		/* two word instruction code */

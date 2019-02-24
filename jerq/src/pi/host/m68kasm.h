// Argument syntax types

#define	AREG	1
#define	AIMM	2
#define	AEXP	3
#define	AIREG	4
#define	AINC	5
#define	ADEC	6
#define	AOFF	7
#define	ANDX	8
#define	AGEN	9
#define	APIC	10
#define	API2	11
#define ABIT	12

#define	AMASK	017

// Modifiers to AREG in op table

#define	A	0100
#define	D	0200
#define	C	0400
#define	SR	01000
#define	P	02000
#define	U	04000
#define	SP	010000
#define CR	020000	/* control registers */

// Modifiers to AGEN

#define	AM	0100	/* alterable memory */
#define	CT	0200	/* Control */
#define	AL	0400	/* alterable */
#define	DA	01000	/* data (not address register) */
#define BF	02000	/* bit field - no auto anything */

// Modifiers to AIMM

#define	O	0100	/* 1 */
#define	Q	0200	/* 1 to 8 */
#define	M	0400	/* -128 to 127 */
#define	N	01000	/* -8 to -1 */
#define	V	02000	/* 0 to 15 */
#define	H	04000	/* -32K to 32K */

// Dispositions of address forms

#define	DIG	0	/* ignore this address */
#define	DEA	1	/* E.A. to low order 6 bits */
#define	DRG	2	/* register to low order 3 bits */
#define	DRGL	3	/* register to bits 11-9 */
#define	DBR	4	/* branch offset (short) */
#define	DMQ	5	/* move-quick 8-bit value */
#define	DAQ	6	/* add-quick 3-bit value in 11-9 */
#define	DIM	7	/* Immediate value, according to size */
#define	DEAM	8	/* E.A. to bits 11-6 as in move */
#define	DBCC	9	/* branch address as in "dbcc" */
#define	DIMH	10	/* Immediate forced to be 1 word */
#define D2L	11	/* register to bits 0-2 of next word */
#define D2H	12	/* register to bits 12-14 of next word */
#define DBL	13	/* qty in bits 0-5 of next word */
#define DBH	14	/* qty in bits 6-11 of next word */
#define DCR	15	/* control reg a bit combination in 0-11 */

// Size codes

#define	B	1	/* byte */
#define	W	2	/* word */
#define	L	4	/* long */
#define	WL	6	/* word or long */
#define	BWL	7	/* any type */

// Size dispositions

#define	SIG	0	/* Ignore the size */
#define	SD	1	/* Standard coding in bits 7-6 */

#define	DREGS	0	/* data registers */
#define	AREGS	8	/* address registers */
#define	PCREG	16
#define	SPREG	17
#define	CCREG	18
#define	SRREG	19
/* unique to 68020 */
#define SFCREG	20	/* source function code */
#define DFCREG	21	/* destination function code */
#define CACRREG	22	/* cache control */
#define USPREG	23	/* user sp */
#define VBRREG	24	/* vector base */
#define CAARREG	25	/* cache address */
#define MSPREG	26	/* master sp */
#define ISPREG	27	/* interrupt sp */

struct m68koptab {
	unsigned short opcode;
	char	*opname;
	short	size;
	short	sdisp;
	short	addr1;
	short	a1disp;
	short	addr2;
	short	a2disp;
	short	iflag;
	unsigned short op2code;
	short	addr3;
	short	a3disp;
	short	addr4;
	short	a4disp;
};

/* special iflags */
#define	ISL	01		/* shrinkable long immediate */
#define	ISH	02		/* shift instruction */
#define I2W	04		/* two word instruction code */

#define	SEGRND	0x1L

class M68kInstr : public Instr {
	char		*dotbwl();
	char		*ea();
	char		*dbr(char);
	char		*dbh();
	char		*dbl();
	short		sizemask();
	int		imm_bytes(short,short);
	char		*dcr();
	char		mode;
	char		size;
	long		immval;
public:
	short		opword0;
	short		opword1;
	m68koptab	*tab;
	char		*arg(int);
	char		*mnemonic();
	int		argtype(int);
	int		nargs();
			M68kInstr(Asm*, long);
};

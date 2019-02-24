struct mac32optab {
	char	*mnem;
	int	type;
};


class Mac32Instr : public Instr {
public:
	char	*arg(int);
	char	*mnemonic();
	int	argtype(int);
	int	nargs();

	char	*extend(int);
	char	*macro();
		Mac32Instr(Asm*, long);
};
extern mac32optab Mac32OpTab[256];

#define UNKNOWN		0
#define OPRNDS0		1	/* instructions with no operands */
#define OPRNDS1		2	/* instructions with 1 operand	 */
#define OPRNDS2		3	/* instructions with 2 operands	 */
#define OPRNDS3		4	/* instructions with 3 operands	 */
#define OPRNDS4		5	/* instructions with 4 operands	 */
#define JUMP		6	/* "jump" instruction */
#define JUMPSB		7	/* "jumpsb" instruction */
#define JUMP1		8	/* 1 byte offset jump instruction */
#define JUMP2		9	/* 2 byte offset jump instruction */
#define	BSBB		10	/* "BSBB" instruction */
#define	BSBH		11	/* "BSBB" instruction */
#define NOOP8		12	/* 8 bit offset (no operands) */
#define NOOP16		13	/* 16 bit offset (no operands) */
#define CALL		14	/* "call" instruction */
#define EXT		15	/* "extendop" instruction */
#define AD1OP3		17	/* 4 operands but add 1 to 3nd operand! */
#define SFPOPS2		18	/* single floating point,  2 operands */
#define	SFPOPS3		19	/* single floating point,  3 operands */
#define	MACRO		20	/* macro-rom instruction */
#define DFPOPS2		21	/* double floating point, 2 operands */
#define	DFPOPS3		22	/* double floating point, 3 operands */
#define SPRTOP0		23	/* support processor inst. 0 operands */
#define SPRTOP1		24	/* support processor inst. 1 operand */
#define SPRTOP2		25	/* support processor inst. 2 operands */
 
/*
 *	address modification types
 */
#define IM6	1	/* immediate 6 bit field */
#define R	2	/* register */
#define RDF	3	/* register deferred (indirect) */
#define R4	4	/* register and 4 bit displacement */
#define IMB	5	/* immediate byte */
#define IMH	6	/* immediate half word */
#define IMW	7	/* immediate word */
#define BDB	8	/* base (register) and byte displacement */
#define BDH	9	/* base (register) and half word displacement */
#define BDW	10	/* base (register) and word displacement */
#define BDBDF	11	/* base and byte displacement deferred */
#define BDHDF	12	/* base and half word displacement deferred */
#define BDWDF	13	/* base and word displacement deferred */
#define EXPSB	14	/* expanded signed byte */
#define EXPUB	15	/* expanded unsigned byte */
#define EXPSH	16	/* expanded signed half word */
#define EXPUH	17	/* expanded unsigned half word */
#define EXPSW	18	/* expanded signed word */
#define EXPUW	19	/* expanded unsigned word */
#define IM4	20	/* immediate 4 bit field */
#define ABAD	21	/* absolute address */
#define ABADDF	22	/* absolute address deferred */
#define	IMD	23	/* immediate double (floating point) */

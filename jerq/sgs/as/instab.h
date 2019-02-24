/*
 *	static char *ID_instabh = "@(#) instab.h: 1.3 12/10/83";
 */

#define SHFT_MSK 0x1F
#define	DUMMY	0x01
#define	INSTRX	0x10
#define	INSTRB	0x11
#define	INSTRH	0x12
#define	INSTRW	0x13
#if	FLOAT
#define	INSTRD	0x14
#endif
#define	INSTRBH	0x15
#define	INSTRBW	0x16
#define	INSTRHW	0x17
#define	INSTRWH	0x18
#define	INSTRWB	0x19
#define	INSTRHB	0x20
/*
 *	the following defines are not needed at this time
 *	but are included for completeness
 *
#if	FLOAT
#define	INSTRHD	0x21
#define	INSTRWD	0x22
#define	INSTRDH	0x23
#define	INSTRDW	0x24
#endif
 *
 */
#define	PSEUDO	0x25
#define	IS25	0x40
#define REGMD		0x1	/* register mode */
#define REGDFMD		0x2	/* register defered mode */
#define IMMD		0x3	/* immediate mode */
#define ABSMD		0x4	/* absolute address mode */
#define ABSDFMD		0x5	/* absolute address deferred mode */
#define EXADMD		0x6	/* external address mode */
#define EXADDFMD	0x7	/* external address deferred mode */
#define DSPMD		0x8	/* displacement mode */
#define DSPDFMD		0x9	/* displacement deferred mode */

#define CREGMD		0x4	/* register mode */
#define CREGDFMD	0x5	/* register deferred mode */
#define CFPDSPMD	0x6	/* FP displacement mode */
#define CAPDSPMD	0x7	/* AP displacement mode */
#define CDSPMD		0x8	/* displacement mode */
#define CDSPDFMD	0x9	/* displacement deferred mode */
#define CHDSPMD		0xA	/* halfword displacement mode */
#define CHDSPDFMD	0xB	/* halfword displacement deferred mode */
#define CBDSPMD		0xC	/* byte displacement mode */
#define CBDSPDFMD	0xD	/* byte displacement deferred mode */
#define CEXPAND		0xE	/* expand byte */
#define CIMMD		0x4F	/* immediate mode */
#define CHIMMD		0x5F	/* halfword immediate  */
#define CBIMMD		0x6F	/* byte immediate  */
#define CABSMD		0x7F	/* absolute address mode */
#define CABSDFMD	0xEF	/* absolute address deferred mode */

#if	FLOAT
#define	FPIMMD		0x13	/* dummy floating point immediate mode */
#define	CDIMMD		0xCF	/* floating point double immediate mode */
#endif

#define FPREG		0x9	/* FP register number */
#define APREG		0xA	/* AP register number */
#define	PSWREG		0xB	/* PSW register number */
#define SPREG		0xC	/* SP register number */
#define	PCREG		0xF	/* PC register number */

#define UBYTE		0x3
#define SBYTE		0x7
#define UHALF		0x2
#define SHALF		0x6
#define UWORD		0x0
#define SWORD		0x4
#define NOTYPE		0xF	/* no new type (i.e. {...}) specified */

#define JMPOPCD 0x24L

#define	NULLSPEC	0x00L
#define BYTESPEC	0x01L
#define	HALFSPEC	0x02L
#define	WORDSPEC	0x03L
#define	LITERALSPEC	0x04L
#define	SHORTSPEC	0x05L

typedef struct {
	short	exptype;
	symbol	*symptr;
	long	expval;		/* holds value of integer expression, coded
				 float or first word of a double constant */
#if	FLOAT
	long	fdexpval2;	/* holds second word of a double constant */
#endif
	/*	variables that may be needed in future fp releases
	 * short fptype;	1=single fp ; 2=double fp constant
	 * double fpexpval;	holds value of floating point expression
	 */
} rexpr;

typedef struct {
	short newtype; /* only need BYTE, but short keeps the structure even */
	BYTE admode;
	BYTE adreg;
	rexpr adexpr;
	/*
	 * The expression specifier describes the size of the expression
	 * and is kept here.
	 */
	long	expspec;
} addrmode;

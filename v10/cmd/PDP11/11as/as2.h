#include <stdio.h>

#define eae 0

#define NCPS 8
#define BSYM 01000		/* builtin symbol */
#define USYM 04000		/* user symbol */

struct expr {
	short typ;
	short val;
};

typedef union {
	struct expr *xp;
	unsigned uv;
	int v;
} OP;

extern	struct expr expres();
extern	OP readop();
extern	OP savop;
extern	OP lastop;

#define LAST(c) (lastop.v==(c))
#define ISCHAR(x) ((x).uv<0200)

struct hdr {
	short	magic;
	short	txtsiz;
	short	datsiz;
	short	bsssiz;
	short	symsiz;
	short	entry;
	short	flag;
	short	dummy;
};
extern struct hdr hdr;

#define T_UNDEF	0
#define T_ABS	1
#define T_TEXT	2
#define T_DATA	3
#define T_BSS	4
#define T_FLOPD	5
#define T_BRANCH	6
#define T_JSR	7
#define T_RTS	010
#define T_SYS	011
#define T_MOVF	012
#define T_DOUBLE	013
#define T_FLOPF	014
#define T_SINGLE	015
#define T_BYTE	016
#define T_STRING	017
#define T_EVEN	020
#define T_IF	021
#define T_ENDIF	022
#define T_GLOBL	023
#define T_REG	024
#define T__TEXT	025
#define T__DATA	026
#define T__BSS	027
#define T_MUL	030
#define T_SOB	031
#define T_COMM	032
#define T_ESTTXT	033
#define T_ESTDAT	034
#define T_JBR	035
#define T_JXX	036

#define T_EXTERN	040
#define R_EXTERN	5
#define PCREL	0100000


extern	short *dot;
extern	short *dotdot;
extern	short *dotrel;
extern	short savdot[3];
extern	long tseek[2];
extern	long rseek[2];
extern	struct expr *curfb[20];
extern	struct expr *usymtab;
extern	struct expr *usymend;
extern	struct expr *xsymbol;
extern	struct expr *fbbuf;
extern	FILE *txtf,*relf;
extern	char argb[20];
extern	int outmod;
extern	int passno;
extern	int errflg;
extern	int line;
extern	int ifflg;
extern	int numval;
extern	int brdelt;

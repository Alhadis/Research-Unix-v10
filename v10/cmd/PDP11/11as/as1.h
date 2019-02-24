#include <stdio.h>

#define eae 0

#define NCPS 8
#define HSHSIZ 3001
#define BSYM 01000		/* builtin symbol */
#define USYM 04000		/* user symbol */

struct expr {
	short typ;
	short val;
};

struct symtab {
	char name[NCPS];
	struct expr sval;
};

typedef union {
	struct expr *xp;
	unsigned uv;
	int v;
} OP;

extern	struct expr expres();
extern	struct expr *rname();
extern	OP readop();
extern	OP _readop();
extern	OP savop;
extern	OP lastop;

#define LAST(c) (lastop.v==(c))
#define ISCHAR(x) ((x).uv<0200)

extern	struct nxtfb {
	char c0;
	char c1;
	short val;
} nxtfb;


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

#define C_IOR	-11
#define C_ESC	-10
#define C_WHITE	-9
#define C_TERM	-8
#define C_DQUO	-7
#define C_GARB	-6
#define C_SQUO	-5
#define C_min4	-4
#define C_SKIP	-3
#define C_min2	-2
#define C_EOL	-1
#define C_STR	0

extern	short *dot;
extern	short *dotdot;
extern	short *dotrel;
extern	char curfbr[10];
extern	int curfb[10];
extern	short savdot[3];
extern	struct symtab *symend;
extern	struct symtab *usymtab;
extern	struct symtab *hshtab[HSHSIZ];
extern	char chartab[];
extern	FILE *pof;
extern	FILE *fbfil;
extern	int errflg;
extern	char ch;
extern	char symbol[NCPS];
extern	int line;
extern	int ifflg;
extern	int nargs;
extern	char **curarg;
extern	int numval;

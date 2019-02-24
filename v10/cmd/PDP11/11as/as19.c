/* a9 -- pdp-11 assembler pass 1 */
#include "as1.h"

/* key to types
/*
/*	T_UNDEF	0 undefined
/*	T_ABS	1 absolute
/*	T_TEXT	2 text
/*	T_DATA	3 data
/*	T_BSS	4 bss
/*	T_FLOPD	5 flop freg,dst (movfo, = stcfd)
/*	T_BRANCH	6 branch
/*	T_JSR	7 jsr
/*	T_RTS	010 rts
/*	T_SYS	011 sys
/*	T_MOVF	012 movf (=ldf,stf)
/*	T_DOUBLE	013 double operand (mov)
/*	T_FLOPF	014 flop fsrc,freg (addf)
/*	T_SINGLE	015 single operand (clr)
/*	T_BYTE	016 .byte
/*	T_STRING	017 string (.ascii, "<")
/*	T_EVEN	020 .even
/*	T_IF	021 .if
/*	T_ENDIF	022 .endif
/*	T_GLOBL	023 .globl
/*	T_REG	024 register
/*	T__TEXT	025 .text
/*	T__DATA	026 .data
/*	T__BSS	027 .bss
/*	T_MUL	030 mul,div, etc
/*	T_SOB	031 sob
/*	T_COMM	032 .comm
/*	T_ESTTXT	033 estimated text
/*	T_ESTDAT	034 estimated data
/*	T_JBR	035 jbr
/*	T_JXX	036 jeq, jne, etc
*/

struct symtab symtab[] = {

	{".", T_TEXT, 000000},
	{"..", T_ABS, 000000},

	{"r0", T_REG, 000000},
	{"r1", T_REG, 000001},
	{"r2", T_REG, 000002},
	{"r3", T_REG, 000003},
	{"r4", T_REG, 000004},
	{"r5", T_REG, 000005},
	{"sp", T_REG, 000006},
	{"pc", T_REG, 000007},

#if eae!=0
	{"csw", T_ABS, 0177570},
	{"div", T_ABS, 0177300},
	{"ac",  T_ABS, 0177302},
	{"mq",  T_ABS, 0177304},
	{"mul", T_ABS, 0177306},
	{"sc",  T_ABS, 0177310},
	{"sr",  T_ABS, 0177311},
	{"nor", T_ABS, 0177312},
	{"lsh", T_ABS, 0177314},
	{"ash", T_ABS, 0177316},
#endif

	{"mov",  T_DOUBLE, 0010000},
	{"movb", T_DOUBLE, 0110000},
	{"cmp",  T_DOUBLE, 0020000},
	{"cmpb", T_DOUBLE, 0120000},
	{"bit",  T_DOUBLE, 0030000},
	{"bitb", T_DOUBLE, 0130000},
	{"bic",  T_DOUBLE, 0040000},
	{"bicb", T_DOUBLE, 0140000},
	{"bis",  T_DOUBLE, 0050000},
	{"bisb", T_DOUBLE, 0150000},
	{"add",  T_DOUBLE, 0060000},
	{"sub",  T_DOUBLE, 0160000},

	{"br",   T_BRANCH, 0000400},
	{"bne",  T_BRANCH, 0001000},
	{"beq",  T_BRANCH, 0001400},
	{"bge",  T_BRANCH, 0002000},
	{"blt",  T_BRANCH, 0002400},
	{"bgt",  T_BRANCH, 0003000},
	{"ble",  T_BRANCH, 0003400},
	{"bpl",  T_BRANCH, 0100000},
	{"bmi",  T_BRANCH, 0100400},
	{"bhi",  T_BRANCH, 0101000},
	{"blos", T_BRANCH, 0101400},
	{"bvc",  T_BRANCH, 0102000},
	{"bvs",  T_BRANCH, 0102400},
	{"bhis", T_BRANCH, 0103000},
	{"bec",  T_BRANCH, 0103000},
	{"bcc",  T_BRANCH, 0103000},
	{"blo",  T_BRANCH, 0103400},
	{"bcs",  T_BRANCH, 0103400},
	{"bes",  T_BRANCH, 0103400},

	{"jbr",  T_JBR, 0000400},
	{"jne",  T_JXX, 0001000},
	{"jeq",  T_JXX, 0001400},
	{"jge",  T_JXX, 0002000},
	{"jlt",  T_JXX, 0002400},
	{"jgt",  T_JXX, 0003000},
	{"jle",  T_JXX, 0003400},
	{"jpl",  T_JXX, 0100000},
	{"jmi",  T_JXX, 0100400},
	{"jhi",  T_JXX, 0101000},
	{"jlos", T_JXX, 0101400},
	{"jvc",  T_JXX, 0102000},
	{"jvs",  T_JXX, 0102400},
	{"jhis", T_JXX, 0103000},
	{"jec",  T_JXX, 0103000},
	{"jcc",  T_JXX, 0103000},
	{"jlo",  T_JXX, 0103400},
	{"jcs",  T_JXX, 0103400},
	{"jes",  T_JXX, 0103400},

	{"clr",  T_SINGLE, 0005000},
	{"clrb", T_SINGLE, 0105000},
	{"com",  T_SINGLE, 0005100},
	{"comb", T_SINGLE, 0105100},
	{"inc",  T_SINGLE, 0005200},
	{"incb", T_SINGLE, 0105200},
	{"dec",  T_SINGLE, 0005300},
	{"decb", T_SINGLE, 0105300},
	{"neg",  T_SINGLE, 0005400},
	{"negb", T_SINGLE, 0105400},
	{"adc",  T_SINGLE, 0005500},
	{"adcb", T_SINGLE, 0105500},
	{"sbc",  T_SINGLE, 0005600},
	{"sbcb", T_SINGLE, 0105600},
	{"tst",  T_SINGLE, 0005700},
	{"tstb", T_SINGLE, 0105700},
	{"ror",  T_SINGLE, 0006000},
	{"rorb", T_SINGLE, 0106000},
	{"rol",  T_SINGLE, 0006100},
	{"rolb", T_SINGLE, 0106100},
	{"asr",  T_SINGLE, 0006200},
	{"asrb", T_SINGLE, 0106200},
	{"asl",  T_SINGLE, 0006300},
	{"aslb", T_SINGLE, 0106300},
	{"jmp",  T_SINGLE, 0000100},
	{"swab", T_SINGLE, 0000300},

	{"jsr", T_JSR, 0004000},

	{"rts", T_RTS, 000200},

	{"sys", T_SYS, 0104400},

	{"clc", T_ABS, 0000241},
	{"clv", T_ABS, 0000242},
	{"clz", T_ABS, 0000244},
	{"cln", T_ABS, 0000250},
	{"sec", T_ABS, 0000261},
	{"sev", T_ABS, 0000262},
	{"sez", T_ABS, 0000264},
	{"sen", T_ABS, 0000270},

	{"cfcc", T_ABS, 0170000},
	{"setf", T_ABS, 0170001},
	{"setd", T_ABS, 0170011},
	{"seti", T_ABS, 0170002},
	{"setl", T_ABS, 0170012},
	{"clrf", T_SINGLE, 0170400},
	{"negf", T_SINGLE, 0170700},
	{"absf", T_SINGLE, 0170600},
	{"tstf", T_SINGLE, 0170500},
	{"movf", T_MOVF, 0172400},
	{"movif", T_FLOPF, 0177000},
	{"movfi", T_FLOPD, 0175400},
	{"movof", T_FLOPF, 0177400},
	{"movfo", T_FLOPD, 0176000},
	{"addf", T_FLOPF, 0172000},
	{"subf", T_FLOPF, 0173000},
	{"mulf", T_FLOPF, 0171000},
	{"divf", T_FLOPF, 0174400},
	{"cmpf", T_FLOPF, 0173400},
	{"modf", T_FLOPF, 0171400},
	{"movie", T_FLOPF, 0176400},
	{"movei", T_FLOPD, 0175000},
	{"ldfps", T_SINGLE, 0170100},
	{"stfps", T_SINGLE, 0170200},
	{"fr0", T_REG, 000000},
	{"fr1", T_REG, 000001},
	{"fr2", T_REG, 000002},
	{"fr3", T_REG, 000003},
	{"fr4", T_REG, 000004},
	{"fr5", T_REG, 000005},

	{"als",  T_MUL, 072000},
	{"alsc", T_MUL, 073000},
	{"mpy",  T_MUL, 070000},
#if (eae-1)!=0
	{"mul", T_MUL, 070000},
	{"div", T_MUL, 071000},
	{"ash", T_MUL, 072000},
	{"ashc", T_MUL, 073000},
#endif
	{"dvd", T_MUL, 071000},
	{"xor", T_JSR, 074000},
	{"sxt", T_SINGLE, 006700},
	{"mark", T_SYS, 006400},
	{"sob", T_SOB, 077000},

	{".byte", T_BYTE, 000000},
	{".even", T_EVEN, 000000},
	{".if", T_IF, 000000},
	{".endif", T_ENDIF, 000000},
	{".globl", T_GLOBL, 000000},
	{".text", T__TEXT, 000000},
	{".data", T__DATA, 000000},
	{".bss", T__BSS, 000000},
	{".comm", T_COMM, 000000},
};

short *dotrel = &symtab[0].sval.typ;
short *dot    = &symtab[0].sval.val;

setup()
{
	register int i,h;
	register char *cp;
	register struct symtab *sp, **hp;

	for (sp=symtab; sp<&symtab[sizeof(symtab)/sizeof(*sp)]; sp++) {
		h=0; cp= &sp->name[0];
		for (i=NCPS; --i>=0; ) {
			if (*cp==0) break;
			h+= *cp++; h=((h&0377)<<8)|((h>>8)&0377);
		}
		hp=hshtab+(((long)(unsigned short)h)%HSHSIZ);
		h= ((long)(unsigned short)h)/HSHSIZ;
		for (;;) {
			if ((hp-=h)<=hshtab) hp+=HSHSIZ;
			if (*--hp==0) break;
		}
		*hp=sp;
	}
	symend=usymtab=(struct symtab *)sbrk(0);
}



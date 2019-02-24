/* a9 -- pdp-11 assembler pass 1 */
#include "as2.h"

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

struct expr symtab[] = {

	{T_TEXT, 000000},	/* . */
	{T_ABS, 000000},	/* .. */

	{T_REG, 000000},	/* r0 */
	{T_REG, 000001},	/* r1 */
	{T_REG, 000002},	/* r2 */
	{T_REG, 000003},	/* r3 */
	{T_REG, 000004},	/* r4 */
	{T_REG, 000005},	/* r5 */
	{T_REG, 000006},	/* sp */
	{T_REG, 000007},	/* pc */

#if eae!=0
	{T_ABS, 0177570},	/* csw */
	{T_ABS, 0177300},	/* div */
	{T_ABS, 0177302},	/* ac */
	{T_ABS, 0177304},	/* mq */
	{T_ABS, 0177306},	/* mul */
	{T_ABS, 0177310},	/* sc */
	{T_ABS, 0177311},	/* sr */
	{T_ABS, 0177312},	/* nor */
	{T_ABS, 0177314},	/* lsh */
	{T_ABS, 0177316},	/* ash */
#endif

	{T_DOUBLE, 0010000},	/* mov */
	{T_DOUBLE, 0110000},	/* movb */
	{T_DOUBLE, 0020000},	/* cmp */
	{T_DOUBLE, 0120000},	/* cmpb */
	{T_DOUBLE, 0030000},	/* bit */
	{T_DOUBLE, 0130000},	/* bitb */
	{T_DOUBLE, 0040000},	/* bic */
	{T_DOUBLE, 0140000},	/* bicb */
	{T_DOUBLE, 0050000},	/* bis */
	{T_DOUBLE, 0150000},	/* bisb */
	{T_DOUBLE, 0060000},	/* add */
	{T_DOUBLE, 0160000},	/* sub */

	{T_BRANCH, 0000400},	/* br */
	{T_BRANCH, 0001000},	/* bne */
	{T_BRANCH, 0001400},	/* beq */
	{T_BRANCH, 0002000},	/* bge */
	{T_BRANCH, 0002400},	/* blt */
	{T_BRANCH, 0003000},	/* bgt */
	{T_BRANCH, 0003400},	/* ble */
	{T_BRANCH, 0100000},	/* bpl */
	{T_BRANCH, 0100400},	/* bmi */
	{T_BRANCH, 0101000},	/* bhi */
	{T_BRANCH, 0101400},	/* blos */
	{T_BRANCH, 0102000},	/* bvc */
	{T_BRANCH, 0102400},	/* bvs */
	{T_BRANCH, 0103000},	/* bhis */
	{T_BRANCH, 0103000},	/* bec */
	{T_BRANCH, 0103000},	/* bcc */
	{T_BRANCH, 0103400},	/* blo */
	{T_BRANCH, 0103400},	/* bcs */
	{T_BRANCH, 0103400},	/* bes */

	{T_JBR, 0000400},	/* jbr */
	{T_JXX, 0001000},	/* jne */
	{T_JXX, 0001400},	/* jeq */
	{T_JXX, 0002000},	/* jge */
	{T_JXX, 0002400},	/* jlt */
	{T_JXX, 0003000},	/* jgt */
	{T_JXX, 0003400},	/* jle */
	{T_JXX, 0100000},	/* jpl */
	{T_JXX, 0100400},	/* jmi */
	{T_JXX, 0101000},	/* jhi */
	{T_JXX, 0101400},	/* jlos */
	{T_JXX, 0102000},	/* jvc */
	{T_JXX, 0102400},	/* jvs */
	{T_JXX, 0103000},	/* jhis */
	{T_JXX, 0103000},	/* jec */
	{T_JXX, 0103000},	/* jcc */
	{T_JXX, 0103400},	/* jlo */
	{T_JXX, 0103400},	/* jcs */
	{T_JXX, 0103400},	/* jes */

	{T_SINGLE, 0005000},	/* clr */
	{T_SINGLE, 0105000},	/* clrb */
	{T_SINGLE, 0005100},	/* com */
	{T_SINGLE, 0105100},	/* comb */
	{T_SINGLE, 0005200},	/* inc */
	{T_SINGLE, 0105200},	/* incb */
	{T_SINGLE, 0005300},	/* dec */
	{T_SINGLE, 0105300},	/* decb */
	{T_SINGLE, 0005400},	/* neg */
	{T_SINGLE, 0105400},	/* negb */
	{T_SINGLE, 0005500},	/* adc */
	{T_SINGLE, 0105500},	/* adcb */
	{T_SINGLE, 0005600},	/* sbc */
	{T_SINGLE, 0105600},	/* sbcb */
	{T_SINGLE, 0005700},	/* tst */
	{T_SINGLE, 0105700},	/* tstb */
	{T_SINGLE, 0006000},	/* ror */
	{T_SINGLE, 0106000},	/* rorb */
	{T_SINGLE, 0006100},	/* rol */
	{T_SINGLE, 0106100},	/* rolb */
	{T_SINGLE, 0006200},	/* asr */
	{T_SINGLE, 0106200},	/* asrb */
	{T_SINGLE, 0006300},	/* asl */
	{T_SINGLE, 0106300},	/* aslb */
	{T_SINGLE, 0000100},	/* jmp */
	{T_SINGLE, 0000300},	/* swab */

	{T_JSR, 0004000},	/* jsr */

	{T_RTS, 000200},	/* rts */

	{T_SYS, 0104400},	/* sys */

	{T_ABS, 0000241},	/* clc */
	{T_ABS, 0000242},	/* clv */
	{T_ABS, 0000244},	/* clz */
	{T_ABS, 0000250},	/* cln */
	{T_ABS, 0000261},	/* sec */
	{T_ABS, 0000262},	/* sev */
	{T_ABS, 0000264},	/* sez */
	{T_ABS, 0000270},	/* sen */

	{T_ABS, 0170000},	/* cfcc */
	{T_ABS, 0170001},	/* setf */
	{T_ABS, 0170011},	/* setd */
	{T_ABS, 0170002},	/* seti */
	{T_ABS, 0170012},	/* setl */
	{T_SINGLE, 0170400},	/* clrf */
	{T_SINGLE, 0170700},	/* negf */
	{T_SINGLE, 0170600},	/* absf */
	{T_SINGLE, 0170500},	/* tstf */
	{T_MOVF, 0172400},	/* movf */
	{T_FLOPF, 0177000},	/* movif */
	{T_FLOPD, 0175400},	/* movfi */
	{T_FLOPF, 0177400},	/* movof */
	{T_FLOPD, 0176000},	/* movfo */
	{T_FLOPF, 0172000},	/* addf */
	{T_FLOPF, 0173000},	/* subf */
	{T_FLOPF, 0171000},	/* mulf */
	{T_FLOPF, 0174400},	/* divf */
	{T_FLOPF, 0173400},	/* cmpf */
	{T_FLOPF, 0171400},	/* modf */
	{T_FLOPF, 0176400},	/* movie */
	{T_FLOPD, 0175000},	/* movei */
	{T_SINGLE, 0170100},	/* ldfps */
	{T_SINGLE, 0170200},	/* stfps */
	{T_REG, 000000},	/* fr0 */
	{T_REG, 000001},	/* fr1 */
	{T_REG, 000002},	/* fr2 */
	{T_REG, 000003},	/* fr3 */
	{T_REG, 000004},	/* fr4 */
	{T_REG, 000005},	/* fr5 */

	{T_MUL, 072000},	/* als */
	{T_MUL, 073000},	/* alsc */
	{T_MUL, 070000},	/* mpy */
#if (eae-1)!=0
	{T_MUL, 070000},	/* mul */
	{T_MUL, 071000},	/* div */
	{T_MUL, 072000},	/* ash */
	{T_MUL, 073000},	/* ashc */
#endif
	{T_MUL, 071000},	/* dvd */
	{T_JSR, 074000},	/* xor */
	{T_SINGLE, 006700},	/* sxt */
	{T_SYS, 006400},	/* mark */
	{T_SOB, 077000},	/* sob */

	{T_BYTE, 000000},	/* .byte */
	{T_EVEN, 000000},	/* .even */
	{T_IF, 000000},	/* .if */
	{T_ENDIF, 000000},	/* .endif */
	{T_GLOBL, 000000},	/* .globl */
	{T__TEXT, 000000},	/* .text */
	{T__DATA, 000000},	/* .data */
	{T__BSS, 000000},	/* .bss */
	{T_COMM, 000000},	/* .comm */

};

short *dotrel = &symtab[0].typ;
short *dot    = &symtab[0].val;
short *dotdot = &symtab[1].val;

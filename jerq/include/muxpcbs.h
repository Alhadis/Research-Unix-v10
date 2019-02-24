struct intrpcb {
	int	initpsw;
	int	(*initpc)();
	int	initsp;
	int	psw;
	int	(*pc)();	/* don't ask */
	int	sp;
	int	lower;
	int	upper;
	int	ap;
	int	fp;
	int	reg[9];		/* 9 user registers */
	int	blkmv;
};

#define PCB_BASE 	((struct intrpcb *)0x71c050)

#define proc_pcb	(PCB_BASE[0])	/* process and interrupt exceptions */
#define stack_pcb	(PCB_BASE[1])	/* stack exceptions */
#define host_pcb	(PCB_BASE[2])	/* RS-232 input interrupts */
#define out_pcb		(PCB_BASE[3])	/* RS-232 output interrupts */
#define pioh_pcb	(PCB_BASE[4])	/* parallel interface */
#define key_pcb		(PCB_BASE[5])	/* keyboard interrupts */
#define msvid_pcb	(PCB_BASE[6])	/* mouse buttons and 60Hz vert. retrace */
#define piol_pcb	(PCB_BASE[7])	/* stray interrupts */
#define int_pcb		(PCB_BASE[8])	/* stray interrupts */
#define main_pcb	(PCB_BASE[9])	/* muxterm itself (short-lived) */

#define intrentry(intr, entry)\
			asm("	.text");\
			asm("	.globl	intr");\
			asm("intr:");\
			asm("	call	&0,entry");\
			asm("	RETPS");

/*
 * psw macros from queue.c.
 */
#define pswmax()	asm("	PUSHW	%psw	");\
			asm("	ORW2	&0x1e000,%psw ");\
			asm("	TSTW	%r0	")

#define pswback()	asm("	POPW	%psw	");\
			asm("   TSTW    %r0     ")

/*
 * pcbs and stacks for standalone programs. Note that the standalone ifile
 * (jxmap) in this directory places pcbs BEFORE stacks.
*/

#define PCBSZ	20
#define STKSZ	100
#define MSTK    1024
#define ISTKSZ	64
#define RESETSZ 256
#define EXCEP_TAB_SIZE	32
#define EXCEP_PSW  0x281e100

#undef	excep_int
#undef	key_int
#undef	host_int
#undef	out_int
#undef	msvid_int

extern int excep_int();


extern int _start,			/* start-up and interrupt handlers */
	key_int,host_int,		/* if not provided by user, from libsys */
	out_int,msvid_int;

/* Stacks (in .bss)  [Must agree with ordering in romterm] */

int istack[ISTKSZ];		/* interrupt stack */
int ststk[RESETSZ];		/* reset exception stack */
int procstk[STKSZ];		/* process exception stack */
int stackstk[STKSZ];		/* stack exception stack */
int hoststk[STKSZ];		/* host input handler stack */
int outstk[STKSZ];		/* host output handler stack */
int piohstk[STKSZ];		/* pioh stack */
int keystk[STKSZ];		/* keyboard interrupt handler stack */
int msvidstk[STKSZ];		/* mouse-video interrupt stack */
int piolstk[STKSZ];		/* piol stack */
int strstk[STKSZ];		/* stray interrupt handler stack */
int mainstk[MSTK];		/* stack for user program */
				/* 4K bytes is default.  User .text, .data and */
				/* .bss follow this, so if it is reduced, it */
				/* will leave more room for user sections - */
				/* just change definition of MSTK and rebuild */
				/* jpcbs.c */


/* PCBs - in .data */

/* Following filled in by startup routine (l.s) */

int proc_pcb[PCBSZ] = {			/* proc_pcb is the process control */
	0x281e180,			/* block for process exceptions    */
	(int)0,				/* excep_proc,  */
	(int)procstk,
	0x281e100,
	(int)0,				/* excep_proc,  */
	(int)procstk,
	(int)procstk,
	(int)&procstk[STKSZ-1],
	0,0,0,0, 0,0,0,0, 0,0,0,0,
};

int stack_pcb[PCBSZ] = {		/* stack_pcb is the process control */
	0x281e180,			/* block for stack exceptions       */
	(int)0,				/* excep_stack, */
	(int)stackstk,
	0x281e100,
	(int)0,				/* excep_stack, */
	(int)stackstk,
	(int)stackstk,
	(int)&stackstk[STKSZ-1],
	0,0,0,0, 0,0,0,0, 0,0,0,0,
};

int host_pcb[PCBSZ] = {
	0x281e180,
	(int)&host_int,
	(int)hoststk,
	0x281e100,
	(int)&host_int,
	(int)hoststk,
	(int)hoststk,
	(int)&hoststk[STKSZ-1],
	0,0,0,0, 0,0,0,0, 0,0,0, 0,
};

int out_pcb[PCBSZ] = {
	0x281e180,
	(int)&out_int,
	(int)outstk,
	0x281e100,
	(int)&out_int,
	(int)outstk,
	(int)outstk,
	(int)&outstk[STKSZ-1],
	0,0,0,0, 0,0,0,0, 0,0,0, 0,
};
int pioh_pcb[PCBSZ] = {
	0x281e180,
	(int)excep_int,
	(int)piohstk,
	0x281e100,
	(int)excep_int,
	(int)piohstk,
	(int)piohstk,
	(int)&piohstk[STKSZ-1],
	0,0,0,0, 0,0,0,0, 0,0,0,0,
};

int key_pcb[PCBSZ] = {
	0x281c180,
	(int)&key_int,
	(int)keystk,
	0x281c100,
	(int)&key_int,
	(int)keystk,
	(int)keystk,
	(int)&keystk[STKSZ-1],
	0,0,0,0, 0,0,0,0, 0,0,0, 0,
};

int msvid_pcb[PCBSZ] = {
	0x281c180,
	(int)&msvid_int,
	(int)msvidstk,
	0x281c100,
	(int)&msvid_int,
	(int)msvidstk,
	(int)msvidstk,
	(int)&msvidstk[STKSZ-1],
	0,0,0,0, 0,0,0,0, 0,0,0, 0,
};

int piol_pcb[PCBSZ] = {
	0x281e180,			/* block for stray interrupts       */
	(int)excep_int,
	(int)piolstk,
	0x281e100,
	(int)excep_int,
	(int)piolstk,
	(int)piolstk,
	(int)&piolstk[STKSZ-1],
	0,0,0,0, 0,0,0,0, 0,0,0,0,
};

int int_pcb[PCBSZ] = {
	0x281e180,			/* block for stray interrupts       */
	(int)excep_int,
	(int)strstk,
	0x281e100,
	(int)excep_int,
	(int)strstk,
	(int)strstk,
	(int)&strstk[STKSZ-1],
	0,0,0,0, 0,0,0,0, 0,0,0,0,
};

/* Following filled in by startup routine (l.s) */

int excep_tab[EXCEP_TAB_SIZE] = {
	EXCEP_PSW,(int)0 /*excep_norm*/, EXCEP_PSW,(int)0 /*excep_norm*/,
	EXCEP_PSW,(int)0 /*excep_norm*/, EXCEP_PSW,(int)0 /*excep_norm*/,
	EXCEP_PSW,(int)0 /*excep_norm*/, EXCEP_PSW,(int)0 /*excep_norm*/,
	EXCEP_PSW,(int)0 /*excep_norm*/, EXCEP_PSW,(int)0 /*excep_norm*/,
	EXCEP_PSW,(int)0 /*excep_norm*/, EXCEP_PSW,(int)0 /*excep_norm*/,
	EXCEP_PSW,(int)0 /*excep_norm*/, EXCEP_PSW,(int)0 /*excep_norm*/,
	EXCEP_PSW,(int)0 /*excep_norm*/, EXCEP_PSW,(int)0 /*excep_norm*/,
	EXCEP_PSW,(int)0 /*excep_norm*/, EXCEP_PSW,(int)0 /*excep_norm*/
		  };
int main_pcb[PCBSZ] = {			/* main_pcb is the process control  */
	0x281e180,			/* block for downloaded programs    */
	(int)&_start,
	(int)mainstk,
	0x281e100,
	(int)&_start,
	(int)mainstk,
	(int)mainstk,
	(int)&mainstk[MSTK-1],
	0,0,0,0, 0,0,0,0, 0,0,0,0,
};

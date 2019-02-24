#define PCBSZ	20
#define STKSZ	100
#define MSTK    1024
#define ISTKSZ	64
#define RESETSZ 256
#define EXCEP_TAB_SIZE	32
#define EXCEP_PSW  0x281e100

extern int excep_norm(); /* exception handlers */
extern int excep_proc();
extern int excep_stack();
extern int excep_int();


extern int _start,key_int,host_int,out_int,msvid_int,pioh_int;
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

/* bss, gets loaded after the following .data stuff */

int proc_pcb[PCBSZ]={	/* process exceptions and interrupt exceptions */
	0x281e180,
	(int)excep_proc,
	(int)procstk,
	0x281e100,
	(int)excep_proc,
	(int)procstk,
	(int)procstk,
	(int)&procstk[STKSZ-1],
	0,0,0,0, 0,0,0,0, 0,0,0,0,
};
int stack_pcb[PCBSZ]={	/* stack exceptions */
	0x281e180,
	(int)excep_stack,
	(int)stackstk,
	0x281e100,
	(int)excep_stack,
	(int)stackstk,
	(int)stackstk,
	(int)&stackstk[STKSZ-1],
	0,0,0,0, 0,0,0,0, 0,0,0,0,
};
int host_pcb[PCBSZ]={	/* RS-232 input interrupts */
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
int out_pcb[PCBSZ]={	/* RS-232 output interrupts */
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
int pioh_pcb[PCBSZ]={	/* parallel interface */
	0x281e180,
	(int)&pioh_int,
	(int)piohstk,
	0x281e100,
	(int)&pioh_int,
	(int)piohstk,
	(int)piohstk,
	(int)&piohstk[STKSZ-1],
	0,0,0,0, 0,0,0,0, 0,0,0,0,
};
int key_pcb[PCBSZ]={	/* keyboard interrupts */
	0x281e180,
	(int)&key_int,
	(int)keystk,
	0x281e100,
	(int)&key_int,
	(int)keystk,
	(int)keystk,
	(int)&keystk[STKSZ-1],
	0,0,0,0, 0,0,0,0, 0,0,0, 0,
};
int msvid_pcb[PCBSZ]={	/* mouse buttons and 60Hz vertical retrace */
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
int piol_pcb[PCBSZ]={	/* stray interrupts */
	0x281e180,
	(int)excep_int,
	(int)piolstk,
	0x281e100,
	(int)excep_int,
	(int)piolstk,
	(int)piolstk,
	(int)&piolstk[STKSZ-1],
	0,0,0,0, 0,0,0,0, 0,0,0,0,
};
int int_pcb[PCBSZ]={	/* stray interrupts */
	0x281e180,
	(int)excep_int,
	(int)strstk,
	0x281e100,
	(int)excep_int,
	(int)strstk,
	(int)strstk,
	(int)&strstk[STKSZ-1],
	0,0,0,0, 0,0,0,0, 0,0,0,0,
};
int excep_tab[EXCEP_TAB_SIZE] = {	/* This could be done just as  */
			/* copies to save rom space exceptab[2] and mult   */
	EXCEP_PSW,(int)excep_norm, EXCEP_PSW, (int)excep_norm,
	EXCEP_PSW,(int)excep_norm, EXCEP_PSW, (int)excep_norm,
	EXCEP_PSW,(int)excep_norm, EXCEP_PSW, (int)excep_norm,
	EXCEP_PSW,(int)excep_norm, EXCEP_PSW, (int)excep_norm,
	EXCEP_PSW,(int)excep_norm, EXCEP_PSW, (int)excep_norm,
	EXCEP_PSW,(int)excep_norm, EXCEP_PSW, (int)excep_norm,
	EXCEP_PSW,(int)excep_norm, EXCEP_PSW, (int)excep_norm,
	EXCEP_PSW,(int)excep_norm, EXCEP_PSW, (int)excep_norm
		  };
int main_pcb[PCBSZ]={	/* muxterm itself (short-lived) */
	0x281e180,
	(int)&_start,
	(int)mainstk,
	0x281e100,
	(int)&_start,
	(int)mainstk,
	(int)mainstk,
	(int)&mainstk[MSTK-1],
	0,0,0,0, 0,0,0,0, 0,0,0,0,
};

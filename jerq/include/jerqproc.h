#include <tty.h>

#ifndef		PROC_H
#define		PROC_H
extern	int	NPROC;
#define	STKSIZ	(4096-sizeof(struct _*))
#define	CBSIZE	64	/* NPROC*CBSIZE <= queue.h/NCHARS */

#ifndef	QUEUE_H
struct clist {
	long	c_tail;
	long	c_head;
	short	c_cc;
	short	state;
};
#endif
struct pcb {
	int	psw;
	char	(*pc)();
	int	sp;
	int	lower;
	int	upper;
	int	ap;
	int	fp;
	int	reg[9];		/* 9 user registers */
	int	blkmv;
};
struct Proc {
	struct pcb pcb;
	char	*text;
	char	*data;
	char	*bss;
	int	state;
	Layer	 * layer;
	Rectangle rect;	/* rectangle on screen */
	struct clist kbdqueue;
	char	**traploc;
	int	*traptype;
	int	nticks;
	Texture * cursor;
	short	inhibited;
	short	nchars;
	unsigned char	cbuf[CBSIZE*3];		/* circular buffer */
	unsigned char	*cbufpin, *cbufpout;
	struct ttychars ttychars;		/* chars for tty driver emulation */
	int	stack[STKSIZ/sizeof(int)];	/* int for alignment  */
	struct Font *defaultfont;
};

struct Nqueue {			/* structure used by putname / getname */
	struct Nqueue *next;
	struct Proc *proc;
	long data;
	char name[4];
};

#ifndef	MUX_H
#	ifdef	TASK_C
	struct Proc *newproc();
#	else
	extern struct Proc *newproc();
#	endif
extern struct Proc *P;
extern struct Proc proctab[];
#endif

#ifdef	MUXTERM
/*
 * Data at start of user program
 */
struct udata {
	Rectangle Drect;
	Layer	 * Jdisplayp;
	int	argc;
	char	**argv;
	struct Mouse mouse;
};
/* states */
#define	RUN		1	/* ready to be scheduled */
#define	BUSY		2	/* active */
#define	BLOCKED		4	/* blocked by user with ^S */
#define	USER		8	/* a user-68ld'd process */
#define	KBDLOCAL	16	/* has requested the KBD */
#define	MOUSELOCAL	32	/* has requested the MOUSE */
#define	GOTMOUSE	64	/* currently owns MOUSE */
#define	WAKEUP		128	/* tell CONTROL to issue setrun(p) */
#define	MOVED		256	/* layer got moved */
#define	UNBLOCKED	512	/* Has been unblocked */
#define	ZOMBIE		1024	/* proc died horribly; waiting for debugger */
#define	RESHAPED	2048	/* layer got reshaped */
#define	ZOMBOOT		4096	/* put in ZOMBIE state after booting */
#define	ALARMREQD	8192	/* has requested an alarm */	

#define	setrun(p)	((p)->state|=RUN)
#define	setnorun(p)	((p)->state&=~RUN)

#else
#define	MOVED		256	/* layer got moved */
#define	RESHAPED	2048	/* layer got reshaped */
#endif	MUXTERM
#endif

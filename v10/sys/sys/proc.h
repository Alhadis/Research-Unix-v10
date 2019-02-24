/*
 * One structure allocated per active
 * process. It contains all data needed
 * about the process while the
 * process may be swapped out.
 * Other per process data (user.h)
 * is swapped with the process.
 */

struct	proc
{
	struct	proc *p_link;	/* linked list of running processes */
	struct	proc *p_rlink;	/* (used by hardware q instructions) */
	struct	pte *p_addr;	/* u-area kernel map address */
	unsigned char	p_usrpri;	/* user-priority based on p_cpu and p_nice */
	char	p_pri;		/* priority, negative is high */
	unsigned char	p_cpu;		/* cpu usage for scheduling */
	char	p_stat;
	char	p_time;		/* resident time for scheduling */
	char	p_nice;		/* nice for cpu usage */
	char	p_slptime;	/* time since last block */
	char	p_cursig;
	long	p_sig;		/* signals pending to this process */
	long	p_siga0;	/* low bit of 2 bit signal action */
	long	p_siga1;	/* high bit of 2 bit signal action */
#define	p_ignsig p_siga0	/* ignored signal mask */
	int	p_flag;
	short	p_uid;		/* user id, used to direct tty signals */
	short	p_pgrp;		/* name of process group leader */
	short	p_pid;		/* unique process id */
	short	p_ppid;		/* process id of parent */
	void	*Jp_lnode;	/* per-user scheduling */
 	float	Jp_sharepri;	/* priority based on p_nice and p_lnode->kl.l_usage */
	short	p_poip;		/* count of page outs in progress */
	short	p_szpt;		/* copy of page table size */
	clicks_t	p_tsize;	/* size of text (clicks) */
	clicks_t	p_dsize;	/* size of data space (clicks) */
	clicks_t	p_ssize;	/* copy of stack size (clicks) */
	clicks_t 	p_rssize; 	/* current resident set size in clicks */
	clicks_t	p_maxrss;	/* copy of u.u_limit[MAXRSS] */
	clicks_t	p_swrss;	/* resident set size before last swap */
	swblk_t	p_swaddr;	/* disk address of u area when swapped */
	caddr_t p_wchan;	/* event process is awaiting */
	struct	text *p_textp;	/* pointer to text structure */
	unsigned short	p_clktim;	/* time to alarm clock signal */
	unsigned short	p_tsleep;	/* time to tsleep */
	struct	pte *p_p0br;	/* page table base P0BR */
	struct	proc *p_xlink;	/* linked list of procs sharing same text */
	short	p_cpticks;	/* ticks of cpu time (only for p_pctcpu) */
	float	p_pctcpu;	/* %cpu for ps (only) */
	short	Jp_ndx;		/* proc index for memall (because of vfork) */
	short	Jp_idhash;	/* hashed based on p_pid for kill+exit+... */
	struct	proc *p_pptr;	/* pointer to process structure of parent */
	struct	inode *p_trace;	/* inode for tracing, see proca.c */
};

#ifdef KERNEL
/*
 * temporary expedient to clean up p_siga0/p_siga1 mess
 */
#define	P_SETDFL(p, mask)	(p->p_siga0&=~(mask),p->p_siga1&=~(mask))
#define	P_SETIGN(p, mask)	(p->p_siga0|=(mask),p->p_siga1&=~(mask))
#define	P_SETCATCH(p, mask)	(p->p_siga0&=~(mask),p->p_siga1|=(mask))
#define	P_SETHOLD(p, mask)	(p->p_siga0|=(mask),p->p_siga1|=(mask))

#define	P_SIGVAL(p, mask)	((int(*)())\
	(((p->p_siga0&(mask))!=0)|(((p->p_siga1&(mask))!=0)<<1)))

#define	SIGMASK(sig)	(1<<((sig)-1))
extern struct proc proc[];
extern	int proccnt;
extern struct	proc *procNPROC;	/* high water mark */
struct	proc *pfind();

#define	NQS	32		/* 32 run queues */
struct	prochd {
	struct	proc *ph_link;	/* linked list of running processes */
	struct	proc *ph_rlink;
} qs[NQS];
int	whichqs;		/* bit mask summarizing non-empty qs's */
#endif

/* stat codes */
#define	SSLEEP	1		/* awaiting an event */
#define	SWAIT	2		/* (abandoned state) */
#define	SRUN	3		/* running */
#define	SIDL	4		/* intermediate state in process creation */
#define	SZOMB	5		/* intermediate state in process termination */
#define	SSTOP	6		/* process being traced */

/* flag codes */
#define	SLOAD	0x00000001	/* in core */
#define	SSYS	0x00000002	/* swapper or pager process */
#define	SLOCK	0x00000004	/* process being swapped out */
#define	SSWAP	0x00000008	/* save area flag */
#define	STRC	0x00000010	/* process is being traced */
#define	SWTED	0x00000020	/* another tracing flag */
#define	SULOCK	0x00000040	/* user settable lock in core */
#define	SPAGE	0x00000080	/* process in page wait state */
#define	SKEEP	0x00000100	/* another flag to prevent swap out */
#define	SDLYU	0x00000200	/* delayed unlock of pages */
#define	SWEXIT	0x00000400	/* working on exiting */
#define	SPHYSIO	0x00000800	/* doing physical i/o (bio.c) */
#define	SPAGI	0x00008000	/* init data space on demand, from inode */
#define	SSEQL	0x00010000	/* user warned of sequential vm behavior */
#define	SUANOM	0x00020000	/* user warned of random vm behavior */
#define	STIMO	0x00040000	/* timing out during sleep */
#define	SOWEUPC	0x00200000	/* owe process an addupc() call at next ast */
#define	SSEL	0x00400000	/* selecting: wakeup/waiting danger */
#define	SPROCTR	0x00800000	/* tracing via /proc */
#define	SPROCIO	0x01000000	/* doing I/O via /proc, so don't swap */
#define	SSEXEC	0x02000000	/* stop on exec */
#define	SPROCWT	0x04000000	/* wanted by /proc after SPAGE */

#define PTRACED(p)	((p)->p_trace || ((p)->p_flag&(STRC|SPROCTR|SSEXEC)))

/*
 * special system process IDs
 * it is assumed that proc[syspid].p_pid == syspid
 */
#define	SWAPPID	0	/* swapper */
#define	INITPID	1	/* init; user process, but still special */
#define	PAGEPID	2	/* pageout daemon */
#define	SYSPIDS	3	/* number of system pids */

/*
 * parallel proc structure
 * to replace part with times
 * to be passed to parent process
 * in ZOMBIE state.
 *
 * THIS SHOULD BE DONE WITH A union() CONSTRUCTION
 */
struct	xproc
{
	struct	proc *xp_link;
	struct	proc *xp_rlink;
	struct	pte *xp_addr;
	char	xp_usrpri;
	char	xp_pri;		/* priority, negative is high */
	char	xp_cpu;		/* cpu usage for scheduling */
	char	xp_stat;
	char	xp_time;	/* resident time for scheduling */
	char	xp_nice;	/* nice for cpu usage */
	char	xp_slptime;
	char	xp_cursig;
	long	xp_sig;		/* signals pending to this process */
	long	xp_siga0;
	long	xp_siga1;
	int	xp_flag;
	short	xp_uid;		/* user id, used to direct tty signals */
	short	xp_pgrp;	/* name of process group leader */
	short	xp_pid;		/* unique process id */
	short	xp_ppid;	/* process id of parent */
 	void	*Jxp_lnode;	/* per-user scheduling */
 	float	Jxp_sharepri;	/* priority based on p_nice and p_lnode->kl.l_usage */
	short	xp_xstat;	/* Exit status for wait */
	struct	vtimes xp_vm;
};

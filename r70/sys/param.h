/*
 * Tunable parameters which do not usually vary per system
 */

#define	MAXUPRC	75		/* max processes per user */
#define	SSIZE	4		/* initial stack size (*512 bytes) */
#define	SINCR	4		/* increment of stack (*512 bytes) */
#define	NOFILE	128		/* max open files per process */
#define	NSYSFILE 4		/* stdin, stdout, stderr, /dev/tty */
#define	NCARGS	(16*1024)	/* # characters in exec arglist */
#define	CMASK	0		/* default mask for file creation */
#define	NGROUPS	32		/* number of simultaneous groups */

/*
 * priorities
 * probably should not be
 * altered too much
 */

#define	PSWP	0
#define	PINOD	10
#define	PRIBIO	20
#define	PZERO	25
#define	PPIPE	26
#define	PWAIT	30
#define	PSLEP	40
#define	PUSER	50
#define	PRIMAX	127

#define	NZERO	20

/*
 * signals
 * dont change
 */

#ifndef	NSIG
#include <signal.h>
#endif

/*
 * Return values from tsleep().
 */
#define	TS_OK	0	/* normal wakeup */
#define	TS_TIME	1	/* timed-out wakeup */
#define	TS_SIG	2	/* asynchronous signal wakeup */

#define	UPAGES	10		/* pages of u-area */
#define	NULL	0
#define	NODEV	(dev_t)(-1)
#define	ROOTINO	((ino_t)2)	/* i number of all roots */
#define	SUPERB	((daddr_t)1)	/* block number of the super block */
#define	DIRSIZ	14		/* max characters per directory */
#define	MAXPID	30000		/* largest process id */

/*
 * clustering of small hardware pages to larger software pages
 * this should be in a separate file, one might guess
 *
 * bugs: SSIZE, SINCR and UPAGES must be multiples of CLSIZE
 * BSIZE must bear some relation to CLSIZE*NBPG (but what?)
 */
#define	CLSIZE	2
#define	CLOFSET		(CLSIZE*NBPG-1)	/* for clusters, like PGOFSET */

/* give the base virtual address (first of CLSIZE) */
#define	clbase(i)	((i) &~ (CLSIZE-1))

/* round a number of clicks up to a whole cluster */
#define	clrnd(i)	(((i) + (CLSIZE-1)) &~ (CLSIZE-1))

/*
 * the following tend to assume that CLSIZE == 2
 */
#define BITFS(dev)	((dev) & 64)
#define BUFSIZE		4096	/* real buffer size */
#define	BSIZE(dev)	(BITFS(dev)? 4096: 1024)
#define	INOPB(dev)	(BITFS(dev)? 64: 16)	/* inodes per block */
#define	BMASK(dev)	(BITFS(dev)? 07777: 01777)	/* BSIZE-1 */
#define	BSHIFT(dev)	(BITFS(dev)? 12: 10)	/* LOG2(BSIZE) */
#define NINDIR(dev)	(BSIZE(dev)/sizeof(daddr_t))
#define	NMASK(dev)	(BITFS(dev)? 01777: 0377)	/* NINDIR-1 */
#define	NSHIFT(dev)	(BITFS(dev)? 10: 8)	/* LOG2(NINDIR) */
#define	NICINOD	100		/* free inodes stored in super block */
#define	NICFREE	178		/* free blocks in (old) super block */
#define itod(dev, x)	((daddr_t)((((unsigned)(x)+2*INOPB(dev)-1)/INOPB(dev))))
#define itoo(dev, x)	((int)(((x)+2*INOPB(dev)-1)%INOPB(dev)))
#define fsbtodb(dev, b)	(BITFS(dev)? (b)*8: (b)*CLSIZE)
#define dbtofsb(dev, b)	(BITFS(dev)? (b)/8: (b)/CLSIZE)

#ifndef INTRLVE
/* macros replacing interleaving functions */
#define	dkblock(bp)	((bp)->b_blkno)
#define	dkunit(bp)	(minor((bp)->b_dev & 077) >> 3)
/* that means 8 units with at most 8 pieces each */
#endif

/*
 * Macros for fast min/max
 */
#define	MIN(a,b) (((a)<(b))?(a):(b))
#define	MAX(a,b) (((a)>(b))?(a):(b))

/*
 * Machine-dependent bits and macros
 */
#define	UMODE	PSL_CURMOD		/* usermode bits */
#define	USERMODE(ps)	(((ps) & UMODE) == UMODE)

#define	BASEPRI(ps)	(((ps) & PSL_IPL) != 0)

/*
 * Provide about n microseconds of delay
 */
#define	DELAY(n)	{extern int delayfact;register int N = (n)*delayfact;while (--N > 0);}

/*
 * hardware definitions
 * some are (stupidly) in types.h
 */
#define	USRSTACK	(0x80000000-UPAGES*NBPG)
					/* Start of user stack */
#define	P1TOP		0x200000	/* boundary between P0 and P1 regions */
#define	KSTART		0x80000000	/* beginning of system space */

#define	HZ	60			/* software clock ticks per second */

/*
 * user-accessible types and macros
 */
#include "sys/types.h"

/*
 * very long stuff
 */
#define	Ltol(x)		(x).lo
llong_t	ltoL();
llong_t	Lladd();
llong_t	Luadd();
llong_t	LLadd();
#define	Lsign(x)	(x).hi
unsigned Lshift();

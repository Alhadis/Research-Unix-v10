#include "sys/param.h"
#include "sys/systm.h"
#include "sys/user.h"
#include "sys/filsys.h"
#include "sys/mtpr.h"
#include "sys/pte.h"
#include "sys/proc.h"
#include "sys/inode.h"
#include "sys/conf.h"
#include "sys/buf.h"
#include "sys/clock.h"
#include "sys/vm.h"
#include "sys/text.h"
#include "sys/vlimit.h"
#include "sys/file.h"

/*
 * Initialization code.
 * Called from cold start routine as
 * soon as a stack and segmentation
 * have been established.
 * Functions:
 *	clear and free user core
 *	turn on clock
 *	hand craft 0th process
 *	call all initialization routines
 *	fork - process 0 to schedule
 *	     - process 2 to page out
 *	     - process 1 execute bootstrap
 *
 * loop at low addr in user mode -- /etc/init
 *	cannot be executed.
 */
struct proc *procNPROC;
struct text *textNTEXT;
struct file *fileNFILE;
struct inode *inodeNINODE;
time_t bootime;		/* should probably go away */

main()
{
	register int i;
	register struct proc *p;
	extern char sigcode[];
	extern int szsigcode;
	extern char icode[];
	extern int szicode;
	extern int swbufcnt;

	rqinit();
	procNPROC = &proc[SYSPIDS];
	textNTEXT = &text[textcnt];
	fileNFILE = &file[filecnt];
	inodeNINODE = &inode[inodecnt];
	startup();

	/*
	 * set up system process 0 (swapper)
	 */
	p = &proc[SWAPPID];
	p->p_stat = SRUN;
	p->p_flag |= SLOAD|SSYS;
	p->p_nice = NZERO;
	u.u_procp = p;
	bcopy((caddr_t)sigcode, (caddr_t)u.u_pcb.pcb_sigc, szsigcode);
	u.u_cmask = CMASK;
	for (i = 0; i < NGROUPS; i++)
		u.u_groups[i] = NOGROUP;	/* null access group for init */
	for (i = 1; i < sizeof(u.u_limit)/sizeof(u.u_limit[0]); i++)
		u.u_limit[i] = INFINITY;
	u.u_limit[LIM_STACK] = ctob(maxssize);
	u.u_limit[LIM_DATA] = ctob(maxdsize);
	u.u_limit[LIM_TEXT] = ctob(maxtsize);
	p->p_maxrss = INFINITY/NBPG;
	callinit();
	clkstart();

	/*
	 * Initialize devices and
	 * set up 'known' i-nodes
	 */

	ihinit();
	bhinit();
	binit();
	qinit();
	bswinit();
	iinit();
	bootime = time;
	u.u_dmap = zdmap;
	u.u_smap = zdmap;

	/*
	 * Set the scan rate and other parameters of the paging subsystem.
	 */
	setupclock();

	/*
	 * make page-out daemon (process 2)
	 * the daemon has ctopt(swbufcnt*CLSIZE*KLMAX) pages of page
	 * table so that it can map dirty pages into
	 * its address space during asychronous pushes.
	 * this stuff should be shoved out to a vm init routine.
	 */

	p->p_szpt = clrnd(ctopt(swbufcnt*CLSIZE*KLMAX + UPAGES));
	if (newproc(&proc[PAGEPID], PAGEPID)) {
		proc[PAGEPID].p_flag |= SLOAD|SSYS;
		proc[PAGEPID].p_dsize = u.u_dsize = swbufcnt*CLSIZE*KLMAX; 
		pageout();
	}
	/*
	 * make init process and
	 * enter scheduling loop
	 */
	p->p_szpt = CLSIZE;
	if (newproc(&proc[INITPID], INITPID)) {
		expand(clrnd((int)btoc(szicode)), P0BR);
		(void) swpexpand(u.u_dsize, 0, &u.u_dmap, &u.u_smap);
		(void) copyout((caddr_t)icode, (caddr_t)0, (unsigned)szicode);
		/*
		 * Return goes to loc. 0 of user init
		 * code just copied out.
		 */
		return;
	}
	p->p_szpt = 1;
	sched();
}

/*
 * iinit is called once (from main)
 * very early in initialization.
 * It reads the root's super block
 * it mounts the root filesystem.
 * there is a small amount of magic:
 * an inode is invented to hold the special file
 * for the root device, so it is known that
 * a special file in that filesystem is involved;
 * the filesystem is mounted on the special file,
 * then patched so that the root is also the mount point.
 * some of this magic should be removed.
 *
 * some filesystem types init the date, again through magic
 */

static struct inode rootspcl;

iinit()
{
	register struct inode *ip;
	extern int rootfstyp;

	bdevopen(rootdev);
	if (u.u_error)
		panic("iinit root");
	bdevopen(swapdev);
	if (u.u_error)
		panic("iinit swap");
	rootspcl.i_dev = NODEV;
	rootspcl.i_fstyp = rootfstyp;
	rootspcl.i_mode = IFBLK;
	rootspcl.i_un.i_rdev = rootdev;
	rootspcl.i_count = 1;
	/* rootspcl.i_un.i_bufp == NULL; does it matter? */
	if (fstypsw[rootfstyp] == NULL)
		panic("iinit fstyp");
	(*fstypsw[rootfstyp]->t_mount)(&rootspcl, &rootspcl, 0, 1, rootfstyp);
	if (u.u_error)
		panic("iinit mount");
	ip = rootspcl.i_mroot;
	ip->i_mpoint = ip;
	ip->i_mroot = NULL;	/* own mount point, but not mounted on self */
	rootdir = ip;
	u.u_cdir = ip;
	ip->i_count += 2;	/* needed? */
	u.u_rdir = NULL;
}

/*
 * open a block device, with no more than dev
 */

bdevopen(dev)
dev_t dev;
{
	register unsigned maj;

	maj = major(dev);
	if (maj >= nblkdev || bdevsw[maj] == NULL) {
		u.u_error = ENODEV;
		return;
	}
	(*bdevsw[maj]->d_open)(dev, FREAD|FWRITE);
}

/*
 * Initialize the buffer I/O system by freeing
 * all buffers and setting all device buffer lists to empty.
 */
binit()
{
	register struct buf *bp;
	register struct buf *dp;
	register int i;

	for (dp = bfreelist; dp < &bfreelist[BQUEUES]; dp++) {
		dp->b_forw = dp->b_back = dp->av_forw = dp->av_back = dp;
		dp->b_flags = B_HEAD;
	}
	dp--;				/* dp = &bfreelist[BQUEUES-1]; */
	for (i=0; i<nbuf; i++) {
		bp = &buf[i];
		bp->b_dev = NODEV;
		bp->b_un.b_addr = buffers + i * BUFSIZE;
		bp->b_back = dp;
		bp->b_forw = dp->b_forw;
		dp->b_forw->b_back = bp;
		dp->b_forw = bp;
		bp->b_flags = B_BUSY|B_INVAL;
		brelse(bp);
	}
}

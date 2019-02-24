/*
 * structures defining driver entry points
 * tables of pointers to, e.g., struct bdevsw are declared in conf.c
 * the structures themselves are defined in the drivers that use them
 *
 * the init macros are used just to check the number of arguments
 */
struct bdevsw
{
	int	(*d_open)();
	int	(*d_close)();
	int	(*d_strategy)();
	int	d_flags;
};
#ifdef KERNEL
extern struct bdevsw *bdevsw[];
extern struct bdevsw nullbdev;
extern int nblkdev;
#define	bdinit(open, close, strat, flag)	{open, close, strat, flag}
#endif

/*
 * Character device switch.
 */
extern struct cdevsw
{
	int	(*d_open)();
	int	(*d_close)();
	int	(*d_read)();
	int	(*d_write)();
	int	(*d_ioctl)();
	int	(*d_reset)();		/* ??? */
	struct	streamtab *qinfo;
};
#ifdef KERNEL
extern struct cdevsw *cdevsw[];
extern struct cdevsw nullcdev;
extern int nchrdev;
#define	cdinit(open, close, read, write, ioctl) {open, close, read, write, ioctl, nulldev, NULL}
#define	cstrinit(stream)	{nodev, nulldev, nodev, nodev, nodev, nulldev, stream}
#endif

/* file system types */
struct fstypsw {
	int		(*t_put)();
	int		(*t_updat)();
	int		(*t_read)();
	int		(*t_write)();
	int		(*t_trunc)();
	int		(*t_stat)();
	int		(*t_nami)();
	int		(*t_mount)();
	int		(*t_ioctl)();
	struct inode *	(*t_open)();
	int		(*t_dirread)();
};
#ifdef KERNEL
extern struct fstypsw *fstypsw[];
extern nfstyp;
#define	fsinit(put, updat, read, write, trunc, stat, nami, mount, ioctl, open, dir) \
	{put, updat, read, write, trunc, stat, nami, mount, ioctl, open, dir}
#endif

/*
 * stream processor table
 */
struct streamtab {
	struct	qinit	*rdinit;
	struct	qinit	*wrinit;
};

#ifdef KERNEL
extern struct streamtab *streamtab[];
extern struct streamtab nullstream;
extern int nstreamtab;
#endif

/*
 * Swap device information
 */
struct swdevt
{
	dev_t	sw_dev;
	daddr_t	sw_size;
	int	sw_freed;
};
#ifdef KERNEL
extern struct swdevt swdevt[];
int	nswdevt;

extern struct inode *nullopen();
extern int nullnami();
extern int nulldev(), nodev();
#endif

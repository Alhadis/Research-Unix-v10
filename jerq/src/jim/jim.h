typedef unsigned long	ulong;
typedef unsigned long	Posn;
extern char	*homedir;
extern char	*alloc();
extern char	*brk();
extern char	*gcalloc();
extern char	*gcrealloc();
extern char	*sbrk();
extern char	*charstar();
extern char	*jerqname();
extern long	min();
#define	gcnew(p, n)	((void)gcalloc((ulong)(n)*sizeof((p)[0]), (long **)&(p)))
#define	gcrenew(p, n)	((void)gcrealloc((char *)p, ((ulong)(n)*sizeof((p)[0]))))
#define	MAXFILES	256	/* 255 real files, 0 for unallocated */

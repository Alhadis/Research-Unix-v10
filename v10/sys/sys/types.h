/*
 * Basic system types and related macros
 */


/*
 * hardware parameters
 */

#define	NBBY		8		/* number of bits in a byte */
#define	NBPW		sizeof(int)	/* number of bytes in an integer */
#define	NBPG		512		/* bytes per hardware page */
#define	PGOFSET		(NBPG-1)	/* byte offset into page */
#define	PGSHIFT		9		/* LOG2(NBPG) */

/* Core clicks (512 bytes) to segments and vice versa */
#define	ctos(x)	(x)
#define	stoc(x)	(x)

/* Core clicks (512 bytes) to disk blocks */
#define	ctod(x)	(x)

/* clicks to bytes */
#define	ctob(x)	((x)<<9)

/* bytes to clicks */
#define	btoc(x)	((((unsigned)(x)+511)>>9))

/*
 * these should go away;
 * just say `unsigned'
 */
typedef	unsigned char	u_char;
typedef	unsigned short	u_short;
typedef	unsigned int	u_int;
typedef	unsigned long	u_long;

typedef	long	daddr_t;	/* disk blocks */
typedef	char *	caddr_t;
typedef	unsigned short	ino_t;
typedef	long	swblk_t;
typedef	long	clicks_t;	/* memory clicks */
typedef	long	time_t;
typedef	long	label_t[14];
typedef	unsigned short	dev_t;
typedef	long	off_t;
typedef	struct { unsigned long lo; long hi; } llong_t;

/* major part of a device */
#define	major(x)	((int)(((unsigned)(x)>>8)&0377))

/* minor part of a device */
#define	minor(x)	((int)((x)&0377))

/* make a device number */
#define	makedev(x,y)	((dev_t)(((x)<<8) | (y)))

/*
 * Set of fds used with the select system call.
 * assumed that NOFILE <= 128
 */
#define FDWORDS		(128+NBPW*NBBY-1)/(NBPW*NBBY)
typedef struct		fd_set { unsigned int fds_bits[FDWORDS]; } fd_set;
#define FD_SET(n,s)	(s).fds_bits[(n)/(NBPW*NBBY)] |= 1<<((n)%(NBPW*NBBY))
#define FD_CLR(n,s)	(s).fds_bits[(n)/(NBPW*NBBY)] &= ~(1<<((n)%(NBPW*NBBY)))
#define FD_ISSET(n,s)	((s).fds_bits[(n)/(NBPW*NBBY)] & (1<<((n)%(NBPW*NBBY))))
#define FD_ZERO(s)	{int i; for(i=0;i<FDWORDS;i++)(s).fds_bits[i]=0; }

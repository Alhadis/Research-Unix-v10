/*
 * addressing stuff for unibus devices
 */

struct ubaddr {
	long uboff;	/* address within unibus space */
	short vec;	/* vector (within unibus) */
	short ubno;	/* unibus number */
};

/*
 * unibus map things
 */

typedef	long	ubm_t;		/* cookie for a piece of unibus map */
typedef	long	uaddr_t;	/* 18-bit unibus or 22-bit q-bus address */

/*
 * pieces of ubm_t;
 * ubmpath is probably the only one
 * useful outside the unibus code
 *
 * bits 0-12	first reg index; enough for 22 bits of addr space (q-bus)
 * bits 13-19	number of map registers; enough for 64kb transfer
 *		number offset by UMSZERO -- alignment page + end sentinel
 * bits 20-23	data path
 */

#define	UMFIRST	0
#define	UMSIZE	13
#define	UMSZERO	2
#define	UMDP	20

#define	ubmfirst(m)	(int)((m)&017777)
#define	ubmsize(m)	(int)((((m)>>UMSIZE)&0177)+UMSZERO)
#define	ubmpath(m)	(int)(((m)>>UMDP)&017)

/*
 * change map m to use path p
 */

#define	ubinspath(p, m)	(((m)&~(017<<UMDP))|((p)<<UMDP))

#define	NOMAP	((-1L)<<23)	/* this is a phony piece of map */

/*
 * map flags
 */

#define	UBDP	01	/* want a bdp if possible */
#define	USLP	02	/* ok to sleep waiting for free map or path */
#define	UPAG	04	/* transfer is page-aligned; saves a map reg */

#ifdef KERNEL
caddr_t ubaddr();
ubm_t ubmalloc();
uaddr_t ubmaddr();
uaddr_t ubmsetmap();
ubm_t ubmbuf(), ubmblk();
uaddr_t ubadbuf(), ubadrptr(), ubadwptr();
#endif

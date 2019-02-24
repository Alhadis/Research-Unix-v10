#define	NBPG		512
#define	PGOFSET		(NBPG-1)	/* byte offset into page */
#define	PGSHIFT		9		/* LOG2(NBPG) */
#define	CLSIZE	2
#define	CLOFSET		(CLSIZE*NBPG-1)	/* for clusters, like PGOFSET */
#define BITFS(dev)	((dev) & 64)
#define	BSIZE(dev)	(BITFS(dev)? 4096: 1024)
#define	BMASK(dev)	(BITFS(dev)? 07777: 01777)
#define	BSHIFT(dev)	(BITFS(dev)? 12: 10)
#define	PAGSIZ	(NBPG*CLSIZE)
#define	PAGRND	((PAGSIZ)-1)

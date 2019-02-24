#define	BADREG	(-1)
#define	MAXREG	0
#define	MINREG	0

/*
 * register offsets for the gnot, gnope
 * saved on the kernel stack from user mode
 * relative to the beginning of the user block
 */

#define	USIZE	8192	/* size of the user block */

#define	PS	(USIZE-2)	/* sr, 2 bytes */
#define	PC	(USIZE-6)
#define	A1	(USIZE-12)
#define	A0	(USIZE-16)
#define	D1	(USIZE-20)
#define	D0	(USIZE-24)
#define	SP	(USIZE-28)

#define	A6	12		/* fp in Label */
#define	D2	20
#define	D3	(D2+4)
#define	D4	(D2+8)
#define	D5	(D2+12)
#define	D6	(D2+16)
#define	D7	(D2+20)
#define	A2	(D2+24)
#define	A3	(A2+4)
#define	A4	(A2+8)
#define	A5	(A2+12)

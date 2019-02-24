/* values for the instruction field */
#define DELTA_TYPE	0300
#define DELTA_MOVE	0200
#define DELTA_ADD	0100

/* number of bytes required to code a value */
#define BASE		256
#define ONE		(BASE)
#define TWO		(BASE*BASE)
#define THREE		(BASE*BASE*BASE)
#define NBYTE(v)	((v) < ONE ? 1 : ((v) < TWO ? 2 : ((v) < THREE ? 3 : 4)))

#define BUFSIZE	2048

#ifndef NULL
#define NULL	(0L)
#endif

#ifndef NIL
#define NIL(type)	((type*)NULL)
#endif

/* function to copy data from one area to another */
#ifdef	SYSV
#define memcopy(to,fr,n)	memcpy(to,fr,n)
#define memzero(s,n)		memset(s,0,n)
#endif

#ifdef BSD
#define memcopy(to,fr,n)	bcopy(fr,to,n)
#define memzero(s,n)		bzero(s,n)
#endif

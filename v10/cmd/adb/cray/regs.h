/*
 * register stuff for the cray x-mp
 */

#define	MAXREG	0
#define	MINREG	0

/*
 * register cookies
 * these are indices in reglist; see reg.c
 * registers work in the obvious sequence;
 * A1 == A0+1 and so on
 */

#define	BADREG	(-1)

#define	PC	(0)
#define	A0	(1)
#define	S0	(1+8)
#define	B00	(1+16)
#define	T00	(1+16+64)

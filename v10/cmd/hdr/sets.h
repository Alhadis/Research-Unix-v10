/*
 *	Set manipulation macros for use with programs preprocessed
 *	by sets(1).
 */
#define	SETS_UNIT_SIZE	8
#define	SETS_SHIFT	3
#define	SETS_MASK	07

#define	in(s, c)	(((s)[((int)c) >> SETS_SHIFT]) & (1 << (((int)c) & SETS_MASK)))
#define	insert(s, c)	(((s)[((int)c) >> SETS_SHIFT]) |= (1 << (((int)c) & SETS_MASK)))
#define	delete(s,c)	(((s)[((int)c) >> SETS_SHIFT]) &= ~(1 << (((int)c) & SETS_MASK)))

typedef	char	*setptr;

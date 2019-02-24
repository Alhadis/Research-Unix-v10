/*
 *	Set manipulation macros for use with programs preprocessed
 *	by sets(1).
 */
#define	in(s, c)	(((s)[((int)c)>>3]) & (1<<(((int)c)&07)))
#define	insert(s, c)	(((s)[((int)c)>>3]) |= (1<<(((int)c)&07)))
#define	delete(s,c)	(((s)[((int)c)>>3]) &= ~(1<<(((int)c)&07)))

typedef	char	*setptr;

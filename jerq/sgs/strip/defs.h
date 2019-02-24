#define	ON	1
#define	OFF	0

#define FATAL	1
#define ERROR	-999L

#define	BSIZE	256
#define MAXSECTIONS	256
#define MAXTEMPENTS	1024

#define	TEMPENT		struct tempent
struct	tempent
{
	long	new;
	long	old;
#if !AR16WR
	TEMPENT	*link;
#endif
};

#define	TEMPSZ		sizeof(TEMPENT)

/*	@(#) defs.h: 1.4 1/19/83	*/

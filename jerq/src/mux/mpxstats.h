#ifdef	MPXSTATS

enum
{
	l0blocked, lblocked, newlayers, nmpxstats
};

#define	L0BLOCKED	(int)l0blocked
#define	LBLOCKED	(int)lblocked
#define	NEWLAYER	(int)newlayers
#define	NMPXSTATS	(int)nmpxstats

struct
{
	char *	descp;
	int	count;
}
	mpxstats[NMPXSTATS] =
{
	 {"channel 0 blocked"}
	,{"channel blocked"}
	,{"layers created"}
};

#undef	MPXSTATS
#define	MPXSTATS(A)	mpxstats[A].count++
#else
#define	MPXSTATS(A)
#endif

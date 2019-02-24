/* PBLKSIZ is the maximum length of a key/record pair */
/* DBLKSIZ * BYTESIZE yeilds # Pblocks filled per Dblock */

/*	tunable size of Pblock in bytes (max is 64K) */
#define	PBLKSIZ	(64L*1024L)
/*	tunable size of Dblock in bytes (min is 512) */
#define	DBLKSIZ	4096L
/*	size of char in bits */
#define	BYTESIZ	8

#ifndef	NULL
#define	NULL	((char *) 0)
#endif	NULL

/* data structures private to DB: */
/* structure of a Pblock: */
typedef struct
{
	union
	{
		unsigned short	P_off[PBLKSIZ / sizeof (unsigned short)];
		char		P_dat[PBLKSIZ];
	}
	p_u;
}
Pblock;
#define	p_off	p_u.P_off
#define	p_dat	p_u.P_dat

/* structure of a Dblock: */
typedef struct
{
	char	d_bits[DBLKSIZ];
}
Dblock;

/* public data types: */
/*	key and record data structure */
typedef	struct
{
	char *	dptr;
	int	dsize;
}
datum;

/*	the state of a DB file */
typedef	struct
{
	/* public data: */
	int		d_flags;	/* flags; see below */
	char *		d_name;		/* name of the database file */
	/* private data: */
	/*	page file info: */
	int		d_pfd;		/* page file descriptor */
	long		d_pblk;		/* block number of page file */
	Pblock *	d_pbuf;		/* buffer for page file */
	/*	direct file info: */
	int		d_dfd;		/* direct file descriptor */
	long		d_dblk;		/* block number of direct file */
	Dblock *	d_dbuf;		/* buffer for direct file */
	/*	hashing info: */
	long		d_hbit;		/* bit number */
	long		d_hmask;	/* hash mask */
	long		d_hnbit;	/* maximum bit # */
}
DBFILE;

/* flag bits in d_flags: */
#define	DB_RONLY	0x0001	/* this database is read only */
#define	DB_SYNC		0x0002	/* sync() on every write (ouch!) */
#define	DB_PMOD		0x0004	/* disk is out of date with respect to d_pbuf */
#define	DB_DMOD		0x0008	/* disk is out of date with respect to d_dbuf */
#define	DB_CREATE	0x0010	/* create database files if they don't exist */
#define	DB_LOCK		0x0020	/* database is locked */
/*	user settable flags: */
#define	DB_USER		(DB_RONLY|DB_SYNC|DB_CREATE)

/* public functions defined in DB: */
extern DBFILE *	DBopen();  /*(char *name, int flags) open a DB file*/
extern void	DBclose(); /*(DBFILE *dp) close a DB file*/
extern void	DBsync();  /*(DBFILE *dp) flush any modified buffers*/
extern datum	DBget();   /*(DBFILE *dp, datum key) get rec at key*/
extern int	DBdel();   /*(DBFILE *dp, datum key) del rec at key*/
extern int	DBput();   /*(DBFILE *dp, datum key, datum rec) put rec at key*/
extern datum	DBkey0();  /*(DBFILE *dp) get lowest key*/
extern datum	DBkeyn();  /*(DBFILE *dp, datum key) get next key*/
extern int	DBlock();  /*(DBFILE *dp) lock a DB file */
extern int	DBunlock();/*(DBFILE *dp) unlock a DB file */

/* public data defined in DB */
extern int	DBdebug;	/* DB debugging level */
#define	DBDBCORE	0	/* errors that produce core files */
#define	DBDBERR		1	/* errors less catastrophic */
#define	DBDBWARN	2	/* various warnings */
#define	DBDBINFO	3	/* entry/exit info for DB*() above */
#define	DBdbprt(level)	if ((level) <= DBdebug) fprintf

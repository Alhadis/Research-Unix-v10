#ifdef TEST
#define NDSZ	64
#else
#define NDSZ 1024/* for real systems (larger may be slower) */
#endif
#define MAXKLEN	(NDSZ/4)-8
#if MAXKLEN > 255
#undef MAXKLEN
#define MAXKLEN 255
#endif
#define MXHT	5
typedef long ndaddr;
/* for communication with users */
typedef struct {
	char *mdata;
	unsigned short mlen;
} mbuf;
typedef struct bfile {
	struct bfile *next;
	struct hdr *path[MXHT + 1];
	char height, advnc, rdwrt, flag[MXHT + 1];
	ndaddr loc[MXHT + 1];
	int tfd, dfd;
	char *fname, *altname;
	struct rdptr {
		struct dkey *rptr;	/* current dkey */
		short rnum;	/* its ordinal */
		char rpref[MAXKLEN];	/* first dcom bytes of its key */
	} rdptr;
	char fatal;			/* this bfile can't be used */
} bfile;
extern bfile *bopen();
extern mbuf bkey();

#define BERROR		100		/* first btree value of errno */
#define BUTRAN		BERROR + 0	/* user caused tran abort */
#define BNOWRITE	BERROR + 1	/* not opened for writing */
#define BIOWRT		BERROR + 2	/* wrote short record */
#define BNOMEM		BERROR + 3	/* no mem from malloc */
#define BFATAL		BERROR + 4	/* last chance for user */
#define BTALL		BERROR + 5	/* tree becoming taller than MXHT */
#define BRDERR		BERROR + 6	/* read short record or read error */
#define BLASTE		BERROR + 7	/* one past last btree value of errno */
extern int	errno;

/* users can ignore the rest of this stuff */
/* keys in nodes */
typedef struct dkey {
	unsigned char dlen;	/* total size of structure */
	char dcom;		/* bytes in common with preceding */
	char dkey[MAXKLEN];	/* rest of key */
} dkey;
#define DKEYSZ	2	/* overhead in dkey */
/* node header */
typedef struct hdr {
	long hstamp;	/* for owning process */
	short kcnt;	/* keys in node */
	char htype;
	char hlev;
} hdr;
typedef struct {
	short tfree;	/* free bytes in node, at end for triv checking */
} trailer;
#define nfree(p)	((trailer *)((char *)(p) + NDSZ - sizeof(trailer)))->tfree
#define SHARED	1
#define INDEX	2
#define READONLY	4
#define bf_type(b, t)	((b)->path[0]->htype & (t))
#define treeonly(b)	bf_type(b, INDEX)
#define shared(b)	bf_type(b, SHARED)
#define readonly(b)	bf_type(b, READONLY)
/* disk addresses */
typedef struct {
	long lloc;
	unsigned short llen;
} lfaddr;
#define ndadr(b, j)	((ndaddr *)((char *)(b) + NDSZ - sizeof(trailer)) - (j) - 1)
#define lfadr(b, j)	((lfaddr *)((char *)(b) + NDSZ - sizeof(trailer)) - (j) - 1)
#define mustwrite(bf, n)	bf->flag[n]	/* for getincore */
/* node format:
 *	hdr, dkey, dkey, dkey, ..., space, adr, adrn, ..., adr0, trailer
 */
extern int	bdump;			/* dump on first fatal error */
extern long	tranid, getlpid();	/* unique transaction id */
#ifndef EOF
#ifndef NULL
#define NULL	0
#endif
#define EOF	-1
#endif
#define NOTFOUND	0
#define FOUND		1

#define alloc(x)	(x *)malloc(sizeof(x))
#define stamped(b)	b->hstamp == tranid
/*1000001111101111*/

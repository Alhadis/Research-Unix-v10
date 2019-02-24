typedef struct {
	short count;
	long bno;
} Bmap;

typedef struct {
	struct header h;	/* this file's file header */
	Bmap lmap;		/* last-used map info */
	long lbase;		/* first block it maps */
	short lmi;		/* index into map */
	unsigned short parent;	/* .. for directories */
	char flags;
	short tboff;		/* last text byte offset */
	long tbno;		/* last text block */
	long tuoff;		/* corresponding unix offset */
} Fsfile;

#define	fsp(f)	((Fsfile *)((f)->fs))

/*
 * flags
 */
#define	FBIN	01		/* non-text file */

long htoutime();
int htouperm();
long hfilesize();
Bmap *getmap(), *getblk();
int gethdr();

#define	plong(l) (l[1]+(l[0]<<16))	/* pdp11 long */

extern int diskfd;
extern struct homeblock home;
extern Fsfile indexf;
extern unsigned short dirtyp;

#define	NFONTS	50

struct pkt
{
	short type;
	short page, nextpage, offset;
	short size;
	char data[256];
	char *next;
};
extern struct pkt pkt;

struct arena
{
	Rectangle layer;		/* congruent to window on virtual page */
	Rectangle page;
	Bitmap *bp;
	Point org;
};
extern struct arena arena;

struct reader
{
	int start1, start2, end;
	unsigned size;
	char *data;
};
extern struct reader reader;

extern int nextpage, page, resolution, dying;
extern int gap;
extern Point cursor, (*pmap)(), plmap(), pdmap();
extern Rectangle rscale(), riscale(), rpage;

#define	S_FONT	0
#define	S_DRAW	1
#define	S_PAGE	2
#define	S_SLICE	3
extern int pstate;
extern char *surealloc();
extern short Mode;

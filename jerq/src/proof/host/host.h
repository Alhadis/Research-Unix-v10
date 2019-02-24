#define	NPAGES	500

struct input
{
	int eof, next;
	unsigned size;
	char *base;
	int maxpage;
	struct
	{
		int start, end;
	} pages[NPAGES];
};
extern struct input input;

extern jerq, going, cursize, curfont, lastpage;
extern char devname[];

#ifdef	DEBUG
extern FILE *debug;
extern int verbose;
#endif	DEBUG

#include	<fio.h>
#include	<ctype.h>
#include	<libc.h>

/*#define	DOSTATS	/* define this to gather stats */
#ifdef	DOSTATS
long nbytes, nlines, ntrans, nfollow, nmaxfoll;
int dostats();
char *statspat, *statsexpr;
#endif

#define BLKSIZE 1024	/* size of reported disk blocks */
#define MAXLIN 10000
#define MAXPOS 20000
#define NCHARS 256
#define NSTATES 128
#define FINAL -1
#define LEFT '\177'	/* serves as ^ */
#define RIGHT '\n'	/* serves as record separator and as $ */

typedef struct State
{
	struct State *gotofn[NCHARS];
	int out;
} State;
extern State states[];
State *nxtst();
extern int state[];
extern int line;
extern int name[];
extern int left[];
extern int right[];
extern int parent[];
extern int foll[];
extern int positions[];
extern char chars[];
extern int nxtpos;
extern int nxtfoll;
extern int inxtpos;
extern int nxtfoll;
extern int nxtchar;
extern int tmpstat[];
extern State *istat;
extern int nstate;
extern int xstate;
extern int count;
extern char *input;
extern char *progname;

extern char reinit;

extern int begout;
extern int begcnt;
extern int begstat[];

extern int colpos[];
extern int cntpos;

extern long	lnum;
extern int	bflag;
extern int	cflag;
extern int	fflag;
extern int	hflag;
extern int	iflag;
extern int	lflag;
extern int	nflag;
extern int	sflag;
extern int	vflag;
extern int	nfile;
extern long	tln;
extern int	nsucc;
extern int	badbotch;

extern int	expfile;

extern int bmegrep;
extern int scanexit;

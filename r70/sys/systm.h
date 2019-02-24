/*
 * Random set of variables
 * used by more than one
 * routine.
 */
int	lbolt;			/* time of day in 60th not in time */
time_t	time;			/* time in sec from 1970 */
char	runin;			/* scheduling flag */
char	runout;			/* scheduling flag */
int	runrun;			/* scheduling flag */
int	noproc;			/* no one is running just now */
char	curpri;			/* more scheduling */
int	maxmem;			/* actual max memory per process */
int	physmem;		/* physical memory on this CPU */
int	wantin;

daddr_t	rablock;		/* block to be read ahead */
dev_t	rootdev;		/* device of the root */
extern dev_t swapdev;		/* swapping device */
dev_t	argdev;			/* device for argument lists */

daddr_t	bmap();
unsigned max();
unsigned min();

/*
 * Structure of the system-entry table
 */
extern struct sysent
{
	int	sy_narg;		/* total number of arguments */
	int	(*sy_call)();		/* handler */
} sysent[];
#define	MAXSYS	128

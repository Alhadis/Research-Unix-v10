/*
 * Structure returned by gettimeofday(2) system call,
 * and used in other calls.
 */

struct timezone
{
	long	tz_minuteswest;	/* minutes west of Greenwich */
	long	tz_dsttime;	/* type of dst correction */
};

#define	DST_NONE	0	/* not on dst */
#define	DST_USA		1	/* USA style dst */
#define	DST_AUST	2	/* Australian style dst */
#define	DST_WET		3	/* Western European dst */
#define	DST_MET		4	/* Middle European dst */
#define	DST_EET		5	/* Eastern European dst */

/*
 * Structure returned by gmtime and localtime calls (see ctime(3)).
 */
typedef
struct tm
{
	int	tm_sec;
	int	tm_min;
	int	tm_hour;
	int	tm_mday;
	int	tm_mon;
	int	tm_year;
	int	tm_wday;
	int	tm_yday;
	int	tm_isdst;
} Tm;

extern	Tm*	gmtime(long);
extern	Tm*	localtime(long);
extern	char*	asctime(Tm*);
extern	char*	ctime(long);

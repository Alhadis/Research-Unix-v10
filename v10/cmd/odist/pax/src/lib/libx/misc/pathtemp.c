/*
 * AT&T Bell Laboratories
 *
 * generate a temp file path name
 *
 *	[<dir>/][<pfx>]<pid><suf>
 *
 * length(<suf>)==3
 * length(<pfx>)<=PREFIX
 * length(<pfx>)+length(<pid>)+length(<suf>)==BASE
 *
 *	tmpnam(p)		pathtemp(p, 0, 0)
 *	tempnam(d, p)		pathtemp(0, d, p)
 *
 * if buf==0 then space is malloc'd
 * otherwise sizeof(buf) must be >= strlen(dir) + BASE + 2
 * dir and pfx may be 0
 * if dir is 0 then sizeof(buf) must be >= TOTAL or it must
 * be a previous pathtemp() return with the same dir and pfx
 * only first PREFIX chars of pfx are used
 */

#define TOTAL	128
#define BASE	14
#define PREFIX	6

#define TMPENV	"TMPDIR"
#define TMP1	"/tmp"
#define TMP2	"/usr/tmp"

#define LOSEED	'a'
#define HISEED	'z'

extern char*	getenv();
extern char*	malloc();

extern int	access();
extern int	strlen();

char*
pathtemp(buf, dir, pfx)
char*		buf;
register char*	dir;
char*		pfx;
{
	register char*	p;
	register char*	s;
	int		pid;

	static char*	tmpdir;
	static char	seed[] = { LOSEED - 1, LOSEED, LOSEED, 0 };

	if ((!dir || *dir && access(dir, 3)) && !(dir = tmpdir))
	{
		if ((!(dir = getenv(TMPENV)) || !*dir || strlen(dir) >= (TOTAL - BASE - 2) || access(dir, 3)) && access(dir = TMP1, 3) && access(dir = TMP2, 3))
			return(0);
		tmpdir = dir;
	}
	if (!(s = buf) && !(s = malloc(strlen(dir) + BASE + 2))) return(0);
	for (p = s; *p = *dir++; p++);
	if (p > s && *(p - 1) != '/') *p++ = '/';
	if (pfx)
	{
		dir = pfx;
		pfx = p;
		while (*dir && p < pfx + PREFIX) *p++ = *dir++;
	}
	else pfx = p;
	pfx = dir = p + (BASE - (p - pfx) - sizeof(seed) + 1);
	pid = getpid();
	while (--dir >= p)
	{
		*dir = pid % 10 + '0';
		pid /= 10;
	}
	p = pfx;
	dir = seed;
	while (*p++ = *dir++);
	do
	{
		for (p = pfx; *p == HISEED; *p++ = LOSEED);
		if (!*p)
		{
			if (!buf) free(s);
			s = 0;
			break;
		}
		++*p;
	} while (!access(s, 0));
	p = seed;
	dir = pfx;
	while (*p++ = *dir++);
	return(s);
}

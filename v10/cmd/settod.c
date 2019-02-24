#include <fio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/clock.h>
#include <sys/timeb.h>
#include <signal.h>
#include <libc.h>

#define	NULL	0

char	*clocks[5] = {
	"dk!nj/astro/clock",
	"dk!nj/mhe/mhpbs.clock",
	NULL
};

char telno[] = "913034944774", class[] = "1200";

#define	UNIXEPOCH	(24*60*60*40587L)	/* MJD of 1 Jan 1970 */

/* buffer to read time into and print it from */
#define	ARB	50
char buf[ARB];

typedef unsigned long tod_t;
tod_t utovax();

time_t readnet(), phonewwv();
long labs();
int sflag, vflag, pflag;

main (argc, argv)
	int argc;
	char **argv;
{
	time_t wwvtime;
	struct timeb ourtime;
	int c;
	extern char *optarg;

	while ((c = getopt (argc, argv, "vspc:")) >= 0) {
		switch (c) {
		case 's':
			sflag++;
			break;
		case 'v':
			vflag++;
			break;
		case 'p':
			pflag++;
			break;
		case 'c':
			clocks[0] = optarg;
			clocks[1] = NULL;
			break;

		case '?':
			exit(1);
		}
	}
	if (!sflag)
		vflag++;
	if ((wwvtime = pflag ? phonewwv() : readnet()) == 0)
		exit(1);
	ftime(&ourtime);
	if (sflag)
		settime(ourtime, wwvtime);
	if (vflag)
		prdiff(ourtime, wwvtime);
	exit(0);
}

/*
 * try biasclock (new); if can't, use settod (old) instead
 */
settime(ourtime, wwvtime)
struct timeb ourtime;
time_t wwvtime;
{
	tod_t oldtod, newtod;
	long delta;

	delta = wwvtime - ourtime.time;
	if (labs(delta) > 100L*3600L) {		/* time too far off, give up */
		print("settod: diff %d seconds, too big\n", delta);
		return;
	}
	delta = delta * 1000L - ourtime.millitm;
	if (delta == 0L)	/* well ... */
		return;
	if (biasclock(delta) >= 0)
		return;
	print("settod: could not do biasclock\n");
#if NOTDEF
	/*
	 * should probably check errno here
	 * someday the rest of this code will vanish
	 */
	newtod = utovax(wwvtime);
	if ((oldtod = settod(0L)) == -1) {
		print("settod: can't read TOY\n");
		return;
	}
	delta = newtod - oldtod;
	/* If delta is too great, maybe TOY year is wrong */
	if (labs(delta) > 100L*86400L*10) {
		time_t tt;
		time(&tt);
		stime(&tt);
		oldtod = settod(0L);
		delta = newtod - oldtod;
	}
	if (labs(delta) > 100L*86400L*10)
		print("settod: delta %d too big: no settod\n", delta);
	else if (settod(newtod) == -1)
		print("settod: could not do settod\n");
#endif
}

prdiff(ourtime, wwvtime)
struct timeb ourtime;
time_t wwvtime;
{
	float delta;
	char *ctime();

	print("%.20s ", ctime(&wwvtime)+4);
	delta = wwvtime - ourtime.time;
	delta -= (float)ourtime.millitm/1000;
	if (delta == 0)
		print("exact\n");
	else if (delta > 0)
		print("advance %.2f\n", delta);
	else
		print("retard %.2f\n", -delta);
}

/*
 * turn a unix time to a VAX TODR time
 */

tod_t
utovax(t)
time_t t;
{
	register int i;

	for (i=1970; ; i++) {
		long secyr;
		secyr = SECYR;
		if (LEAPYEAR(i))
			secyr += SECDAY;
		if (t < secyr)
			break;
		t -= secyr;
	}
	return(t * 100 + TODRZERO);
}

time_t
readnet()
{
	register int i;
	int f;
	int alcatch();

	signal(SIGALRM, alcatch);
	alarm(30);
	for (i=0; clocks[i]; i++) {
		if ((f = ipcopen(ipcpath(clocks[i], "dk", ""), "light")) >= 0)
			break;
	}
	if (f < 0) {
		alarm(0);
		print("settod: can't open clock\n");
		return(0);
	}
	alarm(15);
	do {
		if (read(f, buf, 1) <= 0) {
			print("settod: read error\n");
			alarm(0);
			close(f);
			return (0);
		}
		buf[0] &= 0177;
	} while (buf[0]!='\r' && buf[0]!='\n');
	for (i=0; i < sizeof(buf); i++) {
		if (read(f, &buf[i], 1) <= 0) {
			print("settod: read error\n");
			alarm(0);
			close(f);
			return (0);
		}
		buf[i] &= 0177;
		if (buf[i] == '\n' || buf[i] == '\r') {
			if (i)
				break;
			i--;		/* ignore empty line */
		}
	}
	alarm(0);
	close(f);
	if (i < 10 || i == sizeof(buf)) {	/* plausibility check */
		print("settod: clock format error: %s\n", buf);
		return (0);
	}
	buf[i] = 0;		/* just to make sure */
	return (atol(buf));
}

time_t
phonewwv()
{
	register fd;
	register char *l;
	int alcatch();

	signal(SIGALRM, alcatch);
	alarm(60);
	fd = dialout(telno, class);
	if (fd < 0) {
		print("settod: dialout(\"%s\", \"%s\") returned %d\n",
			telno, class, fd);
		return 0;
	}
	Finit(fd, (char *)0);
	while (l = Frdline(fd)) {
		if (vflag > 1)
			print("%s\n", l);
		if (FIOLINELEN(fd) == 51 && strcmp(&l[38], " UTC(NIST) *\r") == 0)
			break;
	}
	if (l == 0) {
		print("settod: wwv read error\n");
		return 0;
	}
	alarm(0);
	return atol(&l[21]) +
		60*(atol(&l[18]) +
			60*(atol(&l[15]) +
				24*atol(&l[0])))-UNIXEPOCH;
}

alcatch() {}

long
labs(n)
	long n;
{
	if (n >= 0)
		return n;
	return -n;
}

#if NOTDEF

/*
 * old heathkit conversion code
 * not guaranteed even to compile
 *
 * heathkit dates look like
 * HH:MM:SS.S     DD/MM/YY
 * with several irritating quirks
 */

static	char	dmsize[12] =
{
	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

tod_t
hconv()
{
	int year, mon;
	int day, hour, mins, secs, tenth;
	tod_t timerr;

	/* accept '?' for tenths */
	if (buf[9]=='?')
		buf[9] = '0';
	if (sscanf(buf, "%d:%d:%d.%d    %d/%d/%d\r", &hour, &mins, &secs,
	   &tenth, &mon, &day, &year) != 7)
		goto formerr;
	if( mon<1 || mon>12 ||
	    day<0 || day>31 ||	/* June 30, 1984 == July 0, 1984 !!?? */
	    mins<0 || mins>59 ||
	    secs<0 || secs>59 ||
	    tenth<0 || tenth>9 ||
	    hour<0 || hour>23)
		goto formerr;
	/* leap year */
	timbuf = 0;
	if (year % 4 == 0 && mon >= 3)
		timbuf++;
	while(--mon)
		timbuf += dmsize[mon-1];
	timbuf += day-1;
	timbuf = 24*timbuf + hour;
	timbuf = 60*timbuf + mins;
	timbuf = 60*timbuf + secs;
	timbuf = 10*timbuf + tenth;
	timbuf = timbuf * 10 + TODRZERO;
	return timbuf;
formerr:
	print("settod: clock format error: %.24s\n", buf);
	return(0);
}

#endif

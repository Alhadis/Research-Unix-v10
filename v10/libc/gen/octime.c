/*
 * This routine converts time as follows.
 * The epoch is 0000 Jan 1 1970 GMT.
 * The argument time is in seconds since then.
 * The localtime(t) entry returns a pointer to an array
 * containing
 *  seconds (0-59)
 *  minutes (0-59)
 *  hours (0-23)
 *  day of month (1-31)
 *  month (0-11)
 *  year-1970
 *  weekday (0-6, Sun is 0)
 *  day of the year
 *  daylight savings flag
 *
 * The routine calls the system to determine the local
 * timezone and whether Daylight Saving Time is permitted locally.
 * (DST is then determined by the current US standard rules)
 * There is a table that accounts for the peculiarities
 * undergone by daylight time in 1974-1975.
 *
 * The routine does not work
 * in Saudi Arabia which runs on Solar time.
 *
 * asctime(tvec))
 * where tvec is produced by localtime
 * returns a ptr to a character string
 * that has the ascii time in the form
 *	Thu Jan 01 00:00:00 1970n0\\
 *	01234567890123456789012345
 *	0	  1	    2
 *
 * ctime(t) just calls localtime, then asctime.
 */

#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>

static	char	cbuf[26];
static	int	dmsize[12] =
{
	31,
	28,
	31,
	30,
	31,
	30,
	31,
	31,
	30,
	31,
	30,
	31
};

#define	NDAYTAB	10

static int ndaytab;
static struct daytab {
	unsigned char ybeg;		/* year-1900 */
	unsigned char yend;
	unsigned char obeg;		/* offset from sunday */
	unsigned char oend;
	short dbeg;
	short dend;
} daytab[NDAYTAB];

struct tm	*gmtime();
char		*ct_numb();
struct tm	*localtime();
char	*ctime();
char	*ct_num();
char	*asctime();
int	dysize();
static	char *gint();
static	dtparse();

char *
ctime(t)
long *t;
{
	return(asctime(localtime(t)));
}

struct tm *
localtime(tim)
long *tim;
{
	register int dayno;
	register struct tm *ct;
	register daylbegin, daylend;
	int boff, eoff;
	register int i;
	register int incr;
	register int tmp;
	long copyt;
	struct timeb systime;

	ftime(&systime);
	copyt = *tim - (long)systime.timezone*60;
	ct = gmtime(&copyt);
	if (systime.dstflag == 0)
		return (ct);
	dayno = ct->tm_yday;
	daylbegin = 999;	/* default: no dst */
	daylend = 999;
	incr = 1*60*60;
	boff = eoff = 0;
	rdaytab();
	for (i = 0; i < ndaytab; i++)
		if (daytab[i].ybeg <= ct->tm_year && ct->tm_year <= daytab[i].yend) {
			daylbegin = daytab[i].dbeg;
			daylend = daytab[i].dend;
			boff = daytab[i].obeg;
			eoff = daytab[i].oend;
			break;
		}
	if (daylbegin > daylend) {
		tmp = daylbegin;
		daylbegin = daylend;
		daylend = tmp;
		copyt += incr;
		incr = -incr;
	}
	daylbegin = sunday(ct, daylbegin)+boff;
	daylend = sunday(ct, daylend)+eoff;
	if ((dayno>daylbegin || (dayno==daylbegin && ct->tm_hour>=2)) &&
	    (dayno<daylend || (dayno==daylend && ct->tm_hour<1))) {
		copyt += incr;
		ct = gmtime(&copyt);
		ct->tm_isdst++;
	}
	return(ct);
}

/*
 * The argument is a 0-origin day number.
 * The value is the day number of the last
 * Sunday before or after the day.
 */
static
sunday(t, d)
register struct tm *t;
register int d;
{
	if (d >= 58)
		d += dysize(t->tm_year) - 365;
	return(d - (d - t->tm_yday + t->tm_wday + 700) % 7);
}

struct tm *
gmtime(tim)
long *tim;
{
	register int d0, d1;
	long hms, day;
	register int *tp;
	static struct tm xtime;

	/*
	 * break initial number into days
	 */
	hms = *tim % 86400;
	day = *tim / 86400;
	if (hms<0) {
		hms += 86400;
		day -= 1;
	}
	tp = (int *)&xtime;

	/*
	 * generate hours:minutes:seconds
	 */
	*tp++ = hms%60;
	d1 = hms/60;
	*tp++ = d1%60;
	d1 /= 60;
	*tp++ = d1;

	/*
	 * day is the day number.
	 * generate day of the week.
	 * The addend is 4 mod 7 (1/1/1970 was Thursday)
	 */

	xtime.tm_wday = (day+7340036)%7;

	/*
	 * year number
	 */
	/* in 400 years there are 97*366+303*365=146097 days (40 bits and up)*/
	if(day>=0) {
		for(d1 = 70; day >= 146097; d1 += 400)
			day -= 146097;
		for(; day >= dysize(d1); d1++)
			day -= dysize(d1);
	}
	else {
		for(d1 = 70; day <= -146097; d1 -= 400)
			day += 146097;
		for(; day < 0; d1--)
			day += dysize(d1-1);	/* ! */
	}
	xtime.tm_year = d1;
	xtime.tm_yday = d0 = day;

	/*
	 * generate month
	 */

	if (dysize(d1)==366)
		dmsize[1] = 29;
	for(d1=0; d0 >= dmsize[d1]; d1++)
		d0 -= dmsize[d1];
	dmsize[1] = 28;
	*tp++ = d0+1;
	*tp++ = d1;
	xtime.tm_isdst = 0;
	return(&xtime);
}

char *
asctime(t)
struct tm *t;
{
	register char *cp, *ncp;
	register int *tp;

	cp = cbuf;
	for (ncp = "Day Mon 00 00:00:00 1900\n"; *cp++ = *ncp++;);
	ncp = &"SunMonTueWedThuFriSat"[3*t->tm_wday];
	cp = cbuf;
	*cp++ = *ncp++;
	*cp++ = *ncp++;
	*cp++ = *ncp++;
	cp++;
	tp = &t->tm_mon;
	ncp = &"JanFebMarAprMayJunJulAugSepOctNovDec"[(*tp)*3];
	*cp++ = *ncp++;
	*cp++ = *ncp++;
	*cp++ = *ncp++;
	cp = ct_numb(cp, *--tp);
	cp = ct_numb(cp, *--tp+100);
	cp = ct_numb(cp, *--tp+100);
	cp = ct_numb(cp, *--tp+100);
	if (t->tm_year>=100) {
		cp[1] = '2';
		cp[2] = '0';
	}
	cp += 2;
	cp = ct_numb(cp, t->tm_year+100);
	return(cbuf);
}

dysize(y)
{
	if((y%4) == 0)
		return(366);
	return(365);
}

static char *
ct_numb(cp, n)
register char *cp;
{
	cp++;
	if (n>=10)
		*cp++ = (n/10)%10 + '0';
	else
		*cp++ = ' ';
	*cp++ = n%10 + '0';
	return(cp);
}

#include <ctype.h>

/*
 * read the file of daylight savings time boundaries
 * it looks like
 *	1971	1983	32 0	317 1
 * to mean that for years 1971-1983, dst starts the first sunday after
 * day 32, and ends the first monday after day 317
 */
#define	BUFSZ	1024
#define	DSTFILE	"/lib/dst"

static
rdaytab()
{
	static int doneit;
	int fd;
	char buf[BUFSZ];
	char *p;
	register int i;
	char *rdline();

	if (doneit++)
		return;
	if ((fd = open(DSTFILE, 0)) < 0)
		return;
	i = 0;
	while ((p = rdline(fd, buf)) != 0)
		if (dtparse(p, &daytab[i]))
			if (++i >= NDAYTAB)
				break;
	ndaytab = i;
	close(fd);
}

static
dtparse(p, dp)
register char *p;
register struct daytab *dp;
{
	short s;

	if ((p = gint(p, &s)) == 0)
		return (0);
	if (s < 1900)
		dp->ybeg = 0;
	else if (s > (1900+255))
		dp->ybeg = 255;
	else
		dp->ybeg = s - 1900;
	if ((p = gint(p, &s)) == 0)
		return (0);
	if (s < 1900)
		dp->yend = 0;
	else if (s > (1900+255))
		dp->yend = 255;
	else
		dp->yend = s - 1900;
	if ((p = gint(p, &dp->dbeg)) == 0)
		return (0);
	if ((p = gint(p, &s)) == 0)
		return (0);
	dp->obeg = s;
	if ((p = gint(p, &dp->dend)) == 0)
		return (0);
	if ((p = gint(p, &s)) == 0)
		return (0);
	dp->oend = s;
	return (1);
}

static char *
gint(p, sp)
register char *p;
register short *sp;
{
	register int i;

	while (isspace(*p))
		p++;
	if (!isdigit(*p))
		return (0);
	i = 0;
	while (isdigit(*p))
		i = (i*10) + *p++ - '0';
	*sp = i;
	return (p);
}

static char *
rdline(fd, buf)
int fd;
char *buf;
{
	register char *p, *q;
	static char *lastp;
	static char *endp;
	int n;

	if (lastp == 0 || lastp >= endp) {
		if ((n = read(fd, buf, BUFSZ)) <= 0)
			return (0);
		lastp = buf;
		endp = &buf[n];
	}
	for (p = lastp; p < endp && *p != '\n'; p++)
		;
	if (p < endp || lastp == buf) {
		*p++ = 0;
		q = lastp;
		lastp = p;
		return (q);
	}
	/*
	 * unfinished line at end of buffer.  try again.
	 */
	q = lastp;
	p = buf;
	while (q < endp)
		*p++ = *q++;
	if ((n = read(fd, p, BUFSZ - (p-buf))) <= 0)
		return (0);
	endp = &p[n];
	for (; p < endp && *p != '\n'; p++)
		;
	if (p >= endp)
		--p;	/* too bad */
	*p++ = 0;
	lastp = p;
	return (buf);
}

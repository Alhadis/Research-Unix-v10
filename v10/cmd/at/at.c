/*
 * at time mon day
 * at time wday
 * at time wday 'week'
 *
 */
#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dir.h>

#define HOUR 100
#define HALFDAY	(12*HOUR)
#define DAY	(24*HOUR)
#define THISDAY "/usr/spool/at"

char *days[] = {
	"sunday",
	"monday",
	"tuesday",
	"wednesday",
	"thursday",
	"friday",
	"saturday",
};

struct monstr {
	char *mname; 
	int mlen;
} months[] = {
	{ "january", 31 },
	{ "february", 28 },
	{ "march", 31 },
	{ "april", 30 },
	{ "may", 31 },
	{ "june", 30 },
	{ "july", 31 },
	{ "august", 31 },
	{ "september", 30 },
	{ "october", 31 },
	{ "november", 30 },
	{ "december", 31 },
	{ 0, 0 },
};

char	fname[100];
int	utime;  /* requested time in grains */
int	now;	/* when is it */
int	uday; /* day of year to be done */
int	uyear; /* year */
int	today; /* day of year today */
int	thisyear; /* this year */
FILE	*file;
FILE	*ifile;
char	**environ;
char	*prefix();
char	*idend();
char	*strchr();
FILE	*popen();

extern int optind;
char	lflg;
char	rflg;
char	errflg;

void	prdir(), prfile(), delfile(), vprint();
int	readdir();
char *pathdate();

#define EXP 10

main(argc, argv)
char **argv;
{
	extern onintr();
	register c;
	char pwbuf[100];
	FILE *pwfil;
	int larg;
	static char stdbuf[BUFSIZ];

	setbuf (stdout, stdbuf);

	while ((c = getopt (argc, argv, "rl")) != EOF) {
		switch (c) {

		case 'l':
			lflg = 1;
			break;
		
		case 'r':
			rflg = 1;
			break;
		
		default:
			errflg = 1;
			break;
		}
	}

	if (errflg) {
		fprintf (stderr, "usage: at [lr] time [filename]\n");
		exit (1);
	}

	/* argv[optind] is the user's time: e.g.,  3AM */
	/* argv[optind+1] is a month name or day of week */
	/* argv[optind+2] is day of month or 'week' */
	/* another argument might be an input file */
	if (argc < optind+1) {
		if (lflg && !rflg) {
			(void) readdir("", prdir);
			exit (0);
		}
		fprintf(stderr, "at: arg count\n");
		exit(1);
	}
	makeutime(argv[optind]);
	larg = makeuday(argc-optind+1,argv+optind-1)+optind;
	if (uday==today && larg<=optind+1 && utime<=now)
		uday++;
	c = uyear%4==0? 366: 365;
	/*
	The way makeuday is written, the following "if" is rarely
	satisfied. This makes a problem, which is fixed below.
	*/
	if (uday >= c) {
		uday -= c;
		uyear++;
	}
	/* 
	Due to problem described above, "at" schedules a Jan call in
	Dec for LAST Jan.  Line below fixes that.
	*/
	if(uday<today && uyear==thisyear)
		uyear++;

	if (lflg || rflg) {
		char prefix[50];
		sprintf (prefix, "%.2d.%.3d.%.4d", uyear, uday, utime);
		if (lflg) {
			if (readdir (prefix, prfile) == 0)
				exit (0);
			if (rflg)
				putchar ('\n');
		}

		if (rflg) {
			(void) readdir (prefix, delfile);
		}

		exit (0);
	}

	filename(THISDAY, uyear, uday, utime);
	/* Create file, then change UIDS */
	close(creat(fname, 0644));
	file = fopen(fname, "w");
	if (file == NULL) {
		fprintf(stderr, "at: cannot open memo file\n");
		exit(1);
	}
	chown(fname, getuid(), getgid());
	setuid(getuid());
	ifile = stdin;
	if (argc > larg)
		ifile = fopen(argv[larg], "r");
	if (ifile == NULL) {
		fprintf(stderr, "at: cannot open input: %s\n", argv[larg]);
		exit(1);
	}
	if (signal(SIGINT, SIG_IGN) != SIG_IGN)
		signal(SIGINT, onintr);
	if ((pwfil = popen("pwd", "r")) == NULL) {
		fprintf(stderr, "at: can't execute pwd\n");
		exit(1);
	}
	fgets(pwbuf, 100, pwfil);
	pclose(pwfil);
	fprintf(file, "cd %s", pwbuf);
	c = umask(0);
	umask(c);
	fprintf(file, "umask %#o\n", c);
	if (environ) {
		register int pass;
		char *explist[EXP];
		int explen[EXP];
		register int exp = 0;
		register int i;
		for (pass = 1; pass <= 2; pass++) {
			register char **ep;
			for (ep = environ; *ep; ep++) {
				if (pass == 1) {
					vprint(file, *ep);
					putc('\n', file);
				} else {
					char *ix;
					ix = idend(*ep);
					if (*ix) {
						explen[exp] = ix-*ep;
						explist[exp++] = *ep;
						if (exp >= EXP) {
							fprintf(file,"export");
							for (i=0; i<EXP; i++) {
								putc(' ',file);
								fprintf(file, "%.*s",explen[i],explist[i]);
							}
							putc ('\n', file);
							exp = 0;
						}
					}
				}
			}

			if (exp > 0) {
				fprintf(file,"export");
				for (i = 0; i < exp; i++) {
					putc(' ',file);
					fprintf(file, "%.*s",explen[i],explist[i]);				}
				putc ('\n', file);
			}
		}
	}
	while((c = getc(ifile)) != EOF) {
		putc(c, file);
	}
	exit(0);
}

makeutime(pp)
char *pp; 
{
	register val;
	register char *p;

	/* p points to a user time */
	p = pp;
	val = 0;
	while(isdigit(*p)) {
		val = val*10+(*p++ -'0');
	}
	if (p-pp < 3)
		val *= HOUR;

	for (;;) {
		switch(*p) {

		case ':':
			++p;
			if (isdigit(*p)) {
				if (isdigit(p[1])) {
					val +=(10* *p + p[1] - 11*'0');
					p += 2;
					continue;
				}
			}
			fprintf(stderr, "at: bad time format:\n");
			exit(1);

		case 'A':
		case 'a':
			if (val >= HALFDAY+HOUR)
				val = DAY+1;  /* illegal */
			if (val >= HALFDAY && val <(HALFDAY+HOUR))
				val -= HALFDAY;
			break;

		case 'P':
		case 'p':
			if (val >= HALFDAY+HOUR)
				val = DAY+1;  /* illegal */
			if (val < HALFDAY)
				val += HALFDAY;
			break;

		case 'n':
		case 'N':
			val = HALFDAY;
			break;

		case 'M':
		case 'm':
			val = 0;
			break;


		case '\0':
		case ' ':
			/* 24 hour time */
			if (val == DAY)
				val -= DAY;
			break;

		default:
			fprintf(stderr, "at: bad time format\n");
			exit(1);

		}
		break;
	}
	if (val < 0 || val >= DAY) {
		fprintf(stderr, "at: time out of range\n");
		exit(1);
	}
	if (val%HOUR >= 60) {
		fprintf(stderr, "at: illegal minute field\n");
		exit(1);
	}
	utime = val;
}


makeuday(argc,argv)
char **argv;
{
	/* the presumption is that argv[2], argv[3] are either
	   month day OR weekday [week].  Returns 2, 3, or 4 as last
	   argument used */
	/* first of all, what's today */
	long tm;
	int found = -1;
	char **ps;
	struct tm *detail, *localtime();
	struct monstr *pt;

	time(&tm);
	detail = localtime(&tm);
	uday = today = detail->tm_yday;
	thisyear = uyear = detail->tm_year;
	now = detail->tm_hour*100+detail->tm_min;
	if (argc<=2)
		return(1);
	/* is the next argument a month name ? */
	for (pt=months; pt->mname; pt++) {
		if (prefix(argv[2], pt->mname)) {
			if (found<0)
				found = pt-months;
			else {
				fprintf(stderr, "at: ambiguous month\n");
				exit(1);
			}
		}
	}
	if (found>=0) {
		if (argc<=3)
			return(2);
		uday = atoi(argv[3]) - 1;
		if (uday<0) {
			fprintf(stderr, "at: illegal day\n");
			exit(1);
		}
		if (found > 1 && detail->tm_year%4==0)
			uday++; /* because next loop will include Feb */
		while(--found>=0)
			uday += months[found].mlen;
		/* this is bogus...
		if (detail->tm_year%4==0 && uday>59)
			uday += 1;
		*/
		if(argc>4) {
			int t = atoi(argv[4])-1900;
			if(t<100 && (t>uyear || t==uyear && (
			   uday>today || uday==today &&
			   utime>now))) {
				uyear = t;
				return(4);
			}
		}
		if (uday == today && utime < now)
		{
			fprintf(stderr, "at: Warning: time specified is past; assuming next year....\n");
			++uyear;
		}
		return(3);
	}
	/* not a month, try day of week */
	found = -1;
	for (ps=days; ps<days+7; ps++) {
		if (prefix(argv[2], *ps)) {
			if (found<0)
				found = ps-days;
			else {
				fprintf(stderr, "at: ambiguous day of week\n");
				exit(1);
			}
		}
	}
	if (found<0)
		return(1);
	/* find next day of this sort */
	uday = found - detail->tm_wday;
	if (uday<=0)
		uday += 7;
	uday += today;
	/* Bogus...
	if (detail->tm_year%4==0 && uday>59)
		uday += 1;
	*/
	if (argc>3 && strcmp("week", argv[3])==0) {
		uday += 7;
		return(3);
	}
	return(2);
}

char *
prefix(begin, full)
char *begin, *full;
{
	int c;
	while (c = *begin++) {
		if (isupper(c))
			c = tolower(c);
		if (*full != c)
			return(0);
		else
			full++;
	}
	return(full);
}

filename(dir, y, d, t)
char *dir;
{
	register i;

	for (i=0; ; i += 53) {
		sprintf(fname, "%s/%.2d.%.3d.%.4d.%.2d", dir, y, d, t,
		   (getpid()+i)%100);
		if (access(fname, 0) == -1)
			return;
	}
}

onintr()
{
	unlink(fname);
	exit(1);
}

/* hand "fn" all path names in spool directory starting with "prefix" */
int
readdir(prefix, fn)
	char *prefix;
	void (*fn)();
{
	struct dir dbuf;
	register FILE *f;
	register int count = 0;

	f = fopen (THISDAY, "r");
	if (f == NULL) {
		fprintf (stderr, "cannot open %s\n", THISDAY);
		return;
	}

	while (fread (&dbuf, sizeof dbuf, 1, f) == 1) {
		if (dbuf.d_ino != 0 && dbuf.d_name[0] != '.'
		  && strncmp (dbuf.d_name, prefix, strlen(prefix)) == 0) {
			char file[sizeof (THISDAY) + DIRSIZ + 1];
			struct stat sb;
			strcpy (file, THISDAY);
			strcat (file, "/");
			strncat (file, dbuf.d_name, DIRSIZ);
			if (stat (file, &sb) >= 0 && sb.st_uid == getuid()) {
				count++;
				(*fn) (file);
			}
		}
	}

	fclose (f);

	if (count == 0)
		fprintf (stderr, "no items\n");
}

/*
 *	return pointer to (static) area containing date and time
 *	for activation of file, or argument if invalid format.
 */
char *
pathdate (path)
	char *path;
{
	int month, mday, yday, year, tm, junk;
	register int i;
	register char *p, *q;
	static char res[50];

	/* point p at last component of path name */
	p = q = path;
	while (*q)
		if (*q++ == '/')
			p = q;
	
	/* break out the fields */
	if (sscanf(p, "%2d.%3d.%4d.%2d", &year, &yday, &tm, &junk) != 4)
		return path;
	
	/* determine the month and day of month */
	month = 0;
	mday = yday + 1;
	while (months[month].mlen && mday > months[month].mlen) {
		mday -= months[month].mlen;
		month++;
	}

	/* check for impossible date */
	if (++month > 12)
		return path;

	/* check for leap year */
	if (month > 2 && year % 4 == 0 && --mday <= 0) {
		mday = months[--month - 1].mlen;
		if (month == 2)
			mday++;
	}

	/* build the result */
	sprintf (res, "%.4d %s %d 19%d",
		tm, months[month-1].mname, mday, year);
	return res;
}

void
prdir (path)
	register char *path;
{
	printf ("%s\n", pathdate (path));
}

void
prfile (path)
	register char *path;
{
	register FILE *f;

	printf ("\n%s:", pathdate (path));
	f = fopen (path, "r");
	if (f == NULL) {
		fprintf (stderr, " cannot open %s\n", path);
	} else {
		register int c;
		printf ("\n\n");
		while ((c = getc(f)) != EOF)
			putchar (c);
	}
	fclose (f);
}

void
delfile(path)
	register char *path;
{
	if (unlink(path) < 0)
		fprintf (stderr, "cannot remove %s\n", path);
	else
		fprintf (stderr, "remove %s\n", pathdate(path));
}
char *
idend(s)
	char *s;
{
	while(*s && *s!='=' && *s!='(')
		s++;
	return s;
}
/* print a shell variable with quoting */
void
vprint (f, str)
	register FILE *f;
	register char *str;
{
	if(*idend(str)=='(')	/* function; the shell's already quoted it right */
		fputs(str, f);
	else
		while (*str) {
			if (strchr (" \t\b\n\f$\\[]*?'\"`&|#()^<>;", *str))
				putc ('\\', f);
			putc (*str++, f);
		}
}

/*
 * get date from wwv network clock
 */
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <utmp.h>
#include <signal.h>

#define	ARB	50
char buf[ARB];

int	uflag;
int	sflag;
int	bflag;
int	fflag;
char	*timezone();

char	*clocks[5] = {
	"dk!nj/mhe/mhpbs.clock",
	"dk!nj/astro/clock",
	NULL
};

struct utmp wtmp[2] = { {"|", "", 0}, {"{", "", 0}};

char	*ctime();
char	*asctime();
struct	tm *localtime();
struct	tm *gmtime();
long	labs();

main(argc, argv)
char *argv[];
{
	int rc;
	time_t wwvtime, readnet();
	extern char *optarg;

	rc = 0;
	for(;;) {
		switch(getopt(argc, argv, "usbfc:")) {
		case 's':
			sflag++;
			continue;

		case 'u':
			uflag++;
			continue;

		case 'b':
			bflag++;
			continue;

		case 'f':
			fflag++;
			continue;

		case 'c':
			clocks[0] = optarg;
			clocks[1] = NULL;
			continue;

		case '?':
			exit(1);

		case EOF:
			goto OK;
		}
	}
OK:
	wwvtime = readnet();
	if (wwvtime==0)
		exit(1);
	if (sflag)
		rc = settime(wwvtime);
	if (bflag) {
		printf("WWV: "); prt(wwvtime);
		printf("you: "); prt(time((time_t)0));
	} else
		prt(wwvtime);
	exit(rc);
}

prt(t)
time_t t;
{
	struct timeb info;
	char *ap, *tzn;

	if (uflag) {
		ap = asctime(gmtime(&t));
		tzn = "GMT";
	} else {
		struct tm *tp;
		ftime(&info.time);
		tp = localtime(&t);
		ap = asctime(tp);
		tzn = timezone(info.timezone, tp->tm_isdst);
	}
	printf("%.20s", ap);
	if (tzn)
		printf("%s", tzn);
	printf("%s", ap+19);
}

/*
 * set the time
 */

settime(wwvtime)
time_t wwvtime;
{
	time_t nowtime = time((time_t)0);
	int wf;

	if (fflag == 0 && labs(nowtime-wwvtime) >= 20*60) {
		fprintf(stderr, "wwv: >20min difference; force with  wwv -sf\n");
		bflag++;
		return (1);
	}
	wtmp[0].ut_time = nowtime;
	if(stime(&wwvtime) < 0) {
		fprintf(stderr, "wwv: no permission\n");
		return (1);
	}
	if (wwvtime >= nowtime)
		printf("advanced %ld sec\n", wwvtime-nowtime);
	else
		printf("retarded %ld sec\n", nowtime-wwvtime);
	if ((wf = open("/usr/adm/wtmp", 1)) >= 0) {
		time(&wtmp[1].ut_time);
		lseek(wf, 0L, 2);
		write(wf, (char *)wtmp, sizeof(wtmp));
		close(wf);
	}
	return (0);
}

/*
 * read time from the network
 * takes only the modern version;
 * see settod.c if the heath code is ever needed again
 */

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
		fprintf(stderr, "wwv: can't open clock\n");
		return(0);
	}
	do {
		if (read(f, buf, 1) <= 0) {
			fprintf(stderr, "wwv: read error\n");
			alarm(0);
			close(f);
			return (0);
		}
		buf[0] &= 0177;
	} while (buf[0]!='\r' && buf[0]!='\n');
	for (i=0; i < sizeof(buf); i++) {
		if (read(f, &buf[i], 1) <= 0) {
			fprintf(stderr, "wwv: read error\n");
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
		fprintf(stderr, "wwv: clock format error: %s\n", buf);
		return (0);
	}
	buf[i] = 0;		/* just to make sure */
	return (atol(buf));
}

alcatch() {}

long
labs(t)
long t;
{
	if (t < 0)
		return(-t);
	return(t);
}

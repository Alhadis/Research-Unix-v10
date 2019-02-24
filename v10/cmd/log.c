#include <stdio.h>
#include <utmp.h>

struct utmp u, ulist[100], *ufreep = &ulist[0];

int	boot = 0;	/* set to 1 when a boot is encountered */

long	t_final;	/* end of a given period of up time */
long	t_old, t_new;	/* login session start and stop times */

FILE	*ufp;	/* file pointer for wtmp file */

int	argc;
char	**argv;

char	*WTMP = "/usr/adm/wtmp";

struct {
	char *flag;
	char *title;
} spcl[] = {
	"~", "Reboot",
	"|", "Old time",
	"{", "New time",
	NULL, NULL,
};

struct utmp *lookup();
char	*ctime(), *strcpy();

main(ac, av)
int ac;
char **av;
{
	argc = ac-1;
	argv = av+1;
	time(&t_final);
	if ((ufp = fopen(WTMP, "r")) == NULL) {
		fprintf(stderr, "log: cannot open %s\n", WTMP);
		exit(1);
	}
	fseek(ufp, 0L, 2);
	uscan();
	fclose(ufp);
	exit(0);
}

/*
 * display info for current ufp
 */
uscan()
{
	register char *q;
	register int i;

	while (revread()) {
		if (strcmp(u.ut_line, "~") == 0) {
			boot = 1;
			ufreep = &ulist[0];
			t_final = u.ut_time;
		}
		if (strcmp(u.ut_name, "") == 0)
			*lookup() = u;
		if (!okay())
			continue;
		t_old = u.ut_time;
		q = ctime(&t_old);
		if (strcmp(u.ut_name, "") == 0) {
			for (i = 0; spcl[i].flag; i++)
				if (strcmp(spcl[i].flag, u.ut_line) == 0)
					printf("%s: %s", spcl[i].title, q);
			continue;
		} else {
			printf("%-8.8s%-8.8s%16.16s - ",
				u.ut_name, u.ut_line, q);
			t_new = lookup()->ut_time;
			if (t_new == 0) {
				t_new = t_final;
				if (boot)
					printf("boot ");
				else
					printf(" now ");
			}
			else
				printf("%5.5s", 11+ctime(&t_new));
			printf(" ("); prelapse(t_new-t_old); printf(")\n");
		}
	}
}

/*
 * print elapsed time
 */

#define	MINUTE	(60L)
#define	HOUR	(60L * MINUTE)
#define	DAY	(24L * HOUR)

prelapse(t)
	long t;
{
	register int days, hours, minutes;

	days = t/DAY;
	t %= DAY;
	hours = t/HOUR;
	t %= HOUR;
	minutes = t/MINUTE;
	if (days)
		printf("%d+", days);
	printf("%d:%02d", hours, minutes);
}

/*
 * locate tty line entry in ulist; make one if necessary
 */
struct utmp *
lookup()
{
	register struct utmp *p;

	for (p = &ulist[0]; p < ufreep; p++)
		if (strcmp(p->ut_line, u.ut_line) == 0)
			return(p);
	if (p >= &ulist[sizeof(ulist)]) {
		fprintf(stderr, "log: ulist overflow\n");
		exit(1);
	}
	ufreep++;
	strcpy(p->ut_name, "");
	strcpy(p->ut_line, "");
	p->ut_time = 0L;
	return(p);
}

/*
 * return 1 if this entry should be printed, else 0
 */
okay()
{
	register int count;
	register char **pp;

	if (argc <= 0)
		return(1);
	for (count=argc, pp=argv; count>0; --count, pp++) {
		if (strcmp(*pp, u.ut_line) == 0)
			return(1);
		if (strcmp(*pp, u.ut_name) == 0)
			return(1);
	}
	return(0);
}

/*
 * buffered reverse read of next utmp entry.
 */
revread()
{
	static struct utmp ubuf[100];
	static int nitems = 0;
	long nbytes;

	if (nitems <= 0) {
		fflush(stdout);
		nbytes = ftell(ufp);
		if (nbytes > sizeof(ubuf))
			nbytes = sizeof(ubuf);
		nitems = nbytes / sizeof(ubuf[0]);
		if (fseek(ufp, -nbytes, 1) == -1)
			return(0);
		if (fread(&ubuf[0], sizeof(ubuf[0]), nitems, ufp) != nitems)
			return(0);
		if (fseek(ufp, -nbytes, 1) == -1)
			return(0);
	}
	if (--nitems < 0)
		return(0);
	u = ubuf[nitems];
	return(1);
}

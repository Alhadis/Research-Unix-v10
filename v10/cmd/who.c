/*
 * who
 */

#include <stdio.h>
#include <utmp.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
struct utmp utmp;
struct passwd *pw;
struct passwd *getpwuid();
int	idleflg;
int	whoami;

char *mytty(), *ttyname(), *ctime(), *strcpy(), *strchr(), *getlogin();
long	time();

main(argc, argv)
char **argv;
{
	register char *tp, *s;
	register FILE *fi;
	char *myname = getlogin();

	s = "/etc/utmp";
	if (argc>1 && strcmp(argv[1], "-i")==0) {
		idleflg++;
		argc--;
		argv++;
	}
	if(argc == 2)
		s = argv[1];
	if(argc == 3) {
		whoami++;
		tp = mytty();
	}
	if ((fi = fopen(s, "r")) == NULL) {
		fprintf(stderr, "who: cannot open %s\n", s);
		exit(1);
	}
	while (fread((char *)&utmp, sizeof(utmp), 1, fi) == 1) {
		if(whoami) {
			if (strncmp(utmp.ut_line, tp, sizeof(utmp.ut_line)))
				continue;
			strcpy(utmp.ut_name, myname);
			putline();
			exit(0);
		}
		if(utmp.ut_name[0] == '\0' && argc==1)
			continue;
		putline();
	}
	if (whoami) {
		strcpy(utmp.ut_name, myname);
		strcpy(utmp.ut_line, tp);
		time(&utmp.ut_time);
		putline();
	}
	exit(0);
}

char *
mytty()
{
	char *tp;

	if ((tp = ttyname(0)) == NULL
	||  (tp = strchr(tp+1, '/')) == NULL)
		return ("tty??");
	return (tp+1);
}

putline()
{
	struct stat statb;
	char tname[64];

	printf("%-8.8s %-8.8s %.12s",
		utmp.ut_name, utmp.ut_line, 4+ctime(&utmp.ut_time));
	if (idleflg) {
		long t = time((long *)0);
		strcpy(tname, "/dev/");
		strncat(tname, utmp.ut_line, 8);
		if (stat(tname, &statb)>=0
		 && t > statb.st_atime+600) {
			t -= statb.st_atime;
			t /= 60;
			printf("%5.0d:%.2d", (int)t/60, (int)t%60);
		}
	}
	printf("\n");
}

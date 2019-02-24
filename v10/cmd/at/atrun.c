/*
 * Run programs submitted by at.
 */
#include <stdio.h>
#include <sys/param.h>
#include <time.h>
#include <sys/stat.h>
#include <ndir.h>
#include <pwd.h>

# define SDIR "/usr/spool/at"
# define PDIR	"past"
# define LASTF "/usr/spool/at/lasttimedone"

int	nowtime;
int	nowdate;
int	nowyear;

main(argc, argv)
char **argv;
{
	int tt, day, year, uniq;
	DIR *dirp;
	struct direct *dp;

	chdir(SDIR);
	makenowtime();
	if ((dirp = opendir(".")) == 0) {
		fprintf(stderr, "Cannot read at directory\n");
		exit(1);
	}
	for (dp = readdir(dirp); dp; dp = readdir(dirp)) {
		if (dp->d_name[0] == '.') {
			if (dp->d_name[1] == '\0')
				continue;
			if (dp->d_name[1] == '.' && dp->d_name[2] == '\0')
				continue;
		}
		if (sscanf(dp->d_name, "%2d.%3d.%4d.%2d", &year, &day, &tt, &uniq) != 4)
			continue;
		if (nowyear < year)
			continue;
		if (nowyear == year && nowdate < day)
			continue;
		if (nowyear == year && nowdate == day && nowtime < tt)
			continue;
		run(dp->d_name);
	}
	closedir(dirp);
	updatetime(nowtime);
	exit(0);
}

makenowtime()
{
	long t;
	struct tm *localtime();
	register struct tm *tp;

	time(&t);
	tp = localtime(&t);
	nowtime = tp->tm_hour*100 + tp->tm_min;
	nowdate = tp->tm_yday;
	nowyear = tp->tm_year;
}

updatetime(t)
{
	FILE *tfile;

	tfile = fopen(LASTF, "w");
	if (tfile == NULL) {
		fprintf(stderr, "can't write lastfile\n");
		exit(1);
	}
	fprintf(tfile, "%.4d\n", t);
}

run(file)
char *file;
{
	struct stat stbuf;
	register pid, i;
	char sbuf[64];
	struct passwd *pw;
	struct passwd *getpwuid();

	if (fork()!=0)
		return;
	for (i=0; i<NOFILE; i++)
		close(i);
	dup(dup(dup(open("/dev/null", 2))));
	sprintf(sbuf, "%s/%s", PDIR, file);
	link(file, sbuf);
	unlink(file);
	chdir(PDIR);
	if (stat(file, &stbuf) == -1)
		exit(1);
	if ((pw = getpwuid(stbuf.st_uid)) == NULL)
		exit(1);	/* cruel but fair */
	if (pid = fork()) {
		if (pid == -1)
			exit(1);
		wait((int *)0);
		unlink(file);
		exit(0);
	}
	setlogname(pw->pw_name);
	(void)setupshares(stbuf.st_uid, (void (*)())0);
	setupgroups(pw->pw_name, stbuf.st_gid);
	setgid(stbuf.st_gid);
	setuid(stbuf.st_uid);
	nice(5);
	execl("/bin/sh", "sh", file, 0);
	execl("/usr/bin/sh", "sh", file, 0);
	fprintf(stderr, "Can't execl shell\n");
	exit(1);
}

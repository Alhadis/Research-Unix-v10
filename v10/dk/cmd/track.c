/*
 * track
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dk.h>
#include <signal.h>
#include <setjmp.h>
#include <ftw.h>

int	vflag;			/* verbose */
int	nflag;			/* don't actually copy */
int	rflag;			/* act as remote slave */
int	tflag;			/* update only if remote is newer */
char	prefix[128];		/* alternate prefix on remote (-d flag) */
int	nsize;			/* amount to strip off local name */

char	*descfile;
extern	char *optarg;
extern	int optind;
FILE	*torem;
int	rem;
int	descfd;
jmp_buf	almbuf;
char	tempfile[] = "/tmp/trckXXXXXX";
unsigned getsum();
char	*remoten();

main(argc, argv)
char **argv;
{
	int consider();
	char buf[128];
	extern char *dkerror;

	if (argc <= 1) {
		printf("track [-nvtr] {-f description | directory} machine\n");
		exit(0);
	}
	for (;;) {
		switch (getopt(argc, argv, "vnrtf:d:")) {

		case 'v':
			vflag++;
			continue;

		case 'r':
			rflag++;
			continue;

		case 'n':
			nflag++;
			continue;

		case 't':
			tflag++;
			continue;

		case 'f':
			descfile = optarg;
			continue;

		case 'd':
			strcpy(prefix, optarg);
			continue;
		case '?':
			exit(1);

		case EOF:
		default:
			break;
		}
		break;
	}
	if (setjmp(almbuf)) {
		if (rflag==0)
			printf("track: alarm talking to %s\n", argv[optind]);
		exit(1);
	}
	if (rflag)
		remote();
	if (descfile) {
		descfd = open(descfile, 0);
		if (descfd <= 0) {
			printf("track: can't open description %s", descfile);
			exit(1);
		}
	} else {
		if (optind >= argc) {
			printf("track: no description file\n");
			exit(1);
		}
		descfile = argv[optind];
		optind++;
	}
	if (optind >= argc) {
		printf("track: no machine given\n");
		exit(1);
	}
	rem = tdkexec(argv[optind], "track -r");
	if (rem < 0) {
		printf("track: %s call failed: %s\n", argv[optind], dkerror);
		exit(1);
	}
	torem = fdopen(rem, "w");
	fprintf(torem, "testing 123\n");
	fflush(torem);
	vgetline(rem, buf, "<startup>");
	if (strncmp(buf, "ok", 2) !=0) {
		printf("track: can't talk to remote\n");
		exit(1);
	}
	mktemp(tempfile);
	if (descfd <= 0) {
		if (prefix[0])
			nsize = strlen(descfile);
		ftw(descfile, consider, 10);
	} else {
		while (getline(descfd, buf)) {
			if (prefix[0])
				nsize = strlen(buf);
			ftw(buf, consider, 10);
		}
	}
	unlink(tempfile);
	exit(1);
}

remote()
{
	char buf[1024];
	struct stat statb;
	char name[128];
	int x, ofd;
	char obuf[BUFSIZ];

	setbuf(stdout, obuf);
	while (getline(0, buf) != 0) {
		if (sscanf(buf, "testing %d", &x) == 1) {
			printf("ok\n");
			fflush(stdout);
			continue;
		}
		if (sscanf(buf, "stat %s", name) == 1) {
			if (stat(name, &statb) < 0) {
				printf("ng\n");
				fflush(stdout);
				continue;
			}
			printf("ok %ld %ld\n", statb.st_size, statb.st_mtime);
			fflush(stdout);
			continue;
		}
		if (sscanf(buf, "sum %s", name) == 1) {
			printf("ok %ld\n", (long)getsum(name));
			fflush(stdout);
			continue;
		}
		if (sscanf(buf, "send %s", name) == 1) {
			ofd = open(name, 0);
			if (ofd < 0) {
				printf("ng\n");
				fflush(stdout);
				continue;
			}
			printf("ok\n");
			fflush(stdout);
			while ((x = read(ofd, buf, 1024)) > 0)
				write(1, buf, x);
			close(ofd);
			write(1, buf, 0);
			continue;
		}
		break;
	}
	exit(0);
}

consider(name, statp, flag)
char *name;
struct stat *statp;
{
	char buf[128];
	long lsum, rsum;
	long size;
	time_t date;

	switch (flag) {

	case FTW_F:
		break;

	default:
		printf("track: trouble with %s\n", name);
	case FTW_D:
		return(0);
	}
	alarm(60);
	fprintf(torem, "stat %s %ld\n", remoten(name), statp->st_size);
	fflush(torem);
	vgetline(rem, buf, name);
	if (sscanf(buf, "ok %ld %ld", &size, &date) != 2) {
		if (vflag)
			printf("track: %s not found (remote)\n", remoten(name));
		return(0);
	}
	if (size == statp->st_size) {
		fprintf(torem, "sum %s\n", remoten(name));
		fflush(torem);
		lsum = getsum(name);
		vgetline(rem, buf, name);
		if (sscanf(buf, "ok %ld", &rsum) != 1) {
			printf("track: can't read sum for %s\n", remoten(name));
			exit(1);
		}
		if (lsum == rsum) {
			if (vflag > 1)
				printf("track: %s OK\n", name);
			return(0);
		}
	}
	if (tflag && statp->st_mtime > date) {
		if (vflag)
			printf("track: %s newer locally\n", name);
		return(0);
	}
	if (nflag) {
		printf("track: would copy %s\n", name);
		return(0);
	}
	printf("track: copy %s:", name);
	copy(name, rem, date);
	return(0);
}

copy(name, fd, date)
char *name;
time_t date;
{
	char buf[1024];
	int ofd, nfd, i, n;
	time_t ut[2];

	fprintf(torem, "send %s\n", remoten(name));
	fflush(torem);
	vgetline(fd, buf, name);
	if (strcmp(buf, "ok") != 0) {
		printf("can't open remote\n");
		return;
	}
	if ((ofd = creat(tempfile, 0644)) < 0) {
		printf("can't open temp\n");
		return;
	}
	i = 10;
	while ((n = read(fd, buf, 1024)) > 0) {
		if (write(ofd, buf, n) != n) {
			printf("write temp file\n");
			return;
		}
		if (--i <= 0) {
			alarm(60);
			i = 10;
		}
	}
	close(ofd);
	ofd = open(tempfile, 0);
	if (ofd < 0) {
		printf("can't reopen temp\n");
		return;
	}
	nfd = creat(name, 0777);
	if (nfd < 0) {
		printf("can't create new version\n");
		return;
	}
	while ((n = read(ofd, buf, 1024)) > 0) {
		if (write(nfd, buf, n) != n) {
			printf("write error in copy\n");
			return;
		}
	}
	close(ofd);
	close(nfd);
	ut[0] = time((time_t)NULL);
	ut[1] = date;
	utime(name, ut);
	printf("OK\n");
}

char *
remoten(name)
register char *name;
{
	static char rname[256];

	if (prefix[0] == '\0')
		return(name);
	if (name[0] != '/') {
		strcpy(rname, prefix);
		strcat(rname, "/");
		strcat(rname, name);
		return(rname);
	}
	strcpy(rname, prefix);
	strcat(rname, name+nsize);
	return(rname);
}

vgetline(fd, buf, name)
char *buf, *name;
{
	if (getline(fd, buf)==0) {
		printf("track: EOF on remote, file %s\n", name);
		exit(1);
	}
}

getline(fd, buf)
char *buf;
{
	register char *bp;

	bp = buf;
	for(;;) {
		if (read(fd, bp, 1) <= 0)
			return(0);
		if (*bp == '\n') {
			*bp = '\0';
			return(1);
		}
		if (++bp >= &buf[128])
			return(0);
	}
}

unsigned
getsum(name)
char *name;
{
	register FILE *f;
	register unsigned sum;
	register c;

	if ((f = fopen(name, "r")) == NULL)
		return(-1);
	sum = 0;
	while ((c = getc(f)) != EOF) {
		if (sum & 01)
			sum = (sum>>1) + 0x8000;
		else
			sum >>= 1;
		sum += c;
		sum &= 0xFFFF;
	}
	fclose(f);
	return(sum);
}

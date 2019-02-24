#include "stdio.h"
#include "errno.h"
#include "signal.h"

extern int	errno;
char *tapes[] = {
	"/dev/rmt2",
	"/dev/rmt1",
	"/dev/rmt0",
	0
};
long recnum, reccnt;
int broken();
long filenum;
int eof;
unsigned recsz;
char buf[60*1024];
int fd;

timeout()
{
	gprintf(stdout, "timeout\n");
}

dens()
{	int fd, i, n;
	for(i = 0; tapes[i]; i++) {
		close(fd);
		errno = 0;
		alarm(60);
		fd = open(tapes[i], 0);
		alarm(0);
		if(fd < 0) {
			perror(tapes[i]);
			continue;
		}
		alarm(60);
		n = read(fd, buf, sizeof(buf));
		alarm(0);
		if( n >= 0) {
			gprintf(stdout, "%s:\n", tapes[i]);
			close(fd);
			return(i);
		}
		if(errno == ENXIO) continue;
		else perror(tapes[i]);
	}
	gprintf(stdout, "I can't read anything from your tape\n");
	return(-1);
}

main()
{	int i;
	int n;
	signal(SIGALRM, timeout);
	signal(SIGINT, broken);
	i = dens();
	if(i == -1) exit(1);
	alarm(10);
	fd = open(tapes[i], 0);
	alarm(0);
	if(fd < 0) {
		perror("Where did your tape go?");
		exit(1);
	}
	newfile();
	for(;;) {
		alarm(30);
		n = read(fd, buf, sizeof(buf));
		alarm(0);
		if(n == -1) {
			if(errno == 0) {
				if(reccnt > 0) recdata();
				if(eof == 2) {
					gprintf(stdout, "2 eofs read\n");
					exit(0);
				}
				filedata();
				newfile();
			}
			else {
				recdata();
				filedata();
				perror("Bailing out");
				exit(1);
			}
		}
		else if(n == 0) {
			eof++;
			if(eof == 2) {
				gprintf(stdout, "2 eofs read\n");
				exit(0);
			}
			if(reccnt > 0) recdata();
			filedata();
			newfile();
		}
		else {
			eof = 0;
			recnum++;
			if(n == recsz) reccnt++;
			else {
				if(reccnt > 0) recdata();
				recsz = n;
				reccnt = 1;
			}
		}
	}
}

newfile()
{
	filenum++;
	recnum = 0;
	reccnt = 0;
	recsz = 0;
	errno = 0;
}

filedata()
{
	gprintf(stdout, "file %ld had %ld records\n", filenum, recnum);
}

recdata()
{
	if(recsz < sizeof(buf))
		gprintf(stdout, "file %ld, %ld records of size %u\n", filenum, reccnt, recsz);
	else gprintf(stdout, "file %ld, %ld records of size %u (or larger)\n",
		filenum, reccnt, recsz);
}

broken()
{
	gprintf(stderr, "interrupted\n");
	recdata();
	filedata();
	exit(0);
}

gprintf(s,f,a,b,c,d)
FILE *s;
char *f;
{
	fprintf(s,f,a,b,c,d);
	fflush(stdout);
}

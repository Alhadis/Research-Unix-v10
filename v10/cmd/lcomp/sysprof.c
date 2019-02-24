#include "nlist.h"
#include "stdio.h"

struct nlist nl[] = {
	{"_proFptr"},
	{0}
};

struct rec {
	long len;
	struct rec *next;
	char *fname;
} proFptr;
FILE *outfd;
struct rec *x;
char buf[256];
long foo[1024];
int fd;

main(argc, argv)
char **argv;
{
	if(argc > 1)
		nlist(argv[1], nl);
	else
		nlist("/unix", nl);
	if(nl[0].n_value == 0) {
		fprintf(stderr, "weird namelist?\n");
		exit(1);
	}
	fd = open("/dev/kmem", 0);
	if(fd < 0) {
		perror("/dev/kmem");
		exit(1);
	}
	outfd = fopen("prof.out", "w");
	if(outfd == 0) {
		perror("prof.out");
		exit(1);
	}
	if(lseek(fd, nl[0].n_value, 0) == -1) {
		fprintf(stderr, "lseek failed seeking to x%x\n", nl[0].n_value);
		exit(1);
	}
	if(read(fd, (char *)&x, sizeof(long)) != sizeof(long)) {
		fprintf(stderr, "read failed\n");
		exit(1);;
	}
	for(;;) {
		readit();
		if(proFptr.next == 0 || proFptr.next == x)
			break;
		if(seenit(proFptr.next))
			break;
		x = proFptr.next;
	}
}

readit()
{	int i;
	i = lseek(fd, (char *)x, 0);
	if(i == -1)
		fatal("first lseek in readit, seeking to x%x\n", x);
	i = read(fd, (char *)&proFptr, sizeof(proFptr));
	if(i != sizeof(proFptr))
		fatal("reading proFptr in readit\n");
	i = read(fd, (char *)foo, sizeof(long) * proFptr.len);
	if(i != sizeof(long) * proFptr.len)
		fatal("read %d instead of %d\n", i, sizeof(long) * proFptr.len);
	i = lseek(fd, proFptr.fname, 0);
	if(i == -1)
		fatal("seeking to x%x\n", proFptr.fname);
	i = read(fd, buf, sizeof(buf));
	fprintf(outfd, "%s\n", buf);
	fflush(outfd);
	for(i = 3; i < proFptr.len; i++)
		fprintf(outfd, "%d\n", foo[i-3]);
}

fatal(s, a, b, c, d, e, f, g)
char *s;
{
	fprintf(stderr, s, a, b, c, d, e, f, g);
	exit(1);
}

/* very dumb loop finder, and of course loops can't happen */
#define NDUMB 1000
int seen[NDUMB];
int seencnt;
seenit(n)
{	int i;
	for(i = 0; i < seencnt; i++)
		if(n == seen[i]) {
			fprintf(stderr, "looping\n");
			return(1);
		}
	seen[seencnt++] = n;
	if(seencnt >= NDUMB) {
		fprintf(stderr, "seen %d files, quitting\n", seencnt);
		return(1);
	}
	return(0);
}

#include	<stdio.h>
#include	<stddef.h>

#define	NSUMS		100
int BSIZE = 1024;

char *myname;
typedef long Sumfn(long, char *, int);
extern Sumfn sumr, sum5, sum32;
static int sumfile(char *, Sumfn *);

static void
usage()
{
	fprintf(stderr, "Usage: %s [-ir5] [files]\n", myname);
	exit(1);
}

main(int argc, char **argv)
{
	int iflag = 0;
	int exitcode = 0;
	int c;
	char buf[4096];
	Sumfn *fn = sum32;
	extern int optind;

	myname = argv[0];
	while((c = getopt(argc, argv, "ir5")) != -1)
		switch(c)
		{
		case 'i':	iflag = 1; break;
		case 'r':	fn = sumr; break;
		case '5':	fn = sum5; break;
		case '?':	usage(); break;
		}

	if(iflag){
		while(fgets(buf, sizeof buf, stdin) != NULL){
			if(c = strlen(buf)){
				buf[c-1] = 0;
				exitcode |= sumfile(buf, fn);
			}
		}
	} else if(optind < argc){
		while(optind < argc)
			exitcode |= sumfile(argv[optind++], fn);
	} else
		exitcode |= sumfile((char *)0, fn);
	exit(exitcode);
}

static int
sumfile(char *path, Sumfn *fn)
{
	int fd;
	int n;
	long sum;
	int fsize;
	char buf[64*1024];

	if(path){
		if((fd = open(path, 0)) < 0){
			fprintf(stderr, "%s: ", myname);
			perror(path);
			return(1);
		}
	} else
		fd = 0;
	fsize = 0;
	sum = 0;
	while((n = read(fd, buf, sizeof buf)) > 0){
		fsize += n;
		sum = (*fn)(sum, buf, n);
	}
	if(n < 0){
		fprintf(stderr, "%s: %s: ", myname, path? path:"<standard input>");
		perror("read");
		if(path)
			close(fd);
		return(1);
	}
	if(path)
		close(fd);
	(*fn)(sum, (char *)0, fsize);
	if(path)
		printf(" %s", path);
	putchar('\n');
	fflush(stdout);
	return(0);
}

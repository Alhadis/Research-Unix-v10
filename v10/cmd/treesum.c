#include "stdio.h"
#include "ftw.h"
#include "sys/param.h"
#include "sys/stat.h"

#define MAXNAME		1024
#define BLOKSIZE	1024

char buf[BUFSIZ];

main(argc, argv)
char **argv;
{	int i, pr();
	struct stat stb;
	if (argc < 2) argc = 2; /* use current dir if no args */
	for (i = 1; i < argc; i++) {
		if(stat(argv[i],&stb)) { perror(argv[i]); exit(1); }
		ftw(argv[i], pr, 12);
		}
	exit(0);
}

int pr(s, p, n0)
char *s;
int n0;
struct stat *p;
{	int fd;
	register char *x;
	register int i, n;
	register unsigned sum;
	void sym_skip_msg();

#ifdef S_IFLNK
	if (n0 == FTW_SL) /* symbolic link */ {
		sym_skip_msg(s);
		return 0;
		}
#endif
		
	if((p->st_mode & S_IFMT) != S_IFREG)	/* if not an ordinary file */
		return(0);
	if((fd = open(s, 0)) < 0) { perror(s); return 0; }
	sum = 0;
	while((n = read(fd, buf, sizeof(buf))) > 0)
		for(i = 0, x = buf; i < n; x++, i++) {
			if (sum&01)
				sum = (sum>>1) + 0x8000;
			else
				sum >>= 1;
			sum += *x&0377;
			sum &= 0xFFFF;
		}
        if (n==0)
		printf("%05u%6ld %s\n", sum, (p->st_size+BLOKSIZE-1)/BLOKSIZE, s);
        else { perror(s); }
	close(fd);
	return(0);
}

#ifdef S_IFLNK
void
sym_skip_msg(s)
char *s;
{
	char symname[MAXNAME];
	int i;
	i = readlink(s, symname, MAXNAME);
	if (i >= MAXNAME) i = MAXNAME - 1;
	symname[i] = 0;
	printf("skipped symbolic link %s --> %s\n",s,symname);
}
#endif

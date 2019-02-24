/*
 * Type tty name
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

char	*ttyname();
char	*Xttyname();
int sts;

main(argc, argv)
char **argv;
{
	register char *p;

	p = Xttyname(0);
	if (!(argc==2 && strcmp(argv[1], "-s") == 0))
		printf("%s\n", (p? p: "not a tty"));
	exit(sts);
}

char *
Xttyname(fd)
int fd;
{
	struct stat st;
	static char buf[20];
	char *p;

	sts = 0;
	if ((p = ttyname(fd)) != NULL)
		return (p);
	sts = 1;
	if (fstat(fd, &st) < 0)
		return (NULL);
	if (st.st_nlink != 0)	/* hack -- is it a pipe? */
		return (NULL);
	sts = 2;
	sprintf(buf, "nameless pipe/p%04x", st.st_ino);
	return (buf);
}

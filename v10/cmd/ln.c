static	char sccsid[] = "@(#)ln.c 4.3 3/31/82";
/*
 * ln
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

struct	stat stb;
int	fflag;
int	sflag;
char	name[BUFSIZ];
char	*strrchr();

main(argc, argv)
	int argc;
	register char **argv;
{
	register int i, r;

	argc--, argv++;
again:
	if (argc && strcmp(argv[0], "-f") == 0) {
		fflag++;
		argv++;
		argc--;
	}
	if (argc && strcmp(argv[0], "-s") == 0) {
		sflag++;
		argv++;
		argc--;
	}
	if (argc <= 1) 
		goto usage;
	if (sflag == 0 && argc > 2) {
		if (stat(argv[argc-1], &stb) < 0)
			goto usage;
		if ((stb.st_mode&S_IFMT) != S_IFDIR) 
			goto usage;
	}
	r = 0;
	for(i = 0; i < argc-1; i++)
		r |= linkit(argv[i], argv[argc-1]);
	exit(r);
usage:
	fprintf(stderr, "Usage: ln [ -s ] f1 f2\nor: ln [ -s ] f1 ... fn d2\n");
	exit(1);
}

int	link(), symlink();

linkit(from, to)
	char *from, *to;
{
	char *tail;
	int (*linkf)() = sflag ? symlink : link;

	/* is target a directory? */
	if (sflag == 0 && fflag == 0 && stat(from, &stb) >= 0
	    && (stb.st_mode&S_IFMT) == S_IFDIR) {
		fprintf(stderr, "ln: %s is a directory\n", from);
		return (1);
	}
	if (stat(to, &stb) >= 0 && (stb.st_mode&S_IFMT) == S_IFDIR) {
		tail = strrchr(from, '/');
		if (tail == 0)
			tail = from;
		else
			tail++;
		sprintf(name, "%s/%s", to, tail);
		to = name;
	}
	if ((*linkf)(from, to) < 0) {
		lnerror(from, to);
		return (1);
	}
	return (0);
}

lnerror(from, to)
char *from, *to;
{
	char buf[BUFSIZ];

	sprintf(buf, "ln %s %s", from, to);
	perror(buf);
}

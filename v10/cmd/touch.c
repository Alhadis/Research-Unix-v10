#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

long lseek();

main(argc,argv)
int argc;
char *argv[];
{
	int i;
	static int force = 1;
	int status = 0;

	if (argc < 2) {
		fprintf(stderr, "usage: touch files\n");
		exit(1);
	}
	for(i = 1; i < argc; ++i)
		if (strcmp(argv[i], "-c") == 0)
			force = 0;
		else
			status += touch(force, argv[i]);
	exit(status);
}

touch(force, name)
int force;
char *name;
{
	struct stat stbuff;
	char junk[1];
	int fd;

	stbuff.st_size = 0;
	if (stat(name,&stbuff) < 0 && force == 0) {
		fprintf(stderr, "touch: %s: cannot stat\n", name);
		return (1);
	}
	if (stbuff.st_size == 0) {
		if ((fd = creat(name, 0666)) < 0) {
			fprintf(stderr, "touch: %s: cannot create\n", name);
			return (1);
		}
		close(fd);
		return (0);
	}
	if ((fd = open(name, 2)) < 0) {
		fprintf(stderr, "touch: %s: cannot open\n", name);
		return (1);
	}
	if(read(fd, junk, 1) < 1) {
		fprintf(stderr, "touch: %s: read error\n", name);
		close(fd);
		return (1);
	}
	lseek(fd, 0L, 0);
	if(write(fd, junk, 1) < 1 ) {
		fprintf(stderr, "touch: %s: write error\n", name);
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

#include <stdio.h>
#include <ctype.h>
#include <sys/diskio.h>

main(argc, argv)
int argc;
char **argv;
{

	if (argc > 2 && strcmp(argv[1], "-f") == 0)
		exit(filepart(argv[2]));
	if (argc < 2) {
		fprintf(stderr, "usage: setpart device firstblock [size]\n");
		fprintf(stderr, "\tor setpart -f file\n");
		exit(1);
	}
	exit(setpart(argc-1, argv+1));
}

filepart(file)
char *file;
{
	FILE *fp;
	char line[100];		/* arbitrarily large */
	char *args[4];
	int nargs;
	int status;

	if ((fp = fopen(file, "r")) == NULL) {
		fprintf(stderr, "%s: cannot open\n");
		return (1);
	}
	status = 0;
	while (fgets(line, sizeof(line), fp) != NULL) {
		if (line[0] == '#')
			continue;
		if ((nargs = getmfields(line, args, 4)) < 2)
			continue;
		if (setpart(nargs, args))
			status = 1;
	}
	fclose(fp);
	return (status);
}

setpart(n, arg)
int n;
char **arg;
{
	int fd;
	long parts[2];
	long atol();

	if (n < 1) {
		fprintf(stderr, "internal: too few args\n");
		return (1);
	}
	if ((fd = open(arg[0], 1)) < 0) {
		perror(arg[0]);
		return (1);
	}
	if (n < 2) {
		if (ioctl(fd, DIOGSIZ, parts) < 0) {
			perror(arg[0]);
			close(fd);
			return (1);
		}
		printf("%s\t%ld\t\%ld\n", arg[0], parts[0], parts[1]);
	} else {
		if (!isdigit(arg[1][0])) {
			fprintf(stderr, "%s: bad firstblock %s\n", arg[0], arg[1]);
			close(fd);
			return (1);
		}
		if (n > 2 && !isdigit(arg[2][0])) {
			fprintf(stderr, "%s: bad nblock %s\n", arg[0], arg[2]);
			close(fd);
			return (1);
		}
		parts[0] = atol(arg[1]);
		if (n > 2)
			parts[1] = atoi(arg[2]);
		else
			parts[1] = 0x7fffffff;	/* huge */
		if (ioctl(fd, DIOSSIZ, parts) < 0) {
			perror(arg[0]);
			close(fd);
			return (1);
		}
	}
	close(fd);
	return (0);
}

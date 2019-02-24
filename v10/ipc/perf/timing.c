#include <stdio.h>
main()
{
	int i, fd;

	fd = open("quux", 0);
	fprintf(stderr, "open(quux, 0) returns %d\n", fd);
	if (fd < 0)
		exit(1);
	fprintf(stderr, "close(fd) returns %d\n", close(fd));
}

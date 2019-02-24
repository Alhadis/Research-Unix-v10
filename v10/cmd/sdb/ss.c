#include <stdio.h>

main(argc, argv)
int argc;
char **argv;
{
	FILE *fp;

	if ((fp = fopen(".", "r")) == NULL)
		perror(".");
	exit(0);
}

#include <stdio.h>
main(argc, argv)
char *argv[];
{
	long mtime, getstamp();
	char *filename;
	filename = argv[1];
	mtime = getstamp(filename);
	printf(".ds s) %ld\n",mtime);
}

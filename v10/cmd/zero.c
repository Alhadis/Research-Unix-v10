#include	<stdlib.h>
#include	<limits.h>
#include	<stdio.h>

#define		BLOCK		(32*1024)

main(int argc, char **argv)
{
	int byte = 0;
	long nbytes = LONG_MAX;
	char buf[BLOCK];

	if(argc > 3){
		fprintf(stderr, "Usage: zero [byte [nbytes]]\n");
		exit(1);
	}
	if(argc > 1)
		byte = strtol(argv[1], (char **)0, 0);
	if(argc > 2)
		nbytes = strtol(argv[2], (char **)0, 0);
	memset(buf, byte, BLOCK);
	while(nbytes > BLOCK){
		if(write(1, buf, BLOCK) != BLOCK){
			perror("zero: write error");
			exit(2);
		}
		nbytes -= BLOCK;
	}
	if(nbytes){
		if(write(1, buf, (int)nbytes) != (int)nbytes){
			perror("zero: write error");
			exit(2);
		}
	}
	exit(0);
}

#include <stdio.h>
#include <time.h>
#include "../lib/lib.h"
#include "proto.h"
#include "../paths.h"

int
main(int argc, char *argv[])
{
	char mesg[MAXLINE];

	openlog(SDIR "/log");
	if (argc != 2) {
		eprintf("usage: notify jobname");
		exit(1);
	}
	sprintf(mesg, M_ANNOUNCE, argv[1]);
	write(1, mesg, strlen(mesg));
	if (!getline(mesg) || strcmp(mesg, M_ACK) != 0)
		exit(1);
	exit(0);
}

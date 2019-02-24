#include <sys/param.h>
#include <stdio.h>
#include <ipc.h>
#include <libc.h>
#include "defs.h"

/* detach from the terminal */
detach(mtpt)
	char *mtpt;
{
	int i;

	/* equivalent to sh's & */
	switch(fork()) {
	case 0:
	case -1:
		break;
	default:
		exit(0);
	}
	openlog(mtpt);

	/* disassociate from ttys and other process groups */
	setpgrp(getpid(), getpid());
	close(0);
	close(1);
	for (i=4; i<NSYSFILE; i++)
		close(i);
}

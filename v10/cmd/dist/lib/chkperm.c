#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include "lib.h"

/*
 * the right place to put this guy would be in the tarf command;
 * then eventually we could remove all tarfile support from dist
 * proper when (if?) most systems have adequate tar commands.
 */
int
chkperm(int fd)
{
	return 1;
}

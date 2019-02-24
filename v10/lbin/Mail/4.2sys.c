#ifndef	BSD4_2
#ifndef lint
static char *sccsid = "@(#)4.2sys.c	1.0 3/9/84";
#endif

#include "rcv.h"

/*
 * Mail -- a mail program
 *
 * Simulates some 4.2BSD system calls
 */

int oldbmask = 0;		/* previously blocked signals */
int beenhere = 0;		/* have we initialized signals? */
int heldsigs[NSIG];		/* list of held signals */

/*
 * Add signals to list of those to be blocked
 */

sigblock(smask)
int smask;
{
	int retmask;

	retmask = oldbmask;
	oldbmask |= smask;
	blocksig(oldbmask);
	return(retmask);
}

/*
 * Set signals to be blocked
 */

sigsetmask(smask)
int smask;
{
	int retmask;

	retmask = oldbmask;
	oldbmask = smask;
	blocksig(oldbmask);
	return(retmask);
}

/*
 * Block signals in smask
 */

blocksig(smask)
int smask;
{
	int i;

	for (i = 0; i < NSIG; i++) {
		if (!beenhere) {
			sigset(i, sigset(i, SIG_DFL));
			heldsigs[i] = 0;
		}
		if (i != SIGKILL && i != SIGSTOP && i != SIGCONT &&
		    (smask & mask(i)) && !heldsigs[i]) {
			sighold(i);
			heldsigs[i] = 1;
		}
		else if (i != SIGKILL && i != SIGSTOP && i != SIGCONT &&
		    !(smask & mask(i)) && heldsigs[i]) {
			sigrelse(i);
			heldsigs[i] = 0;
		}
	}
	beenhere = 1;
}

#endif	/* BSD4_2 */

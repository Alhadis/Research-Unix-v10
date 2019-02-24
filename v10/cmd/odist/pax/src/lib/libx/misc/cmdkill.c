/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * send sig to cmd opened by cmdopen()
 */

#include "cmdlib.h"

int
cmdkill(fd, sig)
register int	fd;
int		sig;
{
	register struct cmdinfo*	cmd;

	for (cmd = cmds; cmd; cmd = cmd->next)
		if (cmd->fd == fd)
		{
			if (cmd->pid > 0) return(kill(cmd->pid, sig));
			break;
		}
	errno = ESRCH;
	return(-1);
}

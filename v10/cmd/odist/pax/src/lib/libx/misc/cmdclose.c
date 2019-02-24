/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * close a stream opened by cmdopen()
 * -1 returned if cmdopen() had a problem
 * otherwise exit() status of command is returned
 */

#include "cmdlib.h"

int
cmdclose(fd)
register int	fd;
{
	register struct cmdinfo*	cmd;

	for (cmd = cmds; cmd; cmd = cmd->next)
		if (cmd->fd == fd)
		{
			(void)close(fd);
			cmd->fd = -2;
			if (cmd->pid < 0)
			{
				cmd->pid = 0;
				return(cmd->status);
			}
			return(cmdwait(-cmd->pid));
		}
	return(-1);
}

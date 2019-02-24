/*
 * Glenn Fowler
 * AT&T Bell Laboratories
 *
 * wait for cmd pid and return exit status
 * if pid < 0 then table not searched for -pid
 * -1 returned on pid error
 */

#include "cmdlib.h"

struct cmdinfo*		cmds;		/* open cmd list		*/

int
cmdwait(pid)
register int	pid;
{
	register struct cmdinfo*	cmd;
	register int			n;
	struct cmdinfo*			slot;
	int				status;
	int				oerrno;
	long				pos;

	if (pid > 0)
	{
		pid = -pid;
		for (cmd = cmds; cmd; cmd = cmd->next)
			if (cmd->pid == pid)
			{
				cmd->pid = 0;
				return(cmd->status);
			}
	}
	pid = -pid;
	oerrno = errno;
	sigcritical(1);
	for (;;)
	{
		if ((n = wait(&status)) == -1)
		{
			oerrno = ESRCH;
			status = -1;
			break;
		}
		if (EXITED(status))
		{
			status = EXITSTATUS(status);
			if (status == EXITMAX) status = -1;
		}
		else status = TERMSTATUS(status);
		for (slot = 0, cmd = cmds; cmd; cmd = cmd->next)
		{
			if (cmd->pid == n)
			{
				if (pid == n) cmd->pid = 0;
				else
				{
					cmd->pid = -n;
					cmd->status = status;
				}
				break;
			}
			else if (!slot && !cmd->pid) slot = cmd;
		}
		if (pid == n) break;
		if (!cmd && ((cmd = slot) || (cmd = (struct cmdinfo*)malloc(sizeof(struct cmdinfo)))))
		{
			cmd->fd = -2;
			cmd->pid = -n;
			cmd->status = status;
			cmd->next = cmds;
			cmds = cmd;
		}
	}
	if ((pos = lseek(fileno(stdin), 0L, 1)) != ftell(stdin))
		(void)fseek(stdin, pos, 0);
	sigcritical(0);
	errno = oerrno;
	return(status);
}

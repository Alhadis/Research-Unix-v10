/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * common command execution support with
 * proper stdio and wait() syncronization
 *
 * _ contains the command path name and is
 * placed at the top of the environment
 */

#include "cmdlib.h"

static void	redirect();

extern char*	getenv();
extern char*	getshell();
extern char*	pathpath();
extern char*	putenv();
extern char*	strcpy();

/*
 * fork and exec cmd(argv) according to mode combinations:
 *
 *	a	argv[-1] and argv[0] can be modified
 *	e	clear environment
 *	g	run with real gid
 *	p	paranoid
 *	r	cmd stdout pipe fd returned
 *	w	cmd stdin pipe fd returned
 *	u	run with real uid
 *
 * pipe not used when r and w are omitted (no io on return fd)
 * cmd==0 names the current shell
 * envv is the child environment
 * redv is the child redirection vector
 *      even elements duped from the next element
 *	-1 even terminates the list
 *	-1 odd element means close
 */

int
cmdopen(cmd, argv, envv, redv, mode)
char*	cmd;
char**	argv;
char**	envv;
int*	redv;
char*	mode;
{
	register struct cmdinfo*	proc;
	register int			cmdfd = -1;
	register char**			p;
	char**				v;
	int*				r;
	int				argmod = 0;
	int				envclear = 0;
	int				paranoid = 0;
	int				realgid = 0;
	int				realuid = 0;
	long				pos;
	char				path[PATH_MAX];
	char				env[PATH_MAX + 2];
	int				pio[2];

	extern char**			environ;

	if (cmd && !pathpath(path, cmd, (char*)0, 011)) return(-1);
	if (mode) while (*mode) switch (*mode++)
	{
	case 'a':
		argmod = 1;
		break;
	case 'e':
		envclear = 1;
		break;
	case 'g':
		realgid = 1;
		break;
	case 'p':
		paranoid = 1;
		break;
	case 'r':
		cmdfd = 1;
		break;
	case 'u':
		realuid = 1;
		break;
	case 'w':
		cmdfd = 0;
		break;
	default:
		return(-1);
	}
	for (proc = cmds; proc; proc = proc->next)
		if (!proc->pid) break;
	if (!proc)
	{
		if (!(proc = (struct cmdinfo*)malloc(sizeof(struct cmdinfo)))) return(-1);
		proc->pid = 0;
		proc->next = cmds;
		cmds = proc;
	}
	if (!putenv((char*)0)) return(-1);
	if (cmdfd >= 0 && pipe(pio)) return(-1);
	fflush(stdout);
	fflush(stderr);
	sigcritical(1);
	proc->pid = fork();
	sigcritical(0);
	switch (proc->pid)
	{
	case -1:
		proc->pid = 0;
		if (cmdfd >= 0)
		{
			(void)close(pio[0]);
			(void)close(pio[1]);
		}
		return(-1);
	case 0:
		if (paranoid || realgid) (void)setgid(getgid());
		if (paranoid || realuid) (void)setuid(getuid());
		if ((pos = ftell(stdin)) != lseek(fileno(stdin), 0L, 1))
			(void)lseek(fileno(stdin), pos, 0);
		if (cmdfd >= 0)
		{
			redirect(pio[cmdfd], cmdfd);
			(void)close(pio[!cmdfd]);
		}
		if (r = redv) while (*r >= 0)
		{
			redirect(*(r + 1), *r);
			r += 2;
		}
		if (envclear) environ = 0;
		env[0] = '_';
		env[1] = '=';
		if (!putenv(env)) _exit(EXITMAX);
		if (paranoid && !putenv("PATH=:/bin:/usr/bin")) _exit(EXITMAX);
		if (p = envv)
			while (*p)
				if (!putenv(*p++)) _exit(EXITMAX);
#if 0
		if ((mode = getenv("EXEC_OPTIONS")) && (*mode == 'n' || *mode == 'v'))
		{
			if (*(mode + 1) == 'e' && (p = environ))
				while (*p)
					fprintf(stderr, "%s\n", *p++);
			fprintf(stderr, "+ %s", cmd ? path : "sh");
			if ((p = argv) && *p)
				while (*++p) fprintf(stderr, " %s", *p);
			fprintf(stderr, "\n");
			if (*mode == 'n') _exit(0);
		}
#endif
		p = argv;
		if (cmd)
		{
			(void)strcpy(env + 2, path);
			execv(path, p);
			if (errno != ENOEXEC) _exit(EXITMAX);

			/*
			 * try cmd as a shell script
			 */

			if (!argmod)
			{
				while (*p++);
				if (!(v = (char**)malloc((p - argv + 2) * sizeof(char**)))) _exit(EXITMAX);
				p = v + 2;
				if (*argv) argv++;
				while (*p++ = *argv++);
				p = v + 1;
			}
			*p = path;
			*--p = "sh";
		}
		if (!paranoid)
		{
			(void)strcpy(env + 2, getshell());
			execv(env + 2, p);
		}
		(void)strcpy(env + 2, "/bin/sh");
		execv(env + 2, p);
		_exit(EXITMAX);
		/*NOTREACHED*/
	default:
		if (cmdfd >= 0)
		{
			(void)close(pio[cmdfd]);
			proc->fd = pio[!cmdfd];
		}
		else if ((proc->fd = dup(0)) < 0)
			(void)cmdclose(proc->fd);
		return(proc->fd);
	}
}

/*
 * redirect nfd to fd
 * nfd==-1 means close(fd)
 */

static void
redirect(nfd, fd)
int	nfd;
int	fd;
{
	if (nfd != fd)
	{
		if (nfd >= 0)
		{
			if (dup2(nfd, fd) != fd) _exit(EXITMAX);
			(void)close(nfd);
		}
		else (void)close(fd);
	}
}

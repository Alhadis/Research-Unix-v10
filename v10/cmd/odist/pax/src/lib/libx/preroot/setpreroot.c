/*
 * AT&T Bell Laboratories
 * force current command to run under dir preroot
 */

#include <preroot.h>

#if FS_PREROOT

#include <limits.h>
#include <stdio.h>

#ifndef PATH_MAX
#define PATH_MAX	1024
#endif

extern char*	getenv();
extern char*	malloc();
extern char*	pathpath();

void
setpreroot(argv, dir)
register char**	argv;
char*		dir;
{
	register char*	s;
	register char**	ap;
	int		argc;
	char*		cmd;
	char**		av;
	char		buf[PATH_MAX];

	extern char**	opt_argv;

	if ((argv || (argv = opt_argv)) && (dir || (dir = getenv(PR_BASE)) && *dir) && !ispreroot(dir) && (*(cmd = *argv++) == '/' || (cmd = pathpath(buf, cmd, (char*)0, 031))))
	{
		argc = 3;
		for (ap = argv; *ap++; argc++);
		if (av = (char**)malloc(argc * sizeof(char**)))
		{
			ap = av;
			*ap++ = PR_COMMAND;
			*ap++ = dir;
			*ap++ = cmd;
			while (*ap++ = *argv++);
			if (!(s = getenv(PR_SILENT)) || !*s)
			{
				fprintf(stderr, "+");
				ap = av;
				while (s = *ap++)
					fprintf(stderr, " %s", s);
				fprintf(stderr, "\n");
				fflush(stderr);
			}
			execv(*av, av);
			free(av);
		}
	}
}

#else

int	_lib_dummy;

#endif

/*
 * libx popen(3)
 */

#include <stdio.h>

#undef	popen
#undef	pclose

extern int	cmdopen();
extern int	cmdclose();

#if __STDC__
#ifndef fdopen
extern FILE*	fdopen(int, char*);
#endif
#endif

FILE*
popen(cmd, mode)
char*	cmd;
char*	mode;
{
	int	fd;
	FILE*	fp;
	char*	sh[4];

	sh[0] = "sh";
	sh[1] = "-c";
	sh[2] = cmd;
	sh[3] = 0;
	if ((fd = cmdopen((char*)0, sh, (char**)0, (int**)0, mode)) < 0)
		return(0);
	if (!(fp = fdopen(fd, mode)))
		(void)cmdclose(fd);
	return(fp);
}

int
pclose(fp)
FILE*	fp;
{
	int	fd;

	if (fp)
	{
		fd = fileno(fp);
		fclose(fp);
		return(cmdclose(fd));
	}
	return(-1);
}

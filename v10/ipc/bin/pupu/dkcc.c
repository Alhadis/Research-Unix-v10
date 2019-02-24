/*
 *	dkcc -- Remote Execute or Login over DATAKIT VCS to UNIX-V
 */
	static char	SCCSID[] = "@(#)dkrx.c	2.1 DKHOST 84/07/03";

#include <stdio.h>
#include <ipc.h>
#include "sysexits.h"
#include <string.h>

	int		dkverbose = 1;

	char		system[64];
	char		*ttyname();

main(argc, argv)
	char		**argv;
{
	register int	f, exitcode;
	register char	*p, *ttyn;
	int		force, env;
	extern char *miscfield() ;
	char *		cmdname ;

	cmdname = argv[0] ;
	if(p = strrchr(argv[0], '/'))
		strcpy(system, p+1);
	else
		strcpy(system, argv[0]);

	argc--; argv++;

	if(strcmp(argv[0], "-f") == 0){
		force = 1;
		argc--; argv++;
	}else
		force = 0;

	env = 1;

	if(strcmp(system, "dkcc") == 0) {
		if(argc == 0){
			fprintf(stderr, "Usage:  dkcc system [ command [ args ] ]\n");
			exit(EX_USAGE);
		}


		strcpy(system, argv[0]);

		argc--; argv++;
	}


	if(argc > 0)
		strcpy(system, maphost(system, 'x', "rx", "aevx", ""));
	else
		strcpy(system, maphost(system, 'l', "rl", "vx", ""));

	if(p = miscfield((argc > 0) ? 'x' : 'l', 'c'))
		execalt(p, system, argv);

	if(p = miscfield((argc > 0) ? 'x' : 'l', 'v'))
		env = (*p == 'y');

	f = ipcopen (ipcpath (system, "dk", ""), "heavy");
	if (f < 0) {
		fprintf(stderr,
			"%s: call to %s failed, %s\n", cmdname, system, errstr);
		exit (-f);
	}

	exitcode = 0;

	if(env)
		exitcode = dkxenviron(f);

	if(exitcode >= 0 && argc > 0)
		exitcode = sendargs(f, argv, !!miscfield('x', 's'));

	if(exitcode >= 0)
		exitcode = dkxstdio(f);

	if(exitcode == -EX_IOERR && argc == 0)
		exit(0);		/* no exitcode specified */
	else if(exitcode < 0)
		exit(-exitcode);	/* if error processing the call */
	else if(exitcode & 0xFF)
		exit(exitcode | 0x80);	/* remote terminated on signal */
	else
		exit(exitcode>>8);	/* else it finished with this code */
}

execalt(string, system, argv)
	char		*string, *system;
	char		*argv[];
{
	char		**ap, *args[32];

	ap = args;

	do{
		*ap++ = string;

		if(string = strchr(string, ':'))
			*string++ = '\0';
	}while(string);

	*ap++ = system;
	while(*ap++ = *argv++);

	execvp(args[0], args);

	fprintf(stderr, "dk: Can't exec %s\n", args[0]);
	exit(EX_UNAVAILABLE);
}

sendargs(fd, argarray, minusc)
	char		*argarray[];
{
	register int	aoffset;
	char		argstring[2048];

	if(minusc){
		aoffset = 3;
		strcpy(argstring, "-c");
	}else
		aoffset = 0;

	return(dkxlwrite(fd, argstring, bldargs(&argstring[aoffset], argarray) + aoffset));
}

bldargs(args, argv)
	char		*args;
	register char	*argv[];
{
	register char	*s, *t;

	s = args;

	if (t = *argv++) {
		while (*s++ = *t++);

		s[-1] = ' ';
	}
	while(t = *argv++){
		*s++ = '"';

		while(*s++ = *t++);

		s[-1] = '"';
		*s++ = ' ';
	}
	*s++ = '\0';
	return(s - args);
}

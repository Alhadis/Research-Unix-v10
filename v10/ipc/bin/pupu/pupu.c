
	static char	SCCSID[] = "@(#)pupu.c	2.1 DKHOST 85/01/15";

#include	<stdio.h>
#include	<sysexits.h>
#include	<string.h>
#include	<ipc.h>
#include	"tdefs.h"

	XcharP	iam;
int	DEBUG;
	

	Xchar	ebuf[BUFSIZ];

	Efn	printerr;

	int	dkverbose;

extern char * maphost() ;
extern char * miscfield() ;

main(argc, argv)
	PcharP	argv[];
{
	RcharP	p;
	Aint	fd;
	AcharP	dir;

	if (strcmp(&argv[0][strlen(argv[0])-2], "cc") == 0)
		argv[0][strlen(argv[0])-2] = '\0' ;
	iam = &argv[0][strlen(argv[0]) - 4];

	if (strcmp(argv[1], "-d") == 0) {
		DEBUG = 1 ;
		argv++ ; argc-- ;
	} else
		DEBUG = 0 ;
	if(strcmp(iam, "pupu") == 0)
		exit(ppremote(0));

/*	setbuf(stderr, ebuf);/**/

	if(argc < 4){
		fprintf(stderr, "Usage:  %s host file ... directory\n", iam);
		exit(EX_USAGE);
	}

	fd = ipcopen(ipcpath (argv[1], "dk", "pupu"), "heavy delim");

	if(fd < 0) {
		fprintf(stderr, "call to %s failed: %s\n", argv[1], errstr);
		exit(-fd);
	}

	dir = argv[argc - 1];

	argv[argc - 1] = 0;

	if(strcmp(iam, "push") == 0)
		exit(push(fd, &argv[2], dir, printerr, ""));
	else if(strcmp(iam, "pull") == 0)
		exit(pull(fd, &argv[2], dir, printerr, ""));
	else{
		fprintf(stderr, "Command name unrecognized -- must be 'push' or 'pull'\n");
		exit(EX_USAGE);
	}
	/* NOTREACHED */
}

printerr(msg)
	PcharP	msg;
{
	fprintf(stderr, "%s%s\n", iam, msg);

	fflush(stderr);

	return(0);
}

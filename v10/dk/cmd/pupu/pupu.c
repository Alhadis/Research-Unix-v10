
	static char	SCCSID[] = "@(#)pupu.c	2.1 DKHOST 85/01/15";

#include	<stdio.h>
#include	<sysexits.h>
#include	"string.h"
#include	<dk.h>
#include	"tdefs.h"

	XcharP	iam;
int	DEBUG ;
	

	Xchar	ebuf[BUFSIZ];

	Efn	printerr;

extern char * maphost() ;
extern char * miscfield() ;
extern int	dkp_ld ;

main(argc, argv)
	PcharP	argv[];
{
	RcharP	p, dialstring;
	Aint	fd;
	AcharP	dir;
	extern char * dkerror ;

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

	setbuf(stderr, ebuf);

	if(argc < 4){
		fprintf(stderr, "Usage:  %s host file ... directory\n", iam);
		exit(EX_USAGE);
	}

	dialstring = maphost(argv[1], 'f', "pupu", "", "");

	if((p = miscfield('f', 'o')) && *p == 'y')
		execalt(iam, argc, argv);

	fd = tdkdial(dialstring, 2);

	if(fd < 0) {
		fprintf(stderr, "call to %s failed: %s\n", dialstring, dkerror) ;
		exit(-fd);
	}

	dkproto(fd, dkp_ld) ;
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

execalt(name, argc, argv)
	PcharP	name;
	PcharP	argv[];
{
	Achar	oldname[8];

	sprintf(oldname, "o%s", name);
	argv[argc] = 0;

	execvp(oldname, argv);

	fprintf(stderr, "%s: Can't exec %s\n", iam, oldname);
	exit(EX_UNAVAILABLE);
}

printerr(msg)
	PcharP	msg;
{
	fprintf(stderr, "%s%s\n", iam, msg);

	fflush(stderr);

	return(0);
}

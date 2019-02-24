/*	/sccs/src/cmd/uucp/s.uucheck.c
	uucheck.c	1.5	8/30/84 17:37:48
*/
#define UUCHECK
int Uerrors = 0;	/* error count */

/* This unusual include (#include "permission.c") is done because
 * uucheck wants to use the global static variable in permission.c
 */

#include "uucp.h"
VERSION(@(#)uucheck.c	1.5);
#include "permission.c"

/* These are here because uucpdefs.c is not used, and
 * some routines are referenced (never called within uucheck execution)
 * and not included.
 */

#define USAGE	"[-v] [-xNUM]"

int Debug=0;
mkdirs(){}
canPath(){}
char RemSpool[] = SPOOL; /* this is a dummy for chkpth() -- never used here */
char *Spool = SPOOL;
char *Pubdir = PUBDIR;
char *Bnptr;
char	Progname[NAMESIZE];
/* used for READANY and READSOME macros */
struct stat __s_;

/* This is stuff for uucheck */

struct tab
   {
    char *name;
    char *value;
   } tab[] =
   {
#ifdef	CORRUPTDIR
    "CORRUPT",	CORRUPTDIR,
#endif
    "LOGUUCP",	LOGUUCP,
    "LOGUUX",	LOGUUX,
    "LOGUUXQT",	LOGUUXQT,
    "LOGCICO",	LOGCICO,
    "SEQDIR",	SEQDIR,
    "STATDIR",	STATDIR,
    "PFILE",	PFILE,
#ifndef MANYSYS
    "SYSFILE",	SYSFILE,
#endif
    "DEVFILE",	DEVFILE	,
    "DIALFILE",	DIALFILE,
    "DIALERFILE",	DIALERFILE,
#ifdef	USRSPOOLLOCKS
    "USRSPOOLLOCKS",	"/usr/spool/locks",
#endif
#ifdef	NOSTRANGERS
    "NOSTRANGERS",	NOSTRANGERS,
#endif
    "LMTUUXQT",	LMTUUXQT, /* if not defined we'll stat NULL, it's not a bug */
    "LMTUUSCHED",	LMTUUSCHED, /* if not defined we'll stat NULL, it's not a bug */
    "XQTDIR",	XQTDIR,
    "WORKSPACE",	WORKSPACE,
    "admin directory",	ADMIN,
    NULL,
   };

extern char *nextarg();
int verbose = 0;	/* fsck-like verbosity */

main(argc, argv)
char *argv[];
{
    struct stat statbuf;
    struct tab *tabptr;
    int i;

	(void) strcpy(Progname, "uucheck");
	while ((i = getopt(argc, argv, "vx:")) != EOF) {
		switch(i){

		case 'v':
			verbose++;
			break;

		case 'x':
			Debug = atoi(optarg);
			if (Debug <= 0)
				Debug = 1;
			break;

		default:
			(void) fprintf(stderr, "\tusage: %s %s\n",
			    Progname, USAGE);
			exit(1);
		}
	}
	if (argc != optind) {
		(void) fprintf(stderr, "\tusage: %s %s\n", Progname, USAGE);
		exit(1);
	}

    if (verbose) printf("*** uucheck:  Check Required Files and Directories\n");
    for (tabptr = tab; tabptr->name != NULL; tabptr++) {
        if (stat(tabptr->value, &statbuf) < 0) { 
	    fprintf(stderr, "%s - ", tabptr->name);
	    perror(tabptr->value);
	    Uerrors++;
	}
    }
#if MANYSYS
	if (badsys())
		Uerrors++;
#endif

    if (verbose) printf("*** uucheck:  Directories Check Complete\n\n");

    /* check the permissions file */

    if (verbose) printf("*** uucheck:  Check %s file\n", PFILE);
    Uerrors += checkPerm();
    if (verbose) printf("*** uucheck:  %s Check Complete\n\n", PFILE);

    exit(Uerrors);

}

#if MANYSYS

badsys()
{
	FILE *fp;
	FILE *sysopen();
	int n;

	for (n = 0; (fp = sysopen("r")) != NULL; n++)
		fclose(fp);
	if (n)
		return (0);
	fprintf(stderr, "No readable Systems files\n");
	return (0);
}

#endif

char *Name[] = {
"U_LOGNAME", 
"U_MACHINE", 
"U_CALLBACK", 
"U_REQUEST", 
"U_SENDFILES", 
"U_READPATH", 
"U_WRITEPATH", 
"U_NOREADPATH", 
"U_NOWRITEPATH", 
"U_PROTOCOL", 
"U_COMMANDS",
"U_VALIDATE",
};

int
checkPerm ()
{
    int type;
    int error=0;
    char defaults[BUFSIZ];

    for (type=0; type<2; type++) {
	/* type = 0 for LOGNAME, 1 for MACHINE */

	if (verbose) printf("** %s \n\n",
	    type == U_MACHINE
		?"MACHINE PHASE (when we call or execute their uux requests)"
		:"LOGNAME PHASE (when they call us)" );

	Fp = fopen(PFILE, "r");
	if (Fp == NULL) {
		if (verbose) printf("can't open %s\n", PFILE);
		exit(1);
	}

	for (;;) {
	    if (parse_tokens (_Flds) != 0) {
		fclose(Fp);
		break;
	    }
	    if (_Flds[type] == NULL)
	        continue;

	    fillFlds();
	    /* if no ReadPath set num to 1--Path already set */
	    fillList(U_READPATH, _RPaths);
	    fillList(U_WRITEPATH, _WPaths);
	    fillList(U_NOREADPATH, _NoRPaths);
	    fillList(U_NOWRITEPATH, _NoWPaths);
	    if (_Flds[U_COMMANDS] == NULL) {
		strcpy(defaults, DEFAULTCMDS);
		_Flds[U_COMMANDS] = defaults;
	    }
	    fillList(U_COMMANDS, _Commands);
	    error += outLine(type);
	}
    if (verbose) printf("\n");
    }
    return(error);
}

int
outLine(type)
int type;
{
	register int i;
	register char *p;
	char *arg, cmd[BUFSIZ];
	int error = 0;
	char myname[MAXBASENAME+1];

	if (_Flds[type][0] == 0)
	    return(0);

	if (type == U_LOGNAME) { /* for LOGNAME */
	    p = _Flds[U_LOGNAME];
	    if (verbose) printf("When a system logs in as: ");
	    while (*p != '\0') {
		p = nextarg(p, &arg);
		if (verbose) 	printf("(%s) ", arg);
	    }
	    if (verbose) printf("\n");

	    if (callBack()) {
		if (verbose) printf("\tWe will call them back.\n\n");
		return(0);
	    }
	}
	else {	/* MACHINE */
	    p = _Flds[U_MACHINE];
	    if (verbose) printf("When we call system(s): ");
	    while (*p != '\0') {
		p = nextarg(p, &arg);
		if (verbose) printf("(%s) ", arg);
	    }
	    if (verbose) printf("\n");

	}

	if (verbose) printf("\tWe %s allow them to request files.\n",
	    requestOK()? "DO" : "DO NOT");

	if (type == U_LOGNAME) {
		if (verbose) printf("\tWe %s send files queued for them on this call.\n",
		    switchRole()? "WILL" : "WILL NOT");
	}

	if (verbose) printf("\tThey can send files to\n");
	if (_Flds[U_WRITEPATH] == NULL) {
	    if (verbose) printf("\t    %s (DEFAULT)\n", Pubdir);
	}
	else {
	    for (i=0; _WPaths[i] != NULL; i++)
		if (verbose) printf("\t    %s\n", _WPaths[i]);
	}

	if (_Flds[U_NOWRITEPATH] != NULL) {
	    if (verbose) printf("\tExcept\n");
	    for (i=0; _NoWPaths[i] != NULL; i++)
		if (verbose) printf("\t    %s\n", _NoWPaths[i]);
	}

	if (requestOK()) {
	    if (verbose) printf("\tThey can request files from\n");
	    if (_Flds[U_READPATH] == NULL) {
		if (verbose) printf("\t    %s (DEFAULT)\n", Pubdir);
	    }
	    else {
		for (i=0; _RPaths[i] != NULL; i++)
		    if (verbose) printf("\t    %s\n", _RPaths[i]);
	    }

	    if (_Flds[U_NOREADPATH] != NULL) {
		if (verbose) printf("\tExcept\n");
		for (i=0; _NoRPaths[i] != NULL; i++)
		    if (verbose) printf("\t    %s\n", _NoRPaths[i]);
	    }
	}

	myName(myname);
	if (verbose) printf("\tMyname for the conversation will be %s.\n",
	    myname);
	if (verbose) printf("\tPUBDIR for the conversation will be %s.\n",
	    Pubdir);

	if (verbose) printf("\n");

	if (type == U_MACHINE) {
	    if (verbose) printf("Machine(s): ");
	    p = _Flds[U_MACHINE];
	    while (*p != '\0') {
		p = nextarg(p, &arg);
		if (verbose) printf("(%s) ", arg);
	    }
	    if (verbose) printf("\nCAN execute the following commands:\n");
	    for (i=0; _Commands[i] != NULL; i++) {
		if (cmdOK(BASENAME(_Commands[i], '/'), cmd) == FALSE) {
		    if (verbose) printf("Software Error in permission.c\n");
		    error++;
		}
		if (verbose) printf("command (%s), fullname (%s)\n",
		    BASENAME(_Commands[i], '/'), cmd);
	    }
	    if (verbose) printf("\n");
	}

	return(error);
}

/* 
 * tcl.c --
 *
 *	Test driver for TCL.
 *
 * Copyright 1987 Regents of the University of California
 * All rights reserved.
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies.  The University of California
 * makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 */

#ifndef lint
static char rcsid[] = "$Header: /sprite/src/lib/tcl/tclTest/RCS/tclTest.c,v 1.8 90/03/23 16:19:27 ouster Exp $ SPRITE (Berkeley)";
#endif not lint

#include <stdio.h>
#include <sys/time.h>
#include "tcl.h"

Tcl_Interp *interp;
Tcl_CmdBuf buffer;

int
cmdEcho(clientData, interp, argc, argv)
    char *clientData;
    Tcl_Interp *interp;
    int argc;
    char **argv;
{
    int i;

    for (i = 1; ; i++) {
	if (argv[i] == NULL) {
	    if (i != argc) {
		echoError:
		sprintf(interp->result,
		    "argument list wasn't properly NULL-terminated in \"%s\" command",
		    argv[0]);
	    }
	    break;
	}
	if (i >= argc) {
	    goto echoError;
	}
	fputs(argv[i], stdout);
	if (i < (argc-1)) {
	    printf(" ");
	}
    }
    printf("\n");
    return TCL_OK;
}

void
deleteProc(clientData)
    char *clientData;
{
    printf("Deleting command with clientData \"%s\".\n", clientData);
}

int
cmdCreate(clientData, interp, argc, argv)
    ClientData clientData;		/* Not used. */
    Tcl_Interp *interp;
    int argc;
    int *argv;
{
    int count;
    if (argc != 2) {
	sprintf(interp->result, "wrong # args:  should be \"%.50s count\"",
		argv[0]);
	return TCL_ERROR;
    }
    count = atoi(argv[1]);
    for (; count > 0; count--) {
	Tcl_DeleteInterp(Tcl_CreateInterp());
    }
    return TCL_OK;
}

main()
{
    char line[1000], *cmd;
    int result, gotPartial;

    interp = Tcl_CreateInterp();
    Tcl_CreateCommand(interp, "echo", cmdEcho, (ClientData) "echo",
	    deleteProc);
    Tcl_CreateCommand(interp, "create", cmdCreate, (ClientData) "create",
	    deleteProc);
    buffer = Tcl_CreateCmdBuf();

    gotPartial = 0;
    while (1) {
	clearerr(stdin);
	if (!gotPartial) {
	    fputs("% ", stdout);
	    fflush(stdout);
	}
	if (fgets(line, 1000, stdin) == NULL) {
	    if (!gotPartial) {
		exit(0);
	    }
	    line[0] = 0;
	}
	cmd = Tcl_AssembleCmd(buffer, line);
	if (cmd == NULL) {
	    gotPartial = 1;
	    continue;
	}

	gotPartial = 0;
	result = Tcl_RecordAndEval(interp, cmd, 0);
	if (result == TCL_OK) {
	    if (*interp->result != 0) {
		printf("%s\n", interp->result);
	    }
	} else {
	    if (result == TCL_ERROR) {
		printf("Error");
	    } else {
		printf("Error %d", result);
	    }
	    if (*interp->result != 0) {
		printf(": %s\n", interp->result);
	    } else {
		printf("\n");
	    }
	}
    }
}

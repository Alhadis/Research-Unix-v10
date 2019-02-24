/* 
 * tclCmdIZ.c --
 *
 *	This file contains the top-level command routines for most of
 *	the Tcl built-in commands whose names begin with the letters
 *	I to Z.
 *
 * Copyright 1987 Regents of the University of California
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies.  The University of California
 * makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 */

#ifndef lint
static char rcsid[] = "$Header: /sprite/src/lib/tcl/RCS/tclCmdIZ.c,v 1.36 90/04/18 17:09:07 ouster Exp $ SPRITE (Berkeley)";
#pragma ref rcsid
#endif not lint

#define	_POSIX_SOURCE

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/times.h>
#include "tclInt.h"

/*
 *----------------------------------------------------------------------
 *
 * Tcl_IfCmd --
 *
 *	This procedure is invoked to process the "if" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_IfCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
#pragma ref dummy
    char *condition, *ifPart, *elsePart, *cmd, *name;
    int result, value;

    name = argv[0];
    if (argc < 3) {
	ifSyntax:
	sprintf(interp->result, "wrong # args:  should be \"%.50s bool [then] command [[else] command]\"",
		name);
	return TCL_ERROR;
    }
    condition = argv[1];
    argc -= 2;
    argv += 2;
    if ((**argv == 't') && (strncmp(*argv, "then", strlen(*argv)) == 0)) {
	argc--;
	argv++;
    }
    if (argc < 1) {
	goto ifSyntax;
    }
    ifPart = *argv;
    argv++;
    argc--;
    if (argc == 0) {
	elsePart = "";
    } else {
	if ((**argv == 'e') && (strncmp(*argv, "else", strlen(*argv)) == 0)) {
	    argc--;
	    argv++;
	}
	if (argc != 1) {
	    goto ifSyntax;
	}
	elsePart = *argv;
    }

    cmd = ifPart;
    result = Tcl_Expr(interp, condition, &value);
    if (result != TCL_OK) {
	return result;
    }
    if (value == 0) {
	cmd = elsePart;
    }
    result = Tcl_Eval(interp, cmd, 0, (char **) NULL);
    if (result == TCL_ERROR) {
	char msg[60];
	sprintf(msg, " (\"if\" body line %d)", interp->errorLine);
	Tcl_AddErrorInfo(interp, msg);
    }
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_IndexCmd --
 *
 *	This procedure is invoked to process the "index" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

    /* ARGSUSED */
int
Tcl_IndexCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
#pragma ref dummy
    char *p, *element;
    int index, size, parenthesized, result;

    if (argc < 3) {
	indexSyntax:
	sprintf(interp->result,
		"wrong # args:  should be \"%.50s value index [chars]\"",
		argv[0]);
	return TCL_ERROR;
    }
    p = argv[1];
    index = atoi(argv[2]);
    if (!isdigit(*argv[2]) || (index < 0)) {
	sprintf(interp->result, "bad index \"%.50s\"", argv[2]);
	return TCL_ERROR;
    }
    if (argc == 3) {
	for ( ; index >= 0; index--) {
	    result = TclFindElement(interp, p, &element, &p, &size,
		    &parenthesized);
	    if (result != TCL_OK) {
		return result;
	    }
	}
	if (size >= TCL_RESULT_SIZE) {
	    interp->result = (char *) malloc((unsigned) size+1);
	    interp->dynamic = 1;
	}
	if (parenthesized) {
	    bcopy(element, interp->result, size);
	    interp->result[size] = 0;
	} else {
	    TclCopyAndCollapse(size, element, interp->result);
	}
    } else if (argc == 4) {
	if (strncmp(argv[3], "chars", strlen(argv[3])) != 0) {
	    sprintf(interp->result, "bad argument \"%s\":  must be \"chars\"",
		    argv[3]);
	    return TCL_ERROR;
	}
	size = strlen(p);
	if (index < size) {
	    interp->result[0] = p[index];
	    interp->result[1] = 0;
	}
    } else {
	goto indexSyntax;
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_InfoCmd --
 *
 *	This procedure is invoked to process the "info" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_InfoCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
#pragma ref dummy
    register Interp *iPtr = (Interp *) interp;
    Proc *procPtr;
    Var *varPtr;
    Command *cmdPtr;
    int length;
    char c;

    /*
     * When collecting a list of things (e.g. args or vars) "flag" tells
     * what kind of thing is being collected, according to the definitions
     * below.
     */

    int flag;
#   define VARS 0
#   define LOCALS 1
#   define PROCS 2
#   define CMDS 3

#   define ARG_SIZE 20
    char *argSpace[ARG_SIZE];
    int argSize;
    char *pattern;

    if (argc < 2) {
	sprintf(iPtr->result,
		"too few args:  should be \"%.50s option [arg arg ...]\"",
		argv[0]);
	return TCL_ERROR;
    }
    c = argv[1][0];
    length = strlen(argv[1]);
    if ((c == 'a') && (strncmp(argv[1], "args", length)) == 0) {
	if (argc != 3) {
	    sprintf(iPtr->result,
		    "wrong # args: should be \"%.50s args procname\"",
		    argv[0]);
	    return TCL_ERROR;
	}
	procPtr = TclFindProc(iPtr, argv[2]);
	if (procPtr == NULL) {
	    infoNoSuchProc:
	    sprintf(iPtr->result,
		    "info requested on \"%s\", which isn't a procedure",
		    argv[2]);
	    return TCL_ERROR;
	}
	flag = VARS;
	varPtr = procPtr->argPtr;
	argc = 0;			/* Prevent pattern matching. */
    } else if ((c == 'b') && (strncmp(argv[1], "body", length)) == 0) {
	if (argc != 3) {
	    sprintf(iPtr->result,
		    "wrong # args: should be \"%.50s body procname\"",
		    argv[0]);
	    return TCL_ERROR;
	}
	procPtr = TclFindProc(iPtr, argv[2]);
	if (procPtr == NULL) {
	    goto infoNoSuchProc;
	}
	iPtr->result = procPtr->command;
	return TCL_OK;
    } else if ((c == 'c') && (strncmp(argv[1], "cmdcount", length) == 0)
	    && (length >= 2)) {
	if (argc != 2) {
	    sprintf(iPtr->result,
		    "wrong # args: should be \"%.50s cmdcount\"",
		    argv[0]);
	    return TCL_ERROR;
	}
	sprintf(iPtr->result, "%d", iPtr->cmdCount);
	return TCL_OK;
    } else if ((c == 'c') && (strncmp(argv[1], "commands", length) == 0)
	    && (length >= 2)){
	if (argc > 3) {
	    sprintf(iPtr->result,
		    "wrong # args: should be \"%.50s commands [pattern]\"",
		    argv[0]);
	    return TCL_ERROR;
	}
	flag = CMDS;
	cmdPtr = iPtr->commandPtr;
    } else if ((c == 'd') && (strncmp(argv[1], "default", length)) == 0) {
	if (argc != 5) {
	    sprintf(iPtr->result, "wrong # args: should be \"%.50s default procname arg varname\"",
		    argv[0]);
	    return TCL_ERROR;
	}
	procPtr = TclFindProc(iPtr, argv[2]);
	if (procPtr == NULL) {
	    goto infoNoSuchProc;
	}
	for (varPtr = procPtr->argPtr; ; varPtr = varPtr->nextPtr) {
	    if (varPtr == NULL) {
		sprintf(iPtr->result,
			"procedure \"%s\" doesn't have an argument \"%s\"",
			argv[2], argv[3]);
		return TCL_ERROR;
	    }
	    if (strcmp(argv[3], varPtr->name) == 0) {
		if (varPtr->value != NULL) {
		    Tcl_SetVar((Tcl_Interp *) iPtr, argv[4], varPtr->value, 0);
		    iPtr->result = "1";
		} else {
		    Tcl_SetVar((Tcl_Interp *) iPtr, argv[4], "", 0);
		    iPtr->result = "0";
		}
		return TCL_OK;
	    }
	}
    } else if ((c == 'e') && (strncmp(argv[1], "exists", length) == 0)) {
	char *p;
	if (argc != 3) {
	    sprintf(iPtr->result,
		    "wrong # args: should be \"%.50s exists varName\"",
		    argv[0]);
	    return TCL_ERROR;
	}
	p = Tcl_GetVar((Tcl_Interp *) iPtr, argv[2], 0);
	if (p != NULL) {
	    iPtr->result[0] = '1';
	} else {
	    iPtr->result[0] = '0';
	}
	iPtr->result[1] = 0;
	return TCL_OK;
    } else if ((c == 'g') && (strncmp(argv[1], "globals", length) == 0)) {
	if (argc > 3) {
	    sprintf(iPtr->result,
		    "wrong # args: should be \"%.50s globals [pattern]\"",
		    argv[0]);
	    return TCL_ERROR;
	}
	flag = VARS;
	varPtr = iPtr->globalPtr;
    } else if ((c == 'l') && (strncmp(argv[1], "locals", length) == 0)
	     && (length >= 2)) {
	if (argc > 3) {
	    sprintf(iPtr->result,
		    "wrong # args: should be \"%.50s locals [pattern]\"",
		    argv[0]);
	    return TCL_ERROR;
	}
	flag = LOCALS;
	if (iPtr->varFramePtr == NULL) {
	    varPtr = NULL;
	} else {
	    varPtr = iPtr->varFramePtr->varPtr;
	}
    } else if ((c == 'l') && (strncmp(argv[1], "level", length) == 0)
	    && (length >= 2)) {
	if (argc == 2) {
	    if (iPtr->varFramePtr == NULL) {
		iPtr->result = "0";
	    } else {
		sprintf(iPtr->result, "%d", iPtr->varFramePtr->level);
	    }
	    return TCL_OK;
	} else if (argc == 3) {
	    int level;
	    char *end;
	    CallFrame *framePtr;

	    level = strtol(argv[2], &end, 10);
	    if ((end == argv[2]) || (*end != '\0')) {
		levelError:
		sprintf(iPtr->result, "bad level \"%.50s\"", argv[1]);
		return TCL_ERROR;
	    }
	    if (level <= 0) {
		if (iPtr->varFramePtr == NULL) {
		    goto levelError;
		}
		level += iPtr->varFramePtr->level;
	    }
	    if (level == 0) {
		return TCL_OK;
	    }
	    for (framePtr = iPtr->varFramePtr; framePtr != NULL;
		    framePtr = framePtr->callerVarPtr) {
		if (framePtr->level == level) {
		    break;
		}
	    }
	    if (framePtr == NULL) {
		goto levelError;
	    }
	    iPtr->result = Tcl_Merge(framePtr->argc, framePtr->argv);
	    iPtr->dynamic = 1;
	    return TCL_OK;
	}
	sprintf(iPtr->result,
		"wrong # args: should be \"%.50s level [number]\"",
		argv[0]);
	return TCL_ERROR;
    } else if ((c == 'p') && (strncmp(argv[1], "procs", length)) == 0) {
	if (argc > 3) {
	    sprintf(iPtr->result,
		    "wrong # args: should be \"%.50s procs [pattern]\"",
		    argv[0]);
	    return TCL_ERROR;
	}
	flag = PROCS;
	cmdPtr = iPtr->commandPtr;
    } else if ((c == 't') && (strncmp(argv[1], "tclversion", length) == 0)) {

	/*
	 * Note:  TCL_VERSION below is expected to be set with a "-D"
	 * switch in the Makefile.
	 */

	strcpy(iPtr->result, TCL_VERSION);
	return TCL_OK;
    } else if ((c == 'v') && (strncmp(argv[1], "vars", length)) == 0) {
	if (argc > 3) {
	    sprintf(iPtr->result,
		    "wrong # args: should be \"%.50s vars [pattern]\"",
		    argv[0]);
	    return TCL_ERROR;
	}
	flag = VARS;
	if (iPtr->varFramePtr == NULL) {
	    varPtr = iPtr->globalPtr;
	} else {
	    varPtr = iPtr->varFramePtr->varPtr;
	}
    } else {
	sprintf(iPtr->result, "bad \"%.50s\" option \"%.50s\": must be args, body, commands, cmdcount, default, exists, globals, level, locals, procs, tclversion, or vars",
		argv[0], argv[1]);
	return TCL_ERROR;
    }

    /*
     * At this point we have to assemble a list of something or other.
     * Collect them in an expandable argv-argc array.
     */

    if (argc == 3) {
	pattern = argv[2];
    } else {
	pattern = NULL;
    }
    argv = argSpace;
    argSize = ARG_SIZE;
    argc = 0;
    while (1) {
	/*
	 * Increase the size of the argument array if necessary to
	 * accommodate another string.
	 */

	if (argc == argSize) {
	    char **newArgs;

	    argSize *= 2;
	    newArgs = (char **) malloc((unsigned) argSize*sizeof(char *));
	    bcopy((char *) argv, (char *) newArgs, argc*sizeof(char *));
	    if (argv != argSpace) {
		free((char *) argv);
	    }
	    argv = newArgs;
	}

	if ((flag == PROCS) || (flag == CMDS)) {
	    if (flag == PROCS) {
		for ( ; cmdPtr != NULL; cmdPtr = cmdPtr->nextPtr) {
		    if (TclIsProc(cmdPtr)) {
			break;
		    }
		}
	    }
	    if (cmdPtr == NULL) {
		break;
	    }
	    argv[argc] = cmdPtr->name;
	    cmdPtr = cmdPtr->nextPtr;
	} else {
	    if (flag == LOCALS) {
		for ( ; varPtr != NULL; varPtr = varPtr->nextPtr) {
		    if (!(varPtr->flags & VAR_GLOBAL)) {
			break;
		    }
		}
	    }
	    if (varPtr == NULL) {
		break;
	    }
	    argv[argc] = varPtr->name;
	    varPtr = varPtr->nextPtr;
	}
	if ((pattern == NULL)  || Tcl_StringMatch(argv[argc], pattern)) {
	    argc++;
	}
    }

    iPtr->result = Tcl_Merge(argc, argv);
    iPtr->dynamic = 1;
    if (argv != argSpace) {
	free((char *) argv);
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_LengthCmd --
 *
 *	This procedure is invoked to process the "length" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_LengthCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
#pragma ref dummy
    int count;
    char *p;

    if (argc < 2) {
	lengthSyntax:
	sprintf(interp->result,
		"wrong # args: should be \"%.50s value [chars]\"", argv[0]);
	return TCL_ERROR;
    }
    p = argv[1];
    if (argc == 2) {
	char *element;
	int result;

	for (count = 0; *p != 0 ; count++) {
	    result = TclFindElement(interp, p, &element, &p, (int *) NULL,
		    (int *) NULL);
	    if (result != TCL_OK) {
		return result;
	    }
	    if (*element == 0) {
		break;
	    }
	}
    } else if ((argc == 3)
	    && (strncmp(argv[2], "chars", strlen(argv[2])) == 0)) {
	count = strlen(p);
    } else {
	goto lengthSyntax;
    }
    sprintf(interp->result, "%d", count);
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_ListCmd --
 *
 *	This procedure is invoked to process the "list" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_ListCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
#pragma ref dummy
    if (argc < 2) {
	sprintf(interp->result,
		"not enough args:  should be \"%.50s arg [arg ...]\"",
		argv[0]);
	return TCL_ERROR;
    }
    interp->result = Tcl_Merge(argc-1, argv+1);
    interp->dynamic = 1;
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_PrintCmd --
 *
 *	This procedure is invoked to process the "print" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_PrintCmd(notUsed, interp, argc, argv)
    ClientData notUsed;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
#pragma ref notUsed
    FILE *f;
    int result;

    if ((argc < 2) || (argc > 4)) {
	sprintf(interp->result,
		"wrong # args: should be \"%.50s string [file [append]]\"",
		argv[0]);
	return TCL_ERROR;
    }

    if (argc == 2) {
	f = stdout;
    } else {
	if (argc == 4) {
	    if (strncmp(argv[3], "append", strlen(argv[3])) != 0) {
		sprintf(interp->result,
			"bad option \"%.50s\":  must be \"append\"",
			argv[3]);
		return TCL_ERROR;
	    }
	    f = fopen(argv[2], "a");
	} else {
	    f = fopen(argv[2], "w");
	}
	if (f == NULL) {
	    sprintf(interp->result, "couldn't open \"%.50s\": %.80s",
		    argv[2], strerror(errno));
	    return TCL_ERROR;
	}
    }
    fputs(argv[1], f);
    if (argc == 2) {
	result = fflush(stdout);
    } else {
	result = fclose(f);
    }
    if (result == EOF) {
	sprintf(interp->result, "I/O error while writing: %.50s",
		strerror(errno));
	return TCL_ERROR;
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_RangeCmd --
 *
 *	This procedure is invoked to process the "range" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_RangeCmd(notUsed, interp, argc, argv)
    ClientData notUsed;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
#pragma ref notUsed
    int first, last, result;
    char *begin, *end, c, *dummy;
    int count;

    if (argc < 4) {
	rangeSyntax:
	sprintf(interp->result, "wrong #/type of args: should be \"%.50s value first last [chars]\"",
		argv[0]);
	return TCL_ERROR;
    }
    first = atoi(argv[2]);
    if (!isdigit(*argv[2]) || (first < 0)) {
	sprintf(interp->result, "bad range specifier \"%.50s\"", argv[2]);
	return TCL_ERROR;
    }
    if ((*argv[3] == 'e') && (strncmp(argv[3], "end", strlen(argv[3])) == 0)) {
	last = -1;
    } else {
	last = atoi(argv[3]);
	if (!isdigit(*argv[3]) || (last < 0)) {
	    sprintf(interp->result, "bad range specifier \"%.50s\"", argv[3]);
	    return TCL_ERROR;
	}
    }

    if (argc == 5) {
	count = strlen(argv[4]);
	if ((count == 0) || (strncmp(argv[4], "chars", count) != 0)) {
	    goto rangeSyntax;
	}

	/*
	 * Extract a range of characters.
	 */

	count = strlen(argv[1]);
	if (first >= count) {
	    interp->result = "";
	    return TCL_OK;
	}
	begin = argv[1] + first;
	if ((last == -1) || (last >= count)) {
	    last = count;
	} else if (last < first) {
	    interp->result = "";
	    return TCL_OK;
	}
	end = argv[1] + last + 1;
    } else {
	if (argc != 4) {
	    goto rangeSyntax;
	}

	/*
	 * Extract a range of fields.
	 */

	for (count = 0, begin = argv[1]; count < first; count++) {
	    result = TclFindElement(interp, begin, &dummy, &begin, (int *) NULL,
		    (int *) NULL);
	    if (result != TCL_OK) {
		return result;
	    }
	    if (*begin == 0) {
		break;
	    }
	}
	if (last == -1) {
	    Tcl_Return(interp, begin, TCL_VOLATILE);
	    return TCL_OK;
	}
	if (last < first) {
	    interp->result = "";
	    return TCL_OK;
	}
	for (count = first, end = begin; (count <= last) && (*end != 0);
		count++) {
	    result = TclFindElement(interp, end, &dummy, &end, (int *) NULL,
		    (int *) NULL);
	    if (result != TCL_OK) {
		return result;
	    }
	}

	/*
	 * Chop off trailing spaces.
	 */

	while (isspace(end[-1])) {
	    end--;
	}
    }
    c = *end;
    *end = 0;
    Tcl_Return(interp, begin, TCL_VOLATILE);
    *end = c;
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_RenameCmd --
 *
 *	This procedure is invoked to process the "rename" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_RenameCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
#pragma ref dummy
    register Command *oldPtr, *newPtr;
    Interp *iPtr = (Interp *) interp;

    if (argc != 3) {
	sprintf(interp->result,
		"wrong # args: should be \"%.50s oldName newName\"",
		argv[0]);
	return TCL_ERROR;
    }
    if (argv[2][0] == '\0') {
	Tcl_DeleteCommand(interp, argv[1]);
	return TCL_OK;
    }
    newPtr = TclFindCmd(iPtr, argv[2], 0);
    if (newPtr != NULL) {
	sprintf(interp->result, "can't rename to \"%.50s\": already exists",
		argv[2]);
	return TCL_ERROR;
    }
    oldPtr = TclFindCmd(iPtr, argv[1], 0);
    if (oldPtr == NULL) {
	sprintf(interp->result,
		"can't rename \"%.50s\":  command doesn't exist",
		argv[1]);
	return TCL_ERROR;
    }
    iPtr->commandPtr = oldPtr->nextPtr;
    newPtr = (Command *) malloc(CMD_SIZE(strlen(argv[2])));
    newPtr->proc = oldPtr->proc;
    newPtr->clientData = oldPtr->clientData;
    newPtr->deleteProc = oldPtr->deleteProc;
    newPtr->nextPtr = iPtr->commandPtr;
    iPtr->commandPtr = newPtr;
    strcpy(newPtr->name, argv[2]);
    free((char *) oldPtr);
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_ReturnCmd --
 *
 *	This procedure is invoked to process the "return" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_ReturnCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
#pragma ref dummy
    if (argc > 2) {
	sprintf(interp->result, "too many args: should be \"%.50s [value]\"",
		argv[0]);
	return TCL_ERROR;
    }
    if (argc == 2) {
	Tcl_Return(interp, argv[1], TCL_VOLATILE);
    }
    return TCL_RETURN;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_ScanCmd --
 *
 *	This procedure is invoked to process the "scan" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_ScanCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
#pragma ref dummy
    int arg1Length;			/* Number of bytes in argument to be
					 * scanned.  This gives an upper limit
					 * on string field sizes. */
#   define MAX_FIELDS 20
    typedef struct {
	char fmt;			/* Format for field. */
	int size;			/* How many bytes to allow for
					 * field. */
	char *location;			/* Where field will be stored. */
    } Field;
    Field fields[MAX_FIELDS];		/* Info about all the fields in the
					 * format string. */
    register Field *curField;
    int numFields = 0;			/* Number of fields actually
					 * specified. */
    int suppress;			/* Current field is assignment-
					 * suppressed. */
    int totalSize = 0;			/* Number of bytes needed to store
					 * all results combined. */
    char *results;			/* Where scanned output goes.  */
    int numScanned;			/* sscanf's result. */
    register char *fmt;
    int i;

    if (argc < 3) {
	sprintf(interp->result,
		"too few args: should be \"%.50s string format varName ...\"",
		argv[0]);
	return TCL_ERROR;
    }

    /*
     * This procedure operates in four stages:
     * 1. Scan the format string, collecting information about each field.
     * 2. Allocate an array to hold all of the scanned fields.
     * 3. Call sscanf to do all the dirty work, and have it store the
     *    parsed fields in the array.
     * 4. Pick off the fields from the array and assign them to variables.
     */

    arg1Length = (strlen(argv[1]) + 4) & ~03;
    for (fmt = argv[2]; *fmt != 0; fmt++) {
	if (*fmt != '%') {
	    continue;
	}
	fmt++;
	if (*fmt == '*') {
	    suppress = 1;
	    fmt++;
	} else {
	    suppress = 0;
	}
	while (isdigit(*fmt)) {
	    fmt++;
	}
	if (suppress) {
	    continue;
	}
	if (numFields == MAX_FIELDS) {
	    sprintf(interp->result,
		    "can't have more than %d fields in \"%.50s\"", MAX_FIELDS,
		    argv[0]);
	    return TCL_ERROR;
	}
	curField = &fields[numFields];
	numFields++;
	switch (*fmt) {
	    case 'D':
	    case 'O':
	    case 'X':
	    case 'd':
	    case 'o':
	    case 'x':
		curField->fmt = 'd';
		curField->size = sizeof(int);
		break;

	    case 's':
		curField->fmt = 's';
		curField->size = arg1Length;
		break;

	    case 'c':
		curField->fmt = 'c';
		curField->size = sizeof(int);
		break;

	    case 'E':
	    case 'F':
		curField->fmt = 'F';
		curField->size = 8;
		break;

	    case 'e':
	    case 'f':
		curField->fmt = 'f';
		curField->size = 4;
		break;

	    case '[':
		curField->fmt = 's';
		curField->size = arg1Length;
		do {
		    fmt++;
		} while (*fmt != ']');
		break;

	    default:
		sprintf(interp->result, "bad scan conversion character \"%c\"",
			*fmt);
		return TCL_ERROR;
	}
	totalSize += curField->size;
    }

    if (numFields != (argc-3)) {
	interp->result =
		"different numbers of variable names and field specifiers";
	return TCL_ERROR;
    }

    /*
     * Step 2:
     */

    results = (char *) malloc((unsigned) totalSize);
    for (i = 0, totalSize = 0, curField = fields;
	    i < numFields; i++, curField++) {
	curField->location = results + totalSize;
	totalSize += curField->size;
    }

    /*
     * Step 3:
     */

    numScanned = sscanf(argv[1], argv[2],
	    fields[0].location, fields[1].location, fields[2].location,
	    fields[3].location, fields[4].location);

    /*
     * Step 4:
     */

    if (numScanned < numFields) {
	numFields = numScanned;
    }
    for (i = 0, curField = fields; i < numFields; i++, curField++) {
	switch (curField->fmt) {
	    char string[30];

	    case 'd':
		sprintf(string, "%d", *((int *) curField->location));
		Tcl_SetVar(interp, argv[i+3], string, 0);
		break;

	    case 'c':
		sprintf(string, "%d", *((char *) curField->location) & 0xff);
		Tcl_SetVar(interp, argv[i+3], string, 0);
		break;

	    case 's':
		Tcl_SetVar(interp, argv[i+3], curField->location, 0);
		break;

	    case 'F':
		sprintf(string, "%g", *((double *) curField->location));
		Tcl_SetVar(interp, argv[i+3], string, 0);
		break;

	    case 'f':
		sprintf(string, "%g", *((float *) curField->location));
		Tcl_SetVar(interp, argv[i+3], string, 0);
		break;
	}
    }
    free(results);
    sprintf(interp->result, "%d", numScanned);
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_SourceCmd --
 *
 *	This procedure is invoked to process the "source" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_SourceCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
#pragma ref dummy
    int fileId, result;
    struct stat statBuf;
    char *cmdBuffer, *end;
    char *fileName;

    if (argc != 2) {
	sprintf(interp->result, "wrong # args: should be \"%.50s fileName\"",
		argv[0]);
	return TCL_ERROR;
    }
    fileName = Tcl_TildeSubst(interp, argv[1]);
    if (fileName == NULL) {
	return TCL_ERROR;
    }
    fileId = open(fileName, O_RDONLY, 0);
    if (fileId < 0) {
	sprintf(interp->result, "couldn't read file \"%.50s\"", argv[1]);
	return TCL_ERROR;
    }
    if (fstat(fileId, &statBuf) == -1) {
	sprintf(interp->result, "couldn't stat file \"%.50s\"", argv[1]);
	close(fileId);
	return TCL_ERROR;
    }
    cmdBuffer = (char *) malloc((unsigned) statBuf.st_size+1);
    if (read(fileId, cmdBuffer, (int) statBuf.st_size) != statBuf.st_size) {
	sprintf(interp->result, "error in reading file \"%.50s\"", argv[1]);
	close(fileId);
	return TCL_ERROR;
    }
    close(fileId);
    cmdBuffer[statBuf.st_size] = 0;
    result = Tcl_Eval(interp, cmdBuffer, 0, &end);
    if (result == TCL_RETURN) {
	result = TCL_OK;
    }
    if (result == TCL_ERROR) {
	char msg[100];

	/*
	 * Record information telling where the error occurred.
	 */

	sprintf(msg, " (file \"%.50s\" line %d)", argv[1], interp->errorLine);
	Tcl_AddErrorInfo(interp, msg);
    }
    free(cmdBuffer);
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_StringCmd --
 *
 *	This procedure is invoked to process the "string" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_StringCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
#pragma ref dummy
    int length;
    register char *p, c;
    int match;
    int first;

    if (argc != 4) {
	sprintf(interp->result,
		"wrong # args: should be \"%.50s option a b\"",
		argv[0]);
	return TCL_ERROR;
    }
    length = strlen(argv[1]);
    if (strncmp(argv[1], "compare", length) == 0) {
	match = strcmp(argv[2], argv[3]);
	if (match > 0) {
	    interp->result = "1";
	} else if (match < 0) {
	    interp->result = "-1";
	} else {
	    interp->result = "0";
	}
	return TCL_OK;
    }
    if (strncmp(argv[1], "first", length) == 0) {
	first = 1;
    } else if (strncmp(argv[1], "last", length) == 0) {
	first = 0;
    } else if (strncmp(argv[1], "match", length) == 0) {
	if (Tcl_StringMatch(argv[3], argv[2]) != 0) {
	    interp->result = "1";
	} else {
	    interp->result = "0";
	}
	return TCL_OK;
    } else {
	sprintf(interp->result,
		"bad \"%.50s\" option \"%.50s\": must be compare, first, or last",
		argv[0], argv[1]);
	return TCL_ERROR;
    }
    match = -1;
    c = *argv[2];
    length = strlen(argv[2]);
    for (p = argv[3]; *p != 0; p++) {
	if (*p != c) {
	    continue;
	}
	if (strncmp(argv[2], p, length) == 0) {
	    match = p-argv[3];
	    if (first) {
		break;
	    }
	}
    }
    sprintf(interp->result, "%d", match);
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_TimeCmd --
 *
 *	This procedure is invoked to process the "time" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_TimeCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
#pragma ref dummy
    int count, i, result;
    struct tms start, stop;
    int micros;
    double timePer;

    if (argc == 2) {
	count = 1;
    } else if (argc == 3) {
	if (sscanf(argv[2], "%d", &count) != 1) {
	    sprintf(interp->result, "bad count \"%.50s\" given to \"%.50s\"",
		    argv[2], argv[0]);
	    return TCL_ERROR;
	}
    } else {
	sprintf(interp->result,
		"wrong # args: should be \"%.50s command [count]\"",
		argv[0]);
	return TCL_ERROR;
    }
    times(&start);
    for (i = count ; i > 0; i--) {
	result = Tcl_Eval(interp, argv[1], 0, (char **) NULL);
	if (result != TCL_OK) {
	    if (result == TCL_ERROR) {
		char msg[60];
		sprintf(msg, " (\"time\" body line %d)", interp->errorLine);
		Tcl_AddErrorInfo(interp, msg);
	    }
	    return result;
	}
    }
    times(&stop);
    micros = (stop.tms_utime - start.tms_utime)*1000000;
    timePer = micros;
    Tcl_Return(interp, (char *) NULL, TCL_STATIC);
    sprintf(interp->result, "%.0f microseconds per iteration", timePer/count);
    return TCL_OK;
}

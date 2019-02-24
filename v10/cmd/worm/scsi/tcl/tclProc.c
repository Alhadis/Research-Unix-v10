/* 
 * tclProc.c --
 *
 *	This file contains routines that implement Tcl procedures and
 *	variables.
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
static char rcsid[] = "$Header: /sprite/src/lib/tcl/RCS/tclProc.c,v 1.35 90/03/29 10:55:16 ouster Exp $ SPRITE (Berkeley)";
#pragma ref rcsid
#endif not lint

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tclInt.h"

/*
 * Forward references to procedures defined later in this file:
 */

extern Var *	FindVar();
extern int	InterpProc();
extern Var *	NewVar();
extern void	ProcDeleteProc();

/*
 *----------------------------------------------------------------------
 *
 * Tcl_ProcCmd --
 *
 *	This procedure is invoked to process the "proc" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result value.
 *
 * Side effects:
 *	A new procedure gets created.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_ProcCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
#pragma ref dummy
    register Interp *iPtr = (Interp *) interp;
    register Proc *procPtr;
    int result, argCount, i;
    char **argArray;

    if (argc != 4) {
	sprintf(iPtr->result,
		"wrong # args: should be \"%.50s name args body\"",
		argv[0]);
	return TCL_ERROR;
    }

    procPtr = (Proc *) malloc(sizeof(Proc));
    procPtr->iPtr = iPtr;
    procPtr->command = (char *) malloc((unsigned) strlen(argv[3]) + 1);
    strcpy(procPtr->command, argv[3]);
    procPtr->argPtr = NULL;
    Tcl_CreateCommand(interp, argv[1], InterpProc,
	    (ClientData) procPtr, ProcDeleteProc);

    /*
     * Break up the argument list into argument specifiers, then process
     * each argument specifier.
     */

    result = Tcl_SplitList(interp, argv[2], &argCount, &argArray);
    if (result != TCL_OK) {
	return result;
    }
    for (i = 0; i < argCount; i++) {
	int fieldCount, nameLength, valueLength;
	char **fieldValues;
	register Var *argPtr;

	/*
	 * Now divide the specifier up into name and default.
	 */

	result = Tcl_SplitList(interp, argArray[i], &fieldCount,
		&fieldValues);
	if (result != TCL_OK) {
	    goto procError;
	}
	if (fieldCount > 2) {
	    sprintf(iPtr->result,
		    "too many fields in argument specifier \"%.50s\"",
		    argArray[i]);
	    result = TCL_ERROR;
	    goto procError;
	}
	if ((fieldCount == 0) || (*fieldValues[0] == 0)) {
	    sprintf(iPtr->result,
		    "procedure \"%.50s\" has argument with no name", argv[1]);
	    result = TCL_ERROR;
	    goto procError;
	}
	nameLength = strlen(fieldValues[0]);
	if (fieldCount == 2) {
	    valueLength = strlen(fieldValues[1]);
	} else {
	    valueLength = 0;
	}
	if (procPtr->argPtr == NULL) {
	    argPtr = (Var *) malloc(VAR_SIZE(nameLength, valueLength));
	    procPtr->argPtr = argPtr;
	} else {
	    argPtr->nextPtr = (Var *) malloc(VAR_SIZE(nameLength, valueLength));
	    argPtr = argPtr->nextPtr;
	}
	strcpy(argPtr->name, fieldValues[0]);
	if (fieldCount == 2) {
	    argPtr->value = argPtr->name + nameLength + 1;
	    strcpy(argPtr->value, fieldValues[1]);
	} else {
	    argPtr->value = NULL;
	}
	argPtr->valueLength = valueLength;
	argPtr->flags = 0;
	argPtr->nextPtr = NULL;
	free((char *) fieldValues);
    }

    free((char *) argArray);
    return TCL_OK;

    procError:
    free((char *) argArray);
    return result;
}

/*1
 *----------------------------------------------------------------------
 *
 * Tcl_GetVar --
 *
 *	Return the value of a Tcl variable.
 *
 * Results:
 *	The return value points to the current value of varName.  If
 *	the variable is not defined in interp, either as a local or
 *	global variable, then a NULL pointer is returned.  Note:  the
 *	return value is only valid up until the next call to Tcl_SetVar;
 *	if you depend on the value lasting longer than that, then make
 *	yourself a private copy.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

char *
Tcl_GetVar(interp, varName, global)
    Tcl_Interp *interp;		/* Command interpreter in which varName is
				 * to be looked up. */
    char *varName;		/* Name of a variable in interp. */
    int global;			/* If non-zero, use only a global variable */
{
    Var *varPtr;
    Interp *iPtr = (Interp *) interp;

    if (global || (iPtr->varFramePtr == NULL)) {
	varPtr = FindVar(&iPtr->globalPtr, varName);
    } else {
	varPtr = FindVar(&iPtr->varFramePtr->varPtr, varName);
    }
    if (varPtr == NULL) {
	return NULL;
    }
    if (varPtr->flags & VAR_GLOBAL) {
	varPtr = varPtr->globalPtr;
    }
    if (varPtr->flags & VAR_DOESNT_EXIST) {
	return NULL;
    }
    return varPtr->value;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_SetVar --
 *
 *	Change the value of a variable.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	If varName is defined as a local or global variable in interp,
 *	its value is changed to newValue.  If varName isn't currently
 *	defined, then a new global variable by that name is created.
 *
 *----------------------------------------------------------------------
 */

void
Tcl_SetVar(interp, varName, newValue, global)
    Tcl_Interp *interp;		/* Command interpreter in which varName is
				 * to be looked up. */
    char *varName;		/* Name of a variable in interp. */
    char *newValue;		/* New value for varName. */
    int global;			/* If non-zero, use only a global variable. */
{
    register Var *varPtr, **varListPtr;
    register Interp *iPtr = (Interp *) interp;
    int valueLength;

    if (global || (iPtr->varFramePtr == NULL)) {
	varListPtr = &iPtr->globalPtr;
    } else {
	varListPtr = &iPtr->varFramePtr->varPtr;
    }
    varPtr = FindVar(varListPtr, varName);
    if (varPtr == NULL) {
	varPtr = NewVar(varName, newValue);
	varPtr->nextPtr = *varListPtr;
	*varListPtr = varPtr;
    } else {
	if (varPtr->flags & VAR_GLOBAL) {
	    varPtr = varPtr->globalPtr;
	}
	valueLength = strlen(newValue);
	if (valueLength > varPtr->valueLength) {
	    if (varPtr->flags & VAR_DYNAMIC) {
		free(varPtr->value);
	    }
	    varPtr->value = (char *) malloc((unsigned) valueLength + 1);
	    varPtr->flags |= VAR_DYNAMIC;
	    varPtr->valueLength = valueLength;
	}
	strcpy(varPtr->value, newValue);
	varPtr->flags &= ~VAR_DOESNT_EXIST;
    }
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_ParseVar --
 *
 *	Given a string starting with a $ sign, parse off a variable
 *	name and return its value.
 *
 * Results:
 *	The return value is the contents of the variable given by
 *	the leading characters of string.  If termPtr isn't NULL,
 *	*termPtr gets filled in with the address of the character
 *	just after the last one in the variable specifier.  If the
 *	variable doesn't exist, then the return value is NULL and
 *	an error message will be left in interp->result.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

char *
Tcl_ParseVar(interp, string, termPtr)
    Tcl_Interp *interp;			/* Context for looking up variable. */
    register char *string;		/* String containing variable name.
					 * First character must be "$". */
    char **termPtr;			/* If non-NULL, points to word to fill
					 * in with character just after last
					 * one in the variable specifier. */

{
    char *name, c, *result;

    /*
     * There are two cases:
     * 1. The $ sign is followed by an open curly brace.  Then the variable
     *    name is everything up to the next close curly brace.
     * 2. The $ sign is not followed by an open curly brace.  Then the
     *    variable name is everything up to the next character that isn't
     *    a letter, digit, or underscore.
     * 3. The $ sign is followed by something that isn't a letter, digit,
     *    or underscore:  in this case, there is no variable name, and "$"
     *    is returned.
     */

    string++;
    if (*string == '{') {
	string++;
	name = string;
	while ((*string != '}') && (*string != 0)) {
	    string++;
	}
	if (termPtr != 0) {
	    if (*string != 0) {
		*termPtr = string+1;
	    } else {
		*termPtr = string;
	    }
	}
    } else {
	name = string;
	while (isalnum(*string) || (*string == '_')) {
	    string++;
	}
	if (termPtr != 0) {
	    *termPtr = string;
	}
	if (string == name) {
	    return "$";
	}
    }

    c = *string;
    *string = 0;
    result = Tcl_GetVar(interp, name, 0);
    if (result == NULL) {
	Tcl_Return(interp, (char *) NULL, TCL_STATIC);
	sprintf(interp->result, "couldn't find variable \"%.50s\"", name);
    }
    *string = c;
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_SetCmd --
 *
 *	This procedure is invoked to process the "set" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result value.
 *
 * Side effects:
 *	A variable's value may be changed.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_SetCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    register Tcl_Interp *interp;	/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
#pragma ref dummy
    if (argc == 2) {
	char *value;

	value = Tcl_GetVar(interp, argv[1], 0);
	if (value == NULL) {
	    sprintf(interp->result, "couldn't find variable \"%.50s\"",
		    argv[1]);
	    return TCL_ERROR;
	}
	interp->result = value;
	return TCL_OK;
    } else if (argc == 3) {
	Tcl_SetVar(interp, argv[1], argv[2], 0);
	return TCL_OK;
    } else {
	sprintf(interp->result,
		"wrong # args: should be \"%.50s varName [newValue]\"",
		argv[0]);
	return TCL_ERROR;
    }
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_GlobalCmd --
 *
 *	This procedure is invoked to process the "global" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result value.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_GlobalCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
#pragma ref dummy
    register Var *varPtr;
    register Interp *iPtr = (Interp *) interp;
    Var *gVarPtr;

    if (argc < 2) {
	sprintf(iPtr->result,
		"too few args:  should be \"%.50s varName varName ...\"",
		argv[0]);
	return TCL_ERROR;
    }
    if (iPtr->varFramePtr == NULL) {
	return TCL_OK;
    }

    for (argc--, argv++; argc > 0; argc--, argv++) {
	gVarPtr = FindVar(&iPtr->globalPtr, *argv);
	if (gVarPtr == NULL) {
	    gVarPtr = NewVar(*argv, "");
	    gVarPtr->nextPtr = iPtr->globalPtr;
	    iPtr->globalPtr = gVarPtr;
	    gVarPtr->flags |= VAR_DOESNT_EXIST;
	}
	varPtr = NewVar(*argv, "");
	varPtr->flags |= VAR_GLOBAL;
	varPtr->globalPtr = gVarPtr;
	varPtr->nextPtr = iPtr->varFramePtr->varPtr;
	iPtr->varFramePtr->varPtr = varPtr;
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_UplevelCmd --
 *
 *	This procedure is invoked to process the "uplevel" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result value.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_UplevelCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
#pragma ref dummy
    register Interp *iPtr = (Interp *) interp;
    int level, result;
    char *end, *levelArg;
    CallFrame *savedVarFramePtr, *framePtr;

    if (argc < 2) {
	uplevelSyntax:
	sprintf(iPtr->result,
		"too few args:  should be \"%.50s [level] command ...\"",
		argv[0]);
	return TCL_ERROR;
    }

    /*
     * Parse arguments to figure out which level to go to, and set
     * argv and argc to refer to the command to execute at that level.
     */

    levelArg = argv[1];
    if (*levelArg == '#') {
	level = strtoul(levelArg+1, &end, 10);
	if ((end == (levelArg+1)) || (*end != '\0')) {
	    goto levelError;
	}
	argc -= 2;
	argv += 2;
    } else if (isdigit(*levelArg)) {
	level = strtoul(levelArg, &end, 10);
	if ((end == levelArg) || (*end != '\0')) {
	    goto levelError;
	}
	if (iPtr->varFramePtr == NULL) {
	    goto levelError;
	}
	level = iPtr->varFramePtr->level - level;
	argc -= 2;
	argv += 2;
    } else {
	if (iPtr->varFramePtr == NULL) {
	    goto levelError;
	}
	level = iPtr->varFramePtr->level - 1;
	argc--;
	argv++;
    }

    /*
     * Figure out which frame to use, and modify the interpreter so
     * its variables come from that frame.
     */

    savedVarFramePtr = iPtr->varFramePtr;
    if (level == 0) {
	iPtr->varFramePtr = NULL;
    } else {
	for (framePtr = savedVarFramePtr; framePtr != NULL;
		framePtr = framePtr->callerVarPtr) {
	    if (framePtr->level == level) {
		break;
	    }
	}
	if (framePtr == NULL) {
	    goto levelError;
	}
	iPtr->varFramePtr = framePtr;
    }

    /*
     * Execute the residual arguments as a command.
     */

    if (argc == 0) {
	goto uplevelSyntax;
    }
    if (argc == 1) {
	result = Tcl_Eval(interp, argv[0], 0, (char **) NULL);
    } else {
	char *cmd;

	cmd = Tcl_Concat(argc, argv);
	result = Tcl_Eval(interp, cmd, 0, (char **) NULL);
    }
    if (result == TCL_ERROR) {
	char msg[60];
	sprintf(msg, " (\"uplevel\" body line %d)", interp->errorLine);
	Tcl_AddErrorInfo(interp, msg);
    }

    /*
     * Restore the variable frame, and return.
     */

    iPtr->varFramePtr = savedVarFramePtr;
    return result;

    levelError:
    sprintf(iPtr->result, "bad level \"%.50s\"", levelArg);
    return TCL_ERROR;
}

/*
 *----------------------------------------------------------------------
 *
 * TclFindProc --
 *
 *	Given the name of a procedure, return a pointer to the
 *	record describing the procedure.
 *
 * Results:
 *	NULL is returned if the name doesn't correspond to any
 *	procedure.  Otherwise the return value is a pointer to
 *	the procedure's record.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

Proc *
TclFindProc(iPtr, procName)
    Interp *iPtr;		/* Interpreter in which to look. */
    char *procName;		/* Name of desired procedure. */
{
    Command *cmdPtr;

    cmdPtr = TclFindCmd(iPtr, procName, 0);
    if (cmdPtr == NULL) {
	return NULL;
    }
    if (cmdPtr->proc != InterpProc) {
	return NULL;
    }
    return (Proc *) cmdPtr->clientData;
}

/*
 *----------------------------------------------------------------------
 *
 * TclIsProc --
 *
 *	Tells whether a command is a Tcl procedure or not.
 *
 * Results:
 *	If the given command is actuall a Tcl procedure, the
 *	return value is the address of the record describing
 *	the procedure.  Otherwise the return value is 0.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

Proc *
TclIsProc(cmdPtr)
    Command *cmdPtr;		/* Command to test. */
{
    if (cmdPtr->proc == InterpProc) {
	return (Proc *) cmdPtr->clientData;
    }
    return (Proc *) 0;
}

/*
 *----------------------------------------------------------------------
 *
 * TclDeleteVars --
 *
 *	This procedure is called as part of deleting an interpreter:
 *	it recycles all the storage space associated with global
 *	variables (the local ones should already have been deleted).
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Variables are deleted.
 *
 *----------------------------------------------------------------------
 */

void
TclDeleteVars(iPtr)
    Interp *iPtr;		/* Interpreter to nuke. */
{
    register Var *varPtr;

    for (varPtr = iPtr->globalPtr; varPtr != NULL; varPtr = varPtr->nextPtr) {
	if (varPtr->flags & VAR_DYNAMIC) {
	    free(varPtr->value);
	}
	free((char *) varPtr);
    }
}

/*
 *----------------------------------------------------------------------
 *
 * InterpProc --
 *
 *	When a Tcl procedure gets invoked, this routine gets invoked
 *	to interpret the procedure.
 *
 * Results:
 *	A standard Tcl result value, usually TCL_OK.
 *
 * Side effects:
 *	Depends on the commands in the procedure.
 *
 *----------------------------------------------------------------------
 */

int
InterpProc(procPtr, interp, argc, argv)
    register Proc *procPtr;	/* Record describing procedure to be
				 * interpreted. */
    Tcl_Interp *interp;		/* Interpreter in which procedure was
				 * invoked. */
    int argc;			/* Count of number of arguments to this
				 * procedure. */
    char **argv;		/* Argument values. */
{
    char **args;
    register Var *formalPtr, *argPtr;
    register Interp *iPtr = (Interp *) interp;
    CallFrame frame;
    char *value, *end;
    int result;

    /*
     * Set up a call frame for the new procedure invocation.
     */

    iPtr = procPtr->iPtr;
    frame.varPtr = NULL;
    if (iPtr->varFramePtr != NULL) {
	frame.level = iPtr->varFramePtr->level + 1;
    } else {
	frame.level = 1;
    }
    frame.argc = argc;
    frame.argv = argv;
    frame.callerPtr = iPtr->framePtr;
    frame.callerVarPtr = iPtr->varFramePtr;
    iPtr->framePtr = &frame;
    iPtr->varFramePtr = &frame;

    /*
     * Match the actual arguments against the procedure's formal
     * parameters to compute local variables.
     */

    for (formalPtr = procPtr->argPtr, args = argv+1, argc -= 1;
	    formalPtr != NULL;
	    formalPtr = formalPtr->nextPtr, args++, argc--) {

	/*
	 * Handle the special case of the last formal being "args".  When
	 * it occurs, assign it a list consisting of all the remaining
	 * actual arguments.
	 */

	if ((formalPtr->nextPtr == NULL)
		&& (strcmp(formalPtr->name, "args") == 0)) {
	    if (argc < 0) {
		argc = 0;
	    }
	    value = Tcl_Merge(argc, args);
	    argPtr = NewVar(formalPtr->name, value);
	    free(value);
	    argPtr->nextPtr = frame.varPtr;
	    frame.varPtr = argPtr;
	    argc = 0;
	    break;
	} else if (argc > 0) {
	    value = *args;
	} else if (formalPtr->value != NULL) {
	    value = formalPtr->value;
	} else {
	    sprintf(iPtr->result,
		    "no value given for parameter \"%s\" to \"%s\"",
		    formalPtr->name, argv[0]);
	    result = TCL_ERROR;
	    goto procDone;
	}
	argPtr = NewVar(formalPtr->name, value);
	argPtr->nextPtr = frame.varPtr;
	frame.varPtr = argPtr;
    }
    if (argc > 0) {
	sprintf(iPtr->result, "called \"%s\" with too many arguments",
		argv[0]);
	result = TCL_ERROR;
	goto procDone;
    }

    /*
     * Invoke the commands in the procedure's body.
     */

    result = Tcl_Eval(interp, procPtr->command, 0, &end);
    if (result == TCL_RETURN) {
	result = TCL_OK;
    } else if (result == TCL_ERROR) {
	char msg[100];

	/*
	 * Record information telling where the error occurred.
	 */

	sprintf(msg, " (procedure \"%.50s\" line %d)", argv[0],
		iPtr->errorLine);
	Tcl_AddErrorInfo(interp, msg);
    } else if (result == TCL_BREAK) {
	iPtr->result = "invoked \"break\" outside of a loop";
	result = TCL_ERROR;
    } else if (result == TCL_CONTINUE) {
	iPtr->result = "invoked \"continue\" outside of a loop";
	result = TCL_ERROR;
    }

    /*
     * Delete the call frame for this procedure invocation.
     */

    procDone:
    for (argPtr = frame.varPtr; argPtr != NULL; argPtr = argPtr->nextPtr) {
	if (argPtr->flags & VAR_DYNAMIC) {
	    free(argPtr->value);
	}
	free((char *) argPtr);
    }
    iPtr->framePtr = frame.callerPtr;
    iPtr->varFramePtr = frame.callerVarPtr;
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * ProcDeleteProc --
 *
 *	This procedure is invoked just before a command procedure is
 *	removed from an interpreter.  Its job is to release all the
 *	resources allocated to the procedure.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Memory gets freed.
 *
 *----------------------------------------------------------------------
 */

void
ProcDeleteProc(procPtr)
    register Proc *procPtr;		/* Procedure to be deleted. */
{
    register Var *argPtr;

    free((char *) procPtr->command);
    for (argPtr = procPtr->argPtr; argPtr != NULL; argPtr = argPtr->nextPtr) {
	if (argPtr->flags & VAR_DYNAMIC) {
	    free(argPtr->value);
	}
	free((char *) argPtr);
    }
    free((char *) procPtr);
}

/*
 *----------------------------------------------------------------------
 *
 * FindVar --
 *
 *	Locate the Var structure corresponding to varName, if there
 *	is one defined in a given list.
 *
 * Results:
 *	The return value points to the Var structure corresponding to
 *	the current value of varName in varListPtr, or NULL if varName
 *	isn't currently defined in the list.
 *
 * Side effects:
 *	If the variable is found, it is moved to the front of the list.
 *
 *----------------------------------------------------------------------
 */

Var *
FindVar(varListPtr, varName)
    Var **varListPtr;		/* Pointer to head of list.  The value pointed
				 * to will be modified to bring the found
				 * variable to the front of the list. */
    char *varName;		/* Desired variable. */
{
    register Var *prev, *cur;
    register char c;

    c = *varName;

    /*
     * Local variables take precedence over global ones.  Check the
     * first character immediately, before wasting time calling strcmp.
     */

    for (prev = NULL, cur = *varListPtr; cur != NULL;
	    prev = cur, cur = cur->nextPtr) {
	if ((cur->name[0] == c) && (strcmp(cur->name, varName) == 0)) {
	    if (prev != NULL) {
		prev->nextPtr = cur->nextPtr;
		cur->nextPtr = *varListPtr;
		*varListPtr = cur;
	    }
	    return cur;
	}
    }
    return NULL;
}

/*
 *----------------------------------------------------------------------
 *
 * NewVar --
 *
 *	Create a new variable with the given name and initial value.
 *
 * Results:
 *	The return value is a pointer to the new variable.  The variable
 *	will not have been linked into any particular list, and its
 *	nextPtr field will be NULL.
 *
 * Side effects:
 *	Storage gets allocated.
 *
 *----------------------------------------------------------------------
 */

Var *
NewVar(name, value)
    char *name;			/* Name for variable. */
    char *value;		/* Value for variable. */
{
    register Var *varPtr;
    int nameLength, valueLength;

    nameLength = strlen(name);
    valueLength = strlen(value);
    if (valueLength < 20) {
	valueLength = 20;
    }
    varPtr = (Var *) malloc(VAR_SIZE(nameLength, valueLength));
    strcpy(varPtr->name, name);
    varPtr->value = varPtr->name + nameLength + 1;
    strcpy(varPtr->value, value);
    varPtr->valueLength = valueLength;
    varPtr->flags = 0;
    varPtr->globalPtr = NULL;
    varPtr->nextPtr = NULL;
    return varPtr;
}

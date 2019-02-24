/* 
 * tclBasic.c --
 *
 *	Contains the basic facilities for TCL command interpretation,
 *	including interpreter creation and deletion, command creation
 *	and deletion, and command parsing and execution.
 *
 * Copyright 1987, 1990 Regents of the University of California
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies.  The University of California
 * makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 */

#ifndef lint
static char rcsid[] = "$Header: /sprite/src/lib/tcl/RCS/tclBasic.c,v 1.72 90/03/29 10:36:39 ouster Exp $ SPRITE (Berkeley)";
#pragma ref rcsid
#endif not lint

#define	_POSIX_SOURCE

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "tclInt.h"

/*
 * Built-in commands, and the procedures associated with them:
 */

static char *builtInCmds[] = {
    "break",
    "case",
    "catch",
    "concat",
    "continue",
    "error",
    "eval",
    "exec",
    "expr",
    "file",
    "for",
    "foreach",
    "format",
    "glob",
    "global",
    "if",
    "index",
    "info",
    "length",
    "list",
    "print",
    "proc",
    "range",
    "rename",
    "return",
    "scan",
    "set",
    "source",
    "string",
    "time",
    "uplevel",
    NULL
};

static int (*(builtInProcs[]))(ClientData , Tcl_Interp *, int , char **) = {
    Tcl_BreakCmd,
    Tcl_CaseCmd,
    Tcl_CatchCmd,
    Tcl_ConcatCmd,
    Tcl_ContinueCmd,
    Tcl_ErrorCmd,
    Tcl_EvalCmd,
    Tcl_ExecCmd,
    Tcl_ExprCmd,
    Tcl_FileCmd,
    Tcl_ForCmd,
    Tcl_ForeachCmd,
    Tcl_FormatCmd,
    Tcl_GlobCmd,
    Tcl_GlobalCmd,
    Tcl_IfCmd,
    Tcl_IndexCmd,
    Tcl_InfoCmd,
    Tcl_LengthCmd,
    Tcl_ListCmd,
    Tcl_PrintCmd,
    Tcl_ProcCmd,
    Tcl_RangeCmd,
    Tcl_RenameCmd,
    Tcl_ReturnCmd,
    Tcl_ScanCmd,
    Tcl_SetCmd,
    Tcl_SourceCmd,
    Tcl_StringCmd,
    Tcl_TimeCmd,
    Tcl_UplevelCmd,
    NULL
};

/*
 *----------------------------------------------------------------------
 *
 * Tcl_CreateInterp --
 *
 *	Create a new TCL command interpreter.
 *
 * Results:
 *	The return value is a token for the interpreter, which may be
 *	used in calls to procedures like Tcl_CreateCmd, Tcl_Eval, or
 *	Tcl_DeleteInterp.
 *
 * Side effects:
 *	The command interpreter is initialized with an empty variable
 *	table and the built-in commands.
 *
 *----------------------------------------------------------------------
 */

Tcl_Interp *
Tcl_CreateInterp()
{
    register Interp *iPtr;
    register char **namePtr;
    register int (**procPtr)();
    register Command *cmdPtr;

    iPtr = (Interp *) malloc(sizeof(Interp));
    iPtr->result = iPtr->resultSpace;
    iPtr->dynamic = 0;
    iPtr->errorLine = 0;
    iPtr->commandPtr = NULL;
    iPtr->globalPtr = NULL;
    iPtr->numLevels = 0;
    iPtr->framePtr = NULL;
    iPtr->varFramePtr = NULL;
    iPtr->numEvents = 0;
    iPtr->events = NULL;
    iPtr->curEvent = 0;
    iPtr->curEventNum = 0;
    iPtr->revPtr = NULL;
    iPtr->historyFirst = NULL;
    iPtr->evalFirst = iPtr->evalLast = NULL;
    iPtr->cmdCount = 0;
    iPtr->noEval = 0;
    iPtr->flags = 0;
    iPtr->tracePtr = NULL;
    iPtr->callbackPtr = NULL;
    iPtr->resultSpace[0] = 0;

    /*
     * Create the built-in commands.  Do it here, rather than calling
     * Tcl_CreateCommand, because it's faster (there's no need to
     * check for a pre-existing command by the same name).
     */

    for (namePtr = builtInCmds, procPtr = builtInProcs;
	    *namePtr != NULL; namePtr++, procPtr++) {
	cmdPtr = (Command *) malloc(CMD_SIZE(strlen(*namePtr)));
	cmdPtr->proc = *procPtr;
	cmdPtr->clientData = (ClientData) NULL;
	cmdPtr->deleteProc = NULL;
	cmdPtr->nextPtr = iPtr->commandPtr;
	iPtr->commandPtr = cmdPtr;
	strcpy(cmdPtr->name, *namePtr);
    }

    return (Tcl_Interp *) iPtr;
}

/*
 *--------------------------------------------------------------
 *
 * Tcl_WatchInterp --
 *
 *	Arrange for a procedure to be called before a given
 *	interpreter is deleted.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	When Tcl_DeleteInterp is invoked to delete interp,
 *	proc will be invoked.  See the manual entry for
 *	details.
 *
 *--------------------------------------------------------------
 */

void
Tcl_WatchInterp(interp, proc, clientData)
    Tcl_Interp *interp;		/* Interpreter to watch. */
    void (*proc)();		/* Procedure to call when interpreter
				 * is about to be deleted. */
    ClientData clientData;	/* One-word value to pass to proc. */
{
    register InterpCallback *icPtr;
    Interp *iPtr = (Interp *) interp;

    icPtr = (InterpCallback *) malloc(sizeof(InterpCallback));
    icPtr->proc = proc;
    icPtr->clientData = clientData;
    icPtr->nextPtr = iPtr->callbackPtr;
    iPtr->callbackPtr = icPtr;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_DeleteInterp --
 *
 *	Delete an interpreter and free up all of the resources associated
 *	with it.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The interpreter is destroyed.  The caller should never again
 *	use the interp token.
 *
 *----------------------------------------------------------------------
 */

void
Tcl_DeleteInterp(interp)
    Tcl_Interp *interp;		/* Token for command interpreter (returned
				 * by a previous call to Tcl_CreateInterp). */
{
    Interp *iPtr = (Interp *) interp;
    register Command *cmdPtr;
    register Trace *tracePtr;
    register InterpCallback *icPtr;

    /*
     * If the interpreter is in use, delay the deletion until later.
     */

    iPtr->flags |= DELETED;
    if (iPtr->numLevels != 0) {
	return;
    }

    /*
     * Invoke callbacks, if there's anyone who wants to know about
     * the interpreter deletion.
     */

    for (icPtr = iPtr->callbackPtr; icPtr != NULL;
	    icPtr = icPtr->nextPtr) {
	(*icPtr->proc)(icPtr->clientData, interp);
	free((char *) icPtr);
    }

    /*
     * Free up any remaining resources associated with the
     * interpreter.
     */

    for (cmdPtr = iPtr->commandPtr; cmdPtr != NULL;
	    cmdPtr = cmdPtr->nextPtr) {
	if (cmdPtr->deleteProc != NULL) { 
	    (*cmdPtr->deleteProc)(cmdPtr->clientData);
	}
	free((char *) cmdPtr);
    }
    iPtr->commandPtr = NULL;
    TclDeleteVars(iPtr);
    if (iPtr->events != NULL) {
	free((char *) iPtr->events);
    }
    while (iPtr->revPtr != NULL) {
	free((char *) iPtr->revPtr);
	iPtr->revPtr = iPtr->revPtr->nextPtr;
    }
    for (tracePtr = iPtr->tracePtr; tracePtr != NULL;
	    tracePtr = tracePtr->nextPtr) {
	free((char *) tracePtr);
    }
    free((char *) iPtr);
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_CreateCommand --
 *
 *	Define a new command in a command table.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	If a command named cmdName already exists for interp, it is
 *	deleted.  In the future, when cmdName is seen as the name of
 *	a command by Tcl_Eval, proc will be called with the following
 *	syntax:
 *
 *	int
 *	proc(clientData, interp, argc, argv)
 *	    ClientData clientData;
 *	    Tcl_Interp *interp;
 *	    int argc;
 *	    char **argv;
 *	{
 *	}
 *
 *	The clientData and interp arguments are the same as the corresponding
 *	arguments passed to this procedure.  Argc and argv describe the
 *	arguments to the command, in the usual UNIX fashion.  Proc must
 *	return a code like TCL_OK or TCL_ERROR.  It can also set interp->result
 *	("" is the default value if proc doesn't set it) and interp->dynamic (0
 *	is the default).  See tcl.h for more information on these variables.
 *
 *	When the command is deleted from the table, deleteProc will be called
 *	in the following way:
 *
 *	void
 *	deleteProc(clientData)
 *	    ClientData clientData;
 *	{
 *	}
 *
 *	DeleteProc allows command implementors to perform their own cleanup
 *	when commands (or interpreters) are deleted.
 *
 *----------------------------------------------------------------------
 */

void
Tcl_CreateCommand(interp, cmdName, proc, clientData, deleteProc)
    Tcl_Interp *interp;		/* Token for command interpreter (returned
				 * by a previous call to Tcl_CreateInterp). */
    char *cmdName;		/* Name of command. */
    int (*proc)();		/* Command procedure to associate with
				 * cmdName. */
    ClientData clientData;	/* Arbitrary one-word value to pass to proc. */
    void (*deleteProc)();	/* If not NULL, gives a procedure to call when
				 * this command is deleted. */
{
    Interp *iPtr = (Interp *) interp;
    register Command *cmdPtr;

    Tcl_DeleteCommand(interp, cmdName);
    cmdPtr = (Command *) malloc(CMD_SIZE(strlen(cmdName)));
    cmdPtr->proc = proc;
    cmdPtr->clientData = clientData;
    cmdPtr->deleteProc = deleteProc;
    cmdPtr->nextPtr = iPtr->commandPtr;
    iPtr->commandPtr = cmdPtr;
    strcpy(cmdPtr->name, cmdName);
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_DeleteCommand --
 *
 *	Remove the given command from the given interpreter.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	CmdName will no longer be recognized as a valid command for
 *	interp.
 *
 *----------------------------------------------------------------------
 */

void
Tcl_DeleteCommand(interp, cmdName)
    Tcl_Interp *interp;		/* Token for command interpreter (returned
				 * by a previous call to Tcl_CreateInterp). */
    char *cmdName;		/* Name of command to remove. */
{
    Interp *iPtr = (Interp *) interp;
    Command *cmdPtr;

    cmdPtr = TclFindCmd(iPtr, cmdName, 0);
    if (cmdPtr != NULL) {
	if (cmdPtr->deleteProc != NULL) {
	    (*cmdPtr->deleteProc)(cmdPtr->clientData);
	}
	iPtr->commandPtr = cmdPtr->nextPtr;
	free((char *) cmdPtr);
    }
}

/*
 *-----------------------------------------------------------------
 *
 * Tcl_Eval --
 *
 *	Parse and execute a command in the Tcl language.
 *
 * Results:
 *	The return value is one of the return codes defined in
 *	tcl.h (such as TCL_OK), and interp->result contains a string
 *	value to supplement the return code.  The value of interp->result
 *	will persist only until the next call to Tcl_Eval:  copy it
 *	or lose it!
 *
 * Side effects:
 *	Almost certainly;  depends on the command.
 *
 *-----------------------------------------------------------------
 */

int
Tcl_Eval(interp, cmd, flags, termPtr)
    Tcl_Interp *interp;		/* Token for command interpreter (returned
				 * by a previous call to Tcl_CreateInterp). */
    char *cmd;			/* Pointer to TCL command to interpret. */
    int flags;			/* OR-ed combination of flags like
				 * TCL_BRACKET_TERM and TCL_RECORD_BOUNDS. */
    char **termPtr;		/* If non-NULL, fill in the address it points
				 * to with the address of the char. just after
				 * the last one that was part of cmd.  See
				 * the man page for details on this. */
{
    /*
     * While processing the command, make a local copy of
     * the command characters.  This is needed in order to
     * terminate each argument with a null character, replace
     * backslashed-characters, etc.  The copy starts out in
     * a static string (for speed) but gets expanded into
     * dynamically-allocated strings if necessary.  The constant
     * BUFFER indicates how much space there must be in the copy
     * in order to pass through the main loop below (e.g., must
     * have space to copy both a backslash and its following
     * characters).
     */

#   define NUM_CHARS 200
#   define BUFFER 5
    char copyStorage[NUM_CHARS];
    char *copy = copyStorage;	/* Pointer to current copy. */
    int copySize = NUM_CHARS;	/* Size of current copy. */
    register char *dst;		/* Points to next place to copy
				 * a character. */
    char *limit;		/* When dst gets here, must make
				 * the copy larger. */

    /*
     * This procedure generates an (argv, argc) array for the command,
     * It starts out with stack-allocated space but uses dynamically-
     * allocated storage to increase it if needed.
     */

#   define NUM_ARGS 10
    char *(argStorage[NUM_ARGS]);
    char **argv = argStorage;
    int argc;
    int argSize = NUM_ARGS;

    int openBraces = 0;			/* Curent brace nesting level. */
    int openQuote = 0;			/* Non-zero means quoted arg
					 * in progress. */

    register char *src;			/* Points to current character
					 * in cmd. */
    char termChar;			/* Return when this character is found
					 * (either ']' or '\0').  Zero means
					 * that newlines terminate commands. */
    char *argStart;			/* Location in cmd of first							 * non-separator character in
					 * current argument;  it's
					 * used to eliminate multiple
					 * separators between args and
					 * extra separators after last
					 * arg in command. */
    int result = TCL_OK;		/* Return value. */
    int i;
    register Interp *iPtr = (Interp *) interp;
    Command *cmdPtr;
    char *tmp;
    char *dummy;			/* Make termPtr point here if it was
					 * originally NULL. */
    char *syntaxMsg;
    char *syntaxPtr;			/* Points to "relevant" character
					 * for syntax violations. */
    char *cmdStart;			/* Points to first non-blank char. in
					 * command (used in calling trace
					 * procedures). */
    register Trace *tracePtr;

    /*
     * Set up the result so that if there's no command at all in
     * the string then this procedure will return TCL_OK.
     */

    if (iPtr->dynamic) {
	free((char *) iPtr->result);
	iPtr->dynamic = 0;
    }
    iPtr->result = iPtr->resultSpace;
    iPtr->resultSpace[0] = 0;

    /*
     * Check depth of nested calls to Tcl_Eval:  if this gets too large,
     * it's probably because of an infinite loop somewhere (e.g. self-
     * recursive history invocation).
     */

    iPtr->numLevels++;
    if (iPtr->numLevels > MAX_NESTING_DEPTH) {
	iPtr->result =  "too many nested calls to Tcl_Eval (infinite loop?)";
	return TCL_ERROR;
    }

    src = cmd;
    result = TCL_OK;
    if (flags & TCL_BRACKET_TERM) {
	termChar = ']';
    } else {
	termChar = 0;
    }
    if (termPtr == NULL) {
	termPtr = &dummy;
    }

    /*
     * There can be many sub-commands (separated by semi-colons or
     * newlines) in one command string.  This outer loop iterates over
     * the inner commands.
     */

    for (*termPtr = src; *src != termChar; *termPtr = src) {

	/*
	 * Skim off leading white space and semi-colons, and skip comments.
	 */

	while (isspace(*src) || (*src == ';')) {
	    src += 1;
	}
	if (*src == '#') {
	    for (src++; *src != 0; src++) {
		if (*src == '\n') {
		    src++;
		    break;
		}
	    }
	    continue;
	}

	/*
	 * Set up the first argument (the command name).  Note that
	 * the arg pointer gets set up BEFORE the first real character
	 * of the argument has been found.
	 */
    
	dst = copy;
	argc = 0;
	limit = copy + copySize - BUFFER;
	argv[0] = dst;
	argStart = cmdStart = src;

	/*
	 * Skim off the command name and arguments by looping over
	 * characters and processing each one according to its type.
	 */
    
	while (1) {
	    switch (*src) {
    
		/*
		 * All braces are treated as normal characters
		 * unless the first character of the argument is an
		 * open brace.  In that case, braces nest and
		 * the argument terminates when all braces are matched.
		 * Internal braces are also copied like normal chars.
		 */
    
		case '{': {
		    if (!openBraces && !openQuote && (dst == argv[argc])) {
			syntaxPtr = src;
			openBraces = 1;
			break;
		    }
		    *dst = '{'; dst++;
		    if (openBraces > 0) {
			openBraces++;
		    }
		    break;
		}

		case '}': {
		    if (openBraces == 1) {
			openBraces = 0;
			if (!isspace(src[1]) && (src[1] != termChar) &&
				(src[1] != 0) && (src[1] != ';')) {
			    syntaxPtr = src;
			    syntaxMsg = "extra characters after close-brace";
			    goto syntaxError;
			}
		    } else {
			*dst = '}'; dst++;
			if (openBraces > 0) {
			    openBraces--;
			}
		    }
		    break;
		}

		case '"': {
		    if (!openQuote) {
			if (openBraces || (dst != argv[argc])) {
			    *dst = '"'; dst++;
			    break;
			}
			syntaxPtr = src;
			openQuote = 1;
		    } else {
			openQuote = 0;
			if (!isspace(src[1]) && (src[1] != termChar) &&
				(src[1] != 0) && (src[1] != ';')) {
			    syntaxPtr = src;
			    syntaxMsg = "extra characters after close-quote";
			    goto syntaxError;
			}
		    }
		    break;
		}
    
		case '[': {
    
		    /*
		     * Open bracket: if not in middle of braces, then execute
		     * following command and substitute result into argument.
		     */

		    if (openBraces != 0) {
			*dst = '['; dst++;
		    } else {
			int length;
    
			result = Tcl_Eval(interp, src+1,
				TCL_BRACKET_TERM | (flags & TCL_RECORD_BOUNDS),
				&tmp);
			src = tmp;
			if (result != TCL_OK) {
			    goto done;
			}
    
			/*
			 * Copy the return value into the current argument.
			 * May have to enlarge the argument storage.  When
			 * enlarging, get more than enough to reduce the
			 * likelihood of having to enlarge again.  This code
			 * is used for $-processing also.
			 */

			copyResult:
			length = strlen(iPtr->result);
			if ((limit - dst) < length) {
			    char *newCopy;
			    int delta;

			    copySize = length + 10 + dst - copy;
			    newCopy = (char *) malloc((unsigned) copySize);
			    bcopy(copy, newCopy, (dst-copy));
			    delta = newCopy - copy;
			    dst += delta;
			    for (i = 0; i <= argc; i++) {
				argv[i] += delta;
			    }
			    if (copy != copyStorage) {
				free((char *) copy);
			    }
			    copy = newCopy;
			    limit = newCopy + copySize - BUFFER;
			}
			bcopy(iPtr->result, dst, length);
			dst += length;
		    }
		    break;
		}

		case '$': {
		    if (openBraces != 0) {
			*dst = '$'; dst++;
		    } else {
			char *value;

			/*
			 * Parse off a variable name and copy its value.
			 */
    
			value = Tcl_ParseVar(interp, src, &tmp);
			if (value == NULL) {
			    result = TCL_ERROR;
			    goto done;
			}
			if (iPtr->dynamic) {
			    free((char *) iPtr->result);
			    iPtr->dynamic = 0;
			}
			iPtr->result = value;
			src = tmp-1;
			goto copyResult;
		    }
		    break;
		}

		case ']': {
		    if ((openBraces == 0) && (termChar == ']')) {
			goto cmdComplete;
		    }
		    *dst = ']'; dst++;
		    break;
		}

		case ';': {
		    if (!openBraces && !openQuote) {
			goto cmdComplete;
		    }
		    *dst = *src; dst++;
		    break;
		}
    
		case '\n': {

		    /*
		     * A newline can be either a command terminator
		     * or a space character.  If it's a space character,
		     * just fall through to the space code below.
		     */
    
		    if (!openBraces && !openQuote && (termChar == 0)) {
			goto cmdComplete;
		    }
		}

		case '\r':
		case ' ':
		case '\t': {
		    if (openBraces || openQuote) {
    
			/*
			 * Quoted space.  Copy it into the argument.
			 */

			*dst = *src; dst++;
		    } else {

			/*
			 * Argument separator.  If there are many
			 * separators in a row (src == argStart) just
			 * ignore this separator.  Otherwise,
			 * Null-terminate the current argument and
			 * set up for the next one.
			 */

			if (src == argStart) {
			    argStart = src+1;
			    break;
			}
			argStart = src+1;
			*dst = 0;
			dst++; argc++;

			/*
			 * Make sure that the argument array is large enough
			 * for the next argument plus a final NULL argument
			 * pointer to terminate the list.
			 */

			if (argc >= argSize-1) {
			    char **newArgs;
    
			    argSize *= 2;
			    newArgs = (char **)
				    malloc((unsigned) argSize * sizeof(char *));
			    for (i = 0; i < argc; i++) {
				newArgs[i] = argv[i];
			    }
			    if (argv != argStorage) {
				free((char *) argv);
			    }
			    argv = newArgs;
			}
			argv[argc] = dst;
			break;
		    }
		    break;
		}
    
		case '\\': {
		    int numRead;

		    /*
		     * First of all, make the special check for
		     * backslash followed by newline.  This can't
		     * be processed in the normal fashion of
		     * Tcl_Backslash because is maps to "nothing",
		     * rather than to a character.
		     */

		    if (src[1] == '\n') {
			if (argStart  == src) {
			    argStart += 2;
			}
			src++;
			break;
		    }

		    /*
		     * If we're in an argument in braces then the
		     * backslash doesn't get collapsed.  However,
		     * whether we're in braces or not the characters
		     * inside the backslash sequence must not receive
		     * any additional processing:  make src point to
		     * the last character of the sequence.
		     */

		    *dst = Tcl_Backslash(src, &numRead);
		    if (openBraces > 0) {
			for ( ; numRead > 0; src++, dst++, numRead--) {
			    *dst = *src;
			}
			src--;
		    } else {
			src += numRead-1;
			dst++;
		    }
		    break;
		}
    
		case 0: {

		    /*
		     * End of string.  Make sure that braces/quotes
		     * were properly matched.  Also, it's only legal
		     * to terminate a command by a null character if
		     * termChar is zero.
		     */

		    if (openQuote != 0) {
			syntaxMsg = "unmatched quote";
			goto syntaxError;
		    }
		    if (openBraces != 0) {
			syntaxMsg = "unmatched brace";
			goto syntaxError;
		    }
		    if (termChar == ']') {
			syntaxPtr = cmd;
			syntaxMsg = "missing close-bracket";
			goto syntaxError;
		    }
		    goto cmdComplete;
		}
    
		default: {
		    *dst = *src; dst++;
		    break;
		}
	    }
	    src += 1;
    
	    /*
	     * Make sure that we're not running out of space in the
	     * string copy area.  If we are, allocate a larger area
	     * and copy the string.  Be sure to update all of the
	     * relevant pointers too.
	     */
    
	    if (dst >= limit) {
		char *newCopy;
		int delta;
    
		copySize *= 2;
		newCopy = (char *) malloc((unsigned) copySize);
		bcopy(copy, newCopy, (dst-copy));
		delta = newCopy - copy;
		dst += delta;
		for (i = 0; i <= argc; i++) {
		    argv[i] += delta;
		}
		if (copy != copyStorage) {
		    free((char *) copy);
		}
		copy = newCopy;
		limit = newCopy + copySize - BUFFER;
	    }
    
	}
    
	/*
	 * Terminate the last argument and add a final NULL argument.  If
	 * the interpreter has been deleted then return;  if there's no
	 * command, then go on to the next iteration.
	 */

	cmdComplete:
	if (iPtr->flags & DELETED) {
	    goto done;
	}
	if (src != argStart) {
	    *dst = 0;
	    argc++;
	}
	if ((argc == 0) || iPtr->noEval) {
	    continue;
	}
	argv[argc] = NULL;

	cmdPtr = TclFindCmd(iPtr, argv[0], 1);
	if (cmdPtr == NULL) {
	    Tcl_Return(interp, (char *) NULL, TCL_STATIC);
	    sprintf(iPtr->result,
		    "\"%.50s\" is an invalid command name %s",
		    argv[0], "or ambiguous abbreviation");
	    result = TCL_ERROR;
	    goto done;
	}

	/*
	 * Replace argv[0] with the full name of the command (in case
	 * argv[0] was an abbreviation).
	 */

	argv[0] = cmdPtr->name;

	/*
	 * Call trace procedures, if any.
	 */

	for (tracePtr = iPtr->tracePtr; tracePtr != NULL;
		tracePtr = tracePtr->nextPtr) {
	    char saved;

	    if (tracePtr->level < iPtr->numLevels) {
		continue;
	    }
	    saved = *src;
	    *src = 0;
	    (*tracePtr->proc)(tracePtr->clientData, interp, iPtr->numLevels,
		    cmdStart, cmdPtr->proc, cmdPtr->clientData, argc, argv);
	    *src = saved;
	}

	/*
	 * Save information for the history module, if needed.
	 */

	if (flags & TCL_RECORD_BOUNDS) {
	    iPtr->evalFirst = cmdStart;
	    iPtr->evalLast = src;
	} else {
	    iPtr->evalFirst = NULL;
	}

	/*
	 * At long last, invoke the command procedure.  Reset the
	 * result to its default empty value first.
	 */

	iPtr->cmdCount++;
	iPtr->flags &= ~ERR_IN_PROGRESS;
	if (iPtr->dynamic) {
	    free((char *) iPtr->result);
	    iPtr->dynamic = 0;
	}
	iPtr->result = iPtr->resultSpace;
	iPtr->resultSpace[0] = 0;
	result = (*cmdPtr->proc)(cmdPtr->clientData, interp, argc, argv);
	if (result != TCL_OK) {
	    break;
	}
    }

    /*
     * Free up any extra resources that were allocated.
     */

    done:
    if (copy != copyStorage) {
	free((char *) copy);
    }
    if (argv != argStorage) {
	free((char *) argv);
    }
    iPtr->numLevels--;
    if (iPtr->numLevels == 0) {
	if (result == TCL_RETURN) {
	    result = TCL_OK;
	}
	if ((result != TCL_OK) && (result != TCL_ERROR)) {
	    Tcl_Return(interp, (char *) NULL, TCL_STATIC);
	    if (result == TCL_BREAK) {
		iPtr->result = "invoked \"break\" outside of a loop";
	    } else if (result == TCL_CONTINUE) {
		iPtr->result = "invoked \"continue\" outside of a loop";
	    } else {
		iPtr->result = iPtr->resultSpace;
		sprintf(iPtr->resultSpace, "command returned bad code: %d",
			result);
	    }
	    result = TCL_ERROR;
	}
	if (iPtr->flags & DELETED) {
	    Tcl_DeleteInterp(interp);
	}
    }

    /*
     * If an error occurred, record information about what was being
     * executed when the error occurred.
     */

    if ((result == TCL_ERROR) && !(iPtr->flags & ERR_ALREADY_LOGGED)) {
	int numChars;
	register char *p;
	char *ellipsis;

	/*
	 * Compute the line number where the error occurred.
	 */

	iPtr->errorLine = 1;
	for (p = cmd; p != cmdStart; p++) {
	    if (*p == '\n') {
		iPtr->errorLine++;
	    }
	}
	for ( ; isspace(*p) || (*p == ';'); p++) {
	    if (*p == '\n') {
		iPtr->errorLine++;
	    }
	}

	/*
	 * Figure out how much of the command to print in the error
	 * message (up to a certain number of characters, or up to
	 * the first new-line).
	 */

	ellipsis = "";
	p = strchr(cmdStart, '\n');
	if (p == NULL) {
	    numChars = strlen(cmdStart);
	} else {
	    numChars = p - cmdStart;
	    if (p[1] != 0) {
		ellipsis = " ...";
	    }
	}
	if (numChars > 40) {
	    numChars = 40;
	    ellipsis = " ...";
	}

	if (!(iPtr->flags & ERR_IN_PROGRESS)) {
	    /*
	     * This is the first piece of information being recorded
	     * for this error.  Log the error message as well as the
	     * command being executed.
	     */

	    if (strlen(iPtr->result) < 50) {
		sprintf(copyStorage,
			"%s, while executing\n\"%.*s%s\"",
			iPtr->result, numChars, cmdStart, ellipsis);
	    } else {
		sprintf(copyStorage,
			"%.50s..., while executing\n\"%.*s%s\"",
			iPtr->result, numChars, cmdStart, ellipsis);
	    }
	} else {
	    sprintf(copyStorage, ", invoked from within\n\"%.*s%s\"",
		    numChars, cmdStart, ellipsis);
	}
	Tcl_AddErrorInfo(interp, copyStorage);
	iPtr->flags &= ~ERR_ALREADY_LOGGED;
    } else {
	iPtr->flags &= ~ERR_ALREADY_LOGGED;
    }
    return result;

    /*
     * Syntax error:  generate an error message.
     */

    syntaxError: {
	char *first, *last;

	Tcl_Return(interp, (char *) NULL, TCL_STATIC);
	for (first = syntaxPtr; ((first != cmd) && (first[-1] != '\n'));
		first--) {
	    /* Null loop body. */
	}
	for (last = syntaxPtr; ((*last != 0) && (*last!= '\n')); last++) {
	    /* Null loop body. */
	}
	if ((syntaxPtr - first) > 60) {
	    first = syntaxPtr - 60;
	}
	if ((last - first) > 70) {
	    last = first + 70;
	}
	if (last == first) {
	    sprintf(iPtr->result, "%s", syntaxMsg);
	} else {
	    sprintf(iPtr->result, "%s: '%.*s => %.*s'", syntaxMsg,
		    syntaxPtr-first, first, last-syntaxPtr, syntaxPtr);
	}
	result = TCL_ERROR;
    }

    goto done;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_CreateTrace --
 *
 *	Arrange for a procedure to be called to trace command execution.
 *
 * Results:
 *	The return value is a token for the trace, which may be passed
 *	to Tcl_DeleteTrace to eliminate the trace.
 *
 * Side effects:
 *	From now on, proc will be called just before a command procedure
 *	is called to execute a Tcl command.  Calls to proc will have the
 *	following form:
 *
 *	void
 *	proc(clientData, interp, level, command, cmdProc, cmdClientData,
 *		argc, argv)
 *	    ClientData clientData;
 *	    Tcl_Interp *interp;
 *	    int level;
 *	    char *command;
 *	    int (*cmdProc)();
 *	    ClientData cmdClientData;
 *	    int argc;
 *	    char **argv;
 *	{
 *	}
 *
 *	The clientData and interp arguments to proc will be the same
 *	as the corresponding arguments to this procedure.  Level gives
 *	the nesting level of command interpretation for this interpreter
 *	(0 corresponds to top level).  Command gives the ASCII text of
 *	the raw command, cmdProc and cmdClientData give the procedure that
 *	will be called to process the command and the ClientData value it
 *	will receive, and argc and argv give the arguments to the
 *	command, after any argument parsing and substitution.  Proc
 *	does not return a value.
 *
 *----------------------------------------------------------------------
 */

Tcl_Trace
Tcl_CreateTrace(interp, level, proc, clientData)
    Tcl_Interp *interp;		/* Interpreter in which to create the trace. */
    int level;			/* Only call proc for commands at nesting level
				 * <= level (1 => top level). */
    void (*proc)();		/* Procedure to call before executing each
				 * command. */
    ClientData clientData;	/* Arbitrary one-word value to pass to proc. */
{
    register Trace *tracePtr;
    register Interp *iPtr = (Interp *) interp;

    tracePtr = (Trace *) malloc(sizeof(Trace));
    tracePtr->level = level;
    tracePtr->proc = proc;
    tracePtr->clientData = clientData;
    tracePtr->nextPtr = iPtr->tracePtr;
    iPtr->tracePtr = tracePtr;

    return (Tcl_Trace) tracePtr;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_DeleteTrace --
 *
 *	Remove a trace.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	From now on there will be no more calls to the procedure given
 *	in trace.
 *
 *----------------------------------------------------------------------
 */

void
Tcl_DeleteTrace(interp, trace)
    Tcl_Interp *interp;		/* Interpreter that contains trace. */
    Tcl_Trace trace;		/* Token for trace (returned previously by
				 * Tcl_CreateTrace). */
{
    register Interp *iPtr = (Interp *) interp;
    register Trace *tracePtr = (Trace *) trace;
    register Trace *tracePtr2;

    if (iPtr->tracePtr == tracePtr) {
	iPtr->tracePtr = tracePtr->nextPtr;
	free((char *) tracePtr);
    } else {
	for (tracePtr2 = iPtr->tracePtr; tracePtr2 != NULL;
		tracePtr2 = tracePtr2->nextPtr) {
	    if (tracePtr2->nextPtr == tracePtr) {
		tracePtr2->nextPtr = tracePtr->nextPtr;
		free((char *) tracePtr);
		return;
	    }
	}
    }
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_AddErrorInfo --
 *
 *	Add information to a message being accumulated that describes
 *	the current error.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The contents of message are added to the "errorInfo" variable.
 *	If Tcl_Eval has been called since the current value of errorInfo
 *	was set, errorInfo is cleared before adding the new message.
 *
 *----------------------------------------------------------------------
 */

void
Tcl_AddErrorInfo(interp, message)
    Tcl_Interp *interp;		/* Interpreter to which error information
				 * pertains. */
    char *message;		/* Message to record. */
{
    register Interp *iPtr = (Interp *) interp;

    if (iPtr->flags & ERR_IN_PROGRESS) {
	int length;
	char *buffer, *oldVar;

	oldVar = Tcl_GetVar(interp, "errorInfo", 1);
	if (oldVar == NULL) {
	    oldVar = "";
	}
	length = strlen(oldVar);
	buffer = malloc((unsigned) (length + strlen(message) + 1));
	strcpy(buffer, oldVar);
	strcpy(buffer+length, message);
	Tcl_SetVar(interp, "errorInfo", buffer, 1);
    } else {
	iPtr->flags |= ERR_IN_PROGRESS;
	Tcl_SetVar(interp, "errorInfo", message, 1);
    }
}

/*
 *----------------------------------------------------------------------
 *
 * TclFindCmd --
 *
 *	Find a particular command in an interpreter.
 *
 * Results:
 *	If the command doesn't exist in the table, or if it is an
 *	ambiguous abbreviation, then NULL is returned.  Otherwise
 *	the return value is a pointer to the command.  Unique
 *	abbreviations are allowed if abbrevOK is non-zero, but
 *	abbreviations take longer to look up (must scan the whole
 *	table twice).
 *
 * Side effects:
 *	If the command is found and is an exact match, it is relinked
 *	at the front of iPtr's command list so it will be found more
 *	quickly in the future.
 *
 *----------------------------------------------------------------------
 */

Command *
TclFindCmd(iPtr, cmdName, abbrevOK)
    Interp *iPtr;		/* Interpreter in which to search. */
    char *cmdName;		/* Desired command. */
    int abbrevOK;		/* Non-zero means permit abbreviations, if
				 * not disallowed by "noAbbrevs" variable.
				 * Zero means exact matches only. */
{
    register Command *prev;
    register Command *cur;
    register char c;
    Command *match;
    int length;
    char *varValue;

    /*
     * First check for an exact match.
     */

    c = *cmdName;
    for (prev = NULL, cur = iPtr->commandPtr; cur != NULL;
	    prev = cur, cur = cur->nextPtr) {

	/*
	 * Check the first character here before wasting time calling
	 * strcmp.
	 */

	if ((cur->name[0] == c) && (strcmp(cur->name, cmdName) == 0)) {
	    if (prev != NULL) {
		prev->nextPtr = cur->nextPtr;
		cur->nextPtr = iPtr->commandPtr;
		iPtr->commandPtr = cur;
	    }
	    return cur;
	}
    }
    if (!abbrevOK) {
	return NULL;
    }
    varValue = Tcl_GetVar((Tcl_Interp *) iPtr, "noAbbrev", 1);
    if ((varValue != NULL) && (*varValue == '1')) {
	return NULL;
    }

    /*
     * No exact match.  Make a second pass to check for a unique
     * abbreviation.  Don't bother to pull the matching entry to
     * the front of the list, since we have to search the whole list
     * for abbreviations anyway.
     */

    length = strlen(cmdName);
    match = NULL;
    for (prev = NULL, cur = iPtr->commandPtr; cur != NULL;
	    prev = cur, cur = cur->nextPtr) {
	if ((cur->name[0] == c) && (strncmp(cur->name, cmdName, length) == 0)) {
	    if (match != NULL) {
		return NULL;
	    }
	    match = cur;
	}
    }
    return match;
}

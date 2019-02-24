/* 
 * tclHistory.c --
 *
 *	This module implements history as an optional addition to Tcl.
 *	It can be called to record commands ("events") before they are
 *	executed, and it provides a command that may be used to perform
 *	history substitutions.
 *
 * Copyright 1990 Regents of the University of California
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies.  The University of California
 * makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 */

#ifndef lint
static char rcsid[] = "$Header: /sprite/src/lib/tcl/RCS/tclHistory.c,v 1.6 90/03/29 13:20:04 ouster Exp $ SPRITE (Berkeley)";
#pragma ref rcsid
#endif not lint

#define	_POSIX_SOURCE

#include "tclInt.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * This history stuff is mostly straightforward, except for one thing
 * that makes everything very complicated.  Suppose that the following
 * commands get executed:
 *	echo foo
 *	history redo
 * It's important that the history event recorded for the second command
 * be "echo foo", not "history redo".  Otherwise, if another "history redo"
 * command is typed, it will result in infinite recursions on the
 * "history redo" command.  Thus, the actual recorded history must be
 *	echo foo
 *	echo foo
 * To do this, the history command revises recorded history as part of
 * its execution.  In the example above, when "history redo" starts
 * execution, the current event is "history redo", but the history
 * command arranges for the current event to be changed to "echo foo".
 *
 * There are three additional complications.  The first is that history
 * substitution may only be part of a command, as in the following
 * command sequence:
 *	echo foo bar
 *	echo [history word 3]
 * In this case, the second event should be recorded as "echo bar".  Only
 * part of the recorded event is to be modified.  Fortunately, Tcl_Eval
 * helps with this by recording (in the evalFirst and evalLast fields of
 * the intepreter) the location of the command being executed, so the
 * history module can replace exactly the range of bytes corresponding
 * to the history substitution command.
 *
 * The second complication is that there are two ways to revise history:
 * replace a command, and replace the result of a command.  Consider the
 * two examples below:
 *	format {result is %d} $num	   |	format {result is %d} $num
 *	print [history redo]		   |	print [history word 3]
 * Recorded history for these two cases should be as follows:
 *	format {result is %d} $num	   |	format {result is %d} $num
 *	print [format {result is %d} $num] |	print $num
 * In the left case, the history command was replaced with another command
 * to be executed (the brackets were retained), but in the case on the
 * right the result of executing the history command was replaced (i.e.
 * brackets were replaced too).
 *
 * The third complication is that there could potentially be many
 * history substitutions within a single command, as in:
 *	echo [history word 3] [history word 2]
 * There could even be nested history substitutions, as in:
 *	history subs abc [history word 2]
 * If history revisions were made immediately during each "history" command
 * invocations, it would be very difficult to produce the correct cumulative
 * effect from several substitutions in the same command.  To get around
 * this problem, the actual history revision isn't made during the execution
 * of the "history" command.  Information about the changes is just recorded,
 * in xxx records, and the actual changes are made during the next call to
 * Tcl_RecordHistory (when we know that execution of the previous command
 * has finished).
 */

/*
 * Default space allocation for command strings:
 */

#define INITIAL_CMD_SIZE 40

/*
 * Forward declarations for procedures defined later in this file:
 */

static void		DisableRevs();
static void		DoRevs();
static HistoryEvent *	GetEvent();
static char *		GetWords();
static void		HistoryInit();
static void		InsertRev();
static void		MakeSpace();
static void		RevCommand();
static void		RevResult();
static int		SubsAndEval();

/*
 *----------------------------------------------------------------------
 *
 * Tcl_RecordAndEval --
 *
 *	This procedure adds its command argument to the current list of
 *	recorded events and then executes the command by calling Tcl_Eval.
 *
 * Results:
 *	The return value is a standard Tcl return value, the result of
 *	executing cmd.
 *
 * Side effects:
 *	The command is recorded and executed.  In addition, pending history
 *	revisions are carried out, and information is set up to enable
 *	Tcl_Eval to identify history command ranges.  This procedure also
 *	initializes history information for the interpreter, if it hasn't
 *	already been initialized.
 *
 *----------------------------------------------------------------------
 */

int
Tcl_RecordAndEval(interp, cmd, flags)
    Tcl_Interp *interp;		/* Token for interpreter in which command
				 * will be executed. */
    char *cmd;			/* Command to record. */
    int flags;			/* Additional flags to pass to Tcl_Eval. 
				 * TCL_NO_EVAL means only record: don't
				 * execute command. */
{
    register Interp *iPtr = (Interp *) interp;
    register HistoryEvent *eventPtr;
    char *savedFirst;
    int length, result;

    if (iPtr->numEvents == 0) {
	HistoryInit(iPtr, 20);
    }
    DoRevs(iPtr);

    /*
     * Don't record empty commands.
     */

    while (isspace(*cmd)) {
	cmd++;
    }
    if (*cmd == '\0') {
	Tcl_Return(interp, (char *) NULL, TCL_STATIC);
	return TCL_OK;
    }

    iPtr->curEventNum++;
    iPtr->curEvent++;
    if (iPtr->curEvent >= iPtr->numEvents) {
	iPtr->curEvent = 0;
    }
    eventPtr = &iPtr->events[iPtr->curEvent];

    /*
     * Chop off trailing newlines before recording the command.
     */

    length = strlen(cmd);
    while (cmd[length-1] == '\n') {
	length--;
    }
    MakeSpace(eventPtr, length + 1);
    strncpy(eventPtr->command, cmd, length);
    eventPtr->command[length] = 0;

    if (flags == -1) {
	return TCL_OK;
    }

    /*
     * Execute the command.
     */

    savedFirst = iPtr->historyFirst;
    iPtr->historyFirst = cmd;
    result = Tcl_Eval(interp, cmd, flags | TCL_RECORD_BOUNDS, (char **) NULL);
    iPtr->historyFirst = savedFirst;
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_HistoryCmd --
 *
 *	This procedure is invoked to process the "history" Tcl command.
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
Tcl_HistoryCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
#pragma ref dummy
    register Interp *iPtr = (Interp *) interp;
    register HistoryEvent *eventPtr;
    int length;
    char c;

    /*
     * If no arguments, redo last command.
     */

    if (argc == 1) {
	eventPtr = GetEvent(iPtr, "-1");
	if (eventPtr == NULL) {
	    return TCL_ERROR;
	}
	RevCommand(iPtr, eventPtr->command);
	return Tcl_Eval(interp, eventPtr->command, 0, (char **) NULL);
    }

    c = argv[1][0];
    length = strlen(argv[1]);

    if ((c == 'a') && (strncmp(argv[1], "add", length)) == 0) {
	if ((argc != 3) && (argc != 4)) {
	    sprintf(iPtr->result,
		    "wrong # args:  should be \"%.50s add event [exec]\"",
		    argv[0]);
	    return TCL_ERROR;
	}
	if (argc == 4) {
	    if (strncmp(argv[3], "exec", strlen(argv[3])) != 0) {
		sprintf(iPtr->result,
			"bad arg \"%.50s\":  should be \"exec\"", argv[3]);
		return TCL_ERROR;
	    }
	    return Tcl_RecordAndEval(interp, argv[2], 0);
	}
	return Tcl_RecordAndEval(interp, argv[2], -1);
    } else if ((c == 'c') && (strncmp(argv[1], "change", length)) == 0) {
	if ((argc != 3) && (argc != 4)) {
	    sprintf(iPtr->result, "wrong # args:  should be \"%.50s change newValue [event]\"",
		    argv[0]);
	    return TCL_ERROR;
	}
	if (argc == 3) {
	    eventPtr = &iPtr->events[iPtr->curEvent];
	    DisableRevs(iPtr);
	} else {
	    eventPtr = GetEvent(iPtr, argv[3]);
	    if (eventPtr == NULL) {
		return TCL_ERROR;
	    }
	}
	MakeSpace(eventPtr, strlen(argv[2]) + 1);
	strcpy(eventPtr->command, argv[2]);
	return TCL_OK;
    } else if ((c == 'e') && (strncmp(argv[1], "event", length)) == 0) {
	if (argc > 3) {
	    sprintf(iPtr->result,
		    "too many args:  should be \"%.50s event [event]\"",
		    argv[0]);
	    return TCL_ERROR;
	}
	eventPtr = GetEvent(iPtr, argc==2 ? "-1" : argv[2]);
	if (eventPtr == NULL) {
	    return TCL_ERROR;
	}
	RevResult(iPtr, eventPtr->command);
	Tcl_Return(interp, eventPtr->command, TCL_VOLATILE);
	return TCL_OK;
    } else if ((c == 'i') && (strncmp(argv[1], "info", length)) == 0) {
	char *p;
	int count, indx, i;

	if ((argc != 2) && (argc != 3)) {
	    sprintf(iPtr->result,
		    "wrong # args:  should be \"%.50s info [count]\"",
		    argv[0]);
	    return TCL_ERROR;
	}
	if (argc == 3) {
	    char *end;

	    count = strtoul(argv[2], &end, 0);
	    if (end == argv[2]) {
		sprintf(iPtr->result, "bad count \"%.50s\"", argv[2]);
		return TCL_ERROR;
	    }
	    if (count > iPtr->numEvents) {
		count = iPtr->numEvents;
	    }
	} else {
	    count = iPtr->numEvents;
	}
	length = 0;
	for (i = 0, indx = iPtr->curEvent + 1 + iPtr->numEvents - count;
		i < count; i++, indx++) {
	    if (indx >= iPtr->numEvents) {
		indx -= iPtr->numEvents;
	    }
	    p = iPtr->events[indx].command;
	    length += 9 + strlen(p);
	    while (1) {
		p = strchr(p, '\n');
		if (p == NULL) {
		    break;
		}
		length++;
		p++;
	    }
	    length += 9 + strlen(iPtr->events[indx].command);
	}
	p = malloc((unsigned) (length+1));
	iPtr->result = p;
	iPtr->dynamic = 1;
	for (i = 0, indx = iPtr->curEvent + 1 + iPtr->numEvents - count;
		i < count; i++, indx++) {
	    char *cur, *next;
	    int length;

	    if (indx >= iPtr->numEvents) {
		indx -= iPtr->numEvents;
	    }
	    cur = iPtr->events[indx].command;
	    if (*cur == '\0') {
		continue;		/* No command recorded here. */
	    }
	    sprintf(p, "%6d  ", iPtr->curEventNum + 1 - (count - i));
	    p += 8;

	    /*
	     * Tricky formatting here:  for multi-line commands, indent
	     * the continuation lines.
	     */

	    while (1) {
		next = strchr(cur, '\n');
		if (next == NULL) {
		    break;
		}
		length = next+1-cur;
		strncpy(p, cur,length);
		cur += length;
		p += length;
		*p = '\t';
		p++;
	    }
	    strcpy(p, cur);
	    p += strlen(p);
	    *p = '\n';
	    p++;
	}
	p[-1] = '\0';
	return TCL_OK;
    } else if ((c == 'k') && (strncmp(argv[1], "keep", length)) == 0) {
	int count, i, src;
	char *end;
	HistoryEvent *events;

	if (argc != 3) {
	    sprintf(iPtr->result,
		    "wrong # args:  should be \"%.50s keep number\"",
		    argv[0]);
	    return TCL_ERROR;
	}
	count = strtoul(argv[2], &end, 0);
	if ((end == argv[2]) || (count > 1000) || (count == 0)) {
	    sprintf(iPtr->result, "bad number \"%.50s\"", argv[2]);
	    return TCL_ERROR;
	}

	/*
	 * Create a new history array and copy as much existing history
	 * as possible from the old array.
	 */

	events = (HistoryEvent *)
		malloc((unsigned) (count * sizeof(HistoryEvent)));
	if (count < iPtr->numEvents) {
	    src = iPtr->curEvent + 1 - count;
	    if (src < 0) {
		src += iPtr->numEvents;
	    }
	} else {
	    src = iPtr->curEvent + 1;
	}
	for (i = 0; i < count; i++, src++) {
	    if (src >= iPtr->numEvents) {
		src = 0;
	    }
	    if (i < iPtr->numEvents) {
		events[i] = iPtr->events[src];
		iPtr->events[src].command = NULL;
	    } else {
		events[i].command = malloc(INITIAL_CMD_SIZE);
		events[i].command[0] = 0;
		events[i].bytesAvl = INITIAL_CMD_SIZE;
	    }
	}

	/*
	 * Throw away everything left in the old history array, and
	 * substitute the new one for the old one.
	 */

	for (i = 0; i < iPtr->numEvents; i++) {
	    if (iPtr->events[i].command != NULL) {
		free(iPtr->events[i].command);
	    }
	}
	free((char *) iPtr->events);
	iPtr->events = events;
	if (count < iPtr->numEvents) {
	    iPtr->curEvent = count-1;
	} else {
	    iPtr->curEvent = iPtr->numEvents-1;
	}
	iPtr->numEvents = count;
	return TCL_OK;
    } else if ((c == 'n') && (strncmp(argv[1], "nextid", length)) == 0) {
	if (argc != 2) {
	    sprintf(iPtr->result, "wrong # args:  should be \"%.50s nextid\"",
		    argv[0]);
	    return TCL_ERROR;
	}
	sprintf(iPtr->result, "%d", iPtr->curEventNum+1);
	return TCL_OK;
    } else if ((c == 'r') && (strncmp(argv[1], "redo", length)) == 0) {
	if (argc > 3) {
	    sprintf(iPtr->result,
		    "too many args:  should be \"%.50s redo [event]\"",
		    argv[0]);
	    return TCL_ERROR;
	}
	eventPtr = GetEvent(iPtr, argc==2 ? "-1" : argv[2]);
	if (eventPtr == NULL) {
	    return TCL_ERROR;
	}
	RevCommand(iPtr, eventPtr->command);
	return Tcl_Eval(interp, eventPtr->command, 0, (char **) NULL);
    } else if ((c == 's') && (strncmp(argv[1], "substitute", length)) == 0) {
	if ((argc > 5) || (argc < 4)) {
	    sprintf(iPtr->result, "wrong # args:  should be \"%.50s substitute old new [event]\"",
		    argv[0]);
	    return TCL_ERROR;
	}
	eventPtr = GetEvent(iPtr, argc==4 ? "-1" : argv[4]);
	if (eventPtr == NULL) {
	    return TCL_ERROR;
	}
	return SubsAndEval(iPtr, eventPtr->command, argv[2], argv[3]);
    } else if ((c == 'w') && (strncmp(argv[1], "words", length)) == 0) {
	char *words;

	if ((argc != 3) && (argc != 4)) {
	    sprintf(iPtr->result, "wrong # args:  should be \"%.50s words num-num/pat [event]\"",
		    argv[0]);
	    return TCL_ERROR;
	}
	eventPtr = GetEvent(iPtr, argc==3 ? "-1" : argv[3]);
	if (eventPtr == NULL) {
	    return TCL_ERROR;
	}
	words = GetWords(iPtr, eventPtr->command, argv[2]);
	if (words == NULL) {
	    return TCL_ERROR;
	}
	RevResult(iPtr, words);
	iPtr->result = words;
	iPtr->dynamic = 1;
	return TCL_OK;
    }

    sprintf(iPtr->result, "bad \"%.50s\" option \"%.50s\": must be add, change, event, info, keep, nextid, redo, substitute, or words",
		argv[0], argv[1]);
    return TCL_ERROR;
}

/*
 *----------------------------------------------------------------------
 *
 * HistoryInit --
 *
 *	Initialize history-related state in an interpreter.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	History info is initialized in iPtr.
 *
 *----------------------------------------------------------------------
 */

static void
HistoryInit(iPtr, numEvents)
    register Interp *iPtr;		/* Interpreter to initialize. */
    int numEvents;			/* Number of events to retain at
					 * any given time. */
{
    int i;

    iPtr->numEvents = numEvents;
    iPtr->events = (HistoryEvent *)
	    malloc((unsigned) (numEvents * sizeof(HistoryEvent)));
    for (i = 0; i < numEvents; i++) {
	iPtr->events[i].command = malloc(INITIAL_CMD_SIZE);
	*iPtr->events[i].command = 0;
	iPtr->events[i].bytesAvl = INITIAL_CMD_SIZE;
    }
    iPtr->curEvent = 0;
    iPtr->curEventNum = 0;
    Tcl_CreateCommand((Tcl_Interp *) iPtr, "history", Tcl_HistoryCmd,
	    (ClientData) NULL, (void (*)()) NULL);
}

/*
 *----------------------------------------------------------------------
 *
 * MakeSpace --
 *
 *	Given a history event, make sure it has enough space for
 *	a string of a given length (enlarge the string area if
 *	necessary).
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	More memory may get allocated.
 *
 *----------------------------------------------------------------------
 */

static void
MakeSpace(hPtr, size)
    HistoryEvent *hPtr;
    int size;			/* # of bytes needed in hPtr. */
{
    if (hPtr->bytesAvl < size) {
	free(hPtr->command);
	hPtr->command = malloc((unsigned) size);
	hPtr->bytesAvl = size;
    }
}

/*
 *----------------------------------------------------------------------
 *
 * InsertRev --
 *
 *	Add a new revision to the list of those pending for iPtr.
 *	Do it in a way that keeps the revision list sorted in
 *	increasing order of firstIndex.  Also, eliminate revisions
 *	that are subsets of other revisions.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	RevPtr is added to iPtr's revision list.
 *
 *----------------------------------------------------------------------
 */

static void
InsertRev(iPtr, revPtr)
    Interp *iPtr;			/* Interpreter to use. */
    register HistoryRev *revPtr;	/* Revision to add to iPtr's list. */
{
    register HistoryRev *curPtr;
    register HistoryRev *prevPtr;

    for (curPtr = iPtr->revPtr, prevPtr = NULL; curPtr != NULL;
	    prevPtr = curPtr, curPtr = curPtr->nextPtr) {
	/*
	 * If this revision includes the new one (or vice versa) then
	 * just eliminate the one that is a subset of the other.
	 */

	if ((revPtr->firstIndex <= curPtr->firstIndex)
		&& (revPtr->lastIndex >= curPtr->firstIndex)) {
	    curPtr->firstIndex = revPtr->firstIndex;
	    curPtr->lastIndex = revPtr->lastIndex;
	    curPtr->newSize = revPtr->newSize;
	    free(curPtr->newBytes);
	    curPtr->newBytes = revPtr->newBytes;
	    free((char *) revPtr);
	    return;
	}
	if ((revPtr->firstIndex >= curPtr->firstIndex)
		&& (revPtr->lastIndex <= curPtr->lastIndex)) {
	    free(revPtr->newBytes);
	    free((char *) revPtr);
	    return;
	}

	if (revPtr->firstIndex < curPtr->firstIndex) {
	    break;
	}
    }

    /*
     * Insert revPtr just after prevPtr.
     */

    if (prevPtr == NULL) {
	revPtr->nextPtr = iPtr->revPtr;
	iPtr->revPtr = revPtr;
    } else {
	revPtr->nextPtr = prevPtr->nextPtr;
	prevPtr->nextPtr = revPtr;
    }
}

/*
 *----------------------------------------------------------------------
 *
 * RevCommand --
 *
 *	This procedure is invoked by the "history" command to record
 *	a command revision.  See the comments at the beginning of the
 *	file for more information about revisions.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Revision information is recorded.
 *
 *----------------------------------------------------------------------
 */

static void
RevCommand(iPtr, string)
    register Interp *iPtr;	/* Interpreter in which to perform the
				 * substitution. */
    char *string;		/* String to substitute. */
{
    register HistoryRev *revPtr;

    if ((iPtr->evalFirst == NULL) || (iPtr->historyFirst == NULL)) {
	return;
    }
    revPtr = (HistoryRev *) malloc(sizeof(HistoryRev));
    revPtr->firstIndex = iPtr->evalFirst - iPtr->historyFirst;
    revPtr->lastIndex = iPtr->evalLast - iPtr->historyFirst - 1;
    revPtr->newSize = strlen(string);
    revPtr->newBytes = malloc((unsigned) (revPtr->newSize+1));
    strcpy(revPtr->newBytes, string);
    InsertRev(iPtr, revPtr);
}

/*
 *----------------------------------------------------------------------
 *
 * RevResult --
 *
 *	This procedure is invoked by the "history" command to record
 *	a result revision.  See the comments at the beginning of the
 *	file for more information about revisions.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Revision information is recorded.
 *
 *----------------------------------------------------------------------
 */

static void
RevResult(iPtr, string)
    register Interp *iPtr;	/* Interpreter in which to perform the
				 * substitution. */
    char *string;		/* String to substitute. */
{
    register HistoryRev *revPtr;
    char *evalFirst, *evalLast;
    char *argv[2];

    if ((iPtr->evalFirst == NULL) || (iPtr->historyFirst == NULL)) {
	return;
    }

    /*
     * Expand the replacement range to include the brackets that surround
     * the command.  If there aren't any brackets (i.e. this command was
     * invoked at top-level) then don't do any revision.  Also, if there
     * are several commands in brackets, of which this is just one,
     * then don't do any revision.
     */

    evalFirst = iPtr->evalFirst;
    evalLast = iPtr->evalLast;
    while (1) {
	if (evalFirst == iPtr->historyFirst) {
	    return;
	}
	evalFirst--;
	if (*evalFirst == '[') {
	    break;
	}
	if (!isspace(*evalFirst)) {
	    return;
	}
    }
    if (*evalLast != ']') {
	return;
    }

    revPtr = (HistoryRev *) malloc(sizeof(HistoryRev));
    revPtr->firstIndex = evalFirst - iPtr->historyFirst;
    revPtr->lastIndex = evalLast - iPtr->historyFirst;
    argv[0] = string;
    revPtr->newBytes = Tcl_Merge(1, argv);
    revPtr->newSize = strlen(revPtr->newBytes);
    InsertRev(iPtr, revPtr);
}

/*
 *----------------------------------------------------------------------
 *
 * DoRevs --
 *
 *	This procedure is called to apply the history revisions that
 *	have been recorded in iPtr.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The most recent entry in the history for iPtr may be modified.
 *
 *----------------------------------------------------------------------
 */

static void
DoRevs(iPtr)
    register Interp *iPtr;	/* Interpreter whose history is to
				 * be modified. */
{
    register HistoryRev *revPtr;
    register HistoryEvent *eventPtr;
    char *newCommand, *p;
    unsigned int size;
    int bytesSeen, count;

    if (iPtr->revPtr == NULL) {
	return;
    }

    /*
     * The revision is done in two passes.  The first pass computes the
     * amount of space needed for the revised event, and the second pass
     * pieces together the new event and frees up the revisions.
     */

    eventPtr = &iPtr->events[iPtr->curEvent];
    size = strlen(eventPtr->command);
    for (revPtr = iPtr->revPtr; revPtr != NULL; revPtr = revPtr->nextPtr) {
	size -= revPtr->lastIndex + 1 - revPtr->firstIndex;
	size += revPtr->newSize;
    }

    newCommand = malloc(size);
    p = newCommand;
    bytesSeen = 0;
    for (revPtr = iPtr->revPtr; revPtr != NULL; revPtr = revPtr->nextPtr) {
	count = revPtr->firstIndex - bytesSeen;
	if (count > 0) {
	    strncpy(p, eventPtr->command + bytesSeen, count);
	    p += count;
	}
	strncpy(p, revPtr->newBytes, revPtr->newSize);
	p += revPtr->newSize;
	bytesSeen = revPtr->lastIndex+1;
	free(revPtr->newBytes);
	free((char *) revPtr);
    }
    strcpy(p, eventPtr->command + bytesSeen);

    /*
     * Replace the command in the event.
     */

    free(eventPtr->command);
    eventPtr->command = newCommand;
    eventPtr->bytesAvl = size;
    iPtr->revPtr = NULL;
}

/*
 *----------------------------------------------------------------------
 *
 * DisableRevs --
 *
 *	Turn off history revision for this command.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The state of iPtr is modified to discard any pending
 *	history revisions and prevent any future revisions
 *	from being logged for this command.
 *
 *----------------------------------------------------------------------
 */

static void
DisableRevs(iPtr)
    register Interp *iPtr;	/* Interpreter in which to disable revs. */
{
    iPtr->historyFirst = NULL;
    while (iPtr->revPtr != NULL) {
	free(iPtr->revPtr->newBytes);
	free((char *) iPtr->revPtr);
	iPtr->revPtr = iPtr->revPtr->nextPtr;
    }
}

/*
 *----------------------------------------------------------------------
 *
 * GetEvent --
 *
 *	Given a textual description of an event (see the manual page
 *	for legal values) find the corresponding event and return its
 *	command string.
 *
 * Results:
 *	The return value is a pointer to the event named by "string".
 *	If no such event exists, then NULL is returned and an error
 *	message is left in iPtr.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

static HistoryEvent *
GetEvent(iPtr, string)
    register Interp *iPtr;	/* Interpreter in which to look. */
    char *string;		/* Description of event. */
{
    int eventNum, index;
    char *end;
    register HistoryEvent *eventPtr;
    int length;

    /*
     * First check for a numeric specification of an event.
     */

    if (isdigit(*string) || (*string == '-')) {
	eventNum = strtol(string, &end, 0);
	if (*end != 0) {
	    sprintf(iPtr->result, "bad event number \"%.50s\"", string);
	    return NULL;
	}
	if (eventNum < 0) {
	    eventNum += iPtr->curEventNum;
        }
	if (eventNum > iPtr->curEventNum) {
	    sprintf(iPtr->result, "event \"%.50s\" hasn't occurred yet",
		    string);
	    return NULL;
	}
	if ((eventNum <= iPtr->curEventNum-iPtr->numEvents)
		|| (eventNum <= 0)) {
	    sprintf(iPtr->result, "event \"%.50s\" is too far in the past",
		    string);
	    return NULL;
	}
	index = iPtr->curEvent + (eventNum - iPtr->curEventNum);
	if (index < 0) {
	    index += iPtr->numEvents;
	}
	return &iPtr->events[index];
    }

    /*
     * Next, check for an event that contains the string as a prefix or
     * that matches the string in the sense of Tcl_StringMatch.
     */

    length = strlen(string);
    for (index = iPtr->curEvent - 1; ; index--) {
	if (index < 0) {
	    index += iPtr->numEvents;
	}
	if (index == iPtr->curEvent) {
	    break;
	}
	eventPtr = &iPtr->events[index];
	if ((strncmp(eventPtr->command, string, length) == 0)
		|| Tcl_StringMatch(eventPtr->command, string)) {
	    return eventPtr;
	}
    }

    sprintf(iPtr->result, "no event matches \"%.50s\"", string);
    return NULL;
}

/*
 *----------------------------------------------------------------------
 *
 * SubsAndEval --
 *
 *	Generate a new command by making a textual substitution in
 *	the "cmd" argument.  Then execute the new command.
 *
 * Results:
 *	The return value is a standard Tcl error.
 *
 * Side effects:
 *	History gets revised if the substitution is occurring on
 *	a recorded command line.  Also, the re-executed command
 *	may produce side-effects.
 *
 *----------------------------------------------------------------------
 */

static int
SubsAndEval(iPtr, cmd, old, new)
    register Interp *iPtr;	/* Interpreter in which to execute
				 * new command. */
    char *cmd;			/* Command in which to substitute. */
    char *old;			/* String to search for in command. */
    char *new;			/* Replacement string for "old". */
{
    char *src, *dst, *newCmd;
    int count, oldLength, newLength, length, result;

    /*
     * Figure out how much space it will take to hold the
     * substituted command (and complain if the old string
     * doesn't appear in the original command).
     */

    oldLength = strlen(old);
    newLength = strlen(new);
    src = cmd;
    count = 0;
    while (1) {
	src = strstr(src, old);
	if (src == NULL) {
	    break;
	}
	src += oldLength;
	count++;
    }
    if (count == 0) {
	sprintf(iPtr->result, "\"%.50s\" doesn't appear in event",
		old);
	return TCL_ERROR;
    }
    length = strlen(cmd) + count*(newLength - oldLength);

    /*
     * Generate a substituted command.
     */

    newCmd = malloc((unsigned) (length + 1));
    dst = newCmd;
    while (1) {
	src = strstr(cmd, old);
	if (src == NULL) {
	    strcpy(dst, cmd);
	    break;
	}
	strncpy(dst, cmd, src-cmd);
	dst += src-cmd;
	strcpy(dst, new);
	dst += newLength;
	cmd = src + oldLength;
    }

    RevCommand(iPtr, newCmd);
    result = Tcl_Eval((Tcl_Interp *) iPtr, newCmd, 0, (char **) NULL);
    free(newCmd);
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * GetWords --
 *
 *	Given a command string, return one or more words from the
 *	command string.
 *
 * Results:
 *	The return value is a pointer to a dynamically-allocated
 *	string containing the words of command specified by "words".
 *	If the word specifier has improper syntax then an error
 *	message is placed in iPtr->result and NULL is returned.
 *
 * Side effects:
 *	Memory is allocated.  It is the caller's responsibilty to
 *	free the returned string..
 *
 *----------------------------------------------------------------------
 */

static char *
GetWords(iPtr, command, words)
    register Interp *iPtr;	/* Tcl interpreter in which to place
				 * an error message if needed. */
    char *command;		/* Command string. */
    char *words;		/* Description of which words to extract
				 * from the command.  Either num[-num] or
				 * a pattern. */
{
    char *result;
    char *start, *end, *dst;
    register char *next;
    int first;			/* First word desired. -1 means last word
				 * only. */
    int last;			/* Last word desired.  -1 means use everything
				 * up to the end. */
    int index;			/* Index of current word. */
    char *pattern;

    /*
     * Figure out whether we're looking for a numerical range or for
     * a pattern.
     */

    pattern = NULL;
    first = 0;
    last = -1;
    if (*words == '$') {
	if (words[1] != '\0') {
	    goto error;
	}
	first = -1;
    } else if (isdigit(*words)) {
	first = strtoul(words, &start, 0);
	if (*start == 0) {
	    last = first;
	} else if (*start == '-') {
	    start++;
	    if (*start == '$') {
		start++;
	    } else if (isdigit(*start)) {
		last = strtoul(start, &start, 0);
	    } else {
		goto error;
	    }
	    if (*start != 0) {
		goto error;
	    }
	}
	if ((first > last) && (last != -1)) {
	    goto error;
	}
    } else {
	pattern = words;
    }

    /*
     * Scan through the words one at a time, copying those that are
     * relevant into the result string.  Allocate a result area large
     * enough to hold all the words if necessary.
     */

    result = malloc((unsigned) (strlen(command) + 1));
    dst = result;
    for (next = command; isspace(*next); next++) {
	/* Empty loop body:  just find start of first word. */
    }
    for (index = 0; *next != 0; index++) {
	start = next;
	end = TclWordEnd(next, 0);
	for (next = end; isspace(*next); next++) {
	    /* Empty loop body:  just find start of next word. */
	}
	if ((first > index) || ((first == -1) && (*next != 0))) {
	    continue;
	}
	if ((last != -1) && (last < index)) {
	    continue;
	}
	if (pattern != NULL) {
	    int match;
	    char savedChar = *end;

	    *end = 0;
	    match = Tcl_StringMatch(start, pattern);
	    *end = savedChar;
	    if (!match) {
		continue;
	    }
	}
	if (dst != result) {
	    *dst = ' ';
	    dst++;
	}
	strncpy(dst, start, (end-start));
	dst += end-start;
    }
    *dst = 0;

    /*
     * Check for an out-of-range argument index.
     */

    if ((last >= index) || (first >= index)) {
	free(result);
	sprintf(iPtr->result,
		"word selector \"%.50s\" specified non-existent words",
		words);
	return NULL;
    }
    return result;

    error:
    sprintf(iPtr->result,
	    "bad word selector \"%.50s\":  should be num-num or pattern",
	    words);
    return NULL;
}

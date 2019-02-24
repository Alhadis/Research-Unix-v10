/*
 * tclInt.h --
 *
 *	Declarations of things used internally by the Tcl interpreter.
 *
 * Copyright 1987 Regents of the University of California
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies.  The University of California
 * makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 *
 * $Header: /sprite/src/lib/tcl/RCS/tclInt.h,v 1.22 90/03/29 10:55:01 ouster Exp $ SPRITE (Berkeley)
 */

#ifndef _TCLINT
#define _TCLINT

#ifndef _TCL
#include "tcl.h"
#endif

#define	bcopy(src,dest,count)	memcpy(dest,src,count)

/*
 * The structure below defines one Tcl command, by associating a procedure
 * with a textual string.
 */

typedef struct Command {
    int (*proc)();		/* Procedure to process command. */
    ClientData clientData;	/* Arbitrary value to pass to proc. */
    void (*deleteProc)();	/* Procedure to invoke when deleting
				 * command. */
    struct Command *nextPtr;	/* Pointer to next command in list, or NULL
				 * for end of list. */
    char name[4];		/* Name of command.  The actual size of this
				 * portion is as large as is necessary to
				 * hold the characters.  This must be the
				 * last subfield of the record. */
} Command;

#define CMD_SIZE(nameLength) ((unsigned) sizeof(Command) + nameLength - 3)

/*
 * The structure below defines a variable, which associates a string name
 * with a string value.  To cut down on the number of malloc's and free's
 * (particularly for procedure parameters), space for both the variable's
 * name and initial value is allocated at the end of the structure (in
 * "storage").  If the variable's value changes later, a new dynamic
 * string is allocated, if there is insufficient space in the current
 * storage area.
 */

typedef struct Var {
    char *value;		/* Current value of variable (either points
				 * to static space after name, or to dynamic
				 * space if VAR_DYNAMIC is set). */
    int valueLength;		/* Number of bytes of storage at the place
				 * referred to by value, not including space
				 * for NULL terminator. */
    int flags;			/* Miscellaneous flags:  see below. */
    struct Var *globalPtr;	/* If VAR_GLOBAL is set, this points to the
				 * global variable corresponding to name. */
    struct Var *nextPtr;	/* Next variable in list, or NULL for end
				 * of list. */
    char name[4];		/* Storage space for variable's name (and
				 * initial value).  The name is at the
				 * beginning, and is null-terminated.
				 * May contain more than 4 bytes (see
				 * VAR_SIZE macro below). */
} Var;

#define VAR_SIZE(nameLength, valueLength) \
	((unsigned) sizeof(Var) + nameLength + valueLength - 2)

/*
 * Variable flags:
 *
 * VAR_DYNAMIC:		1 means the storage space for the value was
 *			dynamically allocated, and must eventually be
 *			freed.
 * VAR_GLOBAL:		Used only in local variables.  Means that this
 *			is really a global variable.
 * VAR_DOESNT_EXIST:	1 means this variable has not yet been assigned
 *			a value.  Used when a "global" command refers
 *			to a variable that hasn't been set yet.
 */

#define VAR_DYNAMIC		1
#define VAR_GLOBAL		2
#define VAR_DOESNT_EXIST	4

/*
 * The structure below defines a command procedure, which consists of
 * a collection of Tcl commands plus information about arguments and
 * variables.
 */

typedef struct Proc {
    struct Interp *iPtr;	/* Interpreter for which this command
				 * is defined. */
    char *command;		/* Command that constitutes the body of
				 * the procedure (dynamically allocated). */
    Var *argPtr;		/* Pointer to first in list of variables
				 * giving names to the procedure's arguments.
				 * The order of the variables is the same
				 * as the order of the arguments.  The "value"
				 * fields of the variables are the default
				 * values. */
} Proc;

/*
 * The structure below defines a trace.  This is used to allow Tcl
 * clients to find out whenever a command is about to be executed.
 */

typedef struct Trace {
    int level;			/* Only trace commands at nesting level
				 * less than or equal to this. */
    void (*proc)();		/* Procedure to call to trace command. */
    ClientData clientData;	/* Arbitrary value to pass to proc. */
    struct Trace *nextPtr;	/* Next in list of traces for this interp. */
} Trace;

/*
 * The stucture below defines an interpreter callback, which is
 * a procedure to invoke just before an interpreter is deleted.
 */

typedef struct InterpCallback {
    void (*proc)();		/* Procedure to call. */
    ClientData clientData;	/* Value to pass to procedure. */
    struct InterpCallback *nextPtr;
				/* Next in list of callbacks for this
				 * interpreter (or NULL for end of
				 * list). */
} InterpCallback;

/*
 * The structure below defines a frame, which is a procedure invocation.
 * These structures exist only while procedures are being executed, and
 * provide a sort of call stack.
 */

typedef struct CallFrame {
    Var *varPtr;		/* First in list of all local variables
				 * and arguments for this procedure
				 * invocation. */
    int level;			/* Level of this procedure, for "uplevel"
				 * purposes (i.e. corresponds to nesting of
				 * callerVarPtr's, not callerPtr's).  1 means
				 * outer-most procedure, 0 means top-level. */
    int argc;			/* This and argv below describe name and
				 * arguments for this procedure invocation. */
    char **argv;		/* Array of arguments. */
    struct CallFrame *callerPtr;
				/* Frame of procedure that invoked this one
				 * (NULL if level == 1). */
    struct CallFrame *callerVarPtr;
				/* Frame used by caller for accessing local
				 * variables (same as callerPtr unless an
				 * "uplevel" command was active in the
				 * caller).  This field is used in the
				 * implementation of "uplevel". */
} CallFrame;

/*
 * The structure below defines one history event (a previously-executed
 * command that can be re-executed in whole or in part).
 */

typedef struct {
    char *command;		/* String containing previously-executed
				 * command. */
    int bytesAvl;		/* Total # of bytes available at *event (not
				 * all are necessarily in use now). */
} HistoryEvent;

/*
 * The structure below defines a pending revision to the most recent
 * history event.  Changes are linked together into a list and applied
 * during the next call to Tcl_RecordHistory.  See the comments at the
 * beginning of tclHistory.c for information on revisions.
 */

typedef struct HistoryRev {
    int firstIndex;		/* Index of the first byte to replace in
				 * current history event. */
    int lastIndex;		/* Index of last byte to replace in
				 * current history event. */
    int newSize;		/* Number of bytes in newBytes. */
    char *newBytes;		/* Replacement for the range given by
				 * firstIndex and lastIndex. */
    struct HistoryRev *nextPtr;	/* Next in chain of revisions to apply, or
				 * NULL for end of list. */
} HistoryRev;

/*
 * This structure defines an interpreter, which is a collection of commands
 * plus other state information related to interpreting commands, such as
 * variable storage.  The lists of commands and variables are sorted by usage:
 * each time a command or variable is used it is pulled to the front of its
 * list.
 */

typedef struct Interp {

    /*
     * Note:  the first three fields must match exactly the fields in
     * a Tcl_Interp struct (see tcl.h).  If you change one, be sure to
     * change the other.
     */

    char *result;		/* Points to result returned by last
				 * command. */
    int dynamic;		/* Non-zero means result is dynamically-
				 * allocated and must be freed by Tcl_Eval
				 * before executing the next command. */
    int errorLine;		/* When TCL_ERROR is returned, this gives
				 * the line number within the command where
				 * the error occurred (1 means first line). */
    Command *commandPtr;	/* First command in list containing all
				 * commands defined for this table. */

    /*
     * Information related to procedures and variables.  See tclProc.c
     * for usage.
     */

    Var *globalPtr;		/* First in list of all global variables for
				 * this command table. */
    Var *localPtr;		/* First in list of all local variables and
				 * arguments for the Tcl procedure that is
				 * currently being executed.  If no procedure
				 * is being executed, or if it has no vars or
				 * args, this will be NULL. */
    int numLevels;		/* Keeps track of how many nested calls to
				 * Tcl_Eval are in progress for this
				 * interpreter.  It's used to delay deletion
				 * of the table until all Tcl_Eval invocations
				 * are completed. */
    CallFrame *framePtr;	/* If a procedure is being executed, this
				 * points to the call frame for the current
				 * procedure (most recently-called).  NULL
				 * means no procedure is active. */
    CallFrame *varFramePtr;	/* Points to the call frame whose variables
				 * are currently in use (same as framePtr
				 * unless an "uplevel" command is being
				 * executed).  NULL means no procedure is
				 * active or "uplevel 0" is being exec'ed. */

    /*
     * Information related to history:
     */

    int numEvents;		/* Number of previously-executed commands
				 * to retain. */
    HistoryEvent *events;	/* Array containing numEvents entries
				 * (dynamically allocated). */
    int curEvent;		/* Index into events of place where current
				 * (or most recent) command is recorded. */
    int curEventNum;		/* Event number associated with the slot
				 * given by curEvent. */
    HistoryRev *revPtr;		/* First in list of pending revisions. */
    char *historyFirst;		/* First char. of current command executed
				 * from history module.  NULL means don't
				 * do history revision (see tclHistory.c
				 * for details on revision). */
    char *evalFirst;		/* If TCL_RECORD_BOUNDS flag set, Tcl_Eval
				 * sets this field to point to the first
				 * char. of text from which the current
				 * command came.  Otherwise Tcl_Eval sets
				 * this to NULL. */
    char *evalLast;		/* Similar to evalFirst, except points to
				 * last character of current command. */

    /*
     * Miscellaneous information:
     */

    int cmdCount;		/* Total number of times a command procedure
				 * has been called for this interpreter. */
    int noEval;			/* Non-zero means no commands should actually
				 * be executed:  just parse only.  Used in
				 * expressions when the result is already
				 * determined. */
    int flags;			/* Various flag bits.  See below. */
    Trace *tracePtr;		/* List of traces for this interpreter. */
    InterpCallback *callbackPtr;/* List of callbacks to invoke when
				 * interpreter is deleted. */
    char resultSpace[TCL_RESULT_SIZE];
				/* Static space for storing small results. */
} Interp;

/*
 * Flag bits for Interp structures:
 *
 * DELETED:		Non-zero means the interpreter has been deleted:
 *			don't process any more commands for it, and destroy
 *			the structure as soon as all nested invocations of
 *			Tcl_Eval are done.
 * ERR_IN_PROGRESS:	Non-zero means an error unwind is already in progress.
 *			Zero means a command proc has been invoked since last
 *			error occured.
 * ERR_ALREADY_LOGGED:	Non-zero means information has already been logged
 *			in $errorInfo for the current Tcl_Eval instance,
 *			so Tcl_Eval needn't log it (used to implement the
 *			"error message log" command).
 */

#define DELETED			1
#define ERR_IN_PROGRESS		2
#define ERR_ALREADY_LOGGED	4

/*
 * Additional flags passed to Tcl_Eval.  See tcl.h for other flags to
 * Tcl_Eval;  these ones are only used internally by Tcl.
 *
 * TCL_RECORD_BOUNDS	Tells Tcl_Eval to record information in the
 *			evalFirst and evalLast fields for each command
 *			executed directly from the string (top-level
 *			commands and those from command substitution).
 */

#define TCL_RECORD_BOUNDS	0x100

/*
 * Maximum number of levels of nesting permitted in Tcl commands.
 */

#define MAX_NESTING_DEPTH	100

/*
 * Procedures shared among Tcl modules but not used by the outside
 * world:
 */

extern void		TclCopyAndCollapse();
extern void		TclDeleteVars();
extern Command *	TclFindCmd();
extern int		TclFindElement();
extern Proc *		TclFindProc();
extern Proc *		TclIsProc();
extern char *		TclWordEnd();

#endif _TCLINT

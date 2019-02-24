/*
 * tcl.h --
 *
 *	This header file describes the externally-visible facilities
 *	of the Tcl interpreter.
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
 * $Header: /sprite/src/lib/tcl/RCS/tcl.h,v 1.35 90/04/18 16:05:56 ouster Exp $ SPRITE (Berkeley)
 */

#ifndef _TCL
#define _TCL

/*
 * Data structures defined opaquely in this module.  The definitions
 * below just provide dummy types.  A few fields are made visible in
 * Tcl_Interp structures, namely those for returning string values.
 * Note:  any change to the Tcl_Interp definition below must be mirrored
 * in the "real" definition in tclInt.h.
 */

typedef struct Tcl_Interp{
    char *result;		/* Points to result string returned by last
				 * command. */
    int dynamic;		/* Non-zero means result is dynamically-
				 * allocated and must be freed by Tcl_Eval
				 * before executing the next command. */
    int errorLine;		/* When TCL_ERROR is returned, this gives
				 * the line number within the command where
				 * the error occurred (1 means first line). */
} Tcl_Interp;

typedef int *Tcl_Trace;
typedef int *Tcl_CmdBuf;

/*
 * When a TCL command returns, the string pointer interp->result points to
 * a string containing return information from the command.  In addition,
 * the command procedure returns an integer value, which is one of the
 * following:
 *
 * TCL_OK		Command completed normally;  interp->result contains
 *			the command's result.
 * TCL_ERROR		The command couldn't be completed successfully;
 *			interp->result describes what went wrong.
 * TCL_RETURN		The command requests that the current procedure
 *			return;  interp->result contains the procedure's
 *			return value.
 * TCL_BREAK		The command requests that the innermost loop
 *			be exited;  interp->result is meaningless.
 * TCL_CONTINUE		Go on to the next iteration of the current loop;
 *			interp->result is meaninless.
 */

#define TCL_OK		0
#define TCL_ERROR	1
#define TCL_RETURN	2
#define TCL_BREAK	3
#define TCL_CONTINUE	4

#define TCL_RESULT_SIZE 199

/*
 * Flag values passed to Tcl_Eval (see the man page for details;  also
 * see tclInt.h for additional flags that are only used internally by
 * Tcl):
 */

#define TCL_BRACKET_TERM	1

/*
 * Flag value passed to Tcl_RecordAndEval to request no evaluation
 * (record only).
 */

#define TCL_NO_EVAL		-1

/*
 * Flag values passed to Tcl_Return (see the man page for details):
 */

#define TCL_STATIC	0
#define TCL_DYNAMIC	1
#define TCL_VOLATILE	2

#ifndef _CLIENTDATA
typedef int *ClientData;
#define _CLIENTDATA
#endif

/*
 * Exported Tcl procedures:
 */

typedef int (*Tcl_Cmdfn)(ClientData, Tcl_Interp *, int, char **);
typedef void (*Tcl_Tracefn)(ClientData, Tcl_Interp *, int, char *, Tcl_Cmdfn, ClientData, int, char **);
extern char *		Tcl_AssembleCmd(Tcl_CmdBuf, char *);
extern void		Tcl_AddErrorInfo(Tcl_Interp *, char *);
extern char		Tcl_Backslash(char *, int *);
extern char *		Tcl_Concat(int, char **);
extern Tcl_CmdBuf	Tcl_CreateCmdBuf(void);
extern void		Tcl_CreateCommand(Tcl_Interp *, char *, Tcl_Cmdfn, ClientData, void (*)(ClientData));
extern Tcl_Interp *	Tcl_CreateInterp(void);
extern Tcl_Trace	Tcl_CreateTrace(Tcl_Interp *, int, Tcl_Tracefn, ClientData);
extern void		Tcl_DeleteCmdBuf(Tcl_CmdBuf);
extern void		Tcl_DeleteCommand(Tcl_Interp *, char *);
extern void		Tcl_DeleteInterp(Tcl_Interp *);
extern void		Tcl_DeleteTrace(Tcl_Interp *, Tcl_Trace);
extern int		Tcl_Eval(Tcl_Interp *, char *, int, char **);
extern int		Tcl_Expr(Tcl_Interp *, char *, int *);
extern char *		Tcl_GetVar(Tcl_Interp *, char *, int);
extern char *		Tcl_Merge(int, char **);
extern char *		Tcl_ParseVar(Tcl_Interp *, char *, char **);
extern int		Tcl_RecordAndEval(Tcl_Interp *, char *, int);
extern void		Tcl_Return(Tcl_Interp *, char *, int);
extern void		Tcl_SetVar(Tcl_Interp *, char *, char *, int);
extern int		Tcl_SplitList(Tcl_Interp *, char *, int *, char ***);
extern int		Tcl_StringMatch(char *, char *);
extern char *		Tcl_TildeSubst(Tcl_Interp *, char *);
extern void		Tcl_WatchInterp(Tcl_Interp *, void (*)(void), ClientData);

/*
 * Built-in Tcl command procedures:
 */

extern int		Tcl_BreakCmd(ClientData , Tcl_Interp *, int , char **);
extern int		Tcl_CaseCmd(ClientData , Tcl_Interp *, int , char **);
extern int		Tcl_CatchCmd(ClientData , Tcl_Interp *, int , char **);
extern int		Tcl_ConcatCmd(ClientData , Tcl_Interp *, int , char **);
extern int		Tcl_ContinueCmd(ClientData , Tcl_Interp *, int , char **);
extern int		Tcl_ErrorCmd(ClientData , Tcl_Interp *, int , char **);
extern int		Tcl_EvalCmd(ClientData , Tcl_Interp *, int , char **);
extern int		Tcl_ExecCmd(ClientData , Tcl_Interp *, int , char **);
extern int		Tcl_ExprCmd(ClientData , Tcl_Interp *, int , char **);
extern int		Tcl_FileCmd(ClientData , Tcl_Interp *, int , char **);
extern int		Tcl_ForCmd(ClientData , Tcl_Interp *, int , char **);
extern int		Tcl_ForeachCmd(ClientData , Tcl_Interp *, int , char **);
extern int		Tcl_FormatCmd(ClientData , Tcl_Interp *, int , char **);
extern int		Tcl_GlobCmd(ClientData , Tcl_Interp *, int , char **);
extern int		Tcl_GlobalCmd(ClientData , Tcl_Interp *, int , char **);
extern int		Tcl_HistoryCmd(ClientData , Tcl_Interp *, int , char **);
extern int		Tcl_IfCmd(ClientData , Tcl_Interp *, int , char **);
extern int		Tcl_InfoCmd(ClientData , Tcl_Interp *, int , char **);
extern int		Tcl_IndexCmd(ClientData , Tcl_Interp *, int , char **);
extern int		Tcl_LengthCmd(ClientData , Tcl_Interp *, int , char **);
extern int		Tcl_ListCmd(ClientData , Tcl_Interp *, int , char **);
extern int		Tcl_PrintCmd(ClientData , Tcl_Interp *, int , char **);
extern int		Tcl_ProcCmd(ClientData , Tcl_Interp *, int , char **);
extern int		Tcl_RangeCmd(ClientData , Tcl_Interp *, int , char **);
extern int		Tcl_RenameCmd(ClientData , Tcl_Interp *, int , char **);
extern int		Tcl_ReturnCmd(ClientData , Tcl_Interp *, int , char **);
extern int		Tcl_ScanCmd(ClientData , Tcl_Interp *, int , char **);
extern int		Tcl_SetCmd(ClientData , Tcl_Interp *, int , char **);
extern int		Tcl_SourceCmd(ClientData , Tcl_Interp *, int , char **);
extern int		Tcl_StringCmd(ClientData , Tcl_Interp *, int , char **);
extern int		Tcl_TimeCmd(ClientData , Tcl_Interp *, int , char **);
extern int		Tcl_UplevelCmd(ClientData , Tcl_Interp *, int , char **);

/*
 * Miscellaneous declarations (to allow Tcl to be used stand-alone,
 * without the rest of Sprite).
 */

#ifndef NULL
#define NULL 0
#endif

#endif _TCL

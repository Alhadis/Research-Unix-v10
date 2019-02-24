/*	@(#)msg.c	1.6	*/
/*
 *	UNIX shell
 *
 *	Bell Telephone Laboratories
 *
 */


#include	"defs.h"
#include	"sym.h"

/*
 * error messages
 */
char	badopt[]	= "bad option(s)";
char	mailmsg[]	= "you have mail\n";
char	nospace[]	= "no space";
char	nostack[]	= "no stack space";
char	synmsg[]	= "syntax error";

char	badnum[]	= "bad number";
char	badparam[]	= "parameter null or not set";
char	unset[]		= "parameter not set";
char	badsub[]	= "bad substitution";
char	badcreate[]	= "cannot create";
char	nofork[]	= "fork failed - too many processes";
char	noswap[]	= "cannot fork: no swap space";
char	piperr[]	= "cannot make pipe";
char	badopen[]	= "cannot open";
char	coredump[]	= " - core dumped";
char	arglist[]	= "arg list too long";
char	txtbsy[]	= "text busy";
char	toobig[]	= "too big";
char	badexec[]	= "cannot execute";
char	notfound[]	= "not found";
char	notbltin[]	= "not built in";
char	badfile[]	= "bad file number";
char	badshift[]	= "cannot shift";
char	baddir[]	= "bad directory";
char	badtrap[]	= "bad trap";
char	notid[]		= "is not an identifier";
char	badreturn[]	= "cannot return when not in function";
char	badexport[]	= "cannot export functions";
char	badunset[] 	= "cannot unset";
char	nohome[]	= "no home directory";
char 	badperm[]	= "execute permission denied";
char	badfname[]	= "illegal function name";
/*
 * built in names
 */
char	pathname[]	= "PATH";
char	cdpname[]	= "CDPATH";
char	homename[]	= "HOME";
char	mailname[]	= "MAIL";
char	ifsname[]	= "IFS";
char	ps1name[]	= "PS1";
char	ps2name[]	= "PS2";
char	acctname[]	= "SHACCT";
char	histname[]	= "HISTORY";

/*
 * string constants
 */
char	nullstr[]	= "";
char	sptbnl[]	= " \t\n";
char	defpath[]	= ":/bin:/usr/bin";
char	colon[]		= ": ";
char	minus[]		= "-";
char	endoffile[]	= "end of file";
char	unexpected[] 	= " unexpected";
char	atline[]	= " at line ";
char	devnull[]	= "/dev/null";
char	execpmsg[]	= "+ ";
char	readmsg[]	= "> ";
char	stdprompt[]	= "$ ";
char	supprompt[]	= "# ";
char	profile[]	= ".profile";

/*
 * tables
 */

struct sysnod reserved[] =
{
	{ "case",	CASYM	},
	{ "do",		DOSYM	},
	{ "done",	ODSYM	},
	{ "elif",	EFSYM	},
	{ "else",	ELSYM	},
	{ "esac",	ESSYM	},
	{ "fi",		FISYM	},
	{ "for",	FORSYM	},
	{ "if",		IFSYM	},
	{ "in",		INSYM	},
	{ "then",	THSYM	},
	{ "until",	UNSYM	},
	{ "while",	WHSYM	},
};

int no_reserved = 13;

char	*sysmsg[] =
{
	0,
	"Hangup",
	0,	/* Interrupt */
	"Quit",
	"Illegal instruction",
	"Trace/BPT trap",
#ifdef	CRAY
#ifdef	CRAY2
	"Hardware error",
#else
	"Signal 6",
#endif
	"Error exit",
#else
	"abort",
	"EMT trap",
#endif
	"Floating exception",
	"Killed",
#ifdef	CRAY
	"Program range error",
	"Operand range error",
#else
	"Bus error",
	"Memory fault",
#endif
	"Bad system call",
	0,	/* Broken pipe */
	"Alarm call",
	"Terminated",
	"Signal 16",
	"Signal 17",
	"Child death",
#ifdef	CRAY
	"Signal 19",
	"Signal 20",
	"Signal 21",
	"Signal 22",
	"Signal 23",
	"Signal 24",
	"Signal 25",
	"Signal 26",
	"Signal 27",
	"Signal 28",
	"Signal 29",
	"Signal 30",
	"Signal 31",
	"Signal 32",
	"Signal 33",
	"Signal 34",
	"Signal 35",
	"Signal 36",
	"Signal 37",
	"Signal 38",
	"Signal 39",
	"Signal 40",
	"Signal 41",
	"Signal 42",
	"Signal 43",
	"Signal 44",
	"Signal 45",
	"Signal 46",
	"Signal 47",
	"Signal 48",
	"Signal 49",
	"Signal 50",
	"Signal 51",
	"Signal 52",
	"Signal 53",
	"Signal 54",
	"Signal 55",
	"Signal 56",
	"Signal 57",
	"Signal 58",
	"Signal 59",
	"Signal 60",
	"Signal 61",
	"Signal 62",
	"Signal 63"
#else
	"Power Fail"
#endif
};
int	maxsysmsg = sizeof(sysmsg) / sizeof(char *);

char	export[] = "export";
char	duperr[] = "cannot dup";

struct sysnod commands[] =
{
	{ ".",		SYSDOT	},
	{ ":",		SYSNULL	},
	{ "break",	SYSBREAK },
	{ "builtin",	SYSBLTIN },
	{ "cd",		SYSCD	},
	{ "continue",	SYSCONT	},
	{ "eval",	SYSEVAL	},
	{ "exec",	SYSEXEC	},
	{ "exit",	SYSEXIT	},
	{ "export",	SYSXPORT },
/*	{ "newgrp",	SYSNEWGRP },	*/
	{ "read",	SYSREAD	},
	{ "return",	SYSRETURN },
	{ "set",	SYSSET	},
	{ "shift",	SYSSHFT	},
	{ "times",	SYSTIMES },
	{ "trap",	SYSTRAP	},
	{ "umask",	SYSUMASK },
	{ "unset", 	SYSUNS },
	{ "wait",	SYSWAIT	},
	{ "whatis",	SYSWHATIS },
	{ "xxmem",	SYSMEM }
};

int no_commands = sizeof(commands)/sizeof(commands[0]);

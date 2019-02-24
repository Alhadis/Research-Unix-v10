/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * error and message formatter
 *
 *	level is the error level
 *	level >= ERROR_FATAL calls err_exit
 *	level < 0 is for debug tracing
 *	err_write must accept write(2) args
 */

#include <ctype.h>
#include <error.h>
#include <namval.h>
#include <sig.h>
#include <sfio.h>

#if !__STDC__

#include <varargs.h>

#endif

typedef int (*PFI)();

extern int	exit();
extern int	write();
extern char*	strerror();

static PFI	err_aux;		/* call just before return	*/
static PFI	err_exit = exit;	/* error exit routine		*/
static PFI	err_write = write;	/* error output routine		*/

static int	err_clear;		/* clear error() flags		*/
static int	err_core;		/* level>=err_core -> core dump	*/
static int	err_fd = 2;		/* write(2) fd			*/
static int	err_errno;		/* last reported errno		*/
static int	err_init;		/* initialized			*/
static int	err_set;		/* set error() flags		*/

static char*	err_id;			/* command id			*/
static char*	err_version;		/* ERROR_SOURCE command version	*/

static int*	ptr_errors;		/* >= ERROR_ERROR count		*/
static int*	ptr_indent;		/* debug trace indent level	*/
static int*	ptr_line;		/* input|output line number	*/
static int*	ptr_mask;		/* multi level debug trace mask	*/
static int*	ptr_trace;		/* debug trace level		*/
static int*	ptr_warnings;		/* ERROR_WARNING count		*/

static char**	ptr_id;			/* command id			*/
static char**	ptr_file;		/* input|output file name	*/

#define OPT_CORE	1
#define OPT_FD		2
#define OPT_LIBRARY	3
#define OPT_MASK	4
#define OPT_SYSTEM	5
#define OPT_TRACE	6

static struct namval	options[] =	/* environment option names	*/
{
	"core",		OPT_CORE,
	"fd",		OPT_FD,
	"library",	OPT_LIBRARY,
	"mask",		OPT_MASK,
	"system",	OPT_SYSTEM,
	"trace",	OPT_TRACE,
	0,		0
};

/*
 * called by stropt() to set options
 */

/*ARGSUSED*/
static int
setopt(a, p, n, v)
char*			a;
register struct namval*	p;
register int		n;
register char*		v;
{
	static int	mask;
	static int	trace;

	extern long	strtol();

	switch (p->value)
	{
	case OPT_CORE:
		if (n) switch (*v)
		{
		case 'e':
		case 'E':
			err_core = ERROR_ERROR;
			break;
		case 'f':
		case 'F':
			err_core = ERROR_FATAL;
			break;
		case 'p':
		case 'P':
			err_core = ERROR_PANIC;
			break;
		default:
			err_core = strtol(v, (char*)0, 0);
			break;
		}
		else err_core = 0;
		break;
	case OPT_FD:
		err_fd = n ? strtol(v, (char*)0, 0) : -1;
		break;
	case OPT_LIBRARY:
		if (n) err_set |= ERROR_LIBRARY;
		else err_clear |= ERROR_LIBRARY;
		break;
	case OPT_MASK:
		if (n) mask = strtol(v, (char*)0, 0);
		else mask = 0;
		ptr_mask = &mask;
		break;
	case OPT_SYSTEM:
		if (n) err_set |= ERROR_SYSTEM;
		else err_clear |= ERROR_SYSTEM;
		break;
	case OPT_TRACE:
		if (n) trace = -strtol(v, (char*)0, 0);
		else trace = 0;
		ptr_trace = &trace;
		break;
	}
	return(0);
}

/*
 * initialize default environment
 */

static void
init()
{
	extern char*	getenv();

	err_init = 1;
	(void)stropt(getenv("ERROR_OPTIONS"), options, setopt, (char*)0);
}

/*
 * print a name, converting unprintable chars
 */

static void
print(sp, name, delim)
register SFIO*	sp;
register char*	name;
char*		delim;
{
	register int	c;

	while (c = *name++)
	{
		if (c & 0200)
		{
			c &= 0177;
			sfputc(sp, '?');
		}
		if (c < ' ')
		{
			c += 'A' - 1;
			sfputc(sp, '^');
		}
		sfputc(sp, c);
	}
	sfputs(sp, delim, 0);
}

/*
 * functional interface to the error message state
 *
 * upper case options are for static values
 * other options are for pointers to dynamic values
 * previous value for rightmost option is returned
 * 0 returned on first unknown option
 */

#if __STDC__

char*
seterror(int op, ...)

#else

char*
seterror(va_alist)
va_dcl

#endif

{
	va_list		ap;
	char*		prev;
#if !__STDC__
	int		op;
#endif

	prev = 0;
#if __STDC__
	va_start(ap, op);
#else
	va_start(ap);
	op = va_arg(ap, int);
#endif
	if (!op || !err_init) init();
	for (;;)
	{
		switch (op)
		{
		case 'A':
			prev = (char*)err_aux;
			err_aux = va_arg(ap, PFI);
			break;
		case 'C':
		case 'I':
			prev = (char*)err_id;
			err_id = va_arg(ap, char*);
			break;
		case 'E':
			prev = (char*)err_exit;
			err_exit = va_arg(ap, PFI);
			break;
		case 'V':
			prev = (char*)err_version;
			err_version = va_arg(ap, char*);
			break;
		case 'W':
			prev = (char*)err_write;
			err_write = va_arg(ap, PFI);
			break;
		case 'c':
			prev = (char*)ptr_id;
			ptr_id = va_arg(ap, char**);
			break;
		case 'f':
			prev = (char*)ptr_file;
			ptr_file = va_arg(ap, char**);
			break;
		case 'i':
			prev = (char*)ptr_indent;
			ptr_indent = va_arg(ap, int*);
			break;
		case 'l':
			prev = (char*)ptr_line;
			ptr_line = va_arg(ap, int*);
			break;
		case 'm':
			prev = (char*)ptr_mask;
			ptr_mask = va_arg(ap, int*);
			if (prev && ptr_mask) *ptr_mask = *((int*)prev);
			break;
		case 't':
			prev = (char*)ptr_trace;
			ptr_trace = va_arg(ap, int*);
			if (prev && ptr_trace) *ptr_trace = *((int*)prev);
			break;
		case '1':
			prev = (char*)ptr_warnings;
			ptr_warnings = va_arg(ap, int*);
			if (prev && ptr_warnings) *ptr_warnings += *((int*)prev);
			break;
		case '2':
			prev = (char*)ptr_errors;
			ptr_errors = va_arg(ap, int*);
			if (prev && ptr_errors) *ptr_errors += *((int*)prev);
			break;
		case '+':
			prev = (char*)err_set;
			err_set = va_arg(ap, int);
			break;
		case '-':
			prev = (char*)err_clear;
			err_clear = va_arg(ap, int);
			break;
		default:
			prev = 0;
			/*FALLTHROUGH*/
		case 0:
			va_end(ap);
			return(prev);
		}
		op = va_arg(ap, int);
	}
}

#if __STDC__

void
error(int level, ...)

#else

void
error(va_alist)
va_dcl

#endif

{
	va_list	ap;
#if !__STDC__
	int	level;
#endif

#if __STDC__
	va_start(ap, level);
#else
	va_start(ap);
	level = va_arg(ap, int);
#endif
	verror((char*)0, level, ap);
	va_end(ap);
}

#if __STDC__ || __cplusplus
void
verror(const char* lib, int level, va_list ap)
#else
void
verror(lib, level, ap)
char*	lib;
int	level;
va_list	ap;
#endif
{
	register SFIO*	sp;
	register int	n;
	int		fd;
	int		flags;
	char*		s;
	char*		format;

	int		indent;
	int		line;
	int		trace;
	char*		file;

	static int	state;

	if (!err_init) init();
	if (level > 0)
	{
		flags = level & ~ERROR_LEVEL;
		level &= ERROR_LEVEL;
	}
	else flags = 0;
	trace = ptr_trace ? *ptr_trace : 0;
	if (level < trace || lib && (err_clear & ERROR_LIBRARY) || level < 0 && ptr_mask && !((*ptr_mask) & (1<<(-level - 1))))
	{
		if (level >= ERROR_FATAL) (*err_exit)(level - 1);
		return;
	}
	if (trace < 0) flags |= ERROR_LIBRARY|ERROR_SYSTEM;
	flags |= err_set;
	flags &= ~err_clear;
	if (!lib) flags &= ~ERROR_LIBRARY;
	if (sp = sftmp(SF_UNBOUND))
	{
		file = ptr_id ? *ptr_id : err_id;
		if (flags & ERROR_USAGE)
		{
			if (state & ERROR_USAGE) (void)sfprintf(sp, "       ");
			else
			{
				state |= ERROR_USAGE;
				(void)sfprintf(sp, "Usage: ");
			}
			if (file || opt_argv && (file = opt_argv[0])) print(sp, file, " ");
		}
		else
		{
			state &= ~ERROR_USAGE;
			if (level && !(flags & ERROR_NOID))
			{
				if (file) print(sp, file, (flags & ERROR_LIBRARY) ? " " : ": ");
				if (flags & ERROR_LIBRARY) (void)sfprintf(sp, "[%s library]: ", lib);
			}
		}
		line = ptr_line ? *ptr_line : 0;
		file = ptr_file ? *ptr_file : 0;
		if (level > 0 && line > 0)
		{
			if (file && *file) (void)sfprintf(sp, "\"%s\", ", file);
			(void)sfprintf(sp, "line %d: ", line);
		}
		switch (level)
		{
		case 0:
			break;
		case ERROR_WARNING:
			if (ptr_warnings) (*ptr_warnings)++;
			(void)sfprintf(sp, "warning: ");
			break;
		case ERROR_PANIC:
			if (ptr_errors) (*ptr_errors)++;
			(void)sfprintf(sp, "panic: ");
			break;
		default:
			if (level < 0)
			{
				if (trace < -1) (void)sfprintf(sp, "debug%d:%s", level, level > -10 ? " " : "");
				else (void)sfprintf(sp, "debug: ");
				indent = ptr_indent ? *ptr_indent : 0;
				for (n = 0; n < indent; n++)
				{
					sfputc(sp, ' ');
					sfputc(sp, ' ');
				}
			}
			else if (ptr_errors) (*ptr_errors)++;
			break;
		}
		fd = (flags & ERROR_OUTPUT) ? va_arg(ap, int) : err_fd;
		if (flags & ERROR_SOURCE)
		{
			/*
			 * source ([version], file, line) message
			 */

			file = va_arg(ap, char*);
			line = va_arg(ap, int);
			if (err_version) (void)sfprintf(sp, "(%s: %s, line %d) ", err_version, file, line);
			else (void)sfprintf(sp, "(%s, line %d) ", file, line);
		}
		format = va_arg(ap, char*);
		(void)sfvprintf(sp, format, ap);
		if (!(flags & ERROR_PROMPT))
		{
			if ((flags & ERROR_SYSTEM) && errno && errno != err_errno)
			{
				(void)sfprintf(sp, " [%s]", strerror(errno));
				if (err_set & ERROR_SYSTEM) errno = 0;
				err_errno = (level >= 0) ? 0 : errno;
			}
			sfputc(sp, '\n');
		}
		(void)sfseek(sp, 0L, 0);
		n = sfpeek(sp, (unsigned char**)&s);
		if (err_write) (void)(*err_write)(fd, s, n);
		if (err_aux) (void)(*err_aux)(lib, flags, level, s, n);
		(void)sfclose(sp);
	}
	if (level >= err_core && err_core)
	{
		(void)signal(SIGQUIT, SIG_DFL);
		(void)kill(getpid(), SIGQUIT);
		(void)pause();
	}
	if (level >= ERROR_FATAL) (void)(*err_exit)(level - ERROR_FATAL + 1);
}

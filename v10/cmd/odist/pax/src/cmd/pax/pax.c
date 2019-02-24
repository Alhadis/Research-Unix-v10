/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * pax -- portable archive interchange
 *
 * current tests:
 *
 *	010	list bget() buffer alignment
 *	020	ignore delta/compress header info
 *	040	pretend output device is char special
 */

#if !lint
static char id[] = "\n@(#)pax (AT&T Bell Laboratories) 01/01/91\0\n";
#endif

#include "pax.h"

char*			definput = "standard input";
char*			defoutput = "standard output";
char*			eomprompt = "Change to part %d and hit RETURN: ";
char			alar_header[ALAR_LABEL];
struct binary_header	binary_header;
union tar_header_block	tar_header_block;
struct portar_header	portar_header;
struct fileinfo		file;
struct stateinfo	state;

struct formatinfo	format[] =
{
ALAR_NAME,	ALAR_REGULAR,	ALAR_SPECIAL,	ALAR_ALIGN,	ALAR_FLAGS,
BINARY_NAME,	BINARY_REGULAR,	BINARY_SPECIAL,	BINARY_ALIGN,	BINARY_FLAGS,
CPIO_NAME,	CPIO_REGULAR,	CPIO_SPECIAL,	CPIO_ALIGN,	CPIO_FLAGS,
IBMAR_NAME,	IBMAR_REGULAR,	IBMAR_SPECIAL,	IBMAR_ALIGN,	IBMAR_FLAGS,
TAR_NAME,	TAR_REGULAR,	TAR_SPECIAL,	TAR_ALIGN,	TAR_FLAGS,
USTAR_NAME,	USTAR_REGULAR,	USTAR_SPECIAL,	USTAR_ALIGN,	USTAR_FLAGS,
ASC_NAME,	ASC_REGULAR,	ASC_SPECIAL,	ASC_ALIGN,	ASC_FLAGS,
ASCHK_NAME,	ASCHK_REGULAR,	ASCHK_SPECIAL,	ASCHK_ALIGN,	ASCHK_FLAGS,
SAVESET_NAME,	SAVESET_REGULAR,SAVESET_SPECIAL,SAVESET_ALIGN,	SAVESET_FLAGS,
PAX_NAME,	PAX_REGULAR,	PAX_SPECIAL,	PAX_ALIGN,	PAX_FLAGS,
PORTAR_NAME,	PORTAR_REGULAR,	PORTAR_SPECIAL,	PORTAR_ALIGN,	PORTAR_FLAGS,
RANDAR_NAME,	RANDAR_REGULAR,	RANDAR_SPECIAL,	RANDAR_ALIGN,	RANDAR_FLAGS,
0,		0,		0,		0
};

static int		signals[] =	/* signals caught by interrupt() */
{
	SIGHUP,
	SIGINT,
#if !DEBUG
	SIGQUIT,
#endif
	SIGALRM,
	SIGTERM,
};

static void	interrupt();

/*ARGSUSED*/
main(argc, argv)
int	argc;
char**	argv;
{
	register int	i;
	register char*	s;
	char*		idname;
	char*		p;
	char*		t;
	int		n;
	struct maplist*	mp;
	struct maplist*	lastmap;

	seterror('I', "pax", '2', &state.errors, 't', &state.trace, 0);
	state.gid = getegid();
	state.uid = geteuid();
	(void)umask(state.modemask = umask(0));
	(void)time(&state.present);
	state.formatout = -1;
#if physical_is_the_default
	state.ftwflags = FTW_DOT|FTW_PHYSICAL;
#else
	state.ftwflags = FTW_DOT;
#endif
	state.intermediate = 1;
	state.matchsense = 1;
	state.modtime = 1;
	state.delta.offset = 1;
	state.delta.sum = -1;
	state.part = 1;
	state.record.charset = 1;
	state.record.line = 1;
	state.summary = 1;
	initfile(&file, (char*)0, 0);
	state.tmp.file = pathtemp((char*)0, (char*)0, "pax");
	idname = 0;
	lastmap = 0;
	while (i = optget(argv, "b:[blocking]f:[archive]kmors:[/old/new/[pg]]vwx:[format]z:[base]LP?R:[record-control]e:[filter]hnD#[debug]T#[test]cdilpat:[tape]uyB:[vol-max]CM:[vol-message]SU:[user]VX [<file-list | <archive] [file ... [directory]] | [pattern ...]")) switch (i)
	{

	/* basic options */

	case 'b':
		state.blocksize = strton(opt_arg, &p, BLOCKSIZE);
		if (*p) error(3, "%s: invalid block size", opt_arg);
		if (state.blocksize < MINBLOCK) error(3, "block size must be at least %d", MINBLOCK);
		if (state.blocksize & (BLOCKSIZE - 1)) error(1, "block size should probably be a multiple of %d", BLOCKSIZE);
		break;
	case 'f':
		if (state.file) error(1, "%s overrides %s", opt_arg, state.file);
		state.file = opt_arg;
		break;
	case 'm':
		state.modtime = 0;
		break;
	case 'o':
		state.owner = 1;
		break;
	case 'r':
		state.operation |= IN;
		break;
	case 's':
		s = opt_arg;
		if (!(n = *s++)) error(3, "empty substitution");
		while (*s && *s != n)
			if (*s++ == '\\' && !*s++)
				error(3, "`\\' cannot terminate lhs of substitution");
		if (*s != n) error(3, "missing `%c' delimiter for lhs of substitution", n);
		*s++ = 0;
		if (!(mp = allocate(struct maplist))) error(3, "not enough space for substitution");
		mp->re = recomp(opt_arg + 1, RE_EDSTYLE|RE_MATCH);
		mp->into = s;
		while (*s && *s != n)
			if (*s++ == '\\' && !*s++)
				error(3, "`\\' cannot terminate rhs of substitution");
		if (*s != n) error(3, "missing `%c' delimiter for rhs of substitution", n);
		*s++ = 0;
		while (*s) switch (*s++)
		{
		case 'g':
			mp->flags |= RE_ALL;
			break;
		case 'l':
			mp->flags |= RE_LOWER;
			break;
		case 'p':
			mp->flags |= RE_VERBOSE;
			break;
		case 'u':
			mp->flags |= RE_UPPER;
			break;
		default:
			error(3, "extra character%s `%s' after substitution", *s ? "s" : "", s - 1);
			break;
		}
		if (lastmap) lastmap = lastmap->next = mp;
		else state.maps = lastmap = mp;
		break;
	case 'v':
		state.verbose = 1;
		break;
	case 'w':
		state.operation |= OUT;
		break;
	case 'x':
		if ((state.formatout = getformat(opt_arg)) < 0) error(3, "%s: unknown archive format", opt_arg);
		break;
	case 'R':
		s = opt_arg;
		do
		{
			if (t = strchr(s, ',')) *t++ = ',';
			switch (*s++)
			{
			case 'c':
				state.record.charset = 0;
				break;
			case 'f':
				state.record.format = *s;
				break;
			case 'l':
				state.record.line = 0;
				break;
			case 'm':
				state.record.pattern = s;
				break;
			case 'p':
				state.record.pad = 1;
				break;
			case 's':
				state.record.size = strton(s, &p, 0);
				if (*p) error(3, "%s: invalid record size", s);
				break;
			case 'v':
				(void)strncpy(state.id.volume, s, sizeof(state.id.volume) - 1);
				state.id.volume[sizeof(state.id.volume) - 1] = 0;
				(void)strupper(state.id.volume);
				break;
#if DEBUG
			case 'B':
				if (!*s) state.in.blok = state.out.blok = 1;
				else while (*s) switch (*s++)
				{
				case 'i':
					state.in.blok = 1;
					break;
				case 'o':
					state.out.blok = 1;
					break;
				default:
					error(3, "-RB[io] expected", *(s - 1));
				}
				break;
#endif
			case 'U':
				if (!*s) state.out.unblocked = state.out.unblocked = 1;
				else while (*s) switch (*s++)
				{
				case 'i':
					state.in.unblocked = 1;
					break;
				case 'o':
					state.out.unblocked = 1;
					break;
				default:
					error(3, "-RU[io] expected", *(s - 1));
				}
				break;
			default:
				error(3, "invalid tape option %c", *(s - 1));
			}
		} while (s = t);
		break;

	/* extended options */

	case 'e':
		state.filter = opt_arg;
		break;
	case 'h':
		state.summary = 0;
		break;
	case 'k':
		state.keepgoing = 1;
		break;
	case 'n':
		state.exact = 1;
		break;
	case 'z':
		if (streq(opt_arg, FILE_DEFAULT)) state.delta.op |= COMPRESS;
		else state.delta.base = opt_arg;
		break;
	case 'D':
		state.trace = -opt_num;
		break;
	case 'L':
		state.ftwflags &= ~FTW_PHYSICAL;
		break;
	case 'P':
		state.ftwflags |= FTW_PHYSICAL;
		break;
	case 'T':
		state.test |= opt_num;
		break;

	/* tar compatibility options */

	case 'a':
		state.append = 1;
		break;
	case 't':
		if (state.file) error(1, "%s overrides %s", opt_arg, state.file);
		state.file = strtape(opt_arg, &p);
		if (*p) error(3, "%s: invalid tape unit specification", opt_arg);
		break;
	case 'u':
		state.update = 1;
		break;
	case 'y':
		state.verify = 1;
		state.yesno = 1;
		break;

	/* cpio compatibility options */

	case 'c':
		state.matchsense = 0;
		break;
	case 'd':
		state.intermediate = 0;
		break;
	case 'i':
		state.verify = 1;
		break;
	case 'l':
		state.linkf = link;
		break;
	case 'p':
		state.acctime = 1;
		break;
	case 'B':
		state.maxout = strton(opt_arg, &p, 0);
		if (*p) error(3, "%s: invalid block count", opt_arg);
		break;
	case 'C':
		state.complete = 1;
		break;
	case 'M':
		eomprompt = opt_arg;
		break;
	case 'S':
		state.linkf = putsymlink;
		break;
	case 'U':
		idname = opt_arg;
		break;
	case 'V':
		state.drop = 1;
		break;
	case 'X':
		state.xdev = 1;
		break;

	/* option errors */

	case '?':
		error(ERROR_USAGE|4, opt_arg);
		break;
	case ':':
		error(2, opt_arg);
		break;

	}
	argv += opt_index;
	argc -= opt_index;
	if (state.errors) error(ERROR_USAGE|4, optusage((char*)0));
	if (!state.operation)
	{
		state.operation = IN;
		state.list = 1;
	}
	state.statf = (state.ftwflags & FTW_PHYSICAL) ? lstat : lpstat;
	if (state.delta.base) state.delta.op |= (state.operation & IN) ? IN : OUT;
	if (state.formatout < 0) state.formatout = OUT_DEFAULT;
	else switch (state.operation)
	{
	case OUT:
		if (state.append) error(1, "format ignored for archive append");
		break;
	case (IN|OUT):
		if (!(state.delta.op & (IN|OUT))) state.delta.op |= (state.delta.op & COMPRESS) ? IN : CONVERT;
		break;
	default:
		error(1, "format ignored for archive read");
		state.formatout = OUT_DEFAULT;
		break;
	}
	switch (state.operation)
	{
	case IN|OUT:
		if (state.append) error(1, "append ignored for pass mode");
		if (!(state.delta.op & COMPRESS) && state.file)
		{
			error(1, "%s: file name ignored for pass mode", state.file);
			break;
		}
		/*FALLTHROUGH*/
	case IN:
		if (state.operation == IN && (state.delta.op & (COMPRESS|IN)) == COMPRESS) error(1, "compress ignored for archive read");
		if (state.append) error(1, "append ignored for archive read");
		if (!state.file || streq(state.file, FILE_DEFAULT)) state.file = definput;
		else
		{
			close(0);
			if (open(state.file, 0) != 0) error(ERROR_SYSTEM|3, "%s: cannot read", state.file);
		}
		break;
	case OUT:
		if (state.update) state.append = 1;
		if (!state.file || streq(state.file, FILE_DEFAULT))
		{
			if (state.append) error(3, "archive pathname must be specified for append");
			state.file = defoutput;
		}
		else
		{
			close(1);
			if (state.append)
			{
				if (state.delta.op) error(3, "cannot append delta archives");
				if (open(state.file, 2) != 1) error(ERROR_SYSTEM|3, "%s: cannot append", state.file);
			}
			else if (creat(state.file, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH) != 1) error(ERROR_SYSTEM|3, "%s: cannot write", state.file);
		}
		break;
	}
	if (!(state.linktab = hashalloc((HASHTABLE*)0, HASH_set, HASH_ALLOCATE, HASH_namesize, sizeof(struct fileid), HASH_name, "links", 0)))
		error(3, "cannot allocate hard link table");
	if ((state.operation & IN) && !state.list && !(state.restore = hashalloc((HASHTABLE*)0, HASH_set, HASH_ALLOCATE, HASH_name, "restore", 0)))
		error(3, "cannot allocate directory table");
	if (state.modtime) state.modemask = 0;
	if (state.owner || idname)
	{
		if (state.operation & IN)
		{
			state.owner = 1;
			state.modemask = 0;
			if (idname)
			{
				if ((state.setuid = struid(idname)) < 0 || (state.setgid = strgid(idname)) < 0)
					error(3, "%s: invalid user name", idname);
				state.flags |= SETIDS;
			}
		}
		else error(1, "ownership assignment ignored on archive write");
	}
	if (state.verify) interactive();
	state.modemask = ~state.modemask & (S_ISUID|S_ISGID|S_ISVTX|S_IRWXU|S_IRWXG|S_IRWXO);
	for (i = 0; i < elements(signals); i++)
		if (signal(signals[i], interrupt) == SIG_IGN)
			(void)signal(signals[i], SIG_IGN);
#if DEBUG
	message(-1, "%s", operations());
#endif
	switch (state.operation)
	{
	case IN:
		if (*argv)
		{
			state.patterns = initmatch(argv);
			if (state.exact) state.matchsense = 1;
		}
		else if (state.exact) error(3, "file arguments expected");
		binit();
		(void)getcwd(state.pwd, PATH_MAX);
		state.pwdlen = strlen(state.pwd);
		state.pwd[state.pwdlen++] = '/';
		if (state.delta.op & IN) deltabase();
		copyin();
		if (state.exact)
		{
			argv = state.patterns;
			while (s = *argv++)
				if (*s) error(2, "%s: file not found in archive", s);
		}
		break;

	case OUT:
		if (*argv) state.files = argv;
		binit();
		if (!state.maxout && state.complete)
			error(3, "maximum block count required");
		if (state.append) append();
		if (state.delta.op & OUT) deltabase();
		putprologue();
		copy(copyout);
		if (state.delta.op & OUT) deltadelete();
		putepilogue();
		break;

	case (IN|OUT):
		if (state.delta.op)
		{
			if (*argv) state.patterns = initmatch(argv);
			state.operation = OUT;
			binit();
			state.operation = IN|OUT;
			if (state.delta.op & IN) deltabase();
			deltapass();
		}
		else
		{
			if (--argc < 0)
			{
				error(2, "destination directory required for pass mode");
				error(ERROR_USAGE|4, optusage((char*)0));
			}
			state.destination = argv[argc];
			argv[argc] = 0;
			if (*argv) state.files = argv;
			if (state.blocksize) error(1, "blocking ignored in pass mode");
			binit();
			if (state.record.size) error(1, "record size ignored in pass mode");

			/*
			 * initialize destination dir
			 */

			(void)pathcanon(state.destination);
			if (stat(state.destination, file.st) || (file.st->st_mode & S_IFMT) != S_IFDIR)
				error(3, "%s: destination must be a directory", state.destination);
			state.dev = file.st->st_dev;
			(void)strcpy(state.pwd, state.destination);
			state.pwdlen = strlen(state.pwd);
			state.pwd[state.pwdlen++] = '/';
			copy(copyinout);
		}
		break;
	}
	finish(0);
}

/*
 * print number of blocks actually copied and exit
 */

void
finish(code)
int	code;
{
	register char*	x1 = &state.tmp.buffer[0];
	register char*	x2 = &state.tmp.buffer[state.buffersize / 2];
	register long	n;

	(void)remove(state.tmp.file);
	if (state.restore) (void)hashwalk(state.restore, 0, restore);
	fflush(stdout);
	if (state.dropcount)
	{
		fprintf(stderr, "\n");
		fflush(stderr);
	}
	if (state.summary)
	{
		switch (state.operation)
		{
		case OUT:
			n = state.out.count + state.out.offset;
			break;
		case IN|OUT:
			n = state.out.count + state.out.offset;
			if (n || !(state.delta.op & IN)) break;
			/*FALLTHROUGH*/
		case IN:
			n = state.in.count + state.in.offset;
			break;
		}
		if (state.entries)
		{
			if (state.volume > 1) (void)sprintf(x1, ", %d volumes", state.volume);
			else *x1 = 0;
			if (state.volume > 0 && state.part > state.volume) (void)sprintf(x2, ", %d parts", state.part - state.volume + 1);
			else *x2 = 0;
			n = (n + BLOCKSIZE - 1) / BLOCKSIZE;
			if (state.verbose) fprintf(stderr, "%d file%s, %ld block%s%s%s\n", state.selected, state.selected == 1 ? "" : "s", n, n == 1 ? "" : "s", x1, x2);
			else fprintf(stderr, "%ld block%s%s%s\n", n, n == 1 ? "" : "s", x1, x2);
		}
	}
	fflush(stderr);
	if (state.interrupt)
	{
		(void)signal(state.interrupt, SIG_DFL);
		kill(getpid(), state.interrupt);
		pause();
	}
	exit(code ? code : state.errors != 0);
}

/*
 * clean up dir info before exit
 */

static void
interrupt(sig)
int	sig;
{
	(void)signal(sig, SIG_IGN);
	sigunblock(sig);
	switch (sig)
	{
	case SIGINT:
	case SIGQUIT:
		fprintf(stderr, "\n");
		break;
	}
	state.interrupt = sig;
	finish(1);
}

/*
 * error hook for re lib
 */

void
reerror(msg)
char*	msg;
{
	error(3, "substitution: %s", msg);
}

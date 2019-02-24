/*
 * lcc [ option ]... [ file | -llib ]...
 * front end for the ANSI C compiler
 */
/* Fri Feb 5 07:56:38 EST 1993 */
#define VERSION ((2<<8)|0)

#include <stdio.h>
#include <ctype.h>
#include <signal.h>

#ifndef TEMPDIR
#define TEMPDIR "/tmp"
#endif

typedef struct list *List;
struct list {		/* circular list nodes: */
	char *str;		/* option or file name */
	List link;		/* next list element */
};

#if __LCC__ || __STDC__
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define dclproto(func,args) func args
dclproto(static void interrupt,(int));
#else
#define assert(e) ((void)((e)||(fprintf(stderr, "assertion failed: file %s, line %d\n", \
	__FILE__, __LINE__), abort(), 0)))
#define dclproto(func,args) func()
dclproto(static void interrupt,());
#endif

dclproto(static char *alloc,(int));
dclproto(static List append,(char *,List));
dclproto(extern char *basename,(char *));
dclproto(static int callsys,(char *[]));
dclproto(extern char *concat,(char *, char *));
dclproto(static int compile,(char *, char *));
dclproto(static void compose,(char *[], List, List, List));
dclproto(static void cprint,(char *[], char *));
dclproto(static void error,(char *, char *));
dclproto(static void execute,(char *[]));
dclproto(static int exists,(char *));
dclproto(static int filename,(char *, char *));
dclproto(static List find,(char *, List));
dclproto(static void opt,(char *));
dclproto(static void rm,(List));
dclproto(extern char *strsave,(char *));
dclproto(extern int suffix,(char *));

dclproto(extern int execv,(char *, char *[]));
dclproto(extern int fork,(void));
dclproto(extern int close,(int));
dclproto(extern int dup,(int));
dclproto(extern int fork,(void));
dclproto(extern int getpid,(void));
dclproto(extern int open,(char *, int));
dclproto(extern int pipe,(int[]));
dclproto(extern int read,(int, char *, int));
dclproto(extern int unlink,(char *));
dclproto(extern int wait,(int*));

extern char *cpp[], *include[], *com[], *as[],*ld[];
dclproto(extern int option,(char *));

static int errcnt;		/* number of errors */
static int Eflag;		/* -E specified */
static int Sflag;		/* -S specified */
static int cflag;		/* -c specified */
static int verbose;		/* incremented for each -v */
static List llist[2];		/* loader files, flags */
static List alist;		/* assembler flags */
static List clist;		/* compiler flags */
static List plist;		/* preprocessor flags */
static List rmlist;		/* list of files to remove */
static char *outfile;		/* ld output file or -[cS] object file */
static int ac;			/* argument count */
static char **av;		/* argument vector */
static char tempname[sizeof TEMPDIR + 15];	/* temporary .s file */
static char *progname;

main(argc, argv) char *argv[]; {
	int i, j, nf;
	
	progname = argv[0];
	ac = argc + 50;
	av = (char **)alloc(ac*sizeof(char *));
	if (signal(SIGINT, SIG_IGN) != SIG_IGN)
		signal(SIGINT, interrupt);
	if (signal(SIGTERM, SIG_IGN) != SIG_IGN)
		signal(SIGTERM, interrupt);
#ifdef SIGHUP
	if (signal(SIGHUP, SIG_IGN) != SIG_IGN)
		signal(SIGHUP, interrupt);
#endif
	sprintf(tempname, "%s/lcc%05d.s", TEMPDIR, getpid());
	rmlist = append(tempname, rmlist);
	plist = append("-D__LCC__", append("-Dunix", 0));
	for (nf = 0, i = j = 1; i < argc; i++) {
		if (strcmp(argv[i], "-o") == 0) {
			if (++i < argc) {
				if (strchr("ci", suffix(argv[i]))) {
					error("-o would overwrite %s", argv[i]);
					exit(8);
				}
				outfile = argv[i];
				continue;
			} else {
				error("unrecognized option `%s'", argv[i]);
				exit(8);
			}
#ifdef sun
		} else if (strcmp(argv[i], "-target") == 0) {
			if (argv[i+1] && *argv[i+1] != '-')
				i++;
			continue;
#endif
		} else if (*argv[i] == '-' && argv[i][1] != 'l') {
			opt(argv[i]);
			continue;
		} else if (*argv[i] != '-' && strchr("csi", suffix(argv[i])))
			nf++;
		argv[j++] = argv[i];
	}
	if ((cflag || Sflag) && outfile && nf != 1) {
		fprintf(stderr, "%s: -o %s ignored\n", progname, outfile);
		outfile = 0;
	}
	argv[j] = 0;
	for (i = 0; include[i]; i++)
		plist = append(include[i], plist);
	for (i = 1; argv[i]; i++)
		if (*argv[i] == '-')
			opt(argv[i]);
		else {
			if (nf > 1 && strchr("csi", suffix(argv[i]))) {
				fprintf(stderr, "%s:\n", argv[i]);
				fflush(stdout);
			}
			filename(argv[i], 0);
		}
	if (errcnt == 0 && !Eflag && !Sflag && !cflag && llist[1]) {
		if (i == 2 && strchr("csi", suffix(argv[1])))
			rmlist = append(concat(basename(argv[1]), ".o"), rmlist);
		compose(ld, llist[0], llist[1], append(outfile ? outfile : "a.out", 0));
		if (callsys(av))
			errcnt++;
	}
	rm(rmlist);	
	exit(errcnt > 0);
}

/* alloc - allocate n bytes or die */
static char *alloc(n) {
	static char *avail, *limit;
	
	n = (n + sizeof(char *) - 1)&~(sizeof(char *) - 1);
	if (avail + n >= limit) {
		avail = (char *)malloc(n + 4*1024);
		assert(avail);
		limit = avail + n + 4*1024;
	}
	avail += n;
	return avail - n;
}

/* append - append a node with string str onto list, return new list */	
static List append(str, list) char *str; List list; {
	List p = (List) alloc(sizeof *p);

	p->str = str;
	if (list) {
		p->link = list->link;
		list->link = p;
	} else
		p->link = p;
	return p;
}

/* basename - return base name for name, e.g. /usr/drh/foo.c => foo */
char *basename(name) char *name; {
	char *s, *b, *t = 0;

	for (b = s = name; *s; s++)
		if (*s == '/') {
			b = s + 1;
			t = 0;
		} else if (*s == '.')
			t = s;
	s = strsave(b);
	if (t)
		s[t-b] = 0;
	return s;
}

/* callsys - fork and execute the command described by argv[0...], return status */
static int callsys(argv) char **argv; {
	int n, m = 0, status = 0, pid;

	cprint(argv, 0);
	if (verbose >= 2)
		return 0;
	switch (pid = fork()) {
	case -1:
		fprintf(stderr, "%s: no more processes\n", progname);
		return 100;
	case 0:
		execute(argv);
	}
	while ((n = wait(&m)) != pid && n != -1)
		status |= m;
	status |= m;
	if (status&0377) {
		fprintf(stderr, "%s: fatal error in %s\n", progname, argv[0]);
		status |= 0400;
	}
	return (status>>8)&0377;
}

/* concat - return concatenation of strings s1 and s2 */
char *concat(s1, s2) char *s1, *s2; {
	int n = strlen(s1);
	char *s = alloc(n + strlen(s2) + 1);

	strcpy(s, s1);
	strcpy(s + n, s2);
	return s;
}

/* compile - compile src into dst, return status */
static int compile(src, dst) char *src, *dst; {
	int n, status;

	compose(com, clist, append(src, 0), append(dst, 0));
	status = callsys(av);
	if (status && *src == '-') {
		char buf[1024];
		while ((n = read(0, buf, sizeof buf)) > 0)
			;
	}
	return status;
}

/* compose - compose cmd into av substituting a, b, c for $1, $2, $3, resp. */
static void compose(cmd, a, b, c) char *cmd[]; List a, b, c; {
	int i, j;
	List lists[3];

	lists[0] = a;
	lists[1] = b;
	lists[2] = c;
	for (i = j = 0; cmd[i]; i++)
		if (cmd[i][0] == '$' && isdigit(cmd[i][1])) {
			int k = cmd[i][1] - '0';
			assert(k >= 1 && k <= 3);
			if (b = lists[k-1])
				do {
					b = b->link;
					assert(j < ac);
					av[j++] = b->str;
				} while (b != lists[k-1]);
		} else if (*cmd[i]) {
			assert(j < ac);
			av[j++] = cmd[i];
		}
	av[j] = 0;
}

/* cprint - print the command described by argv[0...] followed by str or \n */
static void cprint(argv, str) char *argv[], *str; {
	if (verbose) {
		fprintf(stderr, "%s", *argv++);
		while (*argv)
			fprintf(stderr, " %s", *argv++);
		if (str == 0)
			str = "\n";
		fprintf(stderr, str);
	}
}

/* error - issue error msg according to fmt, bump error count */
static void error(fmt, msg) char *fmt, *msg; {
	fprintf(stderr, "%s: ", progname);
	fprintf(stderr, fmt, msg);
	fprintf(stderr, "\n");
	errcnt++;
}

/* execute - replace this process by the command described by argv[0...] */
static void execute(argv) char *argv[]; {
	if (verbose >= 2)
		return;
	execv(argv[0], argv);
	fprintf(stderr, "%s: can't execute `%s'\n", progname, argv[0]);
	fflush(stdout);
	exit(100);
}

/* exists - is `name' readable? issue message if not */
static int exists(name) char *name; {
	if (verbose > 1 || access(name, 4) == 0)
		return 1;
	error("can't read `%s'", name);
	return 0;
}

/* filename - process file name argument `name', return status */
static int filename(name, base) char *name, *base; {
	int status = 0;

	if (base == 0)
		base = basename(name);
	switch (suffix(name)) {
	case 'c':
		if (!exists(name))
			break;
		compose(cpp, plist, append(name, 0), 0);
		if (Eflag) {
			status = callsys(av);
			break;
		}
		cprint(av, " | ");
		if (verbose <= 1) {
			int fd[2], pid;
			if (pipe(fd) < 0) {
				error("can't create preprocessor-compiler pipe\n", 0);
				exit(1);
			}
			switch (pid = fork()) {
			case -1:
				fprintf(stderr, "%s: no more processes\n", progname);
				return 100;
			case 0:
				close(1);
				dup(fd[1]);
				close(fd[0]);
				close(fd[1]);
				execute(av);
				assert(0);	/* no return from execute */
			}
			close(0);
			dup(fd[0]);
			close(fd[0]);
			close(fd[1]);
		}
		if (Sflag)
			status = compile("-", outfile ? outfile : concat(base, ".s"));
		else if ((status = compile("-", tempname)) == 0)
			return filename(tempname, base);
		break;
	case 'i':
		if (!exists(name) || Eflag)
			break;
		if (Sflag)
			status = compile(name, outfile ? outfile : concat(base, ".s"));
		else if ((status = compile(name, tempname)) == 0)
			return filename(tempname, base);
		break;
	case 's':
		if (Eflag)
			break;
		if (!Sflag) {
			char *ofile = cflag && outfile ? outfile : concat(base, ".o");
			compose(as, alist, append(name, 0), append(ofile, 0));
			status = callsys(av);
			if (!find(ofile, llist[1]))
				llist[1] = append(ofile, llist[1]);
		}
		break;
	case 'o':
		if (!find(name, llist[1]))
			llist[1] = append(name, llist[1]);
		break;
	case -1:
		if (Eflag) {
			compose(cpp, plist, append(name, 0), 0);
			status = callsys(av);
		} /* else fall thru */
	default:
		llist[1] = append(name, llist[1]);
		break;
	}
	if (status)
		errcnt++;
	return status;
}

/* find - find 1st occurrence of str in list, return list node or 0 */
static List find(str, list) char *str; List list; {
	List b;
	
	if (b = list)
		do {
			if (strcmp(str, b->str) == 0)
				return b;
		} while ((b = b->link) != list);
	return 0;
}

static
void
interrupt(n) {
	rm(rmlist);
	exit(n = 100);
}

/* opt - process option in arg */
static void opt(arg) char *arg; {
	switch (arg[1]) {	/* multi-character options */
	case 'W':	/* -Wxarg */
		if (arg[2] && arg[3])
			switch (arg[2]) {
			case 'o':
				if (option(&arg[3]))
					return;
				break;
			case 'p':
				plist = append(&arg[3], plist);
				return;
			case 'f':
				if (strcmp(&arg[3], "-C") || option("-b")) {
					clist = append(&arg[3], clist);
					return;
				}
				break; /* and fall thru */
			case 'a':
				alist = append(&arg[3], alist);
				return;
			case 'l':
				llist[0] = append(&arg[3], llist[0]);
				return;
			}
		fprintf(stderr, "%s: %s ignored\n", progname, arg);
		return;
	case 'd':	/* -dn */
		arg[1] = 's';
		/* fall thru */
	case 't':	/* -t -tname */
		clist = append(arg, clist);
		return;
#ifdef sun
	case 'f':	/* -f68881 */
		if (!option(arg))
			fprintf(stderr, "%s: %s ignored\n", progname, arg);
		return;
#endif
	case 'p':	/* -p -pg */
		if (option(arg))
			clist = append(arg, clist);
		else
			fprintf(stderr, "%s: %s ignored\n", progname, arg);
		return;
	case 'D':	/* -Dname -Dname=def */
	case 'U':	/* -Uname */
	case 'I':	/* -Idir */
		plist = append(arg, plist);
		return;
	case 'B':	/* -Bdir/ -Bstatic -Bdynamic */
#ifdef sun
		if (strcmp(arg, "-Bstatic") == 0 || strcmp(arg, "-Bdynamic") == 0)
			llist[1] = append(arg, llist[1]);
		else
#endif	
		{
		static char *path;
		if (path)
			error("-B overwrites earlier option", 0);
		path = arg + 2;
		com[0] = concat(path, "rcc");
		if (path[0] == 0)
			error("missing directory in -B option", 0);
		}
		return;
	}
	if (arg[2] == 0)
		switch (arg[1]) {	/* single-character options */
		case 'S':
			Sflag++;
			return;
		case 'O':
			fprintf(stderr, "%s: %s ignored\n", progname, arg);
			return;
		case 'A': case 'n': case 'w': case 'P':
			clist = append(arg, clist);
			return;
		case 'g': case 'b':
			if (option(arg))
				clist = append(arg[1] == 'g' ? "-g2" : arg, clist);
			else
				fprintf(stderr, "%s: %s ignored\n", progname, arg);
			return;
		case 'G':
			if (option(arg)) {
				clist = append("-g3", clist);
				llist[0] = append("-N", llist[0]);
			} else
				fprintf(stderr, "%s: %s ignored\n", progname, arg);
			return;
		case 'E':
			Eflag++;
			return;
		case 'c':
			cflag++;
			return;
		case 'C':
			plist = append(arg, plist);
			return;
		case 'N':
			if (strcmp(basename(cpp[0]), "gcc-cpp") == 0)
				plist = append("-nostdinc", plist);
			include[0] = 0;
			return;
		case 'v':
			if (verbose++ == 0) {
				if (strcmp(basename(cpp[0]), "gcc-cpp") == 0)
					plist = append(arg, plist);
				clist = append(arg, clist);
				fprintf(stderr, "%s version %d.%d\n", progname, VERSION>>8, VERSION&0xff);
			}
			return;
		}
	if (cflag || Sflag || Eflag)
		fprintf(stderr, "%s: %s ignored\n", progname, arg);
	else
		llist[1] = append(arg, llist[1]);
}

/* rm - remove files in list */
static void rm(list) List list; {
	if (list) {
		List b = list;
		if (verbose)
			fprintf(stderr, "rm");
		do {
			if (verbose)
				fprintf(stderr, " %s", b->str);
			unlink(b->str);
		} while ((b = b->link) != list);
		if (verbose)
			fprintf(stderr, "\n");
	}
}

/* strsave - return a saved copy of string str */
char *strsave(str) char *str; {
	return (char *)strcpy(alloc(strlen(str)+1), str);
}

/* suffix - return the 1-character suffix of name, e.g. /usr/drh/foo.c => 'c' */
int suffix(name) char *name; {
	char *t = 0, *s;

	for (s = name; *s; s++)
		if (*s == '/')
			t = 0;
		else if (*s == '.')
			t = s + 1;
	if (t && t[1] == 0)
		return t[0];
	return -1;
}

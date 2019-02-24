/*
 * adb - main command loop and error/interrupt handling
 */
#include "defs.h"
#include <signal.h>
#include <setjmp.h>

char *errflg;
BOOL wtflag;

BOOL mkfault;
ADDR maxoff;

int xargc;		/* bullshit */

extern	BOOL	executing;
extern	int	infile;
static int	exitflg;
int	(*sigint)();
int	(*sigqit)();

extern	char	lastc;
extern	int	eof;

extern	char	*Ipath;
static jmp_buf env;

main(argc, argv)
register char **argv;
int argc;
{
	extern int fault();

	maxoff = MAXOFF;
another:
	if (argc>1) {
		if (strcmp("-w", argv[1]) == 0) {
			wtflag = 2;		/* suitable for open() */
			argc--, argv++;
			goto another;
		}
		if (argv[1][0] == '-' && argv[1][1] == 'I') {
			Ipath = argv[1]+2;
			argc--, argv++;
		}
	}
	if (argc > 1)
		symfil = argv[1];
	if (argc > 2)
		corfil = argv[2];
	xargc = argc;
	mchinit();
	setsym();
	setcor();
	setvar();

	if ((sigint=signal(SIGINT,SIG_IGN)) != SIG_IGN) {
		sigint = fault;
		signal(SIGINT, fault);
	}
	sigqit = signal(SIGQUIT, SIG_IGN);
	setjmp(env);
	if (executing)
		delbp();
	executing = FALSE;
	for (;;) {
		flushbuf();
		if (errflg) {
			printf("%s\n", errflg);
			exitflg = 1;
			errflg = NULL;
		}
		if (mkfault) {
			mkfault=0;
			printc('\n');
			prints(DBNAME);
		}
		clrinp();
		rdc();
		reread();
		if (eof) {
			if (infile == STDIN)
				done();
			iclose(-1, 0);
			eof = 0;
			longjmp(env, 1);
		}
		exitflg = 0;
		command((char *)NULL, 0);
		reread();
		if (rdc() != '\n')
			error("newline expected");
	}
}

done()
{
	if (pid)
		endpcs();
	exit(exitflg);
}

WORD
round(a,b)
register WORD a, b;
{
	register WORD w;

	w = (a/b)*b;
	if (a!=w)
		w += b;
	return(w);
}

/*
 * If there has been an error or a fault, take the error.
 */
chkerr()
{
	if (errflg || mkfault)
		error(errflg);
}

/*
 * An error occurred; save the message for later printing,
 * close open files, and reset to main command loop.
 */
error(n)
char *n;
{
	errflg = n;
	iclose(0, 1);
	oclose();
	longjmp(env, 1);
}

/*
 * An interrupt occurred; reset the interrupt
 * catch, seek to the end of the current file
 * and remember that there was a fault.
 */
fault(a)
{
	signal(a, fault);
	lseek(infile, 0L, 2);
	mkfault++;
}

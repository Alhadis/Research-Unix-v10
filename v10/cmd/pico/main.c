#include <stdio.h>
#include <signal.h>
#include <setjmp.h>
#include "pico.h"
#include "pico2.h"

int DEF_LL = 512;		/* default linelength            */
int DEF_NL = 512;		/* default number of lines       */

short CURSCRATCH = 1;
short CUROLD     = 0;

char *progr;
struct SRC src[MANY];
int nsrc = 2;			/* 0,1 reserved for Old, and Scratch */

char frameb	= 0;		/* open framebuffer   */
char metheus	= 0;		/* open metheus display */
char faster	= 1;		/* update display 1 line iso 1 pel at a time */
char usednew	= 0;		/* did program use Scratch  ?    */
char usedold	= 0;		/* did program use Old      ?    */
char seetree	= 0;
char optim	= 1;

int  nrparams	= 0;
char isglobal	= 0;
int whichdevice	= 0;

extern char nesting;
extern int linenumber, nalloc;
FILE *INPUT = stdin;
jmp_buf begin;
int  (*oldquit)();

onquit()
{
	fprintf(stderr, "\ninterrupted\n");
	oldquit = signal(SIGINT, onquit);
	if (INPUT != stdin) undirect();
	fflush(stderr);
	longjmp(begin, 0);
}

Reset()
{
	if (linenumber < 1) linenumber = 1;
	nalloc = 0; nesting = 0;
	usednew = usedold = nrparams = 0;
}

main(argc, argv)
	char **argv;
{
	int i=1, base=1;
	char c;

	Reset();
	while (argc > base && argv[base][0] == '-')
	{	while ((c = argv[base][i++]) != '\0')
			switch (c) {
			case 'f': frameb  = 1; break;
			case 'm': metheus = 1; break;
			case 't': seetree = 1; break;
			case 'o': optim   = 1 - optim; break;
			case 'd': DEF_LL = 1280; DEF_NL = 1024; break;
			case 'w': DEF_LL = atoi(&argv[base][i]); goto done;
			case 'h': DEF_NL = atoi(&argv[base][i]); goto done;
			case '0': case '1': case '2':
			case '3': case '4': case '5':
			case '6': case '7': case '8': case '9':
				whichdevice = c - '0';
				break;
			default : Usage("unknown option");
			}
done:		base++; i = 1;
	}
	Init();
	Old->nchan = Scratch->nchan = 1;
	snore()
	fprintf(stderr, "b&w, %dx%d pel, ", DEF_LL, DEF_NL);
	if (metheus)
		fprintf(stderr, "metheus display\n");
	else if (frameb)
		fprintf(stderr, "framebuffer\n");
	else
		fprintf(stderr, "no display\n");

	setwindow(0, 0, DEF_LL, DEF_NL);
	for (; base < argc; base++)
		prepare(argv[base]);

	linenumber = i = 1;
	setjmp(begin);
	signal(SIGINT, onquit);

	if (i && (i = open("/usr/lib/pico/defines", 0)) > 0)
	{	close(i);
		redirect("/usr/lib/pico/defines");
		linenumber = i = 0;	/* to avoid a loop via longjmp */
	}

	do {	Reset();
		Prompt;
	} while (yyparse());

}

Init()
{
	syminit();				/* initialize symbol table */
	nopix(Old);				/* allocate workspace */
	nopix(Scratch);
	ABSENT
}

extern char lastyy, operator, nolf;
static int curline;

yyerror(s1, d)
	char *s1;
{	extern int pline;
	extern char *thisfun;

	if (nesting > 0)
		fprintf(stderr, "line %d/%d: ", linenumber-curline, pline);
	fprintf(stderr, s1, d);
	fprintf(stderr, "\n");

	if (lastyy != '\n')
		do; while(yylex() != ';' || nolf);
	nesting = 1; operator = 0; sympurge(); nesting = 0;
	thisfun = '\0';
	if (INPUT != stdin) undirect();

	longjmp(begin, 0);
}

redirect(s)
	char *s;
{	extern char lastyy;

	if (INPUT != stdin)
	{	fprintf(stderr, "cannot redirect recursively\n");
		return 0;
	}
	curline = linenumber;
	if ((INPUT = fopen(s, "r")) == NULL)
	{	fprintf(stderr, "cannot open %s\n", s);
		INPUT = stdin;
		return 0;
	}
	ungetc('\n', stdin);
	lastyy = '\n';
	return 1;
}

undirect()
{
	if (INPUT != stdin)
	{	fclose(INPUT);
	} else
	{	fprintf(stderr, "\n");
		exit(0);
	}
	INPUT = stdin;
	linenumber = curline-1;	/* 1 <cr> was unget */
	curline = 0;
}

dohelp()
{	int pid, rpid, retcode;

	if ((pid = fork()) == 0)
	{	signal(SIGINT, oldquit);
		execl("/bin/cat", "/bin/cat", "/usr/lib/pico/help", 0);
		fprintf(stderr, "pico: /bin/cat not found...\n");
		exit(0100);
	}
	signal(SIGINT, SIG_IGN);
	while ((rpid = wait(&retcode)) != pid && rpid != -1)
		;
	signal(SIGINT, onquit);
}

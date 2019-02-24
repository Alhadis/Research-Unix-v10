/*%cc p.c pad.o spname.o
 */
#include <stdio.h>
#include "pad.h"
#include <signal.h>
#define	DEF	22	/* 3*DEF == 66, #lines per nroff page */

#define	STDIN	(char *)0
int nprintfiles = 0;
int lineno = 0;
int pglen = DEF;
FILE *tty;
static char obuf[BUFSIZ];

#ifndef SPNAME
#define	spopen(f, m)	Pfopen(f, m)
#endif

main(argc, argv)
	char *argv[];
{
	if((tty=fopen("/dev/tty", "r")) == NULL) {
		fprintf(stderr, "p: no /dev/tty\n");
		exit(1);
	}
	setbuf(stdout, obuf);
	while(argc > 1) {
		--argc; argv++;
		if(*argv[0] == '-') {
			pglen = atoi(&argv[0][1]);
			if(pglen < 0)
				pglen = DEF;
		} else
			printfile(argv[0], argc==1);
	}
	if(nprintfiles == 0)
		printfile(STDIN, 1);
	return 0;
}

#if SPNAME
PAD *
spopen(file, mode)
	register char *file;
	register char *mode;
{
	register PAD *f;
	register int c;
	extern char *spname();

	if ((f=Pfopen(file, mode)) != NULL)
		return (f);
	if ((file = spname(file)) == NULL)
		return (f);
	fprintf(stderr, "\"p %s\"? ", file);
	fflush(stdout);
	c = getc(tty);
	if(c != 'n')
		f=Pfopen(file, mode);
	while(c != '\n' && c != EOF)
		c = getc(tty);
	return(f);
}
#endif

printfile(file, last)
	register char *file;
{
	register PAD *pad;
	register c;

	nprintfiles++;
	if(file == STDIN)
		pad=Pfdopen(0);
	else if((pad=spopen(file, "r")) == NULL) {
		fprintf(stderr, "p: can't open %s\n", file);
		return;
	}
	while((c=Pgetc(pad))!=EOF || (!last && newline(pad)=='-')) {
		if(c == '\n' && ++lineno==pglen) {
			if(newline(pad)==EOF)
				goto Return;
		} else
			putchar(c);
	}
	fflush(stdout);
 Return:
	Pclose(pad);
}

newline(p)
	register PAD *p;
{
	register c, i;

	lineno = 0;
	fflush(stdout);
  loop:
	switch(getc(tty)){
	case '\n':		/* Continue */
		return '\n';
	case '!':
		callunix();
		goto loop;
	case 'q':
		waitnl();
		/* Fall through */
	case EOF:
		return EOF;
	case '-':
		for(i=1; (c=getc(tty))=='-'; i++)
			;
		ungetc(c, tty);
		backpage(p, i);
		waitnl();
		return '-';
	default:
		waitnl();
		return '\n';
	}
}

backpage(p, pg)
	register PAD *p;
	register pg;
{
	register i;
	while(pg--)
		for(i=0; i<pglen; i++)
			if(backline(p)==EOF)
				return;
	/* Now at end of new first line; back up to beginning */
	if(backline(p)=='\n')
		(void)Pgetc(p);		/* Eat that newline */
}

backline(p)
	register PAD *p;
{
	register c;
	while((c=Pbackc(p))!='\n')
		if(c==EOF)
			return(EOF);
	return('\n');
}

waitnl(){
	do; while(getc(tty) != '\n');
}

callunix()
{
	int rc, status, unixpid;
	char buf[256];
	register char *p;
	for(p=buf; (*p++=getc(tty))!='\n'; )
		;
	*--p=0;
	if( (unixpid=fork())==0 ) {
		close(0); dup(2);
		execl("/bin/sh", "sh", "-c", buf, 0);
		exit(255);
	}
	else if(unixpid == -1){
		fprintf(stderr, "p: can't fork\n");
		return;
	}else{	signal(SIGINT, SIG_IGN); signal(SIGQUIT, SIG_IGN);
		while( (rc = wait(&status)) != unixpid && rc != -1 ) ;
		signal(SIGINT, SIG_DFL); signal(SIGQUIT, SIG_DFL);
		fprintf(stderr, "!\n");
		fflush(stdout);
	}
}

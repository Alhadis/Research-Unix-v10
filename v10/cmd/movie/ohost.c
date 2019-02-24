/*
	This is version 4.  The input language is:

Cmd	Abbrev	Operands / Comments
comment	#		only at start of line
blank	b	s <vnum>	start blanking; erase commands follow
		e <vnum>	end blanking current view
click	c	<clicknum>
define	d	v <number> <viewname> xmin ymin xmax ymax
		c <number> <clickname>
		p e	pragma -- end of defs at front of file
			Other possible ``pragmas'':			???
				Number of views, clicks
				Number of (lines, bytes) in file
erase	e	<line repeated here, except for leading g>
			Object is deletable
geom	g	<slotn>	l <vnum> <opts> <x1> <y1> <x2> <y2>	line
			b <vnum> <opts> <x1> <y1> <x2> <y2>	box
			c <vnum> <opts> <x> <y> <rad>		circle
			t <vnum> <opts> <x> <y> <text string>	text
				one separating blank; string unquoted
			    <opts> format: string of characters, in order
			    <slotn> == 0 => line never erased

At beginning of file:
	d v <number> <view name>
	d c <number> <click name>
	d p <various pragmas here>
	d p e

 */

#include <stdio.h>
#include "anim.h"

#define	sendpoint(p)	{ sendint(p.x); sendint(p.y); }
#define	sendpair(x,y)	{ sendint(x); sendint(y); }
#define	eq(s, t)	(strcmp(s,t) == 0)
#define	FATAL	1

char	*cmdname;
char	*emalloc();

#ifdef BLIT
long	memsize	= 65000;	/* memory allocation */
#endif
#ifdef X11
long	memsize	= 200000;	/* X11 memory allocation */
#endif

int	dbg	= 0;
int	lineno	= 1;	/* ? */

char	*zload	= "";	/* will be -z for load and hang on blit */
char	*termld = TERMLD;	/* will be "32ld" or "dmdld" */
int	nterm	= 0;	/* number of bytes needed in terminal */

main(argc, argv)
	char *argv[];
{
	FILE *fp;
	char *term_process = ANIMTERM;	/* typically "./animterm" */
	char buf[100];
	int i, c, bootstat;

	cmdname = argv[0];
	while (argc > 1 && argv[1][0] == '-') {
		switch (argv[1][1]) {
		case 'd':
			dbg = !dbg;
			break;
		case 'z':
			zload = "-z";
			break;
		case 't':
			term_process = argv[2];
			argc--;
			argv++;
			break;
		case 'm':
			memsize = atoi(&argv[1][2]);
			if (memsize <= 0) {
				memsize = atoi(argv[2]);
				if (memsize <= 0)
					error(FATAL, "memory size must be positive.  use -mN");
				argc--;
				argv++;
			}
			break;
		}
		argc--;
		argv++;
	}
	if (argc <= 1)
		error(FATAL, "no input file");
	if ((fp = fopen(argv[1], "r")) == NULL)
		error(FATAL, "can't open %s\n", argv[1]);
	if (!dbg) {
		bootstat = bootterm(term_process);
		set_tty();
	}
	if (bootstat)
		send_data(fp);
	if (dbg) {
		printf("\nterminal bytes needed: %d\n", nterm);
		exit(0);
	}
	while ((c = readchar()) != P_QUIT) {
		if (c == P_FILE) {	/* rest of line is a filename */
			readstring(buf);
			if ((fp = fopen(buf, "r")) == NULL)
				send1char(P_ERROR);
			else {
				send1char(P_FILE);	/* ack */
				send_data(fp);
			}
		} else
			send1char(P_ERROR);
	}
	reset_tty();
}

send_data(fp)
	FILE *fp;
{
	char text[100], buf[100], opts[100], *p;
	int c, x1, y1, x2, y2, i, v, slot;
	int ntext;

	sendint(memsize);
	send1char(P_INIT);
	send1char(P_PRINT);
	sendstring("here comes the data!");
	while ((c = getc(fp)) != EOF) {
		switch (c) {
		case ' ':
		case '\t':
			break;
		case '\n':
			lineno++;	/* this should be the only increment */
			break;
		case '#':		/* comments */
			skipline(fp);
			break;
		case 'b':	/* blank.  ignore for now */
			skipline(fp);
			break;
		case 'c':		/* click */
			fscanf(fp, "%d", &i);
			send1char(P_OBJECT);
			send1char(c);
			sendint(i);
			skipline(fp);
			nterm += 3;
			break;
		case 'e':		/* erase */
			fscanf(fp, "%d", &i);
			send1char(P_OBJECT);
			send1char(c);
			sendint(i);
			skipline(fp);
			nterm += 6;
			break;
		case 'g':		/* geom:  draw line, box, circle, ... */
			fscanf(fp, "%d", &slot);
			switch (c = skipbl(fp)) {
			case 'l':
			case 'b':
				fscanf(fp, "%d %s %d %d %d %d", &v, opts, &x1, &y1, &x2, &y2);
				send1char(P_OBJECT);
				send1char(c);
				sendint(slot);
				sendint(v);
				sendopt(c=='b' ? boxops : lineops, opts);
				sendpair(x1, y1);
				sendpair(x2, y2);
				nterm += 12;
				break;
			case 'c':	/* circle */
				fscanf(fp, "%d %s %d %d %d", &v, opts, &x1, &y1, &x2);
				send1char(P_OBJECT);
				send1char('o');	/* 'o' is for circle */
				sendint(slot);
				sendint(v);
				sendopt(circops, opts);
				sendpair(x1, y1);
				sendint(x2);
				nterm += 10;
				break;
			case 't':	/* text */
				fscanf(fp, "%d %s %d %d", &v, opts, &x1, &y1);
				send1char(P_OBJECT);
				send1char('t');
				sendint(slot);
				sendint(v);
				sendopt(textops, opts);
				sendpair(x1, y1);
				getc(fp);	/* skip 1 separator; no quotes */
				for (p = text; (c = getc(fp)) != '\n'; )
					*p++ = c;
				*p = 0;
				ungetc('\n', fp);
				ntext = 1;
				/* slow... */
				if (eq(text, "bullet"))
					sendstring("*");
				else if (eq(text, "dot"))
					sendstring(".");
				else if (eq(text, "circle"))
					sendstring("o");
				else if (eq(text, "times"))
					sendstring("x");
				else {
					sendstring(text);
					ntext = strlen(text);
				}
				nterm += 10 + ntext;
				break;
			default:
				send1char(P_ERROR);
				flushproto();
				error(FATAL, "illegal g command: g %c", c);
				break;
			}
			break;
		case 'd':	/* definition of some sort */
			switch (c = skipbl(fp)) {
			case 'v':	/* view */
				fscanf(fp, "%d %s", &i, text);
				send1char(P_DEFINE);
				send1char('v');
				sendint(i);
				sendstring(text);
				skipline(fp);	/* might be a title there */
				break;
			case 'c':	/* click */
				fscanf(fp, "%d %s", &i, text);
				send1char(P_DEFINE);
				send1char('c');
				sendint(i);
				sendstring(text);
				break;
			case 'p':
				skipline(fp);
				break;
			default:
				send1char(P_ERROR);
				flushproto();
				error(FATAL, "unknown define command d %c", c);
				break;
			}
			break;
		default:
			send1char(P_ERROR);
			flushproto();
			error(FATAL, "unknown command %c", c);
			break;
		}
	}
	send1char(P_ENDFILE);
	fclose(fp);
	flushproto();
}

sendopt(optvals, opts)
	int optvals[];
	char *opts;
{
	int i, n;

	n = 0;
	for (i = 0; optvals[i] && opts; i += 2)
		if (*opts == optvals[i]) {
			n += optvals[i+1];
			opts++;
		}
	sendint(n);
}

skipbl(fp)
	FILE *fp;
{
	int c;

	while ((c = getc(fp)) == ' ' || c == '\t')
		;
	return c;
}

skipline(fp)
	FILE *fp;
{
	int c;

	while ((c = getc(fp)) != '\n')
		;
	ungetc('\n', fp);
}

error(f, s, a1, a2, a3, a4, a5, a6, a7)
{
	extern char *cmdname;
	extern int dbg;

	fprintf(stderr, "%s: ", cmdname);
	fprintf(stderr, s, a1, a2, a3, a4, a5, a6, a7);
	fprintf(stderr, "\n");
	if (lineno)
		fprintf(stderr, " source line number %d\n", lineno);
	if (f) {
		if (dbg)
			abort();
		exit(2);
	}
}

#ifndef X11

bootterm(procname)
	char *procname;
{
	char buf[100];

	sprintf(buf, "%s %s %s </dev/tty", termld, zload, procname);
	if (system(buf) != 0) {
		fprintf(stderr,
			"anim: can't create pipe to terminal process\n");
		return 0;
	}
	return 1;
}

#endif

#ifdef X11

bootterm(procname)
	char *procname;
{
	int afildes[2], bfildes[2], pid;
	if((pipe(afildes)==-1)||(pipe(bfildes)==-1)){
		fprintf(stderr,
			"anim: can't create pipe to terminal process\n");
		return 0;
	}
	if((pid=fork())==0){
		close(0);
		dup(afildes[0]);
		close(1);
		dup(bfildes[1]);
		execl(procname, "animterm", 0);
		exit(127);
	}
	if(pid==-1){
		fprintf(stderr,"anim: can't fork %s\n", procname);
		return 0;
	}
	close(0);
	dup(bfildes[0]);
	close(1);
	dup(afildes[1]);
	return 1;
}

#endif

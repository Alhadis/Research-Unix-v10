#include	<stdio.h>
#include	<signal.h>
#include	<sgtty.h>
#include 	"/usr/jerq/include/jioctl.h"
#include	"../comm.h"
#include	"host.h"

#ifdef	DEBUG
FILE *debug;
int verbose;
#endif	DEBUG

int zflag, jerq, going;
int vpos, hpos, curfont, cursize;
char devname[64];
struct sgttyb modes, savetty;
char *fload = "";

/* ARGSUSED */
main(argc, argv)
char *argv[];
{
	char c;
	int p, off, size;
	char name[128];
	int unproof = 0;
	int doinput = 0;
	int donothing = 0;
	/*
		analyse command line
	*/
	for(argv++; *argv && (**argv == '-'); argv++)
		switch(argv[0][1])
		{
		case 'z':
			zflag++;
			break;
		case 'u':
			unproof++;
			break;
		case 'f':
			fload = &argv[0][2];
			break;
		case '\0':
			doinput = 1;
			break;
#ifdef	DEBUG
		case 'd':
			debug = stdin;
			break;
		case 'v':
			verbose++;
			break;
#endif	DEBUG
		default:
			fprintf(stderr, "unknown option '%s' ignored!\n", *argv);
			break;
		}
#ifdef	DEBUG
	if(debug)
	{
		debug = fopen("../debug", "w");
		setbuf(debug, (char *)0);
	}
#endif	DEBUG
	if(*argv)
	{
		(void)close(0);
		if(open(*argv, 0) == -1)
		{
			perror(*argv);
			exit(1);
		}
	}
	else if(!doinput)
	{	/* user just typed proof */
		donothing = isatty(0);
	}
	if(unproof)
	{
		ioctl(jerq, JTERM, (struct sgttyb *)0);
	}
	else
	{
		for(c = 0; c <= 10; c++)
			loadfont(c, "??");
		for(off = 0; off < NPAGES; off++)
			input.pages[off].start = -1;
		initblit();
		if(fload) download(fload);
		if(!donothing)
		{
			send(NAK);
			readpage();
			going = 1;
			tsend(lastpage, 0);
			while(going) switch(c = term())	/* going can be reset in tsend !! */
			{
			case HEXIT:
				/*send(TEXIT);*/
				system("/usr/jerq/bin/term mux");
				going = 0;
				if(debug)fprintf(debug, "exit\n");
				break;
			case HQUIT:
				send(TTERM);
				going = 0;
				if(debug)fprintf(debug, "quit\n");
				break;
			case HSEEK:
				p = termn();
				off = termn();
				tsend(p, off);
				break;
			case HWANT:
				terms(name);
				size = term();
				c = term();
				tfont(name, size, c);
				break;
			case HCOMMENT:
				terms(name);
				if(debug)fprintf(debug, "## %s\n", name);
				break;
			}
		}
		ioctl(jerq, TIOCSETN, &savetty);
	}
}

download(s)
	char *s;
{
	int size;
	char *name, *ss;

	send(NAK);
	while(*s)
	{
		for(ss = s; *ss && (*ss != ','); ss++);
		if(*ss == ',') *ss++ = 0;
		for(name = s; *s && (*s != '.'); s++);
		*s++ = 0;
		if(sscanf(s, "%d", &size))
			tfont(name, size, 1);
		s = ss;
	}
	send(TTERM);
}

quit(s, ret)
	char *s;
{
	ioctl(jerq, TIOCSETN, &savetty);
	if(ret) jterm();
	if(s) fprintf(stderr, s);
	exit(ret);
}


char prooftty[]="/dev/tty";
char *ttyname();

initblit()
{
	char command[300];

	jerq = open(prooftty, 2);
	if(jerq == -1)
	{
		fprintf(stderr, "proof couldn't open /dev/tty\n");
		exit(1);
	}
	ioctl(jerq, TIOCGETP, &modes);
	savetty = modes;
	modes.sg_flags |= RAW;
	modes.sg_flags &= ~ECHO;
	ioctl(jerq, TIOCSETN, &modes);
	if(!isatty(1) || !verify())
	{
		sprintf(command, "%s %s %s <%s >%s ; stty old!", JLD, zflag?"-z":"",
			JERQM, prooftty, prooftty);
		if(system(command) != 0)
			quit("JLD failed", 1);
	}
}

alarmcatch(){
}

verify()
{
	register rval;
	char c;

	(void)signal(SIGALRM, alarmcatch);
	c=REQ;
	write(jerq, &c, 1);
	(void)alarm(3);
	(void)read(jerq, &c, 1);	/* if we alarm c is still a REQ (ACK) */
	(void)alarm(0);
	rval = (c == NAK);
#ifdef	DEBUG
	if(debug)
		fprintf(debug, "verify: read 0%o\n", c);
#endif	DEBUG

	return rval;
}

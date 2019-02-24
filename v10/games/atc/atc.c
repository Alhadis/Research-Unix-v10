/* atc: simulate an air traffic controller at work */

#define NRAND 1
#define die nrand

#include <stdio.h>
#include <sys/ttyio.h>
#include <sys/param.h>
#include <sys/timeb.h>
#include <ctype.h>
#include <signal.h>
#include <pwd.h>
#include "ahdr.h"

#define SYNTAX "Usage: atc [-s=<seed>] [-a=<airspace>] [-t=<time>]\n"

/* size of terminal capability buffer */
#define TCSIZE 1024

int cmdx; /* command located using same cursor positioning as screen */
int cmdy; /* usually will be at 24 lines down */

char screen[MAXWIDTH][MAXHEIGHT]; /* contents of screen when no planes */

char *flowfile = 0;             /* no default flow patterns */
char *airfile = AIRFILE;
char *airspace = {"Apple1"};    /* default airspace is as on Apple */
char *protofile = 0;            /* no default protocol file */
FILE *proto;                    /* stays open all the time if protocol */
char *moviefile = 0;            /* if in movie mode, here's the file */
FILE *movie;                    /* and the file descriptor */
int nextmovie;                  /* next time for a moviefile command */
char *mcommand;                 /* next movie command */
int width,height; /* these values set in the airspace file */
char *bigname = 0;              /* megastatistics file */
FILE *bigstatsfile;             /* megastatistics stream */

struct flow fpath[MAXPATHS];
int maxflow;
int npaths;

int nentry = 0; /* number of entries into the board */
struct pstruct entry[EMAX];

int nairport = 0;
struct pstruct airport[AMAX];

int nnavaid = 0;
struct pstruct navaid[NMAX];

int nplanes = PMAX;
struct astruct plane[PMAX];

int initseed,seed,game_time,initgame_time,last_update;
time_t start_time;      /* real time in seconds that game started */

time_t get_time();
struct passwd *getpwuid();
FILE *popen();

#define BUFSIZE 80      /* buffer for message */

int remain;

int bound = 0;          /* number of boundary errors */
int system = 0;         /* system errors */
int crash = 0;          /* crashed - out of fuel */
int fuel = 0;           /* out of fuel on runway - tow truck */
int goaround = 0;       /* missed an approach */
int icmds = 0;          /* number of cmds issued (including deferred) */
int cmds = 0;           /* number of cmds actually executed */

int fuelused = 0;

int inaptot=0,outaptot=0;   /* denominators for random entries/exits */
int inawtot=0,outawtot=0;   /* for airports and airways */

char tcbuf[TCSIZE];
char cmbuf[100];	/* buffer for cursor motion strings */
char clbuf[100];	/* clear screen sequence */

int rubout = 0;

catchint(n)
	int n;
{
	rubout = 1;
	signal (n, catchint);
}

struct sgttyb tty_setting;
short tty_flags;

main(argc,argv)
int argc;
char **argv;
{       register char *arg;

	char *ttype;
	char *getenv();

	ttype = getenv("TERM");
	if (ttype == NULL || *ttype == '\0' || tgetent (tcbuf, ttype) <= 0) {
		fprintf (stderr, "cannot determine terminal type\n");
		exit (1);
	}

	/* get cursor motion and clear screen code into cmbuf and clbuf */
	{
		char *p;
		p = cmbuf;
		tgetstr ("cm", &p);
		p = clbuf;
		tgetstr ("cl", &p);
	}

	ioctl(0, TIOCGETP, &tty_setting);
	tty_flags = tty_setting.sg_flags;
	seed = 0;
	for (arg = *++argv; --argc; arg = *++argv) /* read args 1 at a time */
	{   if (arg[0] == '-')
		switch(arg[1])
		{   case 's':
			if (arg[2] != '=') err(SYNTAX);
			seed = atoi(&arg[3]);
			break;
		    case 'S':
			if (arg[2] != '=') err(SYNTAX);
			bigname = &arg[3];
			break;
		    case 'a':
			if (arg[2] != '=') err(SYNTAX);
			airspace = &arg[3];
			break;
		    case 'u':
			if (arg[2] != '=') err(SYNTAX);
			airfile = &arg[3];
			break;
		    case 't':
			if (arg[2] != '=') err(SYNTAX);
			initgame_time = game_time = 59 + 60 * atoi(&arg[3]);
			break;
		    case 'f':
			if (arg[2] != '=') err(SYNTAX);
			flowfile = &arg[3];
			break;
		    case 'p':   /* run a protocol */
			if (arg[2] != '=') err(SYNTAX);
			protofile = &arg[3];
			break;
		    case 'm':   /* movie mode */
			if (arg[2] != '=') err(SYNTAX);
			moviefile = &arg[3];
			break;
		    default :
			err("Bad arg: %s\n",arg);
		}
	}
	init(); /* set up flight plans, initialize screen */
	while (command());	/* get commands from user */
	done(1);
	exit(0);
}

#define CMDSIZE 40

int inter;      /* non-zero if an alarm went off */
intp()  /* process interrupt from alarm */
{       inter++;
}


command()
{       time_t now;
	char cmd[CMDSIZE];
	char *c,utype,*d;
	int i,j;

	for (utype = i = *(c=cmd) = 0; utype != RETURN; )
	{
		if (moviefile) game_time--;     /* 1 sec at a time? */
		else
		{       now = get_time();
			game_time = initgame_time - (now - start_time);
		}
		if (remain == 0) return(0);
		if (game_time < 0)
		{       report(OUTOFTIME,0,0);
			return(0);
		}
		if (i == 0)     /* preserve partial command */
		{       cursor(cmdx,cmdy);
			printf("<%.2d> COMMAND:                  ",game_time/60);
		}
		if (last_update - game_time >= UPDATE)
		{       update();
			if (c != cmd) {BEEP;} /* cmd may be obsolete */
		}
		while (moviefile && game_time <= nextmovie)
		{       if (bigname) pcmd(bigstatsfile,mcommand);
			if (protofile) pcmd(proto,mcommand);
			i = parse(mcommand);
			if (!i) return i;
			getmcmd();
		}
		if (moviefile) continue;        /* no longer accept inputs */
		cursor(cmdx+7,cmdy);
		for (j=0; j<i; j++) putchar(cmd[j]);
		signal(SIGALRM,intp);/* catch the alarm if it goes off */
		inter = 0;      /* set to >0 if the alarm goes off */
		if (rubout) return 0;	/* user really wants to quit */
		alarm(TIMEOUT); /* read for TIMEOUT sec unless he types */
		utype = getchar() & 0177;
		alarm(0);
		if (inter > 0) continue; /* didn't type anything */
		delmsg(cmdx,cmdy-1);    /* erase previous echo if he types */
		cursor(cmdx+7,cmdy);
		if (i != 2 || (*(c-1) != '*' && *(c-1) != ':'))
		 if (*cmd != '!' || i == 0) /* don't shift CDL filename */
		  if (*cmd != '@'|| i == 0) /* don't shift location descrip */
		      if (utype >= 'a' && utype <= 'z')
			utype = utype - 'a' + 'A';
		if (utype == INSMODE) utype = RETURN;
		if ((*c++ = utype) != RETURN)
		{       if (++i >= CMDSIZE) c--;
			else
			switch(utype)   /* echo something reasonable */
			{ 
#ifdef ANNARBOR
			    case MIPAGE:/* check cursor controls */
				putchar(*--c = 'N');
				putchar(*++c = 'W');
				c++; i++;
				break;
			    case HOME:
				putchar(*--c = 'N');
				c++;
				break;
			    case PLPAGE:
				putchar(*--c = 'N');
				putchar(*++c = 'E');
				c++; i++;
				break;
			    case MISRCH:
				putchar(*--c = 'W');
				c++;
				break;
			    case UPARROW:
				putchar(*--c = '0');
				c++;
				break;
			    case PLSRCH:
				putchar(*--c = 'E');
				c++;
				break;
			    case LEFTARROW:
				putchar(*--c = 'S');
				putchar(*++c = 'W');
				c++; i++;
				break;
			    case DOWNARROW:
				putchar(*--c = 'S');
				c++;
				break;
			    case RIGHTARROW:
				putchar(*--c = 'S');
				putchar(*++c = 'E');
				c++; i++;
				break;
			    case ':':   /* like a * */
				putchar('*');
				break;
#endif
			    case BS:
				if (i<=0) break;
				if (i==1) { i = 0; break; }
				putchar(BS);
				c--; c--; i--; i--;
				break;
			    default: putchar(utype); break;
			}
		}
	}
	*c = 0;
	if (bigname) pcmd(bigstatsfile,cmd);
	if (protofile) pcmd(proto,cmd);
	return(parse(cmd));
}

pcmd(file,cmd)  /* protocol a command */
FILE *file;
char *cmd;
{       register char *s;

	if (file == proto)
	     fprintf(file,"%5d: ",game_time);
	else fprintf(file,"%5d: ",initgame_time - game_time);
	for (s = cmd; *s && *s != RETURN; s++) fputc(*s,file);
	fputc('\n',file);
}

parse(cmd)      /* command is terminated by RETURN */
register char *cmd;
{       time_t now;
	register int i;
	register struct astruct *pl;
	char dest;
	char *fullcmd;
	char ubuf[10];

	fullcmd = cmd;
	switch(*cmd)        /* parse 1 letter at a time */
	{   case ' ':  /* time advance */
		sprintf (ubuf, "%d", UPDATE);
		echo("%s\" - ROGER",ubuf);
		now = get_time();
		start_time = last_update - UPDATE - 1 - initgame_time + now;
		break;
	    case '$':
		return(0);
	    case '!':   /* read a file of clearance directive lists */
		readcdl(++cmd);
		break;
	    case '@':   /* get a monitoring command */
		getmon(++cmd);
		break;
	    default:    /* should be airplane id */
		/* ASSUMPTION HERE: aircraft are A-Z */
		if (*cmd < 'A' || *cmd > 'Z')
		{       echo("%s - UNABLE",fullcmd);
			BEEP;
			break;
		}
		for (pl = &plane[i=0]; i < nplanes; i++,pl++)
			if (pl->a_id == *cmd) break;
		if (i == nplanes)
		{       echo("%s - NOT ACTIVE",fullcmd);
			BEEP;
			break;
		}

		if (cmd[1] == '?')      /* o.k. to cancel plans */
		{       if (pl->a_active != ACTIVE &&
			    pl->a_active != WAITING)
			{       echo("%s - NOT ACTIVE",fullcmd);
				BEEP;
				break;
			}
		}
		else if (cmd[1] == RETURN)     /* info request only */
		{       if (pl->a_active != ACTIVE &&
			    pl->a_active != APPROACHING &&
			    pl->a_active != WAITING)
			{       echo("%s - NOT ACTIVE",fullcmd);
				BEEP;
				break;
			}
		}
		else
		{       if (pl->a_active != ACTIVE && pl->a_active != WAITING)
			{       echo("%s - NOT ACTIVE",fullcmd);
				BEEP;
				break;
			}
			if (pl->a_active==ACTIVE && pl->a_nextz == 0 && cmd[1]!=RETURN)
			{       echo("%s - LANDING",fullcmd);
				BEEP;
				break;
			}
		}
		switch(*++cmd)  /* parse second char */
		{   case RETURN:  /* info request */
			strip(pl,cmdy+1,VERBOSE);
			break;
		    case 'A':   /* change altitude */
			if (*++cmd < '0' || *cmd > '5')
			{       echo("%s - UNABLE",fullcmd);
				BEEP;
			}
			else
			{       pl->a_nextz = *cmd - '0';
				echo("%s - ROGER",fullcmd);
				cmds++;
				icmds++;
			}
			break;
		    case 'H':   /* hold at beacon */
			pl->a_hold = 1;
			pl->a_clear = 0;
			echo("%s - ROGER",fullcmd);
			cmds++;
			icmds++;
			break;
		    case 'L':   /* left turn */
			rdturn(pl,'L',++cmd,fullcmd);
			break;
		    case 'R':   /* right turn */
			rdturn(pl,'R',++cmd,fullcmd);
			break;
		    case 'T':   /* shortest turn */
			rdturn(pl,'T',++cmd,fullcmd);
			break;
		    case '*':   /* clearance at navaid */
		    case ':':
			dest = *++cmd;
			for (i=0; i<nentry; i++) /* vector to entry? */
				if (dest == entry[i].p_sym) break;
			if (pl->a_hold == 1) pl->a_hold = 0;
			if (i<nentry)
			{       echo("%s - ROGER",fullcmd);
				pl->a_clear = dest;
				cmds++;
				icmds++;
				break;
			}
			for (i=0; i<nairport; i++)
				if (dest == airport[i].p_sym) break;
			if (i<nairport)
			{       echo("%s - ROGER",fullcmd);
				pl->a_clear = dest;
				cmds++;
				icmds++;
				break;
			}
			echo("%s - UNABLE",fullcmd);
			BEEP;
			break;
		    case '?':   /* delete delayed commands */
			ddelay(pl);
			echo("%s - ROGER",fullcmd);
			cmds++;
			icmds++;
			break;
		    default:    /* airport id? - maybe cursor motions? */
			echo("%s - UNABLE",fullcmd);
			BEEP;
			break;
		}
	}
	return(1);
}

getmon(cmd)     /* get monitor command from command line */
register char *cmd;
{       int x,y;
	register char *start;

	start = cmd;
	if (getloc(cmd,&x,&y) == 0)     /* where did you say? */
	{       echo("%s - BAD LOC SPEC",cmd);
		BEEP;
		return;
	}
	while (*cmd)    /* pick up each command that happens here */
	{       while (*cmd && *cmd != ',') cmd++;
		if (*cmd == 0) return; /* read last command */
		putlist(start,x,y,++cmd); /* put the next command on a list */
	}
}

ddelay(pl)      /* delete any delayed commands from this plane's list */
struct astruct *pl;
{       struct list *l,*next;

	if (pl->a_plan == 0) return;
	for (l = pl->a_plan; l; l = next)
	{       next = l->l_next;
		free(l);
	}
	pl->a_plan = 0;
}

putlist(start,x,y,cmd) /* put some commands on the list for a plane */
char *start;
int x,y;
char *cmd;
{       register struct list *l;
	register struct astruct *p;
	register char *c,*d;
	int i;

	d = cmd;
	for (i = 0; cmd[i] != ',' && cmd[i] != 0 && cmd[i] != RETURN; i++)
	  if (i != 2 || (cmd[i-1] != '*' && cmd[i-1] != ':'))
	    if (cmd[i] >= 'a' && cmd[i] <= 'z') cmd[i] += 'A' - 'a';
	for (p = &plane[i = 0]; i < nplanes; i++,p++)
		if (p->a_id == *cmd) break;
	if (i == nplanes)
	{       echo("BAD PLANE SPEC: %s",cmd);
		BEEP;
		return;
	}
	l = p->a_plan;  /* go to the CDL for this plane */
	if (p->a_plan == 0)
		l = p->a_plan = (struct list *) malloc(sizeof (struct list));
	else
	{       for (l = p->a_plan; l->l_next != 0; l = l->l_next); /* eol */
		l->l_next = (struct list *) malloc(sizeof (struct list));
		l = l->l_next;
	}
	l->l_next = 0;
	l->l_x = x;
	l->l_y = y;
	c = l->l_cmd;
	while (*cmd != ',' && *cmd != 0 && *cmd != RETURN)
		*c++ = *cmd++;
	*c++ = RETURN;
	*c = 0;
	for (c = d; *c; c++) if (*c == RETURN) *c = 0; /* return damages dspla*/
	echo("%s - WILCO",d);
	icmds++;
	/* copy the verbatim direction used by the guy */
	for (c = start, d = l->l_loc; *c != ','; *d++ = *c++);
	*d = 0;
}


getloc(cmd,px,py)       /* get location description from a monitor command */
char *cmd;
int *px,*py;
{       register char l,*m;     /* location */
	register int i;
	register struct pstruct *p;
	int dx,dy;
	char buf[300];

	l = *(m = cmd);
	p = 0;
	for (i = 0; i < nentry; i++)
		if (entry[i].p_sym == l) p = &entry[i];
	for (i = 0; i < nairport; i++)
		if (airport[i].p_sym == l) p = &airport[i];
	for (i = 0; i < nnavaid; i++)
		if (navaid[i].p_sym == l) p = &navaid[i];
	if (p == 0) return 0;
	*px = p->p_x;
	*py = p->p_y;
	m++;
	while (*m != ',')     /* read through the other directions */
	{       dx = dy = 0;
		switch(*m)      /* NSEW */
		{   case 'n': dy = -1; break;
		    case 's': dy =  1; break;
		    case 'e': dx =  1; break;
		    case 'w': dx = -1; break;
		    default:
			sprintf(buf,"Got %c instead of n,s,e,w",*m);
			msg(cmdx,cmdy+3,buf);
			return 0;
		}
		if      (*++m == 'w') { dx = -1; m++;}  /* NW or SW */
		else if (*m   == 'e') { dx =  1; m++;}  /* NE or SE */
		if (*m < '0' || *m > '9')
		{       sprintf(buf,"Got %c instead of digit",*m);
			msg(cmdx,cmdy+3,buf);
			return 0;
		}
		for (i = 0; *m >= '0' && *m <= '9'; m++)
			i = 10 * i + *m - '0';
		*px += i * dx;
		*py += i * dy;
	}
	return 1;
}

readcdl(fname)  /* fname points at CDL filename, \r-terminated */
char *fname;
{       FILE *cdlfile;
	char *p;

	while (*fname == ' ') fname++;  /* skip initial blanks */
	for (p = fname; *p && *p != RETURN; p++); /* remove newline */
	*p = 0;
	if ((cdlfile = fopen(fname,"r")) == NULL)
	{       echo("Cannot open %s",fname);
		return;
	}
	echo("Reading CDL file...",0);
	fclose(cdlfile);
}


rdturn(pl,dir,cmd,fullcmd)
struct astruct *pl;
char dir;
char *cmd,*fullcmd;
{       int i,ok,newx,newy,turncount;

	if (pl->a_nextz == 0)
	{       echo("%s - WRONG WAY",fullcmd);
		BEEP;
		return;
	}
	ok = 1; /* check whether this is a legal command */
	newx = newy = 0;
	for (i=0; i<2; i++)     /* look at next 2 chars */
	switch(cmd[i])
	{   case RETURN:
		if (i == 0)
		{       echo("%s - WHICH WAY?",fullcmd);
			BEEP;
			return;
		}
		break;
	    case '0':
		if (i == 1)     /* can't be second char */
		{       ok = 0;
			break;
		}
		pl->a_dxnew = 0;
		pl->a_dynew = 0;
		pl->a_turn = 0;
		pl->a_clear = 0;
		pl->a_hold = 0;
		echo("%s - ROGER",fullcmd);
		cmds++;
		icmds++;
		return;
	    case 'N':
		newy = -1;
		break;
	    case 'S':
		newy = 1;
		break;
	    case 'E':
		newx = 1;
		break;
	    case 'W':
		newx = -1;
		break;
	    default:    /* numbers (e.g.) no longer acceptable */
		ok = 0;
		break;
	}
	if (ok)
	{       echo("%s - ROGER",fullcmd);
		cmds++;
		icmds++;
		pl->a_turn = dir;       /* will replace 'T' with 'L' or 'R' */
		pl->a_dxnew = newx; pl->a_dynew = newy;
		pl->a_hold = pl->a_clear = 0;
		if (dir == 'T')
		{       newx = pl->a_dx; /* start with this direction */
			newy = pl->a_dy; /* and turn until done */
			for (turncount = 1; turncount < 9; turncount++)
			    if (turn(&newx,&newy,'L',pl->a_dxnew,pl->a_dynew))
				break;
			if (turncount <= 4) pl->a_turn = 'L';
			else pl->a_turn = 'R';
		}
	}
	else
	{       echo("%s - UNABLE",fullcmd);
		BEEP;
	}
}

echo(format,cmd)
char *format,*cmd;
{       char *c;
	char mbuf[BUFSIZE];

	for (c=cmd; *c; c++) if (*c == RETURN) *c = 0;
	sprintf(mbuf,format,cmd);
	msg(cmdx,cmdy-1,mbuf);
}

int parity = 0; /* props move every other update */

update()
{       parity = 1 - parity;
	last_update -= UPDATE;  /* must average out to every UPDATE secs */
	moveplanes();
	printplans();
	if (bigname) activity();
}

activity()      /* number of active planes to megastats file */
{       register int i,active,waiting,approaching,planned;
	struct astruct *p;

	for (i=active=waiting=approaching=planned=0; i < nplanes; i++)
	{       p = &plane[i];
		switch(p->a_active)
		{   case ACTIVE:
			active++;
			break;
		    case WAITING:
			waiting++;
			break;
		    case APPROACHING:
			approaching++;
			break;
		}
		if (p->a_plan && p->a_active != DONE) planned++;
	}
	fprintf(bigstatsfile,"%5d: %d active, %d approaching, %d waiting.  ",
		initgame_time - game_time, active,approaching,waiting);
	fprintf(bigstatsfile,"%d planned.\n",planned);
	for (i = 0; i < nplanes; i++)
	{       int a;

		p = &plane[i];
		a = p->a_active;
		if (a == ACTIVE || a == APPROACHING || a == WAITING)
		{       p->a_load += active;
			p->a_ticks++;
			p->a_planned += planned;
		}
		if (a == APPROACHING || a == WAITING)
		{       p->a_aprev += active;
			p->a_prev++;
			p->a_pprev += planned;
			p->a_pplanned = (p->a_plan? YES : NO);
		}
	}
}

moveplanes()
{       register struct astruct *pl;
	register int i,j;
	register int nextx,nexty;
	int ax,ay,oldx,oldy;
	struct pstruct *dst;

	for (pl = &plane[i = 0]; i < PMAX; i++, pl++)
	{       if (parity && pl->a_type == PROP)
		{       pl->a_lastz = pl->a_z;
			if (pl->a_active == LANDING) pl->a_active = DONE;
			continue;
		}
		switch(pl->a_active)
		{   case INACTIVE:
			if (initgame_time - game_time + FPTHRESH>= pl->a_stime
			    && !pl->a_sair)
				pl->a_active = APPROACHING;

		    case APPROACHING:
			if (initgame_time - game_time>= pl->a_stime)
			{       pl->a_active = ACTIVE;
				if (pl->a_sair) pl->a_active = WAITING;
				nextx = pl->a_x;
				nexty = pl->a_y;
			}
			break;

		    case LANDING: /* to allow for conflicts */
			pl->a_active = DONE;
			break;

		    case ACTIVE:
			pl->a_dist++;   /* increment distance traveled */
			nextx = pl->a_x + pl->a_dx;
			nexty = pl->a_y + pl->a_dy;
			if (nextx < 0 || nexty < 0 ||
			    nextx >= width || nexty >= height)
			{       leavescreen(pl);
				break;
			}

			/* turning? */
			if (pl->a_turn)   /* turn 1 tick in right direction */
			    if (turn(&pl->a_dx,&pl->a_dy,
				pl->a_turn,pl->a_dxnew,pl->a_dynew))
				  pl->a_turn = pl->a_dxnew = pl->a_dynew = 0;


			if (pl->a_dair) /* landing? */
			{       dst = &airport[pl->a_dest];
				ax = dst->p_x;
				ay = dst->p_y;
				if (ax == nextx && ay == nexty)
				    if (pl->a_z == 0 &&
					  pl->a_dx == dst->p_dx &&
					  pl->a_dy == dst->p_dy)
				    {   pl->a_active = LANDING;  /* landed */
					erase(pl->a_x,pl->a_y);
					pl->a_x = nextx;
					pl->a_y = nexty;
					remain--;
				    }
				    else if (pl->a_nextz == 0) /* flyby */
				    {   pl->a_nextz = 1; /* down too late*/
					goaround++;
					report(GOAROUND,pl);
				    }
			}

			/* changing altitude? */
			if (pl->a_z > pl->a_nextz)
				pl->a_lastz = pl->a_z--;
			else if (pl->a_z < pl->a_nextz)
				pl->a_lastz = pl->a_z++;
			else pl->a_lastz = pl->a_z;

			/* out of fuel? */
			if (--pl->a_fuel <= 0)
			{       crash++;
				report(CRASH,pl);
				pl->a_active = DONE;
				remain--;
				erase(pl->a_x,pl->a_y);
			}
			if (pl->a_type == JET) fuelused += 5;
			else fuelused += 1;
			break;

		    case WAITING:  /* waiting for takeoff */
			if (--pl->a_fuel <= 0)  /* out of fuel on ground? */
			{       fuel++;
				if (bigname) freport(pl);
				report(FUEL,pl);
				pl->a_active = DONE;
				remain--;
			}
			if (pl->a_nextz != 0)
			{       pl->a_z++;      /* take off */
				pl->a_active = ACTIVE;
				pl->a_dist++;
			}
			nextx = pl->a_x + pl->a_dx;
			nexty = pl->a_y + pl->a_dy;

			/* turning? */
			if (pl->a_turn)   /* turn 1 tick in right direction */
			    if (turn(&pl->a_dx,&pl->a_dy,
				pl->a_turn,pl->a_dxnew,pl->a_dynew))
				  pl->a_turn = pl->a_dxnew = pl->a_dynew = 0;

			if (pl->a_type == JET) fuelused += 5;
			else fuelused += 1;
			break;

		    case DONE:  /* off the screen */
			break;
		}
		if (pl->a_active != ACTIVE) continue;
		oldx = pl->a_x;
		oldy = pl->a_y;
		pl->a_x = nextx;
		pl->a_y = nexty;
		cursor(nextx,nexty);
		printf("%c%c",pl->a_id,(pl->a_z>9? '^' : pl->a_z+'0'));
		erase(oldx,oldy);
		if (pl->a_hold || pl->a_clear)
			for (j = 0; j < nnavaid; j++)   /* at a navaid? */
				if (screen[nextx][nexty] == navaid[j].p_sym)
				{       donavaid(pl);   /* right navaid? */
					break;
				}
	}
	conflict();
	delays();       /* execute any applicable delayed commands */
}

freport(pl)     /* report to big stats file about tow truck */
struct astruct *pl;
{
	fprintf(bigstatsfile,"%5d: %c out of fuel on runway.\n",
		initgame_time - game_time,pl->a_id);
}

delays()        /* execute any applicable delayed actions */
{       register struct astruct *p;
	register int i;
	register struct list *l,*last;

	/* for each plane, if it is active and if it has a command pending,
	 * and if it is at the right place for the command, then execute it.
	 */
	for (i = 0; i < PMAX; i++)
	{       p = &plane[i];
		if (p->a_active != ACTIVE) continue;
		if (p->a_plan == 0) continue;
		last = 0;
		for (l = p->a_plan; l; )
		{       if (p->a_x != l->l_x || p->a_y != l->l_y)
			{
				last = l;
				l = l->l_next;
			}
			else    /* it matched */
			{
				parse(l->l_cmd); /* execute the command */

				/* now delete the command from the list */
				if (last == 0)  /* it was the first one */
				{       p->a_plan = l->l_next;
					free(l);
					l = p->a_plan;
					/* last stays at 0 */
				}
				else
				{       last->l_next = l->l_next;
					free(l);
					l = last->l_next;
					/* last stays where it was */
				}
			}
		}
	}
}

donavaid(pl)    /* take appropriate action at navaid */
struct astruct *pl; /* the plane has either been vectored or told to hold */
{       register struct pstruct *d;
	register int i;

	if (pl->a_hold)
	{       pl->a_hold = 2; /* will be holding now */
		pl->a_turn = 'L'; /* and turning continuously to the left */
		pl->a_dxnew = pl->a_dx; /* until it reaches this heading agn*/
		pl->a_dynew = pl->a_dy;
	}
	if (pl->a_clear)
	{       pl->a_turn = pl->a_dxnew = pl->a_dynew = pl->a_hold = 0;
		/* see what we're vectoring to */
		for (d = &entry[i=0]; i < nentry; i++,d++)
			if (d->p_sym == pl->a_clear) break;
		if (i != nentry)        /* yes, it was a navaid */
		{       pl->a_dx = -d->p_dx;    /* opposite of entry heading*/
			pl->a_dy = -d->p_dy;
			pl->a_clear = 0;
			return;
		}
		for (d = &airport[i=0]; i < nairport; i++,d++)
			if (d->p_sym == pl->a_clear) break;
		if (i == nairport) return; /* program bug - cleared to ?? */
		pl->a_dx = d->p_dx; /* assume landing heading for airport */
		pl->a_dy = d->p_dy;
		pl->a_clear = 0;

		if (pl->a_hold == 1)
			pl->a_hold = pl->a_dxnew = pl->a_dynew = 0;
		else if (pl->a_hold == 0)
			pl->a_dxnew = pl->a_dynew = 0;
		return;
	}
}

turn(newx,newy,direc,dx,dy)     /* messy - enumeration of all cases */
int *newx,*newy;        /* returned values of new heading */
char direc;
int dx,dy;
{       int x,y;

	x = *newx;
	y = *newy;
	switch(direc)
	{   case 'L': /* cases would be more efficient, but this is clearer*/
		if (x== -1 && y== -1) {*newx= -1; *newy=  0; break;}
		if (x== -1 && y==  0) {*newx= -1; *newy=  1; break;}
		if (x== -1 && y==  1) {*newx=  0; *newy=  1; break;}
		if (x==  0 && y== -1) {*newx= -1; *newy= -1; break;}
		if (x==  0 && y==  1) {*newx=  1; *newy=  1; break;}
		if (x==  1 && y== -1) {*newx=  0; *newy= -1; break;}
		if (x==  1 && y==  0) {*newx=  1; *newy= -1; break;}
		if (x==  1 && y==  1) {*newx=  1; *newy=  0; break;}
		break;
	    case 'R':
		if (x== -1 && y== -1) {*newx=  0; *newy= -1; break;}
		if (x== -1 && y==  0) {*newx= -1; *newy= -1; break;}
		if (x== -1 && y==  1) {*newx= -1; *newy=  0; break;}
		if (x==  0 && y== -1) {*newx=  1; *newy= -1; break;}
		if (x==  0 && y==  1) {*newx= -1; *newy=  1; break;}
		if (x==  1 && y== -1) {*newx=  1; *newy=  0; break;}
		if (x==  1 && y==  0) {*newx=  1; *newy=  1; break;}
		if (x==  1 && y==  1) {*newx=  0; *newy=  1; break;}
		break;
	}
	if (*newx == dx && *newy == dy) return(1);      /* finished turn */
	else return(0);
}


report(error,plane1,plane2)
int error;
struct astruct *plane1,*plane2;
{       char mbuf[BUFSIZE];
	int i;

	putchar(BELL);
	switch(error)
	{   case FUEL:
		sprintf(mbuf,"Tow truck called for %c (%c)",
			plane1->a_id,airport[plane1->a_start].p_sym);
		break;
	    case CRASH:
		sprintf(mbuf,"PLANE %c OUT OF GAS",plane1->a_id);
		break;
	    case BOUND:
		sprintf(mbuf,"Boundary error - %c",plane1->a_id);
		break;
	    case SYSTEM:
		sprintf(mbuf,"%c AND %c: SYSTEM ERROR",
			plane1->a_id, plane2->a_id);
		break;
	    case GOAROUND:
		sprintf(mbuf,"%c missed approach",plane1->a_id);
		break;
	    case OUTOFTIME:
		sprintf(mbuf,"Out of time: %d remain",remain);
		break;
	}
	msg(cmdx,cmdy+2,mbuf);
}



leavescreen(pl)
struct astruct *pl;
{       struct pstruct *e;

	pl->a_active = DONE;
	pl->a_dist++;
	erase(pl->a_x,pl->a_y);
	remain--;
	e = &entry[pl->a_dest];
	if (pl->a_z == 5 && !pl->a_dair && /* leaving at non-airport, 5K' */
	    screen[pl->a_x][pl->a_y] == e->p_sym &&  /* right exit */
	    pl->a_dx == -e->p_dx && pl->a_dy == -e->p_dy && /* right direc */
	    pl->a_turn == 0)    /* and not turning */
		return(0);
	bound++;
	if (bigname)
	{       fprintf(bigstatsfile,"%5d: Boundary error - plane %c ",
			initgame_time - game_time,pl->a_id);
		fprintf(bigstatsfile,"left at %d000' ",pl->a_z);
		if (pl->a_nextz > pl->a_z)
		   fprintf(bigstatsfile,"climbing to %d000'",pl->a_nextz);
		if (pl->a_nextz < pl->a_z)
		   fprintf(bigstatsfile,"descending to %d000'",pl->a_nextz);
		fprintf(bigstatsfile,
			"\n\tPosition: (%d,%d)    Bearing: ",pl->a_x,pl->a_y);
		if (pl->a_dy < 0) fprintf(bigstatsfile,"N");
		else if (pl->a_dy > 0) fprintf(bigstatsfile,"S");
		if (pl->a_dx > 0) fprintf(bigstatsfile,"E");
		else if (pl->a_dx < 0) fprintf(bigstatsfile,"W");
		if (pl->a_turn)
		{       fprintf(bigstatsfile," turning %c to ",pl->a_turn);
			if (pl->a_dynew < 0) fprintf(bigstatsfile,"N");
			else if (pl->a_dynew > 0) fprintf(bigstatsfile,"S");
			if (pl->a_dxnew > 0) fprintf(bigstatsfile,"E");
			else if (pl->a_dxnew < 0) fprintf(bigstatsfile,"W");
		}
		fprintf(bigstatsfile,".\n");
	}
	report(BOUND,pl);
	return(0);
}

conflict()      /* check each pair of planes for problems */
{       register struct astruct *q,*p;
	register int i,j,k;
	register int miss;

	for (p = &plane[k=1]; k < nplanes; k++,p++)
	{       if (p->a_active != ACTIVE && p->a_active != LANDING) continue;
		for (q = &plane[i=0]; i < k; i++,q++)
		{       if (q->a_active != ACTIVE) continue;
			if (q->a_z != p->a_z && q->a_z != p->a_lastz &&
			    q->a_lastz != p->a_z && q->a_lastz != p->a_lastz)
				continue;
			miss = abs(q->a_x - p->a_x);
			j = abs(q->a_y - p->a_y);
			if (j>miss) miss=j;
			if (miss < 3)   /* too close - system error */
			{       system++;
				if (bigname) bigsyserr(p,q);
				report(SYSTEM,p,q);
				break;
			}
		}
	}
}

bigsyserr(p,q)  /* spew sys err info into the big stats file */
struct astruct *p,*q;
{
	fprintf(bigstatsfile,
		"%5d: System error between %c (%d,%d,%d000) and %c (%d,%d,%d000)\n",
		initgame_time - game_time,p->a_id,p->a_x,p->a_y,p->a_z,
		q->a_id,q->a_x,q->a_y,q->a_z);
}

erase(x,y)
int x,y;
{       register struct astruct *p;
	register int i;

	if (x<0 || y < 0) return;
	if (x>=width || y>=height) return;
	cursor(x,y);
	printf("%c ",screen[x][y]);
	for (p = &plane[i=0]; i < nplanes; i++,p++)
		if (p->a_active == ACTIVE && x == p->a_x && y == p->a_y)
		{       cursor(x,y);
			printf("%c%c",p->a_id,(p->a_z>9?'^':p->a_z+'0'));
		}
}


int msglen[MAXHEIGHT];  /* length of message string on this row */

init()
{       int i,j;
	char c,mbuf[BUFSIZE];
	int pcompare();
	FILE *statfile;
	struct passwd *pw;
	time_t now;
	char *p,*ct;
	extern char *ctime();
	int nearlies;

	if ((statfile = fopen(STATSFILE,"a")) != NULL)
	{       pw = getpwuid(getuid());
		now = get_time();
		ct = ctime(&now);
		for (p= &ct[4]; p <= &ct[16]; p++) fputc(*p,statfile);
		fprintf(statfile,"%8s\n",pw->pw_name);
		fclose(statfile);
	}

	/* go into raw mode, no echo */
	if (!moviefile)
	{       ioctl(0, TIOCGETP, &tty_setting);
		tty_flags = tty_setting.sg_flags;
#ifdef CBREAK
		tty_setting.sg_flags |= CBREAK;
		tty_setting.sg_flags &= ~CRMOD;
#else
		tty_setting.sg_flags |= RAW;
#endif
		tty_setting.sg_flags &= ~ECHO;
		ioctl(0, TIOCSETP, &tty_setting);
		if (signal (SIGINT, SIG_IGN) != SIG_IGN)
			signal (SIGINT, catchint);
		if (signal (SIGQUIT, SIG_IGN) != SIG_IGN)
			signal (SIGQUIT, catchint);
	}

	for (i = 0; i < MAXHEIGHT; i++)
		msglen[i] = 0;

	for (i=0; i<MAXWIDTH; i++) for (j=0; j<MAXHEIGHT; j++)
		screen[i][j] = '.';

	if (moviefile)
	{       if ((movie = fopen(moviefile,"r")) == NULL)
			err("Cannot open %s for reading.\n",moviefile);
		minit();        /* initialize from movie file */
	}

	if (seed == 0) seed = get_time();
	initseed = seed;
#ifdef vax
	srand(seed);
#else
	srandom(seed);    /* initialize random number generator */
#endif

	clearscreen();
	readspace();


	cmdx = width;  /* start command just to right of display */
	cmdy = height-4; /* and a ways from the bottom */
	display();	/* initial display */
	for (i=0; i<nairport; i++)
	{       sprintf(mbuf,"%c : %c%c",
		     screen[airport[i].p_x][airport[i].p_y],
		     (j = airport[i].p_dy) == 1 ? 'S' : j == -1 ? 'N' : ' ',
		     (j = airport[i].p_dx) == 1 ? 'E' : j == -1 ? 'W' : ' ');
		msg(cmdx,cmdy-2-i,mbuf);
	}

	cdltop.c_next = 0;      /* initially no clearance directive lists */
	cdltop.c_list = 0;

	while (game_time < 16*60 || game_time > 99*60+59) /*get time from user */
	{       char buf[100];
		char *p;
		cursor(cmdx,cmdy);
		printf("<  >");
		cursor(cmdx,cmdy);

		/* move the cursor one space to the right */
		p = buf;
		tgetstr ("nd", &p);
		fwrite (buf, sizeof (*buf), p-buf, stdout);

		game_time = getdigit()*10;   /* get the time as 2 digits */
		game_time += getdigit();
		initgame_time = game_time = game_time*60+59;
	}

	if (protofile)  /* keeping a protocol of the whole game */
	{       if ((proto = fopen(protofile,"w")) == NULL)
			err("Cannot open %s for writing.\n",protofile);
		protostats(proto);
	}

	if (bigname)    /* megastatistics requested */
	{       if ((bigstatsfile = fopen(bigname,"w")) == NULL)
			err("Cannot open %s for writing.\n",bigname);
		protostats(bigstatsfile);
		if (protofile)
			fprintf(bigstatsfile,"Protocol from %s\n",protofile);
	}

	last_update = initgame_time+UPDATE; /* force early update */
	if (moviefile) last_update += OFFSET;

	for (i=nearlies=0; i<nplanes; i++)   /* construct 26 flight plans */
	{       plane[i].a_id = 'A'+i;
		flightplan(&plane[i],0);
		if (plane[i].a_stime < FPTHRESH) nearlies++;
	}
	for (i=nearlies; i < NEARLY; i++) /* ensure 2 or so planes in 1st minute */
		flightplan(&plane[die(nplanes)],1);
	qsort(plane,nplanes,sizeof (struct astruct),pcompare);
	remain = nplanes;
	if (bigname)
		for (i = 0; i < nplanes; i++)
			strip(&plane[i],-1,TERSE);

	for (i=0; i<nairport; i++)      /* erase the approaches */
		delmsg(cmdx,cmdy-2-i);
	start_time = get_time();
}

protostats(proto)    /* bunch of statistics for protocols and stuff */
FILE *proto;
{       char wd[100];   /* result of a "pwd" command */
	FILE *pwd;
	char *s,*t,*u;
	int c;

	if ((pwd = popen("pwd","r")) == NULL)
		err("Cannot open a pipe to pwd.\n");
	for (s = wd; (c = getc(pwd)) != EOF;)
		if (c != '\n') *s++ = c;
	t = s;  /* here's where to put the filename */
	*s = 0;
	fclose(pwd);
	if (*airfile != '/') /* relative airspace file */
	{       s = t;
		*s++ = '/';
		for (u = airfile; *s++ = *u++; );
		fprintf(proto,"Airspace file: %s\n",wd);
	}
	else    fprintf(proto,"Airspace file: %s\n",airfile);
	fprintf(proto,"Airspace: %s\n",airspace);
	if (flowfile)
	{       if (*flowfile != '/') /* relative flowfile */
		{       s = t;
			*s++ = '/';
			for (u = flowfile; *s++ = *u++; );
			fprintf(proto,"Flow file: %s\n",wd);
		}
		else    fprintf(proto,"Flow file: %s\n",flowfile);
	}
	else    fprintf(proto,"Flow file: \n");
	fprintf(proto,"Seed: %d\n",seed);
	fprintf(proto,"Time: %d seconds\n",game_time);
}

minit() /* initialize the game from a protocol file */
{       /* the file is open on stream "movie" */
	char buffer[80];
	static char airfilebuf[80];
	static char airbuf[80];
	static char flowbuf[80];
	char *s,*t;

	if (fgets(buffer,80,movie) == 0)      /* airspace file line */
		err("Unexpected EOF on movie file.\n");
	for (s = buffer; *s != ':' && *s; s++); /* scan to colon */
	if (*s == 0) err("Bad airspace file line.\n");
	s++; s++;       /* skip colon and space */
	for (t = airfilebuf; (*t++ = *s++) != '\n';); /* copy airspace file */
	*--t = 0;
	airfile = airfilebuf;   /* and dummy the pointer */
	if (fgets(buffer,80,movie) == 0)      /* airspace line */
		err("Unexpected EOF on movie file.\n");
	for (s = buffer; *s != ':' && *s; s++); /* scan to colon */
	if (*s == 0) err("Bad airspace line.\n");
	s++; s++;       /* skip colon and space */
	for (t = airbuf; (*t++ = *s++) != '\n';); /* copy airspace name */
	*--t = 0;
	airspace = airbuf; /* and dummy the pointer */
	if (fgets(buffer,80,movie) == 0)      /* flowfile line */
		err("Unexpected EOF on movie file.\n");
	for (s = buffer; *s != ':' && *s; s++); /* scan to colon */
	if (*s == 0) err("Bad flowfile line.\n");
	s++; s++;       /* skip colon and space */
	if (*s == '\n') flowfile = 0;
	else
	{       for (t = flowbuf; (*t++ = *s++) != '\n';); /* copy flowfile */
		*--t = 0;
		flowfile = flowbuf; /* and dummy the pointer */
	}
	if (fgets(buffer,80,movie) == 0)      /* seed */
		err("Unexpected EOF on movie file.\n");
	for (s = buffer; *s != ':' && *s; s++); /* scan to colon */
	if (*s == 0) err("Bad seed line.\n");
	s++; s++;       /* skip colon and space */
	seed = atoi(s);
	if (fgets(buffer,80,movie) == 0)      /* time line */
		err("Unexpected EOF on movie file.\n");
	for (s = buffer; *s != ':' && *s; s++); /* scan to colon */
	if (*s == 0) err("Bad time line.\n");
	s++; s++;       /* skip colon and space */
	initgame_time = game_time = atoi(s);
	getmcmd();      /* get the first command from movie mode */
}

getmcmd()       /* read a command and store time and command */
{       static char buff[80];
	char *s;

	if (fgets(buff,80,movie) == 0)
	{       mcommand = "";
		nextmovie = -20;
		fclose(movie);
		return;
	}
	for (s = buff; *s == ' '; s++);       /* skip initial blanks */
	nextmovie = atoi(s);
	while (*s++ != ' ');  /* skip to the command */
	mcommand = s;           /* and save it in a global */
	for (; *s; s++) if (*s == '\n') *s = RETURN;
}

pcompare(p1,p2) /* compare start times for sorting the plane list */
struct astruct *p1,*p2;
{       if ((p1)->a_stime < (p2)->a_stime) return(-1);
	else if ((p1)->a_stime == (p2)->a_stime) return(0);
	else return(1);
}


printplans()    /* flight plans for active and near-active planes */
{       register int ctr;      /* can only keep cmdy-2 planes on the screen */
	register struct astruct *p;
	register int i;

	for (i = ctr = 0, p = &plane[0]; i < nplanes && ctr < cmdy - 2; i++,p++)
		if (p->a_active == APPROACHING || p->a_active == WAITING)
			strip(p,ctr++,TERSE);
	delmsg(cmdx,ctr++);     /* blank line between approaching and active*/
	for (p = &plane[i=0]; i < nplanes && ctr < cmdy - 2; i++,p++)
		if (p->a_active == ACTIVE)
			strip(p,ctr++,TERSE);
	while (ctr < cmdy - 2) delmsg(cmdx,ctr++); /* delete any msg on line*/
}

strip(p,y,outstyle)     /* display flight strip for this plane on cmd row y */
register struct astruct *p;
int y,outstyle;
{       char mbuf[80];  /* plan message to appear */
	int pfuel;
	struct list *l;
	char *s;

	if (p->a_active == WAITING) sprintf(mbuf,"  ");
	else if (p->a_active == APPROACHING)
		sprintf(mbuf,"%d ",
			(p->a_stime - initgame_time + game_time + 14) / 15);
	else mbuf[0] = 0;
	sprintf(&mbuf[strlen(mbuf)],"%c%c %c->%c ",
		p->a_id,
		(p->a_type? 'j': 'p'),
		screen[p->a_x][p->a_y],
		(p->a_dair? airport[p->a_dest].p_sym :
			    entry[p->a_dest].p_sym));
	sprintf(&mbuf[strlen(mbuf)],"%d%c%c ",
		p->a_z,
		(p->a_nextz > p->a_z ? '^' :
		 p->a_nextz < p->a_z ? 'v' : ' '),
		(p->a_nextz != p->a_z ? p->a_nextz + '0' : ' '));
	if (p->a_dy>0) sprintf(&mbuf[strlen(mbuf)],"S");
	else if (p->a_dy<0) sprintf(&mbuf[strlen(mbuf)],"N");
	if (p->a_dx>0) sprintf(&mbuf[strlen(mbuf)],"E");
	else if (p->a_dx<0) sprintf(&mbuf[strlen(mbuf)],"W");
	if (p->a_dy == 0) sprintf(&mbuf[strlen(mbuf)]," ");
	switch(p->a_hold)
	{   case 2: sprintf(&mbuf[strlen(mbuf)]," h "); break;
	    case 1: sprintf(&mbuf[strlen(mbuf)]," * "); break;
	    case 0: sprintf(&mbuf[strlen(mbuf)]," %c ",p->a_turn-'A'+'a');
		    break;
	}
	if (p->a_clear)
		sprintf(&mbuf[strlen(mbuf)],"* %c",p->a_clear);
	if (p->a_dynew>0) sprintf(&mbuf[strlen(mbuf)],"S");
	else if (p->a_dynew<0) sprintf(&mbuf[strlen(mbuf)],"N");
	if (p->a_dxnew>0) sprintf(&mbuf[strlen(mbuf)],"E");
	else if (p->a_dxnew<0) sprintf(&mbuf[strlen(mbuf)],"W");
	if (p->a_dynew == 0) sprintf(&mbuf[strlen(mbuf)]," ");
	pfuel = (p->a_type == JET? p->a_fuel/4 : p->a_fuel/2);
	sprintf(&mbuf[strlen(mbuf)]," %c",pfuel>=10?'+':pfuel+'0');
	if (p->a_plan)
		sprintf(&mbuf[strlen(mbuf)]," P");
	if (y < 0)      /* flight strip to file, not screen */
	{       if (bigname)
		{       int st;

			st = p->a_stime;
			if (p->a_type == JET)
			{       st = st / 15;
				st = st * 15;
			}
			else
			{       st = (st + 45) / 30;
				st = (st * 30) - 15;
			}
			if (st < FIRST_TIME) st = FIRST_TIME;
			fprintf(bigstatsfile,"%5d: ",st);
			fprintf(bigstatsfile,"%s\n",mbuf);
		}
	}
	else
	{       msg(cmdx,y,mbuf);   /* display the string */
		if (outstyle == VERBOSE && p->a_plan)
		{       for (l = p->a_plan; l; l = l->l_next)
			{       sprintf(mbuf,"%s %s",l->l_loc,l->l_cmd);
				for (s = mbuf; *s; s++) if (*s == RETURN) *s = 0;
				msg(cmdx,++y,mbuf);
			}
		}
		while (++y < MAXHEIGHT && msglen[y]) msg(cmdx,y,"");/* clr old plans*/
	}
}

msg(x,y,string) /* put up a message at cursor position x,y */
int x,y;
char *string;
{       int i,l;

	cursor(x,y);
	printf("%s",string);
	l = strlen(string);
	for (i=l; i<msglen[y]; i++) putchar(' ');
	msglen[y] = l;
}

delmsg(x,y)     /* delete any message starting at cursor posn x,y */
int x,y;
{       int i;

	if (msglen[y] <= 0) return;
	cursor(x,y);
	for (i=0; i<msglen[y]; i++) putchar(' ');
	msglen[y] = 0;
}


flightplan(p,early)
struct astruct *p;
int early;      /* 1 if this plane should be rescheduled for early arrival */
{       int i,r,cum;
	struct astruct *q;
	struct pstruct *loc;

	p->a_type = die(2);
	/* starting time: no planes in last 15 minutes; pretend interval
	 * extends negatively from 0 by FUDGE seconds to make it more likely
	 * that there will be planes available soon.  On the other hand, no
	 * plane can come on for the first FIRST_TIME seconds.
	 */
	p->a_stime = die(FUDGE + game_time - 15*60) - FUDGE;
	if (early) p->a_stime = die(FPTHRESH);
	if (p->a_stime <FIRST_TIME) p->a_stime = FIRST_TIME;
	if (flowfile)   /* flows have been specified */
	{       r = die(maxflow);
		for (i = cum = 0; i < npaths; i++)
		{       cum += fpath[i].f_freq;
			if (cum >= r) break;
		}
		if (i >= npaths) i = 0; /* shouldn't ever do this */
		p->a_sair = fpath[i].f_fair;
		p->a_dair = fpath[i].f_tair;
		p->a_hold = (p->a_dair && !p->a_sair? 1 : 0);
		p->a_start = fpath[i].f_from;
		p->a_dest = fpath[i].f_to;

		if (p->a_sair) loc = &airport[p->a_start];
		else           loc = &entry[p->a_start];
	}
	else
	{       p->a_sair = (die(inaptot+inawtot) <= inaptot? 1 : 0);
		p->a_dair = (die(outaptot+outawtot) <= outaptot? 1 : 0);

		p->a_hold = (p->a_dair? 1 : 0);
		if (p->a_sair)
		{       p->a_start =pick(die(inaptot),&airport[0],nairport,1);
			loc = &airport[p->a_start];
			p->a_hold = 0;
			if (p->a_dair) p->a_dest = pick(die(inaptot),
							&airport[0],nairport,0);
			else           p->a_dest = pick(die(inawtot),
							&entry[0],nentry,0);
		}
		else
		{       p->a_start = pick(die(inawtot),&entry[0],nentry,0);
			loc = &entry[p->a_start];
			if (p->a_dair) p->a_dest = pick(die(inaptot),
							&airport[0],nairport,0);
			else do p->a_dest = pick(die(inawtot),
						 &entry[0],nentry,0);
			     while (p->a_dest == p->a_start);
		}
	}
	p->a_active = INACTIVE;
	p->a_z = (p->a_sair? 0 : 6);
	if (!p->a_sair) /* separate planes by 1000' altitude */
	    for (q = &plane[0]; q != &plane[PMAX]; q++)
	    {   if (q == p && !early) break;
		if (q == p) continue;
		if (p->a_start == q->a_start)
		    if (abs(q->a_stime - p->a_stime) < SAFETY)
			if (q->a_z >= p->a_z)
			    p->a_z = q->a_z + 1;
	    }
	if (flowfile && p->a_z < NALT)
	{       p->a_opt = fpath[i].f_dist[p->a_z];
		p->a_cmds = fpath[i].f_cmds;
	}
	p->a_prev = p->a_pprev = p->a_aprev = 0;
	p->a_x = loc->p_x;
	p->a_y = loc->p_y;
	p->a_dx = loc->p_dx;
	p->a_dy = loc->p_dy;
	p->a_dxnew = p->a_dynew = 0;
	p->a_lastz = p->a_nextz = p->a_z;
	p->a_turn = 0;
	p->a_fuel = (p->a_type == JET ? JETFUEL : PROPFUEL);
	p->a_clear = 0; /* not cleared for any approach */
	p->a_plan = 0;  /* no monitor points on the flight plan */
	p->a_load = p->a_ticks = p->a_planned = p->a_dist = 0;
}

pick(draw,point,num,in)
int draw;
struct pstruct *point;
int num,in;
{       register int i,cum;

	for (i = cum = 0; i<num; i++,point++)
	{       if (in) cum += point->p_inprop;
		else    cum += point->p_outprop;
		if (cum >= draw) return(i);
	}
	return(0); /* should never get here */
}

#if !NRAND
die(n) /* throw an n-sided die (sides 0 thru n-1) */
int n;
{       int r;

#ifdef vax
	r = rand() >> 10;        /* rightmost bits are not very random */
#else
	r = random();   /* rightmost bits are not very random */
#endif
	return(r % n);  /* enough resolution to make this o.k. */
}
#endif

getdigit()      /* keep reading chars from terminal until get a digit */
{       int c;  /* returns digit - '0' */

	do {
		if (rubout) return 9;
		c = getchar() & 0177;
	} while (c < '0' || c > '9');
	putchar(c);
	return(c-'0');
}

time_t
get_time()      /* returns time in seconds since 1970 */
{
	time_t t;

	time(&t);
	return(t);     /* seconds since 1970 */
}


display()	/* put up initial screen */
{	int i,j;

	for (j=0; j<height; j++)
	{       cursor(0,j);    /* general enough to extend to graphics */
		for (i=0; i<width; i++)
		{       putchar(screen[i][j]);
			putchar(' ');
		}
	}
}


cursor(x,y)	/* move to location x,y on WIDTHxHEIGHT screen */
char x,y;       /* this is in terms of screen, i.e. with x's 2 chars wide */
{ 
	poscur(2*x,y);
} 


poscur(x,y)     /* position cursor at this absolute location on screen */
char x,y;
{       
	char *tgoto();
	printf ("%s", tgoto (cmbuf, x, y));
}


clearscreen()
{       
	printf ("%s", clbuf);
}

char *
plural(n)       /* returns "s" unless n=1 */
int n;
{       static char s[2] = "s";

	if (n != 1) return(s);
	else return("");
}

done(flag)  /* clean up */
int flag;
{       FILE *statfile;
	struct passwd *pw;
	time_t now;
	char *c,*ct;
	extern char *ctime();

	if (!moviefile)
	{       tty_setting.sg_flags = tty_flags;/* put terminal back the way it was*/
		ioctl(0, TIOCSETP, &tty_setting);
	}
	if (flag) cursor(LEFT,height+1);

	if ((statfile = fopen(STATSFILE,"a")) != NULL)
	{       pw = getpwuid(getuid());
		now = get_time();
		ct = ctime(&now);
		for (c= &ct[4]; c <= &ct[16]; c++) fputc(*c,statfile);
		if (!flag) fprintf(statfile,"%8s: aborted\n",pw->pw_name);
		else
		{  fprintf(statfile,"%8s: %2d %3d %2d %2d %3d %5d   %3d %2d",
			   pw->pw_name,crash,system,fuel,bound,goaround,
			   fuelused,cmds,remain);
		   fprintf(statfile,". %d  %2d %s\n",
			   initseed,initgame_time/60,airspace);
		   fclose(statfile);
		}
	}
	if (!flag) return;
	ttystats(stdout);
	if (bigname)
	{       int i;
		float load,ticks,plans,route,d,stripped;

		for (i = 0; i < nplanes; i++)
		{       load = plane[i].a_load;
			ticks = plane[i].a_ticks;
			plans = plane[i].a_planned;
			route = plane[i].a_opt;
			stripped = plane[i].a_prev;
			d = plane[i].a_dist;
			fprintf(bigstatsfile,
"%c: %2d tks, %2d/%2d=%1.2f, %d, %.2f a/tk, %.2f p/tk, %.2f a/ptk, %.2f p/ptk, %c, %s\n",
				plane[i].a_id,plane[i].a_ticks,
				plane[i].a_dist,
				plane[i].a_opt,
				d/route,
				plane[i].a_cmds,
				load/ticks,
				plans/ticks,
				plane[i].a_aprev/stripped,
				plane[i].a_pprev/stripped,
				plane[i].a_sair? 'W' : 'A',
				plane[i].a_pplanned? "+P" : "-P"
			);
		}
	}
	if (bigname) ttystats(bigstatsfile);
	if (protofile) fclose(proto);
	if (bigname) fclose(bigstatsfile);
}

ttystats(file)
FILE *file;
{       char *eol;

	if (file == bigstatsfile) eol = "\n";
	else {
		clearscreen();
		eol = "\n\r";
	}
	if (crash)  fprintf(file,"%d plane%s out of fuel, crashed%s",
		crash,plural(crash),eol);
	if (system) fprintf(file,"%d system error%s%s",
		system,plural(system),eol);
	if (fuel)   fprintf(file,"%d tow truck%s called%s",
		fuel,plural(fuel),eol);
	if (bound)  fprintf(file,"%d boundary error%s%s",
		bound,plural(bound),eol);
	if (goaround) fprintf(file,"%d go-around%s%s",
		goaround,plural(goaround),eol);
	if (crash+system+fuel+bound+remain == 0)
	{ fprintf(file,"GREAT JOB!!  Have you considered a career in ATC?\n");
	  fprintf(file,"Now try %d minutes...\n",initgame_time/60-5);
	}
	fprintf(file,"Fuel used: %d\n",fuelused);
	fprintf(file,"Commands issued (including deferred): %d\n",icmds);
	fprintf(file,"Commands executed: %d\n",cmds);
	fprintf(file,"Airspace: %s; seed: %d; time=%d; %d remaining.%s",
		airspace,initseed,initgame_time/60,remain,eol);
	if (file == bigstatsfile) fprintf(file,"\f");
}

err(msg,arg1,arg2)
char *msg; 
{       fprintf(stderr,msg,arg1,arg2);
	done(0);
	exit(1);
} 


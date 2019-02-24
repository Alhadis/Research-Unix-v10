#include <stdio.h>
#include <signal.h>
#ifdef BELL3.0
#include <fcntl.h>
#include <termio.h>
#else
#include <sgtty.h>
#endif
#include <pwd.h>
#include "pacdefs.h"

#ifndef BELL3.0
#define	O_RDWR	2
#define	O_CREAT 0
#define	O_RDONLY 0
#define	O_NDELAY 0
#endif

extern char
	*BC,
	*UP;

extern int
	putch();

extern char
	*tgoto(),
	*mktemp();

extern int
	delay,
	errno,
	goldcnt;

extern long
	time();

extern struct pac
	*pacptr;

extern struct pac
	monst[];

#ifndef MSG
int	comfile;
#else
struct mstruct
{
	int     frompid;
	int     mtype;
};

#endif

#ifndef NODELAY

#ifndef MSG
char	*fnam;
#endif

#endif

/*
 * initbrd is used to re-initialize the display
 * array once a new game is started.
 */
char	initbrd[BRDY][BRDX] =
{
"OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO",
"O + + + * + + + + OOO + + + + * + + + O",
"O X OOO + OOOOO + OOO + OOOOO + OOO X O",
"O * + + * + * + * + + * + * + * + + * O",
"O + OOO + O + OOOOOOOOOOO + O + OOO + O",
"O + + + * O + + + OOO + + + O * + + + O",
"OOOOOOO + OOOOO + OOO + OOOOO + OOOOOOO",
"      O + O + + * + + * + + O + O      ",
"      O + O + OOO - - OOO + O + O      ",
"OOOOOOO + O + O         O + O + OOOOOOO",
"        * + * O         O * + *        ",
"OOOOOOO + O + O         O + O + OOOOOOO",
"      O + O + OOOOOOOOOOO + O + O      ",
"      O + O * + + + + + + * O + O      ",
"OOOOOOO + O + OOOOOOOOOOO + O + OOOOOOO",
"O + + + * + * + + OOO + + * + * + + + O",
"O X OOO + OOOOO + OOO + OOOOO + OOO X O",
"O + + O * + * + * + + * + * + * O + + O",
"OOO + O + O + OOOOOOOOOOO + O + O + OOO",
"O + * + + O + + + OOO + + + O + + * + O",
"O + OOOOOOOOOOO + OOO + OOOOOOOOOOO + O",
"O + + + + + + + * + + * + + + + + + + O",
"OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO",
};

/*
 * brd is kept for historical reasons.
 * It should only be used in the routine "which"
 * to determine the next move for a monster or
 * in the routine "monster" to determine if it
 * was a valid move. Admittedly this is redundant
 * and could be replaced by initbrd, but it is kept
 * so that someday additional intelligence or
 * optimization could be added to the choice of
 * the monster's next move. Hence, note the symbol
 * CHOICE at most points that a move decision
 * logically HAS to be made.
 */
char	brd[BRDY][BRDX] =
{
"OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO",
"O + + + * + + + + OOO + + + + * + + + O",
"O X OOO + OOOOO + OOO + OOOOO + OOO X O",
"O * + + * + * + * + + * + * + * + + * O",
"O + OOO + O + OOOOOOOOOOO + O + OOO + O",
"O + + + * O + + + OOO + + + O * + + + O",
"OOOOOOO + OOOOO + OOO + OOOOO + OOOOOOO",
"      O + O + + * + + * + + O + O      ",
"      O + O + OOO - - OOO + O + O      ",
"OOOOOOO + O + O         O + O + OOOOOOO",
"        * + * O         O * + *        ",
"OOOOOOO + O + O         O + O + OOOOOOO",
"      O + O + OOOOOOOOOOO + O + O      ",
"      O + O * + + + + + + * O + O      ",
"OOOOOOO + O + OOOOOOOOOOO + O + OOOOOOO",
"O + + + * + * + + OOO + + * + * + + + O",
"O X OOO + OOOOO + OOO + OOOOO + OOO X O",
"O + + O * + * + * + + * + * + * O + + O",
"OOO + O + O + OOOOOOOOOOO + O + O + OOO",
"O + * + + O + + + OOO + + + O + + * + O",
"O + OOOOOOOOOOO + OOO + OOOOOOOOOOO + O",
"O + + + + + + + * + + * + + + + + + + O",
"OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO",
};

/*
 * display reflects the screen on the player's
 * terminal at any point in time.
 */
char	display[BRDY][BRDX] =
{
"OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO",
"O + + + + + + + + OOO + + + + + + + + O",
"O X OOO + OOOOO + OOO + OOOOO + OOO X O",
"O + + + + + + + + + + + + + + + + + + O",
"O + OOO + O + OOOOOOOOOOO + O + OOO + O",
"O + + + + O + + + OOO + + + O + + + + O",
"OOOOOOO + OOOOO + OOO + OOOOO + OOOOOOO",
"      O + O + + + + + + + + O + O      ",
"      O + O + OOO - - OOO + O + O      ",
"OOOOOOO + O + O         O + O + OOOOOOO",
"        + + + O         O + + +        ",
"OOOOOOO + O + O         O + O + OOOOOOO",
"      O + O + OOOOOOOOOOO + O + O      ",
"      O + O + + + + + + + + O + O      ",
"OOOOOOO + O + OOOOOOOOOOO + O + OOOOOOO",
"O + + + + + + + + OOO + + + + + + + + O",
"O X OOO + OOOOO + OOO + OOOOO + OOO X O",
"O + + O + + + + + + + + + + + + O + + O",
"OOO + O + O + OOOOOOOOOOO + O + O + OOO",
"O + + + + O + + + OOO + + + O + + + + O",
"O + OOOOOOOOOOO + OOO + OOOOOOOOOOO + O",
"O + + + + + + + + + + + + + + + + + + O",
"OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO",
};

/* terminal type from environment and /etc/termcap ala ex & vi */
char    *vs_cl  = "",           /* clear screen sequence */
	*vs_cm  = "",           /* cursor positioning sequence */
	*vs_vb  = "";

char	combuf[BUFSIZ],
	message[81],	/* temporary message buffer */
	inbuf[2];

int	ppid,
	cpid,
	game,
	killcnt = 0,
	vs_rows,
	vs_cols;

unsigned
	pscore;

long	timein;

#ifdef BELL3.0
struct termio
	savetty,
	newtty;
#else
struct sgttyb
	savetty, newtty;
#endif

struct uscore
{
	unsigned score;	/* same type as pscore */
	int uid;	/* uid of player */
};

struct scorebrd
{
	struct uscore entry[MSSAVE];
} scoresave[MGTYPE] = 
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};

update()
{
	char	str[10];

	(void) sprintf(str, "%6d", pscore);
	SPLOT(0, 52, str);
	(void) sprintf(str, "%6d", goldcnt);
	SPLOT(21, 57, str);
}

reinit()
{
	register int locx, locy;
	register char tmp;

	for (locy = 0; locy < BRDY; locy++)
	{
		for (locx = 0; locx < BRDX; locx++)
		{
			tmp = initbrd[locy][locx];
			brd[locy][locx] = tmp;
			if ((display[locy][locx] = tmp) == CHOICE)
			{
				display[locy][locx] = GOLD;
			};
		};
	};
	goldcnt = GOLDCNT;
	delay -= (delay / 10);	/* hot it up */
}

errgen(string)
char	*string;
{
	SPLOT(23,45,string);
}

dokill(mnum)
	int mnum;
{
	register struct pac *mptr;

	PLOT(0, 0, BEEP);
	if (pacptr->danger == TRUE)
	{
		if (++killcnt == MAXMONSTER)
		{
			if (display[TRYPOS][TRXPOS] == GOLD)
			{
				goldcnt--;
			};
			display[TRYPOS][TRXPOS] = TREASURE;
			PLOT(TRYPOS, TRXPOS, TREASURE);
			killcnt = 0;
		};
		SPLOT(5, 45, "MONSTERS KILLED: ");
		(void) sprintf(message, "%1d", killcnt);
		SPLOT(5, 62, message);
		mptr = (&monst[mnum]);
		mptr->ypos = MSTARTY;
		mptr->xpos = MSTARTX + (2 * mnum);
		mptr->stat = START;
		PLOT(mptr->ypos, mptr->xpos, MONSTER);
		pscore += KILLSCORE;
		return(GOTONE);
	};
	return(TURKEY);
}

/*
/* clr -- issues an escape sequence to clear the display
*/

clr()
{
	(void) fprintf(stdout, "%s", vs_cl);
	fflush(stdout);
	nap(2);
}

/*
 *	display initial instructions
 */

instruct()
{
	clr();
	POS(0, 0);
	(void) fprintf(stdout, "Attention: you are in a dungeon, being chased by monsters!\r\n\n");
	(void) fprintf(stdout, "There are gold coins scattered uniformly in the dungeon, marked by \"+\".\r\n");
	(void) fprintf(stdout, "One magic potion is available at each spot marked \"X\". Each potion will\r\n");
	(void) fprintf(stdout, "enable you to kill monsters by touch for a limited duration. It will also\r\n");
	(void) fprintf(stdout, "scare them away. When you kill a monster it is regenerated, but this takes\r\n");
	(void) fprintf(stdout, "time. You can also regenerate yourself %d times. Killing all the monsters\r\n", MAXPAC);
	(void) fprintf(stdout, "results in further treasure appearing magically somewhere in the dungeon,\r\n");
	(void) fprintf(stdout, "marked by \"$\". There is a magic tunnel connecting the center left and\r\n");
	(void) fprintf(stdout, "center right parts of the dungeon. The monsters know about it!\r\n\n");
	(void) fprintf(stdout, "        Type:   h       to move left\r\n");
	(void) fprintf(stdout, "                l       to move right\r\n");
	(void) fprintf(stdout, "                k or w  to move up\r\n");
	(void) fprintf(stdout, "                j or x  to move down\r\n");
	(void) fprintf(stdout, "                <space> to halt \r\n");
	(void) fprintf(stdout, "                q       to quit\r\n\n");
	(void) fprintf(stdout, "        Type:   1       normal game\r\n");
	(void) fprintf(stdout, "                2       blinking monsters\r\n");
	(void) fprintf(stdout, "                3       intelligent monsters\r\n");
	(void) fprintf(stdout, "                4       blinking intelligent monsters\r\n");
}

/*
 * over -- game over processing
 */

over()
{
	register int i;
	register int line;
	int scorefile = 0;
	struct passwd *getpwuid(), *p;

	fflush(stdout);
	sleep(5);	/* for slow readers */
	poll(0);	/* flush and discard input from player */
	clr();
	/* high score to date processing */
	if (game != 0)
	{
		line = 7;
		POS(line++, 20);
		(void) fprintf(stdout, " ___________________________ ");
		POS(line++, 20);
		(void) fprintf(stdout, "|                           |");
		POS(line++, 20);
		(void) fprintf(stdout, "| G A M E   O V E R         |");
		POS(line++, 20);
		(void) fprintf(stdout, "|                           |");
		POS(line++, 20);
		(void) fprintf(stdout, "| Game type: %1d              |",game);
		if ((scorefile = open(MAXSCORE, O_RDWR | O_CREAT, 0666)) != -1)
		{
			read(scorefile, (char *)scoresave, sizeof(scoresave));
			for (i = MSSAVE - 1; i >= 0; i--) {
				if (scoresave[game - 1].entry[i].score < pscore)
				{
					if (i < MSSAVE - 1)
					{
						scoresave[game - 1].entry[i + 1].score =
							scoresave[game - 1].entry[i].score;
						scoresave[game - 1].entry[i + 1].uid =
							scoresave[game - 1].entry[i].uid;
					};
					scoresave[game - 1].entry[i].score = pscore;
					scoresave[game - 1].entry[i].uid = getuid();
				};
			};
			lseek(scorefile, 0l, 0);
			write(scorefile, (char *)scoresave, sizeof(scoresave));
			close(scorefile);
			POS(line++, 20);
			(void) fprintf(stdout, "| High Scores to date:      |");
			for (i = 0; i < MSSAVE; i++)
			{
				setpwent();
				p = getpwuid(scoresave[game - 1].entry[i].uid);
				POS(line++, 20);
				(void) fprintf(stdout, "| Player : %-8s  %5u  |", p->pw_name,
					scoresave[game - 1].entry[i].score);
			};
		}
		else
		{
			POS(line++, 20);
			(void) fprintf(stdout, "|                           |");
			POS(line++, 20);
			(void) fprintf(stdout, "| Please create a 'paclog'  |");
			POS(line++, 20);
			(void) fprintf(stdout, "| file. See 'MAXSCORE' in   |");
			POS(line++, 20);
			(void) fprintf(stdout, "| 'pacdefs.h'.              |");
		};
		POS(line++, 20);
		(void) fprintf(stdout, "|                           |");
		POS(line++, 20);
		(void) fprintf(stdout, "| Your score: %-5u         |", pscore);
		POS(line, 20);
		(void) fprintf(stdout, "|___________________________|");
	};
	leave();
}

/*
 * leave -- flush buffers,kill the Child, reset tty, and delete tempfile
 */

leave()
{
	POS(23, 0);
	(void) fflush(stdout);
	sleep(1);

#ifndef NODELAY
	kill(cpid, SIGKILL);
#endif

#ifdef BELL3.0
	ioctl(0, TCSETAW, &savetty);
#else
	ioctl(0, TIOCSETP, &savetty);
#endif

#ifndef NODELAY

#ifndef MSG
	close(comfile);
	unlink(fnam);
#endif

#endif
	exit(0);
}

/*
 * init -- does global initialization and spawns a child process to read
 *      the input terminal.
 */

init()
{
	register int tries = 0;
	static int lastchar = DELETE;
	extern short ospeed;		/* baud rate for crt (for tputs()) */

	errno = 0;
	(void) time(&timein);	/* get start time */
	srand((unsigned)timein);	/* start rand randomly */
	/*
	 * verify CRT and get proper cursor control sequence.
	 */

#ifdef NODELAY

#ifndef BELL3.0
	close(0);
	if (open("/dev/tty", O_RDONLY | O_NDELAY) != 0)
	{
		perror("Unable to open /dev/tty");
		exit(1);
	};
#endif

#endif
	vsinit();
	/*
	 * setup raw mode, no echo
	 */
#ifdef BELL3.0
	ioctl(0, TCGETA, &savetty);
	newtty = savetty;
	if ((savetty.c_cflag & CBAUD) == B9600)
	{
		delay = 1000 * (nrand(10) + 5);
	}
	else
	{
		delay = 1000;
	};
	ospeed = savetty.c_cflag & CBAUD;	/* for tputs() */
#ifdef NODELAY
	newtty.c_cc[VTIME] = 7;
	newtty.c_cc[VMIN] = 0;
#else
	newtty.c_cc[VMIN] = 1;
#endif
	newtty.c_iflag = 0;
	newtty.c_oflag &= ~OPOST;
	newtty.c_cflag &= ~(PARENB | CSIZE);
	newtty.c_cflag |= CS8;
	newtty.c_lflag &= ~(ECHO | ECHOE | ISIG | ICANON | XCASE);
	ioctl(0, TCSETAW, &newtty);
#else
	ioctl(0, TIOCGETP, &savetty);
	newtty = savetty;
	if (savetty.sg_ospeed >= B9600)
		delay = 1000 * (nrand(10) + 5);
	else
		delay = 1000;
	ospeed = savetty.sg_ospeed;		/* for tputs() */
	newtty.sg_flags |= CBREAK;
	newtty.sg_flags &= ~ECHO;
	ioctl(0, TIOCSETP, &newtty);
#endif

	/*
	 * set up communications
	 */

#ifndef NODELAY

#ifndef MSG
	fnam = mktemp(TMPF);
	if ((comfile = creat(fnam, 0666)) == -1)
	{
		(void) sprintf(message, "Cannot create %s", fnam);
		perror(message);
		leave();
	};
	/*
	 * initialize semaphore
	 */
	lseek(comfile, 0l, 0);
	combuf[1] = EMPTY;
	write(comfile, combuf, 2);
	close(comfile);
#endif
#endif

#ifndef NODELAY
	/*
	 * fork off a child to read the input
	 */
	ppid = getpid();
	if ((cpid = fork()) == 0)
	{
#ifndef MSG
		comfile = open(fnam, O_RDWR);
#endif
		for (;;)
		{
			read(0, inbuf, 1);
			if (lastchar != inbuf[0])
			{
#ifdef MSG
				if (inbuf[0] == DELETE || inbuf[0] == ABORT)
					leave();
				send(inbuf, 1, ppid, 64);
#else
				lseek(comfile, 0l, 0);
				read(comfile, combuf, 2);
				if (combuf[1] == EMPTY)
				{
					lseek(comfile, 0l, 0);
					combuf[0] = inbuf[0];
					combuf[1] = FULL;
					write(comfile, combuf, 2);
				};
#endif
			};
			lastchar = DELETE;
		};
	};

#ifndef MSG
	comfile = open(fnam, O_RDWR);
#else
	msgenab();
#endif

#endif
	/*
	 * New game starts here
	 */
	game = 0;
	instruct();
	while ((game == 0) && (tries++ < 300))
	{
		poll(1);
	};
	if (tries >= 300)
	{
		/* I give up. Let's call it quits. */
		leave();
	};
	goldcnt = GOLDCNT;
	pscore = 0;
	clr();
}

/*
 * poll -- read characters sent by input subprocess and set global flags
 */

poll(sltime)
{
	int stop;
	register int charcnt;
	int junk;

#ifndef NODELAY
#ifdef MSG
	struct mstruct msghead;
#endif
#endif

	stop = 0;
readin:

#ifdef NODELAY
	fflush(stdout);
	charcnt = 0;
	nap(12);
#ifndef BELL3.0
	ioctl(0, FIONREAD, &junk);
	if (junk)
#endif
		charcnt = read(0, combuf, 1);
	switch (charcnt)
	{

	case 0:
		combuf[1] = EMPTY;
		break;

	case -1:
		errgen("READ ERROR IN POLL");
		abort();

	default:
		combuf[0] = combuf[charcnt-1];
		combuf[1] = FULL;
		break;
	};
#else

#ifdef MSG
	combuf[1] = (recv(combuf, 1, &msghead, 0) == -1) ? EMPTY : FULL;
#else
	lseek(comfile, 0l, 0);	/* rewind */
	read(comfile, combuf, 2);	/* read 2 chars */
#endif

#endif
	if (combuf[1] == EMPTY) 
	{
#ifndef BELL3.0

#ifndef NODELAY
		if (sltime > 0)
		{
			sleep(sltime);
		};
#endif

#endif
		if (stop)
		{
			goto readin;
		};
		return;
	};
	combuf[1] = EMPTY;

#ifndef NODELAY

#ifndef MSG
	lseek(comfile, 0l, 0);	/* another rewind */
	write(comfile, combuf, 2);
#endif
#endif

	switch(combuf[0] & 0177)
	{
	case LEFT:
		pacptr->dirn = DLEFT;
		break;

	case RIGHT:
		pacptr->dirn = DRIGHT;
		break;

	case NORTH:
	case NNORTH:
		pacptr->dirn = DUP;
		break;

	case DOWN:
	case NDOWN:
		pacptr->dirn = DDOWN;
		break;

	case HALT:
		pacptr->dirn = DNULL;
		break;

	case ABORT:
	case DELETE:
	case QUIT:
		over();
		break;

	case CNTLS:
		stop = 1;
		goto readin;

	case GAME1:
		game = 1;
		break;

	case GAME2:
		game = 2;
		break;

	case GAME3:
		game = 3;
		break;

	case GAME4:
		game = 4;
		break;

	default:
		goto readin;
	}
}

vsinit()
{
	char buf[1024];
	char tspace[256], *aoftspace;
	char *tgetstr();
	extern char *UP, *BC;   /* defined in tgoto.c (from ex's termlib */
	extern char PC;		/* defined in tputs.c (from termlib) */

	tgetent(buf, getenv("TERM"));
	aoftspace = tspace;
	vs_cl = tgetstr("cl", &aoftspace);
	vs_cm = tgetstr("cm", &aoftspace);
	BC = tgetstr("bc", &aoftspace);
	UP = tgetstr("up", &aoftspace);
	PC = *tgetstr("pc", &aoftspace);
	vs_vb = tgetstr("vb", &aoftspace);
	vs_rows = tgetnum("li");
	vs_cols = tgetnum("co");
	if ((vs_vb == 0) || (*vs_vb == '\0'))
	{
		vs_vb = aoftspace;
		*vs_vb++ = '';
		*vs_vb++ = '\0';
		aoftspace += 2;
	};
	if ((vs_cl == 0) || (*vs_cl == '\0') ||
		(vs_cm == 0) || (*vs_cm == '\0'))
	{
		(void) fprintf(stderr, "\nPacman is designed for CRT's with addressable cursors.\n");
		(void) fprintf(stderr, "Verify that the TERM environment variable is a proper\n");
		(void) fprintf(stderr, "type and is export-ed, and try it again...\n\n");
		exit(2);
	};
}

/* dfp - real function for tputs function to use */
putch( ch )
	register int ch;
{
	putchar( ch );
}

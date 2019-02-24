/* cc hanoi.c -lcurses -ltermlib
	The Tower of Hanoi problem consists of three posts
	and a set of discs of increasing sizes.  The initial
	configuration places all the discs on one post arranged
	with small discs on large ones.  The goal is to transfer
	all the discs to another post.  There are two restrictions:
	only one disc can be moved at any time, and it is illegal
	to put a larger disc on a smaller.

	This implementation depends on two libraries: The curses
	screen handling package is used to make a pretty display.
	The getopt function is used to handle the command line options;
	I got a public domain version from netnews.

	The options control the size of the problem and the
	display style.
	
	Number of discs:
		The default number of discs is NODISCS,
		defined below.  The -n option can change this
		up to a maximum of MAXDISCS; the maximum is based
		on a terminal screen with 80 columns.
	Display Speed:
		By default, a smooth animation algorithm is used.
		The -f (fast) option makes hanoi less smooth,
		displaying discs at key points in their transfers.
		This is a pretty good choice for slow speed terminals
		because there is still a motion illusion.
		The -t (teleport) option shows discs only
		after they have been moved.  At high speeds,
		this looks really cool.
*/
#include <curses.h>
#include <ctype.h>
#include <signal.h>

#define	LEFT       0
#define	MIDDLE     1
#define	RIGHT      2
#define MAXDISCS   13
#define TOP        (MAXDISCS+5)
#define	BLANK      ' '
#define DISC       '='
#define NODISCS    7

#define	SMOOTH     0
#define	FAST       1
#define	TELEPORT   2

int	Display = SMOOTH;     /* Display style: SMOOTH, FAST, or TELEPORT */
int	Nodiscs = NODISCS;    /* Number of discs, up to MAXDISCS */
int	count[3];             /* number of discs on each post */
int	col[3];               /* column number of each post */

main (argc, argv)
char	**argv;
	{
	initial (argc, argv);
	hanoi (Nodiscs, LEFT, MIDDLE, RIGHT);
	sleep (Nodiscs);
	hanoi (Nodiscs, RIGHT, MIDDLE, LEFT);
	finish ("Done!");
	}

onint ()
	{
	signal (SIGINT, SIG_IGN);
	finish ("...interrupted");
	}
finish (s)
char	*s;
	{
	move (LINES-2, 0);
	refresh ();
	endwin ();
	puts (s);
	exit (0);
	}

initial (argc, argv)
char	**argv;
	{
	int 	errflg = 0;   /* any option errors? */
	int 	C;            /* the option flag name */
	extern	char *optarg; /* defined in getopt */
	extern	onint();      /* called if user interrupts */
	while ((C = getopt (argc, argv, "tfn:")) != EOF)
		{
		switch (C)
			{
			case 'f': Display = FAST; break;
			case 't': Display = TELEPORT; break;
			case 'n': Nodiscs = atoi (optarg);
				if (Nodiscs == 0) Nodiscs = NODISCS;
				else if (Nodiscs > MAXDISCS) Nodiscs = MAXDISCS;
				break;
			default:
				errflg++;
			}
		}
	if (errflg)
		{
		fprintf (stderr, "USAGE: hanoi [-stf] [-n discs]\n");
		exit (1);
		}
	col[LEFT] = Nodiscs;
	col[MIDDLE] = 3*Nodiscs;
	col[RIGHT] = 5*Nodiscs;
	initscr ();
	noecho ();
	signal (SIGINT, onint);
	display (Nodiscs);
	}

display (n)
	{
	clear ();
	standout ();
	mvprintw (0, 0, " The Tower of Hanoi ");
	standend ();
	while (n > 0)
		mkdisc (n--, LEFT);
	}

/*
	The algorithm is a simple recursive one familiar to all.
*/
hanoi (n, start, inter, finish)
	{
	if (n > 0)
		{
		hanoi (n-1, start, finish, inter);
		movedisc (n, start, finish);
		hanoi (n-1, inter, start, finish);
		}
	}

movedisc (n, start, finish)
	{
	rmdisc (n, start);
	showdisc (n, start, finish);
	mkdisc (n, finish);
	}

showdisc (n, start, finish)
	{
	int	y;
	int x;
	int	dir = (col[start] < col[finish]) ? 1 : -1;
	if (Display == TELEPORT) return;
	for (y = count[start]; y < Nodiscs-1; y++)
		{
		plotdisc (n, y, col[start], BLANK);
		if (Display == SMOOTH) refresh ();
		plotdisc (n, y+1, col[start], DISC);
		if (Display == SMOOTH) refresh ();
		}
	refresh ();
	for (x = col[start]; x != col[finish]; x += dir)
		{
		plotdisc (n, y, x, BLANK);
		plotdisc (n, y, x+dir, DISC);
		if (Display == SMOOTH) refresh ();
		}
	refresh ();
	while (y > count[finish])
		{
		plotdisc (n, y--, col[finish], BLANK);
		if (Display == SMOOTH) refresh ();
		plotdisc (n, y, col[finish], DISC);
		if (Display == SMOOTH) refresh ();
		}
	refresh ();
	}

mkdisc (n, pile)
	{
	plotdisc (n, count[pile], col[pile], DISC);
	count[pile]++;
	refresh ();
	}

rmdisc (n, pile)
	{
	count[pile]--;
	plotdisc (n, count[pile], col[pile], BLANK);
	refresh ();
	}

plotdisc (n, y, x, c)
	{
	int	i;
	move (TOP-y, x-n);
	for (i = 1; i <= 2*n; i++)
		addch (c);
	}



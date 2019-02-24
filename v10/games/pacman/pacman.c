/*
/* PACMAN  - written by Dave Nixon, AGS Computers Inc., July, 1981.
/*
/* Terminal handling for video games taken from aliens
/*      the original version  of aliens is from 
/*      Fall 1979                      Cambridge               Jude Miller
/*
/* Score keeping modified and general terminal handling (termcap routines
/* from UCB's ex) added by Rob Coben, BTL, June, 1980.
/*
/* If MSG is defined, the program uses the inter-process message facility
/* of UNIX/TS Augmented to optimize reading the tty while updating the
/* screen. Otherwise, the child process (reading the tty) writes
/* a temporary communications file, which the parent (updating the screen)
/* is constantly seeking to the beginning of and re-reading. (UGH!)
/* If your system has a non-blocking read (read-without-wait), it should
/* be implemented, and the child process could be dispensed with entirely.
*/
#include <stdio.h>
#include	"pacdefs.h"
 
/*
 * global variables
 */

extern char
	*vs_cm;

extern int
	putch();

extern char
	*tgoto();

extern char
	message[];

extern char
	initbrd[BRDY][BRDX],
	display[BRDY][BRDX];

extern unsigned
	pscore;

extern struct pac
	monst[];

int	pacsymb = PACMAN,
	rounds,		/* time keeping mechanism */
	killflg,
	delay,
	potion,
	goldcnt,		/* no. of gold pieces remaining */
	potioncnt;

struct pac
	pac;

struct pac
	pacstart =
{
	PSTARTX,
	PSTARTY,
	DNULL,
	SLOW,
	FALSE
};

struct pac
	*pacptr = &pac;

main()
{
	register int tmp;		/* temp variables */
	register int pac_cnt;
	register int monstcnt;	/* monster number */
	struct pac *mptr;
	char gcnt[10];

	init();		/* global init */
	for (pac_cnt = MAXPAC; pac_cnt > 0; pac_cnt--)
	{
redraw:
		clr();
		SPLOT(0, 45, "SCORE: ");
		SPLOT(21, 45, "gold left = ");
		(void) sprintf(gcnt, "%6d", goldcnt);
		SPLOT(21, 57, gcnt);
		if (potion == TRUE)
		{
			SPLOT(3, 45, "COUNTDOWN: ");
		};
		pacsymb = PACMAN;
		killflg = FALSE;
		(void) sprintf(message, "delay = %6d", delay);
		SPLOT(22, 45, message);
		/*
		 * PLOT maze
		 */
		for (tmp = 0; tmp < BRDY; tmp++)
		{
			SPLOT(tmp, 0, &(display[tmp][0]));
		};
		/* initialize a pacman */
		pac = pacstart;
		PLOT(pacptr->ypos, pacptr->xpos, pacsymb);
		/* display remaining pacmen */
		for (tmp = 0; tmp < pac_cnt - 1; tmp++)
		{
			PLOT(23, (MAXPAC * tmp), PACMAN);
		};
		/*
		 * Init. monsters
	 	 */
		for (mptr = &monst[0], monstcnt = 0; monstcnt < MAXMONSTER; mptr++, monstcnt++)
		{
			mptr->xpos = MSTARTX + (2 * monstcnt);
			mptr->ypos = MSTARTY;
			mptr->speed = SLOW;
			mptr->dirn = DNULL;
			mptr->danger = FALSE;
			mptr->stat = START;
			PLOT(mptr->ypos, mptr->xpos, MONSTER);
		};
		rounds = 0;	/* timing mechanism */

		/* main game loop */
		do
		{
			if (rounds++ % MSTARTINTVL == 0)
			{
				startmonst();
			};
			pacman();
			if (killflg == TURKEY)
				break;
			for (monstcnt = 0; monstcnt < (MAXMONSTER / 2); monstcnt++)
			{
				monster(monstcnt);	/* next monster */
			};
			if (killflg == TURKEY)
				break;
			if (pacptr->speed == FAST)
			{
				pacman();
				if (killflg == TURKEY)
					break;
			};
			for (monstcnt = (MAXMONSTER / 2); monstcnt < MAXMONSTER; monstcnt++)
			{
				monster(monstcnt);	/* next monster */
			};
			if (killflg == TURKEY)
				break;
			if (potion == TRUE)
			{
				(void) sprintf(message, "%2d%c", potioncnt,
					((potioncnt == 10) ? BEEP : ' '));
				SPLOT(3, 60, message);
				if (--potioncnt <= 0)
				{
					SPLOT(3, 45, "                        ");
					potion = FALSE;
					pacptr->speed = SLOW;
					pacptr->danger = FALSE;
					for (monstcnt = 0; monstcnt < MAXMONSTER; monstcnt++)
					{
						monst[monstcnt].danger = TRUE;
					};
				};
			};
			update();	/* score display etc */
			if (goldcnt <= 0)
			{
				reinit();
				goto redraw;
			};
		} while (killflg != TURKEY);
		SPLOT(5, 45, "YOU ARE BEING EATEN");
		SPLOT(6, 45, "THIS TAKES ABOUT 2 SECONDS");
		sleep(2);
	};
	SPLOT(8, 45, "THE MONSTERS ALWAYS TRIUMPH");
	SPLOT(9, 45, "IN THE END!");
	over();
}

pacman()
{
	register int sqtype;
	register int mcnt;
	register int tmpx, tmpy;
	int deltat;
	struct pac *mptr;

	/* pause; wait for the player to hit a key */
	for (deltat = delay; deltat > 0; deltat--);

	/* get instructions from player, but don't wait */
	poll(0);

	/* remember current pacman position */
	tmpx = pacptr->xpos;
	tmpy = pacptr->ypos;

	/* "eat" any gold */
	/* update display array to reflect what is on terminal */
	display[tmpy][tmpx] = VACANT;

	/* what next? */
	switch (pacptr->dirn)
	{
	case DUP:
		pacsymb = PUP;
		switch (sqtype = display[tmpy + UPINT][tmpx])
		{
		case GOLD:
		case VACANT:
		case CHOICE:
		case POTION:
		case TREASURE:

			/* erase where the pacman went */
			PLOT(tmpy, tmpx, VACANT);
			pacptr->ypos += UPINT;
			break;

		default:
			pacptr->dirn = DNULL;
			break;
		};
		break;
	case DDOWN:
		pacsymb = PDOWN;
		switch (sqtype = display[tmpy + DOWNINT][tmpx])
		{
		case GOLD:
		case VACANT:
		case CHOICE:
		case POTION:
		case TREASURE:

			/* erase where the pacman went */
			PLOT(tmpy, tmpx, VACANT);
			pacptr->ypos += DOWNINT;
			break;

		default:
			pacptr->dirn = DNULL;
			break;
		};
		break;
	case DLEFT:
		if(tmpx == 0)
		{
			/* erase where the pacman went */
			PLOT(tmpy, tmpx, VACANT);
			pacptr->xpos = XWRAP;
			sqtype = VACANT;
			break;
		};
		pacsymb = PLEFT;
		switch (sqtype = display[tmpy][tmpx + LEFTINT])
		{
		case GOLD:
		case VACANT:
		case CHOICE:
		case POTION:
		case TREASURE:

			/* erase where the pacman went */
			PLOT(tmpy, tmpx, VACANT);
			pacptr->xpos += LEFTINT;
			break;
		
		default:
			pacptr->dirn = DNULL;
			break;
		};
		break;
	case DRIGHT:
		if(tmpx == XWRAP)
		{
			/* erase where the pacman went */
			PLOT(tmpy, tmpx, VACANT);
			pacptr->xpos = 0;
			sqtype = VACANT;
			break;
		};
		pacsymb = PRIGHT;
		switch (sqtype = display[tmpy][tmpx + RIGHTINT])
		{
		case GOLD:
		case VACANT:
		case CHOICE:
		case POTION:
		case TREASURE:

			/* erase where the pacman went */
			PLOT(tmpy, tmpx, VACANT);
			pacptr->xpos += RIGHTINT;
			break;

		default:
			pacptr->dirn = DNULL;
			break;
		};
		break;
	};

	/* did the pacman get any points or eat a potion? */
	switch (sqtype)
	{
	case CHOICE:
	case GOLD:
		pscore++;
		goldcnt--;
		break;

	case TREASURE:
		pscore += TREASVAL;
		break;

	case POTION:
		SPLOT(3, 45, "COUNTDOWN: ");
		potion = TRUE;
		potioncnt = POTINTVL;
		pacptr->speed = FAST;
		pacptr->danger = TRUE;

		/* slow down monsters and make them harmless */
		mptr = &monst[0];
		for (mcnt = 0; mcnt < MAXMONSTER; mcnt++)
		{
			if (mptr->stat == RUN)
			{
				mptr->speed = SLOW;
				mptr->danger = FALSE;
			};
			mptr++;
		};
		break;
	};

	/* did the pacman run into a monster? */
	for (mptr = &monst[0], mcnt = 0; mcnt < MAXMONSTER; mptr++, mcnt++)
	{
		if ((mptr->xpos == pacptr->xpos) &&
			(mptr->ypos == pacptr->ypos))
		{

			killflg = dokill(mcnt);
		}
		else
		{
			killflg = FALSE;
		};
	};
	if (killflg != TURKEY)
	{
		PLOT(pacptr->ypos, pacptr->xpos, pacsymb);
	};
}

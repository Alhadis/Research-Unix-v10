/*
 *	storage  file for pengo.	BLIT version
 */

#include "pengo.h"
/* externs for files */
int	pengo_left;	/* number of pengos left */
int	sno_bee_left;	/* number of sno_bees left */
int	egg_count_flag;	/* number of eggs still to hatch */
int	manptr;		/* index into new_man */
int	hit;		/* state of button 2 */
int	seggs;		/* eggs on display ? */
char	move_water;	/* side water movement */
char	sdiamond;	/* 10000 points flag */
char	s[MAXX][MAXY];	/* storage for which blocks are on */
long	score;		/* current score */
long	old_score;	/* previous score */
long	high_score;	/* current high score */
long	my_time;	/* 60th of a second counter for play time */
element	pengo;		/* pengo definition */
element	sno_bee[4];	/* which blocks are really sno_bees */
element	eggs[MAX_BEES];	/* which blocks are eggs */
element	diamond[3];	/* which blocks are really diamonds */
element	moving;		/* block which is moving or breaking */
element	breaking;	/* block which is breaking */
element	bee_block[4];	/* blocks being broken by bees */
Point	old_mouse;	/* initial mouse position */
Point	new_mouse;	/* current mouse position */
Point	move_mouse;	/* difference of old & new */
Point	home_mouse;	/* home position of mouse */
Rectangle	board;
Rectangle	board0;
Rectangle	board1;
Rectangle	header;
Rectangle	bwater;
Rectangle	lwater;
Rectangle	rwater;
Rectangle	twater;
Rectangle	counter;
int	sbd;
int	npd;
int	bee_run;

long	new_man[] = {
	10000L, 20000L, 50000L, 100000L, 200000L, 400000L, 800000L,
};

Word	bstore[8 * 48];	/* storage, requires an even number as x, for jerq */
		/* 10000 point bonus bitmap */
Bitmap	bonus	= { (Word *)bstore, shorts(8), { { 0, 0 }, { 16 * 7, 48 } } };
#ifndef	MPX
int	reshaped;
#endif
int	sb_change;
int	sb_random;
int	sb_break;

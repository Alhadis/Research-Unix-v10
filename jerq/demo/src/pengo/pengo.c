/*
	main routine in PENGO!!!!!

*/
#include "pengo.h"

main()
{
	int	level = 0;

#ifndef	JERQ
	*(char *)(384*1024L+040) = 1;	/* set to Black on White */
#endif	JERQ
	sb_change = SNO_BEE_CHANGE;
	sb_random = SNO_BEE_RANDOM;
	sb_break  = SNO_BEE_BREAK;
	pengo_left = NUM_PENGO;
	srand((int)realtime());	/* set up random number thingy */
	my_time = 0;
	init();
	{
		/* do static display */
		
		if (
			((board.corner.x - board.origin.x) > HEAD_X) &&
			((board.corner.y - board.origin.y) > HEAD_Y)
		)
		{
			rectf(&display, board, F_CLR);
			bitblt(&disp_head, disp_head.rect,
				&display,
				add(
				    div(
					sub(
					    sub(
						board.corner,
						board.origin
					    ),
					    Pt(HEAD_X, HEAD_Y)
					),
					2
				    ),
				    board.origin
				), F_STORE);
#ifdef	JERQ
#ifdef	MPX
			if (!VALSCREENCOLOR)	/* change the screen shade */
				rectf(&display, board, F_XOR);
#endif	MPX
#endif	JERQ
		}
		while(!((own() & MOUSE) && button123()))
		{
			rand();		/* roll random number generator */
			wait(CPU);
		}
		texture(&display, Drect, char_space, F_STORE);
	}
	sno_bee_left = (level < (MAX_BEES - 9)) ? level + 8 : MAX_BEES;
	init_scr();
	init_board();
	score = 0;
	for(;;)
	{
		int	screen_time;

		pengo.exists = TRUE;	/* pengo is now alive */
		add_event(MOVE_PENGO, move_player, NULL);
		while (next_event())
		{
#ifdef	MPX
			if (P->state & RESHAPED)
			{
				level = -1;
				changed_screen();
				pengo_left = NUM_PENGO;
			}
#else
			if (reshaped == TRUE)
			{
				level = -1;
				changed_screen();
				pengo_left = NUM_PENGO;
			}
#endif
		}
		if ((screen_time = my_time / 60) < 0)
			screen_time = 0;
		if (pengo.exists == TRUE)
		{
			/* player was alive at the end of that one */
			if (level >= 0)
			{
				extra_score(screen_time);
				if (level++ & 1)
				{
					/* do dance of the levels */
					level_dance(level);
				}
			}
			else
				level = 0;
			sno_bee_left = (level < (MAX_BEES - 9))?level+8 : MAX_BEES;
			my_time = 0;
			init_scr();
			init_board();
		}
		else	/* player was killed */
			if (pengo_left == -1)
			{
				/* all dead now */
				newgame();	/* new game ? */
				my_time = 0;
				level = 0;
				pengo_left = NUM_PENGO;
				sno_bee_left = (level < (MAX_BEES - 9)) ?
					level + 8 : MAX_BEES;
				init_scr();
				init_board();
				put_score(score = 0);
			}
			else
			{
				init_scr();
				redraw_screen();
			}
		breaking.exists = FALSE;	/* if we were breaking a block */
	}
}

Word *
water(x)
int x;
{
	switch(x)
	{
	case STILL:
		return (Word *)nw;

	case UP:
		return (Word *)tw;

	case DOWN:
		return (Word *)bw;

	case LEFT:
		return (Word *)lw;

	case RIGHT:
		return (Word *)rw;
	}
	return (Word *)nw;
}

put_char(x, y, ch, format, other)
int	x;
int	y;
int	ch;
int	format;
int	other;
{
	extern	Bitmap	bit32;
	int	tmp;

#ifdef	MPX
	if (P->state & RESHAPED)
		return;
#else
	if (reshaped == TRUE)
		return;
#endif
	switch(ch)
	{
	case BLANK:
		bit32.base = (Word *)char_blank;
		break;

	case BLOCK:
		bit32.base = (Word *)char_block;
		break;

	case FBLOCK:
		bit32.base = (Word *)flip_block;
		break;

	case PENGO:
		if (pengo.movex)
			if (pengo.movex < 0)
				tmp = 1;
			else
				tmp = 3;
		else
			if (pengo.movey < 0)
				tmp = 5;
			else if (pengo.movey)
				tmp = 7;
			else
				tmp = 0;
		if ((pengo.x & 16) || (pengo.y & 16))
			tmp++;
		bit32.base = (Word *)pengo_moves[tmp];	
		break;

	case PENGO_D:
		bit32.base = (Word *)(other ? char_1dead : char_0dead);
		break;

	case DIAMOND:
		bit32.base = (Word *)(other ? char_1diamond : char_0diamond);
		break;

	case SNO_BEE:
		bit32.base = (Word *)((
				(sno_bee[other].x & 16) |
				(sno_bee[other].y & 16)
			     ) ? char_1bee : char_0bee);
		x += (other>> 1) & 1;
		y += other & 1;
		break;

	case EGG:
		bit32.base = (Word *)(seggs ? char_egg : flip_block);
		break;

	case HATCH:
		bit32.base = (Word *)char_egg;
		break;

	case CONF:
		bit32.base = (Word *)((other & 1) ? conf1 : conf2);
		x += (other >> 2) & 1;
		y += (other >> 1) & 1;
		break;

	default:
		bit32.base = (Word *)char_null;
		break;

/* others here ... */
	}
	bitblt(&bit32, Rect(0, 0, 32, 32), &display,
		add(board.origin, Pt(x, y)), format);
}

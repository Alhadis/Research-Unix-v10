/*
	Initialisation routines for various stages...

*/
#include "pengo.h"

int	sizex;	/* number of pixels across */
int	sizey;	/* number of pixels down */
char	blockx;	/* number of blocks across */
char	blocky;	/* number of blocks down */
char	count;
char	pos;
char	tmp;
char	store[4];
char	store1[MAX_X * MAX_Y][2];
int	point1;
int	block_count;
extern	Rectangle	mouse_board;

Rectangle	board	= { { 0, 0 }, { 0, 0 } };	/* inside water boundary */
Rectangle	board0	= { { 0, 0 }, { 0, 0 } };	/* whole board */
Rectangle	board1	= { { 0, 0 }, { 0, 0 } };	/* board excluding score */
Rectangle	header	= { { 0, 0 }, { 0, 0 } };
Rectangle	bwater	= { { 0, 0 }, { 0, 0 } };
Rectangle	lwater	= { { 0, 0 }, { 0, 0 } };
Rectangle	rwater	= { { 0, 0 }, { 0, 0 } };
Rectangle	twater	= { { 0, 0 }, { 0, 0 } };
Rectangle	pengom	= { { 0, 0 }, { 0, 0 } };
Rectangle	sno_bm	= { { 0, 0 }, { 0, 0 } };
Rectangle	dboard	= { { 0, 0 }, { 0, 0 } };

changed_screen()
{
#ifdef	MPX
	if (P->state & RESHAPED)
	{
		init();
		init_scr();
		P->state &= ~RESHAPED;
	}
#else
	init();
	init_scr();
	reshaped = FALSE;
#endif
}

init()
{
	cursswitch(char_space);
	cancel_events();
	while(TRUE)
	{
		score = 0;	/* reset score */
		my_time = 0;	/* reset pseudo clock */
		cancel_events();
		request(KBD | MOUSE);
#ifdef	MPX
		P->state &= ~RESHAPED;
#else
		reshaped = FALSE;
#endif
		sizex = Drect.corner.x - Drect.origin.x;
		sizey = Drect.corner.y - Drect.origin.y;
		if ((sizex < MINX) || (sizey < MINY))
		{
			string(&defont, "Pengo:", &display,
				Drect.origin, F_XOR);
			string(&defont, "Reshape", &display,
				add(Drect.origin, Pt(0, 12)), F_XOR);
		}
		else
			break;	/* screen is of an adequate size */
		request(KBD);
#ifdef	MPX
		P->state &= ~RESHAPED;
		while(!(P->state & RESHAPED))
		{
			int c;

			if (((c = kbdchar()) == 'q') || (c == 'Q'))
				exit(0);
			wait(CPU);	/* let others have a go */
		}
#endif
	}
	texture(&display, Drect, char_space, F_STORE);
	sizex &= ~31;	/* clear off lower bits */
	sizey &= ~31;	/* same thing... */
	if (!(sizey & 32))	/* if sizey is even take off 32 */
		sizey -= 32;
	if (sizex & 32)		/* if sizex is odd take off 32 */
		sizex -= 32;
	board.corner.x  = Drect.origin.x + sizex - 16;
	board.corner.y  = Drect.origin.y + sizey - 16;
	board.origin.x  = Drect.origin.x + 16;
	board.origin.y  = Drect.origin.y + CHAR_HEIGHT + 16;
	board0.corner.x = Drect.origin.x + sizex;
	board0.corner.y = Drect.origin.y + sizey;
	board0.origin.x = Drect.origin.x;
	board0.origin.y = Drect.origin.y;
	board1.corner.x = Drect.origin.x + sizex;
	board1.corner.y = Drect.origin.y + sizey;
	board1.origin.x = Drect.origin.x;
	board1.origin.y = Drect.origin.y + CHAR_HEIGHT;
	bwater.corner.x = Drect.origin.x + sizex - 16;
	bwater.corner.y = Drect.origin.y + sizey;
	bwater.origin.x = Drect.origin.x + 16;
	bwater.origin.y = Drect.origin.y + sizey - 16;
	lwater.corner.x = Drect.origin.x + 16;
	lwater.corner.y = Drect.origin.y + sizey - 16;
	lwater.origin.x = Drect.origin.x;
	lwater.origin.y = Drect.origin.y + CHAR_HEIGHT + 16;
	rwater.corner.x = Drect.origin.x + sizex;
	rwater.corner.y = Drect.origin.y + sizey - 16;
	rwater.origin.x = Drect.origin.x + sizex - 16;
	rwater.origin.y = Drect.origin.y + CHAR_HEIGHT + 16;
	twater.corner.x = Drect.origin.x + sizex - 16;
	twater.corner.y = Drect.origin.y + CHAR_HEIGHT + 16;
	twater.origin.x = Drect.origin.x + 16;
	twater.origin.y = Drect.origin.y + CHAR_HEIGHT;
	texture(&display, board0, char_space, F_STORE);
	home_mouse = div(add(board.origin, board.corner), 2);
	mouse_board = inset(board0, 16 * 7);
	counter.origin = div(
		sub(
			add(board.origin, board.corner),
			Pt(16 * 7, 0)
		),
		2
	);
	counter.corner = add(counter.origin, Pt(16 * 7, 48));
	{
		/* fix up the screen header */

		int	hx;	/* # chars */

		hx = ((Drect.corner.x - Drect.origin.x) >> 5) - NUM_DIGITS;
		header.origin.x = Drect.origin.x + (hx / 2) * CHAR_WIDTH;
		header.origin.y = Drect.origin.y;
		header.corner.x = header.origin.x + NUM_DIGITS * CHAR_WIDTH;
		header.corner.y = Drect.origin.y + CHAR_HEIGHT;
		pengom.origin.x = Drect.origin.x;
		pengom.origin.y = Drect.origin.y;
		pengom.corner.x = header.origin.x;
		pengom.corner.y = Drect.origin.y + CHAR_HEIGHT;
		sno_bm.origin.x = header.corner.x;
		sno_bm.origin.y = Drect.origin.y;
		sno_bm.corner.x = Drect.origin.x + sizex;
		sno_bm.corner.y = Drect.origin.y + CHAR_HEIGHT;
		npd = hx / 2;
		sbd = ((sno_bm.corner.x - sno_bm.origin.x) >> 5) - 1;
	}
	dboard.origin = board.origin;
	dboard.corner = sub(board.corner, Pt(32, 32));
}

extern	Bitmap	bit16;

init_scr()	/* set up the screen, colour in the bits that need it! */
{
	Word	*water();

	bit16.base = water(STILL);
	texture(&display, raddp(bit16.rect, board1.origin), bit16.base, F_STORE);
	texture(&display,
		raddp(
			bit16.rect,
			add(Drect.origin, Pt(0, sizey - 16))
		),
		bit16.base,
		F_STORE
	);
	texture(&display,
		raddp(
			bit16.rect,
			add(Drect.origin, Pt(sizex - 16, 32))
		),
		bit16.base,
		F_STORE
	);
	texture(&display, raddp(bit16.rect, board.corner), bit16.base, F_STORE);
	texture(&display, twater, bit16.base, F_STORE);
	texture(&display, bwater, bit16.base, F_STORE);
	texture(&display, rwater, bit16.base, F_STORE);
	texture(&display, lwater, bit16.base, F_STORE);
	texture(&display, board,  char_space, F_STORE);
	move_water = FALSE;
}

init_board()
{
	int	x;
	int	y;

	disp_pengo(pengo_left);
	disp_sno_bee(sno_bee_left);
	sdiamond = FALSE;		/* all diamonds are active */
	blockx = ((sizex - 32) >> 5);
	blocky = ((sizey - 64) >> 5);
	for (x = -2; x < blockx + 2; x++)
	{
		screen(x, -2) = WALL;
		screen(x, -1) = BORDER;
		screen(x, blocky) = BORDER;
		screen(x, blocky + 1) = WALL;
	}
	for (y = -1; y < blocky + 1; y++)
	{
		screen(-2, y) = WALL;
		screen(-1, y) = BORDER;
		screen(blockx, y) = BORDER;
		screen(blockx + 1, y) = WALL;
	}
	for (x = 0; x < blockx; x++)
	{
		for (y = 0; y < blocky; y++)
		{
			screen(x, y) = BLOCK;
			put_char(x << 5, y << 5, BLOCK, F_STORE, NULL);
		}
	}
	screen(0, blocky - 1) = BLANK;
	put_char(0, (blocky - 1) << 5, BLANK, F_STORE, NULL);
	store[0] = LEFT;
	store[1] = RIGHT;
	store[2] = UP;
	store[3] = DOWN;
	for (count = 3; count > 0; count--)
	{
		pos = rand() % (count + 1);
		tmp = store[pos];
		store[pos] = store[count];
		store[count] = tmp;
	}
	point1 = 0;
	maze(0, blocky - 1);
	add_a_pengo();
	set_up_diamonds();
	set_up_eggs();
	moving.exists = FALSE;
	bee_run = FALSE;
	cursswitch(char_space);
}

add_a_pengo()
{
	pengo.other = ALIVE;	/* he hasn't died yet, just born */
	pengo.x = blockx / 2;	/* lets put the pengo on the screen */
	pengo.y = blocky / 2;
	pengo.movex = pengo.movey = 0;	/* not moving yet */
	if (screen(pengo.x, pengo.y) != BLANK)
	{
		for (tmp = 0; tmp < 4; tmp++)
		{
			switch(store[tmp])
			{
			case UP:
				if (screen(pengo.x - 1, pengo.y) == BLANK)
				{
					pengo.x--;
					tmp = 4;
				}
				break;
			case DOWN:
				if (screen(pengo.x + 1, pengo.y) == BLANK)
				{
					pengo.x++;
					tmp = 4;
				}
				break;
			case RIGHT:
				if (screen(pengo.x, pengo.y + 1) == BLANK)
				{
					pengo.y++;
					tmp = 4;
				}
				break;
			case LEFT:
				if (screen(pengo.x, pengo.y - 1) == BLANK)
				{
					pengo.y--;
					tmp = 4;
				}
				break;
			}
		}
	}
	if (screen(pengo.x, pengo.y) != BLANK)
	{
		for (tmp = 0; tmp < 4; tmp++)
		{
			switch(store[tmp])
			{
			case UP:
				if (screen(pengo.x - 1, pengo.y - 1) == BLANK)
				{
					pengo.x--;
					pengo.y--;
					tmp = 4;
				}
				break;
			case DOWN:
				if (screen(pengo.x - 1, pengo.y + 1) == BLANK)
				{
					pengo.x--;
					pengo.y++;
					tmp = 4;
				}
				break;
			case RIGHT:
				if (screen(pengo.x + 1, pengo.y - 1) == BLANK)
				{
					pengo.x++;
					pengo.y--;
					tmp = 4;
				}
				break;
			case LEFT:
				if (screen(pengo.x + 1, pengo.y + 1) == BLANK)
				{
					pengo.x++;
					pengo.y++;
					tmp = 4;
				}
				break;
			}
		}
	}
	if (screen(pengo.x, pengo.y) != BLANK)
	{
		for (tmp = 0; tmp < 4; tmp++)
		{
			switch(store[tmp])
			{
			case UP:
				if (screen(pengo.x - 1, pengo.y) == BLOCK)
				{
					pengo.x--;
					tmp = 4;
				}
				break;
			case DOWN:
				if (screen(pengo.x + 1, pengo.y) == BLOCK)
				{
					pengo.x++;
					tmp = 4;
				}
				break;
			case RIGHT:
				if (screen(pengo.x, pengo.y + 1) == BLOCK)
				{
					pengo.y++;
					tmp = 4;
				}
				break;
			case LEFT:
				if (screen(pengo.x, pengo.y - 1) == BLOCK)
				{
					pengo.y--;
					tmp = 4;
				}
				break;
			}
		}
		if (screen(pengo.x, pengo.y) == BLOCK)
		{
			put_char(pengo.x<<5, pengo.y<<5, FBLOCK, F_XOR, NULL);
			screen(pengo.x, pengo.y) = BLANK;
		}
	}
	if (screen(pengo.x, pengo.y) != BLANK)
	{
		for (tmp = 0; tmp < 4; tmp++)
		{
			switch(store[tmp])
			{
			case UP:
				if (screen(pengo.x - 1, pengo.y - 1) == BLOCK)
				{
					pengo.x--;
					pengo.y--;
					tmp = 4;
				}
				break;
			case DOWN:
				if (screen(pengo.x - 1, pengo.y + 1) == BLOCK)
				{
					pengo.x--;
					pengo.y++;
					tmp = 4;
				}
				break;
			case RIGHT:
				if (screen(pengo.x + 1, pengo.y - 1) == BLOCK)
				{
					pengo.x++;
					pengo.y--;
					tmp = 4;
				}
				break;
			case LEFT:
				if (screen(pengo.x + 1, pengo.y + 1) == BLOCK)
				{
					pengo.x++;
					pengo.y++;
					tmp = 4;
				}
				break;
			}
		}
		if (screen(pengo.x, pengo.y) == BLOCK)
		{
			put_char(pengo.x<<5, pengo.y<<5, FBLOCK, F_XOR, NULL);
			screen(pengo.x, pengo.y) = BLANK;
		}
	}
	if (screen(pengo.x, pengo.y) != BLANK)
	{
		/* blast it all, in those 9 spots not one was free
			so I'll just take it anyway!!!	*/
		screen(pengo.x, pengo.y) = BLANK;
		put_char(pengo.x<<5, pengo.y<<5, BLANK, F_STORE, NULL);
	}
	pengo.x <<= 5;
	pengo.y <<= 5;
	put_char(pengo.x, pengo.y, PENGO, F_XOR, NULL);
	old_mouse = mouse.xy;
}

maze(x, y)
int x;
int y;
{
	char	x1;
	char	y1;
	char	count;

nasty1:
	sleep(2);
	for (count = 3; count > 0; count--)
	{
		pos = rand() % (count + 1);
		tmp = store[pos];
		store[pos] = store[count];
		store[count] = tmp;
	}
	for (count = 0; count < 4; count++)
	{
nasty2:
		pos = 0;
	 	x1 = x;
		y1 = y;
		switch (store[count])
		{
		case RIGHT:
			if (screen(x + 2, y) == BLOCK)
			{
				screen(++x1, y) = BLANK;
				put_char(x1 << 5, y1 << 5, BLANK, F_STORE, NULL);
				screen(++x1, y) = BLANK;
				put_char(x1 << 5, y1 << 5, BLANK, F_STORE, NULL);
				pos++;
			}
			break;
		case LEFT:
			if (screen(x - 2, y) == BLOCK)
			{
				screen(--x1, y) = BLANK;
				put_char(x1 << 5, y1 << 5, BLANK, F_STORE, NULL);
				screen(--x1, y) = BLANK;
				put_char(x1 << 5, y1 << 5, BLANK, F_STORE, NULL);
				pos++;
			}
			break;
		case UP:
			if (screen(x, y - 2) == BLOCK)
			{
				screen(x, --y1) = BLANK;
				put_char(x1 << 5, y1 << 5, BLANK, F_STORE, NULL);
				screen(x, --y1) = BLANK;
				put_char(x1 << 5, y1 << 5, BLANK, F_STORE, NULL);
				pos++;
			}
			break;
		case DOWN:
			if (screen(x, y + 2) == BLOCK)
			{
				screen(x, ++y1) = BLANK;
				put_char(x1 << 5, y1 << 5, BLANK, F_STORE, NULL);
				screen(x, ++y1) = BLANK;
				put_char(x1 << 5, y1 << 5, BLANK, F_STORE, NULL);
				pos++;
			}
			break;
		}
		if (pos)
		{
			store1[point1][0] = x;
			store1[point1][1] = y;
			x = x1;
			y = y1;
			point1++;
			goto nasty1;
		}
#ifdef	MPX
		if (P->state & RESHAPED)
			return;
#endif
	}
	if (point1--)
	{
		x = store1[point1][0];
		y = store1[point1][1];
		count  = 0;
		goto nasty2;
	}
}

set_up_diamonds()
{
	int	pos, x, y, z;

	block_count = 0;
	for (x = 1; x < blockx - 1; x++)
		for (y = 1; y < blocky - 1; y++)
			if (screen(x, y) == BLOCK)
				block_count++;
	for (z = 0; z < 3; z++)
	{
		pos = rand() % block_count--;
		for (x = 1; (x < blockx - 1) && (pos >= 0); x++)
			for (y = 1; (y < blocky - 1) && (pos >= 0); y++)
				if (screen(x, y) == BLOCK)
					pos--;
		x--;
		y--;
		diamond[z].x = x << 5;
		diamond[z].y = y << 5;
		diamond[z].exists = TRUE;
		diamond[z].other = 0;	/* diamond state */
		screen(x, y) = DIAMOND;
		put_char(diamond[z].x, diamond[z].y, FBLOCK, F_XOR, 0);
		put_char(diamond[z].x, diamond[z].y, DIAMOND, F_XOR, 0);
	}
}

set_up_eggs()
{
	int	pos, x, y, z;
	extern	int	egg_count_flag;

	egg_count_flag = 0;
	seggs = TRUE;	/* yes, show eggs */
	for (z = 0; z < MAX_BEES; z++)
	{
		eggs[z].exists = FALSE;	/* clear all eggs */
		eggs[z].other  = ALIVE;
	}
	for (z = 0; z < sno_bee_left; z++)
	{
		pos = rand() % block_count--;
		for (x = 1; (x < blockx - 1) && (pos >= 0); x++)
			for (y = 1; (y < blocky - 1) && (pos >= 0); y++)
				if (screen(x, y) == BLOCK)
					pos--;
		x--;
		y--;
		eggs[z].x = x << 5;
		eggs[z].y = y << 5;
		eggs[z].exists = TRUE;
		if (z > 3)
		{
			eggs[z].other = 0;	/* diamond state */
			screen(x, y) = EGG;
		}
		else
		{
			bee_block[z].exists = FALSE;
			eggs[z].exists = FALSE;
			sno_bee[z].exists = FALSE;
			sno_bee[z].other = HATCHING;
			sno_bee[z].count = NUM_HATCH + 1;
			sno_bee[z].x = eggs[z].x;
			sno_bee[z].y = eggs[z].y;
			screen(x, y) = HATCH;
			add_event(EGG_HATCH, egg_hatch, z);
			put_char(eggs[z].x, eggs[z].y, FBLOCK, F_XOR, 0);
		}
	}
	show_eggs();
}

redraw_screen()
{
	int	x;
	int	y;
	int	dx;
	int	dy;
	int	tmp;

	disp_pengo(pengo_left);
	for (tmp = 0; tmp < 3; tmp++)
	{
		diamond[tmp].x &= ~31;
		diamond[tmp].y &= ~31;
		screen(diamond[tmp].x>>5, diamond[tmp].y>>5) = DIAMOND;
	}
	for (tmp = 0; tmp < moving.count; tmp++)
	{
		int	pos;

		for (pos = 0; pos < MAX_BEES; pos++)
		{
			if (eggs[pos].exists == TRUE)
				break;
		}
		if (pos < MAX_BEES)
		{
			int	a;

			for (a = 0; a < 4; a++)
			{
				if (
					(sno_bee[a].exists == FALSE) &&
					(sno_bee[a].other != HATCHING)
				)
					break;
			}
			if (a < 4)
			{
				eggs[pos].exists = FALSE;
				sno_bee[a].other = HATCHING;
				sno_bee[a].count = NUM_HATCH;
				sno_bee[a].x = eggs[pos].x;
				sno_bee[a].y = eggs[pos].y;
				screen(sno_bee[a].x >> 5, sno_bee[a].y >> 5) =
					HATCH;
			}
		}
	}
	for (tmp = 0; tmp < MAX_BEES; tmp++)
		if (eggs[tmp].exists == TRUE)
		{
			eggs[tmp].x &= ~31;
			eggs[tmp].y &= ~31;
			eggs[tmp].other = ALIVE;
			screen(eggs[tmp].x>>5, eggs[tmp].y>>5) = EGG;
		}
	for (tmp = 0; tmp < 4; tmp++)
		if (
			(sno_bee[tmp].exists == FALSE) &&
			(sno_bee[tmp].other == HATCHING)
		)
		{
			sno_bee[tmp].exists = TRUE;
			sno_bee[tmp].other = SNO_BEE;
			screen(sno_bee[tmp].x >> 5, sno_bee[tmp].y >> 5) = BLANK;
		}
	for (x = 0; x < blockx; x++)
		for(y = 0; y < blocky; y++)
			switch(screen(x, y))
			{
			case EGG:
			case BLOCK:
				put_char(x<<5, y<<5, BLOCK, F_STORE, NULL);
				break;

			case DIAMOND:
				put_char(x<<5, y<<5, DIAMOND, F_XOR, NULL);
				put_char(x<<5, y<<5, BLANK, F_XOR, NULL);
				break;

			case BREAK:
				screen(x, y) = BLANK;
			case BLANK:
				put_char(x<<5, y<<5, BLANK, F_STORE, NULL);
				break;

			default:	/* catch for bad things */
				put_char(x<<5, y<<5, WALL, F_STORE, NULL);
				break;
			}
	breaking.exists = FALSE;
	moving.exists = FALSE;
	egg_count_flag = 0;
	show_eggs();
	add_a_pengo();
	x = -1;
	for (tmp = 0; tmp < 4; tmp++)
		if (sno_bee[tmp].exists == TRUE)
		{
			switch(++x)
			{
			case 0:
				dx = 0;
				dy = 0;
				break;

			case 2:
				dx = blockx - 1;
				dy = 0;
				break;

			case 1:
				dx = 0;
				dy = blocky - 1;
				break;

			case 3:
				dx = blockx - 1;
				dy = blocky - 1;
				break;
			}
			add_sno_bee(dx, dy, x, tmp);
		}
	if (x == 0)
		add_event(RUN_DELAY, run_bee, NULL);
}

int	sdir[4][2] = {
	{1, -1},
	{1, 1},
	{-1, -1},
	{-1, 1},
};

add_sno_bee(dx, dy, x, num)
int	dx;
int	dy;
int	x;
int	num;
{
	int	mx = sdir[x][0];
	int	my = sdir[x][1];
	int	mn = min(blockx, blocky);
	int	tmp;
	for (tmp = 0; tmp < mn; tmp++)
	{
		int	tmp1;

		for(tmp1 = 0; tmp1 <= tmp; tmp1++)
		{
			if (screen (dx + mx * tmp1, dy + my * tmp) == BLANK)
			{
				sno_bee[num].x = (dx + mx * tmp1) << 5;
				sno_bee[num].y = (dy + my * tmp) << 5;
				sno_bee[num].other = SNO_BEE;
				add_event(MOVE_BEE0, move_sno_bee, num);
				put_char(
					sno_bee[num].x,
					sno_bee[num].y,
					SNO_BEE,
					F_XOR, num);
				return;
			}
			if (screen (dx + mx * tmp, dy + my * tmp1) == BLANK)
			{
				sno_bee[num].x = (dx + mx * tmp) << 5;
				sno_bee[num].y = (dy + my * tmp1) << 5;
				sno_bee[num].other = SNO_BEE;
				add_event(MOVE_BEE0, move_sno_bee, num);
				put_char(
					sno_bee[num].x,
					sno_bee[num].y,
					SNO_BEE,
					F_XOR, num);
				return;
			}
		}
	}
}

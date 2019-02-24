/*
	This file contains the code to control the players movements
*/

#include "pengo.h"
#define	MMX	(pengo.movex = (move_mouse.x > 0)?-8:8)
#define	MMY	(pengo.movey = (move_mouse.y > 0)?-8:8)

Rectangle	mouse_board;
extern int autoflag;
move_player()
{
	int	push;	/* keyboard push */
	int	type0;
	int	tmp;

	if (pengo.other == DYING)	/* it has hit a sno_bee */
		return;			/* this can overlap with die_pengo */
	put_char(pengo.x, pengo.y, PENGO, F_XOR, NULL);
	if (
		((pengo.x & 31) == 0) &&
		((pengo.y & 31) == 0)
	)
	{
		push = FALSE;
		new_mouse = mouse.xy;		/* get current location */
		move_mouse = sub(old_mouse, new_mouse);
		move_mouse.x /= 32;
		move_mouse.y /= 32;
		if (move_mouse.x | move_mouse.y)
			cursset(new_mouse = home_mouse);
		if (own() & KBD)
		{
			/* keyboard movement routine */

			push = TRUE;
			switch (kbdchar())
			{
			case 'H':
				push = TRUE;
			case 'h':
				move_mouse.x = 8;
				move_mouse.y = 0;
				break;

			case 'J':
				push = TRUE;
			case 'j':
				move_mouse.x = 0;
				move_mouse.y = -8;
				break;

			case 'K':
				push = TRUE;
			case 'k':
				move_mouse.x = 0;
				move_mouse.y = 8;
				break;

			case 'L':
				push = TRUE;
			case 'l':
				move_mouse.x = -8;
				move_mouse.y = 0;
				break;

			case ' ':
				hit = TRUE;		/* stop pengo */
				move_mouse.x = 0;
				move_mouse.y = 0;
				break;

			default:
				;
				/* nothing, just throw the key away */
			}
		}
		if(autoflag)
			push = TRUE, scott();
		{
			int	tmp;
			int	tmp0;

			tmp  = FALSE;
			tmp0 = FALSE;
			if (button1())
			{
				put_char(pengo.x, pengo.y, PENGO, F_XOR, NULL);
				tmp = TRUE;
			}
			while (button1() || hit)
			{
				if (button1() && (!tmp))
				{
					put_char(pengo.x, pengo.y, PENGO, F_XOR, NULL);
					tmp = TRUE;
				}
				tmp0 = TRUE;
				wait(CPU);
				hit = FALSE;
			}
			if (tmp)
				put_char(pengo.x, pengo.y, PENGO, F_XOR, NULL);
			if (tmp0)
			{
				pengo.movex = pengo.movey = 0;
				move_mouse.x = move_mouse.y = 0;
				old_mouse = mouse.xy;
			}
		}
		if ((move_mouse.y | move_mouse.x) != 0)
		{
			if (!(ptinrect(mouse.xy, mouse_board)))
				cursset(old_mouse = home_mouse);
			else
				old_mouse = new_mouse;
			pengo.movex = pengo.movey = 0;
			if (abs(move_mouse.y) == abs(move_mouse.x))
				if (pengo.movex)
					MMX;
				else if (pengo.movey)
					MMY;
				else if (rand() & 1)	/* Random direction */
					MMX;
				else
					MMY;
			else if (abs(move_mouse.x) > abs(move_mouse.y))
				MMX;
			else
				MMY;
		}
		if (
			((pengo.movex) || (pengo.movey)) &&
			screen_blank(
				((pengo.x + 4 * pengo.movex) >> 5),
				((pengo.y + 4 * pengo.movey) >> 5)
			)
		)
		{
			pengo.x += pengo.movex;
			pengo.y += pengo.movey;
		}
		else if (!(button2() || push))	/* stop or hit? */
		{
			pengo.movex = pengo.movey = 0;
		}
		else
			/* try to move or destroy block */

			switch (type0 = screen(
				(pengo.x + 4 * pengo.movex) >> 5,
				(pengo.y + 4 * pengo.movey) >> 5)
			)
			{
			case BLANK:
			default:
				/* should never occur */
				break;
			case WALL:
			case BORDER:
				if (move_water)
					break;
				move_water = pengo.movex ?
					(pengo.movex < 0) ? LEFT : RIGHT :
					(pengo.movey < 0) ? UP   : DOWN;
				swill_water(NUM_WATER);
				break;
			case PENGO:
			case HATCH:
			case BREAK:
				/* sorry, cannot move it .. */
				break;
			case SNO_BEE:
				/* He'll get it shortly. */
				break;
			case DIAMOND:
			case EGG:
			case BLOCK:
				/* ok, lets try to move it */
				if (
					screen(
						(pengo.x + 8 * pengo.movex) >> 5,
						(pengo.y + 8 * pengo.movey) >> 5
					)
					==
					BLANK
				)
				{
					int	x;

					if (moving.exists)
						break;
					/* ok, we can move the block */
					moving.other = type0 == BLOCK ? FBLOCK
						: type0;
					moving.exists = TRUE;
					moving.movex = pengo.movex;
					moving.movey = pengo.movey;
					moving.x = pengo.x + 4 * pengo.movex;
					moving.y = pengo.y + 4 * pengo.movey;
					moving.count = 0;
					if (type0 == DIAMOND)
					{
						for (x = 0; x < 3; x++)
							if (
							(diamond[x].x == moving.x)
							&&
							(diamond[x].y == moving.y)
							)
								break;
					}
					if (type0 == EGG)
					{
						for (x = 0; x < MAX_BEES; x++)
							if (
							(eggs[x].x == moving.x)
							&&
							(eggs[x].y == moving.y)
							)
							{
								eggs[x].other =
									MOVING;
								break;
							}
					}
					add_event(MOVE_BLOCK,
						move_block, x);
					screen(moving.x >> 5,
						moving.y >> 5) = BLANK;
				}
				else switch(type0)
				{
					/* ok, lets break it, if we can! */
				case DIAMOND:
					/* no luck joe. */
					break;
				case EGG:
					if (breaking.exists)
						break;
					/* delete from egg lists */
					{
						int	x;	/* tmp */

						for (x = 0; x < MAX_BEES; x++)
							if (
								(eggs[x].exists ==
									TRUE
								) &&
								(eggs[x].x ==
									(pengo.x +
									4 *
									pengo.movex)
								) &&
								(eggs[x].y ==
									(pengo.y +
									4 *
									pengo.movey)
								)
							)
							{
							    eggs[x].exists = FALSE;
							    if (seggs)
							    {
								put_char(
									eggs[x].x,
									eggs[x].y,
									EGG,
									F_XOR, 0
								);
								put_char(
									eggs[x].x,
									eggs[x].y,
									FBLOCK,
									F_XOR, 0
								);
							    }
							    disp_sno_bee(
								--sno_bee_left);
							}
					}
				{
					int	new_b;

					add_score(
						new_b = (
							(((pengo.x + 4 * pengo.movex >> 5) & 31) << 8) |
							(((pengo.y + 4 * pengo.movey >> 5) & 31) << 3) |
							(5)
						)
					);
					add_event(SHOW_SCORE, add_score, new_b);
				}
				case BLOCK:
					if (breaking.exists)
						break;
					breaking.other = type0;
					breaking.exists = TRUE;
					breaking.count = NUM_BREAK;
					breaking.movex = pengo.movex;
					breaking.movey = pengo.movey;
					breaking.x = pengo.x + 4 * pengo.movex;
					breaking.y = pengo.y + 4 * pengo.movey;
					screen(
						(pengo.x + 4 * pengo.movex) >> 5,
						(pengo.y + 4 * pengo.movey) >> 5
					) = BREAK;
					add_event(DIE_BLOCK,
						die_block, NULL);
				}
			}
	}
	else
	{
		pengo.x += pengo.movex;
		pengo.y += pengo.movey;
	}
	for (tmp = 0; tmp < 4; tmp++)
	{
		if (
			(sno_bee[tmp].exists) &&
			PX(pengo, sno_bee[tmp])
		)
		{
			if (sno_bee[tmp].other == CONFUSED)
			{
				int	new_b;
				int	show;

				put_char(sno_bee[tmp].x, sno_bee[tmp].y,
					CONF, F_XOR, (sno_bee[tmp].count & 1) |
					(tmp << 1));
				sno_bee[tmp].exists = FALSE;
				sno_bee[tmp].other = SQUASH;
				score += 100;
				add_score(
					new_b = (
						(((sno_bee[tmp].x >> 5)& 31) << 8) |
						(((sno_bee[tmp].y >> 5)& 31) << 3) |
						((tmp & 3) << 13) |
						7
					)
				);
				add_event(SHOW_SCORE, add_score, new_b);
				show = FALSE;
				disp_sno_bee(--sno_bee_left);
				for (new_b = 0; new_b < 1; new_b++)
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
							if
							(
								(sno_bee[a].exists
								==
								FALSE ) &&
								(sno_bee[a].other
								!=
								HATCHING )
							)
							break;
						}
						if (a < 4)
						{
							eggs[pos].exists = FALSE;
							sno_bee[a].other = HATCHING;
							sno_bee[a].count=NUM_HATCH;
							sno_bee[a].x = eggs[pos].x;
							sno_bee[a].y = eggs[pos].y;
							screen(
								sno_bee[a].x >> 5,
								sno_bee[a].y >> 5
							) = HATCH;
							if (seggs)
							put_char(
								sno_bee[a].x,
								sno_bee[a].y,
								EGG,
								F_XOR,
								0
							);
							else
							put_char(
								sno_bee[a].x,
								sno_bee[a].y,
								FBLOCK,
								F_XOR,
								0
							);
							put_char(
								sno_bee[a].x,
								sno_bee[a].y,
								HATCH,
								F_XOR,
								0
							);
							add_event(
								EGG_HATCH,
								egg_hatch,
								a
							);
							show = TRUE;
						}
					}
					else
					{
						/* no eggs left */
						if (sno_bee_left <= 0)
						{
							/* all bees dead */
							cancel_events();
							return;
						}
						if (sno_bee_left == 1)
						{
							/* last one.. RUNNING */

							add_event(RUN_DELAY,
								run_bee, NULL);
						}
					}
				}
				if (show)
					show_eggs();
			}
			else if (sno_bee[tmp].other == SNO_BEE)
			{
				pengo.other = DYING;
				add_event(P_DYING, die_pengo, NULL);
			}
		}
	}
	put_char(pengo.x, pengo.y, PENGO, F_XOR, NULL);
	add_event(MOVE_PENGO, move_player, NULL);
}

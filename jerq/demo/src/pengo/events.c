/*
	This file contains the code to handle the actual events that occur
	such as pengo's dying, the score system, the bee system.

*/
#include "pengo.h"

int	bee_score[] = { 0, 400, 1600, 3200, 6400 };

die_pengo()
{
	int	x;

	pengo.exists = FALSE;	/* tis now dead */
	put_char(pengo.x, pengo.y, PENGO, F_XOR, NULL);
	disp_pengo(--pengo_left);
	for (x = 0; x < PENGO_THROES; x++)
	{
		put_char(pengo.x, pengo.y, PENGO_D, F_XOR, x & 1);
		sleep((int)PENGO_D_TIME);
		dead_pengo(pengo_left, x & 1);
		put_char(pengo.x, pengo.y, PENGO_D, F_XOR, x & 1);
	}
	disp_pengo(pengo_left);
	cancel_events();
}

add_man()
{
	disp_pengo(++pengo_left);
}

disp_sno_bee(num)
int	num;
{
	int	x;

	bit32.base = (Word *)sbee;
	rectf(&display, sno_bm, F_CLR);
	for (x = 0; (x < num) && (x < sbd); x++)
		bitblt(&bit32, R3, &display,
			Pt(sno_bm.origin.x + (x + 1) * CHAR_WIDTH, sno_bm.origin.y),
			F_STORE);
}

dead_pengo(num, other)
int	num;
int	other;
{
	int	x;

	bit32.base = (Word *)(other ? char_1dead : char_0dead);
	rectf(&display, pengom, F_CLR);
	for (x = 0; (x < num) && (x < npd); x++)
		bitblt(&bit32, R3, &display,
			Pt(pengom.origin.x + x * CHAR_WIDTH, pengom.origin.y),
			F_STORE);
}

disp_pengo(num)
int	num;
{
	int	x;

	bit32.base = (Word *)char_still;
	rectf(&display, pengom, F_CLR);
	for (x = 0; (x < num) && (x < npd); x++)
		bitblt(&bit32, R3, &display,
			Pt(pengom.origin.x + x * CHAR_WIDTH, pengom.origin.y),
			F_STORE);
}

add_score(num)
int	num;
{

	/*
		add_score's argument is composed of the following sixteen bits:

		Bits -	Function
		0 - 2	value to be added
		3 - 7	y posn of the block (in blocks)
		8 -12	x posn (in blocks)
		13-14	movement in case of score '100'
		15	unused

	*/

	int	x;
	int	y;

	x = ((num >> 8) & 31) << 5;
	y = ((num >> 3) & 31) << 5;
	switch(num & 7)
	{
	case 1:
		bit32.base = (Word *)char_400;
		break;

	case 2:
		bit32.base = (Word *)char_1600;
		break;

	case 3:
		bit32.base = (Word *)char_3200;
		break;

	case 4:
		bit32.base = (Word *)char_6400;
		break;

	case 5:
		bit32.base = (Word *)char_500;
		break;

	case 7:
		bit32.base = (Word *)char_100;
		x += (num >> 14) & 1;
		y += (num >> 13) & 1;
		break;

	default:
		bit32.base = (Word *)char_null;
		break;
	}
	bitblt(&bit32, R3, &display, add(board.origin, Pt(x, y)), F_XOR);
}

move_block(num)
int	num;
{
	int	tmp;
	int	x4 = ((moving.x + 4 * moving.movex) >> 5);
	int	y4 = ((moving.y + 4 * moving.movey) >> 5);

	if (((moving.x & 31) == 0) && ((moving.y & 31) == 0))
	{
		if (screen(x4, y4) == BLANK)
		{
			put_squash();
			put_char(moving.x, moving.y, moving.other, F_XOR, NULL);
			moving.x += moving.movex;
			moving.y += moving.movey;
			put_squash();
			put_char(moving.x, moving.y, moving.other, F_XOR, NULL);
		}
		else
		{
			int	new_b;
			int	show;

			show = FALSE;
			switch(screen(x4, y4))
			{
			case BLOCK:
			case WALL:
			case BORDER:
			case PENGO:
			case HATCH:
			case BREAK:
			case EGG:
			case DIAMOND:
			default:
				/* sorry, end of the line */
				put_squash();	/* turn off */
				moving.movex = moving.movey = 0;
				screen(
					moving.x >> 5,
					moving.y >> 5
				) =
				moving.other == FBLOCK ? BLOCK : moving.other;
				if (moving.other == EGG)
				{
					eggs[num].x = moving.x;
					eggs[num].y = moving.y;
					eggs[num].other = ALIVE;
				}
				score += bee_score[moving.count];
				add_score(
					new_b = (
						(((moving.x >> 5) & 31) << 8) |
						(((moving.y >> 5) & 31) << 3) |
						(moving.count & 7)
					)
				);
				add_event(SHOW_SCORE, add_score, new_b);
				for (new_b = 0; new_b < moving.count; new_b++)
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
								NULL
							);
							else
							put_char(
								sno_bee[a].x,
								sno_bee[a].y,
								FBLOCK,
								F_XOR,
								NULL
							);
							put_char(
								sno_bee[a].x,
								sno_bee[a].y,
								HATCH,
								F_XOR,
								NULL
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
						if (sno_bee_left == 0)
						{
							/* all bees dead */
							cancel_events();
						}
						if (sno_bee_left == 1)
						{
							/* last one.. RUNNING */

							add_event(RUN_DELAY,
								run_bee, NULL);
						}
					}
				}
				moving.count = 0;
				moving.exists = FALSE;
				if (show)
					show_eggs();
				if (moving.other == DIAMOND)
				{
					diamond[num].x = moving.x;
					diamond[num].y = moving.y;
					if (sdiamond == FALSE)
				{
					if((
						diamond[0].x == diamond[1].x &&
						diamond[1].x == diamond[2].x &&
						inline (
							diamond[0].y,
							diamond[1].y,
							diamond[2].y
						)
					) ||
					(
						diamond[0].y == diamond[1].y &&
						diamond[1].y == diamond[2].y &&
						inline (
							diamond[0].x,
							diamond[1].x,
							diamond[2].x
						)
					))
				{
					int	x;
					int	dir;

					if (
						inline (
							diamond[0].x,
							diamond[1].x,
							diamond[2].x
						)
					)
						dir = HORIZ;
					else
						dir = VERT;
					old_mouse = mouse.xy;
					sdiamond = TRUE;
					for (x = 0; x < NUM_FLASH; x++)
					{
						put_char(diamond[0].x,
							diamond[0].y,
							DIAMOND,
							F_XOR,
							1);
						put_char(diamond[1].x,
							diamond[1].y,
							DIAMOND,
							F_XOR,
							1);
						put_char(diamond[2].x,
							diamond[2].y,
							DIAMOND,
							F_XOR,
							1);
						sleep((int)FLASH_DIAMOND);
					}
					bitblt(&display,
						counter,
						&bonus,
						Pt(0, 0),
						F_STORE);
					rectf(&display, counter, F_CLR);
					addstr("BONUS", counter.origin, F_STORE);
					addstr("Pts.", add(counter.origin,
						Pt(48, 32)), F_STORE);
					if ((dir == VERT) ?
						((diamond[0].x == 0) ||
						(diamond[0].x == (blockx - 1) << 5))
						:
						((diamond[0].y == 0) ||
						(diamond[0].y == (blocky - 1) << 5))
					)
					{
						addstr(" 5000", add(counter.origin,
							Pt(16, 16)), F_STORE);
						x = 49;
					}
					else
					{
						addstr("10000", add(counter.origin,
							Pt(16, 16)), F_STORE);
						x = 99;
					}
					sleep(40);
					addchar(' ', add(counter.origin,
						Pt(16, 16)),
						F_STORE);
					for (; x >= 0; x--)
					{
						addchar((x/10)? '0' + (x/10) : ' ',
						 add(counter.origin, Pt(32, 16)),
						 F_STORE);
						addchar(x % 10 + '0',
						 add(counter.origin, Pt(48, 16)),
						 F_STORE);
						score += 100;
						put_score(score);
						sleep(2);
					}
					addstr("  ",add(counter.origin,
						Pt(48,16)), F_STORE);
					sleep(50);
					bitblt(&bonus,
						bonus.rect,
						&display,
						counter.origin,
						F_STORE);
					for (x = 0; x < 4; x++)
					{
					    if (sno_bee[x].exists == TRUE)
					    {
						switch(sno_bee[x].other)
						{
						case SNO_BEE:
							sno_bee[x].other = CONFUSED;
							put_char(sno_bee[x].x,
								sno_bee[x].y,
								SNO_BEE, F_XOR, x);
							put_char(sno_bee[x].x,
								sno_bee[x].y,
								CONF, F_XOR,
								(NUM_CONFUSED & 1) |
								(x << 1));
							sno_bee[x].count =
								NUM_CONFUSED;
							add_event(TIME_CONFUSED,
								confused_bee, x);
							break;

						case CONFUSED:
							sno_bee[x].other = SNO_BEE;
							put_char(sno_bee[x].x,
								sno_bee[x].y,
								CONF, F_XOR,
								(NUM_CONFUSED & 1) |
								(x << 1));
							put_char(sno_bee[x].x,
								sno_bee[x].y,
								SNO_BEE, F_XOR, x);
							if (bee_run)
								add_event(MOVE_BEE0,
									run_sno_bee,
									x);
							else
								add_event(MOVE_BEE0,
								       move_sno_bee,
									x);
							break;

						default:
							break;
						}
					    }
					}
				}
				else
					add_event(FLASH_DIAMOND,
						flash_diamond, NUM_FLASH);
				}
				}
				break;
			case BLANK:
				/* means a sno_bee is there, I'll get it shortly */
				break;
			case SNO_BEE:
				/* shouldn't happen but what the heck */
				break;
			}
		}
	}
	else
	{
		put_char(moving.x, moving.y, moving.other, F_XOR, 0);
		put_squash();
		moving.x += moving.movex;
		moving.y += moving.movey;
		put_char(moving.x, moving.y, moving.other, F_XOR, 0);
		put_squash();
	}
	for (tmp = 0; tmp < 4; tmp++)
		if (
			moving.exists &&
			(sno_bee[tmp].exists) &&
			RX(moving, sno_bee[tmp])
		)
		{
			if (sno_bee[tmp].other != CONFUSED)
				put_char(sno_bee[tmp].x, sno_bee[tmp].y,
					SNO_BEE, F_XOR, tmp);
			else
				put_char(sno_bee[tmp].x, sno_bee[tmp].y,
					CONF, F_XOR,
					(sno_bee[tmp].count & 1) | (tmp << 1));
			sno_bee[tmp].exists = FALSE;	/* it has gone */
			sno_bee[tmp].other = SQUASH;
			disp_sno_bee(--sno_bee_left);
			if (!moving.count++)
				put_squash();	/* only on first sno_bee */
		}
	if (moving.exists == TRUE)
		add_event(MOVE_BLOCK, move_block, num);
}

put_squash()
{
	/* squashed sno_bee on moving block */
	int	tmp;

	if (!moving.count)
		return;
	if (moving.movex)
		if (moving.movex < 0)
			tmp = 2;
		else
			tmp = 3;
	else
		if (moving.movey < 0)
			tmp = 0;
		else
			tmp = 1;
	if ((moving.x & 16) || (moving.y & 16))
		tmp += 4;
	bit32.base = (Word *)squash_moves[tmp];
	bitblt(&bit32, R3, &display,add(board.origin,Pt(moving.x,moving.y)),F_XOR);
}	

die_block()
{
	put_char(breaking.x, breaking.y, FBLOCK, F_XOR, NULL);
	if (--breaking.count)
	{
		add_event(DIE_BLOCK, die_block, NULL);
	}
	else
	{
		breaking.exists = FALSE;
		screen(breaking.x >> 5, breaking.y >> 5) = BLANK;
		if (breaking.other == EGG)
			score += 500;	/* egg score */
		else
			score += 30;	/* for breaking one */
	}
}

bee_break(x)
int	x;
{
	put_char(bee_block[x].x, bee_block[x].y, FBLOCK, F_XOR, NULL);
	if (--bee_block[x].count)
	{
		add_event(DIE_BLOCK, bee_break, x);
	}
	else
	{
		bee_block[x].exists = FALSE;
		screen(bee_block[x].x >> 5, bee_block[x].y >> 5) = BLANK;
	}
}

addstr(str, ptr, fmt)
char	*str;
Point	ptr;
int	fmt;
{
	while (*str)
	{
		addchar(*str++, ptr, fmt);
		ptr.x += 16;
	}
}

inline(a, b, c)
int	a;
int	b;
int	c;
{
	int	x;	/* lowest */
	int	y;
	int	z;	/* highest */

	if (a < b)
		if (a < c)
			if (c < b)
			{
				x = a;
				y = c;
				z = b;
			}
			else
			{
				x = a;
				y = b;
				z = c;
			}
		else
		{
			x = c;
			y = a;
			z = b;
		}
	else
		if (a < c)
		{
			x = b;
			y = a;
			z = c;
		}
		else
			if (c < b)
			{
				x = c;
				y = b;
				z = a;
			}
			else
			{
				x = b;
				y = c;
				z = a;
			}
	return (((x + 32) == y) && ((y + 32) == z));
}

egg_hatch(num)
int	num;
{
	if (--sno_bee[num].count)
	{
		put_char(sno_bee[num].x, sno_bee[num].y, HATCH, F_XOR, 1);
		add_event(EGG_HATCH, egg_hatch, num);
	}
	else
	{
		sno_bee[num].exists = TRUE;
		sno_bee[num].other = SNO_BEE;
		screen(sno_bee[num].x >> 5, sno_bee[num].y >> 5) = BLANK;
		put_char(sno_bee[num].x, sno_bee[num].y, SNO_BEE, F_XOR, num);
		add_event(MOVE_BEE0, move_sno_bee, num);
	}
}

clear_eggs()
{
	int	x;

	if (--egg_count_flag > 0)
		return;
	for (x = 0; x < MAX_BEES; x++)
	{
		if (eggs[x].exists)
			switch(eggs[x].other)
			{
			case ALIVE:
				put_char(eggs[x].x, eggs[x].y, EGG, F_XOR, 0);
				put_char(eggs[x].x, eggs[x].y, FBLOCK, F_XOR, 0);
				break;

			case MOVING:
				put_char(moving.x, moving.y, EGG, F_XOR, 0);
				put_char(moving.x, moving.y, FBLOCK, F_XOR, 0);
				break;

			default:
				/* no action */
				break;
			}
	}
	egg_count_flag = 0;
	seggs = FALSE;
}

show_eggs()
{
	int	x;

	if (egg_count_flag++ <= 0)
	{
		seggs = TRUE;
		for (x = 0; x < MAX_BEES; x++)
		{
			if (eggs[x].exists)
				switch(eggs[x].other)
				{
				case ALIVE:
					put_char(eggs[x].x, eggs[x].y, FBLOCK,
						F_XOR, 0);
					put_char(eggs[x].x, eggs[x].y, EGG,
						F_XOR, 0);
					break;

				case MOVING:
					put_char(moving.x, moving.y, FBLOCK,
						F_XOR, 0);
					put_char(moving.x, moving.y, EGG,
						F_XOR, 0);
					break;

				default:
				/* no action */
					break;
				}
		}
	}
	add_event(SHOW_EGGS, clear_eggs, NULL);
}

run_bee()
{
	bee_run = TRUE;
}

flash_diamond(num)
int	num;
{
	if (num--)
	{
		put_char(diamond[0].x, diamond[0].y, DIAMOND, F_XOR, 1);
		put_char(diamond[1].x, diamond[1].y, DIAMOND, F_XOR, 1);
		put_char(diamond[2].x, diamond[2].y, DIAMOND, F_XOR, 1);
		add_event(FLASH_DIAMOND, flash_diamond, num);
	}
}

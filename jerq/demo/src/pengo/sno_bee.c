/*
	This file contains the code required to move the sno_bees about
*/

#include "pengo.h"
#define	SB	sno_bee[bee_num]
#define	MW(x)	(((x < 0) || (x >= sizeof(move_weights)/sizeof(move_weights[0]))) ? 0 : move_weights[x])

int	move_weights[] = {
	10,	/* BLANK */
	5,	/* BLOCK */
	10,	/* SNO_BEE */
	100,	/* PENGO */
	0,	/* DIAMOND */
	0,	/* EGG */
	0,	/* HATCH */
	5,	/* BREAK ... same as BLOCK */
	10,	/* CONF ... same as SNO_BEE */
};

move_sno_bee(bee_num)
int	bee_num;
{
	if (SB.exists != TRUE)	/* let somebody else handle it!! */
		return;
	if (SB.other != SNO_BEE)
		return;
	if (((SB.x & 31) == 0) && ((SB.y & 31) == 0))
	{
		if ((rand() % 100) < sb_change)	/* how often to change */
		{
			if ((rand() % 100) < sb_random)
			{
				SB.movex = SB.movey = 0;
				switch(rand() % 5)
				{
				case UP:
					SB.movey = -8;
					break;
				case DOWN:
					SB.movey = 8;
					break;
				case RIGHT:
					SB.movex = 8;
					break;
				case LEFT:
					SB.movex = -8;
					break;
				}
			}
			else
			{
				int	dx;
				int	dy;

				SB.movex = SB.movey = 0;
				dx = (pengo.x - SB.x) / 32;
				dy = (pengo.y - SB.y) / 32;
				dx *= MW(screen((SB.x+((dx>0)?32:-32))>>5,SB.y>>5));
				dy *= MW(screen(SB.x>>5,(SB.y+((dy>0)?32:-32))>>5));
				if (abs(dx) > abs(dy))
					SB.movex = ((dx > 0) ? 8 : -8);
				else
					SB.movey = ((dy > 0) ? 8 : -8);
			}
		}
		switch (
			screen(
				(SB.x + 4 * SB.movex) >> 5,
				(SB.y + 4 * SB.movey) >> 5
			)
		)
		{
		case BREAK:	/* can move through breaking blocks */
		case BLANK:
		case PENGO:
		case SNO_BEE:
			put_char(SB.x, SB.y, SNO_BEE, F_XOR, bee_num);
			SB.x += SB.movex;
			SB.y += SB.movey;
			put_char(SB.x, SB.y, SNO_BEE, F_XOR, bee_num);
			break;

		case BLOCK:
			if (
				(bee_block[bee_num].exists == FALSE) &&
				((rand() % 100) < sb_break)
			)
			{
				bee_block[bee_num].count = NUM_BREAK;
				bee_block[bee_num].exists = TRUE;
				bee_block[bee_num].x = SB.x + 4 * SB.movex;
				bee_block[bee_num].y = SB.y + 4 * SB.movey;
				screen(bee_block[bee_num].x >> 5,
					bee_block[bee_num].y >> 5) = BREAK;
				add_event(DIE_BLOCK, bee_break, bee_num);
			}
			break;

		default:
			SB.movex = SB.movey = 0;
			break;
		}
	}
	else
	{
		put_char(SB.x, SB.y, SNO_BEE, F_XOR, bee_num);
		SB.x += SB.movex;
		SB.y += SB.movey;
		put_char(SB.x, SB.y, SNO_BEE, F_XOR, bee_num);
	}
	if (
		(pengo.exists) &&
		PX(pengo, SB)
	)
	{
		pengo.other = DYING;	/* he, he the sno_bee got him */
		add_event(P_DYING, die_pengo, NULL);
	}
	if (bee_run)
		add_event(MOVE_BEE0, run_sno_bee, bee_num);
	else
		add_event(MOVE_BEE0, move_sno_bee, bee_num);
}

run_sno_bee(bee_num)
int	bee_num;
{
	if (SB.exists != TRUE)
		return;
	if (SB.other != SNO_BEE)
		return;
	if (((SB.x & 31) == 0) && ((SB.y & 31) == 0))
	{
		/* run towards a corner */
		int	posx;
		int	posy;
		int	dirn;

		posx = SB.x >> 5;
		posy = SB.y >> 5;

		if (
			(((blockx - 1) == posx) || (posx == 0))
			&&
			(((blocky - 1) == posy) || (posy == 0))
		)
		{
			SB.other = VANISH;
			SB.count = DYING_TIME;
			add_event(DIE_BEE, vanish_bee, bee_num);
			return;
		}
		if (((blockx - 1) - posx) < posx)	/* to the right */
		{
			if (((blocky - 1) - posy) < posy)	/* down */
			{
				if (
					(
						(blockx - 1) != posx
					) &&
					(
						((blocky - 1) == posy) ||
						(
							((blockx - 1) - posx)
							<
							((blocky - 1) - posy)
						)
					)
				)
					dirn = RIGHT;
				else
					dirn = DOWN;
			}
			else				/* up */
			{
				if (
					(
						(blockx - 1) != posx
					) &&
					(
						(posy == 0 ) ||
						(
							((blockx - 1) - posx)
							<
							posy
						)
					)
				)
					dirn = RIGHT;
				else
					dirn = UP;
			}
		}
		else				/* to the left */
		{
			if (((blocky - 1) - posy) < posy)	/* down */
			{
				if (
					(
						posx != 0
					) &&
					(
						((blocky - 1) == posy) ||
						(posx < ((blocky - 1) - posy))
					)
				)
					dirn = LEFT;
				else
					dirn = DOWN;
			}
			else				/* up */
			{
				if (
					(
						posx != 0
					) &&
					(
						(posy == 0) ||
						(posx < posy)
					)
				)
					dirn = LEFT;
				else
					dirn = UP;
			}
		}
		switch(dirn)
		{
		case UP:
			SB.movex = 0;
			SB.movey = -8;
			break;

		case DOWN:
			SB.movex = 0;
			SB.movey = 8;
			break;

		case RIGHT:
			SB.movex = 8;
			SB.movey = 0;
			break;

		case LEFT:
			SB.movex = -8;
			SB.movey = 0;
			break;
		}
		switch (
			screen(
				(SB.x + 4 * SB.movex) >> 5,
				(SB.y + 4 * SB.movey) >> 5
			)
		)
		{
		case BREAK:
		case BLANK:
		case PENGO:
		case SNO_BEE:
			put_char(SB.x, SB.y, SNO_BEE, F_XOR, bee_num);
			SB.x += SB.movex;
			SB.y += SB.movey;
			put_char(SB.x, SB.y, SNO_BEE, F_XOR, bee_num);
			break;

		case BLOCK:
			if (bee_block[bee_num].exists == FALSE)
			{
				bee_block[bee_num].count = NUM_BREAK;
				bee_block[bee_num].exists = TRUE;
				bee_block[bee_num].x = SB.x + 4 * SB.movex;
				bee_block[bee_num].y = SB.y + 4 * SB.movey;
				screen(bee_block[bee_num].x >> 5,
					bee_block[bee_num].y >> 5) = BREAK;
				add_event(DIE_BLOCK, bee_break, bee_num);
			}
			break;

		default:
			SB.other = VANISH;
			SB.count = DYING_TIME;
			add_event(DIE_BEE, vanish_bee, bee_num);
			return;
		}
	}	
	else
	{
		put_char(SB.x, SB.y, SNO_BEE, F_XOR, bee_num);
		SB.x += SB.movex;
		SB.y += SB.movey;
		put_char(SB.x, SB.y, SNO_BEE, F_XOR, bee_num);
	}
	add_event(MOVE_BEE0, run_sno_bee, bee_num);
}

vanish_bee(bee_num)
int	bee_num;
{
	put_char(SB.x, SB.y, SNO_BEE, F_XOR, bee_num);
	if (SB.count--)
		add_event(DIE_BEE, vanish_bee, bee_num);
	else
	{
		disp_sno_bee(0);	/* no sno_bees */
		cancel_events();	/* all gone ... */
	}
}

confused_bee(bee_num)
int	bee_num;
{
	if (SB.other != CONFUSED)
		return;
	put_char(SB.x, SB.y, CONF, F_XOR, (SB.count-- & 1) | (bee_num << 1));
	put_char(SB.x, SB.y, CONF, F_XOR, (SB.count & 1) | (bee_num << 1));
	if (SB.count == 0)
	{
		put_char(SB.x, SB.y, CONF, F_XOR, (bee_num << 1));
		put_char(SB.x, SB.y, SNO_BEE, F_XOR, bee_num);
		SB.other = SNO_BEE;
		if (bee_run)
			add_event(MOVE_BEE0, run_sno_bee, bee_num);
		else
			add_event(MOVE_BEE0, move_sno_bee, bee_num);
	}
	else
		add_event(TIME_CONFUSED, confused_bee, bee_num);
}

#include "pengo.h"

#define	MAX_LEVEL	8	/* maximum level reached in display routines */

Rectangle	dsp0	= { { 0, 0 }, { 0, 0 } }; /* 5 * 1 rectangle */
Rectangle	dsp1	= { { 0, 0 }, { 0, 0 } }; /* 5 * 2 rectangle */
Rectangle	dsp2	= { { 0, 0 }, { 0, 0 } }; /* 3 * 2 rectangle */

level_dance(level)
int	level;
{
	Point	tmp;

	int	blkx;
	int	blky;
	int	x;

	blkx = (board.corner.x - board.origin.x) >> 5;
	blky = (board.corner.y - (board.origin.y + 128)) >> 5;

	dsp0.origin.x = board.origin.x + (((blkx - 5) >> 1) << 5);
	dsp0.origin.y = board.origin.y + 128 + (((blky - 1) >> 1 ) << 5);
	dsp0.corner = add(dsp0.origin, Pt(5 * 32, 32));
	dsp1.origin = dsp0.origin;
	dsp1.corner = add(dsp1.origin, Pt(5 * 32, 2 * 32));
	dsp2.origin = add(dsp0.origin, Pt(0, 32));
	dsp2.corner = add(dsp2.origin, Pt(3 * 32, 2 * 32));
	pengo.movex = 8;
	pengo.movey = 0;
	switch(level % (MAX_LEVEL + 2))
	{
	case 2:
	case 4:
	case 6:		/* to save space */
		tmp.x = board.origin.x - (32 + 8) * 2;
		tmp.y = dsp0.origin.y;
		dance_4(tmp, PENGO, NULL);
		while(tmp.x < dsp0.origin.x)
		{
			sleep((int)DANCE_PENGO);
			dance_4(tmp, PENGO, NULL);
			tmp.x += 8;
			dance_4(tmp, PENGO, NULL);
		}
		sleep((int)DANCE_PENGO);
		dance_4(tmp, PENGO, NULL);
		switch(level % (MAX_LEVEL + 2))
		{
		case 6:
			dance_4(tmp, SALUTE, NULL);
			sleep((int)PENGO_SALUTE);
			dance_4(tmp, SALUTE, NULL);
			break;

		case 4:
			dance_4(tmp, BACK, NULL);
			sleep((int)DANCE_PENGO);
			dance_4(tmp, BACK, NULL);
			dance_4(tmp, WIGGLE, 0);
			for (x = 0; x < NUM_WIGGLE; x++)
			{
				sleep((int)PENGO_W_TIME);
				dance_4(tmp, WIGGLE, x & 1);
				dance_4(tmp, WIGGLE, (x + 1) & 1);
			}
			sleep((int)DANCE_PENGO);
			dance_4(tmp, WIGGLE, x & 1);
			break;

		case 2:
			dance_4(tmp, HULA, 0);
			sleep((int)PENGO_HULA);
			dance_4(tmp, HULA, 0);
			for (x = 0; x < NUM_HULA; x++)
			{
				int	i;

				for (i = 0; i < NUM_HULA0; i++)
				{
					dance_4(tmp, HULA, 2);
					sleep((int)PENGO_HULA);
					dance_4(tmp, HULA, 2);
					dance_4(tmp, HULA, 1);
					sleep((int)PENGO_HULA);
					dance_4(tmp, HULA, 1);
				}
				dance_4(tmp, HULA, 2);
				sleep((int)PENGO_HULA);
				dance_4(tmp, HULA, 2);
				dance_4(tmp, HULA, 4);
				sleep((int)PENGO_HULA);
				dance_4(tmp, HULA, 4);
				for (i = 0; i < NUM_HULA0; i++)
				{
					dance_4(tmp, HULA, 3);
					sleep((int)PENGO_HULA);
					dance_4(tmp, HULA, 3);
					dance_4(tmp, HULA, 4);
					sleep((int)PENGO_HULA);
					dance_4(tmp, HULA, 4);
				}
			}
			dance_4(tmp, HULA, 0);
			sleep((int)PENGO_HULA);
			dance_4(tmp, HULA, 0);
		}
		dance_4(tmp, PENGO, NULL);
		while(ptinrect(tmp, board))
		{
			sleep((int)DANCE_PENGO);
			dance_4(tmp, PENGO, NULL);
			tmp.x += 8;
			dance_4(tmp, PENGO, NULL);
		}
		sleep((int)DANCE_PENGO);
		dance_4(tmp, PENGO, NULL);
		break;

	case 8:
		tmp = add(dsp0.origin, Pt(32, 32));
		level_put_char(add(tmp, Pt(00, 00)), CHAIR, F_XOR, 0);
		level_put_char(add(tmp, Pt(32, 00)), VIDEO, F_XOR, 0);
		level_put_char(add(tmp, Pt(63, 00)), CHAIR, F_XOR, 1);
		level_put_char(add(tmp, Pt(62, -9)), PLAYERR, F_XOR, 0);
		level_put_char(add(tmp, Pt(01, -9)), PLAYERL, F_XOR, 0);
		for (x = 0; x < NUM_BOUNCE; x++)
		{
			sleep((int)SHOW_VIDEO);	/* each bounce */
			level_put_char(add(tmp, Pt(01, -9)), PLAYERL, F_XOR, 0);
			level_put_char(add(tmp, Pt(01, -9)), PLAYERL, F_XOR, 1);
		}
		sleep((int)SHOW_VIDEO);	/* still beginning */
		level_put_char(add(tmp, Pt(01, -9)), PLAYERL, F_XOR, 0);
		level_put_char(add(tmp, Pt(01, -9)), PLAYERL, F_XOR, 1);
		for (x = 0; x < NUM_BOUNCE; x++)
		{
			sleep((int)SHOW_VIDEO);	/* each bounce */
			level_put_char(add(tmp, Pt(01, -9)), PLAYERL, F_XOR, 0);
			level_put_char(add(tmp, Pt(01, -9)), PLAYERL, F_XOR, 1);
		}
		sleep((int)SHOW_VIDEO);	/* last bounce, right turns head */
		level_put_char(add(tmp, Pt(62, -9)), PLAYERR, F_XOR, 0);
		level_put_char(add(tmp, Pt(62, -9)), PLAYERR, F_XOR, 1);
		sleep((int)SHOW_VIDEO);	/* delay whils right turns head */
		for (x = 0; x < NUM_BOUNCE; x++)
		{
			sleep((int)SHOW_VIDEO);	/* each bounce */
			level_put_char(add(tmp, Pt(01, -9)), PLAYERL, F_XOR, 0);
			level_put_char(add(tmp, Pt(01, -9)), PLAYERL, F_XOR, 1);
		}
		sleep((int)SHOW_VIDEO);
		level_put_char(add(tmp, Pt(01, -9)), PLAYERL, F_XOR, 1);
		level_put_char(add(tmp, Pt(62, -9)), PLAYERR, F_XOR, 1);
		level_put_char(add(tmp, Pt(00, 00)), CHAIR, F_XOR, 0);
		level_put_char(add(tmp, Pt(32, 00)), VIDEO, F_XOR, 0);
		level_put_char(add(tmp, Pt(63, 00)), CHAIR, F_XOR, 1);

	default:
		break;
	}
}

dance_4(tmp, ch, other)
Point	tmp;
int	ch;
int	other;
{
	int	x;

	for (x = 0; x < 4; x++)
		level_put_char(add(tmp, Pt((32 + 8) * x, 0)), ch, F_XOR, other);
}

level_put_char(tmp, ch, format, other)
Point	tmp;
int	ch;
int	format;
int	other;
{
	int	tmp0;

	if (!ptinrect(tmp, dboard))
		return;
	switch(ch)
	{
	case PENGO:
		if (pengo.movex)
			if (pengo.movex < 0)
				tmp0 = 1;
			else
				tmp0 = 3;
		else
			if (pengo.movey < 0)
				tmp0 = 5;
			else if (pengo.movey)
				tmp0 = 7;
			else
				tmp0 = 0;
		if (tmp.x & 16)
			tmp0++;
		bit32.base = (Word *)pengo_moves[tmp0];	
		break;

	case BACK:
		bit32.base = (Word *)char_pback;
		break;

	case WIGGLE:
		bit32.base = (Word *)(other ? char_1wiggle : char_0wiggle);
		break;

	case SALUTE:
		bit32.base = (Word *)char_salute;
		break;

	case HULA:
		bit32.base = (Word *)hula_moves[other];
		break;

	case CHAIR:
		bit32.base = (Word *)(other ? char_rchair : char_lchair);
		break;

	case VIDEO:
		bit32.base = (Word *)(other ? char_null : char_0video);
		break;

	case PLAYERR:
		bit32.base = (Word *)(other ? char_0rplayer : char_1rplayer);
		break;

	case PLAYERL:
		bit32.base = (Word *)(other ? char_0lplayer : char_1lplayer);
		break;

	default:
		bit32.base = (Word *)char_null;
		break;

/* others here ... */
	}
	bitblt(&bit32, Rect(0, 0, 32, 32), &display,
		add(board.origin, Pt(tmp.x, tmp.y)), format);
}

int	padd_score[] = { 5000, 1000, 500, 100, 10, 1, 0, };

extra_score(seconds)
int	seconds;
{
	Point	tmp;
	int	count;

	if (seconds < 0)
		seconds = 0;
	tmp.x = board.origin.x + (board.corner.x - board.origin.x - 16 * 16) / 2;
	tmp.y = board.origin.y + 16;
	rectf(&display, board, F_STORE);
	addstr("Time Taken ....", tmp, F_CLR);
	add_time(seconds, add(tmp, Pt(16 * 15, 0)));
	tmp.y += 16;
	addstr(" 0 ...  9  5000", tmp, F_CLR);
	tmp.y += 16;
	addstr("10 ... 19  1000", tmp, F_CLR);
	tmp.y += 16;
	addstr("20 ... 29   500", tmp, F_CLR);
	tmp.y += 16;
	addstr("30 ... 39   100", tmp, F_CLR);
	tmp.y += 16;
	addstr("40 ... 49    10", tmp, F_CLR);
	tmp.y += 16;
	addstr("50 ... 59     1", tmp, F_CLR);
	tmp.y += 16;
	addstr("Over 59 NO BONUS", tmp, F_CLR);
	if (seconds >= 60)
		seconds = 60;
	rectf(&display, Rect(
		board.origin.x,
		board.origin.y + ((seconds/10) * 16) + 32,
		board.corner.x,
		board.origin.y + ((seconds/10) * 16) + 48), F_XOR);
	put_score(score += padd_score[(seconds / 10) % 7]);
	count = 0;
	if (button123())
		for (;count < 60; count++)
			if (!button123())
				break;
			else
				sleep(2);
	for (;count < 60; count++)
		if (button123())
			break;
		else
			sleep(2);
	while(button123())
		wait(CPU);
}

add_time(seconds, pos)
int	seconds;
Point	pos;
{
	if (!seconds)
	{
		addstr("0 seconds", pos, F_CLR);
		return;
	}
	if (seconds > 3600)	/* 1 hour */
	{
		addstr("Over 1 Hour", pos, F_CLR);
		return;
	}
	if (seconds > 60)
	{
		int	minutes;

		minutes = seconds / 60;
		if (minutes > 10)
		{
			addchar((minutes / 10) + '0', pos, F_CLR);
			pos.x += 16;
		}
		addchar((minutes % 10) + '0', pos, F_CLR);
		seconds = seconds % 60;
		pos.x += 16;
		addstr(" Min:", pos, F_CLR);
		pos.x += 5 * 16;
	}
	if (seconds > 10)
	{
		addchar((seconds / 10) + '0', pos, F_CLR);
		pos.x += 16;
	}
	addchar((seconds % 10) + '0', pos, F_CLR);
	pos.x += 16;
	addstr(" Sec", pos, F_CLR);
}

/*
	character arrays for water shapes
*/
#include "pengo.h"

short nw[] = {
	0x5555, 0xAAAA, 0x5555, 0xAAAA,
	0x5555, 0xAAAA, 0x5555, 0xAAAA,
	0x5555, 0xAAAA, 0x5555, 0xAAAA,
	0x5555, 0xAAAA, 0x5555, 0xAAAA,
};

short tw[] = {
	0x5555, 0xAAAA, 0x4141, 0x0808,
	0x5555, 0xAAAA, 0x4141, 0x0808,
	0x5555, 0xAAAA, 0x4141, 0x0808,
	0x5555, 0xAAAA, 0x4141, 0x0808,
};

short bw[] = {
	0x5555, 0x0808, 0x4141, 0xAAAA,
	0x5555, 0x0808, 0x4141, 0xAAAA,
	0x5555, 0x0808, 0x4141, 0xAAAA,
	0x5555, 0x0808, 0x4141, 0xAAAA,
};

short lw[] = {
	0x1111, 0xAAAA, 0x1111, 0x8888,
	0x5555, 0x8888, 0x1111, 0xAAAA,
	0x1111, 0xAAAA, 0x1111, 0x8888,
	0x5555, 0x8888, 0x1111, 0xAAAA,
};

short rw[] = {
	0x5555, 0x8888, 0x1111, 0xAAAA,
	0x1111, 0x8888, 0x5555, 0x8888,
	0x5555, 0x8888, 0x1111, 0xAAAA,
	0x1111, 0x8888, 0x5555, 0x8888,
};

swill_water(num)
int	num;
{
	Word	*p;
	Rectangle w;
	int	dx;
	int	dy;
	int	x;

	if (num == 0)
	{
		move_water = STILL;
		return;
	}
	dx = dy = -1;
	switch(move_water)
	{
	case UP:
		p = (Word *)tw;
		w = twater;
		dy = 0;
		break;

	case DOWN:
		p = (Word *)bw;
		w = bwater;
		dy = blocky - 1;
		break;

	case LEFT:
		p = (Word *)lw;
		w = lwater;
		dx = 0;
		break;

	case RIGHT:
		p = (Word *)rw;
		w = rwater;
		dx = blockx - 1;
		break;

	default:
		return;
	}
	if (num & 1)
		p = (Word *)nw;
	texture(&display, w, p, F_STORE);
	for (x = 0; x < 4; x++)
		if (
			(sno_bee[x].exists == TRUE) &&
			(sno_bee[x].other == SNO_BEE) &&
			(
				(
					(dx != -1) &&
					(sno_bee[x].x == (dx << 5))
				) ||
				(
					(dy != -1) &&
					(sno_bee[x].y == (dy << 5))
				)
			)
		)
		{
			sno_bee[x].count = NUM_CONFUSED;
			sno_bee[x].other = CONFUSED;
			add_event(TIME_CONFUSED, confused_bee, x);
			put_char(sno_bee[x].x, sno_bee[x].y,
				SNO_BEE, F_XOR, x);
			put_char(sno_bee[x].x, sno_bee[x].y,
				CONF, F_XOR, (NUM_CONFUSED & 1) | (x << 1));
		}
	add_event(WATER_TIME, swill_water, --num);
}

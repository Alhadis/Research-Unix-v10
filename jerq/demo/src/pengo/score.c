/*
	score.c:	put score onto the screen.
*/

#include "pengo.h"

add_men(score, old_score)
long	score;
long	old_score;
{
	if ((manptr >= NUM_ADD_MAN) && ((score % 800000L) < (old_score % 800000L)))
		add_man();
	else
		while ((manptr < NUM_ADD_MAN) && (score > new_man[manptr]))
		{
			add_man();
			manptr++;
		}
}

put_score(score)
long	score;
{
	int	count;

	rectf(&display, header, F_CLR);
	for (count = NUM_DIGITS - 1; ((count >= 0) && (score > 0)); count--)
	{
		bit32.base = (Word *)&N[(score % 10) * 64];
		bitblt(&bit32, R3, &display,
			Pt(header.origin.x + count * CHAR_WIDTH, header.origin.y),
			F_STORE);
		score /= 10;
	}
}

/*
	this file handles contradictions between the screen appearance
	and what is stored in the block array
*/
#include "pengo.h"

screen_blank(x, y)
int	x;
int	y;
{
	Point	sp;
	int	count;

	sp.x = x << 5;
	sp.y = y << 5;
	if (
		PX(sp, pengo)  ||
		(moving.exists && PX(sp, moving)) ||
		(breaking.exists && PX(sp, breaking))
	)
		return FALSE;	/* something is there */
	for (count = 0; count < MAX_BEES; count++)
		if (
			eggs[count].exists &&
			(eggs[count].other == SNO_BEE) &&
			PX(sp, eggs[count])
		)
			return FALSE;
	if (screen(x, y) != BLANK)
		return FALSE;
	return TRUE;	/* oh well it appears to be blank */
}

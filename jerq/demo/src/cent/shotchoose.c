#include "playdefs.h"

/* 'shotchoose' determines how the player's missile should move
 * next.
 */
shotchoose ()
{
	shot.next = raddp (shot.rect, Pt(shot.dxdt, shot.dydt));
	if (!(ptinrect (shot.next.origin, Drect))) {
		CLEAR_ATT (shot, EXISTS);
		put_object (shot);
		return (FALSE);
	}
	return (TRUE);
}

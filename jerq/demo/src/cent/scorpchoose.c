#include "playdefs.h"

/* 'scorpchoose' helps a scorpion decide where it should move next.
 */
scorpchoose ()
{
	scorpion.next = raddp (scorpion.rect, Pt (scorpion.dxdt, 0));
	if (((scorpion.dxdt > 0) &&
	     (!(ptinrect(scorpion.next.corner, Drect)))) ||
	    ((scorpion.dxdt < 0) &&
	     (!(ptinrect(scorpion.next.origin, Drect))))) {
		CLEAR_ATT (scorpion, EXISTS);
		put_object (scorpion);
		scorp_fuse = scorpgone ();
		return (FALSE);
	}
	return (TRUE);
}

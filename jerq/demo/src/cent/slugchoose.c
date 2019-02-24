#include "playdefs.h"

/* 'slugchoose' decides how the mushroom dropper should move next.
 */
slugchoose()
{
	slug.next = raddp (slug.rect, Pt(slug.dxdt, slug.dydt));
	if (!(ptinrect(slug.next.corner, Drect))) {
		CLEAR_ATT (slug, EXISTS);
		put_object (slug);
		return (FALSE);
	}
	return (TRUE);
}

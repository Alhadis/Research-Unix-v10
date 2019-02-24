#include "playdefs.h"

/* 'spiderchoose' decides in a highly random manner how the spider
 *	should move next.
 */
spiderchoose ()
{
	static int	xdir;
	
	if ((xdir != spider.dxdt) && (xdir != 0))
		xdir = spider.dxdt;
	if (!(isallin(raddp(spider.rect, Pt(spider.dxdt, 0)), botrect))) {
		spider.dxdt = -spider.dxdt;
		xdir = -xdir;
	}
	if (urnd (0, 20) < 1)
		if (xdir != 0)
			xdir = 0;
		else
			xdir = spider.dxdt;
			
	if (urnd (0, 40) < 1)
		spider.dydt = -spider.dydt;

	if (!(isallin(raddp(spider.rect, Pt(0, spider.dydt)), botrect)))
		spider.dydt = -spider.dydt;
		
	spider.next = raddp(spider.rect, Pt(xdir, spider.dydt));
	return (TRUE);
}

/* 'isallin' determines if Rectangle 'rect' is completely contained
 *	in Rectangle 'bigrect'.
 */
isallin (rect, bigrect)
Rectangle	rect,
		bigrect;
{
	if ((rect.origin.x >= bigrect.origin.x) &&
	    (rect.origin.y >= bigrect.origin.y) &&
	    (rect.corner.x <= bigrect.corner.x) &&
	    (rect.corner.y <= bigrect.corner.y))
		return (TRUE);
	else
		return (FALSE);
}

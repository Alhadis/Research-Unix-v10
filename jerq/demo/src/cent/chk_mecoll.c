#include "playdefs.h"

/* 'chk_mecollision' sees if the player bumped into anything. */
chk_mecollision()
{
	register int	i;
	
	if (IS_SET(spider, EXISTS) && rectXrect (me.rect, spider.rect)
	    && XX(&me, &spider)) {
		killplayer ();
		return;
	}
	if (IS_SET(slug, EXISTS) && rectXrect (me.rect, slug.rect)
	    && XX(&me, &slug)) {
		killplayer ();
		return;
	}
	for (i = 0; i < WORMMAX; i++) {
		if (IS_SET(worm[i], EXISTS) && rectXrect(me.rect, worm[i].rect)
		    && XX(&me, &worm[i])) {
			killplayer ();
			return;
		}
	}
}
static Word xxwords[32];
static Bitmap xxcol = {
	xxwords, 1, {{0,0}, {32,32}}
};
XX(p,q)
register struct object *p, *q;
{
	register dx, dy;
	register int i;

	dx = q->rect.origin.x - p->rect.origin.x;
	dy = q->rect.origin.y - p->rect.origin.y;
	
	bitblt(p->looks, p->looks->rect, &xxcol, Pt(0,0), F_STORE);
	bitblt(q->looks, q->looks->rect, &xxcol, Pt(dx,dy), F_CLR);
	bitblt(p->looks, p->looks->rect, &xxcol, Pt(0,0), F_XOR);
	for(i=0; i<32; i++)if(xxwords[i])return 1;
	return 0;
}

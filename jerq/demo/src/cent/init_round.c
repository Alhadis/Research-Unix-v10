#include "playdefs.h"
#define	CORMUSH (mushspace[mushx(worm[i].rect.corner.x-1)][mushy(worm[i].rect.corner.y-1)])
init_round ()
{
	register int	i;
	
	for (i = 0; i < WORMMAX; i++) {
		SET_ATT (worm[i], EXISTS);
		CLEAR_ATT (worm[i], CLIMBMODE);
		CLEAR_ATT (worm[i], HIT_BOTTOM);
		
		/* place segment also */
		worm[i].looks = &wormlooks; /* Needed to find
						corner of worm seg. */
		worm[i].rect = raddp (worm[i].looks->rect,
					mushorigin ((i + 1), 1));
		/* The segment is never actually even with a mushroom. */
		worm[i].rect = raddp (worm[i].rect, Pt (8, 0));

		worm[i].dxdt = WORMXSPEED;
		worm[i].dydt = 0;
		CLEAR_ATT (worm[i], PLUMMETING);
		
		if ((i == WORMMAX-1) || CORMUSH) {
			SET_ATT (worm[i], IS_HEAD);
			worm[i].looks = &headlooks;
		}
		else {
			CLEAR_ATT (worm[i], IS_HEAD);
			worm[i].looks = &wormlooks;
		}
		put_object (worm[i]);
	}

	if (IS_SET(shot, EXISTS)) {
		CLEAR_ATT (shot, EXISTS);
		put_object (shot);
	}
	
	if (mush_cnt != 0) { /* Remove silliness	*/
		if (IS_SET (spider, EXISTS)) {
			put_object (spider);
			spider.looks = &spiderlooks;
			put_object (spider);
		}
		else
			spider.looks = &spiderlooks;
	}
	
	if (spidbuf[0] != '\0') { /* clear old score display */
		string (&defont, spidbuf, &display, spider.rect.origin, F_XOR);
		spidbuf[0] = '\0';
	}
	
	me.rect = raddp (me.looks->rect, mouse.xy);
	oldmouse = mouse.xy;

	if (me.rect.origin.x < playerrect.origin.x)
		me.rect = raddp (me.looks->rect, Pt (playerrect.origin.x,
						  me.rect.origin.y));
	else if (me.rect.corner.x > playerrect.corner.x) {
		me.rect.corner.x = playerrect.corner.x;
		me.rect.origin.x = playerrect.corner.x - MEWIDTH;
	}

	if (me.rect.origin.y < playerrect.origin.y)
		me.rect = raddp (me.looks->rect, Pt (me.rect.origin.x,
						  playerrect.origin.y));
	else if (me.rect.corner.y > playerrect.corner.y) {
		me.rect.origin.y = playerrect.corner.y - MEHEIGHT;
		me.rect.corner.y = playerrect.corner.y;
	}

	put_object (me);
}

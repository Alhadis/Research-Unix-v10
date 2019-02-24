#include "playdefs.h"

/* 'personchoose' ('*objp' == me for this one) */

personchoose ()
{
	register int	c;
	register int	owned;
	Point	displace;
	
	if ((!IS_SET(shot, EXISTS)) && (MOUSE&(owned = own()))) {
		if (button2()) {
			SET_ATT(shot, EXISTS);
			shot.rect = raddp (shot.looks->rect,
					   Pt(div2(me.rect.corner.x +
						   me.rect.origin.x),
					      me.rect.origin.y));
			put_object (shot);
		} /*else*/ if (button1() || button3()) {
			request(0);
			cursswitch ((Texture16*)0);
			sleep(2);
			request (MOUSE|KBD);
			cursswitch (&plain);
#ifdef MUX
			if (P->state & RESHAPED) {
				start_over = TRUE;
				return (FALSE);
			}
#endif
		}
	}
	if (owned & KBD) {
		while ((c = kbdchar()) != -1) {
			switch (c) {
				case 'q':
				case 'Q':
					exit_game();
					start_over = TRUE;
					return (FALSE);
			}
		}
	}
	displace = sub (mouse.xy, oldmouse);
	if ((displace.x == 0) && (displace.y == 0))
		return (FALSE);
	oldmouse = mouse.xy;

	me.next = raddp (me.rect, displace);

	if (me.next.origin.x < playerrect.origin.x)
		me.next = raddp (me.looks->rect, Pt (playerrect.origin.x,
						  me.next.origin.y));
	else if (me.next.corner.x > playerrect.corner.x) {
		me.next.corner.x = playerrect.corner.x;
		me.next.origin.x = playerrect.corner.x - MEWIDTH;
	}

	if (me.next.origin.y < playerrect.origin.y)
		me.next = raddp (me.looks->rect, Pt (me.next.origin.x,
						  playerrect.origin.y));
	else if (me.next.corner.y > playerrect.corner.y) {
		me.next.origin.y = playerrect.corner.y - MEHEIGHT;
		me.next.corner.y = playerrect.corner.y;
	}
	return (TRUE);
}

#include "playdefs.h"
/* A centipede segment can be in one of two places relative to the
 * 	mushroom in which the origin of the segment currently lies.
 * 	These two places I've called the LEFTNODE and RIGHTNODE.
 */
#define	IS_FLAT	(objp->dydt == 0)
#define IS_DIVING	(objp->dydt > 0)
#define	IS_CLIMBING	(objp->dydt < 0)
#define	IS_PLUMMET	(IS_SET(*objp, PLUMMETING))
#define GOING_RIGHT	(objp->dxdt > 0)
#define GOING_LEFT	(objp->dxdt < 0)
#define	ORGMUSH	(mushspace[mushx(objp->rect.origin.x)][mushy(objp->rect.origin.y)] != 0)
#define	CORMUSH	(mushspace[mushx(objp->rect.corner.x-1)][mushy(objp->rect.corner.y-1)] != 0)
#define	ORGGLOW	(glowmush[mushx(objp->rect.origin.x)][mushy(objp->rect.origin.y)])
#define	CORGLOW	(glowmush[mushx(objp->rect.corner.x-1)][mushy(objp->rect.corner.y-1)])
#define	LEFTMOST	(mushx(objp->rect.origin.x) == 0)
#define	RIGHTMOST	(mushx(objp->rect.origin.x) == (mushcols-2))
#define	TOPMOST	(mushy(objp->rect.origin.y) == topofbottom)
#define	BOTTOMMOST	(mushy(objp->rect.origin.y) == (mushrows-1))

wormchoose (objp)
register struct	object	*objp;
{
	register	int	leftnode,
				corright,
				orgleft;
	
	leftnode = (((objp->rect.origin.x - Drect.origin.x) & 037) == 8);
	
	if (IS_FLAT) {
		if (((RIGHTMOST || CORMUSH) &&
		     GOING_RIGHT &&
		     leftnode) ||
		    ((LEFTMOST || ORGMUSH) &&
		     GOING_LEFT &&
		     (!(leftnode)))) { /* No more flat */
			if ((objp == &(worm[firstworm])) &&
			    IS_SET (*objp, HIT_BOTTOM) &&
			    IS_SET (*objp, IS_HEAD) &&
			    (RIGHTMOST || LEFTMOST))
				start_seg (objp);
			if (IS_SET (*objp, CLIMBMODE)) {
				if (TOPMOST) {
					objp->dydt = WORMYSPEED;
					CLEAR_ATT (*objp, CLIMBMODE);
				}
				else
					objp->dydt = -WORMYSPEED;
			}
			else {
				if (BOTTOMMOST) {
					SET_ATT (*objp, CLIMBMODE);
					SET_ATT (*objp, HIT_BOTTOM);
					CLEAR_ATT (*objp, PLUMMETING);
					objp->dydt = -WORMYSPEED;
				}
				else {
					objp->dydt = WORMYSPEED;
					if ((GOING_RIGHT && CORGLOW) ||
					    (GOING_LEFT && ORGGLOW))
						SET_ATT(*objp, PLUMMETING);
				}
			}
		}
	}
	else { /* diving or climbing */
		if ((leftnode && GOING_LEFT) || 
		    ((!(leftnode)) && GOING_RIGHT)) {
			corright = (CORMUSH||RIGHTMOST);
			orgleft = (ORGMUSH||LEFTMOST);
			if (((corright ) &&
			     GOING_LEFT) ||
			    ((orgleft ) &&
			      GOING_RIGHT)) {
				if ((GOING_LEFT && orgleft) ||
				    (GOING_RIGHT && corright) ||
				    IS_PLUMMET) {
					if (BOTTOMMOST && IS_DIVING) {
						CLEAR_ATT (*objp, PLUMMETING);
						SET_ATT (*objp, CLIMBMODE);
						SET_ATT(*objp, HIT_BOTTOM);
						objp->dydt = -objp->dydt;
					}
					else if (TOPMOST && IS_CLIMBING) {
						CLEAR_ATT (*objp, CLIMBMODE);
						objp->dydt = -objp->dydt;
					}
					else if ((CORGLOW && GOING_RIGHT)||
						 (ORGGLOW && GOING_LEFT)) {
						SET_ATT(*objp, PLUMMETING);
					}
					objp->dxdt = -objp->dxdt;
				}
				else { /* flatten out */
					objp->dydt = 0;
				}
			}
			else {
				objp->dxdt = -objp->dxdt;
				if (IS_PLUMMET) {
					if (BOTTOMMOST) {
						objp->dydt = -objp->dydt;
						CLEAR_ATT (*objp, PLUMMETING);
						SET_ATT (*objp, CLIMBMODE);
						SET_ATT(*objp, HIT_BOTTOM);
					}
				}
				else
					objp->dydt = 0;
			}
		}
	}
	objp->next = raddp (objp->rect, Pt(objp->dxdt, objp->dydt));
	return (TRUE);
}

/* 'start_seg' makes a new centipede head come out at the bottom of
 *	the screen.
 */
start_seg (objp)
register struct	object	*objp;
{
	int	i;
	
	for (i = WORMMAX-1; i >= 0; i--)
		if (!IS_SET(worm[i], EXISTS))
			break;
	if (i < 0)
		return;
	if  (firstworm > i)
		firstworm = i;
	SET_ATT (worm[i], EXISTS);
	worm[i].looks = &headlooks;
	if (LEFTMOST) { /* start one from the left */
		worm[i].rect = raddp (worm[i].looks->rect,
				mushorigin (mushcols-1, urnd (topofbottom,
						   mushrows-1)));
		worm[i].rect = rsubp (worm[i].rect, Pt(8, 0));
		worm[i].dxdt = -WORMXSPEED;
	}
	else { /* start one from the right	*/
		worm[i].rect = raddp (worm[i].looks->rect,
			      mushorigin (0, urnd (topofbottom,
						   mushrows-1)));
		worm[i].rect = raddp (worm[i].rect, Pt(8, 0));
		worm[i].dxdt = WORMXSPEED;
	}
	worm[i].dydt = 0;
	SET_ATT (worm[i], IS_HEAD);
	SET_ATT (worm[i], HIT_BOTTOM);
	CLEAR_ATT (worm[i], CLIMBMODE);
	CLEAR_ATT (worm[i], PLUMMETING);
	put_object (worm[i]);
}

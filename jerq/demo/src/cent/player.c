#include "playdefs.h"

/* 'main' for Dougslug contains the main timing loop. Objects are
 *	moved based on the ticks of 'mv_clock'.
 */
main ()
{
	int	i, j;
	int	mv_clock,
		hold[WORMMAX];
		
TOP:	start_over = FALSE;
	init_game ();
	
/* This loop may be exited by quitting (typing 'q') or by losing all 	*/
/*	of the players.							*/

	while (TRUE) {
		init_round ();
		round_cnt++;
		mv_clock = 0;
		while (TRUE) {
#ifdef MUX
			/* Release the cpu once in a while	*/
			if ((!(mv_clock & WAITRATE)) && ptinrect (mouse.xy, Drect)) {
				request (KBD|MOUSE);
				wait (CPU);
			}
			if (P->state & RESHAPED)
				goto TOP;
#endif
				
			/* Move or maybe start the spider	*/
			if (!(mv_clock & SPIDERRATE)) {
				if (IS_SET(spider, EXISTS)) {
					moveobj (spider);
					spid_collision ();
				}
				else if (spider_fuse-- <= 0)
					spidstart();
			}
			
			/* Move or maybe start the scorpion	*/
			if (!(mv_clock & SCORPRATE)) {
				if (IS_SET(scorpion, EXISTS)) {
					moveobj (scorpion);
					scorp_collision ();
				}
				else if ((round_cnt > 3) &&
					 (scorp_fuse-- <= 0))
					scorpstart();
			}
			
			/* Move the player's missile	*/
			if (IS_SET (shot, EXISTS) &&
			    (!(mv_clock & SHOTRATE))) {
				moveobj (shot);
				shot_collision ();
			}
			
			/* Move or maybe start the slug(mushroom dropper)*/
			if (IS_SET(slug, EXISTS)) {
				if (!(mv_clock & SLUGRATE)) {
					moveobj (slug);
					slug_collision ();
				}
			}
			else if ((round_cnt > 1) &&
				 (!(mv_clock & SLUGCHECK)) &&
				 (botmush_cnt < minbotmush())) {
					slugstart();
			}

			/* Move all existing centipede segments.
			  	Start new round if none are left.	*/
			if (!(mv_clock & WORMRATE)) {
				firstworm = -1;
				for (i = WORMMAX-1; i >= 0; i--)
					hold[i] = FALSE;
				for (i = WORMMAX-1; i >= 0; i--) {
					if (!(IS_SET(worm[i], EXISTS))) {
						for(j=0;j<=150;j++);/* wait loop */
						continue;
					}
					else { /* hold this seg. to prevent overlapping */
						if (firstworm == -1)
							firstworm = i;
						if (hold[i])
							continue;
					}
					if (worm[i].dydt == 0) { /*
						try to prevent
						overlapping */
						for (j = i-1; j >= 0; j--){
							if (hold[j] || (!(IS_SET (worm[j], EXISTS))))
								continue;
/* This amazing 'if' attempts to prevent two segments from overlapping for
 *	for any period of time.	*/
		if ((worm[i].rect.origin.y == worm[j].rect.origin.y) &&
		    (worm[i].dxdt == worm[j].dxdt) &&
		    (worm[j].dydt == 0) &&
		    ((worm[i].rect.origin.x & ~037) ==
		     (worm[j].rect.origin.x & ~037))) {
			hold[j] = TRUE;
		}
						}
					}
					moveobj (worm[i]);
					/* worm_collision (i); not needed*/
				}
				if (firstworm == -1)
					break;
			}
			
			/* Move player's object	*/
			if (TRUE) {
				moveobj (me);
				chk_mecollision();
			}
			mv_clock++;
			if (start_over) /* END OF GAME - start another	*/
				goto TOP;
		}
		
		/* END OF ROUND - reinitialize some things	*/
		put_object (me);

		rectf (&display, Drect, F_XOR);	
	}
}

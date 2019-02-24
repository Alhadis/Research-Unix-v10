#include "playdefs.h"

/* 'init_game' initializes everything for the beginning of a new
 * game.
 */
init_game ()
{
	register int	i;
	
	score = 0;
	for (i = 0; i < WORMMAX; i++) {
		worm[i].dirchoose = wormchoose;
		worm[i].value = WORMVALUE;
	}
	/* Set spider, shot, scorpion, slug to "off" */
	CLEAR_ATT (shot, EXISTS);
	CLEAR_ATT (slug, EXISTS);
	CLEAR_ATT (spider, EXISTS);
	CLEAR_ATT (scorpion, EXISTS);
	scorp_fuse = scorpgone ();
	spider_fuse = SPIDERGONE;
	round_cnt = 0;
	me_cnt = ME_MAX;
	newmanat = NEWMANSTEP;
	
	spidbuf[0] = '\0';

	init_scr ();
	return;
}

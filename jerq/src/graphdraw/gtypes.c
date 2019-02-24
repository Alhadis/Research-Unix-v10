#include "defs.h"

char *gtypemenutext[9] =  {
	 "undirected",
	 "(directed)",
	 "~~~~~~~~~~~~~~~~~~~~~~~~",
	 "(multiple edges)",
	 "(loops)",
  	 "single edges, no loops",	
	 "~~~~~~~~~~~~~~~~~~~~~~~~",
	 "original menu",
 	 0};	
Menu gtypemenu={gtypemenutext}; 


/* graph types */
gtypes() {
	
	int sel;

	for (;; wait(MOUSE) ) {

		if (button1() && ptinrect(mouse.xy, box)) {
			cmds();
			return;
		}

		if ( button3() ) {
			sel = menuhit(&gtypemenu,3);
			for(;;wait(MOUSE)) if (!button3()) break;
			if (sel ==  7 || sel == -1) 
				return;
			setgraphtype(sel);
		}
	}
}

setgraphtype(i) 
int i; {

	switch(i) {
		case 0:
			gtypemenutext[0] = "* undirected";
			gtypemenutext[1] = "directed";
			dirflag = 0;
			break;

		case 1:
			gtypemenutext[0] = "undirected";
			gtypemenutext[1] = "* directed";
			dirflag = 1;
			break;


		case 3:
			gtypemenutext[3] = "* multiple edges";
			gtypemenutext[5] = "single edges, no loops";
			multedflag = 1;
			break;

		case 4:
			gtypemenutext[4] = "* loops";
			gtypemenutext[5] = "single edges, no loops";
			loopflag = 1;
			break;

		case 5:
			gtypemenutext[3] = "multiple edges";
			gtypemenutext[4] = "loops";
			gtypemenutext[5] = "* single edges, no loops";
			multedflag = 0;
			loopflag = 0;
			break;

	}
}

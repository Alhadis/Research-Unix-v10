#include "defs.h"

char *colormenutext[8] =  {
	 "light vertex",
	 "heavy vertex",
	 "empty vertex",
	 "full vertex",
	 "invisible vertex",
	 "light edge",
	 "heavy edge",
 	 0};	
Menu colormenu={colormenutext}; 


graphicsmenu() {
	
	int sel;

	for (;; wait(MOUSE) ) {

		if (button1() && ptinrect(mouse.xy, box)) {
			cmds();
			return;
		}

		if ( button3() ) {
			sel = menuhit(&colormenu,3);
			for(;;wait(MOUSE)) if (!button3()) break;
			switch( sel ) {

				case 0:
					vertgraphics("L");
					break;

				case 1:
					vertgraphics("H");
					break;

				case 2:
					vertgraphics("E");
					break;

				case 3:
					vertgraphics("F");
					break;

				case 4:
					vertgraphics("I");
					break;

				case 5:
					edgegraphics("L");
					break;

				case 6:
					edgegraphics("H");
					break;

				case -1:
					return;
					break;
			}
		}
	}
}

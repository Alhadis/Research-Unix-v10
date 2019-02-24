#include "defs.h"

char *macmenutext[10] = 
	{"select set",
	 "unselect set",
	 "shrink set",
	 "expand set",
	 "reshape set",
	 "complete subgraph",
	 "disconnect subgraph",
	 "labels menu",
	 "original menu",
 	 0};	
Menu macmenu={macmenutext}; 


macros() {
	
	int sel;

	for (;; wait(MOUSE) ) {

		if (button1() && ptinrect(mouse.xy, box)) {
			cmds();
			return;
		}

		if ( button3() ) {
			sel = menuhit(&macmenu,3);
			for(;;wait(MOUSE)) if (!button3()) break;
			switch( sel ) {

				case 0:
					selset();
					break;

				case 1:
					unselset();
					break;

				case 2:
					newsetsize(0);
					break;

				case 3:
					newsetsize(1);
					break;

				case 4:
					reshapeset();
					break;

				case 5:
					compsubg();
					break;

				case 6:
					disconnsubg();
					break;

				case 7:
					labs();
					break;

				case 8:
					return;
					break;

				case -1:
					return;
					break;
			}
		}
	}
}

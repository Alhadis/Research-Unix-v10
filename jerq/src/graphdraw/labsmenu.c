#include "defs.h"
#include <font.h>

char *labsmenutext[11] =  {
	 "label vertex",
	 "label edge",
	 "show vertex labels",
	 "show edge labels",
  	 "number vertices",	
  	 "turn euclidean edge labels on",
	 "show sum of edges",
	 "graphics menu",
	 "macros menu",
	 "original menu",
 	 0};	
Menu labsmenu={labsmenutext}; 


labs() {
	
	int sel;

	for (;; wait(MOUSE) ) {

		if (button1() && ptinrect(mouse.xy, box)) {
			cmds();
			return;
		}

		if ( button3() ) {
			if (edgesumflag == 1) {
				rectf(&display, box, f1);
			  	showEsum();	
			}
			sel = menuhit(&labsmenu,3);
			for(;;wait(MOUSE)) if (!button3()) break;
			switch( sel ) {

				case 0:
					labelvert();
					break;

				case 1:
					labeledge();
					break;

				case 2:
					showvertlabels();
					break;

				case 3:
					showedgelabels();
					break;

				case 4:
					nmbrverts();
					break;


				case 5:
					euc();
					break;

				case 6:
					edgesum();
					break;

				case 7:
					graphicsmenu();
					break;

				case 8:  
					macros();
					break;

				case 9:  
					return;
					break;

				case -1:  
					return;
					break;
			}
		}
	}
}

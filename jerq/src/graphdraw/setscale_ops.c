#include "defs.h"

newsetsize(op)
int op; {     /* op = 0, shrink; op = 1, expand */
	int i, flag;
	Point p1;

	for(;; wait(MOUSE)) {
		if (button1()) {
			if (ptinrect(p1=mouse.xy, box)) {
				cmds();
				return;
			}
			flag = 0;
			for (i = 1; i < lastset; i++)
				if (set[i].status && ptinrect(p1, set[i].Srect)) {
					flag = 1;
					break;
				}

			if (!flag)
				continue;

			/* erase old set rect */
			rectf(&display, set[i].Srect, f2);	
			/* erase old verts & edges */
			setmbrs(i);
			scaleset(i, op);
			/* draw new rect */
			rectf(&display, set[i].Srect, f2);	
			/* draw new verts & edges */
			setmbrs(i);
		}
		
		if (button3())
			return;
	}
								
}

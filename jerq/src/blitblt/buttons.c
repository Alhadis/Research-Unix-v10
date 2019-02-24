#include <jerq.h>

#define	UP	0
#define	DOWN	1

buttons(updown)
int updown;
{
	Rectangle r, s, kbdrect();
	do {
		checkshape(0);
		sleep(6);
	} while((button123()!=0) != updown || !ptinrect(mouse.xy,Drect));

	switch (button123()) {
		case 4:
			return 1;
		case 2:
			return 2;
		case 1:
			return 3;
	}
	return 0;
}

lexit3()	/* return true if button3 is clicked */
{
	extern Texture skull; Texture *prev; int lexit;
	prev=cursswitch(&skull);
	lexit=buttons(DOWN); buttons(UP);
	cursswitch(prev);
	return(lexit == 3);
}

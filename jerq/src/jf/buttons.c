#include <jerq.h>

#define UP	0
#define DOWN	1

buttons(updown)
int updown;
{
	do {
		wait(MOUSE);
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

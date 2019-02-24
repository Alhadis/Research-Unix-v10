#include <jerq.h>

#define UP	0
#define DOWN	1
#define TNULL	(Texture *)0

menuhit3(menup)
Menu *menup;
{
	extern Texture menu3; Texture *prev; int m;
	prev=cursswitch(&menu3);
	if (buttons(DOWN) == 3) { cursswitch(TNULL); m=menuhit(menup,3); }
	else { buttons(UP); m=-1; }
	cursswitch(prev);
	return m;
}

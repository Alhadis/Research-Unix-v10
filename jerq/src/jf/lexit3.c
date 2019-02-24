#include <jerq.h>

#define	UP	0
#define	DOWN	1

lexit3()	/* return true if button3 is clicked */
{
	extern Texture skull; Texture *prev; int lexit;
	prev=cursswitch(&skull);
	lexit=buttons(DOWN); buttons(UP);
	cursswitch(prev);
	return(lexit == 3);
}

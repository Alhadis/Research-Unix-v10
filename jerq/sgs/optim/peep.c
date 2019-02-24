/* @(#) peep.c: 1.2 3/3/84				*/
/* peep.c
**
**	3B20S peephole improvement driver
**
**
** This module contains the driver for the 3B peephole improver.
*/




#include "optim.h"

extern void w1opt();
extern void w2opt();
extern void w3opt();
extern int window();

extern int dflag;			/* non-0 to display live/dead data */
extern int hflag;			/* non-0 to disable peephole entirely */
void peep()
{
    void prld();			/* routine to print live/dead data */


    if (hflag == 0)
    {
#ifdef LIVEDEAD
	if (dflag != 0)			/* if enabled */
	    prld();			/* print the world's live/dead data */
#endif

	window(2, w2opt);		/* do 2-instruction first to merge
					** triadics
					*/
	window(3, w3opt);		/* ... to discard extra compares */
	window(1, w1opt);		/* do 1-instruction next to kill dead
					** code, split triadics again (!)
					*/

	window(2, w2opt);		/* now repeat to clean up stragglers */
	window(1, w1opt);
    }
    return;
}
/* Print live/dead data for all instruction nodes */

#ifdef LIVEDEAD


void
prld()
{
    register NODE * p;
    int prinst();

    for (ALLN(p))			/* for all instruction nodes... */
    {
	PUTCHAR(CC);			/* write comment char */

	PRINTF("(live: 0x%X)", p->nlive); /* print live/dead data */
	(void) prinst(p);			/* print instruction */
    }
    return;
}

#endif

#include "playdefs.h"

/* 'exit_game' updates score file if necessary and exits or starts
 *	a new game if the player desires.
 */
exit_game ()
{
	Point	poynt;

	poynt.x = (Drect.origin.x + Drect.corner.x) / 3;
	poynt.y = ((Drect.corner.y - Drect.origin.y) - 300) / 2 + Drect.origin.y;
	poynt.y = max (poynt.y, Drect.origin.y);
	string (&defont, "                                ",
		&display, poynt, F_STORE);
	string (&defont, "<space> to go again, 'q' to quit",
		&display, poynt, F_STORE);

	while (TRUE) {
		request (KBD|MOUSE);
		wait(KBD);
		switch (kbdchar()) {
			case 'q':
			case 'Q':
				doublecheck ();
			case ' ':
			case '\n':
				start_over = TRUE;
				return (FALSE);
		}
	}
}

/* 'doublecheck' assures that the player really wants to exit the
 *	layer.
 */
doublecheck ()
{
	int	c;
	
	texture16 (&display, Drect, (own()&MOUSE? &plain : &fancy), F_STORE);
	string (&defont, "Really exit Centipede? (y or n)",
		&display, div(add(Drect.origin, Drect.corner), 3), F_XOR);

	while (TRUE) {
		request (KBD|MOUSE);
		if (((c = kbdchar()) == 'q') || (c == 'Q') ||
		    (c == 'y') || (c == 'Y')) {
			cursswitch((Texture16*)0);
			exit (0);
		}
		else if (c != -1)
			return;
		wait (CPU);
	}
}

#include "playdefs.h"

/* 'disp_score' displays the value of 'score'.				*/


disp_score ()
{
	register	int	i;
	int	x;
	long	tempscr;
	
	tempscr = score;
	x = div2 (Drect.origin.x + Drect.corner.x) - 96;
	rectf (&display, Rect (	x, Drect.origin.y, x + 192,
		Drect.origin.y + 32), F_CLR);
	/* show score */
	for (i = 5; i >= 0; i--) {
		bitblt (&N[tempscr % 10], Rect(0, 0, 32, 32),
			&display, Pt(x + i * 32, Drect.origin.y), F_STORE);
		tempscr /= 10;

	}
	if (score >= newmanat) {
		newmanat += NEWMANSTEP;
		me_cnt++;
		show_extra (me_cnt-1);
		/* make sounds */
		beep;
		nap(10);
		beep;
		nap(10);
		beep;
		nap(10);
	}
}

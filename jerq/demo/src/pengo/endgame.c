#include "pengo.h"

Bitmap	disp_head = {
	(Word *) char_header,
	shorts(14),
	{ { 0, 0 }, { HEAD_X, HEAD_Y } }
};

newgame()
{
	int	ch;
	int	x;
	int	y;

	y = FALSE;
	rectf(&display, counter, F_CLR);
	addstr("Another", counter.origin, F_STORE);
	addstr("Game?", add(counter.origin, Pt(16, 16)), F_STORE);
	addstr("(Y/N)?_", add(counter.origin, Pt(0, 32)), F_STORE);
	for (;;)
	{
		for(x = 0; x < HEAD_COUNT; x++)
			if ((ch = kbdchar()) != -1)
				break;
			else
				sleep(2);
		if (ch == -1)
		{
			/* do static display */
			
			if (
				((board.corner.x - board.origin.x) > HEAD_X) &&
				((board.corner.y - board.origin.y) > HEAD_Y)
			)
			{
				if (y == FALSE)
				{
					rectf(&display, board, F_CLR);
					bitblt(&disp_head, disp_head.rect,
						&display,
						add(
						    div(
							sub(
							    sub(
								board.corner,
								board.origin
							    ),
							    Pt(HEAD_X, HEAD_Y)
							),
							2
						    ),
						    board.origin
						), F_STORE);
#ifdef	JERQ
#ifdef	MPX
					if (!VALSCREENCOLOR)
						rectf(&display, board, F_XOR);
#endif	MPX
#endif	JERQ
					y = TRUE;
				}
			}
			while(((ch = kbdchar()) == -1) && (!button123()))
				wait(CPU);
		}
		if (button123() && (ch == -1))
			return(TRUE);
		switch(ch)
		{
		case 'Y':
		case 'y':
			return(TRUE);
	
		case 'N':
		case 'n':
		case 'Q':
		case 'q':
			exit();
		}
	}
}
